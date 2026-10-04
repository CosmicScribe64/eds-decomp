#include "global.h"
#include "util.h"                   /* MemClear16, MemCopy16 */
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_TOKEN_FIRST / END */
#include "constants/duel.h"
#include "constants/sound.h"

/*
 * Duel commands that change or move a card on the field (wiki/functions/duel-cmd-field-c.md):
 * DUEL_CMD_SEND_TO_GRAVEYARD .. DUEL_CMD_BANISH_FLAGGED (0x79-0x7B), DUEL_CMD_CHANGE_POSITION, FLIP_CARD
 * (0x7E, 0x7F), RETURN_TO_HAND, RETURN_TO_DECK, MOVE_TO_ZONE (0x80-0x82) and SWAP_ZONES (0x84).
 * DuelCmd_Dispatch calls the handler of the running command once per frame. Each handler is a small state
 * machine on gDuelCmd.step: scroll the field to the zone, play the animation, then change the duel state,
 * redraw the field and clear gDuelCmd.running. A card leaving the field is saved in gDuelCmd.card first, and
 * its move to the pile is animated unless it is a token.
 */

/* ---- BEGIN header subset (pre-H0) ---- */
/*
 * The parts of duel.h, duel_screen.h, duel_cmd.h and sound.h this unit uses, with the headers' tags, names,
 * types and bitfield containers. include/duel.h and include/sound.h still hold the legacy headers until the
 * header switch (H0, build/readability/HEADERS.md), and duel_screen.h and duel_cmd.h include duel.h. After
 * H0, replace this block (BEGIN to END) with:
 *     #include "legacy/duel.h"
 *     #include "duel_cmd.h"
 *     #include "duel_screen.h"
 *     #include "legacy/sound.h"
 */

/* duel.h */
struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player: whose graveyard, hand or deck the card returns to */
    u32 unk13:19;
};

struct DuelCardStatusBytes {
    u8 cardIdLow;                   /* +0x00: card word bits 0-7 */
    u8 cardWordBits8to13:6;         /* +0x01: id (high bits), owner, unk13 */
    u8 unk14:1;                     /* +0x01 bit 6 = card bit 14 */
    u8 normalSummoned:1;            /* bit 15 */
    u8 specialSummoned:1;           /* +0x02 bit 0 = card bit 16 */
    u8 planted:1;                   /* bit 17 */
    u8 graverobbed:1;               /* bit 18 */
    u8 unk19:1;                     /* bit 19 */
    u8 isFusionMaterial:1;          /* bit 20 */
    u8 destroyedInBattle:1;         /* bit 21 */
    u8 destroyedByOpponent:1;       /* bit 22 */
    u8 flag23:1;                    /* bit 23 */
    u8 pendingEquip:1;              /* +0x03 bit 0 = card bit 24 */
    u8 equipZone:3;                 /* bits 25-27 */
    u8 pendingOpponentSummon:1;     /* bit 28 */
    u8 unk29:3;
    u8 restOfZone[0x94 - 4];
};

struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13: zone within the row, hand index, 0 for the piles */
    u16 isDefense:1;                /* bit 14: drawn sideways (defense position) */
    u16 isFaceUp:1;                 /* bit 15: drawn face up, else the card back */
    u16 unk2;                       /* +0x02: padding, copied with the word */
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04: gDuel.serial when the card was placed (replay check) */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 unk6_2:6;
    u8 unk7_0:2;
    u8 positionLocked:1;            /* +0x07 bit 2: cannot change position (set when the monster attacked) */
    u8 unk7_3:5;
    u8 unk8[0x94 - 0x8];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[]; also the slot the next card lands in */
    u8 unk3[0xD64 - 0x3];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

u32 IsFusionMonster(u16 cardId);
void CopyDuelCard(u32 *dst, u32 *src);
void ClearZoneCardStatusFlags(u32 player, u32 zone);
void SendZoneCardToGraveyard(int player, int zone);
void BanishZoneCard(int player, int zone);
void ReturnZoneCardToHand(int player, int zone);
void ReturnZoneCardToDeck(int player, int zone);

/* duel_screen.h */
enum DuelAnimKind {
    DUEL_ANIM_CHANGE_POSITION = 1,  /* quarter turn between attack and defense, optionally flipping */
    DUEL_ANIM_FLIP = 2,             /* card flips face up or down in its zone */
    DUEL_ANIM_MOVE_CARD = 3,        /* one card moves between two locations (DuelAnim_MoveCard) */
    DUEL_ANIM_SWAP_CARDS = 4,       /* two face-down cards swap places (DuelAnim_SwapCards) */
    DUEL_ANIM_ZONE_EFFECT = 5       /* sprite animation stream on a zone (DuelAnim_PlayZoneEffect) */
};
void DuelScreen_StartScroll(u32 target);
void DuelScreen_ScrollToZone(u32 player, u32 area);
void DuelCursor_Select(s32 player, s32 area, s32 index);
void DuelAnim_Request(u16 kind, u32 arg);
void DuelAnim_MoveCard(u16 cardId, struct DuelLoc *from, struct DuelLoc *to);
void DuelAnim_SwapCards(struct DuelLoc *a, struct DuelLoc *b);
void DuelAnim_PlayZoneEffect(struct DuelLoc *loc, u32 anim, u32 dx, u32 dy);
void ClearZoneTiles(u32 player, u32 area);
void DrawAllAreaTiles(void);
int GetZoneArea(int zone);

/* duel_cmd.h (the fields used here) */
struct DuelCmdEntry {
    u16 cmd;
    u16 arg2;
    u16 arg4;
    u16 arg6;
};
struct DuelCmd {
    u16 cmd;                        /* +0x000: enum DuelCmdId in bits 0-11, acting player in bit 15 */
    u16 arg2;                       /* +0x002: first operand */
    u16 arg4;                       /* +0x004: second operand */
    u16 arg6;                       /* +0x006: third operand */
    struct DuelCmdEntry queue[256]; /* +0x008 */
    u16 queueCount;                 /* +0x808 */
    u16 step:7;                     /* +0x80A bits 0-6: handler step; 0 when a command starts */
    u16 counter:7;
    u16 unk80A_14:2;
    u32 unk80C_0:5;                 /* +0x80C */
    u32 timer:7;
    u32 unk80C_12:1;
    u32 running:1;                  /* +0x80D bit 5: cleared by the finished handler */
    u32 unk80C_14:18;
    u16 *hofsTable;                 /* +0x810 */
    struct DuelCard card;           /* +0x814: card saved by the current command (the moving card) */
};
extern struct DuelCmd gDuelCmd;                 /* 0x020185C0 */

/* sound.h */
void PlaySE(u32 seId);
/* ---- END header subset ---- */

/*
 * Matching: DuelCmd_SendToGraveyard and DuelCmd_Banish call DuelAnim_MoveCard with a u32 card ID; the
 * header's u16 parameter moves the argument set-up one instruction earlier.
 */
extern void DuelAnim_MoveCard32(u32 cardId, struct DuelLoc *from, struct DuelLoc *to) asm("DuelAnim_MoveCard");

extern const u8 gExplosionAnim[];   /* 8-frame 32x32 explosion: sprite animation stream played on a zone */

/* Acting player of the running command: bit 15 of the command word (DUEL_CMD_PLAYER). */
#define CMD_ACTING_PLAYER() (gDuelCmd.cmd >> 15)

/* Card ID of the card word at zone (a zone, whose card word comes first, or a card). Matching: the ROM loads
 * the whole word (ldr and shifts); a member access (zone->card.id) loads only the halfword. */
#define ZONE_CARD_ID(zone) (((struct DuelCard *)(zone))->id)

/* Card number of a card ID, read through the integer-constant address of gCardIdToNumber. Matching: the ROM
 * forms the address after the index math (with the symbol, before). Same table, same bytes. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/* Card numbers 1920-1999 are monster tokens: they vanish instead of moving to a pile. */
#define IS_TOKEN_NUMBER(no) ((u16)((no) - CARD_NUMBER_TOKEN_FIRST) < CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST)

/* gDuel.serial (+0x0000) and gDuelPlayers (gDuel + 4) reached from gDuelZones (gDuel + 0x2C). Matching: the
 * ROM addresses them from the gDuelZones literal it already holds. */
#define DUEL_SERIAL_FROM_ZONES   (*(u16 *)((u8 *)gDuelZones - 0x2C))
#define PLAYERS_FROM_ZONES_OFFSET 0x28

/*
 * DUEL_CMD_CHANGE_POSITION (arg2: zone, arg4: also turn face up): switch a monster between attack and defense
 * position. Ends at once if the zone is empty. Steps: 0 scroll to the zone, 1 start the quarter-turn
 * animation, then toggle isDefense (and turn the card face up if arg4 asks), redraw and select the zone.
 */
void DuelCmd_ChangePosition(void)
{
    u32 player = CMD_ACTING_PLAYER();
    /* FAKEMATCH: slot pinned to r5, the ROM's register for it. */
    register u32 slot asm("r5") = gDuelCmd.arg2;
    u32 turnFaceUp = gDuelCmd.arg4;
    struct DuelZonesPlayer *zones = &gDuelZones[player];
    struct DuelZone *zone = &zones->zones[slot];

    if (ZONE_CARD_ID(zone) == 0) {
        gDuelCmd.running = 0;
        return;
    }
    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, GetZoneArea(slot));
        gDuelCmd.step++;
        break;
    case 1: {
        /* struct DuelAnimZoneArg: player, slot, isDefense before the turn, turn face up too. */
        u32 loc = (u8)slot << 8 | player;
        u32 isDefense = zone->isDefense;
        u32 extra = turnFaceUp << 24;

        DuelAnim_Request(DUEL_ANIM_CHANGE_POSITION, loc | (isDefense << 16 | extra));
        gDuelCmd.step++;
        break;
    }
    default:
        zone->isDefense = !zone->isDefense;
        if (turnFaceUp && !zone->isFaceUp)
            zone->isFaceUp = 1;
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_MONSTER, slot);
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_FLIP_CARD (arg2: zone): turn a card face up or face down. Ends at once if the zone is empty.
 * Steps: 0 scroll to the zone, 1 start the flip animation, then toggle isFaceUp; a card turned face up gets
 * the next gDuel.serial, so "newest card wins" rules follow the order cards were turned face up.
 */
void DuelCmd_FlipCard(void)
{
    u32 player = CMD_ACTING_PLAYER();
    /* FAKEMATCH: slot pinned to r6, the ROM's register for it. */
    register u32 slot asm("r6") = gDuelCmd.arg2;
    struct DuelZonesPlayer *zones = &gDuelZones[player];
    u32 offset = 0x94 * slot;
    struct DuelZone *zone = (struct DuelZone *)((u8 *)zones + offset);

    offset += player * 0xD64;
    if (ZONE_CARD_ID((u8 *)gDuelZones + offset) == 0) {
        gDuelCmd.running = 0;
        return;
    }
    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, GetZoneArea(slot));
        gDuelCmd.step++;
        break;
    case 1:
        /* struct DuelAnimZoneArg: player, slot, isDefense, isFaceUp before the flip. */
        DuelAnim_Request(DUEL_ANIM_FLIP, ((u8)slot << 8 | player) | ((zone->isDefense | zone->isFaceUp << 8) << 16));
        gDuelCmd.step++;
        break;
    default:
        zone->isFaceUp = !zone->isFaceUp;
        if (zone->isFaceUp)
            zone->serial = DUEL_SERIAL_FROM_ZONES++;
        DuelCursor_Select(player, DUEL_AREA_MONSTER, slot);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_SEND_TO_GRAVEYARD (arg2: zone, arg4 low byte: play the explosion). Steps: 0 scroll to the zone (an
 * empty zone also clears running, but the step still runs; the dispatcher stops calling the handler), 1 the
 * explosion and SE_DESTROY if asked, clear the zone's cell, 2 save the card in gDuelCmd.card, clear its
 * graverobbed bit, SendZoneCardToGraveyard, and animate the move to the owner's graveyard (not for a token),
 * then redraw and select the zone.
 */
void DuelCmd_SendToGraveyard(void)
{
    struct DuelLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = CMD_ACTING_PLAYER();
    u32 slot = cmd->arg2;

    switch (cmd->step) {
    case 0:
        if (ZONE_CARD_ID(player * 0xD64 + slot * 0x94 + (u32)gDuelZones) == 0) {
            DrawAllAreaTiles();
            cmd->running = 0;
        }
        DuelScreen_ScrollToZone(player, GetZoneArea(slot));
        cmd->step++;
        break;
    case 1:
        if (*(u8 *)&cmd->arg4) {
            struct DuelZone *zone;
            u32 playerBit;

            PlaySE(SE_DESTROY);
            from.player = player;
            from.area = DUEL_AREA_MONSTER;
            from.index = slot;
            playerBit = player & 1;
            zone = (struct DuelZone *)(slot * 0x94 + playerBit * 0xD64 + (u32)gDuelZones);
            from.isDefense = zone->isDefense;
            from.isFaceUp = zone->isFaceUp;
            DuelAnim_PlayZoneEffect(&from, (u32)gExplosionAnim, 0, 0);
        }
        ClearZoneTiles(player, slot);
        cmd->step++;
        break;
    case 2: {
        struct DuelCard *saved = &cmd->card;
        u32 playerOffset = (player & 1) * 0xD64;
        u8 *zones = (u8 *)gDuelZones;
        u8 *playerZones = zones + playerOffset;
        u32 slotOffset = slot * 0x94;
        struct DuelZone *zone;
        struct DuelCard card;
        u32 cardId;

        CopyDuelCard((u32 *)saved, (u32 *)(playerZones + slotOffset));
        zone = (struct DuelZone *)(slotOffset + playerOffset + (u32)zones);
        ((struct DuelCardStatusBytes *)zone)->graverobbed = 0;
        SendZoneCardToGraveyard(player, slot);
        card = *saved;
        cardId = card.id;
        if (!IS_TOKEN_NUMBER(CARD_NUMBER(card.id))) {
            from.player = player;
            from.area = DUEL_AREA_MONSTER;
            from.index = slot;
            from.isDefense = zone->isDefense;
            from.isFaceUp = zone->isFaceUp;
            to.player = card.owner;
            to.area = DUEL_AREA_GRAVEYARD;
            to.index = 0;
            to.isDefense = 0;
            to.isFaceUp = 1;
            DuelAnim_MoveCard32(cardId, &from, &to);
        }
        cmd->step++;
        break;
    }
    default:
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_MONSTER, slot);
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_BANISH (arg2: zone). Steps: 0 stop if the zone is empty, else scroll to it; 1 SE_BANISH and the
 * explosion, clear the zone's cell; 2 save the card in gDuelCmd.card, BanishZoneCard, and animate the move to
 * the owner's banished pile (not for a token); then select the zone and redraw.
 * Reusing `from` across the steps keeps the byte stores after its address escapes.
 */
void DuelCmd_Banish(void)
{
    struct DuelLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = CMD_ACTING_PLAYER();
    u32 slot = cmd->arg2;

    switch (cmd->step) {
    case 0:
        if (ZONE_CARD_ID(player * 0xD64 + slot * 0x94 + (u32)gDuelZones) == 0)
            cmd->running = 0;
        else {
            DuelScreen_ScrollToZone(player, GetZoneArea(slot));
            cmd->step++;
        }
        break;
    case 1: {
        struct DuelZone *zone;
        u32 playerBit;

        PlaySE(SE_BANISH);
        from.player = player;
        from.area = DUEL_AREA_MONSTER;
        from.index = slot;
        playerBit = player & 1;
        zone = (struct DuelZone *)(slot * 0x94 + playerBit * 0xD64 + (u32)gDuelZones);
        from.isDefense = zone->isDefense;
        from.isFaceUp = zone->isFaceUp;
        DuelAnim_PlayZoneEffect(&from, (u32)gExplosionAnim, 0, 0);
        ClearZoneTiles(player, slot);
        cmd->step++;
        break;
    }
    case 2: {
        struct DuelCard *saved = &cmd->card;
        u32 playerOffset = (player & 1) * 0xD64;
        u8 *playerZones = (u8 *)gDuelZones + playerOffset;
        u32 slotOffset = slot * 0x94;
        struct DuelCard card;
        u32 cardId;

        CopyDuelCard((u32 *)saved, (u32 *)(playerZones + slotOffset));
        BanishZoneCard(player, slot);
        card = *saved;
        cardId = card.id;
        if (!IS_TOKEN_NUMBER(CARD_NUMBER(card.id))) {
            struct DuelZone *zone;

            from.player = player;
            from.area = DUEL_AREA_MONSTER;
            from.index = slot;
            zone = (struct DuelZone *)(slotOffset + playerOffset + (u32)gDuelZones);
            from.isDefense = zone->isDefense;
            from.isFaceUp = zone->isFaceUp;
            to.player = card.owner;
            to.area = DUEL_AREA_BANISHED;
            to.index = 0;
            to.isDefense = 0;
            to.isFaceUp = 1;
            DuelAnim_MoveCard32(cardId, &from, &to);
        }
        cmd->step++;
        break;
    }
    default:
        DuelCursor_Select(player, DUEL_AREA_MONSTER, slot);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_BANISH_FLAGGED (arg2: zone): DuelCmd_Banish, but step 2 first sets the card's isFusionMaterial bit
 * (card bit 20) and always animates the move, to the acting player's banished pile, tokens included. Only
 * used by the effect of card number 1547 (not an EDS card).
 */
void DuelCmd_BanishFlagged(void)
{
    struct DuelLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = CMD_ACTING_PLAYER();
    u32 slot = cmd->arg2;

    switch (cmd->step) {
    case 0: {
        u32 slotOffset = slot * 0x94;
        u32 playerOffset = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(slotOffset + playerOffset + (u32)gDuelZones);

        if (card->id == 0)
            cmd->running = 0;
        else {
            DuelScreen_ScrollToZone(player, GetZoneArea(slot));
            cmd->step++;
        }
        break;
    }
    case 1: {
        struct DuelZone *zone;
        u32 playerBit;

        PlaySE(SE_BANISH);
        from.player = player;
        from.area = DUEL_AREA_MONSTER;
        from.index = slot;
        playerBit = player & 1;
        zone = (struct DuelZone *)(slot * 0x94 + playerBit * 0xD64 + (u32)gDuelZones);
        from.isDefense = zone->isDefense;
        from.isFaceUp = zone->isFaceUp;
        DuelAnim_PlayZoneEffect(&from, (u32)gExplosionAnim, 0, 0);
        ClearZoneTiles(player, slot);
        cmd->step++;
        break;
    }
    case 2: {
        u32 playerBit = player & 1;
        u32 slotOffset = slot * 0x94;
        u32 playerOffset = playerBit * 0xD64;
        u32 zoneOffset = slotOffset + playerOffset;
        u8 *zones = (u8 *)gDuelZones;
        struct DuelZone *zone = (struct DuelZone *)(zoneOffset + (u32)zones);
        struct DuelCard *saved;

        ((struct DuelCardStatusBytes *)zone)->isFusionMaterial = 1;
        saved = &cmd->card;
        CopyDuelCard((u32 *)saved, (u32 *)(playerOffset + (u32)zones + slotOffset));
        BanishZoneCard(player, slot);
        from.player = player;
        from.area = DUEL_AREA_MONSTER;
        from.index = slot;
        from.isDefense = zone->isDefense;
        from.isFaceUp = zone->isFaceUp;
        to.player = player;
        to.area = DUEL_AREA_BANISHED;
        to.index = 0;
        to.isDefense = 0;
        to.isFaceUp = 1;
        DuelAnim_MoveCard(saved->id, &from, &to);
        cmd->step++;
        break;
    }
    default:
        DuelCursor_Select(player, DUEL_AREA_MONSTER, slot);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_RETURN_TO_HAND (arg2: zone). Steps: 0 stop if the zone is empty, else scroll to it; 1 save the
 * card in gDuelCmd.card, clear its status bits and the zone's cell, and animate the move (not for a token)
 * to the owner's fusion deck for a Fusion monster, else to the end of the owner's hand; then
 * ReturnZoneCardToHand, redraw and select the zone.
 */
void DuelCmd_ReturnToHand(void)
{
    struct DuelLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = CMD_ACTING_PLAYER();
    u32 slot = cmd->arg2;

    switch (cmd->step) {
    case 0: {
        u32 slotOffset = slot * 0x94;
        u32 playerOffset = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(slotOffset + playerOffset + (u32)gDuelZones);

        if (card->id == 0) {
            cmd->running = 0;
            break;
        }
        DuelScreen_ScrollToZone(player, GetZoneArea(slot));
        cmd->step++;
        break;
    }
    case 1: {
        struct DuelCard *saved = &cmd->card;
        u32 playerOffset = (player & 1) * 0xD64;
        u8 *zones = (u8 *)gDuelZones;
        u8 *playerZones = zones + playerOffset;
        u32 slotOffset = slot * 0x94;
        struct DuelZone *zone;
        struct DuelCard card;
        u32 handIndex;

        CopyDuelCard((u32 *)saved, (u32 *)(playerZones + slotOffset));
        ClearZoneCardStatusFlags(player, slot);
        ClearZoneTiles(player, slot);
        card = *saved;
        if (!IS_TOKEN_NUMBER(CARD_NUMBER(card.id))) {
            from.player = player;
            from.area = DUEL_AREA_MONSTER;
            from.index = slot;
            zone = (struct DuelZone *)(slotOffset + playerOffset + (u32)zones);
            from.isDefense = zone->isDefense;
            from.isFaceUp = zone->isFaceUp;
            to.player = card.owner;
            to.area = IsFusionMonster(cmd->card.id) ? DUEL_AREA_FUSION_DECK : DUEL_AREA_HAND;
            if (IsFusionMonster(cmd->card.id) == 0) {
                /* FAKEMATCH: players (gDuelPlayers, reached as gDuelZones - 0x28) pinned to r1, the ROM's
                 * register for it, while the owner is read into r0. */
                register u8 *players asm("r1") = zones - PLAYERS_FROM_ZONES_OFFSET;
                u32 owner = saved->owner;

                handIndex = ((struct DuelPlayer *)players)[owner & 1].handCount;
            } else
                handIndex = 0;
            to.index = handIndex;
            to.isDefense = 0;
            to.isFaceUp = ((struct DuelZone *)((player & 1) * 0xD64 + slot * 0x94 + (u32)gDuelZones))->isFaceUp;
            DuelAnim_MoveCard(ZONE_CARD_ID(&gDuelCmd.card), &from, &to);
        }
        gDuelCmd.step++;
        break;
    }
    default:
        ReturnZoneCardToHand(player, slot);
        DrawAllAreaTiles();
        DuelCursor_Select(player, DUEL_AREA_MONSTER, slot);
        cmd->running = 0;
        break;
    }
}

/*
 * DUEL_CMD_RETURN_TO_DECK (arg2: zone): put the card on top of its owner's deck (a Fusion monster into the
 * fusion deck). Steps: 0 stop if the zone is empty, else scroll to it; 1 save the card in gDuelCmd.card,
 * clear its graverobbed bit and the zone's cell, and animate the move to the deck or fusion deck (not for a
 * token); then clear its status bits, ReturnZoneCardToDeck, select the zone and redraw.
 */
void DuelCmd_ReturnToDeck(void)
{
    struct DuelLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 player = CMD_ACTING_PLAYER();
    u32 slot = cmd->arg2;

    switch (cmd->step) {
    case 0: {
        u32 slotOffset = slot * 0x94;
        u32 playerOffset = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(slotOffset + playerOffset + (u32)gDuelZones);

        if (card->id == 0)
            cmd->running = 0;
        else {
            DuelScreen_ScrollToZone(player, GetZoneArea(slot));
            cmd->step++;
        }
        break;
    }
    case 1: {
        struct DuelCard *saved = &cmd->card;
        u32 playerOffset = (player & 1) * 0xD64;
        u8 *zones = (u8 *)gDuelZones;
        u8 *playerZones = zones + playerOffset;
        u32 slotOffset = slot * 0x94;
        struct DuelZone *zone;
        struct DuelCard card;

        CopyDuelCard((u32 *)saved, (u32 *)(playerZones + slotOffset));
        zone = (struct DuelZone *)(slotOffset + playerOffset + (u32)zones);
        ((struct DuelCardStatusBytes *)zone)->graverobbed = 0;
        ClearZoneTiles(player, slot);
        card = *saved;
        if (!IS_TOKEN_NUMBER(CARD_NUMBER(card.id))) {
            from.player = player;
            from.area = DUEL_AREA_MONSTER;
            from.index = slot;
            from.isDefense = zone->isDefense;
            from.isFaceUp = zone->isFaceUp;
            to.player = card.owner;
            to.area = IsFusionMonster(cmd->card.id) ? DUEL_AREA_FUSION_DECK : DUEL_AREA_DECK;
            to.index = 0;
            to.isDefense = 0;
            to.isFaceUp = 1;
            DuelAnim_MoveCard(saved->id, &from, &to);
        }
        gDuelCmd.step++;
        break;
    }
    default:
        ClearZoneCardStatusFlags(player, slot);
        ReturnZoneCardToDeck(player, slot);
        DuelCursor_Select(player, DUEL_AREA_MONSTER, slot);
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    }
}

/*
 * DUEL_CMD_MOVE_TO_ZONE (arg2: source loc, arg4: destination loc, both player | zone << 8): move a whole zone
 * record (card, links, counters, serial) to another zone, e.g. when a monster changes control. Steps: 0 stop
 * if the source is empty, else scroll to it; 1 save the card in gDuelCmd.card, clear the source cell and
 * animate the move (the destination keeps the source's position and face; the moving card is drawn upright
 * when the destination is a spell/trap zone); 2 copy the source zone to the destination, write the saved card
 * word into it, clear the source zone and the destination's positionLocked, select the destination, redraw.
 */
void DuelCmd_MoveToZone(void)
{
    struct DuelLoc from, to;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 srcPlayer = (u8)cmd->arg2;
    u32 srcSlot = cmd->arg2 >> 8;
    u32 dstPlayer = (u8)cmd->arg4;
    int dstSlot = cmd->arg4 >> 8;

    switch (gDuelCmd.step) {
    case 0:
        if (ZONE_CARD_ID((srcPlayer & 1) * 0xD64 + srcSlot * 0x94 + (u32)gDuelZones) == 0) {
            cmd->running = 0;
        } else {
            DuelScreen_ScrollToZone(srcPlayer, GetZoneArea(srcSlot));
            gDuelCmd.step++;
        }
        break;
    case 1: {
        struct DuelCard *saved = &cmd->card;
        u32 playerOffset = (srcPlayer & 1) * 0xD64;
        u8 *zones = (u8 *)gDuelZones;
        u8 *playerZones = zones + playerOffset;
        u32 slotOffset = srcSlot * 0x94;
        struct DuelZone *zone;

        CopyDuelCard((u32 *)saved, (u32 *)(playerZones + slotOffset));
        ClearZoneTiles(srcPlayer, srcSlot);
        from.player = srcPlayer;
        from.area = DUEL_AREA_MONSTER;
        from.index = srcSlot;
        zone = (struct DuelZone *)(slotOffset + playerOffset + (u32)zones);
        from.isDefense = zone->isDefense;
        from.isFaceUp = zone->isFaceUp;
        to.player = dstPlayer;
        to.area = DUEL_AREA_MONSTER;
        to.index = dstSlot;
        to.isDefense = from.isDefense;
        to.isFaceUp = from.isFaceUp;
        if (dstSlot > ZONE_MONSTER_4)
            from.isDefense = 0;
        DuelAnim_MoveCard(saved->id, &from, &to);
        gDuelCmd.step++;
        break;
    }
    default: {
        u8 *zones = (u8 *)gDuelZones;
        u32 dstPlayerOffset = (dstPlayer & 1) * 0xD64;
        u8 *dstPlayerZones = zones + dstPlayerOffset;
        u32 dstSlotOffset = dstSlot * 0x94;
        struct DuelZone *dst = (struct DuelZone *)(dstPlayerZones + dstSlotOffset);
        u32 srcPlayerOffset = (srcPlayer & 1) * 0xD64;
        u8 *srcPlayerZones = zones + srcPlayerOffset;
        struct DuelZone *src = (struct DuelZone *)(srcPlayerZones + srcSlot * 0x94);

        MemCopy16(dst, src, sizeof(struct DuelZone));
        CopyDuelCard((u32 *)&dst->card, (u32 *)&cmd->card);
        MemClear16(src, sizeof(struct DuelZone));
        ((struct DuelZone *)(dstPlayerOffset + dstSlotOffset + (u32)zones))->positionLocked = 0;
        DuelCursor_Select(dstPlayer, DUEL_AREA_MONSTER, dstSlot);
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    }
    }
}

/*
 * Zone address forms of DuelCmd_SwapZones. The add order picks the ROM's evaluation order: a local `zones`
 * keeps (zones + p * 0xD64) + s * 0x94 from being reassociated, and in a memory address the second product
 * is emitted first.
 */
#define SWAP_ZONE(p, s)        ((struct DuelZone *)(zones + (p) * 0xD64 + (s) * 0x94))
#define SWAP_ZONE_GLOBAL(p, s) ((struct DuelZone *)((p) * 0xD64 + (s) * 0x94 + (u32)gDuelZones))

/*
 * DUEL_CMD_SWAP_ZONES (arg2, arg4: the two locs, player | zone << 8): swap two zone records. Steps: 0 stop
 * unless both zones hold a card, else scroll; 1 save the first card in gDuelCmd.card, clear both cells and
 * start the swap animation; 2 swap the two 0x94-byte zones through a copy on the stack and redraw.
 */
void DuelCmd_SwapZones(void)
{
    struct DuelLoc from;
    struct DuelLoc to;
    struct DuelZone tmp;
    struct DuelCmd *cmd = &gDuelCmd;
    u32 playerA = (u8)cmd->arg2;
    u32 slotA = cmd->arg2 >> 8;
    u32 playerB = (u8)cmd->arg4;
    int slotB = cmd->arg4 >> 8;

    switch (gDuelCmd.step) {
    case 0:
        /* Two separate ifs (cross-jumped later) give cmd the extra reference that puts it in r9 ahead of
         * slotA. */
        if (ZONE_CARD_ID(SWAP_ZONE_GLOBAL(playerA & 1, slotA)) == 0) {
            cmd->running = 0;
            break;
        }
        if (ZONE_CARD_ID(SWAP_ZONE_GLOBAL(playerB & 1, slotB)) == 0) {
            cmd->running = 0;
            break;
        }
        DuelScreen_StartScroll(0x50);
        gDuelCmd.step++;
        break;
    case 1: {
        u8 *zones = (u8 *)gDuelZones;
        u32 playerOffset;

        /* Naming only the player offset, assigned inside the argument, gives it r5 and the slot product r4
         * (as in the ROM). */
        CopyDuelCard((u32 *)&cmd->card, (u32 *)(zones + (playerOffset = (playerA & 1) * 0xD64) + slotA * 0x94));
        ClearZoneTiles(playerA, slotA);
        ClearZoneTiles(playerB, slotB);
        from.player = playerA;
        from.area = DUEL_AREA_MONSTER;
        from.index = slotA;
        from.isDefense = ((struct DuelZone *)(slotA * 0x94 + playerOffset + (u32)zones))->isDefense;
        from.isFaceUp = ((struct DuelZone *)(slotA * 0x94 + playerOffset + (u32)zones))->isFaceUp;
        to.player = playerB;
        to.area = DUEL_AREA_MONSTER;
        to.index = slotB;
        to.isDefense = from.isDefense;
        to.isFaceUp = from.isFaceUp;
        if (slotB > ZONE_MONSTER_4)
            from.isDefense = 0;
        DuelAnim_SwapCards(&from, &to);
        gDuelCmd.step++;
        break;
    }
    default: {
        u8 *zones = (u8 *)gDuelZones;

        MemCopy16(&tmp, SWAP_ZONE(playerB & 1, slotB), sizeof(struct DuelZone));
        MemCopy16(SWAP_ZONE(playerB & 1, slotB), SWAP_ZONE(playerA & 1, slotA), sizeof(struct DuelZone));
        MemCopy16(SWAP_ZONE(playerA & 1, slotA), &tmp, sizeof(struct DuelZone));
        DrawAllAreaTiles();
        cmd->running = 0;
        break;
    }
    }
}
