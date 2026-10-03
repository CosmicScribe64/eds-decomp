#include "global.h"
#include "gba.h"

/* Card location descriptor (4 bytes) passed to the card-move animation DuelAnim_MoveCard. */
struct CardLoc {
    u16 player:1;       /* bit 0 */
    u16 area:4;         /* bits 1-4 */
    u16 index:9;        /* bits 5-13 */
    u16 flag14:1;
    u16 flag15:1;
    u16 unk2;
};

/* Duel screen / animation state at 0x0201CFB0 (fields used here). */
struct DuelScreen {
    u8 fast:1;          /* +0x000 bit 0 */
    u8 cursorOn:1;      /* +0x000 bit 1: draw the field cursor sprite */
    u8 active:1;        /* +0x000 bit 2: duel screen running (per-frame update enabled) */
    u8 unk0_3:5;
    u8 unk1[3];
    u8 scroll;          /* +0x004: current field scroll (copied to gMain+0x4422/0x4424) */
    u8 scrollFrom;      /* +0x005 */
    u8 scrollTo;        /* +0x006 */
    u8 scrollSteps:4;   /* +0x007 bits 0-3: interpolation steps left (4 = start) */
    u8 unk7_4:4;
    u8 tiles[0x800];    /* +0x008: tile buffer copied to VRAM 0x060091C0 */
    u16 tilesDirty:1;   /* +0x808 bit 0 */
    u16 flag808_1:1;    /* +0x808 bit 1: call TextCellsResetMap after the tile copy */
    u16 cursorDone:1;   /* +0x808 bit 2: cursor move finished (DuelScreen_DrawCursorInfo is called) */
    u16 cursorShow:1;   /* +0x808 bit 3 */
    u16 cursorFlipA:1;  /* +0x808 bit 4: OAM attr1 0x40 (hypothesis) */
    u16 cursorFlipB:1;  /* +0x808 bit 5: OAM attr2 0x30 (hypothesis: palette) */
    u16 cursorSteps:4;  /* +0x808 bits 6-9: interpolation steps left */
    u16 unk808_10:6;
    u8 filler80A[2];
    s32 cursorX;        /* +0x80C */
    s32 cursorY;        /* +0x810 */
    s32 cursorFromX;    /* +0x814 */
    s32 cursorFromY;    /* +0x818 */
    s32 cursorToX;      /* +0x81C */
    s32 cursorToY;      /* +0x820 */
    s32 player824;      /* +0x824 */
    s32 zone828;        /* +0x828 */
    s32 idx82C;         /* +0x82C */
    u8 animActive:1;    /* +0x830 bit 0: card animation pending */
    u8 animKind:7;      /* +0x830 bits 1-7: 1..5, dispatched by DuelAnim_Update */
    u8 filler831[3];
    u32 animArg;        /* +0x834: card id and other fields */
    u8 animStep;        /* +0x838 */
    u8 animTimer;       /* +0x839 */
    u16 animArg83A;     /* +0x83A */
    u16 animArg83C;     /* +0x83C */
    u8 filler83E[2];
    struct CardLoc from; /* +0x840 */
    struct CardLoc to;   /* +0x844 */
    u8 unk848[1];        /* +0x848 (= 0x0201D7F8): sub-object passed to sub_08076xxx */
};

extern struct DuelScreen gDuelScreen;
#define sScreen gDuelScreen

extern const u16 gDuelZoneScrollTargets[][16];
struct ZoneAnimEntry { u32 unk0; u32 unk4; };
extern const struct ZoneAnimEntry gDuelZonePositions[][16];

void DuelAnim_UpdateChangePosition(void);
void DuelAnim_UpdateFlip(void);
void DuelAnim_UpdateMoveCard(void);
void DuelAnim_UpdateSwapCards(void);
void DuelAnim_UpdateZoneEffect(void);
s32 MulFix8(s32 a, s32 b);     /* 8.8 fixed-point multiply */

/* Message/sequence block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 unk0[6];
    u16 opts;           /* +0x6: options (bit 7, bits 8-14, bit 15) */
    u8 unk8[3];
    u8 step;            /* +0xB: sequence step, index into gCoinTossSteps */
};
extern struct DuelMsg gDuelScene;
extern u16 (*const gCoinTossSteps[])(void);

/* One animated token of the toss animation (12 bytes). */
struct TossSlot {
    u8 timer;           /* +0x0: frames until next anim frame */
    u8 frame;           /* +0x1: animation frame 0-7 */
    u8 state;           /* +0x2: 1 = moving */
    u8 unk3;
    s16 y;              /* +0x4: height (8.8) */
    u16 t;              /* +0x6: time parameter */
    u8 unk8;
    u8 unk9;
    u8 unkA;
    u8 unkB;
};

struct Toss {
    struct TossSlot slots[8];   /* +0x00 */
    u8 count;           /* +0x60: number of tokens (opts bits 8-14) */
    u8 next;            /* +0x61: next token to launch */
    u8 mode;            /* +0x62: 0 or 4 (opts bit 15) */
    u8 unk63;           /* +0x63: 0 or 10 (opts bit 7) */
    u8 landed;          /* +0x64: tokens that landed with result 0 */
    u8 done;            /* +0x65: set when CoinToss_CountUnfinished reports all finished */
};

/* Overlay work area at 0x02015280 (0xC58 bytes, fields used here). */
struct Work15280 {
    u8 unk0[0x618];
    struct {
        u16 unk0;
        u16 unk2;
        u8 unk4[0x14];
    } unk618[3];                /* +0x618: passed to ObjAffineInit */
    u8 filler660[0xAAC - 0x660];
    struct Toss toss;           /* +0xAAC (0x68 bytes) */
    struct {
        u8 unk0;
        u8 unk1;
        u8 unk2;
        u8 unk3;
    } unkB14[4];                /* +0xB14: [0] is passed to Scroller_Move/Scroller_SnapToStop, [1] to Scroller_StopAtEnds */
    u8 unkB24;
    u8 unkB25;
    u8 unkB26;
    u8 unkB27;
    u16 unkB28;
    u16 unkB2A;
    struct {
        u16 unk0;
        u16 unk2;
    } unkB2C[2];
    u16 unkB34;
    u8 unkB36;
    u8 unkB37;
    u16 unkB38;
    u16 unkB3A;
    u8 unkB3C;
    u8 unkB3D;
    u16 unkB3E;
    u16 timer;                  /* +0xB40: frame counter; the toss ends after 0x140 frames */
    u8 unkB42;
    u8 fillerB43[0xB48 - 0xB43];
    u8 unkB48[6];               /* +0xB48: FadeTick / FadeStart */
    u8 unkB4E;                  /* +0xB4E: 2 = done, 3 = set BLDCNT */
    u8 unkB4F;
    u8 objB50[0xC54 - 0xB50];   /* +0xB50: CoinToss_UpdateSparkles/CoinToss_DrawSparkles/CoinToss_ClearSparkles */
    u8 phase;                   /* +0xC54: 0 = launch all, 1 = launch one every 16 frames */
    u8 delay;                   /* +0xC55 */
    u8 launched;                /* +0xC56 */
    u8 fillerC57;
};
extern struct Work15280 gCoinTossWork;
#define gWork gCoinTossWork
void CoinToss_InitCoins(struct Toss *t);
/* Toss screen graphics (ROM). */
extern const u8 gEgyptCorridorBitmap[];
extern const u8 gEgyptCorridorPal[];
extern const u8 gCoinPalette[];
extern const u8 gSparklePalette[];
extern const u8 gCoinSpinGfx[];
extern const u8 gCoinSpinGfx_Frame1[];
extern const u8 gCoinSpinGfx_Frame2[];
extern const u8 gCoinSpinGfx_Frame3[];
extern const u8 gCoinSpinGfx_Frame4[];
extern const u8 gCoinSpinGfx_Frame5[];
extern const u8 gCoinSpinGfx_Frame6[];
extern const u8 gCoinSpinGfx_Frame7[];
extern const u8 gCoinGlintGfx[];
extern const u8 gCoinGlintGfx_Frame1[];
extern const u8 gCoinGlintGfx_Frame2[];
extern const u8 gCoinGlintGfx_Frame3[];
extern const u8 gCoinGlintGfx_Frame4[];
extern const u8 gCoinGlintGfx_Frame5[];
extern const u8 gCoinGlintGfx_Frame6[];
extern const u8 gCoinGlintGfx_Frame7[];
extern const u8 gCoinGlintGfx_Frame8[];
extern const u8 gCoinGlintGfx_Frame9[];
extern const u8 gSparkleGfx[];
extern const u8 gSparkleGfx_Frame1[];
extern const u8 gSparkleGfx_Frame2[];
extern const u8 gSparkleGfx_Frame3[];
extern const u8 gSparkleGfx_Frame4[];
extern const u8 gSparkleGfx_Frame5[];
void CoinToss_UpdateSparkles(void *p);
void CoinToss_DrawSparkles(void *p);
void FadeTick(void *p);
u32 CoinToss_CountUnfinished(struct Toss *t, u8 n);
void PlaySE(u32 se);
void FadeStart(u32 a, u32 b, u32 c, void *p);
void CoinToss_MarkMatchingCoins(struct TossSlot *s, u8 n, u8 mode);
void CoinToss_AnimateHighlights(struct TossSlot *s, u8 n);
void OamListFlush(void *p);
void Scroller_Move(void *p);
void Scroller_SnapToStop(void *p);
void Scroller_StopAtEnds(void *p);
void CoinToss_AnimateSpin(struct TossSlot *s, u8 n);
void CoinToss_UpdateFlight(struct TossSlot *s, u8 n, u32 unused, u8 *count);
void CoinToss_DrawCoins(struct TossSlot *s, u8 n);
void CoinToss_DrawGlints(struct TossSlot *s, u8 n, u8 mode);


/* gMain (0x03000040): only the fields used here. */
struct Main {
    u8 unk0[0x40E];
    u16 unk40E;         /* +0x40E */
    u8 filler410[0x4422 - 0x410];
    u16 bgScroll4422;   /* +0x4422 */
    u16 bgScroll4424;   /* +0x4424 */
};
extern struct Main gMain;
#define gMain gMain
void MemClear16(void *dst, u32 size); /* MemClear16 */
void OamListClear(void *p);
void ObjAffineInit(void *p);
void SetBldAlpha(u32 a);
void CoinToss_ClearSparkles(void *p);

extern const u16 gCoinSpinTiles[];   /* toss token tile per anim frame */
extern const u16 gCoinGlintTiles[];   /* result tile (+5: second set) */
void OamListAddSprite(u32 a, u32 tile, s32 x, s32 y, u32 w, u32 h, u32 a6, u32 a7, u32 a8,
                  u32 a9, u32 a10, u32 a11, void *work);

extern u8 gCoinTossSparkles[];
void CoinToss_SpawnSparkle(u8 x, u8 y, void *work);

/* Second tile buffer at 0x0201AE60 (fields used here). */
struct TileBuf {
    u8 unk0_0:1;
    u8 dirty:1;         /* +0x00 bit 1 */
    u8 unk0_2:6;
    u8 filler1[0x24 - 1];
    u8 tiles[0x1B00];   /* +0x24: copied to VRAM 0x06009AE0 */
};
extern struct TileBuf gTextBox;
extern const u16 gDuelScreenLerpWeights[];   /* interpolation weights (8.8), indexed by steps left */
void CopyDoubleWords(void *dst, const void *src, u32 size);
void TextCellsResetMap(void);
void DuelScreen_DrawCursorInfo(void);
void AddAffineSprite(u32 yx, u16 shapeSize, u16 attr2, u32 affine);

void DuelScreen_StartScroll(u32 bg);
void DuelScreen_ScrollToZone(u32 player, u32 zone);
void DuelCursor_MoveTo(u32 x, u32 y);
void DuelCursor_Select(s32 player, s32 zone, s32 idx);
u32 GetAreaX(u32 player, u32 a, u32 b);
void SprAnimLoad(u32 a, void *obj);
void SprAnimRewind(void *obj);
void SprAnimDrawFrame(s16 a, s16 b, void *obj, u16 c);
void SprAnimDrawFrameAt(s16 a, s16 b, void *obj, u16 c);
void SprAnimDrawFrameAtFlip(u32 a, void *obj, u16 b, u16 c);


void DuelScreen_StartScroll(u32 bg)
{
    sScreen.scrollFrom = sScreen.scroll;
    sScreen.scrollTo = bg;
    sScreen.scrollSteps = 4;
}
void DuelScreen_ScrollToZone(u32 player, u32 zone)
{
    u16 v = gDuelZoneScrollTargets[player][zone];
    if (sScreen.scrollTo != (u8)v)
        DuelScreen_StartScroll(v);
}
/* Start moving the field cursor from its current position to (x, y) over 4 frames. */
void DuelCursor_MoveTo(u32 x, u32 y)
{
    sScreen.cursorFromX = sScreen.cursorX;
    sScreen.cursorFromY = sScreen.cursorY;
    sScreen.cursorToX = x;
    sScreen.cursorToY = y;
    sScreen.cursorSteps = 4;
}
void DuelCursor_Select(s32 player, s32 zone, s32 idx)
{
    s32 i;

    sScreen.player824 = player;
    sScreen.zone828 = zone;
    sScreen.idx82C = idx;
    if (sScreen.zone828 == 0) {
        if (idx == 10) {
            sScreen.zone828 = idx;
            sScreen.idx82C = zone;
        }
        if (sScreen.idx82C > 4) {
            sScreen.zone828 = 5;
            sScreen.idx82C = sScreen.idx82C - 5;
        }
    }
    if (zone == 0 || zone == 5)
        i = zone + idx;
    else
        i = zone;
    DuelCursor_MoveTo(GetAreaX(player, zone, idx), gDuelZonePositions[player][i].unk4);
    DuelScreen_ScrollToZone(sScreen.player824, sScreen.zone828);
}
void DuelCursor_Refresh(void)
{
    DuelCursor_Select(sScreen.player824, sScreen.zone828, sScreen.idx82C);
}

void DuelSprAnim_Load(u32 a)
{
    SprAnimLoad(a, sScreen.unk848);
    sScreen.cursorOn = 0;
}

void DuelSprAnim_Rewind(void)
{
    SprAnimRewind(sScreen.unk848);
}

void DuelSprAnim_DrawAt(s16 a, s16 b, u16 c)
{
    SprAnimDrawFrame(a, b, sScreen.unk848, c);
}

void DuelSprAnim_Draw(s16 a, s16 b, u16 c)
{
    SprAnimDrawFrameAt(a, b, sScreen.unk848, c);
}

void DuelSprAnim_DrawFlip(u32 a, u16 b, u16 c)
{
    SprAnimDrawFrameAtFlip(a, sScreen.unk848, b, c);
}

void DuelAnim_Request(u16 kind, u32 arg)
{
    sScreen.animKind = kind;
    sScreen.animArg = arg;
    sScreen.animStep = 0;
    sScreen.animTimer = 0;
    sScreen.animActive = 1;
}

void DuelAnim_MoveCard(u16 id, struct CardLoc *from, struct CardLoc *to)
{
    sScreen.animKind = 3;
    sScreen.animArg = id;
    sScreen.from = *from;
    sScreen.to = *to;
    sScreen.animStep = 0;
    sScreen.animTimer = 0;
    sScreen.animActive = 1;
}

void DuelAnim_SwapCards(struct CardLoc *from, struct CardLoc *to)
{
    sScreen.animKind = 4;
    sScreen.from = *from;
    sScreen.to = *to;
    sScreen.animStep = 0;
    sScreen.animTimer = 0;
    sScreen.animActive = 1;
}

void DuelAnim_PlayZoneEffect(struct CardLoc *from, u32 arg, u32 a, u32 b)
{
    sScreen.animKind = 5;
    sScreen.animArg = arg;
    sScreen.from = *from;
    sScreen.animArg83A = a;
    sScreen.animArg83C = b;
    sScreen.animStep = 0;
    sScreen.animTimer = 0;
    sScreen.animActive = 1;
}

u16 DuelAnim_Update(void)
{
    if (sScreen.animActive) {
        switch (sScreen.animKind) {
        case 1:
            DuelAnim_UpdateChangePosition();
            return 1;
        case 2:
            DuelAnim_UpdateFlip();
            return 1;
        case 3:
            DuelAnim_UpdateMoveCard();
            return 1;
        case 4:
            DuelAnim_UpdateSwapCards();
            return 1;
        case 5:
            DuelAnim_UpdateZoneEffect();
            return 1;
        }
    }
    return 0;
}
u32 DuelScreen_Update(void)
{
    u32 busy = 0;

    if (sScreen.active) {
        if (sScreen.tilesDirty) {
            CopyDoubleWords((void *)0x060091C0, sScreen.tiles, 0x800);
            if (sScreen.flag808_1) {
                TextCellsResetMap();
                sScreen.flag808_1 = 0;
            }
            sScreen.tilesDirty = 0;
        }
        if (sScreen.active) {
            if (gTextBox.dirty) {
                CopyDoubleWords((void *)0x06009AE0, gTextBox.tiles, 0x1B00);
                gTextBox.dirty = 0;
            }
            if (sScreen.active) {
                int steps = sScreen.scrollSteps;
                if (steps > 0) {
                    s32 d = sScreen.scrollTo - sScreen.scrollFrom;
                    sScreen.scrollSteps = steps - 1;
                    d *= gDuelScreenLerpWeights[sScreen.scrollSteps];
                    d /= 256;
                    sScreen.scroll = sScreen.scrollFrom + d;
                    busy = 1;
                }
                gMain.bgScroll4422 = sScreen.scroll;
                gMain.bgScroll4424 = sScreen.scroll;
            }
        }
    }
    if (sScreen.cursorDone) {
        sScreen.cursorDone = 0;
        DuelScreen_DrawCursorInfo();
    }
    if (sScreen.cursorSteps) {
        sScreen.cursorX = sScreen.cursorToX - sScreen.cursorFromX;
        sScreen.cursorY = sScreen.cursorToY - sScreen.cursorFromY;
        sScreen.cursorSteps--;
        sScreen.cursorX *= gDuelScreenLerpWeights[sScreen.cursorSteps];
        sScreen.cursorY *= gDuelScreenLerpWeights[sScreen.cursorSteps];
        sScreen.cursorX /= 256;
        sScreen.cursorY /= 256;
        sScreen.cursorX += sScreen.cursorFromX;
        sScreen.cursorY += sScreen.cursorFromY;
        busy = 1;
        if (sScreen.cursorSteps == 0)
            sScreen.cursorDone = 1;
    }
    {
        s32 y = sScreen.cursorY - sScreen.scroll;
        if (y >= 0 && y < 160) {
            u16 pal = sScreen.cursorFlipB ? 0x30 : 0;
            u32 flip = sScreen.cursorFlipA ? 0x40 : 0;
            if (sScreen.cursorOn && sScreen.cursorShow)
                AddAffineSprite((sScreen.cursorX + 8) | (y << 16), 0x80, pal, flip | 0x1000000);
        }
    }
    if (DuelAnim_Update())
        busy = 1;
    return busy;
}
u32 CoinToss_Init(void)
{
    MemClear16(&gCoinTossWork, sizeof(gCoinTossWork));
    gMain.unk40E = 1;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= 0xE0FF;
    OamListClear(&gCoinTossWork);
    ObjAffineInit(gCoinTossWork.unk618);
    SetBldAlpha(8);
    gCoinTossWork.timer = 0;
    gCoinTossWork.unkB42 = 0;
    gCoinTossWork.unkB27 = 0xFF;
    CoinToss_ClearSparkles(gCoinTossWork.objB50);
    return 1;
}
u32 CoinToss_Load(void)
{
    vu16 zero;
    vu16 zero2;
    u8 i;

    FadeStart(0, -0x180, 0, gWork.unkB48);
    CpuSet(gEgyptCorridorBitmap, (void *)0x06000000, 0x4B00);
    CpuSet(gEgyptCorridorPal, (void *)0x05000000, 0x100);
    CpuSet(gCoinPalette, (void *)0x05000200, 0x10);
    zero = 0;
    CpuSet((void *)&zero, (void *)0x06014000, 0x01000010);
    CpuSet(gCoinSpinGfx, (void *)0x06014020, 0x100);
    CpuSet(gCoinSpinGfx_Frame1, (void *)0x06014220, 0x100);
    CpuSet(gCoinSpinGfx_Frame2, (void *)0x06014420, 0x100);
    CpuSet(gCoinSpinGfx_Frame3, (void *)0x06014620, 0x100);
    CpuSet(gCoinSpinGfx_Frame4, (void *)0x06014820, 0x100);
    CpuSet(gCoinSpinGfx_Frame5, (void *)0x06014A20, 0x100);
    CpuSet(gCoinSpinGfx_Frame6, (void *)0x06014C20, 0x100);
    CpuSet(gCoinSpinGfx_Frame7, (void *)0x06014E20, 0x100);
    CpuSet(gCoinGlintGfx, (void *)0x06015020, 0x100);
    CpuSet(gCoinGlintGfx_Frame1, (void *)0x06015220, 0x100);
    CpuSet(gCoinGlintGfx_Frame2, (void *)0x06015420, 0x100);
    CpuSet(gCoinGlintGfx_Frame3, (void *)0x06015620, 0x100);
    CpuSet(gCoinGlintGfx_Frame4, (void *)0x06015820, 0x100);
    CpuSet(gCoinGlintGfx_Frame5, (void *)0x06015A20, 0x100);
    CpuSet(gCoinGlintGfx_Frame6, (void *)0x06015C20, 0x100);
    CpuSet(gCoinGlintGfx_Frame7, (void *)0x06015E20, 0x100);
    CpuSet(gCoinGlintGfx_Frame8, (void *)0x06016020, 0x100);
    CpuSet(gCoinGlintGfx_Frame9, (void *)0x06016220, 0x100);
    CpuSet(gSparkleGfx, (void *)0x060164A0, 0x40);
    CpuSet(gSparkleGfx_Frame1, (void *)0x06016520, 0x40);
    CpuSet(gSparkleGfx_Frame2, (void *)0x060165A0, 0x40);
    CpuSet(gSparkleGfx_Frame3, (void *)0x06016620, 0x40);
    CpuSet(gSparkleGfx_Frame4, (void *)0x060166A0, 0x40);
    CpuSet(gSparkleGfx_Frame5, (void *)0x06016720, 0x40);
    CpuSet(gSparklePalette, (void *)0x05000220, 0x10);
    zero2 = 0;
    CpuSet((void *)&zero2, (void *)0x06016420, 0x01000040);
    for (i = 0; i < 3; i++) {
        gWork.unk618[i].unk0 = 0;
        gWork.unk618[i].unk2 = 0;
    }
    CoinToss_InitCoins(&gWork.toss);
    for (i = 0; i < 4; i++) {
        gWork.unkB14[i].unk1 = 0;
        gWork.unkB14[i].unk0 = 0;
        gWork.unkB14[i].unk2 = 0;
    }
    for (i = 0; i < 2; i++)
        gWork.unkB2C[i].unk0 = 0;
    gWork.unkB3C = 0;
    gWork.unkB3D = 0;
    gWork.unkB24 = 0xFF;
    gWork.unkB25 = 0;
    gWork.unkB28 = 0;
    gWork.unkB36 = 0xFA;
    gWork.unkB37 = 0;
    gWork.unkB38 = 0;
    gWork.unkB3A = 0;
    gWork.timer = 0;
    REG_DISPCNT = 0x1F44;
    return 1;
}
u32 CoinToss_Update(void)
{
    CoinToss_UpdateSparkles(gWork.objB50);
    CoinToss_DrawSparkles(gWork.objB50);
    FadeTick(gWork.unkB48);
    if (gWork.unkB4E == 2)
        return 1;
    if (gWork.unkB4E == 3)
        REG_BLDCNT = 0x1040;
    if (gWork.toss.done == 0 && CoinToss_CountUnfinished(&gWork.toss, gWork.toss.count) == 0) {
        gWork.toss.done = 1;
        gWork.delay = 100;
    }
    switch (gWork.phase) {
    case 0:
        switch (gWork.toss.done) {
        case 0:
            gWork.toss.slots[gWork.toss.next].state = 1;
            gWork.toss.next++;
            if (gWork.toss.next == gWork.toss.count + 1)
                gWork.toss.next = gWork.toss.count;
            else
                PlaySE(30);
            break;
        case 1:
            FadeStart(0, 0x180, 0, gWork.unkB48);
            break;
        }
        break;
    case 1:
        if (--gWork.delay == 0xFF) {
            gWork.delay = 15;
            switch (gWork.toss.done) {
            case 0:
                gWork.toss.slots[gWork.toss.next].state = 1;
                gWork.toss.next++;
                if (gWork.toss.next == gWork.toss.count + 1)
                    gWork.toss.next = gWork.toss.count;
                if (gWork.launched++ < gWork.toss.count)
                    PlaySE(30);
                break;
            case 1:
                FadeStart(0, 0x180, 0, gWork.unkB48);
                break;
            }
        }
        break;
    }
    CoinToss_AnimateSpin(gWork.toss.slots, gWork.toss.count);
    CoinToss_UpdateFlight(gWork.toss.slots, gWork.toss.count, gWork.timer, &gWork.toss.landed);
    CoinToss_DrawCoins(gWork.toss.slots, gWork.toss.count);
    CoinToss_MarkMatchingCoins(gWork.toss.slots, gWork.toss.count, gWork.toss.mode);
    CoinToss_AnimateHighlights(gWork.toss.slots, gWork.toss.count);
    CoinToss_DrawGlints(gWork.toss.slots, gWork.toss.count, gWork.toss.mode);
    OamListFlush(&gWork);
    OamListClear(&gWork);
    Scroller_Move(&gWork.unkB14[0]);
    Scroller_SnapToStop(&gWork.unkB14[0]);
    Scroller_StopAtEnds(&gWork.unkB14[1]);
    if (gWork.timer++ > 0x140)
        return 1;
    return 0;
}
u32 CoinToss_Run(void)
{
    if (gCoinTossSteps[gDuelScene.step]) {
        if (gCoinTossSteps[gDuelScene.step]())
            gDuelScene.step++;
        return 0;
    }
    return 1;
}

u32 CoinToss_RunStaggered(void)
{
    if (gDuelScene.step == 1) {
        gCoinTossWork.phase = 1;
        gCoinTossWork.delay = 30;
    }
    if (gCoinTossSteps[gDuelScene.step]) {
        if (gCoinTossSteps[gDuelScene.step]())
            gDuelScene.step++;
        return 0;
    }
    return 1;
}

void CoinToss_InitCoins(struct Toss *t)
{
    u8 i;

    for (i = 0; i < 8; i++) {
        t->slots[i].timer = 3;
        t->slots[i].frame = 0;
        t->slots[i].state = 0;
        t->slots[i].y = 0;
        t->slots[i].t = 0;
        t->slots[i].unkA = 0;
        t->slots[i].unk9 = 0;
        t->slots[i].unk8 = 6;
    }
    t->next = 0;
    t->landed = 0;
    t->done = 0;
    t->count = 3;
    t->mode = 0;
    t->unk63 = 2;
    t->count = (gDuelScene.opts & 0x7F00) >> 8;
    t->mode = (gDuelScene.opts >> 13) & 4;
    if (gDuelScene.opts & 0x80)
        t->unk63 = 0;
    else
        t->unk63 = 10;
}

void CoinToss_AnimateSpin(struct TossSlot *s, u8 n)
{
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].state == 1) {
            if (s[i].timer == 0) {
                s[i].timer = 3;
                if (++s[i].frame == 8)
                    s[i].frame = 0;
            } else {
                s[i].timer--;
            }
        }
    }
}

void CoinToss_UpdateFlightUniform(struct TossSlot *s, u8 n, u8 mask, u8 *count)
{
    u8 i;

    for (i = 0; i < n; i++) {
        u8 st = s[i].state;
        if (st == 1) {
            s[i].t += 40;
            s[i].y = MulFix8(0x3000, s[i].t) - MulFix8(0x500, MulFix8(s[i].t, s[i].t));
            if (s[i].y < 0) {
                u8 f;
                s[i].y = 0;
                s[i].t = 0;
                s[i].state++;
                f = (st & mask) ? 0 : 4;
                s[i].frame = f;
                if (f == 0)
                    (*count)++;
            }
        }
    }
}

void CoinToss_UpdateFlight(struct TossSlot *s, u8 n, u32 unused, u8 *count)
{
    u8 i;

    for (i = 0; i < n; i++) {
        u8 st = s[i].state;
        if (st == 1) {
            s[i].t += 40;
            s[i].y = MulFix8(0x3000, s[i].t) - MulFix8(0x500, MulFix8(s[i].t, s[i].t));
            if (s[i].y < 0) {
                s[i].y = 0;
                s[i].t = 0;
                s[i].state++;
                if ((gDuelScene.opts >> i) & st)
                    s[i].frame = 4;
                else
                    s[i].frame = 0;
                if (s[i].frame == 0)
                    (*count)++;
            } else if ((s[i].t & 0xF) == 0) {
                CoinToss_SpawnSparkle((i + 1) * 240 / (n + 1) - 16, 0x78 - (s[i].y >> 8), gCoinTossSparkles);
            }
        }
    }
}
void CoinToss_DrawCoins(struct TossSlot *s, u8 n)
{
    u8 i;

    for (i = 0; i < n; i++) {
        int tile = gCoinSpinTiles[s[i].frame] + 0x200;
        OamListAddSprite(0, tile, (i + 1) * 240 / (n + 1) - 16, 0x78 - (s[i].y >> 8),
                     32, 32, 4, 0, 0x200, 0, 0, 0, &gCoinTossWork);
    }
}
void CoinToss_DrawGlints(struct TossSlot *s, u8 n, u8 mode)
{
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].unkA == 1) {
            int tile = gCoinGlintTiles[s[i].unk9 + (mode != 4 ? 5 : 0)] + 0x200;
            OamListAddSprite(0, tile, (i + 1) * 240 / (n + 1) - 16, 0x78 - (s[i].y >> 8),
                         32, 32, 4, 0, 0x200, 0, 0, 0, &gCoinTossWork);
        }
    }
}
