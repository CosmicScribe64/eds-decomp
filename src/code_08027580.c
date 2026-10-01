#include "global.h"
#include "gba.h"

/* Four-byte scroller entry (toss work area +0xB14, see code_0802408C). */
struct Scroller {
    u8 pos;             /* +0 */
    s8 speed;           /* +1 */
    u8 stop;            /* +2: index of the stop position reached */
    u8 unk3;
};

/* Reel/marker object (0x14 bytes), initialised by sub_08026D60. */
struct Obj14 {
    u8 state:3;         /* +0x0 bits 0-2 */
    u8 unk0_3:5;
    u8 unk1;
    s16 unk2;           /* +0x2 */
    u8 unk4[2];
    s16 pos;            /* +0x6: 12.4 fixed (>> 4) */
    u8 unk8[0xC];
};

/* Work area at 0x02020310 (0xB24 bytes, fields used here). */
struct Work20310 {
    u8 unk0[0x618];
    struct {
        u16 scaleX;     /* +0x0 (0x100 = 1.0) */
        u16 scaleY;     /* +0x2 */
        u16 angle;      /* +0x4 */
        u8 unk6[0x12];
    } aff[6];           /* +0x618: OBJ affine parameter sets (sub_0807B534) */
    u8 filler6A8[0x918 - 0x6A8];
    struct {
        u8 unk0[0xC];
        union {
            u32 frame;  /* +0xC: animation frame word (masked with 0xFF00FF00) */
            struct {
                u8 b0;
                u8 b1;
                u8 ctl;  /* +0xE: 0 = finished, 0xFF = stopped, 1 = play */
                u8 b3;
            } f;
        } u;
        u8 unk10[4];
    } grp[5];           /* +0x918: sprite groups (sub_08078670 on [0]) */
    u8 filler97C[0xAAC - 0x97C];
    u8 unkAAC[8];       /* +0xAAC: object for sub_080787F4/sub_0807883C (+6 == 2: finished) */
    struct Obj14 objs[4];   /* +0xAB4 */
    u8 unkB04;
    u8 unkB05;
    u8 unkB06[2];
    u8 timer;           /* +0xB08: frames until next tick */
    u8 tick;            /* +0xB09 */
    u8 fillerB0A[2];
    struct {
        u8 pos;         /* +0 ([2].pos is tested for 0x60) */
        u8 count;       /* +1 */
        u8 unk2[2];
    } counters[5];      /* +0xB0C */
    u8 unkB20;          /* +0xB20 */
};
extern struct Work20310 gUnk_02020310;
#define gWork gUnk_02020310

/* Message/sequence block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 unk0[0xB];
    u8 step;            /* +0xB */
};
extern struct DuelMsg gUnk_02017A30;

extern const s8 gUnk_08082404[];    /* counter index per tick (-1 = none) */
extern const u8 gUnk_080826DC[];    /* 3 stop positions */
extern u16 (*const gUnk_08199DFC[])(void);
void sub_08077AEC(u32 se);
struct Main {
    u8 unk0[0x40E];
    u16 unk40E;         /* +0x40E */
    u8 filler410[4];
    void (*vblankCallback)(void);   /* +0x414 */
};
struct IntrVectors {
    u32 unk0;
    void (*hblankCallback)(void);
};
extern struct IntrVectors gUnk_03000000;
void sub_08026E10(void);
void sub_08026CC8(void);
void sub_08026DC8(struct Obj14 *obj);
void sub_08026D30(void);
void sub_08026D84(struct Obj14 *obj);
void sub_08026E84(u32 a, u32 b, s32 y);
void sub_0807883C(void *p);
void sub_0807871C(void *p);
void sub_08078534(void *grp, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, s32 y, void *work);
void sub_0807B5A0(void *aff);
void sub_0807A298(void *p);
void sub_08027580(void);
extern const u8 gUnk_086DE178[];
void sub_0807A908(const void *src, void *dst, u32 w, u32 h);
void sub_08077CEC(const void *src, void *dst, u32 n);
void sub_08077B24(u16 bgm);
extern const u8 gUnk_086E11A4[], gUnk_086E1C6C[], gUnk_086E21D0[];
extern const u8 gUnk_086D0178[], gUnk_086D2178[], gUnk_086D4178[], gUnk_086D6178[];
extern const u8 gUnk_086D8178[], gUnk_086DA178[], gUnk_086DC178[], gUnk_086E22D0[], gUnk_086E24D0[];
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040
struct ObjInit {
    u32 unk0;
    u32 unk4;
    s16 unk8;
    s16 unkA;
};
extern const struct ObjInit gUnk_08199DCC[4];
extern const u8 gUnk_0819A698[];
extern const u16 gUnk_080826E0[];
extern const u16 gUnk_080826E6[];
extern const s16 gUnk_08087BA4[];   /* sine table */
extern const u8 gUnk_08082703[];
extern const u16 gUnk_080826EA[];
extern const u8 gUnk_080826FE[];
void sub_0807A2EC(void *p);
void sub_0807B534(void *p);
void sub_08078670(const void *a, void *b);
void sub_08026D60(u32 a, u32 b, s16 c, s16 d, struct Obj14 *obj);
s32 sub_0807B4D0(s32 a, s32 b);     /* 8.8 fixed-point multiply */
u32 *sub_0807B6B8(u32 a, u32 tile, s32 x, s32 y, u32 w, u32 h, u32 a6, u32 a7, u32 a8,
                  u32 a9, u32 a10, u32 a11, void *work);
void sub_080787F4(u32 a, u32 b, u32 c, void *p);


void sub_08027580(void)
{
    if (--gWork.timer == 0xFF && gWork.tick <= 5) {
        if (gUnk_08082404[gWork.tick] != -1) {
            gWork.counters[gUnk_08082404[gWork.tick]].count++;
            sub_08077AEC(0x2F);
        }
        gWork.timer = 20;
        gWork.tick++;
    }
    if (gWork.counters[2].pos == 0x60) {
        sub_080787F4(0, 0x60, 0, gWork.unkAAC);
        REG_DISPCNT &= ~0x100;
    }
} /* 0x08027580 size 0xA0 */

void sub_08027620(void)
{
    u8 i;

    gWork.timer = 20;
    for (i = 0; i < 5; i++) {
        gWork.counters[i].pos = 0;
        gWork.counters[i].count = 0;
    }
    gWork.tick = 0;
}
u32 sub_08027670(void)
{
    vu16 zero;
    int i;
    const struct ObjInit *init;
    int off;

    zero = 0;
    CpuSet((void *)&zero, &gWork, 0x01000592);
    gMain.unk40E = 1;
    REG_BG0VOFS = 0;
    REG_BG0HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG16(0x028) = 0;
    REG16(0x02A) = 0;
    REG16(0x03C) = 0;
    REG16(0x03E) = 0;
    REG_DISPCNT &= 0xE0FF;
    sub_0807A2EC(&gWork);
    sub_08078670(gUnk_0819A698, &gWork.grp[0]);
    sub_0807B534(gWork.aff);
    for (i = 0; i < 4; i++)
        sub_08026D60(gUnk_08199DCC[i].unk0, gUnk_08199DCC[i].unk4, gUnk_08199DCC[i].unk8,
                     gUnk_08199DCC[i].unkA, &gWork.objs[i]);
    gWork.unkB05 = 0x62;
    return 1;
}
u32 sub_08027754(void)
{
    vu32 zero;
    u8 i, j;

    sub_080787F4(0, -0x180, 0, gWork.unkAAC);
    zero = 0;
    CpuFastSet((void *)&zero, (void *)0x06000000, 0x01006000);
    sub_0807A908(gUnk_086E11A4, (void *)0x0600E000, 30, 20);
    sub_0807A908(gUnk_086E1C6C, (void *)0x0600D000, 30, 20);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            sub_0807A908(gUnk_086E21D0, (u16 *)0x0600C000 + (i * 256 + j * 16), 16, 8);
            sub_0807A908(gUnk_086E21D0, (u16 *)0x0600F000 + (i * 256 + j * 16), 16, 8);
        }
    }
    CpuFastSet(gUnk_086D0178, (void *)0x06000000, 0x800);
    CpuFastSet(gUnk_086D2178, (void *)0x06002000, 0x800);
    CpuFastSet(gUnk_086D4178, (void *)0x06004000, 0x800);
    sub_08077CEC(gUnk_086D6178, (void *)0x06010000, 0x10);
    sub_08077CEC(gUnk_086D8178, (void *)0x06010200, 0x10);
    sub_08077CEC(gUnk_086DA178, (void *)0x06014000, 0x10);
    sub_08077CEC(gUnk_086DC178, (void *)0x06014200, 0x10);
    CpuFastSet(gUnk_086E22D0, (void *)0x05000000, 0x80);
    CpuFastSet(gUnk_086E24D0, (void *)0x05000200, 0x80);
    REG_BG0CNT = 0x1800;
    REG_BG1CNT = 0x1A01;
    REG_BG2CNT = 0x1C02;
    REG_BG3CNT = 0x1E03;
    REG_DISPCNT = 0x1F00;
    gMain.vblankCallback = sub_08026E10;
    for (i = 1; i < 5; i++)
        gWork.grp[i].u.f.ctl |= 0xFF;
    gWork.grp[0].u.f.ctl = 1;
    gWork.grp[2].u.f.ctl = 1;
    sub_08026DC8(&gWork.objs[1]);
    sub_08026DC8(&gWork.objs[2]);
    REG_IME = 0;
    REG_IE &= ~2;
    gUnk_03000000.hblankCallback = sub_08026CC8;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= 2;
    REG_IME = 1;
    REG_BLDCNT = 0x3F41;
    REG_BLDALPHA = 0x100B;
    sub_08077B24(20);
    return 1;
}
u32 sub_080279AC(void)
{
    u8 i;

    sub_0807883C(gWork.unkAAC);
    sub_08026D30();
    if (gWork.unkB05 == 1) {
        gWork.objs[1].state = 1;
        gWork.objs[2].state = 1;
        gWork.objs[3].state = 1;
    }
    if (gWork.unkB05)
        gWork.unkB05--;
    for (i = 0; i < 4; i++)
        sub_08026D84(&gWork.objs[i]);
    if (gWork.objs[0].state == 0 && gWork.objs[1].state == 0 && gWork.unkB05 == 0)
        gWork.objs[3].unk2 = -2;
    sub_08026DC8(&gWork.objs[1]);
    sub_08026DC8(&gWork.objs[2]);
    sub_0807871C(&gWork.grp[0]);
    switch (gWork.unkB04) {
    case 0:
        if ((s8)gWork.grp[0].u.f.ctl == 0) {
            gWork.grp[0].u.f.ctl = 0xFF;
            gWork.grp[1].u.f.ctl = 1;
            gWork.unkB04++;
        }
        if ((gWork.grp[0].u.frame & 0xFF00FF00) == 0xA00)
            sub_08077CEC(gUnk_086DE178, (void *)0x06010000, 0x10);
        if ((gWork.grp[0].u.frame & 0xFF00FF00) == 0x100)
            sub_08077AEC(0x2D);
        if ((gWork.grp[0].u.frame & 0xFF00FF00) == 0xB00)
            sub_08077AEC(0x2E);
        break;
    case 1:
        if ((s8)gWork.grp[1].u.f.ctl == 0) {
            gWork.grp[1].u.f.ctl = 0xFF;
            gWork.grp[3].u.f.ctl = 1;
            gWork.unkB04++;
            gWork.unkB20 = 30;
        }
        break;
    case 2:
        gWork.grp[3].u.f.ctl = 1;
        if (--gWork.unkB20 == 0xFF)
            gWork.unkB04 = 3;
        break;
    }
    switch (gWork.unkB04) {
    case 0:
    case 1:
    case 2:
        sub_08078534(&gWork.grp[0], 0, 1, 0, 0, 0, 8, 0, 0x48 - (gWork.objs[2].pos >> 4), &gWork);
        break;
    case 3:
        sub_08027580();
        gWork.grp[3].u.f.ctl = 1;
        sub_08026E84(3, 0, 0x48 - (gWork.objs[1].pos >> 4));
        break;
    }
    for (i = 0; i < 5; i++)
        sub_0807B5A0(&gWork.aff[i]);
    sub_0807A298(&gWork);
    sub_0807A2EC(&gWork);
    if (gWork.unkAAC[6] == 2)
        return 1;
    return 0;
}
u32 sub_08027C34(void)
{
    REG_IME = 0;
    REG_IE &= ~2;
    REG_IME = 1;
    return 1;
}

u32 sub_08027C58(void)
{
    if (gUnk_08199DFC[gUnk_02017A30.step]) {
        if (gUnk_08199DFC[gUnk_02017A30.step]())
            gUnk_02017A30.step++;
        return 0;
    }
    return 1;
}

void sub_08027C90(void)
{
    gWork.grp[0].u.f.ctl = 1;
}

void sub_08027CA4(void)
{
    gWork.grp[0].u.f.ctl = 0xFF;
}

void sub_08027CB8(struct Scroller *s)
{
    u8 i;

    for (i = 0; i < 4; i++)
        s[i].pos += s[i].speed;
}

void sub_08027CDC(struct Scroller *s)
{
    u8 i = 0;
    s32 pos = s->pos;

    for (; i < 3; i++) {
        if (pos >= gUnk_080826DC[i]) {
            s32 v = s->speed;
            if (v < 0)
                v = -v;
            if (pos < gUnk_080826DC[i] + v) {
                s->speed = 0;
                s->pos = gUnk_080826DC[i];
                s->stop = i;
            }
        }
    }
}

void sub_08027D1C(struct Scroller *s)
{
    if (s->pos == 0x30)
        s->speed = 0;
    if (s->pos == 0)
        s->speed = 0;
}
void sub_08027D34(u16 *tiles, u8 *pals, u8 angle, u8 sel, u8 bump, u16 flags, u8 spread)
{
    u8 dy[3];
    u8 back = 0xFF - spread;
    u8 i;
    s32 prio, front;
    s32 y;
    s32 width = 0x20, height = 0x40;
    s16 c;
    u32 *oam;
    u32 attr;

    for (i = 0; i < 3; i++) {
        if (i == sel)
            dy[i] = -bump;
        else
            dy[i] = bump * 2;
    }
    if (((angle + spread) % 256 >= 0x40 && (angle + spread) % 256 < 0xC0) || (flags & 1)) {
        prio = 1;
        front = 1;
    } else {
        prio = 0;
        front = 0;
    }
    oam = sub_0807B6B8(prio, tiles[2], (sub_0807B4D0(gUnk_08087BA4[(angle + spread) % 256], 0x3000) >> 8) + 0x58,
                       (y = sub_0807B4D0(gUnk_08087BA4[(angle + spread) % 256 + 0x40], 0x1000), y >>= 8, y += 0x32, y += dy[2]),
                       width, height, 4, pals[2], 0x200, 0, 0, 0, &gWork);
    attr = *oam;
    *oam = attr | (front ? 0x02000700 : 0x02000300);
    gWork.aff[1].angle = 0x8000;
    c = gUnk_08087BA4[(angle + spread) % 256 + 0x40];
    gWork.aff[1].scaleX = sub_0807B4D0(0x40, c) + 0xC0;
    gWork.aff[1].scaleY = sub_0807B4D0(0x40, c) + 0xC0;
    if (((angle + back) % 256 >= 0x40 && (angle + back) % 256 < 0xC0) || (flags & 1)) {
        prio = 1;
        front = 1;
    } else {
        prio = 0;
        front = 0;
    }
    oam = sub_0807B6B8(prio, tiles[1], (sub_0807B4D0(gUnk_08087BA4[(angle + back) % 256], 0x3000) >> 8) + 0x58,
                       (y = sub_0807B4D0(gUnk_08087BA4[(angle + back) % 256 + 0x40], 0x1000), y >>= 8, y += 0x32, y += dy[1]),
                       width, height, 4, pals[1], 0x200, 0, 0, 0, &gWork);
    attr = *oam;
    *oam = attr | (front ? 0x04000700 : 0x04000300);
    gWork.aff[2].angle = 0x8000;
    c = gUnk_08087BA4[(angle + back) % 256 + 0x40];
    gWork.aff[2].scaleX = sub_0807B4D0(0x40, c) + 0xC0;
    gWork.aff[2].scaleY = sub_0807B4D0(0x40, c) + 0xC0;
    if ((angle % 256 >= 0x40 && angle % 256 < 0xC0) || (flags & 1)) {
        prio = 1;
        front = 1;
    } else {
        prio = 0;
        front = 0;
    }
    oam = sub_0807B6B8(prio, tiles[0], (sub_0807B4D0(gUnk_08087BA4[angle % 256], 0x3000) >> 8) + 0x58,
                       (y = sub_0807B4D0(gUnk_08087BA4[angle % 256 + 0x40], 0x1000), y >>= 8, y += 0x32, y += dy[0]),
                       width, height, 4, pals[0], 0x200, 0, 0, 0, &gWork);
    attr = *oam;
    *oam = attr | (front ? 0x700 : 0x300);
    gWork.aff[0].angle = 0x8000;
    c = gUnk_08087BA4[angle % 256 + 0x40];
    gWork.aff[0].scaleX = sub_0807B4D0(0x40, c) + 0xC0;
    gWork.aff[0].scaleY = sub_0807B4D0(0x40, c) + 0xC0;
}

#if 0 /* NONMATCHING: logic and size match; ROM keeps r7 free (0x38 built in r7, y = 0x6E - 0x42) and puts the table base in sl */
void sub_080280D0(u8 idx, u16 flags)
{
    s32 w = 0x40;
    u32 i;
    u16 hflip;

    for (i = 0, hflip = flags & 4; i < 2; i++) {
        s16 tile = gUnk_080826EA[idx * 2 + i];
        s32 x = 0x38;
        s32 y = 0x6E;
        u32 *oam;
        u32 attr;
        x += i * w;
        if (idx == 0)
            y -= 0x42;
        oam = sub_0807B6B8(0, tile, x, y, w, 0x20, 4, gUnk_080826FE[idx], 0x200, 0, 0, 0, &gWork);
        attr = *oam;
        if (hflip)
            *oam = attr | 0x400;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08027580", sub_080280D0); /* 0x080280D0 size 0xA8 */

void sub_08028178(u8 idx, u8 t, u16 flags)
{
    s32 w = 0x20;
    s32 h = 0x40;
    u32 *oam = sub_0807B6B8(0, gUnk_080826E0[idx], 0x69, (sub_0807B4D0(t << 8, 0x160) >> 8) + 0xC0,
                            w, h, 4, gUnk_08082703[idx], 0x200, 0, 0, 0, &gWork);
    *oam |= (flags & 2) ? 0x06000400 : 0x06000000;
    gWork.aff[3].angle = 0;
    gWork.aff[3].scaleX = 0x100;
    gWork.aff[3].scaleY = 0x100;
}

void sub_08028238(u32 unused, u8 angle, u16 flags, s16 *scale, u8 sel)
{
    s32 w = 0x40;
    s32 h = 0x20;
    u32 *oam;
    u32 attr;

    oam = sub_0807B6B8(0, gUnk_080826E6[0], 0x28,
                       (sub_0807B4D0(gUnk_08087BA4[(angle * 2) % 256], scale[0]) >> 8) + 0x30,
                       w, h, 4, sel == 0 ? 3 : 9, 0x200, 0, 0, 0, &gWork);
    attr = *oam;
    *oam = attr | ((flags & 8) ? 0x08000400 : 0x08000000);
    gWork.aff[4].angle = 0;
    gWork.aff[4].scaleX = 0x100;
    gWork.aff[4].scaleY = 0x100;
    {
        s32 y = (sub_0807B4D0(gUnk_08087BA4[(angle * 2) % 256], scale[2]) >> 8) + 0x30;
        u16 tile = gUnk_080826E6[1];
        s32 x = 0x88;

        /* FAKEMATCH: keep the initialized x in r2 after the tile load,
         * before preparing the stack arguments, as in the ROM. */
        __asm__("" : : "r"(x));
        oam = sub_0807B6B8(0, tile, x, y, w, h, 4, sel == 1 ? 3 : 9, 0x200, 0, 0, 0, &gWork);
    }
    attr = *oam;
    *oam = attr | ((flags & 8) ? 0x0A000400 : 0x0A000000);
    gWork.aff[5].angle = 0;
    gWork.aff[5].scaleX = 0x100;
    gWork.aff[5].scaleY = 0x100;
}
#if 0 /* NONMATCHING: ordinary extents recover y0 live in sl and exact size; selector/X/OAM-value allocation remains different */
void sub_080283BC(u32 unused, u8 angle, u16 flags, s16 *scale, u8 sel, s16 *anim)
{
    s32 width = 0x40, height = 0x20;
    s32 x0 = 0x78;
    s32 y0 = 0x30;
    s32 x;
    u8 i;
    u32 *oam;
    u32 attr;

    for (i = 0; i < 2; i++) {
        if (scale[i * 2] > 0x7F)
            scale[i * 2] -= 0x80;
        else
            scale[i * 2] = 0;
    }
    switch (sel) {
    case 0:
        x = (sub_0807B4D0(0x100 - gUnk_08087BA4[anim[0] + 0x40], 0x3000) >> 8) - 0x50;
        x = x0 + x;
        oam = sub_0807B6B8(0, gUnk_080826E6[0], x,
                           y0 + (sub_0807B4D0(gUnk_08087BA4[(angle * 2) % 256], scale[0]) >> 8),
                           width, height, 4, 3, 0x200, 0, 0, 0, &gWork);
        attr = *oam;
        *oam = attr | ((flags & 8) ? 0x08000400 : 0x08000000);
        oam = sub_0807B6B8(0, gUnk_080826E6[1], x0 - 0x10, ((anim[1] * anim[1]) >> 1) + y0,
                           width, height, 4, 9, 0x200, 0, 0, 0, &gWork);
        attr = *oam;
        *oam = attr | ((flags & 8) ? 0x08000700 : 0x08000300);
        gWork.aff[4].angle = ((u32)(u16)anim[1]) << 9;
        break;
    case 1:
        oam = sub_0807B6B8(0, gUnk_080826E6[0], x0 - 0x70, ((anim[1] * anim[1]) >> 1) + y0,
                           width, height, 4, 9, 0x200, 0, 0, 0, &gWork);
        attr = *oam;
        *oam = attr | ((flags & 8) ? 0x08000700 : 0x08000300);
        oam = sub_0807B6B8(0, gUnk_080826E6[1],
                           x0 - ((sub_0807B4D0(0x100 - gUnk_08087BA4[anim[0] + 0x40], 0x3000) >> 8) - 0x10),
                           (sub_0807B4D0(gUnk_08087BA4[(angle * 2) % 256], scale[2]) >> 8) + y0,
                           width, height, 4, 3, 0x200, 0, 0, 0, &gWork);
        attr = *oam;
        *oam = attr | ((flags & 8) ? 0x08000400 : 0x08000000);
        gWork.aff[4].angle = -(((u32)(u16)anim[1]) << 9);
        break;
    }
    gWork.aff[4].scaleX = 0x100;
    gWork.aff[4].scaleY = 0x100;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08027580", sub_080283BC); /* 0x080283BC size 0x2C8 */

