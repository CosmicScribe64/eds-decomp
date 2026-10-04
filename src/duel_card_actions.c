#include "global.h"
#include "gba.h"                    /* OBJ_PLTT, OBJ_VRAM0, A_BUTTON, B_BUTTON */
#include "main.h"                  /* gMain.heldKeys / newKeys */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind, gCardStats bit layout */
#include "constants/duel.h"         /* enum DuelPhase, ResponseEventKind, ChainEntryKind, ZoneStatusFlag */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* ids, DUEL_CMD_PLAYER */
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_ALT_ART, CARD_NAME_SIZE, gCardNames, card icons */
#include "util.h"                   /* MemCopy16, CopyDoubleWords, StrLen */
#include "sprite.h"                 /* AddSprite, enum SpriteShape */
#include "text.h"                   /* TextCanvasInit, TextDrawString, TextCanvasToTiles, gSystemFontPal */
#include "duel_actions.h"           /* the card-list actions, card displays and LP changes defined here */
#include "duel_prompt.h"            /* DuelPrompt_PostRandomDiscard */
#include "effect.h"                 /* TriggerAppropriate */

/*
 * Duel actions on the card lists, card displays, life points and the chain-list overlay
 * (wiki/functions/duel-card-actions-c.md):
 *  - find a card number in the graveyard, deck or hand and queue the duel command that moves it (to the
 *    hand, the field, the graveyard or the banished pile); mill or banish the top of the deck;
 *  - draw cards with their draw triggers (Parasite Paracide, the Crush Card virus, Appropriate);
 *  - the one-line wrappers that queue a card display (the Card Detail view and five card-picture
 *    animations, commands 0x72-0x76: the same picture, different animations);
 *  - life-point loss, gain and battle damage, with Robbin' Goblin;
 *  - the chain-list overlay ("Chain : Activating" / "Chain : Resolving") with its sprite helpers and the
 *    card-icon lookup.
 * Every list action is deferred: it queues a duel command (DuelCmd_Push) that the duel command runner
 * executes later, so the lists are unchanged when these functions return.
 */

#include "duel.h"                  /* struct DuelCard / DuelZone / DuelPlayer, gDuel */
#include "chain.h"                  /* Chain_AddPending, EventResponse_Request, struct ChainList / ChainEntry */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_screen.h"            /* gDuelScreen, gChainListScreen, the field fades, text cells, icons */

/*
 * Local views, kept on purpose (matching choices, see build/readability/HEADERS.md):
 * - DuelCmd_Push16: DuelCmd_Push with u16 operands (the definition takes int arg4/arg6). DrawCards (the
 *   Paracide summon) and PlaceDeckCardOnField call it this way, so the caller narrows arg4/arg6 as the ROM
 *   does; every other call here matches with the header prototype;
 * - TextDrawNumber is called with the value as a 4th argument (the definition names only three).
 */
void DuelCmd_Push16(u16 cmd, u16 arg2, u16 arg4, u16 arg6) asm("DuelCmd_Push");
void TextDrawNumber4(s32 x, s32 y, u16 sizeColor, s32 value) asm("TextDrawNumber");

/* ROM data used only here. */
/* 0x08198DE4: the two chain-list headers, struct ChainListHeader[2] (duel_screen.h). Matching: declared as
 * byte rows, so agbcc forms each field address as (symbol + offset) + index * 0x4C, as the ROM does (a
 * 4-aligned struct folds the offset into the load). */
extern const u8 gChainListHeaders[][0x4C];
/* The first byte of a field of header `index`. Matching: an array reference, not *&: the address form
 * folds the field offset into the literal. */
#define CHAIN_LIST_HEADER(index, field) (gChainListHeaders[index][OFFSET_OF(struct ChainListHeader, field)])
extern const u8 gStrLink[];         /* "LINK" */
extern const u8 gStrOpposite[];     /* "Opposite": link activated by player 1 */
extern const u8 gStrYours[];        /* "Yours": link activated by player 0 */
extern const u8 gStrDestroyed[];    /* "Destroyed": ChainEntry.destroyIfNegated */
extern const u8 gStrInvalidated[];  /* "Invalidated": ChainEntry.negated */

/* Command id for DuelCmd_Push: DUEL_CMD_PLAYER (bit 15) marks a command of player 1. */
#define PLAYER_CMD(player, cmd) ((player) ? (DUEL_CMD_PLAYER | (cmd)) : (cmd))

/* The player's side of the duel. */
#define PLAYER(p) (gDuelPlayers[(p) & 1])

/* A card word held in a u32 (or reached through a u32 *), seen as a struct DuelCard. */
#define CARD(word) (*(struct DuelCard *)&(word))

/* The card tables through integer-constant addresses: gCardIdToNumber (0x08622AB4), gCardStats
 * (0x08621DE0) and gCardNumberToId (0x08623DF4). Matching: the literal form, not the symbols. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))   /* enum CardType */
#define CARD_NUMBER_TO_ID ((const u16 *)0x08623DF4)

/* Fields of a Chain_AddPending trigger word: card | zone << 16 | kind << 21 | event << 25 | player << 31
 * (struct ChainEntry, chain.h). kind is an enum ChainEntryKind, event an enum ResponseEventKind. */
#define TRIGGER_ZONE(zone) ((zone) << 16)
#define TRIGGER_KIND(kind) ((kind) << 21)
#define TRIGGER_EVENT(event) ((event) << 25)
#define TRIGGER_PLAYER(player) ((player) << 31)

/* A location (zone << 8) | player packed into a byte: player | zone << 4 (both nibbles). Matching: the u8
 * casts decide which register holds the 0xF constant. */
#define PACK_LOC(loc) ((u8)((loc) & 0xF) | (u8)(((loc) >> 8) & 0xF) << 4)

/* Font size and colour index packed for the sizeColor argument of the TextDraw* functions: TEXT_SIZE and
 * TEXT_SIZE_COLOR come from text.h. */

/* Return the first graveyard card with card number cardNo to the hand
 * (DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND with its card word); 1 if there was one. */
int ReturnGraveyardCardToHand(int player, u16 cardNo)
{
    int i;
    for (i = 0; i < PLAYER(player).graveCount; i++) {
        u32 *entry = (u32 *)&PLAYER(player).graveyard[i];
        u32 card = *entry;
        if (CARD_NUMBER(CARD(card).id) == cardNo) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND), card, card >> 16, 0);
            return 1;
        }
    }
    return 0;
}

/* Copy the first graveyard card with card number cardNo to *out and queue its removal
 * (DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD); 1 if there was one. */
int RemoveGraveyardCardByNumber(int player, u16 cardNo, u32 *out)
{
    int i;
    for (i = 0; i < PLAYER(player).graveCount; i++) {
        u32 *card = (u32 *)&PLAYER(player).graveyard[i];
        if (CARD_NUMBER(CARD(*card).id) == cardNo) {
            CopyDuelCard((struct DuelCard *)out, (struct DuelCard *)card);
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD),
                         ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 1;
        }
    }
    return 0;
}

/* Banish a graveyard card (card: its word, in halves): DUEL_CMD_BANISH_GRAVEYARD_CARD. */
void BanishGraveyardCard(int player, u16 *card)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH_GRAVEYARD_CARD), card[0], card[1], 0);
}

/* Copy the first deck card with card number cardNo to *out and queue its removal
 * (DUEL_CMD_REMOVE_CARD_FROM_DECK); its deck index, or -1. */
int RemoveDeckCardByNumber(int player, u16 cardNo, u32 *out)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        u32 *card = (u32 *)&PLAYER(player).deck[i];
        if (CARD_NUMBER(CARD(*card).id) == cardNo) {
            CopyDuelCard((struct DuelCard *)out, (struct DuelCard *)card);
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_REMOVE_CARD_FROM_DECK), ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return i;
        }
    }
    return -1;
}

/* Add the first deck card with card number cardNo to the hand (DUEL_CMD_ADD_DECK_CARD_TO_HAND); 1 if
 * there was one. */
int AddDeckCardToHand(int player, u16 cardNo)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        u32 *entry = (u32 *)&PLAYER(player).deck[i];
        u32 card = *entry;
        if (CARD_NUMBER(CARD(card).id) == cardNo) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_ADD_DECK_CARD_TO_HAND), card, card >> 16, 0);
            return 1;
        }
    }
    return 0;
}

/* Show the Card Detail page of cardId (DUEL_CMD_SHOW_CARD_DETAIL), then reopen the duel screen. */
void ShowCardDetail(int player, u16 cardId)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_DETAIL), cardId, 1, 0);
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_OPEN_DUEL_SCREEN), 1, 0, 0);
}

/* The card-picture displays (commands 0x72-0x76): the same full-screen card picture with different
 * animations. The names say how card effects use them; the uses overlap. */

/* DUEL_CMD_SHOW_CARD_ZOOM_IN: the picture zooms in, holds and fades (the card an effect targets). */
void ShowActivatedCard(int player, u16 arg)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_ZOOM_IN), arg, 1, 0);
}

/* DUEL_CMD_SHOW_CARD_EFFECT: the picture with a white flash (the card whose effect applies). */
void ShowCardEffect(int player, u16 cardId)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), cardId, 1, 0);
}

/* DUEL_CMD_SHOW_CARD_SCATTER: the picture appears, then its tiles fly apart (a card being destroyed). */
void ShowDestroyedCard(int player, u16 cardId)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_SCATTER), cardId, 1, 0);
}

/* DUEL_CMD_SHOW_CARD_UNROLL_DOWN: the picture's rows grow from the top (a card the player picked). */
void ShowPickedCard(int player, u16 arg)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_UNROLL_DOWN), arg, 1, 0);
}

/* DUEL_CMD_SHOW_CARD_UNROLL_SIDEWAYS: the picture opens from its centre line (a card being revealed). */
void ShowRevealedCard(int player, u16 cardId)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_UNROLL_SIDEWAYS), cardId, 1, 0);
}

/* Lose amount LP (DUEL_CMD_LOSE_LP, animated) and open a RESPONSE_LP_CHANGE window; nothing for 0. */
void LoseLifePoints(int player, int amount)
{
    if (amount != 0) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_LOSE_LP), amount, 1, 0);
        EventResponse_Request(player, RESPONSE_LP_CHANGE, (player << 16) | (u16)amount);
    }
}

/*
 * Battle damage: player loses amount LP. Each of the opponent's active Robbin' Goblins is shown and makes
 * the player discard a random card. Then a response window opens: RESPONSE_BATTLE_DAMAGE when the player
 * controls the defender, else RESPONSE_BATTLE_DEFLECTED_DAMAGE; its argument is the amount in the low half
 * and both locations as PACK_LOC bytes in the high half (the damaged player's location first).
 * attackerLoc / defenderLoc are player | zone << 8.
 */
void InflictBattleDamage(int player, int amount, u16 attackerLoc, u16 defenderLoc)
{
    u16 cardNo;
    /* Matching: the card number stays in one local, used for the count and the table index. */
    int goblins = CountActiveCardsOnField(1 - player, cardNo = CARD_ROBBIN_GOBLIN);

    if (amount != 0) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_LOSE_LP), amount, 1, 0);
        if (goblins > 0) {
            DuelCmd_Push(PLAYER_CMD(1 - player, DUEL_CMD_SHOW_CARD_EFFECT), CARD_NUMBER_TO_ID[cardNo], 1, 0);
            DuelPrompt_PostRandomDiscard(player, 1, goblins);
        }
        if ((u8)defenderLoc == player)
            EventResponse_Request(player, RESPONSE_BATTLE_DAMAGE,
                                  (u16)amount | (PACK_LOC(defenderLoc) | PACK_LOC(attackerLoc) << 8) << 16);
        else
            EventResponse_Request(player, RESPONSE_BATTLE_DEFLECTED_DAMAGE,
                                  (u16)amount | (PACK_LOC(attackerLoc) | PACK_LOC(defenderLoc) << 8) << 16);
    }
}

/* Gain amount LP (DUEL_CMD_GAIN_LP, animated); nothing for 0. Each of the player's face-up monsters with
 * effect key 1434 (no EDS card, not in constants/cards.h) is shown and makes the opponent lose 500 LP. */
void GainLifePoints(int player, int amount)
{
    /* Matching: the card number stays in one local, used for the count and the table index. */
    u16 cardNo = 1434;
    int copies = CountFaceUpMonstersByNumber(player, cardNo);
    if (amount != 0) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_GAIN_LP), amount, 1, 0);
        if (copies > 0) {
            ShowCardEffect(player, CARD_NUMBER_TO_ID[cardNo]);
            LoseLifePoints(1 - player, copies * 500);
        }
    }
}

/* Printed ATK of a card: 0 for Magic, Trap and Ticket cards, 4000 for the Egyptian Gods, else the stats
 * field (gCardStats bits 9-17) times 10. */
static inline u32 CardAttack(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    }
    return ((CARD_STATS(id) >> CARD_STATS_ATK_SHIFT) & 0x1FF) * CARD_STATS_POINTS_SCALE;
}

/*
 * Draw count cards: one DUEL_CMD_DRAW_CARDS per card, and the draw triggers. The deck is unchanged until
 * the commands run, so the i-th card drawn is deck[i]; handIdx follows the hand slot it lands in and does
 * not advance for a card that is summoned or discarded at once.
 *  - First, each active copy of effect key 1305 (no EDS card) gives the player 500 LP.
 *  - Parasite Paracide planted by the opponent (owner != player, planted): summoned face up in defense
 *    position to the first free monster zone (summon status UNK14 | SPECIAL_SUMMONED | PLANTED) and the
 *    player takes 1000 damage; discarded when no zone is free.
 *  - The Crush Card virus (crushCardTurns, while traps are not negated): Crush Card is shown and each drawn
 *    card (except a summoned Paracide) revealed; monsters with 1500 or more ATK are discarded (effect key
 *    1242 then queues a trigger).
 *  - Outside the Draw Phase, the opponent's Appropriate draws and a RESPONSE_DREW window opens.
 */
void DrawCards(int player, int count)
{
    int handIdx = PLAYER(player).handCount;
    int i;
    /* Matching: the card number stays in one local, used for the count and the table index. */
    u16 cardNo = CARD_1305;
    int copies = CountActiveCardsOnField(player, cardNo);

    if (copies > 0) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), CARD_NUMBER_TO_ID[cardNo], 1, 0);
        GainLifePoints(player, copies * 500);
    }
    for (i = 0; i < count; i++) {
        u32 *card = (u32 *)&PLAYER(player).deck[i];
        int handled = 0;
        int placed = 0;

        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_DRAW_CARDS), 1, 1, 0);
        if (CARD(*card).owner != player && CARD(*card).planted
            && CARD_NUMBER(CARD(*card).id) == CARD_PARASITE_PARACIDE) {
            int zone = FindFreeMonsterZone(player);
            handled = 1;
            placed = 1;
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), CARD(*card).id, 1, 0);
            if (zone >= 0) {
                /* arg4: zone | hand index << 4 | faceUp << 8 | defense << 9 */
                DuelCmd_Push16(PLAYER_CMD(player, DUEL_CMD_PLACE_MONSTER_FROM_HAND), CARD(*card).id,
                               ((handIdx & 0xF) << 4) | (zone & 0xF) | 0x300, 0);
                DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SET_ZONE_STATUS_FLAGS), zone,
                             ZONE_STATUS_UNK14 | ZONE_STATUS_SPECIAL_SUMMONED | ZONE_STATUS_PLANTED, 0);
                LoseLifePoints(player, 1000);
            } else {
                DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD), handIdx, 1, 0);
            }
        }
        /* gDuel.trapsNegated (+0x1ACC bit 7). Matching: read as a byte of gDuelPlayers (+0x1AC8), the base
         * this function already holds. */
        if (PLAYER(player).crushCardTurns && !(((u8 *)gDuelPlayers)[0x1AC8] & 0x80) && !handled) {
            u16 id = CARD(*card).id;
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), CARD_NUMBER_TO_ID[CARD_CRUSH_CARD], 1, 0);
            ShowRevealedCard(player, id);
            if (CARD_TYPE(id) <= CARD_TYPE_REPTILE && CardAttack(id) >= 1500) {
                DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD), handIdx, 1, 0);
                if (CARD_NUMBER(id) == CARD_1242) {
                    /* The OR order matches only when built in steps; the zone field holds the player. */
                    u32 trigger = TRIGGER_ZONE(player & 0x1F)
                        | (TRIGGER_EVENT(RESPONSE_DISCARDED) | TRIGGER_KIND(CHAIN_KIND_OFF_FIELD));
                    trigger |= TRIGGER_PLAYER(player & 1);
                    Chain_AddPending(trigger | id, 0);
                }
                placed = 1;
            }
        }
        if (!placed)
            handIdx++;
    }
    if (gDuel.phase != PHASE_DRAW) {
        TriggerAppropriate(1 - player);
        EventResponse_Request(1 - gDuel.turnPlayer, RESPONSE_DREW, (u8)player | ((u8)(1 - player) << 16));
    }
}

/* Banish the top count cards of the deck (DUEL_CMD_BANISH_TOP_DECK_CARDS). */
void BanishTopDeckCards(int player, int count)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH_TOP_DECK_CARDS), count, 0, 0);
}

/* Send the top count cards of the deck to the graveyard (DUEL_CMD_SEND_TOP_DECK_CARDS_TO_GRAVEYARD), or
 * banish them under Banisher of the Light. Among those cards, Penguin Knight (only byOpponentEffect) and
 * effect key 1242 queue their RESPONSE_DECK_TO_GRAVE triggers. */
void SendTopDeckCardsToGraveyard(int player, int count, u16 byOpponentEffect)
{
    int i;

    if (CountFaceUpMonstersByNumber(0, CARD_BANISHER_OF_THE_LIGHT) > 0
        || CountFaceUpMonstersByNumber(1, CARD_BANISHER_OF_THE_LIGHT) > 0) {
        BanishTopDeckCards(player, count);
        return;
    }
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SEND_TOP_DECK_CARDS_TO_GRAVEYARD), count, 0, 0);
    for (i = 0; i < count && i < PLAYER(player).deckCount; i++) {
        u32 id = CARD(PLAYER(player).deck[i]).id;
        switch (CARD_NUMBER(id)) {
        case CARD_PENGUIN_KNIGHT:
            if (byOpponentEffect)
                Chain_AddPending(TRIGGER_PLAYER(player & 1)
                                 | (TRIGGER_EVENT(RESPONSE_DECK_TO_GRAVE) | TRIGGER_KIND(CHAIN_KIND_OFF_FIELD))
                                 | (id & 0xFFFF), 0);
            break;
        case CARD_1242:
            Chain_AddPending(TRIGGER_PLAYER(player & 1)
                             | (TRIGGER_EVENT(RESPONSE_DECK_TO_GRAVE) | TRIGGER_KIND(CHAIN_KIND_OFF_FIELD))
                             | (id & 0xFFFF), 0);
            break;
        }
    }
}

/* The card ID of deck[index] / fusionDeck[index] of the player, read as a whole word (ldr; lsl #20;
 * lsr #20). Matching: the index offset and the player offset are computed separately and added to the
 * gDuelPlayers base before the pile offset; these readers keep the card-number lookups of the callers'
 * loops in the loop. */
static inline u16 ReadDeckCardId(int player, int index)
{
    int indexBytes = index * sizeof(struct DuelCard);
    u32 offset, word;

    offset = (player & 1) * sizeof(struct DuelPlayer);
    word = *(u32 *)(indexBytes + offset + (u32)gDuelPlayers + OFFSET_OF(struct DuelPlayer, deck));
    return (word << 20) >> 20;
}

static inline u16 ReadFusionCardId(int player, int index)
{
    int indexBytes = index * sizeof(struct DuelCard);
    u32 offset, word;

    offset = (player & 1) * sizeof(struct DuelPlayer);
    word = *(u32 *)(indexBytes + offset + (u32)gDuelPlayers + OFFSET_OF(struct DuelPlayer, fusionDeck));
    return (word << 20) >> 20;
}

/* Card number to card ID: 0xFFFF (no card) gives 0; an alternate-art number (2000 + n) gives n's ID + 1. */
static inline u16 CardNumberToId(u16 cardNo)
{
    if (cardNo == 0xFFFF)
        return 0;
    if (cardNo < CARD_NUMBER_ALT_ART)
        return CARD_NUMBER_TO_ID[cardNo & CARD_ID_MASK];
    return CARD_NUMBER_TO_ID[(cardNo - CARD_NUMBER_ALT_ART) & CARD_ID_MASK] + 1;
}

/* Send every deck card (DUEL_CMD_SEND_DECK_CARD_TO_GRAVEYARD) and fusion-deck card
 * (DUEL_CMD_SEND_FUSION_DECK_CARD_TO_GRAVEYARD) with the same name as card number cardNo to the graveyard.
 * Penguin Knight sent by an opponent's effect also returns the graveyard to the deck and shuffles it. */
void SendDeckCopiesToGraveyard(int player, u16 cardNo, u16 byOpponentEffect)
{
    int i;

    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (IsSameCardName(ReadDeckCardId(player, i), CardNumberToId(cardNo))) {
            u16 *half = (u16 *)&PLAYER(player).deck[i];
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SEND_DECK_CARD_TO_GRAVEYARD), half[0], half[1], 0);
        }
    }
    for (i = 0; i < PLAYER(player).fusionCount; i++) {
        if (IsSameCardName(ReadFusionCardId(player, i), CardNumberToId(cardNo))) {
            u16 *half = (u16 *)&PLAYER(player).fusionDeck[i];
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SEND_FUSION_DECK_CARD_TO_GRAVEYARD), half[0], half[1], 0);
        }
    }
    if (byOpponentEffect && cardNo == CARD_PENGUIN_KNIGHT) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_RETURN_GRAVEYARD_TO_DECK), 1, 0, 0);
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
    }
}

/* Banish every deck card with card number cardNo (DUEL_CMD_BANISH_DECK_CARD). */
void BanishDeckCopies(int player, u16 cardNo)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (CARD_NUMBER(ReadDeckCardId(player, i)) == cardNo) {
            u16 *half = (u16 *)&PLAYER(player).deck[i];
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH_DECK_CARD), half[0], half[1], 0);
        }
    }
}

/* Summon the first deck card with card number cardNo to zone (DUEL_CMD_SUMMON_FROM_DECK; the handler
 * places it face up in attack position, even in a spell/trap zone); 1 if there was one. */
int PlaceDeckCardOnField(int player, u16 cardNo, int zone)
{
    int i;

    /* FAKEMATCH: an empty asm that clobbers r8 keeps the narrowed card number in ip while the command
     * operand uses r8, as in the ROM. */
    __asm__("" : : : "r8");
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (CARD_NUMBER(CARD(PLAYER(player).deck[i]).id) == cardNo) {
            u16 *half = (u16 *)&PLAYER(player).deck[i];
            DuelCmd_Push16(PLAYER_CMD(player, DUEL_CMD_SUMMON_FROM_DECK), half[0], half[1], zone);
            return 1;
        }
    }
    return 0;
}

/* Discard the first hand card with card number cardNo (DiscardHandCard(player, i, 0, 1)); 1 if there was
 * one. */
int DiscardHandCardByNumber(int player, u16 cardNo)
{
    int i = 0;
    u8 *players = (u8 *)gDuelPlayers;
    u32 offset = (player & 1) * sizeof(struct DuelPlayer);
    struct DuelPlayer *p = (struct DuelPlayer *)(players + offset);
    if (i < p->handCount) {
        /* Matching: the hand pointer is a fresh variable born as offset + (players + hand offset), with the
         * base in its own temporary. */
        u8 *hands = players + OFFSET_OF(struct DuelPlayer, hand);
        u32 *hand = (u32 *)(offset + (u32)hands);
        do {
            if (CARD_NUMBER(CARD(*hand).id) == cardNo) {
                DiscardHandCard(player, i, 0, 1);
                return 1;
            }
            hand++;
            i++;
        } while (i < p->handCount);
    }
    return 0;
}

/* The chain-list header: eight 32x16 sprites across the top of the screen, showing the first two OBJ tile
 * rows (2D mapping), where ChainListScreen_Run renders the header text. */
void ChainListScreen_DrawHeader(void)
{
    int i;
    for (i = 0; i <= 7; i++)
        AddSprite(i << 5, SPRITE_SHAPE_32x16, i << 2);
}

/* The four chain rows, 32 px apart from y = 16, each a 32x4-tile block from OBJ tile row * 128 + 64: a
 * 32x32 card icon (attr2 palette 1, priority 1) and seven 32x32 text sprites (priority 1). With `scaled`,
 * each row's y is multiplied by scale / 16 (the open/close animation). The third argument is unused
 * (callers pass -1). */
void ChainListScreen_DrawRows(int scale, u16 scaled, int unused)
{
    int i, j;
    for (i = 0; i < 4; i++) {
        int tile = i * 128 + 64;
        int y = i * 32 + 16;
        if (scaled) {
            /* Matching: two statements give the ROM's register use for the signed divide. */
            y *= scale;
            y /= 16;
        }
        AddSprite(y << 16, SPRITE_SHAPE_32x32, tile + 0x1400);
        for (j = 1; j < 8; j++)
            AddSprite((j << 5) | (y << 16), SPRITE_SHAPE_32x32, tile + j * 4 + 0x400);
    }
}

/* The enum CardKind that picks a card's frame: Obelisk ritual, Slifer and Ra effect, Magic / Trap /
 * Ticket by type, else gCardStats bits 18-19. */
static inline int CardFrameKind(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    }
    return CARD_STATS_KIND(CARD_STATS(id));
}

/* The card's 32x32 icon graphics in its frame colour (4 poses, 0x800 bytes): Magic, Trap, else by frame
 * kind (Tickets get the normal icon). */
const u8 *GetCardIconGfx(u16 cardId)
{
    switch (CARD_TYPE(cardId)) {
    case CARD_TYPE_MAGIC:
        return gCardIconMagicGfx;
    case CARD_TYPE_TRAP:
        return gCardIconTrapGfx;
    }
    switch (CardFrameKind(cardId)) {
    case CARD_KIND_EFFECT:
        return gCardIconEffectGfx;
    case CARD_KIND_FUSION:
        return gCardIconFusionGfx;
    case CARD_KIND_RITUAL:
        return gCardIconRitualGfx;
    }
    return gCardIconNormalGfx;
}

/* B held or the duel fast mode: the overlay's timers run faster. */
#define FAST() ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)

/*
 * One frame of the chain-list overlay started by ChainListScreen_Start: it lists the last four links of
 * gChainListScreen.chain ("LINK n", "Opposite" / "Yours", "Destroyed", "Invalidated", the card name and
 * its icon) under the header gChainListHeaders[resolving]. Returns 1 when the overlay has closed.
 */
int ChainListScreen_Run(void)
{
    int i, row, x, j;
    struct ChainEntry *e;

    /* Every case returns (no break + trailing return 0): this puts the shared "return 0" block before the
     * default case, and lets the two timer stores of CHAIN_LIST_HOLD cross-jump into one. */
    switch (gChainListScreen.state) {
    case CHAIN_LIST_SETUP:
        TextCellsClear();
        DuelScreen_ScrollToZone(0, 0);
        gChainListScreen.state++;
        return 0;
    case CHAIN_LIST_DIM:
        if (DuelFieldDim(1))
            gChainListScreen.state++;
        return 0;
    case CHAIN_LIST_CLEAR_OBJ:
        UnloadDuelUiGfx();
        gChainListScreen.state++;
        return 0;
    case CHAIN_LIST_DRAW:
        /* OBJ palette 0: the font; palette 1: the card icons. */
        MemCopy16((void *)OBJ_PLTT, gSystemFontPal, 0x20);
        CopyDoubleWords((void *)(OBJ_PLTT + 0x20), gCardIconPal, 0x20);
        /* The header text (shadow pass, then the text) into the first two OBJ tile rows. */
        TextCanvasInit(0x20, 2);
        TextDrawString(3, 3, CHAIN_LIST_HEADER(gChainListScreen.resolving, shadowColor) | TEXT_SIZE(12),
                       &CHAIN_LIST_HEADER(gChainListScreen.resolving, text));
        TextDrawString(2, 2, CHAIN_LIST_HEADER(gChainListScreen.resolving, textColor) | TEXT_SIZE(12),
                       &CHAIN_LIST_HEADER(gChainListScreen.resolving, text));
        TextCanvasToTiles((u16 *)OBJ_VRAM0, *(u16 *)&CHAIN_LIST_HEADER(gChainListScreen.resolving, bgColor));
        /* The last four links, one row each: the text canvas into the row's tile block (OBJ tile row * 128 +
         * 64, 2D mapping), then the card icon over its first four tile columns. */
        i = 0;
        if (gChainListScreen.chain->count > 4)
            i = gChainListScreen.chain->count - 4;
        for (row = 0; i < gChainListScreen.chain->count && row <= 3; row++, i++) {
            e = &gChainListScreen.chain->entries[i];
            TextCanvasInit(0x20, 4);
            TextDrawString(0x22, 5, TEXT_SIZE_COLOR(10, 1), gStrLink);
            TextDrawString(0x21, 4, TEXT_SIZE_COLOR(10, 7), gStrLink);
            x = StrLen(gStrLink) * 5;
            TextDrawNumber4(x + 0x22, 5, TEXT_SIZE_COLOR(10, 1), i + 1);
            TextDrawNumber4(x + 0x21, 4, TEXT_SIZE_COLOR(10, 7), i + 1);
            if (i <= 9)
                x += 10;
            else
                x += 15;
            if (e->player) {
                x += 4;
                TextDrawString(x + 0x22, 5, TEXT_SIZE_COLOR(10, 1), gStrOpposite);
                TextDrawString(x + 0x21, 4, TEXT_SIZE_COLOR(10, 4), gStrOpposite);
                x += StrLen(gStrOpposite) * 5;
            } else {
                x += 4;
                TextDrawString(x + 0x22, 5, TEXT_SIZE_COLOR(10, 1), gStrYours);
                TextDrawString(x + 0x21, 4, TEXT_SIZE_COLOR(10, 6), gStrYours);
                x += StrLen(gStrYours) * 5;
            }
            if (e->destroyIfNegated) {
                x += 4;
                TextDrawString(x + 0x22, 5, TEXT_SIZE_COLOR(10, 11), gStrDestroyed);
                TextDrawString(x + 0x21, 4, TEXT_SIZE_COLOR(10, 3), gStrDestroyed);
                x += StrLen(gStrDestroyed) * 5;
            }
            if (e->negated) {
                x += 4;
                TextDrawString(x + 0x22, 5, TEXT_SIZE_COLOR(10, 13), gStrInvalidated);
                TextDrawString(x + 0x21, 4, TEXT_SIZE_COLOR(10, 5), gStrInvalidated);
                x += StrLen(gStrInvalidated) * 5;
            }
            TextDrawString(0x23, 0x12, TEXT_SIZE_COLOR(10, 1), gCardNames + e->card * CARD_NAME_SIZE);
            TextDrawString(0x22, 0x11, TEXT_SIZE_COLOR(10, 7), gCardNames + e->card * CARD_NAME_SIZE);
            TextCanvasToTiles((u16 *)(OBJ_VRAM0 + ((row * 128 + 64) << 5)), 0);
            /* Matching: the tile-index form keeps VRAM + offset out of GCSE, so loop.c rebuilds it for the
             * induction variable. */
            for (j = 0; j <= 3; j++)
                CopyDoubleWords((void *)(OBJ_VRAM0 + ((row * 128 + 64 + j * 32) << 5)),
                                GetCardIconGfx(e->card) + j * 128, 0x80);
        }
        gChainListScreen.timer = 0;
        gChainListScreen.state++;
        return 0;
    case CHAIN_LIST_OPEN:
        /* The rows grow over 16 frames (faster with FAST()). */
        ChainListScreen_DrawHeader();
        ChainListScreen_DrawRows(gChainListScreen.timer, 1, -1);
        if (gChainListScreen.timer <= 15) {
            gChainListScreen.timer++;
            if (FAST()) {
                if (gChainListScreen.timer <= 7)
                    gChainListScreen.timer += 7;
                else
                    gChainListScreen.timer = 16;
            }
        } else {
            gChainListScreen.timer = 0;
            gChainListScreen.state++;
        }
        return 0;
    case CHAIN_LIST_HOLD:
        /* Hold until A or B, or 120 frames (faster with FAST()). */
        ChainListScreen_DrawHeader();
        ChainListScreen_DrawRows(0, 0, -1);
        gChainListScreen.timer++;
        if ((gMain.newKeys & (A_BUTTON | B_BUTTON)) || gChainListScreen.timer > 120) {
            gChainListScreen.timer = 16;
            gChainListScreen.state++;
            return 0;
        }
        if (FAST()) {
            if (gChainListScreen.timer <= 103)
                gChainListScreen.timer += 16;
            else
                gChainListScreen.timer = 120;
        }
        return 0;
    case CHAIN_LIST_CLOSE:
        /* The rows shrink back over 16 frames (faster with FAST()). */
        ChainListScreen_DrawHeader();
        ChainListScreen_DrawRows(gChainListScreen.timer, 1, -1);
        if (gChainListScreen.timer != 0) {
            gChainListScreen.timer--;
            if (FAST()) {
                if (gChainListScreen.timer > 8)
                    gChainListScreen.timer -= 8;
                else
                    gChainListScreen.timer = 0;
            }
        } else {
            gChainListScreen.timer = 0;
            gChainListScreen.state++;
        }
        return 0;
    case CHAIN_LIST_UNDIM:
        if (DuelFieldFadeFromBlack(1))
            gChainListScreen.state++;
        return 0;
    default: /* CHAIN_LIST_DONE */
        LoadDuelUiGfx();
        return 1;
    }
}
