#include "global.h"
#include "duel.h"
#include "duel_ui.h"

/*
 * Duel command handlers that move cards between areas (for example hand to
 * field or graveyard). Each is dispatched from DuelCmd_Dispatch with its operands in
 * the command block at 0x020185C0 and runs as a small state machine on
 * gDuelCmd.step (0: open the area, 1: start the card-move animation,
 * 2: commit the move to the duel state), clearing gDuelCmd.running when
 * it is done. See wiki/functions/code-08010bdc.md.
 */

extern const u32 gCardStats[];   /* card stats, indexed by card ID */

/* Read as a whole u16 then shifted (a 1-bit bitfield would compile to ldrb). */
#define CMD_PLAYER (gDuelCmd.cmd >> 15)
#define ZONE(p, s) ((struct DuelZone *)((u8 *)gDuelZones + (s) * 0x94 + (p) * 0xD64))
#define CARD_STATS(id) (gCardStats[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_SUBTYPE(id) ((CARD_STATS(id) & 0xE0000) >> 17)   /* magic/trap subtype (2 = Field) */
#define CMD_CARD (&gDuelCmd.card)

void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);
void SwapDuelCards(struct DuelCard *dst, struct DuelCard *src);
void PlaceMonsterCard(u32, u32, struct DuelCard *, u32, u32);
void PlaceSpellTrapCard(u32, u32, struct DuelCard *, u32);
void SendZoneCardToGraveyard(u32, u32);
void ReturnZoneCardToHand(u32, u32);
void AddCardToBanished(struct DuelCard *);
void AddCardToGraveyard(struct DuelCard *);
void AddCardToBanishedFaceDown(struct DuelCard *);
void RemoveBanishedCardAt(u32, u32);
void AddCardToHand(u32, void *);
void RemoveCardFromHand(u32, void *);
void CompactHand(u32);
void DuelScreen_ScrollToZone(u32, u32);
void DuelAnim_MoveCard(u32, struct DuelLoc *, struct DuelLoc *);
void ClearZoneTiles(u32, u32);
void DrawAllAreaTiles(void);

void DuelCmd_RemoveCardFromHand(void)
{
    u32 player = CMD_PLAYER;
    u32 w = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    RemoveCardFromHand(player, &w);
    gDuelCmd.running = 0;
}

/*
 * Summon: moves hand card (arg4 bits 4-7) of the acting player to monster
 * zone (arg4 bits 0-3). arg2 = card ID for the animation, arg4 bit 8 / bit 9 =
 * flag15 / flag14 of the destination (hypothesis: face-down / defence).
 */
void DuelCmd_PlaceMonsterFromHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u32 cardId = gDuelCmd.arg2;
    u8 zone = (u8)gDuelCmd.arg4 & 0xF;
    u8 handIdx = ((u8)gDuelCmd.arg4 & 0xF0) >> 4;
    u8 flag15 = (gDuelCmd.arg4 >> 8) & 1;
    u8 flag14 = (u8)((gDuelCmd.arg4 >> 8) & 2) >> 1;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard(CMD_CARD, gDuelPlayers[player & 1].hand + handIdx);
        (gDuelPlayers[player & 1].hand + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = flag15;
        to.player = player;
        to.area = 0;
        to.index = zone;
        to.flag14 = flag14;
        to.flag15 = flag15;
        DuelAnim_MoveCard(cardId, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        CompactHand(player);
        PlaceMonsterCard(player, zone, CMD_CARD, flag14, flag15);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Set/activate a magic or trap card: moves hand card (arg4 bits 4-7) of the
 * acting player to spell/trap zone (arg4 bits 0-3; values 5..9 map to 0..4), or
 * to the field zone when the card is a Field magic. arg2 = card ID.
 */
#define CSTATS_C(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CTYPE_C(id) ((CSTATS_C(id) & 0x1F00000) >> 20)
#define CSUB_C(id) ((CSTATS_C(id) & 0xE0000) >> 17)
void DuelCmd_PlaceSpellTrapFromHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u32 cardId = gDuelCmd.arg2;
    int zone = (u8)gDuelCmd.arg4 & 0xF;
    u8 handIdx = ((u8)gDuelCmd.arg4 & 0xF0) >> 4;
    u8 flag15 = (gDuelCmd.arg4 >> 8) & 1;

    if (zone > 4)
        zone -= 5;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard(CMD_CARD, gDuelPlayers[player & 1].hand + handIdx);
        (gDuelPlayers[player & 1].hand + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = flag15;
        to.player = player;
        to.area = 5;
        to.index = zone;
        to.flag14 = 0;
        to.flag15 = flag15;
        if (CTYPE_C(cardId) == 22 && CSUB_C(cardId) == 2) {
            /* Field magic goes to the field zone */
            to.player = player;
            to.area = 10;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = flag15;
        }
        DuelAnim_MoveCard(cardId, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        CompactHand(player);
        PlaceSpellTrapCard(player, zone, CMD_CARD, flag15);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_CompactHand(void)
{
    CompactHand(CMD_PLAYER);
    gDuelCmd.running = 0;
}

void DuelCmd_AddCardToHand(void)
{
    u32 player = CMD_PLAYER;
    u32 w = (gDuelCmd.arg4 << 16) | gDuelCmd.arg2;
    AddCardToHand(player, &w);
    gDuelCmd.running = 0;
}

/*
 * Discards hand card arg2 of the acting player: animates it from the hand
 * (area 11) to its owner's area 15 (hypothesis: graveyard), then commits it
 * with AddCardToBanishedFaceDown. arg4 != 0 also calls CompactHand first.
 */
void DuelCmd_BanishHandCardFaceDown(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u16 handIdx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard(CMD_CARD, gDuelPlayers[player & 1].hand + handIdx);
        (gDuelPlayers[player & 1].hand + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 15;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 0;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        AddCardToBanishedFaceDown(CMD_CARD);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Returns card arg2 of the acting player's listB84 to the hand: animates it
 * from area 15 to the next free hand slot, then commits.
 */
void DuelCmd_ReturnBanishedCardToHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u16 idx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        from.player = player;
        from.area = 15;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 11;
        to.index = gDuelPlayers[player & 1].handCount;
        to.flag14 = 0;
        to.flag15 = 0;
        DuelAnim_MoveCard(1, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        AddCardToHand(player, &gDuelPlayers[player].listB84[idx]);
        RemoveBanishedCardAt(player, idx);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Swaps/copies a hand card between the players: animates hand slot arg2 of
 * the acting player to hand slot arg4 of the opponent and back, then
 * SwapDuelCards copies opponent hand[arg4] into acting hand[arg2].
 * Note: from/to are word-accessed until `&from` is first taken (the C front
 * end then moves them to memory), so case 2 uses byte/halfword accesses.
 */
void DuelCmd_ExchangeHandCards(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        from.player = player;
        from.area = 11;
        from.index = gDuelCmd.arg2;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = 1 - player;
        to.area = 11;
        to.index = gDuelCmd.arg4;
        to.flag14 = 0;
        to.flag15 = 0;
        DuelAnim_MoveCard(1, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        from.player = 1 - player;
        from.area = 11;
        from.index = gDuelCmd.arg4;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 11;
        to.index = gDuelCmd.arg2;
        to.flag14 = 0;
        to.flag15 = 0;
        DuelAnim_MoveCard(1, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        SwapDuelCards(gDuelPlayers[player & 1].hand + gDuelCmd.arg2,
                     gDuelPlayers[(1 - player) & 1].hand + gDuelCmd.arg4);
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Like DuelCmd_BanishHandCardFaceDown but sends hand card arg2 to its owner's area 14
 * (hypothesis: banished / removed from play), setting its flag20 before
 * AddCardToGraveyard commits it.
 */
void DuelCmd_SendHandFusionMaterialToGraveyard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u16 handIdx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard(CMD_CARD, gDuelPlayers[player & 1].hand + handIdx);
        (gDuelPlayers[player & 1].hand + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 14;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        ((u8 *)&gDuelCmd)[0x816] |= 0x10;   /* CMD_CARD->flag20 = 1 */
        AddCardToGraveyard(CMD_CARD);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Like DuelCmd_SendHandFusionMaterialToGraveyard but to the owner's area 15 (graveyard?) with flag15 set,
 * committed by AddCardToBanished.
 */
void DuelCmd_BanishHandFusionMaterial(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u16 handIdx = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        CopyDuelCard(CMD_CARD, gDuelPlayers[player & 1].hand + handIdx);
        (gDuelPlayers[player & 1].hand + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 15;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (gDuelCmd.arg4 != 0)
            CompactHand(player);
        ((u8 *)&gDuelCmd)[0x816] |= 0x10;   /* CMD_CARD->flag20 = 1 */
        AddCardToBanished(CMD_CARD);
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Returns a card on the field to its owner's hand: arg2 = slot (0-4 monster
 * zones, 5-9 spell/trap zones, which are zones 5..9 of area 5, and above 9 the
 * field zone). Animates it to the next free hand slot of the card's owner.
 */
/*
 * Moves a spell/trap (arg2 <= 9: zone arg2 + 5) or field card (zone arg2) back
 * to its owner's hand. The zone flags are read through `(&zones[i])->flag`, a
 * pointer sum expanded as an address (P + S + const), so it does not CSE with
 * the CopyDuelCard argument; both reads sit inside the branches, where the
 * `& 1` masks reuse the register holding step == 1.
 */
void DuelCmd_ReturnSpellTrapToHand(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    int slot = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        if (slot <= 9)
            ClearZoneTiles(player, slot + 5);
        else
            ClearZoneTiles(player, 10);
        gDuelCmd.step++;
        break;
    case 1:
        if (slot <= 9) {
            CopyDuelCard(CMD_CARD, &gDuelPlayers[player & 1].zones[slot + 5].card);
            from.player = player;
            from.area = 5;
            from.index = slot;
            from.flag14 = (&gDuelPlayers[player & 1].zones[slot + 5])->flag6_0;
            from.flag15 = (&gDuelPlayers[player & 1].zones[slot + 5])->flag6_1;
        } else {
            CopyDuelCard(CMD_CARD, &gDuelPlayers[player & 1].zones[slot].card);
            from.player = player;
            from.area = 10;
            from.index = 0;
            from.flag14 = (&gDuelPlayers[player & 1].zones[slot])->flag6_0;
            from.flag15 = (&gDuelPlayers[player & 1].zones[slot])->flag6_1;
        }
        to.player = player;
        to.area = 11;
        to.index = gDuelPlayers[CMD_CARD->owner].handCount;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    case 2:
        if (slot <= 9)
            ReturnZoneCardToHand(player, slot + 5);
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

/* Clears the card in zone (arg2 & 7) of the acting player. */
void DuelCmd_ClearZoneCardNoRedraw(void)
{
    ZONE(CMD_PLAYER, gDuelCmd.arg2 & 7)->card.id = 0;
    gDuelCmd.running = 0;
}

/*
 * Sends a card on the field to its owner's area 14 (hypothesis: removed from
 * play): arg2 = slot as in DuelCmd_ReturnSpellTrapToHand, cleared with SendZoneCardToGraveyard.
 */
void DuelCmd_SendSpellTrapToGraveyard(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    int slot = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        if (slot <= 9)
            ClearZoneTiles(player, slot + 5);
        else
            ClearZoneTiles(player, 10);
        gDuelCmd.step++;
        break;
    case 1:
        if (slot <= 9) {
            CopyDuelCard(CMD_CARD, &gDuelPlayers[player & 1].zones[slot + 5].card);
            SendZoneCardToGraveyard(player, slot + 5);
            from.player = player;
            from.area = 5;
            from.index = slot;
            from.flag14 = (&gDuelPlayers[player & 1].zones[slot + 5])->flag6_0;
            from.flag15 = (&gDuelPlayers[player & 1].zones[slot + 5])->flag6_1;
        } else {
            CopyDuelCard(CMD_CARD, &gDuelPlayers[player & 1].zones[slot].card);
            SendZoneCardToGraveyard(player, slot);
            from.player = player;
            from.area = 10;
            from.index = 0;
            from.flag14 = (&gDuelPlayers[player & 1].zones[slot])->flag6_0;
            from.flag15 = (&gDuelPlayers[player & 1].zones[slot])->flag6_1;
        }
        to.player = player;
        to.area = 14;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        DuelAnim_MoveCard(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}
