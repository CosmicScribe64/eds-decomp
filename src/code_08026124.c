#include "global.h"
#include "gba.h"

typedef u16 (*StepFunc)(void);

/* Scrolling reel object (0x14 bytes), see also code_08027580. */
struct Reel {
    u8 state:3;         /* +0x0 bits 0-2: nonzero = moving */
    u8 unk0_3:5;
    u8 unk1;
    u16 speed;          /* +0x2: s16 */
    u16 target;         /* +0x4: s16 */
    u16 pos;            /* +0x6: s16, 12.4 fixed */
    s8 row;             /* +0x8: last tile row drawn */
    u8 unk9[3];
    u8 *src;            /* +0xC: tilemap source (rows of 60 bytes) */
    u8 *dst;            /* +0x10: BG map (rows of 64 bytes) */
};

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
    u8 unkD;                /* +0xD: 1 = ... */
    u8 unkE;                /* +0xE */
    u8 unkF;
    u8 unk10;               /* +0x10 */
    u8 unk11[3];
};

/* Moving object (0x14 bytes, sub_0807A420). */
struct Mover {
    u16 x;
    u16 y;
    u8 unk4[0x10];
};

/* Work area at 0x02020310 (0x1738 bytes). It is shared by several screens,
 * so the fields below overlap: reels[] (sub_08026D84..) vs movers[] (sub_080268E4). */
struct Work20310 {
    u8 oam[0x618];          /* +0x000: OAM buffer (sub_0807A298 / sub_0807A2EC) */
    struct {
        u16 scaleX;         /* +0x0 (0x100 = 1.0) */
        u16 scaleY;         /* +0x2 */
        u16 angle;          /* +0x4 */
        u8 unk6[0x12];
    } aff[0x20];            /* +0x618: OBJ affine sets (sub_0807B534) */
    struct SpriteGroup grp[5];  /* +0x918 */
    u8 filler97C[0xAA8 - 0x97C];
    u16 unkAA8;             /* +0xAA8 */
    u8 fillerAAA[2];
    u8 unkAAC[4];           /* +0xAAC */
    union {
        struct {                    /* slot-reel screen (code_08027580) */
            u8 pad[4];
            struct Reel reels[4];   /* +0xAB4 */
            u8 unkB04;
            u8 unkB05;
            u8 unkB06;
            u8 unkB07;
            u8 fillerB08[4];
            struct {
                u8 pos;             /* +0 */
                u8 count;           /* +1: step of the sub_08026E84 animation */
                u8 unk2[2];
            } counters[5];          /* +0xB0C */
        } r;
        struct {
            struct Mover movers[5]; /* +0xAB0 */
            u8 unkB14;
            u8 unkB15;
            u8 unkB16;              /* +0xB16: sine phase */
            u8 unkB17;
            struct {
                u16 unk0;
                u16 blend;          /* +0xB1A: high byte = blend coefficient */
                s16 speed;          /* +0xB1C */
                u8 state;           /* +0xB1E: 2 = finished */
                u8 unk7;
            } fade;                 /* +0xB18: object for sub_080787F4/sub_0807883C */
        } m;
    } u;
    struct Timer timer;     /* +0xB20 */
    u8 unkB24;              /* +0xB24: wave phase (HBlank sub_080261E0) */
    u8 unkB25;
    u8 fillerB26[2];
    u8 ramp[0x1728 - 0xB28];    /* +0xB28: palette ramp (sub_0807B150 / sub_0807B224) */
    u8 unk1728;
    u8 filler1729[0x1734 - 0x1729];
    u8 unk1734;
    u8 unk1735;
    u8 filler1736[2];
};
extern struct Work20310 gUnk_02020310;
#define gWork gUnk_02020310

/* Message/sequence block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 unk0[0xB];
    u8 step;            /* +0xB */
};
extern struct DuelMsg gUnk_02017A30;

extern const StepFunc gUnk_08199A58[];
extern const StepFunc gUnk_08199A70[];
extern const StepFunc gUnk_08199DA4[];
extern const s16 gUnk_08087BA4[];   /* sine table */

void sub_0807883C(void *p);
void sub_080787F4(u32 a, s32 b, u32 c, void *p);
void sub_0807B0D0(struct Timer *t);
void sub_0807A908(const void *src, void *dst, u32 w, u32 h);
void sub_08075294(void *dest, const void *src, u32 size);
u16 *sub_0807A320(u32 a, void *b);
void sub_080269FC(void);

struct Main {
    u32 rngState;
    u16 heldKeys;           /* +0x4 */
    u16 newKeys;            /* +0x6 */
    u8 filler8[0x40E - 0x8];
    u16 unk40E;             /* +0x40E */
    u8 filler410[4];
    void (*vblankCallback)(void);   /* +0x414 */
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040

struct IntrVectors {
    u32 unk0;
    void (*hblankCallback)(void);   /* +0x4 */
};
extern struct IntrVectors gUnk_03000000;

s32 sub_0807B4D0(s32 a, s32 b);     /* 8.8 fixed-point multiply */
void sub_0807B224(void *ramp);
void sub_0807B150(void *pal, u32 a, u32 b, void *ramp);
void sub_0807B0C0(struct Timer *t);
void sub_0807B0C8(struct Timer *t, u16 time);
u16 sub_08078670(const void *anim, struct SpriteGroup *grp);
void sub_080786D0(struct SpriteGroup *grp);
void sub_08077CEC(const void *src, void *dst, u32 n);
void sub_08075278(void *dst, u32 size);
void sub_0807A298(void *p);
void sub_0807A2EC(void *p);
void sub_0807B534(void *p);
void sub_08077AEC(u16 se);          /* PlaySE */
void sub_08026194(void *p);
void sub_080263B0(const u8 *src, u32 tile, u32 srcTile, u32 size);
void sub_08026220(struct SpriteGroup *grp, u8 prio, void *work);

extern const u8 gUnk_08199D9C[];
extern const u8 gUnk_086B8568[], gUnk_086C1B68[], gUnk_086B6368[], gUnk_086B6568[];
extern const u8 gUnk_086CED78[], gUnk_086CF778[];
struct Tmpl8 { u8 b[8]; };
extern const struct Tmpl8 gUnk_08199D74[];
u16 *sub_08077EF4(const void *templates, u32 a1, u32 a2, s32 x, s32 y, u32 a5, u32 a6, u32 a7,
                         u32 a8, u32 a9, u32 a10, void *work);
void sub_0807A420(struct Mover *m);
void sub_0807B5A0(void *aff);
extern const u8 gUnk_086C1D68[], gUnk_086C3D68[], gUnk_086C5D68[], gUnk_086C7D68[];
extern const u8 gUnk_086CAB78[], gUnk_086CAD78[], gUnk_086CCD78[];
extern const u8 gUnk_086C9D68[], gUnk_086CA218[], gUnk_086CA6C8[];
extern const u8 gUnk_08199CC8[];
/* Two literal-pool words that are 0 in the ROM (hypothesis: link-time symbols at address 0). */
extern u8 gUnkA_00000000[], gUnkB_00000000[];
void sub_0807AD40(const void *src, void *a1, void *a2, u32 w, void *dst, u32 a5, u32 a6, u32 a7,
                  u32 h, u32 a9);
void sub_080261E0(void);
void sub_080261A8(struct Mover *m);

u16 sub_08026124(void)
{
    StepFunc step = gUnk_08199A58[gUnk_02017A30.step];
    if (step != NULL) {
        if (step())
            gUnk_02017A30.step++;
        return 0;
    }
    return 1;
}

u16 sub_0802615C(void)
{
    StepFunc step = gUnk_08199A70[gUnk_02017A30.step];
    if (step != NULL) {
        if (step())
            gUnk_02017A30.step++;
        return 0;
    }
    return 1;
}

void sub_08026194(void *p)
{
    gWork.unkAAC[0] = 0;
}

extern const struct { s16 unk0; s16 unk2; } gUnk_080823B0[];
void sub_0807A398(s32 a, s32 b, u32 c, u32 d, struct Mover *m);

/* Start the five movers towards (0x68, 0x40) from the positions in gUnk_080823B0. */
void sub_080261A8(struct Mover *m)
{
    u8 i;
    for (i = 0; i < 5; i++)
        sub_0807A398(gUnk_080823B0[i].unk2, gUnk_080823B0[i].unk0, 0x68, 0x40, m++);
}

/* HBlank: BG0HOFS wave from the sine table. */
void sub_080261E0(void)
{
    REG_BG0HOFS = gUnk_08087BA4[(REG_VCOUNT + gWork.unkB24) & 0xFF] >> 5;
}

/* Draw a sprite group as affine OBJs with the given priority (tiles from 0x200). */
void sub_08026220(struct SpriteGroup *grp, u8 prio, void *work)
{
    const u16 *e = grp->templates;
    u8 i;
    for (i = 0; i < grp->count; i++) {
        u16 *o = sub_0807A320(0, work);
        o[0] = ((e[0] | 0x100) & 0xFF00) | (e[0] & 0xFF);
        o[1] = e[1] & 0xC1FF;
        o[2] = (((e[2] & 0xFF0F) | ((e[2] & 0xF0) << 1)) & 0xF3FF) | (((prio << 10) & 0xC00) + 0x200);
        e += 4;
    }
}

u16 sub_080262BC(void)
{
    sub_08075278(&gWork, sizeof(gWork));
    gMain.unk40E = 1;
    REG_DISPCNT &= 0xE0FF;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    sub_08026194(gWork.unkAAC);
    gWork.u.m.unkB15 = 0;
    gWork.u.m.unkB16 = 0;
    gWork.u.m.unkB14 = 0;
    sub_0807A2EC(&gWork);
    sub_0807B534(gWork.aff);
    REG_BLDCNT = 0x3F3F;
    REG_BLDALPHA = 0x808;
    REG_BLDY = 0x10;
    gWork.unk1734 = 0;
    gWork.unk1735 = 0;
    return 1;
}

/* Copy 4 rows of 0x80 bytes into OBJ VRAM (row stride 0x400). */
void sub_08026388(const u8 *src, u8 *dst)
{
    int i;
    for (i = 0; i < 4; i++) {
        sub_08075294(dst, src, 0x80);
        dst += 0x400;
        src += 0x80;
    }
}

void sub_080263B0(const u8 *src, u32 tile, u32 srcTile, u32 size)
{
    sub_08026388(src + srcTile * 32, (u8 *)0x06014000 + tile * 32);
}

u16 sub_080263C8(void)
{
    sub_080787F4(0, -0x80, 0, &gWork.u.m.fade);
    gWork.unkAA8 = sub_08078670(gUnk_08199D9C, &gWork.grp[0]);
    CpuFastSet(gUnk_086B8568, (void *)VRAM, 0x2580);
    CpuFastSet(gUnk_086C1B68, (void *)PLTT, 0x80);
    CpuSet(gUnk_086B6368, (void *)(PLTT + 0x200), 0x100);
    sub_08077CEC(gUnk_086B6568, (void *)0x06014000, 0x10);
    sub_080263B0(gUnk_086CED78, 4, 0x10, 0x40);
    sub_080263B0(gUnk_086CED78, 8, 0x20, 0x40);
    sub_080263B0(gUnk_086CED78, 0xC, 0x30, 0x40);
    sub_080263B0(gUnk_086CED78, 0x80, 0x40, 0x40);
    sub_080263B0(gUnk_086CF778, 0x88, 0x10, 0x40);
    sub_080263B0(gUnk_086CF778, 0x8C, 0x20, 0x40);
    sub_080263B0(gUnk_086CF778, 0x100, 0x30, 0x40);
    sub_080263B0(gUnk_086CF778, 0x104, 0x40, 0x40);
    REG_DISPCNT = 0x1F04;
    sub_0807B0C0(&gWork.timer);
    sub_0807B0C8(&gWork.timer, 0x3C);
    return 1;
}

u16 sub_080264D4(void)
{
    CpuFastSet(gUnk_086C1D68, (void *)VRAM, 0x800);
    CpuFastSet(gUnk_086C3D68, (void *)(VRAM + 0x2000), 0x800);
    CpuFastSet(gUnk_086C5D68, (void *)(VRAM + 0x4000), 0x800);
    CpuFastSet(gUnk_086C7D68, (void *)(VRAM + 0x6000), 0x800);
    CpuFastSet(gUnk_086CAB78, (void *)PLTT, 0x80);
    sub_08077CEC(gUnk_086CAD78, (void *)0x06010000, 0x10);
    sub_08077CEC(gUnk_086CCD78, (void *)0x06014000, 0x10);
    sub_0807A908(gUnk_086C9D68, (void *)(VRAM + 0xE000), 0x1E, 0x14);
    sub_0807A908(gUnk_086CA218, (void *)(VRAM + 0xD000), 0x1E, 0x14);
    sub_0807A908(gUnk_086CA6C8, (void *)(VRAM + 0xC000), 0x1E, 0x14);
    sub_0807AD40(gUnk_086CA6C8, gUnkA_00000000, gUnkB_00000000, 0x1E, (void *)(VRAM + 0xC03C), 0, 0, 2, 0x14, 0);
    sub_080787F4(1, -0x80, 0, &gWork.u.m.fade);
    sub_0807B0C8(&gWork.timer, 0x96);
    sub_08078670(gUnk_08199CC8, &gWork.grp[0]);
    gWork.grp[0].unk10 = 1;
    REG_BG0CNT = 0x1800;
    REG_BG1CNT = 0x1A01;
    REG_BG2CNT = 0x1C02;
    REG_DISPCNT = 0x1700;
    sub_0807B150((void *)PLTT, 0x200, 0x1F, gWork.ramp);
    sub_0807B534(gWork.aff);
    gWork.u.m.unkB16 = 0;
    REG_IME = 0;
    REG_IE &= ~2;
    gUnk_03000000.hblankCallback = sub_080261E0;
    REG_IME = 1;
    gWork.unkB24 = 0;
    gWork.unkB25 = 0;
    REG_IME = 0;
    REG_IE |= 2;
    REG_IME = 1;
    return 1;
}

u16 sub_080266A4(void)
{
    struct SpriteGroup *g = gWork.grp;
    u8 i;

    sub_0807883C(&gWork.u.m.fade);
    switch (gWork.unkAAC[0]) {
    case 0:
        if (g->unkD == 5) {
            g->unkE = 0xFF;
            sub_080787F4(1, 0xA0, 0, &gWork.u.m.fade);
            REG_BLDCNT = 0x90;
            gWork.unkAAC[0]++;
            sub_08077AEC(0x13);
        }
        break;
    case 1:
        if (gWork.u.m.fade.blend >= 0x800) {
            gWork.u.m.fade.speed *= -1;
            gWork.unkAAC[0]++;
            g->unkE = 0;
            g->unkF = 0;
        }
        break;
    case 2:
        if (gWork.u.m.fade.state == 3) {
            gWork.u.m.fade.state = 0;
            gWork.unkAAC[0]++;
        }
        g->unkE = 0xFF;
        break;
    case 3:
        sub_080261A8(gWork.u.m.movers);
        for (i = 0; i < gWork.unkAA8; i++)
            sub_08077EF4(g->templates, g->unk10, g->count, g->x, g->y, 0, 0, 0, 1, 0, 0, &gWork);
        sub_0807A298(&gWork);
        sub_0807A2EC(&gWork);
        sub_080787F4(1, 0x20, 0, &gWork.u.m.fade);
        gWork.timer.state = 0;
        gWork.u.m.unkB16 = 0;
        sub_08077AEC(0x14);
        return 1;
    }
    gWork.unk1735 = gWork.grp[0].unkD;
    if (gWork.unkAAC[0] == 0 && gWork.unk1735 != gWork.unk1734) {
        gWork.unk1734 = gWork.unk1735;
        sub_08077AEC(0x13);
    }
    for (i = 0; i < gWork.unkAA8; i++)
        sub_080786D0(&gWork.grp[i]);
    for (i = 0; i < gWork.unkAA8; i++) {
        sub_08077EF4(g->templates, g->unk10, g->count, g->x, g->y, 0, 0, 0, 1, 0, 0, &gWork);
        g++;
    }
    sub_0807A298(&gWork);
    sub_0807A2EC(&gWork);
    sub_0807B0D0(&gWork.timer);
    return 0;
}

u16 sub_080268E4(void)
{
    u8 i;
    const struct Tmpl8 *tmpl;

    sub_0807883C(&gWork.u.m.fade);
    if (gWork.u.m.unkB16 < 0x40)
        gWork.u.m.unkB16++;
    gWork.u.m.fade.speed = sub_0807B4D0(0x40, 0x100 - gUnk_08087BA4[gWork.u.m.unkB16 + 0x40]);
    if (gWork.u.m.fade.state == 2) {
        gWork.u.m.fade.state = 0;
        sub_0807B0C8(&gWork.timer, 1);
    }
    if (gWork.timer.state == 2) {
        sub_08077AEC(0x15);
        return 1;
    }
    tmpl = gUnk_08199D74;
    for (i = 0; i < 5; i++) {
        sub_0807A420(&gWork.u.m.movers[i]);
        gWork.grp[i].x = gWork.u.m.movers[i].x;
        gWork.grp[i].y = gWork.u.m.movers[i].y;
        sub_08077EF4(tmpl++, 0, 1, gWork.u.m.movers[i].x, gWork.u.m.movers[i].y, 1, 0, 0, 1, 0, 0, &gWork);
    }
    sub_0807A298(&gWork);
    sub_0807A2EC(&gWork);
    sub_0807B0D0(&gWork.timer);
    return 0;
}

/* Per-frame sprite update: draw both groups, animate the wave and the zoom. */
void sub_080269FC(void)
{
    struct SpriteGroup *g = &gWork.grp[0];
    sub_08077EF4(g->templates, 1, g->count, g->x, g->y, 0, 2, 0, 0, 0, 0, &gWork);
    sub_08026220(&gWork.grp[1], 1, &gWork);
    sub_080786D0(&gWork.grp[0]);
    sub_080786D0(&gWork.grp[1]);
    if (gWork.grp[0].unkD == 1)
        gWork.grp[0].unkE = 0xFF;
    gWork.grp[1].unkE = 1;
    gWork.unkB25++;
    if (gWork.unkB25 & 2) {
        gWork.unkB24++;
        gWork.unkB25 = 0;
    }
    gWork.aff[0].scaleX = gWork.aff[0].scaleY = sub_0807B4D0(0x90, 0x100 - gUnk_08087BA4[gWork.u.m.unkB16 + 0x40]) + 0x80;
    gWork.u.m.unkB16++;
    sub_0807B5A0(gWork.aff);
    sub_0807A298(&gWork);
    sub_0807A2EC(&gWork);
}

u16 sub_08026AE0(void)
{
    sub_0807883C(&gWork.u.m.fade);
    if (gWork.u.m.fade.state == 2) {
        gWork.u.m.fade.state = 0;
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        return 1;
    }
    if (gWork.timer.state == 2) {
        sub_080787F4(0, 0x100, 0, &gWork.u.m.fade);
        gWork.timer.state = 0;
        gWork.grp[1].unkE = 0xFF;
    }
    sub_080269FC();
    if (gWork.u.m.unkB16 >= 0x50) {
        sub_0807B224(gWork.ramp);
        gWork.unk1728 = sub_0807B4D0(0x1F00, 0x100 - gUnk_08087BA4[gWork.u.m.unkB16 - 0x10]) >> 8;
    }
    sub_0807B0D0(&gWork.timer);
    if (gMain.newKeys & 2) {
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        return 1;
    }
    return 0;
}

u16 sub_08026BA8(void)
{
    sub_0807883C(&gWork.u.m.fade);
    if (gWork.timer.state == 2)
        return 1;
    sub_0807B0D0(&gWork.timer);
    return 0;
}

u16 sub_08026BD0(void)
{
    sub_080269FC();
    if (gWork.u.m.fade.state == 3) {
        gWork.u.m.fade.state = 0;
        sub_080787F4(1, -0x40, 0, &gWork.u.m.fade);
        REG_BLDCNT = 0x3F41;
        REG_BLDALPHA = 0x80D;
        return 1;
    }
    sub_0807883C(&gWork.u.m.fade);
    return 0;
}

u16 sub_08026C38(void)
{
    u32 blend;
    sub_080269FC();
    REG_BLDALPHA = ((blend = gWork.u.m.fade.blend << 16) >> 24) | 0x800;
    if (blend <= 0xA000000) {
        gWork.u.m.fade.state = 0;
        return 1;
    }
    sub_0807883C(&gWork.u.m.fade);
    return 0;
}

u16 sub_08026C90(void)
{
    StepFunc step = gUnk_08199DA4[gUnk_02017A30.step];
    if (step != NULL) {
        if (step())
            gUnk_02017A30.step++;
        return 0;
    }
    return 1;
}

extern const s8 gUnk_080823C4[];

/* HBlank: BG0/BG3 HOFS wave (64-entry table). */
void sub_08026CC8(void)
{
    REG_BG0HOFS = gUnk_080823C4[(gWork.u.r.unkB06 + (REG_VCOUNT >> 1)) % 64] + gWork.u.r.unkB06;
    REG_BG3HOFS = gWork.u.r.unkB06 - gUnk_080823C4[(gWork.u.r.unkB06 + (REG_VCOUNT >> 2)) % 64];
}

void sub_08026D30(void)
{
    if (--gWork.u.r.unkB07 == 0xFF) {
        gWork.u.r.unkB07 = 8;
        gWork.u.r.unkB06++;
    }
}

void sub_08026D60(u8 *src, u8 *dst, s16 speed, s16 target, struct Reel *reel)
{
    reel->pos = 0;
    reel->speed = speed;
    reel->target = target;
    reel->state = 0;
    reel->row = -1;
    reel->src = src;
    reel->dst = dst;
}

/* Move the reel towards its target; stop there. */
void sub_08026D84(struct Reel *reel)
{
    if (reel->state) {
        reel->pos += reel->speed;
        if ((s16)reel->speed < 0) {
            if ((s16)reel->pos <= (s16)reel->target) {
                reel->state = 0;
                reel->pos = reel->target;
            }
        } else {
            if ((s16)reel->pos >= (s16)reel->target) {
                reel->state = 0;
                reel->pos = reel->target;
            }
        }
    }
}

void sub_08026DC8(struct Reel *reel)
{
    int old = reel->row;
    int y = (s16)reel->pos >> 4;
    if (old != y / 8) {
        reel->row = y / 8;
        sub_0807A908(reel->src + (reel->row + 0x16) * 60, reel->dst + ((reel->row - 1) & 0x1F) * 64, 0x1E, 1);
    }
}

void sub_08026E10(void)
{
    REG_BG0HOFS = 0;
    REG_BG0VOFS = (s16)gWork.u.r.reels[0].pos >> 4;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = (s16)gWork.u.r.reels[1].pos >> 4;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = (s16)gWork.u.r.reels[2].pos >> 4;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = (s16)gWork.u.r.reels[3].pos >> 4;
}

#define SIN(i) gUnk_08087BA4[i]

/* Draw the five reel-stop markers at (x, y): each pulses (affine scale) and,
 * once its counter is started, flies along an arc (two sprites each). */
void sub_08026E84(u32 unused, u16 x, u16 y)
{
    u8 i;
    u16 affine, dx1;
    /* FAKEMATCH: a word-sized destination preserves the chained zero copy from r9. */
    u32 dy1;
    u16 dy2;
    /* FAKEMATCH: retain X in r9; explicit u16 casts below preserve coordinate wrapping. */
    register u32 dx2 asm("r9");
    u32 pal;
    s32 skip = 0;

    for (i = 0; i < 5; i++) {
        pal = 7;
        switch (i) {
        case 0:
            gWork.aff[0].scaleX = gWork.aff[0].scaleY = 0x100 - sub_0807B4D0(0xC0, SIN((gWork.u.r.counters[0].pos * 3 / 2) & 0xFF));
            break;
        case 1:
            gWork.aff[i].scaleX = gWork.aff[i].scaleY = 0x100 - sub_0807B4D0(0xC0, SIN((gWork.u.r.counters[i].pos * 3 / 2) & 0xFF));
            break;
        case 2:
            gWork.aff[i].scaleX = gWork.aff[i].scaleY = 0x100 - sub_0807B4D0(0xC0, SIN((gWork.u.r.counters[i].pos * 3 / 2) & 0xFF));
            break;
        case 3:
            gWork.aff[i].scaleX = gWork.aff[i].scaleY = 0x100 - sub_0807B4D0(0xC0, SIN((gWork.u.r.counters[i].pos * 3 / 2) & 0xFF));
            break;
        case 4:
            gWork.aff[i].scaleX = gWork.aff[i].scaleY = 0x100 - sub_0807B4D0(0xC0, SIN((gWork.u.r.counters[i].pos * 3 / 2) & 0xFF));
            break;
        }
        switch (i) {
        case 0:
            switch (gWork.u.r.counters[0].count) {
            case 0:
                dy1 = dx2 = dx1 = 0;
                dy2 = 0;
                affine = 0;
                break;
            case 1:
                /* FAKEMATCH: narrow the word-sized r9 value to the original arc width. */
                dx2 = (u16)(-(sub_0807B4D0(0xA00, SIN(0x30)) >> 4) + (sub_0807B4D0(0xA00, SIN((gWork.u.r.counters[0].pos + 0x30) & 0xFF)) >> 4));
                dy2 = (sub_0807B4D0(0x6000, SIN(0x10)) >> 8) - (sub_0807B4D0(0x6000, SIN(gWork.u.r.counters[0].pos + 0x10)) >> 8);
                /* Unsigned packing preserves the original u16 coordinate wrap. */
                {
                    register u32 px asm("r1"); /* FAKEMATCH: packed X scratch register. */
                    register u32 big asm("r2"); /* FAKEMATCH: shared 32-pixel offset register. */
                    register u32 small asm("r3"); /* FAKEMATCH: shared 16-pixel offset register. */
                    u32 tmp;
                    px = (u32)dx2 << 16;
                    asm("" : "+r"(px)); /* FAKEMATCH: preserve the packed X register. */
                    big = 0x200000;
                    asm("" : "+r"(big)); /* FAKEMATCH: retain the shared large offset. */
                    tmp = px + big;
                    dx1 = tmp >> 16;
                    tmp = ((u32)dy2 << 16) + big;
                    dy1 = tmp >> 16;
                    small = 0x100000;
                    asm("" : "+r"(small)); /* FAKEMATCH: retain the shared small offset. */
                    px += small;
                    asm("" : "+r"(px)); /* FAKEMATCH: keep the updated X in its scratch register. */
                    dx2 = px >> 16;
                    dy2 = (((u32)dy2 << 16) + small) >> 16;
                }
                affine = 0x300;
                if (gWork.u.r.counters[0].pos == 0x6C)
                    gWork.u.r.counters[0].count++;
                gWork.u.r.counters[0].pos += 2;
                break;
            case 2:
                skip = 1;
                break;
            }
            if ((s16)gWork.aff[0].scaleX > 0x100)
                pal = 6;
            break;
        case 1:
            switch (gWork.u.r.counters[i].count) {
            case 0:
                dy1 = dx2 = dx1 = 0;
                dy2 = 0;
                affine = 0;
                break;
            case 1:
                /* FAKEMATCH: narrow the word-sized r9 value to the original arc width. */
                dx2 = (u16)(-(sub_0807B4D0(0x400, SIN(0x30)) >> 4) + (sub_0807B4D0(0x400, SIN((gWork.u.r.counters[i].pos + 0x30) & 0xFF)) >> 4));
                dy2 = (sub_0807B4D0(0x4000, SIN(0x10)) >> 8) - (sub_0807B4D0(0x4000, SIN(gWork.u.r.counters[i].pos + 0x10)) >> 8);
                {
                    register u32 px asm("r1"); /* FAKEMATCH: packed X scratch register. */
                    register u32 big asm("r2"); /* FAKEMATCH: shared 32-pixel offset register. */
                    register u32 small asm("r3"); /* FAKEMATCH: shared 16-pixel offset register. */
                    u32 tmp;
                    px = (u32)dx2 << 16;
                    asm("" : "+r"(px)); /* FAKEMATCH: preserve the packed X register. */
                    big = 0x200000;
                    asm("" : "+r"(big)); /* FAKEMATCH: retain the shared large offset. */
                    tmp = px + big;
                    dx1 = tmp >> 16;
                    tmp = ((u32)dy2 << 16) + big;
                    dy1 = tmp >> 16;
                    small = 0x100000;
                    asm("" : "+r"(small)); /* FAKEMATCH: retain the shared small offset. */
                    px += small;
                    asm("" : "+r"(px)); /* FAKEMATCH: keep the updated X in its scratch register. */
                    dx2 = px >> 16;
                    dy2 = (((u32)dy2 << 16) + small) >> 16;
                }
                affine = 0x300;
                if (gWork.u.r.counters[i].pos == 0x84)
                    gWork.u.r.counters[i].count++;
                gWork.u.r.counters[i].pos += 2;
                break;
            case 2:
                skip = 1;
                break;
            }
            if ((s16)gWork.aff[i].scaleX > 0x100)
                pal = 6;
            break;
        case 2:
            switch (gWork.u.r.counters[i].count) {
            case 0:
                dy1 = dx2 = dx1 = 0;
                dy2 = 0;
                affine = 0;
                break;
            case 1:
                /* FAKEMATCH: narrow the word-sized r9 value to the original arc width. */
                dx2 = (u16)((sub_0807B4D0(0x200, SIN(((gWork.u.r.counters[i].pos + 0x30) * 2 & 0xFF) + 0x40)) >> 4) + 0x23 - (sub_0807B4D0(0x200, SIN(0x70)) >> 4));
                dy2 = (sub_0807B4D0(0x7000, SIN(0x10)) >> 8) - (sub_0807B4D0(0x7000, SIN(gWork.u.r.counters[i].pos + 0x10)) >> 8);
                {
                    register u32 px asm("r1"); /* FAKEMATCH: packed X scratch register. */
                    register u32 big asm("r2"); /* FAKEMATCH: shared 32-pixel offset register. */
                    register u32 small asm("r3"); /* FAKEMATCH: shared 16-pixel offset register. */
                    u32 tmp;
                    px = (u32)dx2 << 16;
                    asm("" : "+r"(px)); /* FAKEMATCH: preserve the packed X register. */
                    big = 0x200000;
                    asm("" : "+r"(big)); /* FAKEMATCH: retain the shared large offset. */
                    tmp = px + big;
                    dx1 = tmp >> 16;
                    tmp = ((u32)dy2 << 16) + big;
                    dy1 = tmp >> 16;
                    small = 0x100000;
                    asm("" : "+r"(small)); /* FAKEMATCH: retain the shared small offset. */
                    px += small;
                    asm("" : "+r"(px)); /* FAKEMATCH: keep the updated X in its scratch register. */
                    dx2 = px >> 16;
                    dy2 = (((u32)dy2 << 16) + small) >> 16;
                }
                affine = 0x300;
                if (gWork.u.r.counters[i].pos == 0x96)
                    gWork.u.r.counters[i].count++;
                gWork.u.r.counters[i].pos += 2;
                break;
            case 2:
                skip = 1;
                break;
            }
            if ((s16)gWork.aff[i].scaleX > 0x100)
                pal = 6;
            break;
        case 3:
            switch (gWork.u.r.counters[i].count) {
            case 0:
                dy1 = dx2 = dx1 = 0;
                dy2 = 0;
                affine = 0;
                break;
            case 1:
                /* FAKEMATCH: narrow the word-sized r9 value to the original arc width. */
                dx2 = (u16)((sub_0807B4D0(0xA00, SIN(0x30)) >> 4) - (sub_0807B4D0(0xA00, SIN((gWork.u.r.counters[i].pos + 0x30) & 0xFF)) >> 4));
                dy2 = (sub_0807B4D0(0xA000, SIN(0x10)) >> 8) - (sub_0807B4D0(0xA000, SIN(gWork.u.r.counters[i].pos + 0x10)) >> 8);
                {
                    register u32 px asm("r1"); /* FAKEMATCH: packed X scratch register. */
                    register u32 big asm("r2"); /* FAKEMATCH: shared 32-pixel offset register. */
                    register u32 small asm("r3"); /* FAKEMATCH: shared 16-pixel offset register. */
                    u32 tmp;
                    px = (u32)dx2 << 16;
                    asm("" : "+r"(px)); /* FAKEMATCH: preserve the packed X register. */
                    big = 0x200000;
                    asm("" : "+r"(big)); /* FAKEMATCH: retain the shared large offset. */
                    tmp = px + big;
                    dx1 = tmp >> 16;
                    tmp = ((u32)dy2 << 16) + big;
                    dy1 = tmp >> 16;
                    small = 0x100000;
                    asm("" : "+r"(small)); /* FAKEMATCH: retain the shared small offset. */
                    px += small;
                    asm("" : "+r"(px)); /* FAKEMATCH: keep the updated X in its scratch register. */
                    dx2 = px >> 16;
                    dy2 = (((u32)dy2 << 16) + small) >> 16;
                }
                affine = 0x300;
                if (gWork.u.r.counters[i].pos == 0x6D)
                    gWork.u.r.counters[i].count++;
                gWork.u.r.counters[i].pos += 1;
                break;
            case 2:
                skip = 1;
                break;
            }
            if ((s16)gWork.aff[i].scaleX > 0x100)
                pal = 6;
            break;
        case 4:
            switch (gWork.u.r.counters[i].count) {
            case 0:
                dy1 = dx2 = dx1 = 0;
                dy2 = 0;
                affine = 0;
                break;
            case 1:
                /* FAKEMATCH: narrow the word-sized r9 value to the original arc width. */
                dx2 = (u16)((sub_0807B4D0(0xA00, SIN(0x30)) >> 4) - (sub_0807B4D0(0xA00, SIN((gWork.u.r.counters[i].pos + 0x30) & 0xFF)) >> 4));
                /* ROM marker 4 shifts both Y helper results by 8. */
                dy2 = -(sub_0807B4D0(0x6000, SIN(0x10)) >> 8) + (sub_0807B4D0(0x6000, SIN(gWork.u.r.counters[i].pos + 0x10)) >> 8);
                {
                    register u32 px asm("r2"); /* FAKEMATCH: marker 4 uses r2 for packed X. */
                    register u32 big asm("r3"); /* FAKEMATCH: large offset register for marker 4. */
                    register u32 small asm("r1"); /* FAKEMATCH: small offset register for marker 4. */
                    u32 tmp;
                    px = (u32)dx2 << 16;
                    asm("" : "+r"(px)); /* FAKEMATCH: preserve the packed X register. */
                    big = 0x200000;
                    asm("" : "+r"(big)); /* FAKEMATCH: retain the shared large offset. */
                    tmp = px + big;
                    dx1 = tmp >> 16;
                    tmp = ((u32)dy2 << 16) + big;
                    dy1 = tmp >> 16;
                    small = 0x100000;
                    asm("" : "+r"(small)); /* FAKEMATCH: retain the shared small offset. */
                    px += small;
                    asm("" : "+r"(px)); /* FAKEMATCH: keep the updated X in its scratch register. */
                    dx2 = px >> 16;
                    dy2 = (((u32)dy2 << 16) + small) >> 16;
                }
                {
                    register u32 af asm("r2"); /* FAKEMATCH: preserve the final affine store's register. */
                    register u32 countpos asm("r3"); /* FAKEMATCH: preserve the final counter test's register. */
                    af = 0x300;
                    asm("" : : "r"(af)); /* FAKEMATCH: keep this tail distinct from the earlier markers. */
                    affine = af;
                    countpos = gWork.u.r.counters[i].pos;
                    asm("" : : "r"(countpos)); /* FAKEMATCH: keep the ROM's tail allocation and branch layout. */
                    if (countpos == 0x6E)
                        gWork.u.r.counters[i].count++;
                    gWork.u.r.counters[i].pos += 2;
                }
                break;
            case 2:
                skip = 1;
                break;
            }
            if ((s16)gWork.aff[i].scaleX > 0x100)
                pal = 6;
            break;
        }
        if (!skip) {
            /* This is gWork.grp[3]; the separate address keeps its base load. */
            extern struct SpriteGroup gUnk_02020C64;
            s16 sx = x;
            s16 sy = y;
            u16 *o = sub_08077EF4((const u16 *)((u8 *)gUnk_02020C64.templates + (i * 8 + 8)), pal, 1, sx - (s16)dx1, sy - (s16)dy1, 8, 1, 0, 0, 0, affine, &gWork);
            if (affine == 0x300)
                o[1] = (o[1] & 0xC1FF) | (i << 9);
            o = sub_08077EF4((const u16 *)((u8 *)gUnk_02020C64.templates + (i * 8 + 0x30)), pal, 1, sx - (s16)dx2, sy - (s16)dy2, 8, 1, 0, 0, 0, affine, &gWork);
            if (affine == 0x300)
                o[1] = (o[1] & 0xC1FF) | (i << 9);
        }
        skip = 0;
    }
}
