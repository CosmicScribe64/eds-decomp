/*
 * Duel commands 0x72-0x76: full-screen card presentations (wiki/functions/duel-cmd-presentation-c.md).
 *
 * Each handler shows the card arg2 (a card ID) in the middle of the screen and animates it in and out.
 * LoadCardFrame, LoadCardPicture and DrawCardInfo draw the card image into OBJ VRAM (8bpp, 2D mapping) after
 * UnloadDuelUiGfx has made room, and the image is shown as a grid of 4 x 5 sprites of 32x32 pixels
 * (CARD_GRID_*). The handlers differ only in how the grid moves and blends:
 *
 *   0x72 DuelCmd_ShowCardZoomIn          grows from the centre
 *   0x73 DuelCmd_ShowCardEffect          grows from the centre while the background flashes white
 *   0x74 DuelCmd_ShowCardScatter         appears in place, then its tiles fly apart
 *   0x75 DuelCmd_ShowCardUnrollDown      unrolls downwards, holds, rolls back up
 *   0x76 DuelCmd_ShowCardUnrollSideways  opens from its vertical centre line, flashes, closes again
 *
 * DuelCmd_ShowCardAssemble (0x71, duel_cmd_turn.c) is the sixth member of the family. Every handler runs one
 * step per frame on gDuelCmd.step / gDuelCmd.timer, restores the duel sprites with LoadDuelUiGfx and clears
 * gDuelCmd.running at the end. Fast-forward (B held, or gDuelScreen.fast) skips timer frames.
 */
#include "global.h"
#include "legacy/gba.h"            /* REG_BLDCNT, REG_BLDALPHA, REG_BLDY, B_BUTTON, BLDCNT_* */
#include "legacy/main.h"           /* gMain.heldKeys */
#include "legacy/sound.h"          /* PlaySE */
#include "sprite.h"         /* SPRITE_SHAPE_32x32 */
#include "duel_flow.h"      /* gPulseScaleCurve */
#include "duel_cmd.h"       /* gDuelCmd, gDuelCmdT16, gScatterScaleCurve */

#ifdef DISPCNT_MODE_4
#include "duel_screen.h"    /* gDuelScreen, DuelScreen_ScrollToZone, the card-image loaders */
#else
/* ---- BEGIN pre-H0 subset ---- */
/*
 * Before H0 (build/readability/HEADERS.md) include/gba.h, main.h, sound.h and duel.h still hold the legacy
 * headers, and duel_screen.h cannot be included (it needs the new duel.h). This block repeats what the unit
 * uses from the new gba.h, sound.h and duel_screen.h, with the same names, values and prototypes (truncated
 * structs end after the last field used here). With the new headers installed the block is skipped; then
 * delete it (build/readability/issues/duel_cmd_presentation.md).
 */
#define BLDCNT_TGT1_BG0         0x0001
#define BLDCNT_TGT1_BG1         0x0002
#define BLDCNT_TGT1_BG2         0x0004
#define BLDCNT_TGT1_OBJ         0x0010
#define BLDCNT_TGT1_BD          0x0020
#define BLDCNT_EFFECT_BLEND     0x0040
#define BLDCNT_EFFECT_LIGHTEN   0x0080
#define BLDCNT_TGT2_BG0         0x0100
#define BLDCNT_TGT2_BG1         0x0200
#define BLDCNT_TGT2_BG2         0x0400
#define BLDCNT_TGT2_BG3         0x0800
#define BLDCNT_TGT2_OBJ         0x1000
#define BLDCNT_TGT2_BD          0x2000
void PlaySE(u32 seId);
struct DuelScreen {
    u8 fast:1;              /* +0x000 bit 0: fast-forward card animations, as if B were held */
    u8 uiGfxLoaded:1;
    u8 active:1;
    u8 unk0_3:5;
};
extern struct DuelScreen gDuelScreen;
void DuelScreen_ScrollToZone(u32 player, u32 area);
void LoadDuelUiGfx(void);
void UnloadDuelUiGfx(void);
void TextCellsClear(void);
void DuelInfo_DrawCardNameCentered(u16 cardId);
void LoadCardFrame(u16 cardId);
void LoadCardPicture(u16 cardId);
void DrawCardInfo(u16 cardId);
/* ---- END pre-H0 subset ---- */
#endif

/*
 * Local views of the sprite emitters (sprite.h declares the shape and tile as u16). This unit calls them with
 * u32 parameters: the u16 ones add narrowing at the call sites, and the ROM has none.
 */
void AddSprite8bppU32(u32 yx, u32 shape, u32 tile) asm("AddSprite8bpp");
void AddSprite8bppAlphaU32(u32 yx, u32 shape, u32 tile) asm("AddSprite8bppAlpha");
void AddAffineSprite8bppAlphaU32(u32 yx, u32 shape, u32 tile, u32 scaleAngle) asm("AddAffineSprite8bppAlpha");

/* Fast-forward: B held, or the duel's fast mode. */
#define FAST_FORWARD() ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)

/*
 * The card grid: 4 columns x 5 rows of 32x32 sprites, top-left corner at (0x44, 2), so the card is centred
 * on (CARD_CENTER_X, CARD_CENTER_Y). CARD_GRID_TILE is the tile argument of the AddSprite8bpp* calls for
 * cell (row, col) of the card image.
 */
#define CARD_GRID_X(col)        ((col) * 32 + 0x44)
#define CARD_GRID_Y(row)        ((row) * 32 + 2)
#define CARD_GRID_TILE(row, col) ((u16)((col) * 4) + ((u16)((row) * 2 + 1) << 5))
#define CARD_CENTER_X           0x68
#define CARD_CENTER_Y           0x40

/* BLDCNT values of the presentations. */
#define BLEND_CARD_OVER_BG      (BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 | BLDCNT_TGT2_BG2 \
                                 | BLDCNT_TGT2_BG3)                     /* 0x0F40: card alpha-blended over BG0-3 */
#define BRIGHTEN_BACKGROUND     (BLDCNT_TGT1_BG0 | BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT1_BD \
                                 | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_BG0 | BLDCNT_TGT2_BG1 \
                                 | BLDCNT_TGT2_BG2 | BLDCNT_TGT2_BD)    /* 0x27A7: BG0-2 and backdrop to white */
#define BRIGHTEN_SPRITES        (BLDCNT_TGT1_OBJ | BLDCNT_EFFECT_LIGHTEN | BLDCNT_TGT2_OBJ) /* 0x1090 */

/*
 * BLDALPHA value: weight eva of the card (1st target) and evb of the background (2nd target), 0-16 each.
 * Same value as gba.h's BLDALPHA_BLEND, but ORed in the ROM's operand order (the other order changes the code).
 */
#define BLEND_WEIGHTS(eva, evb) ((eva) | ((evb) << 8))

/* Sound effect played when the card has opened (no SE name yet; DuelCmd_ShowCardAssemble plays it too). */
#define SE_CARD_SHOWN           0x2C

/*
 * Command 0x72 (DUEL_CMD_SHOW_CARD_ZOOM_IN), arg2 = card ID.
 * Steps 0-4 scroll the field to player 0's monster row, free the sprite VRAM and build the card image; step 5
 * shows the card name in the info bar. Step 6 runs for 0x78 frames: for the first 32 the grid grows from the
 * centre (position * timer / 32), for the first 16 each cell also pops (gPulseScaleCurve) and the card fades
 * in, and from frame 0x68 it fades out again. Step 7 is one idle frame; then the duel sprites are reloaded.
 */
void DuelCmd_ShowCardZoomIn(void)
{
    int row, col;
    int x, y;
    int tZoom, tBlend, tPulse, tEnd;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 5:
        TextCellsClear();
        DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 6:
        /* FAKEMATCH: the timer is read through the u16-container view gDuelCmdT16 with (u8) casts. The extra
         * RTL keeps the inner loop above loop.c's hoisting threshold, so the timer address stays inside the
         * loop and the base copy is spilled, as in the ROM (wiki: duel-cmd-presentation-c). */
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                x = CARD_GRID_X(col);
                y = CARD_GRID_Y(row);
                if ((tZoom = (u8)gDuelCmdT16.timer) <= 0x1F) {
                    /* grow from the centre: pos = centre + (pos - centre) * t / 32 */
                    x -= CARD_CENTER_X;
                    y -= CARD_CENTER_Y;
                    x *= tZoom;
                    y *= tZoom;
                    x /= 32;
                    y /= 32;
                    x += CARD_CENTER_X;
                    y += CARD_CENTER_Y;
                }
                tBlend = (u8)gDuelCmdT16.timer;
                if (tBlend < 16) {
                    /* fade in: card weight t / 16 */
                    REG_BLDCNT = BLEND_CARD_OVER_BG;
                    REG_BLDALPHA = BLEND_WEIGHTS((u8)tBlend, (u8)(16 - tBlend));
                } else if (tBlend > 0x67) {
                    /* fade out over frames 0x68-0x77 */
                    REG_BLDCNT = BLEND_CARD_OVER_BG;
                    REG_BLDALPHA = BLEND_WEIGHTS((u8)(0x78 - tBlend), (u8)(tBlend - 0x68));
                } else {
                    REG_BLDCNT = 0;
                    REG_BLDALPHA = 0;
                }
                tPulse = (u8)gDuelCmdT16.timer;
                if (tPulse < 16)
                    AddAffineSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col),
                                                gPulseScaleCurve[(u8)tPulse] << 16);
                else
                    AddSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        tEnd = gDuelCmd.timer;
        if (tEnd < 0x78) {
            if (FAST_FORWARD() && tEnd <= 0x6F)
                gDuelCmd.timer = tEnd + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 7:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0x73 (DUEL_CMD_SHOW_CARD_EFFECT), arg2 = card ID: the "card effect activated" announcement, queued
 * by ShowCardEffect before an effect resolves. DuelCmd_ShowCardZoomIn with a white flash of the background in
 * step 6: after the fade-in, BG0-2 and the backdrop brighten (BLDY 0 -> 0x1F over frames 16-47) and darken
 * back (frames 48-79), then the card fades out from frame 0x68.
 */
void DuelCmd_ShowCardEffect(void)
{
    int row, col;
    int x, y;
    int tZoom, tBlend, tPulse, tEnd;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 5:
        TextCellsClear();
        DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 6:
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                x = CARD_GRID_X(col);
                y = CARD_GRID_Y(row);
                if ((tZoom = gDuelCmd.timer) <= 0x1F) {
                    x -= CARD_CENTER_X;
                    y -= CARD_CENTER_Y;
                    x *= tZoom;
                    y *= tZoom;
                    x /= 32;
                    y /= 32;
                    x += CARD_CENTER_X;
                    y += CARD_CENTER_Y;
                }
                tBlend = gDuelCmd.timer;
                if (tBlend < 16) {
                    REG_BLDCNT = BLEND_CARD_OVER_BG;
                    REG_BLDALPHA = BLEND_WEIGHTS(tBlend, (u8)(16 - tBlend));
                } else if ((u32)((u8)tBlend - 0x10) <= 0x1F) {
                    /* frames 16-47: the background brightens. FAKEMATCH: the (u8) cast is not needed for the
                     * value (tBlend < 0x80); without it this function and the next three no longer match. */
                    REG_BLDCNT = BRIGHTEN_BACKGROUND;
                    REG_BLDY = tBlend - 0x10;
                } else if ((u32)(tBlend - 0x30) <= 0x1F) {
                    /* frames 48-79: and darkens back */
                    REG_BLDCNT = BRIGHTEN_BACKGROUND;
                    REG_BLDY = 0x4F - tBlend;
                } else if (tBlend > 0x67) {
                    REG_BLDCNT = BLEND_CARD_OVER_BG;
                    REG_BLDALPHA = BLEND_WEIGHTS((u8)(0x78 - tBlend), (u8)(tBlend - 0x68));
                } else {
                    REG_BLDCNT = 0;
                    REG_BLDALPHA = 0;
                }
                tPulse = gDuelCmd.timer;
                if (tPulse < 16)
                    AddAffineSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col),
                                                gPulseScaleCurve[tPulse] << 16);
                else
                    AddSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        tEnd = gDuelCmd.timer;
        if (tEnd < 0x78) {
            if (FAST_FORWARD() && tEnd <= 0x6F)
                gDuelCmd.timer = tEnd + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 7:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0x74 (DUEL_CMD_SHOW_CARD_SCATTER), arg2 = card ID.
 * Steps 0-5 as DuelCmd_ShowCardZoomIn. In step 6 the card appears in place (cell pop and fade-in over 16
 * frames) and holds; from frame 0x68 its cells fly apart from the centre, scaled by
 * gScatterScaleCurve[timer - 0x68] / 256 (up to about 3.8x), while the card fades out.
 */
void DuelCmd_ShowCardScatter(void)
{
    int row, col;
    int x, y, dx, dy, k;
    int tScatter, tBlend, tPulse, tEnd;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 5:
        TextCellsClear();
        DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 6:
        /* FAKEMATCH: gDuelCmdT16 and the (u8) timer casts, as in DuelCmd_ShowCardZoomIn. */
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                x = CARD_GRID_X(col);
                y = CARD_GRID_Y(row);
                if ((tScatter = (u8)gDuelCmdT16.timer) > 0x67) {
                    /* fly apart: pos = centre + (pos - centre) * curve[t - 0x68] / 256 */
                    dx = x - CARD_CENTER_X;
                    dy = y - CARD_CENTER_Y;
                    k = tScatter - 0x68;
                    dx *= gScatterScaleCurve[k];
                    dy *= gScatterScaleCurve[k];
                    dx /= 256;
                    dy /= 256;
                    x = dx + CARD_CENTER_X;
                    y = dy + CARD_CENTER_Y;
                }
                tBlend = (u8)gDuelCmdT16.timer;
                if (tBlend < 16) {
                    REG_BLDCNT = BLEND_CARD_OVER_BG;
                    REG_BLDALPHA = BLEND_WEIGHTS((u8)tBlend, (u8)(16 - tBlend));
                } else if (tBlend > 0x67) {
                    REG_BLDCNT = BLEND_CARD_OVER_BG;
                    REG_BLDALPHA = BLEND_WEIGHTS((u8)(0x78 - tBlend), (u8)(tBlend - 0x68));
                } else {
                    REG_BLDCNT = 0;
                    REG_BLDALPHA = 0;
                }
                tPulse = (u8)gDuelCmdT16.timer;
                if (tPulse < 16)
                    AddAffineSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col),
                                                gPulseScaleCurve[(u8)tPulse] << 16);
                else
                    AddSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        tEnd = gDuelCmd.timer;
        if (tEnd < 0x78) {
            if (FAST_FORWARD() && tEnd <= 0x6F)
                gDuelCmd.timer = tEnd + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 7:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0x75 (DUEL_CMD_SHOW_CARD_UNROLL_DOWN), arg2 = card ID.
 * Step 0 clears the info bar and scrolls the field to player 0's monster row; steps 1-4 build the card
 * image. Step 5 unrolls the card downwards (row y * timer / 16) while it fades in, then shows the card name.
 * Step 6 holds the opaque card for 32 frames. Step 7 rolls it back up (row y * (0x38 - timer) / 16 from frame
 * 0x28) while it fades out. Step 8 is one idle frame; then the duel sprites are reloaded.
 */
void DuelCmd_ShowCardUnrollDown(void)
{
    int row, col;
    int x, y;
    int tRoll, tBlend, tEnd;

    switch (gDuelCmd.step) {
    case 0:
        TextCellsClear();
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 5:
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                x = CARD_GRID_X(col);
                y = CARD_GRID_Y(row);
                if (gDuelCmd.timer <= 0xF) {
                    y *= gDuelCmd.timer;
                    y /= 16;
                }
                AddSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        tBlend = gDuelCmd.timer;
        if (tBlend < 16) {
            REG_BLDCNT = BLEND_CARD_OVER_BG;
            REG_BLDALPHA = BLEND_WEIGHTS(tBlend, (u8)(16 - tBlend));
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 11)
            gDuelCmd.timer += 3;
        if (gDuelCmd.timer > 15) {
            TextCellsClear();
            DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 6:
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++)
                AddSprite8bppU32(CARD_GRID_X(col) | (CARD_GRID_Y(row) << 16), SPRITE_SHAPE_32x32,
                                 CARD_GRID_TILE(row, col));
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 0x17)
            gDuelCmd.timer += 7;
        if (gDuelCmd.timer > 0x1F) {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 7:
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                x = CARD_GRID_X(col);
                y = CARD_GRID_Y(row);
                if ((tRoll = gDuelCmd.timer) > 0x27) {
                    y *= (0x38 - tRoll);
                    y /= 16;
                }
                AddSprite8bppAlphaU32(x | (y << 16), SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        tBlend = gDuelCmd.timer;
        if (tBlend > 0x27) {
            REG_BLDCNT = BLEND_CARD_OVER_BG;
            REG_BLDALPHA = BLEND_WEIGHTS((u8)(0x38 - tBlend), (u8)(tBlend - 0x28));
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        tEnd = gDuelCmd.timer;
        if (tEnd < 0x38) {
            if (FAST_FORWARD() && tEnd <= 0x2F)
                gDuelCmd.timer = tEnd + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 8:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Command 0x76 (DUEL_CMD_SHOW_CARD_UNROLL_SIDEWAYS), arg2 = card ID.
 * Like DuelCmd_ShowCardUnrollDown, but the card opens horizontally from its centre line
 * ((x - centre) * timer / 16 + centre) and plays SE_CARD_SHOWN when it is open. Step 6 flashes the card
 * itself white (BLDY up over 16 frames, then down) instead of holding it, and step 7 closes it horizontally
 * while it fades out.
 */
void DuelCmd_ShowCardUnrollSideways(void)
{
    int row, col;
    int x, y;
    int tRoll, tBlend, tFlash, tEnd;

    switch (gDuelCmd.step) {
    case 0:
        TextCellsClear();
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 5: {
        int rowY;   /* CARD_GRID_Y(row) << 16, computed in the inner loop (see the wiki for the allocation) */

        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                rowY = CARD_GRID_Y(row) << 16;
                x = CARD_GRID_X(col);
                if (gDuelCmd.timer <= 0xF) {
                    x -= CARD_CENTER_X;
                    x *= gDuelCmd.timer;
                    x /= 16;
                    x += CARD_CENTER_X;
                }
                AddSprite8bppAlphaU32(x | rowY, SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        if (gDuelCmd.timer < 16) {
            REG_BLDCNT = BLEND_CARD_OVER_BG;
            REG_BLDALPHA = BLEND_WEIGHTS(gDuelCmd.timer, (u8)(16 - gDuelCmd.timer));
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 11)
            gDuelCmd.timer += 3;
        if (gDuelCmd.timer > 15) {
            TextCellsClear();
            DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
            PlaySE(SE_CARD_SHOWN);
        }
        break;
    }
    case 6:
        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                y = CARD_GRID_Y(row) << 16;
                AddSprite8bppU32(CARD_GRID_X(col) | y, SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        /* white flash on the card: BLDY 0 -> 15 over 16 frames, then back to 0 */
        tFlash = gDuelCmd.timer;
        if (tFlash < 0x20) {
            if (tFlash < 16) {
                REG_BLDY = tFlash;
                REG_BLDCNT = BRIGHTEN_SPRITES;
            } else {
                REG_BLDY = 0x1F - tFlash;
                REG_BLDCNT = BRIGHTEN_SPRITES;
            }
        } else {
            REG_BLDY = 0;
            REG_BLDCNT = 0;
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 0x1B)
            gDuelCmd.timer += 3;
        if (gDuelCmd.timer > 0x1F) {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 7: {
        int rowY;

        for (row = 0; row <= 4; row++) {
            for (col = 0; col <= 3; col++) {
                rowY = CARD_GRID_Y(row) << 16;
                x = CARD_GRID_X(col);
                if ((tRoll = gDuelCmd.timer) > 0x27) {
                    x -= CARD_CENTER_X;
                    x *= (0x38 - tRoll);
                    x /= 16;
                    x += CARD_CENTER_X;
                }
                AddSprite8bppAlphaU32(x | rowY, SPRITE_SHAPE_32x32, CARD_GRID_TILE(row, col));
            }
        }
        tBlend = gDuelCmd.timer;
        if (tBlend > 0x27) {
            REG_BLDCNT = BLEND_CARD_OVER_BG;
            REG_BLDALPHA = BLEND_WEIGHTS((u8)(0x38 - tBlend), (u8)(tBlend - 0x28));
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        tEnd = gDuelCmd.timer;
        if (tEnd < 0x38) {
            if (FAST_FORWARD() && tEnd <= 0x2F)
                gDuelCmd.timer = tEnd + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    }
    case 8:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}
