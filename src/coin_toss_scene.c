/*
 * Coin toss helpers and the die-roll scene.
 *
 * The first seven functions finish the coin toss of duel_field_view.c (DUEL_SCENE_COIN_TOSS: Time Wizard,
 * Goddess of Whim, Barrel Dragon). Once every coin has landed, the coins showing the called face glint; these
 * functions start and animate that glint, count the coins still to resolve, and run the pool of 32 sparkles
 * that trails behind the flying coins.
 *
 * The rest is the die roll on gDiceScreen, played for Skull Dice (DUEL_SCENE_DICE_SKULL), Graceful Dice
 * (DUEL_SCENE_DICE_GRACEFUL) and a plain die (DUEL_SCENE_DICE_PLAIN, no character). The card's character
 * swings in holding the die and throws it; the die bounces once, then rolls for ten frames and stops on the
 * face the effect has already rolled (gDuelScene.arg). The scene fades out and writes the face to
 * gDuelScene.result. The Skull Dice scene handler is here too; the other two are in dice_scene.c.
 *
 * Die-roll steps (gDiceScreen.step, enum DiceStep) run by DiceScreen_Update:
 *   DICE_STEP_ENTER   DiceScreen_CharacterEnter   the character comes down with the die
 *   DICE_STEP_HOLD    DiceScreen_HoldDie          a 15-frame wait (the plain die starts here)
 *   DICE_STEP_THROW   DiceScreen_ThrowDie         the throw: a high arc with the tumble frames
 *   DICE_STEP_BOUNCE  DiceScreen_ThrowDie         a lower hop rolling along a random axis
 *   DICE_STEP_ROLL    DiceScreen_RollToResult     the final roll to the result face, the character leaves
 */
#include "global.h"
#include "gba.h"            /* REG_*, CpuSet, VRAM, BG_PLTT, OBJ_PLTT, OBJ_VRAM0, A_BUTTON */
#include "main.h"           /* gMain */
#include "util.h"           /* MemClear16, Random, gSineTable, struct Ease, Ease_Init/Start/Tick */
#include "palette.h"        /* struct Fade, FadeStart, FadeTick, FADE_STATE_FADED_OUT */
#include "bg.h"             /* CopyTileSheetTo2D, TILE_COLORS_16 */
#include "sprite.h"         /* struct OamList / OamListEntry / AnimSeq / AnimState, OamList*, AnimStateTick */
#include "duel_scenes.h"    /* gDuelScene, gCoinTossWork, gDiceScreen, struct Coin / SparklePool, enums */
#include "turn_order.h"     /* gEgyptCorridorBitmap, gEgyptCorridorPal */

/* ---- Names the legacy headers lack (until H0 installs the new gba.h, main.h and sound.h) ---- */

/* Values as in the new gba.h and main.h; skipped once they are installed (then delete the block). */
#ifndef DISPCNT_MODE_4
#define DISPCNT_MODE_4          0x0004
#define DISPCNT_BG_ALL_ON       0x0F00
#define DISPCNT_OBJ_ON          0x1000
#define BLDCNT_EFFECT_BLEND     0x0040
#define BLDCNT_TGT2_ALL         0x3F00
#define BLDALPHA_BLEND(eva, evb) (((evb) << 8) | (eva))
#define OAM_ATTR0_BLEND         0x0400
#define VBLANK_COPY_OAM         0x1
#endif

/* The legacy sound.h does not declare PlaySE (the new one does, with this prototype). */
void PlaySE(u32 seId);

/* ---- Local views kept on purpose (matching choices, see build/readability/HEADERS.md) ---- */

/* OamListAddSprite as CoinToss_DrawSparkles calls it: the tile number is passed without narrowing (the
 * definition takes u16), and the blend bit is ORed into attr0 through the entry's first word (u32). */
extern u32 *OamListAddSpriteWideTile(u8 layer, u32 tile, u16 x, int y, u8 width, u8 height, u8 bpp, u8 palette,
                                     u32 unused, u16 attr0Flags, u8 attr1Bits, u8 priority, struct OamList *list)
    asm("OamListAddSprite");

/* MulFix8 with int parameters and result: the ROM neither narrows the arguments nor sign-extends the result
 * (util.h: s16 MulFix8(s16, s16)). */
extern s32 MulFix8Int(s32 a, s32 b) asm("MulFix8");

/* Ease values: util.h's struct Ease has a u16 cur, but this unit does signed arithmetic on it (ldrsh), so
 * those reads are written (s16)gDiceScreen.xxxEase.cur. Reads that only take the low byte are left alone. */

/* ---- ROM data used only here ---- */

/* 0x08081FA4: OBJ tile (before + 0x200) of each sparkle frame, 0-terminated (22 frames). */
extern const u16 gCoinSparkleTiles[];
/* 0x08082308: for each face 1-6, two {axis, quarter} places in gDieAxisFaces where the face shows. */
extern const u8 gDieFacePaths[6][2][2];
/* 0x080822FC: the face (1-6) at each quarter of each roll axis: one ring of four faces of a die per axis. */
extern const u8 gDieAxisFaces[][4];
/* 0x081999EC: per roll axis (0-2), a 20-frame strip of the die rolling (4 faces x 5 frames). */
extern const struct AnimSeq *const gDieRollFrames[];
/* 0x08081FD4: 8 frames of the die tumbling during the throw. */
extern const struct AnimSeq gDieTumbleFrames[];
/* 0x081999F8 / 0x08199A04: NULL-terminated animation lists of the Skull Dice and Graceful Dice characters:
 * [0] hovering and throwing, [1] the result pose. Not const: AnimBlockInit takes a struct AnimSeq **. */
extern struct AnimSeq *gSkullDiceCharAnims[];
extern struct AnimSeq *gGracefulDiceCharAnims[];
/* 0x086B2168 / 0x086B4168: the two 128x128 halves of the OBJ sheet (dice, the characters' pieces; not const:
 * CopyTileSheetTo2D takes a u8 *), and 0x086B6168 its 16 OBJ palettes (palette 2 for Skull Dice's die). */
extern u8 gDiceSceneObjTilesLeft[];
extern u8 gDiceSceneObjTilesRight[];
extern const u8 gDiceSceneObjPal[];
/* 0x08199A10 / 0x08199A28: the steps of DiceScreen_Update (enum DiceStep), with and without a character. */
extern u16 (*const gDiceScreenSteps[])(void);
extern u16 (*const gPlainDieScreenSteps[])(void);
/* 0x08199A40: the outer steps of the Skull Dice scene (PrepareRoll, Init, SetupSkullDice, LoadGraphics,
 * Update, NULL). */
extern u16 (*const gSkullDiceSceneSteps[])(void);

/* The die and the character are drawn relative to these screen positions. */
#define DIE_BASE_X      0x68    /* die and character x */
#define DIE_FLOOR_Y     0x64    /* die y when it touches the floor */
#define OBJ_TILE_BITMAP 0x200   /* first OBJ tile usable in the bitmap modes (OBJ VRAM + 0x4000) */

/* ---- Coin toss: glints and sparkles ---- */

/* Advances the glint of each coin whose glint is playing: one frame every 7 frames; after frame 4 it wraps
 * to 0 and the coin is resolved (COIN_HIGHLIGHT_DONE). */
void CoinToss_AnimateHighlights(struct Coin *coins, u8 count)
{
    u8 i;

    for (i = 0; i < count; i++) {
        if (coins[i].glintState == COIN_HIGHLIGHT_ANIMATING) {
            if (coins[i].glintTimer == 0) {
                coins[i].glintTimer = 6;
                if (++coins[i].glintFrame == 5) {
                    coins[i].glintFrame = 0;
                    coins[i].glintState++;
                }
            } else {
                coins[i].glintTimer--;
            }
        }
    }
}

/* Once no coin is in the air any more (every state is COIN_STATE_LANDED), the coins showing calledFace start
 * their glint and become COIN_STATE_MATCHED; the others are resolved at once. A matched coin no longer counts
 * as landed, so this fires only once. */
void CoinToss_MarkMatchingCoins(struct Coin *coins, u8 count, u8 calledFace)
{
    u8 notLanded = 0;
    u8 i;

    for (i = 0; i < count; i++) {
        if (coins[i].state != COIN_STATE_LANDED)
            notLanded++;
    }
    if (notLanded == 0) {
        for (i = 0; i < count; i++) {
            if (coins[i].frame == calledFace) {
                coins[i].glintState = COIN_HIGHLIGHT_ANIMATING;
                coins[i].state = COIN_STATE_MATCHED;
            } else {
                coins[i].glintState = COIN_HIGHLIGHT_DONE;
            }
        }
    }
}

/* Returns the number of coins not resolved yet (glintState != COIN_HIGHLIGHT_DONE): still flying, landed but
 * not marked, or glinting. */
u8 CoinToss_CountUnfinished(struct Coin *coins, u8 count)
{
    u8 unfinished = 0;
    u8 i;

    for (i = 0; i < count; i++) {
        if (coins[i].glintState != COIN_HIGHLIGHT_DONE)
            unfinished++;
    }
    return unfinished;
}

/* Starts a sparkle at (x + Random() % 16, y) in the next slot of the pool (a ring of 32); returns 1. */
u32 CoinToss_SpawnSparkle(u8 x, u8 y, struct SparklePool *pool)
{
    u8 slot = pool->next++ % 32;
    s32 rnd = Random();
    struct Sparkle *sparkle = &pool->sparkles[slot];

    sparkle->x = x + rnd % 16;
    sparkle->y = y;
    sparkle->active = 1;
    sparkle->timer = 0;
    sparkle->frame = 0;
    return 1;
}

/* Advances every active sparkle; it ends at the 0 entry of gCoinSparkleTiles. The 2-bit timer goes from 0
 * to 3 on every decrement, so the frame advances every frame. Declared u32, returns nothing. */
u32 CoinToss_UpdateSparkles(struct SparklePool *pool)
{
    u8 i;

    for (i = 0; i < 32; i++) {
        struct Sparkle *sparkle = &pool->sparkles[i];
        if (sparkle->active) {
            if (--sparkle->timer == 3) {
                sparkle->timer = 0;
                if (gCoinSparkleTiles[++sparkle->frame] == 0)
                    sparkle->active = 0;
            }
        }
    }
}

/* Adds every active sparkle to the coin toss OAM list (layer 1) as a semi-transparent 16x16 4bpp sprite with
 * palette 1. Declared u32, returns nothing. */
u32 CoinToss_DrawSparkles(struct SparklePool *pool)
{
    u8 i;

    for (i = 0; i < 32; i++) {
        struct Sparkle *sparkle = &pool->sparkles[i];
        if (sparkle->active) {
            u32 *entry = OamListAddSpriteWideTile(1, gCoinSparkleTiles[sparkle->frame] + OBJ_TILE_BITMAP,
                                                  sparkle->x, sparkle->y, 16, 16, 4, 1, 0, 0, 0, 0,
                                                  &gCoinTossWork.oamList);
            *entry |= OAM_ATTR0_BLEND;
        }
    }
}

/* Deactivates all 32 sparkles and resets the next slot. */
void CoinToss_ClearSparkles(struct SparklePool *pool)
{
    u8 i;

    for (i = 0; i < 32; i++)
        pool->sparkles[i].active = 0;
    pool->next = 0;
}

/* ---- Die roll ---- */

/* Allocates an OAM entry on `layer` of `list` (a struct OamList) and fills it from the 4-halfword sprite
 * piece (struct OamTemplate) placed at (x, y): attr0 Y and attr1 X are offset with wrap-around; the tile is
 * tileBase + the piece's sheet row (attr2 bits 4-7) * 32 + its column (bits 0-3), + 16 for a piece of the
 * sheet's right half (bit 8); `palette` is used only if the piece has none. Returns the entry.
 * K&R definition: the callers in this unit pass their u8/u16 arguments without narrowing them. */
u16 *DiceScreen_AddOamPiece(piece, layer, x, y, palette, tileBase, list)
    u16 *piece;
    u8 layer;
    u16 x;
    u16 y;
    u16 palette;
    u16 tileBase;
    void *list;
{
    struct OamListEntry *entry = OamListAlloc(layer, list);
    /* Matching: the returned pointer is kept in its own variable (the ROM's mov ip, r7 copy). */
    u16 *ret = (u16 *)entry;

    entry->attr0 = (piece[0] & 0xFF00) | ((y + (piece[0] & 0xFF)) & 0xFF);
    entry->attr1 = (piece[1] & 0xFE00) | ((x + (piece[1] & 0x1FF)) & 0x1FF);
    entry->attr2 = (piece[2] & 0xFC0F) | (tileBase + ((piece[2] & 0xF0) << 1));
    if (piece[2] & 0x100)
        entry->attr2 += 0x10;
    if ((piece[2] & 0xF000) == 0)
        entry->attr2 |= palette << 12;
    return ret;
}

/* Draws every piece of the current frame of `anim` at (x, y) on OAM layer 1, OR-ing attr0Flags into attr0
 * (OAM_ATTR0_BLEND = semi-transparent). Nothing for the plain die, which has no character.
 * K&R definition, as DiceScreen_AddOamPiece. */
void DiceScreen_DrawCharacter(anim, x, y, attr0Flags)
    struct AnimState *anim;
    u16 x;
    u16 y;
    u16 attr0Flags;
{
    u8 i;

    if (gDiceScreen.variant != DICE_VARIANT_PLAIN) {
        for (i = 0; i < anim->pieceCount; i++) {
            u16 *attrs = DiceScreen_AddOamPiece((u16 *)&anim->pieces[i], 1, x, y, 0, OBJ_TILE_BITMAP,
                                                &gDiceScreen.oamList);
            *attrs |= attr0Flags;
        }
    }
}

/* AnimStateTick(anim) unless the scene is the plain die. */
void DiceScreen_TickCharacter(void *anim)
{
    if (gDiceScreen.variant != DICE_VARIANT_PLAIN)
        AnimStateTick(anim);
}

/* Outer step 1: VBlank copies only OAM, BG1-3 scroll reset, BGs and sprites off; clears the OAM list, the
 * affine records and the step counters, and sets up the eases (enterEase runs 0 -> 0x100 by 2) and the
 * character's fade and exit spiral. Returns 1. */
u32 DiceScreen_Init(void)
{
    gMain.vblankFlags = VBLANK_COPY_OAM;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= 0xE0FF;      /* ~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON) */
    OamListClear((u8 *)&gDiceScreen.oamList);
    ObjAffineInit(gDiceScreen.affine);
    gDiceScreen.step = DICE_STEP_ENTER;
    gDiceScreen.spinTimer = 0;
    gDiceScreen.rollAxis = 0;
    gDiceScreen.resultTimer = 0xFF;
    gDiceScreen.throwStarted = 0;
    Ease_Init(0, 0, 0, &gDiceScreen.dieEase);
    Ease_Start(0, 0x100, 2, &gDiceScreen.enterEase);
    Ease_Init(0, 0, 0, &gDiceScreen.holdEase);
    gDiceScreen.charFade = 0x1600;
    gDiceScreen.orbitAngle = 0x4000;
    gDiceScreen.orbitSpeed = 0x2B0;
    return 1;
}

/* Outer step 3: the character animations for the variant (the plain die loads Graceful Dice's but never draws
 * them, and skips the entry: enterEase at its end, holdEase started, step DICE_STEP_HOLD); a fade in from
 * black; the corridor bitmap and palette, both halves of the OBJ sheet and the OBJ palettes; Mode 4 with all
 * layers on. Returns 1. */
u32 DiceScreen_LoadGraphics(void)
{
    switch (gDiceScreen.variant) {
    case DICE_VARIANT_SKULL:
        AnimBlockInit(gSkullDiceCharAnims, (u8 *)gDiceScreen.anims);
        break;
    case DICE_VARIANT_GRACEFUL:
        AnimBlockInit(gGracefulDiceCharAnims, (u8 *)gDiceScreen.anims);
        break;
    case DICE_VARIANT_PLAIN:
        AnimBlockInit(gGracefulDiceCharAnims, (u8 *)gDiceScreen.anims);
        gDiceScreen.enterEase.cur = 0x100;
        gDiceScreen.enterEase.end = 0x100;
        gDiceScreen.enterEase.state = TICK_IDLE;
        Ease_Start(0, 15, 1, &gDiceScreen.holdEase);
        gDiceScreen.step = DICE_STEP_HOLD;
        break;
    }
    FadeStart(FADE_BLACK, -0x180, 0, &gDiceScreen.fade);
    /* The Egyptian corridor backdrop (also behind the coin toss); CpuSet counts are in halfwords. */
    CpuSet(gEgyptCorridorBitmap, (void *)VRAM, 240 * 160 / 2);
    CpuSet(gEgyptCorridorPal, (void *)BG_PLTT, 256);
    /* The two halves side by side from OBJ tile 0x200 (2D mapping: the right half starts 16 tiles further). */
    CopyTileSheetTo2D(gDiceSceneObjTilesLeft, (u8 *)(OBJ_VRAM0 + OBJ_TILE_BITMAP * 32), TILE_COLORS_16);
    CopyTileSheetTo2D(gDiceSceneObjTilesRight, (u8 *)(OBJ_VRAM0 + OBJ_TILE_BITMAP * 32 + 16 * 32), TILE_COLORS_16);
    CpuSet(gDiceSceneObjPal, (void *)OBJ_PLTT, 256);
    REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    return 1;
}

/*
 * DICE_STEP_THROW and DICE_STEP_BOUNCE: the die flies while the character hovers with a sine bob.
 * The throw (first call) starts dieEase 0 -> 10000 by 4 and draws the 8 tumble frames on a high arc; when it
 * lands, the bounce starts along a random axis (0 or 1) on a lower arc with that axis's roll frames. When the
 * bounce lands, the final roll starts: dieEase rollStart -> rollStart + 10 by 1 on finalAxis. The end value
 * 10000 is never reached: the die lands first (dieY > 0, after about 46 and 38 frames). Each landing plays
 * SE 0x1F and enables alpha blending (BLDALPHA 16/16) for the semi-transparent character of DICE_STEP_ROLL.
 * Returns 1 on landing.
 */
u32 DiceScreen_ThrowDie(void)
{
    u16 swayX, charY;
    s16 dieY;
    s32 bob;
    struct AnimState *anim;
    u8 i;

    /* The character's position from enterEase, as in DiceScreen_CharacterEnter (a sideways sway of +-16 px,
     * a height of (cur - 0xBE) / 2 plus a +-8 px wave); enterEase has ended at 0x100 here. */
    swayX = MulFix8Int(0x100, gSineTable[gDiceScreen.enterEase.cur & 0xFF]) >> 4;
    charY = ((s16)gDiceScreen.enterEase.cur - 0xBE) / 2
        + (MulFix8Int(0x80, gSineTable[(((s16)gDiceScreen.enterEase.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gDiceScreen.throwStarted == 0) {
        Ease_Start(0, 10000, 4, &gDiceScreen.dieEase);
        gDiceScreen.step = DICE_STEP_THROW;
        gDiceScreen.throwStarted = 1;
    }
    if (gDiceScreen.dieEase.state != TICK_IDLE)
        gDiceScreen.spinTimer++;
    /* dieY: the die's y relative to the floor (negative = in the air): parabolas in dieEase.cur. */
    if (gDiceScreen.step == DICE_STEP_THROW) {
        dieY = -60 - (((s16)gDiceScreen.dieEase.cur * (100 - (s16)gDiceScreen.dieEase.cur)) >> 8);
        for (i = 0; i < gDieTumbleFrames[(gDiceScreen.spinTimer >> 2) & 7].pieceCount; i++)
            DiceScreen_AddOamPiece((u16 *)&gDieTumbleFrames[(gDiceScreen.spinTimer >> 2) & 7].pieces[i],
                                   0, DIE_BASE_X, dieY + DIE_FLOOR_Y, gDiceScreen.diePalette, OBJ_TILE_BITMAP,
                                   &gDiceScreen.oamList);
    } else {
        dieY = -(((s16)gDiceScreen.dieEase.cur * (150 - (s16)gDiceScreen.dieEase.cur)) >> 8);
        for (i = 0; i < gDieRollFrames[gDiceScreen.rollAxis][(gDiceScreen.spinTimer >> 2) & 7].pieceCount; i++)
            DiceScreen_AddOamPiece(
                (u16 *)&gDieRollFrames[gDiceScreen.rollAxis][(gDiceScreen.spinTimer >> 2) & 7].pieces[i],
                0, DIE_BASE_X, dieY + DIE_FLOOR_Y, gDiceScreen.diePalette, OBJ_TILE_BITMAP, &gDiceScreen.oamList);
    }
    anim = &gDiceScreen.anims[0];
    anim->active = ANIM_PLAYING;    /* loop the hover animation */
    DiceScreen_TickCharacter(anim);
    /* Matching: the bob needs its own variable (written inline in the call, the code differs). */
    bob = (MulFix8Int(0x100, gSineTable[(gDiceScreen.spinTimer * 2 + 0x20) & 0xFF]) >> 4) + 8;
    DiceScreen_DrawCharacter(anim, swayX + DIE_BASE_X, charY - bob, 0);
    Ease_Tick(&gDiceScreen.dieEase);
    if (dieY > 0) {
        /* Landed: start the bounce, or after the bounce the final roll. */
        gDiceScreen.dieEase.cur = 0;
        gDiceScreen.dieEase.state = TICK_IDLE;
        if (gDiceScreen.step == DICE_STEP_THROW) {
            Ease_Start(0, 10000, 4, &gDiceScreen.dieEase);
            gDiceScreen.rollAxis = Random() % 2;
        } else {
            gDiceScreen.rollStart = gDiceScreen.finalRollStart;
            gDiceScreen.rollAxis = gDiceScreen.finalAxis;
            Ease_Start(gDiceScreen.rollStart, gDiceScreen.rollStart + 10, 1, &gDiceScreen.dieEase);
        }
        PlaySE(0x1F);
        REG_BLDALPHA = BLDALPHA_BLEND(16, 16);
        REG_BLDY = 8;
        REG_BLDCNT = BLDCNT_TGT2_ALL | BLDCNT_EFFECT_BLEND;
        return 1;
    }
    return 0;
}

/*
 * DICE_STEP_ROLL: the die rolls 10 frames of the final axis's strip, 2 px lower each frame, and stops on the
 * result face; SE 0x1F every 3 frames. The character is drawn semi-transparent. Once the roll is done
 * (dieEase TICK_DONE) it shows its result pose (anims[1]) and its blend weight runs down (BLDALPHA EVA =
 * charFade >> 8); the Graceful Dice character also spirals away. On A (at any time in this step), or 256
 * frames after the roll (resultTimer), it starts the fade-out and returns 1.
 */
u32 DiceScreen_RollToResult(void)
{
    u16 swayX, charY;
    u16 orbitX;
    struct AnimState *anim;     /* Matching: declared between orbitX and orbitY (picks the stack slots) */
    u16 orbitY;
    s32 baseX, charX;
    s32 bob;
    u8 i;

    swayX = MulFix8Int(0x100, gSineTable[gDiceScreen.enterEase.cur & 0xFF]) >> 4;
    /* Dead store, recomputed below; the ROM keeps its MulFix8 call. */
    charY = ((s16)gDiceScreen.enterEase.cur - 0xBE) / 2
        + (MulFix8Int(0x80, gSineTable[(((s16)gDiceScreen.enterEase.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gMain.newKeys & A_BUTTON) {
        FadeStart(FADE_BLACK, 0x180, 0, &gDiceScreen.fade);
        return 1;
    }
    if (gDiceScreen.dieEase.state == TICK_IDLE) {
        FadeStart(FADE_BLACK, 0x180, 0, &gDiceScreen.fade);
        return 1;
    }
    if (gDiceScreen.dieEase.state == TICK_DONE) {
        /* The result is showing: when the countdown wraps, the next call leaves (state TICK_IDLE). */
        if (--gDiceScreen.resultTimer == 0xFF)
            gDiceScreen.dieEase.state = TICK_IDLE;
    }
    for (i = 0; i < gDieRollFrames[gDiceScreen.rollAxis][(s16)gDiceScreen.dieEase.cur % 20].pieceCount; i++)
        DiceScreen_AddOamPiece(
            (u16 *)&gDieRollFrames[gDiceScreen.rollAxis][(s16)gDiceScreen.dieEase.cur % 20].pieces[i],
            0, DIE_BASE_X, ((s16)gDiceScreen.dieEase.cur - gDiceScreen.rollStart) * 2 + DIE_FLOOR_Y,
            gDiceScreen.diePalette, OBJ_TILE_BITMAP, &gDiceScreen.oamList);
    orbitX = 0;
    orbitY = 0;
    if (gDiceScreen.dieEase.state == TICK_DONE) {
        anim = &gDiceScreen.anims[1];
        /* charFade starts at 0x1600 (8.8), so BLDALPHA follows it only once it is down to 16. */
        if (gDiceScreen.charFade != 0) {
            if (gDiceScreen.charFade <= 0x1000)
                REG_BLDALPHA = gDiceScreen.charFade >> 8;
            gDiceScreen.charFade -= 0x18;
        }
        if (gDiceScreen.variant == DICE_VARIANT_GRACEFUL) {
            /* Exit spiral: an ellipse of 32 x 20 px radii around (0, -20), the angle step growing by 8 each
             * frame. */
            gDiceScreen.orbitAngle += gDiceScreen.orbitSpeed;
            gDiceScreen.orbitSpeed += 8;
            orbitX = MulFix8Int(0x2000, gSineTable[(gDiceScreen.orbitAngle >> 8) + 0x40]) >> 8;
            orbitY = (MulFix8Int(0x1400, gSineTable[gDiceScreen.orbitAngle >> 8]) >> 8) - 0x14;
        }
    } else if (gDiceScreen.dieEase.state != TICK_IDLE) {
        anim = &gDiceScreen.anims[0];
        if ((s16)gDiceScreen.dieEase.cur % 3 == 0)
            PlaySE(0x1F);
    }
    /* On the frame the countdown ends the state is already TICK_IDLE: neither branch sets anim, and this
     * store goes through an uninitialised pointer (a bug of the original code; nothing is drawn). */
    anim->active = ANIM_PLAYING;
    charY = ((s16)gDiceScreen.enterEase.cur - 0xBE) / 2
        + (MulFix8Int(0x80, gSineTable[(((s16)gDiceScreen.enterEase.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gDiceScreen.dieEase.state != TICK_IDLE) {
        DiceScreen_TickCharacter(anim);
        /* Matching: the x sum in two steps gives the ROM's operand order. */
        baseX = orbitX + DIE_BASE_X;
        charX = swayX + baseX;
        bob = (MulFix8Int(0x100, gSineTable[(gDiceScreen.spinTimer * 2 + 0x20) & 0xFF]) >> 4) + 8;
        DiceScreen_DrawCharacter(anim, charX, charY - bob + orbitY, OAM_ATTR0_BLEND);
    }
    Ease_Tick(&gDiceScreen.dieEase);
    return 0;
}

/* DICE_STEP_ENTER: the character comes down from above the screen with a sideways sway (enterEase 0 ->
 * 0x100), holding the die (the current roll frame) under it. When enterEase is done, starts holdEase (15
 * frames) and returns 1. */
u32 DiceScreen_CharacterEnter(void)
{
    u16 swayX, charY;
    struct AnimState *anim;
    u8 i;

    swayX = MulFix8Int(0x100, gSineTable[gDiceScreen.enterEase.cur & 0xFF]) >> 4;
    charY = ((s16)gDiceScreen.enterEase.cur - 0xBE) / 2
        + (MulFix8Int(0x80, gSineTable[(((s16)gDiceScreen.enterEase.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    anim = &gDiceScreen.anims[0];
    anim->active = ANIM_PLAYING;
    DiceScreen_TickCharacter(anim);
    DiceScreen_DrawCharacter(anim, swayX + DIE_BASE_X, charY - 8, 0);
    /* The die, 0x14 px below the character. */
    for (i = 0; i < gDieRollFrames[gDiceScreen.rollAxis][(s16)gDiceScreen.dieEase.cur % 20].pieceCount; i++)
        DiceScreen_AddOamPiece(
            (u16 *)&gDieRollFrames[gDiceScreen.rollAxis][(s16)gDiceScreen.dieEase.cur % 20].pieces[i],
            0, swayX + DIE_BASE_X, charY + 0x14, gDiceScreen.diePalette, OBJ_TILE_BITMAP, &gDiceScreen.oamList);
    Ease_Tick(&gDiceScreen.enterEase);
    if (gDiceScreen.enterEase.state == TICK_DONE) {
        gDiceScreen.enterEase.state = TICK_IDLE;
        Ease_Start(0, 15, 1, &gDiceScreen.holdEase);
        return 1;
    }
    return 0;
}

/* DICE_STEP_HOLD: draws the character (not for the plain die) and the die it holds at the end position of the
 * entry, and ticks holdEase. When it is done, plays the throw sound (SE 0x1F) and returns 1. */
u32 DiceScreen_HoldDie(void)
{
    u16 swayX, charY;
    struct AnimState *anim;
    u8 i;

    swayX = MulFix8Int(0x100, gSineTable[gDiceScreen.enterEase.cur & 0xFF]) >> 4;
    charY = ((s16)gDiceScreen.enterEase.cur - 0xBE) / 2
        + (MulFix8Int(0x80, gSineTable[(((s16)gDiceScreen.enterEase.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gDiceScreen.variant != DICE_VARIANT_PLAIN) {
        anim = &gDiceScreen.anims[0];
        anim->active = ANIM_PLAYING;
        DiceScreen_TickCharacter(anim);
        for (i = 0; i < anim->pieceCount; i++)
            DiceScreen_AddOamPiece((u16 *)&anim->pieces[i], 1, swayX + DIE_BASE_X, charY - 8, 0, OBJ_TILE_BITMAP,
                                   &gDiceScreen.oamList);
    }
    for (i = 0; i < gDieRollFrames[gDiceScreen.rollAxis][(s16)gDiceScreen.dieEase.cur % 20].pieceCount; i++)
        DiceScreen_AddOamPiece(
            (u16 *)&gDieRollFrames[gDiceScreen.rollAxis][(s16)gDiceScreen.dieEase.cur % 20].pieces[i],
            0, swayX + DIE_BASE_X, charY + 0x14, gDiceScreen.diePalette, OBJ_TILE_BITMAP, &gDiceScreen.oamList);
    Ease_Tick(&gDiceScreen.holdEase);
    if (gDiceScreen.holdEase.state == TICK_DONE) {
        gDiceScreen.holdEase.state = TICK_IDLE;
        PlaySE(0x1F);
        return 1;
    }
    return 0;
}

/* Outer step 4, every frame: ticks the fade. Once the fade-out is complete, stores the face in
 * gDuelScene.result and returns 1. Otherwise runs gDiceScreenSteps[step] (gPlainDieScreenSteps for the plain
 * die), advancing when the step returns non-zero, and flushes the OAM list. */
u32 DiceScreen_Update(void)
{
    FadeTick(&gDiceScreen.fade);
    if (gDiceScreen.fade.state == FADE_STATE_FADED_OUT) {
        gDuelScene.result = gDiceScreen.result;
        return 1;
    }
    /* Matching: two copies of the call with identical tails (cross-jumped in the ROM); a table pointer chosen
     * first gets if-converted. */
    if (gDiceScreen.variant == DICE_VARIANT_PLAIN) {
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
    OamListFlush(&gDiceScreen.oamList);
    OamListClear((u8 *)&gDiceScreen.oamList);
    return 0;
}

/* Outer step 2 of the Skull Dice scene: variant DICE_VARIANT_SKULL, die palette 2. Returns 1. */
u32 DiceScreen_SetupSkullDice(void)
{
    gDiceScreen.variant = DICE_VARIANT_SKULL;
    gDiceScreen.diePalette = 2;
    return 1;
}

/* Outer step 2 of the Graceful Dice scene: variant DICE_VARIANT_GRACEFUL, die palette 0. Returns 1. */
u32 DiceScreen_SetupGracefulDice(void)
{
    gDiceScreen.variant = DICE_VARIANT_GRACEFUL;
    gDiceScreen.diePalette = 0;
    return 1;
}

/* Outer step 2 of the plain-die scene: variant DICE_VARIANT_PLAIN (no character), die palette 0. Returns 1. */
u32 DiceScreen_SetupPlainDie(void)
{
    gDiceScreen.variant = DICE_VARIANT_PLAIN;
    gDiceScreen.diePalette = 0;
    return 1;
}

/*
 * Outer step 0: clears gDiceScreen and picks the final roll for the face n = gDuelScene.arg (1-6). A roll strip
 * has four quarters of 5 frames, one per face of its ring. Each face lies on two rings (gDieFacePaths[n - 1]);
 * one is picked at random: finalAxis is its axis, and the roll starts two quarters before the face's quarter,
 * at frame startQuarter * 5 + 2, so that the 10-frame roll stops on the face. result is then
 * gDieAxisFaces[finalAxis][(startQuarter + 2) % 4], which is n again. Returns 1.
 */
u32 DiceScreen_PrepareRoll(void)
{
    struct DiceScreen *work = &gDiceScreen;
    u8 path;
    u16 face;
    u8 startQuarter;
    u8 faceQuarter;
    int sum;    /* Matching: the + 2 in its own signed variable (the ROM's add before the signed % 4) */
    u8 *result;

    MemClear16(work, sizeof(*work));
    path = Random() % 2;
    face = gDuelScene.arg;
    result = &work->result;
    *result = face;
    work->finalAxis = gDieFacePaths[(face - 1) % 6][path][0];
    faceQuarter = gDieFacePaths[(*result - 1) % 6][path][1];
    sum = faceQuarter + 2;
    startQuarter = sum % 4;
    work->finalRollStart = startQuarter * 5 + 2;
    *result = gDieAxisFaces[work->finalAxis][(startQuarter + 2) % 4];
    /* FAKEMATCH: one more reference keeps &work->result in a register through the last store (the ROM keeps
     * the work base in r7 and &result in r6; tools/regoracle.py: result needs refs 4 -> 5 to be allocated
     * before work). */
    asm volatile ("" : : "r"(result));
    return 1;
}

/* Scene handler of DUEL_SCENE_DICE_SKULL: runs gSkullDiceSceneSteps[gDuelScene.sceneStep], advancing when the
 * step returns non-zero; returns 1 at the NULL end of the table. */
u32 DuelScene_SkullDice(void)
{
    if (gSkullDiceSceneSteps[gDuelScene.sceneStep]) {
        if (gSkullDiceSceneSteps[gDuelScene.sceneStep]())
            gDuelScene.sceneStep++;
        return 0;
    }
    return 1;
}
