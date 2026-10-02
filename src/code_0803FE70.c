#include "global.h"

/*
 * Target-selection routines ("choose a card/zone" prompts), int f(struct CardRef *ref), returning 1 when done and 0 while waiting.
 * Step counter at 0x02017A40+0x3E5 (0 = prompt, >0 = wait for input). See wiki/functions/code-0803fe70.md.
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
extern const u8 gUnk_080848E4[];
extern const u8 gUnk_08084694[];
extern const char gUnk_080846CC[];
int sub_0802C080(struct CardRef *ref, u16 pos);
int sub_0800CCCC(int a, int b, int c, int d);
int sub_0800CD68(int a, int b);
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
extern const u8 gUnk_08084704[];
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
extern const u8 gUnk_0808484C[];
int sub_0802EFC4(struct CardRef *ref, int a, int b);
extern const u8 gUnk_080844E4[], gUnk_08084888[];
extern const u8 gUnk_08084658[];
int sub_0802F5F8(u16 num, struct CardRef *other, int p, int zn);
extern const u8 gUnk_08084520[], gUnk_08084558[], gUnk_08084590[];
extern const u8 gUnk_08084620[];
extern const u16 gUnk_08622AB4[];
extern const u8 gUnk_080845C8[];
int sub_08060B4C(void);
void sub_08073574(void);
int sub_0806F05C(void);
void sub_080609C4(void);
int sub_08060B2C(void);
void sub_08019820(int player, int id);
extern const u8 gUnk_08084814[];
struct DuelCard { u32 id : 12; u32 unk12 : 20; };
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flags6;
    u8 unk7[0x94 - 7];
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
#define ZA(o) ((struct DuelZone *)((o) + (0xD64 * ((1 - ref->player) & 1)) + (u32)gUnk_0201930C))
#define ZO(p, o) ((struct DuelZone *)((p) * 0xD64 + (o) + (u32)gUnk_0201930C))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
extern const u8 gUnk_08084788[];
extern const u8 gUnk_080847C8[];

int sub_0803FE70(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    if (es[0x3E5] == 0) {
        ref->numTargets = 0;
        switch (((const u16 *)0x08622AB4)[ref->id & 0x7FF]) {
        case 0x433:
        case 0x3F7:
        case 0x5FE:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084520);
            break;
        case 0x3F8:
        case 0x434:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084558);
            break;
        case 0x43A:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084590);
            break;
        default:
            return 1;
        }
        gUnk_02017A40[0x3E5]++;
    } else if (sub_08052F38(0xE000E0) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
        sub_08077AEC(3);
    }
    return 0;
}
int sub_0803FF8C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080845C8);
        ref->numTargets = 0;
        (*st)++;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0x700000) != 0) {
        u32 p = gUnk_0201CFB0.w824;
        int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        if (sub_0802B1B8(ref->id, p, zn) != 0) {
            sub_0803DDAC(ref, p, zn);
            return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}
int sub_08040040(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084620);
        (*st)++;
    } else if (sub_08052F38(0xE000E0) != 0) {
        u32 p = gUnk_0201CFB0.w824;
        int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        int pl = 1 & p;
        u16 n = ((const u16 *)0x08622AB4)[(*(u32 *)ZB2(pl, zn) << 21) >> 21];
        if ((u16)(n - 0x780) > 0x4F) {
            if (sub_0803DDAC(ref, p, zn) != 0)
                return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}
int sub_08040110(struct CardRef *ref, struct CardRef *other)
{
    char buf[0x80];
    if (((const u16 *)0x08622AB4)[ref->id & 0x7FF] == 0x525 && (((u8 *)ref)[3] & 0xFC) == 0x40) {
        u8 *es = gUnk_02017A40;
        u8 *st = es + 0x3E5;
        if (*st == 0) {
            ref->numTargets = 0;
            sub_08022678(ref->player, 0x14, ref->unk8 >> 8, 0);
            (*st)++;
            return 0;
        }
        sub_0803DDAC(ref, ref->player, gUnk_020192E0.w1B64);
        return 1;
    } else {
        u8 *es = gUnk_02017A40;
        u8 *st = es + 0x3E5;
        if (*st == 0) {
            sub_080753F4(buf, gUnk_08084658, gUnk_0822C720[other->id]);
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
            return 0;
        } else {
            /* Effect-table entries 216/327 select this routine for 0x431/0x525.
             * ROM assigns keys only for those numbers; its default path
             * passes the existing r2 value unchanged. Preserve that behavior. */
            int keys;
            switch (((const u16 *)0x08622AB4)[ref->id & 0x7FF]) {
            case 0x431:
                keys = 0xF000F0;
                break;
            case 0x525:
                keys = 0xF0;
                break;
            }
            if (sub_08052F38(keys) != 0) {
                u32 p = gUnk_0201CFB0.w824;
                int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
                if (sub_0802F5F8(((const u16 *)0x08622AB4)[ref->id & 0x7FF], other, p, zn) != 0) {
                    u32 a = (1 - p) << 24;
                    u32 b = (u32)zn << 24;
                    if (other->targets[0] != (u16)((a >> 8 | b) >> 16)) {
                        sub_0803DDAC(ref, p, zn);
                        return 1;
                    }
                }
                sub_08077AEC(3);
            }
        }
    }
    return 0;
}

#if 0 /* NONMATCHING: decoded (a 4-step machine) but far from matching. The ROM keeps the step pointer in r9 and the three cursor addresses in r8/r7/sl, and shares the inc block placed after case 0. */
int sub_08040294(struct CardRef *ref)
{
    char buf[0x80];
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int v = *st;
    switch (v) {
    case 0:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084694);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    case 1:
        if (sub_08052F38(0xC000C) == 0)
            return 0;
        {
            u32 p = gUnk_0201CFB0.w824;
            int sum = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
            u16 w = (u8)p | (u8)sum << 8;
            if (sub_0802C080(ref, w) != 0) {
                u16 msg;
                sub_08077AEC(1);
                msg = (v & ((u8 *)ref)[2]) ? 0x8008 : 8;
                sub_0801EC58(msg, gUnk_0201CFB0.w824, (u8)gUnk_0201CFB0.w828 | (u8)gUnk_0201CFB0.w82C << 8, 0);
                sub_0803DD7C(ref, w);
                (*st)++;
                return 0;
            }
            sub_08077AEC(3);
        }
        return 0;
    case 2: {
        struct DuelZone *zp = ZB(1 & ref->targets[0], ref->targets[0] >> 8);
        sub_080753F4(buf, gUnk_080846CC, gUnk_0822C720[(*(u32 *)zp << 20) >> 20]);
        sub_080602A4(0x206, 0x712, 0xB, buf);
        (*st)++;
        return 0;
    }
    case 3:
        if (sub_08052F38(0xE000E0) == 0)
            return 0;
        {
            s16 p = gUnk_0201CFB0.w824;
            int tp = (u8)ref->targets[0];
            s16 zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
            int tz = ref->targets[0] >> 8;
            if (sub_0800CCCC(tp, tz, p, zn) != 0) {
                if (sub_0800CD68(tp, tz) != (u16)(((p << 24) >> 8 | ((u32)zn << 24)) >> 16)) {
                    (*st)++;
                    sub_0803DDAC(ref, p, zn);
                    return 0;
                }
            }
            sub_08077AEC(3);
        }
        return 0;
    default:
        return 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0803FE70", sub_08040294); /* 0x08040294 size 0x1E4 */
/* AI side: per player, zones 5-10, first a card with flags6 bit 1 and type 0x16, then any card without that flag; human side: prompt + cursor. */
int sub_08040478(struct CardRef *ref)
{
    int pl = 1 & ((u8 *)ref)[2];
    u8 *es;
    u8 *st;
    if (pl) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                u16 id = (*(u32 *)z << 20) >> 20;
                /* FAKEMATCH: the empty use raises z's allocation priority above id's, so z gets r1 and id r3 as in the ROM */
                asm("" : : "r"(z));
                if (id != 0 && (z->flags6 & 2) && CARD_TYPE(id) == 0x16) {
                    sub_0803DDAC(ref, i, j);
                    return 1;
                }
            }
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB2(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && !(z->flags6 & 2)) {
                    sub_0803DDAC(ref, i, j);
                    return 1;
                }
            }
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084704);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    } else if (gUnk_03000040.h6 & 2) {
        *st = pl;
        return 0;
    } else if (sub_08052F38(0xE000E) != 0) {
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        return 1;
    }
    return 0;
}
int sub_080405E4(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084740);
        ref->numTargets = 0;
        (*st)++;
        return z;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0xE0000) != 0) {
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        return 1;
    }
    return 0;
}
int sub_0804067C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    u16 value; /* FAKEMATCH: shared narrow temporary matches the return paths. */

    switch (value = *st) {
    case 0:
        ref->numTargets = 0;
        sub_08022678(ref->player, 0xA, 0, 0);
        (*st)++;
        return 0;
    case 1:
    case 2:
    case 3:
    case 4:
        (*st)++;
        return 0;
    case 5:
        sub_08022678(1 - ref->player, 0xB, gUnk_020192E0.w1B64, gUnk_020192E0.w1B66);
        (*st)++;
        return 0;
    default:
        sub_0803DD7C(ref, (value = gUnk_020192E0.w1B64) + 1);
        return 1;
    }
} /* 0x0804067C size 0x98 */
int sub_08040714(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        sub_08022678(ref->player, 8, 0, 0);
        break;
    case 1:
        sub_0803DD7C(ref, gUnk_020192E0.w1B64 + 1);
        break;
    default:
        return 1;
    }
    (*st)++;
    return 0;
}
int sub_0804077C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084788);
        ref->numTargets = 0;
        (*st)++;
        return z;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0xD000D0) != 0) {
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        return 1;
    }
    return 0;
}
int sub_08040818(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080847C8);
        ref->numTargets = 0;
        (*st)++;
        return z;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0x20002) != 0) {
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        return 1;
    }
    return 0;
}
/* Text-entry style prompt: steps 0..4; the result u16 at 0x03004872 becomes the target (hypothesis). */
int sub_080408B4(struct CardRef *ref)
{
    switch (SEL_STEP) {
    case 0:
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084814);
        SEL_STEP++;
        return 0;
    case 1:
        if (sub_08060B4C() != 0) {
            sub_08073574();
            gUnk_02017A40[0x3E6] = 0;
            gUnk_02017A40[0x3E7] = 0;
            SEL_STEP++;
        }
        return 0;
    case 2:
        if (sub_0806F05C() != 0) {
            sub_080609C4();
            SEL_STEP++;
        }
        return 0;
    case 3:
        if (sub_08060B2C() != 0) {
            SEL_STEP++;
        }
        return 0;
    case 4: {
        u8 *mv;
        u16 *r;
        int pl;
        ref->numTargets = 0;
        pl = ref->player;
        mv = (u8 *)&gUnk_03000040;
        r = (u16 *)(mv + 0x4872);
        sub_08019820(pl, *r);
        sub_0803DD7C(ref, *r);
        SEL_STEP++;
        return 1;
    }
    default:
        return 1;
    }
}
int sub_080409E0(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        if (sub_0802EFC4(ref, 0, 0) == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_0808484C);
        (*st)++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xE000E0) == 0)
            return 0;
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        (*st)++;
        return 0;
    case 2:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084520);
        (*st)++;
        return 0;
    case 3:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xE000E0) == 0)
            return 0;
        {
            u32 p = gUnk_0201CFB0.w824;
            int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
            u16 w = (u8)p | (u8)zn << 8;
            if (ref->targets[0] == w) {
                sub_08077AEC(3);
                return 0;
            }
            sub_0803DDAC(ref, p, zn);
        }
        return 1;
    default:
        return 1;
    }
}
int sub_08040B14(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080844E4);
        (*st)++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            int z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xF00000) == 0)
            return 0;
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        (*st)++;
        return 0;
    case 2:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084888);
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
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        (*st)++;
        return 0;
    default:
        return 1;
    }
}
int sub_08040C10(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080848E4);
        (*st)++;
        return z;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0xF00000) != 0) {
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        return 1;
    }
    return 0;
}
int sub_08040CA8(struct CardRef *ref)
{
    char buf[0x100];
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    int z = 0;
    if (*st == 0) {
        {
            register const char *fmt __asm__("r1") = gUnk_08084930;
            u16 id = gUnk_08623E1E[0];
            register u32 off __asm__("r2");
            register const char *names __asm__("r3");

            off = id << 6;
            names = (const char *)gUnk_0822C720;
            sub_080753F4(buf, fmt, (const char *)(off + (u32)names));
        }
        gUnk_0201CFB0.w828 += 0;
        sub_080753F4(buf, buf, gUnk_08084968);
        sub_080602A4(0x206, 0x712, 0xB, buf);
        ref->numTargets = 0;
        (*st)++;
    } else if (gUnk_03000040.h6 & 2) {
        *st = z;
        return z;
    } else if (sub_08052F38(0xE0) != 0) {
        u32 p = gUnk_0201CFB0.w824;
        int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        if (sub_0802C334(ref, (u8)p | (u8)zn << 8) != 0) {
            sub_0803DDAC(ref, p, zn);
            return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}
static inline struct DuelZone *ZoneFromOpponentOffset(struct CardRef *ref, int o)
{
    int product = ((1 - ref->player) & 1) * 0xD64;

    /* FAKEMATCH: preserve the offset-first address sum without emitting code. */
    __asm__("" : : "r"(product));
    return (struct DuelZone *)(o + product + (u32)gUnk_0201930C);
}

int sub_08040D84(struct CardRef *ref)
{
    char buf[0x100];
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 4; i++) {
            u32 shifted = (u32)((u8 *)ref)[2] << 31;
            int o;

            /* FAKEMATCH: load the player bit before multiplying the zone index. */
            __asm__("" : : "r"(shifted));
            o = i * 0x94;
            if ((ZoneFromOpponentOffset(ref, o)->flags6 & 2) && (*(u32 *)ZoneFromOpponentOffset(ref, o) << 20) != 0
                && sub_0800C8BC(1 - ref->player, i) == 7) {
                sub_080753F4(buf, gUnk_08084970, gUnk_080849AC);
                sub_080602A4(0x206, 0x712, 0xB, buf);
                SEL_STEP++;
                return 0;
            }
        }
        return 1;
    } else if (gUnk_03000040.h6 & 2) {
        int zero = 0;
        *st = zero;
        return zero;
    } else if (sub_08052F38(0xE00000) != 0) {
        u32 p = gUnk_0201CFB0.w824;
        int zn = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        if (sub_0800C8BC(p, zn) == 7) {
            sub_0803DDAC(ref, p, zn);
            return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}
