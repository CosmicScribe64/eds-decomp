#include "global.h"
#include "legacy/gba.h"                    /* B_BUTTON */
#include "legacy/main.h"                   /* gMain.newKeys, gMain.frameCounter */
#include "util.h"                   /* MemCopy16, FormatStr */
#include "sprite.h"                 /* AddAffineSprite, enum SpriteShape */
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_ALT_ART, CARD_NAME_SIZE, CARD_STATS_* */
#include "link.h"                   /* LinkQueueMessage */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "duel_flow.h"              /* gDuelCtrl, gPulseScaleCurve, the dead-code sub_ functions here */
#include "duel_prompt.h"            /* the DuelPrompt_* API and the prompt handlers */
#include "duel_actions.h"           /* DiscardHandCard, sub_080197C0 */
#include "constants/cards.h"        /* CARD_INSPECTION, CARD_1517, CARD_1519, CARD_1520 */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum DuelPromptKind, enum FieldPickMask */
#include "constants/sound.h"        /* SE_ERROR */

/*
 * Duel prompts and the duel link send helpers (wiki/functions/duel-prompts-c.md).
 *
 * The duel has one pending prompt at gDuel +0x1B50: a question or pick that one player must answer before
 * the duel goes on. Card effects post it (DuelPrompt_Post and the DuelPrompt_Post* wrappers), DuelMainStep
 * runs it every frame through DuelPrompt_Run, which calls the handler of gDuel.promptKind until it returns
 * 1, and the effect reads the answer from gDuel.promptResult. In a link duel a prompt for player 1 is
 * forwarded to the partner (LINKMSG_PROMPT), and this side waits for its LINKMSG_PROMPT_RESULT.
 *
 * This unit holds six of the handlers (the Yes/No confirmations, the Magic discard offer and the
 * replacement attack-target picks; duel_prompt.h lists the others, which live in duel_prompt_handlers.c,
 * duel_turn_end.c and summon_checks.c), and the senders of the link protocol: the short message, the
 * message with a payload, and the five card-list messages. Five functions are dead code (no caller and no
 * pointer in the ROM).
 */

/* ---- BEGIN pre-H0 subset of duel.h, sound.h and gba.h ---- */
/*
 * The part of those headers this unit uses, with their names, types and bitfield containers (unused bytes
 * are padding). include/duel.h, sound.h and gba.h still hold the legacy headers until the header switch
 * (H0, build/readability/HEADERS.md), and duel_link.h, duel_screen.h and summon.h include duel.h, so this
 * block also defines duel.h's include guard. After H0, replace this block (BEGIN to END) with
 *     #include "legacy/duel.h"
 *     #include "legacy/sound.h"
 * (gba.h is already included above). Checked: the unit compiles to the same assembly both ways.
 */
#define GUARD_DUEL_H

#ifndef OAM_ATTR2_PALETTE
#define OAM_ATTR2_PALETTE(n)        ((n) << 12) /* 16-colour palette 0..15 */
#endif

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:1;
    u32 unk14:1;
    u32 normalSummoned:1;           /* bit 15 */
    u32 specialSummoned:1;          /* bit 16 */
    u32 planted:1;                  /* bit 17: Parasite Paracide shuffled into the other player's deck */
    u32 graverobbed:1;              /* bit 18: taken with Graverobber */
    u32 unk19:13;
};

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
    u8 unk4[0x94 - 4];              /* not used here; chain.h embeds a whole zone */
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 banishedCount;               /* +0x006: entries in banished[] */
    u8 unk7[0x684 - 0x7];
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4 */
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B14 - 0x1ACC];
    u16 unk1B14_0:1;
    u16 interruptActive:1;          /* +0x1B14 bit 1 */
    u16 unk1B14_2:7;                /* +0x1B14 bits 2-8 */
    u8 unk1B16[0x1B43 - 0x1B16];
    u8 unk1B43;                     /* +0x1B43: only cleared (by dead code) */
    u8 unk1B44;                     /* +0x1B44: only cleared (by dead code) */
    u8 unk1B45[11];
    u8 promptLinked:1;              /* +0x1B50 bit 0: the prompt is mirrored over the link */
    u8 promptActive:1;              /* +0x1B50 bit 1: a prompt is pending */
    u8 promptPlayer:1;              /* +0x1B50 bit 2: player who answers */
    u8 unk1B50_3:1;
    u16 promptKind:6;               /* +0x1B50 bits 4-9: enum DuelPromptKind */
    u16 unk1B51_2:6;
    u16 promptArgs[8];              /* +0x1B52: [0] argument, [1] value (DuelPrompt_Post); all 8 for PostData */
    u8 promptStep;                  /* +0x1B62: step of the running prompt handler */
    u8 unk1B63;
    u16 promptResult;               /* +0x1B64: the answer; the link partner's 16-byte result is copied here */
    u8 unk1B66[0x1B78 - 0x1B66];
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */

void CopyDuelCard(u32 *dst, u32 *src);
u32 IsSpecialSummonOnly(u16 cardId);
void PlaySE(u32 seId);
/* ---- END pre-H0 subset ---- */

#include "duel_link.h"              /* gLinkState, gLinkTxCards, LINKMSG_*, DuelLink_* */
#include "duel_screen.h"            /* gDuelScreen.selIndex, DuelCursor_PickTarget, GetCardIconObjTile */
#include "summon.h"                 /* CanSummonFromHand */

/*
 * Local views kept for matching:
 * - DuelPrompt_PickOneOfFiveCards is defined (void), but DuelPrompt_Run passes it promptArgs[0] and [1]:
 *   the ROM loads both into r0/r1 before the call.
 */
int DuelPrompt_PickOneOfFiveCardsArgs(u16 arg, u16 value) asm("DuelPrompt_PickOneOfFiveCards");

/* ROM data used only here: the prompt texts. */
extern const u8 gStrPromptPayToViewHand[];              /* "... pay 500LP to view your opponent's hand?" */
extern const u8 gStrPromptChangeOpponentPosition[];     /* "... change the position of your opponent's
                                                         * face-up monster ?" */
extern const u8 gStrPromptDiscardMagic[];               /* "... discard a Magic Card from your hand ?" */
extern const u8 gStrPromptSelectMagicToDiscard[];       /* "Please select the Magic Card you wish to discard
                                                         * from your hand." */
extern const char gStrPromptSpecialSummonFmt[];         /* "Do you wish to Special-Summon %s to the field?" */
extern const char gStrPromptOpponentSpecialSummonedFmt[]; /* "Your opponent has Special Summoned %s. Do you
                                                         * wish to Special-Summon a monster from your
                                                         * Graveyard?" */
extern const u8 gStrPromptSelectOwnReplacementTarget[]; /* "As a replacement, select one of your monsters
                                                         * as a target for attack." */
extern const u8 gStrPromptSelectOpponentReplacementTarget[]; /* same, "... of your opponent's monsters ..." */

/* gCardNumberToId[CARD_1520] (0 in EDS) through its own address-suffixed symbol, as the ROM loads it. */
extern u16 gUnk_086249D4;

/* gCardNumberToId (0x08623DF4) and gCardStats (0x08621DE0) through their integer addresses: the ROM's
 * register allocation needs these forms (integer-constant address, not the symbol). */
#define CARD_NUMBER_TO_ID_TABLE ((const u16 *)0x08623DF4)
#define CARD_STATS_TABLE        ((const u32 *)0x08621DE0)

/* The text box of every prompt here: at cell (6, 2), 18 x 7 cells (19 x 7 for the long texts). */
#define PROMPT_BOX_POS          0x206
#define PROMPT_BOX_SIZE         0x712
#define PROMPT_BOX_SIZE_WIDE    0x713

/* The card word as one u32 (the ROM loads the whole word with ldr for these tests). */
#define CARD_WORD(card)         (*(u32 *)&(card))
/* Card ID bits 0-11 of a card word (lsl #20; lsr #20), and bits 0-10, the ID & CARD_ID_MASK (lsl #21;
 * lsr #21). Where the ROM uses these forms, card.id gives other code. */
#define CARD_WORD_ID(word)      (((word) << 20) >> 20)
#define CARD_WORD_ID11(word)    (((word) << 21) >> 21)

/* CopyDuelCard takes u32 *; the piles are struct DuelCard arrays. */
#define COPY_CARD(dst, src)     CopyDuelCard((u32 *)(dst), (u32 *)(src))

/*
 * FAKEMATCH: the hand counts are read as an array behind a cast pointer to gDuel.players; agbcc then loads
 * the 0x020192E4 base before computing the index (the loop-bottom tests), unlike gDuel.players[k].
 */
struct DuelPlayerPair { struct DuelPlayer p[2]; };
#define PLAYERS_VIA_PTR (((struct DuelPlayerPair *)gDuel.players)->p)

/* Card number to card ID (2000 + n is the alternate art of card n, whose ID is n's ID + 1); 0xFFFF maps to
 * 0 (the same inline as in title_screen.c). */
static inline u16 CardNumberToId(u16 cardNumber)
{
    if (cardNumber == 0xFFFF)
        return 0;
    if (cardNumber < CARD_NUMBER_ALT_ART)
        return *(CARD_NUMBER_TO_ID_TABLE + (cardNumber & CARD_ID_MASK));
    return *(CARD_NUMBER_TO_ID_TABLE + ((cardNumber - CARD_NUMBER_ALT_ART) & CARD_ID_MASK)) + 1;
}

/* Monster level of a card ID: 0 for Trap, Magic and Ticket cards, 10 for the Divine cards, else stats
 * bits 25-28. */
static inline u32 GetCardLevel(u16 cardId)
{
    int type = CARD_STATS_TYPE(CARD_STATS_TABLE[cardId & CARD_ID_MASK]);

    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS_TABLE[cardId & CARD_ID_MASK]);
    }
}

/* Dead code: returns 0. */
u32 sub_08021CC8(void)
{
    return 0;
}

/* Dead code: clears gDuel bytes +0x1B43 and +0x1B44, which nothing reads. */
void sub_08021CCC(void)
{
    gDuel.unk1B43 = 0;
    gDuel.unk1B44 = 0;
}

/*
 * Dead code: draws two 32x32 card icons of cardId at (0x50, 0x40) and (0xA0, 0x40), OBJ palette 1. The
 * right one is turned a quarter turn (angle 0x20 of 128) and shows OBJ tile 0x40 unless showSecond. The
 * selected one (0 = left) pulses with gPulseScaleCurve (one step every 2 frames); the other keeps scale
 * 0x100. The last AddAffineSprite argument is scale << 16 | angle.
 */
void sub_08021CEC(u16 cardId, u16 showSecond, u16 selected)
{
    u16 tile;

    tile = GetCardIconObjTile(cardId) | OAM_ATTR2_PALETTE(1);
    AddAffineSprite(0x00400050, SPRITE_SHAPE_32x32, tile,
                    selected == 0 ? gPulseScaleCurve[(gMain.frameCounter & 0x1E) >> 1] << 16 : 0x01000000);
    if (showSecond)
        tile = GetCardIconObjTile(cardId) | OAM_ATTR2_PALETTE(1);
    else
        tile = 0x40;
    AddAffineSprite(0x004000A0, SPRITE_SHAPE_32x32, tile,
                    selected ? (gPulseScaleCurve[(gMain.frameCounter & 0x1E) >> 1] << 16) | 0x20 : 0x01000020);
}

/*
 * PROMPT_CONFIRM_CARD_EFFECT: show the card's picture, then ask its Yes/No question: Inspection, pay
 * 500 LP to view the opponent's hand; keys 1517 and 1519 (no EDS card), change the position of an opponent's
 * face-up monster. promptResult = the answer (nonzero = Yes). Returns 1 when finished.
 */
u32 DuelPrompt_ConfirmCardEffect(int player, u16 cardNumber)
{
    switch (gDuel.promptStep) {
    case 0:
        sub_080197C0(player, CardNumberToId(cardNumber));
        gDuel.promptStep++;
        return 0;
    case 1:
        switch (cardNumber) {
        case CARD_INSPECTION:
            TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrPromptPayToViewHand);
            break;
        case CARD_1517:
        case CARD_1519:
            TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrPromptChangeOpponentPosition);
            break;
        }
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        gDuel.promptStep++;
        return 0;
    default:
        gDuel.promptResult = gTextBox.result;
        return 1;
    }
}

/*
 * PROMPT_OFFER_DISCARD_MAGIC: offer to discard a Magic card from the hand; promptResult = 1 if one was
 * discarded. Nothing happens unless the hand holds a Magic card that is not graverobbed. The CPU (any
 * player but 0) then discards its first Magic card at once (that search skips the graverobbed test). The
 * human answers a Yes/No question, then picks the card with the hand cursor: B goes back to the question,
 * and a pick that is not a Magic card, or is planted or graverobbed, plays the error sound. Returns 1 when
 * finished.
 */
u32 DuelPrompt_OfferDiscardMagic(int player)
{
    int i;

    switch (gDuel.promptStep) {
    case 0:
        gDuel.promptResult = 0;
        for (i = 0; i < PLAYERS_VIA_PTR[player & 1].handCount; i++) {
            struct DuelCard *card = &gDuel.players[player & 1].hand[i];
            if (CARD_STATS_TYPE(CARD_STATS_TABLE[CARD_WORD_ID11(CARD_WORD(*card))]) == CARD_TYPE_MAGIC
                && !card->graverobbed) {
                if (player) {
                    /* FAKEMATCH: makes player & 1 loop-variant so it is recomputed every pass, as in the ROM */
                    asm("" : "+r"(player));
                    gDuel.promptResult = 1;
                    i = 0;
                    /* FAKEMATCH: lengthens i's live range by one insn, so the 0xD64 constant is allocated
                     * first (r3) and i gets r4 */
                    asm("");
                    for (; i < PLAYERS_VIA_PTR[1].handCount; i++) {
                        if (CARD_STATS_TYPE(CARD_STATS_TABLE[CARD_WORD_ID(CARD_WORD(gDuel.players[1].hand[i]))
                                                             & CARD_ID_MASK]) == CARD_TYPE_MAGIC) {
                            DiscardHandCard(1, i, 1, 1);
                            return 1;
                        }
                    }
                    gDuel.promptResult = 0;
                    return 1;
                }
                TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrPromptDiscardMagic);
                TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
                gDuel.promptStep++;
                return 0;
            }
        }
        return 1;
    case 1:
        if (gTextBox.result == 0) {
            gDuel.promptResult = 0;
            return 1;
        }
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectMagicToDiscard);
        gDuel.promptStep++;
        return 0;
    default:
        if (gMain.newKeys & B_BUTTON) {
            gDuel.promptStep = 0;
            return 0;
        }
        if (DuelCursor_PickTarget(PICK_HAND)) {
            u32 handIdx = gDuelScreen.selIndex;
            struct DuelCard *card = &gDuel.players[0].hand[handIdx];
            if (CARD_STATS_TYPE(CARD_STATS_TABLE[card->id & CARD_ID_MASK]) == CARD_TYPE_MAGIC) {
                int ok = 1;
                if (card->planted)
                    ok = 0;
                if (card->graverobbed)
                    ok = 0;
                if (ok) {
                    gDuel.promptResult = 1;
                    DiscardHandCard(0, handIdx, 1, 1);
                    return 1;
                }
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    }
}

/*
 * PROMPT_CONFIRM_SPECIAL_SUMMON: 'Do you wish to Special-Summon <card> to the field?' (Yes/No).
 * promptResult = the answer; cardId 0 finishes at once with 0. Returns 1 when finished.
 */
u32 DuelPrompt_ConfirmSpecialSummon(int unusedPlayer, u16 cardId)
{
    char buf[0x80];

    if (gDuel.promptStep == 0) {
        gDuel.promptResult = 0;
        if (cardId == 0)
            return 1;
        FormatStr(buf, gStrPromptSpecialSummonFmt, (const char *)&gCardNames[cardId * CARD_NAME_SIZE]);
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE_WIDE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)buf);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        gDuel.promptStep++;
        return 0;
    }
    gDuel.promptResult = gTextBox.result;
    return 1;
}

/*
 * PROMPT_CONFIRM_GRAVEYARD_SUMMON: 'Your opponent has Special Summoned <card>. Do you wish to Special-Summon
 * a monster from your Graveyard?' (Yes/No); promptResult = the answer. The card is card number 1520 (no
 * EDS card). Original bug: the name is formatted into buf, but the box shows the raw format string.
 * Returns 1 when finished.
 */
u32 DuelPrompt_ConfirmGraveyardSummon(int unusedPlayer)
{
    char buf[0x100];

    if (gDuel.promptStep != 0) {
        gDuel.promptResult = gTextBox.result;
        return 1;
    }
    gDuel.promptResult = 0;
    FormatStr(buf, gStrPromptOpponentSpecialSummonedFmt,
              (const char *)&gCardNames[gUnk_086249D4 * CARD_NAME_SIZE]);
    TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE_WIDE, TEXTBOX_FLAGS_DEFAULT,
                (const u8 *)gStrPromptOpponentSpecialSummonedFmt);   /* the format, not buf */
    TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
    gDuel.promptStep++;
    return 0;
}

/*
 * PROMPT_SELECT_OWN_REPLACEMENT_TARGET: player 0 picks one of their own monsters (other than currentSlot)
 * as the new attack target; promptResult = its zone. Every pick plays the error sound, even a valid one.
 * Returns 1 when finished.
 */
u32 DuelPrompt_SelectOwnReplacementTarget(int unusedPlayer, u32 currentSlot)
{
    switch (gDuel.promptStep) {
    case 0:
        gDuel.promptResult = 0;
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE_WIDE, TEXTBOX_FLAGS_DEFAULT,
                    gStrPromptSelectOwnReplacementTarget);
        gDuel.promptStep++;
        break;
    case 1:
        if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
            if ((u32)gDuelScreen.selIndex != currentSlot) {
                gDuel.promptResult = gDuelScreen.selIndex;
                gDuel.promptStep++;
            }
            PlaySE(SE_ERROR);
        }
        break;
    default:
        return 1;
    }
    return 0;
}

/* PROMPT_SELECT_OPPONENT_REPLACEMENT_TARGET: as above, on player 1's monster row. */
u32 DuelPrompt_SelectOpponentReplacementTarget(int unusedPlayer, u32 currentSlot)
{
    switch (gDuel.promptStep) {
    case 0:
        gDuel.promptResult = 0;
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_SIZE_WIDE, TEXTBOX_FLAGS_DEFAULT,
                    gStrPromptSelectOpponentReplacementTarget);
        gDuel.promptStep++;
        break;
    case 1:
        if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_ANY_MONSTER))) {
            if ((u32)gDuelScreen.selIndex != currentSlot) {
                gDuel.promptResult = gDuelScreen.selIndex;
                gDuel.promptStep++;
            }
            PlaySE(SE_ERROR);
        }
        break;
    default:
        return 1;
    }
    return 0;
}

/*
 * Run the pending prompt (called every frame by DuelMainStep); 1 while it is still running, 0 when there is
 * none or it has just finished. A prompt for player 1 that DuelPrompt_Start forwarded to the link partner
 * only waits for the partner's LINKMSG_PROMPT_RESULT; any other runs its handler with (promptPlayer,
 * promptArgs[0], promptArgs[1]). A prompt received from the partner (linked, for player 0 here) sends its
 * 16-byte result back. An unknown kind returns 0 and leaves the prompt active.
 */
u32 DuelPrompt_Run(void)
{
    u16 done;

    if (!gDuel.promptActive)
        return 0;
    if (gDuel.promptLinked && gDuel.promptPlayer) {
        done = gLinkState.promptResultReceived;
    } else {
        switch (gDuel.promptKind) {
        case PROMPT_DISCARD:
            done = DuelPrompt_Discard(gDuel.promptPlayer, gDuel.promptArgs[0], gDuel.promptArgs[1] & 1,
                                      gDuel.promptArgs[1] & 2);
            break;
        case PROMPT_DISCARD_COST:
            done = DuelPrompt_DiscardCost(gDuel.promptPlayer, gDuel.promptArgs[0], gDuel.promptArgs[1] & 1,
                                          gDuel.promptArgs[1] & 2);
            break;
        case PROMPT_DISCARD_RANDOM:
            done = DuelPrompt_DiscardRandom(gDuel.promptPlayer, gDuel.promptArgs[0], gDuel.promptArgs[1]);
            break;
        case PROMPT_BANISH_RANDOM:
            done = DuelPrompt_BanishRandom(gDuel.promptPlayer, gDuel.promptArgs[0], gDuel.promptArgs[1]);
            break;
        case PROMPT_BANISH_RANDOM_FACE_DOWN:
            done = DuelPrompt_BanishRandomFaceDown(gDuel.promptPlayer, gDuel.promptArgs[0], gDuel.promptArgs[1]);
            break;
        case PROMPT_PICK_OPPONENT_HAND_CARD:
            done = DuelPrompt_PickOpponentHandCard(gDuel.promptPlayer);
            break;
        case PROMPT_TRIBUTE:
            done = DuelPrompt_Tribute(gDuel.promptPlayer);
            break;
        case PROMPT_SELECT_TYPE:
            done = DuelPrompt_SelectType();
            break;
        case PROMPT_SELECT_ATTRIBUTE:
            done = DuelPrompt_SelectAttribute();
            break;
        case PROMPT_SELECT_TWO_ATTRIBUTES:
            done = DuelPrompt_SelectTwoAttributes();
            break;
        case PROMPT_PICK_ONE_OF_TWO_ATTRIBUTES:
            done = DuelPrompt_PickOneOfTwoAttributes();
            break;
        case PROMPT_PICK_ONE_OF_FIVE_CARDS:
            done = DuelPrompt_PickOneOfFiveCardsArgs(gDuel.promptArgs[0], gDuel.promptArgs[1]);
            break;
        case PROMPT_SET_MONSTER_FROM_HAND:
            done = DuelPrompt_SetMonsterFromHand(gDuel.promptPlayer);
            break;
        case PROMPT_SELECT_GRAVEYARD_MONSTER:
            done = DuelPrompt_SelectGraveyardMonster(gDuel.promptPlayer, gDuel.promptArgs[0], gDuel.promptArgs[1]);
            break;
        case PROMPT_CONFIRM_CARD_EFFECT:
            done = DuelPrompt_ConfirmCardEffect(gDuel.promptPlayer, gDuel.promptArgs[0]);
            break;
        case PROMPT_SELECT_OPPONENT_REPLACEMENT_TARGET:
            done = DuelPrompt_SelectOpponentReplacementTarget(gDuel.promptPlayer, gDuel.promptArgs[0]);
            break;
        case PROMPT_OFFER_DISCARD_MAGIC:
            done = DuelPrompt_OfferDiscardMagic(gDuel.promptPlayer);
            break;
        case PROMPT_CONFIRM_SPECIAL_SUMMON:
            done = DuelPrompt_ConfirmSpecialSummon(gDuel.promptPlayer, gDuel.promptArgs[0]);
            break;
        case PROMPT_CONFIRM_GRAVEYARD_SUMMON:
            done = DuelPrompt_ConfirmGraveyardSummon(gDuel.promptPlayer);
            break;
        case PROMPT_SELECT_OWN_REPLACEMENT_TARGET:
            done = DuelPrompt_SelectOwnReplacementTarget(gDuel.promptPlayer, gDuel.promptArgs[0]);
            break;
        default:
            return 0;
        }
    }
    if (done) {
        if (gDuel.promptLinked && !gDuel.promptPlayer)
            DuelLink_SendMessageData(LINKMSG_PROMPT_RESULT, &gDuel.promptResult, 0x10);
        gDuel.promptActive = 0;
        return 0;
    }
    return 1;
}

/*
 * Make the prompt the caller filled in active. In a link duel a prompt for player 1 is forwarded to the
 * partner as LINKMSG_PROMPT {kind, promptArgs[8]} and marked linked, except PROMPT_DISCARD_RANDOM: the
 * random pick runs here and reaches the partner as ordinary duel commands.
 */
void DuelPrompt_Start(void)
{
    u16 msg[9];

    gDuel.promptActive = 1;
    gDuel.promptStep = 0;
    gDuel.unk1B63 = 0;
    gDuel.promptLinked = 0;
    gDuel.unk1B50_3 = 0;
    if (gDuel.promptPlayer && gDuelCtrl.isLinkDuel) {
        u16 kind = gDuel.promptKind;
        if (kind == PROMPT_DISCARD_RANDOM)
            return;
        msg[0] = kind;
        MemCopy16(&msg[1], gDuel.promptArgs, 0x10);
        DuelLink_SendMessageData(LINKMSG_PROMPT, msg, 0x12);
        gLinkState.promptResultReceived = 0;
        gDuel.promptLinked = 1;
    }
}

/* Post a prompt (enum DuelPromptKind) for player with promptArgs[0] = arg, [1] = value. */
void DuelPrompt_Post(int player, int kind, u16 arg, u16 value)
{
    gDuel.promptPlayer = player;
    gDuel.promptKind = kind;
    gDuel.promptArgs[0] = arg;
    gDuel.promptArgs[1] = value;
    DuelPrompt_Start();
}

/* Post a prompt with up to 8 argument halfwords (more are dropped). */
void DuelPrompt_PostData(int player, int kind, const u16 *args, int count)
{
    if (count > 8)
        count = 8;
    gDuel.promptPlayer = player;
    gDuel.promptKind = kind;
    MemCopy16(gDuel.promptArgs, args, count * 2);
    DuelPrompt_Start();
}

/* PROMPT_DISCARD: discard count hand cards, then the discard trigger. value bit 0 = monsters only,
 * bit 1 = caused by the opponent. */
void DuelPrompt_PostDiscard(int player, int count, u16 monstersOnly, u16 byOpponent)
{
    u16 flags = monstersOnly != 0;
    if (byOpponent)
        flags |= 2;
    DuelPrompt_Post(player, PROMPT_DISCARD, count, flags);
}

/* PROMPT_DISCARD_COST: discard count hand cards as a cost (no trigger); flags as DuelPrompt_PostDiscard. */
void DuelPrompt_PostDiscardCost(int player, int count, u16 monstersOnly, u16 byOpponent)
{
    u16 flags = monstersOnly != 0;
    if (byOpponent)
        flags |= 2;
    DuelPrompt_Post(player, PROMPT_DISCARD_COST, count, flags);
}

/* PROMPT_DISCARD_RANDOM: discard count random hand cards. While one is active, its count grows instead
 * (several Robbin' Goblin hits). */
void DuelPrompt_PostRandomDiscard(int player, u16 byOpponent, int count)
{
    if (gDuel.promptActive && gDuel.promptKind == PROMPT_DISCARD_RANDOM)
        gDuel.promptArgs[1] += count;
    else
        DuelPrompt_Post(player, PROMPT_DISCARD_RANDOM, byOpponent, count);
}

/* PROMPT_BANISH_RANDOM: banish count random hand cards; adds to an active one, like random discards. */
void DuelPrompt_PostRandomBanish(int player, int count)
{
    if (gDuel.promptActive && gDuel.promptKind == PROMPT_BANISH_RANDOM)
        gDuel.promptArgs[1] += count;
    else
        DuelPrompt_Post(player, PROMPT_BANISH_RANDOM, 0, count);
}

/* PROMPT_BANISH_RANDOM_FACE_DOWN: banish one random hand card face down. */
void DuelPrompt_PostRandomBanishFaceDown(int player)
{
    DuelPrompt_Post(player, PROMPT_BANISH_RANDOM_FACE_DOWN, 0, 1);
}

/* PROMPT_TRIBUTE: the player tributes one of their monsters. */
void DuelPrompt_PostTribute(int player)
{
    DuelPrompt_Post(player, PROMPT_TRIBUTE, 0, 0);
}

/*
 * Post PROMPT_SET_MONSTER_FROM_HAND if the hand holds a monster of level 4 or lower that can be summoned
 * now and is not Special Summon only; 1 if posted, else 0.
 */
u32 DuelPrompt_TryPostSetMonster(int player)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 cardId = CARD_WORD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));
        if (CanSummonFromHand(player, cardId) && !IsSpecialSummonOnly(cardId) && GetCardLevel(cardId) <= 4) {
            DuelPrompt_Post(player, PROMPT_SET_MONSTER_FROM_HAND, 0, 0);
            return 1;
        }
    }
    return 0;
}

/* Dead code: request a link interrupt (interruptRequested) when none is pending and gDuel +0x1B14 bits 2-8
 * are 0 (it rewrites them with 0). */
void sub_08022914(void)
{
    if (!gLinkState.interruptRequested && gDuel.unk1B14_2 == 0) {
        gLinkState.interruptRequested = 1;
        gDuel.unk1B14_2 = 0;
    }
}

/* Dead code: clears gDuel +0x1B14 bits 2-8; returns 0. */
u32 sub_0802295C(void)
{
    gDuel.unk1B14_2 = 0;
    return 0;
}

/*
 * Matching: struct LinkMessage (duel_link.h) as u32:16 bitfields. The ROM builds the message as two words
 * with mask-and-or stores; the header's u16 members give four halfword stores.
 */
struct LinkMessageWords {
    u32 id:16;                      /* +0x0: enum LinkMsgId */
    u32 arg1:16;                    /* +0x2 */
    u32 arg2:16;                    /* +0x4 */
    u32 arg3:16;                    /* +0x6 */
};

/* Queue the 8-byte link message {id, arg1, arg2, arg3}; returns the LinkQueueMessage result (0 = queue
 * full). */
u16 DuelLink_SendMessage(u16 id, u16 arg1, u16 arg2, u16 arg3)
{
    struct LinkMessageWords msg;

    msg.id = id;
    msg.arg1 = arg1;
    msg.arg2 = arg2;
    msg.arg3 = arg3;
    return LinkQueueMessage((u16 *)&msg, 8);
}

/* Queue the link message {id, data[size]}, built in a 0x100-byte buffer (so size <= 0xFE). */
u16 DuelLink_SendMessageData(u16 id, const void *data, int size)
{
    u16 buf[0x80];

    buf[0] = id;
    if (size > 0)
        MemCopy16(&buf[1], data, size);
    return LinkQueueMessage(buf, size + 2);
}

/*
 * The five card-list senders. A list message is {id, count | player << 8, card words[count]}, built in
 * gLinkState.txId / txHeader / txCards. The partner flips the player and every card's owner bit, stores the
 * list and acks it with id + 0x10, which sets the matching *Acked flag cleared here. The do/while form keeps
 * agbcc from strength-reducing &txCards[i], as in the ROM.
 */

/* Send the player's hand (LINKMSG_HAND_LIST). */
void DuelLink_SendHand(int player)
{
    int i;

    gLinkState.txId = LINKMSG_HAND_LIST;
    gLinkState.txHeader = gDuelPlayers[player & 1].handCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].handCount)
        do
            COPY_CARD(&gLinkTxCards[i], &gDuelPlayers[player & 1].hand[i]);
        while (++i < gDuelPlayers[player & 1].handCount);
    LinkQueueMessage(&gLinkState.txId, gDuelPlayers[player & 1].handCount * 4 + 4);
    gLinkState.handAcked = 0;
}

/* Send the player's deck (LINKMSG_DECK_LIST). */
void DuelLink_SendDeck(int player)
{
    int i;

    gLinkState.txId = LINKMSG_DECK_LIST;
    gLinkState.txHeader = gDuelPlayers[player & 1].deckCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].deckCount)
        do
            COPY_CARD(&gLinkTxCards[i], &gDuelPlayers[player & 1].deck[i]);
        while (++i < gDuelPlayers[player & 1].deckCount);
    LinkQueueMessage(&gLinkState.txId, gDuelPlayers[player & 1].deckCount * 4 + 4);
    gLinkState.deckAcked = 0;
}

/* Send the player's graveyard (LINKMSG_GRAVEYARD_LIST). */
void DuelLink_SendGraveyard(int player)
{
    int i;

    gLinkState.txId = LINKMSG_GRAVEYARD_LIST;
    gLinkState.txHeader = gDuelPlayers[player & 1].graveCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].graveCount)
        do
            COPY_CARD(&gLinkTxCards[i], &gDuelPlayers[player & 1].graveyard[i]);
        while (++i < gDuelPlayers[player & 1].graveCount);
    LinkQueueMessage(&gLinkState.txId, gDuelPlayers[player & 1].graveCount * 4 + 4);
    gLinkState.graveAcked = 0;
}

/* Send the player's fusion deck (LINKMSG_FUSION_DECK_LIST). */
void DuelLink_SendFusionDeck(int player)
{
    int i;

    gLinkState.txId = LINKMSG_FUSION_DECK_LIST;
    gLinkState.txHeader = gDuelPlayers[player & 1].fusionCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].fusionCount)
        do
            COPY_CARD(&gLinkTxCards[i], &gDuelPlayers[player & 1].fusionDeck[i]);
        while (++i < gDuelPlayers[player & 1].fusionCount);
    LinkQueueMessage(&gLinkState.txId, gDuelPlayers[player & 1].fusionCount * 4 + 4);
    gLinkState.fusionAcked = 0;
}

/* Send the player's banished pile (LINKMSG_BANISHED_LIST). */
void DuelLink_SendBanished(int player)
{
    int i;

    gLinkState.txId = LINKMSG_BANISHED_LIST;
    gLinkState.txHeader = gDuelPlayers[player & 1].banishedCount | ((u8)player << 8);
    i = 0;
    if (i < gDuelPlayers[player & 1].banishedCount)
        do
            COPY_CARD(&gLinkTxCards[i], &gDuelPlayers[player & 1].banished[i]);
        while (++i < gDuelPlayers[player & 1].banishedCount);
    LinkQueueMessage(&gLinkState.txId, gDuelPlayers[player & 1].banishedCount * 4 + 4);
    gLinkState.banishedAcked = 0;
}
