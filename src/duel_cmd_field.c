/*
 * Duel commands that change or move a card on the field (wiki/functions/duel-cmd-field-c.md):
 * DUEL_CMD_SEND_TO_GRAVEYARD .. DUEL_CMD_BANISH_FLAGGED (0x79-0x7B), DUEL_CMD_CHANGE_POSITION, FLIP_CARD
 * (0x7E, 0x7F), RETURN_TO_HAND, RETURN_TO_DECK, MOVE_TO_ZONE (0x80-0x82) and SWAP_ZONES (0x84).
 * DuelCmd_Dispatch calls the handler of the running command once per frame. Each handler is a small state
 * machine on gDuelCmd.step: scroll the field to the zone, play the animation, then change the duel state,
 * redraw the field and clear gDuelCmd.running. A card leaving the field is saved in gDuelCmd.card first, and
 * its move to the pile is animated unless it is a token.
 *
 * Most handlers form a zone address by byte arithmetic on gDuelZones (slot * 0x94 + player * 0xD64 +
 * (u32)gDuelZones, the terms in the ROM's order) instead of &gDuelZones[player].zones[slot]: the member form
 * emits the instructions in another order (tried on DuelCmd_Banish). A zone's card id is read through a
 * struct DuelCard pointer (DUEL_CARD_ID), which loads the whole card word as the ROM does.
 */
#include "global.h"
#include "util.h"                   /* MemClear16, MemCopy16 */
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_TOKEN_FIRST / END */
#include "constants/duel.h"
#include "constants/sound.h"
#include "sound.h"                  /* PlaySE */
#include "duel.h"                   /* struct DuelCard / DuelZone / DuelPlayer, gDuelZones, the zone actions */
#include "duel_cmd.h"               /* struct DuelCmd, gDuelCmd */
#include "duel_screen.h"            /* DuelAnim_*, DuelCursor_Select, DuelScreen_*, DrawAllAreaTiles, GetZoneArea */

/* Local view of DuelAnim_MoveCard (duel_screen.h, u16 cardId): the same symbol called with a u32 card ID, by
 * DuelCmd_SendToGraveyard and DuelCmd_Banish. With the header's u16 parameter the argument set-up (mov r0, ip)
 * moves one instruction earlier; the other four calls use the header prototype. */
extern void DuelAnim_MoveCard32(u32 cardId, struct DuelLoc *from, struct DuelLoc *to) asm("DuelAnim_MoveCard");

/* 0x0868CAC0: the 8-frame 32x32 explosion, a sprite animation stream played on a zone (used only here). */
extern const u8 gExplosionAnim[];

/* Acting player of the running command: bit 15 of the command word (DUEL_CMD_PLAYER). */
#define CMD_ACTING_PLAYER() (gDuelCmd.cmd >> 15)

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

    if (DUEL_CARD_ID(zone) == 0) {
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
    if (DUEL_CARD_ID((u8 *)gDuelZones + offset) == 0) {
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
        if (DUEL_CARD_ID(player * 0xD64 + slot * 0x94 + (u32)gDuelZones) == 0) {
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

        CopyDuelCard(saved, (struct DuelCard *)(playerZones + slotOffset));
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
        if (DUEL_CARD_ID(player * 0xD64 + slot * 0x94 + (u32)gDuelZones) == 0)
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

        CopyDuelCard(saved, (struct DuelCard *)(playerZones + slotOffset));
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
        CopyDuelCard(saved, (struct DuelCard *)(playerOffset + (u32)zones + slotOffset));
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

        CopyDuelCard(saved, (struct DuelCard *)(playerZones + slotOffset));
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
            DuelAnim_MoveCard(DUEL_CARD_ID(&gDuelCmd.card), &from, &to);
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

        CopyDuelCard(saved, (struct DuelCard *)(playerZones + slotOffset));
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
        if (DUEL_CARD_ID((srcPlayer & 1) * 0xD64 + srcSlot * 0x94 + (u32)gDuelZones) == 0) {
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

        CopyDuelCard(saved, (struct DuelCard *)(playerZones + slotOffset));
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
        CopyDuelCard(&dst->card, &cmd->card);
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
 * Zone address forms of DuelCmd_SwapZones (Matching: byte arithmetic instead of a member access, see the top of
 * the file). The add order picks the ROM's evaluation order: a local `zones` keeps (zones + p * 0xD64) + s * 0x94
 * from being reassociated, and in a memory address the second product is emitted first.
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
        /* FAKEMATCH: two separate ifs (cross-jumped later) instead of one || give cmd the extra reference
         * that puts it in r9 ahead of slotA. */
        if (DUEL_CARD_ID(SWAP_ZONE_GLOBAL(playerA & 1, slotA)) == 0) {
            cmd->running = 0;
            break;
        }
        if (DUEL_CARD_ID(SWAP_ZONE_GLOBAL(playerB & 1, slotB)) == 0) {
            cmd->running = 0;
            break;
        }
        DuelScreen_StartScroll(0x50);
        gDuelCmd.step++;
        break;
    case 1: {
        u8 *zones = (u8 *)gDuelZones;
        u32 playerOffset;

        /* FAKEMATCH: naming only the player offset, assigned inside the argument, gives it r5 and the slot
         * product r4, as in the ROM. */
        CopyDuelCard(&cmd->card, (struct DuelCard *)(zones + (playerOffset = (playerA & 1) * 0xD64) + slotA * 0x94));
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
