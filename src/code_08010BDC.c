#include "global.h"
#include "duel.h"
#include "duel_ui.h"

/*
 * Duel command handlers that move cards between areas (for example hand to
 * field or graveyard). Each is dispatched from sub_0801ECA8 with its operands in
 * the command block at 0x020185C0 and runs as a small state machine on
 * gUnk_020185C0.step (0: open the area, 1: start the card-move animation,
 * 2: commit the move to the duel state), clearing gUnk_020185C0.running when
 * it is done. See wiki/functions/code-08010bdc.md.
 */

extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */

/* Read as a whole u16 then shifted (a 1-bit bitfield would compile to ldrb). */
#define CMD_PLAYER (gUnk_020185C0.cmd >> 15)
#define ZONE(p, s) ((struct DuelZone *)((u8 *)gUnk_0201930C + (s) * 0x94 + (p) * 0xD64))
#define CARD_STATS(id) (gUnk_08621DE0[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_SUBTYPE(id) ((CARD_STATS(id) & 0xE0000) >> 17)   /* magic/trap subtype (2 = Field) */
#define CMD_CARD (&gUnk_020185C0.card)

void sub_08007558(struct DuelCard *dst, struct DuelCard *src);
void sub_08007560(struct DuelCard *dst, struct DuelCard *src);
void sub_08007A4C(u32, u32, struct DuelCard *, u32, u32);
void sub_08007B24(u32, u32, struct DuelCard *, u32);
void sub_08008E44(u32, u32);
void sub_08008EB4(u32, u32);
void sub_08009768(struct DuelCard *);
void sub_080096F4(struct DuelCard *);
void sub_080099E8(struct DuelCard *);
void sub_08009D08(u32, u32);
void sub_08009EAC(u32, void *);
void sub_0800A004(u32, void *);
void sub_0800A0A8(u32);
void sub_080240A8(u32, u32);
void sub_080242C4(u32, struct DuelLoc *, struct DuelLoc *);
void sub_08060FD0(u32, u32);
void sub_080611AC(void);

void sub_08010BDC(void)
{
    u32 player = CMD_PLAYER;
    u32 w = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    sub_0800A004(player, &w);
    gUnk_020185C0.running = 0;
}

/*
 * Summon: moves hand card (arg4 bits 4-7) of the acting player to monster
 * zone (arg4 bits 0-3). arg2 = card ID for the animation, arg4 bit 8 / bit 9 =
 * flag15 / flag14 of the destination (hypothesis: face-down / defence).
 */
void sub_08010C14(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u32 cardId = gUnk_020185C0.arg2;
    u8 zone = (u8)gUnk_020185C0.arg4 & 0xF;
    u8 handIdx = ((u8)gUnk_020185C0.arg4 & 0xF0) >> 4;
    u8 flag15 = (gUnk_020185C0.arg4 >> 8) & 1;
    u8 flag14 = (u8)((gUnk_020185C0.arg4 >> 8) & 2) >> 1;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08007558(CMD_CARD, gUnk_020192E4[player & 1].hand + handIdx);
        (gUnk_020192E4[player & 1].hand + handIdx)->id = 0;
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
        sub_080242C4(cardId, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_0800A0A8(player);
        sub_08007A4C(player, zone, CMD_CARD, flag14, flag15);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
#if 0 /* NONMATCHING: register allocation only. The 0xF mask constant gets its
       * own pseudo-register (r7), and `zone -= 5` is rematerialised as r7-20
       * instead of r0(=1)-6. */
/*
 * Set/activate a magic or trap card: moves hand card (arg4 bits 4-7) of the
 * acting player to spell/trap zone (arg4 bits 0-3; values 5..9 map to 0..4), or
 * to the field zone when the card is a Field magic. arg2 = card ID.
 */
void sub_08010D94(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u32 cardId = gUnk_020185C0.arg2;
    int zone = (u8)gUnk_020185C0.arg4 & 0xF;
    u8 handIdx = ((u8)gUnk_020185C0.arg4 & 0xF0) >> 4;
    u8 flag15 = (gUnk_020185C0.arg4 >> 8) & 1;

    if (zone > 4)
        zone -= 5;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08007558(CMD_CARD, gUnk_020192E4[player & 1].hand + handIdx);
        (gUnk_020192E4[player & 1].hand + handIdx)->id = 0;
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
        if (CARD_TYPE(cardId) == 22 && CARD_SUBTYPE(cardId) == 2) {
            /* Field magic goes to the field zone */
            to.player = player;
            to.area = 10;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = flag15;
        }
        sub_080242C4(cardId, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_0800A0A8(player);
        sub_08007B24(player, zone, CMD_CARD, flag15);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08010BDC", sub_08010D94); /* 0x08010D94 size 0x1F0 */

void sub_08010F84(void)
{
    sub_0800A0A8(CMD_PLAYER);
    gUnk_020185C0.running = 0;
}

void sub_08010FAC(void)
{
    u32 player = CMD_PLAYER;
    u32 w = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    sub_08009EAC(player, &w);
    gUnk_020185C0.running = 0;
}

/*
 * Discards hand card arg2 of the acting player: animates it from the hand
 * (area 11) to its owner's area 15 (hypothesis: graveyard), then commits it
 * with sub_080099E8. arg4 != 0 also calls sub_0800A0A8 first.
 */
void sub_08010FE4(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u16 handIdx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08007558(CMD_CARD, gUnk_020192E4[player & 1].hand + handIdx);
        (gUnk_020192E4[player & 1].hand + handIdx)->id = 0;
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
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        if (gUnk_020185C0.arg4 != 0)
            sub_0800A0A8(player);
        sub_080099E8(CMD_CARD);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Returns card arg2 of the acting player's listB84 to the hand: animates it
 * from area 15 to the next free hand slot, then commits.
 */
void sub_08011148(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u16 idx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        from.player = player;
        from.area = 15;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 11;
        to.index = gUnk_020192E4[player & 1].handCount;
        to.flag14 = 0;
        to.flag15 = 0;
        sub_080242C4(1, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_08009EAC(player, &gUnk_020192E4[player].listB84[idx]);
        sub_08009D08(player, idx);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Swaps/copies a hand card between the players: animates hand slot arg2 of
 * the acting player to hand slot arg4 of the opponent and back, then
 * sub_08007560 copies opponent hand[arg4] into acting hand[arg2].
 * Note: from/to are word-accessed until `&from` is first taken (the C front
 * end then moves them to memory), so case 2 uses byte/halfword accesses.
 */
void sub_08011278(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        from.player = player;
        from.area = 11;
        from.index = gUnk_020185C0.arg2;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = 1 - player;
        to.area = 11;
        to.index = gUnk_020185C0.arg4;
        to.flag14 = 0;
        to.flag15 = 0;
        sub_080242C4(1, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        from.player = 1 - player;
        from.area = 11;
        from.index = gUnk_020185C0.arg4;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 11;
        to.index = gUnk_020185C0.arg2;
        to.flag14 = 0;
        to.flag15 = 0;
        sub_080242C4(1, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_08007560(gUnk_020192E4[player & 1].hand + gUnk_020185C0.arg2,
                     gUnk_020192E4[(1 - player) & 1].hand + gUnk_020185C0.arg4);
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Like sub_08010FE4 but sends hand card arg2 to its owner's area 14
 * (hypothesis: banished / removed from play), setting its flag20 before
 * sub_080096F4 commits it.
 */
void sub_08011498(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u16 handIdx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08007558(CMD_CARD, gUnk_020192E4[player & 1].hand + handIdx);
        (gUnk_020192E4[player & 1].hand + handIdx)->id = 0;
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
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        if (gUnk_020185C0.arg4 != 0)
            sub_0800A0A8(player);
        ((u8 *)&gUnk_020185C0)[0x816] |= 0x10;   /* CMD_CARD->flag20 = 1 */
        sub_080096F4(CMD_CARD);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Like sub_08011498 but to the owner's area 15 (graveyard?) with flag15 set,
 * committed by sub_08009768.
 */
void sub_08011610(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    u16 handIdx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08007558(CMD_CARD, gUnk_020192E4[player & 1].hand + handIdx);
        (gUnk_020192E4[player & 1].hand + handIdx)->id = 0;
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
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        if (gUnk_020185C0.arg4 != 0)
            sub_0800A0A8(player);
        ((u8 *)&gUnk_020185C0)[0x816] |= 0x10;   /* CMD_CARD->flag20 = 1 */
        sub_08009768(CMD_CARD);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Returns a card on the field to its owner's hand: arg2 = slot (0-4 monster
 * zones, 5-9 spell/trap zones, which are zones 5..9 of area 5, and above 9 the
 * field zone). Animates it to the next free hand slot of the card's owner.
 */
#if 0 /* NONMATCHING: same size, but the built frame is 12 bytes versus the
       * ROM's 16 (the ROM also spills (player&1)*0xD64), the zone flag reads
       * keep an extra "& 1", and the slot > 9 path makes different register
       * choices. */
void sub_08011780(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER;
    int slot = gUnk_020185C0.arg2;
    struct DuelZone *zone;

    switch (gUnk_020185C0.step) {
    case 0:
        if (slot <= 9)
            sub_08060FD0(player, slot + 5);
        else
            sub_08060FD0(player, 10);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (slot <= 9) {
            sub_08007558(CMD_CARD, &gUnk_020192E4[player & 1].zones[slot + 5].card);
            from.player = player;
            from.area = 5;
            from.index = slot;
            zone = ZONE(player & 1, slot + 5);
        } else {
            sub_08007558(CMD_CARD, &gUnk_020192E4[player & 1].zones[slot].card);
            from.player = player;
            from.area = 10;
            zone = ZONE(player & 1, slot);
            from.index = 0;
        }
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        to.player = player;
        to.area = 11;
        to.index = gUnk_020192E4[CMD_CARD->owner].handCount;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        if (slot <= 9)
            sub_08008EB4(player, slot + 5);
        else
            sub_08008EB4(player, slot);
        gUnk_020185C0.step++;
        break;
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08010BDC", sub_08011780); /* 0x08011780 size 0x230 */

/* Clears the card in zone (arg2 & 7) of the acting player. */
void sub_080119B0(void)
{
    ZONE(CMD_PLAYER, gUnk_020185C0.arg2 & 7)->card.id = 0;
    gUnk_020185C0.running = 0;
}

/*
 * Sends a card on the field to its owner's area 14 (hypothesis: removed from
 * play): arg2 = slot as in sub_08011780, cleared with sub_08008E44.
 */
#if 0 /* NONMATCHING: same problems as sub_08011780 (frame size, "& 1" on the zone flag reads) */
void sub_080119F8(void)
{
    struct DuelLoc from, to;
    u16 player = CMD_PLAYER;
    s16 slot = gUnk_020185C0.arg2;
    struct DuelZone *zone;

    switch (gUnk_020185C0.step) {
    case 0:
        if (slot <= 9)
            sub_08060FD0(player, slot + 5);
        else
            sub_08060FD0(player, 10);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (slot <= 9) {
            sub_08008E44(player, slot + 5);
            from.player = player;
            sub_08007558(CMD_CARD, &gUnk_020192E4[player & 1].zones[slot + 5].card);
            from.area = 5;
            from.index = slot;
            zone = ZONE(player & 1, slot + 5);
        } else {
            sub_08007558(CMD_CARD, &gUnk_020192E4[player & 1].zones[slot].card);
            sub_08008E44(player, slot);
            from.player = player;
            from.area = 10;
            zone = ZONE(player & 1, slot);
            from.index = 0;
        }
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        to.player = player;
        to.area = 14;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08010BDC", sub_080119F8); /* 0x080119F8 size 0x1E8 */
