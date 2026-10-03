#include "global.h"
#include "gba.h"

/* Four-byte scroller entry (toss work area +0xB14, see code_0802408C). */
struct Scroller {
    u8 pos;             /* +0 */
    s8 speed;           /* +1 */
    u8 stop;            /* +2: index of the stop position reached */
    u8 unk3;
};

/* Reel/marker object (0x14 bytes), initialised by ScrollLayer_Init. */
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
    } aff[6];           /* +0x618: OBJ affine parameter sets (ObjAffineInit) */
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
    } grp[5];           /* +0x918: sprite groups (AnimBlockInit on [0]) */
    u8 filler97C[0xAAC - 0x97C];
    u8 unkAAC[8];       /* +0xAAC: object for FadeStart/FadeTick (+6 == 2: finished) */
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
extern struct Work20310 gSceneWork;
#define gWork gSceneWork

/* Message/sequence block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 unk0[0xB];
    u8 step;            /* +0xB */
};
extern struct DuelMsg gDuelScene;

extern const s8 gFinalLetterLaunchOrder[];    /* counter index per tick (-1 = none) */
extern const u8 gHandCarouselStops[];    /* 3 stop positions */
extern u16 (*const gDestinyBoardSceneSteps[])(void);
void PlaySE(u32 se);
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
extern struct IntrVectors IntrTable;
void DestinyBoardScene_VBlank(void);
void DestinyBoardScene_HBlank(void);
void ScrollLayer_StreamRow(struct Obj14 *obj);
void DestinyBoardScene_AdvanceWave(void);
void ScrollLayer_Move(struct Obj14 *obj);
void DestinyBoardScene_DrawFinalLetters(u32 a, u32 b, s32 y);
void FadeTick(void *p);
void AnimBlockTick(void *p);
void AnimBlockDraw(void *grp, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, s32 y, void *work);
void ObjAffineApply(void *aff);
void OamListFlush(void *p);
void DestinyBoardScene_LaunchLetters(void);
extern const u8 gFinalLetterTiles[];
void CopyMapRect(const void *src, void *dst, u32 w, u32 h);
void CopyTileSheetTo2D(const void *src, void *dst, u32 n);
void PlayBGM(u16 bgm);
extern const u8 gDestinyBoardBg2Map[], gDestinyBoardBg1Map[], gDestinyBoardWaveMap[];
extern const u8 gDestinyBoardBgTiles0[], gDestinyBoardBgTiles1[], gDestinyBoardBgTiles2[], gDestinyBoardObjTiles0[];
extern const u8 gDestinyBoardObjTiles1[], gDestinyBoardObjTiles2[], gDestinyBoardObjTiles3[], gDestinyBoardBgPal[], gDestinyBoardObjPal[];
extern struct Main gMain;
#define gMain gMain
struct ObjInit {
    u32 unk0;
    u32 unk4;
    s16 unk8;
    s16 unkA;
};
extern const struct ObjInit gDestinyBoardLayerInit[4];
extern const u8 gDestinyBoardAnimList[];
extern const u16 gHandCardTileNums[];
extern const u16 gTurnChoiceBannerTileNums[];
extern const s16 gSineTable[];   /* sine table */
extern const u8 gHandCardPalNums[];
extern const u16 gTurnOrderBannerTileNums[];
extern const u8 gTurnOrderBannerPalNums[];
void OamListClear(void *p);
void ObjAffineInit(void *p);
void AnimBlockInit(const void *a, void *b);
void ScrollLayer_Init(u32 a, u32 b, s16 c, s16 d, struct Obj14 *obj);
s32 MulFix8(s32 a, s32 b);     /* 8.8 fixed-point multiply */
u32 *OamListAddSprite(u32 a, u32 tile, s32 x, s32 y, u32 w, u32 h, u32 a6, u32 a7, u32 a8,
                  u32 a9, u32 a10, u32 a11, void *work);
void FadeStart(u32 a, u32 b, u32 c, void *p);


void DestinyBoardScene_LaunchLetters(void)
{
    if (--gWork.timer == 0xFF && gWork.tick <= 5) {
        if (gFinalLetterLaunchOrder[gWork.tick] != -1) {
            gWork.counters[gFinalLetterLaunchOrder[gWork.tick]].count++;
            PlaySE(0x2F);
        }
        gWork.timer = 20;
        gWork.tick++;
    }
    if (gWork.counters[2].pos == 0x60) {
        FadeStart(0, 0x60, 0, gWork.unkAAC);
        REG_DISPCNT &= ~0x100;
    }
} /* 0x08027580 size 0xA0 */

void DestinyBoardScene_ResetLetters(void)
{
    u8 i;

    gWork.timer = 20;
    for (i = 0; i < 5; i++) {
        gWork.counters[i].pos = 0;
        gWork.counters[i].count = 0;
    }
    gWork.tick = 0;
}
u32 DestinyBoardScene_Init(void)
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
    OamListClear(&gWork);
    AnimBlockInit(gDestinyBoardAnimList, &gWork.grp[0]);
    ObjAffineInit(gWork.aff);
    for (i = 0; i < 4; i++)
        ScrollLayer_Init(gDestinyBoardLayerInit[i].unk0, gDestinyBoardLayerInit[i].unk4, gDestinyBoardLayerInit[i].unk8,
                     gDestinyBoardLayerInit[i].unkA, &gWork.objs[i]);
    gWork.unkB05 = 0x62;
    return 1;
}
u32 DestinyBoardScene_Load(void)
{
    vu32 zero;
    u8 i, j;

    FadeStart(0, -0x180, 0, gWork.unkAAC);
    zero = 0;
    CpuFastSet((void *)&zero, (void *)0x06000000, 0x01006000);
    CopyMapRect(gDestinyBoardBg2Map, (void *)0x0600E000, 30, 20);
    CopyMapRect(gDestinyBoardBg1Map, (void *)0x0600D000, 30, 20);
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            CopyMapRect(gDestinyBoardWaveMap, (u16 *)0x0600C000 + (i * 256 + j * 16), 16, 8);
            CopyMapRect(gDestinyBoardWaveMap, (u16 *)0x0600F000 + (i * 256 + j * 16), 16, 8);
        }
    }
    CpuFastSet(gDestinyBoardBgTiles0, (void *)0x06000000, 0x800);
    CpuFastSet(gDestinyBoardBgTiles1, (void *)0x06002000, 0x800);
    CpuFastSet(gDestinyBoardBgTiles2, (void *)0x06004000, 0x800);
    CopyTileSheetTo2D(gDestinyBoardObjTiles0, (void *)0x06010000, 0x10);
    CopyTileSheetTo2D(gDestinyBoardObjTiles1, (void *)0x06010200, 0x10);
    CopyTileSheetTo2D(gDestinyBoardObjTiles2, (void *)0x06014000, 0x10);
    CopyTileSheetTo2D(gDestinyBoardObjTiles3, (void *)0x06014200, 0x10);
    CpuFastSet(gDestinyBoardBgPal, (void *)0x05000000, 0x80);
    CpuFastSet(gDestinyBoardObjPal, (void *)0x05000200, 0x80);
    REG_BG0CNT = 0x1800;
    REG_BG1CNT = 0x1A01;
    REG_BG2CNT = 0x1C02;
    REG_BG3CNT = 0x1E03;
    REG_DISPCNT = 0x1F00;
    gMain.vblankCallback = DestinyBoardScene_VBlank;
    for (i = 1; i < 5; i++)
        gWork.grp[i].u.f.ctl |= 0xFF;
    gWork.grp[0].u.f.ctl = 1;
    gWork.grp[2].u.f.ctl = 1;
    ScrollLayer_StreamRow(&gWork.objs[1]);
    ScrollLayer_StreamRow(&gWork.objs[2]);
    REG_IME = 0;
    REG_IE &= ~2;
    IntrTable.hblankCallback = DestinyBoardScene_HBlank;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= 2;
    REG_IME = 1;
    REG_BLDCNT = 0x3F41;
    REG_BLDALPHA = 0x100B;
    PlayBGM(20);
    return 1;
}
u32 DestinyBoardScene_Update(void)
{
    u8 i;

    FadeTick(gWork.unkAAC);
    DestinyBoardScene_AdvanceWave();
    if (gWork.unkB05 == 1) {
        gWork.objs[1].state = 1;
        gWork.objs[2].state = 1;
        gWork.objs[3].state = 1;
    }
    if (gWork.unkB05)
        gWork.unkB05--;
    for (i = 0; i < 4; i++)
        ScrollLayer_Move(&gWork.objs[i]);
    if (gWork.objs[0].state == 0 && gWork.objs[1].state == 0 && gWork.unkB05 == 0)
        gWork.objs[3].unk2 = -2;
    ScrollLayer_StreamRow(&gWork.objs[1]);
    ScrollLayer_StreamRow(&gWork.objs[2]);
    AnimBlockTick(&gWork.grp[0]);
    switch (gWork.unkB04) {
    case 0:
        if ((s8)gWork.grp[0].u.f.ctl == 0) {
            gWork.grp[0].u.f.ctl = 0xFF;
            gWork.grp[1].u.f.ctl = 1;
            gWork.unkB04++;
        }
        if ((gWork.grp[0].u.frame & 0xFF00FF00) == 0xA00)
            CopyTileSheetTo2D(gFinalLetterTiles, (void *)0x06010000, 0x10);
        if ((gWork.grp[0].u.frame & 0xFF00FF00) == 0x100)
            PlaySE(0x2D);
        if ((gWork.grp[0].u.frame & 0xFF00FF00) == 0xB00)
            PlaySE(0x2E);
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
        AnimBlockDraw(&gWork.grp[0], 0, 1, 0, 0, 0, 8, 0, 0x48 - (gWork.objs[2].pos >> 4), &gWork);
        break;
    case 3:
        DestinyBoardScene_LaunchLetters();
        gWork.grp[3].u.f.ctl = 1;
        DestinyBoardScene_DrawFinalLetters(3, 0, 0x48 - (gWork.objs[1].pos >> 4));
        break;
    }
    for (i = 0; i < 5; i++)
        ObjAffineApply(&gWork.aff[i]);
    OamListFlush(&gWork);
    OamListClear(&gWork);
    if (gWork.unkAAC[6] == 2)
        return 1;
    return 0;
}
u32 DestinyBoardScene_DisableHBlank(void)
{
    REG_IME = 0;
    REG_IE &= ~2;
    REG_IME = 1;
    return 1;
}

u32 DestinyBoardScene_Run(void)
{
    if (gDestinyBoardSceneSteps[gDuelScene.step]) {
        if (gDestinyBoardSceneSteps[gDuelScene.step]())
            gDuelScene.step++;
        return 0;
    }
    return 1;
}

void TurnOrder_ShowWaitSign(void)
{
    gWork.grp[0].u.f.ctl = 1;
}

void TurnOrder_HideWaitSign(void)
{
    gWork.grp[0].u.f.ctl = 0xFF;
}

void Scroller_Move(struct Scroller *s)
{
    u8 i;

    for (i = 0; i < 4; i++)
        s[i].pos += s[i].speed;
}

void Scroller_SnapToStop(struct Scroller *s)
{
    u8 i = 0;
    s32 pos = s->pos;

    for (; i < 3; i++) {
        if (pos >= gHandCarouselStops[i]) {
            s32 v = s->speed;
            if (v < 0)
                v = -v;
            if (pos < gHandCarouselStops[i] + v) {
                s->speed = 0;
                s->pos = gHandCarouselStops[i];
                s->stop = i;
            }
        }
    }
}

void Scroller_StopAtEnds(struct Scroller *s)
{
    if (s->pos == 0x30)
        s->speed = 0;
    if (s->pos == 0)
        s->speed = 0;
}
void TurnOrder_DrawHandCarousel(u16 *tiles, u8 *pals, u8 angle, u8 sel, u8 bump, u16 flags, u8 spread)
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
    oam = OamListAddSprite(prio, tiles[2], (MulFix8(gSineTable[(angle + spread) % 256], 0x3000) >> 8) + 0x58,
                       (y = MulFix8(gSineTable[(angle + spread) % 256 + 0x40], 0x1000), y >>= 8, y += 0x32, y += dy[2]),
                       width, height, 4, pals[2], 0x200, 0, 0, 0, &gWork);
    attr = *oam;
    *oam = attr | (front ? 0x02000700 : 0x02000300);
    gWork.aff[1].angle = 0x8000;
    c = gSineTable[(angle + spread) % 256 + 0x40];
    gWork.aff[1].scaleX = MulFix8(0x40, c) + 0xC0;
    gWork.aff[1].scaleY = MulFix8(0x40, c) + 0xC0;
    if (((angle + back) % 256 >= 0x40 && (angle + back) % 256 < 0xC0) || (flags & 1)) {
        prio = 1;
        front = 1;
    } else {
        prio = 0;
        front = 0;
    }
    oam = OamListAddSprite(prio, tiles[1], (MulFix8(gSineTable[(angle + back) % 256], 0x3000) >> 8) + 0x58,
                       (y = MulFix8(gSineTable[(angle + back) % 256 + 0x40], 0x1000), y >>= 8, y += 0x32, y += dy[1]),
                       width, height, 4, pals[1], 0x200, 0, 0, 0, &gWork);
    attr = *oam;
    *oam = attr | (front ? 0x04000700 : 0x04000300);
    gWork.aff[2].angle = 0x8000;
    c = gSineTable[(angle + back) % 256 + 0x40];
    gWork.aff[2].scaleX = MulFix8(0x40, c) + 0xC0;
    gWork.aff[2].scaleY = MulFix8(0x40, c) + 0xC0;
    if ((angle % 256 >= 0x40 && angle % 256 < 0xC0) || (flags & 1)) {
        prio = 1;
        front = 1;
    } else {
        prio = 0;
        front = 0;
    }
    oam = OamListAddSprite(prio, tiles[0], (MulFix8(gSineTable[angle % 256], 0x3000) >> 8) + 0x58,
                       (y = MulFix8(gSineTable[angle % 256 + 0x40], 0x1000), y >>= 8, y += 0x32, y += dy[0]),
                       width, height, 4, pals[0], 0x200, 0, 0, 0, &gWork);
    attr = *oam;
    *oam = attr | (front ? 0x700 : 0x300);
    gWork.aff[0].angle = 0x8000;
    c = gSineTable[angle % 256 + 0x40];
    gWork.aff[0].scaleX = MulFix8(0x40, c) + 0xC0;
    gWork.aff[0].scaleY = MulFix8(0x40, c) + 0xC0;
}

void TurnOrder_DrawBanner(u8 idx, u16 flags)
{
    /* Constants held in locals declared outside the loop: CSE cannot see them inside the
       loop, so reload rematerializes them (x0 in r7, y0 into r3), and the round-robin reload
       register choice depends on w/h/pal being variables. */
    s32 w = 0x40;
    s32 h = 0x20;
    u8 i;
    const u16 *tbl;
    s32 x0 = 0x38;
    s32 y0 = 0x6E;
    s32 pal = 4;
    u16 hflip;

    i = 0;
    tbl = gTurnOrderBannerTileNums;
    hflip = flags & 4;
    for (; i < 2; i++) {
        u16 tile = tbl[idx * 2 + i];
        s32 x, y;
        u32 *oam;
        u32 attr;
        x = x0 + i * w;
        y = y0;
        if (idx == 0)
            y -= 0x42;
        oam = OamListAddSprite(0, tile, x, y, w, h, pal, gTurnOrderBannerPalNums[idx], 0x200, 0, 0, 0, &gWork);
        attr = *oam;
        if (hflip)
            *oam = attr | 0x400;
    }
}

void TurnOrder_DrawOpponentCard(u8 idx, u8 t, u16 flags)
{
    s32 w = 0x20;
    s32 h = 0x40;
    u32 *oam = OamListAddSprite(0, gHandCardTileNums[idx], 0x69, (MulFix8(t << 8, 0x160) >> 8) + 0xC0,
                            w, h, 4, gHandCardPalNums[idx], 0x200, 0, 0, 0, &gWork);
    *oam |= (flags & 2) ? 0x06000400 : 0x06000000;
    gWork.aff[3].angle = 0;
    gWork.aff[3].scaleX = 0x100;
    gWork.aff[3].scaleY = 0x100;
}

void TurnOrder_DrawTurnChoice(u32 unused, u8 angle, u16 flags, s16 *scale, u8 sel)
{
    s32 w = 0x40;
    s32 h = 0x20;
    u32 *oam;
    u32 attr;

    oam = OamListAddSprite(0, gTurnChoiceBannerTileNums[0], 0x28,
                       (MulFix8(gSineTable[(angle * 2) % 256], scale[0]) >> 8) + 0x30,
                       w, h, 4, sel == 0 ? 3 : 9, 0x200, 0, 0, 0, &gWork);
    attr = *oam;
    *oam = attr | ((flags & 8) ? 0x08000400 : 0x08000000);
    gWork.aff[4].angle = 0;
    gWork.aff[4].scaleX = 0x100;
    gWork.aff[4].scaleY = 0x100;
    {
        s32 y = (MulFix8(gSineTable[(angle * 2) % 256], scale[2]) >> 8) + 0x30;
        u16 tile = gTurnChoiceBannerTileNums[1];
        s32 x = 0x88;

        /* FAKEMATCH: keep the initialized x in r2 after the tile load,
         * before preparing the stack arguments, as in the ROM. */
        __asm__("" : : "r"(x));
        oam = OamListAddSprite(0, tile, x, y, w, h, 4, sel == 1 ? 3 : 9, 0x200, 0, 0, 0, &gWork);
    }
    attr = *oam;
    *oam = attr | ((flags & 8) ? 0x0A000400 : 0x0A000000);
    gWork.aff[5].angle = 0;
    gWork.aff[5].scaleX = 0x100;
    gWork.aff[5].scaleY = 0x100;
}
void TurnOrder_DrawTurnChoiceConfirm(u32 unused, u8 angle, u16 flags, s16 *scale, u8 sel, s16 *anim)
{
    s32 width = 0x40, height = 0x20;
    s32 x0 = 0x78;
    s32 y0 = 0x30;
    /* Separate x per case (case 0 x in r7, case 1 x in r5) and a separate offset t:
       x = x0 - (t) must not fold into (x0 + 0x10) - (...). */
    s32 x, x1, t;
    u8 i;
    u32 *oam;

    for (i = 0; i < 2; i++) {
        if (scale[i * 2] > 0x7F)
            scale[i * 2] -= 0x80;
        else
            scale[i * 2] = 0;
    }
    switch (sel) {
    case 0:
        t = (MulFix8(0x100 - gSineTable[anim[0] + 0x40], 0x3000) >> 8) - 0x50;
        x = x0 + t;
        oam = OamListAddSprite(0, gTurnChoiceBannerTileNums[0], x,
                           y0 + (MulFix8(gSineTable[(angle * 2) % 256], scale[0]) >> 8),
                           width, height, 4, 3, 0x200, 0, 0, 0, &gWork);
        *oam |= ((flags & 8) ? 0x08000400 : 0x08000000);
        oam = OamListAddSprite(0, gTurnChoiceBannerTileNums[1], x0 - 0x10, ((anim[1] * anim[1]) >> 1) + y0,
                           width, height, 4, 9, 0x200, 0, 0, 0, &gWork);
        *oam |= ((flags & 8) ? 0x08000700 : 0x08000300);
        gWork.aff[4].angle = ((u32)(u16)anim[1]) << 9;
        break;
    case 1:
        oam = OamListAddSprite(0, gTurnChoiceBannerTileNums[0], x0 - 0x70, ((anim[1] * anim[1]) >> 1) + y0,
                           width, height, 4, 9, 0x200, 0, 0, 0, &gWork);
        *oam |= ((flags & 8) ? 0x08000700 : 0x08000300);
        t = (MulFix8(0x100 - gSineTable[anim[0] + 0x40], 0x3000) >> 8) - 0x10;
        x1 = x0 - t;
        oam = OamListAddSprite(0, gTurnChoiceBannerTileNums[1], x1,
                           (MulFix8(gSineTable[(angle * 2) % 256], scale[2]) >> 8) + y0,
                           width, height, 4, 3, 0x200, 0, 0, 0, &gWork);
        *oam |= ((flags & 8) ? 0x08000400 : 0x08000000);
        gWork.aff[4].angle = -(((u32)(u16)anim[1]) << 9);
        break;
    }
    gWork.aff[4].scaleX = 0x100;
    gWork.aff[4].scaleY = 0x100;
}

