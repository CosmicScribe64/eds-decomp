/*
 * effect_resolve1 (0x08030B88-0x08031BC7): card effect resolve handlers, part 1
 * (wiki/functions/effect-resolve1-c.md; part 2 is effect_resolve2.c).
 *
 * Every function here is the resolve handler (+0x04) of a gCardEffects row (struct CardEffect,
 * include/effect.h), for card numbers 317-505 plus Blue Medicine, Raimei, Restructer Revolution, De-Spell
 * and the Field Magic cards. Chain_Resolve calls the handler of each link when the chain resolves, first
 * with gChain.effectStep = EFFECT_STEP_START (0x80), then once per frame with the value the handler
 * returned last, until it returns EFFECT_STEP_DONE (0). Multi-step handlers count down from 0x80 (0x7F,
 * 0x7E, ...): a step waits for a prompt answer or handles one card per call. EFFECT_STEP_END (0x64) asks
 * for one more call, which falls to the default case and ends the link.
 *
 * The handlers act through queued duel commands (DuelCmd_Push, include/duel_cmd.h) and the duel actions
 * (include/duel_actions.h). Player 1 is the CPU or the link partner: its branches choose without prompts.
 * Most handlers return at once when link->negated is set. link->targets[] holds the positions
 * player | zone << 8 chosen by the chainB handler; the single-target handlers test the target again,
 * because the board can change before the chain resolves.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_TYPE */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum DuelZoneIndex, ResponseEventKind, ZoneLinkKind, FieldPickMask */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, duel_cmd.h, card_list_view.h, summon.h and duel_screen.h do not pull in the legacy
 * header. After H0, replace the block (BEGIN to END) with #include "duel.h": that gives identical assembly
 * (checked against the staged header, build/readability/issues/effect_resolve1.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

/* Needed by duel_screen.h (DuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13 */
    u16 isDefense:1;                /* bit 14 */
    u16 isFaceUp:1;                 /* bit 15 */
    u16 unk2;
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

u32 GetFieldMagicIndex(u16 cardNo);
u32 GetZoneCardType(s32 player, s32 slot);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFreeMonsterZones(int player);
/* ---- END duel.h stand-in ---- */

#include "chain.h"                  /* struct ChainEntry, gChain, EventResponse_Request */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_actions.h"           /* LP, destroy, flip, position, hand and deck actions */
#include "effect.h"                 /* enum EffectStep, enum ChosenPlayer, effect helpers and prompt texts */
#include "effect_handlers.h"        /* the resolve handlers defined here */
#include "summon.h"                 /* QueueSpecialSummon, QueueSpecialSummonChoosePosition */
#include "card_list_view.h"         /* gCardListView, CardListView_Open, enum CardListSource */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "ai.h"                     /* gAiWork, AiPickCardListEntry */
#include "util.h"                   /* FormatStr */

/* Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view"). */
/* Matching: CollectEffectTargets is defined with a u16 return and a u16 card number; this unit passes an
 * int number and uses the count as an int, with no narrowing at the call. */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* The Field Magic zone of each player (gDuelPlayers[player].zones[ZONE_FIELD]) indexed with the player
 * stride. Matching: the ROM addresses it from the symbol gDuelFieldZone (0x020198D4), which duel.h
 * declares as a single struct DuelZone. */
struct FieldZoneStride {
    struct DuelCard card;
    u8 rest[0xD64 - 4];
};
extern struct FieldZoneStride gDuelFieldZoneByPlayer[2] asm("gDuelFieldZone");

/* The card word of a zone or pile slot as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with lsl #20; lsr #20 (CARD_ID). CARD_ID11 keeps 11 bits, the form the compiler folds
 * into a gCardIdToNumber index (lsl #21; lsr #20). A bitfield read of .id loads only the halfword. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)
#define CARD_ID11(word) (((word) << 21) >> 21)

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0), gCardIdToNumber (0x08622AB4)
 * and gCardNumberToId (0x08623DF4). Matching: these give the ROM's literal pools; the symbol forms
 * generate other code.
 */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType; <= 20 is a monster */
#define ID_TO_NUMBER ((const u16 *)0x08622AB4)              /* gCardIdToNumber */
#define NUMBER_TO_ID ((const u16 *)0x08623DF4)              /* gCardNumberToId */

/* &gDuelZones[player].zones[zone] by byte arithmetic, zone term first. Matching: this is the ROM's address
 * order (array indexing emits the player term first). player must be 0 or 1 (masked with & 1). */
#define ZONE_AT(player, zone) ((struct DuelZone *)((zone) * 0x94 + (player) * 0xD64 + (u32)gDuelZones))

/* ChainEntry.negated as the masked byte (byte 4 & 4: 0 or 4). Matching: Cyber-Stein keeps this value in a
 * local and passes it on; the 1-bit field read gives other code. */
#define ENTRY_NEGATED_BYTE(entry) (((u8 *)(entry))[4] & 4)

/* Duel command id for a player: DUEL_CMD_PLAYER marks player 1 as the acting player. */
#define CMD_FOR(isPlayer1, cmd) ((isPlayer1) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* Text box of the effect prompts: x 5, y 2, 20 x 9 cells. */
#define EFFECT_TEXT_POS 0x205
#define EFFECT_TEXT_SIZE 0x914

/* Card number 1249: an effect key with no EDS card that counts as a Harpie Lady (constants/cards.h has no
 * CARD_1249; build/readability/issues/effect_resolve1.md). */
#define HARPIE_LADY_KEY 1249

/*
 * Elegant Egotist (317): Special Summon a Harpie Lady or Harpie Lady Sisters from the hand or the deck.
 *   START   needs Harpie Lady (or key 1249) face up on a field, a candidate (CollectEffectTargets fills
 *           gCardListView) and a free monster zone. The CPU takes Harpie Lady Sisters, else key 1249, else
 *           Harpie Lady (else the AI's pick), and goes on to step 3; the human gets a prompt.
 *   2       open the card list for the human's pick.
 *   3       remove the picked card from the deck or the hand (its gCardListView source).
 *   4       Special Summon it, choosing the position.
 *   5       shuffle the deck if the card came from it.
 */
int EffectElegantEgotistResolve(struct ChainEntry *link)
{
    int i, count;

    if (link->negated)
        return EFFECT_STEP_DONE;
    switch (gChain.effectStep) {
    case EFFECT_STEP_START:
        if (!CountActiveCardsOnField(0, CARD_HARPIE_LADY) && !CountActiveCardsOnField(1, CARD_HARPIE_LADY)
            && !CountActiveCardsOnField(0, HARPIE_LADY_KEY) && !CountActiveCardsOnField(1, HARPIE_LADY_KEY))
            return EFFECT_STEP_DONE;
        if (CollectEffectTargetsInt(link->player, CARD_ELEGANT_EGOTIST, 0) == 0)
            return EFFECT_STEP_DONE;
        if (CountFreeMonsterZones(link->player) == 0)
            return EFFECT_STEP_DONE;
        if (link->player) {
            count = CollectEffectTargetsInt(1, CARD_ELEGANT_EGOTIST, 0);
            /* Matching: `cards + i` (not cards[i]) keeps 0x0201D81C as one pool constant, so the found
             * blocks address the viewer as base - 12. The u16 id locals in the first and third loops add
             * the RTL insns that make loop.c hoist the card table only in its second pass (after the
             * pointer copy), as the ROM does. */
            for (i = 0; i < count; i++) {
                u16 id = CARD_ID(*(gCardListView.cards + i));
                if (CARD_NUMBER(id) == CARD_HARPIE_LADY_SISTERS)
                    goto foundSisters;
            }
            for (i = 0; i < count; i++) {
                if (CARD_NUMBER(CARD_ID(*(gCardListView.cards + i))) == HARPIE_LADY_KEY)
                    goto foundKey;
            }
            for (i = 0; i < count; i++) {
                u16 id = CARD_ID(*(gCardListView.cards + i));
                if (CARD_NUMBER(id) == CARD_HARPIE_LADY)
                    goto foundHarpieLady;
            }
            AiPickCardListEntry(link->card);
            gCardListView.cursorRow = 0;
            gCardListView.top = gAiWork.listPick;
            return EFFECT_STEP_3;
        } else {
            TextBoxOpen(EFFECT_TEXT_POS, EFFECT_TEXT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrElegantEgotistSelectPrompt);
            return EFFECT_STEP_2;
        }
    case EFFECT_STEP_2:
        CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
        return EFFECT_STEP_3;
    case EFFECT_STEP_3: {
        /* the picked card word as its two halves (DuelCmd_Push operands) */
        u16 *picked = (u16 *)&gCardListView.cards[gCardListView.cursorRow + gCardListView.top];

        switch (gCardListView.sources[gCardListView.cursorRow + gCardListView.top]) {
        case CARDLIST_SRC_DECK:
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_REMOVE_CARD_FROM_DECK), picked[0], picked[1], 0);
            break;
        case CARDLIST_SRC_HAND:
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_REMOVE_CARD_FROM_HAND), picked[0], picked[1], 0);
            break;
        }
        return EFFECT_STEP_4;
    }
    case EFFECT_STEP_4:
        QueueSpecialSummonChoosePosition(link->player,
            (struct DuelCard *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow], 1, 1);
        return EFFECT_STEP_5;
    case EFFECT_STEP_5:
        if (gCardListView.sources[gCardListView.top + gCardListView.cursorRow] == CARDLIST_SRC_DECK)
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
        return EFFECT_STEP_END;
    /* the CPU's pick: show entry i at the top of the list */
    foundSisters:
        gCardListView.cursorRow = 0;
        gCardListView.top = i;
        return EFFECT_STEP_3;
    foundKey:
        gCardListView.cursorRow = 0;
        gCardListView.top = i;
        return EFFECT_STEP_3;
    foundHarpieLady:
        gCardListView.cursorRow = 0;
        gCardListView.top = i;
        return EFFECT_STEP_3;
    }
    return EFFECT_STEP_DONE;
}

/*
 * Stop Defense (319): switch the target to face-up Attack Position (ChangeBattlePosition with the flip
 * effect). A face-down Defense Position Big Shield Gardna is only flipped face up and shown.
 */
int EffectStopDefenseResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        u8 targetPlayer = link->targets[0];
        int targetZone = link->targets[0] >> 8;
        int side = targetPlayer & 1;
        struct DuelZone *zone = ZONE_AT(side, targetZone);

        if (CARD_NUMBER(CARD_ID11(CARD_WORD(zone->card))) == CARD_BIG_SHIELD_GARDNA && zone->isDefense
            && !zone->isFaceUp) {
            u16 cmd = CMD_FOR(targetPlayer, DUEL_CMD_FLIP_CARD);

            DuelCmd_Push(cmd, targetZone, 0, 0);
            sub_080197C0(targetPlayer, CARD_ID(CARD_WORD(zone->card)));
        } else {
            int side2 = targetPlayer & 1;
            struct DuelZone *zone2 = ZONE_AT(side2, targetZone);

            if (zone2->isDefense)
                ChangeBattlePosition(targetPlayer, targetZone, 1, 1);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Dragon Capture Jar (328): switch every face-up Attack Position Dragon (effective type) on both fields to
 * Defense Position. */
int EffectDragonCaptureJarResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int player;

        for (player = 0; player <= 1; player++) {
            int i;

            for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
                struct DuelZone *zone = ZONE_AT(player & 1, i);

                if (zone->isFaceUp && !zone->isDefense && CARD_ID(CARD_WORD(zone->card))
                    && GetZoneCardType(player, i) == CARD_TYPE_DRAGON)
                    ChangeBattlePosition(player, i, 0, 0);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * The Field Magic cards (14 rows): if the player's field zone holds a card, set the field background
 * (GetFieldMagicIndex of the card) and open the field-magic response window for the opponent. Does not
 * test link->negated.
 */
int EffectFieldMagicResolve(struct ChainEntry *link)
{
    if (CARD_ID(CARD_WORD(gDuelFieldZoneByPlayer[1 & link->player]))) {
        u16 cmd = CMD_FOR(link->player, DUEL_CMD_SET_FIELD_BACKGROUND);

        DuelCmd_Push(cmd, GetFieldMagicIndex(CARD_NUMBER(link->card)), 1, 0);
        EventResponse_Request(1 - link->player, RESPONSE_FIELD_MAGIC_PLAYED, 0);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Dark Hole (335, also key 1425): destroy every monster on the field, one per call. gChain.effectSubStep is
 * the side being cleared: the opponent's first, then the player's own. When a side has no monster left the
 * handler switches sides; it goes on after the switch to the player's side and ends after the switch back.
 */
int EffectDarkHoleResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = 1 - link->player;
            gChain.effectStep--;
            /* fall through */
        case EFFECT_STEP_2: {
            int i;

            for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
                if (IsZoneTargetable(gChain.effectSubStep, i)) {
                    DestroyFieldCardByEffect(gChain.effectSubStep, i);
                    OnCardDestroyedByEffect(link->player, gChain.effectSubStep, i);
                    return EFFECT_STEP_2;
                }
            }
            {
                /* Side cleared: switch sides; continue only if the new side is the player's own.
                 * FAKEMATCH: the side pointer pinned to r0 and the volatile reload reproduce the ROM's
                 * reload of the side byte after the store. */
                register u8 *side asm("r0");
                u8 *chain = (u8 *)&gChain;
                int player;

                side = chain + OFFSET_OF(struct ChainState, effectSubStep);
                *side = 1 - *side;
                player = link->player;
                if (*(volatile u8 *)side == player)
                    return EFFECT_STEP_2;
            }
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Raigeki (336): destroy the opponent's monsters, one per call (the first one Magic can affect). */
int EffectRaigekiResolve(struct ChainEntry *link)
{
    int i;

    if (link->negated)
        return EFFECT_STEP_DONE;
    for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
        if (IsZoneTargetable(1 - link->player, i)) {
            DestroyFieldCardByEffect(1 - link->player, i);
            OnCardDestroyedByEffect(link->player, 1 - link->player, i);
            return EFFECT_STEP_START;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Recover LP for the chosen player: Mooyan Curry 200, Goblin's Secret Remedy 600, Soul of the Pure 800,
 * Blue Medicine 400. targets[0] is not a position here but the answer of the chainB menu (enum
 * ChosenPlayer).
 */
int EffectGainLpChosenPlayerResolve(struct ChainEntry *link)
{
    int amount = 0;

    if (!link->negated) {
        switch (CARD_NUMBER(link->card)) {
        case CARD_MOOYAN_CURRY:
            amount = 200;
            break;
        case CARD_GOBLINS_SECRET_REMEDY:
            amount = 600;
            break;
        case CARD_SOUL_OF_THE_PURE:
            amount = 800;
            break;
        case CARD_BLUE_MEDICINE:
            amount = 400;
            break;
        }
        if (amount != 0) {
            switch (link->targets[0]) {
            case CHOSEN_PLAYER_SELF:
                GainLifePoints(link->player, amount);
                break;
            case CHOSEN_PLAYER_OPPONENT:
                GainLifePoints(1 - link->player, amount);
                break;
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Recover LP: Red Medicine 500, Dian Keto the Cure Master 1000, key 1315 1000 for both players. */
int EffectGainLpResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int player;
        u16 amount;

        switch (CARD_NUMBER(link->card)) {
        case CARD_RED_MEDICINE:
            player = link->player;
            amount = 500;
            break;
        case CARD_DIAN_KETO_THE_CURE_MASTER:
            player = link->player;
            amount = 1000;
            break;
        case CARD_1315:
            GainLifePoints(link->player, 1000);
            {
                int self = link->player;

                /* FAKEMATCH: the empty asm with an input and an r0 clobber keeps the reloaded player in r1,
                 * leaving r0 for the 1 - player subtraction. No code emitted. */
                asm("" : : "r"(self) : "r0");
                player = 1 - self;
            }
            amount = 1000;
            break;
        default:
            return EFFECT_STEP_DONE;
        }
        GainLifePoints(player, amount);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Burn the opponent: Sparks 200, Hinotama 500, Final Flame 600, Ookazi 800, Tremendous Fire 1000 (and 500
 * to its own controller), Raimei 300, Restructer Revolution 200 per card in the opponent's hand.
 */
int EffectDamageOpponentResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (CARD_NUMBER(link->card)) {
        case CARD_SPARKS:
            LoseLifePoints(1 - link->player, 200);
            break;
        case CARD_HINOTAMA:
            LoseLifePoints(1 - link->player, 500);
            break;
        case CARD_FINAL_FLAME:
            LoseLifePoints(1 - link->player, 600);
            break;
        case CARD_OOKAZI:
            LoseLifePoints(1 - link->player, 800);
            break;
        case CARD_TREMENDOUS_FIRE:
            LoseLifePoints(1 - link->player, 1000);
            LoseLifePoints(link->player, 500);
            break;
        case CARD_RAIMEI:
            LoseLifePoints(1 - link->player, 300);
            break;
        case CARD_RESTRUCTER_REVOLUTION: {
            struct DuelPlayer *players = gDuelPlayers;
            int side = (1 - link->player) & 1;

            if (players[side].handCount != 0) {
                /* Matching: the opponent is computed before the second side index (operand order). */
                int opponent = 1 - link->player;
                int side2 = (1 - link->player) & 1;

                LoseLifePoints(opponent, players[side2].handCount * 200);
            }
            break;
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Swords of Revealing Light (347): START flips the opponent's face-down monsters face up (with their flip
 * effects) and shows each one; the next call runs the Kotodama check, since the flips can reveal monsters
 * with the same name. The three-turn attack lock is handled elsewhere.
 */
int EffectSwordsOfRevealingLightResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (gChain.effectStep == EFFECT_STEP_START) {
            int i;

            for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
                int opponent = 1 - link->player;
                int side = opponent & 1;
                struct DuelZone *zone = ZONE_AT(side, i);

                if (CARD_ID(CARD_WORD(zone->card)) && !zone->isFaceUp) {
                    FlipFieldCard(opponent, i, 1);
                    ShowRevealedCard(opponent, CARD_ID(CARD_WORD(zone->card)));
                }
            }
            return EFFECT_STEP_2;
        }
        ApplyKotodama();
    }
    return EFFECT_STEP_DONE;
}

/* Spellbinding Circle (348, also key 1244): link the target monster to the Circle (a continuous-effect
 * link), which locks it. Does not test link->negated. */
int EffectSpellbindingCircleResolve(struct ChainEntry *link)
{
    if (link->numTargets == 1) {
        int side = link->targets[0] & 1;
        struct DuelZone *zone = ZONE_AT(side, link->targets[0] >> 8);

        if (CARD_ID(CARD_WORD(zone->card)))
            QueueAddZoneLink(link->player, link->player | (link->zone << 8), link->targets[0],
                             ZONE_LINK_CONTINUOUS);
    }
    return EFFECT_STEP_DONE;
}

/* Dark-Piercing Light (349): flip the opponent's face-down monsters face up, with their flip effects. */
int EffectDarkPiercingLightResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int i;

        for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
            int side = (1 - link->player) & 1;
            struct DuelZone *zone = ZONE_AT(side, i);

            if (CARD_ID(CARD_WORD(zone->card))) {
                int side2 = (1 - link->player) & 1;
                struct DuelZone *zone2 = ZONE_AT(side2, i);

                if (!zone2->isFaceUp)
                    FlipFieldCard(1 - link->player, i, 1);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Monster Eye (401): return a Polymerization (either of its two card numbers) from the graveyard to the
 * hand. Does not test link->negated. */
int EffectMonsterEyeResolve(struct ChainEntry *link)
{
    if (ReturnGraveyardCardToHand(link->player, CARD_POLYMERIZATION) == 0)
        ReturnGraveyardCardToHand(link->player, CARD_POLYMERIZATION_ALT);
    return EFFECT_STEP_DONE;
}

/* Blast Juggler (416): send Blast Juggler to the graveyard, then destroy each of its (up to two) targets that
 * still passes its Check handler (face up, ATK 1000 or less). */
int EffectBlastJugglerResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int i;

        TributeMonster(link->player, link->zone);
        for (i = 0; i < link->numTargets && i <= 1; i++) {
            u8 targetPlayer = link->targets[i];
            int targetZone = link->targets[i] >> 8;
            int side = targetPlayer & 1;
            struct DuelZone *zone = ZONE_AT(side, targetZone);

            if (CARD_ID(CARD_WORD(zone->card)) && EffectBlastJugglerCheck(link, link->targets[i])) {
                DestroyFieldCardByEffect(targetPlayer, targetZone);
                OnCardDestroyedByEffect(link->player, targetPlayer, targetZone);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Cyber-Stein (419) and Gale Dogra (505): pick a monster from the Fusion Deck.
 *   START   needs a Fusion Deck card (Cyber-Stein also a free monster zone); the CPU takes the AI's pick
 *           and goes on to step 3, the human gets a prompt.
 *   2       open the card list for the human's pick.
 *   3       remove the picked card from the Fusion Deck.
 *   4       Cyber-Stein Special Summons it; Gale Dogra sends it to the graveyard.
 */
int EffectCyberSteinResolve(struct ChainEntry *link)
{
    u8 negated = ENTRY_NEGATED_BYTE(link);

    if (!negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            if (gDuelPlayers[1 & link->player].fusionCount == 0)
                return EFFECT_STEP_DONE;
            switch (CARD_NUMBER(link->card)) {
            case CARD_CYBER_STEIN:
                if (CountFreeMonsterZones(link->player) == 0)
                    return EFFECT_STEP_DONE;
                if (link->player) {
                    AiPickCardListEntry(link->card);
                    gCardListView.cursorRow = 0;
                    gCardListView.top = gAiWork.listPick;
                    return EFFECT_STEP_3;
                }
                TextBoxOpen(EFFECT_TEXT_POS, EFFECT_TEXT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrCyberSteinSelectPrompt);
                return EFFECT_STEP_2;
            case CARD_GALE_DOGRA:
                if (link->player) {
                    AiPickCardListEntry(link->card);
                    gCardListView.cursorRow = 0;
                    gCardListView.top = gAiWork.listPick;
                    return EFFECT_STEP_3;
                }
                TextBoxOpen(EFFECT_TEXT_POS, EFFECT_TEXT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrGaleDograSelectPrompt);
                return EFFECT_STEP_2;
            }
            break;
        case EFFECT_STEP_2:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3: {
            u16 *picked = (u16 *)&gCardListView.cards[gCardListView.cursorRow + gCardListView.top];

            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_REMOVE_CARD_FROM_FUSION_DECK), picked[0], picked[1], 0);
            return EFFECT_STEP_4;
        }
        case EFFECT_STEP_4:
            switch (CARD_NUMBER(link->card)) {
            case CARD_CYBER_STEIN:
                /* Matching: the status flags are the (zero) negated byte, as in the ROM. */
                QueueSpecialSummon(link->player,
                    (struct DuelCard *)&gCardListView.cards[gCardListView.cursorRow + gCardListView.top], 1, 0,
                    negated);
                return EFFECT_STEP_END;
            case CARD_GALE_DOGRA: {
                u16 *picked = (u16 *)&gCardListView.cards[gCardListView.cursorRow + gCardListView.top];

                DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_ADD_CARD_TO_GRAVEYARD), picked[0], picked[1], 0);
                LoseLpOnSendToGraveyard(link->player, 1);
                break;
            }
            }
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Thunder Dragon (424): add up to two Thunder Dragons from the deck to the hand, one Yes/No prompt each.
 * gChain.effectSubStep counts the cards that may still be added.
 *   START/2 look for a Thunder Dragon in the deck and ask; with none left, shuffle the deck and end.
 *   3       on Yes, add it to the hand and ask again while the count allows; then shuffle and end.
 */
int EffectThunderDragonResolve(struct ChainEntry *link)
{
    char text[0x100];

    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = 2;
            gChain.effectStep--;
            /* fall through */
        case EFFECT_STEP_2: {
            int i;

            for (i = 0; i < gDuelPlayers[1 & link->player].deckCount; i++) {
                u32 word = CARD_WORD(gDuelPlayers[1 & link->player].deck[i]);
                /* FAKEMATCH: the mask in a local (live for three insns) makes loop.c hoist it in its second
                 * pass, after the copy of the table base, as in the ROM. */
                u32 idMask = CARD_ID_MASK;
                u16 number = ID_TO_NUMBER[CARD_ID(word) & idMask];

                if (number == CARD_THUNDER_DRAGON) {
                    FormatStr(text, (const char *)gStrThunderDragonAddPromptFmt,
                              (const char *)(gCardNames + NUMBER_TO_ID[number] * CARD_NAME_SIZE));
                    TextBoxOpen(EFFECT_TEXT_POS, EFFECT_TEXT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
                    TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
                    return EFFECT_STEP_3;
                }
            }
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
            return EFFECT_STEP_END;
        }
        case EFFECT_STEP_3:
            if (gTextBox.result != 0 && AddDeckCardToHand(link->player, CARD_THUNDER_DRAGON) != 0) {
                if (--gChain.effectSubStep != 0)
                    return EFFECT_STEP_2;
            }
            DuelCmd_Push(CMD_FOR(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
            return EFFECT_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* The Immortal of Thunder (461), flip effect: gain 3000 LP and clear the zone's effectUnused flag, which
 * arms the 5000 LP loss for when it leaves the field. */
int EffectTheImmortalOfThunderResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        u16 cmd;

        GainLifePoints(link->player, 3000);
        cmd = CMD_FOR(link->player, DUEL_CMD_SET_EFFECT_UNUSED);
        DuelCmd_Push(cmd, link->zone, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Armed Ninja (468) and De-Spell: destroy the single target unless it is a Trap. A face-down target is
 * flipped face up and shown first; a Trap is then flipped back down.
 */
int EffectDestroyMagicTargetResolve(struct ChainEntry *link)
{
    if (link->negated)
        return EFFECT_STEP_DONE;
    if (link->numTargets != 1)
        return EFFECT_STEP_DONE;
    {
        int player = (u8)link->targets[0];
        int zoneIndex = link->targets[0] >> 8;
        int side = 1 & player;
        struct DuelZone *zone = ZONE_AT(side, zoneIndex);
        u16 id = CARD_ID(CARD_WORD(zone->card));

        if (id == 0)
            return EFFECT_STEP_DONE;
        if (zone->isFaceUp) {
            if (CARD_TYPE(id) != CARD_TYPE_TRAP)
                DestroyFieldCard(player, zoneIndex, 1);
            return EFFECT_STEP_DONE;
        }
        DuelCmd_Push(CMD_FOR(player, DUEL_CMD_FLIP_CARD), zoneIndex, 0, 0);
        ShowRevealedCard(player, id);
        if (CARD_TYPE(id) != CARD_TYPE_TRAP)
            DestroyFieldCard(player, zoneIndex, 1);
        else
            DuelCmd_Push(CMD_FOR(player, DUEL_CMD_FLIP_CARD), zoneIndex, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Needle Ball (489): pay 2000 LP to deal 1000 damage to the opponent. The CPU pays only when the human
 * (player 0) has 999 LP or less and it has more than 2000; the human answers a Yes/No prompt. Does not
 * test link->negated.
 */
int EffectNeedleBallResolve(struct ChainEntry *link)
{
    if (link->player) {
        if (gDuelPlayers[0].lifePoints <= 999 && gDuelPlayers[1].lifePoints > 2000) {
            DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_LOSE_LP, 2000, 1, 0);
            LoseLifePoints(1 - link->player, 1000);
        }
    } else {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            TextBoxOpen(EFFECT_TEXT_POS, EFFECT_TEXT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrNeedleBallPayLpPrompt);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:
            if (gTextBox.result) {
                DuelCmd_Push(DUEL_CMD_LOSE_LP, 2000, 1, 0);
                LoseLifePoints(1 - link->player, 1000);
            }
            return EFFECT_STEP_3;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Yado Karu (496), human player only: return hand cards to the bottom of the deck, one Yes/No prompt each.
 *   START   with a non-empty hand, ask whether to return a card.
 *   2       on Yes, ask for the card.
 *   3       wait for the cursor pick in the hand, return that card and ask again (START).
 */
int EffectYadoKaruResolve(struct ChainEntry *link)
{
    if (!link->negated && !link->player) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            struct DuelPlayer *players = gDuelPlayers;
            int side = 1 & link->player;

            if (players[side].handCount == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(EFFECT_TEXT_POS, EFFECT_TEXT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrYadoKaruReturnPrompt);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_2;
        }
        case EFFECT_STEP_2:
            if (gTextBox.result == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(EFFECT_TEXT_POS, EFFECT_TEXT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrYadoKaruSelectPrompt);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:
            if (DuelCursor_PickTarget(PICK_HAND) != 0) {
                ReturnHandCardToDeck(link->player, gDuelScreen.selIndex, 0);
                return EFFECT_STEP_START;
            }
            return EFFECT_STEP_3;
        }
    }
    return EFFECT_STEP_DONE;
}
