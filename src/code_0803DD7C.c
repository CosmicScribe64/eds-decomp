#include "global.h"
#include "main.h"
#include "duel.h"

/* Duel target helpers. struct CardRef is described in wiki/functions/code-08030b88.md. */
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
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
int sub_0802B1B8(u16 id, int a, int b);
void sub_0803DD7C(struct CardRef *ref, u16 v);
u16 sub_0803DDAC(struct CardRef *ref, int a, int z);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, u32 b, u32 c);
u32 sub_08052F38(u32 keys);
int sub_08044224(int player, int number, int b);
int sub_08056ECC(u16 id);
void sub_08019820(int player, int id);
void sub_0802AF34(int player, int area, int a2, int a3);
int sub_08056544(int a);
extern const u8 gUnk_08083E8C[];
extern const u8 gUnk_08083E44[];
extern const u8 gUnk_08083CC8[];
extern const u8 gUnk_08083C50[];
extern const u8 gUnk_08083FD0[];
extern const u8 gUnk_08084000[];
extern const u8 gUnk_08084044[];
int sub_0802CE38(struct CardRef *ref, int a, int b);
int sub_08057550(int a, int b, int c, int d);
extern const u8 gUnk_08083F94[];
extern const u8 gUnk_08083FC8[];
int sub_0802BAD0(struct CardRef *ref, u16 pos);
void sub_080753F4(void *dst, const void *a, const void *b);
extern const u8 gUnk_08083F60[];
int sub_08008B70(int a, int b, int c, int d);
extern const u8 gUnk_08083E14[];
int sub_0802B558(struct CardRef *ref, u16 pos);
int sub_0800C894(int player, int zone);
#define LP gUnk_020192E4
extern const u8 gUnk_08083ED0[];
extern const u8 gUnk_08083EF4[];
extern const u8 gUnk_08083F38[];
int sub_0802D67C(struct CardRef *ref, int a, int b);
int sub_0802B9EC(struct CardRef *ref, u16 pos);
extern const u8 gUnk_08083CF8[];
extern const u8 gUnk_08083D50[];
extern const u8 gUnk_08083D90[];
extern const u8 gUnk_08083DCC[];
int sub_08008860(int player);
int sub_0805748C(int a, int b, int c, int d);
extern const u8 gUnk_08083C94[];
extern const u16 gUnk_08622AB4[];
extern u8 gUnk_02017A40[];
/* Step counter of the current target-selection routine (0 = prompt, 1 = wait for input, ...). */
#define SEL_STEP gUnk_02017A40[0x3E5]
struct AE60 { u8 unk0[0x14]; u16 flag14; };
extern struct AE60 gUnk_0201AE60;
struct DuelScreenView { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreenView gUnk_0201CFB0;
struct ListView {
    u8 unk0[5];
    u8 row : 2;
    u8 unk5_2 : 6;
    u16 top;
    u8 unk8[4];
    u32 cards[0x80];
};
extern struct ListView gUnk_0201D810;
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
extern u32 gUnk_0201D81C[];   /* = gUnk_0201D810.cards */
#define CARD_NUMBER(id) (gUnk_08622AB4[0x7FF & (id)])

/* Append a packed (zone << 8 | player) target word. */
void sub_0803DD7C(struct CardRef *ref, u16 v)
{
    if (ref != 0) {
        ref->targets[ref->numTargets] = v;
        ref->numTargets++;
    }
}

/* Add target (a, zone) if the card can be placed; msg shows the zone (0-4 monster, 5-9 spell/trap, 10 field). */
u16 sub_0803DDAC(struct CardRef *ref, int a, int z)
{
    int hi = 0;
    int lo = z;
    if (z > 4) {
        hi = 5;
        lo = z - 5;
    }
    if (z == 10) {
        hi = 10;
        lo = 0;
    }
    if (sub_0802B1B8(ref->id, a, z) != 0) {
        if (!(1 & ((u8 *)ref)[2]))
            sub_08077AEC(1);
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, a, (u8)lo << 8 | hi, 0);
        sub_0803DD7C(ref, (u8)a | (u8)z << 8);
        return 1;
    }
    return 0;
}

/* Same without the placement test. */
void sub_0803DE40(struct CardRef *ref, int a, int z)
{
    int hi = 0;
    int lo = z;
    if (z > 4) {
        do { hi = 5; lo = z - 5; } while (0); /* FAKEMATCH: block form reproduces the ROM's hi/lo register order */
    }
    if (z == 10) {
        hi = 10;
        lo = 0;
    }
    if (!(1 & ((u8 *)ref)[2]))
        sub_08077AEC(1);
    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, a, (u8)lo << 8 | hi, 0);
    sub_0803DD7C(ref, (u8)a | (u8)z << 8);
}
#if 0 /* NONMATCHING: two differences. (1) The AI arm scales i<<2 into r4
       * instead of r1 and then adds the base (the ROM does r4=r1+base). (2)
       * The default arm parks the list base in r5 (the ROM uses r4, which
       * frees after the switch), so base+0xC in r5 survives the call and is
       * reused; this build recomputes it from caller-saved r2. Case 0/1 and
       * everything else match. */
/* Pick a card from the list viewer (its 12-bit id and the two halves become targets). */
int sub_0803DEB8(struct CardRef *ref)
{
    u8 *es;
    u8 *st;
    if (1 & ((u8 *)ref)[2]) {
        int i;
        ref->numTargets = 0;
        i = sub_08056ECC(ref->id);
        if (i >= 0) {
            u16 *c = (u16 *)((u8 *)gUnk_0201D81C + (i << 2));
            sub_08019820(ref->player, *(u32 *)c << 20 >> 20);
            sub_0803DD7C(ref, c[0]);
            sub_0803DD7C(ref, c[1]);
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        if (sub_08044224(ref->player, ((const u16 *)0x08622AB4)[0x7FF & ref->id], 0) == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083C50);
        (*st)++;
        return 0;
    case 1:
        sub_0802AF34(ref->player, -1, ((const u16 *)0x08622AB4)[0x7FF & ref->id], 0);
        (*st)++;
        return 0;
    default: {
        struct ListView *lv = &gUnk_0201D810;
        u32 *c;
        sub_08019820(ref->player, lv->cards[lv->top + lv->row] << 20 >> 20);
        c = &lv->cards[lv->top + lv->row];
        sub_0803DD7C(ref, ((u16 *)c)[0]);
        sub_0803DD7C(ref, ((u16 *)c)[1]);
        return 1;
    }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0803DD7C", sub_0803DEB8); /* 0x0803DEB8 size 0x12C */

/* Auto-pick / choose a spell-trap zone target (zones 5-9): first the face-down ones of type 0x15, then any card not flagged. */
int sub_0803DFE4(struct CardRef *ref)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 5; j <= 9; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                u32 id = (*(u32 *)z << 20) >> 20;
                if (id != 0 && (z->flag6_1) && CARD_TYPE((u16)id) == 0x15) {
                    if (sub_0803DDAC(ref, i, j) != 0)
                        return 1;
                }
            }
            for (j = 5; j <= 9; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && !(z->flag6_1)) {
                    if (sub_0803DDAC(ref, i, j) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083C94);
        ref->numTargets = 0;
        (*st)++;
    } else if (gUnk_03000040.newKeys & 2) {
        *st = pl;
        return pl;
    } else if (sub_08052F38(0xA000A) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
    }
    return 0;
}


/* Select a target for player 0 by cursor; player 1 (AI) asks sub_08056544 for one. */
int sub_0803E160(struct CardRef *ref)
{
    int pl = 1 & ((u8 *)ref)[2];
    u8 *es;
    u8 *st;
    if (pl) {
        int m = -1;
        int r = sub_08056544(m);
        ref->numTargets = 0;
        if (r > m) {
            if (sub_0803DDAC(ref, 1, r) != 0)
                return 1;
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    if (*st == 0) {
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083CC8);
        ref->numTargets = 0;
        (*st)++;
    } else if (gUnk_03000040.newKeys & 2) {
        *st = pl;
        return pl;
    } else if (sub_08052F38(0xF0) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
    }
    return 0;
}

/* Target selection with a per-card prompt text (numbers 0x5E / 0x77, 0x2E6 / 0x2DA, 0x536). */
int sub_0803E22C(struct CardRef *ref)
{
    int one;
    int pl;
    u8 *es;
    u8 *st;
    pl = 1 & ((u8 *)ref)[2];
    one = 1;
    if (pl) {
        int m;
        ref->numTargets = 0;
        if (sub_08008860(0) > 0) {
            int r;
            m = one - 2;
            r = sub_0805748C(0, m, 1, 1);
            if (r > m) {
                if (sub_0803DDAC(ref, 0, r) != 0)
                    return 1;
            }
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (sub_08008860(one - ref->player) == 0)
            return 1;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x5E:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083CF8);
            break;
        case 0x77:
        case 0x2E6:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083D50);
            break;
        case 0x2DA:
        case 0x536:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083D90);
            break;
        default:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083DCC);
            break;
        }
        gUnk_02017A40[0x3E5]++;
    } else if (gUnk_03000040.newKeys & 2) {
        *st = pl;
    } else if (sub_08052F38(0xF0 << 16) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
        sub_08077AEC(3);
    }
    return 0;
}


/* AI picks the monster with the highest value (sub_0800C894) among those sub_0802B558 accepts; player 0 picks with the cursor. */
int sub_0803E3C4(struct CardRef *ref)
{
    int one;
    int pl;
    if ((((u8 *)ref)[2] & 0xE) == 6)
        return 1;
    pl = 1 & ((u8 *)ref)[2];
    one = 1;
    if (pl) {
        int side;
        int bestVal;
        int bestP;
        int bestZ;
        int i;
        ref->numTargets = 0;
        side = ref->player;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x290:
            if (LP[1 & ref->player].lifePoints > LP[(one - ref->player) & one].lifePoints)
                side = one - ref->player;
            break;
        case 0x416:
        case 0x417:
        case 0x58B:
        case 0x60C:
            side = 1 - ref->player;
            break;
        }
        bestVal = -1;
        bestP = -1;
        bestZ = -1;
        for (i = 0; i <= 1; i++, side = 1 - side) {
            int j;
            int sb;
            j = 0;
            sb = (u8)side;
            for (; j <= 4; j++) {
                if (sub_0802B558(ref, (u8)j << 8 | sb) != 0 && bestVal < sub_0800C894(side, j)) {
                    bestVal = sub_0800C894(side, j);
                    bestP = side;
                    bestZ = j;
                }
            }
            if (bestVal > -1 && bestP > -1 && bestZ > -1) {
                if (sub_0803DDAC(ref, bestP, bestZ) != 0)
                    return 1;
            }
        }
        if (bestVal > -1 && bestP > -1 && bestZ > -1) {
            if (sub_0803DDAC(ref, bestP, bestZ) != 0)
                return 1;
        }
        return 1;
    } else {
        u8 *es = gUnk_02017A40;
        u8 *st = es + 0x3E5;
        if (*st == 0) {
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083E14);
            ref->numTargets = 0;
            (*st)++;
        } else if (gUnk_03000040.newKeys & 2) {
            *st = pl;
        } else if (sub_08052F38(0xE000E0) != 0) {
            u8 *base = (u8 *)&gUnk_0201CFB0;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            if ((u16)sub_0802B558(ref, (u8)z << 8 | *(u8 *)pa) != 0) {
                if (sub_0803DDAC(ref, p, z) != 0)
                    return 1;
                return 1;
            }
            sub_08077AEC(3);
        }
    }
    return 0;
}

/* Prompt (text 0x08083E44), wait for a key press, then add the cursor position as a target. */
int sub_0803E5B8(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083E44);
        (*st)++;
        goto ret0;
    }
    if (gUnk_03000040.newKeys & 2) {
        u8 z = 0;
        *st = z;
        return z;
    }
    if (sub_08052F38(0xB0 << 16) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
    }
ret0:
    return 0;
}

/* Target selection: 1 = done (targets filled), 0 = still waiting. Player 1 (AI) picks by itself. */
int sub_0803E658(struct CardRef *ref)
{
    u8 *st;
    u8 *es;
    if (1 & ((u8 *)ref)[2]) {
        sub_0803DD7C(ref, 0);
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    switch (*st) {
    case 0:
        ref->numTargets = 0;
        sub_080602A4(0x205, 0x514, 0xB, gUnk_08083E8C);
        sub_08060308(2, 0, 0);
        break;
    case 1:
        sub_0803DD7C(ref, gUnk_0201AE60.flag14);
        break;
    default:
        return 1;
    }
    (*st)++;
    return 0;
}

/* Two-target selection (kind 2): first target, then a second one that differs from it. */
int sub_0803E6DC(struct CardRef *ref)
{
    u8 *es = gUnk_02017A40;
    int s = es[0x3E5];
    u8 *e2 = es;
    u8 *q;
    switch (s) {
    case 0:
        ref->numTargets = 0;
        ref->kind = 2;
        if (sub_0802D67C(ref, 0, 0) == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083ED0);
        sub_08060308(1, 0, 0);
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
        return 0;
    case 1:
        if (gUnk_0201AE60.flag14 == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083EF4);
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
        return 0;
    case 2:
        if (gUnk_03000040.newKeys & 2) {
            q = e2 + 0x3E5;
            goto reset;
        }
        if (sub_08052F38(0xE000E0) != 0) {
            u8 *base = (u8 *)&gUnk_0201CFB0;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            if (sub_0802B9EC(ref, (u8)z << 8 | *(u8 *)pa) != 0
                && sub_0803DDAC(ref, p, z) != 0) {
                int n = sub_08008860(0);
                n += sub_08008860(1);
                if (n == 1)
                    return 1;
                { u8 *e = gUnk_02017A40; e[0x3E5]++; }
            }
            sub_08077AEC(3);
        }
        return 0;
    case 3:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083F38);
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
        return 0;
    case 4:
        if (gUnk_03000040.newKeys & 2) {
            q = e2 + 0x3E5;
        reset:
            {
                u8 zz = 0;
                *q = zz;
                return zz;
            }
        }
        if (sub_08052F38(0xE000E0) != 0) {
            u8 *base = (u8 *)&gUnk_0201CFB0;
            u32 *pa = (u32 *)(base + 0x824);
            int z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
            int p = *pa;
            int pos = (u8)z << 8 | *(u8 *)pa;
            if (ref->targets[0] != pos && sub_0802B9EC(ref, pos) != 0 && sub_0803DDAC(ref, p, z) != 0)
                return 1;
            sub_08077AEC(3);
        }
        return 0;
    default:
        return 0;
    }
}


/* Like sub_0803DFE4 for spell/trap zones 5-10 of type 0x16. */
int sub_0803E8F8(struct CardRef *ref)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int i;
        ref->numTargets = 0;
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                u32 id = (*(u32 *)z << 20) >> 20;
                if (id != 0 && (z->flag6_1) && CARD_TYPE((u16)id) == 0x16) {
                    if (sub_0803DDAC(ref, i, j) != 0)
                        return 1;
                }
            }
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0 && !(z->flag6_1)) {
                    if (sub_0803DDAC(ref, i, j) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (sub_08008B70(0, 0, 0, 1) == 0 && sub_08008B70(1, 0, 0, 1) == 0)
            return 1;
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08083F60);
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
    } else if (gUnk_03000040.newKeys & 2) {
        *st = pl;
        return pl;
    } else if (sub_08052F38(0x60006) != 0) {
        if (sub_0803DDAC(ref, gUnk_0201CFB0.w824, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C) != 0)
            return 1;
        sub_08077AEC(3);
    }
    return 0;
}

/* Pick a card on a field position accepted by sub_0802BAD0: AI takes the first, player 0 gets a prompt (text built into a stack buffer). */
int sub_0803EA9C(struct CardRef *ref)
{
    char buf[0x80];
    u8 *es = gUnk_02017A40;
    u8 *st = es + 0x3E5;
    if (*st == 0) {
        int i;
        u8 *st2;
        ref->numTargets = 0;
        i = 0;
        st2 = st;
        for (; i <= 1; i++) {
            int j;
            int sb;
            j = 0;
            sb = (u8)i;
            for (; j <= 4; j++) {
                if (sub_0802BAD0(ref, (u8)j << 8 | sb) != 0) {
                    if (1 & ((u8 *)ref)[2]) {
                        if (sub_0803DDAC(ref, i, j) != 0)
                            return 1;
                        return 1;
                    }
                    sub_080753F4(buf, gUnk_08083F94, gUnk_08083FC8);
                    sub_080602A4(0x206, 0x712, 0xB, buf);
                    (*st2)++;
                    return 0;
                }
            }
        }
        return 1;
    } else if (gUnk_03000040.newKeys & 2) {
        u8 zz = 0;
        *st = zz;
        return zz;
    } else if (sub_08052F38(0xE000E0) != 0) {
        u8 *base = (u8 *)&gUnk_0201CFB0;
        u32 *pa = (u32 *)(base + 0x824);
        int p;
        int z;

        z = *(u32 *)(base + 0x828) + *(u32 *)(base + 0x82C);
        /* Keep the cursor sum in r4 before loading the player cursor into r5. */
        __asm__("" : "+r"(z));
        p = *pa;
        if (sub_0802BAD0(ref, (u8)*(u32 *)(base + 0x82C) << 8 | *(u8 *)pa) != 0) {
            if (sub_0803DDAC(ref, p, z) != 0)
                return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}

/* AI: when sub_0802CE38 accepts, pick a target via sub_0805748C / sub_08057550, else the first occupied monster zone; player 0 gets a per-card prompt. */
int sub_0803EBB0(struct CardRef *ref, int arg)
{
    u8 *es;
    u8 *st;
    int pl = 1 & ((u8 *)ref)[2];
    if (pl) {
        int i;
        ref->numTargets = 0;
        if (sub_0802CE38(ref, arg, 0) == 0)
            return 1;
        for (i = 0; i <= 1; i++) {
            if (sub_08008860(i) > 0) {
                int r;
                if (i != 0)
                    r = sub_08057550(i, -1, 1, 1);
                else
                    r = sub_0805748C(0, -1, 1, 1);
                if (r >= 0) {
                    if (sub_0803DDAC(ref, i, r) != 0)
                        return 1;
                }
            }
        }
        for (i = 0; i <= 1; i++) {
            int j;
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(1 & i, j);
                if ((*(u32 *)z << 20) != 0) {
                    if (sub_0803DDAC(ref, i, j) != 0)
                        return 1;
                }
            }
        }
        return 1;
    }
    es = gUnk_02017A40;
    st = es + 0x3E5;
    if (*st == 0) {
        ref->numTargets = 0;
        if (sub_0802CE38(ref, arg, 0) == 0 && ((const u16 *)0x08622AB4)[0x7FF & ref->id] != 0x3FF)
            return 1;
        switch (((const u16 *)0x08622AB4)[0x7FF & ref->id]) {
        case 0x1F4:
        case 0x3FF:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083FD0);
            break;
        case 0x21C:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084000);
            break;
        default:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08084044);
            break;
        }
        { u8 *e = gUnk_02017A40; e[0x3E5]++; }
    } else if (gUnk_03000040.newKeys & 2) {
        *st = pl;
    } else if (sub_08052F38(0xF000F0) != 0) {
        int p = gUnk_0201CFB0.w824;
        int z = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        if (sub_0802B1B8(ref->id, p, z) != 0) {
            if (sub_0803DDAC(ref, p, z) != 0)
                return 1;
        }
        sub_08077AEC(3);
    }
    return 0;
}

