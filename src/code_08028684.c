#include "global.h"
#include "gba.h"

struct Timer {
    u8 state;
    u16 timer;
};

/* Sprite group (0x14 bytes, sub_08078670 / sub_080786D0 / sub_08077EF4). */
struct SpriteGroup {
    u32 unk0;
    const u16 *templates;   /* +0x4 */
    u16 x;                  /* +0x8 */
    u16 y;                  /* +0xA */
    u8 count;               /* +0xC */
    u8 unkD;
    u8 unkE;                /* +0xE */
    u8 unkF;
    u8 unk10;
    u8 unk11[3];
};

/* Work area at 0x02020310 as used by this screen (0xB24 bytes). */
struct Work20310 {
    u8 oam[0x618];          /* +0x000: OAM buffer (sub_0807A298 / sub_0807A2EC) */
    struct {
        u16 scaleX;         /* +0x0 (0x100 = 1.0) */
        u16 scaleY;         /* +0x2 */
        u16 angle;          /* +0x4 */
        u8 unk6[0x12];
    } aff[0x20];            /* +0x618: OBJ affine sets (sub_0807B534) */
    struct SpriteGroup grp[5];  /* +0x918 */
    u8 filler97C[0xAAC - 0x97C];
    struct {
        u8 unk0;            /* +0 */
        s8 offset;          /* +1: slide offset (-4 / +4 when moving) */
        u8 hand;            /* +2: 0-2 */
        u8 unk3;
    } hands[4];             /* +0xAAC */
    u8 unkABC;              /* +0xABC: opponent hand */
    u8 unkABD;              /* +0xABD */
    u8 unkABE;              /* +0xABE: result (sub_08028930) */
    u8 unkABF;              /* +0xABF: cursor */
    u16 unkAC0;             /* +0xAC0 */
    u8 fillerAC2[2];
    struct {
        u16 v;
        u16 unk2;
    } unkAC4[2];            /* +0xAC4 */
    u16 unkACC;             /* +0xACC */
    s8 unkACE;              /* +0xACE */
    u8 unkACF;              /* +0xACF */
    u16 unkAD0;             /* +0xAD0 */
    u16 unkAD2;             /* +0xAD2 */
    u8 unkAD4;
    u8 unkAD5;
    u8 fillerAD6[0xADC - 0xAD6];
    s16 unkADC[2];          /* +0xADC: object for sub_0807BAB4 / sub_0807B9D4 */
    u8 fillerAE0[0xAF0 - 0xAE0];
    u8 unkAF0;              /* +0xAF0: state of the +0xADC object */
    u8 fillerAF1[3];
    u8 unkAF4;              /* +0xAF4 */
    u8 unkAF5;              /* +0xAF5: step */
    u8 fillerAF6[2];
    u8 fade[8];             /* +0xAF8: object for sub_080787F4 */
    struct Timer timer;     /* +0xB00 */
    u8 fillerB04[0xB0D - 0xB04];
    u8 unkB0D;              /* +0xB0D */
    u8 unkB0E;              /* +0xB0E */
    u8 fillerB0F;
    u8 unkB10[6];           /* +0xB10: link exchange (sub_0807BCF4 / sub_0807BCFC) */
    u16 unkB16;             /* +0xB16: received value */
    u8 fillerB18[4];
    u8 unkB1C;
    u8 unkB1D;
    u8 unkB1E;
    u8 fillerB1F;
    u16 unkB20;             /* +0xB20 */
    u8 fillerB22[2];
};
extern struct Work20310 gUnk_02020310;
#define gWork gUnk_02020310

struct Main {
    u32 rngState;
    u16 heldKeys;           /* +0x4 */
    u16 newKeys;            /* +0x6 */
    u8 filler8[0x40E - 0x8];
    u16 unk40E;             /* +0x40E */
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040

void sub_08075294(void *dest, const void *src, u32 size);
void sub_08075278(void *dst, u32 size);
u32 *sub_0807B6B8(u32 a, u32 tile, s32 x, s32 y, u32 w, u32 h, u32 a6, u32 a7, u32 a8,
                  u32 a9, u32 a10, u32 a11, void *work);
s32 sub_0807B4D0(s32 a, s32 b);     /* 8.8 fixed-point multiply */
void sub_0807B0C0(struct Timer *t);
void sub_0807B0C8(struct Timer *t, u16 time);
void sub_0807B0D0(struct Timer *t);
void sub_0807B4A8(u32 a);           /* BLDALPHA */
void sub_0807B4C0(u32 a);           /* BLDY */
void sub_08077AEC(u16 se);          /* PlaySE */

extern const u16 gUnk_0808270C[];
extern const u8 gUnk_08082710[];

#define CpuFastFill(value, dest, size)                                  \
{                                                                       \
    vu32 tmp = (vu32)(value);                                           \
    CpuFastSet((void *)&tmp, dest, 0x01000000 | (((size) / 4) & 0x1FFFFF)); \
}

void sub_08078670(const void *anim, struct SpriteGroup *grp);
void sub_0807A2EC(void *p);
void sub_0807B534(void *p);
void sub_0807BAB4(void *obj);
void sub_0807B9D4(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, void *obj, u32 g);
void sub_080283BC(u8 a, u8 b, u16 c, void *d, u8 e, void *obj);
void sub_08028AB8(const u8 *src, u32 tile, u32 width, s32 rows);
void sub_080287A4(u8 a, u8 b, u8 c);
extern const u8 gUnk_0819A780[];
extern const u8 gUnk_086AC828[], gUnk_086AD028[], gUnk_086AD828[];
extern const u16 gUnk_080826E6[];
extern const s16 gUnk_08087BA4[];   /* sine table (0x100 = 1.0) */
#define SIN(i) gUnk_08087BA4[i]
extern const u16 gUnk_08082706[];
extern const u16 gUnk_08082712[];
void sub_08077BCC(void);
void sub_08028684(u32 unused0, u32 unused1, u16 flags, void *unused3, u8 which, s16 *pos);
void sub_0807BCF4(void *p);
u32 sub_0807BCFC(u32 a, u8 b, void *p);
u16 sub_08029458(u8 *step);
void sub_080787F4(u32 a, s32 b, u32 c, void *p);
extern const u8 gUnk_086A12EC[], gUnk_086AA8EC[], gUnk_086AAAEC[], gUnk_086AAB00[], gUnk_086AAB20[];
extern const u8 gUnk_086AAB40[], gUnk_086AAB60[], gUnk_086AABA0[], gUnk_086AABBC[], gUnk_086AABDC[];
extern const u8 gUnk_086AAC00[], gUnk_086AAB80[], gUnk_086AAC20[];
extern const u8 gUnk_086AAC28[], gUnk_086AB028[], gUnk_086AB428[], gUnk_086AB828[], gUnk_086AC028[];
extern const u8 gUnk_086AE028[], gUnk_086AE828[], gUnk_086AF828[], gUnk_086AF028[];
s32 sub_08076F9C(void);             /* Random */
void sub_08027CA4(void);
void sub_08027C90(void);
void sub_080280D0(u32 a, u16 b);
u8 sub_08028930(u8 a, u8 b);

/* Draw the player's hand sprite (which = 0/1), bobbing with pos[0]; flags bit 3 picks the palette. */
void sub_08028684(u32 unused0, u32 unused1, u16 flags, void *unused3, u8 which, s16 *pos)
{
    u32 w = 0x40;
    u32 h = 0x20;
    u32 *o;

    switch (which) {
    case 0:
        o = sub_0807B6B8(0, gUnk_080826E6[0], 0x58, pos[1] - (sub_0807B4D0(pos[0], 0x2000) >> 8) + 0x50,
                         w, h, 4, 3, 0x200, 0, 0, 0, &gWork);
        *o |= (flags & 8) ? 0x08000500 : 0x08000100;
        break;
    case 1:
        o = sub_0807B6B8(0, gUnk_080826E6[1], 0x58, pos[1] - (sub_0807B4D0(pos[0], 0x2000) >> 8) + 0x50,
                         w, h, 4, 3, 0x200, 0, 0, 0, &gWork);
        *o |= (flags & 8) ? 0x08000500 : 0x08000100;
        break;
    }
    gWork.aff[4].angle = 0;
    gWork.aff[4].scaleX = 0x100;
    gWork.aff[4].scaleY = pos[0];
}

/* Draw the three opponent hand sprites swinging by `angle`; `k` picks the spread (hypothesis). */
void sub_080287A4(u8 unused, u8 angle, u8 k)
{
    u8 i;
    s32 c, s, a, a2, b, x, y, four = 4;
    u32 *o;

    for (i = 0; i < 3; i++) {
        c = SIN(angle + 0x40);
        a = sub_0807B4D0(c, 0x4000);
        /* `four` stops fold-const from reassociating (A*i - 4) - B into A*i - (B + 4) */
        four = 4;
        x = (s16)(a >> 8) * i - four - (sub_0807B4D0(0x60, c - SIN(0x134)) >> 8);
        s = SIN(angle);
        a2 = sub_0807B4D0(s, 0x4000);
        b = sub_0807B4D0(0x60, s - SIN(0xF4));
        y = (((a2 + 0xA) >> 8) * i - (b >> 8) + (sub_0807B4D0(0x4E0, gUnk_08082712[k]) >> 4) - 0x5E) & 0xFFFF;
        o = sub_0807B6B8(0, gUnk_08082706[i], x, y, 0x40, 0x40, 4, 0xA, 0x200, 0, 0, 0, &gWork);
        *o |= 0x300;
        gWork.aff[0].angle = angle << 8;
        gWork.aff[0].scaleX = 0x100;
        gWork.aff[0].scaleY = 0x100;
    }
}

void sub_080288DC(u8 k)
{
    u32 w = 0x40;
    u32 h = 0x20;
    sub_0807B6B8(0, gUnk_0808270C[k], 0x58, 0x64, w, h, 4, gUnk_08082710[k], 0x200, 0, 0, 0, &gWork);
}

u8 sub_08028930(u8 a, u8 b)
{
    switch (b) {
    case 0:
        switch (a) {
        case 0: return 2;
        case 1: return 1;
        case 2: return 0;
        }
        break;
    case 1:
        switch (a) {
        case 0: return 0;
        case 1: return 2;
        case 2: return 1;
        }
        break;
    case 2:
        switch (a) {
        case 0: return 1;
        case 1: return 0;
        case 2:
        {
            u8 result = 2;

            /* FAKEMATCH: preserve this initialized result as a separate
             * return block, as in the ROM's final draw case. */
            __asm__("" : "+r"(result));
            return result;
        }
        }
        break;
    }
    return a;
}

struct Pair { s16 v; s16 unk2; };

/* Raise entry `i` by 0x20 (max 0x800), lower the other one by 0x40 (min 0). */
void sub_0802899C(u8 i, struct Pair *p)
{
    u8 j = i;
    if ((p[j].v += 0x20) > 0x800)
        p[j].v = 0x800;
    j ^= 1;
    if ((p[j].v -= 0x40) < 0)
        p[j].v = 0;
}

u8 sub_080289DC(u8 x)
{
    return x & 1;
}

/* Clear the work area and reset the display (step 0). */
u16 sub_080289E8(void)
{
    sub_08075278(&gWork, sizeof(gWork));
    gMain.unk40E = 1;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= 0xE0FF;
    sub_0807A2EC(&gWork);
    sub_0807B4A8(8);
    gWork.unkAF4 = 0;
    gWork.unkAF5 = 0;
    gWork.unkABF = 0xFF;
    gWork.unkB0D = 0;
    gWork.unkB1C = 0;
    gWork.unkB1D = 0;
    sub_08078670(gUnk_0819A780, &gWork.grp[0]);
    gWork.grp[0].unkE = 0;
    sub_0807B534(gWork.aff);
    gWork.unkB20 = 0;
    return 1;
}

/* Copy `rows` rows of `width` tiles into OBJ VRAM at `tile` (row stride 32 tiles). */
void sub_08028AB8(const u8 *src, u32 tile, u32 width, s32 rows)
{
    u8 *dst = (u8 *)0x06014000 + tile * 32;
    s32 i;
    for (i = 0; i < rows; i++) {
        sub_08075294(dst, src, width * 32);
        dst += 0x400;
        src += width * 32;
    }
}

/* Load graphics and palettes and reset the hands (step 1). */
u16 sub_08028AEC(void)
{
    u8 i;

    sub_080787F4(0, -0x180, 0, gWork.fade);
    sub_08075294((void *)VRAM, gUnk_086A12EC, 0x9600);
    sub_08075294((void *)PLTT, gUnk_086AA8EC, 0x200);
    sub_08075294((void *)(PLTT + 0x200), gUnk_086AAAEC, 0x20);
    sub_08075294((void *)(PLTT + 0x220), gUnk_086AAB00, 0x20);
    sub_08075294((void *)(PLTT + 0x240), gUnk_086AAB20, 0x20);
    sub_08075294((void *)(PLTT + 0x260), gUnk_086AAB40, 0x20);
    sub_08075294((void *)(PLTT + 0x280), gUnk_086AAB60, 0x20);
    sub_08075294((void *)(PLTT + 0x2A0), gUnk_086AABA0, 0x20);
    sub_08075294((void *)(PLTT + 0x2C0), gUnk_086AABBC, 0x20);
    sub_08075294((void *)(PLTT + 0x2E0), gUnk_086AABDC, 0x20);
    sub_08075294((void *)(PLTT + 0x320), gUnk_086AAC00, 0x20);
    sub_08075294((void *)(PLTT + 0x340), gUnk_086AAB80, 0x20);
    sub_08075294((void *)(PLTT + 0x3A0), gUnk_086AAC20, 0x20);
    sub_08028AB8(gUnk_086AAC28, 0, 4, 8);
    sub_08028AB8(gUnk_086AB028, 4, 4, 8);
    sub_08028AB8(gUnk_086AB428, 8, 4, 8);
    sub_08028AB8(gUnk_086AB828, 0x100, 0x10, 4);
    sub_08028AB8(gUnk_086AC028, 0x180, 0x10, 4);
    sub_08028AB8(gUnk_086AE028, 0x10, 0x10, 4);
    sub_08028AB8(gUnk_086AE828, 0x90, 0x10, 4);
    sub_08028AB8(gUnk_086AF828, 0x110, 4, 4);
    sub_08028AB8(gUnk_086AF028, 0x190, 0x10, 4);
    for (i = 0; i < 5; i++) {
        gWork.aff[i].scaleX = 0x100;
        gWork.aff[i].scaleY = 0x100;
        gWork.aff[i].angle = 0;
    }
    for (i = 0; i < 4; i++) {
        gWork.hands[i].offset = 0;
        gWork.hands[i].unk0 = 0;
        gWork.hands[i].hand = 0;
    }
    for (i = 0; i < 2; i++)
        gWork.unkAC4[i].v = 0;
    gWork.unkAD4 = 0;
    gWork.unkAD5 = 0;
    gWork.unkABC = 0xFF;
    gWork.unkABD = 0;
    gWork.unkAC0 = 0;
    gWork.unkACE = 0xF4;
    gWork.unkACF = 0;
    gWork.unkAD0 = 0;
    gWork.unkAD2 = 0;
    REG_DISPCNT = 0x1F04;
    return 1;
}

#if 0 /* NONMATCHING: r3 keeps a copy of the work pointer (used by the 0x20 branch) and r5 is a
       * scratch. We get r7 and CSE the unkB0D address. */
/* Choose a hand (Left/Right, A); the CPU (or the link partner) answers. */
u16 sub_08028D9C(void)
{
    struct Work20310 *w = &gWork;

    if (w->unkB0D == 0) {
        if ((gMain.newKeys & 1) && w->hands[0].offset == 0) {
            if (w->unkB0E == 1) {
                w->unkB0D = 1;
                sub_0807BCF4(w->unkB10);
            } else {
                if (sub_08076F9C() % 500 < 100)
                    w->unkABC = w->hands[0].hand;
                else if (sub_08076F9C() & 1)
                    w->unkABC = (w->hands[0].hand + 1) % 3;
                else
                    w->unkABC = (w->hands[0].hand + 2) % 3;
                gWork.unkABD = 1;
                gWork.unkABE = sub_08028930(gWork.hands[0].hand, gWork.unkABC);
                gWork.unkAF5++;
                gWork.unkB1E = 0;
                gWork.hands[1].offset = 4;
            }
            sub_08077AEC(1);
        } else if ((gMain.newKeys & 0x20) && w->hands[0].offset == 0) {
            w->hands[0].offset = -4;
            sub_08077AEC(0);
        } else if ((gMain.newKeys & 0x10) && gWork.hands[0].offset == 0) {
            gWork.hands[0].offset = 4;
            sub_08077AEC(0);
        }
    }
    if (gWork.unkB0D != 0) {
        if (sub_0807BCFC(0x51, w->hands[0].hand, w->unkB10)) {
            gWork.hands[1].offset = 4;
            gWork.unkABD = 1;
            gWork.unkABC = gWork.unkB16;
            gWork.unkABE = sub_08028930(gWork.hands[0].hand, gWork.unkB16);
            gWork.unkAF5++;
            gWork.unkB0D = 0;
            sub_0807BCF4(w->unkB10);
            sub_08027CA4();
            gWork.unkB1E = 0;
        } else {
            sub_08027C90();
        }
    }
    if (gWork.hands[1].offset == 0)
        sub_080280D0(0, gWork.unkAC0);
    return 0;
}
#endif

INCLUDE_ASM("asm/nonmatching/code_08028684", sub_08028D9C); /* 0x08028D9C size 0x234 */

/* Result phase (step 3, hypothesis). Handles the A press and the link partner's answer (0x53), with a saturating unkB1E timer. */
u16 sub_08028FD0(void)
{
    struct Work20310 *w = &gWork;
    u8 *q = &gWork.unkB1C;

    if (w->unkB0D == 0) {
        if (w->hands[1].unk0 == 0x30) {
            if ((gMain.newKeys & 1) && gWork.unkABE == 2) {
                if (gWork.unkB0E == 1) {
                    w->unkB1D = 1;
                } else {
                    gWork.hands[1].offset = -4;
                    gWork.unkABD = 0;
                    gWork.unkAF5--;
                }
                sub_08077AEC(1);
            } else if ((gMain.newKeys & 1) && gWork.unkABE == 0) {
                gWork.unkACC = 0;
                gWork.unkAC0 = 7;
                gWork.unkAF5++;
                REG_BLDALPHA = 0x10;
                REG_BLDY = 8;
                REG_BLDCNT = 0x440;
                sub_0807B4A8(gWork.unkACC);
                gWork.unkABF = 0;
                sub_08077AEC(1);
            } else if (gWork.unkABE == 1) {
                if (gWork.unkB0E == 1) {
                    gWork.unkB0D = 1;
                    sub_0807BCF4(w->unkB10);
                } else if (gMain.newKeys & 1) {
                    gWork.unkABF = sub_080289DC(gWork.unkAF4);
                    gWork.unkAF5 += 2;
                    sub_08077AEC(1);
                }
                sub_0807B9D4(0, 0, 0x40, 0xF, 3, 1, gWork.unkADC, 0);
                sub_08029458(&gWork.unkAF5);
            }
        }
    }
    if (gWork.unkB0D == 1) {
        sub_08027C90();
        if (sub_0807BCFC(0x52, w->hands[0].hand, w->unkB10)) {
            gWork.unkABF = 1 ^ *(u8 *)&gWork.unkB16;
            gWork.unkAF5 += 2;
            gWork.unkB0D = 0;
            sub_08027CA4();
        }
    }
    if (gWork.unkB0E == 1 && gWork.unkABE == 2) {
        if (sub_0807BCFC(0x53, *q, w->unkB10)) {
            if (w->unkB16 == 2 || *q == 2) {
                gWork.hands[1].offset = -4;
                gWork.unkABD = 0;
                gWork.unkAF5--;
                gWork.unkB0D = 0;
                *q = 0;
                w->unkB1D = 0;
            } else {
                sub_0807BCF4(w->unkB10);
                if (w->unkB1D != 0)
                    *q = 2;
            }
        }
    }
    if (gWork.unkABE == 2) {
        if (++gWork.unkB1E == 0) {
            gWork.unkB1E = 0xFF;
            if (sub_0807BCFC(0x53, *q, w->unkB10)) {
                gWork.hands[1].offset = -4;
                gWork.unkABD = 0;
                gWork.unkAF5--;
                gWork.unkB0D = 0;
                *q = 0;
                w->unkB1D = 0;
            }
        }
    }
    return 0;
}

/* Choose with Left/Right, confirm with A. */
u16 sub_080292C8(void)
{
    struct Work20310 *w = &gWork;

    if (w->unkB0D == 0) {
        if ((gMain.newKeys & 0x20) && w->unkABF == 1) {
            w->unkABF = 0;
            sub_08077AEC(0);
        } else if ((gMain.newKeys & 0x10) && gWork.unkABF == 0) {
            gWork.unkABF = 1;
            sub_08077AEC(0);
        }
        if (gMain.newKeys & 1) {
            gWork.unkACC = 0x1000;
            sub_08028AB8(gUnk_086AC828, 0x10, 8, 8);
            sub_08028AB8(gUnk_086AD028, 0x18, 8, 8);
            sub_08028AB8(gUnk_086AD828, 0x110, 8, 8);
            sub_0807B9D4(0, 0, 0x40, 0xF, 3, 1, gWork.unkADC, 0);
            if (gWork.unkB0E == 1) {
                gWork.unkB0D = 1;
                sub_0807BCF4(w->unkB10);
            } else {
                sub_08029458(&gWork.unkAF5);
                gWork.unkAF5++;
            }
            sub_08077AEC(1);
        }
    }
    gWork.unkACC += 0x80;
    if (gWork.unkACC > 0x1000)
        gWork.unkACC = 0x1000;
    sub_0807B4A8(gWork.unkACC >> 8);
    if (gWork.unkB0D != 0 && sub_0807BCFC(0x52, gWork.unkABF, w->unkB10)) {
        gWork.unkAF5++;
        gWork.unkB0D = 0;
        sub_08029458(&gWork.unkAF5);
    }
    return 0;
}

u16 sub_08029458(u8 *step)
{
    sub_0807BAB4(gWork.unkADC);
    sub_080283BC(gWork.unkABF, gWork.unkAF4, gWork.unkAC0, gWork.unkAC4, gWork.unkABF, gWork.unkADC);
    if (gWork.unkAF0 == 2) {
        sub_08028AB8(gUnk_086AC828, 0x10, 8, 8);
        sub_08028AB8(gUnk_086AD028, 0x18, 8, 8);
        sub_08028AB8(gUnk_086AD828, 0x110, 8, 8);
        sub_0807B9D4(0x100, 0, 0x100, 0, 1, 0, gWork.unkADC, 0);
        (*step)++;
    }
    return 0;
}

u16 sub_080294E8(void)
{
    if (gWork.unkACF < 0x10)
        gWork.unkACF++;
    if (gWork.unkACF >= 0x10) {
        if (gWork.unkACE < 0) {
            if ((u8)gWork.unkACE == 0xF4)
                sub_08077AEC(0x2A);
            gWork.unkACE += 3;
        } else {
            gWork.unkACE = 0;
            sub_08077BCC();
        }
    }
    if (gWork.unkB20++ == 90.0 || (gMain.newKeys & 1)) {
        sub_0807B4C0(gWork.unkAD0);
        REG_BLDCNT = 0xBF;
        gWork.unkAD2 = 0x300;
        gWork.unkAF5++;
        sub_0807B0C0(&gWork.timer);
        sub_08077BCC();
    }
    if (gWork.unkACF == 0xF && gWork.unkADC[1] == 0)
        gWork.unkADC[0] = 0xC0;
    if (gWork.unkACE > -10 && gWork.unkADC[1] == 0)
        sub_0807B9D4(0xC0, 0, 0x100, 0x38, 6, 0x14, gWork.unkADC, 0);
    sub_0807BAB4(gWork.unkADC);
    sub_08028684(gWork.unkABF, gWork.unkAF4, gWork.unkAC0, gWork.unkAC4, gWork.unkABF, gWork.unkADC);
    sub_080287A4(gWork.unkABF, gWork.unkACE, gWork.unkACF);
    return 0;
}

u16 sub_0802965C(void)
{
    gWork.unkAD0 += gWork.unkAD2;
    if (gWork.timer.state == 2) {
        gWork.timer.state = 0;
        gWork.unkAD0 = 0;
        gWork.unkAD2 = 0x60;
        gWork.unkAF5++;
        sub_0807B4C0(0);
        REG_BLDCNT = 0xFF;
        return 0;
    }
    if (gWork.timer.state != 1 && gWork.unkAD0 == 0xC00)
        gWork.unkAD2 = 0x100;
    if (gWork.timer.state != 1 && gWork.unkAD0 > 0x10FF) {
        CpuFastFill(-1, (void *)PLTT, 0x200);
        CpuFastFill(-1, (void *)(PLTT + 0x200), 0x200);
        sub_0807B0C8(&gWork.timer, 0x14);
    }
    sub_0807B4C0(gWork.unkAD0 >> 8);
    sub_080287A4(gWork.unkABF, gWork.unkACE, gWork.unkACF);
    sub_0807B0D0(&gWork.timer);
    return 0;
}
