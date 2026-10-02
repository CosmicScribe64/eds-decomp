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
    s8 unkE;                /* +0xE */
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
    u8 filler410[0xC9C - 0x410];
    u16 mapA[8];            /* +0xC9C: BG tile map buffer pieces (hypothesis) */
    u16 mapB[21];           /* +0xCAC */
    u16 mapS0;              /* +0xCD6 */
    u16 padCD8[2];
    u16 mapS1;              /* +0xCDC */
    u8 padCDE[0xD16 - 0xCDE];
    u16 mapS2;              /* +0xD16 */
    u16 padD18[2];
    u16 mapS3;              /* +0xD1C */
    u16 mapD[28];           /* +0xD1E */
    u16 mapS4;              /* +0xD56 */
    u8 fillerD58[0x4859 - 0xD58];
    u8 seqIndex1;           /* +0x4859: step index of the runner */
    u8 filler485A[0x4870 - 0x485A];
    u8 unk4870b0 : 1;       /* +0x4870 bit 0 = last pick (hypothesis) */
    u8 unk4870b1 : 5;       /* bits 1-5: opponent index (see code_0801bcfc) */
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


u16 sub_0807EE98();
u16 sub_0807EE9C();
void sub_080754F8(u32 arg);
void sub_08074C80(u32 ch, s32 x, s32 y, u32 attr);
void sub_08074D48(u32 ch, s32 x, s32 y, u32 attr);
extern struct { u8 unk0[4]; u8 flags4; } gUnk_02011C20;
void sub_08074B08(u32 a, u32 b);
struct Entry { u32 id : 12; u32 flag : 1; u32 rest : 19; };
struct Sel {                /* 0x0201D810: card list selection (hypothesis) */
    u8 flags;               /* +0: bits 5-7 mode, bit 1 */
    u8 filler1[4];
    u8 sel : 2;             /* +5 */
    u8 unk5b : 6;
    u16 scroll;             /* +6 */
    u8 filler8[4];
    u32 list[0x80];         /* +0xC: entries: bits 0-11 card id, bit 12 flag (struct Entry) */
    u16 kind[0x80];         /* +0x20C: per-entry state (1, 2, 4) (hypothesis) */
    u16 count;              /* +0x30C */
};
extern struct Sel gUnk_0201D810;
extern u8 gUnk_02019FA8[];
extern u8 gUnk_020192E4[];
struct Name { u16 s[0x20]; };
extern const struct Name gUnk_0822C720[];
extern const u16 gUnk_0808275C[];
void sub_08029FC8(s32 x, s32 y, const u16 *str, s32 w);
struct SelFlags { u8 b0 : 1; u8 b1 : 1; u8 b2 : 1; u8 b3 : 1; u8 rest : 4; u8 pad[7]; };
#define SELF ((struct SelFlags *)&gUnk_0201D810)
void sub_080761F0(u32 x, u32 y, u16 tile);
void sub_0807883C(void *obj);
void sub_08028178(u8 a, u8 b, u16 c);
void sub_08027D34(const void *a, const void *b, u8 c, u8 d, u8 e, u16 f, u8 g);
void sub_080280D0(u32 a, u16 b);
void sub_0802899C(u8 i, void *p);
void sub_08028238(u8 a, u8 b, u16 c, void *d, u8 e);
void sub_0807B5A0(void *p);
void sub_080786D0(void *grp);
void sub_08078534(void *grp, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, void *work);
void sub_0807A298(void *p);
void sub_08027CB8(void *p);
void sub_08027CDC(void *p);
void sub_08027D1C(void *p);
extern const u32 gUnk_0819A6B0[];
extern const u32 gUnk_0819A6D0[];
extern const u8 gUnk_080826E0[];
u32 sub_08007834(u32 id);
extern const u32 gUnk_08621DE0[];
#define MAPP(off) ((u16 *)((u8 *)&gMain + (off)))
extern u32 gUnk_0819A718[];
extern u32 gUnk_0819A72C[];
extern u32 gUnk_0819A73C[];

/* Fade in with BLDY; when done, latch the pick into gMain.unk4870 bit 0. */
u16 sub_08029750(void)
{
    struct Work20310 *w = &gWork;

    w->unkAD0 += w->unkAD2;
    if (w->unkAD0 > 0x1000) {
        gMain.unk4870b0 = w->unkABF;
        return 1;
    }
    sub_0807B4C0(w->unkAD0 >> 8);
    return 0;
}

/* Main play step: run the fade object, advance the sequence and update all sprites. */
u16 sub_080297B4(void)
{
    u8 *f = gWork.fade;
    u8 i;
    s32 e;
    u32 v;

    sub_0807883C(f);
    if (f[6] == 2)
        gMain.seqIndex1 += f[7];
    if (f[6] == 3) {
        REG_BLDCNT = 0x440;
        f[6] = 0;
        gWork.unkAD5 = 6;
    }
    if (f[6] == 0 && gWork.unkAD5 == 0) {
        v = gUnk_0819A6B0[gWork.unkAF5];
        if ((u8)sub_0807EE9C(&gWork.unkAF5, v))
            return 1;
    }
    if (gWork.unkAD5 != 0) {
        gWork.unkAD4 += gWork.unkAD5;
        if (gWork.unkAD4 > 0x54) {
            gWork.unkAD4 = 0x55;
            gWork.unkAD5 = 0;
        }
    }
    if (gWork.unkAF5 <= 2) {
        sub_08028178(gWork.unkABC, gWork.hands[1].unk0, gWork.unkAC0);
        sub_08027D34(gUnk_080826E0, (const u8 *)0x08082703, gWork.hands[0].unk0, gWork.hands[0].hand,
                     gWork.hands[1].unk0, gWork.unkAC0, gWork.unkAD4);
        if (gWork.hands[1].unk0 == 0x30 && gWork.unkABD == 1 && gWork.unkABC != 0xFF) {
            if (gWork.unkABE == 0)
                sub_080280D0(1, gWork.unkAC0);
            else if (gWork.unkABE == 1)
                sub_080280D0(2, gWork.unkAC0);
            else if (gWork.unkABE == 2)
                sub_080280D0(4, gWork.unkAC0);
        }
        if (gWork.unkABF != 0xFF) {
            sub_0802899C(gWork.unkABF, gWork.unkAC4);
            sub_08028238(gWork.unkABF, gWork.unkAF4, gWork.unkAC0, gWork.unkAC4, gWork.unkABF);
        }
    }
    for (i = 0; i <= 4; i++)
        sub_0807B5A0(&gWork.aff[i]);
    e = gWork.grp[0].unkE;
    if (e == 1) {
        sub_080786D0(&gWork.grp[0]);
        sub_08078534(&gWork.grp[0], 0, 0, 1, e, 0, 0, 0, 0, &gWork);
    }
    sub_0807A298(&gWork);
    sub_0807A2EC(&gWork);
    sub_08027CB8(gWork.hands);
    sub_08027CDC(gWork.hands);
    sub_08027D1C(&gWork.hands[1]);
    gWork.unkAF4++;
    return 0;
}
/* Draws the hand selection sprites and advances the step. */
u16 sub_080299F8(void)
{
    sub_080283BC(gWork.unkABF, gWork.unkAF4, gWork.unkAC0, gWork.unkAC4, gWork.unkABF, gWork.unkADC);
    gWork.unkAF5++;
    return 0;
}
/* Step 0 clears the work area (DMA fill) and resets the display. */
u16 sub_08029A50(void)
{
    struct Work20310 *w;

    {
        vu16 zero = 0;
        struct Work20310 *loaded;
        u32 status;
        u32 mask;
        register vu32 *dma __asm__("r1") = (vu32 *)0x040000D4;

        dma[0] = (u32)&zero;
        loaded = &gWork;
        dma[1] = (u32)loaded;
        dma[2] = 0x81000592;
        dma[2];
        status = dma[2];
        mask = 0x80000000;
        /* FAKEMATCH: retain the initialized DMA values through the first
         * poll before copying the work pointer into its saved register. */
        __asm__("" : : "r"(loaded), "r"(status), "r"(mask));
        w = loaded;
        if ((s32)status < 0) {
            do {} while (dma[2] & 0x80000000);
        }
    }
    gMain.unk40E = 1;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= 0xE0FF;
    sub_0807A2EC(w);
    sub_0807B4A8(8);
    w->unkAF4 = 0;
    w->unkAF5 = 2;
    w->unkABF = 0;
    sub_0807B534(w->aff);
    return 1;
}
/* Choose Left/Right (step 2), then animate the fade object and the hands. */
u16 sub_08029B0C(void)
{
    struct Work20310 *v = &gWork;
    struct Work20310 *w;    /* second pointer: set after the affine loop (one pointer for both halves does not match) */
    u8 *f;
    u8 i;

    if (v->unkAF5 == 2) {
        if (gMain.newKeys & 0x20)
            v->unkABF = 0;
        else if (gMain.newKeys & 0x10)
            v->unkABF = 1;
        if (gMain.newKeys & 1) {
            sub_0807B9D4(0, 0, 0x40, 0xF, 3, 1, gWork.unkADC, 0);
            if (gWork.unkB0E == 1) {
                gWork.unkB0D = 1;
                sub_0807BCF4(v->unkB10);
            } else {
                gWork.unkAF5++;
            }
        }
        if (gWork.unkB0D != 0) {
            if (sub_0807BCFC(0x51, gWork.unkABF, v->unkB10)) {
                gWork.unkAF5++;
                gWork.unkB0D = 0;
            }
        }
    }
    f = gWork.fade;
    sub_0807883C(f);
    if (f[6] == 2)
        gMain.seqIndex1 += f[7];
    for (i = 0; i <= 3; i++) {
        gWork.aff[i].angle = 0;
        gWork.aff[i].scaleX = 0x80;
        gWork.aff[i].scaleY = 0x80;
    }
    w = &gWork;
    if ((u8)sub_0807EE9C(&w->unkAF5, gUnk_0819A6D0[w->unkAF5]))
        return 1;
    if (w->unkAF5 <= 3) {
        if (w->unkABF != 0xFF) {
            sub_0802899C(w->unkABF, w->unkAC4);
            sub_08028238(w->unkABF, w->unkAF4, w->unkAC0, w->unkAC4, w->unkABF);
        }
    }
    for (i = 0; i <= 4; i++)
        sub_0807B5A0(&gWork.aff[i]);
    sub_0807A298(&gWork);
    sub_0807A2EC(&gWork);
    gWork.unkAF4++;
    return 0;
}
u16 sub_08029D0C(void)
{
    return 1;
}
/* Load the three hand graphics, start the hand object, randomly pick the first hand. */
u16 sub_08029D10(void)
{
    sub_08028AB8(gUnk_086AC828, 0x10, 8, 8);
    sub_08028AB8(gUnk_086AD028, 0x18, 8, 8);
    sub_08028AB8(gUnk_086AD828, 0x110, 8, 8);
    sub_0807B9D4(0, 0, 0x40, 0xF, 3, 1, gWork.unkADC, 0);
    gWork.unkAF5 = 3;
    gWork.unkABF = sub_08076F9C() & 1;
    return 1;
}
u16 sub_08029D7C(void)
{
    gWork.unkB0E = 0;
    if (gUnk_0819A718[gMain.seqIndex1] != 0) {
        if (sub_0807EE98())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
u16 sub_08029DCC(void)
{
    u32 v;

    gWork.unkB0E = 1;
    v = gUnk_0819A718[gMain.seqIndex1];
    /* FAKEMATCH: keep the callback address in r1 without emitting code. */
    __asm__("" : : : "r0");
    if (v != 0) {
        if (gMain.newKeys & 2) {
            sub_080754F8(0x08003AA5);
            return 0;
        }
        if (((u16 (*)(void))v)())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
u16 sub_08029E34(void)
{
    if (gUnk_0819A72C[gMain.seqIndex1] != 0) {
        if (sub_0807EE98())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
u16 sub_08029E74(void)
{
    gWork.unkB0E = 1;
    if (gUnk_0819A72C[gMain.seqIndex1] != 0) {
        if (sub_0807EE98())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
u16 sub_08029EC4(void)
{
    if (gUnk_0819A73C[gMain.seqIndex1] != 0) {
        if (sub_0807EE98())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
/* Fill the BG tile map buffers at gMain+0xC9C.. with a running tile counter (mode 1 starts 15 tiles earlier). */
void sub_08029F04(u32 mode)
{
    u16 t = 0x169;
    u16 *p;
    register u8 *base __asm__("r0");
    s32 i;

    if (mode == 1)
        t -= 15;
    t += 0x4000;
    for (i = 0; i < 8; i++)
        gMain.mapA[i] = t++;
    {
        base = (u8 *)&gMain;
        /* FAKEMATCH: keep the initialized second-loop base separate from
         * the saved global base, preventing its earlier pointer hoist. */
        __asm__("" : : "r"(base));
        for (p = (u16 *)(base + 0xCAC), i = 0; i < 21; i++)
            *p++ = t;
    }
    t++;
    gMain.mapS0 = t++;
    gMain.mapS1 = t++;
    gMain.mapS2 = t++;
    gMain.mapS3 = t++;
    for (p = gMain.mapD, i = 0; i < 28; i++)
        *p++ = t;
    gMain.mapS4 = t + 1;
}
/* Draw a string at (x, y) with a drop shadow; w = advance and palette. Two-byte (byte-swapped) chars when save flag 0x80 is set. */
void sub_08029FC8(s32 x, s32 y, const u16 *str, s32 w)
{
    if (gUnk_02011C20.flags4 & 0x80) {
        while (*(u8 *)str != 0) {
            u16 ch = *str;
            ch = (ch >> 8) | ((u8)ch << 8);
            sub_08074C80(ch, x + 1, y + 1, (u16)(((u8)w << 8) | 9));
            sub_08074C80(ch, x, y, (u16)(((u8)w << 8) | 7));
            x += w;
            str++;
        }
    } else {
        const u8 *q = (const u8 *)str;
        while (*q != 0) {
            sub_08074D48(*q, x + 1, y + 1, (u16)(((u8)w << 8) | 9));
            sub_08074D48(*q, x, y, (u16)(((u8)w << 8) | 7));
            x += w / 2;
            q++;
        }
    }
}
void sub_0802A09C(struct Entry *list, s32 count)
{
    s32 i;
    struct Entry e;
    u32 raw;
    u32 bits;
    u8 *tab;
    u8 *base;
    int bound;
    u8 *selection;
    const struct Name *names;
    sub_08074B08(0x20, 9);
    for (i = 0; i < 4 && i < count; i++) {
        /* Loop-invariant table pointer: loop.c hoists it, global alloc leaves it
           without a register, and reload rematerializes it at the use. */
        names = gUnk_0822C720;
        raw = *(u32 *)list;
        bits = raw << 20;
        e = *(struct Entry *)&raw;
        if (bits != 0) {
            s32 ok;
            ok = 1;
            if ((gUnk_0201D810.flags & 0xE0) == 0x60) {
                u8 *t;
                tab = gUnk_02019FA8;
                t = &tab[(gUnk_0201D810.scroll + i) * 2 + 0xD64 * (((struct SelFlags *)&gUnk_0201D810)->b1 & 1)];
                if (*t == 2 && (*t & gUnk_0201D810.flags))
                    ok = 0;
            }
            if (ok) {
                const u16 *name = (const u16 *)((e.id << 6) + (u32)names);
                list++;
                sub_08029FC8(8, i * 16 + 7, name, 10);
            } else {
                sub_08029FC8(8, i * 16 + 7, gUnk_0808275C, 10);
                list++;
                /* FAKEMATCH: an empty insn in the else path lengthens the loop so the
                   giv i*16+7 loses priority to e (r7 vs r8); names then gets no register
                   and its reload in r0 rotates the 16 reload into r1. */
                asm volatile("");
            }
        }
    }
    i = 0;
    bound = 0x11F;
    selection = (u8 *)&gUnk_0201D810;
    base = (u8 *)&gUnk_03000040;
    /* FAKEMATCH: keeps gMain + 0x49C unfused and orders the preheader as the ROM. */
    asm("" : "+r"(base) : "r"(i), "r"(bound), "r"(selection));
    for (; i <= bound; i++)
        *(u16 *)(base + 0x49C + i * 2) = i + 16;
    *selection |= 4;
}
extern const u32 gUnk_081989A8[];
extern const u32 gUnk_081989D0[];
extern const u32 gUnk_081989EC[];
extern u16 gUnk_0300045C[];
void sub_08073500(u16 row, u16 col, u16 w, u16 h);
void sub_08072D28(u32 a, u32 b, u32 c, u32 d, u32 e);
void sub_0807326C(u16 a, u16 b, u16 c, const void *img);
void sub_08072C0C(u32 a, u32 b, s32 val, u32 zero);

#define CARD_STATS(id) (gUnk_08621DE0[(id) & 0x7FF])
#define CARD_TYPE2(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* Card table entry by integer address: each use reloads the table base literal. */
#define A188_STATS_C(id) (((u32 *)0x08621DE0)[(id) & 0x7FF])

/* Spell subtype (stats bits 17-19) for Magic cards, else 0 (cf. code_080619E8). */
static inline int A188_SpellSub(u32 stats)
{
    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
        return (stats & 0xE0000) >> 17;
    default:
        return 0;
    }
}

static inline int A188_Level(u32 id)
{
    switch ((int)((A188_STATS_C(id) & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (A188_STATS_C(id) & 0x1E000000) >> 25;
    }
}

static inline u16 A188_Def10(u32 id)
{
    switch ((int)((A188_STATS_C(id) & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    default:
        return (A188_STATS_C(id) & 0x1FF) * 10;
    }
}

static inline u16 A188_Atk10(u32 *p, u32 id)
{
    switch ((int)((*p & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    default:
        return ((A188_STATS_C(id) << 14) >> 23) * 10;
    }
}


struct A188Flags { u16 b0:1; u16 b1:1; u16 b2:1; u16 b3:1; u16 mode:3; u8 pad[10]; };
#define A188F ((struct A188Flags *)&gUnk_0201D810)

/* Card detail page: draw the frame/type icons and ATK/DEF/level of the selected entry. */
void sub_0802A188(u32 *entry)
{
    u32 *stats;
    u32 id;
    u32 b;
    int row;
    int i;
    int kind;

    id = ((struct Entry *)entry)->id;
    b = SELF->b1;
    row = gUnk_0201D810.scroll + gUnk_0201D810.sel;

    sub_08073500(0, 0x1A1, 0x13, 6);
    sub_08073500(3, 0x155, 0xA, 0xA);
    if ((gUnk_0201D810.flags & 0xE0) == 0x60) {
        u8 *players = gUnk_020192E4;
        int r = row * 2;
        r += 0xD64 * b;
        players += 0xCC4;
        if (players[r] == 2 && b == 1)
            return;
    }

    A188F->b3 = 1 - A188F->b3;
    sub_08072D28(3, 0x155, id, A188F->b3 * 0xB4 + 0x178, (A188F->b3 * 4 + 8) * 16);

    if (id == 0)
        return;

    stats = &A188_STATS_C(id);
    kind = (*stats & 0x1F00000) >> 20;
    switch (kind) {
    case 0x15:
    case 0x16:
        sub_0807326C(0x1A1, 0x50, 0x130,
                     (const void *)(kind == 0x16 ? 0x08636DA0 : 0x08636CD8));
        if (A188_SpellSub(*stats))
            sub_0807326C(0x1A3, 0x60, 0x134,
                         (const void *)gUnk_081989D0[A188_SpellSub(A188_STATS_C(id))]);
        break;
    default: {
        u32 *p;
        sub_0807326C(0x1A1, 0x50, 0x130, (const void *)gUnk_081989A8[*(p = &A188_STATS_C(id)) >> 29]);
        sub_0807326C(0x1A3, 0x60, 0x134, (const void *)gUnk_081989EC[(*p & 0x1F00000) >> 20]);
        sub_08072C0C(0x701A7, 0x4013A, A188_Atk10(p, id), 0);
        sub_08072C0C(0x701C7, 0x4013E, A188_Def10(id), 0);
        for (i = 0; i < A188_Level(id); i++)
            { int x = i & 7; int y = (i >> 3) + 0xD; x += 0xC; gUnk_0300045C[x + ((u16)y << 5)] = 2; }
        break;
    }
    }
}
void sub_0802A188(u32 *entry);
void sub_0802A45C(void)
{
    sub_0802A188(&gUnk_0201D810.list[gUnk_0201D810.sel + gUnk_0201D810.scroll]);
}
void sub_0802A09C(struct Entry *list, s32 count);
void sub_0802A47C(void)
{
    sub_0802A09C((struct Entry *)&gUnk_0201D810.list[gUnk_0201D810.scroll], gUnk_0201D810.count - gUnk_0201D810.scroll);
}
void sub_08029F04(u32 mode);
void sub_0802A4A4(void)
{
    sub_08029F04((gUnk_0201D810.list[gUnk_0201D810.sel + gUnk_0201D810.scroll] << 19) >> 31);
}
static inline u32 GetListStatusCardType(u16 id)
{
    return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
}
void sub_0802A4CC(struct Entry *e)
{
    u32 x = 8;
    u32 t;
    u32 mode;

    if ((s32)(*(u32 *)e << 12) < 0) {
        x |= 0x880000;
        sub_080761F0(x, 0x40, 0x1100);
        x = 24;
    }
    if ((gUnk_0201D810.flags & 0xE0) == 0x80) {
        switch (gUnk_0201D810.kind[gUnk_0201D810.sel + gUnk_0201D810.scroll]) {
        case 1:
            sub_080761F0(0x880000 | x, 0x40, 0x1106);
            x += 16;
            break;
        case 2:
            sub_080761F0(0x880000 | x, 0x40, 0x1108);
            x += 16;
            break;
        case 4:
            sub_080761F0(0x880000 | x, 0x40, 0x110A);
            x += 16;
            break;
        }
    }
    t = GetListStatusCardType(e->id);
    if (t <= 20 && (gUnk_0201D810.flags & 0xE0) != 0x40) {
        if (sub_08007834(e->id) && (((u8 *)e)[1] & 0xC0) == 0) {
            sub_080761F0(0x880000 | x, 0x40, 0x1102);
            x += 16;
        }
        mode = gUnk_0201D810.flags & 0xE0;
        if (mode == 0x60 || mode == 0 || mode == 0x20) {
            if ((s32)(*(u32 *)e << 11) < 0) {
                sub_080761F0(0x880000 | x, 0x40, 0x110C);
                x += 16;
            }
        }
    }
    {
        struct Sel *sel = &gUnk_0201D810;
        u32 flags = sel->flags;
        if ((flags & 0xE0) == 0x60) {
            u32 side = flags << 30;
            register int row __asm__("r0") = sel->sel + sel->scroll;
            u8 *players;

            /* FAKEMATCH: retain the initialized row in r0 before forming
             * the player-state base. This input hint emits no instructions. */
            __asm__("" : : "r"(row));
            players = gUnk_020192E4;
            row *= 2;
            row += 0xD64 * (side >> 31);
            players += 0xCC4;
            switch (players[row]) {
            case 1:
                x |= 0x880000;
                sub_080761F0(x, 0x40, 0x1110);
                break;
            case 2:
                x |= 0x880000;
                sub_080761F0(x, 0x40, 0x110E);
                break;
            }
        }
    }
}

/* Draw up to 4 icons (bit i of mask = present), the one at sel highlighted with 3 sprites. */
void sub_0802A658(s32 sel, u16 mask)
{
    s32 y = 0x68;
    s32 i;

    for (i = 0; i <= 3; i++) {
        u16 t = i << 1;
        s32 a = t << 5;
        s32 b = a + 2;

        if ((mask >> i) & 1) {
            if (i == sel) {
                sub_080761F0(y, 0x40, a);
                sub_080761F0(0xA0, 0x4080, b + 2);
                sub_080761F0(0xC0, 0x4080, a + 8);
            } else {
                sub_080761F0(y, 0x40, b);
            }
            y += 0x12;
        }
    }
}
struct DuelFlags {
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
    u8 pad1[3];
    u8 b4;
};
extern struct DuelFlags gUnk_0201CFB0;
extern const u8 gUnk_08698C7C[], gUnk_08698C9C[], gUnk_0869AD1C[], gUnk_0869AD3C[];
extern const u8 gUnk_0869E8E4[], gUnk_0869C45C[], gUnk_0869D758[], gUnk_0869EECC[];
extern const u8 gUnk_0869EEEC[], gUnk_0869B53C[], gUnk_0869B55C[];
extern const u16 gUnk_08082768[];
u32 sub_08060B4C(void);
void sub_080731D0(u32 a, u32 b, u32 c, const void *src);
void sub_08073498(void);
void sub_08073574(void);
void sub_08075114(void *dst, u32 v);
void sub_080752B0(void *dst, const void *src, u32 size);
void sub_080757AC(void);
void sub_080759F4(void);
u16 sub_08075AE4(u16 step);

struct ListInit {
    u8 lo : 4;              /* +0 */
    u8 b4 : 1;
    u8 mode : 3;
    u8 pad1[2];
    u8 state;               /* +3 */
    u8 pad4;
    u8 sel : 2;             /* +5 */
    u8 pad5 : 6;
    u16 scroll;             /* +6 */
    u8 cur : 2;             /* +8 */
    u8 mask : 4;
    u8 pad8 : 2;
    u8 pad9[0x30C - 9];
    u16 count;              /* +0x30C */
};
#define LI ((struct ListInit *)&gUnk_0201D810)
struct MainLI {
    u8 f0[0x49C];
    u16 tiles[0x120];       /* +0x49C */
    u8 f6DC[0x4422 - 0x6DC];
    s16 ofs;                /* +0x4422 */
};
#define ML ((struct MainLI *)&gMain)

u16 sub_0802A6DC(void)
{
    s32 i, j, k;
    u32 tbl;

    switch (LI->state) {
    case 0:
        if (sub_08060B4C()) {
            gUnk_0201CFB0.bit2 = 0;
            gUnk_0201CFB0.bit1 = 0;
            LI->state++;
        }
        return 0;
    case 1:
        sub_08073574();
        sub_08073498();
        gMain.unk40E = 0x303;
        REG_DISPCNT = 0;
        REG_BLDCNT = 0;
        REG_BG0CNT = 4;
        REG_BG1CNT = 0x104;
        REG_BG2CNT = 0x206;
        REG_BG3CNT = 0x387;
        LI->state++;
        return 0;
    case 2:
        /* Assigned up front so the base is a call-crossing pseudo that loses the
           register contest and is rematerialised at its use, as in the ROM. */
        tbl = (u32)gUnk_0869B55C;
        sub_080759F4();
        sub_080757AC();
        sub_080752B0((void *)0x05000200, gUnk_08698C7C, 0x20);
        sub_080752B0((void *)0x06010000, gUnk_08698C9C, 0x2000);
        sub_080752B0((void *)0x05000220, gUnk_0869AD1C, 0x20);
        sub_080752B0((void *)0x06012000, gUnk_0869AD3C, 0x800);
        sub_080731D0(0x400, 0x10, 0x3C6, gUnk_0869E8E4);
        sub_080731D0(0x440, 0x20, 0x354, gUnk_0869C45C);
        sub_080731D0(0x560, 0x30, 0x2E0, gUnk_0869D758);
        sub_08075294((void *)0x05000080, gUnk_0869EECC, 0x20);
        for (i = 0; i < 2; i++) {
            u16 s = i * 0x60;
            u16 d = i * 15;
            sub_080752B0((u8 *)0x06004000 + (d + 0x15A) * 32, gUnk_0869EEEC + s * 32, 0x100);
            sub_080752B0((u8 *)0x06004000 + (d + 0x162) * 32, gUnk_0869EEEC + (s + 0x1C) * 32, 0x40);
            sub_080752B0((u8 *)0x06004000 + (d + 0x164) * 32, gUnk_0869EEEC + (s + 0x20) * 32, 0x20);
            sub_080752B0((u8 *)0x06004000 + (d + 0x165) * 32, gUnk_0869EEEC + (s + 0x3D) * 32, 0x20);
            sub_080752B0((u8 *)0x06004000 + (d + 0x166) * 32, gUnk_0869EEEC + (s + 0x40) * 32, 0x40);
            sub_080752B0((u8 *)0x06004000 + (d + 0x168) * 32, gUnk_0869EEEC + (s + 0x5D) * 32, 0x20);
        }
        sub_080752B0((void *)0x050000E0, gUnk_0869B53C, 0x20);
        if (LI->mode <= 4) {
            sub_080752B0((void *)0x06006840, (const u8 *)(LI->mode * 0x300 + tbl), 0x300);
            for (j = 0; j < 12; j++) {
                u16 x = j;
                gUnk_0300045C[x] = j + 0x7142;
                gUnk_0300045C[x + 0x20] = j + 0x714E;
            }
        }
        LI->state++;
        return 0;
    case 3:
        if (LI->count != 0) {
            sub_0802A47C();
            sub_0802A45C();
            sub_0802A4A4();
            ML->ofs = -(LI->sel * 16);
            LI->cur = 0;
            LI->mask |= 1;
        } else {
            sub_08074B08(0x20, 9);
            sub_08029FC8(0x42, 0x18, gUnk_08082768, 0xC);
            for (k = 0; k < 0x120; k++)
                ML->tiles[k] = k + 0x10;
            sub_08075114((void *)0x06004200, 0);
            LI->cur = 1;
        }
        while (!((LI->mask >> LI->cur) & 1))
            LI->cur++;
        LI->b4 = 1;
        LI->state++;
        return 0;
    case 4:
        REG_DISPCNT |= 0x1F00;
        if (sub_08075AE4(4))
            LI->state++;
        return 0;
    }
    return 1;
}
