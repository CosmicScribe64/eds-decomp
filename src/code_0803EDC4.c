#include "global.h"

/* Duel target pickers, continued from code_0803DD7C (see wiki/functions/code-0803dd7c.md). */
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
u16 sub_0803DDAC(struct CardRef *ref, int player, int zone);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
u32 sub_08052F38(u32 keys);
extern u8 gUnk_02017A40[];
extern const u8 gUnk_0808405C[];
extern const u8 gUnk_0808410C[];
extern const u8 gUnk_0808413C[];
extern const u8 gUnk_08084178[];
int sub_0802CE38(struct CardRef *ref, int a, int b);
int sub_08008860(int player);
struct G5EE8 { u8 unk0[4]; u32 w4; };
extern struct G5EE8 gUnk_02015EE8;
extern const u8 gUnk_080841AC[];
extern const u8 gUnk_08084200[];
extern const u8 gUnk_08084244[];
extern const u8 gUnk_08083DCC[];
int sub_0805748C(int a, int b, int c, int d);
int sub_0802B1B8(u16 id, int a, int b);
extern const u8 gUnk_0808449C[];
extern const u8 gUnk_080844C0[];
extern const u8 gUnk_080844E4[];
extern const u8 gUnk_08084318[];
extern const u8 gUnk_08084044[];
int sub_0800C894(int player, int zone);
extern const u8 gUnk_08083E14[];
extern const u8 gUnk_080843E4[];
extern const u8 gUnk_08084420[];
int sub_0802B558(struct CardRef *ref, u16 pos);
int sub_0802BE70(struct CardRef *ref, u16 pos);
void sub_08060308(u32 a, u32 b, u32 c);
void sub_0803DD7C(struct CardRef *ref, u16 v);
struct AE60 { u8 unk0[0x14]; u16 flag14; };
extern struct AE60 gUnk_0201AE60;
extern const u8 gUnk_080840A4[];
extern const u8 gUnk_080840D8[];
int sub_0802D800(struct CardRef *ref);
struct DuelCard {
    u32 id : 12;
    u32 unk12 : 20;
};
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
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))
extern const u8 gUnk_08084348[];
extern const u8 gUnk_08084368[];
extern const u8 gUnk_080843A0[];
extern const u8 gUnk_08084290[];
extern const u8 gUnk_080842CC[];
extern const u8 gUnk_08084470[];
int sub_080088A4(int player, int a, int b);
/* Step byte of the current target picker is gUnk_02017A40[0x3E5]. */
struct MainView { u8 unk0[6]; u16 h6; };
extern struct MainView gUnk_03000040;
struct DuelScreenView { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreenView gUnk_0201CFB0;

/* Prompt, wait for keys 0xD2 << 16, add the cursor position as a target. */
int sub_0803EDC4(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_0808405C);
        ref->numTargets = 0;
        (*st)++;
    } else if (gUnk_03000040.h6 & 2) {
        u8 z = 0;
        *st = z;
        return z;
    } else if (sub_08052F38(0xD2 << 16) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
        sub_08077AEC(3);
    }
    return 0;
}

/* sub_0802D800 is a condition callback that ignores its arguments; this caller
 * passes its own second argument through to it (the ROM leaves it in r1 from
 * entry to the call). The unit's prototype only names ref. */
typedef int (*CondFunc_0803EE6C)(struct CardRef *ref, int arg);

/* (ref, arg): AI: add the first two occupied, unflagged spell/trap zones (5-9); player 0: gate on sub_0802D800(ref, arg), then a two-step pick where the second target must differ from the first. */
int sub_0803EE6C(struct CardRef *ref, int arg)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int n = 0;
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 5; j <= 9; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && !(z->flags6 & 2)) {
                    sub_0803DDAC(ref, i, j);
                    n++;
                    if (n == 2)
                        return 1;
                }
            }
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        if (((CondFunc_0803EE6C)sub_0802D800)(ref, arg) == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080840A4);
        (*st)++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
        reset:
            *st = pl;
            return 0;
        }
        if (sub_08052F38(0x20002) != 0) {
            if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0) {
                (*st)++;
                return 0;
            }
            sub_08077AEC(3);
        }
        return 0;
    case 2:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080840D8);
        (*st)++;
        return 0;
    case 3:
        if (gUnk_03000040.h6 & 2)
            goto reset;
        if (sub_08052F38(0x20002) != 0) {
            u8 *base = (u8 *)&gUnk_0201CFB0;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            int pos = (u8)z << 8 | *(u8 *)pa;
            if (ref->targets[0] != pos && sub_0803DDAC(ref, p, z) != 0)
                return 1;
            sub_08077AEC(3);
        }
        return 0;
    default:
        return 0;
    }
}
#if 0 /* NONMATCHING: the AI arm differs. The ROM keeps ref in r6, i in r7, and the constants
       * 1 / 0xFFFF / zone base in r8 / sl / ip, while ours has ref in r7 and i in r8. The human
       * arm is untested (structure believed right). */
/* (ref, arg): AI picks up to two player-1 zones (card numbers 0x10-0x14) or falls back to sub_0805748C; player 0: 6-step prompt machine. */
int sub_0803F034(struct CardRef *ref, int arg)
{
    if (1 & ((u8 *)ref)[2]) {
        int i;
        ref->numTargets = 0;
        if (sub_0802CE38(ref, arg, 0) == 0)
            return 1;
        if (sub_08008860(0) <= 0)
            return 1;
        for (i = 0; i <= 1; i++) {
            int cand = 0xFFFF;
            if (gUnk_02015EE8.w4 & 0x200) {
                int j;
                for (j = 0; j <= 4; j++) {
                    if ((*(u32 *)ZB(1, i) << 20) != 0) {
                        s16 no = ((const u16 *)0x08622AB4)[*(u32 *)ZB(1, j) << 21 >> 21];
                        if (no > 0x14)
                            continue;
                        if (no < 0x10)
                            continue;
                        if (i > 0 && ref->targets[0] == ((u8)j << 8 | 1))
                            continue;
                        cand = (u8)j << 8 | 1;
                        j = 5;
                    }
                }
            }
            if (cand == 0xFFFF) {
                u8 m = -1;
                int r;
                if (i > 0)
                    m = 0;
                r = sub_0805748C(0, m, 1, 1);
                if (r >= 0)
                    cand = (u8)r << 8;
            }
            if (i > 0 && cand == ref->targets[0])
                cand = 0xFFFF;
            if (cand == 0xFFFF)
                return 1;
            sub_0803DDAC(ref, (u8)cand, (u8)(cand >> 8));
        }
        return 1;
    } else {
        u8 *es = gUnk_02017A40;
        u8 *e2 = es;
        int s = es[0x3E5];
        switch (s) {
        case 0:
            ref->numTargets = 0;
            if (sub_0802CE38(ref, arg, 0) == 0)
                return 1;
            if (sub_08008860(0) + sub_08008860(1) == 0)
                return 1;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_0808410C);
            sub_08060308(1, 0, 0);
            { u8 *e = gUnk_02017A40; e[0x3E5]++; }
            return 0;
        case 1:
        case 4:
            if (gUnk_0201AE60.flag14 == 0)
                return 1;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_0808413C);
            { u8 *e = gUnk_02017A40; e[0x3E5]++; }
            return 0;
        case 2:
            if (gUnk_03000040.h6 & 2)
                goto reset;
            if (sub_08052F38(0xF000F0) == 0)
                return 0;
            if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0) {
                s16 n = sub_08008860(0);
                n += sub_08008860(1);
                if (n == 1)
                    return 1;
                { u8 *e = gUnk_02017A40; e[0x3E5]++; }
            }
            sub_08077AEC(3);
            return 0;
        case 3:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084178);
            sub_08060308(1, 0, 0);
            { u8 *e = gUnk_02017A40; e[0x3E5]++; }
            return 0;
        case 5:
            if (gUnk_03000040.h6 & 2) {
            reset:
                {
                    u8 *q = e2 + 0x3E5;
                    u8 zz = 0;
                    *q = zz;
                    return zz;
                }
            }
            if (sub_08052F38(0xF000F0) == 0)
                return 0;
            {
                u8 *base = (u8 *)&gUnk_0201CFB0;
                u32 *pa = (u32 *)(base + 0x824);
                int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
                int p = *pa;
                int pos = (u8)z << 8 | *(u8 *)pa;
                if (ref->targets[0] != pos) {
                    if (sub_0803DDAC(ref, p, z) != 0)
                        return 1;
                }
            }
            sub_08077AEC(3);
            return 0;
        default:
            return 0;
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0803EDC4", sub_0803F034); /* 0x0803F034 size 0x310 */
/* AI: sub_0805748C(0, -1, 1, 1) result becomes target 0; player 0: per-card prompt (only if sub_080088A4 allows), then the pick is validated per card number (0x42C: not 0x547, 0x42C/0x4DC: face-down flag 2). */
int sub_0803F344(struct CardRef *ref)
{
    int one;
    int pl;
    u32 keys;
    pl = 1 & ((u8 *)ref)[2];
    one = 1;
    if (pl) {
        int m;
        int r;
        ref->numTargets = 0;
        m = -1;
        r = sub_0805748C(0, m, 1, one);
        if (r > m)
            sub_0803DDAC(ref, 0, r);
        return 1;
    }
    if (gUnk_02017A40[0x3E5] == 0) {
        ref->numTargets = 0;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x280:
            if (sub_080088A4(one - ref->player, 0, 0) == 0)
                return 1;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_080841AC);
            break;
        case 0x403:
            if (sub_080088A4(one - ref->player, 0, 0) == 0)
                return 1;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084200);
            break;
        case 0x42C:
        case 0x5EA:
            if (sub_080088A4(1 - ref->player, 1, 0) == 0)
                return 1;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084244);
            break;
        default:
            if (sub_080088A4(1 - ref->player, 0, 0) == 0)
                return 1;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083DCC);
            break;
        }
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
        return 0;
    }
    switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
    case 0x280:
    case 0x403:
        keys = 0xF0 << 16;
        break;
    case 0x42C:
    case 0x4DC:
    case 0x5EA:
        keys = 0xE0 << 16;
        break;
    }
    if (sub_08052F38(keys) != 0) {
        u8 *base = (u8 *)&gUnk_0201CFB0;
        u32 *pa = (u32 *)(base + 0x824);
        int p = *pa;
        int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
        int pp = 1 & p;
        struct DuelZone *zn = ZB(pp, z);
        u16 id = (*(u32 *)zn << 20) >> 20;
        if (sub_0802B1B8(ref->id, p, z) != 0) {
            switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
            case 0x42C:
                if (((const u16 *)0x08622AB4)[0x7FF & id] == 0x547) {
                snd:
                    sub_08077AEC(3);
                    return 0;
                }
            case 0x4DC:
                {
                    int pq = 1 & p;
                    if (!(ZB2(pq, z)->flags6 & 2))
                        goto snd;
                }
                break;
            }
            sub_0803DDAC(ref, p, z);
            return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}

/* Prompt only if sub_080088A4(p, 1, 0) allows it (else done at once), keys 0xE0. */
int sub_0803F5F0(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (sub_080088A4(ref->player, 1, 0) == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084290);
        (*st)++;
    } else if (gUnk_03000040.h6 & 2) {
        u8 z = 0;
        *st = z;
        return z;
    } else if (sub_08052F38(0xE0) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
    }
    return 0;
}

/* Prompt (gUnk_080842CC), then keys 0x900090 add the cursor position unconditionally. */
int sub_0803F69C(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080842CC);
        (*st)++;
        return 0;
    }
    if (gUnk_03000040.h6 & 2) {
        u8 z = 0;
        *st = z;
        return z;
    }
    if (sub_08052F38(0x900090) != 0) {
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        return 1;
    }
    return 0;
}

/* AI: per side, the face-down (flag 2) zone with the highest sub_0800C894 value; player 0: per-card prompt (0x3AB / 0x5AB), keys 0xE000E0. */
int sub_0803F738(struct CardRef *ref)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int best = -1;
            int bestZ = -1;
            int j;
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && (z->flags6 & 2)) {
                    int v = sub_0800C894(i, j);
                    if (v > best) {
                        best = v;
                        bestZ = j;
                    }
                }
            }
            if (bestZ >= 0) {
                if (sub_0803DDAC(ref, i, bestZ) != 0)
                    return 1;
            }
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x3AB:
        case 0x5AB:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084318);
            break;
        default:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084044);
            break;
        }
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
    } else if (gUnk_03000040.h6 & 2) {
        *st = pl;
    } else if (sub_08052F38(0xE000E0) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
    }
    return 0;
}

/* Per-card prompt (numbers 0x3B1, 0x524, 0x527), then keys 0xF0 add the cursor position unconditionally. */
int sub_0803F8D4(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x3B1:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084348);
            break;
        case 0x524:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084368);
            break;
        case 0x527:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_080843A0);
            break;
        }
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
        break;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            u8 z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xF0) != 0) {
            sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
            return 1;
        }
        break;
    }
    return 0;
}

/* Prompt, cursor pick accepted by sub_0802B558, then a second prompt (0x613), and finally the value 0x0201AE60+0x14 + 1 as a target. */
int sub_0803F9F4(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    switch (*st) {
    case 0:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083E14);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    case 1:
        if (gUnk_03000040.h6 & 2) {
            u8 z = 0;
            *st = z;
            return z;
        }
        if (sub_08052F38(0xE000E0) != 0) {
            u8 *base = (u8 *)&gUnk_0201CFB0;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            if (sub_0802B558(ref, (u8)z << 8 | *(u8 *)pa) != 0) {
                sub_0803DDAC(ref, p, z);
                (*st)++;
                return 0;
            }
            sub_08077AEC(3);
        }
        break;
    case 2:
        sub_080602A4(0x206, 0x613, 0xB, gUnk_080843E4);
        sub_08060308(2, 0, 0);
        (*st)++;
        return 0;
    case 3:
        sub_0803DD7C(ref, gUnk_0201AE60.flag14 + 1);
        return 1;
    }
    return 0;
}

/* AI: first zone 0-4 of the opponent side accepted by sub_0802BE70; player 0: prompt (gUnk_08084420), keys 0xE0 << 16, pick must be accepted (opponent side). */
int sub_0803FB00(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (1 & ((u8 *)ref)[2]) {
            int i;
            for (i = 0; i <= 4; i++) {
                if (sub_0802BE70(ref, (u8)(1 - ref->player) | (u8)i << 8) != 0) {
                    sub_0803DDAC(ref, 1 - ref->player, i);
                    return 1;
                }
            }
            return 1;
        }
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084420);
        (*st)++;
    } else if (sub_08052F38(0xE0 << 16) != 0) {
        u8 *base = (u8 *)&gUnk_0201CFB0;
        u32 *pa = (u32 *)(base + 0x824);
        int p = *pa;
        int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
        if (sub_0802BE70(ref, (u8)(1 - ref->player) | (u8)*(u32 *)(base + 0x82C) << 8) != 0) {
            sub_0803DDAC(ref, p, z);
            return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}

/* Prompt (gUnk_08084470), then keys 0x80008 add the cursor position unconditionally. */
int sub_0803FBEC(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08084470);
        ref->numTargets = 0;
        (*st)++;
        return 0;
    }
    if (gUnk_03000040.h6 & 2) {
        u8 z = 0;
        *st = z;
        return z;
    }
    if (sub_08052F38(0x80008) != 0) {
        sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
        return 1;
    }
    return 0;
}

/* 6-step picker: three cursor picks with prompts between them; the third
 * pick (keys 0xF0 << 16) must succeed to finish. */
int sub_0803FC88(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    int s = es[0x3E5];
    u8 *e2 = es;
    switch (s) {
    case 0:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_0808449C);
        ref->numTargets = 0;
        goto inc;
    case 1:
        if (gUnk_03000040.h6 & 2)
            goto reset;
        if (sub_08052F38(0xF0) == 0)
            goto ret0;
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) == 0)
            goto ret0;
        goto inc;
    case 2:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080844C0);
        {
            u8 *e = gUnk_02017A40;
            register int off __asm__("r2") = 0x3E5;
            u8 *p;
            /* FAKEMATCH: retain this initialized offset in r2 so this arm
             * keeps its address setup. The signed subtraction is e + off;
             * it preserves the ROM's ADD operand order. No instruction is
             * emitted by the empty constraint. */
            __asm__("" : "+r"(off));
            p = e - (-off);
            (*p)++;
        }
        goto ret0;
    case 3:
        if (gUnk_03000040.h6 & 2)
            goto reset;
        if (sub_08052F38(0xF0) != 0) {
            u8 *base = (u8 *)&gUnk_0201CFB0;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            int pos = (u8)z << 8 | *(u8 *)pa;
            if (ref->targets[0] != pos) {
                if (sub_0803DDAC(ref, p, z) != 0) {
                    u8 *e = gUnk_02017A40;
                    int off = 0x3E5;
                    u8 *next = e + off;
                    (*next)++;
                }
            } else {
                u32 se = 3;
                /* FAKEMATCH: retain the initialized sound id separately
                 * from case 5's call, preserving this call's branch tail.
                 * The empty constraint emits no instruction. */
                __asm__("" : "+r"(se));
                sub_08077AEC(se);
            }
        }
        goto ret0;
    case 4:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_080844E4);
    inc:
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
    ret0:
        return 0;
    case 5:
        if (gUnk_03000040.h6 & 2) {
        reset:
            {
                u8 *q = e2 + 0x3E5;
                u8 zz = 0;
                *q = zz;
                return zz;
            }
        }
        if (sub_08052F38(0xF0 << 16) != 0) {
            if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
                return 1;
            sub_08077AEC(3);
        }
        goto ret0;
    default:
        return 1;
    }
}


