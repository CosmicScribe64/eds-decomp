/*
 * Battle Phase stages 3-7: declare the attack and pay its costs, the attack-declared response window, reveal a
 * face-down target, and the damage calculation with the defender's optional answers and the battle scene
 * (wiki/functions/battle-phase1-c.md).
 *
 * BattlePhase_Run calls these through gBattleStageHandlers[gDuel.battleStage] with the attacking player; each
 * returns 1 when its stage is done. They are step machines on gDuel.battleStep (the step numbers 100-101, 200 and
 * 10-35 are the card-specific branches). The attacker is (player, gBattle.atkSlot), the target
 * (1 - player, gBattle.defSlot); defSlot 5 is a direct attack, for which stage 6 is skipped by the caller.
 */
#include "global.h"
#include "duel.h"                   /* struct DuelZone / DuelCard / DuelLoc, gDuel, gDuelZones, HasFlipEffect, CountMonsters,
                                     * CountActiveCardsOnField, CountFreeMonsterZones, FindFreeMonsterZone, ... */
#include "sound.h"                  /* PlaySE */
#include "card_data.h"              /* CARD_ID_MASK, gCardNames */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel.h"         /* enum BattleStage, ResponseEventKind, ZoneLinkKind, PromptKind */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* SE_ERROR */
#include "ai.h"                     /* AiPickTributeMonster */
#include "battle.h"                 /* gBattle (struct Battle), CanMonsterAttack, Battle_CheckReplay, ... */
#include "chain.h"                  /* Chain_AddPending, EventResponse_Request */
#include "duel_actions.h"           /* TributeMonster, MoveFieldCard, EquipCard, FlipFieldCard, ... */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl */
#include "duel_link.h"              /* gLinkState, DuelLink_SendMessage */
#include "duel_prompt.h"            /* DuelPrompt_Post */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* TriggerMysteriousPuppeteer */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatStr, HalveRoundDown, Random */

/* ROM data used only here. */
extern const u8 gStrSelectTributeToAttack[];    /* 0x080859E0: "Select a tribute in order to attack with this monster." */
extern const u8 gStrCoinTossCall[];             /* 0x08085A18: "Coin-toss Selection:\n  Heads\n  Tails" */
extern const char gStrAskZeroAttackerAtkFmt[];  /* 0x08085A48: "%s has been designated as the attack target. Do you wish
                                                 * to exercise this creature's effect and reduce the attacking
                                                 * monster's ATK to 0?" */
extern const char gStrAskSubstituteTargetFmt[]; /* 0x08085ADC: "... select your opponent's monster to substitute as
                                                 * the target?" */
/* Matching: card IDs read through their alias symbol (= &gCardNumberToId[CARD_1243], 0 in EDS). */
extern const u16 gCardNumberToId_1243;

/* The card tables through their integer addresses (different literals from gCardIdToNumber / gCardNumberToId). */
#define CARD_NUMBER_OF(id)  (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber[id] */
#define CARD_ID_OF(number)  (((const u16 *)0x08623DF4)[(number)])              /* gCardNumberToId[number] */
/* The name of a card ID as a string (gCardNames, one 64-byte record per card). */
#define CARD_NAME(id)       (((const char (*)[64])0x0822C720)[id])

/* The zone of (player, zone) of gDuelZones: player stride first, then the zone. */
#define ZONE_AT(player, zone) ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))

/* Byte offsets: a zone from gDuelZones (zone * zone size + player * stride). */
#define ZONE_OFFSET(player, zone) ((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer))

/*
 * Matching: gBattle through u8 containers (struct Battle keeps its first word in u16 containers): the flags
 * of the attack in byte 0, the defender slot in byte 1 and the flag byte of each side. Side flags: slot (bits 0-2),
 * destroyed, defensePos (BattleSide.defensePos; bit 0 of the battle-scene flags).
 */
struct BattleSideBytes {
    u8 slot:3;
    u8 destroyed:1;
    u8 defensePos:1;
    u8 unk0_5:3;
    u8 unk1;
    u16 cardId;                     /* +0x2 */
    u8 unk4[4];                     /* atk, def */
    u16 battleValue;                /* +0x8 */
    u16 damage;                     /* +0xA */
};
struct BattleBytes {
    u8 attacker:1;                  /* +0x0 */
    u8 direct:1;
    u8 flipEffectPending:1;
    u8 attackDeclared:1;
    u8 attackCostsPaid:1;
    u8 zeroAttackerAtk:1;
    u8 unk0_6:2;
    u8 unk1_0:1;                    /* +0x1: bit 8 of the first word */
    u8 defSlot:3;
    u8 unk1_4:4;
    u16 flipCardId;                 /* +0x2 */
    u8 unk4[4];
    struct BattleSideBytes side[2]; /* +0x8 */
};
#define BATTLE_BYTES (*(struct BattleBytes *)&gBattle)

/*
 * Matching: gDuel.battleStep and gDuel.battleStage are reached in several forms, the one the ROM uses in each
 * place: the member of gDuel (gDuel.battleStep), the gDuelZones symbol (= gDuel + 0x2C) as the base with the step as
 * a halfword view and the stage in a u32 or a u16 container (StepView, StageWordView, StageHalfViaZones), gDuel
 * through a cast with the stage in a u16 container (StageHalfView), and the alias symbol gDuelBattleStep (= the step).
 */
struct StepView {
    u16 unk0:1;
    u16 step:8;
    u16 unk9:7;
    u8 pad[0x20];
};
/* gDuel.battleStage through gDuel itself, with a halfword container (the value crosses a halfword boundary). */
struct StageHalfView {
    u8 prefix[0x1B14];
    u32 unk0:9;
    u16 stage:8;
    u32 unk17:15;
    u8 pad[8];
};
#define STAGE_E (*(struct StageHalfView *)&gDuel)
/* The same through the gDuelZones symbol (prefix 0x1AE8 = 0x1B14 - 0x2C), with a halfword container. */
struct StageHalfViaZones {
    u8 prefix[0x1AE8];
    u32 unk0:9;
    u16 stage:8;
    u32 unk17:15;
    u8 tail[4];
};
#define STAGE_HALF_VIA_ZONES (*(struct StageHalfViaZones *)gDuelZones)
struct StageWordView {
    u32 unk0:9;
    u32 stage:8;                    /* gDuel.battleStage, bits 9-16 of the word at +0x1B14 */
    u32 unk17:15;
};
#define STEP_VIA_ZONES   (*(struct StepView *)((u8 *)gDuelZones + 0x1AEA))        /* gDuel + 0x1B16 */
#define STAGE_VIA_ZONES  (*(struct StageWordView *)((u8 *)gDuelZones + 0x1AE8))   /* gDuel + 0x1B14 */

/* gDuel.battleStep in BattleStage_DeclareAttack. */
enum DeclareAttackStep {
    DECLARE_STEP_CHECK = 0,             /* check the attacker, save the snapshots, pay its own attack cost */
    DECLARE_STEP_PICK_TRIBUTE = 100,    /* Panther Warrior / Insect Queen, human: pick the tribute (cursor) */
    DECLARE_STEP_TRIBUTE_PAID = 101,    /* replay check, then on to the target or back to the attacker */
    DECLARE_STEP_COIN_TOSS = 200        /* Jirai Gumo: toss the coin the human called */
};

/* DuelZone.serial of (player, zone). The stages save it with the monster counts (gBattle.zoneSerialSnapshot,
 * monsterCountSnapshot) so that Battle_CheckReplay can tell that the attack changed during a response window. */
static inline u16 ZoneSerial(int player, int zone)
{
    int p = player & 1;
    int offset = zone * sizeof(struct DuelZone) + p * sizeof(struct DuelPlayer);
    u8 *base = (u8 *)gDuelZones;

    return ((struct DuelZone *)(offset + (u32)base))->serial;
}

/*
 * Stage 3: check that the attacker may still attack, save the replay snapshots and pay the attacker's own cost.
 *   step 0   already declared (a replay): return 1. If CanMonsterAttack(player, atkSlot, 1) fails: mark the
 *            attacker (DUEL_CMD_MARK_ATTACKED) and go back to BATTLE_STAGE_SELECT_ATTACKER. Otherwise save the
 *            snapshots, set attackDeclared, then by card: the Toons lose 500 LP, Dark Elf 1000, Panther Warrior
 *            and Insect Queen tribute a monster (the CPU picks it through AiPickTributeMonster, and without one
 *            the attack is given up: battleStage += 8; the human gets step 100), Jirai Gumo opens the coin call
 *            menu (step 200); anything else returns 1
 *   100      cursor pick of another own monster, which is tributed
 *   101      Battle_CheckReplay, then the target stage (or back to the attacker if it cannot attack any more)
 *   200      DUEL_CMD_TOSS_COIN with the call and Random() & 1; a wrong call costs half the LP
 * Returns 1 when the stage is done.
 */
int BattleStage_DeclareAttack(int player)
{
    u32 side = player & 1;
    struct StepView *step;
    u16 id;

    id = DUEL_CARD_ID(ZONE_AT(side, gBattle.atkSlot));
    step = &STEP_VIA_ZONES;
    switch (step->step) {
    case DECLARE_STEP_CHECK:
        if (BATTLE_BYTES.attackDeclared)
            return 1;
        if (CanMonsterAttack(player, gBattle.atkSlot, 1) == 0) {
            DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_MARK_ATTACKED : DUEL_CMD_MARK_ATTACKED,
                         gBattle.atkSlot, 1, 0);
            STAGE_VIA_ZONES.stage = BATTLE_STAGE_SELECT_ATTACKER;
            return 0;
        } else {
            int i;

            for (i = 0; i < 2; i++) {
                u32 slot;

                if (i == player)
                    slot = gBattle.atkSlot;
                else
                    slot = gBattle.defSlot;
                gBattle.monsterCountSnapshot[i] = CountMonsters(i);
                gBattle.zoneSerialSnapshot[i] = ZoneSerial(i, slot);
            }
            BATTLE_BYTES.attackDeclared = 1;
            switch (CARD_NUMBER_OF(id)) {
            case CARD_MANGA_RYU_RAN:
            case CARD_TOON_MERMAID:
            case CARD_TOON_SUMMONED_SKULL:
            case CARD_BLUE_EYES_TOON_DRAGON:
                /* The Toons pay 500 LP to attack. */
                DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_LOSE_LP : DUEL_CMD_LOSE_LP, 500, 1, 0);
                return 1;
            case CARD_DARK_ELF:
                DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_LOSE_LP : DUEL_CMD_LOSE_LP, 1000, 1, 0);
                return 1;
            case CARD_PANTHER_WARRIOR:
            case CARD_INSECT_QUEEN:
                /* They need a monster to tribute. */
                if (player != 0) {
                    int tribute = AiPickTributeMonster(gBattle.atkSlot, 0);

                    if (tribute >= 0) {
                        TributeMonster(player, tribute);
                        return 1;
                    }
                    MarkMonsterAttacked(player, gBattle.atkSlot);
                    /* DECLARE_ATTACK + 8 = BATTLE_STAGE_END_ATTACK: no attack. */
                    STAGE_E.stage = STAGE_E.stage + 8;
                    gDuel.battleStep = 0;
                    return 0;
                } else {
                    TextBoxOpen(TEXTBOX_POS(4, 2), TEXTBOX_SIZE(21, 7), TEXTBOX_FLAGS_DEFAULT, gStrSelectTributeToAttack);
                    gDuel.battleStep = DECLARE_STEP_PICK_TRIBUTE;
                    return 0;
                }
            case CARD_JIRAI_GUMO:
                TextBoxOpen(TEXTBOX_POS(6, 2), TEXTBOX_SIZE(19, 6), TEXTBOX_FLAGS_DEFAULT, gStrCoinTossCall);
                TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
                STEP_VIA_ZONES.step = DECLARE_STEP_COIN_TOSS;
                return 0;
            }
            return 1;
        }
    case DECLARE_STEP_PICK_TRIBUTE:
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
            if (gBattle.atkSlot != gDuelScreen.selIndex) {
                DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD : DUEL_CMD_POINT_AT_CARD, player,
                             (u8)gDuelScreen.selArea | (u8)gDuelScreen.selIndex << 8, 0);
                TributeMonster(player, gDuelScreen.selIndex);
                step->step++;
            } else {
                PlaySE(SE_ERROR);
            }
        }
        return 0;
    case DECLARE_STEP_TRIBUTE_PAID:
        if ((u16)Battle_CheckReplay(player) == 0)
            return 1;
        if (CanMonsterAttack(player, gBattle.atkSlot, 0) == 0)
            STAGE_VIA_ZONES.stage = BATTLE_STAGE_SELECT_ATTACKER;
        else
            STAGE_VIA_ZONES.stage = BATTLE_STAGE_SELECT_TARGET;
        return 0;
    case DECLARE_STEP_COIN_TOSS: {
        u16 toss = Random() & 1;

        DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_TOSS_COIN : DUEL_CMD_TOSS_COIN, gTextBox.result, toss, 0);
        DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_OPEN_DUEL_SCREEN : DUEL_CMD_OPEN_DUEL_SCREEN, 0, 0, 0);
        /* A wrong call loses half the LP. Matching: gDuelPlayers[side].lifePoints is spelled from the gDuelZones
         * literal (zones - 0x28 + side * 0xD64); the member form swaps r5 and r6 in this function. */
        if (toss != gTextBox.result)
            DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_LOSE_LP : DUEL_CMD_LOSE_LP,
                         HalveRoundDown(*(u16 *)((u8 *)gDuelZones - OFFSET_OF(struct DuelPlayer, zones)
                                                 + side * sizeof(struct DuelPlayer))), 1, 0);
        return 1;
    }
    default:
        return 1;
    }
}

/*
 * Stage 4: the attack costs of the field, paid once per attack (attackCostsPaid): each Gravekeeper's Servant on
 * the opponent's field sends the attacker's top deck card to the graveyard, each Toll on either field costs 500 LP.
 * Always returns 1.
 */
int BattleStage_PayAttackCosts(int player)
{
    if (!BATTLE_BYTES.attackCostsPaid) {
        u16 number;
        int count;

        BATTLE_BYTES.attackCostsPaid = 1;
        /* FAKEMATCH: the assignment inside the argument loads the card number after `1 - player`, as the ROM does;
         * a separate statement loads it before. */
        count = CountActiveCardsOnField(1 - player, number = CARD_GRAVEKEEPERS_SERVANT);
        if (count > 0) {
            ShowCardEffect(player, CARD_ID_OF(number));
            SendTopDeckCardsToGraveyard(player, count, 1);
        }
        number = CARD_TOLL;
        count = CountActiveCardsOnField(0, number);
        count += CountActiveCardsOnField(1, number);
        if (count > 0) {
            ShowCardEffect(player, CARD_ID_OF(number));
            LoseLifePoints(player, count * 500);
        }
    }
    return 1;
}

/* Chain_AddPending trigger word: player << 31 | zone << 16 | kind << 21 | event << 25 | card. `kindEvent20` is the
 * kind and event part shifted right by 20, (kind << 1 | event << 5): the ROM shifts it back at run time. */
static inline u32 TriggerWord(int player, int zone, u16 cardId, u32 kindEvent20)
{
    return ((u32)(player & 1) << 31) | (((zone & 0x1F) << 16) | (kindEvent20 << 20)) | cardId;
}

/* gDuel.battleStep in BattleStage_RespondToAttack. */
enum RespondStep {
    RESPOND_STEP_SNAPSHOT = 0,          /* save the monster counts and zone serials for the replay check */
    RESPOND_STEP_CHAIN_AUTO = 1,        /* key 1424 face up on the opponent's side is chained at once */
    RESPOND_STEP_CHECK = 2,             /* Battle_CheckReplay */
    RESPOND_STEP_REQUEST = 3,           /* open the response window for RESPONSE_ATTACK_DECLARED */
    RESPOND_STEP_FINAL_CHECK = 4        /* Battle_CheckReplay once more, then done */
};

/*
 * Stage 5: the attack-declaration response window.
 *   0   save the replay snapshots again
 *   1   key 1424 face up on the opponent's side is chained automatically (Chain_AddPending, RESPONSE_ATTACK_DECLARED)
 *       and the machine skips to step 4
 *   2   Battle_CheckReplay (1 sends the battle back to the attacker selection: leave the stage)
 *   3   EventResponse_Request(opponent, RESPONSE_ATTACK_DECLARED) with the attacker and the target locations
 *   4   Battle_CheckReplay, then return 1
 */
int BattleStage_RespondToAttack(int player)
{
    switch (gDuel.battleStep) {
    case RESPOND_STEP_SNAPSHOT: {
        int i;

        for (i = 0; i < 2; i++) {
            u32 slot;

            if (i == player)
                slot = gBattle.atkSlot;
            else
                slot = gBattle.defSlot;
            gBattle.monsterCountSnapshot[i] = CountMonsters(i);
            gBattle.zoneSerialSnapshot[i] = ZoneSerial(i, slot);
        }
        STEP_VIA_ZONES.step++;
        return 0;
    }
    case RESPOND_STEP_CHAIN_AUTO: {
        int opponent = 1 - player;
        u16 number = CARD_1424;

        if (CountActiveCardsOnField(opponent, number) != 0) {
            int zone = FindFaceUpCardOnField(opponent, number, -1);

            Chain_AddPending(TriggerWord(opponent, zone, CARD_ID_OF(number),
                                         CHAIN_KIND_SPELL_TRAP << 1 | RESPONSE_ATTACK_DECLARED << 5), 0);
            gDuel.battleStep = RESPOND_STEP_FINAL_CHECK;
        } else {
            gDuel.battleStep++;
        }
        return 0;
    }
    case RESPOND_STEP_CHECK:
        if ((u16)Battle_CheckReplay(player) != 0)
            return 0;
        gDuel.battleStep++;
        return 0;
    case RESPOND_STEP_REQUEST:
        EventResponse_Request(1 - player, RESPONSE_ATTACK_DECLARED,
                              (u8)player | gBattle.atkSlot << 8 | ((u8)(1 - player) | gBattle.defSlot << 8) << 16);
        gDuel.battleStep++;
        return 0;
    case RESPOND_STEP_FINAL_CHECK:
        if ((u16)Battle_CheckReplay(player) != 0)
            return 0;
        return 1;
    default:
        return 1;
    }
}

/* Matching: the alias symbol of gDuel.battleStep (0x0201ADF6 = gDuel + 0x1B16), used by BattleStage_RevealDefender. */
extern struct StepView gDuelBattleStep;

/* gDuel.battleStep in BattleStage_RevealDefender. */
enum RevealStep {
    REVEAL_STEP_FLIP = 0,               /* flip a face-down target face up */
    REVEAL_STEP_FLIP_EFFECT = 1,        /* note its flip effect; Blast Sphere / Kiseitai equip themselves */
    REVEAL_STEP_REQUEST = 2             /* open the response window for RESPONSE_DAMAGE_STEP */
};

/* The target's zone: (1 - player, gBattle.defSlot), formed from gDuelZones with the zone first. */
static inline struct DuelZone *DefenderZone(int player)
{
    int p = player & 1;
    int zone = gBattle.defSlot;
    int offset = ZONE_OFFSET(p, zone);
    u8 *base = (u8 *)gDuelZones;

    return (struct DuelZone *)(offset + (u32)base);
}
/* Byte +6 of the zone: isDefense (bit 0), isFaceUp (bit 1). */
#define DEFENDER_FLAGS(player)  (((u8 *)DefenderZone(player))[6])
#define DEFENDER_ID(player)     DUEL_CARD_ID(DefenderZone(player))

/*
 * Stage 6 (skipped by the caller for a direct attack): reveal the target.
 *   step 0   a face-down target is flipped face up (FlipFieldCard, ShowRevealedCard, TriggerMysteriousPuppeteer
 *            for its owner); a face-up one gets flipCardId = 0 and skips step 1 (the step is advanced twice)
 *   step 1   flipCardId = the target's card and flipEffectPending = HasFlipEffect(number, 1) (not while key 1530
 *            is on a field). A flipped Blast Sphere or Kiseitai in defense position with a free spell/trap
 *            zone instead moves there and equips itself to the attacker (MoveFieldCard, EquipCard), the attacker
 *            is marked, CalcBattle runs with the destroyed flags and the damage cleared, and the stage jumps +4
 *            to BATTLE_STAGE_DESTROY
 *   step 2   EventResponse_Request(opponent, RESPONSE_DAMAGE_STEP) with the attacker and the target locations
 * Returns 1 when done (also at once for a direct attack).
 */
int BattleStage_RevealDefender(int player)
{
    /* FAKEMATCH: keep the duel base in ip through the opening state dispatch. */
    register u8 *e asm("r12");

    if (BATTLE_BYTES.direct)
        return 1;
    e = (u8 *)&gDuel;
    switch (gDuelBattleStep.step) {
    case REVEAL_STEP_FLIP: {
        int opponent = 1 - player;

        if (!(DEFENDER_FLAGS(opponent) & 2)) {
            /* FAKEMATCH: retain the initialized base until the defender slot is spilled. */
            asm("" : : "r"(e));
            FlipFieldCard(opponent, gBattle.defSlot, 0);
            ShowRevealedCard(player, DEFENDER_ID(opponent));
            TriggerMysteriousPuppeteer(opponent);
        } else {
            BATTLE_BYTES.flipCardId = 0;
            gDuelBattleStep.step++;
        }
        gDuel.battleStep++;
        return 0;
    }
    case REVEAL_STEP_FLIP_EFFECT: {
        u16 id;
        u16 number;
        int p = (1 - player) & 1;
        int offset = ZONE_OFFSET(p, gBattle.defSlot);
        u8 *zones = e + OFFSET_OF(struct DuelState, players[0].zones);

        id = DUEL_CARD_ID((struct DuelZone *)(offset + (u32)zones));
        BATTLE_BYTES.flipCardId = id;
        BATTLE_BYTES.flipEffectPending = HasFlipEffect(CARD_NUMBER_OF(id), 1);
        if (CountActiveCardsOnField(0, CARD_1530) != 0 || CountActiveCardsOnField(1, CARD_1530) != 0)
            BATTLE_BYTES.flipEffectPending = 0;
        number = CARD_NUMBER_OF(BATTLE_BYTES.flipCardId);
        if (number == CARD_BLAST_SPHERE || number == CARD_KISEITAI) {
            int opponent = 1 - player;

            /* Defense position, key 1530 not on a field and somewhere to put the card. */
            if ((DEFENDER_FLAGS(opponent) & 1)
                && CountActiveCardsOnField(0, CARD_1530) == 0
                && CountActiveCardsOnField(1, CARD_1530) == 0
                && CanPlaceSpellTrapCard(opponent, DEFENDER_ID(opponent)) != 0) {
                int zone = FindFreeSpellTrapZone(opponent);
                u16 source, destination;

                ShowActivatedCard(player, BATTLE_BYTES.flipCardId);
                source = (u8)(1 - player) | gBattle.defSlot << 8;
                destination = (u8)(1 - player) | (u8)zone << 8;
                MoveFieldCard(opponent, source, destination);
                EquipCard(opponent, destination, (u8)player | gBattle.atkSlot << 8);
                DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_MARK_ATTACKED : DUEL_CMD_MARK_ATTACKED,
                             gBattle.atkSlot, 1, 0);
                CalcBattle(player, 0);
                BATTLE_BYTES.side[0].destroyed = 0;
                BATTLE_BYTES.side[1].destroyed = 0;
                BATTLE_BYTES.side[0].damage = 0;
                BATTLE_BYTES.side[1].damage = 0;
                BATTLE_BYTES.flipEffectPending = 0;
                /* REVEAL_DEFENDER + 4 = BATTLE_STAGE_DESTROY. */
                STAGE_HALF_VIA_ZONES.stage += 4;
                STEP_VIA_ZONES.step = 0;
                return 0;
            }
            BATTLE_BYTES.flipEffectPending = 0;
        }
        gDuel.battleStep++;
        return 0;
    }
    case REVEAL_STEP_REQUEST:
        EventResponse_Request(1 - player, RESPONSE_DAMAGE_STEP,
                              (u8)player | gBattle.atkSlot << 8 | ((u8)(1 - player) | gBattle.defSlot << 8) << 16);
        gDuelBattleStep.step++;
        return 0;
    default:
        return 1;
    }
}

/* gDuel.battleStep, battleStage and the prompt result through a base pointer e = (u8 *)&gDuel: the ROM keeps the
 * base of gDuel in a register in BattleStage_DamageCalc. */
struct StepAtBase {
    u8 prefix[0x1B16];
    u16 unk0:1;
    u16 step:8;
    u16 unk9:7;
};
#define STEP_AT(e)          (((struct StepAtBase *)(e))->step)
#define STAGE_AT(e)         (((struct StageHalfView *)(e))->stage)
#define PROMPT_PICK_AT(e)   (*((u8 *)(e) + 0x1B64))     /* the low byte of gDuel.promptResult: the zone picked */


/* The zone of the attacker (player, atkSlot), formed like DefenderZone. */
static inline struct DuelZone *AttackerZone(int player)
{
    int p = player & 1;
    int zone = gBattle.atkSlot;
    int offset = ZONE_OFFSET(p, zone);
    u8 *base = (u8 *)gDuelZones;

    return (struct DuelZone *)(offset + (u32)base);
}
/* Byte +7 of a zone: effectUnused is bit 5 (0x20). */
#define ATTACKER_FLAGS7(player)     (((u8 *)AttackerZone(player))[7])
#define DEFENDER_FLAGS7(player)     (((u8 *)DefenderZone(player))[7])
#define DefenderFlags(player)       DEFENDER_FLAGS(player)
/* The zone of (player, zone) with the zone first, through the gDuelZones symbol. */
#define ZONE_VIA_ZONES(player, zone) ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))

/* DUEL_LOC with the operands in the order the ROM evaluates them: the player (as a byte) first. */
#define LOC(player, zone) (((u8)(player)) | (zone) << 8)

/* A location with both bytes narrowed first (the u8 parameters), as the ROM builds the Mirror Wall's link target. */
static inline u16 PackLoc(u8 player, u8 zone)
{
    return player | zone << 8;
}

/* gDuel.battleArg0 (+0x1B17 bit 1 .. +0x1B18 bit 0) assembled and split by hand. */
struct BattleArgHigh {
    u8 prefix[0x1B18];
    u8 value:1;
};
static inline u16 ArgDestination(u8 player, u8 low, u8 high)
{
    int lower = low >> 1;
    int upper = (high & 1) << 7;

    return player | (upper | lower) << 8;
}

/* gDuel.battleStep in BattleStage_DamageCalc. */
enum DamageCalcStep {
    DAMAGE_STEP_MARK_ATTACKER = 0,      /* mark the attacker, link the Mirror Walls to it */
    DAMAGE_STEP_CALC = 1,               /* CalcBattle, then the defender's answers */
    DAMAGE_STEP_START_SCENE = 2,        /* DUEL_CMD_START_BATTLE_SCENE */
    DAMAGE_STEP_PLAY_SCENE = 3,         /* DUEL_CMD_PLAY_BATTLE_SCENE */
    DAMAGE_STEP_CLEAR_ZONES = 4,        /* clear the zones of the destroyed monsters */
    DAMAGE_STEP_ASK_ZERO_ATK = 10,      /* Sanga / Kazejin / Suijin: "reduce the attacker's ATK to 0?" */
    DAMAGE_STEP_WAIT_ZERO_ATK = 11,
    DAMAGE_STEP_APPLY_ZERO_ATK = 12,
    DAMAGE_STEP_REPLACE_OWN = 20,       /* key 1522: pick another own monster as the target */
    DAMAGE_STEP_REPLACED_OWN = 21,
    DAMAGE_STEP_ASK_SUBSTITUTE = 30,    /* key 1243: "take an opponent's monster as the target?" */
    DAMAGE_STEP_WAIT_SUBSTITUTE = 31,
    DAMAGE_STEP_PICK_SUBSTITUTE = 32,
    DAMAGE_STEP_MOVE_SUBSTITUTE = 33,
    DAMAGE_STEP_FLIP_SUBSTITUTE = 34,
    DAMAGE_STEP_SUBSTITUTE_DONE = 35
};

/* The monster's number in a zone, as a u32 (the ROM does not narrow it). */
static inline u32 CardNumberWide(u32 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}

/* Matching: byte view of gLinkState; byte +0x450 holds cardPromptPending (bit 0) and cardPromptAnswered
 * (bit 1), duel_link.h's u32 container would load a word. The answer is the halfword at +0x45A. */
extern u8 gLinkStateBytes[] asm("gLinkState");
struct LinkCardPromptByte {
    u8 unk0[0x450];
    u8 pending:1;
    u8 answered:1;
    u8 unk450_2:6;
};
#define LINK_CARD_PROMPT ((struct LinkCardPromptByte *)gLinkStateBytes)

/*
 * Stage 7: the damage calculation and the defender's optional answers, then the battle scene.
 *   0       mark the attacker (DUEL_CMD_MARK_ATTACKED; key 1336 with its extra attack unused and key 1344 push
 *           DUEL_CMD_SET_EFFECT_UNUSED instead) and link every face-up, enabled Mirror Wall of the opponent to it
 *   1       CalcBattle; a face-up target may answer: Sanga of the Thunder / Kazejin / Suijin with effectUnused go to
 *           step 10, key 1522 to step 20, key 1243 (when it is the card that was flipped) to step 30. Falls into 2
 *   2       DUEL_CMD_START_BATTLE_SCENE with both card IDs
 *   3       DUEL_CMD_PLAY_BATTLE_SCENE with both battle values and the packed per-side flags
 *   4       DUEL_CMD_CLEAR_ZONE_CARD for each destroyed side, DUEL_CMD_OPEN_DUEL_SCREEN; return 1
 *   10-12   ask the defender "reduce the attacking monster's ATK to 0?" (the CPU says yes, the link partner is
 *           asked with LINKMSG_CARD_PROMPT); yes: the effect is used up (DUEL_CMD_SET_EFFECT_UNUSED), the attacker's
 *           ATK counts as 0 (DUEL_CMD_ZERO_ATTACKER_ATK) and CalcBattle runs again (step 2 follows)
 *   20-21   key 1522: DuelPrompt_Post(PROMPT_SELECT_OWN_REPLACEMENT_TARGET), the picked zone becomes the target
 *           (DUEL_CMD_SET_ATTACK_TARGET) and the stage goes back to BATTLE_STAGE_REVEAL_DEFENDER
 *   30-35   key 1243: ask, DuelPrompt_Post(PROMPT_SELECT_OPPONENT_REPLACEMENT_TARGET), move the picked monster of
 *           the attacker into a free zone of the defender, make it the target, flip it, CalcBattle
 */
int BattleStage_DamageCalc(int player)
{
    char askText[256];
    char substituteText[256];
    u8 *e;
    u32 state = gDuel.battleStep;

    e = (u8 *)&gDuel;
    switch (state) {
    case DAMAGE_STEP_MARK_ATTACKER: {
        int handled = 0;
        int i;
        int opponent;

        switch (CardNumberWide(DUEL_CARD_ID(AttackerZone(player)))) {
        case CARD_1336:
            /* Its extra attack: while it is unused, attacking uses it up instead of marking the monster. */
            if (ATTACKER_FLAGS7(player) & 0x20) {
                DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_SET_EFFECT_UNUSED : DUEL_CMD_SET_EFFECT_UNUSED,
                             gBattle.atkSlot, 0, 0);
                handled = 1;
            }
            break;
        case CARD_1344:
            DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_SET_EFFECT_UNUSED : DUEL_CMD_SET_EFFECT_UNUSED,
                         gBattle.atkSlot, 0, 0);
            break;
        }
        if (!handled)
            DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_MARK_ATTACKED : DUEL_CMD_MARK_ATTACKED,
                         gBattle.atkSlot, 1, 0);
        i = ZONE_SPELL_0;
        opponent = 1 - player;
        /* Every face-up, enabled Mirror Wall of the opponent gets a link to the attacker. */
        for (; i <= ZONE_SPELL_4; i++) {
            u8 *zone = (u8 *)ZONE_VIA_ZONES(opponent & 1, i);
            u16 id = DUEL_CARD_ID(zone);

            if (id != 0 && (zone[6] & 2) && !(zone[0x91] & 8) && CardNumberWide(id) == CARD_MIRROR_WALL)
                QueueAddZoneLink(opponent, PackLoc(opponent, i), LOC(player, gBattle.atkSlot), ZONE_LINK_CONTINUOUS);
        }
        gDuel.battleStep++;
        return 0;
    }
    case DAMAGE_STEP_CALC: {
        int opponent;
        u16 number;

        CalcBattle(player, 0);
        if (!BATTLE_BYTES.direct) {
            opponent = 1 - player;
            if (DefenderFlags(opponent) & 2) {
                {
                    u16 id = DUEL_CARD_ID(DefenderZone(opponent));

                    number = CardNumberWide(id);
                }
                switch (number) {
                case CARD_SANGA_OF_THE_THUNDER:
                case CARD_KAZEJIN:
                case CARD_SUIJIN:
                    if (DEFENDER_FLAGS7(opponent) & 0x20) {
                        STEP_VIA_ZONES.step = DAMAGE_STEP_ASK_ZERO_ATK;
                        return 0;
                    }
                    break;
                case CARD_1243:
                    if (CARD_NUMBER_OF(BATTLE_BYTES.flipCardId) == number
                        && CountFreeMonsterZones(opponent) > 0 && CountMonsters(player) > 1) {
                        STEP_VIA_ZONES.step = DAMAGE_STEP_ASK_SUBSTITUTE;
                        return 0;
                    }
                    break;
                case CARD_1522:
                    if (CountMonsters(opponent) > 1) {
                        STEP_VIA_ZONES.step = DAMAGE_STEP_REPLACE_OWN;
                        return 0;
                    }
                    break;
                }
            }
        }
        gDuel.battleStep++;
        /* The normal path immediately performs step 2 as well. */
    }
    case DAMAGE_STEP_START_SCENE:
        DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_START_BATTLE_SCENE : DUEL_CMD_START_BATTLE_SCENE,
                     BATTLE_BYTES.side[0].cardId, BATTLE_BYTES.side[1].cardId, 0);
        gDuel.battleStep++;
        return 0;
    case DAMAGE_STEP_PLAY_SCENE: {
        u16 message = DUEL_CMD_PLAY_BATTLE_SCENE;

        if (player)
            message = DUEL_CMD_PLAYER | DUEL_CMD_PLAY_BATTLE_SCENE;
        { u16 attack = BATTLE_BYTES.side[0].battleValue;
        u16 defense = BATTLE_BYTES.side[1].battleValue;
        /* BattleSideFlags of each side: defensePos | destroyed << 1 (| BATTLE_SIDE_DAMAGE when it takes damage). */
        int defender = BATTLE_BYTES.side[1].defensePos | BATTLE_BYTES.side[1].destroyed << 1;
        int attacker = BATTLE_BYTES.side[0].defensePos | BATTLE_BYTES.side[0].destroyed << 1;
        int combined;
        u16 packed;

        if (BATTLE_BYTES.side[0].damage != 0)
            combined = BATTLE_SIDE_DAMAGE | attacker;
        else
            combined = attacker;
        if (BATTLE_BYTES.side[1].damage != 0)
            packed = combined | ((defender | BATTLE_SIDE_DAMAGE) << 8);
        else
            packed = combined | (defender << 8);
        DuelCmd_Push(message, attack, defense, packed);
        gDuel.battleStep++;
        return 0;
    }
    }
    case DAMAGE_STEP_CLEAR_ZONES: {
        int i = 0;
        u8 *side = (u8 *)&gBattle;

        for (; i <= 1; side += sizeof(struct BattleSide), i++) {
            u8 flags = side[8];

            if ((s32)((u32)flags << 28) < 0)       /* destroyed */
                DuelCmd_Push(i ? DUEL_CMD_PLAYER | DUEL_CMD_CLEAR_ZONE_CARD : DUEL_CMD_CLEAR_ZONE_CARD,
                             ((struct BattleSideBytes *)(side + 8))->slot, 0, 0);
        }
        DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_OPEN_DUEL_SCREEN : DUEL_CMD_OPEN_DUEL_SCREEN, 0, 0, 0);
        return 1;
    }
    case DAMAGE_STEP_ASK_ZERO_ATK:
        if (player != 0) {
            /* The defender is the human. */
            FormatStr(askText, gStrAskZeroAttackerAtkFmt, CARD_NAME(DUEL_CARD_ID(DefenderZone(1 - player))));
            TextBoxOpen(TEXTBOX_POS(4, 2), TEXTBOX_SIZE(22, 11), TEXTBOX_FLAGS_DEFAULT, (u8 *)askText);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            STEP_VIA_ZONES.step++;
        } else if (!(gDuelCtrl.isLinkDuel)) {
            /* The CPU always uses the effect. */
            gTextBox.result = 1;
            gDuel.battleStep++;
        } else {
            DuelLink_SendMessage(LINKMSG_CARD_PROMPT, DUEL_CARD_ID(DefenderZone(1)), 0, 0);
            LINK_CARD_PROMPT->answered = 0;
        }
        gDuel.battleStep++;
        return 0;
    case DAMAGE_STEP_WAIT_ZERO_ATK:
        if ((s32)((u32)gLinkStateBytes[0x450] << 30) < 0) {     /* cardPromptAnswered */
            gTextBox.result = *(u16 *)(gLinkStateBytes + 0x45A);   /* cardPromptAnswer */
            STEP_AT(e)++;
        }
        return 0;
    case DAMAGE_STEP_APPLY_ZERO_ATK:
        if (gTextBox.result != 0) {
            ShowActivatedCard(1 - player, DUEL_CARD_ID(DefenderZone(1 - player)));
            DuelCmd_Push(player != 1 ? DUEL_CMD_PLAYER | DUEL_CMD_SET_EFFECT_UNUSED : DUEL_CMD_SET_EFFECT_UNUSED,
                         gBattle.defSlot, 0, 0);
            DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_ZERO_ATTACKER_ATK : DUEL_CMD_ZERO_ATTACKER_ATK, 0, 0, 0);
            CalcBattle(player, 1);
        }
        gDuel.battleStep = DAMAGE_STEP_START_SCENE;
        return 0;
    case DAMAGE_STEP_REPLACE_OWN:
        DuelPrompt_Post(1 - player, PROMPT_SELECT_OWN_REPLACEMENT_TARGET, gBattle.defSlot, 0);
        gDuel.battleStep++;
        return 0;
    case DAMAGE_STEP_REPLACED_OWN:
        DuelCmd_Push(player != 1 ? DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD : DUEL_CMD_POINT_AT_CARD,
                     (u16)(1 - player), PROMPT_PICK_AT(e) << 8, 0);
        DuelCmd_Push(player != 1 ? DUEL_CMD_PLAYER | DUEL_CMD_SET_ATTACK_TARGET : DUEL_CMD_SET_ATTACK_TARGET,
                     (u8)(1 - player) | PROMPT_PICK_AT(e) << 8, 0, 0);
        STAGE_AT(e)--;
        STEP_AT(e) = 0;
        return 0;
    case DAMAGE_STEP_ASK_SUBSTITUTE:
        if (player != 0) {
            FormatStr(substituteText, gStrAskSubstituteTargetFmt, CARD_NAME(DUEL_CARD_ID(DefenderZone(1 - player))));
            TextBoxOpen(TEXTBOX_POS(6, 2), TEXTBOX_SIZE(19, 7), TEXTBOX_FLAGS_DEFAULT, (u8 *)substituteText);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            STEP_VIA_ZONES.step++;
        } else if (!(gDuelCtrl.isLinkDuel)) {
            gTextBox.result = 1;
            gDuel.battleStep++;
        } else {
            DuelLink_SendMessage(LINKMSG_CARD_PROMPT, DUEL_CARD_ID(DefenderZone(1)), 0, 0);
            LINK_CARD_PROMPT->answered = 0;
        }
        gDuel.battleStep++;
        return 0;
    case DAMAGE_STEP_WAIT_SUBSTITUTE:
        if ((s32)((u32)gLinkStateBytes[0x450] << 30) < 0) {
            gTextBox.result = *(u16 *)(gLinkStateBytes + 0x45A);
            STEP_AT(e)++;
        }
        return 0;
    case DAMAGE_STEP_PICK_SUBSTITUTE:
        if (gTextBox.result == 0) {
            STEP_AT(e) = DAMAGE_STEP_START_SCENE;
            return 0;
        }
        DuelPrompt_Post(1 - player, PROMPT_SELECT_OPPONENT_REPLACEMENT_TARGET, gBattle.atkSlot, 0);
        gDuel.battleStep++;
        return 0;
    case DAMAGE_STEP_MOVE_SUBSTITUTE: {
        u16 zone;
        u32 freshOne;
        u32 low;
        u16 lowerZone;
        /* FAKEMATCH: reuse r1 for the mask and loaded byte, leaving the zone in r2. */
        register u32 mask asm("r1");
        u8 *lowPtr;

        DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD : DUEL_CMD_POINT_AT_CARD, (u16)player,
                     PROMPT_PICK_AT(e) << 8, 0);
        zone = FindFreeMonsterZone(1 - player);
        /* FAKEMATCH: preserve the initialized halfword before splitting its bits. */
        asm("" : : "r"(zone));
        /* FAKEMATCH: stage the low-byte merge to preserve the original register
         * lifetimes; cache that byte for the packed destination. */
        mask = 127;
        lowerZone = zone & mask;
        lowPtr = e + 0x1B17;
        lowerZone <<= 1;
        low = 1;
        mask = *lowPtr;
        low &= mask;
        low |= lowerZone;
        *lowPtr = low;
        /* FAKEMATCH: the tied input initializes freshOne to 1; consuming the
         * high bits here schedules that constant before the high-byte pointer. */
        asm("" : "=r"(freshOne) : "0"(1), "r"(zone >> 7));
        ((struct BattleArgHigh *)e)->value = zone >> 7;
        MoveFieldCard(1 - player, (u8)player | PROMPT_PICK_AT(e) << 8,
                      ArgDestination(freshOne - player, low, e[0x1B18]));
        {
            struct BattleBytes *battle = (struct BattleBytes *)&gBattle;
            int low = e[0x1B17] >> 1;

            battle->defSlot = ((e[0x1B18] & 1) << 7) | low;
        }
        STEP_AT(e)++;
        return 0;
    }
    case DAMAGE_STEP_FLIP_SUBSTITUTE: {
        int opponent = 1 - player;

        if (!(DefenderFlags(opponent) & 2)) {
            FlipFieldCard(opponent, gBattle.defSlot, 0);
            ShowRevealedCard(player, DUEL_CARD_ID(DefenderZone(opponent)));
            TriggerMysteriousPuppeteer(opponent);
        }
        STEP_VIA_ZONES.step++;
        return 0;
    }
    case DAMAGE_STEP_SUBSTITUTE_DONE: {
        struct BattleBytes *battle = (struct BattleBytes *)&gBattle;
        u16 id = DUEL_CARD_ID(DefenderZone(1 - player));

        battle->flipCardId = id;
        battle->flipEffectPending = HasFlipEffect(CARD_NUMBER_OF(id), 1);
        if (CountActiveCardsOnField(0, CARD_1530) != 0 || CountActiveCardsOnField(1, CARD_1530) != 0)
            battle->flipEffectPending = 0;
        QueueAddZoneLink(1 - player, gCardNumberToId_1243, LOC(1 - player, gBattle.defSlot), ZONE_LINK_CARD_EFFECT);
        CalcBattle(player, 0);
        gDuel.battleStep = DAMAGE_STEP_START_SCENE;
        return 0;
    }
    default:
        return 1;
    }
}
