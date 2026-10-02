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

#if 0 /* NONMATCHING: switch layout and register allocation differ; player stride is hoisted. */
u16 sub_08052B78(u16 direction, int *player, int *zone, int *index, u32 mask)
{
    int p = *player, z = *zone, i = *index;
    int oldP = p, oldZ = z, oldI = i;
    struct Player *players = gUnk_020192E4;
    int side = p & 1;
    do {
        if (direction & 4) {
            switch (z) {
            case 0:
                if (p != 0) {
                    if (i > 0) i--;
                    else { z = 10; i = 0; }
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
            case 11:
                if (p != 0) {
                    i--;
                    if (i <= 0) i = *(u8 *)((u32)players + side * 0xD64 + 2);
                } else {
                    if (i < players[0].handCount - 1) i++;
                    else i = 0;
                }
                break;
            }
        }
        if (direction & 8) {
            switch (z) {
            case 0:
                if (p != 0) {
                    if (i <= 3) i++;
                    else { z = 10; i = 0; }
                } else {
                    if (i > 0) i--;
                    else { z = 10; i = 0; }
                }
                break;
            case 5:
                i = (p != 0 ? i + 1 : i + 4) % 5;
                break;
            case 10:
                z = 0;
                i = p != 0 ? 0 : 4;
                break;
            case 11:
                if (p != 0) {
                    if (i < *(u8 *)((u32)players + side * 0xD64 + 2) - 1) i++;
                    else i = 0;
                } else {
                    if (i <= 0) i = players[0].handCount;
                    i--;
                }
                break;
            }
        }
        if (oldP == p && oldZ == z && oldI == i)
            return 0;
    } while (!sub_08052908(p, z, i, mask));
    *zone = z;
    *player = p;
    *index = i;
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08052B78", sub_08052B78);

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
INCLUDE_ASM("asm/nonmatching/code_08052B78", sub_0805304C); /* 0x0805304C size 0x688 */
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
#if 0 /* NONMATCHING: cached pointers change register allocation and RMW scheduling. */
int sub_080538C8(void)
{
    struct PromptWork *work = &gUnk_02017A40;
    u32 *animation = &work->animation;
    u8 *cursor = (u8 *)animation;
    u16 *phase = (u16 *)((u8 *)work + 0x53E);
    u32 *cards = (u32 *)((u8 *)work + 0x544);
    switch ((int)((*animation << 12) >> 24)) {
    case 10:
        if ((int)(((u32)*phase << 20) >> 24) <= 15) {
            int next = (((u32)*phase << 20) >> 24) + 1;
            *phase = (*phase & ~0xFF0) | ((next & 0xFF) << 4);
        } else {
            sub_08007560((u8 *)work + 0x540 + *cursor * 4, cards + *cursor);
            --*cursor;
            *animation = (*animation & ~0xFF000) | 0x1000;
        }
        return 0;
    case 20:
        if ((int)(((u32)*phase << 20) >> 24) <= 15) {
            int next = (((u32)*phase << 20) >> 24) + 1;
            *phase = (*phase & ~0xFF0) | ((next & 0xFF) << 4);
        } else {
            sub_08007560((u8 *)work + 0x548 + *cursor * 4, cards + *cursor);
            ++*cursor;
            *animation = (*animation & ~0xFF000) | 0x1000;
        }
        return 0;
    default:
        if (gUnk_03000040.keys & 0x20) {
            sub_08077AEC(0);
            *cursor = (*cursor + 4) % 5;
            sub_0805ED9C();
            sub_0805F074((cards[*cursor] << 20) >> 20, 1);
        }
        break;
    }
    if (gUnk_03000040.keys & 0x10) {
        sub_08077AEC(0);
        *cursor = (*cursor + 1) % 5;
        sub_0805ED9C();
        sub_0805F074((cards[*cursor] << 20) >> 20, 1);
    }
    if (gUnk_03000040.keys & 0x200) {
        if (*cursor != 0) {
            sub_08077AEC(6);
            *animation = (*animation & ~0xFF000) | 0xA000;
            *phase &= ~0xFF0;
            return 0;
        }
        sub_08077AEC(3);
    }
    if (gUnk_03000040.keys & 0x100) {
        if (*cursor <= 3) {
            sub_08077AEC(6);
            *animation = (*animation & ~0xFF000) | 0x14000;
            *phase &= ~0xFF0;
            return 0;
        }
        sub_08077AEC(3);
    }
    if (gUnk_03000040.keys & 1) {
        sub_08077AEC(1);
        *animation = (*animation & ~0xFF000) | 0xF000;
        return 1;
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08052B78", sub_080538C8);

#if 0 /* NONMATCHING: logic fully decoded from asm, but almost every instruction differs in register allocation/scheduling (ROM keeps base 0x02017A40 in r5, &animation in r8 and shifted in r6; build rotates these). */
int sub_08053AF8(int arg0)
{
    u8 *base = (u8 *)&gUnk_02017A40;
    u32 shifted = *(u32 *)(base + 0x53C) << 12;

    switch ((int)(shifted >> 24)) {
    case 0:
        if (arg0 == 0) {
            sub_080602A4(0x206, 0x813, 0xB, &gUnk_080862C4);
            sub_08060308(5, sub_08053864, sub_080538C8);
            base[0x53F] &= 0xF;
            base[0x540] = (base[0x540] & 0xF0) | ((shifted >> 28) & 0xF);
            sub_0805ED9C();
            sub_0805F074((*(u32 *)(base + 0x544) << 20) >> 20, 1);
        }
        *(u32 *)(base + 0x53C) = (*(u32 *)(base + 0x53C) & 0xFFF00FFF) | ((((shifted >> 24) + 1) & 0xFF) << 12);
        return 0;
    case 1:
        if (arg0 == 0)
            return 1;
        sub_080536D4(arg0);
        {
            s8 timer = ((base[0x540] & 0xF) << 4) | (base[0x53F] >> 4);
            if (timer <= 0x1D) {
                u32 next = timer + 1;
                base[0x53F] = (base[0x53F] & 0xF) | ((next & 0xF) << 4);
                base[0x540] = (base[0x540] & 0xF0) | ((next >> 4) & 0xF);
                return 0;
            }
        }
        base[0x53F] &= 0xF;
        base[0x540] &= 0xF0;
        if (base[0x53C] <= 3) {
            u32 cur = (*(u32 *)(base + 0x544 + base[0x53C] * 4) << 20) >> 20;
            u32 nxt = (*(u32 *)(base + 0x548 + base[0x53C] * 4) << 20) >> 20;
            if (sub_08056300(1, gUnk_08622AB4[cur & 0x7FF]) == 0 &&
                sub_08056300(1, gUnk_08622AB4[nxt & 0x7FF]) != 0) {
                *(u32 *)(base + 0x53C) = (*(u32 *)(base + 0x53C) & 0xFFF00FFF) | 0x14000;
                *(u16 *)(base + 0x53E) &= 0xFFFFF00F;
                return 0;
            }
        }
        if (base[0x53C] != 0) {
            u32 cur = (*(u32 *)(base + 0x544 + base[0x53C] * 4) << 20) >> 20;
            u32 prv = (*(u32 *)(base + 0x540 + base[0x53C] * 4) << 20) >> 20;
            if (sub_08056300(1, gUnk_08622AB4[cur & 0x7FF]) != 0 &&
                sub_08056300(1, gUnk_08622AB4[prv & 0x7FF]) == 0) {
                *(u32 *)(base + 0x53C) = (*(u32 *)(base + 0x53C) & 0xFFF00FFF) | 0xA000;
                *(u16 *)(base + 0x53E) &= 0xFFFFF00F;
                return 0;
            }
        }
        if (base[0x53C] > 3)
            return 1;
        base[0x53C] = base[0x53C] + 1;
        return 0;
    case 10:
        if (arg0 == 0)
            return 0;
        if ((((u32)*(u16 *)(base + 0x53E)) << 20) >> 24 <= 0xF) {
            s16 p;
            sub_08053770(arg0, base[0x53C] - 1, base[0x53C]);
            p = (((u32)*(u16 *)(base + 0x53E)) << 20) >> 24;
            *(u16 *)(base + 0x53E) = (*(u16 *)(base + 0x53E) & 0xFFFFF00F) | (((p + 1) & 0xFF) << 4);
            return 0;
        }
        sub_080536D4(arg0);
        sub_08007560(base + 0x540 + base[0x53C] * 4, base + 0x544 + base[0x53C] * 4);
        *(u32 *)(base + 0x53C) = (*(u32 *)(base + 0x53C) & 0xFFF00FFF) | 0x1000;
        return 0;
    case 20:
        if (arg0 == 0)
            return 0;
        if ((((u32)*(u16 *)(base + 0x53E)) << 20) >> 24 <= 0xF) {
            u32 p;
            p = (((u32)*(u16 *)(base + 0x53E)) << 20) >> 24;
            sub_08053770(arg0, base[0x53C], base[0x53C] + 1);
            *(u16 *)(base + 0x53E) = (*(u16 *)(base + 0x53E) & 0xFFFFF00F) | (((p + 1) & 0xFF) << 4);
            return 0;
        }
        sub_08007560(base + 0x548 + base[0x53C] * 4, base + 0x544 + base[0x53C] * 4);
        *(u32 *)(base + 0x53C) = (*(u32 *)(base + 0x53C) & 0xFFF00FFF) | 0x1000;
        sub_080536D4(arg0);
        return 0;
    default:
        return 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08052B78", sub_08053AF8); /* 0x08053AF8 size 0x360 */
