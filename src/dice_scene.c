#include "global.h"
#include "gba.h"

typedef u16 (*StepFunc)(void);

/* Scrolling reel object (0x14 bytes), see also destiny_board_scene. */
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

/* Sprite group (0x14 bytes, AnimBlockInit / AnimStateTick / OamListAddSpriteGroup). */
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

/* Moving object (0x14 bytes, LineStep). */
struct Mover {
    u16 x;
    u16 y;
    u8 unk4[0x10];
};

/* Work area at 0x02020310 (0x1738 bytes). It is shared by several screens,
 * so the fields below overlap: reels[] (ScrollLayer_Move..) vs movers[] (ExodiaScene_GatherPieces). */
struct Work20310 {
    u8 oam[0x618];          /* +0x000: OAM buffer (OamListFlush / OamListClear) */
    struct {
        u16 scaleX;         /* +0x0 (0x100 = 1.0) */
        u16 scaleY;         /* +0x2 */
        u16 angle;          /* +0x4 */
        u8 unk6[0x12];
    } aff[0x20];            /* +0x618: OBJ affine sets (ObjAffineInit) */
    struct SpriteGroup grp[5];  /* +0x918 */
    u8 filler97C[0xAA8 - 0x97C];
    u16 unkAA8;             /* +0xAA8 */
    u8 fillerAAA[2];
    u8 unkAAC[4];           /* +0xAAC */
    union {
        struct {                    /* slot-reel screen (destiny_board_scene) */
            u8 pad[4];
            struct Reel reels[4];   /* +0xAB4 */
            u8 unkB04;
            u8 unkB05;
            u8 unkB06;
            u8 unkB07;
            u8 fillerB08[4];
            struct {
                u8 pos;             /* +0 */
                u8 count;           /* +1: step of the DestinyBoardScene_DrawFinalLetters animation */
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
            } fade;                 /* +0xB18: object for FadeStart/FadeTick */
        } m;
    } u;
    struct Timer timer;     /* +0xB20 */
    u8 unkB24;              /* +0xB24: wave phase (HBlank ExodiaScene_HBlank) */
    u8 unkB25;
    u8 fillerB26[2];
    u8 ramp[0x1728 - 0xB28];    /* +0xB28: palette ramp (PalFade_Start / PalFade_Apply) */
    u8 unk1728;
    u8 filler1729[0x1734 - 0x1729];
    u8 unk1734;
    u8 unk1735;
    u8 filler1736[2];
};
extern struct Work20310 gSceneWork;
#define gWork gSceneWork

/* Message/sequence block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 unk0[0xB];
    u8 step;            /* +0xB */
};
extern struct DuelMsg gDuelScene;

extern const StepFunc gDiceScreenGracefulSteps[];
extern const StepFunc gDiceScreenPlainSteps[];
extern const StepFunc gExodiaSceneSteps[];
extern const s16 gSineTable[];   /* sine table */

void FadeTick(void *p);
void FadeStart(u32 a, s32 b, u32 c, void *p);
void Timer_Tick(struct Timer *t);
void CopyMapRect(const void *src, void *dst, u32 w, u32 h);
void MemCopy16(void *dest, const void *src, u32 size);
u16 *OamListAlloc(u32 a, void *b);
void ExodiaScene_DrawSprites(void);

struct Main {
    u32 rngState;
    u16 heldKeys;           /* +0x4 */
    u16 newKeys;            /* +0x6 */
    u8 filler8[0x40E - 0x8];
    u16 unk40E;             /* +0x40E */
    u8 filler410[4];
    void (*vblankCallback)(void);   /* +0x414 */
};
extern struct Main gMain;
#define gMain gMain

struct IntrVectors {
    u32 unk0;
    void (*hblankCallback)(void);   /* +0x4 */
};
extern struct IntrVectors IntrTable;

s32 MulFix8(s32 a, s32 b);     /* 8.8 fixed-point multiply */
void PalFade_Apply(void *ramp);
void PalFade_Start(void *pal, u32 a, u32 b, void *ramp);
void Timer_Reset(struct Timer *t);
void Timer_Start(struct Timer *t, u16 time);
u16 AnimBlockInit(const void *anim, struct SpriteGroup *grp);
void AnimStateTick(struct SpriteGroup *grp);
void CopyTileSheetTo2D(const void *src, void *dst, u32 n);
void MemClear16(void *dst, u32 size);
void OamListFlush(void *p);
void OamListClear(void *p);
void ObjAffineInit(void *p);
void PlaySE(u16 se);          /* PlaySE */
void ExodiaScene_ResetPieceState(void *p);
void LoadObjTileBlock4x4(const u8 *src, u32 tile, u32 srcTile, u32 size);
void ExodiaScene_DrawAffineAnim(struct SpriteGroup *grp, u8 prio, void *work);

extern const u8 gExodiaPiecesAnimList[];
extern const u8 gMillenniumEyeBitmap[], gMillenniumEyePal[], gExodiaPiecesObjPal[], gExodiaPiecesObjTiles[];
extern const u8 gUnk_086CED78[], gUnk_086CF778[];
struct Tmpl8 { u8 b[8]; };
extern const struct Tmpl8 gExodiaPieceOamTemplates[];
u16 *OamListAddSpriteGroup(const void *templates, u32 a1, u32 a2, s32 x, s32 y, u32 a5, u32 a6, u32 a7,
                         u32 a8, u32 a9, u32 a10, void *work);
void LineStep(struct Mover *m);
void ObjAffineApply(void *aff);
extern const u8 gExodiaFlameBgTiles0[], gExodiaFlameBgTiles1[], gExodiaFlameBgTiles2[], gExodiaFlameBgTiles3[];
extern const u8 gExodiaFlameBgPal[], gUnk_086CAD78[], gExodiaFlameObjTilesB[];
extern const u8 gExodiaFlameBg2Map[], gExodiaFlameBg1Map[], gExodiaFlameBg0Map[];
extern const u8 gExodiaFlameAnimList[];
/* Two literal-pool words that are 0 in the ROM (hypothesis: link-time symbols at address 0). */
extern u8 gUnkA_00000000[], gUnkB_00000000[];
void CopyMapBlock(const void *src, void *a1, void *a2, u32 w, void *dst, u32 a5, u32 a6, u32 a7,
                  u32 h, u32 a9);
void ExodiaScene_HBlank(void);
void ExodiaScene_StartPieceFlight(struct Mover *m);

u16 DiceScreen_RunGraceful(void)
{
    StepFunc step = gDiceScreenGracefulSteps[gDuelScene.step];
    if (step != NULL) {
        if (step())
            gDuelScene.step++;
        return 0;
    }
    return 1;
}

u16 DiceScreen_RunPlain(void)
{
    StepFunc step = gDiceScreenPlainSteps[gDuelScene.step];
    if (step != NULL) {
        if (step())
            gDuelScene.step++;
        return 0;
    }
    return 1;
}

void ExodiaScene_ResetPieceState(void *p)
{
    gWork.unkAAC[0] = 0;
}

extern const struct { s16 unk0; s16 unk2; } gExodiaPieceStartPos[];
void LineInit(s32 a, s32 b, u32 c, u32 d, struct Mover *m);

/* Start the five movers towards (0x68, 0x40) from the positions in gExodiaPieceStartPos. */
void ExodiaScene_StartPieceFlight(struct Mover *m)
{
    u8 i;
    for (i = 0; i < 5; i++)
        LineInit(gExodiaPieceStartPos[i].unk2, gExodiaPieceStartPos[i].unk0, 0x68, 0x40, m++);
}

/* HBlank: BG0HOFS wave from the sine table. */
void ExodiaScene_HBlank(void)
{
    REG_BG0HOFS = gSineTable[(REG_VCOUNT + gWork.unkB24) & 0xFF] >> 5;
}

/* Draw a sprite group as affine OBJs with the given priority (tiles from 0x200). */
void ExodiaScene_DrawAffineAnim(struct SpriteGroup *grp, u8 prio, void *work)
{
    const u16 *e = grp->templates;
    u8 i;
    for (i = 0; i < grp->count; i++) {
        u16 *o = OamListAlloc(0, work);
        o[0] = ((e[0] | 0x100) & 0xFF00) | (e[0] & 0xFF);
        o[1] = e[1] & 0xC1FF;
        o[2] = (((e[2] & 0xFF0F) | ((e[2] & 0xF0) << 1)) & 0xF3FF) | (((prio << 10) & 0xC00) + 0x200);
        e += 4;
    }
}

u16 ExodiaScene_Init(void)
{
    MemClear16(&gWork, sizeof(gWork));
    gMain.unk40E = 1;
    REG_DISPCNT &= 0xE0FF;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    ExodiaScene_ResetPieceState(gWork.unkAAC);
    gWork.u.m.unkB15 = 0;
    gWork.u.m.unkB16 = 0;
    gWork.u.m.unkB14 = 0;
    OamListClear(&gWork);
    ObjAffineInit(gWork.aff);
    REG_BLDCNT = 0x3F3F;
    REG_BLDALPHA = 0x808;
    REG_BLDY = 0x10;
    gWork.unk1734 = 0;
    gWork.unk1735 = 0;
    return 1;
}

/* Copy 4 rows of 0x80 bytes into OBJ VRAM (row stride 0x400). */
void CopyObjTileBlock4x4(const u8 *src, u8 *dst)
{
    int i;
    for (i = 0; i < 4; i++) {
        MemCopy16(dst, src, 0x80);
        dst += 0x400;
        src += 0x80;
    }
}

void LoadObjTileBlock4x4(const u8 *src, u32 tile, u32 srcTile, u32 size)
{
    CopyObjTileBlock4x4(src + srcTile * 32, (u8 *)0x06014000 + tile * 32);
}

u16 ExodiaScene_LoadEye(void)
{
    FadeStart(0, -0x80, 0, &gWork.u.m.fade);
    gWork.unkAA8 = AnimBlockInit(gExodiaPiecesAnimList, &gWork.grp[0]);
    CpuFastSet(gMillenniumEyeBitmap, (void *)VRAM, 0x2580);
    CpuFastSet(gMillenniumEyePal, (void *)PLTT, 0x80);
    CpuSet(gExodiaPiecesObjPal, (void *)(PLTT + 0x200), 0x100);
    CopyTileSheetTo2D(gExodiaPiecesObjTiles, (void *)0x06014000, 0x10);
    LoadObjTileBlock4x4(gUnk_086CED78, 4, 0x10, 0x40);
    LoadObjTileBlock4x4(gUnk_086CED78, 8, 0x20, 0x40);
    LoadObjTileBlock4x4(gUnk_086CED78, 0xC, 0x30, 0x40);
    LoadObjTileBlock4x4(gUnk_086CED78, 0x80, 0x40, 0x40);
    LoadObjTileBlock4x4(gUnk_086CF778, 0x88, 0x10, 0x40);
    LoadObjTileBlock4x4(gUnk_086CF778, 0x8C, 0x20, 0x40);
    LoadObjTileBlock4x4(gUnk_086CF778, 0x100, 0x30, 0x40);
    LoadObjTileBlock4x4(gUnk_086CF778, 0x104, 0x40, 0x40);
    REG_DISPCNT = 0x1F04;
    Timer_Reset(&gWork.timer);
    Timer_Start(&gWork.timer, 0x3C);
    return 1;
}

u16 ExodiaScene_LoadFlames(void)
{
    CpuFastSet(gExodiaFlameBgTiles0, (void *)VRAM, 0x800);
    CpuFastSet(gExodiaFlameBgTiles1, (void *)(VRAM + 0x2000), 0x800);
    CpuFastSet(gExodiaFlameBgTiles2, (void *)(VRAM + 0x4000), 0x800);
    CpuFastSet(gExodiaFlameBgTiles3, (void *)(VRAM + 0x6000), 0x800);
    CpuFastSet(gExodiaFlameBgPal, (void *)PLTT, 0x80);
    CopyTileSheetTo2D(gUnk_086CAD78, (void *)0x06010000, 0x10);
    CopyTileSheetTo2D(gExodiaFlameObjTilesB, (void *)0x06014000, 0x10);
    CopyMapRect(gExodiaFlameBg2Map, (void *)(VRAM + 0xE000), 0x1E, 0x14);
    CopyMapRect(gExodiaFlameBg1Map, (void *)(VRAM + 0xD000), 0x1E, 0x14);
    CopyMapRect(gExodiaFlameBg0Map, (void *)(VRAM + 0xC000), 0x1E, 0x14);
    CopyMapBlock(gExodiaFlameBg0Map, gUnkA_00000000, gUnkB_00000000, 0x1E, (void *)(VRAM + 0xC03C), 0, 0, 2, 0x14, 0);
    FadeStart(1, -0x80, 0, &gWork.u.m.fade);
    Timer_Start(&gWork.timer, 0x96);
    AnimBlockInit(gExodiaFlameAnimList, &gWork.grp[0]);
    gWork.grp[0].unk10 = 1;
    REG_BG0CNT = 0x1800;
    REG_BG1CNT = 0x1A01;
    REG_BG2CNT = 0x1C02;
    REG_DISPCNT = 0x1700;
    PalFade_Start((void *)PLTT, 0x200, 0x1F, gWork.ramp);
    ObjAffineInit(gWork.aff);
    gWork.u.m.unkB16 = 0;
    REG_IME = 0;
    REG_IE &= ~2;
    IntrTable.hblankCallback = ExodiaScene_HBlank;
    REG_IME = 1;
    gWork.unkB24 = 0;
    gWork.unkB25 = 0;
    REG_IME = 0;
    REG_IE |= 2;
    REG_IME = 1;
    return 1;
}

u16 ExodiaScene_AssemblePieces(void)
{
    struct SpriteGroup *g = gWork.grp;
    u8 i;

    FadeTick(&gWork.u.m.fade);
    switch (gWork.unkAAC[0]) {
    case 0:
        if (g->unkD == 5) {
            g->unkE = 0xFF;
            FadeStart(1, 0xA0, 0, &gWork.u.m.fade);
            REG_BLDCNT = 0x90;
            gWork.unkAAC[0]++;
            PlaySE(0x13);
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
        ExodiaScene_StartPieceFlight(gWork.u.m.movers);
        for (i = 0; i < gWork.unkAA8; i++)
            OamListAddSpriteGroup(g->templates, g->unk10, g->count, g->x, g->y, 0, 0, 0, 1, 0, 0, &gWork);
        OamListFlush(&gWork);
        OamListClear(&gWork);
        FadeStart(1, 0x20, 0, &gWork.u.m.fade);
        gWork.timer.state = 0;
        gWork.u.m.unkB16 = 0;
        PlaySE(0x14);
        return 1;
    }
    gWork.unk1735 = gWork.grp[0].unkD;
    if (gWork.unkAAC[0] == 0 && gWork.unk1735 != gWork.unk1734) {
        gWork.unk1734 = gWork.unk1735;
        PlaySE(0x13);
    }
    for (i = 0; i < gWork.unkAA8; i++)
        AnimStateTick(&gWork.grp[i]);
    for (i = 0; i < gWork.unkAA8; i++) {
        OamListAddSpriteGroup(g->templates, g->unk10, g->count, g->x, g->y, 0, 0, 0, 1, 0, 0, &gWork);
        g++;
    }
    OamListFlush(&gWork);
    OamListClear(&gWork);
    Timer_Tick(&gWork.timer);
    return 0;
}

u16 ExodiaScene_GatherPieces(void)
{
    u8 i;
    const struct Tmpl8 *tmpl;

    FadeTick(&gWork.u.m.fade);
    if (gWork.u.m.unkB16 < 0x40)
        gWork.u.m.unkB16++;
    gWork.u.m.fade.speed = MulFix8(0x40, 0x100 - gSineTable[gWork.u.m.unkB16 + 0x40]);
    if (gWork.u.m.fade.state == 2) {
        gWork.u.m.fade.state = 0;
        Timer_Start(&gWork.timer, 1);
    }
    if (gWork.timer.state == 2) {
        PlaySE(0x15);
        return 1;
    }
    tmpl = gExodiaPieceOamTemplates;
    for (i = 0; i < 5; i++) {
        LineStep(&gWork.u.m.movers[i]);
        gWork.grp[i].x = gWork.u.m.movers[i].x;
        gWork.grp[i].y = gWork.u.m.movers[i].y;
        OamListAddSpriteGroup(tmpl++, 0, 1, gWork.u.m.movers[i].x, gWork.u.m.movers[i].y, 1, 0, 0, 1, 0, 0, &gWork);
    }
    OamListFlush(&gWork);
    OamListClear(&gWork);
    Timer_Tick(&gWork.timer);
    return 0;
}

/* Per-frame sprite update: draw both groups, animate the wave and the zoom. */
void ExodiaScene_DrawSprites(void)
{
    struct SpriteGroup *g = &gWork.grp[0];
    OamListAddSpriteGroup(g->templates, 1, g->count, g->x, g->y, 0, 2, 0, 0, 0, 0, &gWork);
    ExodiaScene_DrawAffineAnim(&gWork.grp[1], 1, &gWork);
    AnimStateTick(&gWork.grp[0]);
    AnimStateTick(&gWork.grp[1]);
    if (gWork.grp[0].unkD == 1)
        gWork.grp[0].unkE = 0xFF;
    gWork.grp[1].unkE = 1;
    gWork.unkB25++;
    if (gWork.unkB25 & 2) {
        gWork.unkB24++;
        gWork.unkB25 = 0;
    }
    gWork.aff[0].scaleX = gWork.aff[0].scaleY = MulFix8(0x90, 0x100 - gSineTable[gWork.u.m.unkB16 + 0x40]) + 0x80;
    gWork.u.m.unkB16++;
    ObjAffineApply(gWork.aff);
    OamListFlush(&gWork);
    OamListClear(&gWork);
}

u16 ExodiaScene_Finale(void)
{
    FadeTick(&gWork.u.m.fade);
    if (gWork.u.m.fade.state == 2) {
        gWork.u.m.fade.state = 0;
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        return 1;
    }
    if (gWork.timer.state == 2) {
        FadeStart(0, 0x100, 0, &gWork.u.m.fade);
        gWork.timer.state = 0;
        gWork.grp[1].unkE = 0xFF;
    }
    ExodiaScene_DrawSprites();
    if (gWork.u.m.unkB16 >= 0x50) {
        PalFade_Apply(gWork.ramp);
        gWork.unk1728 = MulFix8(0x1F00, 0x100 - gSineTable[gWork.u.m.unkB16 - 0x10]) >> 8;
    }
    Timer_Tick(&gWork.timer);
    if (gMain.newKeys & 2) {
        REG_IME = 0;
        REG_IE &= ~2;
        REG_IME = 1;
        return 1;
    }
    return 0;
}

u16 ExodiaScene_FadeInEye(void)
{
    FadeTick(&gWork.u.m.fade);
    if (gWork.timer.state == 2)
        return 1;
    Timer_Tick(&gWork.timer);
    return 0;
}

u16 ExodiaScene_FadeInFlames(void)
{
    ExodiaScene_DrawSprites();
    if (gWork.u.m.fade.state == 3) {
        gWork.u.m.fade.state = 0;
        FadeStart(1, -0x40, 0, &gWork.u.m.fade);
        REG_BLDCNT = 0x3F41;
        REG_BLDALPHA = 0x80D;
        return 1;
    }
    FadeTick(&gWork.u.m.fade);
    return 0;
}

u16 ExodiaScene_BlendFlames(void)
{
    u32 blend;
    ExodiaScene_DrawSprites();
    REG_BLDALPHA = ((blend = gWork.u.m.fade.blend << 16) >> 24) | 0x800;
    if (blend <= 0xA000000) {
        gWork.u.m.fade.state = 0;
        return 1;
    }
    FadeTick(&gWork.u.m.fade);
    return 0;
}

u16 ExodiaScene_Run(void)
{
    StepFunc step = gExodiaSceneSteps[gDuelScene.step];
    if (step != NULL) {
        if (step())
            gDuelScene.step++;
        return 0;
    }
    return 1;
}

extern const s8 gDestinyBoardWaveTable[];

/* HBlank: BG0/BG3 HOFS wave (64-entry table). */
void DestinyBoardScene_HBlank(void)
{
    REG_BG0HOFS = gDestinyBoardWaveTable[(gWork.u.r.unkB06 + (REG_VCOUNT >> 1)) % 64] + gWork.u.r.unkB06;
    REG_BG3HOFS = gWork.u.r.unkB06 - gDestinyBoardWaveTable[(gWork.u.r.unkB06 + (REG_VCOUNT >> 2)) % 64];
}

void DestinyBoardScene_AdvanceWave(void)
{
    if (--gWork.u.r.unkB07 == 0xFF) {
        gWork.u.r.unkB07 = 8;
        gWork.u.r.unkB06++;
    }
}

void ScrollLayer_Init(u8 *src, u8 *dst, s16 speed, s16 target, struct Reel *reel)
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
void ScrollLayer_Move(struct Reel *reel)
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

void ScrollLayer_StreamRow(struct Reel *reel)
{
    int old = reel->row;
    int y = (s16)reel->pos >> 4;
    if (old != y / 8) {
        reel->row = y / 8;
        CopyMapRect(reel->src + (reel->row + 0x16) * 60, reel->dst + ((reel->row - 1) & 0x1F) * 64, 0x1E, 1);
    }
}

void DestinyBoardScene_VBlank(void)
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

#define SIN(i) gSineTable[i]

/* Draw the five reel-stop markers at (x, y): each pulses (affine scale) and,
 * once its counter is started, flies along an arc (two sprites each). */
void DestinyBoardScene_DrawFinalLetters(u32 unused, u16 x, u16 y)
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
            gWork.aff[0].scaleX = gWork.aff[0].scaleY = 0x100 - MulFix8(0xC0, SIN((gWork.u.r.counters[0].pos * 3 / 2) & 0xFF));
            break;
        case 1:
            gWork.aff[i].scaleX = gWork.aff[i].scaleY = 0x100 - MulFix8(0xC0, SIN((gWork.u.r.counters[i].pos * 3 / 2) & 0xFF));
            break;
        case 2:
            gWork.aff[i].scaleX = gWork.aff[i].scaleY = 0x100 - MulFix8(0xC0, SIN((gWork.u.r.counters[i].pos * 3 / 2) & 0xFF));
            break;
        case 3:
            gWork.aff[i].scaleX = gWork.aff[i].scaleY = 0x100 - MulFix8(0xC0, SIN((gWork.u.r.counters[i].pos * 3 / 2) & 0xFF));
            break;
        case 4:
            gWork.aff[i].scaleX = gWork.aff[i].scaleY = 0x100 - MulFix8(0xC0, SIN((gWork.u.r.counters[i].pos * 3 / 2) & 0xFF));
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
                dx2 = (u16)(-(MulFix8(0xA00, SIN(0x30)) >> 4) + (MulFix8(0xA00, SIN((gWork.u.r.counters[0].pos + 0x30) & 0xFF)) >> 4));
                dy2 = (MulFix8(0x6000, SIN(0x10)) >> 8) - (MulFix8(0x6000, SIN(gWork.u.r.counters[0].pos + 0x10)) >> 8);
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
                dx2 = (u16)(-(MulFix8(0x400, SIN(0x30)) >> 4) + (MulFix8(0x400, SIN((gWork.u.r.counters[i].pos + 0x30) & 0xFF)) >> 4));
                dy2 = (MulFix8(0x4000, SIN(0x10)) >> 8) - (MulFix8(0x4000, SIN(gWork.u.r.counters[i].pos + 0x10)) >> 8);
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
                dx2 = (u16)((MulFix8(0x200, SIN(((gWork.u.r.counters[i].pos + 0x30) * 2 & 0xFF) + 0x40)) >> 4) + 0x23 - (MulFix8(0x200, SIN(0x70)) >> 4));
                dy2 = (MulFix8(0x7000, SIN(0x10)) >> 8) - (MulFix8(0x7000, SIN(gWork.u.r.counters[i].pos + 0x10)) >> 8);
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
                dx2 = (u16)((MulFix8(0xA00, SIN(0x30)) >> 4) - (MulFix8(0xA00, SIN((gWork.u.r.counters[i].pos + 0x30) & 0xFF)) >> 4));
                dy2 = (MulFix8(0xA000, SIN(0x10)) >> 8) - (MulFix8(0xA000, SIN(gWork.u.r.counters[i].pos + 0x10)) >> 8);
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
                dx2 = (u16)((MulFix8(0xA00, SIN(0x30)) >> 4) - (MulFix8(0xA00, SIN((gWork.u.r.counters[i].pos + 0x30) & 0xFF)) >> 4));
                /* ROM marker 4 shifts both Y helper results by 8. */
                dy2 = -(MulFix8(0x6000, SIN(0x10)) >> 8) + (MulFix8(0x6000, SIN(gWork.u.r.counters[i].pos + 0x10)) >> 8);
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
            extern struct SpriteGroup gFinalLettersAnim;
            s16 sx = x;
            s16 sy = y;
            u16 *o = OamListAddSpriteGroup((const u16 *)((u8 *)gFinalLettersAnim.templates + (i * 8 + 8)), pal, 1, sx - (s16)dx1, sy - (s16)dy1, 8, 1, 0, 0, 0, affine, &gWork);
            if (affine == 0x300)
                o[1] = (o[1] & 0xC1FF) | (i << 9);
            o = OamListAddSpriteGroup((const u16 *)((u8 *)gFinalLettersAnim.templates + (i * 8 + 0x30)), pal, 1, sx - (s16)dx2, sy - (s16)dy2, 8, 1, 0, 0, 0, affine, &gWork);
            if (affine == 0x300)
                o[1] = (o[1] & 0xC1FF) | (i << 9);
        }
        skip = 0;
    }
}
