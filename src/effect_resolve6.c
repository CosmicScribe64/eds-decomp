/*
 * effect_resolve6 (0x080361D0-0x0803732B): card effect resolve handlers, part 4: Giant Trunade (1080) to
 * Nimble Momonga (1115), with the handlers several card families share: the six recruiters with ATK 1500
 * or less, Senju of the Thousand Hands / Sonic Bird, Call of the Haunted / Premature Burial, and Giant Germ
 * / Nimble Momonga / key 1307 (wiki/functions/effect-resolve6-c.md).
 *
 * Every function is the resolve slot of a gCardEffects row (include/effect.h). Chain_Resolve calls it as
 * resolve(link, chainedTo) with gChain.effectStep = EFFECT_STEP_START (0x80), stores the return value in
 * gChain.effectStep and calls it again next frame until it returns EFFECT_STEP_DONE (0). Multi-step
 * handlers count their steps down from 0x80 (enum EffectStep) and keep scratch values in
 * gChain.effectSubStep, gChain.effectCounter and gChain.scratch.effect.effectCards. Most handlers do
 * nothing when the activation was negated (link->negated). The CPU (player 1) is not asked the Yes/No
 * questions; the recruiters let AiPickCardListEntry choose its card.
 *
 * Positions: link->targets[] hold player | zone << 8 (DUEL_LOC), or the two halves of a card word
 * (Graverobber, Call of the Haunted, Premature Burial); zones 0-4 hold monsters, 5-9 spells and traps, 10
 * the Field Magic (enum DuelZoneIndex). A duel command carries DUEL_CMD_PLAYER when it acts for player 1.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE, CARD_STATS_SUBTYPE, gCardNames */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, SpellSubtype */
#include "constants/duel.h"         /* enum DuelZoneIndex, ZoneLinkKind, ZoneStatusFlag, ChainEntryKind, ... */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* enum SoundEffect */
#include "legacy/gba.h"                    /* B_BUTTON */
#include "legacy/main.h"                   /* gMain.newKeys */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h (build/readability/hcheck/duel_core/staged/duel.h)
 * that this unit and the headers below use, with its names, types and bitfield containers (unused bytes are
 * padding), and defines duel.h's include guard so that the headers below do not pull in the legacy one.
 * After H0, replace the block (BEGIN to END) with #include "legacy/duel.h" and #include "sound.h"
 * (build/readability/issues/effect_resolve6.md). */
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
    u8 revivedByMonsterReborn:1;    /* +0x07 bit 6 */
    u8 summonedFromGraveyard:1;     /* +0x07 bit 7: ZONE_STATUS_FROM_GRAVEYARD */
    u8 unk8[0x94 - 0x8];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 unk4[0x28 - 0x4];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    u8 unk904[0xD64 - 0x904];
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
extern struct DuelCard gDuelDecks[];            /* 0x02019AA8 = gDuelPlayers[0].deck */

void CopyDuelCard(u32 *dst, u32 *src);
u32 IsSpecialSummonOnly(u16 cardId);
int IsCardInGraveyard(int player, struct DuelCard *card);
int FindTrapInHand(int player);
int FindNonFieldMagicInHand(int player);
int CountFreeMonsterZones(int player);
int FindFreeSpellTrapZone(int player);
u32 GetZoneCardAttribute(s32 player, s32 slot);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* gAiWork.listPick, AiPickCardListEntry */
#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain, Chain_AddPending */
#include "duel_actions.h"           /* field actions: DestroyFieldCard, EquipCard, DiscardHandCard, ... */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_PostData */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* enum EffectStep, prompt texts */
#include "effect_handlers.h"        /* the handlers defined here */
#include "summon.h"                 /* gSummonAction, CanSpecialSummon, QueueSpecialSummon* */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatInt, FormatStr */

/* Local views kept on purpose (matching choices, see build/readability/HEADERS.md). */
/* Matching: EffectPainfulChoicePrepare is defined with one parameter; Painful Choice calls it with
 * (link, chainedTo, 0), which loads r1 and r2 before the call. */
extern int EffectPainfulChoicePrepare3(struct ChainEntry *card, int chainedTo, int fromHand)
    asm("EffectPainfulChoicePrepare");
/* Matching: CollectEffectTargets is defined to return u16; this unit tests the count as an int, with no
 * narrowing of r0. */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: GetGraveyardCardById is defined with a u16 cardId; Graverobber passes an int, without the
 * narrowing (lsl; lsr) that the u16 parameter adds. */
extern int GetGraveyardCardByIdInt(int player, int cardId, struct DuelCard *out) asm("GetGraveyardCardById");
/* Matching: DuelCmd_Push is defined with int arg4/arg6; Dust Tornado calls it with u16 ones (the narrowing
 * changes how the two 0xF masks are loaded). */
extern void DuelCmd_Push16(u16 cmd, u16 arg2, u16 arg4, u16 arg6) asm("DuelCmd_Push");

/* The command id for player: player 1's commands carry DUEL_CMD_PLAYER. */
#define PLAYER_CMD(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* ChainEntry.numTargets read as `7 & byte` (movs #7; ldrb; ands). Matching: where Dust Tornado keeps the
 * count in a local, the bitfield read (lsl #29; lsr #29) gives other code. */
#define LINK_NUM_TARGETS_BYTE(link) (7 & ((u8 *)(link))[0xA])

/* The card word of a zone as one u32. Matching: the ROM loads the whole word (ldr) and extracts the ID
 * with lsl #20; lsr #20. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)
/* The card ID as a table index: bits 0-10 (CARD_ID_MASK) of a card word, lsl #21; lsr #21. */
#define CARD_ID_INDEX(word) (((word) << 21) >> 21)
/* DuelCard.owner (bit 12) and DuelCard.planted (bit 17) of a card word, extracted with shifts. */
#define CARD_WORD_OWNER(word) (((word) << 19) >> 31)
#define CARD_WORD_PLANTED(word) ((int)((word) << 14) < 0)

/* gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4) through integer-constant addresses. Matching:
 * the ROM reloads the table address at each use, which the symbols do not give. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))         /* enum CardType */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define IS_MONSTER_TYPE(type) ((type) <= CARD_TYPE_REPTILE)    /* types 1-20 are monsters */

/* &gDuelZones[player].zones[zone] by byte arithmetic from the gDuelZones literal. Matching: ZONE_AT adds the
 * zone term first, ZONE_AT_PLAYER_FIRST the player term first; each function keeps the ROM's order.
 * Callers pass player & 1. */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
#define ZONE_AT_PLAYER_FIRST(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))

/* CardListView_Open area -1: list the candidates CollectEffectTargets gathers for the card number. */
#define CARDLIST_AREA_EFFECT_TARGETS (-1)

/* Text box rectangles: TextBoxOpen takes x | y << 8 and width | height << 8, in cells. */
#define TEXTBOX_XY(x, y) ((x) | (y) << 8)

/* A Chain_AddPending trigger word: card | kind << 21 | event << 25 | player << 31 (include/chain.h). */
#define CHAIN_TRIGGER(kind, event) ((event) << 25 | (kind) << 21)

/* Giant Trunade (1080): return every Magic and Trap card on the field (zones 5-10) to its owner's hand, the
 * opponent's first. 9 is passed on as arg4 of DUEL_CMD_RETURN_TO_HAND (meaning unknown). */
int EffectGiantTrunadeResolve(struct ChainEntry *link)
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
            for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
                struct DuelZone *z = ZONE_AT(player & 1, zone);

                if (CARD_ID(CARD_WORD(z->card)))
                    ReturnFieldCardToHand(player, zone, 9);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Painful Choice (1081): pick 5 cards from the deck (gChain.effectSubStep counts the picks left and indexes
 * gChain.scratch.effect.effectCards, filled from [4] down to [0]); the opponent chooses one of them
 * (PROMPT_PICK_ONE_OF_FIVE_CARDS). The first card equal to the choice goes to the hand, the others (including
 * duplicates of it) to the graveyard; then the deck is shuffled.
 */
int EffectPainfulChoiceResolve(struct ChainEntry *link, int chainedTo)
{
    u16 cardIds[16];                /* only [0..4] are used */
    u8 text[0x80];
    int i;

    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            if (EffectPainfulChoicePrepare3(link, chainedTo, 0) == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 5), TEXTBOX_FLAGS_DEFAULT, gStrPainfulChoiceSelect5);
            gChain.effectSubStep = 5;
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:         /* open the deck list */
            CardListView_Open(link->player, CARDLIST_AREA_EFFECT_TARGETS, CARD_NUMBER(link->card), 0);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3: {       /* take the picked card out of the deck and keep it */
            u32 *card;

            gChain.effectSubStep--;
            card = &gCardListView.cards[gCardListView.cursorRow + gCardListView.top];
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_REMOVE_CARD_FROM_DECK), ((u16 *)card)[0],
                         ((u16 *)card)[1], 0);
            CopyDuelCard((u32 *)&gChain.scratch.effect.effectCards[gChain.effectSubStep], card);
            if (gChain.effectSubStep != 0) {
                FormatInt((char *)text, (const char *)gStrPainfulChoiceCardsRemaining, gChain.effectSubStep);
                TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 5), TEXTBOX_FLAGS_DEFAULT, text);
                return EFFECT_STEP_2;
            }
            return EFFECT_STEP_4;
        }
        case EFFECT_STEP_4:         /* the opponent chooses one of the five */
            for (i = 0; i <= 4; i++)
                cardIds[i] = CARD_ID(CARD_WORD(gChain.scratch.effect.effectCards[i]));
            DuelPrompt_PostData(1 - link->player, PROMPT_PICK_ONE_OF_FIVE_CARDS, cardIds, 5);
            return EFFECT_STEP_5;
        case EFFECT_STEP_5: {       /* promptResult: the chosen card ID */
            int toHandLeft = 1;

            for (i = 0; i <= 4; i++) {
                u32 *card = (u32 *)&gChain.scratch.effect.effectCards[i];
                u16 *cardHalves = (u16 *)card;

                if (CARD_ID(*card) == gDuel.promptResult && toHandLeft != 0) {
                    sub_08019820(link->player, CARD_ID(*card));
                    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_ADD_CARD_TO_HAND), cardHalves[0],
                                 cardHalves[1], 0);
                    toHandLeft = 0;
                } else {
                    u32 word;
                    u32 cardId;
                    u32 number;

                    ShowDestroyedCard(link->player, CARD_ID(*card));
                    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_ADD_CARD_TO_GRAVEYARD_NO_REDRAW),
                                 cardHalves[0], cardHalves[1], 0);
                    word = *card;
                    cardId = CARD_ID(word);
                    /* Matching: an int-width temporary keeps the compare in SImode, so loop.c does not hoist
                     * the constant. */
                    number = CARD_NUMBER(cardId);
                    if (number == CARD_1242)    /* key 1242: queue its sent-to-the-graveyard trigger */
                        Chain_AddPending(CHAIN_TRIGGER(CHAIN_KIND_OFF_FIELD, RESPONSE_MONSTER_TO_GRAVE)
                                             | (CARD_WORD_OWNER(word) & 1) << 31 | cardId,
                                         0);
                }
            }
            return EFFECT_STEP_END;
        }
        default:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Time Seal (1090): the opponent skips its next Draw Phase. */
int EffectTimeSealResolve(struct ChainEntry *link)
{
    if (!link->negated)
        DuelCmd_Push(PLAYER_CMD(!link->player, DUEL_CMD_SKIP_NEXT_DRAW_PHASE), 0, 0, 0);
    return EFFECT_STEP_DONE;
}

/* Graverobber (1091): take the chosen Magic card (a card word in targets[0..1]) from the opponent's
 * graveyard, if it is still there. */
int EffectGraverobberResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 2) {
        struct DuelCard found;
        int opponent = 1 - link->player;
        int cardId = CARD_ID((u32)link->targets[0]);

        if (GetGraveyardCardByIdInt(opponent, cardId, &found))
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_TAKE_OPPONENT_GRAVEYARD_CARD), cardId, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/* Gift of The Mystical Elf (1092): gain 300 LP for each face-up monster on the field. */
int EffectGiftOfTheMysticalElfResolve(struct ChainEntry *link)
{
    int count = 0;
    /* FAKEMATCH: taking the address of z (never used otherwise) keeps the ROM's loop shape and register
     * allocation. */
    struct DuelZone **zonePtr;

    if (!link->negated) {
        int player;

        for (player = 0; player <= 1; player++) {
            int zone;

            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                struct DuelZone *z = ZONE_AT(player & 1, zone);

                zonePtr = &z;
                if (CARD_ID(CARD_WORD(z->card)) && (*zonePtr)->isFaceUp)
                    count++;
            }
        }
        if (count > 0)
            GainLifePoints(link->player, count * 300);
    }
    return EFFECT_STEP_DONE;
}

/* gDuelPlayers[player].hand[index] as a word, by byte arithmetic from the gDuelHands literal (index term
 * first, as the ROM adds them). */
static inline u32 GetHandCardWord(int player, int index)
{
    int indexOffset = index * 4;
    int playerOffset = player * 0xD64;

    return *(u32 *)(indexOffset + playerOffset + (u32)gDuelHands);
}

/*
 * Dust Tornado (1094): destroy the opponent's Magic/Trap card targets[0]; then the player may set a Magic
 * or Trap card from the hand (not a Field Magic) in a free spell/trap zone. B in the hand-card pick goes
 * back to the Yes/No question.
 */
int EffectDustTornadoResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {   /* destroy the target */
            int numTargets = LINK_NUM_TARGETS_BYTE(link);

            if (numTargets == 1) {
                u8 targetPlayer = link->targets[0];
                int targetZone = link->targets[0] >> 8;
                int side = targetPlayer & 1;
                struct DuelZone *z = ZONE_AT(side, targetZone);

                if (CARD_ID(CARD_WORD(z->card))) {
                    DestroyFieldCard(targetPlayer, targetZone, 1);
                    return EFFECT_STEP_2;
                }
            }
            break;
        }
        case EFFECT_STEP_2: {       /* a Trap or non-Field Magic in the hand and a free zone: ask Yes/No */
            int trapIdx;

            if (gDuelPlayers[link->player].handCount == 0) {
                /* FAKEMATCH: an empty repeated test ends the extended block, so the player bit is shifted
                 * again for FindTrapInHand. */
                if (gDuelPlayers[link->player].handCount) {
                }
                break;
            }
            trapIdx = FindTrapInHand(link->player);
            {
                int none = -1;

                /* FAKEMATCH: the empty asm keeps trapIdx in its register across the second search and
                 * makes -1 be loaded again for the zone test below. */
                asm("" : "+r"(none));
                if (trapIdx == none && FindNonFieldMagicInHand(link->player) == trapIdx)
                    break;
            }
            if (FindFreeSpellTrapZone(link->player) == -1)
                break;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrDustTornadoSetPrompt);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_3;
        }
        case EFFECT_STEP_3:         /* No ends the effect */
            if (gTextBox.result == 0)
                break;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrDustTornadoSelectCards);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:         /* pick the hand card and set it face down */
            if (DuelCursor_PickTarget(PICK_HAND) != 0) {
                u32 word = GetHandCardWord(link->player, gDuelScreen.selIndex);
                u16 cardId = CARD_ID(word);
                int subtype;
                int type;

                if (IS_MONSTER_TYPE(CARD_TYPE(cardId)))
                    goto refuse;
                type = CARD_TYPE(cardId);
                switch (type) {
                case CARD_TYPE_TRAP:
                case CARD_TYPE_MAGIC:
                    subtype = CARD_STATS_SUBTYPE(CARD_STATS(cardId));
                    break;
                default:
                    subtype = SPELL_NORMAL;
                }
                if (subtype == SPELL_FIELD)
                    goto refuse;
                /* arg4: zone | hand index << 4, face down (bit 8 clear) */
                DuelCmd_Push16(PLAYER_CMD(link->player, DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND), cardId,
                               (gDuelScreen.selIndex & 0xF) << 4 | (FindFreeSpellTrapZone(link->player) & 0xF), 0);
                return EFFECT_STEP_END;
            refuse:
                PlaySE(SE_ERROR);
            }
            if (gMain.newKeys & B_BUTTON)
                return EFFECT_STEP_2;
            return EFFECT_STEP_4;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Call of the Haunted (1095) and Premature Burial (1160): Special Summon the chosen graveyard monster (its
 * card word in targets[0..1]) face up in Attack Position, then link the card to it: Call of the Haunted with
 * a ZONE_LINK_CONTINUOUS link, Premature Burial as an equip. Needs the card still in its zone and the
 * monster still in the graveyard. The count of the player's monsters with the same ID summoned from the
 * graveyard, kept in gChain.effectSubStep, is not read here.
 */
int EffectReviveFromGraveyardResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 2) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            int side = link->player & 1;
            struct DuelZone *z = ZONE_AT(side, link->zone);
            u32 card;
            int zone;

            if (CARD_ID(CARD_WORD(z->card)) == 0)      /* the card has left its zone */
                return EFFECT_STEP_DONE;
            card = link->targets[1] << 16 | link->targets[0];
            if (IsCardInGraveyard(link->player, (struct DuelCard *)&card) == 0)
                return EFFECT_STEP_DONE;
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD), link->targets[0],
                         link->targets[1], 0);
            QueueSpecialSummon(link->player, (struct DuelCard *)&card, 1, 0, ZONE_STATUS_FROM_GRAVEYARD);
            gChain.effectSubStep = 0;
            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                if (CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(link->player & 1, zone)->card)) == link->targets[0]
                    && ZONE_AT_PLAYER_FIRST(link->player & 1, zone)->summonedFromGraveyard)
                    gChain.effectSubStep++;
            }
            return EFFECT_STEP_2;
        }
        case EFFECT_STEP_2:         /* link the card to the zone the summon landed in (gSummonAction.zone) */
            switch (CARD_NUMBER(link->card)) {
            case CARD_CALL_OF_THE_HAUNTED:
                QueueAddZoneLink(link->player, link->player | link->zone << 8,
                                 link->player | gSummonAction.zone << 8, ZONE_LINK_CONTINUOUS);
                break;
            case CARD_PREMATURE_BURIAL:
                EquipCard(link->player, link->player | link->zone << 8,
                          link->player | gSummonAction.zone << 8);
                break;
            }
            return EFFECT_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Solomon's Lawbook (1096): the player skips their next Standby Phase. */
int EffectSolomonsLawbookResolve(struct ChainEntry *link)
{
    if (!link->negated)
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SKIP_NEXT_STANDBY_PHASE), 0, 0, 0);
    return EFFECT_STEP_DONE;
}

/* Earthshaker (1097): destroy every face-up monster of the attribute the opponent picked (targets[0], enum
 * CardAttribute), the opponent's first. */
int EffectEarthshakerResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
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

                if (CARD_ID(CARD_WORD(z->card)) && z->isFaceUp
                    && GetZoneCardAttribute(player, zone) == link->targets[0]) {
                    DestroyFieldCardByEffect(player, zone);
                    OnCardDestroyedByEffect(link->player, player, zone);
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Matching: the (u8) narrowing makes the AND take the mask register as its first operand, so the constant
 * 1 stays one QImode pseudo shared by both player lookups and `1 - side` below loads a fresh 1. */
static inline int PlayerSide(int player)
{
    return (u8)player & 1;
}

/* Level of a card: 0 for Trap, Magic and Ticket cards, 10 for the Divine cards, else gCardStats bits 25-28. */
static inline u32 GetCardLevel(int type, int cardId)
{
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS(cardId));
    }
}

/*
 * Cyber Jar (1106): destroy every monster; then each player, the turn player first, draws the top 5 cards
 * of the deck one at a time and reveals them: a monster of level 4 or lower that can be Normal Summoned is
 * Special Summoned (the player picks the position), the other cards stay in the hand. A planted Parasite
 * Paracide of the other player is Special Summoned face up on the other player's side if there is a free
 * zone, else it is discarded. An empty deck ends the whole effect (EFFECT_STEP_9 has no case).
 * gChain.effectSubStep is the side, gChain.effectCounter the cards left for it.
 */
int EffectCyberJarResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            int player;

            for (player = 0; player <= 1; player++) {
                int zone;

                for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                    DestroyFieldCardByEffect(player, zone);
                    OnCardDestroyedByEffect(link->player, player, zone);
                }
            }
            gChain.effectSubStep = gDuel.turnPlayer;
            gChain.effectCounter = 5;
            return EFFECT_STEP_2;
        }
        case EFFECT_STEP_2: {       /* draw and reveal the top card of the side's deck */
            u32 *top;
            u16 *topHalves;
            u32 word;
            int cardId;

            if (gDuelPlayers[PlayerSide(gChain.effectSubStep)].deckCount == 0)
                return EFFECT_STEP_9;
            top = (u32 *)gDuelPlayers[PlayerSide(gChain.effectSubStep)].deck;
            topHalves = (u16 *)top;     /* Matching: a second copy of the pointer (the ROM keeps it in r8) */
            DuelCmd_Push(PLAYER_CMD(gChain.effectSubStep, DUEL_CMD_DRAW_CARDS), 1, 1, 0);
            ShowRevealedCard(gChain.effectSubStep, CARD_ID(*top));
            word = *top;
            if (CARD_WORD_OWNER(word) != gChain.effectSubStep && CARD_WORD_PLANTED(word)
                && CARD_NUMBER(CARD_ID_INDEX(word)) == CARD_PARASITE_PARACIDE) {
                if (CountFreeMonsterZones(1 - gChain.effectSubStep) > 0) {
                    DuelCmd_Push(PLAYER_CMD(gChain.effectSubStep, DUEL_CMD_REMOVE_CARD_FROM_HAND), topHalves[0],
                                 topHalves[1], 0);
                    CopyDuelCard((u32 *)&gChain.scratch.effect.effectCards[0], top);
                    return EFFECT_STEP_4;
                }
                /* the drawn card lands at hand index handCount (the draw command has not run yet) */
                DiscardHandCard(gChain.effectSubStep, gDuelPlayers[PlayerSide(gChain.effectSubStep)].handCount,
                                0, 1);
            } else {
                u32 type;

                cardId = CARD_ID(*top);
                type = CARD_TYPE(cardId);
                if (IS_MONSTER_TYPE(type) && GetCardLevel(type, cardId) <= 4 && IsSpecialSummonOnly(cardId) == 0) {
                    DuelCmd_Push(PLAYER_CMD(gChain.effectSubStep, DUEL_CMD_REMOVE_CARD_FROM_HAND), topHalves[0],
                                 topHalves[1], 0);
                    CopyDuelCard((u32 *)&gChain.scratch.effect.effectCards[0], top);
                    return EFFECT_STEP_3;
                }
            }
            return EFFECT_STEP_5;
        }
        case EFFECT_STEP_3:         /* Special Summon the monster for the side */
            QueueSpecialSummonChoosePosition(gChain.effectSubStep, &gChain.scratch.effect.effectCards[0], 0, 0);
            return EFFECT_STEP_5;
        case EFFECT_STEP_4:         /* Special Summon Parasite Paracide face up for the other side */
            QueueSpecialSummonChoosePosition(1 - gChain.effectSubStep, &gChain.scratch.effect.effectCards[0], 1, 0);
            return EFFECT_STEP_5;
        case EFFECT_STEP_5:         /* next card; after 5, the other side; done when back at the turn player */
            if (--gChain.effectCounter == 0) {
                gChain.effectSubStep = 1 - gChain.effectSubStep;
                gChain.effectCounter = 5;
                /* FAKEMATCH: the memory clobber makes effectSubStep be read again after the effectCounter
                 * store, as in the ROM. */
                asm volatile("" ::: "memory");
                if (gChain.effectSubStep == gDuel.turnPlayer)
                    return EFFECT_STEP_9;
            }
            return EFFECT_STEP_2;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Giant Rat, UFO Turtle, Shining Fairy, Mother Grizzly, Flying Kamakiri #1, Mystic Tomato: Special Summon a
 * deck monster that CollectEffectTargets accepts for the card (ATK 1500 or less, of the card's attribute),
 * then shuffle. The human is asked first and picks in the card-list viewer; the CPU skips to the summon
 * with the entry AiPickCardListEntry chose.
 */
int EffectSpecialSummonFromDeckResolve(struct ChainEntry *link)
{
    u32 *card = &gCardListView.cards[gCardListView.cursorRow + gCardListView.top];

    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            if (CanSpecialSummon(link->player) == 0)
                return EFFECT_STEP_DONE;
            if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) == 0) {
                if (link->player)
                    return EFFECT_STEP_DONE;
                TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrRecruiterNoCardsInDeck);
                return EFFECT_STEP_END;
            }
            if (link->player) {   /* the CPU: its pick becomes the selected list entry */
                if (AiPickCardListEntry(link->card) < 0)
                    return EFFECT_STEP_DONE;
                gCardListView.cursorRow = 0;
                gCardListView.top = gAiWork.listPick;
                return EFFECT_STEP_4;
            }
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrRecruiterSummonPrompt);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:         /* No ends the effect */
            if (gTextBox.result == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrRecruiterSelectMonster);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:         /* open the list of candidates */
            CardListView_Open(link->player, CARDLIST_AREA_EFFECT_TARGETS, CARD_NUMBER(link->card), 0);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:         /* take the chosen card out of the deck */
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_REMOVE_CARD_FROM_DECK), ((u16 *)card)[0],
                         ((u16 *)card)[1], 0);
            return EFFECT_STEP_5;
        case EFFECT_STEP_5:         /* Special Summon it face up in Attack Position */
            QueueSpecialSummon(link->player,
                               (struct DuelCard *)&gCardListView.cards[gCardListView.cursorRow + gCardListView.top],
                               1, 0, 0);
            return EFFECT_STEP_6;
        case EFFECT_STEP_6:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
            return EFFECT_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Senju of the Thousand Hands (1109) and Sonic Bird (1122): add a Ritual Monster (Senju) or a Ritual Magic
 * card (Sonic Bird) from the deck to the hand, picked in the card-list viewer. */
int EffectAddRitualCardToHandResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:     /* candidates in the deck: ask Yes/No */
            if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) == 0)
                return EFFECT_STEP_DONE;
            switch (CARD_NUMBER(link->card)) {
            case CARD_SENJU_OF_THE_THOUSAND_HANDS:
                TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT,
                            gStrSenjuAddRitualMonsterPrompt);
                break;
            case CARD_SONIC_BIRD:
                TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT,
                            gStrSonicBirdAddRitualMagicPrompt);
                break;
            default:
                return EFFECT_STEP_DONE;
            }
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:         /* No ends the effect */
            if (gTextBox.result == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrRitualSearchSelectCard);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:
            CardListView_Open(link->player, CARDLIST_AREA_EFFECT_TARGETS, CARD_NUMBER(link->card), 0);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:         /* add the chosen card (by its card number; gCardIdToNumber, unmasked) */
            AddDeckCardToHand(link->player, ((const u16 *)0x08622AB4)[CARD_ID_INDEX(
                                                gCardListView.cards[gCardListView.cursorRow + gCardListView.top])]);
            return EFFECT_STEP_5;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Karate Man (1112) and key 1254: use up the card's once-per-turn effect (DUEL_CMD_SET_EFFECT_UNUSED 0);
 * GetZoneCardStats applies the ATK change while the flag is clear. */
int EffectKarateManResolve(struct ChainEntry *link)
{
    if (!link->negated)
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
    return EFFECT_STEP_DONE;
}

/*
 * Giant Germ (1114), Nimble Momonga (1115) and key 1307: first the LP effect (Giant Germ: the opponent loses
 * 500 LP; Nimble Momonga: the player gains 1000 LP), then Special Summon further copies of the card from the
 * deck, one Yes/No question each (the CPU always answers Yes): Giant Germ face up in Attack Position, the
 * others face down in Defense Position. Key 1307 stops after one copy: it returns step 0x0A, which has no
 * case and ends the effect through the default case (shuffle).
 */
int EffectSummonSameNameFromDeckResolve(struct ChainEntry *link)
{
    u8 text[0x80];
    u32 removed;

    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            switch (CARD_NUMBER(link->card)) {
            case CARD_GIANT_GERM:
                LoseLifePoints(1 - link->player, 500);
                break;
            case CARD_NIMBLE_MOMONGA:
                GainLifePoints(link->player, 1000);
                break;
            }
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:         /* another copy in the deck: ask Yes/No */
            if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) == 0)
                return EFFECT_STEP_DONE;
            if (link->player) {
                gTextBox.result = 1;
                return EFFECT_STEP_3;
            }
            switch (CARD_NUMBER(link->card)) {
            case CARD_GIANT_GERM:
                /* Matching: gCardNames through its integer address here, through the symbol below. */
                FormatStr((char *)text, (const char *)gStrGiantGermSummonPrompt,
                          (const char *)((u8 *)0x0822C720 + link->card * CARD_NAME_SIZE));
                break;
            case CARD_NIMBLE_MOMONGA:
            case CARD_1307:
                FormatStr((char *)text, (const char *)gStrSameNameSetPrompt,
                          (const char *)((u8 *)gCardNames + link->card * CARD_NAME_SIZE));
                break;
            }
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:         /* Yes: take the next copy out of the deck */
            if (gTextBox.result != 0) {
                int deckIdx = RemoveDeckCardByNumber(link->player, CARD_NUMBER(link->card), &removed);

                if (deckIdx >= 0) {
                    /* the removal is only queued, so deck[deckIdx] still holds the card */
                    u32 *dst = (u32 *)&gChain.scratch.effect.effectCards[0];
                    struct DuelCard *src = (struct DuelCard *)((u32)gDuelDecks + link->player * 0xD64);

                    src = (struct DuelCard *)((u32)src + deckIdx * 4);
                    CopyDuelCard(dst, (u32 *)src);
                    return EFFECT_STEP_4;
                }
            }
            goto shuffle;
        case EFFECT_STEP_4:         /* summon it, then ask again */
            switch (CARD_NUMBER(link->card)) {
            case CARD_GIANT_GERM:
                QueueSpecialSummon(link->player, &gChain.scratch.effect.effectCards[0], 1, 0, 0);
                break;
            case CARD_NIMBLE_MOMONGA:
                QueueSpecialSummon(link->player, &gChain.scratch.effect.effectCards[0], 0, 1, 0);
                break;
            case CARD_1307:
                QueueSpecialSummon(link->player, &gChain.scratch.effect.effectCards[0], 0, 1, 0);
                return 0xA;
            }
            return EFFECT_STEP_2;
        default:
        shuffle:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
        }
    }
    return EFFECT_STEP_DONE;
}
