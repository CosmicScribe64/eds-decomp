/*
 * Destiny Board win scene, and the sprite helpers of the pre-duel turn-order screen.
 *
 * Destiny Board (DUEL_SCENE_DESTINY_BOARD_WIN): when Destiny Board wins the duel, the duel screen gives way to
 * a Ouija board over scrolling, wavy BG layers. A ghost-hand animation plays, then the letters F-I-N-A-L
 * appear and fly off one by one, and the scene fades to black. The scene
 * handler DestinyBoardScene_Run runs gDestinyBoardSceneSteps (enum DestinyBoardSceneStep) on gSceneWork,
 * whose own data is gSceneWork.u.destinyBoard (struct DestinyBoardSceneWork):
 *   DESTINY_STEP_INIT            DestinyBoardScene_Init           clear the work area, scroll layers
 *   DESTINY_STEP_LOAD            DestinyBoardScene_Load           graphics, HBlank wave, music, fade-in
 *   DESTINY_STEP_UPDATE          DestinyBoardScene_Update         every frame until the fade-out is done
 *   DESTINY_STEP_DISABLE_HBLANK  DestinyBoardScene_DisableHBlank  mask the HBlank IRQ again
 * The VBlank/HBlank handlers, the scroll layers (ScrollLayer_*) and the letter drawer are in dice_scene.c.
 *
 * The rest of the unit draws the turn-order screen (turn_order.h; work area gSceneWork.u.turnOrder):
 * the link "Wait" sign, the 4-byte scrollers that turn the hand-card carousel and slide the opponent's card,
 * the rock/scissors/paper carousel, the result banners, the opponent's card and the FIRST/SECOND banners.
 */
#include "global.h"
#include "gba.h"                /* REG_*, CpuSet, CpuFastSet, VRAM addresses */
#include "main.h"               /* gMain */
#include "sound.h"              /* PlaySE, PlayBGM (new sound.h) */
#include "util.h"               /* gSineTable */
#include "palette.h"            /* struct Fade, FadeStart, FadeTick */
#include "bg.h"                 /* CopyMapRect, CopyTileSheetTo2D, TILE_COLORS_16 */
#include "sprite.h"             /* struct AnimSeq / AnimState / ObjAffine, OamList*, AnimBlock*, ObjAffine* */
#include "duel_scenes.h"        /* gDuelScene, gSceneWork, struct DestinyBoardSceneWork, Destiny Board steps */
#include "turn_order.h"         /* turn-order enums, sprite helpers defined here, tile tables */

/* ---- Names the legacy headers lack (until H0 installs the new gba.h, main.h and sound.h) ---- */

/* Values and prototypes as in the new headers; this block compiles away once they are installed. */
#ifndef INTR_FLAG_HBLANK
#define BG_VRAM                 0x06000000
#define BG_CHAR_SIZE            0x4000
#define BG_SCREEN_SIZE          0x800
#define BG_CHAR_ADDR(n)         (BG_VRAM + BG_CHAR_SIZE * (n))
#define BG_SCREEN_ADDR(n)       (BG_VRAM + BG_SCREEN_SIZE * (n))
#define DISPCNT_MODE_0          0x0000
#define DISPCNT_BG0_ON          0x0100
#define DISPCNT_BG_ALL_ON       0x0F00
#define DISPCNT_OBJ_ON          0x1000
#define BGCNT_PRIORITY(n)       (n)
#define BGCNT_SCREENBASE(n)     ((n) << 8)
#define BLDCNT_TGT1_BG0         0x0001
#define BLDCNT_EFFECT_BLEND     0x0040
#define BLDCNT_TGT2_ALL         0x3F00
#define BLDALPHA_BLEND(eva, evb) (((evb) << 8) | (eva))
#define INTR_FLAG_HBLANK        0x0002
#define CPU_SET_SRC_FIXED       0x01000000
#define CPU_SET_16BIT           0x00000000
#define CPU_FAST_SET_SRC_FIXED  0x01000000
#define OAM_ATTR0_AFFINE_DOUBLE 0x0300
#define OAM_ATTR0_BLEND         0x0400
#define OAM_ATTR1_MATRIX(n)     ((n) << 9)
#define INTR_SLOT_HBLANK        1
#define VBLANK_COPY_OAM         0x1
extern void (*IntrTable[16])(void);
void PlaySE(u32 seId);
void PlayBGM(u32 songId);
#endif

/* The two 16-bit halves of the BG2X and BG3Y reference points (gba.h names them only as 32-bit registers). */
#define REG_BG2X_L REG16(0x028)
#define REG_BG2X_H REG16(0x02A)
#define REG_BG3Y_L REG16(0x03C)
#define REG_BG3Y_H REG16(0x03E)

/* OamListAddSprite returns the entry; these drawers OR attr0 and attr1 into its first word in one go. */
#define OAM_ATTR01(attr0, attr1) (((attr1) << 16) | (attr0))

/* ---- Local views kept on purpose (matching choices, see build/readability/HEADERS.md) ---- */

/* OamListAddSprite as this unit calls it: every argument as a full word (the definition narrows to u8/u16,
 * which would add narrowing at these call sites), and the entry returned as a u32 * so attr0 and attr1 can be
 * ORed in as one word. */
extern u32 *OamListAddSpriteWide(u32 layer, u32 tile, s32 x, s32 y, u32 width, u32 height, u32 bpp, u32 palette,
                                 u32 unused, u32 attr0Flags, u32 attr1Bits, u32 priority, struct OamList *list)
    asm("OamListAddSprite");

/* MulFix8 with int parameters and result: the ROM neither narrows the arguments nor sign-extends the result
 * (util.h: s16 MulFix8(s16, s16)). */
extern s32 MulFix8Int(s32 a, s32 b) asm("MulFix8");

/* AnimBlockDraw and DestinyBoardScene_DrawFinalLetters with an unnarrowed y: the ROM passes the computed
 * position as it is (the definitions take u16). */
extern void AnimBlockDrawWideY(u8 *block, u32 layer, u32 priority, u32 tileOffset, u32 palette, u32 format,
                               u32 mode, u32 x, s32 y, struct OamList *oam) asm("AnimBlockDraw");
extern void DestinyBoardScene_DrawFinalLettersWideY(u32 unused, u32 x, s32 y)
    asm("DestinyBoardScene_DrawFinalLetters");

/* ---- ROM data used only here ---- */

/* One ScrollLayer_Init argument set (gDestinyBoardLayerInit, 0x08199DCC). */
struct ScrollLayerInit {
    u8 *srcMap;     /* +0x0: tall source map (30 entries per row); none for BG0 */
    u8 *bgMap;      /* +0x4: BG screen block it streams into */
    s16 speed;      /* +0x8: 12.4 pixels per frame */
    s16 target;     /* +0xA: 12.4 stop position */
};

/* 0x08199DFC: the scene steps (Init, Load, Update, DisableHBlank, NULL). */
extern u16 (*const gDestinyBoardSceneSteps[])(void);
/* 0x08199DCC: BG0-3 scroll layers. */
extern const struct ScrollLayerInit gDestinyBoardLayerInit[4];
/* 0x0819A698: NULL-terminated animation list; anims[0] the ghost hand, anims[3] the letters
 * (gFinalLettersAnim). Not const: AnimBlockInit takes a struct AnimSeq **. */
extern struct AnimSeq *gDestinyBoardAnimList[];
/* 0x08082404: {0, 3, 1, 4, 2, -1}: letter launched at each tick of DestinyBoardScene_LaunchLetters (F, A,
 * I, L, N), -1 = none. */
extern const s8 gFinalLetterLaunchOrder[];
/* Graphics (not const: CopyMapRect and CopyTileSheetTo2D take non-const pointers). */
extern u8 gDestinyBoardBg2Map[];        /* 0x086E11A4: 30x20 map for BG2 */
extern u8 gDestinyBoardBg1Map[];        /* 0x086E1C6C: 30x20 map for BG1 */
extern u8 gDestinyBoardWaveMap[];       /* 0x086E21D0: 16x8 map tiled over BG0 and BG3, the waving layers */
extern const u8 gDestinyBoardBgTiles0[];    /* 0x086D0178: BG tiles 0x000-0x0FF: the Ouija board */
extern const u8 gDestinyBoardBgTiles1[];    /* 0x086D2178: BG tiles 0x100-0x1FF */
extern const u8 gDestinyBoardBgTiles2[];    /* 0x086D4178: BG tiles 0x200-0x2FF: a spirit figure */
extern u8 gDestinyBoardObjTiles0[];     /* 0x086D6178: 128x128 OBJ sheet (ghost hand), OBJ tile 0 */
extern u8 gDestinyBoardObjTiles1[];     /* 0x086D8178: 128x128 OBJ sheet, OBJ tile 16 */
extern u8 gDestinyBoardObjTiles2[];     /* 0x086DA178: 128x128 OBJ sheet (ghost auras), OBJ tile 512 */
extern u8 gDestinyBoardObjTiles3[];     /* 0x086DC178: 128x128 OBJ sheet, OBJ tile 528 */
extern u8 gFinalLetterTiles[];          /* 0x086DE178: 128x128 sheet of the F-I-N-A-L letters */
extern const u8 gDestinyBoardBgPal[];   /* 0x086E22D0: 256-colour BG palette */
extern const u8 gDestinyBoardObjPal[];  /* 0x086E24D0: 256-colour OBJ palette */

/* 0x080826DC: {0, 86, 171}: carousel angles at which rock, scissors and paper face the player. */
extern const u8 gHandCarouselStops[];
/* 0x08082703: {0, 1, 2}: OBJ palette of each hand card. */
extern const u8 gHandCardPalNums[];
/* 0x080826EA: the two 64x32 halves (OBJ tiles) of each banner (enum TurnOrderBanner). */
extern const u16 gTurnOrderBannerTileNums[];
/* 0x080826FE: {4, 5, 6, 8, 7}: OBJ palette of each banner (palette 8 is never loaded: entry 3 is unused). */
extern const u8 gTurnOrderBannerPalNums[];

/* The scene's own data, at gSceneWork + 0xAAC. */
#define sBoard gSceneWork.u.destinyBoard

/* Bytes of gSceneWork the scene uses (0xB24), cleared by DestinyBoardScene_Init. */
#define DESTINY_WORK_SIZE (OFFSET_OF(struct SceneWork, u) + sizeof(struct DestinyBoardSceneWork))

/* True on the last tick of step `step` of `anim` (stepIdx == step and timer == 0). Matching: the ROM reads
 * the word at AnimState +0xC (pieceCount, stepIdx, active, timer) and masks stepIdx and timer. */
#define ANIM_ENDING_STEP(anim, step) ((*(u32 *)&(anim).pieceCount & 0xFF00FF00) == ((step) << 8))

/* ---- Destiny Board win scene ---- */

/* Launches the F-I-N-A-L letters in gFinalLetterLaunchOrder, one every 21 frames (letterTimer counts 20 down
 * through 0 to 0xFF; the first one goes at once). Once the flight progress of the middle letter N (launched
 * last) reaches 0x60, starts the fade to black that ends the scene and hides BG0. */
void DestinyBoardScene_LaunchLetters(void)
{
    if (--sBoard.letterTimer == 0xFF && sBoard.letterTick <= 5) {
        if (gFinalLetterLaunchOrder[sBoard.letterTick] != -1) {
            sBoard.letters[gFinalLetterLaunchOrder[sBoard.letterTick]].state++; /* LETTER_AT_REST -> LETTER_FLYING */
            PlaySE(0x2F);
        }
        sBoard.letterTimer = 20;
        sBoard.letterTick++;
    }
    if (sBoard.letters[2].t == 0x60) {
        FadeStart(FADE_BLACK, 0x60, 0, &sBoard.fade);
        REG_DISPCNT &= ~DISPCNT_BG0_ON;
    }
}

/* Puts the five letters back at rest and restarts the launch sequence. Unused: Init clears the whole work
 * area instead. */
void DestinyBoardScene_ResetLetters(void)
{
    u8 i;

    sBoard.letterTimer = 20;
    for (i = 0; i < 5; i++) {
        sBoard.letters[i].t = 0;
        sBoard.letters[i].state = LETTER_AT_REST;
    }
    sBoard.letterTick = 0;
}

/* DESTINY_STEP_INIT: clears the scene work, resets the BG scroll, hides every layer, and sets up the OAM list,
 * the animations, the affine records and the four scroll layers. Returns 1. */
u32 DestinyBoardScene_Init(void)
{
    vu16 zero;
    int i;

    zero = 0;
    CpuSet((void *)&zero, &gSceneWork, CPU_SET_SRC_FIXED | CPU_SET_16BIT | (DESTINY_WORK_SIZE / 2));
    gMain.vblankFlags = VBLANK_COPY_OAM;
    REG_BG0VOFS = 0;
    REG_BG0HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    /* Of the affine reference points only BG2X and BG3Y are cleared, not BG2Y and BG3X. */
    REG_BG2X_L = 0;
    REG_BG2X_H = 0;
    REG_BG3Y_L = 0;
    REG_BG3Y_H = 0;
    REG_DISPCNT &= ~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
    OamListClear((u8 *)&gSceneWork.oamList);
    AnimBlockInit(gDestinyBoardAnimList, (u8 *)gSceneWork.anims);
    ObjAffineInit(gSceneWork.aff);
    for (i = 0; i < 4; i++)
        ScrollLayer_Init(gDestinyBoardLayerInit[i].srcMap, gDestinyBoardLayerInit[i].bgMap,
                         gDestinyBoardLayerInit[i].speed, gDestinyBoardLayerInit[i].target, &sBoard.layers[i]);
    sBoard.layerDelay = 0x62;
    return 1;
}

/* DESTINY_STEP_LOAD: starts the fade-in from black, loads the maps, tiles and palettes, turns on BG0-3 and OBJ
 * (BG0 alpha-blended over the rest), installs the VBlank and HBlank handlers and starts the music. Returns 1. */
u32 DestinyBoardScene_Load(void)
{
    vu32 zero;
    u8 i, j;

    FadeStart(FADE_BLACK, -0x180, 0, &sBoard.fade);
    zero = 0;
    CpuFastSet((void *)&zero, (void *)VRAM, CPU_FAST_SET_SRC_FIXED | (0x18000 / 4));
    CopyMapRect(gDestinyBoardBg2Map, (void *)BG_SCREEN_ADDR(28), 30, 20);
    CopyMapRect(gDestinyBoardBg1Map, (void *)BG_SCREEN_ADDR(26), 30, 20);
    /* BG0 and BG3: the 16x8 wave map tiled 2 across and 4 down. */
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 2; j++) {
            CopyMapRect(gDestinyBoardWaveMap, (u16 *)BG_SCREEN_ADDR(24) + (i * 256 + j * 16), 16, 8);
            CopyMapRect(gDestinyBoardWaveMap, (u16 *)BG_SCREEN_ADDR(30) + (i * 256 + j * 16), 16, 8);
        }
    }
    CpuFastSet(gDestinyBoardBgTiles0, (void *)BG_CHAR_ADDR(0), 0x2000 / 4);
    CpuFastSet(gDestinyBoardBgTiles1, (void *)(BG_CHAR_ADDR(0) + 0x2000), 0x2000 / 4);
    CpuFastSet(gDestinyBoardBgTiles2, (void *)BG_CHAR_ADDR(1), 0x2000 / 4);
    CopyTileSheetTo2D(gDestinyBoardObjTiles0, (u8 *)OBJ_VRAM0, TILE_COLORS_16);
    CopyTileSheetTo2D(gDestinyBoardObjTiles1, (u8 *)(OBJ_VRAM0 + 0x200), TILE_COLORS_16);
    CopyTileSheetTo2D(gDestinyBoardObjTiles2, (u8 *)(OBJ_VRAM0 + 0x4000), TILE_COLORS_16);
    CopyTileSheetTo2D(gDestinyBoardObjTiles3, (u8 *)(OBJ_VRAM0 + 0x4200), TILE_COLORS_16);
    CpuFastSet(gDestinyBoardBgPal, (void *)BG_PLTT, 0x200 / 4);
    CpuFastSet(gDestinyBoardObjPal, (void *)OBJ_PLTT, 0x200 / 4);
    REG_BG0CNT = BGCNT_SCREENBASE(24) | BGCNT_PRIORITY(0);
    REG_BG1CNT = BGCNT_SCREENBASE(26) | BGCNT_PRIORITY(1);
    REG_BG2CNT = BGCNT_SCREENBASE(28) | BGCNT_PRIORITY(2);
    REG_BG3CNT = BGCNT_SCREENBASE(30) | BGCNT_PRIORITY(3);
    REG_DISPCNT = DISPCNT_MODE_0 | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    gMain.vblankCallback = DestinyBoardScene_VBlank;
    /* The ghost hand (anims[0]) and anims[2] play; the others wait (|= 0xFF: ANIM_HIDDEN). */
    for (i = 1; i < 5; i++)
        gSceneWork.anims[i].active |= ANIM_HIDDEN;
    gSceneWork.anims[0].active = ANIM_PLAYING;
    gSceneWork.anims[2].active = ANIM_PLAYING;
    ScrollLayer_StreamRow(&sBoard.layers[1]);
    ScrollLayer_StreamRow(&sBoard.layers[2]);
    /* Install the HBlank handler with the IRQ masked, then enable it. */
    REG_IME = 0;
    REG_IE &= ~INTR_FLAG_HBLANK;
    IntrTable[INTR_SLOT_HBLANK] = DestinyBoardScene_HBlank;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= INTR_FLAG_HBLANK;
    REG_IME = 1;
    REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = BLDALPHA_BLEND(11, 16);
    PlayBGM(20);
    return 1;
}

/* DESTINY_STEP_UPDATE, every frame: scrolls the layers, plays the ghost-hand and board animations, then
 * launches and draws the letters (enum DestinyBoardAnimPhase). Returns 1 once the final fade-out is done. */
u32 DestinyBoardScene_Update(void)
{
    u8 i;

    FadeTick(&sBoard.fade);
    DestinyBoardScene_AdvanceWave();
    /* After 0x62 frames layers 1-3 start to scroll. */
    if (sBoard.layerDelay == 1) {
        sBoard.layers[1].moving = 1;
        sBoard.layers[2].moving = 1;
        sBoard.layers[3].moving = 1;
    }
    if (sBoard.layerDelay)
        sBoard.layerDelay--;
    for (i = 0; i < 4; i++)
        ScrollLayer_Move(&sBoard.layers[i]);
    /* Once layers 0 and 1 have stopped, layer 3 keeps drifting slowly. */
    if (sBoard.layers[0].moving == 0 && sBoard.layers[1].moving == 0 && sBoard.layerDelay == 0)
        sBoard.layers[3].speed = -2;
    ScrollLayer_StreamRow(&sBoard.layers[1]);
    ScrollLayer_StreamRow(&sBoard.layers[2]);
    AnimBlockTick((u8 *)gSceneWork.anims);
    /* Matching: the ROM tests active as a signed byte (ldsb). */
    switch (sBoard.animPhase) {
    case DESTINY_PHASE_ANIM0:
        if ((s8)gSceneWork.anims[0].active == ANIM_FINISHED) {
            gSceneWork.anims[0].active = ANIM_HIDDEN;
            gSceneWork.anims[1].active = ANIM_PLAYING;
            sBoard.animPhase++;
        }
        /* At step 10 the ghost-hand sheet in OBJ VRAM is replaced by the letters. */
        if (ANIM_ENDING_STEP(gSceneWork.anims[0], 10))
            CopyTileSheetTo2D(gFinalLetterTiles, (u8 *)OBJ_VRAM0, TILE_COLORS_16);
        if (ANIM_ENDING_STEP(gSceneWork.anims[0], 1))
            PlaySE(0x2D);
        if (ANIM_ENDING_STEP(gSceneWork.anims[0], 11))
            PlaySE(0x2E);
        break;
    case DESTINY_PHASE_ANIM1:
        if ((s8)gSceneWork.anims[1].active == ANIM_FINISHED) {
            gSceneWork.anims[1].active = ANIM_HIDDEN;
            gSceneWork.anims[3].active = ANIM_PLAYING;
            sBoard.animPhase++;
            sBoard.pauseTimer = 30;
        }
        break;
    case DESTINY_PHASE_PAUSE:
        gSceneWork.anims[3].active = ANIM_PLAYING;
        if (--sBoard.pauseTimer == 0xFF)
            sBoard.animPhase = DESTINY_PHASE_LETTERS;
        break;
    }
    switch (sBoard.animPhase) {
    case DESTINY_PHASE_ANIM0:
    case DESTINY_PHASE_ANIM1:
    case DESTINY_PHASE_PAUSE:
        /* The animations move with BG2. */
        AnimBlockDrawWideY((u8 *)gSceneWork.anims, 0, 1, 0, 0, OAM_TILES_SHEET16, OAM_GROUP_QUAD_REL_POS, 0,
                           0x48 - (sBoard.layers[2].pos >> 4), &gSceneWork.oamList);
        break;
    case DESTINY_PHASE_LETTERS:
        DestinyBoardScene_LaunchLetters();
        gSceneWork.anims[3].active = ANIM_PLAYING;
        DestinyBoardScene_DrawFinalLettersWideY(3, 0, 0x48 - (sBoard.layers[1].pos >> 4));
        break;
    }
    for (i = 0; i < 5; i++)
        ObjAffineApply(&gSceneWork.aff[i]);
    OamListFlush(&gSceneWork.oamList);
    OamListClear((u8 *)&gSceneWork.oamList);
    if (sBoard.fade.state == FADE_STATE_FADED_OUT)
        return 1;
    return 0;
}

/* DESTINY_STEP_DISABLE_HBLANK: masks the HBlank IRQ that DestinyBoardScene_Load enabled. Returns 1. */
u32 DestinyBoardScene_DisableHBlank(void)
{
    REG_IME = 0;
    REG_IE &= ~INTR_FLAG_HBLANK;
    REG_IME = 1;
    return 1;
}

/* Scene handler of DUEL_SCENE_DESTINY_BOARD_WIN: runs the current step and moves to the next one when it
 * returns non-zero. Returns 1 at the NULL end of gDestinyBoardSceneSteps. */
u32 DestinyBoardScene_Run(void)
{
    if (gDestinyBoardSceneSteps[gDuelScene.sceneStep]) {
        if (gDestinyBoardSceneSteps[gDuelScene.sceneStep]())
            gDuelScene.sceneStep++;
        return 0;
    }
    return 1;
}

/* ---- Turn-order screen: sprites ---- */

/* Shows the flashing link "Wait" sign (anims[0] of the turn-order screen). */
void TurnOrder_ShowWaitSign(void)
{
    gSceneWork.anims[0].active = ANIM_PLAYING;
}

/* Hides the "Wait" sign. */
void TurnOrder_HideWaitSign(void)
{
    gSceneWork.anims[0].active = ANIM_HIDDEN;
}

/* Moves the four scrollers by their speed (pos wraps at 256). */
void Scroller_Move(struct Scroller *scrollers)
{
    u8 i;

    for (i = 0; i < 4; i++)
        scrollers[i].pos += scrollers[i].speed;
}

/* Stops the carousel at the stop position it is passing (gHandCarouselStops[i] <= pos < stop + |speed|) and
 * stores that stop: the hand now facing the player (enum RpsHand). */
void Scroller_SnapToStop(struct Scroller *scroller)
{
    u8 i = 0;
    s32 pos = scroller->pos;

    for (; i < 3; i++) {
        if (pos >= gHandCarouselStops[i]) {
            s32 absSpeed = scroller->speed;
            if (absSpeed < 0)
                absSpeed = -absSpeed;
            if (pos < gHandCarouselStops[i] + absSpeed) {
                scroller->speed = 0;
                scroller->pos = gHandCarouselStops[i];
                scroller->stop = i;
            }
        }
    }
}

/* Stops the opponent-card slide at its ends: 0x30 (fully in) and 0 (out). */
void Scroller_StopAtEnds(struct Scroller *scroller)
{
    if (scroller->pos == 0x30)
        scroller->speed = 0;
    if (scroller->pos == 0)
        scroller->speed = 0;
}

/* Draws the three 32x64 hand cards on a ring seen from the side, using affine records 0-2: card 0 at `angle`,
 * card 2 at angle + spread, card 1 at angle - spread - 1 (256 steps per turn). Each card sits at
 * x = 0x58 + 0x30 * sin, y = 0x32 + 0x10 * cos and is scaled by 0.75 + cos / 4, so the front card is the
 * largest (its matrix angle is 0x8000, half a turn). Cards in the back half (0x40 <= a < 0xC0), or all of them when blendMask has BLEND_CAROUSEL, go to
 * OAM layer 1 and are semi-transparent. As the opponent's card slides in (`lift`), the selected card rises by
 * lift and the others sink by 2 * lift. */
void TurnOrder_DrawHandCarousel(u16 *tileNums, u8 *palNums, u8 angle, u8 selected, u8 lift, u16 blendMask,
                                u8 spread)
{
    u8 liftY[3];
    u8 backAngle = 0xFF - spread;
    u8 i;
    s32 layer, dim;
    s32 y;
    s32 width = 0x20, height = 0x40;
    s16 cos;
    u32 *oam;
    u32 attr;

    for (i = 0; i < 3; i++) {
        if (i == selected)
            liftY[i] = -lift;
        else
            liftY[i] = lift * 2;
    }

    /* Matching: each card's y is computed in sequence through `y` (cos * 0x10, >> 8, + 0x32, + liftY); the
     * same sum written as one expression is scheduled differently. */

    /* Card 2, matrix 1 */
    if (((angle + spread) % 256 >= 0x40 && (angle + spread) % 256 < 0xC0) || (blendMask & BLEND_CAROUSEL)) {
        layer = 1;
        dim = 1;
    } else {
        layer = 0;
        dim = 0;
    }
    oam = OamListAddSpriteWide(layer, tileNums[2],
                               (MulFix8Int(gSineTable[(angle + spread) % 256], 0x3000) >> 8) + 0x58,
                               (y = MulFix8Int(gSineTable[(angle + spread) % 256 + 0x40], 0x1000), y >>= 8,
                                y += 0x32, y += liftY[2]),
                               width, height, 4, palNums[2], 0x200, 0, 0, 0, &gSceneWork.oamList);
    attr = *oam;
    *oam = attr | (dim ? OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE | OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(1))
                       : OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE, OAM_ATTR1_MATRIX(1)));
    gSceneWork.aff[1].angle = 0x8000;
    cos = gSineTable[(angle + spread) % 256 + 0x40];
    gSceneWork.aff[1].scaleX = MulFix8Int(0x40, cos) + 0xC0;
    gSceneWork.aff[1].scaleY = MulFix8Int(0x40, cos) + 0xC0;

    /* Card 1, matrix 2 */
    if (((angle + backAngle) % 256 >= 0x40 && (angle + backAngle) % 256 < 0xC0) || (blendMask & BLEND_CAROUSEL)) {
        layer = 1;
        dim = 1;
    } else {
        layer = 0;
        dim = 0;
    }
    oam = OamListAddSpriteWide(layer, tileNums[1],
                               (MulFix8Int(gSineTable[(angle + backAngle) % 256], 0x3000) >> 8) + 0x58,
                               (y = MulFix8Int(gSineTable[(angle + backAngle) % 256 + 0x40], 0x1000), y >>= 8,
                                y += 0x32, y += liftY[1]),
                               width, height, 4, palNums[1], 0x200, 0, 0, 0, &gSceneWork.oamList);
    attr = *oam;
    *oam = attr | (dim ? OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE | OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(2))
                       : OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE, OAM_ATTR1_MATRIX(2)));
    gSceneWork.aff[2].angle = 0x8000;
    cos = gSineTable[(angle + backAngle) % 256 + 0x40];
    gSceneWork.aff[2].scaleX = MulFix8Int(0x40, cos) + 0xC0;
    gSceneWork.aff[2].scaleY = MulFix8Int(0x40, cos) + 0xC0;

    /* Card 0, matrix 0 */
    if ((angle % 256 >= 0x40 && angle % 256 < 0xC0) || (blendMask & BLEND_CAROUSEL)) {
        layer = 1;
        dim = 1;
    } else {
        layer = 0;
        dim = 0;
    }
    oam = OamListAddSpriteWide(layer, tileNums[0],
                               (MulFix8Int(gSineTable[angle % 256], 0x3000) >> 8) + 0x58,
                               (y = MulFix8Int(gSineTable[angle % 256 + 0x40], 0x1000), y >>= 8,
                                y += 0x32, y += liftY[0]),
                               width, height, 4, palNums[0], 0x200, 0, 0, 0, &gSceneWork.oamList);
    attr = *oam;
    *oam = attr | (dim ? OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE | OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(0))
                       : OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE, OAM_ATTR1_MATRIX(0)));
    gSceneWork.aff[0].angle = 0x8000;
    cos = gSineTable[angle % 256 + 0x40];
    gSceneWork.aff[0].scaleX = MulFix8Int(0x40, cos) + 0xC0;
    gSceneWork.aff[0].scaleY = MulFix8Int(0x40, cos) + 0xC0;
}

/* Draws a 128x32 banner (enum TurnOrderBanner) from its two 64x32 halves at x 0x38 and 0x78, y 0x6E
 * (BANNER_SELECT_CARD: y 0x2C, near the top); semi-transparent when blendMask has BLEND_BANNER. */
void TurnOrder_DrawBanner(u8 banner, u16 blendMask)
{
    /* Matching: the constants are locals declared outside the loop. CSE cannot see them inside the loop, so
     * reload rematerializes them (x0 in r7, y0 into r3), and the round-robin reload register choice depends on
     * width, height and bpp being variables. */
    s32 width = 0x40;
    s32 height = 0x20;
    u8 i;
    const u16 *tileNums;
    s32 x0 = 0x38;
    s32 y0 = 0x6E;
    s32 bpp = 4;
    u16 translucent;

    i = 0;
    tileNums = gTurnOrderBannerTileNums;
    translucent = blendMask & BLEND_BANNER;
    for (; i < 2; i++) {
        u16 tile = tileNums[banner * 2 + i];
        s32 x, y;
        u32 *oam;
        u32 attr;
        x = x0 + i * width;
        y = y0;
        if (banner == BANNER_SELECT_CARD)
            y -= 0x42;
        oam = OamListAddSpriteWide(0, tile, x, y, width, height, bpp, gTurnOrderBannerPalNums[banner], 0x200, 0, 0,
                                   0, &gSceneWork.oamList);
        attr = *oam;
        if (translucent)
            *oam = attr | OAM_ATTR0_BLEND;
    }
}

/* Draws the opponent's 32x64 card for `hand` (enum RpsHand) at x 0x69, sliding down from above the screen:
 * y = 0xC0 + slide * 1.375 (8-bit y: slide 0x30 gives y 2). Matrix 3 is set to identity; semi-transparent when
 * blendMask has BLEND_OPPONENT_CARD. */
void TurnOrder_DrawOpponentCard(u8 hand, u8 slide, u16 blendMask)
{
    s32 width = 0x20;
    s32 height = 0x40;
    u32 *oam = OamListAddSpriteWide(0, gHandCardTileNums[hand], 0x69, (MulFix8Int(slide << 8, 0x160) >> 8) + 0xC0,
                                    width, height, 4, gHandCardPalNums[hand], 0x200, 0, 0, 0, &gSceneWork.oamList);
    *oam |= (blendMask & BLEND_OPPONENT_CARD) ? OAM_ATTR01(OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(3))
                                              : OAM_ATTR01(0, OAM_ATTR1_MATRIX(3));
    gSceneWork.aff[3].angle = 0;
    gSceneWork.aff[3].scaleX = 0x100;
    gSceneWork.aff[3].scaleY = 0x100;
}

/* Draws the "FIRST to go" (x 0x28) and "SECOND to go" (x 0x88) 64x32 banners bobbing at
 * y = 0x30 + sin(2 * frame) * amplitude. `bob` is struct ChoiceBob[2] as halfwords: bob[0] and bob[2] are the
 * two amplitudes. The banner of `choice` (enum TurnChoice) uses palette 3, the other the dim palette 9;
 * matrices 4 and 5 are set to identity. Semi-transparent when blendMask has BLEND_TURN_CHOICE. */
void TurnOrder_DrawTurnChoice(u32 unused, u8 frame, u16 blendMask, s16 *bob, u8 choice)
{
    s32 width = 0x40;
    s32 height = 0x20;
    u32 *oam;
    u32 attr;

    oam = OamListAddSpriteWide(0, gTurnChoiceBannerTileNums[0], 0x28,
                               (MulFix8Int(gSineTable[(frame * 2) % 256], bob[0]) >> 8) + 0x30,
                               width, height, 4, choice == TURN_CHOICE_FIRST ? 3 : 9, 0x200, 0, 0, 0,
                               &gSceneWork.oamList);
    attr = *oam;
    *oam = attr | ((blendMask & BLEND_TURN_CHOICE) ? OAM_ATTR01(OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(4))
                                                   : OAM_ATTR01(0, OAM_ATTR1_MATRIX(4)));
    gSceneWork.aff[4].angle = 0;
    gSceneWork.aff[4].scaleX = 0x100;
    gSceneWork.aff[4].scaleY = 0x100;
    {
        s32 y = (MulFix8Int(gSineTable[(frame * 2) % 256], bob[2]) >> 8) + 0x30;
        u16 tile = gTurnChoiceBannerTileNums[1];
        s32 x = 0x88;

        /* FAKEMATCH: keep the initialized x in r2 after the tile load,
         * before preparing the stack arguments, as in the ROM. */
        __asm__("" : : "r"(x));
        oam = OamListAddSpriteWide(0, tile, x, y, width, height, 4, choice == TURN_CHOICE_SECOND ? 3 : 9, 0x200, 0,
                                   0, 0, &gSceneWork.oamList);
    }
    attr = *oam;
    *oam = attr | ((blendMask & BLEND_TURN_CHOICE) ? OAM_ATTR01(OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(5))
                                                   : OAM_ATTR01(0, OAM_ATTR1_MATRIX(5)));
    gSceneWork.aff[5].angle = 0;
    gSceneWork.aff[5].scaleX = 0x100;
    gSceneWork.aff[5].scaleY = 0x100;
}

/* After the FIRST/SECOND choice: both bob amplitudes decay by 0x80 per frame. The chosen banner (palette 3)
 * slides to the centre (x 0x58 when tween[0] reaches 0x40: x moves by 0x30 * (1 - cos(tween[0]))) and keeps
 * bobbing. The other one (palette 9) becomes a double-size affine sprite on matrix 4 that falls by
 * tween[1]^2 / 2 while it turns by +-tween[1] << 9. `bob` is struct ChoiceBob[2] as halfwords (amplitudes at
 * [0] and [2]); `tween` is the struct Tween as halfwords ([0] x, [1] y). */
void TurnOrder_DrawTurnChoiceConfirm(u32 unused, u8 frame, u16 blendMask, s16 *bob, u8 choice, s16 *tween)
{
    s32 width = 0x40, height = 0x20;
    s32 x0 = 0x78;
    s32 y0 = 0x30;
    /* Matching: a separate x per case (case 0 x in r7, case 1 x in r5) and a separate offset t: x = x0 - t
     * must not fold into (x0 + 0x10) - (...). */
    s32 x, x1, t;
    u8 i;
    u32 *oam;

    for (i = 0; i < 2; i++) {
        if (bob[i * 2] > 0x7F)
            bob[i * 2] -= 0x80;
        else
            bob[i * 2] = 0;
    }
    switch (choice) {
    case TURN_CHOICE_FIRST:
        t = (MulFix8Int(0x100 - gSineTable[tween[0] + 0x40], 0x3000) >> 8) - 0x50;
        x = x0 + t;
        oam = OamListAddSpriteWide(0, gTurnChoiceBannerTileNums[0], x,
                                   y0 + (MulFix8Int(gSineTable[(frame * 2) % 256], bob[0]) >> 8),
                                   width, height, 4, 3, 0x200, 0, 0, 0, &gSceneWork.oamList);
        *oam |= ((blendMask & BLEND_TURN_CHOICE) ? OAM_ATTR01(OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(4))
                                                 : OAM_ATTR01(0, OAM_ATTR1_MATRIX(4)));
        oam = OamListAddSpriteWide(0, gTurnChoiceBannerTileNums[1], x0 - 0x10, ((tween[1] * tween[1]) >> 1) + y0,
                                   width, height, 4, 9, 0x200, 0, 0, 0, &gSceneWork.oamList);
        *oam |= ((blendMask & BLEND_TURN_CHOICE)
                     ? OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE | OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(4))
                     : OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE, OAM_ATTR1_MATRIX(4)));
        gSceneWork.aff[4].angle = ((u32)(u16)tween[1]) << 9;
        break;
    case TURN_CHOICE_SECOND:
        oam = OamListAddSpriteWide(0, gTurnChoiceBannerTileNums[0], x0 - 0x70, ((tween[1] * tween[1]) >> 1) + y0,
                                   width, height, 4, 9, 0x200, 0, 0, 0, &gSceneWork.oamList);
        *oam |= ((blendMask & BLEND_TURN_CHOICE)
                     ? OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE | OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(4))
                     : OAM_ATTR01(OAM_ATTR0_AFFINE_DOUBLE, OAM_ATTR1_MATRIX(4)));
        t = (MulFix8Int(0x100 - gSineTable[tween[0] + 0x40], 0x3000) >> 8) - 0x10;
        x1 = x0 - t;
        oam = OamListAddSpriteWide(0, gTurnChoiceBannerTileNums[1], x1,
                                   (MulFix8Int(gSineTable[(frame * 2) % 256], bob[2]) >> 8) + y0,
                                   width, height, 4, 3, 0x200, 0, 0, 0, &gSceneWork.oamList);
        *oam |= ((blendMask & BLEND_TURN_CHOICE) ? OAM_ATTR01(OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(4))
                                                 : OAM_ATTR01(0, OAM_ATTR1_MATRIX(4)));
        gSceneWork.aff[4].angle = -(((u32)(u16)tween[1]) << 9);
        break;
    }
    gSceneWork.aff[4].scaleX = 0x100;
    gSceneWork.aff[4].scaleY = 0x100;
}
