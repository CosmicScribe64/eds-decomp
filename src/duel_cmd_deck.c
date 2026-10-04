/*
 * Duel command handlers for the deck and for zone state (wiki/functions/duel-cmd-deck-c.md).
 *
 * DuelCmd_Dispatch calls the handler of gDuelCmd.cmd once per frame while gDuelCmd.running is set. A handler
 * reads its operands from gDuelCmd (cmd bit 15: the acting player; arg2, arg4, arg6) and clears running
 * when it is done.
 *  - Zone-state commands (0x83-0x8F): one-frame updates of a zone's links, turn counter, declared value and
 *    card bits, and of the Prohibition list in gDuel.
 *  - Deck commands (0x60-0x68, 0xDD): shuffle, draw, send or banish the top cards, and move one given card
 *    out of the deck or the fusion deck. They are step machines on gDuelCmd.step: scroll the field, animate
 *    the card with DuelAnim_MoveCard, then make the move in the duel state. A single card is given as a
 *    card word (struct DuelCard) split over two operands: arg2 | arg4 << 16.
 */
#include "global.h"
#include "constants/duel.h"         /* enum DuelArea, ZONE_LINK_EQUIP, DUEL_LOC */
#include "constants/duel_cmds.h"    /* DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* SE_CARD_MOVE */
#include "duel.h"           /* struct DuelCard, DuelZone, DuelLoc; gDuel, gDuelPlayers, gDuelZones; deck helpers */
#include "sound.h"          /* PlaySE */
#include "duel_cmd.h"       /* gDuelCmd, gDuelCmdT16; the DuelCmd_* handlers defined here */
#include "duel_screen.h"    /* gDuelScreen, DuelScreen_*, DuelCursor_Select, DuelAnim_MoveCard, DuelSprAnim_* */
#include "duel_link.h"      /* gLinkState, DuelLink_SendDeck */
#include "duel_flow.h"      /* gDuelCtrl */
#include "util.h"           /* MemCopy16 */

/* Shuffle animation (sprite.h SprAnim stream, 6 frames of a card stack being cut), played in
 * gDuelScreen.sprAnim by DuelCmd_ShuffleDeck. Used only here. */
extern const u8 gDeckShuffleAnim[];             /* 0x08694EA8 */

/* ---- Local views kept for matching ---- */

/* Matching: view of DuelDrawCard (duel.h, one parameter) with a second one. DuelCmd_DrawCards passes
 * gDuelCmd.arg2 in r1; the function ignores it. */
void DuelDrawCard2(int player, u16 unused) asm("DuelDrawCard");

/* Matching: view of TakeDeckCardAt (duel.h, returns u16) that returns int. The two deck-pile commands test the
 * result as an int; the header prototype adds a lsl #16 before the test. */
int TakeDeckCardAtInt(int player, int idx, struct DuelCard *out) asm("TakeDeckCardAt");

/* Matching: a local view of struct DuelZone with the turn counter (DuelZone.turnCounter, +0x06 bits 2-5) as
 * bits 18-21 of the u32 at zone +0x04. DuelCmd_AddZoneTurnCounter adds through this container; with duel.h's
 * u8 container the operands of the add come out swapped. */
struct DuelZoneTurnCounterWord {
    struct DuelCard card;
    u32 unk4_0:18;                  /* serial, isDefense, isFaceUp */
    u32 turnCounter:4;
    u32 unk6_6:10;
};

/* The card ID of a zone or of gDuelCmd.card is read with DUEL_CARD_ID (duel.h): through a struct DuelCard
 * pointer that loads the whole card word (ldr; lsl #20; lsr #20) as the ROM does, where a direct member read
 * (zone->card.id) loads a halfword. */

/* ---- Helpers ---- */

/* The acting player of the command (bit 15 of the command id, DUEL_CMD_PLAYER). */
#define CMD_PLAYER (gDuelCmd.cmd >> 15)

/* Matching: &gDuelZones[player].zones[zone], with the terms summed in the ROM's order (zone offset + player
 * offset + base); the array form orders the adds differently. */
#define CMD_ZONE(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelZonesPlayer) \
                         + (u8 *)gDuelZones))

/* Fill a struct DuelLoc for DuelAnim_MoveCard; the moving card is never drawn in defense position. */
#define SET_LOC(loc, player_, area_, index_, faceUp_) \
    ((loc).player = (player_), (loc).area = (area_), (loc).index = (index_), (loc).isDefense = 0, \
     (loc).isFaceUp = (faceUp_))

/* FAKEMATCH: the value x hidden from the optimizer by an empty asm volatile, so that loop.c does not hoist
 * it out of the loop (DuelCmd_RemoveProhibition). */
#define OPAQUE(x) ({ int opaque_ = (x); asm volatile("" : "+r"(opaque_)); opaque_; })

/* ======== Zone-state commands (one frame each) ======== */

/* 0x83 DUEL_CMD_ADD_EQUIP_LINK: arg2 = DUEL_LOC of an equip card, arg4 = DUEL_LOC of the monster it
 * equips. The monster's zone gets a ZONE_LINK_EQUIP link to the equip card; the info bar and the field
 * are redrawn. */
void DuelCmd_AddEquipLink(void)
{
    AddZoneLink(gDuelCmd.arg4, gDuelCmd.arg2, ZONE_LINK_EQUIP);
    DuelScreen_DrawCursorInfo();
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

/* 0x85 DUEL_CMD_ADD_ZONE_LINK: the zone at DUEL_LOC arg4 gets a link of kind arg6 (enum ZoneLinkKind) to
 * target arg2; the info bar and the field are redrawn. */
void DuelCmd_AddZoneLink(void)
{
    AddZoneLink(gDuelCmd.arg4, gDuelCmd.arg2, gDuelCmd.arg6);
    DuelScreen_DrawCursorInfo();
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

/* 0x86 DUEL_CMD_REMOVE_ZONE_LINK: remove the kind-arg6 link to target arg2 from the zone at DUEL_LOC arg4;
 * the info bar and the field are redrawn. */
void DuelCmd_RemoveZoneLink(void)
{
    RemoveZoneLink(gDuelCmd.arg4, gDuelCmd.arg2, gDuelCmd.arg6);
    DuelScreen_DrawCursorInfo();
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

/* 0x87 DUEL_CMD_SET_ZONE_DECLARED_VALUE: declaredValue of the acting player's zone (u8)arg2 = arg4, with
 * no occupancy check; redraws the field. It is the choice made when the card resolved (DNA Surgery's type,
 * 7 Completed's 1 = ATK / 2 = DEF, an attribute); GetZoneCardStats reads it. */
void DuelCmd_SetZoneDeclaredValue(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = CMD_ZONE(player, (u8)gDuelCmd.arg2);

    zone->declaredValue = gDuelCmd.arg4;
    DrawAllAreaTiles();
    gDuelCmd.running = 0;
}

/* 0x8B DUEL_CMD_SET_DESTROYED_BY_OPPONENT_FLAG: destroyedByOpponent (card bit 22) of the card in the acting
 * player's zone arg2 = arg4. Pushed just before an effect destroys a card its controller does not own. */
void DuelCmd_SetDestroyedByOpponentFlag(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = CMD_ZONE(player, gDuelCmd.arg2);

    zone->card.destroyedByOpponent = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

/* 0x8A DUEL_CMD_ADD_ZONE_TURN_COUNTER: if the acting player's zone arg2 holds a card, add arg4 to its turn
 * counter (4 bits, wraps). Swords of Revealing Light and similar cards count their turns with it. */
void DuelCmd_AddZoneTurnCounter(void)
{
    u16 amount = gDuelCmd.arg4;
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = CMD_ZONE(player, gDuelCmd.arg2);

    if (DUEL_CARD_ID(zone))
        ((struct DuelZoneTurnCounterWord *)zone)->turnCounter
            = amount + ((struct DuelZoneTurnCounterWord *)zone)->turnCounter;
    gDuelCmd.running = 0;
}

/* 0x89 DUEL_CMD_SET_ZONE_TURN_COUNTER: if the acting player's zone arg2 holds a card, its turn counter =
 * arg4. */
void DuelCmd_SetZoneTurnCounter(void)
{
    u16 value = gDuelCmd.arg4;
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = CMD_ZONE(player, gDuelCmd.arg2);
    /* FAKEMATCH: unused read. The ROM loads the counter byte before the occupancy test and reuses it for
     * the store below; without this read the load moves inside the if. */
    u8 oldCounter = zone->turnCounter;

    if (DUEL_CARD_ID(zone))
        zone->turnCounter = value;
    gDuelCmd.running = 0;
}

/* 0x88 DUEL_CMD_RESET_ZONE_TURN_COUNTER_AND_SET_DECLARED_VALUE: if the acting player's zone arg2 holds a
 * card, its turn counter = 0 and its declaredValue = arg4. */
void DuelCmd_ResetZoneTurnCounterAndSetDeclaredValue(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = CMD_ZONE(player, gDuelCmd.arg2);

    if (DUEL_CARD_ID(zone)) {
        zone->turnCounter = 0;
        zone->declaredValue = gDuelCmd.arg4;
    }
    gDuelCmd.running = 0;
}

/* 0x8C DUEL_CMD_CLEAR_ZONE_LINKS: drop every link of the acting player's zone arg2 (Relinquished clears
 * its zone before it absorbs a monster). */
void DuelCmd_ClearZoneLinks(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = CMD_ZONE(player, gDuelCmd.arg2);

    zone->numLinks = 0;
    gDuelCmd.running = 0;
}

/* 0x8D DUEL_CMD_MOVE_ZONE_LINKS: move every link of the acting player's zone arg2 to zone arg4. Magical
 * Hats parks a monster's links in the field zone while the hats are shuffled, then moves them to the
 * monster's new zone. */
void DuelCmd_MoveZoneLinks(void)
{
    struct DuelZone *zones = (struct DuelZone *)((u8 *)gDuelZones + CMD_PLAYER * sizeof(struct DuelZonesPlayer));
    struct DuelZone *src = zones + gDuelCmd.arg2;
    struct DuelZone *dst;

    /* FAKEMATCH: clobbering r6 here makes global-alloc put src in r5 and dst in r6 as in the ROM
     * (tools/regoracle.py: src lacks one reference, or one instruction less of live range, to win r5). */
    __asm__("" : : : "r6");
    dst = zones + gDuelCmd.arg4;
    MemCopy16(dst->links, src->links, sizeof(dst->links));
    MemCopy16(dst->linkKinds, src->linkKinds, sizeof(dst->linkKinds));
    dst->numLinks = src->numLinks;
    src->numLinks = 0;
    gDuelCmd.running = 0;
}

/* 0x8E DUEL_CMD_ADD_PROHIBITION: record a Prohibition in gDuel: its zone (the acting player's zone arg2) and
 * the card ID it declared (arg4; nothing is recorded for 0). IsCardProhibited checks this list. */
void DuelCmd_AddProhibition(void)
{
    if (gDuelCmd.arg4 != 0) {
        gDuel.prohibitionZones[gDuel.prohibitionCount] = DUEL_LOC(CMD_PLAYER, (u8)gDuelCmd.arg2);
        gDuel.prohibitedCards[gDuel.prohibitionCount] = gDuelCmd.arg4;
        gDuel.prohibitionCount++;
    }
    gDuelCmd.running = 0;
}

/* 0x8F DUEL_CMD_REMOVE_PROHIBITION: remove every Prohibition entry of the acting player's zone arg2 (the
 * card left the field), closing the gap in both lists. */
void DuelCmd_RemoveProhibition(void)
{
    s32 i, j;

    for (i = 0; i < gDuel.prohibitionCount; i++) {
        /* The key is DUEL_LOC(player, zone).
         * FAKEMATCH: the (u16) cast makes the else arm recompute arg2 << 8 (combine splits the zero-extension
         * off the shared shift). OPAQUE keeps the 0x8000 mask and the AND inside the loop as in the ROM (only
         * the cmd load is hoisted). */
        if (gDuel.prohibitionZones[i]
            == (u16)DUEL_LOC((gDuelCmd.cmd & OPAQUE(DUEL_CMD_PLAYER)) ? 1 : 0, (u8)gDuelCmd.arg2)) {
            /* FAKEMATCH: the (u16) cast; prohibitionCount-- compiles differently. */
            gDuel.prohibitionCount = (u16)(gDuel.prohibitionCount - 1);
            for (j = i; j < gDuel.prohibitionCount; j++) {
                gDuel.prohibitionZones[j] = gDuel.prohibitionZones[j + 1];
                gDuel.prohibitedCards[j] = gDuel.prohibitedCards[j + 1];
            }
        }
    }
    gDuelCmd.running = 0;
}

/* ======== Deck commands (step machines on gDuelCmd.step) ======== */

/* 0x60 DUEL_CMD_SHUFFLE_DECK: shuffle the acting player's deck while the shuffle animation plays four
 * times. In a link duel, the GBA whose turn it is (turnPlayer 0: each GBA is player 0 on its own side, the
 * partner's commands arrive mirrored) then sends the new deck order to the partner and waits for its
 * acknowledgement, so both GBAs hold the same order. */
void DuelCmd_ShuffleDeck(void)
{
    int player = CMD_PLAYER;

    switch (gDuelCmd.step) {
    case 0:     /* scroll to the middle of the field (80 px, the scroll target of the monster rows) */
        DuelScreen_StartScroll(80);
        gDuelCmd.step++;
        break;
    case 1:     /* load the animation (it replaces the duel UI's OBJ tiles) */
        DuelSprAnim_Load((u32)gDeckShuffleAnim);
        gDuelCmd.counter = 0;
        gDuelCmd.step++;
        /* fallthrough */
    case 2:     /* every frame: shuffle and draw; frameIndex is back at 0 when a loop of the animation ends */
        ShuffleDeck(player, 3);             /* 3 x deckCount random swaps per frame */
        DuelSprAnim_DrawAt(0, 0, 1);        /* draw at (0, 0) and advance one frame */
        if (gDuelScreen.sprAnim.frameIndex != 0)
            break;
        gDuelCmd.counter++;
        /* FAKEMATCH: (s8) gives the ROM's signed compare; the counter is 0-127, so the value is the same. */
        if ((s8)gDuelCmd.counter <= 3) {
            PlaySE(SE_CARD_MOVE);   /* the shuffle sound */
            DuelSprAnim_Rewind();
        } else {
            gDuelCmd.step++;
        }
        break;
    case 3:     /* restore the UI graphics; in a link duel on our turn, send the deck */
        LoadDuelUiGfx();
        if (gDuelCtrl.isLinkDuel && !gDuel.turnPlayer) {
            DuelLink_SendDeck(player);
            gDuelCmd.step = 10;
        } else {
            gDuelCmd.running = 0;
        }
        break;
    case 10:    /* wait until the partner acknowledges the deck list */
        if (gLinkState.deckAcked)
            gDuelCmd.running = 0;
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/* 0x61 DUEL_CMD_DRAW_CARDS: the acting player draws arg4 cards, one animation each (arg2 is passed to
 * DuelDrawCard, which ignores it). A player who has to draw from an empty deck gets deckOut and loses the
 * duel (Duel_CheckWin). */
void DuelCmd_DrawCards(void)
{
    struct DuelLoc from, to;
    int player = CMD_PLAYER;

    switch (gDuelCmd.step) {
    case 0:     /* scroll to the hand; an empty deck ends the command */
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        if (gDuelPlayers[player].deckCount != 0)
            break;
        gDuelPlayers[player].deckOut = 1;
        gDuelCmd.running = 0;
        break;
    case 1:     /* the top card flies face down from the deck to the next hand slot */
        SET_LOC(from, player, DUEL_AREA_DECK, 0, FALSE);
        SET_LOC(to, player, DUEL_AREA_HAND, gDuelPlayers[player & 1].handCount, FALSE);
        DuelAnim_MoveCard(0, &from, &to);   /* no card ID: it moves face down */
        gDuelCmd.step++;
        break;
    case 2:     /* draw it; repeat from step 1 while cards remain */
        DuelDrawCard2(player, gDuelCmd.arg2);
        gDuelCmd.arg4--;
        if (gDuelCmd.arg4 != 0) {
            gDuelCmd.timer = 0;
            gDuelCmd.step--;
            break;
        }
        /* fallthrough */
    default:    /* point the cursor at the last card drawn */
        DuelCursor_Select(player, DUEL_AREA_HAND, gDuelPlayers[player].handCount - 1);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* 0x62 DUEL_CMD_SEND_TOP_DECK_CARDS_TO_GRAVEYARD: send the top arg2 cards of the acting player's deck to
 * the graveyard, one at a time (arg2 counts down). Stops early when the deck is empty. */
void DuelCmd_SendTopDeckCardsToGraveyard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    /* FAKEMATCH: a second copy of the player; the ROM keeps it in two registers (r5 for the calls, r8). */
    int player2 = player;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:     /* take the top card into gDuelCmd.card and animate it face up to the graveyard */
        if (gDuelCmd.arg2 == 0) {
            gDuelCmd.running = 0;
            break;
        }
        if (TakeDeckCardAtInt(player, 0, &gDuelCmd.card)) {
            SET_LOC(from, player2, DUEL_AREA_DECK, 0, TRUE);
            SET_LOC(to, player2, DUEL_AREA_GRAVEYARD, 0, TRUE);
            DuelAnim_MoveCard(DUEL_CARD_ID(&gDuelCmd.card), &from, &to);
            gDuelCmd.step++;
        } else {
            DuelCursor_Select(player2, DUEL_AREA_DECK, 0);
            DrawAllAreaTiles();
            gDuelCmd.running = 0;
        }
        break;
    case 2:     /* put it in the graveyard; repeat from step 1 while cards remain */
        AddCardToGraveyard(&gDuelCmd.card);
        DrawAllAreaTiles();
        gDuelCmd.arg2--;
        if (gDuelCmd.arg2 != 0)
            gDuelCmd.step--;
        else
            gDuelCmd.step++;
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/* 0x63 DUEL_CMD_BANISH_TOP_DECK_CARDS: banish the top arg2 cards of the acting player's deck, one at a
 * time (counted down in gDuelCmd.counter). Stops early when the deck is empty. */
void DuelCmd_BanishTopDeckCards(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u32 player2 = player;   /* FAKEMATCH: second copy of the player, as in DuelCmd_SendTopDeckCardsToGraveyard */

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.counter = gDuelCmd.arg2;
        /* FAKEMATCH: the cast makes the ROM's re-extraction of the stored counter (lsl; lsr; cmp) */
        if ((s8)gDuelCmd.counter == 0) {
            gDuelCmd.running = 0;
            break;
        }
        gDuelCmd.step++;
        break;
    case 1:     /* take the top card into gDuelCmd.card and animate it face up to the banished pile */
        if (TakeDeckCardAtInt(player, 0, &gDuelCmd.card)) {
            SET_LOC(from, player2 & 1, DUEL_AREA_DECK, 0, TRUE);
            SET_LOC(to, player2 & 1, DUEL_AREA_BANISHED, 0, TRUE);
            DuelAnim_MoveCard(DUEL_CARD_ID(&gDuelCmd.card), &from, &to);
            gDuelCmd.step++;
        } else {
            DuelCursor_Select(player2, DUEL_AREA_DECK, 0);
            DrawAllAreaTiles();
            gDuelCmd.running = 0;
        }
        break;
    case 2:     /* banish it; repeat from step 1 while the counter is positive */
        AddCardToBanished(&gDuelCmd.card);
        DrawAllAreaTiles();
        gDuelCmd.counter--;
        /* FAKEMATCH: (s8) gives the ROM's signed compare (ble); the counter is 0-127 */
        if ((s8)gDuelCmd.counter > 0) {
            gDuelCmd.step--;
            break;
        }
        /* fallthrough */
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/* 0x64 DUEL_CMD_ADD_DECK_CARD_TO_HAND: search the card word arg2 | arg4 << 16 out of its owner's deck into
 * the acting player's hand. */
void DuelCmd_AddDeckCardToHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u32 cardWord = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:     /* finish if the card is not in the deck; else animate it face down to the next hand slot */
        if (RemoveCardFromDeck(((struct DuelCard *)&cardWord)->owner, (struct DuelCard *)&cardWord) == 0) {
            gDuelCmd.running = 0;
            break;
        }
        SET_LOC(from, player, DUEL_AREA_DECK, 0, FALSE);
        SET_LOC(to, player, DUEL_AREA_HAND, gDuelPlayers[player & 1].handCount, FALSE);
        /* the id is gDuelCmd.card's, which this command does not set; the card moves face down, so only
         * its back is drawn */
        DuelAnim_MoveCard(DUEL_CARD_ID(&gDuelCmd.card), &from, &to);
        gDuelCmd.step++;
        break;
    default:
        AddCardToHand(player, (struct DuelCard *)&cardWord);
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_HAND, 0);
        gDuelCmd.running = 0;
        break;
    }
}

/* 0x65 DUEL_CMD_REMOVE_CARD_FROM_DECK: remove the card word arg2 | arg4 << 16 from the acting player's
 * deck, without animation (the pusher handles where the card goes). */
void DuelCmd_RemoveCardFromDeck(void)
{
    u32 player = CMD_PLAYER;
    u32 cardWord = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;

    RemoveCardFromDeck(player, (struct DuelCard *)&cardWord);
    gDuelCmd.running = 0;
}

/* 0x66 DUEL_CMD_SUMMON_FROM_DECK: Special Summon the card word arg2 | arg4 << 16 from the acting player's
 * deck to monster zone arg6, face-up attack position. */
void DuelCmd_SummonFromDeck(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u32 cardWord = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);
    u32 zone = gDuelCmd.arg6;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:     /* take the card out of the deck and animate it face up to the zone */
        if (RemoveCardFromDeck(player, (struct DuelCard *)&cardWord) != 0) {
            CopyDuelCard(&gDuelCmd.card, (struct DuelCard *)&cardWord);
            SET_LOC(from, player, DUEL_AREA_DECK, 0, TRUE);
            SET_LOC(to, player, DUEL_AREA_MONSTER, zone, TRUE);
            DuelAnim_MoveCard(DUEL_CARD_ID(&gDuelCmd.card), &from, &to);
            gDuelCmd.step++;
            break;
        }
        DrawAllAreaTiles();     /* the card was not in the deck */
        gDuelCmd.running = 0;
        break;
    default:    /* place it in attack position, face up */
        PlaceMonsterCard(player, zone, &gDuelCmd.card, FALSE, TRUE);
        DuelCursor_Select(player, DUEL_AREA_MONSTER, zone);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* 0x67 DUEL_CMD_SEND_DECK_CARD_TO_GRAVEYARD: send the card word arg2 | arg4 << 16 from the acting player's
 * deck to the graveyard. */
void DuelCmd_SendDeckCardToGraveyard(void)
{
    struct DuelLoc from, to;
    int player = CMD_PLAYER;
    u32 cardWord = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:     /* take the card out of the deck and animate it face up to the graveyard */
        if (RemoveCardFromDeck(player, (struct DuelCard *)&cardWord) != 0) {
            CopyDuelCard(&gDuelCmd.card, (struct DuelCard *)&cardWord);
            SET_LOC(from, player & 1, DUEL_AREA_DECK, 0, TRUE);
            SET_LOC(to, player & 1, DUEL_AREA_GRAVEYARD, 0, TRUE);
            DuelAnim_MoveCard(DUEL_CARD_ID(&gDuelCmd.card), &from, &to);
            gDuelCmd.step++;
            break;
        }
        DrawAllAreaTiles();     /* the card was not in the deck */
        gDuelCmd.running = 0;
        break;
    default:
        AddCardToGraveyard(&gDuelCmd.card);
        DuelCursor_Select(player, DUEL_AREA_GRAVEYARD, 0);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* 0x68 DUEL_CMD_BANISH_DECK_CARD: banish the card word arg2 | arg4 << 16 from the acting player's deck. */
void DuelCmd_BanishDeckCard(void)
{
    struct DuelLoc from, to;
    int player = CMD_PLAYER;
    u32 cardWord = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:     /* take the card out of the deck and animate it face up to the banished pile */
        if (RemoveCardFromDeck(player, (struct DuelCard *)&cardWord) != 0) {
            CopyDuelCard(&gDuelCmd.card, (struct DuelCard *)&cardWord);
            SET_LOC(from, player & 1, DUEL_AREA_DECK, 0, TRUE);
            SET_LOC(to, player & 1, DUEL_AREA_BANISHED, 0, TRUE);
            DuelAnim_MoveCard(DUEL_CARD_ID(&gDuelCmd.card), &from, &to);
            gDuelCmd.step++;
            break;
        }
        DrawAllAreaTiles();     /* the card was not in the deck */
        gDuelCmd.running = 0;
        break;
    default:
        AddCardToBanished(&gDuelCmd.card);
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_BANISHED, 0);
        gDuelCmd.running = 0;
        break;
    }
}

/* 0xDD DUEL_CMD_SEND_FUSION_DECK_CARD_TO_GRAVEYARD: send the card word arg2 | arg4 << 16 from the acting
 * player's fusion deck to the graveyard. */
void DuelCmd_SendFusionDeckCardToGraveyard(void)
{
    struct DuelLoc from, to;
    int player = CMD_PLAYER;
    u32 cardWord = gDuelCmd.arg2 | (gDuelCmd.arg4 << 16);

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:     /* take the card out of the fusion deck and animate it face up to the graveyard */
        if (RemoveCardFromFusionDeck(player, (struct DuelCard *)&cardWord) != 0) {
            CopyDuelCard(&gDuelCmd.card, (struct DuelCard *)&cardWord);
            SET_LOC(from, player & 1, DUEL_AREA_FUSION_DECK, 0, TRUE);
            SET_LOC(to, player & 1, DUEL_AREA_GRAVEYARD, 0, TRUE);
            DuelAnim_MoveCard(DUEL_CARD_ID(&gDuelCmd.card), &from, &to);
            gDuelCmd.step++;
            break;
        }
        DrawAllAreaTiles();     /* the card was not in the fusion deck */
        gDuelCmd.running = 0;
        break;
    default:
        AddCardToGraveyard(&gDuelCmd.card);
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_FUSION_DECK, 0);
        gDuelCmd.running = 0;
        break;
    }
}
