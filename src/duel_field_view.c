/*
 * duel_field_view (0x0802408C-0x08025107): the duel field view and the coin toss scene
 * (wiki/functions/duel-field-view-c.md).
 *
 *  - Field view (gDuelScreen, duel_screen.h): the vertical field scroll and the field cursor, both eased
 *    over 4 frames with gDuelScreenLerpWeights; the info-bar and text-box tile uploads; the sprite
 *    animation slot gDuelScreen.sprAnim; the requests of the five card animations (enum DuelAnimKind),
 *    which DuelAnim_Update dispatches. DuelMainStep calls DuelScreen_Update once per frame.
 *  - Coin toss (DUEL_SCENE_COIN_TOSS, duel_scenes.h; work area gCoinTossWork): up to 8 coins fly up and
 *    land on the faces encoded in gDuelScene.arg, and the coins that show the called face glint. The rest
 *    of the scene (sparkles, glints, matching) is in coin_toss_scene.c.
 */
#include "global.h"
#include "gba.h"                /* IO registers, VRAM / palette addresses, CpuSet */
#include "main.h"               /* gMain.vblankFlags, gMain.bgVofs */
#include "sound.h"              /* PlaySE */
#include "constants/duel.h"     /* enum DuelArea */

/* ---- BEGIN pre-H0 block ---- */
/*
 * Before H0 (build/readability/HEADERS.md) include/gba.h, main.h, sound.h and duel.h still hold the legacy
 * headers: they lack the names below, and duel_screen.h needs struct DuelLoc from the new duel.h. This
 * block repeats them with the new headers' values and layout, and defines GUARD_DUEL_H so that
 * duel_screen.h does not pull in the legacy duel.h. With the new gba.h installed the block is skipped;
 * then delete it (build/readability/issues/duel_field_view.md).
 */
#ifndef DISPCNT_MODE_4
#define GUARD_DUEL_H
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13: zone within the row, hand index, 0 for the piles */
    u16 isDefense:1;                /* bit 14: drawn sideways (defense position) */
    u16 isFaceUp:1;                 /* bit 15: drawn face up, else the card back */
    u16 unk2;                       /* +0x02: padding, copied with the word */
};

#define DISPCNT_MODE_4          0x0004
#define DISPCNT_OBJ_1D_MAP      0x0040
#define DISPCNT_BG_ALL_ON       0x0F00
#define DISPCNT_OBJ_ON          0x1000
#define BLDCNT_EFFECT_BLEND     0x0040
#define BLDCNT_TGT2_OBJ         0x1000
#define CPU_SET_SRC_FIXED       0x01000000
#define BG_CHAR_ADDR(n)         (VRAM + 0x4000 * (n))
#define VBLANK_COPY_OAM         0x1

void PlaySE(u32 seId);
#endif
/* ---- END pre-H0 block ---- */

#include "duel_screen.h"        /* gDuelScreen, gDuelZonePositions, DuelScreen_*, DuelCursor_*, DuelAnim_* */
#include "duel_scenes.h"        /* gDuelScene, gCoinTossWork, struct Coin / CoinToss, CoinToss_* */
#include "text_box.h"           /* gTextBox */
#include "turn_order.h"         /* gEgyptCorridorBitmap / Pal, Scroller_* */
#include "palette.h"            /* FadeStart, FadeTick, SetBldAlpha */
#include "sprite.h"             /* SprAnim*, OamList*, ObjAffineInit, AddAffineSprite */
#include "util.h"               /* MemClear16, CopyDoubleWords */

/*
 * Local views of callees, kept on purpose (build/readability/HEADERS.md "Keeping a deliberate local view"):
 * the header prototypes have narrower parameters or return values, which add or drop narrowing at the
 * call sites of this unit. Same symbols, other C types.
 */
/* sprite.h: u16 dx, dy. The wrappers pass their s16 parameters through without re-narrowing. */
extern void SprAnimDrawFrameS16(s16 dx, s16 dy, struct SprAnim *anim, u16 advance) asm("SprAnimDrawFrame");
extern void SprAnimDrawFrameAtS16(s16 x, s16 y, struct SprAnim *anim, u16 advance) asm("SprAnimDrawFrameAt");
/* util.h: s16 MulFix8(s16, s16). The flight code passes u16 times and uses the full int result. */
extern s32 MulFix8Int(s32 a, s32 b) asm("MulFix8");
/* duel_scenes.h: returns u8. CoinToss_Update tests the full register (no narrowing after the call). */
extern u32 CoinToss_CountUnfinishedU32(struct Coin *coins, u8 count) asm("CoinToss_CountUnfinished");
/* sprite.h: u8 / u16 parameters. The coin drawers pass the int tile and x without narrowing them. */
extern struct OamListEntry *OamListAddSpriteWide(u32 layer, u32 tile, s32 x, s32 y, u32 width, u32 height,
                                                 u32 bpp, u32 palette, u32 unused, u32 attr0Flags,
                                                 u32 attr1Bits, u32 priority, struct OamList *list)
    asm("OamListAddSprite");

/* Sound effect of a coin being thrown (only used here; constants/sound.h has no name for it yet). */
#define SE_COIN_TOSS 30

#ifndef CpuFill16
/* Fills `size` bytes at dest with the halfword `value` through a CpuSet fill, as the SDK's CpuFill16 macro
 * does. The source must be a volatile stack temporary: with a plain local the compiler moves the zero store
 * ahead of the address computation (mov r5, #0 before mov r0, sp), unlike the ROM. */
#define CpuFill16(value, dest, size)                                \
    {                                                               \
        vu16 fill_ = (value);                                       \
        CpuSet((void *)&fill_, (dest), CPU_SET_SRC_FIXED | (size) / 2); \
    }
#endif

/* Address of 4bpp OBJ tile n (in the bitmap modes the OBJ tiles start at tile 0x200, 0x06014000). */
#define OBJ_TILE_ADDR(n) ((void *)(OBJ_VRAM0 + (n) * 0x20))

/* ROM data used only by this unit. */
extern const u16 gDuelZoneScrollTargets[][16];  /* field scroll per [player][area]: 20, 80 or 140 */
extern const u16 gDuelScreenLerpWeights[];      /* 8.8 ease-out weights by steps left: 256, 218, 128, 37 */
extern u16 (*const gCoinTossSteps[])(void);     /* CoinToss_Init, CoinToss_Load, CoinToss_Update, NULL */
/* The two tile tables count from OBJ tile 0x200, the first OBJ tile in the bitmap modes. */
extern const u16 gCoinSpinTiles[];              /* spin frames 0-7 (0 heads, 4 tails): 1, 17, ..., 113 */
extern const u16 gCoinGlintTiles[];             /* glint frames: 0-4 over tails, 5-9 over heads */
extern const u8 gCoinPalette[];                 /* OBJ palette 0: coins and glints */
extern const u8 gSparklePalette[];              /* OBJ palette 1: sparkles */
/* Coin spin and glint frames (32x32 4bpp, 0x200 bytes each) and sparkle frames (16x16 4bpp, 0x80 bytes),
 * each table contiguous in ROM. Every frame keeps its own symbol: written as base + n * 0x200, the
 * compiler shares one base register between the copies and the code no longer matches (checked). */
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

/* ------------------------------------------------------------------------------------------------------ */
/* Field scroll and cursor                                                                                */
/* ------------------------------------------------------------------------------------------------------ */

/* Starts a 4-step eased scroll from the current field scroll to `target` (DuelScreen_Update steps it). */
void DuelScreen_StartScroll(u32 target)
{
    gDuelScreen.scrollFrom = gDuelScreen.scroll;
    gDuelScreen.scrollTo = target;
    gDuelScreen.scrollSteps = 4;
}

/* Scrolls to the scroll target of (player, area) unless the scroll already heads there. */
void DuelScreen_ScrollToZone(u32 player, u32 area)
{
    u16 target = gDuelZoneScrollTargets[player][area];

    if (gDuelScreen.scrollTo != (u8)target)
        DuelScreen_StartScroll(target);
}

/* Starts moving the field cursor from its current position to field pixel (x, y) over 4 frames. */
void DuelCursor_MoveTo(u32 x, u32 y)
{
    gDuelScreen.cursorFromX = gDuelScreen.cursorX;
    gDuelScreen.cursorFromY = gDuelScreen.cursorY;
    gDuelScreen.cursorToX = x;
    gDuelScreen.cursorToY = y;
    gDuelScreen.cursorSteps = 4;
}

/*
 * Selects (player, area, index), moves the cursor there and scrolls to it. Callers pass either
 * (area, index) or (DUEL_AREA_MONSTER, zone 0-10); the stored selection is normalised so that zone 5-9 of
 * the monster row becomes (DUEL_AREA_SPELL_TRAP, zone - 5) and zone 10 becomes (DUEL_AREA_FIELD, 0). The
 * cursor position is computed from the arguments as passed.
 */
void DuelCursor_Select(s32 player, s32 area, s32 index)
{
    s32 posIndex;

    gDuelScreen.selPlayer = player;
    gDuelScreen.selArea = area;
    gDuelScreen.selIndex = index;
    /* Matching: this test reads the stored selArea, the one below the argument (the ROM keeps both
     * compares); testing `area` in both places lets the compiler thread the jump. */
    if (gDuelScreen.selArea == DUEL_AREA_MONSTER) {
        if (index == DUEL_AREA_FIELD) {
            gDuelScreen.selArea = index;
            gDuelScreen.selIndex = area;
        }
        if (gDuelScreen.selIndex > 4) {
            gDuelScreen.selArea = DUEL_AREA_SPELL_TRAP;
            gDuelScreen.selIndex = gDuelScreen.selIndex - 5;
        }
    }
    /* gDuelZonePositions has one entry per zone for the two rows and one per area for the rest. */
    if (area == DUEL_AREA_MONSTER || area == DUEL_AREA_SPELL_TRAP)
        posIndex = area + index;
    else
        posIndex = area;
    DuelCursor_MoveTo(GetAreaX(player, area, index), gDuelZonePositions[player][posIndex].y);
    DuelScreen_ScrollToZone(gDuelScreen.selPlayer, gDuelScreen.selArea);
}

/* Re-applies the stored selection (after the field changed under the cursor). */
void DuelCursor_Refresh(void)
{
    DuelCursor_Select(gDuelScreen.selPlayer, gDuelScreen.selArea, gDuelScreen.selIndex);
}

/* ------------------------------------------------------------------------------------------------------ */
/* Sprite animation slot gDuelScreen.sprAnim                                                              */
/* ------------------------------------------------------------------------------------------------------ */

/*
 * Starts sprite animation stream `anim` (an address) in the slot. The stream overwrites OBJ palette 15 and
 * the OBJ tiles from tile 1, so the duel UI graphics count as unloaded until LoadDuelUiGfx runs again.
 */
void DuelSprAnim_Load(u32 anim)
{
    SprAnimLoad((u8 *)anim, &gDuelScreen.sprAnim);
    gDuelScreen.uiGfxLoaded = 0;
}

/* Rewinds the slot's stream to frame 0. */
void DuelSprAnim_Rewind(void)
{
    SprAnimRewind(&gDuelScreen.sprAnim);
}

/* Draws the current frame with each piece at its own position + (x, y); advance != 0 steps to the next. */
void DuelSprAnim_DrawAt(s16 x, s16 y, u16 advance)
{
    SprAnimDrawFrameS16(x, y, &gDuelScreen.sprAnim, advance);
}

/* Draws the current frame with every piece at (x, y); advance != 0 steps to the next. */
void DuelSprAnim_Draw(s16 x, s16 y, u16 advance)
{
    SprAnimDrawFrameAtS16(x, y, &gDuelScreen.sprAnim, advance);
}

/* As DuelSprAnim_Draw with a packed position (y << 16 | x), flipped horizontally if flip & 1. Unused. */
void DuelSprAnim_DrawFlip(u32 yx, u16 advance, u16 flip)
{
    SprAnimDrawFrameAtFlip(yx, &gDuelScreen.sprAnim, advance, flip);
}

/* ------------------------------------------------------------------------------------------------------ */
/* Card animation requests                                                                                */
/* ------------------------------------------------------------------------------------------------------ */

/*
 * The requests write animKind first and animActive last with nothing else on that byte in between; the
 * compiler merges the two into one byte store, as in the ROM.
 */

/* Requests DUEL_ANIM_CHANGE_POSITION or DUEL_ANIM_FLIP; arg is a struct DuelAnimZoneArg packed in a word. */
void DuelAnim_Request(u16 kind, u32 arg)
{
    gDuelScreen.animKind = kind;
    gDuelScreen.animArg = arg;
    gDuelScreen.animStep = 0;
    gDuelScreen.animTimer = 0;
    gDuelScreen.animActive = 1;
}

/* Requests DUEL_ANIM_MOVE_CARD: card cardId moves from `from` to `to`. */
void DuelAnim_MoveCard(u16 cardId, struct DuelLoc *from, struct DuelLoc *to)
{
    gDuelScreen.animKind = DUEL_ANIM_MOVE_CARD;
    gDuelScreen.animArg = cardId;
    gDuelScreen.from = *from;
    gDuelScreen.to = *to;
    gDuelScreen.animStep = 0;
    gDuelScreen.animTimer = 0;
    gDuelScreen.animActive = 1;
}

/* Requests DUEL_ANIM_SWAP_CARDS: two face-down cards swap places between `a` and `b`. */
void DuelAnim_SwapCards(struct DuelLoc *a, struct DuelLoc *b)
{
    gDuelScreen.animKind = DUEL_ANIM_SWAP_CARDS;
    gDuelScreen.from = *a;
    gDuelScreen.to = *b;
    gDuelScreen.animStep = 0;
    gDuelScreen.animTimer = 0;
    gDuelScreen.animActive = 1;
}

/* Requests DUEL_ANIM_ZONE_EFFECT: plays sprite animation stream `anim` (an address) at the position of
 * `loc` + (dx, dy): explosion, negate, tribute whirlwind, smoke puff. */
void DuelAnim_PlayZoneEffect(struct DuelLoc *loc, u32 anim, u32 dx, u32 dy)
{
    gDuelScreen.animKind = DUEL_ANIM_ZONE_EFFECT;
    gDuelScreen.animArg = anim;
    gDuelScreen.from = *loc;
    gDuelScreen.animDx = dx;
    gDuelScreen.animDy = dy;
    gDuelScreen.animStep = 0;
    gDuelScreen.animTimer = 0;
    gDuelScreen.animActive = 1;
}

/* Runs one frame of the requested card animation; returns 1 if one ran. Each handler clears animActive
 * when it ends. */
u16 DuelAnim_Update(void)
{
    if (gDuelScreen.animActive) {
        switch (gDuelScreen.animKind) {
        case DUEL_ANIM_CHANGE_POSITION:
            DuelAnim_UpdateChangePosition();
            return 1;
        case DUEL_ANIM_FLIP:
            DuelAnim_UpdateFlip();
            return 1;
        case DUEL_ANIM_MOVE_CARD:
            DuelAnim_UpdateMoveCard();
            return 1;
        case DUEL_ANIM_SWAP_CARDS:
            DuelAnim_UpdateSwapCards();
            return 1;
        case DUEL_ANIM_ZONE_EFFECT:
            DuelAnim_UpdateZoneEffect();
            return 1;
        }
    }
    return 0;
}

/*
 * Per-frame update of the field view, called from DuelMainStep. While the screen is active: uploads the
 * info-bar text cells and the text box tiles when they are dirty, and steps the field scroll (copied to
 * the BG1 and BG2 VOFS shadows). Then redraws the info bar once a cursor move has ended, steps the cursor,
 * draws it (only while on screen, the duel UI graphics are loaded and showCursor is set) and runs the card
 * animation. Returns 1 while the scroll, the cursor or an animation is still moving.
 */
u32 DuelScreen_Update(void)
{
    u32 moving = 0;

    /* The ROM re-reads `active` before each part (probably inlined helpers). */
    if (gDuelScreen.active) {
        if (gDuelScreen.textTilesDirty) {
            /* BG tiles 0x28E-0x2CD of char block 1 (0x060091C0) */
            CopyDoubleWords((void *)(BG_CHAR_ADDR(1) + 0x28E * 0x20), gDuelScreen.textTiles,
                            sizeof(gDuelScreen.textTiles));
            if (gDuelScreen.textMapReset) {
                TextCellsResetMap();
                gDuelScreen.textMapReset = 0;
            }
            gDuelScreen.textTilesDirty = 0;
        }
        if (gDuelScreen.active) {
            if (gTextBox.tilesDirty) {
                /* BG tiles from 0x2D7 of char block 1 (0x06009AE0) */
                CopyDoubleWords((void *)(BG_CHAR_ADDR(1) + 0x2D7 * 0x20), gTextBox.tiles, sizeof(gTextBox.tiles));
                gTextBox.tilesDirty = 0;
            }
            if (gDuelScreen.active) {
                int steps = gDuelScreen.scrollSteps;

                if (steps > 0) {
                    /* scroll = from + (to - from) * weight / 256 */
                    s32 delta = gDuelScreen.scrollTo - gDuelScreen.scrollFrom;

                    gDuelScreen.scrollSteps = steps - 1;
                    delta *= gDuelScreenLerpWeights[gDuelScreen.scrollSteps];
                    delta /= 256;
                    gDuelScreen.scroll = gDuelScreen.scrollFrom + delta;
                    moving = 1;
                }
                gMain.bgVofs[1] = gDuelScreen.scroll;   /* board */
                gMain.bgVofs[2] = gDuelScreen.scroll;   /* card cells, life points, phases */
            }
        }
    }
    if (gDuelScreen.cursorDone) {
        gDuelScreen.cursorDone = 0;
        DuelScreen_DrawCursorInfo();
    }
    if (gDuelScreen.cursorSteps) {
        /* cursor = from + (to - from) * weight / 256, computed in place */
        gDuelScreen.cursorX = gDuelScreen.cursorToX - gDuelScreen.cursorFromX;
        gDuelScreen.cursorY = gDuelScreen.cursorToY - gDuelScreen.cursorFromY;
        gDuelScreen.cursorSteps--;
        gDuelScreen.cursorX *= gDuelScreenLerpWeights[gDuelScreen.cursorSteps];
        gDuelScreen.cursorY *= gDuelScreenLerpWeights[gDuelScreen.cursorSteps];
        gDuelScreen.cursorX /= 256;
        gDuelScreen.cursorY /= 256;
        gDuelScreen.cursorX += gDuelScreen.cursorFromX;
        gDuelScreen.cursorY += gDuelScreen.cursorFromY;
        moving = 1;
        if (gDuelScreen.cursorSteps == 0)
            gDuelScreen.cursorDone = 1;
    }
    {
        s32 screenY = gDuelScreen.cursorY - gDuelScreen.scroll;

        if (screenY >= 0 && screenY < 160) {
            u16 attr2 = gDuelScreen.cursorAltTile ? 0x30 : 0;   /* OBJ tile */
            u32 angle = gDuelScreen.cursorRotate180 ? 0x40 : 0; /* half of the 128-step turn */

            if (gDuelScreen.uiGfxLoaded && gDuelScreen.showCursor)
                AddAffineSprite((gDuelScreen.cursorX + 8) | (screenY << 16), SPRITE_SHAPE_32x32, attr2,
                                angle | (0x100 << 16));         /* scale 1.0 */
        }
    }
    if (DuelAnim_Update())
        moving = 1;
    return moving;
}

/* ------------------------------------------------------------------------------------------------------ */
/* Coin toss scene                                                                                        */
/* ------------------------------------------------------------------------------------------------------ */

/* Step 0: clears gCoinTossWork, hides BG0-3 and OBJ, zeroes the BG1-3 scroll and sets up the OAM list,
 * the affine records and the sparkle pool. Returns 1. */
u32 CoinToss_Init(void)
{
    MemClear16(&gCoinTossWork, sizeof(gCoinTossWork));
    gMain.vblankFlags = VBLANK_COPY_OAM;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= ~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
    OamListClear((u8 *)&gCoinTossWork.oamList);
    ObjAffineInit(gCoinTossWork.affine);
    SetBldAlpha(8);
    gCoinTossWork.timer = 0;
    gCoinTossWork.unkB42 = 0;
    gCoinTossWork.unkB27 = 0xFF;
    CoinToss_ClearSparkles(&gCoinTossWork.sparkles);
    return 1;
}

/*
 * Step 1: starts the fade-in from black, loads the Mode-4 backdrop, the palettes and the OBJ tiles (OBJ
 * tile 0x200 blank, coin spin frames from 0x201, glint frames from 0x281, a blank 16x16 at 0x321, sparkle
 * frames from 0x325), resets the coins and the unused turn-order fields, and turns the display on in
 * Mode 4. Returns 1.
 */
u32 CoinToss_Load(void)
{
    u8 i;

    FadeStart(FADE_BLACK, -0x180, 0, &gCoinTossWork.fade);
    CpuSet(gEgyptCorridorBitmap, (void *)VRAM, 240 * 160 / 2);     /* 8bpp frame 0 */
    CpuSet(gEgyptCorridorPal, (void *)BG_PLTT, 0x200 / 2);
    CpuSet(gCoinPalette, (void *)OBJ_PLTT, 0x20 / 2);
    CpuFill16(0, OBJ_TILE_ADDR(0x200), 0x20);
    CpuSet(gCoinSpinGfx, OBJ_TILE_ADDR(0x201), 0x200 / 2);
    CpuSet(gCoinSpinGfx_Frame1, OBJ_TILE_ADDR(0x211), 0x200 / 2);
    CpuSet(gCoinSpinGfx_Frame2, OBJ_TILE_ADDR(0x221), 0x200 / 2);
    CpuSet(gCoinSpinGfx_Frame3, OBJ_TILE_ADDR(0x231), 0x200 / 2);
    CpuSet(gCoinSpinGfx_Frame4, OBJ_TILE_ADDR(0x241), 0x200 / 2);
    CpuSet(gCoinSpinGfx_Frame5, OBJ_TILE_ADDR(0x251), 0x200 / 2);
    CpuSet(gCoinSpinGfx_Frame6, OBJ_TILE_ADDR(0x261), 0x200 / 2);
    CpuSet(gCoinSpinGfx_Frame7, OBJ_TILE_ADDR(0x271), 0x200 / 2);
    CpuSet(gCoinGlintGfx, OBJ_TILE_ADDR(0x281), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame1, OBJ_TILE_ADDR(0x291), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame2, OBJ_TILE_ADDR(0x2A1), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame3, OBJ_TILE_ADDR(0x2B1), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame4, OBJ_TILE_ADDR(0x2C1), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame5, OBJ_TILE_ADDR(0x2D1), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame6, OBJ_TILE_ADDR(0x2E1), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame7, OBJ_TILE_ADDR(0x2F1), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame8, OBJ_TILE_ADDR(0x301), 0x200 / 2);
    CpuSet(gCoinGlintGfx_Frame9, OBJ_TILE_ADDR(0x311), 0x200 / 2);
    CpuSet(gSparkleGfx, OBJ_TILE_ADDR(0x325), 0x80 / 2);
    CpuSet(gSparkleGfx_Frame1, OBJ_TILE_ADDR(0x329), 0x80 / 2);
    CpuSet(gSparkleGfx_Frame2, OBJ_TILE_ADDR(0x32D), 0x80 / 2);
    CpuSet(gSparkleGfx_Frame3, OBJ_TILE_ADDR(0x331), 0x80 / 2);
    CpuSet(gSparkleGfx_Frame4, OBJ_TILE_ADDR(0x335), 0x80 / 2);
    CpuSet(gSparkleGfx_Frame5, OBJ_TILE_ADDR(0x339), 0x80 / 2);
    CpuSet(gSparklePalette, (void *)(OBJ_PLTT + 0x20), 0x20 / 2);
    CpuFill16(0, OBJ_TILE_ADDR(0x321), 0x80);
    for (i = 0; i < 3; i++) {
        gCoinTossWork.affine[i].scaleX = 0;
        gCoinTossWork.affine[i].scaleY = 0;
    }
    CoinToss_InitCoins(&gCoinTossWork.toss);
    for (i = 0; i < 4; i++) {
        gCoinTossWork.scrollers[i].speed = 0;
        gCoinTossWork.scrollers[i].pos = 0;
        gCoinTossWork.scrollers[i].stop = 0;
    }
    for (i = 0; i < 2; i++)
        gCoinTossWork.unkB2C[i].amp = 0;
    gCoinTossWork.unkB3C = 0;
    gCoinTossWork.unkB3D = 0;
    gCoinTossWork.unkB24 = 0xFF;
    gCoinTossWork.unkB25 = 0;
    gCoinTossWork.unkB28 = 0;
    gCoinTossWork.unkB36 = 0xFA;
    gCoinTossWork.unkB37 = 0;
    gCoinTossWork.unkB38 = 0;
    gCoinTossWork.unkB3A = 0;
    gCoinTossWork.timer = 0;
    REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_OBJ_1D_MAP | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    return 1;
}

/*
 * Step 2, once per frame: steps the sparkles and the fade, launches the coins (COIN_LAUNCH_ALL: one per
 * frame; COIN_LAUNCH_STAGGERED: one every 16 frames), then spins, flies and draws them, starts and draws
 * the glints and moves the scrollers. Once every coin is resolved it starts the fade-out. Returns 1 when
 * the fade-out has finished or after 0x140 frames.
 *
 * In COIN_LAUNCH_ALL mode (the one the game uses) the fade-out is restarted every frame once the coins are
 * done, which resets its level, so it never finishes and the scene ends on the timer.
 */
u32 CoinToss_Update(void)
{
    CoinToss_UpdateSparkles(&gCoinTossWork.sparkles);
    CoinToss_DrawSparkles(&gCoinTossWork.sparkles);
    FadeTick(&gCoinTossWork.fade);
    if (gCoinTossWork.fade.state == FADE_STATE_FADED_OUT)
        return 1;
    if (gCoinTossWork.fade.state == FADE_STATE_FADED_IN)
        REG_BLDCNT = BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_OBJ;     /* blend the semi-transparent sparkles */
    if (gCoinTossWork.toss.done == 0
        && CoinToss_CountUnfinishedU32(gCoinTossWork.toss.coins, gCoinTossWork.toss.count) == 0) {
        gCoinTossWork.toss.done = 1;
        gCoinTossWork.launchDelay = 100;    /* staggered mode: the fade-out starts 101 frames later */
    }
    switch (gCoinTossWork.launchMode) {
    case COIN_LAUNCH_ALL:
        switch (gCoinTossWork.toss.done) {
        case 0:
            /* After the last coin nextLaunch stays at count: the slot after it is set flying again
             * each frame, and the sound plays only for real launches. */
            gCoinTossWork.toss.coins[gCoinTossWork.toss.nextLaunch].state = COIN_STATE_FLYING;
            gCoinTossWork.toss.nextLaunch++;
            if (gCoinTossWork.toss.nextLaunch == gCoinTossWork.toss.count + 1)
                gCoinTossWork.toss.nextLaunch = gCoinTossWork.toss.count;
            else
                PlaySE(SE_COIN_TOSS);
            break;
        case 1:
            FadeStart(FADE_BLACK, 0x180, 0, &gCoinTossWork.fade);
            break;
        }
        break;
    case COIN_LAUNCH_STAGGERED:
        /* launchDelay counts down to the wrap; then one launch, and 16 frames until the next */
        if (--gCoinTossWork.launchDelay == 0xFF) {
            gCoinTossWork.launchDelay = 15;
            switch (gCoinTossWork.toss.done) {
            case 0:
                gCoinTossWork.toss.coins[gCoinTossWork.toss.nextLaunch].state = COIN_STATE_FLYING;
                gCoinTossWork.toss.nextLaunch++;
                if (gCoinTossWork.toss.nextLaunch == gCoinTossWork.toss.count + 1)
                    gCoinTossWork.toss.nextLaunch = gCoinTossWork.toss.count;
                if (gCoinTossWork.launchedCount++ < gCoinTossWork.toss.count)
                    PlaySE(SE_COIN_TOSS);
                break;
            case 1:
                FadeStart(FADE_BLACK, 0x180, 0, &gCoinTossWork.fade);
                break;
            }
        }
        break;
    }
    CoinToss_AnimateSpin(gCoinTossWork.toss.coins, gCoinTossWork.toss.count);
    CoinToss_UpdateFlight(gCoinTossWork.toss.coins, gCoinTossWork.toss.count, gCoinTossWork.timer,
                          &gCoinTossWork.toss.headsCount);
    CoinToss_DrawCoins(gCoinTossWork.toss.coins, gCoinTossWork.toss.count);
    CoinToss_MarkMatchingCoins(gCoinTossWork.toss.coins, gCoinTossWork.toss.count, gCoinTossWork.toss.calledFace);
    CoinToss_AnimateHighlights(gCoinTossWork.toss.coins, gCoinTossWork.toss.count);
    CoinToss_DrawGlints(gCoinTossWork.toss.coins, gCoinTossWork.toss.count, gCoinTossWork.toss.calledFace);
    OamListFlush(&gCoinTossWork.oamList);
    OamListClear((u8 *)&gCoinTossWork.oamList);
    Scroller_Move(&gCoinTossWork.scrollers[0]);
    Scroller_SnapToStop(&gCoinTossWork.scrollers[0]);
    Scroller_StopAtEnds(&gCoinTossWork.scrollers[1]);
    if (gCoinTossWork.timer++ > 0x140)
        return 1;
    return 0;
}

/* Scene handler of DUEL_SCENE_COIN_TOSS: runs gCoinTossSteps[sceneStep], advancing when a step returns
 * non-zero; returns 1 at the NULL end of the table. */
u32 CoinToss_Run(void)
{
    if (gCoinTossSteps[gDuelScene.sceneStep]) {
        if (gCoinTossSteps[gDuelScene.sceneStep]())
            gDuelScene.sceneStep++;
        return 0;
    }
    return 1;
}

/* Unreferenced variant of CoinToss_Run: before the load step it switches to COIN_LAUNCH_STAGGERED, so the
 * coins are launched one every 16 frames after a 31-frame wait. */
u32 CoinToss_RunStaggered(void)
{
    if (gDuelScene.sceneStep == 1) {
        gCoinTossWork.launchMode = COIN_LAUNCH_STAGGERED;
        gCoinTossWork.launchDelay = 30;
    }
    if (gCoinTossSteps[gDuelScene.sceneStep]) {
        if (gCoinTossSteps[gDuelScene.sceneStep]())
            gDuelScene.sceneStep++;
        return 0;
    }
    return 1;
}

/*
 * Resets the eight coins and decodes gDuelScene.arg: bits 8-14 the coin count, bit 15 the called face
 * (1 = tails), bit 7 selects unk63 (0 if set, else 10). DuelCmd_TossCoin passes call << 15 | 0x180 | face,
 * DuelCmd_TossThreeCoins 0x380 | faces.
 */
void CoinToss_InitCoins(struct CoinToss *toss)
{
    u8 i;

    for (i = 0; i < 8; i++) {
        toss->coins[i].spinTimer = 3;
        toss->coins[i].frame = COIN_FRAME_HEADS;
        toss->coins[i].state = COIN_STATE_READY;
        toss->coins[i].height = 0;
        toss->coins[i].flightTime = 0;
        toss->coins[i].glintState = GLINT_NONE;
        toss->coins[i].glintFrame = 0;
        toss->coins[i].glintTimer = 6;
    }
    toss->nextLaunch = 0;
    toss->headsCount = 0;
    toss->done = 0;
    toss->count = 3;
    toss->calledFace = COIN_FRAME_HEADS;
    toss->unk63 = 2;
    /* Each read of gDuelScene.arg stays a separate expression: a u16 local changes the shifts. */
    toss->count = (gDuelScene.arg & 0x7F00) >> 8;
    toss->calledFace = (gDuelScene.arg >> 13) & COIN_FRAME_TAILS;  /* bit 15 -> 4 */
    if (gDuelScene.arg & 0x80)
        toss->unk63 = 0;
    else
        toss->unk63 = 10;
}

/* Advances the spin frame (0-7) of each flying coin every 4 frames. */
void CoinToss_AnimateSpin(struct Coin *coins, u8 count)
{
    u8 i;

    for (i = 0; i < count; i++) {
        if (coins[i].state == COIN_STATE_FLYING) {
            if (coins[i].spinTimer == 0) {
                coins[i].spinTimer = 3;
                if (++coins[i].frame == 8)
                    coins[i].frame = 0;
            } else {
                coins[i].spinTimer--;
            }
        }
    }
}

/*
 * Unreferenced: as CoinToss_UpdateFlight, but every coin lands on the face given by mask bit 0 (1 = heads,
 * the opposite sense of the gDuelScene.arg bits) and leaves no sparkle trail.
 */
void CoinToss_UpdateFlightUniform(struct Coin *coins, u8 count, u8 mask, u8 *headsCount)
{
    u8 i;

    for (i = 0; i < count; i++) {
        u8 state = coins[i].state;

        if (state == COIN_STATE_FLYING) {
            coins[i].flightTime += 40;
            coins[i].height = MulFix8Int(0x3000, coins[i].flightTime)
                - MulFix8Int(0x500, MulFix8Int(coins[i].flightTime, coins[i].flightTime));
            if (coins[i].height < 0) {
                u8 face;

                coins[i].height = 0;
                coins[i].flightTime = 0;
                coins[i].state++;
                face = (state & mask) ? COIN_FRAME_HEADS : COIN_FRAME_TAILS;
                coins[i].frame = face;
                if (face == COIN_FRAME_HEADS)
                    (*headsCount)++;
            }
        }
    }
}

/*
 * Moves each flying coin along its arc: t += 40 per frame, height = 48 t - 5 t^2 (8.8 fixed point). When
 * the height drops below 0 the coin lands (COIN_STATE_LANDED) on the face given by bit i of gDuelScene.arg
 * (1 = tails) and heads are counted in *headsCount. While in flight it spawns a sparkle at the coin's
 * position whenever t is a multiple of 16 (every other frame), leaving a trail.
 */
void CoinToss_UpdateFlight(struct Coin *coins, u8 count, u32 unused, u8 *headsCount)
{
    u8 i;

    for (i = 0; i < count; i++) {
        u8 state = coins[i].state;

        if (state == COIN_STATE_FLYING) {
            coins[i].flightTime += 40;
            coins[i].height = MulFix8Int(0x3000, coins[i].flightTime)
                - MulFix8Int(0x500, MulFix8Int(coins[i].flightTime, coins[i].flightTime));
            if (coins[i].height < 0) {
                coins[i].height = 0;
                coins[i].flightTime = 0;
                coins[i].state++;
                if ((gDuelScene.arg >> i) & state)     /* state is 1 here: bit i */
                    coins[i].frame = COIN_FRAME_TAILS;
                else
                    coins[i].frame = COIN_FRAME_HEADS;
                if (coins[i].frame == COIN_FRAME_HEADS)
                    (*headsCount)++;
            } else if ((coins[i].flightTime & 0xF) == 0) {
                CoinToss_SpawnSparkle((i + 1) * 240 / (count + 1) - 16, 120 - (coins[i].height >> 8),
                                      &gCoinTossSparkles);
            }
        }
    }
}

/* Draws each coin as a 32x32 sprite: the coins are spread evenly across the 240-pixel screen, the top of
 * the sprite at y = 120 - height. */
void CoinToss_DrawCoins(struct Coin *coins, u8 count)
{
    u8 i;

    for (i = 0; i < count; i++) {
        int tile = gCoinSpinTiles[coins[i].frame] + 0x200;

        /* layer 0, 32x32 px, 4bpp, palette 0 */
        OamListAddSpriteWide(0, tile, (i + 1) * 240 / (count + 1) - 16, 120 - (coins[i].height >> 8),
                             32, 32, 4, 0, 0x200, 0, 0, 0, &gCoinTossWork.oamList);
    }
}

/* Draws the glint over each coin whose glint is playing; the glint frames differ for a call of heads
 * (frames 5-9) and of tails (frames 0-4). */
void CoinToss_DrawGlints(struct Coin *coins, u8 count, u8 calledFace)
{
    u8 i;

    for (i = 0; i < count; i++) {
        if (coins[i].glintState == GLINT_PLAYING) {
            int tile = gCoinGlintTiles[coins[i].glintFrame + (calledFace != COIN_FRAME_TAILS ? 5 : 0)] + 0x200;

            OamListAddSpriteWide(0, tile, (i + 1) * 240 / (count + 1) - 16, 120 - (coins[i].height >> 8),
                                 32, 32, 4, 0, 0x200, 0, 0, 0, &gCoinTossWork.oamList);
        }
    }
}
