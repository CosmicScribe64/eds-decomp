/*
 * effect_resolve5 (0x08035198-0x080361CF): card effect resolve handlers, part 3: The Cheerful Coffin (1025)
 * to The Forceful Sentry (1077), and the resolve shared by the attack-response traps
 * (wiki/functions/effect-resolve5-c.md).
 *
 * Every function is the resolve slot of a gCardEffects row (include/effect.h). Chain_Resolve calls it as
 * resolve(link, chainedTo) with gChain.effectStep = EFFECT_STEP_START (0x80), stores the return value in
 * gChain.effectStep and calls it again next frame until it returns EFFECT_STEP_DONE (0). Multi-step
 * handlers count their steps down from 0x80 (enum EffectStep) and keep scratch values in
 * gChain.effectSubStep. Most handlers do nothing when the activation was negated (link->negated).
 *
 * Positions: link->loc0 and link->targets[] hold player | zone << 8 (DUEL_LOC); zones 0-4 hold monsters,
 * 5-9 spells and traps, 10 the Field Magic (enum DuelZoneIndex). A duel command carries DUEL_CMD_PLAYER
 * when it acts for player 1.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum DuelZoneIndex, ZoneLinkKind, ZoneStatusFlag, DuelPromptKind, ... */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* enum SoundEffect */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h (build/readability/hcheck/duel_core/staged/duel.h)
 * that this unit and the headers below use, with its names, types and bitfield containers (unused bytes are
 * padding), and defines duel.h's include guard so that the headers below do not pull in the legacy one.
 * After H0, replace the block (BEGIN to END) with #include "legacy/duel.h" and #include "sound.h"
 * (build/readability/issues/effect_resolve5.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

/* Needed by duel_screen.h (DuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;
    u16 area:4;
    u16 index:9;
    u16 isDefense:1;
    u16 isFaceUp:1;
    u16 unk2;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u16 destroyCountdown:4;         /* +0x06 bits 6-9 */
    u8 positionLocked:1;            /* +0x07 bit 2 */
    u8 unk7_3:1;
    u8 unk7_4:1;
    u8 effectUnused:1;              /* +0x07 bit 5 */
    u8 revivedByMonsterReborn:1;    /* +0x07 bit 6: Call of the Dark destroys these */
    u8 summonedFromGraveyard:1;     /* +0x07 bit 7 */
    u8 unk8[0x94 - 0x8];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 unk4[0x28 - 0x4];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 linkError:1;                 /* +0x1B12 bit 5 */
    u8 result:2;                    /* +0x1B12 bits 6-7: enum DuelResult */
    u8 unk1B13[0x1B64 - 0x1B13];
    u16 promptResult;               /* +0x1B64: answer of the last duel prompt (a hand slot, a card ID) */
    u8 unk1B66[0x1B78 - 0x1B66];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */

int CountMonsters(int player);
int FindFreeMonsterZone(int player);
int CountTributableMonsters(int player, int excludeZone);
u32 GetZoneCardAtk(u32 player, u32 slot);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* field actions: DestroyFieldCard, MoveFieldCard, EquipCard, ... */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_Post* */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* gCardEffects, enum EffectStep, FindCardEffect, prompt texts */
#include "effect_handlers.h"        /* the handlers defined here */
#include "summon.h"                 /* QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* MemCopy16, FormatInt */

/* Local views kept on purpose (matching choices, see build/readability/HEADERS.md). */
/* Matching: these prepare handlers are defined with one parameter; their resolve handlers call them with
 * (link, chainedTo, 0), which loads r1 and r2 before the call. */
extern int EffectTheCheerfulCoffinPrepare3(struct ChainEntry *card, int chainedTo, int fromHand)
    asm("EffectTheCheerfulCoffinPrepare");
extern int EffectLastWillPrepare3(struct ChainEntry *card, int chainedTo, int fromHand)
    asm("EffectLastWillPrepare");
/* Matching: Heavy Storm passes its loop counter as withEffects; the definition's u16 parameter would add a
 * narrowing (lsl; lsr) that the ROM does not have. */
extern void DestroyFieldCardInt(int player, int zone, int withEffects) asm("DestroyFieldCard");
/* Matching: IsValidEquipTarget and FindMonsterLinkedToCard are defined to return u16; Tailor of the Fickle
 * tests the results as ints, with no narrowing of r0 (lsl #16). */
extern int IsValidEquipTargetInt(int equipPlayer, int equipSlot, int targetPlayer, int targetSlot)
    asm("IsValidEquipTarget");
extern int FindMonsterLinkedToCardInt(int player, int slot) asm("FindMonsterLinkedToCard");

/* Matching: DuelCmd_Push is defined with int arg4/arg6; The Stern Mystic calls it with u16 ones (the
 * narrowing changes its register allocation). */
extern void DuelCmd_Push16(u16 cmd, u16 arg2, u16 arg4, u16 arg6) asm("DuelCmd_Push");

/* The command id for player: player 1's commands carry DUEL_CMD_PLAYER. */
#define PLAYER_CMD(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* ChainEntry.numTargets read as `7 & byte` (movs #7; ldrb; ands). Matching: where the handlers keep the
 * count in a local, the bitfield read (lsl #29; lsr #29) gives other code. */
#define LINK_NUM_TARGETS_BYTE(link) (7 & ((u8 *)(link))[0xA])

/* The card word of a zone as one u32. Matching: the ROM loads the whole word (ldr) and extracts the ID
 * with lsl #20; lsr #20. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)
/* The card ID as a table index: bits 0-10 (CARD_ID_MASK) of a card word, lsl #21; lsr #21. */
#define CARD_ID_INDEX(word) (((word) << 21) >> 21)

/* gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4) through integer-constant addresses. Matching:
 * the ROM reloads the table address at each use, which the symbols do not give. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))         /* enum CardType */
#define IS_MONSTER_TYPE(type) ((type) <= CARD_TYPE_REPTILE)    /* types 1-20 are monsters */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/* &gDuelZones[player].zones[zone] by byte arithmetic. Matching: the ROM adds the zone term, then the player
 * term, then the gDuelZones literal. Callers pass player & 1. */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))

/* DuelZone +0x06 (isDefense bit 0, isFaceUp bit 1) as a whole byte, for the tests of both bits at once
 * ((byte & 3) == value: ldrb; and; cmp). */
#define ZONE_POSITION_BYTE(zone) (((u8 *)(zone))[6])
#define ZONE_POSITION_DEFENSE 0x1   /* isDefense */
#define ZONE_POSITION_FACE_UP 0x2   /* isFaceUp */
#define ZONE_POSITION_MASK    0x3

/* gDuelPlayers[player].hand[index] as a word, by byte arithmetic from the gDuelHands literal. */
#define HAND_CARD_WORD(player, index) \
    (*(u32 *)((u32)gDuelHands + (index) * sizeof(struct DuelCard) + (player) * sizeof(struct DuelPlayer)))

/* CardListView_Open area -1: list the candidates CollectEffectTargets gathers for the card number. */
#define CARDLIST_AREA_EFFECT_TARGETS (-1)

/* Text box rectangles: TextBoxOpen takes x | y << 8 and width | height << 8, in cells. */
#define TEXTBOX_XY(x, y) ((x) | (y) << 8)

/* The status flags Solemn Judgment and Horn of Heaven clear on the monster whose summon they negate. */
#define ZONE_STATUS_SUMMON_FLAGS (ZONE_STATUS_UNK14 | ZONE_STATUS_NORMAL_SUMMONED | ZONE_STATUS_SPECIAL_SUMMONED)

/*
 * The Cheerful Coffin (1025): discard up to 3 Monster cards from the hand, asking before each one.
 * gChain.effectSubStep counts the discards left. A pick that is not a monster buzzes and waits for another
 * pick (there is no B-button exit, unlike Dust Tornado).
 */
int EffectTheCheerfulCoffinResolve(struct ChainEntry *link, int chainedTo)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = 3;
            gChain.effectStep--;
            /* fall through */
        case EFFECT_STEP_2:         /* discards left and a monster in the hand: ask Yes/No */
            if (gChain.effectSubStep == 0)
                return EFFECT_STEP_DONE;
            if (EffectTheCheerfulCoffinPrepare3(link, chainedTo, 0) == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrCheerfulCoffinDiscardPrompt);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:         /* No ends the effect */
            if (gTextBox.result == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrCheerfulCoffinSelectMonster);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:         /* pick a Monster card in the hand */
            if (DuelCursor_PickTarget(PICK_HAND) != 0) {
                int side = link->player & 1;
                struct DuelScreen *screen = &gDuelScreen;
                s32 *handIdx = &screen->selIndex;

                if (IS_MONSTER_TYPE(CARD_TYPE(CARD_ID_INDEX(HAND_CARD_WORD(side, *handIdx))))) {
                    PlaySE(SE_CONFIRM);
                    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_POINT_AT_CARD), screen->selPlayer,
                                 (u8)screen->selArea | (u8)*handIdx << 8, 0);
                    /* byOpponentEffect 1, although the player discards their own card */
                    DiscardHandCard(link->player, *handIdx, 1, 1);
                    gChain.effectSubStep--;
                    return EFFECT_STEP_2;
                }
                PlaySE(SE_ERROR);
            }
            return EFFECT_STEP_4;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Call of the Dark (1026): destroy every monster revived by Monster Reborn (DuelZone.revivedByMonsterReborn),
 * the opponent's first. */
int EffectCallOfTheDarkResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int i;

        for (i = 0; i <= 1; i++) {
            int player;
            int zone;

            if (i)
                player = link->player;
            else
                player = 1 - link->player;
            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                struct DuelZone *z = ZONE_AT(player & 1, zone);

                if (z->revivedByMonsterReborn) {
                    DestroyFieldCardByEffect(player, zone);
                    OnCardDestroyedByEffect(link->player, player, zone);
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Change of Heart (1027): take control of the opponent's monster targets[0] for the turn: move it into a
 * free monster zone of the player and link the card to it (ZONE_LINK_CARD_EFFECT). A face-down Defense
 * Position Big Shield Gardna negates the Magic: it is flipped face up and shown (sub_080197C0) instead.
 */
int EffectChangeOfHeartResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int numTargets = LINK_NUM_TARGETS_BYTE(link);

        if (numTargets == 1) {
            int targetPlayer = (u8)link->targets[0];
            int targetZone = link->targets[0] >> 8;
            int freeZone = FindFreeMonsterZone(link->player);

            if (targetPlayer != link->player) {
                int side = targetPlayer & 1;
                struct DuelZone *z = ZONE_AT(side, targetZone);
                int cardId = CARD_ID(CARD_WORD(z->card));

                if (cardId && freeZone != -1) {
                    if (CARD_NUMBER(cardId) == CARD_BIG_SHIELD_GARDNA
                        && (ZONE_POSITION_BYTE(z) & ZONE_POSITION_MASK) == ZONE_POSITION_DEFENSE) {
                        DuelCmd_Push(PLAYER_CMD(targetPlayer, DUEL_CMD_FLIP_CARD), targetZone, 0, 0);
                        sub_080197C0(targetPlayer, CARD_ID(CARD_WORD(z->card)));
                        return EFFECT_STEP_DONE;
                    } else {
                        MoveFieldCard(link->player, link->targets[0], link->player | (u8)freeZone << 8);
                        QueueAddZoneLink(link->player, link->card, link->player | (u8)freeZone << 8,
                                         ZONE_LINK_CARD_EFFECT);
                    }
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Solemn Judgment (1028). Answering a Magic or Trap card (chainedTo, read as its card ID): negate the
 * activation and destroy that card. Answering a summon: on the first call (if the conditions still hold)
 * clear the summon flags of the monster at loc0 and come back; on the next call destroy it.
 */
int EffectSolemnJudgmentResolve(struct ChainEntry *link, u16 *chainedTo)
{
    if (!link->negated) {
        if (chainedTo) {
            /* Matching: a switch, not type == CARD_TYPE_TRAP || type == CARD_TYPE_MAGIC (which gives
             * sub #21; cmp #1; bhi instead of the ROM's signed range compares). */
            int type = CARD_TYPE(*chainedTo);

            switch (type) {
            case CARD_TYPE_TRAP:
            case CARD_TYPE_MAGIC:
                /* arg2 1: also destroy the negated card */
                DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
            }
        } else if (gChain.effectStep == EFFECT_STEP_START) {
            if (EffectSolemnJudgmentPrepare(link, NULL, 0)) {
                DuelCmd_Push(PLAYER_CMD((u8)link->loc0, DUEL_CMD_CLEAR_ZONE_STATUS_FLAGS), link->loc0 >> 8,
                             ZONE_STATUS_SUMMON_FLAGS, 0);
                return EFFECT_STEP_2;
            }
        } else {
            u8 summonPlayer = link->loc0;
            int summonZone = link->loc0 >> 8;

            DestroyFieldCard(summonPlayer, summonZone, 1);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Horn of Heaven (1031): negate the summon of the monster at loc0 (clear its summon flags), then destroy it
 * on the next call. The tribute is the cost paid by its chainA handler. */
int EffectHornOfHeavenResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (gChain.effectStep == EFFECT_STEP_START) {
            /* Matching: (u8)link->loc0 written in the call (no local) loads loc0 as a halfword first. */
            DuelCmd_Push(PLAYER_CMD((u8)link->loc0, DUEL_CMD_CLEAR_ZONE_STATUS_FLAGS), link->loc0 >> 8,
                         ZONE_STATUS_SUMMON_FLAGS, 0);
            return EFFECT_STEP_2;
        } else {
            u8 summonPlayer = link->loc0;
            int summonZone = link->loc0 >> 8;

            DestroyFieldCard(summonPlayer, summonZone, 1);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Just Desserts (1032): the opponent loses 500 LP for each monster it controls. */
int EffectJustDessertsResolve(struct ChainEntry *link)
{
    int count = CountMonsters(1 - link->player);

    if (!link->negated && count > 0)
        LoseLifePoints(1 - link->player, count * 500);
    return EFFECT_STEP_DONE;
}

/* Fusion Sage (1040): add a Polymerization (either of its two card numbers) from the deck to the hand, then
 * shuffle. Does not test negated. */
int EffectFusionSageResolve(struct ChainEntry *link)
{
    if (AddDeckCardToHand(link->player, CARD_POLYMERIZATION)
        || AddDeckCardToHand(link->player, CARD_POLYMERIZATION_ALT))
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
    return EFFECT_STEP_DONE;
}

/* Block Attack (1043): switch the target (an opponent's monster) to Defense Position if it is in Attack
 * Position. */
int EffectBlockAttackResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int numTargets = LINK_NUM_TARGETS_BYTE(link);

        if (numTargets == 1) {
            u8 targetPlayer = link->targets[0];
            int targetZone = link->targets[0] >> 8;
            int side = targetPlayer & 1;
            struct DuelZone *z = ZONE_AT(side, targetZone);

            if (!z->isDefense)
                ChangeBattlePosition(targetPlayer, targetZone, 1, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/* The Stern Mystic (1048): look at every face-down card on the field, the opponent's first: point at it,
 * flip it face up, show it in the Card Detail view and flip it back. */
int EffectTheSternMysticResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int i;

        for (i = 0; i <= 1; i++) {
            int player;
            int zone;

            if (i)
                player = link->player;
            else
                player = 1 - link->player;
            for (zone = ZONE_MONSTER_0; zone <= ZONE_FIELD; zone++) {
                struct DuelZone *z = ZONE_AT(player & 1, zone);

                if (CARD_ID(CARD_WORD(z->card)) && !z->isFaceUp) {
                    /* Matching: the u16 view (DuelCmd_Push16) narrows (u8)zone << 8, which keeps (u16)player in
                     * a register across the loop as in the ROM. */
                    DuelCmd_Push16(PLAYER_CMD(link->player, DUEL_CMD_POINT_AT_CARD), player,
                                   (u8)zone << 8, 0);
                    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_FLIP_CARD), zone, 0, 0);
                    ShowCardDetail(link->player, CARD_ID(CARD_WORD(z->card)));
                    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_FLIP_CARD), zone, 0, 0);
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Wall of Illusion (1049): return the attacking monster (loc0) to its owner's hand. Does not test
 * negated. */
int EffectWallOfIllusionResolve(struct ChainEntry *link)
{
    u8 attackerPlayer = link->loc0;
    int attackerZone = link->loc0 >> 8;

    ReturnFieldCardToHand(attackerPlayer, attackerZone, 0);
    return EFFECT_STEP_DONE;
}

/* Last Will (1054): Special Summon a monster with ATK 1500 or less from the deck (picked in the card-list
 * viewer), then shuffle the deck. */
int EffectLastWillResolve(struct ChainEntry *link, int chainedTo)
{
    /* Matching: the chosen entry's address is computed here and again in EFFECT_STEP_3, as in the ROM. */
    u32 *card = &gCardListView.cards[gCardListView.cursorRow + gCardListView.top];

    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:     /* open the list of deck candidates */
            if (EffectLastWillPrepare3(link, chainedTo, 0)) {
                CardListView_Open(link->player, CARDLIST_AREA_EFFECT_TARGETS, CARD_NUMBER(link->card), 0);
                return EFFECT_STEP_2;
            }
            break;
        case EFFECT_STEP_2:         /* take the chosen card out of the deck */
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_REMOVE_CARD_FROM_DECK), ((u16 *)card)[0],
                         ((u16 *)card)[1], 0);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:         /* Special Summon it (the player chooses the position) */
            QueueSpecialSummonChoosePosition(
                link->player, (struct DuelCard *)&gCardListView.cards[gCardListView.cursorRow + gCardListView.top],
                1, 0);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
            return EFFECT_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Waboku (1055): on the opponent's turn, set the player's noBattleDamage and battleProtected flags
 * (DUEL_CMD_SET_BATTLE_PROTECTION 1, 1). Does not test negated. */
int EffectWabokuResolve(struct ChainEntry *link)
{
    if (gDuel.turnPlayer != link->player)
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_BATTLE_PROTECTION), 1, 1, 0);
    return EFFECT_STEP_DONE;
}

/*
 * The attack-response traps, chosen by card number; loc0 is the attacking monster:
 *   Mirror Force (1056)        destroy every Attack Position monster of the attacker's side
 *   Enchanted Javelin (1098)   gain LP equal to the attacker's ATK
 *   key 1214 (no EDS card)     negate the attack; the attacker's controller loses LP equal to its ATK
 *   key 1415 (no EDS card)     link the card to the attacker (ZONE_LINK_CARD_EFFECT)
 *   Widespread Ruin (685)      destroy the face-up Attack Position monster with the highest ATK on the
 *                              attacker's side; on a tie the human picks one of them (the CPU takes the first)
 * The single-target cases do nothing when the attacker is gone or face down.
 */
int EffectAttackResponseResolve(struct ChainEntry *link)
{
    u8 attackerPlayer = link->loc0;
    int attackerZone = link->loc0 >> 8;

    if (!link->negated) {
        switch (CARD_NUMBER(link->card)) {
        case CARD_MIRROR_FORCE: {
            int zone;

            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                struct DuelZone *z = ZONE_AT(attackerPlayer & 1, zone);

                if (CARD_ID(CARD_WORD(z->card)) && !z->isDefense) {
                    DestroyFieldCardByEffect(attackerPlayer, zone);
                    OnCardDestroyedByEffect(link->player, attackerPlayer, zone);
                }
            }
            break;
        }
        case CARD_ENCHANTED_JAVELIN: {
            int side = attackerPlayer & 1;
            struct DuelZone *z = ZONE_AT(side, attackerZone);

            if (CARD_ID(CARD_WORD(z->card)) && z->isFaceUp)
                GainLifePoints(link->player, GetZoneCardAtk(attackerPlayer, attackerZone));
            break;
        }
        case CARD_1214: {
            int side = attackerPlayer & 1;
            struct DuelZone *z = ZONE_AT(side, attackerZone);

            if (CARD_ID(CARD_WORD(z->card)) && z->isFaceUp) {
                DuelCmd_Push(PLAYER_CMD(attackerPlayer, DUEL_CMD_NEGATE_ATTACK), attackerZone, 0, 0);
                LoseLifePoints(attackerPlayer, GetZoneCardAtk(attackerPlayer, attackerZone));
            }
            break;
        }
        case CARD_1415: {
            int side = attackerPlayer & 1;
            struct DuelZone *z = ZONE_AT(side, attackerZone);

            if (CARD_ID(CARD_WORD(z->card)) && z->isFaceUp)
                QueueAddZoneLink(link->player, link->card, link->loc0, ZONE_LINK_CARD_EFFECT);
            break;
        }
        default:
            if (CARD_NUMBER(link->card) == CARD_WIDESPREAD_RUIN) {
                switch (gChain.effectStep) {
                case EFFECT_STEP_START: {   /* find the highest ATK among the face-up attackers */
                    int bestAtk = -1;
                    int bestZone = -1;
                    int count = 0;          /* monsters with bestAtk */
                    int zone;
                    u8 text[0x80];

                    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                        struct DuelZone *z = ZONE_AT(attackerPlayer & 1, zone);

                        if (CARD_ID(CARD_WORD(z->card))
                            && (ZONE_POSITION_BYTE(z) & ZONE_POSITION_MASK) == ZONE_POSITION_FACE_UP) {
                            int atk = GetZoneCardAtk(attackerPlayer, zone);

                            if (atk == bestAtk)
                                count++;
                            if (atk > bestAtk) {
                                bestAtk = atk;
                                bestZone = zone;
                                count = 1;
                            }
                        }
                    }
                    if (bestZone < 0)
                        break;
                    if (count == 1 || link->player) {
                        DestroyFieldCardByEffect(attackerPlayer, bestZone);
                        OnCardDestroyedByEffect(link->player, attackerPlayer, bestZone);
                    } else {
                        /* a tie: the human picks one of the monsters with that ATK */
                        FormatInt((char *)text, (const char *)gStrWidespreadRuinTiePrompt, bestAtk);
                        TextBoxOpen(TEXTBOX_XY(4, 2), TEXTBOX_XY(23, 8), TEXTBOX_FLAGS_DEFAULT, text);
                        gChain.scratch.effect.savedValue = bestAtk;
                        gChain.effectStep = EFFECT_STEP_9;
                    pick_again:
                        return EFFECT_STEP_9;
                    }
                    break;
                }
                case EFFECT_STEP_9:         /* the human picks a monster with the saved ATK */
                    if (DuelCursor_PickTarget(PICK_ANY_MONSTER << (attackerPlayer << 4)) == 0)
                        goto pick_again;
                    if (GetZoneCardAtk(attackerPlayer, gDuelScreen.selIndex) != gChain.scratch.effect.savedValue)
                        goto wrong_pick;
                    DestroyFieldCardByEffect(attackerPlayer, gDuelScreen.selIndex);
                    OnCardDestroyedByEffect(link->player, attackerPlayer, gDuelScreen.selIndex);
                    break;
                wrong_pick:
                    PlaySE(SE_ERROR);
                    goto pick_again;
                /* Matching: the default case after the wrong-pick block keeps the ROM's block order. */
                default:
                    return EFFECT_STEP_DONE;
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Share the Pain (1059): the opponent tributes a monster too (the player's tribute is the chainA cost). */
int EffectShareThePainResolve(struct ChainEntry *link)
{
    if (!link->negated && gChain.effectStep == EFFECT_STEP_START) {
        if (CountTributableMonsters(1 - link->player, -1)) {
            DuelPrompt_PostTribute(1 - link->player);
            return EFFECT_STEP_2;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Heavy Storm (1061): destroy every Magic and Trap card (zones 5-10), one card per call so that each
 * destruction gets its own step: gChain.effectSubStep is the side being swept, the opponent's first, then
 * the player's. The third DestroyFieldCard argument is the zone (5-10), which only acts as a nonzero
 * withEffects flag.
 */
int EffectHeavyStormResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = 1 - link->player;
            gChain.effectStep--;
            /* fall through */
        case EFFECT_STEP_2: {
            int zone;

            for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
                /* Matching: the side is read inside the loop (loop.c hoists it after zone = 5, the ROM's
                 * order) and the zone is reached through struct indexing. */
                int side = gChain.effectSubStep;
                struct DuelZone *z = &gDuelZones[(u8)side & 1].zones[zone];

                if (CARD_ID(CARD_WORD(z->card))) {
                    DestroyFieldCardInt(side, zone, zone);
                    return EFFECT_STEP_2;
                }
            }
            {
                /* This side is empty: go on with the other one, unless that was the player's own side.
                 * Matching: the pointer to effectSubStep and the player read before the compare give the
                 * ROM's flip-and-compare tail. */
                struct ChainState *chain = &gChain;
                u8 *side = &chain->effectSubStep;
                int player;

                *side = 1 - *side;
                player = link->player;
                if (*side == player)
                    return EFFECT_STEP_2;
            }
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Curse of Fiend (1064): change the battle position of every monster on the field, then lock position
 * changes for the turn. */
int EffectCurseOfFiendResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int player;

        for (player = 0; player <= 1; player++) {
            int zone;

            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                struct DuelZone *z = ZONE_AT(player & 1, zone);

                if (CARD_ID(CARD_WORD(z->card)))
                    ChangeBattlePosition(player, zone, 1, 1);
            }
        }
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_POSITION_CHANGE_LOCK), 1, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/* Upstart Goblin (1065): draw 1 card; the opponent gains 1000 LP. */
int EffectUpstartGoblinResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        DrawCards(link->player, 1);
        GainLifePoints(1 - link->player, 1000);
    }
    return EFFECT_STEP_DONE;
}

/* Final Destiny (1067): destroy every card on the field, the opponent's first (monsters only when
 * IsZoneTargetable allows it). */
int EffectFinalDestinyResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int i;

        for (i = 0; i <= 1; i++) {
            int player;
            int zone;

            if (i)
                player = link->player;
            else
                player = 1 - link->player;
            for (zone = ZONE_MONSTER_0; zone <= ZONE_FIELD; zone++) {
                struct DuelZone *z = ZONE_AT(player & 1, zone);

                if (CARD_ID(CARD_WORD(z->card)) && (zone > ZONE_MONSTER_4 || IsZoneTargetable(player, zone))) {
                    DestroyFieldCardByEffect(player, zone);
                    OnCardDestroyedByEffect(link->player, player, zone);
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Snatch Steal (1068): equip the card to the opponent's face-up monster targets[0] and take control of it
 * (move it into a free monster zone of the player). It needs the Snatch Steal card to be still face up in
 * its zone. destroyIfNegated is tested first; the negated bit only blocks the control change, so a negated
 * Snatch Steal still equips its target.
 */
int EffectSnatchStealResolve(struct ChainEntry *link)
{
    int player = link->player;
    int zone = link->zone;

    if (!link->destroyIfNegated) {
        struct DuelZone *z = ZONE_AT(player, zone);
        int cardId = CARD_ID(CARD_WORD(z->card));
        int numTargets;

        if (cardId && z->isFaceUp && CARD_NUMBER(cardId) == CARD_SNATCH_STEAL) {
            numTargets = LINK_NUM_TARGETS_BYTE(link);
            if (numTargets == 1) {
                int targetPlayer = (u8)link->targets[0];
                int targetZone = link->targets[0] >> 8;
                int freeZone = FindFreeMonsterZone(link->player);

                if (targetPlayer != link->player) {
                    int targetSide = targetPlayer & 1;
                    struct DuelZone *target = ZONE_AT(targetSide, targetZone);

                    if (CARD_ID(CARD_WORD(target->card)) && target->isFaceUp) {
                        /* Matching: the player term first in both packed locations (register allocation). */
                        EquipCard(link->player, link->player | link->zone << 8, targetPlayer | targetZone << 8);
                        if (!link->negated && freeZone != -1)
                            MoveFieldCard(link->player, link->targets[0], link->player | (u8)freeZone << 8);
                    }
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Confiscation (1070): look at the opponent's hand and discard the card the player picks. */
int EffectConfiscationResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            DuelPrompt_Post(link->player, PROMPT_PICK_OPPONENT_HAND_CARD, 0, 0);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:         /* promptResult: the picked hand slot */
            DiscardHandCard(1 - link->player, gDuel.promptResult, 1, 1);
            return EFFECT_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Delinquent Duo (1071): the opponent discards one random card, then one card of its choice. */
int EffectDelinquentDuoResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            DuelPrompt_PostRandomDiscard(1 - link->player, 1, 1);    /* byOpponent, 1 card */
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:
            DuelPrompt_PostDiscard(1 - link->player, 1, 0, 1);      /* 1 card, any kind, byOpponent */
            return EFFECT_STEP_3;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Darkness Approaches (1072): turn the face-up target face down without changing its position. */
int EffectDarknessApproachesResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int numTargets = LINK_NUM_TARGETS_BYTE(link);

        if (numTargets == 1) {
            u8 targetPlayer = link->targets[0];
            int targetZone = link->targets[0] >> 8;
            int side = targetPlayer & 1;
            struct DuelZone *z = ZONE_AT(side, targetZone);

            if (z->isFaceUp)
                FlipFieldCard(targetPlayer, targetZone, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Fairy's Hand Mirror (1073): run the chained Magic card's resolve handler on the new target. On the first
 * call, gChain.proxyLink becomes a copy of this link (with its new target) carrying the Magic card's ID and
 * player, and gChain.proxyResolve that card's resolve handler. Every call runs proxyResolve(&proxyLink,
 * NULL) and returns its step; when it is done (or the card has no handler) the original activation is
 * negated and destroyed.
 */
int EffectFairysHandMirrorResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    u8 *step;

    if (gChain.effectStep == EFFECT_STEP_START) {
        MemCopy16(&gChain.proxyLink, link, sizeof(struct ChainEntry));
        gChain.proxyLink.card = chainedTo->card;
        gChain.proxyLink.player = chainedTo->player;
        gChain.proxyResolve = gCardEffects[FindCardEffect(chainedTo->card)].resolve;
        if (gChain.proxyResolve == NULL) {
            /* FAKEMATCH: dead store; it keeps the step pointer from being shared with the setup block, so
             * the base is reloaded from the pool and link stays in r7. */
            step = 0;
            goto negate;
        }
    }
    {
        struct ChainState *chain = &gChain;
        int result = chain->proxyResolve(&chain->proxyLink, NULL);

        step = &chain->effectStep;
        *step = result;
        if (*step != EFFECT_STEP_DONE)
            goto running;
    }
negate:
    /* arg2 1: also destroy the negated card */
    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
    return EFFECT_STEP_DONE;
running:
    return *step;
}

/* Tailor of the Fickle (1074): move the equip card targets[0] to the monster targets[1], if the move is
 * still valid and the card is not already equipped to it. */
int EffectTailorOfTheFickleResolve(struct ChainEntry *link)
{
    u8 equipPlayer = link->targets[0];
    int equipZone = link->targets[0] >> 8;
    u8 monsterPlayer = link->targets[1];
    int monsterZone = link->targets[1] >> 8;

    if (!link->negated && link->numTargets == 2) {
        if (EffectTailorOfTheFickleCheck(link, link->targets[0])
            && IsValidEquipTargetInt(equipPlayer, equipZone, monsterPlayer, monsterZone)) {
            if (FindMonsterLinkedToCardInt(equipPlayer, equipZone) != link->targets[1])
                MoveEquipCard(link->targets[0], link->targets[1]);
        }
    }
    return EFFECT_STEP_DONE;
}

/* The Forceful Sentry (1077): look at the opponent's hand and return the card the player picks to the deck,
 * which the opponent shuffles. */
int EffectTheForcefulSentryResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            DuelPrompt_Post(link->player, PROMPT_PICK_OPPONENT_HAND_CARD, 0, 0);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:         /* promptResult: the picked hand slot */
            ReturnHandCardToDeck(1 - link->player, gDuel.promptResult, 1);
            DuelCmd_Push(PLAYER_CMD(!link->player, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
            return EFFECT_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}
