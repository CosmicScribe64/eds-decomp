/*
 * Duel card animations and the graphics of the battle scene (0x0805D58C-0x0805E787,
 * wiki/functions/duel-card-anim-c.md).
 *
 *  - DuelAnim_Update*: the per-frame handlers of the five card animations. DuelAnim_Update (duel_field_view.c)
 *    runs the one for gDuelScreen.animKind (enum DuelAnimKind); DuelAnim_Request and its wrappers fill in
 *    gDuelScreen.animArg, from and to. Every handler clears gDuelScreen.animActive when it is done.
 *  - The battle scene that DUEL_CMD_START_BATTLE_SCENE / DUEL_CMD_PLAY_BATTLE_SCENE show after an attack: the two
 *    battling cards, full size, side by side. BattleScene_Init loads player 0's card onto the affine background
 *    BG2 (left) and player 1's onto BG3 (right); BattleScene_HBlank rolls both open line by line; the number
 *    helpers draw the ATK / DEF values and the damage as sprites. BattleScene_Update (battle_scene.c) runs it.
 */
#include "global.h"
#include "gba.h"                    /* DISPCNT_*, BGCNT_*, REG_BG2X..REG_BG3PA, B_BUTTON, OBJ_PLTT, OBJ_VRAM0 */
#include "main.h"                   /* gMain, IntrTable, INTR_SLOT_HBLANK, ResetBgScroll */
#include "sound.h"                  /* PlaySE */
#include "duel.h"                  /* struct DuelLoc, struct DuelZone */
#include "constants/duel.h"         /* DUEL_AREA_DECK, DUEL_AREA_HAND */
#include "constants/sound.h"        /* SE_CARD_FLIP */

#include "duel_screen.h"            /* gDuelScreen, DuelAnim_*, GetAreaX/Y, DuelSprAnim_*, DuelCursor_Select */
#include "battle.h"                 /* gBattle.scene, struct BattleScene */
#include "duel_cmd.h"                /* gLpDigitsPal, gLpDigitsGfx */
#include "card_data.h"              /* CARD_ID_MASK, CARD_ART_SIZE, CARD_STATS_*, gCardStatDigits*, gCardFrame*Gfx */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind */
#include "constants/cards.h"        /* CARD_OBELISK_THE_TORMENTOR, CARD_SLIFER_THE_SKY_DRAGON, ... */
#include "sprite.h"                 /* AddSprite, AddAffineSprite, enum SpriteShape */
#include "util.h"                   /* MemClear16, MemCopy16, CopyDoubleWords, Random */
#include "bg.h"                     /* ClearBgMapBuffers */

/*
 * Local views of callees, kept on purpose (build/readability/HEADERS.md "Keeping a deliberate local view"):
 * the header prototypes have narrower parameters, which would add narrowing at the call sites of this unit.
 * Same symbols, other C types.
 */
/* duel_screen.h: u16 cardId. The animations pass an int / u32 that is not narrowed before the call. */
extern int GetCardIconObjTileInt(int cardId) asm("GetCardIconObjTile");
/* duel_screen.h: s16 x, y and u16 advance. The effect animation passes the int sums unnarrowed. */
extern void DuelSprAnim_DrawInt(int x, int y, int advance) asm("DuelSprAnim_Draw");

/*
 * Local views of data, kept on purpose.
 *
 * gDuelAnimArgs is the linker symbol 0x0201D7E4 = &gDuelScreen.animArg: the handlers that move cards reach the
 * animation arguments through its own address literal, so it stays a separate view. DuelLocWord reads a
 * DuelLoc through one u32 container (the header's u16 containers would load halfwords).
 */
struct DuelLocWord {
    u32 player:1;                   /* bit 0 */
    u32 area:4;                     /* bits 1-4: enum DuelArea */
    u32 index:9;                    /* bits 5-13 */
    u32 flags:18;                   /* bit 14 isDefense, bit 15 isFaceUp, then DuelLoc.unk2 */
};

struct DuelAnimArgsView {
    u32 animArg;                    /* +0x00 = gDuelScreen.animArg */
    u8 animStep;                    /* +0x04 = gDuelScreen.animStep */
    u8 pad5;
    s16 animDx;                     /* +0x06 */
    s16 animDy;                     /* +0x08 */
    u16 padA;
    struct DuelLocWord endpoints[2];    /* +0x0C from, +0x10 to */
    u8 pad14[0x1E - 0x14];
    u16 sprAnimFrame;               /* +0x1E = gDuelScreen.sprAnim.frameIndex (back at 0 when the stream ends) */
};
extern struct DuelAnimArgsView gDuelAnimArgs;   /* 0x0201D7E4 */

/* A zone's card word as a plain u32 member. Matching: reading the id as gDuelZones[..].zones[..].card (a bitfield
 * struct) makes the compiler fold the zone address in another order and share it with the isFaceUp read of the
 * same zone; the ROM computes the two addresses separately. */
struct ZoneCardWord {
    u32 cardWord;                   /* struct DuelCard */
    u8 rest[0x94 - 4];
};
struct PlayerZoneCardWords {
    struct ZoneCardWord zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};
#define ZONE_CARD_WORD(side, slot)  (((struct PlayerZoneCardWords *)gDuelZones)[side].zones[slot].cardWord)
#define CARD_ID_FROM_WORD(word)     (((word) << 20) >> 20)      /* the low 12 bits, read with shifts as the ROM does */

/* gDuelScreen.animArg of kinds 1 and 2 with its upper two bytes as one halfword. Matching: the handlers load
 * isDefense and extra together (ldrh, then >> 8 for extra); the header's struct DuelAnimZoneArg has two bytes. */
struct DuelAnimZoneArgHalf {
    u8 player;                      /* +0: zone owner */
    u8 slot;                        /* +1: zone 0-10 */
    u16 flags;                      /* +2: low byte isDefense, high byte extra */
};

/* ROM tables used only by this unit (duel_screen.h and battle_scene.h list them but do not declare them). */
extern const u16 gDuelAnimLerpWeights[];            /* 0x081A4454: [16] ease-out weights /256: 30, 59, ... 255 */
/* [isFaceUp before the flip][frame]: OBJ tile of each of the 24 flip frames. Card back 0x40 / 0x50 / 0x60, then
 * the face 0x1020 / 0x1010 / 0x1000 (bit 12: add the card's own OBJ tile); row 1 is row 0 reversed. Declared
 * 2D so that the index arithmetic comes out in the ROM's order (frame * 2 + row * 48, then the base). */
extern const u16 gCardFlipTiles[][24];              /* 0x081A4474 */
/* [isDefense before the change][frame]: affine angle of each of the 10 rotation frames, 0 -> 0x20 or 0x20 -> 0. */
extern const u16 gCardRotateAngles[][10];           /* 0x081A44D4 */
/* [open step 0-15][scanline 0-159]: BG2/BG3 reference point and PA for BattleScene_HBlank. */
extern const u32 gBattleSceneOpenBgX[16][160];      /* 0x0819DD94 */
extern const u32 gBattleSceneOpenBgY[16][160];      /* 0x081A0594 */
extern const u16 gBattleSceneOpenBgPA[16][160];     /* 0x081A2D94 */

/* Sound effects that constants/sound.h does not name yet. */
#define ANIM_SE_CARD_MOVE   7       /* a card slides to another place (move and swap animations) */
#define ANIM_SE_CARD_DRAW   14      /* a card is drawn from the deck to the hand */

/* Angles are 128 steps per turn: 0x20 is a quarter turn. attr2 | priority 1 puts a card sprite above the field. */
#define OBJ_ANGLE_QUARTER_TURN  0x20
#define OBJ_SCALE_ANGLE(scale, angle)   (((scale) << 16) | (angle))     /* AddAffineSprite's last argument; 0x100 = 1.0 */
#define CARD_BACK_TILE      0x40    /* OBJ tile of the card back */
#define CARD_ICON_PALETTE_ATTR2 OAM_ATTR2_PALETTE(1)    /* the 32x32 card icons use OBJ palette 1 (LoadDuelUiGfx) */
#define FLIP_TILE_CARD_FACE 0x1000  /* gCardFlipTiles entry bit (the same bit as palette 1): a face frame, whose tile
                                       is the card's own OBJ tile (GetCardIconObjTile) plus the entry's offset */

/* Last frame index of each animation (the handler runs while the frame counter is at or below it). */
#define ROTATE_LAST_FRAME   9       /* DUEL_ANIM_CHANGE_POSITION: 10 frames */
#define FLIP_LAST_FRAME     0x17    /* DUEL_ANIM_FLIP: 24 frames */
#define MOVE_LAST_STEP      15      /* DUEL_ANIM_MOVE_CARD / SWAP_CARDS: 16 steps */
#define MOVE_DONE_STEP      16

/* A card's DuelLoc halfword as bytes: byte 0 = player | area << 1 | index << 5 (low bits), byte 1 holds
 * isDefense (bit 6) and isFaceUp (bit 7). Matching: the ROM tests them as byte masks. */
#define LOC_AREA_MASK       0x1E    /* byte 0, bits 1-4 */
#define LOC_DEFENSE_BIT     0x40    /* byte 1 */
#define LOC_FACE_UP_BIT     0x80    /* byte 1 */

/* The ROM compares the fast-forward flag (gDuelScreen bit 0) as a whole byte, not as the bitfield member. */
#define DUEL_SCREEN_FAST(screen)    (1 & *(u8 *)(screen))

/*
 * Kind 1 (DUEL_ANIM_CHANGE_POSITION): the card in zone (player, slot) turns a quarter turn between attack and
 * defense position over 10 frames, optionally flipping at the same time.
 *   animArg (struct DuelAnimZoneArg): player, slot, isDefense (position before the change), extra (also flip).
 * Step 0 plays the flip sound and clears the zone's field tiles (the animated sprite replaces them); step 1 draws
 * one affine sprite per frame, rotated by gCardRotateAngles[isDefense][frame] and using the tile
 * gCardFlipTiles[isFaceUp][0] (the card as it is) or, when `extra` is set, the matching frame of the flip.
 */
void DuelAnim_UpdateChangePosition(void)
{
    struct DuelScreen *screen = &gDuelScreen;
    struct DuelAnimZoneArgHalf *arg = (struct DuelAnimZoneArgHalf *)&screen->animArg;
    u8 player, slot;
    int zoneIndex;
    u8 isDefense;
    int side;
    int isFaceUp;
    int x, y;
    int alsoFlip;
    int cardId;
    u8 *step;
    player = arg->player;
    /* Loading slot through an int temporary shortens its live range by one insn before combine, so that slot is
       allocated before player (slot r6, player r7 as in the ROM). */
    zoneIndex = arg->slot;
    slot = zoneIndex;
    isDefense = *(u8 *)&arg->flags;
    side = player & 1;
    /* Pointer arithmetic (symbol last) puts the base literal after the offset, as in the ROM. */
    isFaceUp = ((struct DuelZone *)(side * sizeof(struct DuelZonesPlayer) + slot * sizeof(struct DuelZone) + (u32)gDuelZones))->isFaceUp;
    alsoFlip = arg->flags >> 8;
    cardId = CARD_ID_FROM_WORD(ZONE_CARD_WORD(side, slot));
    step = &screen->animStep;
    switch (*step) {
    case 0:
        PlaySE(SE_CARD_FLIP);
        ClearZoneTiles(player, slot);
        screen->animTimer = 0;
        (*step)++;
        /* fall through */
    case 1:
        if (gDuelScreen.animTimer <= ROTATE_LAST_FRAME) {
            u16 attr2;
            x = GetAreaX(player, DUEL_AREA_MONSTER, slot);
            y = GetAreaY(player, DUEL_AREA_MONSTER, slot);
            attr2 = gCardFlipTiles[isFaceUp][0];
            if (alsoFlip != 0)
                /* Spread the 24 flip frames over the 10 rotation frames. */
                attr2 = gCardFlipTiles[isFaceUp][(gDuelScreen.animTimer * 24) / 10];
            if (attr2 & FLIP_TILE_CARD_FACE) {
                attr2 &= (u16)~FLIP_TILE_CARD_FACE;
                {
                    /* An int temporary keeps the (s16) sign extension and the call + 0x1000 order. */
                    int faceTile = GetCardIconObjTileInt(cardId) + CARD_ICON_PALETTE_ATTR2;
                    int sum = (s16)attr2 + faceTile;
                    attr2 = sum;
                }
            }
            AddAffineSprite((y << 16) | x, SPRITE_SHAPE_32x32, attr2 | OAM_ATTR2_PRIORITY(1),
                            OBJ_SCALE_ANGLE(0x100, gCardRotateAngles[isDefense][gDuelScreen.animTimer]));
            gDuelScreen.animTimer++;
            if (gDuelScreen.animTimer <= ROTATE_LAST_FRAME)
                return;
        }
        /* fall through */
    default:
        gDuelScreen.animActive = 0;
        break;
    }
}

/*
 * Kind 2 (DUEL_ANIM_FLIP): the card in zone (player, slot) flips face up or face down over 24 frames, kept at
 * a quarter turn when it is in defense position.
 *   animArg (struct DuelAnimZoneArg): player, slot, isDefense, extra = isFaceUp before the flip (the row of
 *   gCardFlipTiles).
 */
void DuelAnim_UpdateFlip(void)
{
    struct DuelScreen *screen = &gDuelScreen;
    struct DuelAnimZoneArgHalf *arg = (struct DuelAnimZoneArgHalf *)&screen->animArg;
    u8 player, slot;
    u8 isDefense;
    int faceRow;
    int cardId;
    u8 *step;
    player = arg->player;
    slot = arg->slot;
    isDefense = *(u8 *)&arg->flags;
    faceRow = arg->flags >> 8;
    cardId = CARD_ID_FROM_WORD(ZONE_CARD_WORD(player & 1, slot));
    step = &screen->animStep;
    switch (*step) {
    case 0:
        PlaySE(SE_CARD_FLIP);
        ClearZoneTiles(player, slot);
        screen->animTimer = 0;
        (*step)++;
        /* fall through */
    case 1:
        if (gDuelScreen.animTimer <= FLIP_LAST_FRAME) {
            int x = GetAreaX(player, DUEL_AREA_MONSTER, slot);
            int y = GetAreaY(player, DUEL_AREA_MONSTER, slot);
            /* FAKEMATCH: `player = faceRow` (faceRow <= 0xFF) reuses player as the row index; it keeps player live
               past the second call, which puts player in r7 and slot in r6 as in the ROM. */
            u16 attr2 = gCardFlipTiles[player = faceRow][gDuelScreen.animTimer];
            if (attr2 & FLIP_TILE_CARD_FACE) {
                attr2 &= (u16)~FLIP_TILE_CARD_FACE;
                {
                    /* An int temporary keeps the (s16) sign extension and the call + 0x1000 order. */
                    int faceTile = GetCardIconObjTileInt(cardId) + CARD_ICON_PALETTE_ATTR2;
                    int sum = (s16)attr2 + faceTile;
                    attr2 = sum;
                }
            }
            AddAffineSprite((y << 16) | x, SPRITE_SHAPE_32x32, attr2 | OAM_ATTR2_PRIORITY(1),
                            isDefense ? OBJ_SCALE_ANGLE(0x100, OBJ_ANGLE_QUARTER_TURN) : OBJ_SCALE_ANGLE(0x100, 0));
            gDuelScreen.animTimer++;
            if (gDuelScreen.animTimer <= FLIP_LAST_FRAME)
                return;
        }
        /* fall through */
    default:
        gDuelScreen.animActive = 0;
        break;
    }
}

/*
 * Kind 3 (DUEL_ANIM_MOVE_CARD): one card sprite moves from gDuelScreen.from to gDuelScreen.to over 16 steps.
 *   animArg: card ID; from / to: struct DuelLoc.
 * The position eases out (gDuelAnimLerpWeights / 256), the size follows gBounceScaleCurve (shrinks to half and
 * rebounds), the sprite shows the card face (OBJ tile of animArg) when `to` is face up, else the card back, and
 * turns to match the defense bit of `to` (interpolated when the position changes on the way). Step 0 plays the
 * draw sound for deck -> hand, else the move sound. Holding B (or gDuelScreen.fast) adds 3 extra steps per frame
 * until step 11.
 */
void DuelAnim_UpdateMoveCard(void)
{
    struct DuelLocWord *from = &gDuelAnimArgs.endpoints[0];
    struct DuelLocWord *to = &gDuelAnimArgs.endpoints[1];
    if (*((u8 *)to - 12) == 0) {                    /* gDuelAnimArgs.animStep == 0 */
        if ((*(u8 *)from & LOC_AREA_MASK) == (DUEL_AREA_DECK << 1) && (*(u8 *)to & LOC_AREA_MASK) == (DUEL_AREA_HAND << 1))
            PlaySE(ANIM_SE_CARD_DRAW);
        else
            PlaySE(ANIM_SE_CARD_MOVE);
    }
    if (gDuelScreen.animStep <= MOVE_LAST_STEP) {
        int fromX = GetAreaX(from->player, from->area, from->index);
        int fromY = GetAreaY(from->player, from->area, from->index);
        int toX = GetAreaX(to->player, to->area, to->index);
        int toY = GetAreaY(to->player, to->area, to->index);
        int dx, dy;
        u16 tile = CARD_BACK_TILE;
        int angle = 0;
        u16 ease;
        int oldStep;
        dx = toX - fromX;
        dy = toY - fromY;
        ease = gDuelAnimLerpWeights[gDuelScreen.animStep];
        dx *= ease;
        dy *= ease;
        dx /= 256;
        dy /= 256;
        if (((u8 *)to)[1] & LOC_FACE_UP_BIT)
            tile = GetCardIconObjTileInt(gDuelScreen.animArg) + CARD_ICON_PALETTE_ATTR2;
        if ((u8)(((u8 *)from)[1] & LOC_DEFENSE_BIT) == (u8)(((u8 *)to)[1] & LOC_DEFENSE_BIT)) {
            if ((u8)(((u8 *)to)[1] & LOC_DEFENSE_BIT)) angle = OBJ_ANGLE_QUARTER_TURN;
        } else if ((u8)(((u8 *)to)[1] & LOC_DEFENSE_BIT)) {
            angle = gDuelScreen.animStep * 2;                           /* turning to defense: 0 -> 0x20 */
        } else {
            angle = OBJ_ANGLE_QUARTER_TURN - gDuelScreen.animStep * 2;  /* turning to attack: 0x20 -> 0 */
        }
        AddAffineSprite((fromX + dx) | ((fromY + dy) << 16), SPRITE_SHAPE_32x32, tile + OAM_ATTR2_PRIORITY(1),
                        ((u32)gBounceScaleCurve[gDuelScreen.animStep] << 16) | angle);
        oldStep = gDuelScreen.animStep;
        gDuelScreen.animStep = oldStep + 1;
        if ((gMain.heldKeys & B_BUTTON) || DUEL_SCREEN_FAST(&gDuelScreen)) {
            if (gDuelScreen.animStep <= 11)
                gDuelScreen.animStep = oldStep + 4;
        }
    }
    if (gDuelScreen.animStep == MOVE_DONE_STEP)
        gDuelScreen.animActive = 0;
}

/*
 * Kind 4 (DUEL_ANIM_SWAP_CARDS): two card backs trade places, one from gDuelScreen.from to .to and the other the
 * opposite way, over 16 eased steps with the same bounce scale and fast-forward rule as DuelAnim_UpdateMoveCard.
 * Step 0 plays the move sound.
 */
void DuelAnim_UpdateSwapCards(void)
{
    struct DuelLocWord *endpoints = gDuelAnimArgs.endpoints;
    struct DuelScreen *screen;
    u8 *step = (u8 *)endpoints - 8;                 /* gDuelAnimArgs.animStep */
    if (*step == 0)
        PlaySE(ANIM_SE_CARD_MOVE);
    if (*step <= MOVE_LAST_STEP) {
        int x1 = GetAreaX(endpoints[0].player, endpoints[0].area, endpoints[0].index);
        int y1 = GetAreaY(endpoints[0].player, endpoints[0].area, endpoints[0].index);
        int x2 = GetAreaX(endpoints[1].player, endpoints[1].area, endpoints[1].index);
        int y2 = GetAreaY(endpoints[1].player, endpoints[1].area, endpoints[1].index);
        int dx1 = x2 - x1;
        int dy1 = y2 - y1;
        int dx2 = x1 - x2;
        int dy2 = y1 - y2;
        u16 ease = gDuelAnimLerpWeights[*step];
        u8 oldStep;
        dx1 *= ease;
        dy1 *= ease;
        dx1 /= 256;
        dy1 /= 256;
        dx2 *= ease;
        dy2 *= ease;
        dx2 /= 256;
        dy2 /= 256;
        /* Stage the screen address before drawing; read its fast flag afterward. */
        screen = &gDuelScreen;
        AddAffineSprite((x1 + dx1) | ((y1 + dy1) << 16), SPRITE_SHAPE_32x32,
                        CARD_BACK_TILE | OAM_ATTR2_PRIORITY(1), (u32)gBounceScaleCurve[*step] << 16);
        AddAffineSprite((x2 + dx2) | ((y2 + dy2) << 16), SPRITE_SHAPE_32x32,
                        CARD_BACK_TILE | OAM_ATTR2_PRIORITY(1), (u32)gBounceScaleCurve[*step] << 16);
        oldStep = *step;
        *step = oldStep + 1;
        if ((gMain.heldKeys & B_BUTTON) || DUEL_SCREEN_FAST(screen)) {
            if (*step <= 11)
                *step = oldStep + 4;
        }
    }
    if (gDuelScreen.animStep == MOVE_DONE_STEP)
        gDuelScreen.animActive = 0;
}

/*
 * Kind 5 (DUEL_ANIM_ZONE_EFFECT): plays a sprite animation stream (explosion, negate, ...) on a zone.
 *   animArg: address of the stream; from: the zone's DuelLoc; animDx / animDy: offset from the zone position.
 * Step 0 resets the cursor selection, step 1 loads the stream, step 2 draws one frame per call until the stream
 * wraps back to frame 0, and the last step reloads the duel UI graphics that the stream overwrote.
 */
void DuelAnim_UpdateZoneEffect(void)
{
    struct DuelLocWord *zone = &gDuelAnimArgs.endpoints[0];
    int x = GetAreaX(zone->player, zone->area, zone->index);
    int y = GetAreaY(zone->player, zone->area, zone->index);
    u8 *step = &gDuelAnimArgs.animStep;
    switch (*step) {
    case 0:
        DuelCursor_Select(0, 0, 0);
        (*step)++;
        break;
    case 1: {
        u32 *stream = &gDuelAnimArgs.animArg;
        DuelSprAnim_Load(*stream);
        (*step)++;
        break;
    }
    case 2: {
        s16 *dx = &gDuelAnimArgs.animDx;
        s16 *dy;
        int drawX = x + *dx;
        dy = &gDuelAnimArgs.animDy;
        DuelSprAnim_DrawInt(drawX, y + *dy, 1);
        if (gDuelAnimArgs.sprAnimFrame == 0)
            (*step)++;
        break;
    }
    default:
        LoadDuelUiGfx();
        gDuelScreen.animActive = 0;
        break;
    }
}

/*
 * HBlank handler of the battle scene (installed as IntrTable[INTR_SLOT_HBLANK] by BattleScene_Init). For the
 * scanline just drawn it loads the BG2 and BG3 reference point and PA from the open tables, row
 * gBattle.scene.subState. Row 0 shows nothing (PA 0); each row widens the visible band around the middle line
 * until row 15 is the identity, so both cards roll open vertically while the screen fades in.
 */
void BattleScene_HBlank(void)
{
    gMain.lastVcount = REG_VCOUNT;
    REG_BG2X = gBattleSceneOpenBgX[gBattle.scene.subState][gMain.lastVcount];
    REG_BG2Y = gBattleSceneOpenBgY[gBattle.scene.subState][gMain.lastVcount];
    REG_BG2PA = gBattleSceneOpenBgPA[gBattle.scene.subState][gMain.lastVcount];
    REG_BG3X = gBattleSceneOpenBgX[gBattle.scene.subState][gMain.lastVcount];
    REG_BG3Y = gBattleSceneOpenBgY[gBattle.scene.subState][gMain.lastVcount];
    REG_BG3PA = gBattleSceneOpenBgPA[gBattle.scene.subState][gMain.lastVcount];
}

/* Resets BG2 and BG3 to the identity transform (reference point 0, PA = 1.0 in 8.8), undoing the last values
 * BattleScene_HBlank wrote. */
void BattleScene_ResetBgAffine(void)
{
    REG_BG2X = 0;
    REG_BG2Y = 0;
    REG_BG2PA = 0x100;
    REG_BG3X = 0;
    REG_BG3Y = 0;
    REG_BG3PA = 0x100;
}

/* OBJ tiles and palettes that BattleScene_Init loads (gCardStatDigitsGfx at tile 0x20, gLpDigitsGfx at 0x2E). */
#define STAT_DIGIT_PALETTE  2
#define STAT_DIGIT_TILE     0x20    /* tiles 0x20-0x29: the 8x8 digits 0-9 */
#define STAT_ATK_LABEL_TILE 0x2A    /* 16x8 "ATK" */
#define STAT_DEF_LABEL_TILE 0x2C    /* 16x8 "DEF" */
#define LP_DIGIT_PALETTE    3
#define LP_DIGIT_TILE       0x2E    /* four colours of 0x30 tiles each; a 16x16 digit is 4 tiles */

/* Draws `value` right-aligned as 8x8 digit sprites (OBJ palette 2): the last digit at x + 0x14, each earlier
 * one 4 pixels to the left. 0 draws a single "0". */
void BattleScene_DrawSmallNumber(int x, int y, int value)
{
    int n = value;
    int base = OAM_ATTR2_PALETTE(STAT_DIGIT_PALETTE) | STAT_DIGIT_TILE;
    x += 0x14;
    if (n == 0) {
        AddSprite((y << 16) | x, SPRITE_SHAPE_8x8, base);
    } else {
        do {
            AddSprite(x | (y << 16), SPRITE_SHAPE_8x8, (u16)(n % 10 + base));
            n /= 10;
            x -= 4;
        } while (n != 0);
    }
}

/* Draws `value` right-aligned as 16x16 digit sprites (OBJ palette 3) of colour 0-3: the last digit at x + 0x50,
 * each earlier one 16 pixels to the left. 0 draws a single "0". */
void BattleScene_DrawBigNumber(int x, int y, int value, int colour)
{
    int n = value;
    u16 base = OAM_ATTR2_PALETTE(LP_DIGIT_PALETTE) + LP_DIGIT_TILE + colour * 0x30;
    x += 0x50;
    if (n == 0) {
        AddSprite((y << 16) | x, SPRITE_SHAPE_16x16, base);
    } else {
        do {
            AddSprite(x | (y << 16), SPRITE_SHAPE_16x16, (u16)(base + (n % 10) * 4));
            n /= 10;
            x -= 0x10;
        } while (n != 0);
    }
}

/* Draws the "ATK" label and `value` on the ATK line of `side`'s card frame (side 1 is 0x78 pixels to the right);
 * (x, y) is an extra offset. */
void BattleScene_DrawAtk(int side, int x, int y, u32 value)
{
    x += side * 0x78;
    x += 0x47;
    y += 0x7E;
    AddSprite((y << 16) | x, SPRITE_SHAPE_16x8, OAM_ATTR2_PALETTE(STAT_DIGIT_PALETTE) | STAT_ATK_LABEL_TILE);
    BattleScene_DrawSmallNumber(x + 4, y, value);
}

/* As BattleScene_DrawAtk with the "DEF" label, one row lower (for a monster in defense position). */
void BattleScene_DrawDef(int side, int x, int y, u32 value)
{
    x += side * 0x78;
    x += 0x47;
    y += 0x86;
    AddSprite((y << 16) | x, SPRITE_SHAPE_16x8, OAM_ATTR2_PALETTE(STAT_DIGIT_PALETTE) | STAT_DEF_LABEL_TILE);
    BattleScene_DrawSmallNumber(x + 4, y, value);
}

/*
 * Draws the value of each side: values[side] with the DEF label when the side's flag byte has
 * BATTLE_SIDE_DEFENSE, else with the ATK label. `flags` holds player 0's byte in bits 0-7 and player 1's in
 * bits 8-15 (enum BattleSideFlags); with hideDestroyed set, a side flagged BATTLE_SIDE_DESTROYED is skipped.
 */
void BattleScene_DrawValues(u16 flags, u32 *values, u16 hideDestroyed)
{
    int side;
    u32 *value;
    int shift;
    for (side = 0, value = values, shift = 0; side <= 1; value++, shift += 8, side++) {
        if (hideDestroyed == 0 || (flags & (BATTLE_SIDE_DESTROYED << shift)) == 0) {
            if (((int)flags >> shift) & BATTLE_SIDE_DEFENSE)
                BattleScene_DrawDef(side, 0, 0, *value);
            else
                BattleScene_DrawAtk(side, 0, 0, *value);
        }
    }
}

/* Draws the life-point damage `damage` as big digits over `side`'s card. With `flash` set the colour is random
 * (it cycles while the card shakes), else the side's own colour. */
void BattleScene_DrawDamage(int side, int damage, u16 flash)
{
    int x = side * 0x68 + 8;
    int colour = side;
    if (flash != 0)
        colour = Random() & 3;
    BattleScene_DrawBigNumber(x, 0x40, damage, colour);
}

/*
 * Matching: the ROM reaches the card art tables through integer-constant addresses (as LoadCardPicture does),
 * not through the symbols gCardArtPalettes / gCardArtGfx, so that the compiler reloads both bases.
 */
#define CARD_ART_PALETTES_ADDR  0x08608360  /* gCardArtPalettes */
#define CARD_ART_GFX_ADDR       0x082A6500  /* gCardArtGfx */

/*
 * Loads the picture of card `cardId` for BG `bg` (0 = BG2 / player 0, 1 = BG3 / player 1): copies the card's
 * 64-colour palette to BG palette entries (palBase >> 4) * 16, unpacks its 72x80 6bpp art (8 pixels per 3
 * halfwords) to one pixel per byte at BG_CHAR_ADDR(1 + bg) + tileBase * 32, and adds the palette base
 * (palBase & 0xFF) to all 0xB40 halfwords (2880 = 72 * 80 / 2 pixels pairs).
 * `(s1 & 0xFC) * 64` (not `<< 6`) keeps that term out of the u16 narrowing, so its mask ties to the output.
 */
void BattleScene_LoadCardArt(int bg, u16 cardId, u16 tileBase, u16 palBase)
{
    const u16 *src;
    u16 *dst;
    u16 *p;
    int n;
    u32 i;
    u16 mask6, mask12;

    MemCopy16((void *)(PLTT + (palBase >> 4) * 0x20), (const void *)(CARD_ART_PALETTES_ADDR + cardId * CARD_ART_PALETTE_SIZE),
              CARD_ART_PALETTE_SIZE);
    src = (const u16 *)(CARD_ART_GFX_ADDR + cardId * CARD_ART_SIZE);
    {
        u32 addr = bg << 14;
        addr += BG_CHAR_ADDR(1);
        addr += tileBase << 5;
        dst = (u16 *)addr;
    }
    mask6 = 0x3F;
    mask12 = 0xFC0;
    /* 720 groups of 8 pixels: 3 source halfwords (48 bits) -> 4 destination halfwords (8 bytes). */
    for (n = 720; n != 0; n--) {
        u16 s0 = src[0];
        u32 s1 = src[1];
        u32 s2 = src[2];
        u16 t, x;
        dst[0] = (s0 & mask6) | ((s0 & mask12) << 2);
        dst[1] = (s0 >> 12) | ((s1 & 3) << 4) | ((s1 & 0xFC) * 64);
        t = s1 >> 8;
        dst[2] = (t & mask6) | (((t >> 6) | ((s2 & 0xF) << 2)) << 8);
        x = s2 >> 4;
        dst[3] = (x & mask6) | ((x & mask12) << 2);
        src += 3;
        dst += 4;
    }
    {
        u32 addr = bg << 14;
        addr += BG_CHAR_ADDR(1);
        addr += tileBase << 5;
        p = (u16 *)addr;
    }
    for (i = 0; i <= 0xB3F; i++) {
        *p = (*p & 0x3F3F) + ((u8)palBase << 8 | (u8)palBase);
        p++;
    }
}

/*
 * Loads a card frame (an 8bpp image pack: u16 colour count, 4 header bytes, the palette, then a u16 tile count
 * and 64-byte tiles; the pack's map is not used) for BG `bg`: the tiles go to BG_CHAR_ADDR(1 + bg) + tileBase * 32,
 * with (palBase & 0xFF) added to every non-zero pixel byte, and the first 32 palette colours to BG palette
 * entries (palBase >> 4) * 16.
 */
void BattleScene_LoadCardFrame(int bg, u16 *imagePack, u16 tileBase, u16 palBase)
{
    int paletteBytes = *imagePack * 2;
    u16 *tileCount = (u16 *)((u8 *)imagePack + (paletteBytes + 8));
    u16 *src = (u16 *)((u8 *)imagePack + (paletteBytes + 16));
    u32 addr = bg << 14;
    u16 *dst;
    u16 i;
    u16 pixels;
    addr += BG_CHAR_ADDR(1);
    addr += tileBase << 5;
    dst = (u16 *)addr;
    for (i = 0; i < (*tileCount << 5); i++) {
        u16 word = *src;
        pixels = word;
        /* FAKEMATCH: an empty asm that reads `word` keeps the loaded halfword in its own register, apart from
           the pixel accumulator `pixels`, as the ROM does. */
        __asm__ __volatile__("" : : "r"(word));
        if (word & 0xFF00)
            pixels += (u8)palBase << 8;
        if (pixels & 0xFF)
            pixels += (u8)palBase;
        *dst = pixels;
        dst++;
        src++;
    }
    MemCopy16((void *)(PLTT + (palBase >> 4) * 0x20), imagePack + 4, 0x40);
}

/*
 * Writes the 9x10 tile map of the card picture into the affine map of BG `bg` (BG_SCREEN_ADDR(bg)), top-left at
 * tile (x, y). An affine map has one byte per tile, so the map is written as halfwords and an odd x splits the
 * first and last entry of each row across two halfwords. The tile numbers are consecutive from tileBase / 2
 * (8bpp tiles are two 4bpp units).
 */
void BattleScene_SetCardArtMap(int bg, int x, int y, int tile)
{
    u16 *dst = (u16 *)((bg << 11) + BG_VRAM);
    int row;
    tile /= 2;      /* from here on: the running 8bpp tile number */
    dst += x / 2;
    dst += y * 16;
    for (row = 0; row < 10; row++) {
        int j;
        u16 *p;
        if (x & 1) {
            *dst = (u8)tile << 8;
            tile++;
            p = dst + 1;
            for (j = 0; j < 4; j++) {
                p[j] = (u8)tile | (u8)(tile + 1) << 8;
                tile += 2;
            }
        } else {
            for (j = 0; j < 4; j++) {
                dst[j] = (u8)tile | (u8)(tile + 1) << 8;
                tile += 2;
            }
            dst[4] = (u8)tile;
            tile++;
        }
        dst += 16;
    }
}

/*
 * Writes the 13x18 tile map of the card frame into the affine map of BG `bg`, top-left at tile (x, y): 4 full
 * rows, 10 rows with only the two border tiles on each side (OR-ed in, so that the picture's tiles that share a
 * halfword survive), and 4 full rows. The 9x10 hole is where BattleScene_SetCardArtMap puts the picture. Tile
 * numbers are consecutive from tileBase / 2.
 */
void BattleScene_SetCardFrameMap(int bg, int x, int y, int tile)
{
    u16 *dst = (u16 *)((bg << 11) + BG_VRAM);
    int row;
    int j;
    tile /= 2;      /* from here on: the running 8bpp tile number */
    dst += x / 2;
    dst += y * 16;
    /* Top rows: 13 entries each. */
    for (row = 0; row < 4; row++) {
        u16 *p;
        if (x & 1) {
            *dst = (u8)tile << 8;
            tile++;
            p = dst + 1;
            for (j = 0; j < 6; j++) {
                p[j] = (u8)tile | (u8)(tile + 1) << 8;
                tile += 2;
            }
        } else {
            for (j = 0; j < 6; j++) {
                dst[j] = (u8)tile | (u8)(tile + 1) << 8;
                tile += 2;
            }
            dst[6] = (u8)tile;
            tile++;
        }
        dst += 16;
    }
    /* Middle rows: two tiles on the left, the 9-tile hole, two tiles on the right. */
    for (row = 0; row < 10; row++) {
        if (x & 1) {
            dst[0] |= (u8)tile << 8;
            tile++;
            dst[1] |= (u8)tile;
            tile++;
            dst[6] |= (u8)tile | (u8)(tile + 1) << 8;
            tile += 2;
        } else {
            dst[0] |= (u8)tile | (u8)(tile + 1) << 8;
            tile += 2;
            dst[5] |= (u8)tile << 8;
            tile++;
            dst[6] |= (u8)tile;
            tile++;
        }
        dst += 16;
    }
    /* Bottom rows. */
    for (row = 0; row < 4; row++) {
        u16 *p;
        if (x & 1) {
            *dst = (u8)tile << 8;
            tile++;
            p = dst + 1;
            for (j = 0; j < 6; j++) {
                p[j] = (u8)tile | (u8)(tile + 1) << 8;
                tile += 2;
            }
        } else {
            for (j = 0; j < 6; j++) {
                dst[j] = (u8)tile | (u8)(tile + 1) << 8;
                tile += 2;
            }
            dst[6] = (u8)tile;
            tile++;
        }
        dst += 16;
    }
}

/*
 * Matching: Init reads the card tables through integer-constant addresses (the compiler reloads the address at
 * each use); the symbols gCardIdToNumber / gCardStats would give other literal pools.
 */
#define CARD_NUMBER_OF(id)  (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber */
#define CARD_STATS_OF(id)   (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* gCardStats */
#define CARD_TYPE_OF(id)    CARD_STATS_TYPE(CARD_STATS_OF(id))

/* The frame kind of a card (enum CardKind): the three Egyptian Gods are special-cased by card number, Magic /
 * Trap / Ticket cards by type, monsters by the kind bits of their stats. */
static inline int GetCardFrameKind(u16 cardId)
{
    switch (CARD_NUMBER_OF(cardId)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((u8)CARD_TYPE_OF(cardId)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_STATS_KIND(CARD_STATS_OF(cardId));
    }
}

/* The full-size frame image pack (104x144, 8bpp) for a card: by type for Trap / Magic / Ticket, else by kind. */
static inline const u8 *GetCardFrameGfx(u16 cardId)
{
    switch ((int)CARD_TYPE_OF(cardId)) {
    case CARD_TYPE_TRAP:
        return gCardFrameTrapGfx;
    case CARD_TYPE_MAGIC:
        return gCardFrameMagicGfx;
    case CARD_TYPE_TICKET:
        return gCardFrameTicketGfx;
    }
    switch (GetCardFrameKind(cardId)) {
    case CARD_KIND_EFFECT:
        return gCardFrameEffectGfx;
    case CARD_KIND_FUSION:
        return gCardFrameFusionGfx;
    case CARD_KIND_RITUAL:
        return gCardFrameRitualGfx;
    default:
        return gCardFrameNormalGfx;
    }
}

/*
 * Starts the battle scene for the two battling cards (card ID 0 leaves that side empty). Sets BG mode 2 with all
 * layers off, clears gBattle.scene and the duel screen's uiGfxLoaded / active flags, programs BG2 / BG3 as 256x256
 * 8bpp affine backgrounds (char blocks 1 / 2, screen blocks 0 / 1), loads the stat-digit and life-point-digit
 * OBJ graphics, then loads each card's picture and frame (player 0 left on BG2, player 1 right on BG3; picture at
 * tile (3, 5) / (0x12, 5), frame at (1, 1) / (0x10, 1)) and installs BattleScene_HBlank with the HBlank IRQ on.
 * BG palette plan: side 0 picture 0x40-0x7F, frame 0x80-0x9F; side 1 picture 0xA0-0xDF, frame 0xE0-0xFF.
 * Tile plan (4bpp units): frame at 0x2C (144 tiles), picture at 0x14C (90 tiles).
 */
void BattleScene_Init(u16 cardId0, u16 cardId1)
{
    MemClear16(&gBattleScene, sizeof(struct BattleScene));
    gDuelScreen.uiGfxLoaded = 0;
    gDuelScreen.active = 0;
    REG_DISPCNT = DISPCNT_MODE_2 | DISPCNT_OBJ_1D_MAP;
    gMain.vblankFlags = VBLANK_COPY_OAM;
    REG_BLDCNT = 0;
    REG_BG2CNT = BGCNT_AFF256x256 | BGCNT_256COLOR | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(0) | BGCNT_PRIORITY(0);
    REG_BG3CNT = BGCNT_AFF256x256 | BGCNT_256COLOR | BGCNT_CHARBASE(2) | BGCNT_SCREENBASE(1) | BGCNT_PRIORITY(0);
    ClearBgMapBuffers();
    ResetBgScroll();
    CopyDoubleWords((void *)(OBJ_PLTT + STAT_DIGIT_PALETTE * 0x20), gCardStatDigitsPal, 0x20);
    CopyDoubleWords((void *)(OBJ_VRAM0 + STAT_DIGIT_TILE * 0x20), gCardStatDigitsGfx, 0x1C0);
    CopyDoubleWords((void *)(OBJ_PLTT + LP_DIGIT_PALETTE * 0x20), gLpDigitsPal, 0x20);
    CopyDoubleWords((void *)(OBJ_VRAM0 + LP_DIGIT_TILE * 0x20), gLpDigitsGfx, 0x1800);
    MemClear16((void *)BG_CHAR_ADDR(1), BG_CHAR_SIZE);
    MemClear16((void *)BG_SCREEN_ADDR(0), BG_SCREEN_SIZE);
    if (cardId0 != 0) {
        BattleScene_LoadCardArt(0, cardId0, 0x14C, 0x40);
        BattleScene_SetCardArtMap(0, 3, 5, 0x14C);
        BattleScene_LoadCardFrame(0, (u16 *)GetCardFrameGfx(cardId0), 0x2C, 0x80);
        BattleScene_SetCardFrameMap(0, 1, 1, 0x2C);
    }
    MemClear16((void *)BG_CHAR_ADDR(2), BG_CHAR_SIZE);
    MemClear16((void *)BG_SCREEN_ADDR(1), BG_SCREEN_SIZE);
    if (cardId1 != 0) {
        BattleScene_LoadCardArt(1, cardId1, 0x14C, 0xA0);
        BattleScene_SetCardArtMap(1, 0x12, 5, 0x14C);
        BattleScene_LoadCardFrame(1, (u16 *)GetCardFrameGfx(cardId1), 0x2C, 0xE0);
        BattleScene_SetCardFrameMap(1, 0x10, 1, 0x2C);
    }
    gBattle.scene.subState = 0;
    /* Install the HBlank handler with interrupts off. */
    REG_IME = 0;
    REG_IE &= (u16)~INTR_FLAG_HBLANK;
    IntrTable[INTR_SLOT_HBLANK] = BattleScene_HBlank;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE |= INTR_FLAG_HBLANK;
    REG_IME = 1;
}
