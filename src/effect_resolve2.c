/*
 * effect_resolve2 (0x08031BC8-0x08032CAF): card effect resolve handlers, part 2
 * (wiki/functions/effect-resolve2-c.md; part 1 is effect_resolve1.c).
 *
 * Resolve handlers (+0x04) of gCardEffects rows (struct CardEffect, include/effect.h) for card numbers
 * 499-689, plus the shared single-target destroy, return-to-hand and destroy-by-type handlers: mostly
 * flip-effect monsters (Dragon Seeker, Needle Worm, Patrol Robo, ...) and Traps (Kunai with Chain, Crush
 * Card, Acid Trap Hole, Reverse Trap, Fake Trap). Same convention as part 1: Chain_Resolve calls a handler
 * with gChain.effectStep = EFFECT_STEP_START (0x80), then with the value it returned last, until it returns
 * EFFECT_STEP_DONE (0); the handlers act through queued duel commands and the duel actions. Player 1 is the
 * CPU or the link partner. link->targets[] holds the positions player | zone << 8 chosen by the chainB
 * handler; single-target handlers test the target again (occupied, face up or down, the Check handler),
 * because the board can change before the chain resolves.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE, CARD_STATS_POINTS_SCALE */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum DuelZoneIndex, ResponseEventKind, ChainEntryKind */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h and duel_cmd.h do not pull in the legacy header. After H0, replace the block (BEGIN
 * to END) with #include "duel.h": that gives identical assembly (checked against the staged header,
 * build/readability/issues/effect_resolve2.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u8 unk6_6:2;
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 banishedCount;               /* +0x006 */
    u8 unk7[0x28 - 0x7];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4 */
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

u32 HasFlipEffect(u16 cardNo, int inBattle);
int CountActiveCardsOnField(int player, u16 cardNo);
/* ---- END duel.h stand-in ---- */

#include "chain.h"                  /* struct ChainEntry, gChain, Chain_AddPending */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_actions.h"           /* destroy, flip, position, hand and deck actions, card pictures */
#include "effect.h"                 /* enum EffectStep, DestroyFieldCardByEffect, IsZoneTargetable */
#include "effect_handlers.h"        /* the resolve handlers defined here and the Check handlers they call */

/* Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view"). */
/* Matching: the ROM compares the effective ATK and DEF as signed ints (cmp; bgt/ble); the header's u32
 * returns would give unsigned compares. */
extern int GetZoneCardAtkInt(int player, int zone) asm("GetZoneCardAtk");
extern int GetZoneCardDefInt(int player, int zone) asm("GetZoneCardDef");
/* Matching: the card pictures are defined with a u16 card ID; where this unit passes an int or u32 ID the
 * ROM has no narrowing at the call (the narrowing would also reorder the argument moves). */
extern void ShowCardDetailInt(int player, int cardId) asm("ShowCardDetail");
extern void ShowDestroyedCardInt(int player, int cardId) asm("ShowDestroyedCard");
extern void ShowRevealedCardInt(int player, int cardId) asm("ShowRevealedCard");
/* Matching: DuelCmd_Push is defined with int arg4/arg6; Crush Card's pointer at a monster zone passes
 * zone << 8 as arg4, which the ROM narrows to u16 at the call (lsl #24; lsr #16). */
extern void DuelCmd_Push16(u16 cmd, u16 arg2, u16 arg4, u16 arg6) asm("DuelCmd_Push");
/* gDuel's rule-flag byte at +0x1ACD (bit 5: statChangesReversed, set by Reverse Trap). Matching: the ROM
 * reads the whole byte and computes 1 & ~(byte >> 5); the 1-bit field would give lsl/lsr. */
struct DuelRuleFlagsView {
    u8 unk0[0x1ACD];
    u8 flags1ACD;
};
extern struct DuelRuleFlagsView gDuelRuleFlagsView asm("gDuel");
#define RULE_FLAGS_STAT_CHANGES_REVERSED_SHIFT 5

/* The card word of a zone or pile slot as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with lsl #20; lsr #20; a bitfield read of .id loads only the halfword. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: these give the ROM's literal pools; the symbol forms generate other code.
 */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType; <= 20 is a monster */

/*
 * &gDuelZones[player].zones[zone] by byte arithmetic. Matching: ZONE_AT puts the zone term first (the
 * ROM's usual order); EffectFakeTrapResolve was matched with the player term first (ZONE_AT_PLAYER_FIRST).
 * player must be 0 or 1 (masked with & 1).
 */
#define ZONE_AT(player, zone) ((struct DuelZone *)((zone) * 0x94 + (player) * 0xD64 + (u32)gDuelZones))
#define ZONE_AT_PLAYER_FIRST(player, zone) \
    ((struct DuelZone *)((player) * 0xD64 + (zone) * 0x94 + (u32)gDuelZones))

/* Duel command id for a player: DUEL_CMD_PLAYER marks player 1 as the acting player. */
#define CMD_FOR(isPlayer1, cmd) ((isPlayer1) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* Dragon Seeker (499): destroy the target if it still is a face-up Dragon it may target (its Check). */
int EffectDragonSeekerResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        u8 targetPlayer = link->targets[0];
        int targetZone = link->targets[0] >> 8;

        if (EffectDragonSeekerCheck(link, targetZone << 8 | targetPlayer)) {
            DestroyFieldCardByEffect(targetPlayer, targetZone);
            OnCardDestroyedByEffect(link->player, targetPlayer, targetZone);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Single-target destroy (Dream Clown, Man-Eater Bug, Tribute to The Doomed, Mystical Space Typhoon, Gust,
 * Driving Snow, keys 1211 and 1301). Tribute to The Doomed and key 1211 only flip up and show a face-down
 * Defense Position Big Shield Gardna. A Magic card cannot destroy a monster that Magic cannot affect
 * (IsZoneTargetable). Destroying an opponent's card first sets its destroyed-by-opponent flag.
 */
int EffectDestroyTargetResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        u8 targetZone;
        int targetPlayer;
        int side;
        int targetZoneInt; /* FAKEMATCH: an int copy of the u8 targetZone makes `<= 4` a signed compare (bgt) */
        struct DuelZone *zone;
        int id;

        targetPlayer = (u8)link->targets[0];
        targetZone = link->targets[0] >> 8;
        side = targetPlayer & 1;
        zone = ZONE_AT(side, targetZone);
        id = CARD_ID(CARD_WORD(zone->card));

        if (id) {
            switch (CARD_NUMBER(link->card)) {
            case CARD_TRIBUTE_TO_THE_DOOMED:
            case CARD_1211:
                if (CARD_NUMBER(id) == CARD_BIG_SHIELD_GARDNA && zone->isDefense && !zone->isFaceUp) {
                    DuelCmd_Push(CMD_FOR(targetPlayer, DUEL_CMD_FLIP_CARD), targetZone, 0, 0);
                    sub_080197C0(targetPlayer, CARD_ID(CARD_WORD(zone->card)));
                    return EFFECT_STEP_DONE;
                }
                break;
            }
            targetZoneInt = targetZone;
            {
                /* The type of the activating card (link->card), not of the target.
                 * FAKEMATCH: the volatile read forces the second link->card load (otherwise CSE'd with the
                 * switch's CARD_NUMBER read); putting the mask in a local makes its load precede that
                 * volatile ldrh, as in the ROM. */
                u32 idMask = CARD_ID_MASK;
                u16 cardId = *(volatile u16 *)link;
                int cardType = CARD_STATS_TYPE(((const u32 *)0x08621DE0)[idMask & cardId]);

                if (cardType == CARD_TYPE_MAGIC && targetZoneInt <= ZONE_MONSTER_4
                    && IsZoneTargetable(targetPlayer, targetZone) == 0)
                    return EFFECT_STEP_DONE;
            }
            if (link->player != targetPlayer)
                DuelCmd_Push(CMD_FOR(targetPlayer, DUEL_CMD_SET_DESTROYED_BY_OPPONENT_FLAG), targetZone, 1, 0);
            DestroyFieldCardByEffect(targetPlayer, targetZone);
            OnCardDestroyedByEffect(link->player, targetPlayer, targetZone);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Crass Clown (94) and Hane-Hane: return the target to its owner's hand. */
int EffectReturnTargetToHandResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        u8 targetPlayer = link->targets[0];
        int targetZone = link->targets[0] >> 8;
        int side = targetPlayer & 1;
        struct DuelZone *zone = ZONE_AT(side, targetZone);

        if (CARD_ID(CARD_WORD(zone->card)))
            ReturnFieldCardToHand(targetPlayer, targetZone, 0);
    }
    return EFFECT_STEP_DONE;
}

/* Needle Worm (561): send the top 5 cards of the opponent's deck to the graveyard. */
int EffectNeedleWormResolve(struct ChainEntry *link)
{
    if (!link->negated)
        SendTopDeckCardsToGraveyard(1 - link->player, 5, 1);
    return EFFECT_STEP_DONE;
}

/*
 * Patrol Robo (579): look at the opponent's face-down target: flip it up, show it in the Card Detail view,
 * use up Patrol Robo's once-per-turn effect (clear its zone's effectUnused) and flip the card back down.
 */
int EffectPatrolRoboResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = link->targets[0] >> 8;
        int side = targetPlayer & 1;
        struct DuelZone *zone = ZONE_AT(side, targetZone);
        int id = CARD_ID(CARD_WORD(zone->card));

        if (id && targetPlayer != link->player && !zone->isFaceUp) {
            DuelCmd_Push(CMD_FOR(targetPlayer, DUEL_CMD_FLIP_CARD), targetZone, 0, 0);
            ShowCardDetailInt(targetPlayer, id);
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
            DuelCmd_Push(CMD_FOR(targetPlayer, DUEL_CMD_FLIP_CARD), targetZone, 0, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Weather Report (582): destroy the opponent's face-up Swords of Revealing Light; if there was one, the
 * player gets a second Battle Phase. Does not test link->negated. */
int EffectWeatherReportResolve(struct ChainEntry *link)
{
    int found = 0;
    int i;

    for (i = ZONE_SPELL_0; i <= ZONE_SPELL_4; i++) {
        int side = (1 - link->player) & 1;
        struct DuelZone *zone = ZONE_AT(side, i);
        u16 id = CARD_ID(CARD_WORD(zone->card));
        int side2 = (1 - link->player) & 1;
        struct DuelZone *zone2 = ZONE_AT(side2, i);

        if (zone2->isFaceUp && id && CARD_NUMBER(id) == CARD_SWORDS_OF_REVEALING_LIGHT) {
            DestroyFieldCard(1 - link->player, i, 1);
            found = 1;
        }
    }
    if (found)
        DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_SET_EXTRA_BATTLE_PHASE), 1, 0, 0);
    return EFFECT_STEP_DONE;
}

/* Greenkappa (585): destroy the two targets that are still face down. */
int EffectGreenkappaResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 2) {
        int i;

        for (i = 0; i < link->numTargets; i++) {
            int targetPlayer = (u8)link->targets[i];
            int targetZone = link->targets[i] >> 8;
            int side = 1 & targetPlayer;
            struct DuelZone *zone = ZONE_AT(side, targetZone);

            if (CARD_ID(CARD_WORD(zone->card)) && !zone->isFaceUp)
                DestroyFieldCard(targetPlayer, targetZone, 1);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Morphing Jar (590): both players discard their hands, then draw 5 cards.
 *   START   discard the player's first hand card per call until the hand is empty.
 *   2       the same for the opponent.
 *   else    both players draw 5.
 */
int EffectMorphingJarResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            struct DuelPlayer *players = gDuelPlayers;

            if (players[link->player].handCount != 0) {
                DiscardHandCard(link->player, 0, 0, 1);
                return EFFECT_STEP_START;
            }
            return EFFECT_STEP_2;
        }
        case EFFECT_STEP_2: {
            struct DuelPlayer *players = gDuelPlayers;
            int opponentSide = (1 - link->player) & 1;

            if (players[opponentSide].handCount != 0) {
                DiscardHandCard(1 - link->player, 0, 1, 1);
                return EFFECT_STEP_2;
            }
            return EFFECT_STEP_3;
        }
        default:
            DrawCards(link->player, 5);
            DrawCards(1 - link->player, 5);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Penguin Soldier (601): return the (up to two) targets to their owners' hands. */
int EffectPenguinSoldierResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int i;

        for (i = 0; i < link->numTargets && i <= 1; i++) {
            u8 targetPlayer = link->targets[i];
            int targetZone = link->targets[i] >> 8;
            int side = targetPlayer & 1;
            struct DuelZone *zone = ZONE_AT(side, targetZone);

            if (CARD_ID(CARD_WORD(zone->card)))
                ReturnFieldCardToHand(targetPlayer, targetZone, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Hiro's Shadow Scout (610): the opponent draws 3 cards (the top three of the deck, one draw command each).
 * Each Magic card among them is shown and discarded; the others are shown and kept. handIndex follows the
 * drawn cards in the opponent's hand: it starts at the hand count and skips the kept cards.
 */
int EffectHirosShadowScoutResolve(struct ChainEntry *link)
{
    /* FAKEMATCH: the redundant outer & 1 puts movs #1 after the bit extraction */
    int handIndex = gDuelPlayers[((link->player & 1) ^ 1) & 1].handCount;

    if (!link->negated) {
        int i;

        for (i = 0; i <= 2; i++) {
            int side = (1 - link->player) & 1;

            if (i < gDuelPlayers[side].deckCount) {
                /* FAKEMATCH: a u16 copy of the side (an int copy changes the code) */
                u16 side2 = (1 - link->player) & 1;
                u16 id = CARD_ID(CARD_WORD(gDuelPlayers[side2].deck[i]));

                /* the opponent draws: the command is for the other player */
                DuelCmd_Push(CMD_FOR(!link->player, DUEL_CMD_DRAW_CARDS), 1, 1, 0);
                if (CARD_TYPE(id) == CARD_TYPE_MAGIC) {
                    ShowDestroyedCard(link->player, id);
                    DiscardHandCard(1 - link->player, handIndex, 1, 1);
                } else {
                    ShowRevealedCard(link->player, id);
                    handIndex++;
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Invader of the Throne (640): swap control of Invader of the Throne and the opponent's target monster. */
int EffectInvaderOfTheThroneResolve(struct ChainEntry *link)
{
    /* FAKEMATCH: the empty r8 clobber gives the ROM's register allocation of the two zone copies */
    __asm__("" : : : "r8");
    if (!link->negated && link->numTargets == 1) {
        int player = link->player;
        int ownZone = link->zone;
        int targetPlayer = (u8)link->targets[0];
        int targetZone = link->targets[0] >> 8;

        if (player != targetPlayer) {
            int side = player & 1;
            struct DuelZone *zone = ZONE_AT(side, ownZone);

            if (CARD_ID(CARD_WORD(zone->card))) {
                int side2 = 1 & targetPlayer;
                struct DuelZone *zone2 = ZONE_AT(side2, targetZone);

                if (CARD_ID(CARD_WORD(zone2->card)))
                    SwapFieldCards(player, ownZone << 8 | player, targetPlayer | targetZone << 8);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Kunai with Chain (650): activated against an attack (event RESPONSE_ATTACK_DECLARED), switch the
 * opponent's attacking monster (loc0) in Attack Position to Defense Position. Then, with one target: equip
 * Kunai to the player's own face-up monster, or else destroy Kunai itself. Does not test link->negated.
 */
int EffectKunaiWithChainResolve(struct ChainEntry *link)
{
    if (link->event == RESPONSE_ATTACK_DECLARED) {
        int attackerPlayer = (u8)link->loc0;
        int attackerZone = link->loc0 >> 8;
        int side = 1 & attackerPlayer;
        struct DuelZone *zone = ZONE_AT(side, attackerZone);

        if (CARD_ID(CARD_WORD(zone->card)) && !zone->isDefense && link->player != attackerPlayer)
            ChangeBattlePosition(attackerPlayer, attackerZone, 0, 0);
    }
    if (link->numTargets == 1) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = link->targets[0] >> 8;
        int side = targetPlayer & 1;
        struct DuelZone *zone = ZONE_AT(side, targetZone);

        if (CARD_ID(CARD_WORD(zone->card)) && zone->isFaceUp && link->player == targetPlayer)
            EquipCard(link->player, link->player | (link->zone << 8), link->targets[0]);
        else
            DestroyFieldCard(link->player, link->zone, 1);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Warrior Elimination, Eternal Rest, Stain Storm and the other "destroy all <type> monsters" cards: destroy
 * every monster that the card's Check handler accepts, the opponent's first.
 */
int EffectDestroyAllByTypeResolve(struct ChainEntry *link)
{
    int sideCopy; /* FAKEMATCH: extra copy of side feeds EffectDestroyByTypeCheck and fixes scheduling */
    if (!link->negated) {
        int round;

        for (round = 0; round <= 1; round++) {
            int side;
            u8 sideByte;
            int i;

            if (round)
                side = link->player;
            else
                side = 1 - link->player;
            for (i = ZONE_MONSTER_0, sideByte = (sideCopy = side); i <= ZONE_MONSTER_4; i++) {
                sideCopy = sideByte;
                if (EffectDestroyByTypeCheck(link, (u8)i << 8 | sideCopy)) {
                    DestroyFieldCard(side, i, 1);
                    OnCardDestroyedByEffect(link->player, side, i);
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Printed ATK of a card ID: 0 for Trap, Magic and Ticket cards, 4000 for the Divine-Beasts, else the stats
 * field * 10. Matching: the u16 return keeps the register order of Crush Card's monster loop (the face-up
 * bit in r5, the ID in r6).
 */
static inline u16 GetCardBaseAtk(u16 id)
{
    u32 type = CARD_TYPE(id);
    switch ((s32)type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    }
    return ((CARD_STATS(id) >> 9) & 0x1FF) * CARD_STATS_POINTS_SCALE;
}

/*
 * Crush Card (660): destroy the opponent's monsters with 1500 ATK or more, on the field and in the hand.
 *   START   point at each opponent monster. A face-up one is destroyed if its effective ATK is over 1499.
 *           A face-down one is flipped up and checked by its printed ATK: destroyed (shown as destroyed),
 *           or shown and flipped back down.
 *   2       walk the opponent's hand, one card per call (gChain.effectSubStep is the hand index): point at
 *           it; a monster with printed ATK over 1499 is shown and discarded (the index stays), any other
 *           card is shown and skipped.
 *   3       set the opponent's Crush Card turns (3; its draws are checked then) and end.
 */
int EffectCrushCardResolve(struct ChainEntry *link)
{
    int opponent = 1 - link->player;
    int i;
    u32 type;

    if (link->negated)
        return EFFECT_STEP_DONE;
    switch (gChain.effectStep) {
    case EFFECT_STEP_START:
        for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
            int side = opponent & 1;
            struct DuelZone *zone = ZONE_AT(side, i);
            u16 id = CARD_ID(CARD_WORD(zone->card));
            if (id != 0) {
                u32 faceUp = zone->isFaceUp;
                DuelCmd_Push16(CMD_FOR(link->player, DUEL_CMD_POINT_AT_CARD), opponent, i << 8, 0);
                if (faceUp == 0) {
                    u32 atk;
                    DuelCmd_Push(CMD_FOR(opponent, DUEL_CMD_FLIP_CARD), i, 0, 0);
                    atk = GetCardBaseAtk(id);
                    if (atk <= 1499) {
                        ShowRevealedCard(opponent, id);
                        DuelCmd_Push(CMD_FOR(opponent, DUEL_CMD_FLIP_CARD), i, 0, 0);
                    } else {
                        ShowDestroyedCard(opponent, id);
                        DestroyFieldCardByEffect(opponent, i);
                        OnCardDestroyedByEffect(link->player, opponent, i);
                    }
                } else if (GetZoneCardAtkInt(opponent, i) > 1499) {
                    DestroyFieldCardByEffect(opponent, i);
                    OnCardDestroyedByEffect(link->player, opponent, i);
                }
            }
        }
        gChain.effectSubStep = 0;
        return EFFECT_STEP_2;
    case EFFECT_STEP_2:
        if (gChain.effectSubStep < gDuelPlayers[opponent & 1].handCount) {
            u8 handIndex;
            u32 id;
            u32 index = gChain.effectSubStep;
            handIndex = index;
            /* FAKEMATCH: load into r0, copy to r2, index from r0 (cf. AiStrategyCyberStein) */
            asm("" : "+r"(index));
            id = CARD_ID(CARD_WORD(gDuelPlayers[opponent & 1].hand[index]));
            /* point at hand card handIndex (area | index << 8, area DUEL_AREA_HAND) */
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_POINT_AT_CARD),
                         opponent, (handIndex << 8) | DUEL_AREA_HAND, 0);
            type = CARD_TYPE(id);
            if (type <= CARD_TYPE_REPTILE) {
                u32 atk;
                switch ((int)type) {
                case CARD_TYPE_TRAP:
                case CARD_TYPE_MAGIC:
                case CARD_TYPE_TICKET:
                    atk = 0;
                    break;
                case CARD_TYPE_DIVINE:
                    atk = 4000;
                    break;
                default:
                    atk = CARD_STATS_ATK(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE;
                    break;
                }
                if (atk > 1499) {
                    ShowDestroyedCardInt(opponent, id);
                    DiscardHandCard(opponent, gChain.effectSubStep, 1, 1);
                    return EFFECT_STEP_2;
                }
            }
            /* Matching: the int view (u32 id, no u16 narrowing) keeps opponent's live range short enough
             * for r4. */
            ShowRevealedCardInt(opponent, id);
            gChain.effectSubStep++;
            return EFFECT_STEP_2;
        }
        return EFFECT_STEP_3;
    default:
        DuelCmd_Push(CMD_FOR(opponent, DUEL_CMD_SET_CRUSH_CARD_TURNS), 3, 0, 0);
        return EFFECT_STEP_DONE;
    }
}

/* Harpie's Feather Duster (671): destroy every card in the opponent's spell/trap and field zones. */
int EffectHarpiesFeatherDusterResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int i;

        for (i = ZONE_SPELL_0; i <= ZONE_FIELD; i++) {
            int side = (1 - link->player) & 1;
            struct DuelZone *zone = ZONE_AT(side, i);

            if (CARD_ID(CARD_WORD(zone->card)))
                DestroyFieldCard(1 - link->player, i, 5);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Acid Trap Hole (684): flip the face-down Defense Position target face up (without its flip effect).
 *   START   needs a face-down Defense Position card in the target zone; flip it.
 *   2       DEF over 2000: show it and flip it back down. Otherwise show it as destroyed, queue its flip
 *           effect (a RESPONSE_FLIPPED trigger) if HasFlipEffect allows it and key 1530 (negates flip
 *           effects) is on neither field, and destroy it.
 */
int EffectAcidTrapHoleResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = link->targets[0] >> 8;
        /* FAKEMATCH: the ROM keeps side in r8 and the CARD_1530 constant in r7; as a plain pseudo side
         * outranks the constant in global allocation (2*5/54 vs 3/18) and takes r7. */
        register int side asm("r8");
        struct DuelZone *zone;
        int id;

        side = targetPlayer;
        /* FAKEMATCH: keeps the copy side = targetPlayer separate from the and (ROM: mov r8,r4 first) */
        asm("" : "+r"(side));
        side &= 1;
        zone = ZONE_AT(side, targetZone);
        id = CARD_ID(CARD_WORD(zone->card));

        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            if (id && zone->isDefense && !zone->isFaceUp) {
                FlipFieldCard(targetPlayer, targetZone, 0);
                return EFFECT_STEP_2;
            }
            break;
        case EFFECT_STEP_2:
            if (GetZoneCardDefInt(targetPlayer, targetZone) > 2000) {
                /* Matching: the int view; the u16 prototype adds a narrowing that reorders the argument
                 * moves */
                ShowRevealedCardInt(targetPlayer, id);
                FlipFieldCard(targetPlayer, targetZone, 0);
            } else {
                ShowDestroyedCardInt(targetPlayer, id);
                if (HasFlipEffect(CARD_NUMBER(id), 0) != 0 && CountActiveCardsOnField(0, CARD_1530) == 0
                    && CountActiveCardsOnField(1, CARD_1530) == 0) {
                    /* Trigger word (Chain_AddPending): card | zone << 16 | kind << 21 | event << 25 |
                     * player << 31. Matching: separate statements; in one expression fold moves the
                     * constant next to side << 31. */
                    u32 playerBit = (u32)side << 31;
                    u32 trigger = (targetZone & 0x1F) << 16 | (CHAIN_KIND_MONSTER << 21 | RESPONSE_FLIPPED << 25);

                    Chain_AddPending(playerBit | trigger | id, 0);
                }
                DestroyFieldCardByEffect(targetPlayer, targetZone);
                OnCardDestroyedByEffect(link->player, targetPlayer, targetZone);
            }
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Reverse Trap (688): toggle gDuel.statChangesReversed (stat modifiers are subtracted while it is set). */
int EffectReverseTrapResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        u16 cmd = CMD_FOR(link->player, DUEL_CMD_SET_STAT_CHANGES_REVERSED);
        int reversed = 1 & ~(gDuelRuleFlagsView.flags1ACD >> RULE_FLAGS_STAT_CHANGES_REVERSED_SHIFT);

        DuelCmd_Push(cmd, reversed, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Fake Trap (689): answers the opponent's link chainedTo that would destroy the player's Traps. Negated by
 * this: the threatened Traps are revealed (a face-down one is flipped, shown and flipped back) and stay;
 * the player's other cards in the swept zones are destroyed; then chainedTo is negated when it is a Magic
 * or Trap activation (DUEL_CMD_NEGATE_ACTIVATION).
 *   Reaper of the Cards, Trap Master, Remove Trap, Mystical Space Typhoon, Gust, Driving Snow: their single
 *     target, if it is a Trap other than Fake Trap itself (anything else: no effect).
 *   Harpie's Feather Duster, Gryphon Wing: the player's zones 5-10.
 *   Heavy Storm: zones 5-10; Final Destiny: zones 0-10. For these two, each zone that holds a card on the
 *     opponent's side is then destroyed on the player's side (the ROM's code; it looks like an original bug,
 *     hypothesis).
 */
int EffectFakeTrapResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    int zone;
    u16 id; /* function scope, shared by every case: its loop-weighted refs give it r5 in case A too */

    if (link->negated || chainedTo == NULL || chainedTo->player == link->player)
        return EFFECT_STEP_DONE;
    switch (CARD_NUMBER(chainedTo->card)) {
    case CARD_REAPER_OF_THE_CARDS:
    case CARD_TRAP_MASTER:
    case CARD_REMOVE_TRAP:
    case CARD_MYSTICAL_SPACE_TYPHOON:
    case CARD_GUST:
    case CARD_DRIVING_SNOW: {
        u8 targetPlayer = chainedTo->targets[0];
        u32 targetZone = chainedTo->targets[0] >> 8;
        id = CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(targetPlayer & 1, targetZone)->card));
        if (id == 0)
            return EFFECT_STEP_DONE;
        if (targetPlayer == link->player && targetZone == link->zone)
            return EFFECT_STEP_DONE;
        if (CARD_TYPE(id) != CARD_TYPE_TRAP)
            return EFFECT_STEP_DONE;
        if (!ZONE_AT_PLAYER_FIRST(targetPlayer & 1, targetZone)->isFaceUp) {
            FlipFieldCard(targetPlayer, targetZone, 0);
            ShowRevealedCard(link->player, id);
            FlipFieldCard(targetPlayer, targetZone, 0);
        }
        if (CARD_TYPE(chainedTo->card) > CARD_TYPE_REPTILE)
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
        return EFFECT_STEP_DONE;
    }
    case CARD_HARPIES_FEATHER_DUSTER:
    case CARD_GRYPHON_WING:
        for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
            id = CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(link->player & 1, zone)->card));
            if (id != 0) {
                if (CARD_TYPE(id) == CARD_TYPE_TRAP) {
                    if (!ZONE_AT_PLAYER_FIRST(link->player & 1, zone)->isFaceUp) {
                        FlipFieldCard(link->player, zone, 0);
                        ShowRevealedCard(link->player, id);
                        FlipFieldCard(link->player, zone, 0);
                    }
                } else {
                    DestroyFieldCard(link->player, zone, 5);
                }
            }
        }
        if (CARD_TYPE(chainedTo->card) > CARD_TYPE_REPTILE)
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
        return EFFECT_STEP_DONE;
    case CARD_HEAVY_STORM:
        for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
            id = CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(link->player & 1, zone)->card));
            if (id != 0) {
                if (CARD_TYPE(id) == CARD_TYPE_TRAP) {
                    if (!ZONE_AT_PLAYER_FIRST(link->player & 1, zone)->isFaceUp) {
                        FlipFieldCard(link->player, zone, 0);
                        ShowRevealedCard(link->player, id);
                        FlipFieldCard(link->player, zone, 0);
                    }
                } else {
                    DestroyFieldCard(link->player, zone, 1);
                }
            }
        }
        for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
            /* nonzero card ID (card word << 20) on the opponent's side */
            if (CARD_WORD(ZONE_AT_PLAYER_FIRST((1 - link->player) & 1, zone)->card) << 20)
                DestroyFieldCard(link->player, zone, 1);
        }
        DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
        return EFFECT_STEP_DONE;
    case CARD_FINAL_DESTINY:
        for (zone = ZONE_MONSTER_0; zone <= ZONE_FIELD; zone++) {
            id = CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(link->player & 1, zone)->card));
            if (id != 0) {
                if (CARD_TYPE(id) == CARD_TYPE_TRAP) {
                    if (!ZONE_AT_PLAYER_FIRST(link->player & 1, zone)->isFaceUp) {
                        FlipFieldCard(link->player, zone, 0);
                        ShowRevealedCard(link->player, id);
                        FlipFieldCard(link->player, zone, 0);
                    }
                } else {
                    DestroyFieldCard(link->player, zone, 1);
                }
            }
        }
        for (zone = ZONE_MONSTER_0; zone <= ZONE_FIELD; zone++) {
            if (CARD_WORD(ZONE_AT_PLAYER_FIRST((1 - link->player) & 1, zone)->card) << 20)
                DestroyFieldCard(link->player, zone, 1);
        }
        DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
        return EFFECT_STEP_DONE;
    }
    return EFFECT_STEP_DONE;
}
