#include "global.h"

struct PromptWork {
    u8 pad[0x53C];
    u32 animation;   /* +0x53C: cursor bits 0-7, mode bits 12-19, phase bits 20-27 */
    u8 timerLo;      /* +0x540: CPU think timer low nibble */
};
extern struct PromptWork gUnk_02017A40;
void sub_080536D4(int hidden);
void sub_08053770(int hidden, int from, int to);
extern u32 gUnk_02017F84[];
extern u8 gUnk_02017F7C;
extern u16 gUnk_0300489E;
extern const u16 gUnk_081A4424[];
int sub_08062140(int id);
void sub_08076714(u32 position, int shape, int tile, u32 flags);
extern const int gUnk_0819D27C[];
extern const int gUnk_0819D280[];
struct Main { u32 rng; u16 held, keys; u8 pad[0x485E - 8]; u16 frame; };
extern struct Main gUnk_03000040;
struct DuelScreen { u8 pad[0x808]; u8 flags; u8 pad809[0x824-0x809]; int player, zone, index; };
extern struct DuelScreen gUnk_0201CFB0;
u16 sub_08052908(int player, int zone, int index, u32 mask);
u16 sub_08052B78(u16 direction, int *player, int *zone, int *index, u32 mask);
u16 sub_08052CE8(u16 direction, int *player, int *zone, int *index, u32 mask);
void sub_08024134(int player, int zone, int index);
void sub_08077AEC(int sound);
struct Player { u8 pad[2]; u8 handCount; u8 rest[0xD66-3]; u8 unkD66; };
extern struct Player gUnk_020192E4[];
void sub_08007560(void *a, void *b);
void sub_0805ED9C(void);
void sub_0805F074(int id, int flag);
void sub_080602A4(int a, int b, int c, const void *text);
void sub_08060308(int kind, void (*draw)(void), int (*keys)(void));
int sub_08056300(int a, u16 number);
int sub_080538C8(void);
extern const u16 gUnk_08622AB4[];
extern u8 gUnk_080862C4;

/* Per-player duel block at 0x020192E4: stride 0xD64, hand count at +2. */
struct Side52B78 { u8 pad[2]; u8 handCount; u8 rest[0xD64 - 3]; };
u16 sub_08052B78(u16 direction, int *player, int *zone, int *index, u32 mask)
{
    int p = *player, z = *zone, i = *index;
    int oldP = p, oldZ = z, oldI = i;
    struct Side52B78 *players = (struct Side52B78 *)gUnk_020192E4;
    int side = p & 1;
    /* Assigning the player offset to one variable in both direction blocks
     * keeps the multiply inside the loop: a twice-set destination is not
     * loop-invariant, so loop.c leaves the 0xD64 multiply in place as in ROM. */
    int off;
    for (;;) {
    if (direction & 4) {
        switch (z) {
        case 11:
            if (p != 0) {
                if (i <= 0) { off = side * 0xD64; i = *(u8 *)(off + (u32)players + 2); }
            dec4:
                i--;
            } else {
                if (i < players[0].handCount - 1) i++;
                else i = 0;
            }
            break;
        case 0:
            if (p != 0) {
                if (i > 0) goto dec4; /* shares case 11's decrement, as in ROM */
                z = 10; i = 0;
            } else {
                if (i <= 3) i++;
                else { z = 10; i = 0; }
            }
            break;
        case 5:
            i = (p != 0 ? i + 4 : i + 1) % 5;
            break;
        case 10:
            z = 0;
            i = p != 0 ? 4 : 0;
            break;
        }
    }
    if (direction & 8) {
        switch (z) {
        case 11:
            if (p != 0) {
                off = side * 0xD64;
                if (i < *(u8 *)(off + (u32)players + 2) - 1) i++;
                else i = 0;
            } else {
                if (i <= 0) i = players[0].handCount;
            dec8:
                i--;
            }
            break;
        case 0:
            if (p != 0) {
                if (i <= 3) i++;
                else { z = 10; i = 0; }
            } else {
                if (i > 0) goto dec8; /* shares case 11's decrement, as in ROM */
                z = 10; i = 0;
            }
            break;
        case 5:
            i = (p != 0 ? i + 1 : i + 4) % 5;
            break;
        case 10:
            z = 0;
            if (p != 0) i = 0; else i = 4;
            break;
        }
    }
    if (oldP == p && oldZ == z && oldI == i)
        return 0;
    if (sub_08052908(p, z, i, mask)) {
        *player = p;
        *zone = z;
        *index = i;
        return 1;
    }
    }
}

u16 sub_08052CE8(u16 direction, int *player, int *zone, int *index, u32 mask)
{
    int p = *player;
    int z = *zone;
    int i = *index;
    int oldP = p, oldZ = z, oldI = i;
    int backP, backZ, backI;

    /* The main loop is nested inside this if so the "no horizontal/vertical
     * bit" path falls through to the shared sub_08052B78 tail; writing it as an
     * early `return sub_08052B78(...)` instead makes agbcc move that tail to the
     * top of the function and the unit no longer matches. */
    if (direction & 3) {
    for (;;) {
        if (direction & 1) {
            switch (z) {
            case 11:
                if (p != 0) {
                    if (gUnk_020192E4->handCount != 0) {
                        p = 0;
                        i = gUnk_020192E4->handCount - 1;
                    } else {
                        p = 0;
                        z = 5;
                        i = 0;
                    }
                } else {
                    p = 0;
                    z = 5;
                    i = 0;
                }
                break;
            case 5:
                if (p == 0) {
                    z = 0;
                    i = 0;
                } else if (gUnk_020192E4->unkD66 != 0) {
                    z = 11;
                    i = 0;
                } else if (gUnk_020192E4->handCount != 0) {
                    p = 0;
                    z = 11;
                    i = 0;
                } else {
                    p = 0;
                    i = 0;
                }
                break;
            case 0:
                if (p != 0) {
                    z = 5;
                    i = 0;
                } else {
                    p = 1;
                    i = 0;
                }
                break;
            case 10:
                if (p != 0) {
                    z = 5;
                    i = 0;
                } else {
                    z = 0;
                    i = 0;
                }
                break;
            }
        }
        if (direction & 2) {
            switch (z) {
            case 11:
                if (p != 0) {
                    z = 5;
                    i = 0;
                } else if (gUnk_020192E4->unkD66 != 0) {
                    p = 1;
                    i = gUnk_020192E4->unkD66 - 1;
                } else {
                    p = 1;
                    z = 5;
                    i = 0;
                }
                break;
            case 5:
                if (p != 0) {
                    z = 0;
                    i = 0;
                } else if (gUnk_020192E4->handCount != 0) {
                    z = 11;
                    i = 0;
                } else if (gUnk_020192E4->unkD66 != 0) {
                    p = 1;
                    z = 11;
                    i = 0;
                } else {
                    p = 1;
                    i = 0;
                }
                break;
            case 0:
                if (p != 0) {
                    p = 0;
                    i = 0;
                } else {
                    z = 5;
                    i = 0;
                }
                break;
            case 10:
                if (p != 0) {
                    z = 0;
                    i = 0;
                } else {
                    z = 5;
                    i = 0;
                }
                break;
            }
        }
        if (oldP == p && oldZ == z && oldI == i)
            return 0;
        if (sub_08052908(p, z, i, mask)) {
            *player = p;
            *zone = z;
            *index = i;
            return 1;
        }
        backP = p;
        backZ = z;
        backI = i;
        do {
            if (sub_08052B78(4, &p, &z, &i, mask)) {
                *player = p;
                *zone = z;
                *index = i;
                return 1;
            }
        } while (p != backP || z != backZ || i != backI);
    }
    }
    return sub_08052B78(direction, player, zone, index, mask);
}
int sub_08052F38(u32 mask)
{
    u16 keys = gUnk_03000040.keys;
    int player = gUnk_0201CFB0.player;
    int zone = gUnk_0201CFB0.zone;
    int *zonePtr = &gUnk_0201CFB0.zone;
    int index = *(int *)((u32)&gUnk_0201CFB0 + 0x82C);
    gUnk_0201CFB0.flags |= 8;
    switch (zone) {
    case 12: case 13: case 14: case 15:
        sub_08024134(player, 0, 0);
        return 0;
    }
    if (!sub_08052908(player, zone, index, mask)) {
        if (sub_08052CE8(1, &player, &zone, &index, mask)) {
            sub_08024134(player, zone, index);
            return 0;
        }
    }
    if (keys & 0x40) {
        if (sub_08052CE8(1, &player, &zone, &index, mask)) {
            sub_08077AEC(0);
            sub_08024134(player, zone, index);
            return 0;
        }
    }
    if (keys & 0x80) {
        if (sub_08052CE8(2, &player, &zone, &index, mask)) {
            sub_08077AEC(0);
            sub_08024134(player, zone, index);
            return 0;
        }
    }
    if (keys & 0x20) {
        if (sub_08052CE8(8, &player, &zone, &index, mask)) {
            sub_08077AEC(0);
            sub_08024134(player, zone, index);
            return 0;
        }
    }
    if (keys & 0x10) {
        if (sub_08052CE8(4, &player, &zone, &index, mask)) {
            sub_08077AEC(0);
            sub_08024134(player, zone, index);
            return 0;
        }
    }
    if (keys & 1)
        return 1;
    return 0;
}
/* Per-player duel block at 0x020192E4: stride 0xD64, hand count at +2. */
struct Side5304C { u8 pad[2]; u8 handCount; u8 rest[0xD64 - 3]; };
/* Duel-field cursor keys: Left/Right/Up/Down move the cursor between zones
 * (both players, zone codes 0-15) via sub_08024134 and play SE 0; A returns 1.
 * Each case keeps its own sound/return so cross-jumping merges the tails as in
 * ROM; the first switch uses the local zone, the later ones re-read the field. */
int sub_0805304C(void)
{
    u16 keys = gUnk_03000040.keys;
    int player = gUnk_0201CFB0.player;
    int zone = gUnk_0201CFB0.zone;
    int index = gUnk_0201CFB0.index;
    struct Side5304C *sides, *side;

    gUnk_0201CFB0.flags |= 8;
    if (keys & 0x20) {
        switch (player) {
        case 0:
            switch (zone) {
            case 0:
                if (index > 0)
                    sub_08024134(player, zone, index - 1);
                else
                    sub_08024134(player, 10, 0);
                sub_08077AEC(0);
                return 0;
            case 5:
                if (index > 0)
                    sub_08024134(player, zone, index - 1);
                else
                    sub_08024134(player, 12, 0);
                sub_08077AEC(0);
                return 0;
            case 11:
                sides = (struct Side5304C *)gUnk_020192E4; side = &sides[player & 1];
                if (side->handCount != 0) {
                    if (index > 0)
                        sub_08024134(player, zone, index - 1);
                    else
                        sub_08024134(player, zone, side->handCount - 1);
                    sub_08077AEC(0);
                    return 0;
                } else {
                    /* FAKEMATCH: an int-returning cast turns this into a call_value, so cross-jumping keeps a separate movs r0, #3; bl in each copy as in ROM */
                    ((int (*)(int))sub_08077AEC)(3);
                    break;
                }
            case 10:
                sub_08024134(player, 14, 0);
                sub_08077AEC(0);
                return 0;
            case 12:
                sub_08024134(player, 13, 0);
                sub_08077AEC(0);
                return 0;
            case 13:
                sub_08024134(player, 5, 4);
                sub_08077AEC(0);
                return 0;
            case 14: case 15:
                sub_08024134(player, 0, 4);
                sub_08077AEC(0);
                return 0;
            }
            return 0;
        case 1:
            switch (zone) {
            case 0:
                if (index <= 3)
                    sub_08024134(player, zone, index + 1);
                else
                    sub_08024134(player, 14, 0);
                sub_08077AEC(0);
                return 0;
            case 5:
                if (index <= 3)
                    sub_08024134(player, zone, index + 1);
                else
                    sub_08024134(player, 13, 0);
                sub_08077AEC(0);
                return 0;
            case 11:
                sides = (struct Side5304C *)gUnk_020192E4; side = &sides[player & 1];
                if (side->handCount != 0) {
                    if (index < side->handCount - 1)
                        sub_08024134(player, zone, index + 1);
                    else
                        sub_08024134(player, zone, 0);
                    sub_08077AEC(0);
                    return 0;
                } else {
                    /* FAKEMATCH: an int-returning cast turns this into a call_value, so cross-jumping keeps a separate movs r0, #3; bl in each copy as in ROM */
                    ((int (*)(int))sub_08077AEC)(3);
                    break;
                }
            case 10:
                sub_08024134(player, 0, 0);
                sub_08077AEC(0);
                return 0;
            case 12:
                sub_08024134(player, 5, 0);
                sub_08077AEC(0);
                return 0;
            case 13:
                sub_08024134(player, 12, 0);
                sub_08077AEC(0);
                return 0;
            case 14: case 15:
                sub_08024134(player, 10, 0);
                sub_08077AEC(0);
                return 0;
            }
            return 0;
        }
    }
    if (keys & 0x10) {
        switch (player) {
        case 0:
            switch (gUnk_0201CFB0.zone) {
            case 0:
                if (index <= 3)
                    sub_08024134(player, zone, index + 1);
                else
                    sub_08024134(player, 14, 0);
                sub_08077AEC(0);
                return 0;
            case 5:
                if (index <= 3)
                    sub_08024134(player, zone, index + 1);
                else
                    sub_08024134(player, 13, 0);
                sub_08077AEC(0);
                return 0;
            case 11:
                sides = (struct Side5304C *)gUnk_020192E4; side = &sides[player & 1];
                if (side->handCount != 0) {
                    if (index < side->handCount - 1)
                        sub_08024134(player, zone, index + 1);
                    else
                        sub_08024134(player, zone, 0);
                    sub_08077AEC(0);
                    return 0;
                } else {
                    /* FAKEMATCH: an int-returning cast turns this into a call_value, so cross-jumping keeps a separate movs r0, #3; bl in each copy as in ROM */
                    ((int (*)(int))sub_08077AEC)(3);
                    break;
                }
            case 10:
                sub_08024134(player, 0, 0);
                sub_08077AEC(0);
                return 0;
            case 12:
                sub_08024134(player, 5, 0);
                sub_08077AEC(0);
                return 0;
            case 13:
                sub_08024134(player, 12, 0);
                sub_08077AEC(0);
                return 0;
            case 14: case 15:
                sub_08024134(player, 10, 0);
                sub_08077AEC(0);
                return 0;
            }
            return 0;
        case 1:
            switch (gUnk_0201CFB0.zone) {
            case 0:
                if (index > 0)
                    sub_08024134(player, zone, index - 1);
                else
                    sub_08024134(player, 10, 0);
                sub_08077AEC(0);
                return 0;
            case 5:
                if (index > 0)
                    sub_08024134(player, zone, index - 1);
                else
                    sub_08024134(player, 12, 0);
                sub_08077AEC(0);
                return 0;
            case 11:
                sides = (struct Side5304C *)gUnk_020192E4; side = &sides[player & 1];
                if (side->handCount != 0) {
                    if (index > 0)
                        sub_08024134(player, zone, index - 1);
                    else
                        sub_08024134(player, zone, side->handCount - 1);
                    sub_08077AEC(0);
                    return 0;
                } else {
                    /* FAKEMATCH: an int-returning cast turns this into a call_value, so cross-jumping keeps a separate movs r0, #3; bl in each copy as in ROM */
                    ((int (*)(int))sub_08077AEC)(3);
                    break;
                }
            case 10:
                sub_08024134(player, 14, 0);
                sub_08077AEC(0);
                return 0;
            case 12:
                sub_08024134(player, 13, 0);
                sub_08077AEC(0);
                return 0;
            case 13:
                sub_08024134(player, 5, 4);
                sub_08077AEC(0);
                return 0;
            case 14: case 15:
                sub_08024134(player, 0, 4);
                sub_08077AEC(0);
                return 0;
            }
            return 0;
        }
    }
    if (keys & 0x40) {
        switch (player) {
        case 0:
            switch (gUnk_0201CFB0.zone) {
            case 0:
                sub_08024134(1 - player, zone, 4 - index);
                sub_08077AEC(0);
                return 0;
            case 5:
                sub_08024134(player, 0, index);
                sub_08077AEC(0);
                return 0;
            case 11:
                sub_08024134(player, 5, 0);
                sub_08077AEC(0);
                return 0;
            case 10:
                sub_08024134(1 - player, 15, 0);
                sub_08077AEC(0);
                return 0;
            case 12:
                sub_08024134(player, 10, 0);
                sub_08077AEC(0);
                return 0;
            case 13:
                sub_08024134(player, 14, 0);
                sub_08077AEC(0);
                return 0;
            case 14:
                sub_08024134(player, 15, 0);
                sub_08077AEC(0);
                return 0;
            case 15:
                sub_08024134(1 - player, 10, 0);
                sub_08077AEC(0);
                return 0;
            }
            return 0;
        case 1:
            switch (gUnk_0201CFB0.zone) {
            case 0:
                sub_08024134(player, 5, index);
                sub_08077AEC(0);
                return 0;
            case 5:
                sub_08024134(player, 11, 0);
                sub_08077AEC(0);
                return 0;
            case 11:
                sub_08024134(1 - player, 11, 0);
                sub_08077AEC(0);
                return 0;
            case 10:
                sub_08024134(player, 12, 0);
                sub_08077AEC(0);
                return 0;
            case 12:
                sub_08024134(1 - player, 13, 0);
                sub_08077AEC(0);
                return 0;
            case 13:
                sub_08024134(1 - player, 12, 0);
                sub_08077AEC(0);
                return 0;
            case 14:
                sub_08024134(player, 13, 0);
                sub_08077AEC(0);
                return 0;
            case 15:
                sub_08024134(player, 14, 0);
                sub_08077AEC(0);
                return 0;
            }
            return 0;
        }
    }
    if (keys & 0x80) {
        switch (player) {
        case 0:
            switch (gUnk_0201CFB0.zone) {
            case 0:
                sub_08024134(player, 5, index);
                sub_08077AEC(0);
                return 0;
            case 5:
                sub_08024134(player, 11, 0);
                sub_08077AEC(0);
                return 0;
            case 11:
                sub_08024134(1 - player, zone, 0);
                sub_08077AEC(0);
                return 0;
            case 10:
                sub_08024134(player, 12, 0);
                sub_08077AEC(0);
                return 0;
            case 12:
                sub_08024134(1 - player, 13, 0);
                sub_08077AEC(0);
                return 0;
            case 13:
                sub_08024134(1 - player, 12, 0);
                sub_08077AEC(0);
                return 0;
            case 14:
                sub_08024134(player, 13, 0);
                sub_08077AEC(0);
                return 0;
            case 15:
                sub_08024134(player, 14, 0);
                sub_08077AEC(0);
                return 0;
            }
            return 0;
        case 1:
            switch (gUnk_0201CFB0.zone) {
            case 0:
                sub_08024134(1 - player, zone, 4 - index);
                sub_08077AEC(0);
                return 0;
            case 5:
                sub_08024134(player, 0, index);
                sub_08077AEC(0);
                return 0;
            case 11:
                sub_08024134(player, 5, 0);
                sub_08077AEC(0);
                return 0;
            case 10:
                sub_08024134(1 - player, 15, 0);
                sub_08077AEC(0);
                return 0;
            case 12:
                sub_08024134(player, 10, 0);
                sub_08077AEC(0);
                return 0;
            case 13:
                sub_08024134(player, 14, 0);
                sub_08077AEC(0);
                return 0;
            case 14:
                sub_08024134(player, 15, 0);
                sub_08077AEC(0);
                return 0;
            case 15:
                sub_08024134(1 - player, 10, 0);
                sub_08077AEC(0);
                return 0;
            }
            return 0;
        }
    }
    if (gUnk_03000040.keys & 1)
        return 1;
    return 0;
}
void sub_080536D4(int hidden)
{
    int i = 0;
    u32 *base = gUnk_02017F84;
    u32 y = (0x60 - (hidden << 4)) << 16;
    int x = 0x24;
    u32 *cards;
    const u16 *colors;
    u16 *frame;
    u8 *sel;
    asm("" :: "r"(base)); /* FAKEMATCH: keep base live so combine leaves the copy */
    cards = base;
    sel = &gUnk_02017F7C;
    colors = gUnk_081A4424;
    frame = &gUnk_0300489E;
    for (; i < 5; i++) {
        u16 tile = sub_08062140((*cards << 20) >> 20) | 0x1000;
        u32 flags;
        u32 position;
        if (hidden != 0)
            tile = 0x40;
        position = x | y;
        if (i == *sel)
            flags = *(const u16 *)((u32)colors + (*frame & 0x1E)) << 16;
        else
            flags = 0x1000000;
        sub_08076714(position, 0x80, tile, flags);
        x += 0x20;
        cards++;
    }
}


void sub_08053770(int hidden, int from, int to)
{
    int i = 0;
    /* FAKEMATCH: pin cards to r8; otherwise hidden takes r8 and cards r9 */
    register u32 *cards asm("r8") = gUnk_02017F84;
    for (; i < 5; i++) {
        u16 id = (cards[i] << 20) >> 20;
        int x = (i << 5) + 0x24;
        int y = 0x60 - (hidden << 4);
        u16 tile = sub_08062140(id) | 0x1000;
        u32 position, flags;
        if (hidden != 0)
            tile = 0x40;
        if (i == from) {
            /* FAKEMATCH: table pointers set at block start (y before x) give
             * long lifetimes so loop.c hoists both; x then wins sl and y is
             * spilled and rematerialized by reload, which shifts reload's
             * register rotation so the `to` compare loads into r2. u8 phase
             * adds combinable insns that keep 0x03000040 from being hoisted
             * in the second loop pass. */
            const int *oy = gUnk_0819D280;
            const int *ox = gUnk_0819D27C;
            u8 phase = ((u32)*(u16 *)((u8 *)cards - 6) << 20) >> 24;
            x += *(const int *)((phase << 3) + (u32)ox);
            y -= *(const int *)((u32)oy + (phase << 3));
        }
        if (i == to) {
            const int *oy = gUnk_0819D280;
            const int *ox = gUnk_0819D27C;
            u8 phase = ((u32)*(u16 *)((u8 *)cards - 6) << 20) >> 24;
            x -= *(const int *)((phase << 3) + (u32)ox);
            y += *(const int *)((u32)oy + (phase << 3));
        }
        position = (y << 16) | x;
        if (i == gUnk_02017F7C)
            flags = gUnk_081A4424[(gUnk_03000040.frame & 0x1E) / 2] << 16;
        else
            flags = 0x1000000;
        sub_08076714(position, 0x80, tile, flags);
    }
}

void sub_08053864(void)
{
    struct PromptWork *work = &gUnk_02017A40;
    u32 *animation = &work->animation;
    switch ((int)((*animation << 12) >> 24)) {
    case 10:
        if ((int)(((u32)*(u16 *)((u8 *)work + 0x53E) << 20) >> 24) <= 15) {
            int to = *(u8 *)animation;
            sub_08053770(0, to - 1, to);
            break;
        }
        sub_080536D4(0);
        break;
    case 20:
        if ((int)(((u32)*(u16 *)((u8 *)work + 0x53E) << 20) >> 24) <= 15) {
            int from = *(u8 *)animation;
            sub_08053770(0, from, from + 1);
            break;
        }
        sub_080536D4(0);
        break;
    default:
        sub_080536D4(0);
        break;
    }
}
struct PW38C8Card { u32 id:12; u32 rest:20; };
struct PW38C8 {
    u8 pad[0x53C];
    u32 cursor:8;
    u32 unk8:4;
    u32 mode:8;
    u32 phase:8;
    u32 unk28:4;
    u32 unk540;
    u32 cards[5];
};
#define gPW38C8 (*(struct PW38C8 *)&gUnk_02017A40)

int sub_080538C8(void)
{
    switch (gPW38C8.mode) {
    case 10:
        if (gPW38C8.phase <= 15) {
            gPW38C8.phase++;
        } else {
            sub_08007560(&gPW38C8.cards[gPW38C8.cursor - 1], &gPW38C8.cards[gPW38C8.cursor]);
            gPW38C8.cursor--;
            gPW38C8.mode = 1;
        }
        return 0;
    case 20:
        if (gPW38C8.phase <= 15) {
            gPW38C8.phase++;
        } else {
            sub_08007560(&gPW38C8.cards[gPW38C8.cursor + 1], &gPW38C8.cards[gPW38C8.cursor]);
            gPW38C8.cursor++;
            gPW38C8.mode = 1;
        }
        return 0;
    default:
        if (gUnk_03000040.keys & 0x20) {
            sub_08077AEC(0);
            gPW38C8.cursor = (gPW38C8.cursor + 4) % 5;
            sub_0805ED9C();
            sub_0805F074((gPW38C8.cards[gPW38C8.cursor] << 20) >> 20, 1);
        }
        break;
    }
    if (gUnk_03000040.keys & 0x10) {
        sub_08077AEC(0);
        gPW38C8.cursor = (gPW38C8.cursor + 1) % 5;
        sub_0805ED9C();
        sub_0805F074((gPW38C8.cards[gPW38C8.cursor] << 20) >> 20, 1);
    }
    if (gUnk_03000040.keys & 0x200) {
        if (gPW38C8.cursor != 0) {
            sub_08077AEC(6);
            gPW38C8.mode = 10;
            gPW38C8.phase = 0;
            return 0;
        }
        sub_08077AEC(3);
    }
    if (gUnk_03000040.keys & 0x100) {
        if (gPW38C8.cursor <= 3) {
            sub_08077AEC(6);
            gPW38C8.mode = 20;
            gPW38C8.phase = 0;
            return 0;
        }
        sub_08077AEC(3);
    }
    if (gUnk_03000040.keys & 1) {
        sub_08077AEC(1);
        gPW38C8.mode = 15;
        return 1;
    }
    return 0;
}

struct AF8Card { u32 id:12; u32 rest:20; };
struct AF8 {
    u8 pad[0x53C];
    u32 cursor:8;   /* +0x53C bits 0-7 */
    u32 unk8:4;
    u32 mode:8;     /* bits 12-19 */
    u32 phase:8;    /* bits 20-27 */
    u32 timer:8;    /* bits 28-35: straddles into +0x540 */
    u32 unk36:8;
    u32 unk44:20;
    u32 cards[5]; /* +0x544 */
};
#define gAF8 (*(struct AF8 *)&gUnk_02017A40)

int sub_08053AF8(int arg0)
{
    int mode = gAF8.mode;
    switch (mode) {
    case 0:
        if (arg0 == 0) {
            sub_080602A4(0x206, 0x813, 0xB, &gUnk_080862C4);
            sub_08060308(5, sub_08053864, sub_080538C8);
            gAF8.timer = mode;
            sub_0805ED9C();
            ((void (*)(u16, u16))sub_0805F074)(gAF8.cards[0] << 20 >> 20, 1);
        }
        gAF8.mode++;
        return 0;
    case 1:
        if (arg0 == 0)
            return 1;
        sub_080536D4(arg0);
        if (gAF8.timer <= 0x1D) {
            gAF8.timer++;
            return 0;
        }
        gAF8.timer = 0;
        if (gAF8.cursor <= 3) {
            u32 cur = *(gAF8.cards + gAF8.cursor) << 20 >> 20;
            u32 nxt = *(gAF8.cards + (gAF8.cursor + 1)) << 20 >> 20;
            if (sub_08056300(1, ((const u16 *)0x08622AB4)[cur & 0x7FF]) == 0 &&
                sub_08056300(1, ((const u16 *)0x08622AB4)[nxt & 0x7FF]) != 0) {
                gAF8.mode = 20;
                gAF8.phase = 0;
                return 0;
            }
        }
        if (gAF8.cursor != 0) {
            u32 cur = *(gAF8.cards + gAF8.cursor) << 20 >> 20;
            u32 prv = *(gAF8.cards + (gAF8.cursor - 1)) << 20 >> 20;
            if (sub_08056300(1, ((const u16 *)0x08622AB4)[cur & 0x7FF]) != 0 &&
                sub_08056300(1, ((const u16 *)0x08622AB4)[prv & 0x7FF]) == 0) {
                gAF8.mode = 10;
                gAF8.phase = 0;
                return 0;
            }
        }
        if (gAF8.cursor > 3)
            return 1;
        gAF8.cursor++;
        return 0;
    case 10:
        if (arg0 == 0)
            return 0;
        if (gAF8.phase <= 15) {
            sub_08053770(arg0, gAF8.cursor - 1, gAF8.cursor);
            gAF8.phase++;
        } else {
            sub_08007560(&gAF8.cards[gAF8.cursor - 1], &gAF8.cards[gAF8.cursor]);
            gAF8.mode = 1;
            sub_080536D4(arg0);
        }
        return 0;
    case 20:
        if (arg0 == 0)
            return 0;
        if (gAF8.phase <= 15) {
            sub_08053770(arg0, gAF8.cursor, gAF8.cursor + 1);
            gAF8.phase++;
        } else {
            sub_08007560(gAF8.cards + (gAF8.cursor + 1), &gAF8.cards[gAF8.cursor]);
            gAF8.mode = 1;
            sub_080536D4(arg0);
        }
        return 0;
    }
    return 1;
}
