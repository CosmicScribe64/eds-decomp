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
    u8 unkE;                /* +0xE */
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

/* Draw the player's hand sprite (which = 0/1), bobbing with pos[0]; flags bit 3 picks the palette. */
void TurnOrder_DrawChosenTurnBanner(u32 unused0, u32 unused1, u16 flags, void *unused3, u8 which, s16 *pos)
{
    u32 w = 0x40;
    u32 h = 0x20;
    u32 *o;

    switch (which) {
    case 0:
        o = OamListAddSprite(0, gTurnChoiceBannerTileNums[0], 0x58, pos[1] - (MulFix8(pos[0], 0x2000) >> 8) + 0x50,
                         w, h, 4, 3, 0x200, 0, 0, 0, &gWork);
        *o |= (flags & 8) ? 0x08000500 : 0x08000100;
        break;
    case 1:
        o = OamListAddSprite(0, gTurnChoiceBannerTileNums[1], 0x58, pos[1] - (MulFix8(pos[0], 0x2000) >> 8) + 0x50,
                         w, h, 4, 3, 0x200, 0, 0, 0, &gWork);
        *o |= (flags & 8) ? 0x08000500 : 0x08000100;
        break;
    }
    gWork.aff[4].angle = 0;
    gWork.aff[4].scaleX = 0x100;
    gWork.aff[4].scaleY = pos[0];
}

/* Draw the three opponent hand sprites swinging by `angle`; `k` picks the spread (hypothesis). */
void TurnOrder_DrawDuelLogo(u8 unused, u8 angle, u8 k)
{
    u8 i;
    s32 c, s, a, a2, b, x, y, four = 4;
    u32 *o;

    for (i = 0; i < 3; i++) {
        c = SIN(angle + 0x40);
        a = MulFix8(c, 0x4000);
        /* `four` stops fold-const from reassociating (A*i - 4) - B into A*i - (B + 4) */
        four = 4;
        x = (s16)(a >> 8) * i - four - (MulFix8(0x60, c - SIN(0x134)) >> 8);
        s = SIN(angle);
        a2 = MulFix8(s, 0x4000);
        b = MulFix8(0x60, s - SIN(0xF4));
        y = (((a2 + 0xA) >> 8) * i - (b >> 8) + (MulFix8(0x4E0, gSquareTable[k]) >> 4) - 0x5E) & 0xFFFF;
        o = OamListAddSprite(0, gDuelLogoTileNums[i], x, y, 0x40, 0x40, 4, 0xA, 0x200, 0, 0, 0, &gWork);
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
    OamListAddSprite(0, gUnk_0808270C[k], 0x58, 0x64, w, h, 4, gUnk_08082710[k], 0x200, 0, 0, 0, &gWork);
}

u8 JudgeRockPaperScissors(u8 a, u8 b)
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
void TurnOrder_UpdateChoiceBob(u8 i, struct Pair *p)
{
    u8 j = i;
    if ((p[j].v += 0x20) > 0x800)
        p[j].v = 0x800;
    j ^= 1;
    if ((p[j].v -= 0x40) < 0)
        p[j].v = 0;
}

u8 TurnOrder_CpuPickTurn(u8 x)
{
    return x & 1;
}

/* Clear the work area and reset the display (step 0). */
u16 TurnOrder_Init(void)
{
    MemClear16(&gWork, sizeof(gWork));
    gMain.unk40E = 1;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= 0xE0FF;
    OamListClear(&gWork);
    SetBldAlpha(8);
    gWork.unkAF4 = 0;
    gWork.unkAF5 = 0;
    gWork.unkABF = 0xFF;
    gWork.unkB0D = 0;
    gWork.unkB1C = 0;
    gWork.unkB1D = 0;
    AnimBlockInit(gTurnOrderWaitAnimList, &gWork.grp[0]);
    gWork.grp[0].unkE = 0;
    ObjAffineInit(gWork.aff);
    gWork.unkB20 = 0;
    return 1;
}

/* Copy `rows` rows of `width` tiles into OBJ VRAM at `tile` (row stride 32 tiles). */
void TurnOrder_LoadObjTiles(const u8 *src, u32 tile, u32 width, s32 rows)
{
    u8 *dst = (u8 *)0x06014000 + tile * 32;
    s32 i;
    for (i = 0; i < rows; i++) {
        MemCopy16(dst, src, width * 32);
        dst += 0x400;
        src += width * 32;
    }
}

/* Load graphics and palettes and reset the hands (step 1). */
u16 TurnOrder_Load(void)
{
    u8 i;

    FadeStart(0, -0x180, 0, gWork.fade);
    MemCopy16((void *)VRAM, gEgyptCorridorBitmap, 0x9600);
    MemCopy16((void *)PLTT, gEgyptCorridorPal, 0x200);
    MemCopy16((void *)(PLTT + 0x200), gRockCardPal, 0x20);
    MemCopy16((void *)(PLTT + 0x220), gScissorsCardPal, 0x20);
    MemCopy16((void *)(PLTT + 0x240), gPaperCardPal, 0x20);
    MemCopy16((void *)(PLTT + 0x260), gTurnChoiceBannerPal, 0x20);
    MemCopy16((void *)(PLTT + 0x280), gSelectCardBannerPal, 0x20);
    MemCopy16((void *)(PLTT + 0x2A0), gWinBannerPal, 0x20);
    MemCopy16((void *)(PLTT + 0x2C0), gLoseBannerPal, 0x20);
    MemCopy16((void *)(PLTT + 0x2E0), gDrawBannerPal, 0x20);
    MemCopy16((void *)(PLTT + 0x320), gTurnChoiceBannerDimPal, 0x20);
    MemCopy16((void *)(PLTT + 0x340), gDuelLogoPal, 0x20);
    MemCopy16((void *)(PLTT + 0x3A0), gWaitSignPal, 0x20);
    TurnOrder_LoadObjTiles(gRockCardTiles, 0, 4, 8);
    TurnOrder_LoadObjTiles(gScissorsCardTiles, 4, 4, 8);
    TurnOrder_LoadObjTiles(gPaperCardTiles, 8, 4, 8);
    TurnOrder_LoadObjTiles(gTurnChoiceBannerTiles, 0x100, 0x10, 4);
    TurnOrder_LoadObjTiles(gSelectCardBannerTiles, 0x180, 0x10, 4);
    TurnOrder_LoadObjTiles(gWinBannerTiles, 0x10, 0x10, 4);
    TurnOrder_LoadObjTiles(gLoseBannerTiles, 0x90, 0x10, 4);
    TurnOrder_LoadObjTiles(gWaitSignTiles, 0x110, 4, 4);
    TurnOrder_LoadObjTiles(gDrawBannerTiles, 0x190, 0x10, 4);
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

/* Choose a hand (Left/Right, A); the CPU (or the link partner) answers. */
u16 TurnOrder_ChooseHand(void)
{
    struct Work20310 *w = &gWork;

    if (gWork.unkB0D == 0) {
        if ((gMain.newKeys & 1) && w->hands[0].offset == 0) {
            if (w->unkB0E == 1) {
                w->unkB0D = 1;
                LinkSyncStart(w->unkB10);
            } else {
                if (Random() % 500 < 100)
                    w->unkABC = w->hands[0].hand;
                else if (Random() & 1)
                    w->unkABC = (w->hands[0].hand + 1) % 3;
                else
                    w->unkABC = (w->hands[0].hand + 2) % 3;
                gWork.unkABD = 1;
                gWork.unkABE = JudgeRockPaperScissors(gWork.hands[0].hand, gWork.unkABC);
                gWork.unkAF5++;
                gWork.hands[1].offset = 4;
                gWork.unkB1E = 0;
            }
            PlaySE(1);
        } else if ((gMain.newKeys & 0x20) && gWork.hands[0].offset == 0) {
            gWork.hands[0].offset = -4;
            PlaySE(0);
        } else if ((gMain.newKeys & 0x10) && gWork.hands[0].offset == 0) {
            gWork.hands[0].offset = 4;
            PlaySE(0);
        }
    }
    if (gWork.unkB0D != 0) {
        if (LinkSyncStep(0x51, w->hands[0].hand, w->unkB10)) {
            gWork.hands[1].offset = 4;
            gWork.unkABD = 1;
            gWork.unkABC = gWork.unkB16;
            gWork.unkABE = JudgeRockPaperScissors(gWork.hands[0].hand, gWork.unkB16);
            gWork.unkAF5++;
            gWork.unkB0D = 0;
            LinkSyncStart(w->unkB10);
            TurnOrder_HideWaitSign();
            gWork.unkB1E = 0;
        } else {
            TurnOrder_ShowWaitSign();
        }
    }
    if (gWork.hands[1].offset == 0)
        TurnOrder_DrawBanner(0, gWork.unkAC0);
    return 0;
}

/* Result phase (step 3, hypothesis). Handles the A press and the link partner's answer (0x53), with a saturating unkB1E timer. */
u16 TurnOrder_ShowResult(void)
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
                PlaySE(1);
            } else if ((gMain.newKeys & 1) && gWork.unkABE == 0) {
                gWork.unkACC = 0;
                gWork.unkAC0 = 7;
                gWork.unkAF5++;
                REG_BLDALPHA = 0x10;
                REG_BLDY = 8;
                REG_BLDCNT = 0x440;
                SetBldAlpha(gWork.unkACC);
                gWork.unkABF = 0;
                PlaySE(1);
            } else if (gWork.unkABE == 1) {
                if (gWork.unkB0E == 1) {
                    gWork.unkB0D = 1;
                    LinkSyncStart(w->unkB10);
                } else if (gMain.newKeys & 1) {
                    gWork.unkABF = TurnOrder_CpuPickTurn(gWork.unkAF4);
                    gWork.unkAF5 += 2;
                    PlaySE(1);
                }
                TweenInit(0, 0, 0x40, 0xF, 3, 1, gWork.unkADC, 0);
                TurnOrder_AnimateTurnChoice(&gWork.unkAF5);
            }
        }
    }
    if (gWork.unkB0D == 1) {
        TurnOrder_ShowWaitSign();
        if (LinkSyncStep(0x52, w->hands[0].hand, w->unkB10)) {
            gWork.unkABF = 1 ^ *(u8 *)&gWork.unkB16;
            gWork.unkAF5 += 2;
            gWork.unkB0D = 0;
            TurnOrder_HideWaitSign();
        }
    }
    if (gWork.unkB0E == 1 && gWork.unkABE == 2) {
        if (LinkSyncStep(0x53, *q, w->unkB10)) {
            if (w->unkB16 == 2 || *q == 2) {
                gWork.hands[1].offset = -4;
                gWork.unkABD = 0;
                gWork.unkAF5--;
                gWork.unkB0D = 0;
                *q = 0;
                w->unkB1D = 0;
            } else {
                LinkSyncStart(w->unkB10);
                if (w->unkB1D != 0)
                    *q = 2;
            }
        }
    }
    if (gWork.unkABE == 2) {
        if (++gWork.unkB1E == 0) {
            gWork.unkB1E = 0xFF;
            if (LinkSyncStep(0x53, *q, w->unkB10)) {
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
u16 TurnOrder_ChooseTurn(void)
{
    struct Work20310 *w = &gWork;

    if (w->unkB0D == 0) {
        if ((gMain.newKeys & 0x20) && w->unkABF == 1) {
            w->unkABF = 0;
            PlaySE(0);
        } else if ((gMain.newKeys & 0x10) && gWork.unkABF == 0) {
            gWork.unkABF = 1;
            PlaySE(0);
        }
        if (gMain.newKeys & 1) {
            gWork.unkACC = 0x1000;
            TurnOrder_LoadObjTiles(gDuelLogoTiles0, 0x10, 8, 8);
            TurnOrder_LoadObjTiles(gDuelLogoTiles1, 0x18, 8, 8);
            TurnOrder_LoadObjTiles(gDuelLogoTiles2, 0x110, 8, 8);
            TweenInit(0, 0, 0x40, 0xF, 3, 1, gWork.unkADC, 0);
            if (gWork.unkB0E == 1) {
                gWork.unkB0D = 1;
                LinkSyncStart(w->unkB10);
            } else {
                TurnOrder_AnimateTurnChoice(&gWork.unkAF5);
                gWork.unkAF5++;
            }
            PlaySE(1);
        }
    }
    gWork.unkACC += 0x80;
    if (gWork.unkACC > 0x1000)
        gWork.unkACC = 0x1000;
    SetBldAlpha(gWork.unkACC >> 8);
    if (gWork.unkB0D != 0 && LinkSyncStep(0x52, gWork.unkABF, w->unkB10)) {
        gWork.unkAF5++;
        gWork.unkB0D = 0;
        TurnOrder_AnimateTurnChoice(&gWork.unkAF5);
    }
    return 0;
}

u16 TurnOrder_AnimateTurnChoice(u8 *step)
{
    TweenUpdate(gWork.unkADC);
    TurnOrder_DrawTurnChoiceConfirm(gWork.unkABF, gWork.unkAF4, gWork.unkAC0, gWork.unkAC4, gWork.unkABF, gWork.unkADC);
    if (gWork.unkAF0 == 2) {
        TurnOrder_LoadObjTiles(gDuelLogoTiles0, 0x10, 8, 8);
        TurnOrder_LoadObjTiles(gDuelLogoTiles1, 0x18, 8, 8);
        TurnOrder_LoadObjTiles(gDuelLogoTiles2, 0x110, 8, 8);
        TweenInit(0x100, 0, 0x100, 0, 1, 0, gWork.unkADC, 0);
        (*step)++;
    }
    return 0;
}

u16 TurnOrder_ShowDuelLogo(void)
{
    if (gWork.unkACF < 0x10)
        gWork.unkACF++;
    if (gWork.unkACF >= 0x10) {
        if (gWork.unkACE < 0) {
            if ((u8)gWork.unkACE == 0xF4)
                PlaySE(0x2A);
            gWork.unkACE += 3;
        } else {
            gWork.unkACE = 0;
            FadeOutBGM();
        }
    }
    if (gWork.unkB20++ == 90.0 || (gMain.newKeys & 1)) {
        SetBldY(gWork.unkAD0);
        REG_BLDCNT = 0xBF;
        gWork.unkAD2 = 0x300;
        gWork.unkAF5++;
        Timer_Reset(&gWork.timer);
        FadeOutBGM();
    }
    if (gWork.unkACF == 0xF && gWork.unkADC[1] == 0)
        gWork.unkADC[0] = 0xC0;
    if (gWork.unkACE > -10 && gWork.unkADC[1] == 0)
        TweenInit(0xC0, 0, 0x100, 0x38, 6, 0x14, gWork.unkADC, 0);
    TweenUpdate(gWork.unkADC);
    TurnOrder_DrawChosenTurnBanner(gWork.unkABF, gWork.unkAF4, gWork.unkAC0, gWork.unkAC4, gWork.unkABF, gWork.unkADC);
    TurnOrder_DrawDuelLogo(gWork.unkABF, gWork.unkACE, gWork.unkACF);
    return 0;
}

u16 TurnOrder_FlashWhite(void)
{
    gWork.unkAD0 += gWork.unkAD2;
    if (gWork.timer.state == 2) {
        gWork.timer.state = 0;
        gWork.unkAD0 = 0;
        gWork.unkAD2 = 0x60;
        gWork.unkAF5++;
        SetBldY(0);
        REG_BLDCNT = 0xFF;
        return 0;
    }
    if (gWork.timer.state != 1 && gWork.unkAD0 == 0xC00)
        gWork.unkAD2 = 0x100;
    if (gWork.timer.state != 1 && gWork.unkAD0 > 0x10FF) {
        CpuFastFill(-1, (void *)PLTT, 0x200);
        CpuFastFill(-1, (void *)(PLTT + 0x200), 0x200);
        Timer_Start(&gWork.timer, 0x14);
    }
    SetBldY(gWork.unkAD0 >> 8);
    TurnOrder_DrawDuelLogo(gWork.unkABF, gWork.unkACE, gWork.unkACF);
    Timer_Tick(&gWork.timer);
    return 0;
}
