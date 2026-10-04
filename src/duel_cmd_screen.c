/*
 * duel_cmd_screen (0x080162C4-0x08017313): duel command handlers for the duel screen, the banners and the
 * full-screen scenes (wiki/functions/duel-cmd-screen-c.md).
 *
 * DuelCmd_Dispatch calls one of these once per frame while gDuelCmd.running is set. Each reads its operands
 * from gDuelCmd (cmd bit 15 = acting player, arg2/arg4), runs a small state machine on gDuelCmd.step with
 * gDuelCmd.timer as its frame counter, and clears running when it is done. The banners hold B (or
 * gDuelScreen.fast) to fast-forward.
 *
 *  - Duel start: DUEL_CMD_RESET_DUEL_STATE, DUEL_CMD_START_DUEL_BANNER.
 *  - Duel screen: open (fade in), close (fade out), field background.
 *  - Banners: the phase banners (DUEL_CMD_DRAW_PHASE .. DUEL_CMD_END_PHASE), the Battle Phase banner, the
 *    "Chain" banner; the hand pointer (DUEL_CMD_POINT_AT_CARD) and the field cursor.
 *  - Scenes (duel_scenes.h): the Exodia and Destiny Board win scenes, the coin tosses and the dice. The
 *    command starts the scene with DuelScene_Start, sets gDuelScene.arg and waits for DuelScene_Run.
 */
#include "global.h"
#include "gba.h"                    /* REG_BLDCNT, REG_BLDALPHA, BLDCNT_*, OBJ_PLTT, OBJ_VRAM0, B_BUTTON */
#include "main.h"                   /* gMain.heldKeys */
#include "duel.h"                   /* gDuel, gDuelPlayers, struct DuelState / DuelPlayer / DuelCard */
#include "sound.h"                  /* PlaySE */
#include "constants/duel.h"         /* enum DuelPhase, DUEL_AREA_MONSTER */
#include "constants/duel_cmds.h"    /* enum DuelCmdId */
#include "constants/sound.h"        /* SE_DUEL_START_BANNER, SE_BANNER_SLIDE, SE_BATTLE_PHASE_BANNER */
#include "duel_cmd.h"       /* gDuelCmd, gBannerSlideOffsets, gShrinkScaleSteps, gDuelBannerPal */
#include "duel_screen.h"    /* gDuelScreen, DuelScreen_*, DuelCursor_Select, DrawPhaseIndicator, DuelFieldFade* */
#include "duel_scenes.h"    /* gDuelScene, DuelScene_Start, enum DuelSceneId */
#include "duel_flow.h"      /* PlayDuelBGM, gPulseScaleCurve */
#include "chain.h"          /* gChain */
#include "sprite.h"         /* AddSprite, AddSpriteAlpha, AddAffineSprite, enum SpriteShape */
#include "util.h"           /* CopyDoubleWords, MemClear16 */

/* ROM data used only here. */
extern const u8 gPhaseBannerPal[];          /* 0x0867F01C: OBJ palette of the phase banners */
extern const u8 gPhaseBannerGfx[];          /* 0x0867F03C: six 0x200-byte 32x16 banners, by enum DuelPhase */
extern const u8 gBattlePhaseBannerGfx[];    /* 0x0867F63C = gPhaseBannerGfx + PHASE_BATTLE * 0x200 (own symbol) */
extern const u8 gStartDuelBannerGfx[];      /* 0x08687BBC: "Start Duel", 64x32 */
extern const u8 gChainBannerPal[];          /* 0x08688FBC */
extern const u8 gChainBannerGfx[];          /* 0x08688FD8: "Chain", 64x32 */
/* 0x08081728: [16] x of the left half of the Battle Phase banner, 88 at [0] easing out to 32 at [15]; the
 * right half is drawn at 0xD0 - x, so the halves slide together from x = 32 / 176 to 88 / 120. */
extern const u32 gBattleBannerSlideX[];

/*
 * Matching: u32-returning views of DuelScreen_FadeInStep (called by DuelCmd_OpenDuelScreen) and DuelScene_Run
 * (called by the scene commands). Both are declared and defined as returning u16, and with those prototypes
 * every call adds a narrowing of r0 (the unit grows by 0x20 bytes); the ROM tests the raw result.
 */
u32 DuelScreen_FadeInStepU32(void) asm("DuelScreen_FadeInStep");
u32 DuelScene_RunU32(void) asm("DuelScene_Run");

/*
 * gDuel reached through the gDuelPlayers symbol (gDuelPlayers - 4). Matching: DuelCmd_ResetDuelState forms
 * the gDuel.phase address from the gDuelPlayers base it already holds; gDuel.phase would load a second
 * literal.
 */
#define DUEL_FROM_PLAYERS ((struct DuelState *)((u8 *)gDuelPlayers - OFFSET_OF(struct DuelState, players)))

/* Banners and the hand pointer fast-forward while B is held or the duel screen runs in fast mode. */
#define FAST_FORWARD() ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)

/* Acting player of the current command: bit 15 of the command word (DUEL_CMD_PLAYER). */
#define CMD_PLAYER (gDuelCmd.cmd >> 15)

/*
 * Every banner loads its palette into OBJ palette 15 and its tiles at OBJ tile 0x364 (the shared banner slot
 * of duel_cmd.h); a 64x32 banner is one sprite, a 32x16 phase banner is drawn as two halves (tiles 0x364
 * and 0x36C).
 */
#define BANNER_PAL      ((void *)(OBJ_PLTT + DUEL_BANNER_PAL_SLOT * 0x20))      /* 0x050003E0 */
#define BANNER_GFX      ((void *)(OBJ_VRAM0 + DUEL_BANNER_OBJ_TILE * 0x20))     /* 0x06016C80 */
#define BANNER_ATTR2_L  DUEL_BANNER_ATTR2           /* attr2 0xF364: palette 15, tile 0x364 (left half or whole) */
#define BANNER_ATTR2_R  (DUEL_BANNER_ATTR2 + 8)     /* attr2 0xF36C: palette 15, tile 0x36C (right half) */

/* AddSprite position argument: y << 16 | x. */
#define SPRITE_YX(y, x) (((y) << 16) | (x))

/* Phase banner fade: alpha blend over BG0-BG3 as 2nd targets (semi-transparent sprites are always a 1st
 * target), 0xF40. */
#define BANNER_BLDCNT (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BG3)

/* BLDALPHA value: EVA (1st target weight) in bits 0-4, EVB (2nd target) in bits 8-12. Matching: each weight
 * is narrowed to u8 as the ROM does (gba.h's BLDALPHA_BLEND has no casts). */
#define BLDALPHA(eva, evb) ((u8)(eva) | ((u8)(evb) << 8))

/*
 * DUEL_CMD_RESET_DUEL_STATE (0x10), pushed once at duel start before the opening draws: clear gChain and
 * both players (with the rule flags and the Prohibition list, up to gDuel.turnCount) but keep the decks and
 * fusion decks that duel setup loaded and shuffled; they wait in gDuelCmd across the clear. Both players
 * start at 8000 LP and the phase becomes PHASE_NONE.
 */
void DuelCmd_ResetDuelState(void)
{
    int i;

    for (i = 0; i < 2; i++) {
        gDuelCmd.savedDeckCount[i] = gDuelPlayers[i & 1].deckCount;
        CopyDoubleWords(gDuelCmd.savedDeck[i], gDuelPlayers[i & 1].deck, sizeof(gDuelCmd.savedDeck[i]));
        gDuelCmd.savedFusionCount[i] = gDuelPlayers[i & 1].fusionCount;
        CopyDoubleWords(gDuelCmd.savedFusionDeck[i], gDuelPlayers[i & 1].fusionDeck,
                        sizeof(gDuelCmd.savedFusionDeck[i]));
    }
    MemClear16(&gChain, sizeof(gChain));
    MemClear16(gDuelPlayers, OFFSET_OF(struct DuelState, turnCount) - OFFSET_OF(struct DuelState, players));
    for (i = 0; i < 2; i++) {
        gDuelPlayers[i & 1].deckCount = gDuelCmd.savedDeckCount[i];
        CopyDoubleWords(gDuelPlayers[i & 1].deck, gDuelCmd.savedDeck[i], sizeof(gDuelCmd.savedDeck[i]));
        gDuelPlayers[i & 1].fusionCount = gDuelCmd.savedFusionCount[i];
        CopyDoubleWords(gDuelPlayers[i & 1].fusionDeck, gDuelCmd.savedFusionDeck[i],
                        sizeof(gDuelCmd.savedFusionDeck[i]));
    }
    gDuelPlayers[0].lifePoints = 8000;
    gDuelPlayers[1].lifePoints = 8000;
    DUEL_FROM_PLAYERS->phase = PHASE_NONE;
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_OPEN_DUEL_SCREEN (0x12): set up the duel screen if it is not active, then fade it in. Pushed at
 * duel start and after every full-screen scene.
 */
void DuelCmd_OpenDuelScreen(void)
{
    if (!gDuelScreen.active)
        DuelScreen_Init();
    else if (DuelScreen_FadeInStepU32())
        gDuelCmd.running = 0;
}

/* DUEL_CMD_CLOSE_DUEL_SCREEN (0x13): fade the duel screen out and leave it (before a full-screen scene). */
void DuelCmd_CloseDuelScreen(void)
{
    if (DuelScreen_FadeOutStep())
        gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_SET_FIELD_BACKGROUND (0x11): load field background arg2 (0 none, 1-14 a Field Magic's
 * texture) and remember it in gDuel.fieldBackground, which DuelScreen_Init re-applies.
 */
void DuelCmd_SetFieldBackground(void)
{
    DuelScreen_LoadFieldBackground(gDuelCmd.arg2);
    gDuel.fieldBackground = gDuelCmd.arg2;
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_BATTLE_PHASE (0x53): the Battle Phase banner. Steps:
 *   0  load the banner, scroll the field to player 0's monster row
 *   1  the two 32x16 halves slide in from the sides (gBattleBannerSlideX), then SE_BATTLE_PHASE_BANNER
 *   2  flash the field to white (4x speed when fast-forwarding)
 *   3  fade back from white
 *   4  the halves slide out
 *   5+ gDuel.phase = PHASE_BATTLE, mark it in the phase indicator, put the cursor on the turn player's
 *      first monster zone
 */
void DuelCmd_EnterBattlePhase(void)
{
    switch (gDuelCmd.step) {
    case 0:
        CopyDoubleWords(BANNER_PAL, gPhaseBannerPal, 0x20);
        CopyDoubleWords(BANNER_GFX, gBattlePhaseBannerGfx, 0x200);
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.timer = 16;
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.timer > 0) {
            gDuelCmd.timer--;
            if (FAST_FORWARD() && gDuelCmd.timer > 4)
                gDuelCmd.timer -= 3;
            AddSprite(gBattleBannerSlideX[gDuelCmd.timer] | SPRITE_YX(0x40, 0), SPRITE_SHAPE_32x16,
                      BANNER_ATTR2_L);
            AddSprite((0xD0 - gBattleBannerSlideX[gDuelCmd.timer]) | SPRITE_YX(0x40, 0), SPRITE_SHAPE_32x16,
                      BANNER_ATTR2_R);
            break;
        }
        PlaySE(SE_BATTLE_PHASE_BANNER);
        gDuelCmd.step++;
        /* fall through */
    case 2:
        AddSprite(SPRITE_YX(0x40, 0x58), SPRITE_SHAPE_32x16, BANNER_ATTR2_L);
        AddSprite(SPRITE_YX(0x40, 0x78), SPRITE_SHAPE_32x16, BANNER_ATTR2_R);
        if (DuelFieldFadeToWhite(FAST_FORWARD() ? 4 : 1))
            gDuelCmd.step++;
        break;
    case 3:
        AddSprite(SPRITE_YX(0x40, 0x58), SPRITE_SHAPE_32x16, BANNER_ATTR2_L);
        AddSprite(SPRITE_YX(0x40, 0x78), SPRITE_SHAPE_32x16, BANNER_ATTR2_R);
        if (DuelFieldFadeFromWhite(FAST_FORWARD() ? 4 : 1)) {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 4:
        if (gDuelCmd.timer < 16) {
            AddSprite(gBattleBannerSlideX[gDuelCmd.timer] | SPRITE_YX(0x40, 0), SPRITE_SHAPE_32x16,
                      BANNER_ATTR2_L);
            AddSprite((0xD0 - gBattleBannerSlideX[gDuelCmd.timer]) | SPRITE_YX(0x40, 0), SPRITE_SHAPE_32x16,
                      BANNER_ATTR2_R);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 11)
                gDuelCmd.timer += 3;
            break;
        }
        gDuelCmd.step++;
        /* fall through */
    default:
        gDuel.phase = PHASE_BATTLE;
        DrawPhaseIndicator(gDuel.turnPlayer, gDuel.phase);
        DuelCursor_Select(gDuel.turnPlayer, DUEL_AREA_MONSTER, 0);
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * gDuelCmd's +0x80C word with the timer in a u32 container, for the timer clear in DuelCmd_EnterPhase
 * (struct DuelCmd has the same bits; this view only serves the FAKEMATCH there).
 */
struct DuelCmdTimerWord {
    u32 unk80C_0:5;
    u32 timer:7;                    /* +0x80C bits 5-11: struct DuelCmd.timer */
    u32 unk80C_12:20;
    u32 unk810;
};

/*
 * DUEL_CMD_DRAW_PHASE .. DUEL_CMD_END_PHASE except the Battle Phase (0x50-0x52, 0x54, 0x55; the dispatcher
 * passes the enum DuelPhase): the phase banner gPhaseBannerGfx[phase] fades in over 16 frames, holds, and
 * fades out over the last 16 of 0x60 frames (OBJ alpha blend over BG0-BG3; +8 frames per frame when
 * fast-forwarding). Then gDuel.phase = phase and the phase indicator shows it.
 */
void DuelCmd_EnterPhase(u32 phase)
{
    u32 step = gDuelCmd.step;
    int blending;

    switch (step) {
    case 0:
        CopyDoubleWords(BANNER_PAL, gPhaseBannerPal, 0x20);
        CopyDoubleWords(BANNER_GFX, gPhaseBannerGfx + phase * 0x200, 0x200);
        {
            /*
             * gDuelCmd.timer = 0. FAKEMATCH: the ROM clears the timer with a halfword read-modify-write
             * through base + 0x80C with the offset in r3 and the pointer in r1; only this pinned form
             * (offset kept live by the empty asm, pointer built from the base, then the offset added)
             * reproduces that allocation and also case 1's (wiki: duel-cmd-screen-c.md).
             */
            u8 *base = (u8 *)&gDuelCmd;
            register u32 offset asm("r3") = 0x80C;
            register struct DuelCmdTimerWord *timerWord asm("r1");
            asm("" : "+r"(offset));
            timerWord = (struct DuelCmdTimerWord *)base;
            timerWord = (struct DuelCmdTimerWord *)((u32)timerWord + offset);
            timerWord->timer = 0;
        }
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.timer < 0x60) {
            blending = FALSE;
            if (gDuelCmd.timer < 16) {
                /* fade in: EVA rises 0 -> 15 */
                REG_BLDCNT = BANNER_BLDCNT;
                REG_BLDALPHA = BLDALPHA(gDuelCmd.timer, 16 - gDuelCmd.timer);
                blending = TRUE;
            }
            if (gDuelCmd.timer >= 0x50) {
                /* fade out over the last 16 frames */
                REG_BLDCNT = BANNER_BLDCNT;
                REG_BLDALPHA = BLDALPHA(0x60 - gDuelCmd.timer, gDuelCmd.timer - 0x50);
                blending = TRUE;
            }
            if (!blending) {
                REG_BLDCNT = 0;
                REG_BLDALPHA = 0;
            }
            AddSpriteAlpha(SPRITE_YX(0x40, 0x58), SPRITE_SHAPE_32x16, BANNER_ATTR2_L);
            AddSpriteAlpha(SPRITE_YX(0x40, 0x78), SPRITE_SHAPE_32x16, BANNER_ATTR2_R);
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x57)
                gDuelCmd.timer += 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step = step + 1;
        /* fall through */
    default:
        gDuel.phase = phase;
        DrawPhaseIndicator(gDuel.turnPlayer, gDuel.phase);
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_CHAIN_BANNER (0x07): the "Chain" banner (64x32) slides in vertically from the acting player's
 * side (player 1 from the top, player 0 from the bottom) to y = 0x40, pulses for 0x40 frames
 * (gPulseScaleCurve), then slides back out. Same shape as DuelCmd_ShowJustAMomentBanner. Steps:
 *   0  hide the cursor, load the banner, clear the field overlay callback, SE_BANNER_SLIDE
 *   1  slide in over 16 frames, then SE_BANNER_SLIDE
 *   2  pulse
 *   3  slide out
 */
void DuelCmd_ShowChainBanner(void)
{
    u32 player = CMD_PLAYER;
    s32 step = gDuelCmd.step;
    s32 timer;
    s32 y;

    switch (step) {
    case 0:
        gDuelScreen.showCursor = 0;
        CopyDoubleWords(BANNER_PAL, gChainBannerPal, 0x20);
        CopyDoubleWords(BANNER_GFX, gChainBannerGfx, 0x400);
        gDuelScreen.overlayCallback = NULL;
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        PlaySE(SE_BANNER_SLIDE);
        break;
    case 1:
        timer = gDuelCmd.timer;
        if (timer < 16) {
            if (player)
                y = gBannerSlideOffsets[timer];
            else
                y = 0x80 - gBannerSlideOffsets[timer];
            AddSprite(SPRITE_YX(y, 0x58), SPRITE_SHAPE_64x32, BANNER_ATTR2_L);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 11)
                gDuelCmd.timer += 3;
            break;
        }
        gDuelCmd.timer = 0;
        gDuelCmd.step = step + 1;
        PlaySE(SE_BANNER_SLIDE);
        /* fall through */
    case 2:
        if (gDuelCmd.timer < 0x40) {
            AddAffineSprite(SPRITE_YX(0x40, 0x58), SPRITE_SHAPE_64x32, BANNER_ATTR2_L,
                            gPulseScaleCurve[gDuelCmd.timer & 0xF] << 16);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x37)
                gDuelCmd.timer += 7;
            break;
        }
        gDuelCmd.timer = 16;
        gDuelCmd.step++;
        /* fall through */
    case 3:
        timer = gDuelCmd.timer;
        if (timer != 0) {
            if (player)
                y = gBannerSlideOffsets[timer - 1];
            else
                y = 0x80 - gBannerSlideOffsets[timer - 1];
            AddSprite(SPRITE_YX(y, 0x58), SPRITE_SHAPE_64x32, BANNER_ATTR2_L);
            gDuelCmd.timer--;
            if (FAST_FORWARD() && gDuelCmd.timer > 4)
                gDuelCmd.timer -= 3;
            break;
        }
        /* fall through */
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_POINT_AT_CARD (0x08): select arg2 = player, arg4 = area | index << 8 with the field cursor,
 * then draw the pulsing hand pointer (gShrinkScaleSteps) over the cursor for 64 frames (+8 per frame when
 * fast-forwarding), turned half a turn when the acting player is player 1. Effects use it to show the card
 * they target.
 */
void DuelCmd_PointAtCard(void)
{
    u32 player = CMD_PLAYER;
    u32 targetPlayer = gDuelCmd.arg2;
    u32 area = (u8)gDuelCmd.arg4;
    u32 index = gDuelCmd.arg4 >> 8;
    u32 step = gDuelCmd.step;

    switch (step) {
    case 0:
        gDuelScreen.showCursor = 1;
        DuelCursor_Select(targetPlayer, area, index);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 1:
        gDuelScreen.showCursor = 0;
        if (gDuelCmd.timer < 64) {
            /* scaleAngle: scale << 16 | angle (0x40 = half of the 128-step turn) */
            AddAffineSprite((gDuelScreen.cursorX + 8) | ((gDuelScreen.cursorY - gDuelScreen.scroll) << 16),
                            SPRITE_SHAPE_32x32, 0,
                            (gShrinkScaleSteps[gDuelCmd.timer & 7] << 16) | (player << 6));
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

/* DUEL_CMD_MOVE_CURSOR (0x09): show the field cursor on the acting player's area arg2, index arg4. */
void DuelCmd_MoveCursor(void)
{
    u32 player = CMD_PLAYER;
    u32 area = gDuelCmd.arg2;
    u32 index = gDuelCmd.arg4;

    gDuelScreen.showCursor = 1;
    DuelCursor_Select(player, area, index);
    gDuelCmd.running = 0;
}

/*
 * DUEL_CMD_START_DUEL_BANNER (0x14): plays SE_DUEL_START_BANNER, then shows the "Start Duel" banner over 0x100
 * frames: frames 1-15 it zooms from large to normal size, 16-96 it holds, 97-127 it shrinks away; frame 0x7F
 * calls PlayDuelBGM, which fades the pre-duel music out (bgmOn is still clear). When the 7-bit timer wraps to 0
 * the duel BGM is allowed (gDuel.bgmOn = 1).
 */
void DuelCmd_StartDuelBanner(void)
{
    u32 scale;

    switch (gDuelCmd.step) {
    case 0:
        PlaySE(SE_DUEL_START_BANNER);
        CopyDoubleWords(BANNER_PAL, gDuelBannerPal, 0x20);
        CopyDoubleWords(BANNER_GFX, gStartDuelBannerGfx, 0x400);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.timer != 0 && gDuelCmd.timer <= 15) {
            /* scaleAngle = (timer + 1) * 0x10 << 16: scale 0x20 .. 0x100 */
            scale = gDuelCmd.timer << 20;
            AddAffineSprite(SPRITE_YX(0x30, 0x58), SPRITE_SHAPE_64x32, BANNER_ATTR2_L, scale + 0x100000);
        }
        if (gDuelCmd.timer >= 16 && gDuelCmd.timer <= 96)
            AddSprite(SPRITE_YX(0x30, 0x58), SPRITE_SHAPE_64x32, BANNER_ATTR2_L);
        scale = gDuelCmd.timer;
        if (scale > 96 && scale < 128) {
            /* scaleAngle = (timer - 96) * 0x100 << 16: scale 0x100 .. 0x1F00 */
            scale -= 96;
            AddAffineSprite(SPRITE_YX(0x30, 0x58), SPRITE_SHAPE_64x32, BANNER_ATTR2_L, scale << 24);
            if (gDuelCmd.timer == 0x7F)
                PlayDuelBGM();
        }
        gDuelCmd.timer++;
        if (gDuelCmd.timer == 0)
            gDuelCmd.step++;
        break;
    default:
        gDuel.bgmOn = 1;
        gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_EXODIA_WIN_SCENE (0x05): run the Exodia win scene. */
void DuelCmd_ExodiaWinScene(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(DUEL_SCENE_EXODIA_WIN, 0);
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_RunU32())
            gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_DESTINY_BOARD_WIN_SCENE (0x06): run the Destiny Board win scene. */
void DuelCmd_DestinyBoardWinScene(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(DUEL_SCENE_DESTINY_BOARD_WIN, 0);
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_RunU32())
            gDuelCmd.running = 0;
        break;
    }
}

/*
 * DUEL_CMD_TOSS_COIN (0xE0): toss one coin; arg2 = the called face, arg4 = the result (1 = tails).
 * gDuelScene.arg: bit 15 the called face, bits 8-14 the coin count (1), bit 7 set, bit 0 the result.
 */
void DuelCmd_TossCoin(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(DUEL_SCENE_COIN_TOSS, 0);
        gDuelScene.arg = (gDuelCmd.arg2 << 15) | (1 << 8) | 0x80 | (u8)gDuelCmd.arg4;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_RunU32())
            gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_TOSS_THREE_COINS (0xE1, Barrel Dragon): toss three coins; arg2 bits 0-2 are the results. */
void DuelCmd_TossThreeCoins(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(DUEL_SCENE_COIN_TOSS, 0);
        gDuelScene.arg = (3 << 8) | 0x80 | (u8)gDuelCmd.arg2;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_RunU32())
            gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_ROLL_GRACEFUL_DICE (0xE2): roll Graceful Dice; arg2 = the face (1-6). */
void DuelCmd_RollGracefulDice(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(DUEL_SCENE_DICE_GRACEFUL, 0);
        gDuelScene.arg = gDuelCmd.arg2;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_RunU32())
            gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_ROLL_SKULL_DICE (0xE3; the dispatcher also sends 0xE5 here): roll Skull Dice; arg2 = the face. */
void DuelCmd_RollSkullDice(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(DUEL_SCENE_DICE_SKULL, 0);
        gDuelScene.arg = gDuelCmd.arg2;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_RunU32())
            gDuelCmd.running = 0;
        break;
    }
}

/* DUEL_CMD_ROLL_PLAIN_DIE (0xE4): roll a plain die (no character swing-in); arg2 = the face. */
void DuelCmd_RollPlainDie(void)
{
    switch (gDuelCmd.step) {
    case 0:
        DuelScene_Start(DUEL_SCENE_DICE_PLAIN, 0);
        gDuelScene.arg = gDuelCmd.arg2;
        gDuelCmd.step++;
        break;
    case 1:
        if (DuelScene_RunU32())
            gDuelCmd.running = 0;
        break;
    }
}
