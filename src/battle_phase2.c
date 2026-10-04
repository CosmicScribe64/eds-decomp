/*
 * Battle Phase stages 8-10: the battle damage, the flip effect of a defender flipped by the attack, and the
 * destruction of the battled monsters (wiki/functions/battle-phase2-c.md).
 *
 * BattlePhase_Run calls these through gBattleStageHandlers[gDuel.battleStage] with the attacking player. Each
 * returns 1 when its stage is done. BattleStage_InflictDamage and BattleStage_DestroyMonsters are step machines
 * on gDuel.battleStep; the numbers of the battle itself (cards, slots, damage and the "destroyed" flags of both
 * sides) were filled in by CalcBattle (gBattle, include/battle.h).
 *
 * Sides: gBattle.side[player] is the side of the attacking player's monster and side[1 - player] the target's.
 * "side[p].damage" is the life-point damage that side's PLAYER takes, so the "inflicts battle damage" effects
 * of a card are read from the opposite side.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, gCardNames */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/duel.h"         /* enum ChainEntryKind, ResponseEventKind, ZoneLinkKind */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h (build/readability/hcheck/duel_core/staged/duel.h)
 * that this unit and the headers below use, with its names, types and bitfield containers (unused bytes are
 * padding), and defines duel.h's include guard so that the headers below do not pull in the legacy one.
 * After H0, replace the block (BEGIN to END) with #include "legacy/duel.h". */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:11;
    u32 pendingEquip:1;             /* bit 24: graveyard card waiting to be equipped at end of turn */
    u32 equipZone:3;                /* bits 25-27: monster zone that card goes to */
    u32 pendingOpponentSummon:1;    /* bit 28: graveyard card the opponent may Special Summon */
    u32 unk29:3;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 unk6_2:6;
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u8 unk0[8];
    u8 noBattleDamage:1;            /* +0x008 bit 0 */
    u8 battleProtected:1;           /* +0x008 bit 1 */
    u8 unk8_2:1;
    u8 insectQueenWonBattle:1;      /* +0x008 bit 3: set by BattleStage_DestroyMonsters */
    u8 unk8_4:4;
    u8 unk9[0x28 - 0x9];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    u8 unk684[0xD64 - 0x684];
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B14 - 0x1ACC];
    u16 unk1B14_0:1;                /* +0x1B14 */
    u16 interruptActive:1;
    u16 unk1B14_2:7;
    u32 battleStage:8;              /* +0x1B15 bit 1 .. +0x1B16 bit 0: enum BattleStage */
    u16 battleStep:8;               /* +0x1B16 bits 1-8: step inside the stage */
    u16 battleArg0:8;
    u16 battleArg1:8;
    u16 unk1B19_1:7;
    u8 unk1B1A[0x1B78 - 0x1B1A];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

u16 FindAbsorbedMonsterLink(int player, int zone);
int CountHandCardsByNumber(int player, u16 cardNo);
u32 GetZoneCardType(s32 player, s32 slot);
/* ---- END duel.h stand-in ---- */

#include "battle.h"                 /* gBattle (struct Battle), BattleStage_* defined here */
#include "chain.h"                  /* Chain_AddPending, EventResponse_Request */
#include "duel_actions.h"           /* InflictBattleDamage, ShowCardEffect, DrawCards, QueueAddZoneLink, ... */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl */
#include "duel_link.h"              /* gLinkState, DuelLink_SendMessage */
#include "duel_prompt.h"            /* DuelPrompt_PostRandomDiscard */
#include "effect.h"                 /* CanActivateEffectOfCard */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatStr */

/* ROM data used only here. */
extern const char gStrAskKuribohFmt[];      /* 0x08085B7C: "You've suffered damage from battle. Do you wish to
                                             * reduce the damage to 0 by discarding %s?" */
/* Matching: Kuriboh's card ID read through its own alias symbol (= &gCardNumberToId[CARD_KURIBOH]); indexing
 * gCardNumberToId instead changes the code. */
extern const u16 gUnk_08623E66[];

/* Matching: byte view of gLinkState; byte +0x450 holds cardPromptPending (bit 0) and cardPromptAnswered
 * (bit 1), duel_link.h's u32 bitfield container would load a word. The answer is the halfword at +0x45A. */
extern u8 gLinkStateBytes[] asm("gLinkState");
struct LinkCardPromptByte {
    u8 unk0[0x450];
    u8 pending:1;
    u8 answered:1;
    u8 unk450_2:6;
};
#define LINK_CARD_PROMPT ((struct LinkCardPromptByte *)gLinkStateBytes)

/* Matching: gBattle as rows of 12 bytes from its start; row i's byte 8 is gBattle.side[i]'s flag byte
 * (slot:3, destroyed:1, defensePos:1, destroyedCopy:1, effectDestroy:1). battle.h's BattleSide mixes u16 and u8
 * containers. */
struct BattleSideRow {
    u8 unk0[8];
    u8 flags;
};
#define SIDE_ROW(i) ((struct BattleSideRow *)((u8 *)&gBattle + (i) * sizeof(struct BattleSide)))

/* Matching: the card table is read through its integer address (a literal-pool entry of its own). */
#define CARD_NUMBER_OF(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/* Matching: FindAbsorbedMonsterLink is defined to return u16; this unit compares the result with 0xFFFF as an
 * int, with no narrowing of r0. */
extern int FindAbsorbedMonsterLinkInt(int player, int zone) asm("FindAbsorbedMonsterLink");

/* Matching: IsBattleEffectBlocked is defined with no parameter and returns int (battle.h); this unit calls
 * it with the player and tests a u16 result. */
extern u16 IsBattleEffectBlockedFor(int player) asm("IsBattleEffectBlocked");

/* Matching: gDuelPlayers indexed through a struct that holds the array, so its base is loaded before the
 * index is computed (ROM order; a plain array index loads the base last). */
struct DuelPlayerPair {
    struct DuelPlayer players[2];
};
#define PLAYER_VIA_BASE(i) (((struct DuelPlayerPair *)gDuelPlayers)->players[(i) & 1])

/* Byte offset of a zone from gDuelZones: zone * zone size + side * player stride. */
#define ZONE_OFFSET(side, zone) ((zone) * sizeof(struct DuelZone) + (side) * sizeof(struct DuelPlayer))

/* DUEL_LOC with the operands in the order the ROM evaluates them: the player (as a byte) first. */
#define LOC(player, zone) (((u8)(player)) | (zone) << 8)

/* QueueAddZoneLink kind of key 1523's self link: ZONE_LINK_ATK_DOWN_200 with a count of 1 in the high byte. */
#define LINK_ATK_DOWN_200_ONCE (ZONE_LINK_ATK_DOWN_200 | 1 << 8)

/* Text box rectangles: TextBoxOpen takes x | y << 8 and width | height << 8, in cells. */
#define TEXTBOX_XY(x, y) ((x) | (y) << 8)

/* Matching: the field zones through gDuel + 0x2C, so that CSE reuses the register that holds &gDuel from the
 * step test (the ROM adds #0 and #0x2C to it). */
#define FIELD_ZONE(offset) ((struct DuelZone *)((offset) + (int)((u8 *)&gDuel + 0x2C)))

/* Trigger word of Chain_AddPending: card | zone << 16 | kind << 21 | event << 25 | player << 31. These are the
 * kind and event parts of the words this unit builds: a monster of kind CHAIN_KIND_MONSTER that chains when it
 * deals battle damage (event 14 for the damage the target's card dealt to the attacker's player, 13 for the
 * attacker's card), or when it battles (event 18, the same event the flip effect of stage 9 uses). */
#define TRIGGER_BATTLE_DEFLECTED_DAMAGE (CHAIN_KIND_MONSTER << 21 | RESPONSE_BATTLE_DEFLECTED_DAMAGE << 25)
#define TRIGGER_BATTLE_DAMAGE           (CHAIN_KIND_MONSTER << 21 | RESPONSE_BATTLE_DAMAGE << 25)
#define TRIGGER_BATTLE_FLIP_EFFECT      (CHAIN_KIND_MONSTER << 21 | RESPONSE_BATTLE_FLIP_EFFECT << 25)
#define TRIGGER_MONSTER_BATTLED         TRIGGER_BATTLE_FLIP_EFFECT      /* a monster's own battle effect */

/*
 * Stage 8: apply the battle damage of both sides, then the effects of the monsters that inflicted it.
 *   step 0   damage to the attacker's player, effects of the target's card
 *   step 1   Kuriboh check for the defending player: with Kuriboh in hand go to the prompt (step 10)
 *   step 2   damage to the defender's player, effects of the attacker's card (steps 0 and 2 mirror each other)
 *   10-12    "discard Kuriboh to take no damage?" (the CPU says yes; the link partner answers over the link);
 *            on yes the defender's damage is zeroed and the machine goes back to step 2
 *   default  clear noBattleDamage of both players; Big Shield Gardna in defense position turns to attack
 *            position; sides marked effectDestroy that survived get a Sword of Dragon's Soul link
 * Returns 1 when the stage is done (default step only).
 *
 * Matching: every `return 0` goes to one label after step 10 (FAKEMATCH below); `1 - player` stays inline in
 * place of a local, which lets `player` take r7.
 */
int BattleStage_InflictDamage(int player)
{
    char question[256];

    switch (gDuel.battleStep) {
    case 0:
        if (gBattle.side[player].damage != 0 && !gDuelPlayers[player & 1].noBattleDamage
            && !gDuelPlayers[player & 1].battleProtected) {
            InflictBattleDamage(player, gBattle.side[player].damage,
                                LOC(player, gBattle.atkSlot), LOC((1 - player), gBattle.defSlot));
            /* Effects of the monster that dealt the damage (the target's, for the attacker's player). */
            switch (CARD_NUMBER_OF(gBattle.side[1 - player].cardId)) {
            case CARD_WHITE_MAGICAL_HAT:
                ShowCardEffect(1 - player, gBattle.side[1 - player].cardId);
                DuelPrompt_PostRandomDiscard(player, 1, 1);
                break;
            case CARD_MASKED_SORCERER:
                ShowCardEffect(1 - player, gBattle.side[1 - player].cardId);
                DrawCards(1 - player, 1);
                break;
            case CARD_THE_BISTRO_BUTCHER:
                ShowCardEffect(1 - player, gBattle.side[1 - player].cardId);
                DrawCards(player, 2);
                break;
            case CARD_1328:
            case CARD_1511:
                {
                    /* Matching: separate statements keep the ROM's order of the OR chain. */
                    u32 trigger = ((1 - player) & 1) << 31;
                    u32 kindEvent = gBattle.defSlot << 16;
                    kindEvent |= TRIGGER_BATTLE_DEFLECTED_DAMAGE;
                    trigger |= kindEvent;
                    trigger |= gBattle.side[1 - player].cardId;
                    /* locs: the damage, then the attacker and the target (player | zone << 4 each) in the
                     * high half. */
                    Chain_AddPending(trigger,
                                     gBattle.side[player].damage
                                     | ((((u8)player & 15) | gBattle.atkSlot << 4)
                                        | (((u8)(1 - player) & 15) | gBattle.defSlot << 4) << 8) << 16);
                }
                break;
            }
            {
                /* Relinquished and key 1334 with an absorbed monster deal the same damage to the other
                 * player. */
                int number = CARD_NUMBER_OF(gBattle.side[player].cardId);

                if ((number == CARD_RELINQUISHED || number == CARD_1334)
                    && FindAbsorbedMonsterLinkInt(player, gBattle.atkSlot) != 0xFFFF
                    && !gDuelPlayers[(1 - player) & 1].noBattleDamage
                    && !gDuelPlayers[(1 - player) & 1].battleProtected)
                    InflictBattleDamage(1 - player, gBattle.side[player].damage,
                                        LOC(player, gBattle.atkSlot),
                                        LOC((1 - player), gBattle.defSlot));
            }
        }
        gDuel.battleStep++;
        goto ret0;
    case 1:
        if (gBattle.side[1 - player].damage != 0 && !gDuelPlayers[(1 - player) & 1].noBattleDamage
            && !gDuelPlayers[(1 - player) & 1].battleProtected
            && CountHandCardsByNumber(1 - player, CARD_KURIBOH) != 0) {
            /* Matching: the same field as gDuel.battleStep, addressed from the player base so CSE reuses that
             * register (ROM: r4 + 0x1B12). */
            ((struct DuelState *)((u8 *)gDuelPlayers - OFFSET_OF(struct DuelState, players)))->battleStep = 10;
            goto ret0;
        }
        gDuel.battleStep++;
        goto ret0;
    case 2:
        if (gBattle.side[1 - player].damage != 0 && !gDuelPlayers[(1 - player) & 1].noBattleDamage
            && !gDuelPlayers[(1 - player) & 1].battleProtected) {
            InflictBattleDamage(1 - player, gBattle.side[1 - player].damage,
                                LOC(player, gBattle.atkSlot), LOC((1 - player), gBattle.defSlot));
            /* Effects of the monster that dealt the damage (the attacker's, for the defender's player). */
            switch (CARD_NUMBER_OF(gBattle.side[player].cardId)) {
            case CARD_WHITE_MAGICAL_HAT:
                ShowCardEffect(player, gBattle.side[player].cardId);
                DuelPrompt_PostRandomDiscard(1 - player, 1, 1);
                break;
            case CARD_MASKED_SORCERER:
                ShowCardEffect(player, gBattle.side[player].cardId);
                DrawCards(player, 1);
                break;
            case CARD_THE_BISTRO_BUTCHER:
                ShowCardEffect(player, gBattle.side[player].cardId);
                DrawCards(1 - player, 2);
                break;
            case CARD_1328:
            case CARD_1511:
                {
                    u32 trigger = (player & 1) << 31;
                    u32 kindEvent = gBattle.atkSlot << 16;
                    kindEvent |= TRIGGER_BATTLE_DAMAGE;
                    trigger |= kindEvent;
                    trigger |= gBattle.side[player].cardId;
                    Chain_AddPending(trigger,
                                     gBattle.side[1 - player].damage
                                     | ((((u8)(1 - player) & 15) | gBattle.defSlot << 4)
                                        | (((u8)player & 15) | gBattle.atkSlot << 4) << 8) << 16);
                }
                break;
            }
            {
                int number = CARD_NUMBER_OF(gBattle.side[1 - player].cardId);

                if ((number == CARD_RELINQUISHED || number == CARD_1334)
                    && FindAbsorbedMonsterLinkInt(1 - player, gBattle.defSlot) != 0xFFFF
                    && !gDuelPlayers[player & 1].noBattleDamage && !gDuelPlayers[player & 1].battleProtected)
                    InflictBattleDamage(player, gBattle.side[1 - player].damage,
                                        LOC(player, gBattle.atkSlot),
                                        LOC((1 - player), gBattle.defSlot));
            }
        }
        gDuel.battleStep++;
        goto ret0;
    case 10:
        /* Ask the defender (1 - player) whether to discard Kuriboh: the human (player 0) gets a Yes/No box, the
         * CPU always says yes, and the link partner is asked over the link. */
        if (player != 0) {
            /* Matching: the name table is a cast constant, reloaded into the next rotation register (r5). */
            FormatStr(question, gStrAskKuribohFmt, (const char *)0x0822C720 + (gUnk_08623E66[0] << 6));
            TextBoxOpen(TEXTBOX_XY(4, 2), TEXTBOX_XY(21, 9), TEXTBOX_FLAGS_DEFAULT, (u8 *)question);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            gDuel.battleStep++;
        } else if (!gDuelCtrl.isLinkDuel) {
            gTextBox.result = 1;
            gDuel.battleStep++;
        } else {
            DuelLink_SendMessage(LINKMSG_CARD_PROMPT, gUnk_08623E66[0], 0, 0);
            LINK_CARD_PROMPT->answered = 0;
        }
        gDuel.battleStep++;
    /* FAKEMATCH: every `return 0` jumps to this one label after case 10's step++, which places the shared
     * return-0 block between case 10 and case 11 as in the ROM. */
    ret0:
        return 0;
    case 11:
        /* Wait for the link partner's answer. */
        if ((int)(gLinkStateBytes[0x450] << 30) >= 0)   /* !cardPromptAnswered */
            goto ret0;
        gTextBox.result = *(u16 *)(gLinkStateBytes + 0x45A);   /* cardPromptAnswer */
        gDuel.battleStep++;
        goto ret0;
    case 12:
        /* Yes: show Kuriboh and discard it; the defender takes no damage. Back to step 2 either way. */
        if (gTextBox.result != 0) {
            sub_080197C0(1 - player, gUnk_08623E66[0]);
            if (DiscardHandCardByNumber(1 - player, CARD_KURIBOH) != 0)
                gBattle.side[1 - player].damage = 0;
        }
        gDuel.battleStep = 2;
        goto ret0;
    default:
        gDuel.players[0].noBattleDamage = 0;
        gDuel.players[1].noBattleDamage = 0;
        /* Big Shield Gardna that survived in defense position turns to attack position. */
        if (CARD_NUMBER_OF(gBattle.side[1 - player].cardId) == CARD_BIG_SHIELD_GARDNA
            && !gBattle.side[1 - player].destroyed) {
            int side = (1 - player) & 1;
            int offset = ZONE_OFFSET(side, gBattle.defSlot);

            if (FIELD_ZONE(offset)->isDefense) {
                ShowCardEffect(1 - player, gBattle.side[1 - player].cardId);
                ChangeBattlePosition(1 - player, gBattle.defSlot, 0, 0);
            }
        }
        {
            int i;
            struct Battle *bs;
            const u16 *swordOfDragonsSoul;

            /* Sides marked effectDestroy that were not destroyed get a link from Sword of Dragon's Soul
             * (destroyed at the end of the Battle Phase). Matching: bs and the card ID table are loop-hoisted
             * bases after i = 0 (ROM order); the flags byte is read through SIDE_ROW() so its base is not
             * merged with bs. */
            for (i = 0, bs = &gBattle, swordOfDragonsSoul = &gCardNumberToId[CARD_SWORD_OF_DRAGONS_SOUL]; i < 2; i++) {
                u8 flags = SIDE_ROW(i)->flags;

                if ((int)(flags << 25) < 0 && (int)(flags << 28) >= 0)
                    QueueAddZoneLink(player, swordOfDragonsSoul[0],
                                     (u8)i | (i == player ? bs->atkSlot : bs->defSlot) << 8, ZONE_LINK_CARD_EFFECT);
            }
        }
        return 1;
    }
}

/*
 * Stage 9: queue the flip effect of a defender that the attack flipped face up. flipEffectPending was set by
 * BattleStage_RevealDefender (HasFlipEffect) and flipCardId is the defender's card. The effect is chained as a
 * monster trigger of the defender's controller (1 - player) unless Parasite Paracide's side was destroyed.
 * Always returns 1.
 */
int BattleStage_TriggerFlipEffect(int player)
{
    int opponent;
    u32 canActivate;

    if (gBattle.flipEffectPending) {
        opponent = 1 - player;
        canActivate = (u16)CanActivateEffectOfCard(opponent, gBattle.flipCardId, 0);
        if (CARD_NUMBER_OF(gBattle.flipCardId) == CARD_PARASITE_PARACIDE) {
            if (gBattle.side[opponent].destroyed)
                canActivate = 0;
        }
        if (canActivate != 0) {
            /* Matching: separate statements, as in InflictDamage. */
            u32 trigger = ((1 - player) & 1) << 31;
            u32 kindEvent = gBattle.defSlot << 16;
            kindEvent |= TRIGGER_BATTLE_FLIP_EFFECT;
            trigger |= kindEvent;
            trigger |= gBattle.flipCardId;
            Chain_AddPending(trigger, 0);
        }
    }
    return 1;
}

/* Matching: gBattle.zones[] (the copies of the two battling zones) with the status byte of the card word
 * spelled out: byte +3 holds pendingEquip (bit 0), equipZone (bits 1-3) and pendingOpponentSummon (bit 4), the
 * struct DuelCard bits 24-28. struct DuelCard has a u32 container (a word read-modify-write); the member chain
 * of this view folds the offset of byte 3 into the address, which the ROM does. */
struct BattleZoneCopy {
    u8 unk0[3];
    u8 pendingEquip:1;
    u8 equipZone:3;
    u8 pendingOpponentSummon:1;
    u8 unk3_5:3;
    u8 unk4[0x94 - 4];
};
struct BattleZoneCopies {
    u8 unk0[0x20];
    struct BattleZoneCopy zones[2];     /* gBattle.zones */
};
#define ZONE_COPY(side) (((struct BattleZoneCopies *)&gBattle)->zones[side])

/* Matching: the card word of a zone is read whole (ldr, lsl 20) and tested for a nonzero ID. */
#define ZONE_HAS_CARD(zone) ((*(u32 *)&(zone)->card << 20) != 0)

/*
 * Stage 10: the effects that trigger when two monsters battle, then send the destroyed monsters away.
 *   Direct attack: nothing to do, return 1.
 *   Second call (battleStep already advanced): post RESPONSE_BATTLE_DESTROYED to the target's controller with
 *     both destroyedLoc entries, unless neither side sent a card away (both 0xFFFF); return 1.
 *   First call: for the attacker's side, then the target's side:
 *     - the monster's own battle effect: Dimensional Warrior chains its effect; Zone Eater and Swordsman from
 *       a Foreign Land put a 5-turn destroy countdown on the target (Steel Scorpion: 3 turns on the attacker
 *       unless it is a Machine; Electric Lizard: the attacker cannot attack next turn unless it is a Zombie);
 *       Wall of Illusion chains when its attacker survived; Big Shield Gardna turns to attack position; key
 *       1523 links ATK -200 to itself when the other monster was destroyed;
 *     - when this side's monster was destroyed: Insect Queen on the other side sets that player's
 *       insectQueenWonBattle; Relinquished or key 1334 with an absorbed monster lose that monster first
 *       (DestroyAbsorbedMonsters, and DUEL_CMD_SET_ZONE_CARD_WORD restores the card word); a Dimensional Warrior
 *       in the battle banishes the card (BanishBattleDestroyedCard); otherwise keys 1254-1256 link a stats
 *       penalty to the other monster, key 1327 / 1340 flag the graveyard card (pendingEquip with the zone of the
 *       other monster, pendingOpponentSummon: resolved by BattleStage_EndBattlePhase) and the card goes to the
 *       graveyard (SendBattleDestroyedCardToGraveyard); destroyedLoc[side] records its location.
 *   Advances battleStep and returns 0, so the commands play out before the second call.
 *
 * Matching: one function-scope pointer `z` for both zone copies. Its refs and live length put it ahead of
 * 1 - player in global allocation (ROM: z in r5, 1 - player in r6 in the second block); a block-local z loses
 * r5 to 1 - player.
 */
int BattleStage_DestroyMonsters(int player)
{
    int number;
    int done;
    u16 *z;

    if (gBattle.direct)
        return 1;
    if (gDuel.battleStep) {
        /* Both entries are 0xFFFF when no card was sent away. */
        if (*(s32 *)gBattle.destroyedLoc != -1)
            EventResponse_Request(1 - player, RESPONSE_BATTLE_DESTROYED,
                                  gBattle.destroyedLoc[1] << 16 | gBattle.destroyedLoc[0]);
        return 1;
    }

    /* ---- The attacking monster ---- */
    switch (CARD_NUMBER_OF(gBattle.side[player].cardId)) {
    case CARD_DIMENSIONAL_WARRIOR:
        {
            u32 trigger = player << 31;
            u32 kindEvent = gBattle.atkSlot << 16;
            kindEvent |= TRIGGER_MONSTER_BATTLED;
            trigger |= kindEvent;
            trigger |= gBattle.side[player].cardId;
            /* locs: the attacker and the target (player | zone << 8 each). */
            Chain_AddPending(trigger, (LOC(player, gBattle.atkSlot))
                                      | (LOC((1 - player), gBattle.defSlot)) << 16);
        }
        break;
    case CARD_ZONE_EATER:
    case CARD_SWORDSMAN_FROM_A_FOREIGN_LAND:
        if (!IsBattleEffectBlockedFor(player)) {
            ShowCardEffect(player, gBattle.side[player].cardId);
            /* The command acts for the target's controller (1 - player): a 5-turn destroy countdown. */
            DuelCmd_Push(player != 1 ? DUEL_CMD_PLAYER | DUEL_CMD_SET_DESTROY_COUNTDOWN
                                     : DUEL_CMD_SET_DESTROY_COUNTDOWN, gBattle.defSlot, 5, 0);
        }
        break;
    case CARD_MECHANICAL_SPIDER:
        if (!IsBattleEffectBlockedFor(player))
            ShowCardEffect(player, gBattle.side[player].cardId);
        break;
    case CARD_1523:
        if (gBattle.side[1 - player].destroyed) {
            int side = player & 1;
            int offset = ZONE_OFFSET(side, gBattle.atkSlot);

            if (ZONE_HAS_CARD(FIELD_ZONE(offset))) {
                ShowCardEffect(player, gBattle.side[player].cardId);
                QueueAddZoneLink(player, LOC(player, gBattle.atkSlot), LOC(player, gBattle.atkSlot),
                                 LINK_ATK_DOWN_200_ONCE);
            }
        }
        break;
    }
    gBattle.destroyedLoc[0] = 0xFFFF;
    if (gBattle.side[player].destroyed) {
        done = 0;
        if (CARD_NUMBER_OF(gBattle.side[1 - player].cardId) == CARD_INSECT_QUEEN)
            PLAYER_VIA_BASE(1 - player).insectQueenWonBattle = 1;
        number = CARD_NUMBER_OF(gBattle.side[player].cardId);
        if ((number == CARD_RELINQUISHED || number == CARD_1334)
            && FindAbsorbedMonsterLinkInt(player, gBattle.atkSlot) != 0xFFFF) {
            z = (u16 *)&gBattle.zones[player];
            DestroyAbsorbedMonsters(player, gBattle.atkSlot);
            DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_SET_ZONE_CARD_WORD : DUEL_CMD_SET_ZONE_CARD_WORD,
                         gBattle.atkSlot, z[0], z[1]);
            done = 1;
        }
        if (CARD_NUMBER_OF(gBattle.side[1 - player].cardId) != CARD_DIMENSIONAL_WARRIOR
            && CARD_NUMBER_OF(gBattle.side[player].cardId) != CARD_DIMENSIONAL_WARRIOR) {
            if (!done) {
                /* A switch, not `number >= CARD_1254 && number <= CARD_1256`: the ROM reloads the number and
                 * tests both bounds. */
                switch (CARD_NUMBER_OF(gBattle.side[player].cardId)) {
                case CARD_1254:
                case CARD_1255:
                case CARD_1256:
                    sub_080197C0(player, gBattle.side[player].cardId);
                    QueueAddZoneLink(player, gBattle.side[player].cardId,
                                     LOC((1 - player), gBattle.defSlot), ZONE_LINK_STATS_DOWN_500);
                    break;
                }
                switch (CARD_NUMBER_OF(gBattle.side[1 - player].cardId)) {
                case CARD_1327:
                    ZONE_COPY(player).pendingEquip = 1;
                    ZONE_COPY(player).equipZone = gBattle.defSlot;
                    break;
                case CARD_1340:
                    ZONE_COPY(player).pendingOpponentSummon = 1;
                    break;
                }
                SendBattleDestroyedCardToGraveyard(1 - player, player, gBattle.atkSlot,
                                                   (u16 *)&gBattle.zones[player]);
                gBattle.destroyedLoc[0] = LOC(player, gBattle.atkSlot);
            }
        } else if (!done)
            BanishBattleDestroyedCard(player, gBattle.atkSlot, (u16 *)&gBattle.zones[player]);
    }

    /* ---- The target ---- */
    switch (CARD_NUMBER_OF(gBattle.side[1 - player].cardId)) {
    case CARD_DIMENSIONAL_WARRIOR:
        {
            u32 trigger = ((1 - player) & 1) << 31;
            /* (sic) atkSlot, as in the ROM, although the card is the target's. */
            u32 kindEvent = gBattle.atkSlot << 16;
            kindEvent |= TRIGGER_MONSTER_BATTLED;
            trigger |= kindEvent;
            trigger |= gBattle.side[1 - player].cardId;
            Chain_AddPending(trigger, (LOC(player, gBattle.atkSlot))
                                      | (LOC((1 - player), gBattle.defSlot)) << 16);
        }
        break;
    case CARD_WALL_OF_ILLUSION:
        if (!IsBattleEffectBlockedFor(player) && !gBattle.side[player].destroyed) {
            u32 trigger = ((1 - player) & 1) << 31;
            u32 kindEvent = gBattle.defSlot << 16;
            kindEvent |= TRIGGER_MONSTER_BATTLED;
            trigger |= kindEvent;
            trigger |= gBattle.side[1 - player].cardId;
            Chain_AddPending(trigger, (LOC(player, gBattle.atkSlot))
                                      | (LOC((1 - player), gBattle.defSlot)) << 16);
        }
        break;
    case CARD_STEEL_SCORPION:
        if (!IsBattleEffectBlockedFor(player) && GetZoneCardType(player, gBattle.atkSlot) != CARD_TYPE_MACHINE) {
            ShowCardEffect(1 - player, gBattle.side[1 - player].cardId);
            /* A 3-turn destroy countdown on the attacker. */
            DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_SET_DESTROY_COUNTDOWN
                                : DUEL_CMD_SET_DESTROY_COUNTDOWN, gBattle.atkSlot, 3, 0);
        }
        break;
    case CARD_ELECTRIC_LIZARD:
        if (!IsBattleEffectBlockedFor(player) && GetZoneCardType(player, gBattle.atkSlot) != CARD_TYPE_ZOMBIE) {
            ShowCardEffect(1 - player, gBattle.side[1 - player].cardId);
            DuelCmd_Push(player ? DUEL_CMD_PLAYER | DUEL_CMD_SET_CANNOT_ATTACK_NEXT_TURN
                                : DUEL_CMD_SET_CANNOT_ATTACK_NEXT_TURN, gBattle.atkSlot, 1, 0);
        }
        break;
    case CARD_BIG_SHIELD_GARDNA:
        if (!gBattle.side[1 - player].destroyed) {
            int side = (1 - player) & 1;
            int offset = ZONE_OFFSET(side, gBattle.defSlot);

            if (FIELD_ZONE(offset)->isDefense) {
                ShowCardEffect(1 - player, gBattle.side[1 - player].cardId);
                DuelCmd_Push(player != 1 ? DUEL_CMD_PLAYER | DUEL_CMD_CHANGE_POSITION
                                         : DUEL_CMD_CHANGE_POSITION, gBattle.defSlot, 0, 0);
            }
        }
        break;
    case CARD_1523:
        if (gBattle.side[player].destroyed) {
            int side = (1 - player) & 1;
            int offset = ZONE_OFFSET(side, gBattle.defSlot);

            if (ZONE_HAS_CARD(FIELD_ZONE(offset))) {
                ShowCardEffect(1 - player, gBattle.side[1 - player].cardId);
                QueueAddZoneLink(1 - player, LOC((1 - player), gBattle.defSlot),
                                 LOC((1 - player), gBattle.defSlot), LINK_ATK_DOWN_200_ONCE);
            }
        }
        break;
    }
    gBattle.destroyedLoc[1] = 0xFFFF;
    if (gBattle.side[1 - player].destroyed) {
        done = 0;
        if (CARD_NUMBER_OF(gBattle.side[player].cardId) == CARD_INSECT_QUEEN)
            PLAYER_VIA_BASE(player).insectQueenWonBattle = 1;
        number = CARD_NUMBER_OF(gBattle.side[1 - player].cardId);
        if ((number == CARD_RELINQUISHED || number == CARD_1334)
            && FindAbsorbedMonsterLinkInt(1 - player, gBattle.defSlot) != 0xFFFF) {
            z = (u16 *)&gBattle.zones[1 - player];
            DestroyAbsorbedMonsters(1 - player, gBattle.defSlot);
            DuelCmd_Push(player != 1 ? DUEL_CMD_PLAYER | DUEL_CMD_SET_ZONE_CARD_WORD
                                     : DUEL_CMD_SET_ZONE_CARD_WORD, gBattle.defSlot, z[0], z[1]);
            done = 1;
        }
        if (CARD_NUMBER_OF(gBattle.side[1 - player].cardId) != CARD_DIMENSIONAL_WARRIOR
            && CARD_NUMBER_OF(gBattle.side[player].cardId) != CARD_DIMENSIONAL_WARRIOR) {
            if (!done) {
                switch (CARD_NUMBER_OF(gBattle.side[1 - player].cardId)) {
                case CARD_1254:
                case CARD_1255:
                case CARD_1256:
                    /* (sic) player, as in the ROM. */
                    sub_080197C0(player, gBattle.side[1 - player].cardId);
                    QueueAddZoneLink(1 - player, gBattle.side[1 - player].cardId,
                                     LOC(player, gBattle.atkSlot), ZONE_LINK_STATS_DOWN_500);
                    break;
                }
                switch (CARD_NUMBER_OF(gBattle.side[player].cardId)) {
                case CARD_1327:
                    ZONE_COPY(1 - player).pendingEquip = 1;
                    ZONE_COPY(1 - player).equipZone = gBattle.atkSlot;
                    break;
                case CARD_1340:
                    ZONE_COPY(1 - player).pendingOpponentSummon = 1;
                    break;
                }
                SendBattleDestroyedCardToGraveyard(1 - player, 1 - player, gBattle.defSlot,
                                                   (u16 *)&gBattle.zones[1 - player]);
                gBattle.destroyedLoc[1] = LOC((1 - player), gBattle.defSlot);
            }
        } else if (!done)
            BanishBattleDestroyedCard(1 - player, gBattle.defSlot, (u16 *)&gBattle.zones[1 - player]);
    }
    gDuel.battleStep++;
    return 0;
}
