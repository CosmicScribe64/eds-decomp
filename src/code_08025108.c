#include "global.h"
#include "gba.h"

/* One token of the toss screen (12 bytes), see code_0802408C. */
struct TossSlot {
    u8 timer;           /* +0x0 */
    u8 frame;           /* +0x1: animation frame / result face */
    u8 state;           /* +0x2: 1 = flying, 2 = landed, 3 = winner */
    u8 unk3;
    s16 y;              /* +0x4 */
    u16 t;              /* +0x6 */
    u8 resultTimer;     /* +0x8 */
    u8 resultFrame;     /* +0x9: 0-4 */
    u8 resultState;     /* +0xA: 1 = animating, 2 = done */
    u8 unkB;
};

/* Sparkle particle (8 bytes). */
struct Sparkle {
    u32 active:1;       /* +0 bit 0 (u32 container: tested with lsl #31) */
    u8 timer:2;         /* +0 bits 1-2 */
    u8 frame:5;         /* +0 bits 3-7: index into gCoinSparkleTiles (0 entry ends) */
    u8 unk1[3];
    u8 x;               /* +4 */
    u8 y;               /* +5 */
    u8 unk6[2];
};

/* Sparkle pool at 0x02015DD0 (= toss work area +0xB50). */
struct Sparkles {
    struct Sparkle p[32];
    u8 next;            /* +0x100: next slot (mod 32) */
};

extern u8 gCoinTossWork[];          /* toss work area (OAM builder argument) */
extern const u16 gCoinSparkleTiles[];   /* sparkle tile per frame, 0-terminated */

/* Sprite group: list of 4-halfword OAM templates. */
struct SprGroup {
    u32 unk0;
    u16 *list;          /* +0x4: count entries of 4 halfwords */
    u32 unk8;
    u8 count;           /* +0xC */
    u8 unkD;
    u8 unkE;            /* +0xE */
    u8 unkF;
};
/* Value ramp driven by Ease_Init/Ease_Start (init) and Ease_Tick (tick). */
struct Ramp {
    u8 state;           /* +0: 2 = finished */
    u8 unk1;
    s16 cur;            /* +2 */
    s16 target;         /* +4 */
    s16 unk6;
};

/* Screen work area at 0x0201F820 (0xAF0 bytes, fields used here). */
struct Work1F820 {
    u8 unk0[0x618];
    u8 unk618[0x918 - 0x618];   /* +0x618: ObjAffineInit */
    struct SprGroup grp918;     /* +0x918: die sprite (hypothesis) */
    u8 filler928[4];
    struct SprGroup grp92C;     /* +0x92C */
    u8 filler93C[0xABC - 0x93C];
    u8 unkABC[6];       /* +0xABC: object for FadeTick */
    u8 unkAC2;          /* +0xAC2: 2 = finished */
    u8 unkAC3;
    u8 step;            /* +0xAC4: index into gDiceScreenSteps / gPlainDieScreenSteps */
    u8 unkAC5;
    u8 unkAC6;
    u8 unkAC7;
    u8 unkAC8;
    u8 unkAC9;
    u8 result;          /* +0xACA: copied to 0x02017A30+8 at the end */
    u8 unkACB;
    u8 unkACC;
    u8 unkACD;          /* +0xACD */
    u8 unkACE[2];
    struct Ramp rampAD0;    /* +0xAD0 */
    struct Ramp rampAD8;    /* +0xAD8 */
    struct Ramp rampAE0;    /* +0xAE0 */
    u16 unkAE8;
    u8 unkAEA;          /* +0xAEA: mode 0/1/2 (2 = no sprites) */
    u8 unkAEB;
    u16 unkAEC;
    u16 unkAEE;
};
extern struct Work1F820 gDiceScreen;

/* Message/sequence block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 unk0[6];
    u16 opts;           /* +0x6 */
    u16 arg8;           /* +0x8 */
    u8 unkA;
    u8 step;            /* +0xB */
};
extern struct DuelMsg gDuelScene;


/* gMain (0x03000040): only the fields used here. */
struct Main {
    u32 rngState;       /* +0x000 */
    u16 heldKeys;       /* +0x004 */
    u16 newKeys;        /* +0x006 */
    u8 unk8[0x40E - 8];
    u16 unk40E;         /* +0x40E */
};
extern struct Main gMain;
#define gMain gMain

extern const u8 gDieFacePaths[6][2][2];
extern const u8 gDieAxisFaces[][4];
void MemClear16(void *dst, u32 size); /* MemClear16 */
void ObjAffineInit(void *p);
void Ease_Init(u32 a, u32 b, u32 c, struct Ramp *ramp);
void Ease_Tick(struct Ramp *ramp);
s32 MulFix8(s32 a, s32 b);     /* 8.8 fixed-point multiply */
extern const s16 gSineTable[];   /* sine table, 256 entries */
struct DieFrame { u8 unk0; u8 count; u16 unk2; u16 *list; };
extern struct DieFrame *const gDieRollFrames[];
extern const struct DieFrame gDieTumbleFrames[];
extern const u8 gSkullDiceCharAnims[];
extern const u8 gGracefulDiceCharAnims[];
extern const u8 gEgyptCorridorBitmap[];
extern const u8 gEgyptCorridorPal[];
extern const u8 gDiceSceneObjTilesLeft[];
extern const u8 gDiceSceneObjTilesRight[];
extern const u8 gDiceSceneObjPal[];
void AnimBlockInit(const void *a, void *b);
void FadeStart(u32 a, u32 b, u32 c, void *p);
void CopyTileSheetTo2D(const void *src, void *dst, u32 n);
void PlaySE(u32 se);
void Ease_Start(u32 a, u32 b, u32 c, struct Ramp *ramp);
extern u16 (*const gDiceScreenSteps[])(void);
extern u16 (*const gPlainDieScreenSteps[])(void);
extern u16 (*const gSkullDiceSceneSteps[])(void);
u16 *OamListAlloc(u8 a, void *work);
void AnimStateTick(void *p);
void FadeTick(void *p);
void OamListFlush(void *p);
void OamListClear(void *p);
s32 Random(void);             /* random */
u32 *OamListAddSprite(u32 a, u32 tile, s32 x, s32 y, u32 w, u32 h, u32 a6, u32 a7, u32 a8,
                  u32 a9, u32 a10, u32 a11, void *work);


void CoinToss_AnimateHighlights(struct TossSlot *s, u8 n)
{
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].resultState == 1) {
            if (s[i].resultTimer == 0) {
                s[i].resultTimer = 6;
                if (++s[i].resultFrame == 5) {
                    s[i].resultFrame = 0;
                    s[i].resultState++;
                }
            } else {
                s[i].resultTimer--;
            }
        }
    }
}

void CoinToss_MarkMatchingCoins(struct TossSlot *s, u8 n, u8 face)
{
    u8 flying = 0;
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].state != 2)
            flying++;
    }
    if (flying == 0) {
        for (i = 0; i < n; i++) {
            if (s[i].frame == face) {
                s[i].resultState = 1;
                s[i].state = 3;
            } else {
                s[i].resultState = 2;
            }
        }
    }
}

u8 CoinToss_CountUnfinished(struct TossSlot *s, u8 n)
{
    u8 count = 0;
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].resultState != 2)
            count++;
    }
    return count;
}

u32 CoinToss_SpawnSparkle(u8 x, u8 y, struct Sparkles *sp)
{
    u8 k = sp->next++ % 32;
    s32 r = Random();
    struct Sparkle *p = &sp->p[k];

    p->x = x + r % 16;
    p->y = y;
    p->active = 1;
    p->timer = 0;
    p->frame = 0;
    return 1;
}

u32 CoinToss_UpdateSparkles(struct Sparkles *sp)
{
    u8 i;

    for (i = 0; i < 32; i++) {
        struct Sparkle *p = &sp->p[i];
        if (p->active) {
            if (--p->timer == 3) {
                p->timer = 0;
                if (gCoinSparkleTiles[++p->frame] == 0)
                    p->active = 0;
            }
        }
    }
}

u32 CoinToss_DrawSparkles(struct Sparkles *sp)
{
    u8 i;

    for (i = 0; i < 32; i++) {
        struct Sparkle *p = &sp->p[i];
        if (p->active) {
            u32 *oam = OamListAddSprite(1, gCoinSparkleTiles[p->frame] + 0x200, p->x, p->y,
                                    16, 16, 4, 1, 0, 0, 0, 0, gCoinTossWork);
            *oam |= 0x400;
        }
    }
}

void CoinToss_ClearSparkles(struct Sparkles *sp)
{
    u8 i;

    for (i = 0; i < 32; i++)
        sp->p[i].active = 0;
    sp->next = 0;
}

/* K&R definition: callers pass the u16/u8 arguments without narrowing them. */
u16 *DiceScreen_AddOamPiece(src, a, x, y, pal, tileBase, work)
    u16 *src;
    u8 a;
    u16 x;
    u16 y;
    u16 pal;
    u16 tileBase;
    void *work;
{
    u16 *oam = OamListAlloc(a, work);
    u16 *ret = oam;

    oam[0] = (src[0] & 0xFF00) | ((y + (src[0] & 0xFF)) & 0xFF);
    oam[1] = (src[1] & 0xFE00) | ((x + (src[1] & 0x1FF)) & 0x1FF);
    oam[2] = (src[2] & 0xFC0F) | (tileBase + ((src[2] & 0xF0) << 1));
    if (src[2] & 0x100)
        oam[2] += 0x10;
    if ((src[2] & 0xF000) == 0)
        oam[2] |= pal << 12;
    return ret;
}

void DiceScreen_DrawCharacter(g, x, y, attr0)
    struct SprGroup *g;
    u16 x;
    u16 y;
    u16 attr0;
{
    u8 i;

    if (gDiceScreen.unkAEA != 2) {
        for (i = 0; i < g->count; i++) {
            u16 *oam = DiceScreen_AddOamPiece(g->list + i * 4, 1, x, y, 0, 0x200, &gDiceScreen);
            *oam |= attr0;
        }
    }
}

void DiceScreen_TickCharacter(void *p)
{
    if (gDiceScreen.unkAEA != 2)
        AnimStateTick(p);
}
u32 DiceScreen_Init(void)
{
    gMain.unk40E = 1;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= 0xE0FF;
    OamListClear(&gDiceScreen);
    ObjAffineInit(gDiceScreen.unk618);
    gDiceScreen.step = 0;
    gDiceScreen.unkAC5 = 0;
    gDiceScreen.unkAC6 = 0;
    gDiceScreen.unkACB = 0xFF;
    gDiceScreen.unkACC = 0;
    Ease_Init(0, 0, 0, &gDiceScreen.rampAD0);
    Ease_Start(0, 0x100, 2, &gDiceScreen.rampAD8);
    Ease_Init(0, 0, 0, &gDiceScreen.rampAE0);
    gDiceScreen.unkAE8 = 0x1600;
    gDiceScreen.unkAEC = 0x4000;
    gDiceScreen.unkAEE = 0x2B0;
    return 1;
}
u32 DiceScreen_LoadGraphics(void)
{
    switch (gDiceScreen.unkAEA) {
    case 0:
        AnimBlockInit(gSkullDiceCharAnims, &gDiceScreen.grp918);
        break;
    case 1:
        AnimBlockInit(gGracefulDiceCharAnims, &gDiceScreen.grp918);
        break;
    case 2:
        AnimBlockInit(gGracefulDiceCharAnims, &gDiceScreen.grp918);
        gDiceScreen.rampAD8.cur = 0x100;
        gDiceScreen.rampAD8.target = 0x100;
        gDiceScreen.rampAD8.state = 0;
        Ease_Start(0, 0xF, 1, &gDiceScreen.rampAE0);
        gDiceScreen.step = 1;
        break;
    }
    FadeStart(0, -0x180, 0, gDiceScreen.unkABC);
    CpuSet(gEgyptCorridorBitmap, (void *)0x06000000, 0x4B00);
    CpuSet(gEgyptCorridorPal, (void *)0x05000000, 0x100);
    CopyTileSheetTo2D(gDiceSceneObjTilesLeft, (void *)0x06014000, 0x10);
    CopyTileSheetTo2D(gDiceSceneObjTilesRight, (void *)0x06014200, 0x10);
    CpuSet(gDiceSceneObjPal, (void *)0x05000200, 0x100);
    REG_DISPCNT = 0x1F04;
    return 1;
}
u32 DiceScreen_ThrowDie(void)
{
    u16 x, y;
    s16 z;
    s32 bob;
    struct SprGroup *g;
    u8 i;

    x = MulFix8(0x100, gSineTable[gDiceScreen.rampAD8.cur & 0xFF]) >> 4;
    y = (gDiceScreen.rampAD8.cur - 0xBE) / 2
        + (MulFix8(0x80, gSineTable[((gDiceScreen.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gDiceScreen.unkACC == 0) {
        Ease_Start(0, 10000, 4, &gDiceScreen.rampAD0);
        gDiceScreen.step = 2;
        gDiceScreen.unkACC = 1;
    }
    if (gDiceScreen.rampAD0.state)
        gDiceScreen.unkAC5++;
    if (gDiceScreen.step == 2) {
        z = -60 - ((gDiceScreen.rampAD0.cur * (100 - gDiceScreen.rampAD0.cur)) >> 8);
        for (i = 0; i < gDieTumbleFrames[(gDiceScreen.unkAC5 >> 2) & 7].count; i++)
            DiceScreen_AddOamPiece(gDieTumbleFrames[(gDiceScreen.unkAC5 >> 2) & 7].list + i * 4,
                         0, 0x68, z + 0x64, gDiceScreen.unkACD, 0x200, &gDiceScreen);
    } else {
        z = -((gDiceScreen.rampAD0.cur * (150 - gDiceScreen.rampAD0.cur)) >> 8);
        for (i = 0; i < gDieRollFrames[gDiceScreen.unkAC6][(gDiceScreen.unkAC5 >> 2) & 7].count; i++)
            DiceScreen_AddOamPiece(gDieRollFrames[gDiceScreen.unkAC6][(gDiceScreen.unkAC5 >> 2) & 7].list + i * 4,
                         0, 0x68, z + 0x64, gDiceScreen.unkACD, 0x200, &gDiceScreen);
    }
    g = &gDiceScreen.grp918;
    g->unkE = 1;
    DiceScreen_TickCharacter(g);
    bob = (MulFix8(0x100, gSineTable[(gDiceScreen.unkAC5 * 2 + 0x20) & 0xFF]) >> 4) + 8;
    DiceScreen_DrawCharacter(g, x + 0x68, y - bob, 0);
    Ease_Tick(&gDiceScreen.rampAD0);
    if (z > 0) {
        gDiceScreen.rampAD0.cur = 0;
        gDiceScreen.rampAD0.state = 0;
        if (gDiceScreen.step == 2) {
            Ease_Start(0, 10000, 4, &gDiceScreen.rampAD0);
            gDiceScreen.unkAC6 = Random() % 2;
        } else {
            gDiceScreen.unkAC8 = gDiceScreen.unkAC9;
            gDiceScreen.unkAC6 = gDiceScreen.unkAC7;
            Ease_Start(gDiceScreen.unkAC8, gDiceScreen.unkAC8 + 10, 1, &gDiceScreen.rampAD0);
        }
        PlaySE(0x1F);
        REG_BLDALPHA = 0x1010;
        REG_BLDY = 8;
        REG_BLDCNT = 0x3F40;
        return 1;
    }
    return 0;
}
u32 DiceScreen_RollToResult(void)
{
    u16 x, y;
    u16 dx;
    struct SprGroup *g;
    u16 dy;
    s32 sx, sx2;
    s32 bob;
    u8 i;

    x = MulFix8(0x100, gSineTable[gDiceScreen.rampAD8.cur & 0xFF]) >> 4;
    y = (gDiceScreen.rampAD8.cur - 0xBE) / 2
        + (MulFix8(0x80, gSineTable[((gDiceScreen.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gMain.newKeys & 1) {
        FadeStart(0, 0x180, 0, gDiceScreen.unkABC);
        return 1;
    }
    if (gDiceScreen.rampAD0.state == 0) {
        FadeStart(0, 0x180, 0, gDiceScreen.unkABC);
        return 1;
    }
    if (gDiceScreen.rampAD0.state == 2) {
        if (--gDiceScreen.unkACB == 0xFF)
            gDiceScreen.rampAD0.state = 0;
    }
    for (i = 0; i < gDieRollFrames[gDiceScreen.unkAC6][gDiceScreen.rampAD0.cur % 20].count; i++)
        DiceScreen_AddOamPiece(gDieRollFrames[gDiceScreen.unkAC6][gDiceScreen.rampAD0.cur % 20].list + i * 4,
                     0, 0x68, (gDiceScreen.rampAD0.cur - gDiceScreen.unkAC8) * 2 + 0x64,
                     gDiceScreen.unkACD, 0x200, &gDiceScreen);
    dx = 0;
    dy = 0;
    if (gDiceScreen.rampAD0.state == 2) {
        g = &gDiceScreen.grp92C;
        if (gDiceScreen.unkAE8 != 0) {
            if (gDiceScreen.unkAE8 <= 0x1000)
                REG_BLDALPHA = gDiceScreen.unkAE8 >> 8;
            gDiceScreen.unkAE8 -= 0x18;
        }
        if (gDiceScreen.unkAEA == 1) {
            gDiceScreen.unkAEC += gDiceScreen.unkAEE;
            gDiceScreen.unkAEE += 8;
            dx = MulFix8(0x2000, gSineTable[(gDiceScreen.unkAEC >> 8) + 0x40]) >> 8;
            dy = (MulFix8(0x1400, gSineTable[gDiceScreen.unkAEC >> 8]) >> 8) - 0x14;
        }
    } else if (gDiceScreen.rampAD0.state != 0) {
        g = &gDiceScreen.grp918;
        if (gDiceScreen.rampAD0.cur % 3 == 0)
            PlaySE(0x1F);
    }
    g->unkE = 1;
    y = (gDiceScreen.rampAD8.cur - 0xBE) / 2
        + (MulFix8(0x80, gSineTable[((gDiceScreen.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gDiceScreen.rampAD0.state) {
        DiceScreen_TickCharacter(g);
        sx = dx + 0x68;
        sx2 = x + sx;
        bob = (MulFix8(0x100, gSineTable[(gDiceScreen.unkAC5 * 2 + 0x20) & 0xFF]) >> 4) + 8;
        DiceScreen_DrawCharacter(g, sx2, y - bob + dy, 0x400);
    }
    Ease_Tick(&gDiceScreen.rampAD0);
    return 0;
}
u32 DiceScreen_CharacterEnter(void)
{
    u16 x, y;
    struct SprGroup *g;
    u8 i;

    x = MulFix8(0x100, gSineTable[gDiceScreen.rampAD8.cur & 0xFF]) >> 4;
    y = (gDiceScreen.rampAD8.cur - 0xBE) / 2
        + (MulFix8(0x80, gSineTable[((gDiceScreen.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    g = &gDiceScreen.grp918;
    g->unkE = 1;
    DiceScreen_TickCharacter(g);
    DiceScreen_DrawCharacter(g, x + 0x68, y - 8, 0);
    for (i = 0; i < gDieRollFrames[gDiceScreen.unkAC6][gDiceScreen.rampAD0.cur % 20].count; i++)
        DiceScreen_AddOamPiece(gDieRollFrames[gDiceScreen.unkAC6][gDiceScreen.rampAD0.cur % 20].list + i * 4,
                     0, x + 0x68, y + 0x14, gDiceScreen.unkACD, 0x200, &gDiceScreen);
    Ease_Tick(&gDiceScreen.rampAD8);
    if (gDiceScreen.rampAD8.state == 2) {
        gDiceScreen.rampAD8.state = 0;
        Ease_Start(0, 0xF, 1, &gDiceScreen.rampAE0);
        return 1;
    }
    return 0;
}
u32 DiceScreen_HoldDie(void)
{
    u16 x, y;
    struct SprGroup *g;
    u8 i;

    x = MulFix8(0x100, gSineTable[gDiceScreen.rampAD8.cur & 0xFF]) >> 4;
    y = (gDiceScreen.rampAD8.cur - 0xBE) / 2
        + (MulFix8(0x80, gSineTable[((gDiceScreen.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gDiceScreen.unkAEA != 2) {
        g = &gDiceScreen.grp918;
        g->unkE = 1;
        DiceScreen_TickCharacter(g);
        for (i = 0; i < g->count; i++)
            DiceScreen_AddOamPiece(g->list + i * 4, 1, x + 0x68, y - 8, 0, 0x200, &gDiceScreen);
    }
    for (i = 0; i < gDieRollFrames[gDiceScreen.unkAC6][gDiceScreen.rampAD0.cur % 20].count; i++)
        DiceScreen_AddOamPiece(gDieRollFrames[gDiceScreen.unkAC6][gDiceScreen.rampAD0.cur % 20].list + i * 4,
                     0, x + 0x68, y + 0x14, gDiceScreen.unkACD, 0x200, &gDiceScreen);
    Ease_Tick(&gDiceScreen.rampAE0);
    if (gDiceScreen.rampAE0.state == 2) {
        gDiceScreen.rampAE0.state = 0;
        PlaySE(0x1F);
        return 1;
    }
    return 0;
}
u32 DiceScreen_Update(void)
{
    FadeTick(gDiceScreen.unkABC);
    if (gDiceScreen.unkAC2 == 2) {
        gDuelScene.arg8 = gDiceScreen.result;
        return 1;
    }
    if (gDiceScreen.unkAEA == 2) {
        if (gPlainDieScreenSteps[gDiceScreen.step]) {
            if (gPlainDieScreenSteps[gDiceScreen.step]())
                gDiceScreen.step++;
        }
    } else {
        if (gDiceScreenSteps[gDiceScreen.step]) {
            if (gDiceScreenSteps[gDiceScreen.step]())
                gDiceScreen.step++;
        }
    }
    OamListFlush(&gDiceScreen);
    OamListClear(&gDiceScreen);
    return 0;
}

u32 DiceScreen_SetupSkullDice(void)
{
    gDiceScreen.unkAEA = 0;
    gDiceScreen.unkACD = 2;
    return 1;
}

u32 DiceScreen_SetupGracefulDice(void)
{
    gDiceScreen.unkAEA = 1;
    gDiceScreen.unkACD = 0;
    return 1;
}

u32 DiceScreen_SetupPlainDie(void)
{
    gDiceScreen.unkAEA = 2;
    gDiceScreen.unkACD = 0;
    return 1;
}
u32 DiceScreen_PrepareRoll(void)
{
    struct Work1F820 *w = &gDiceScreen;
    u8 side;
    u16 n;
    u8 t;
    u8 raw;
    int sum;
    u8 *result;
    MemClear16(w, sizeof(*w));
    side = Random() % 2;
    n = gDuelScene.opts;
    result = &w->result;
    *result = n;
    w->unkAC7 = gDieFacePaths[(n - 1) % 6][side][0];
    raw = gDieFacePaths[(*result - 1) % 6][side][1];
    sum = raw + 2;
    t = sum % 4;
    w->unkAC9 = t * 5 + 2;
    *result = gDieAxisFaces[w->unkAC7][(t + 2) % 4];
    /* FAKEMATCH: keep the initialized result address live through the last store. */
    asm volatile ("" : : "r"(result));
    return 1;
}

u32 DuelScene_SkullDice(void)
{
    if (gSkullDiceSceneSteps[gDuelScene.step]) {
        if (gSkullDiceSceneSteps[gDuelScene.step]())
            gDuelScene.step++;
        return 0;
    }
    return 1;
}
