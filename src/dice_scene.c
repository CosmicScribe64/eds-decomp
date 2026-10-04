/*
 * Duel overlay scenes: two die-roll scene handlers, the Exodia win scene, and helpers of the Destiny Board
 * win scene.
 *
 * 1. DiceScreen_RunGraceful and DiceScreen_RunPlain are the scene handlers of DUEL_SCENE_DICE_GRACEFUL and
 *    DUEL_SCENE_DICE_PLAIN. The steps they run are in coin_toss_scene.c.
 *
 * 2. The Exodia win scene (DUEL_SCENE_EXODIA_WIN) on gSceneWork.u.exodia. ExodiaScene_Run runs
 *    gExodiaSceneSteps (enum ExodiaSceneStep):
 *      EXODIA_STEP_INIT             ExodiaScene_Init             clear the work area, set up blending
 *      EXODIA_STEP_LOAD_EYE         ExodiaScene_LoadEye          Millennium Eye bitmap (mode 4), piece sprites
 *      EXODIA_STEP_FADE_IN_EYE      ExodiaScene_FadeInEye        fade in from black, 60 frames
 *      EXODIA_STEP_ASSEMBLE_PIECES  ExodiaScene_AssemblePieces   the five pieces appear one by one and flash
 *      EXODIA_STEP_GATHER_PIECES    ExodiaScene_GatherPieces     they fly to the centre in a whiteout
 *      EXODIA_STEP_LOAD_FLAMES      ExodiaScene_LoadFlames       Exodia in flames on BG0-2, HBlank wave on BG0
 *      EXODIA_STEP_FADE_IN_FLAMES   ExodiaScene_FadeInFlames     fade in from white
 *      EXODIA_STEP_BLEND_FLAMES     ExodiaScene_BlendFlames      BG0 alpha 16/16 -> 10/16
 *      EXODIA_STEP_FINALE           ExodiaScene_Finale           pulsing red tint, then a fade to black (or B)
 *
 * 3. Helpers of the Destiny Board win scene (its steps are in destiny_board_scene.c): the four vertically
 *    scrolling BG layers (ScrollLayer_*), the HBlank and VBlank scroll handlers, and the F-I-N-A-L letters.
 */
#include "global.h"
#include "legacy/gba.h"            /* REG_*, CpuSet, CpuFastSet, VRAM, BG_PLTT, OBJ_PLTT, OBJ_VRAM0, B_BUTTON */
#include "legacy/main.h"           /* gMain */
#include "util.h"           /* MemClear16, MemCopy16, gSineTable, struct Timer / Line, Timer_*, LineInit/Step */
#include "palette.h"        /* struct Fade / PalFade, FadeStart, FadeTick, PalFade_Start/Apply */
#include "bg.h"             /* CopyMapRect, CopyTileSheetTo2D, TILE_COLORS_16 */
#include "sprite.h"         /* struct OamTemplate / OamListEntry / AnimSeq / AnimState, OamList*, ObjAffine* */
#include "duel_scenes.h"    /* gDuelScene, gSceneWork, gFinalLettersAnim, struct ScrollLayer, scene enums */

/* ---- Names the legacy headers lack (until H0 installs the new gba.h and main.h) ---- */

/* Values as in the new gba.h and main.h; inert once they are installed. */
#ifndef INTR_FLAG_HBLANK
#define BG_VRAM                     VRAM
#define BG_SCREEN_ADDR(n)           (BG_VRAM + 0x800 * (n))
#define DISPCNT_MODE_4              0x0004
#define DISPCNT_BG0_ON              0x0100
#define DISPCNT_BG1_ON              0x0200
#define DISPCNT_BG2_ON              0x0400
#define DISPCNT_BG_ALL_ON           0x0F00
#define DISPCNT_OBJ_ON              0x1000
#define BGCNT_PRIORITY(n)           (n)
#define BGCNT_SCREENBASE(n)         ((n) << 8)
#define BLDCNT_TGT1_BG0             0x0001
#define BLDCNT_TGT1_OBJ             0x0010
#define BLDCNT_TGT1_ALL             0x003F
#define BLDCNT_EFFECT_BLEND         0x0040
#define BLDCNT_EFFECT_LIGHTEN       0x0080
#define BLDCNT_TGT2_ALL             0x3F00
#define BLDALPHA_BLEND(eva, evb)    (((evb) << 8) | (eva))
#define INTR_FLAG_HBLANK            0x0002
#define OAM_ATTR0_AFFINE            0x0100
#define OAM_ATTR0_AFFINE_DOUBLE     0x0300
#define OAM_ATTR1_MATRIX(n)         ((n) << 9)
#define INTR_SLOT_HBLANK            1       /* main.h enum IntrSlot */
#define VBLANK_COPY_OAM             0x1     /* main.h enum VBlankFlag */
extern void (*IntrTable[16])(void);         /* main.h: IRQ handlers at 0x03000000 */
#endif

/* The legacy sound.h does not declare PlaySE (the new one does, with this prototype). */
void PlaySE(u32 seId);

/* ---- Local views kept on purpose (matching choices, see build/readability/HEADERS.md) ---- */

/* MulFix8 with int parameters and result: the ROM neither narrows the arguments nor sign-extends the result
 * (util.h: s16 MulFix8(s16, s16)). */
extern s32 MulFix8Int(s32 a, s32 b) asm("MulFix8");

/* AnimBlockInit with a u16 result: ExodiaScene_LoadEye stores it to animCount without narrowing (sprite.h:
 * u8 AnimBlockInit, whose result the caller would narrow to 8 bits first). */
extern u16 AnimBlockInitU16(struct AnimSeq **scripts, u8 *block) asm("AnimBlockInit");

/* OamListAddSpriteGroup as DestinyBoardScene_DrawFinalLetters calls it: layer, position and flags are passed
 * as words without narrowing. With sprite.h's u8/u16 parameters that function grows to 0x700 bytes. */
extern u16 *OamListAddSpriteGroupWide(u16 *tmpls, u32 layer, u32 count, s32 x, s32 y, u32 mode, u32 priority,
                                      u32 sheetX, u32 sheetY, u32 format, u32 attr0Flags, void *list)
    asm("OamListAddSpriteGroup");

/* CopyMapBlock as ExodiaScene_LoadFlames calls it: the source cell (sx, sy) is passed as two addresses
 * (bg.h: u16 sx, u16 sy would narrow them). */
extern u32 CopyMapBlockAddrXY(u16 *srcMap, u8 *sx, u8 *sy, u32 srcStride, u16 *dstMap, u32 dx, u32 dy, u32 w,
                              u32 h, u32 mapSize) asm("CopyMapBlock");
/* FAKEMATCH: the ROM loads sx and sy (both 0) from two literal-pool words instead of using `mov #0`. Two
 * symbols at address 0 reproduce that; two distinct names keep two pool entries. */
extern u8 gUnkA_00000000[], gUnkB_00000000[];

/* ---- ROM data used only here ---- */

/* 0x08199A58 / 0x08199A70: the outer steps of the Graceful Dice and plain-die scenes (PrepareRoll, Init,
 * SetupGracefulDice / SetupPlainDie, LoadGraphics, Update, NULL). */
extern u16 (*const gDiceScreenGracefulSteps[])(void);
extern u16 (*const gDiceScreenPlainSteps[])(void);
/* 0x08199DA4: the steps of the Exodia win scene (enum ExodiaSceneStep), NULL-terminated. */
extern u16 (*const gExodiaSceneSteps[])(void);

/* 0x080823B0: start positions of the five piece flights, {y, x}: (44, 44), (164, 44), (64, 112), (144, 112)
 * and (104, 4) as (x, y). */
struct PieceStartPos {
    s16 y;
    s16 x;
};
extern const struct PieceStartPos gExodiaPieceStartPos[];
/* 0x08199D74: the five 32x32 piece sprites drawn during the flight. */
extern const struct OamTemplate gExodiaPieceOamTemplates[];
/* 0x08199D9C: NULL-terminated animation list with one script (0x08082368): steps 0-5 show 0-5 pieces for 16
 * frames each, then two steps with all five. */
extern struct AnimSeq *gExodiaPiecesAnimList[];
/* 0x08199CC8: NULL-terminated animation list of the flames phase: [0] two steps of 18 sprites from the sheet
 * at OBJ_VRAM0, [1] a five-step loop drawn as affine sprites from OBJ_VRAM_BITMAP. */
extern struct AnimSeq *gExodiaFlameAnimList[];

/* 0x086B8568 / 0x086C1B68: the Millennium Eye, a 240x160 mode-4 bitmap, and its 256-colour palette. */
extern const u8 gMillenniumEyeBitmap[];
extern const u8 gMillenniumEyePal[];
/* 0x086B6368 / 0x086B6568: OBJ palettes and the 128x128 OBJ sheet of the pieces phase. */
extern const u8 gExodiaPiecesObjPal[];
extern u8 gExodiaPiecesObjTiles[];
/* 0x086CED78 / 0x086CF778: two strips of five linear 4x4-tile blocks of the pieces (low-confidence names
 * gExodiaPieceBlocksA / B, not applied). */
extern const u8 gExodiaPiecesColorObjTiles[];
extern const u8 gExodiaPiecesGreyObjTiles[];

/* 0x086C1D68..0x086C7D68: the four 0x2000-byte quarters of the flames BG tile set, and 0x086CAB78 its
 * palette. */
extern const u8 gExodiaFlameBgTiles0[];
extern const u8 gExodiaFlameBgTiles1[];
extern const u8 gExodiaFlameBgTiles2[];
extern const u8 gExodiaFlameBgTiles3[];
extern const u8 gExodiaFlameBgPal[];
/* 0x086CAD78 (low-confidence name gExodiaFlameObjTilesA, not applied) and 0x086CCD78: the OBJ sheets of the
 * flames phase. */
extern u8 gExodiaFlameArmsObjTiles[];
extern u8 gExodiaFlameObjTilesB[];
/* 0x086C9D68 / 0x086CA218 / 0x086CA6C8: the 30x20 maps of BG2, BG1 and BG0 (BG0 is the waving layer). */
extern u16 gExodiaFlameBg2Map[];
extern u16 gExodiaFlameBg1Map[];
extern u16 gExodiaFlameBg0Map[];

/* 0x080823C4: one period (64 entries) of the Destiny Board HBlank wave, a sine of amplitude 16. */
extern const s8 gDestinyBoardWaveTable[];

/* OBJ tiles 0x200 and up (OBJ VRAM + 0x4000): the only OBJ tiles the bitmap modes leave usable. The flames
 * phase (mode 0) also loads a sheet there. */
#define OBJ_VRAM_BITMAP     (OBJ_VRAM0 + 0x4000)

/* Sine, 8.8 fixed (0x100 = 1.0), 256 steps per turn; SIN(i + 0x40) is the cosine. */
#define SIN(i) gSineTable[i]

/* ---- Die-roll scene handlers ---- */

/* Scene handler of DUEL_SCENE_DICE_GRACEFUL: runs gDiceScreenGracefulSteps[gDuelScene.sceneStep], advancing
 * when a step returns non-zero. Returns 1 once the table's NULL end is reached, else 0. */
u16 DiceScreen_RunGraceful(void)
{
    u16 (*step)(void) = gDiceScreenGracefulSteps[gDuelScene.sceneStep];

    if (step != NULL) {
        if (step())
            gDuelScene.sceneStep++;
        return 0;
    }
    return 1;
}

/* Scene handler of DUEL_SCENE_DICE_PLAIN: as DiceScreen_RunGraceful over gDiceScreenPlainSteps. */
u16 DiceScreen_RunPlain(void)
{
    u16 (*step)(void) = gDiceScreenPlainSteps[gDuelScene.sceneStep];

    if (step != NULL) {
        if (step())
            gDuelScene.sceneStep++;
        return 0;
    }
    return 1;
}

/* ---- Exodia win scene ---- */

/* piecesState = PIECES_APPEAR. The argument (ExodiaScene_Init passes &piecesState) is not used. */
void ExodiaScene_ResetPieceState(void *unused)
{
    gSceneWork.u.exodia.piecesState = PIECES_APPEAR;
}

/* Starts the five piece paths: from gExodiaPieceStartPos[i] to the screen centre (0x68, 0x40). */
void ExodiaScene_StartPieceFlight(struct Line *lines)
{
    u8 i;

    for (i = 0; i < 5; i++)
        LineInit(gExodiaPieceStartPos[i].x, gExodiaPieceStartPos[i].y, 0x68, 0x40, lines++);
}

/* HBlank handler: BG0 HOFS follows a sine of the scanline, +-8 px (the flame wave). */
void ExodiaScene_HBlank(void)
{
    REG_BG0HOFS = gSineTable[(REG_VCOUNT + gSceneWork.u.exodia.wavePhase) & 0xFF] >> 5;
}

/* Adds the current frame of `anim` to layer 0 of `oamList` as affine sprites (matrix 0) with the given
 * priority. The template tiles are on a 16-tile-wide sheet: their column (bits 4-7) moves to bits 5-8 of the
 * 32-tile-wide 2D layout, plus tile 0x200 (the sheet is loaded at OBJ_VRAM_BITMAP). */
void ExodiaScene_DrawAffineAnim(struct AnimState *anim, u8 priority, void *oamList)
{
    const struct OamTemplate *tmpl = anim->pieces;
    u8 i;

    for (i = 0; i < anim->pieceCount; i++) {
        struct OamListEntry *entry = OamListAlloc(0, oamList);

        /* attr0 with the affine bit, attr1 with matrix 0, attr2 with the tile moved to the 2D layout + 0x200
         * and the priority replaced. */
        entry->attr0 = ((tmpl->attr0 | OAM_ATTR0_AFFINE) & 0xFF00) | (tmpl->attr0 & 0xFF);
        entry->attr1 = tmpl->attr1 & 0xC1FF;
        entry->attr2 = (((tmpl->attr2 & 0xFF0F) | ((tmpl->attr2 & 0xF0) << 1)) & 0xF3FF)
                     | (((priority << 10) & 0xC00) + 0x200);
        tmpl++;
    }
}

/* EXODIA_STEP_INIT: clears gSceneWork, copies only OAM at VBlank, hides every layer, zeroes the BG1-3
 * scroll, resets the OAM list and affine records, and sets BLDCNT (no effect yet), BLDALPHA 8/8 and BLDY 16.
 * Returns 1. */
u16 ExodiaScene_Init(void)
{
    MemClear16(&gSceneWork, sizeof(gSceneWork));
    gMain.vblankFlags = VBLANK_COPY_OAM;
    REG_DISPCNT &= (u16)~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    ExodiaScene_ResetPieceState(&gSceneWork.u.exodia.piecesState);
    gSceneWork.u.exodia.unkB15 = 0;
    gSceneWork.u.exodia.pulsePhase = 0;
    gSceneWork.u.exodia.unkB14 = 0;
    OamListClear((u8 *)&gSceneWork.oamList);
    ObjAffineInit(gSceneWork.aff);
    REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_TGT2_ALL;
    REG_BLDALPHA = BLDALPHA_BLEND(8, 8);
    REG_BLDY = 16;
    gSceneWork.u.exodia.prevPieceFrame = 0;
    gSceneWork.u.exodia.pieceFrame = 0;
    return 1;
}

/* Copies a linear 32x32-pixel (4x4-tile, 4bpp) block into 2D-mapped OBJ VRAM: four rows of 0x80 bytes, one
 * OBJ tile row (32 tiles, 0x400 bytes) apart. */
void CopyObjTileBlock4x4(const u8 *src, u8 *dst)
{
    int row;

    for (row = 0; row < 4; row++) {
        MemCopy16(dst, src, 0x80);
        dst += 0x400;
        src += 0x80;
    }
}

/* Copies the 4x4-tile block at tile srcTile of src to OBJ tile 0x200 + dstTile (the bitmap-mode OBJ area).
 * The fourth argument is not used. */
void LoadObjTileBlock4x4(const u8 *src, u32 dstTile, u32 srcTile, u32 unused)
{
    CopyObjTileBlock4x4(src + srcTile * 32, (u8 *)OBJ_VRAM_BITMAP + dstTile * 32);
}

/* EXODIA_STEP_LOAD_EYE: starts a fade-in from black, the pieces animation, the Millennium Eye bitmap and
 * palette, the OBJ palettes and piece tiles (blocks 1-4 of both strips; the destination tiles are in the
 * 32-tile-wide 2D layout), mode 4 with OBJ on, and a 60-frame timer. Returns 1. */
u16 ExodiaScene_LoadEye(void)
{
    FadeStart(FADE_BLACK, -0x80, 0, &gSceneWork.u.exodia.fade);
    gSceneWork.animCount = AnimBlockInitU16(gExodiaPiecesAnimList, (u8 *)gSceneWork.anims);
    CpuFastSet(gMillenniumEyeBitmap, (void *)VRAM, 240 * 160 / 4);
    CpuFastSet(gMillenniumEyePal, (void *)BG_PLTT, 0x200 / 4);
    CpuSet(gExodiaPiecesObjPal, (void *)OBJ_PLTT, 0x200 / 2);
    CopyTileSheetTo2D(gExodiaPiecesObjTiles, (u8 *)OBJ_VRAM_BITMAP, TILE_COLORS_16);
    LoadObjTileBlock4x4(gExodiaPiecesColorObjTiles, 0x04, 0x10, 0x40);
    LoadObjTileBlock4x4(gExodiaPiecesColorObjTiles, 0x08, 0x20, 0x40);
    LoadObjTileBlock4x4(gExodiaPiecesColorObjTiles, 0x0C, 0x30, 0x40);
    LoadObjTileBlock4x4(gExodiaPiecesColorObjTiles, 0x80, 0x40, 0x40);
    LoadObjTileBlock4x4(gExodiaPiecesGreyObjTiles, 0x88, 0x10, 0x40);
    LoadObjTileBlock4x4(gExodiaPiecesGreyObjTiles, 0x8C, 0x20, 0x40);
    LoadObjTileBlock4x4(gExodiaPiecesGreyObjTiles, 0x100, 0x30, 0x40);
    LoadObjTileBlock4x4(gExodiaPiecesGreyObjTiles, 0x104, 0x40, 0x40);
    REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    Timer_Reset(&gSceneWork.u.exodia.timer);
    Timer_Start(&gSceneWork.u.exodia.timer, 60);
    return 1;
}

/* EXODIA_STEP_LOAD_FLAMES: loads Exodia in flames: the BG tile set and palette, both OBJ sheets, the BG0-2
 * maps (BG0's first two columns are copied again to columns 30-31, for the wave), a fade-in from white, a
 * 150-frame timer, the flames animations (anims[0] on layer 1), BG0-2 (priority 0-2, screen blocks 24, 26,
 * 28), a red tint fade of all 512 colours, and installs ExodiaScene_HBlank. Returns 1. */
u16 ExodiaScene_LoadFlames(void)
{
    CpuFastSet(gExodiaFlameBgTiles0, (void *)VRAM, 0x2000 / 4);
    CpuFastSet(gExodiaFlameBgTiles1, (void *)(VRAM + 0x2000), 0x2000 / 4);
    CpuFastSet(gExodiaFlameBgTiles2, (void *)(VRAM + 0x4000), 0x2000 / 4);
    CpuFastSet(gExodiaFlameBgTiles3, (void *)(VRAM + 0x6000), 0x2000 / 4);
    CpuFastSet(gExodiaFlameBgPal, (void *)BG_PLTT, 0x200 / 4);
    CopyTileSheetTo2D(gExodiaFlameArmsObjTiles, (u8 *)OBJ_VRAM0, TILE_COLORS_16);
    CopyTileSheetTo2D(gExodiaFlameObjTilesB, (u8 *)OBJ_VRAM_BITMAP, TILE_COLORS_16);
    CopyMapRect(gExodiaFlameBg2Map, (void *)BG_SCREEN_ADDR(28), 30, 20);
    CopyMapRect(gExodiaFlameBg1Map, (void *)BG_SCREEN_ADDR(26), 30, 20);
    CopyMapRect(gExodiaFlameBg0Map, (void *)BG_SCREEN_ADDR(24), 30, 20);
    /* The 2x20 block at cell (0, 0) of the BG0 map (30 wide) to column 30 of screen block 24. */
    CopyMapBlockAddrXY(gExodiaFlameBg0Map, gUnkA_00000000, gUnkB_00000000, 30,
                       (u16 *)(BG_SCREEN_ADDR(24) + 30 * 2), 0, 0, 2, 20, 0);
    FadeStart(FADE_WHITE, -0x80, 0, &gSceneWork.u.exodia.fade);
    Timer_Start(&gSceneWork.u.exodia.timer, 150);
    AnimBlockInit(gExodiaFlameAnimList, (u8 *)gSceneWork.anims);
    gSceneWork.anims[0].layer = 1;
    REG_BG0CNT = BGCNT_SCREENBASE(24) | BGCNT_PRIORITY(0);
    REG_BG1CNT = BGCNT_SCREENBASE(26) | BGCNT_PRIORITY(1);
    REG_BG2CNT = BGCNT_SCREENBASE(28) | BGCNT_PRIORITY(2);
    REG_DISPCNT = DISPCNT_BG0_ON | DISPCNT_BG1_ON | DISPCNT_BG2_ON | DISPCNT_OBJ_ON;
    PalFade_Start((u16 *)PLTT, 0x200, 0x001F, &gSceneWork.u.exodia.palFade);   /* towards red */
    ObjAffineInit(gSceneWork.aff);
    gSceneWork.u.exodia.pulsePhase = 0;
    REG_IME = 0;
    REG_IE &= ~INTR_FLAG_HBLANK;
    IntrTable[INTR_SLOT_HBLANK] = ExodiaScene_HBlank;
    REG_IME = 1;
    gSceneWork.u.exodia.wavePhase = 0;
    gSceneWork.u.exodia.waveTick = 0;
    REG_IME = 0;
    REG_IE |= INTR_FLAG_HBLANK;
    REG_IME = 1;
    return 1;
}

/*
 * EXODIA_STEP_ASSEMBLE_PIECES, per frame (enum ExodiaPiecesState):
 *   PIECES_APPEAR     the pieces script shows one more piece every 16 frames (SE 0x13 each); at step 5 (all
 *                     five) it is frozen and a white flash of the sprites starts (SE 0x13)
 *   PIECES_FLASH_IN   at half white (level 0x800) the flash reverses
 *   PIECES_FLASH_OUT  waits for the flash to end
 *   PIECES_LAUNCH     starts the five flights and a fade to white (SE 0x14); returns 1
 * Draws the animations and ticks the timer every frame. Returns 0 until the launch.
 */
u16 ExodiaScene_AssemblePieces(void)
{
    struct AnimState *anim = gSceneWork.anims;
    u8 i;

    FadeTick(&gSceneWork.u.exodia.fade);
    switch (gSceneWork.u.exodia.piecesState) {
    case PIECES_APPEAR:
        if (anim->stepIdx == 5) {
            anim->active = ANIM_HIDDEN;     /* stops AnimStateTick; the sprites are still drawn below */
            FadeStart(FADE_WHITE, 0xA0, 0, &gSceneWork.u.exodia.fade);
            REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN;
            gSceneWork.u.exodia.piecesState++;
            PlaySE(0x13);
        }
        break;
    case PIECES_FLASH_IN:
        if (gSceneWork.u.exodia.fade.level >= 0x800) {
            gSceneWork.u.exodia.fade.step *= -1;
            gSceneWork.u.exodia.piecesState++;
            anim->active = ANIM_FINISHED;
            anim->timer = 0;
        }
        break;
    case PIECES_FLASH_OUT:
        if (gSceneWork.u.exodia.fade.state == FADE_STATE_FADED_IN) {
            gSceneWork.u.exodia.fade.state = FADE_STATE_IDLE;
            gSceneWork.u.exodia.piecesState++;
        }
        anim->active = ANIM_HIDDEN;
        break;
    case PIECES_LAUNCH:
        ExodiaScene_StartPieceFlight(gSceneWork.u.exodia.pieceLines);
        /* anim is not advanced: anims[0] is drawn animCount times (the list has one script). */
        for (i = 0; i < gSceneWork.animCount; i++)
            OamListAddSpriteGroup((u16 *)anim->pieces, anim->layer, anim->pieceCount, anim->x, anim->y,
                                  OAM_GROUP_TEMPLATE_POS, 0, 0, 1, OAM_TILES_SHEET16, 0, &gSceneWork.oamList);
        OamListFlush(&gSceneWork.oamList);
        OamListClear((u8 *)&gSceneWork.oamList);
        FadeStart(FADE_WHITE, 0x20, 0, &gSceneWork.u.exodia.fade);
        gSceneWork.u.exodia.timer.state = TICK_IDLE;
        gSceneWork.u.exodia.pulsePhase = 0;
        PlaySE(0x14);
        return 1;
    }

    /* A new step of the pieces script while they appear: one more piece, SE 0x13. */
    gSceneWork.u.exodia.pieceFrame = gSceneWork.anims[0].stepIdx;
    if (gSceneWork.u.exodia.piecesState == PIECES_APPEAR
        && gSceneWork.u.exodia.pieceFrame != gSceneWork.u.exodia.prevPieceFrame) {
        gSceneWork.u.exodia.prevPieceFrame = gSceneWork.u.exodia.pieceFrame;
        PlaySE(0x13);
    }
    for (i = 0; i < gSceneWork.animCount; i++)
        AnimStateTick(&gSceneWork.anims[i]);
    /* Sheet quadrant sheetY 1 = tile 0x200 and up (OBJ_VRAM_BITMAP). */
    for (i = 0; i < gSceneWork.animCount; i++) {
        OamListAddSpriteGroup((u16 *)anim->pieces, anim->layer, anim->pieceCount, anim->x, anim->y,
                              OAM_GROUP_TEMPLATE_POS, 0, 0, 1, OAM_TILES_SHEET16, 0, &gSceneWork.oamList);
        anim++;
    }
    OamListFlush(&gSceneWork.oamList);
    OamListClear((u8 *)&gSceneWork.oamList);
    Timer_Tick(&gSceneWork.u.exodia.timer);
    return 0;
}

/* EXODIA_STEP_GATHER_PIECES, per frame: the fade to white speeds up (step 0x40 * (1 - cos(pulsePhase)),
 * pulsePhase 0 -> 0x40), and each piece takes one step along its line to the centre. At full white a 1-frame
 * timer starts; when it expires SE 0x15 plays and the step returns 1. */
u16 ExodiaScene_GatherPieces(void)
{
    u8 i;
    const struct OamTemplate *tmpl;

    FadeTick(&gSceneWork.u.exodia.fade);
    if (gSceneWork.u.exodia.pulsePhase < 0x40)
        gSceneWork.u.exodia.pulsePhase++;
    gSceneWork.u.exodia.fade.step = MulFix8Int(0x40, 0x100 - gSineTable[gSceneWork.u.exodia.pulsePhase + 0x40]);
    if (gSceneWork.u.exodia.fade.state == FADE_STATE_FADED_OUT) {
        gSceneWork.u.exodia.fade.state = FADE_STATE_IDLE;
        Timer_Start(&gSceneWork.u.exodia.timer, 1);
    }
    if (gSceneWork.u.exodia.timer.state == TICK_DONE) {
        PlaySE(0x15);
        return 1;
    }
    tmpl = gExodiaPieceOamTemplates;
    for (i = 0; i < 5; i++) {
        LineStep(&gSceneWork.u.exodia.pieceLines[i]);
        gSceneWork.anims[i].x = gSceneWork.u.exodia.pieceLines[i].x;
        gSceneWork.anims[i].y = gSceneWork.u.exodia.pieceLines[i].y;
        OamListAddSpriteGroup((u16 *)tmpl++, 0, 1, gSceneWork.u.exodia.pieceLines[i].x,
                              gSceneWork.u.exodia.pieceLines[i].y, OAM_GROUP_ABS_POS, 0, 0, 1, OAM_TILES_SHEET16,
                              0, &gSceneWork.oamList);
    }
    OamListFlush(&gSceneWork.oamList);
    OamListClear((u8 *)&gSceneWork.oamList);
    Timer_Tick(&gSceneWork.u.exodia.timer);
    return 0;
}

/* Sprites of steps 6-8, per frame: anims[0] (layer 1, priority 2) and the affine anims[1] (priority 1). Both
 * are ticked; anims[0] is frozen once it reaches its step 1, anims[1] is restarted whenever it ends. Advances
 * the HBlank wave phase every other frame and pulses the size of the affine sprites (affine record 0): scale
 * 0x80 + 0x90 * (1 - cos(pulsePhase)), 0.5 to 1.625. */
void ExodiaScene_DrawSprites(void)
{
    struct AnimState *anim = &gSceneWork.anims[0];

    OamListAddSpriteGroup((u16 *)anim->pieces, 1, anim->pieceCount, anim->x, anim->y, OAM_GROUP_TEMPLATE_POS, 2,
                          0, 0, OAM_TILES_SHEET16, 0, &gSceneWork.oamList);
    ExodiaScene_DrawAffineAnim(&gSceneWork.anims[1], 1, &gSceneWork.oamList);
    AnimStateTick(&gSceneWork.anims[0]);
    AnimStateTick(&gSceneWork.anims[1]);
    if (gSceneWork.anims[0].stepIdx == 1)
        gSceneWork.anims[0].active = ANIM_HIDDEN;
    gSceneWork.anims[1].active = ANIM_PLAYING;
    gSceneWork.u.exodia.waveTick++;
    if (gSceneWork.u.exodia.waveTick & 2) {
        gSceneWork.u.exodia.wavePhase++;
        gSceneWork.u.exodia.waveTick = 0;
    }
    gSceneWork.aff[0].scaleX = gSceneWork.aff[0].scaleY =
        MulFix8Int(0x90, 0x100 - gSineTable[gSceneWork.u.exodia.pulsePhase + 0x40]) + 0x80;
    gSceneWork.u.exodia.pulsePhase++;
    ObjAffineApply(gSceneWork.aff);
    OamListFlush(&gSceneWork.oamList);
    OamListClear((u8 *)&gSceneWork.oamList);
}

/* EXODIA_STEP_FINALE, per frame: when the timer expires (150 frames: ExodiaScene_LoadFlames starts it, only
 * this step ticks it), a fade to black starts. Once pulsePhase reaches 0x50 the palettes are tinted towards
 * red, palFade.step = 0x1F * (1 - sin(pulsePhase - 0x10)). Returns 1 (and masks the HBlank interrupt) when
 * the screen is black or B is pressed. */
u16 ExodiaScene_Finale(void)
{
    FadeTick(&gSceneWork.u.exodia.fade);
    if (gSceneWork.u.exodia.fade.state == FADE_STATE_FADED_OUT) {
        gSceneWork.u.exodia.fade.state = FADE_STATE_IDLE;
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_HBLANK;
        REG_IME = 1;
        return 1;
    }
    if (gSceneWork.u.exodia.timer.state == TICK_DONE) {
        FadeStart(FADE_BLACK, 0x100, 0, &gSceneWork.u.exodia.fade);
        gSceneWork.u.exodia.timer.state = TICK_IDLE;
        gSceneWork.anims[1].active = ANIM_HIDDEN;
    }
    ExodiaScene_DrawSprites();
    if (gSceneWork.u.exodia.pulsePhase >= 0x50) {
        PalFade_Apply(&gSceneWork.u.exodia.palFade);
        gSceneWork.u.exodia.palFade.step =
            MulFix8Int(0x1F00, 0x100 - gSineTable[gSceneWork.u.exodia.pulsePhase - 0x10]) >> 8;
    }
    Timer_Tick(&gSceneWork.u.exodia.timer);
    if (gMain.newKeys & B_BUTTON) {
        REG_IME = 0;
        REG_IE &= ~INTR_FLAG_HBLANK;
        REG_IME = 1;
        return 1;
    }
    return 0;
}

/* EXODIA_STEP_FADE_IN_EYE: ticks the fade-in from black; returns 1 when the 60-frame timer has expired. */
u16 ExodiaScene_FadeInEye(void)
{
    FadeTick(&gSceneWork.u.exodia.fade);
    if (gSceneWork.u.exodia.timer.state == TICK_DONE)
        return 1;
    Timer_Tick(&gSceneWork.u.exodia.timer);
    return 0;
}

/* EXODIA_STEP_FADE_IN_FLAMES: draws the sprites and ticks the fade-in from white. When it ends, a new fade
 * starts (level 0x1000 down by 0x40 per frame; ExodiaScene_BlendFlames uses the level as BG0's alpha), and BG0
 * is blended over the other layers (BLDALPHA 13/16, 8/16). Returns 1 then. */
u16 ExodiaScene_FadeInFlames(void)
{
    ExodiaScene_DrawSprites();
    if (gSceneWork.u.exodia.fade.state == FADE_STATE_FADED_IN) {
        gSceneWork.u.exodia.fade.state = FADE_STATE_IDLE;
        FadeStart(FADE_WHITE, -0x40, 0, &gSceneWork.u.exodia.fade);
        REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
        REG_BLDALPHA = BLDALPHA_BLEND(13, 8);
        return 1;
    }
    FadeTick(&gSceneWork.u.exodia.fade);
    return 0;
}

/* EXODIA_STEP_BLEND_FLAMES: draws the sprites and sets BG0's alpha EVA to the fade level >> 8 (EVB 8);
 * returns 1 once the level is down to 0xA00 (EVA 10), else ticks the fade. */
u16 ExodiaScene_BlendFlames(void)
{
    u32 level;

    ExodiaScene_DrawSprites();
    /* level holds the u16 fade level shifted to the top halfword; >> 24 is level >> 8. */
    REG_BLDALPHA = ((level = gSceneWork.u.exodia.fade.level << 16) >> 24) | BLDALPHA_BLEND(0, 8);
    if (level <= 0xA00 << 16) {
        gSceneWork.u.exodia.fade.state = FADE_STATE_IDLE;
        return 1;
    }
    FadeTick(&gSceneWork.u.exodia.fade);
    return 0;
}

/* Scene handler of DUEL_SCENE_EXODIA_WIN: runs gExodiaSceneSteps[gDuelScene.sceneStep] (enum
 * ExodiaSceneStep), advancing when a step returns non-zero. Returns 1 at the table's NULL end, else 0. */
u16 ExodiaScene_Run(void)
{
    u16 (*step)(void) = gExodiaSceneSteps[gDuelScene.sceneStep];

    if (step != NULL) {
        if (step())
            gDuelScene.sceneStep++;
        return 0;
    }
    return 1;
}

/* ---- Destiny Board win scene helpers ---- */

/* HBlank handler: two wavy, drifting layers. BG0 HOFS = wave[(phase + line / 2) % 64] + phase, BG3 HOFS =
 * phase - wave[(phase + line / 4) % 64]. */
void DestinyBoardScene_HBlank(void)
{
    REG_BG0HOFS = gDestinyBoardWaveTable[(gSceneWork.u.destinyBoard.wavePhase + (REG_VCOUNT >> 1)) % 64]
                + gSceneWork.u.destinyBoard.wavePhase;
    REG_BG3HOFS = gSceneWork.u.destinyBoard.wavePhase
                - gDestinyBoardWaveTable[(gSceneWork.u.destinyBoard.wavePhase + (REG_VCOUNT >> 2)) % 64];
}

/* Advances the HBlank wave phase every 9 frames (waveTimer counts 8 down to 0xFF). */
void DestinyBoardScene_AdvanceWave(void)
{
    if (--gSceneWork.u.destinyBoard.waveTimer == 0xFF) {
        gSceneWork.u.destinyBoard.waveTimer = 8;
        gSceneWork.u.destinyBoard.wavePhase++;
    }
}

/* Sets up a scroll layer: position 0, speed and target (12.4 fixed), stopped, row -1 (so the first
 * ScrollLayer_StreamRow copies a row), the source map (30 entries per row) and the BG map. */
void ScrollLayer_Init(u8 *srcMap, u8 *bgMap, s16 speed, s16 target, struct ScrollLayer *layer)
{
    layer->pos = 0;
    layer->speed = speed;
    layer->target = target;
    layer->moving = 0;
    layer->row = -1;
    layer->srcMap = (const u16 *)srcMap;
    layer->bgMap = (u16 *)bgMap;
}

/* If the layer is moving: pos += speed; it stops exactly at target (the stop test depends on the sign of
 * speed). */
void ScrollLayer_Move(struct ScrollLayer *layer)
{
    if (layer->moving) {
        layer->pos += layer->speed;
        if (layer->speed < 0) {
            if (layer->pos <= layer->target) {
                layer->moving = 0;
                layer->pos = layer->target;
            }
        } else {
            if (layer->pos >= layer->target) {
                layer->moving = 0;
                layer->pos = layer->target;
            }
        }
    }
}

/* When the scroll (pos >> 4 pixels) enters another 8-pixel tile row, copies row `row + 22` of the source map
 * (30 entries) into row (row - 1) & 31 of the BG map, the tile row just above the top of the screen. */
void ScrollLayer_StreamRow(struct ScrollLayer *layer)
{
    int oldRow = layer->row;
    int scrollY = layer->pos >> 4;

    if (oldRow != scrollY / 8) {
        layer->row = scrollY / 8;
        CopyMapRect((u16 *)layer->srcMap + (layer->row + 22) * 30, layer->bgMap + ((layer->row - 1) & 31) * 32,
                    30, 1);
    }
}

/* VBlank callback: BGn HOFS = 0 and BGn VOFS = layers[n].pos >> 4 for BG0-3. */
void DestinyBoardScene_VBlank(void)
{
    REG_BG0HOFS = 0;
    REG_BG0VOFS = gSceneWork.u.destinyBoard.layers[0].pos >> 4;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = gSceneWork.u.destinyBoard.layers[1].pos >> 4;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = gSceneWork.u.destinyBoard.layers[2].pos >> 4;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = gSceneWork.u.destinyBoard.layers[3].pos >> 4;
}

/* Letter n of the Destiny Board scene (enum FinalLetterState in .state, flight progress in .t). */
#define LETTER(n) (gSceneWork.u.destinyBoard.letters[n])

/* Pulse of flying letter n: the scale of affine record n is 1.0 - 0.75 * sin(t * 3 / 2). */
#define SET_LETTER_PULSE(n)                                                                                 \
    gSceneWork.aff[n].scaleX = gSceneWork.aff[n].scaleY =                                                   \
        0x100 - MulFix8Int(0xC0, SIN((LETTER(n).t * 3 / 2) & 0xFF))

/*
 * A flying letter is drawn with double-size affine sprites, whose top-left corner moves up and left by half
 * the sprite size: 32 px for the 64x64 aura, 16 px for the 32x32 letter. On entry letterDx / letterDy hold
 * the arc offset; this adds the two recentring offsets in the high halfword ((v << 16) + 0x200000) >> 16, as
 * the ROM does, which keeps the u16 wrap-around.
 * FAKEMATCH: the register bindings (packed X in pxReg, 0x200000 in bigReg, 0x100000 in smallReg) and the
 * empty asm statements reproduce the ROM's register choice; letter 4 (L) uses another set than letters 0-3.
 */
#define ADD_DOUBLE_SIZE_OFFSETS(pxReg, bigReg, smallReg)    \
    {                                                       \
        register u32 px asm(pxReg);                         \
        register u32 big asm(bigReg);                       \
        register u32 small asm(smallReg);                   \
        u32 tmp;                                            \
        px = (u32)letterDx << 16;                           \
        asm("" : "+r"(px));                                 \
        big = 0x200000;                                     \
        asm("" : "+r"(big));                                \
        tmp = px + big;                                     \
        auraDx = tmp >> 16;                                 \
        tmp = ((u32)letterDy << 16) + big;                  \
        auraDy = tmp >> 16;                                 \
        small = 0x100000;                                   \
        asm("" : "+r"(small));                              \
        px += small;                                        \
        asm("" : "+r"(px));                                 \
        letterDx = px >> 16;                                \
        letterDy = (((u32)letterDy << 16) + small) >> 16;   \
    }

/*
 * Draws the five F-I-N-A-L letters of the Destiny Board relative to (x, y). Each letter is two sprites of the
 * letters animation (gFinalLettersAnim = gSceneWork.anims[3]): a 64x64 ghost aura (template 1 + i) and the
 * 32x32 letter (template 6 + i). By letters[i].state (enum FinalLetterState):
 *   LETTER_AT_REST  drawn in place
 *   LETTER_FLYING   drawn as double-size affine sprites (matrix i) moving along a per-letter sine arc; t
 *                   advances by 2 per frame (1 for A) and at its end value (0x6C, 0x84, 0x96, 0x6D, 0x6E) the
 *                   letter is gone
 *   LETTER_GONE     not drawn
 * The scale of affine record i follows t (1.0 at t = 0). While it is above 1.0 (the letter is enlarged) the
 * letter is drawn on OAM layer 6, in front of the others on layer 7. The first argument is not used.
 */
void DestinyBoardScene_DrawFinalLetters(u32 unused, u16 x, u16 y)
{
    u8 i;
    u16 affineMode, auraDx;
    /* FAKEMATCH: a word-sized destination preserves the chained zero copy from r9. */
    u32 auraDy;
    u16 letterDy;
    /* FAKEMATCH: retain letterDx in r9; the explicit u16 casts below preserve the coordinate wrap. */
    register u32 letterDx asm("r9");
    u32 layer;
    s32 hidden = 0;

    for (i = 0; i < 5; i++) {
        layer = 7;
        /* Matching: five separate cases (case 0 with the constant index) give the ROM's jump table. */
        switch (i) {
        case 0:
            SET_LETTER_PULSE(0);
            break;
        case 1:
            SET_LETTER_PULSE(i);
            break;
        case 2:
            SET_LETTER_PULSE(i);
            break;
        case 3:
            SET_LETTER_PULSE(i);
            break;
        case 4:
            SET_LETTER_PULSE(i);
            break;
        }
        /* The arc of each letter: x = rx * (sin(t + 0x30) - sin(0x30)) (letter N: a cosine at double speed),
         * y = ry * (sin(0x10) - sin(t + 0x10)). */
        switch (i) {
        case 0: /* F */
            switch (LETTER(0).state) {
            case LETTER_AT_REST:
                auraDy = letterDx = auraDx = 0;
                letterDy = 0;
                affineMode = 0;
                break;
            case LETTER_FLYING:
                /* FAKEMATCH: narrow the word-sized r9 value to the arc width. */
                letterDx = (u16)(-(MulFix8Int(0xA00, SIN(0x30)) >> 4)
                                 + (MulFix8Int(0xA00, SIN((LETTER(0).t + 0x30) & 0xFF)) >> 4));
                letterDy = (MulFix8Int(0x6000, SIN(0x10)) >> 8)
                         - (MulFix8Int(0x6000, SIN(LETTER(0).t + 0x10)) >> 8);
                ADD_DOUBLE_SIZE_OFFSETS("r1", "r2", "r3");
                affineMode = OAM_ATTR0_AFFINE_DOUBLE;
                if (LETTER(0).t == 0x6C)
                    LETTER(0).state++;
                LETTER(0).t += 2;
                break;
            case LETTER_GONE:
                hidden = 1;
                break;
            }
            if ((s16)gSceneWork.aff[0].scaleX > 0x100)
                layer = 6;
            break;
        case 1: /* I */
            switch (LETTER(i).state) {
            case LETTER_AT_REST:
                auraDy = letterDx = auraDx = 0;
                letterDy = 0;
                affineMode = 0;
                break;
            case LETTER_FLYING:
                /* FAKEMATCH: narrow the word-sized r9 value to the arc width. */
                letterDx = (u16)(-(MulFix8Int(0x400, SIN(0x30)) >> 4)
                                 + (MulFix8Int(0x400, SIN((LETTER(i).t + 0x30) & 0xFF)) >> 4));
                letterDy = (MulFix8Int(0x4000, SIN(0x10)) >> 8)
                         - (MulFix8Int(0x4000, SIN(LETTER(i).t + 0x10)) >> 8);
                ADD_DOUBLE_SIZE_OFFSETS("r1", "r2", "r3");
                affineMode = OAM_ATTR0_AFFINE_DOUBLE;
                if (LETTER(i).t == 0x84)
                    LETTER(i).state++;
                LETTER(i).t += 2;
                break;
            case LETTER_GONE:
                hidden = 1;
                break;
            }
            if ((s16)gSceneWork.aff[i].scaleX > 0x100)
                layer = 6;
            break;
        case 2: /* N */
            switch (LETTER(i).state) {
            case LETTER_AT_REST:
                auraDy = letterDx = auraDx = 0;
                letterDy = 0;
                affineMode = 0;
                break;
            case LETTER_FLYING:
                /* FAKEMATCH: narrow the word-sized r9 value to the arc width. */
                letterDx = (u16)((MulFix8Int(0x200, SIN(((LETTER(i).t + 0x30) * 2 & 0xFF) + 0x40)) >> 4)
                                 + 0x23 - (MulFix8Int(0x200, SIN(0x70)) >> 4));
                letterDy = (MulFix8Int(0x7000, SIN(0x10)) >> 8)
                         - (MulFix8Int(0x7000, SIN(LETTER(i).t + 0x10)) >> 8);
                ADD_DOUBLE_SIZE_OFFSETS("r1", "r2", "r3");
                affineMode = OAM_ATTR0_AFFINE_DOUBLE;
                if (LETTER(i).t == 0x96)
                    LETTER(i).state++;
                LETTER(i).t += 2;
                break;
            case LETTER_GONE:
                hidden = 1;
                break;
            }
            if ((s16)gSceneWork.aff[i].scaleX > 0x100)
                layer = 6;
            break;
        case 3: /* A: mirrored arc, half speed */
            switch (LETTER(i).state) {
            case LETTER_AT_REST:
                auraDy = letterDx = auraDx = 0;
                letterDy = 0;
                affineMode = 0;
                break;
            case LETTER_FLYING:
                /* FAKEMATCH: narrow the word-sized r9 value to the arc width. */
                letterDx = (u16)((MulFix8Int(0xA00, SIN(0x30)) >> 4)
                                 - (MulFix8Int(0xA00, SIN((LETTER(i).t + 0x30) & 0xFF)) >> 4));
                letterDy = (MulFix8Int(0xA000, SIN(0x10)) >> 8)
                         - (MulFix8Int(0xA000, SIN(LETTER(i).t + 0x10)) >> 8);
                ADD_DOUBLE_SIZE_OFFSETS("r1", "r2", "r3");
                affineMode = OAM_ATTR0_AFFINE_DOUBLE;
                if (LETTER(i).t == 0x6D)
                    LETTER(i).state++;
                LETTER(i).t += 1;
                break;
            case LETTER_GONE:
                hidden = 1;
                break;
            }
            if ((s16)gSceneWork.aff[i].scaleX > 0x100)
                layer = 6;
            break;
        case 4: /* L: mirrored arc, y mirrored too */
            switch (LETTER(i).state) {
            case LETTER_AT_REST:
                auraDy = letterDx = auraDx = 0;
                letterDy = 0;
                affineMode = 0;
                break;
            case LETTER_FLYING:
                /* FAKEMATCH: narrow the word-sized r9 value to the arc width. */
                letterDx = (u16)((MulFix8Int(0xA00, SIN(0x30)) >> 4)
                                 - (MulFix8Int(0xA00, SIN((LETTER(i).t + 0x30) & 0xFF)) >> 4));
                letterDy = -(MulFix8Int(0x6000, SIN(0x10)) >> 8)
                         + (MulFix8Int(0x6000, SIN(LETTER(i).t + 0x10)) >> 8);
                ADD_DOUBLE_SIZE_OFFSETS("r2", "r3", "r1");
                {
                    register u32 af asm("r2"); /* FAKEMATCH: the register of the affineMode store. */
                    register u32 t asm("r3");  /* FAKEMATCH: the register of the end test. */
                    af = OAM_ATTR0_AFFINE_DOUBLE;
                    asm("" : : "r"(af)); /* FAKEMATCH: keeps this tail apart from the other letters' tails. */
                    affineMode = af;
                    t = LETTER(i).t;
                    asm("" : : "r"(t)); /* FAKEMATCH: keeps the ROM's register allocation and branch layout. */
                    if (t == 0x6E)
                        LETTER(i).state++;
                    LETTER(i).t += 2;
                }
                break;
            case LETTER_GONE:
                hidden = 1;
                break;
            }
            if ((s16)gSceneWork.aff[i].scaleX > 0x100)
                layer = 6;
            break;
        }
        if (!hidden) {
            s16 sx = x;
            s16 sy = y;
            u16 *attrs;

            /* Templates 1 + i (aura) and 6 + i (letter) of the current animation step, offset from (x, y).
             * Matching: the alias symbol gFinalLettersAnim (= gSceneWork.anims[3]) keeps the ROM's separate
             * base load. */
            attrs = OamListAddSpriteGroupWide((u16 *)&gFinalLettersAnim.pieces[i + 1], layer, 1,
                                              sx - (s16)auraDx, sy - (s16)auraDy, OAM_GROUP_QUAD_REL_POS, 1, 0, 0,
                                              OAM_TILES_SHEET16, affineMode, &gSceneWork.oamList);
            if (affineMode == OAM_ATTR0_AFFINE_DOUBLE)
                attrs[1] = (attrs[1] & 0xC1FF) | OAM_ATTR1_MATRIX(i);
            attrs = OamListAddSpriteGroupWide((u16 *)&gFinalLettersAnim.pieces[i + 6], layer, 1,
                                              sx - (s16)letterDx, sy - (s16)letterDy, OAM_GROUP_QUAD_REL_POS, 1, 0, 0,
                                              OAM_TILES_SHEET16, affineMode, &gSceneWork.oamList);
            if (affineMode == OAM_ATTR0_AFFINE_DOUBLE)
                attrs[1] = (attrs[1] & 0xC1FF) | OAM_ATTR1_MATRIX(i);
        }
        hidden = 0;
    }
}
