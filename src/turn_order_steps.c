#include "global.h"
#include "gba.h"

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
    u8 unkD;
    s8 unkE;                /* +0xE */
    u8 unkF;
    u8 unk10;
    u8 unk11[3];
};

/* Work area at 0x02020310 as used by this screen (0xB24 bytes). */
struct Work20310 {
    u8 oam[0x618];          /* +0x000: OAM buffer (OamListFlush / OamListClear) */
    struct {
        u16 scaleX;         /* +0x0 (0x100 = 1.0) */
        u16 scaleY;         /* +0x2 */
        u16 angle;          /* +0x4 */
        u8 unk6[0x12];
    } aff[0x20];            /* +0x618: OBJ affine sets (ObjAffineInit) */
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
    u8 unkABE;              /* +0xABE: result (JudgeRockPaperScissors) */
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
    s16 unkADC[2];          /* +0xADC: object for TweenUpdate / TweenInit */
    u8 fillerAE0[0xAF0 - 0xAE0];
    u8 unkAF0;              /* +0xAF0: state of the +0xADC object */
    u8 fillerAF1[3];
    u8 unkAF4;              /* +0xAF4 */
    u8 unkAF5;              /* +0xAF5: step */
    u8 fillerAF6[2];
    u8 fade[8];             /* +0xAF8: object for FadeStart */
    struct Timer timer;     /* +0xB00 */
    u8 fillerB04[0xB0D - 0xB04];
    u8 unkB0D;              /* +0xB0D */
    u8 unkB0E;              /* +0xB0E */
    u8 fillerB0F;
    u8 unkB10[6];           /* +0xB10: link exchange (LinkSyncStart / LinkSyncStep) */
    u16 unkB16;             /* +0xB16: received value */
    u8 fillerB18[4];
    u8 unkB1C;
    u8 unkB1D;
    u8 unkB1E;
    u8 fillerB1F;
    u16 unkB20;             /* +0xB20 */
    u8 fillerB22[2];
};
extern struct Work20310 gSceneWork;
#define gWork gSceneWork

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
extern struct Main gMain;
#define gMain gMain

void MemCopy16(void *dest, const void *src, u32 size);
void MemClear16(void *dst, u32 size);
u32 *OamListAddSprite(u32 a, u32 tile, s32 x, s32 y, u32 w, u32 h, u32 a6, u32 a7, u32 a8,
                  u32 a9, u32 a10, u32 a11, void *work);
s32 MulFix8(s32 a, s32 b);     /* 8.8 fixed-point multiply */
void Timer_Reset(struct Timer *t);
void Timer_Start(struct Timer *t, u16 time);
void Timer_Tick(struct Timer *t);
void SetBldAlpha(u32 a);           /* BLDALPHA */
void SetBldY(u32 a);           /* BLDY */
void PlaySE(u16 se);          /* PlaySE */

extern const u16 gUnk_0808270C[];
extern const u8 gUnk_08082710[];

#define CpuFastFill(value, dest, size)                                  \
{                                                                       \
    vu32 tmp = (vu32)(value);                                           \
    CpuFastSet((void *)&tmp, dest, 0x01000000 | (((size) / 4) & 0x1FFFFF)); \
}

void AnimBlockInit(const void *anim, struct SpriteGroup *grp);
void OamListClear(void *p);
void ObjAffineInit(void *p);
void TweenUpdate(void *obj);
void TweenInit(u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, void *obj, u32 g);
void TurnOrder_DrawTurnChoiceConfirm(u8 a, u8 b, u16 c, void *d, u8 e, void *obj);
void TurnOrder_LoadObjTiles(const u8 *src, u32 tile, u32 width, s32 rows);
void TurnOrder_DrawDuelLogo(u8 a, u8 b, u8 c);
extern const u8 gTurnOrderWaitAnimList[];
extern const u8 gDuelLogoTiles0[], gDuelLogoTiles1[], gDuelLogoTiles2[];
extern const u16 gTurnChoiceBannerTileNums[];
extern const s16 gSineTable[];   /* sine table (0x100 = 1.0) */
#define SIN(i) gSineTable[i]
extern const u16 gDuelLogoTileNums[];
extern const u16 gSquareTable[];
void FadeOutBGM(void);
void TurnOrder_DrawChosenTurnBanner(u32 unused0, u32 unused1, u16 flags, void *unused3, u8 which, s16 *pos);
void LinkSyncStart(void *p);
u32 LinkSyncStep(u32 a, u8 b, void *p);
u16 TurnOrder_AnimateTurnChoice(u8 *step);
void FadeStart(u32 a, s32 b, u32 c, void *p);
extern const u8 gEgyptCorridorBitmap[], gEgyptCorridorPal[], gRockCardPal[], gScissorsCardPal[], gPaperCardPal[];
extern const u8 gTurnChoiceBannerPal[], gSelectCardBannerPal[], gWinBannerPal[], gLoseBannerPal[], gDrawBannerPal[];
extern const u8 gTurnChoiceBannerDimPal[], gDuelLogoPal[], gWaitSignPal[];
extern const u8 gRockCardTiles[], gScissorsCardTiles[], gPaperCardTiles[], gTurnChoiceBannerTiles[], gSelectCardBannerTiles[];
extern const u8 gWinBannerTiles[], gLoseBannerTiles[], gWaitSignTiles[], gDrawBannerTiles[];
s32 Random(void);             /* Random */
void TurnOrder_HideWaitSign(void);
void TurnOrder_ShowWaitSign(void);
void TurnOrder_DrawBanner(u32 a, u16 b);
u8 JudgeRockPaperScissors(u8 a, u8 b);


u16 sub_0807EE98();
u16 sub_0807EE9C();
void SetMainCallback(u32 arg);
void TextDrawSjisGlyph(u32 ch, s32 x, s32 y, u32 attr);
void TextDrawLatinGlyph(u32 ch, s32 x, s32 y, u32 attr);
extern struct { u8 unk0[4]; u8 flags4; } gSaveData;
void TextCanvasInit(u32 a, u32 b);
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
extern struct Sel gCardListView;
extern u8 gDuelBanishedInfo[];
extern u8 gDuelPlayers[];
struct Name { u16 s[0x20]; };
extern const struct Name gCardNames[];
extern const u16 gStrCardListViewUnknown[];
void TextDrawShadowedString(s32 x, s32 y, const u16 *str, s32 w);
struct SelFlags { u8 b0 : 1; u8 b1 : 1; u8 b2 : 1; u8 b3 : 1; u8 rest : 4; u8 pad[7]; };
#define SELF ((struct SelFlags *)&gCardListView)
void AddSprite(u32 x, u32 y, u16 tile);
void FadeTick(void *obj);
void TurnOrder_DrawOpponentCard(u8 a, u8 b, u16 c);
void TurnOrder_DrawHandCarousel(const void *a, const void *b, u8 c, u8 d, u8 e, u16 f, u8 g);
void TurnOrder_DrawBanner(u32 a, u16 b);
void TurnOrder_UpdateChoiceBob(u8 i, void *p);
void TurnOrder_DrawTurnChoice(u8 a, u8 b, u16 c, void *d, u8 e);
void ObjAffineApply(void *p);
void AnimStateTick(void *grp);
void AnimBlockDraw(void *grp, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g, u32 h, void *work);
void OamListFlush(void *p);
void Scroller_Move(void *p);
void Scroller_SnapToStop(void *p);
void Scroller_StopAtEnds(void *p);
extern const u32 gTurnOrderRpsSubsteps[];
extern const u32 gTurnOrderChoiceSubsteps[];
extern const u8 gHandCardTileNums[];
u32 IsSpecialSummonOnly(u32 id);
extern const u32 gCardStats[];
#define MAPP(off) ((u16 *)((u8 *)&gMain + (off)))
extern u32 gTurnOrderRpsSteps[];
extern u32 gTurnOrderPlayerChoiceSteps[];
extern u32 gTurnOrderCpuChoiceSteps[];

/* Fade in with BLDY; when done, latch the pick into gMain.unk4870 bit 0. */
u16 TurnOrder_FadeOutAndSetFirstPlayer(void)
{
    struct Work20310 *w = &gWork;

    w->unkAD0 += w->unkAD2;
    if (w->unkAD0 > 0x1000) {
        gMain.unk4870b0 = w->unkABF;
        return 1;
    }
    SetBldY(w->unkAD0 >> 8);
    return 0;
}

/* Main play step: run the fade object, advance the sequence and update all sprites. */
u16 TurnOrder_RpsMain(void)
{
    u8 *f = gWork.fade;
    u8 i;
    s32 e;
    u32 v;

    FadeTick(f);
    if (f[6] == 2)
        gMain.seqIndex1 += f[7];
    if (f[6] == 3) {
        REG_BLDCNT = 0x440;
        f[6] = 0;
        gWork.unkAD5 = 6;
    }
    if (f[6] == 0 && gWork.unkAD5 == 0) {
        v = gTurnOrderRpsSubsteps[gWork.unkAF5];
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
        TurnOrder_DrawOpponentCard(gWork.unkABC, gWork.hands[1].unk0, gWork.unkAC0);
        TurnOrder_DrawHandCarousel(gHandCardTileNums, (const u8 *)0x08082703, gWork.hands[0].unk0, gWork.hands[0].hand,
                     gWork.hands[1].unk0, gWork.unkAC0, gWork.unkAD4);
        if (gWork.hands[1].unk0 == 0x30 && gWork.unkABD == 1 && gWork.unkABC != 0xFF) {
            if (gWork.unkABE == 0)
                TurnOrder_DrawBanner(1, gWork.unkAC0);
            else if (gWork.unkABE == 1)
                TurnOrder_DrawBanner(2, gWork.unkAC0);
            else if (gWork.unkABE == 2)
                TurnOrder_DrawBanner(4, gWork.unkAC0);
        }
        if (gWork.unkABF != 0xFF) {
            TurnOrder_UpdateChoiceBob(gWork.unkABF, gWork.unkAC4);
            TurnOrder_DrawTurnChoice(gWork.unkABF, gWork.unkAF4, gWork.unkAC0, gWork.unkAC4, gWork.unkABF);
        }
    }
    for (i = 0; i <= 4; i++)
        ObjAffineApply(&gWork.aff[i]);
    e = gWork.grp[0].unkE;
    if (e == 1) {
        AnimStateTick(&gWork.grp[0]);
        AnimBlockDraw(&gWork.grp[0], 0, 0, 1, e, 0, 0, 0, 0, &gWork);
    }
    OamListFlush(&gWork);
    OamListClear(&gWork);
    Scroller_Move(gWork.hands);
    Scroller_SnapToStop(gWork.hands);
    Scroller_StopAtEnds(&gWork.hands[1]);
    gWork.unkAF4++;
    return 0;
}
/* Draws the hand selection sprites and advances the step. */
u16 TurnOrder_ShowChoice(void)
{
    TurnOrder_DrawTurnChoiceConfirm(gWork.unkABF, gWork.unkAF4, gWork.unkAC0, gWork.unkAC4, gWork.unkABF, gWork.unkADC);
    gWork.unkAF5++;
    return 0;
}
/* Step 0 clears the work area (DMA fill) and resets the display. */
u16 TurnOrder_InitChoice(void)
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
    OamListClear(w);
    SetBldAlpha(8);
    w->unkAF4 = 0;
    w->unkAF5 = 2;
    w->unkABF = 0;
    ObjAffineInit(w->aff);
    return 1;
}
/* Choose Left/Right (step 2), then animate the fade object and the hands. */
u16 TurnOrder_ChoiceMain(void)
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
            TweenInit(0, 0, 0x40, 0xF, 3, 1, gWork.unkADC, 0);
            if (gWork.unkB0E == 1) {
                gWork.unkB0D = 1;
                LinkSyncStart(v->unkB10);
            } else {
                gWork.unkAF5++;
            }
        }
        if (gWork.unkB0D != 0) {
            if (LinkSyncStep(0x51, gWork.unkABF, v->unkB10)) {
                gWork.unkAF5++;
                gWork.unkB0D = 0;
            }
        }
    }
    f = gWork.fade;
    FadeTick(f);
    if (f[6] == 2)
        gMain.seqIndex1 += f[7];
    for (i = 0; i <= 3; i++) {
        gWork.aff[i].angle = 0;
        gWork.aff[i].scaleX = 0x80;
        gWork.aff[i].scaleY = 0x80;
    }
    w = &gWork;
    if ((u8)sub_0807EE9C(&w->unkAF5, gTurnOrderChoiceSubsteps[w->unkAF5]))
        return 1;
    if (w->unkAF5 <= 3) {
        if (w->unkABF != 0xFF) {
            TurnOrder_UpdateChoiceBob(w->unkABF, w->unkAC4);
            TurnOrder_DrawTurnChoice(w->unkABF, w->unkAF4, w->unkAC0, w->unkAC4, w->unkABF);
        }
    }
    for (i = 0; i <= 4; i++)
        ObjAffineApply(&gWork.aff[i]);
    OamListFlush(&gWork);
    OamListClear(&gWork);
    gWork.unkAF4++;
    return 0;
}
u16 TurnOrder_NopStep(void)
{
    return 1;
}
/* Load the three hand graphics, start the hand object, randomly pick the first hand. */
u16 TurnOrder_CpuChooseTurn(void)
{
    TurnOrder_LoadObjTiles(gDuelLogoTiles0, 0x10, 8, 8);
    TurnOrder_LoadObjTiles(gDuelLogoTiles1, 0x18, 8, 8);
    TurnOrder_LoadObjTiles(gDuelLogoTiles2, 0x110, 8, 8);
    TweenInit(0, 0, 0x40, 0xF, 3, 1, gWork.unkADC, 0);
    gWork.unkAF5 = 3;
    gWork.unkABF = Random() & 1;
    return 1;
}
u16 TurnOrder_RunRps(void)
{
    gWork.unkB0E = 0;
    if (gTurnOrderRpsSteps[gMain.seqIndex1] != 0) {
        if (sub_0807EE98())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
u16 TurnOrder_RunRpsLink(void)
{
    u32 v;

    gWork.unkB0E = 1;
    v = gTurnOrderRpsSteps[gMain.seqIndex1];
    /* FAKEMATCH: keep the callback address in r1 without emitting code. */
    __asm__("" : : : "r0");
    if (v != 0) {
        if (gMain.newKeys & 2) {
            SetMainCallback(0x08003AA5);
            return 0;
        }
        if (((u16 (*)(void))v)())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
u16 TurnOrder_RunPlayerChoice(void)
{
    if (gTurnOrderPlayerChoiceSteps[gMain.seqIndex1] != 0) {
        if (sub_0807EE98())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
u16 TurnOrder_RunPlayerChoiceLink(void)
{
    gWork.unkB0E = 1;
    if (gTurnOrderPlayerChoiceSteps[gMain.seqIndex1] != 0) {
        if (sub_0807EE98())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
u16 TurnOrder_RunCpuChoice(void)
{
    if (gTurnOrderCpuChoiceSteps[gMain.seqIndex1] != 0) {
        if (sub_0807EE98())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}
/* Fill the BG tile map buffers at gMain+0xC9C.. with a running tile counter (mode 1 starts 15 tiles earlier). */
void CardListView_DrawCursorFrame(u32 mode)
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
void TextDrawShadowedString(s32 x, s32 y, const u16 *str, s32 w)
{
    if (gSaveData.flags4 & 0x80) {
        while (*(u8 *)str != 0) {
            u16 ch = *str;
            ch = (ch >> 8) | ((u8)ch << 8);
            TextDrawSjisGlyph(ch, x + 1, y + 1, (u16)(((u8)w << 8) | 9));
            TextDrawSjisGlyph(ch, x, y, (u16)(((u8)w << 8) | 7));
            x += w;
            str++;
        }
    } else {
        const u8 *q = (const u8 *)str;
        while (*q != 0) {
            TextDrawLatinGlyph(*q, x + 1, y + 1, (u16)(((u8)w << 8) | 9));
            TextDrawLatinGlyph(*q, x, y, (u16)(((u8)w << 8) | 7));
            x += w / 2;
            q++;
        }
    }
}
void CardListView_DrawNames(struct Entry *list, s32 count)
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
    TextCanvasInit(0x20, 9);
    for (i = 0; i < 4 && i < count; i++) {
        /* Loop-invariant table pointer: loop.c hoists it, global alloc leaves it
           without a register, and reload rematerializes it at the use. */
        names = gCardNames;
        raw = *(u32 *)list;
        bits = raw << 20;
        e = *(struct Entry *)&raw;
        if (bits != 0) {
            s32 ok;
            ok = 1;
            if ((gCardListView.flags & 0xE0) == 0x60) {
                u8 *t;
                tab = gDuelBanishedInfo;
                t = &tab[(gCardListView.scroll + i) * 2 + 0xD64 * (((struct SelFlags *)&gCardListView)->b1 & 1)];
                if (*t == 2 && (*t & gCardListView.flags))
                    ok = 0;
            }
            if (ok) {
                const u16 *name = (const u16 *)((e.id << 6) + (u32)names);
                list++;
                TextDrawShadowedString(8, i * 16 + 7, name, 10);
            } else {
                TextDrawShadowedString(8, i * 16 + 7, gStrCardListViewUnknown, 10);
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
    selection = (u8 *)&gCardListView;
    base = (u8 *)&gMain;
    /* FAKEMATCH: keeps gMain + 0x49C unfused and orders the preheader as the ROM. */
    asm("" : "+r"(base) : "r"(i), "r"(bound), "r"(selection));
    for (; i <= bound; i++)
        *(u16 *)(base + 0x49C + i * 2) = i + 16;
    *selection |= 4;
}
extern const u32 gAttributeIconImages[];
extern const u32 gSpellSubtypeIconImages[];
extern const u32 gMonsterTypeIconImages[];
extern u16 gUnk_0300045C[];
void FillMapRect(u16 row, u16 col, u16 w, u16 h);
void DrawCardPortrait(u32 a, u32 b, u32 c, u32 d, u32 e);
void LoadBgImage4bpp(u16 a, u16 b, u16 c, const void *img);
void DrawBgDecimal(u32 a, u32 b, s32 val, u32 zero);

#define CARD_STATS(id) (gCardStats[(id) & 0x7FF])
#define CARD_TYPE2(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* Card table entry by integer address: each use reloads the table base literal. */
#define A188_STATS_C(id) (((u32 *)0x08621DE0)[(id) & 0x7FF])

/* Spell subtype (stats bits 17-19) for Magic cards, else 0 (cf. card_canvas). */
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
#define A188F ((struct A188Flags *)&gCardListView)

/* Card detail page: draw the frame/type icons and ATK/DEF/level of the selected entry. */
void CardListView_DrawCardInfo(u32 *entry)
{
    u32 *stats;
    u32 id;
    u32 b;
    int row;
    int i;
    int kind;

    id = ((struct Entry *)entry)->id;
    b = SELF->b1;
    row = gCardListView.scroll + gCardListView.sel;

    FillMapRect(0, 0x1A1, 0x13, 6);
    FillMapRect(3, 0x155, 0xA, 0xA);
    if ((gCardListView.flags & 0xE0) == 0x60) {
        u8 *players = gDuelPlayers;
        int r = row * 2;
        r += 0xD64 * b;
        players += 0xCC4;
        if (players[r] == 2 && b == 1)
            return;
    }

    A188F->b3 = 1 - A188F->b3;
    DrawCardPortrait(3, 0x155, id, A188F->b3 * 0xB4 + 0x178, (A188F->b3 * 4 + 8) * 16);

    if (id == 0)
        return;

    stats = &A188_STATS_C(id);
    kind = (*stats & 0x1F00000) >> 20;
    switch (kind) {
    case 0x15:
    case 0x16:
        LoadBgImage4bpp(0x1A1, 0x50, 0x130,
                     (const void *)(kind == 0x16 ? 0x08636DA0 : 0x08636CD8));
        if (A188_SpellSub(*stats))
            LoadBgImage4bpp(0x1A3, 0x60, 0x134,
                         (const void *)gSpellSubtypeIconImages[A188_SpellSub(A188_STATS_C(id))]);
        break;
    default: {
        u32 *p;
        LoadBgImage4bpp(0x1A1, 0x50, 0x130, (const void *)gAttributeIconImages[*(p = &A188_STATS_C(id)) >> 29]);
        LoadBgImage4bpp(0x1A3, 0x60, 0x134, (const void *)gMonsterTypeIconImages[(*p & 0x1F00000) >> 20]);
        DrawBgDecimal(0x701A7, 0x4013A, A188_Atk10(p, id), 0);
        DrawBgDecimal(0x701C7, 0x4013E, A188_Def10(id), 0);
        for (i = 0; i < A188_Level(id); i++)
            { int x = i & 7; int y = (i >> 3) + 0xD; x += 0xC; gUnk_0300045C[x + ((u16)y << 5)] = 2; }
        break;
    }
    }
}
void CardListView_DrawCardInfo(u32 *entry);
void CardListView_DrawSelectedInfo(void)
{
    CardListView_DrawCardInfo(&gCardListView.list[gCardListView.sel + gCardListView.scroll]);
}
void CardListView_DrawNames(struct Entry *list, s32 count);
void CardListView_DrawPage(void)
{
    CardListView_DrawNames((struct Entry *)&gCardListView.list[gCardListView.scroll], gCardListView.count - gCardListView.scroll);
}
void CardListView_DrawCursorFrame(u32 mode);
void CardListView_DrawSelectedCursorFrame(void)
{
    CardListView_DrawCursorFrame((gCardListView.list[gCardListView.sel + gCardListView.scroll] << 19) >> 31);
}
static inline u32 GetListStatusCardType(u16 id)
{
    return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
}
void CardListView_DrawCardStatus(struct Entry *e)
{
    u32 x = 8;
    u32 t;
    u32 mode;

    if ((s32)(*(u32 *)e << 12) < 0) {
        x |= 0x880000;
        AddSprite(x, 0x40, 0x1100);
        x = 24;
    }
    if ((gCardListView.flags & 0xE0) == 0x80) {
        switch (gCardListView.kind[gCardListView.sel + gCardListView.scroll]) {
        case 1:
            AddSprite(0x880000 | x, 0x40, 0x1106);
            x += 16;
            break;
        case 2:
            AddSprite(0x880000 | x, 0x40, 0x1108);
            x += 16;
            break;
        case 4:
            AddSprite(0x880000 | x, 0x40, 0x110A);
            x += 16;
            break;
        }
    }
    t = GetListStatusCardType(e->id);
    if (t <= 20 && (gCardListView.flags & 0xE0) != 0x40) {
        if (IsSpecialSummonOnly(e->id) && (((u8 *)e)[1] & 0xC0) == 0) {
            AddSprite(0x880000 | x, 0x40, 0x1102);
            x += 16;
        }
        mode = gCardListView.flags & 0xE0;
        if (mode == 0x60 || mode == 0 || mode == 0x20) {
            if ((s32)(*(u32 *)e << 11) < 0) {
                AddSprite(0x880000 | x, 0x40, 0x110C);
                x += 16;
            }
        }
    }
    {
        struct Sel *sel = &gCardListView;
        u32 flags = sel->flags;
        if ((flags & 0xE0) == 0x60) {
            u32 side = flags << 30;
            register int row __asm__("r0") = sel->sel + sel->scroll;
            u8 *players;

            /* FAKEMATCH: retain the initialized row in r0 before forming
             * the player-state base. This input hint emits no instructions. */
            __asm__("" : : "r"(row));
            players = gDuelPlayers;
            row *= 2;
            row += 0xD64 * (side >> 31);
            players += 0xCC4;
            switch (players[row]) {
            case 1:
                x |= 0x880000;
                AddSprite(x, 0x40, 0x1110);
                break;
            case 2:
                x |= 0x880000;
                AddSprite(x, 0x40, 0x110E);
                break;
            }
        }
    }
}

/* Draw up to 4 icons (bit i of mask = present), the one at sel highlighted with 3 sprites. */
void CardListView_DrawButtons(s32 sel, u16 mask)
{
    s32 y = 0x68;
    s32 i;

    for (i = 0; i <= 3; i++) {
        u16 t = i << 1;
        s32 a = t << 5;
        s32 b = a + 2;

        if ((mask >> i) & 1) {
            if (i == sel) {
                AddSprite(y, 0x40, a);
                AddSprite(0xA0, 0x4080, b + 2);
                AddSprite(0xC0, 0x4080, a + 8);
            } else {
                AddSprite(y, 0x40, b);
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
extern struct DuelFlags gDuelScreen;
extern const u8 gUnk_08698C7C[], gCardListViewButtonsGfx[], gUnk_0869AD1C[], gCardListViewStatusIconsGfx[];
extern const u8 gUnk_0869E8E4[], gUnk_0869C45C[], gCardListViewInfoPanelImage[], gUnk_0869EECC[];
extern const u8 gCardListViewCursorFrameGfx[], gUnk_0869B53C[], gCardListViewTitlesGfx[];
extern const u16 gStrCardListViewNoCards[];
u32 DuelScreen_FadeOutStep(void);
void LoadBgImage4bppMap1(u32 a, u32 b, u32 c, const void *src);
void ClearBgMapBuffers(void);
void ResetVideo(void);
void TextCanvasToTiles(void *dst, u32 v);
void CopyDoubleWords(void *dst, const void *src, u32 size);
void ResetBgScroll(void);
void SetBrightnessBlack(void);
u16 FadeFromBlack(u16 step);

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
#define LI ((struct ListInit *)&gCardListView)
struct MainLI {
    u8 f0[0x49C];
    u16 tiles[0x120];       /* +0x49C */
    u8 f6DC[0x4422 - 0x6DC];
    s16 ofs;                /* +0x4422 */
};
#define ML ((struct MainLI *)&gMain)

u16 CardListView_InitScreen(void)
{
    s32 i, j, k;
    u32 tbl;

    switch (LI->state) {
    case 0:
        if (DuelScreen_FadeOutStep()) {
            gDuelScreen.bit2 = 0;
            gDuelScreen.bit1 = 0;
            LI->state++;
        }
        return 0;
    case 1:
        ResetVideo();
        ClearBgMapBuffers();
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
        tbl = (u32)gCardListViewTitlesGfx;
        SetBrightnessBlack();
        ResetBgScroll();
        CopyDoubleWords((void *)0x05000200, gUnk_08698C7C, 0x20);
        CopyDoubleWords((void *)0x06010000, gCardListViewButtonsGfx, 0x2000);
        CopyDoubleWords((void *)0x05000220, gUnk_0869AD1C, 0x20);
        CopyDoubleWords((void *)0x06012000, gCardListViewStatusIconsGfx, 0x800);
        LoadBgImage4bppMap1(0x400, 0x10, 0x3C6, gUnk_0869E8E4);
        LoadBgImage4bppMap1(0x440, 0x20, 0x354, gUnk_0869C45C);
        LoadBgImage4bppMap1(0x560, 0x30, 0x2E0, gCardListViewInfoPanelImage);
        MemCopy16((void *)0x05000080, gUnk_0869EECC, 0x20);
        for (i = 0; i < 2; i++) {
            u16 s = i * 0x60;
            u16 d = i * 15;
            CopyDoubleWords((u8 *)0x06004000 + (d + 0x15A) * 32, gCardListViewCursorFrameGfx + s * 32, 0x100);
            CopyDoubleWords((u8 *)0x06004000 + (d + 0x162) * 32, gCardListViewCursorFrameGfx + (s + 0x1C) * 32, 0x40);
            CopyDoubleWords((u8 *)0x06004000 + (d + 0x164) * 32, gCardListViewCursorFrameGfx + (s + 0x20) * 32, 0x20);
            CopyDoubleWords((u8 *)0x06004000 + (d + 0x165) * 32, gCardListViewCursorFrameGfx + (s + 0x3D) * 32, 0x20);
            CopyDoubleWords((u8 *)0x06004000 + (d + 0x166) * 32, gCardListViewCursorFrameGfx + (s + 0x40) * 32, 0x40);
            CopyDoubleWords((u8 *)0x06004000 + (d + 0x168) * 32, gCardListViewCursorFrameGfx + (s + 0x5D) * 32, 0x20);
        }
        CopyDoubleWords((void *)0x050000E0, gUnk_0869B53C, 0x20);
        if (LI->mode <= 4) {
            CopyDoubleWords((void *)0x06006840, (const u8 *)(LI->mode * 0x300 + tbl), 0x300);
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
            CardListView_DrawPage();
            CardListView_DrawSelectedInfo();
            CardListView_DrawSelectedCursorFrame();
            ML->ofs = -(LI->sel * 16);
            LI->cur = 0;
            LI->mask |= 1;
        } else {
            TextCanvasInit(0x20, 9);
            TextDrawShadowedString(0x42, 0x18, gStrCardListViewNoCards, 0xC);
            for (k = 0; k < 0x120; k++)
                ML->tiles[k] = k + 0x10;
            TextCanvasToTiles((void *)0x06004200, 0);
            LI->cur = 1;
        }
        while (!((LI->mask >> LI->cur) & 1))
            LI->cur++;
        LI->b4 = 1;
        LI->state++;
        return 0;
    case 4:
        REG_DISPCNT |= 0x1F00;
        if (FadeFromBlack(4))
            LI->state++;
        return 0;
    }
    return 1;
}
