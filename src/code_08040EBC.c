#include "global.h"

/*
 * Target-selection routines ("choose a card/zone" prompts). Each is
 * `int f(struct CardRef *ref)` and returns 1 when done or 0 while waiting.
 * The step counter is at 0x02017A40+0x3E5 (0 = prompt, >0 = wait for input).
 * See wiki/functions/code-08040ebc.md.
 */
struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;
    u8 unk4_3 : 5;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;  /* +0x0A bits 0-2 */
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[3];     /* +0x0C */
};

void sub_08077AEC(int a);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08022678(int a, int b, int c, int d);
void sub_0803DD7C(struct CardRef *ref, u16 v);
u16 sub_0803DDAC(struct CardRef *ref, int a, int z);
int sub_0802B1B8(u16 id, int a, int z);
u32 sub_08052F38(u32 keys);
extern u8 gUnk_02017A40[];
#define SEL_STEP gUnk_02017A40[0x3E5]
struct MainView { u8 unk0[6]; u16 h6; };
extern struct MainView gUnk_03000040;
struct DuelScreenView { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreenView gUnk_0201CFB0;
struct DuelGlobal {
    u8 unk0[0x1B64];
    u16 w1B64;          /* prompt result */
    u16 w1B66;
};
extern struct DuelGlobal gUnk_020192E0;
extern const u8 gUnk_08084740[];
void sub_080753F4(char *dst, const char *fmt, const char *arg);
int sub_0802C334(struct CardRef *ref, u16 v);
int sub_0800C8BC(int player, int zone);
extern const u16 gUnk_08623E1E[];
extern const char gUnk_0822C720[][0x40];
extern const char gUnk_08084930[], gUnk_08084968[], gUnk_08084970[], gUnk_080849AC[];
struct DuelCard { u32 id : 12; u32 unk12 : 20; };
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flags6;
    u8 unk7[0x91 - 7];
    u8 f91_0 : 1;
    u8 f91_1 : 1;
    u8 f91_2 : 1;       /* bit 2 */
    u8 f91_3 : 1;       /* bit 3 */
    u8 f91_4 : 4;
    u8 unk92[2];
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
int sub_0802C674(struct CardRef *ref, u16 pos);
extern const char gUnk_08083FD0[], gUnk_08084C3C[], gUnk_08084C84[];
void sub_08075434(char *dst, const char *fmt, int n);
int sub_08009DEC(int player);
void sub_0802AF34(int player, int area, int a2, int a3);
struct ListView {
    u8 unk0[5];
    u8 row : 2;         /* +0x05 bits 0-1: cursor row */
    u8 unk5_2 : 6;
    u16 top;            /* +0x06: first visible entry */
    u8 unk8[4];
    u32 cards[0x80];    /* +0x0C: card words */
};
extern struct ListView gUnk_0201D810;
extern const u8 gUnk_080849F4[];
int sub_0802C3C0(struct CardRef *ref, u16 pos);
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void sub_08017FF4(int player, int zone);
#define CARD_LEVEL(id, r)                                     \
    switch ((int)CARD_TYPE(id)) {                             \
    case 0x15:                                                \
    case 0x16:                                                \
    case 0x17:                                                \
        r = 0;                                                \
        break;                                                \
    case 0x18:                                                \
        r = 10;                                               \
        break;                                                \
    default:                                                  \
        r = (CARD_STATS(id) & 0x1E000000) >> 25;              \
        break;                                                \
    }
int sub_0802CD28(u16 id);
int sub_08008524(int player, u16 number);
int sub_0802CE38(struct CardRef *ref, struct CardRef *other, u16 flags);
struct DG12 { u8 pad[0x1B12]; u8 b; };
struct PS7 { u8 pad[7]; u8 b7; u8 rest[0xD64 - 8]; };
extern struct PS7 gUnk_020192E4[2];
extern const u8 gUnk_08084B6C[], gUnk_08084BA0[];
int sub_0802FCEC(struct CardRef *ref, int a, int b);
extern const u8 gUnk_08084A30[], gUnk_08084A68[];
int sub_0805304C(void);
int sub_08008940(int a, int b);
extern const u8 gUnk_080849F4[], gUnk_08084B38[];
extern const char gUnk_080849B4[];
extern const char gUnk_0822C720[][0x40];
extern const u16 gUnk_086248EE[];
void sub_080753F4(char *dst, const char *fmt, const char *arg);
void sub_0803DE40(struct CardRef *ref, int a, int z);
extern const u8 gUnk_08084AA8[], gUnk_08083E14[];
int sub_0802B558(struct CardRef *ref, u16 pos);
extern const u8 gUnk_08084C98[], gUnk_08084CEC[], gUnk_08084D20[], gUnk_08084BD4[], gUnk_08084AF8[];

static inline int CardLevel(u32 id)
{
    int r;
    CARD_LEVEL(id, r);
    return r;
}
int sub_08040EBC(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    char buf[0x100];
    if (*st == 0) {
        sub_080753F4(buf, gUnk_080849B4, gUnk_0822C720[gUnk_086248EE[0]]);
        sub_080602A4(0x206, 0x712, 0xB, buf);
        {
            int mask = ~7;
            register u8 fields __asm__("r1");

            /* FAKEMATCH: keep this bitfield-update scratch in r1. */
            fields = ((u8 *)ref)[0xA];
            __asm__("" : : "r"(fields));
            ((u8 *)ref)[0xA] = mask & fields;
        }
        (*st)++;
    } else if (sub_08052F38(0xE0) != 0) {
        u32 p = gUnk_0201CFB0.w824;
        int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        int pl = 1 & p;
        struct DuelZone *z = ZB(pl, zn);
        u32 id = (*(u32 *)z << 20) >> 20;
        if ((z->flags6 & 2) && id != 0 && ((const u16 *)0x08622AB4)[id & 0x7FF] == 0x57D) {
            sub_0803DE40(ref, p, zn);
            return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}
int sub_08040FB0(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int v = *st;
    switch (v) {
    case 0:
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080849F4);
        (*st)++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xF0) != 0) {
            u32 p = gUnk_0201CFB0.w824;
            int sum = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
            u16 w = (u8)p | (u8)sum << 8;
            if (sub_0802C3C0(ref, w) != 0) {
                u16 msg;
                int pl;
                int lvl;
                struct DuelZone *zp;
                u32 id;
                sub_08077AEC(1);
                msg = (v & ((u8 *)ref)[2]) ? 0x8008 : 8;
                sub_0801EC58(msg, gUnk_0201CFB0.w824, (u8)gUnk_0201CFB0.w828 | (u8)gUnk_0201CFB0.w82C << 8, 0);
                sub_08017FF4(p, w);
                pl = p & v;
                zp = ZB(pl, w);
                id = (*(u32 *)zp << 20) >> 20;
                CARD_LEVEL(id, lvl);
                sub_0803DD7C(ref, lvl + 1);
                return 1;
            }
            sub_08077AEC(3);
        }
        return 0;
    default:
        return 0;
    }
}
int sub_0804112C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084A30);
        (*st)++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_0805304C() != 0) {
            u32 p = gUnk_0201CFB0.w824;
            int zn = gUnk_0201CFB0.w82C;
            if (gUnk_0201CFB0.w828 == 0 && sub_08008940(p, zn) != 0) {
                sub_0803DE40(ref, p, zn);
                (*st)++;
                return 0;
            }
            sub_08077AEC(3);
        }
        return 0;
    case 2:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084A68);
        (*st)++;
        return 0;
    case 3:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_0805304C() != 0) {
            u32 p = gUnk_0201CFB0.w824;
            int zn = gUnk_0201CFB0.w82C;
            if (gUnk_0201CFB0.w828 == 0 && (*(u32 *)ZB2(1 & p, zn) << 20) == 0
                && (u16)((u8)p | (u8)zn << 8) != ref->targets[0]) {
                sub_0803DE40(ref, p, zn);
                (*st)++;
            } else
                sub_08077AEC(3);
        }
        return 0;
    default:
        return 1;
    }
}
int sub_0804128C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0: {
        int i;
        int found;
        ref->numTargets = 0;
        found = 0;
        for (i = 0; i <= 4; i++) {
            if ((*(u32 *)ZB2((1 - ref->player) & 1, i) << 20) != 0 && (ZB2((1 - ref->player) & 1, i)->flags6 & 3) == 1)
                found = 1;
        }
        if (found == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084AA8);
        gUnk_02017A40[0x3E5]++;
        break;
    }
    case 1:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0x900000) != 0) {
            if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
                return 1;
        }
        break;
    }
    return 0;
}
int sub_0804139C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084AF8);
        ref->numTargets = 0;
        (*st)++;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0x40004) != 0) {
        u32 p = gUnk_0201CFB0.w824;
        int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        if (p != ref->player || zn != ref->zone) {
            if (sub_0803DDAC(ref, p, zn) != 0)
                return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}
int sub_0804145C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080849F4);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xF0) == 0)
            return 0;
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0) {
            (*st)++;
            return 0;
        }
        return 0;
    case 2:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084B38);
        (*st)++;
        return 0;
    case 3:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xF0) == 0)
            return 0;
        {
            u32 p = gUnk_0201CFB0.w824;
            int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
            if (((u8)p | (u8)zn << 8) != ref->targets[0]) {
                if (sub_0803DDAC(ref, p, zn) != 0)
                    return 1;
            }
            sub_08077AEC(3);
        }
        return 0;
    default:
        return 0;
    }
}
int sub_0804158C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        sub_08022678(ref->player, 9, 0, 0);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    case 1:
        sub_0803DD7C(ref, gUnk_020192E0.w1B64 + 1);
        (*st)++;
        return 0;
    case 2:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083E14);
        (*st)++;
        return 0;
    default:
        if (gUnk_03000040.h6 & 2) {
            u8 *e2 = gUnk_02017A40;
            u8 *q = e2 + 0x3E5;
            int z = 0;

            *q = z;
            return z;
        }
        if (sub_08052F38(0xE000E0) != 0) {
            u32 p = gUnk_0201CFB0.w824;
            int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
            if (sub_0802B558(ref, (u8)p | (u8)zn << 8) != 0) {
                if (sub_0803DDAC(ref, p, zn) != 0)
                    return 1;
            }
            sub_08077AEC(3);
        }
        return 0;
    }
}
int sub_0804169C(struct CardRef *ref, int a)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        if (sub_0802FCEC(ref, a, 0) == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084B6C);
        (*st)++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xE000E) == 0)
            return 0;
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0) {
            (*st)++;
            return 0;
        }
        return 0;
    case 2:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084BA0);
        (*st)++;
        return 0;
    case 3:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xE000E) == 0)
            return 0;
        {
            u32 p = gUnk_0201CFB0.w824;
            int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
            u16 w = (u8)p | (u8)zn << 8;
            if (ref->targets[0] != w) {
                if (sub_0803DDAC(ref, p, zn) != 0)
                    return 1;
            }
            sub_08077AEC(3);
        }
        return 0;
    default:
        return 0;
    }
}
int sub_080417DC(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084BD4);
        ref->numTargets = 0;
        (*st)++;
    } else if (sub_08052F38(0xE00000) != 0) {
        u32 p = gUnk_0201CFB0.w824;
        int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        int pl = 1 & p;
        if ((*(u32 *)ZB2(pl, zn) << 20) != 0 && (ref->pos >> 8) != zn) {
            if (sub_0803DDAC(ref, p, zn) != 0)
                return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}
/*
 * Card level as a u8 inline with a return per case: the QImode result pseudo
 * is copied to its use through a subreg, which reproduces the ROM's level in
 * r0 plus a register copy (`adds r4,r0,#0` / `adds r1,r0,#0`).
 */
static inline u8 CardLevelU8(u32 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (CARD_STATS(id) & 0x1E000000) >> 25;
    }
}
int sub_08041898(struct CardRef *ref)
{
    char buf1[0x40];
    char buf2[0x80];
    u8 *es;
    int sw = gUnk_02017A40[0x3E5];
    es = gUnk_02017A40;
    switch (sw) {
    case 0:
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083FD0);
        gUnk_02017A40[0x3E5]++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            u8 *q = es + 0x3E5;
            int z = 0;
            *q = z;
            return z;
        }
        if (sub_08052F38(0xE000E0) == 0)
            goto ret0;
        {
            u32 p = gUnk_0201CFB0.w824;
            int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
            int pl = 1 & p;
            struct DuelZone *zp = ZB(pl, zn);
            u32 id = (*(u32 *)zp << 20) >> 20;
            if (CardLevelU8(id) <= sub_08009DEC(ref->player)) {
                if (sub_0803DDAC(ref, p, zn) != 0) {
                    sub_0803DD7C(ref, CardLevelU8(id));
                    gUnk_02017A40[0x3E6] = CardLevelU8(id);
                    gUnk_02017A40[0x3E5]++;
                    return 0;
                }
            }
            sub_08077AEC(3);
        }
        goto ret0;
    case 2:
        sub_08075434(buf1, gUnk_08084C3C, *(es + 0x3E6));
        sub_080602A4(0x206, 0x712, 0xB, buf1);
        (*(es + 0x3E5))++;
        return 0;
    case 3:
        sub_0802AF34(ref->player, -1, 0x5FC, 0);
        gUnk_02017A40[0x3E5]++;
        return 0;
    case 4: {
        u32 *c = &gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row];
        u16 msg = (1 & ((u8 *)ref)[2]) ? 0x80D4 : 0xD4;
        sub_0801EC58(msg, ((u16 *)c)[0], ((u16 *)c)[1], 0);
        gUnk_02017A40[0x3E6]--;
        gUnk_02017A40[0x3E5]++;
        return 0;
    }
    case 5:
        /* The positive test keeps the fall-through label-free, so post-reload
         * CSE turns the second count load into `adds r2,r0,#0`; the ret0 label
         * here makes this tail merge into case 4's `return 0`. */
        if (*(es + 0x3E6) != 0) {
            sub_08075434(buf2, gUnk_08084C84, *(es + 0x3E6));
            sub_080602A4(0x206, 0x712, 0xB, buf2);
            *(es + 0x3E5) = 3;
ret0:
            return 0;
        }
        return 1;
    default:
        return 1;
    }
}
int sub_08041BC0(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084C98);
        ref->numTargets = 0;
        (*st)++;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0xE0000) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
    }
    return 0;
}
int sub_08041C60(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084CEC);
        (*st)++;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0x20002) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
    }
    return 0;
}
int sub_08041D00(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084D20);
        (*st)++;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0xF000F0) != 0) {
        u32 p = gUnk_0201CFB0.w824;
        int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        if (sub_0802C674(ref, (u8)p | (u8)zn << 8) != 0) {
            if (sub_0803DDAC(ref, p, zn) != 0)
                return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}
/* Preserve the zero-extended halfword result for word-valued callers. */
int sub_08041DC4(struct CardRef *ref, int p, int z)
{
    u32 id;
    int f;
    int pl = 1 & p;
    struct DuelZone *zp = ZB(pl, z);
    id = (*(u32 *)zp << 20) >> 20;
    f = ((u32)zp->flags6 << 30) >> 31;
    if (id == 0)
        return 0;
    if (CARD_TYPE(id) <= 0x14)
        return 0;
    if ((((u32)((u8 *)gUnk_0201930C)[0x1AE6] << 30) >> 31) != p) {
        if (sub_0802CD28(id) <= 1)
            return 0;
    }
    switch (((const u16 *)0x08622AB4)[id & 0x7FF]) {
    case 0x52C:
    case 0x3F9:
    case 0x594:
    case 0x5FC:
        f = 0;
        break;
    }
    if (f != 0)
        return 0;
    {
        int pl2 = 1 & p;
        struct DuelZone *z2 = ZB(pl2, z);
        if (!z2->f91_2)
            return 0;
        if (z2->f91_3)
            return 0;
    }
    if (CARD_TYPE(id) == 0x15) {
        int n = 0x2EF;
        if (sub_08008524(0, n) != 0)
            return 0;
        if (sub_08008524(1, n) != 0)
            return 0;
    }
    if (sub_0802CD28(id) > 1)
        goto fill;
    {
        struct DG12 *g = (struct DG12 *)&gUnk_020192E0;
        u32 b = g->b;
        u32 t = (b << 27) >> 29;
        if (t == 2 || t == 4) {
            if (((b << 30) >> 31) == p)
                goto fill;
        }
    }
ret0:
    return 0;
fill:
    ref->id = id;
    ref->player = p & 1;
    ref->zone = z & 0x3F;
    {
        /* FAKEMATCH: preserve the ROM's p/z registers without emitting code. */
        __asm__("" : : : "r0");
        switch ((int)CARD_TYPE(id & 0x7FF)) {
        case 0x15:
        case 0x16:
            if ((gUnk_020192E4[1 & p].b7 >> 6) != 0)
                goto ret0;
            break;
        }
    }
    return (u16)sub_0802CE38(ref, 0, 0);
}
