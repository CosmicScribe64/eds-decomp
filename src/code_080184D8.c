#include "global.h"
#include "duel.h"

/*
 * Duel field-zone effects: per-zone "card entered / left / flipped" handlers
 * that queue duel events (sub_0801EC58) and re-apply field bonuses.
 * See wiki/functions/code-080184d8.md.
 */

/* Zone arrays of both players: 0x0201930C + (p & 1) * 0xD64 + zone * 0x94
 * (player+0x28; zones 0-4 monsters, 5-9 magic/trap, 10 field). */
#define ZONE(p, z) (&gUnk_020192E4[(p) & 1].zones[z])
/* Same address written as a raw integer (keeps the base inside loops). */
#define ZONE_AT(p, z) ((struct DuelZone *)((z) * 0x94 + ((p) & 1) * 0xD64 + 0x0201930C))
/* A zone's card ID, read through a struct DuelCard pointer (whole-word load). */
#define ZONE_CARD_ID(zp) (((struct DuelCard *)(zp))->id)

extern const u16 gUnk_08622AB4[];   /* maps card ID to card number */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern const u16 gUnk_08623DF4[];   /* maps card key to card number (see code_0801F454) */

/* Hand card words at 0x02019968 = 0x020192E4 + 0x684, 80 per player. */
struct HandRow {
    struct DuelCard c[80];
    u8 pad[0xD64 - 80 * 4];
};
extern struct HandRow gUnk_02019968[2];

/* Scratch card word / event globals used by sub_08018690. */
extern u16 gUnk_02017ECA;
extern const u32 gUnk_08621DE0[];   /* maps card ID to card data word */
struct Unk02017A40 { u8 pad[0x48A]; u16 w48A; };
extern struct Unk02017A40 gUnk_02017A40;
struct Unk02018450 { u16 w0; u8 b1; };
extern struct Unk02018450 gUnk_02018450;

/* Event message ids have bit 15 set when they concern player 1. */
#define EVT(p, id) ((p) ? (0x8000 | (id)) : (id))

void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void sub_08017DE0(int player, int zone, int arg);
void sub_08017460(int player, int zone, u16 arg, int arg3);
void sub_08018544(int player, int zone, u16 arg);
void sub_08018C3C(int player, int zone);
int sub_08008860(int player);
u16 sub_0800CD68(int player, int zone);
void sub_08019800(int player, int cardId);

/* Helpers used by the zone-event handlers below. */
u32 sub_08007590(u16 cardNo, u16 flag);
int sub_08008524(int player, u16 cardNo);
int sub_080086CC(int player, u16 cardNo);
int sub_0802CFA0(int player, u32 id, int x);
void sub_0801FBCC(u32 event, int arg);
void sub_08017C48(int player, int zone);
void sub_080197C0(int player, u16 arg);
void sub_080197E0(int player, u16 id);
void sub_08019860(int player, int lp);
void sub_08019980(int player, int lp);
void sub_080199E0(int player, int n);
void sub_08046C20(int player, int arg);
void sub_08042AB0(int player, int kind, u32 arg);

void sub_080184D8(int player, u16 cardNo)
{
    int i;

    for (i = 0; i <= 10; i++) {
        struct DuelZone *z = ZONE_AT(player, i);
        u16 id = ZONE_CARD_ID(z);
        if (id != 0 && z->flag6_1 && CARD_NUMBER(id) == cardNo)
            sub_08018544(player, i, 1);
    }
}
void sub_08018544(int player, int zone, u16 arg)
{
    u32 id;
    u32 cardId;
    int i;

    id = ZONE_CARD_ID(ZONE(player, zone));
    cardId = id;

    sub_08017460(player, zone, arg, arg != 0);
    if (id == 0)
        return;
    switch (CARD_NUMBER(id)) {
    case 0x5F8:
    case 0x605:
    case 0x606:
    case 0x607:
    case 0x608:
        break;
    default:
        return;
    }
    sub_08019800(player, cardId);
    for (i = 5; i <= 9; i++) {
        if (i != zone) {
            u32 id2 = ZONE_CARD_ID(ZONE(player, i));
            if (id2 != 0) {
                switch (CARD_NUMBER(id2)) {
                case 0x5F8:
                case 0x605:
                case 0x606:
                case 0x607:
                case 0x608:
                    sub_08017460(player, i, arg, 0);
                    break;
                }
            }
        }
    }
}
void sub_08018620(int player, u16 arg)
{
    int i;

    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = ZONE_AT(player, i);
        if (ZONE_CARD_ID(z) != 0)
            sub_08018544(player, i, arg);
    }
}
void sub_08018664(int player, int zone, u16 *args)
{
    sub_0801EC58(EVT(player, 0x7D), args[0], args[1], 0);
    sub_08017DE0(player, zone, 1);
}
#if 0 /* NONMATCHING: the card-word reloads and the byte +1 target packing
       * match the ROM, but register allocation and scheduling still differ. */
/* A card entered zone `zone` from the hand (args = its two word list entry):
 * if play is locked, just report it; card 0x1DE summons directly; otherwise
 * announce 0x7C, set the entry's 0x20 bit and dispatch per card key. */
void sub_08018690(int arg0, int player, int zone, u16 *args)
{
    u32 id;
    u32 dispatchWord;
    u32 owner;

    if (zone > 4)
        return;
    if (sub_080086CC(0, 0x453) > 0 || sub_080086CC(1, 0x453) > 0) {
        sub_08018664(player, zone, args);
        return;
    }
    gUnk_02017ECA = 0x13;
    owner = ((struct DuelCard *)args)->owner;
    id = ((struct DuelCard *)args)->id;
    if (CARD_NUMBER(id) == 0x1DE) {
        sub_080197E0(player, id);
        sub_0801EC58(EVT(player, 0x6A), args[0], args[1], 0);
        sub_08017DE0(player, zone, 1);
        return;
    }
    ((u8 *)args)[2] |= 0x20;
    sub_0801EC58(EVT(player, 0x7C), args[0], args[1], 0);
    id = ((struct DuelCard *)args)->id;
    if (((gUnk_08621DE0[id & 0x7FF] & 0x1F00000) >> 20) <= 0x14)
        gUnk_020192E4[player & 1].flagB_3 = 1;
    dispatchWord = *(u32 *)args;
    id = (dispatchWord << 20) >> 20;
    switch (CARD_NUMBER(id)) {
    case 0x2F: case 0x12F: case 0x136: case 0x138: case 0x139: case 0x140:
    case 0x23D: case 0x414: case 0x454: case 0x456:
    case 0x45A: case 0x45B: case 0x45D: case 0x45F: case 0x460: case 0x463:
    case 0x4D9: case 0x4DA: case 0x4E9: case 0x57D:
        {
            u32 lo = 0xFFFF;
            u32 ev = owner << 31;
            u32 hi;
            ev |= (0x3F & gUnk_02017A40.w48A) << 25;
            ev |= ((dispatchWord << 20) >> 20) | 0x600000;
            if (player == gUnk_020192E0.linkSkip)
                lo = gUnk_020192E0.linkSkip | ((((u32)gUnk_02018450.w0 << 0x17) >> 0x1D) << 8);
            if (player == 1 - gUnk_020192E0.linkSkip)
                hi = ((u8)(1 - gUnk_020192E0.linkSkip) | ((((u32)((u8 *)&gUnk_02018450)[1] << 0x1C) >> 0x1D) << 8)) << 16;
            else
                hi = 0xFFFF0000;
            sub_0801FBCC(ev, hi | lo);
        }
        break;
    case 0x1CD:
        if (!(ZONE(player, zone)->unk7 & 0x20)) {
            sub_0801EC58(EVT(owner, 0x73), id, 1, 0);
            sub_08019860(owner, 0x1388);
        }
        break;
    case 0x45C:
        if (!(ZONE(player, zone)->unk7 & 0x20))
            sub_0801FBCC((((dispatchWord << 19) >> 31) << 31) | ((0x3F & gUnk_02017ECA) << 25) | (id | 0x600000), 0);
        break;
    case 0x2D9:
    case 0x534:
        sub_08017C48(player, zone);
        break;
    case 0x5EA:
        if (arg0 == player) {
            sub_080197C0(player, id);
            sub_0801EC58(EVT(player, 0x4C), 1, 0, 0);
        }
        break;
    }
    sub_08046C20(player, 1);
    sub_08017DE0(player, zone, 1);
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080184D8", sub_08018690); /* 0x08018690 size 0x36C */
void sub_080189FC(int player, int zone, u16 arg)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if (arg != 0)
        sub_0801EC58(EVT(player, 0x7B), zone, 1, 0);
    else
        sub_0801EC58(EVT(player, 0x7A), zone, 1, 0);
    if (zone <= 4)
        sub_08017DE0(player, zone, 1);
    if (CARD_NUMBER(id) == 0x48A)
        sub_0801EC58(EVT(player, 0x8F), zone, 0, 0);
    if (zone == 10 && ZONE(player, zone)->flag6_1)
        sub_0801EC58(EVT(player, 0x11), 0, 0, 0);
}
/* A card left a zone: if its number is 0x780-0x7CF re-apply field bonuses,
 * otherwise announce 0x80; on the monster rows also notify the zone-event
 * queue; card 0x447 hands its destruction target over. */
void sub_08018AE8(int player, int zone, u16 arg2)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if ((u16)(CARD_NUMBER(id) - 0x780) < 0x50)
        sub_08018544(player, zone, 1);
    else
        sub_0801EC58(EVT(player, 0x80), zone, arg2, 0);
    if (zone <= 4) {
        sub_08017DE0(player, zone, 0);
        sub_08042AB0(1 - gUnk_020192E0.linkSkip, 0x1B, (u16)((u8)player | ((u8)zone << 8)));
    }
    if (zone == 10 && ZONE(player, zone)->flag6_1)
        sub_0801EC58(EVT(player, 0x11), 0, 0, 0);
    if (CARD_NUMBER(id) == 0x447) {
        u32 r = sub_0800CD68(player, zone);
        u16 target = r;
        if (target != 0xFFFF && !(ZONE(player, zone)->unk8C[5] & 8))
            sub_08018544((u8)target, r >> 8 & 0xFF, 1);
    }
}
void sub_08018C3C(int player, int zone)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if ((u16)(CARD_NUMBER(id) - 0x780) < 0x50)
        sub_08018544(player, zone, 1);
    else
        sub_0801EC58(EVT(player, 0x81), zone, 0, 0);
    if (zone <= 4)
        sub_08017DE0(player, zone, 0);
    if (zone == 10 && ZONE(player, zone)->flag6_1)
        sub_0801EC58(EVT(player, 0x11), 0, 0, 0);
    if (CARD_NUMBER(id) == 0x447) {
        u32 r = sub_0800CD68(player, zone);
        u16 target = r;
        if (target != 0xFFFF && !(ZONE(player, zone)->unk8C[5] & 8))
            sub_08018544((u8)target, r >> 8 & 0xFF, 1);
    }
}
void sub_08018D64(int player, u16 arg)
{
    int pass;
    int p;
    int i;

    for (pass = 0; pass <= 1; pass++) {
        if (pass == 0)
            p = 1 - player;
        else
            p = player;
        for (i = 0; i <= 4; i++)
            sub_08018C3C(p, i);
        if (arg != 0 && sub_08008860(p) > 0)
            sub_0801EC58(EVT(p, 0x60), 1, 0, 0);
    }
}
/* A monster was summoned/flipped into `zone`: announce event 0x7F, then either the
 * "flip" message 0x90 (if not yet face-up) and possibly a special summon, or the
 * re-apply call. */
void sub_08018DC8(int player, int zone, u16 arg)
{
    int p = player & 1;
    int zoneBytes = zone * 0x94;
    struct DuelZone *z;
    u16 id;

    __asm__("" : : "r"(p));
    z = (struct DuelZone *)(zoneBytes + p * 0xD64 + 0x0201930C);
    id = ZONE_CARD_ID(z);

    if (id == 0)
        return;
    sub_0801EC58(EVT(player, 0x7F), zone, 0, 0);
    if (zone <= 4) {
        if (!z->flag6_1) {
            sub_0801EC58(EVT(player, 0x90), zone, 2, 0);
            if (arg != 0 && sub_08007590(CARD_NUMBER(id), 0) != 0
                && sub_0802CFA0(player, id, 0) != 0
                && sub_08008524(0, 0x5FA) == 0 && sub_08008524(1, 0x5FA) == 0) {
                u32 ev = p << 31;
                u32 ev2 = ((zone & 0x1F) << 16) | 0x16400000;

                sub_0801FBCC(ev | ev2 | id, 0);
            }
        } else {
            sub_08017DE0(player, zone, 1);
        }
    }
}
/* A monster flipped/summoned face-* in zone `zone`: announce 0x7E; for some
 * cards with a set flag (or a key-monster) and later if arg2/arg3 ask for it,
 * hand the battle effect to the other player. */
void sub_08018ED8(int player, int zone, u16 arg2, u16 arg3)
{
    int one = 1;
    int p = player & one;
    int zoneBytes = zone * 0x94;
    s16 playerBytes = p * 0xD64;
    struct DuelZone *z;
    u32 id;

    /* The masked player's offset fits in an s16. The asm statement keeps `one`
     * live, matching the lifetime the ROM gives it. */
    __asm__("" : : "r"(one));
    z = (struct DuelZone *)(zoneBytes + playerBytes + 0x0201930C);
    id = ZONE_CARD_ID(z);

    if (zone > 4)
        return;
    if (id == 0)
        return;
    sub_0801EC58(EVT(player, 0x7E), zone, arg2, 0);
    if (z->flag6_0) {
        if (CARD_NUMBER(id) == 0x5E && sub_0802CFA0(player, id, 0) != 0) {
            u32 ev = (player & 1) << 31;
            u32 ev2 = ((0x1F & zone) << 16) | (0xA2 << 0x15);
            sub_0801FBCC(ev | ev2 | id, 0);
        }
    } else {
        switch (CARD_NUMBER(id)) {
        case 0x77:
        case 0xA1:
        case 0x1F0:
            if (sub_0802CFA0(player, id, 0) != 0) {
                u32 ev = (player & 1) << 31;
                u32 ev2 = ((0x1F & zone) << 16) | (0xA2 << 0x15);
                sub_0801FBCC(ev | ev2 | id, 0);
            }
            break;
        }
    }
    if (arg3 == 0)
        return;
    if (arg2 == 0)
        return;
    if (ZONE(player, zone)->flag6_1)
        return;
    if (sub_08007590(CARD_NUMBER(id), 0) == 0)
        return;
    if (sub_08008524(0, 0x5FA) != 0)
        return;
    if (sub_08008524(1, 0x5FA) != 0)
        return;
    {
        u32 ev = (player & 1) << 31;
        u32 ev2 = ((0x1F & zone) << 16) | (0xA2 << 0x15);
        sub_0801FBCC(ev | ev2 | id, 0);
    }
}
/* Move between two zones (`arg1`/`arg2` = player | zone << 8): if the source holds a
 * card and the target is empty, announce 0x82; across players, card 0x1E3/0x222
 * with +7 bit 0x20 hands its effect to the other player (0x92). Each zone is read
 * through ZONE() in place: the array form keeps the zone base in a shared register. */
void sub_08019078(int player, u16 arg1, u16 arg2)
{
    int p1 = (u8)arg1;
    int zone1 = arg1 >> 8;
    int p2 = (u8)arg2;
    int zone2 = arg2 >> 8;
    u32 id;

    if (ZONE_CARD_ID(ZONE(p1, zone1)) == 0)
        return;
    if (ZONE_CARD_ID(ZONE(p2, zone2)) != 0)
        return;
    sub_0801EC58(EVT(p1, 0x82), arg1, arg2, 0);
    if (p1 == p2)
        return;
    id = ZONE_CARD_ID(ZONE(p1, zone1));
    switch (CARD_NUMBER(id)) {
    case 0x1E3:
        if (ZONE(p1, zone1)->unk7 & 0x20) {
            sub_080197C0(p1, id);
            sub_08019860(p2, 0x7D0);
            sub_0801EC58(EVT(p2, 0x92), zone2, 0, 0);
            return;
        }
        break;
    case 0x222:
        if (ZONE(p1, zone1)->unk7 & 0x20) {
            sub_080197C0(p1, id);
            sub_08019980(p1, 0xBB8);
            sub_0801EC58(EVT(p2, 0x92), zone2, 0, 0);
        }
        break;
    }
}
#if 0 /* NONMATCHING: logic/instruction shapes match; agbcc permutes the high-register roles (target: arg2=sl, p1=r8, p2=r7, zone2=r9; ip holds const 1) */
/* Two zones (arg1/arg2 = player|zone<<8): if both hold a card, announce 0x84;
 * then for card 0x1E3/0x222 in either zone with the 0x20 flag bit, hand its
 * flip effect to the other player. */
void sub_0801919C(int player, u16 arg1, u16 arg2)
{
    u16 p1 = (u8)arg1;
    u16 zone1 = arg1 >> 8;
    u16 p2 = (u8)arg2;
    u16 zone2 = arg2 >> 8;
    struct DuelZone *z1 = ZONE_AT(p1, zone1);
    s16 id1 = ZONE_CARD_ID(z1);
    s8 id2 = ZONE_CARD_ID(ZONE_AT(p2, zone2));

    if (id1 == 0)
        return;
    if (id2 == 0)
        return;
    sub_0801EC58(EVT(player, 0x84), arg1, arg2, 0);
    switch (CARD_NUMBER(id1)) {
    case 0x1E3:
        if (z1->unk7 & 0x20) {
            sub_080197C0(p1, id1);
            sub_08019860(p2, 0x7D0);
            sub_0801EC58(EVT(p2, 0x92), zone2, 0, 0);
        }
        break;
    case 0x222:
        if (z1->unk7 & 0x20) {
            sub_080197C0(p1, id1);
            sub_08019980(p1, 0xBB8);
            sub_0801EC58(EVT(p2, 0x92), zone2, 0, 0);
        }
        break;
    }
    switch (CARD_NUMBER(id2)) {
    case 0x1E3:
        if (ZONE_AT(p2, zone2)->unk7 & 0x20) {
            sub_080197C0(p2, id2);
            sub_08019860(p1, 0x7D0);
            sub_0801EC58(EVT(p1, 0x92), zone1, 0, 0);
        }
        break;
    case 0x222:
        if (ZONE_AT(p2, zone2)->unk7 & 0x20) {
            sub_08019980(p2, 0xBB8);
            sub_080197C0(p2, id2);
            sub_0801EC58(EVT(p1, 0x92), zone1, 0, 0);
        }
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080184D8", sub_0801919C); /* 0x0801919C size 0x214 */
void sub_080193B0(int player, int arg1, u16 arg2)
{
    sub_0801EC58(EVT(player, 0xC3), arg1, arg2, 0);
}
/* Hand-row card used/leaves: announce 0xC0 (0xC1 while a 0x453 lock is up),
 * then per card key hand control to the other player or queue an effect. */
void sub_080193D4(int player, int arg1, u16 arg2, u16 arg3)
{
    u32 id;
    int n;
    u16 cardNo;

    if (sub_080086CC(0, 0x453) > 0 || sub_080086CC(1, 0x453) > 0) {
        sub_0801EC58(EVT(player, 0xC1), arg1, arg3, 0);
        return;
    }
    sub_0801EC58(EVT(player, 0xC0), arg1, arg3, 0);
    {
        int p = player & 1;
        int indexBytes = arg1 * 4;
        u32 offset = p * 0xD64;
        u32 word;

        word = *(u32 *)(indexBytes + offset + 0x02019968);
        id = (word << 20) >> 20;
    }
    switch (CARD_NUMBER(id)) {
    case 0x215:
        if (arg2 != 0) {
            sub_0801EC58(EVT(player, 0x73), id, 1, 0);
            sub_08019860(1 - player, 1000);
        }
        break;
    case 0x1CE:
        if (arg2 != 0) {
            sub_0801EC58(EVT(player, 0x73), id, 1, 0);
            sub_080199E0(player, 2);
        }
        break;
    case 0x4DA:
        { u32 ev = (player & 1) << 31;
          u32 ev2 = ((0x1F & player) << 16) | 0x3A600000;
          sub_0801FBCC(ev | ev2 | id, 0); }
        break;
    }
    n = sub_08008524(1 - player, cardNo = 0x40E);
    if (n > 0) {
        sub_0801EC58(EVT(player, 0x73), ((const u16 *)0x08623DF4)[cardNo], 1, 0);
        sub_08019860(player, n * 500);
    }
    sub_08046C20(player, 1);
}
