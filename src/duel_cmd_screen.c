#include "global.h"
#include "main.h"
#include "duel.h"
#include "duel_ui.h"

/*
 * Duel command handlers (continued from duel_cmd_deck): each one runs from the
 * command dispatcher, reads its operands from the command block at 0x020185C0,
 * and clears the "running" flag (bit 5 of byte +0x80D) when it is done.
 * Multi-frame handlers keep their state in the 7-bit `step` (+0x80A) and the
 * 7-bit `timer` (+0x80C bits 5-11).  See wiki/functions/code-080162c4.md.
 */

/* gMain (0x03000040) comes from main.h. */
#define gMain gMain

/* Duel command block gDuelCmd comes from duel_ui.h. */

/* Per-player duel state (gDuelPlayers) and the duel state (gDuel) come from duel.h. */
/* gDuel addressed through the gDuelPlayers symbol (lets CSE share the base, see DuelCmd_ResetDuelState). */
#define DUEL_FROM_PLAYERS ((struct DuelState *)((u8 *)gDuelPlayers - 4))

/* Duel screen / animation state gDuelScreen comes from duel_ui.h. */

/* Message box request block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 filler0[6];
    u16 arg;                /* +0x006 */
};
extern struct DuelMsg gDuelScene;

/*
 * Local view for DuelCmd_SetFieldBackground only. It touches gDuel+0x1ACC as a u8 bitfield
 * (unk1ACC_0:4) and the ROM uses ldrb/strb there. duel.h models that word as
 * `u32 unk1ACC_0:15`, which makes agbcc emit ldrh/strh, so the unit keeps its own split.
 */
struct DuelStateUnk1ACCView {
    u32 unk0;                       /* +0x0000 */
    struct DuelPlayer players[2];   /* +0x0004 */
    u8 unk1ACC_0:4;                 /* +0x1ACC (unit-local split; canonical is u32:15) */
    u8 unk1ACC_4:4;
};
extern struct DuelStateUnk1ACCView gUnk_020192E0_lo asm("gDuel");

extern u8 gChain[];
extern const u8 gDuelBannerPal[];
extern const u8 gStartDuelBannerGfx[];
extern const u16 gShrinkScaleSteps[];
extern const u8 gPhaseBannerPal[];
extern const u8 gPhaseBannerGfx[];
extern const u8 gBattlePhaseBannerGfx[];
extern const u8 gChainBannerPal[];
extern const u8 gChainBannerGfx[];
extern const u16 gBannerSlideOffsets[];   /* slide offsets, 16 entries */
extern const u16 gPulseScaleCurve[];   /* pulse scale, 16 entries */
extern const u32 gBattleBannerSlideX[];   /* slide-in x offsets, 16 entries */

#define REG_BLDCNT (*(vu16 *)0x04000050)
#define REG_BLDALPHA (*(vu16 *)0x04000052)
#define BLDALPHA(eva, evb) ((u8)(eva) | ((u8)(evb) << 8))

void CopyDoubleWords(void *dst, const void *src, u32 size);   /* CopyDoubleWords */
void MemClear16(void *dst, u32 size);                    /* MemClear16 */
void PlaySE(u16 se);                                 /* PlaySE */
void AddSprite(u32 yx, u16 shapeSize, u16 attr2);       /* AddSprite */
void AddSpriteAlpha(u32 yx, u16 shapeSize, u16 attr2);
void AddAffineSprite(u32 yx, u16 shapeSize, u16 attr2, s32 affine);
void DuelCursor_Select(u32 player, u32 a, u32 b);
void DuelScreen_ScrollToZone(u32 player, u32 a);
void DuelScene_Start(u16 msg, u32 a);
u32 DuelScene_Run(void);
void PlayDuelBGM(void);
void DrawPhaseIndicator(u32 a, u32 b);
u32 DuelFieldFadeToWhite(u32 a);
u32 DuelFieldFadeFromWhite(u32 a);
void DuelScreen_Init(void);
u32 DuelScreen_FadeInStep(void);
u32 DuelScreen_FadeOutStep(void);
void DuelScreen_LoadFieldBackground(u16 a);

#define FAST_FORWARD() ((gMain.heldKeys & 2) || gDuelScreen.fast)

/*
 * Restart the duel. Save both players' list7C4/listA44 (and their counts) into the
 * command block, clear the whole duel state and gChain, restore the lists,
 * and reset both players to 8000 LP.
 */
void DuelCmd_ResetDuelState(void)
{
    int i;

    for (i = 0; i < 2; i++) {
        gDuelCmd.savedCount3[i] = gDuelPlayers[i & 1].deckCount;
        CopyDoubleWords(gDuelCmd.savedList7C4[i], gDuelPlayers[i & 1].deck, 0x140);
        gDuelCmd.savedCount5[i] = gDuelPlayers[i & 1].fusionCount;
        CopyDoubleWords(gDuelCmd.savedListA44[i], gDuelPlayers[i & 1].fusionDeck, 0x140);
    }
    MemClear16(gChain, 0x56C);
    MemClear16(gDuelPlayers, 0x1B0C);
    for (i = 0; i < 2; i++) {
        gDuelPlayers[i & 1].deckCount = gDuelCmd.savedCount3[i];
        CopyDoubleWords(gDuelPlayers[i & 1].deck, gDuelCmd.savedList7C4[i], 0x140);
        gDuelPlayers[i & 1].fusionCount = gDuelCmd.savedCount5[i];
        CopyDoubleWords(gDuelPlayers[i & 1].fusionDeck, gDuelCmd.savedListA44[i], 0x140);
    }
    gDuelPlayers[0].lifePoints = 8000;
    gDuelPlayers[1].lifePoints = 8000;
    DUEL_FROM_PLAYERS->phase1B12 = 7;
    gDuelCmd.running = 0;
}

void DuelCmd_OpenDuelScreen(void)
{
    if (!gDuelScreen.flag0_2)
        DuelScreen_Init();
    else if (DuelScreen_FadeInStep())
        gDuelCmd.running = 0;
}

void DuelCmd_CloseDuelScreen(void)
{
    if (DuelScreen_FadeOutStep())
        gDuelCmd.running = 0;
}

void DuelCmd_SetFieldBackground(void)
{
    DuelScreen_LoadFieldBackground(gDuelCmd.arg2);
    gUnk_020192E0_lo.unk1ACC_0 = gDuelCmd.arg2;
    gDuelCmd.running = 0;
}

/*
 * Duel-start banner: slide the two halves in (gBattleBannerSlideX offsets), run
 * DuelFieldFadeToWhite / DuelFieldFadeFromWhite (4x speed when fast-forwarding), slide them out,
 * then set phase1B12 = 3 and notify DrawPhaseIndicator / DuelCursor_Select.
 */
void DuelCmd_EnterBattlePhase(void)
{
    switch (gDuelCmd.step) {
    case 0:
        CopyDoubleWords((void *)0x050003E0, gPhaseBannerPal, 0x20);
        CopyDoubleWords((void *)0x06016C80, gBattlePhaseBannerGfx, 0x200);
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.timer = 16;
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.timer > 0) {
            gDuelCmd.timer--;
            if (FAST_FORWARD() && gDuelCmd.timer > 4)
                gDuelCmd.timer -= 3;
            AddSprite(gBattleBannerSlideX[gDuelCmd.timer] | 0x400000, 0x4080, 0xF364);
            AddSprite((0xD0 - gBattleBannerSlideX[gDuelCmd.timer]) | 0x400000, 0x4080, 0xF36C);
            break;
        }
        PlaySE(0x1D);
        gDuelCmd.step++;
    case 2:
        AddSprite(0x00400058, 0x4080, 0xF364);
        AddSprite(0x00400078, 0x4080, 0xF36C);
        if (DuelFieldFadeToWhite(FAST_FORWARD() ? 4 : 1))
            gDuelCmd.step++;
        break;
    case 3:
        AddSprite(0x00400058, 0x4080, 0xF364);
        AddSprite(0x00400078, 0x4080, 0xF36C);
        if (DuelFieldFadeFromWhite(FAST_FORWARD() ? 4 : 1)) {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 4:
        if (gDuelCmd.timer < 16) {
            AddSprite(gBattleBannerSlideX[gDuelCmd.timer] | 0x400000, 0x4080, 0xF364);
            AddSprite((0xD0 - gBattleBannerSlideX[gDuelCmd.timer]) | 0x400000, 0x4080, 0xF36C);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0xB)
                gDuelCmd.timer += 3;
            break;
        }
        gDuelCmd.step++;
    default:
        gDuel.phase1B12 = 3;
        DrawPhaseIndicator(gDuel.linkSkip, gDuel.phase1B12);
        DuelCursor_Select(gDuel.linkSkip, 0, 0);
        gDuelCmd.running = 0;
        break;
    }
}
/* +0x80C view; the larger container preserves the original halfword RMW. */
struct BannerTimer {
    u32 reserved : 5;
    u32 timer : 7;
    u32 remainder : 20;
    u32 trailing;
};
void DuelCmd_EnterPhase(u32 kind)
{
    u32 step = gDuelCmd.step;
    int blend;

    switch (step) {
    case 0:
        CopyDoubleWords((void *)0x050003E0, gPhaseBannerPal, 0x20);
        CopyDoubleWords((void *)0x06016C80, gPhaseBannerGfx + kind * 0x200, 0x200);
        {
            u8 *base = (u8 *)&gDuelCmd;
            /* FAKEMATCH: preserve the initialized offset and RMW pointer roles. */
            register u32 offset asm("r3") = 0x80C;
            register struct BannerTimer *timer asm("r1");
            asm("" : "+r"(offset));
            timer = (struct BannerTimer *)base;
            timer = (struct BannerTimer *)((u32)timer + offset);
            timer->timer = 0;
        }
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.timer < 0x60) {
            blend = 0;
            if (gDuelCmd.timer < 16) {
                REG_BLDCNT = 0xF40;
                REG_BLDALPHA = BLDALPHA(gDuelCmd.timer, 16 - gDuelCmd.timer);
                blend = 1;
            }
            if (gDuelCmd.timer >= 0x50) {
                REG_BLDCNT = 0xF40;
                REG_BLDALPHA = BLDALPHA(0x60 - gDuelCmd.timer, gDuelCmd.timer - 0x50);
                blend = 1;
            }
            if (!blend) {
                REG_BLDCNT = 0;
                REG_BLDALPHA = 0;
            }
            AddSpriteAlpha(0x00400058, 0x4080, 0xF364);
            AddSpriteAlpha(0x00400078, 0x4080, 0xF36C);
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x57)
                gDuelCmd.timer += 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step = step + 1;
    default:
        gDuel.phase1B12 = kind;
        DrawPhaseIndicator(gDuel.linkSkip, gDuel.phase1B12);
        gDuelCmd.running = 0;
        break;
    }
}

/* Banner that slides in from the acting player's side, pulses for 0x40 frames, then slides back out (cf. DuelCmd_ShowJustAMomentBanner). */
void DuelCmd_ShowChainBanner(void)
{
    u32 player = gDuelCmd.cmd >> 15;
    s32 step = gDuelCmd.step;
    s32 timer;
    s32 y;

    switch (step) {
    case 0:
        gDuelScreen.busy = 0;
        CopyDoubleWords((void *)0x050003E0, gChainBannerPal, 0x20);
        CopyDoubleWords((void *)0x06016C80, gChainBannerGfx, 0x400);
        gDuelScreen.cb85C = (void (*)(void))step;
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        PlaySE(0x16);
        break;
    case 1:
        timer = gDuelCmd.timer;
        if (timer < 16) {
            if (player)
                y = gBannerSlideOffsets[timer];
            else
                y = 0x80 - gBannerSlideOffsets[timer];
            AddSprite((y << 16) | 0x58, 0x40C0, 0xF364);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0xB)
                gDuelCmd.timer += 3;
            break;
        }
        gDuelCmd.timer = 0;
        gDuelCmd.step = step + 1;
        PlaySE(0x16);
    case 2:
        if (gDuelCmd.timer < 0x40) {
            AddAffineSprite(0x00400058, 0x40C0, 0xF364, gPulseScaleCurve[gDuelCmd.timer & 0xF] << 16);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x37)
                gDuelCmd.timer += 7;
            break;
        }
        gDuelCmd.timer = 16;
        gDuelCmd.step++;
    case 3:
        timer = gDuelCmd.timer;
        if (timer != 0) {
            if (player)
                y = gBannerSlideOffsets[timer - 1];
            else
                y = 0x80 - gBannerSlideOffsets[timer - 1];
            AddSprite((y << 16) | 0x58, 0x40C0, 0xF364);
            gDuelCmd.timer--;
            if (FAST_FORWARD() && gDuelCmd.timer > 4)
                gDuelCmd.timer -= 3;
            break;
        }
    default:
        gDuelCmd.running = 0;
        break;
    }
}
void DuelCmd_PointAtCard(void)
{
    u32 player = gDuelCmd.cmd >> 15;
    u32 arg2 = gDuelCmd.arg2;
    u32 lo = (u8)gDuelCmd.arg4;
    u32 hi = gDuelCmd.arg4 >> 8;
    u32 step = gDuelCmd.step;

    switch (step) {
    case 0:
        gDuelScreen.busy = 1;
        DuelCursor_Select(arg2, lo, hi);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 1:
        gDuelScreen.busy = 0;
        if (gDuelCmd.timer < 64) {
            AddAffineSprite((gDuelScreen.cursorX + 8) | ((gDuelScreen.cursorY - gDuelScreen.scroll) << 16),
                         0x80, 0, (gShrinkScaleSteps[gDuelCmd.timer & 7] << 16) | (player << 6));
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x37)
                gDuelCmd.timer += 7;
        } else {
            gDuelCmd.step = step + 1;
        }
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_MoveCursor(void)
{
    u32 player = gDuelCmd.cmd >> 15;
    u32 arg2 = gDuelCmd.arg2;
    u32 arg4 = gDuelCmd.arg4;
    gDuelScreen.busy = 1;
    DuelCursor_Select(player, arg2, arg4);
    gDuelCmd.running = 0;
}

void DuelCmd_StartDuelBanner(void)
{
    u32 scale;

    switch (gDuelCmd.step) {
    case 0:
        PlaySE(0xB);
        CopyDoubleWords((void *)0x050003E0, gDuelBannerPal, 0x20);
        CopyDoubleWords((void *)0x06016C80, gStartDuelBannerGfx, 0x400);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.timer != 0 && gDuelCmd.timer <= 15) {
            scale = gDuelCmd.timer << 20;
            AddAffineSprite(0x00300058, 0x40C0, 0xF364, scale + 0x100000);
        }
        if (gDuelCmd.timer >= 16 && gDuelCmd.timer <= 96)
            AddSprite(0x00300058, 0x40C0, 0xF364);
        scale = gDuelCmd.timer;
        if (scale > 96 && scale < 128) {
            scale -= 0x60;
            AddAffineSprite(0x00300058, 0x40C0, 0xF364, scale << 24);
            if (gDuelCmd.timer == 0x7F)
                PlayDuelBGM();
        }
        gDuelCmd.timer++;
        if (gDuelCmd.timer == 0)
            gDuelCmd.step++;
        break;
    default:
        gDuel.flag1B12_0 = 1;
        gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_ExodiaWinScene(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(1, 0);
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_Run())
            gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_DestinyBoardWinScene(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(5, 0);
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_Run())
            gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_TossCoin(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(0, 0);
        gDuelScene.arg = (gDuelCmd.arg2 << 15) | 0x180 | (u8)gDuelCmd.arg4;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_Run())
            gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_TossThreeCoins(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(0, 0);
        gDuelScene.arg = 0x380 | (u8)gDuelCmd.arg2;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_Run())
            gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_RollGracefulDice(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(2, 0);
        gDuelScene.arg = gDuelCmd.arg2;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_Run())
            gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_RollSkullDice(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(3, 0);
        gDuelScene.arg = gDuelCmd.arg2;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_Run())
            gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_RollPlainDie(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(4, 0);
        gDuelScene.arg = gDuelCmd.arg2;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_Run())
            gDuelCmd.running = 0;
        break;
    }
}
