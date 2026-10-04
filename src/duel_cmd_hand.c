#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE / CARD_STATS_SUBTYPE */
#include "constants/card_stats.h"   /* enum CardType, enum SpellSubtype */
#include "constants/duel.h"         /* enum DuelArea, enum DuelZoneIndex */

/*
 * Duel command handlers that move cards out of and into the hand (commands 0xC2, 0xC4, 0xC5, 0xC7 and
 * 0xCA-0xCF; the hand <-> pile commands 0xC0, 0xC1 and 0xC3 are in duel_cmd_piles.c), plus three handlers
 * that no command id reaches. DuelCmd_Dispatch calls the handler of gDuelCmd.cmd once per frame until the
 * handler clears gDuelCmd.running.
 *
 * One-frame handlers update the hand directly. The others are small state machines on gDuelCmd.step:
 *   0        scroll the field to the hand (DuelScreen_ScrollToZone) or clear a zone's tiles
 *   1        take the card out of its slot into gDuelCmd.card and start the card-move animation
 *   2        put the card into its new place (most fall through into the finish)
 *   default  redraw the field and the piles and clear running
 * DuelMainStep runs the command queue only while DuelScreen_Update reports nothing in progress, so each step
 * starts after the scroll or animation of the previous one has finished.
 *
 * Hand-leaving commands clear the slot's card ID in step 1, leaving a hole; with arg4 != 0 they compact the
 * hand in step 2, otherwise the caller queues DUEL_CMD_COMPACT_HAND later. The acting player is bit 15 of
 * the command. See wiki/functions/duel-cmd-hand-c.md.
 */

/* ---- BEGIN duel.h / duel_cmd.h / duel_screen.h subset (pre-H0) ---- */
/*
 * The declarations of include/duel.h, duel_cmd.h and duel_screen.h this unit uses, with the headers' tags,
 * field names, types and bitfield containers. include/duel.h still holds the legacy header until the header
 * switch (H0, build/readability/HEADERS.md), and duel_cmd.h and duel_screen.h include it. After H0, replace
 * this block (BEGIN to END) with #include "legacy/duel.h", "duel_cmd.h" and "duel_screen.h".
 */
struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player: whose graveyard, hand or deck the card returns to */
    u32 unk13:7;
    u32 isFusionMaterial:1;         /* bit 20: one of Polymerization's materials */
    u32 unk21:11;
};

/* A card location on the duel screen: the endpoints of the card-move animation. */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13: zone within the row, hand index, 0 for the piles */
    u16 isDefense:1;                /* bit 14: drawn sideways (defense position) */
    u16 isFaceUp:1;                 /* bit 15: drawn face up, else the card back */
    u16 unk2;
};

/* One field zone (0x94 bytes): zones 0-4 hold monsters, 5-9 spells and traps, 10 the Field Magic. */
struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 unk6_2:6;
    u8 unk7[0x94 - 7];
};

struct DuelPlayer {
    u8 unk0[2];
    u8 handCount;                   /* +0x002: entries in hand[]; also the slot the next card lands in */
    u8 unk3[0x28 - 3];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84: removed from play (area 15) */
    u16 banishedInfo[80];           /* +0xCC4: parallel to banished[]: low byte enum BanishKind, high byte zone */
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

struct DuelCmd {
    u16 cmd;                        /* +0x000: enum DuelCmdId in bits 0-11, acting player in bit 15 */
    u16 arg2;                       /* +0x002 */
    u16 arg4;                       /* +0x004 */
    u16 arg6;                       /* +0x006 */
    u8 queue[0x808 - 0x8];          /* +0x008: struct DuelCmdEntry queue[256] */
    u16 queueCount;                 /* +0x808 */
    u16 step:7;                     /* +0x80A bits 0-6: handler step; 0 when a command starts */
    u16 counter:7;                  /* +0x80A bits 7-13 */
    u16 unk80A_14:2;
    u32 unk80C_0:5;                 /* +0x80C */
    u32 timer:7;                    /* +0x80C bits 5-11 */
    u32 unk80C_12:1;
    u32 running:1;                  /* +0x80D bit 5: set by the queue runner, cleared by the finished handler */
    u32 unk80C_14:18;
    u16 *hofsTable;                 /* +0x810 */
    struct DuelCard card;           /* +0x814: card saved by the current command (the moving card) */
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuelPlayers[0].zones */
extern struct DuelCmd gDuelCmd;                 /* 0x020185C0 */

void CopyDuelCard(u32 *dst, u32 *src);
void SwapDuelCards(u32 *a, u32 *b);
void PlaceMonsterCard(int player, int zone, struct DuelCard *card, u16 defense, u16 faceUp);
void PlaceSpellTrapCard(int player, int slot, struct DuelCard *card, u16 faceUp);
void SendZoneCardToGraveyard(int player, int zone);
void ReturnZoneCardToHand(int player, int zone);
void AddCardToGraveyard(struct DuelCard *card);
void AddCardToBanished(struct DuelCard *card);
void AddCardToBanishedFaceDown(struct DuelCard *card);
int RemoveBanishedCardAt(int player, int index);
void AddCardToHand(int player, struct DuelCard *card);
int RemoveCardFromHand(int player, struct DuelCard *card);
void CompactHand(int player);

void DuelScreen_ScrollToZone(u32 player, u32 area);
void DuelAnim_MoveCard(u16 cardId, struct DuelLoc *from, struct DuelLoc *to);
void ClearZoneTiles(u32 player, u32 area);
void DrawAllAreaTiles(void);
/* ---- END duel.h / duel_cmd.h / duel_screen.h subset ---- */

/* Acting player of the current command (bit 15 of cmd, read as a whole halfword and shifted). */
#define CMD_PLAYER()    (gDuelCmd.cmd >> 15)
/* The card word that arg2 (low half) and arg4 (high half) carry. */
#define CMD_CARD_WORD() ((gDuelCmd.arg4 << 16) | gDuelCmd.arg2)
/* The card the command moves. Reading a field through the pointer loads the whole word. */
#define CMD_CARD        (&gDuelCmd.card)

/*
 * Byte 2 of gDuelCmd.card (card bits 16-23), addressed as gDuelCmd + 0x816. Matching: the ROM sets
 * isFusionMaterial (card bit 20 = bit 4 of this byte) with a byte access on that address; struct DuelCard's
 * u32 container gives a word access, and a byte view of the card (base 0x814, offset 2) another address.
 */
#define CMD_CARD_BYTE2  (((u8 *)&gDuelCmd)[0x816])

/*
 * &gDuelZones[player].zones[zone] as explicit address arithmetic (0x94 = sizeof(struct DuelZone), 0xD64 =
 * sizeof(struct DuelPlayer)). Matching: the ROM adds the zone term before the player term.
 */
#define ZONE_AT(player, zone) ((struct DuelZone *)((u8 *)gDuelZones + (zone) * 0x94 + (player) * 0xD64))

/*
 * gCardStats read through its integer-constant address. Matching: with the symbol, old_agbcc loads the
 * table address before the index math, which shifts every reload register of
 * DuelCmd_PlaceSpellTrapFromHand by one.
 */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])    /* gCardStats[id] */
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS_WORD(id))                 /* enum CardType */
#define CARD_SUBTYPE(id)    CARD_STATS_SUBTYPE(CARD_STATS_WORD(id))              /* enum SpellSubtype */

/* DUEL_CMD_REMOVE_CARD_FROM_HAND (0xC2), one frame: remove the first card of the acting player's hand whose
 * whole word equals the card word, closing the gap (no animation). */
void DuelCmd_RemoveCardFromHand(void)
{
    u32 player = CMD_PLAYER();
    u32 cardWord = CMD_CARD_WORD();

    RemoveCardFromHand(player, (struct DuelCard *)&cardWord);
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_PLACE_MONSTER_FROM_HAND (0xC4): Normal Summon, Set or Special Summon of a hand card. arg2 is the
 * card ID shown by the animation; arg4 packs the monster zone (bits 0-3), the hand index (bits 4-7), face up
 * (bit 8) and defense position (bit 9). Step 1 moves hand[index] into gDuelCmd.card and animates it to the
 * zone; step 2 compacts the hand and places the card (PlaceMonsterCard).
 */
void DuelCmd_PlaceMonsterFromHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 cardId = gDuelCmd.arg2;
    u8 zone = (u8)gDuelCmd.arg4 & 0xF;
    u8 handIndex = ((u8)gDuelCmd.arg4 & 0xF0) >> 4;
    u8 faceUp = (gDuelCmd.arg4 >> 8) & 1;
    u8 defense = (u8)((gDuelCmd.arg4 >> 8) & 2) >> 1;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].hand[handIndex]);
        (&gDuelPlayers[player & 1].hand[handIndex])->id = 0;  /* a hole; word access via the pointer */
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = handIndex;
        from.isDefense = 0;
        from.isFaceUp = faceUp;
        to.player = player;
        to.area = DUEL_AREA_MONSTER;
        to.index = zone;
        to.isDefense = defense;
        to.isFaceUp = faceUp;
        DuelAnim_MoveCard(cardId, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        CompactHand(player);
        PlaceMonsterCard(player, zone, CMD_CARD, defense, faceUp);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND (0xC5): Set or activate a Magic or Trap card from the hand. arg2 is
 * the card ID; arg4 packs the zone (bits 0-3; 5-9 become the slots 0-4 of the spell/trap row), the hand
 * index (bits 4-7) and face up (bit 8). A Field Magic is animated to the field zone; PlaceSpellTrapCard
 * puts it there itself.
 */
void DuelCmd_PlaceSpellTrapFromHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 cardId = gDuelCmd.arg2;
    int slot = (u8)gDuelCmd.arg4 & 0xF;
    u8 handIndex = ((u8)gDuelCmd.arg4 & 0xF0) >> 4;
    u8 faceUp = (gDuelCmd.arg4 >> 8) & 1;

    if (slot >= ZONE_SPELL_0)       /* zone 5-9 -> slot 0-4 */
        slot -= ZONE_SPELL_0;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].hand[handIndex]);
        (&gDuelPlayers[player & 1].hand[handIndex])->id = 0;  /* a hole; word access via the pointer */
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = handIndex;
        from.isDefense = 0;
        from.isFaceUp = faceUp;
        to.player = player;
        to.area = DUEL_AREA_SPELL_TRAP;
        to.index = slot;
        to.isDefense = 0;
        to.isFaceUp = faceUp;
        if (CARD_TYPE(cardId) == CARD_TYPE_MAGIC && CARD_SUBTYPE(cardId) == SPELL_FIELD) {
            to.player = player;
            to.area = DUEL_AREA_FIELD;
            to.index = 0;
            to.isDefense = 0;
            to.isFaceUp = faceUp;
        }
        DuelAnim_MoveCard(cardId, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        CompactHand(player);
        PlaceSpellTrapCard(player, slot, CMD_CARD, faceUp);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_COMPACT_HAND (0xCA), one frame: close the holes (ID 0) that the hand-leaving commands left in the
 * acting player's hand. */
void DuelCmd_CompactHand(void)
{
    CompactHand(CMD_PLAYER());
    gDuelCmd.running = 0;
}

/* DUEL_CMD_ADD_CARD_TO_HAND (0xCB), one frame: add the card word to the acting player's hand (a Fusion
 * monster goes to the fusion deck, a token is dropped). */
void DuelCmd_AddCardToHand(void)
{
    u32 player = CMD_PLAYER();
    u32 cardWord = CMD_CARD_WORD();

    AddCardToHand(player, (struct DuelCard *)&cardWord);
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_BANISH_HAND_CARD_FACE_DOWN (0xCE, Lightforce Sword): move hand[arg2] of the acting player face
 * down to its owner's banished pile (AddCardToBanishedFaceDown: banishedInfo BANISH_FACE_DOWN). The card
 * comes back through DUEL_CMD_RETURN_BANISHED_CARD_TO_HAND.
 */
void DuelCmd_BanishHandCardFaceDown(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIndex = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].hand[handIndex]);
        (&gDuelPlayers[player & 1].hand[handIndex])->id = 0;  /* a hole; word access via the pointer */
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = handIndex;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = CMD_CARD->owner;
        to.area = DUEL_AREA_BANISHED;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 0;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        AddCardToBanishedFaceDown(CMD_CARD);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_RETURN_BANISHED_CARD_TO_HAND (0xCF, Lightforce Sword's return): animate a card back from the
 * acting player's banished pile to the next hand slot, then add banished[arg2] to the hand and remove it
 * from the pile. The animation passes card ID 1; the destination is face down, so a card back is drawn.
 */
void DuelCmd_ReturnBanishedCardToHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 index = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        from.player = player;
        from.area = DUEL_AREA_BANISHED;
        from.index = 0;
        from.isDefense = 0;
        from.isFaceUp = 0;
        to.player = player;
        to.area = DUEL_AREA_HAND;
        to.index = gDuelPlayers[player & 1].handCount;
        to.isDefense = 0;
        to.isFaceUp = 0;
        DuelAnim_MoveCard(1, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        AddCardToHand(player, &gDuelPlayers[player].banished[index]);
        RemoveBanishedCardAt(player, index);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_EXCHANGE_HAND_CARDS (0xC7, Exchange): animate the acting player's hand[arg2] to the opponent's
 * hand slot arg4 (step 1) and the opponent's hand[arg4] back to slot arg2 (step 2), both face down with card
 * ID 1, then swap the two hand cards.
 * Matching: from/to stay in registers until their address is first taken, so step 1 stores them as whole
 * words and step 2 field by field (byte/halfword stores), as in the ROM. Both steps use the same two locals.
 */
void DuelCmd_ExchangeHandCards(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = gDuelCmd.arg2;
        from.isDefense = 0;
        from.isFaceUp = 0;
        to.player = 1 - player;
        to.area = DUEL_AREA_HAND;
        to.index = gDuelCmd.arg4;
        to.isDefense = 0;
        to.isFaceUp = 0;
        DuelAnim_MoveCard(1, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        from.player = 1 - player;
        from.area = DUEL_AREA_HAND;
        from.index = gDuelCmd.arg4;
        from.isDefense = 0;
        from.isFaceUp = 0;
        to.player = player;
        to.area = DUEL_AREA_HAND;
        to.index = gDuelCmd.arg2;
        to.isDefense = 0;
        to.isFaceUp = 0;
        DuelAnim_MoveCard(1, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        SwapDuelCards((u32 *)&gDuelPlayers[player & 1].hand[gDuelCmd.arg2],
                      (u32 *)&gDuelPlayers[(1 - player) & 1].hand[gDuelCmd.arg4]);
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_SEND_HAND_FUSION_MATERIAL_TO_GRAVEYARD (0xCC): Polymerization's hand materials. Like
 * DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD (hand[arg2] to its owner's graveyard, compact the hand if arg4 != 0),
 * but the card's isFusionMaterial bit is set before it enters the graveyard.
 */
void DuelCmd_SendHandFusionMaterialToGraveyard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIndex = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].hand[handIndex]);
        (&gDuelPlayers[player & 1].hand[handIndex])->id = 0;  /* a hole; word access via the pointer */
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = handIndex;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = CMD_CARD->owner;
        to.area = DUEL_AREA_GRAVEYARD;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        CMD_CARD_BYTE2 |= 0x10;    /* CMD_CARD->isFusionMaterial = 1 */
        AddCardToGraveyard(CMD_CARD);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_BANISH_HAND_FUSION_MATERIAL (0xCD): DuelCmd_SendHandFusionMaterialToGraveyard, but to the owner's
 * banished pile (Polymerization sends it for card number 1547, a banish-the-materials fusion not in EDS).
 */
void DuelCmd_BanishHandFusionMaterial(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIndex = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].hand[handIndex]);
        (&gDuelPlayers[player & 1].hand[handIndex])->id = 0;  /* a hole; word access via the pointer */
        from.player = player;
        from.area = DUEL_AREA_HAND;
        from.index = handIndex;
        from.isDefense = 0;
        from.isFaceUp = 1;
        to.player = CMD_CARD->owner;
        to.area = DUEL_AREA_BANISHED;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        CMD_CARD_BYTE2 |= 0x10;    /* CMD_CARD->isFusionMaterial = 1 */
        AddCardToBanished(CMD_CARD);
        /* fall through */
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Unreferenced handler (no command id reaches it; the dispatcher has no case 0xC6, 0xC8 or 0xC9): return a
 * card from the acting player's spell/trap row to its owner's hand. arg2 <= 9: slot arg2 of the row, zone
 * arg2 + 5; above 9: zone arg2 (the field zone). Step 0 clears the zone's tiles, step 1 animates the card to
 * the acting player's hand row at the owner's handCount, step 2 moves it (ReturnZoneCardToHand).
 * Matching: the zone flags are read through (&zones[i])->isDefense, a pointer sum expanded as an address, so
 * it does not share its address with the CopyDuelCard argument; both reads sit inside the branches.
 */
void DuelCmd_ReturnSpellTrapToHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    int slot = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        if (slot <= 9)
            ClearZoneTiles(player, slot + ZONE_SPELL_0);
        else
            ClearZoneTiles(player, ZONE_FIELD);
        gDuelCmd.step++;
        break;
    case 1:
        if (slot <= 9) {
            CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].zones[slot + ZONE_SPELL_0].card);
            from.player = player;
            from.area = DUEL_AREA_SPELL_TRAP;
            from.index = slot;
            from.isDefense = (&gDuelPlayers[player & 1].zones[slot + ZONE_SPELL_0])->isDefense;
            from.isFaceUp = (&gDuelPlayers[player & 1].zones[slot + ZONE_SPELL_0])->isFaceUp;
        } else {
            CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].zones[slot].card);
            from.player = player;
            from.area = DUEL_AREA_FIELD;
            from.index = 0;
            from.isDefense = (&gDuelPlayers[player & 1].zones[slot])->isDefense;
            from.isFaceUp = (&gDuelPlayers[player & 1].zones[slot])->isFaceUp;
        }
        to.player = player;
        to.area = DUEL_AREA_HAND;
        to.index = gDuelPlayers[CMD_CARD->owner].handCount;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (slot <= 9)
            ReturnZoneCardToHand(player, slot + ZONE_SPELL_0);
        else
            ReturnZoneCardToHand(player, slot);
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* Unreferenced handler, one frame: clear the card ID of zone (arg2 & 7) of the acting player, without
 * redrawing or removing its links. */
void DuelCmd_ClearZoneCardNoRedraw(void)
{
    ZONE_AT(CMD_PLAYER(), gDuelCmd.arg2 & 7)->card.id = 0;
    gDuelCmd.running = 0;
}

/*
 * Unreferenced handler: send a card from the acting player's spell/trap row (arg2 as in
 * DuelCmd_ReturnSpellTrapToHand) to its owner's graveyard (SendZoneCardToGraveyard), animated to the acting
 * player's graveyard. The zone is cleared before its flags are read, so the animation always starts face
 * down in attack position (as in the ROM).
 */
void DuelCmd_SendSpellTrapToGraveyard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    int slot = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        if (slot <= 9)
            ClearZoneTiles(player, slot + ZONE_SPELL_0);
        else
            ClearZoneTiles(player, ZONE_FIELD);
        gDuelCmd.step++;
        break;
    case 1:
        if (slot <= 9) {
            CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].zones[slot + ZONE_SPELL_0].card);
            SendZoneCardToGraveyard(player, slot + ZONE_SPELL_0);
            from.player = player;
            from.area = DUEL_AREA_SPELL_TRAP;
            from.index = slot;
            from.isDefense = (&gDuelPlayers[player & 1].zones[slot + ZONE_SPELL_0])->isDefense;
            from.isFaceUp = (&gDuelPlayers[player & 1].zones[slot + ZONE_SPELL_0])->isFaceUp;
        } else {
            CopyDuelCard((u32 *)CMD_CARD, (u32 *)&gDuelPlayers[player & 1].zones[slot].card);
            SendZoneCardToGraveyard(player, slot);
            from.player = player;
            from.area = DUEL_AREA_FIELD;
            from.index = 0;
            from.isDefense = (&gDuelPlayers[player & 1].zones[slot])->isDefense;
            from.isFaceUp = (&gDuelPlayers[player & 1].zones[slot])->isFaceUp;
        }
        to.player = player;
        to.area = DUEL_AREA_GRAVEYARD;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
