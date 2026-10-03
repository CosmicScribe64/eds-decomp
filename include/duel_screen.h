#ifndef GUARD_DUEL_SCREEN_H
#define GUARD_DUEL_SCREEN_H

/*
 * The duel field screen (gDuelScreen, 0x0201CFB0). The field is a 256x512 map that scrolls vertically: BG0
 * is a drifting background texture, BG1 the board image, BG2 the card cells, life points and phase letters,
 * BG3 the text box and the info bar (two rows of 32 text cells at the bottom). On top of that are the field
 * cursor, the hand cards, the link and attack markers and the card animations (sprites).
 *
 * Coordinates: positions are field pixels (gDuelZonePositions); screen y = field y - gDuelScreen.scroll.
 * A screen area (enum DuelArea, constants/duel.h) is 0 monster row, 5 spell/trap row, 10 field zone,
 * 11 hand, 12 fusion deck, 13 deck, 14 graveyard, 15 banished; the rows add the zone index (0-4).
 *
 * Code: duel_field_view.c (scrolling, cursor moves, animation requests, the per-frame update),
 * duel_cursor.c (cursor input and target search), duel_card_anim.c (animation handlers), battle_scene.c
 * (info-bar text cells), duel_info_bar.c (graphics, life points, phase letters), duel_field_screen.c
 * (screen setup and teardown, fades, field cells, markers, hand cards), card_canvas.c (card-image canvas,
 * card icons, area positions), duel_card_actions.c and link_battle.c (chain-list overlay),
 * card_menu_input.c, duel_prompt_handlers.c, duel_cmd_moves.c and turn_order_steps.c (one function each).
 *
 * Every prototype is the function's definition as compiled. Many units call these functions through a
 * local declaration with other parameter or return widths (DuelCursor_PickTarget alone has 18 such
 * units). The call-site narrowing is part of the matched code, so those units keep their view as a
 * commented local alias prototype when they migrate (build/readability/proto_mismatches.txt).
 *
 * Alias symbols: two linker symbols point inside gDuelScreen. gDuelTextTiles (= textTiles) is declared
 * below. gDuelAnimArgs (0x0201D7E4 = &gDuelScreen.animArg) stays a unit-local view in duel_card_anim.c
 * (struct Pick: animArg +0, animStep +4, animDx +6, animDy +8, from +0xC, to +0x10, sprAnim.frameIndex
 * +0x1E), because the matched code loads that address from its own literal. Keep both access forms.
 */

#include "global.h"
#include "duel.h"       /* struct DuelLoc (DuelScreen.from / .to) */
#include "sprite.h"     /* struct SprAnim (DuelScreen.sprAnim) */

struct ChainList;       /* chain.h */

/* ------------------------------------------------------------------------------------------------------ */
/* Types                                                                                                  */
/* ------------------------------------------------------------------------------------------------------ */

/* gDuelScreen.animKind, the first argument of DuelAnim_Request: which DuelAnim_Update* handler runs. */
enum DuelAnimKind {
    DUEL_ANIM_CHANGE_POSITION = 1,  /* quarter turn between attack and defense, optionally flipping */
    DUEL_ANIM_FLIP = 2,             /* card flips face up or down in its zone */
    DUEL_ANIM_MOVE_CARD = 3,        /* one card moves between two locations (DuelAnim_MoveCard) */
    DUEL_ANIM_SWAP_CARDS = 4,       /* two face-down cards swap places (DuelAnim_SwapCards) */
    DUEL_ANIM_ZONE_EFFECT = 5       /* sprite animation stream on a zone (DuelAnim_PlayZoneEffect) */
};

/* `direction` argument of DuelCursor_FindTarget / DuelCursor_FindTargetHorizontal (one bit set). */
enum CursorDirection {
    CURSOR_DIR_UP = 1,
    CURSOR_DIR_DOWN = 2,
    CURSOR_DIR_RIGHT = 4,
    CURSOR_DIR_LEFT = 8
};

/* BG2 map entries of the field cells written by DrawZoneTiles / DrawAreaTiles (palette in bits 12-15).
 * Medium confidence for the names. */
enum FieldCellTile {
    FIELD_TILE_POSE_SIDEWAYS = 0x30,    /* added for a card in defense position */
    FIELD_TILE_CARD_BACK = 0x1070,      /* face-down card, or a deck of 1-5 cards */
    FIELD_TILE_THICK_PILE = 0x1230,     /* deck / fusion deck of more than 5 cards */
    FIELD_TILE_HELD_ZONE_MARK = 0x1240, /* 2x2 mark in an empty monster zone held for a banished monster */
    FIELD_TILE_ICON_PAL = 0x2000        /* added to GetCardIconBgTile for a face-up card (palette 2) */
};

/* Base OBJ tile of the marker sprites of DrawLinkMarkerPair / DrawFieldOverlay; attr2 adds the animation
 * frame and 0x5400 (palette 5, priority 1). Medium confidence for the names. */
enum ZoneMarkerTile {
    MARKER_TILE_CAN_ATTACK = 0x200,     /* Battle Phase: a monster that may still attack */
    MARKER_TILE_LINK_EQUIP = 0x20C,     /* zone links of kinds 1, 5 and 10 */
    MARKER_TILE_LINK_OTHER = 0x218      /* zone links of kinds 2 and 7, and a zone held for a banished card */
};

/* gChainListScreen.state: the steps of ChainListScreen_Run. */
enum ChainListScreenState {
    CHAIN_LIST_SETUP = 0,       /* clear the text cells, scroll to (0, 0) */
    CHAIN_LIST_DIM = 1,         /* darken the field */
    CHAIN_LIST_CLEAR_OBJ = 2,   /* clear OBJ VRAM */
    CHAIN_LIST_DRAW = 3,        /* render the header and the last four links into OBJ tiles */
    CHAIN_LIST_OPEN = 4,        /* rows grow over 16 frames */
    CHAIN_LIST_HOLD = 5,        /* until A, B or 120 frames */
    CHAIN_LIST_CLOSE = 6,
    CHAIN_LIST_UNDIM = 7,
    CHAIN_LIST_DONE = 8         /* reload the duel UI graphics and return 1 */
};

/* gDuelScreen (0x0201CFB0, 0x860 bytes): the state of the duel field screen. The field view
 * (duel_field_view.c) runs it every frame through DuelScreen_Update; the cursor pickers, the animation
 * requests and the info-bar code write the fields named after them. */
struct DuelScreen {
    u8 fast:1;                      /* +0x000 bit 0: fast-forward card animations, as if B were held (the
                                     *   link partner's LINKMSG_FAST_MODE sets it too) */
    u8 uiGfxLoaded:1;               /* bit 1: the duel UI OBJ tiles and palettes are in VRAM. LoadDuelUiGfx sets it;
                                     *   DuelSprAnim_Load, UnloadDuelUiGfx and the scene start clear it. The
                                     *   cursor, markers and hand cards are drawn only while it is set */
    u8 active:1;                    /* bit 2: the screen runs its per-frame update (DuelScreen_Init sets it;
                                     *   DuelScreen_Exit(1), the card viewer and the scenes clear it) */
    u8 unk0_3:5;
    u8 unk1;
    u16 fieldBgScroll;              /* +0x002: BG0 HOFS and VOFS, decremented every VBlank (DuelScreen_VBlank):
                                     *   the background texture drifts diagonally */
    u8 scroll;                      /* +0x004: field scroll in pixels, written to BG1 and BG2 VOFS */
    u8 scrollFrom;                  /* +0x005: scroll at the start of the interpolation */
    u8 scrollTo;                    /* +0x006: scroll target (DuelScreen_StartScroll) */
    u8 scrollSteps:4;               /* +0x007 bits 0-3: interpolation steps left, 4 -> 0 (gDuelScreenLerpWeights) */
    u8 fieldBackground:4;           /* bits 4-7: gFieldBackgroundImages index on BG0 (0 plain field, 1-14 the
                                     *   field-magic textures; DuelScreen_LoadFieldBackground) */
    u8 textTiles[0x800];            /* +0x008: the 64 info-bar text cells (4bpp 8x8, two rows of 32), uploaded to
                                     *   VRAM 0x060091C0 = BG tiles 0x28E-0x2CD; alias symbol gDuelTextTiles */
    u16 textTilesDirty:1;           /* +0x808 bit 0: upload textTiles this frame */
    u16 textMapReset:1;             /* bit 1: after the upload call TextCellsResetMap (TextCellsClear sets both) */
    u16 cursorDone:1;               /* bit 2: the cursor reached its target; DuelScreen_Update clears it and
                                     *   redraws the info bar (DuelScreen_DrawCursorInfo) */
    u16 showCursor:1;               /* bit 3: draw the cursor sprite (set by the cursor pickers) */
    u16 cursorRotate180:1;          /* bit 4: cursor drawn with affine angle 0x40 (half a turn of the 128-step
                                     *   table); no writer seen (hypothesis) */
    u16 cursorAltTile:1;            /* bit 5: cursor uses OBJ tile 0x30 instead of 0 (hypothesis: a second
                                     *   cursor graphic) */
    u16 cursorSteps:4;              /* bits 6-9: cursor interpolation steps left, 4 -> 0 */
    u16 unk808_10:6;
    u16 unk80A;
    s32 cursorX;                    /* +0x80C: cursor x in field pixels (the sprite is drawn at cursorX + 8) */
    s32 cursorY;                    /* +0x810: cursor y in field pixels (screen y = cursorY - scroll) */
    s32 cursorFromX;                /* +0x814: start of the cursor move */
    s32 cursorFromY;                /* +0x818 */
    s32 cursorToX;                  /* +0x81C: target of the cursor move (DuelCursor_MoveTo) */
    s32 cursorToY;                  /* +0x820 */
    s32 selPlayer;                  /* +0x824: player of the cursor selection (DuelCursor_Select); passed to
                                     *   IsTributableMonster and DUEL_CMD_POINT_AT_CARD */
    s32 selArea;                    /* +0x828: enum DuelArea of the selection (0, 5, 10, 11, 12-15) */
    s32 selIndex;                   /* +0x82C: index inside the area: zone within the row, or hand index */
    u8 animActive:1;                /* +0x830 bit 0: a card animation runs; each DuelAnim_Update* handler clears
                                     *   it when it ends */
    u8 animKind:7;                  /* bits 1-7: enum DuelAnimKind */
    u8 unk831[3];
    u32 animArg;                    /* +0x834: kinds 1 and 2: struct DuelAnimZoneArg packed in a word; kind 3:
                                     *   card ID; kind 5: address of the sprite animation stream */
    u8 animStep;                    /* +0x838: handler step (kinds 3 and 4: the move step, 0-16) */
    u8 animTimer;                   /* +0x839: frame counter of kinds 1 and 2 */
    s16 animDx;                     /* +0x83A: kind 5: x offset of the stream from the zone position */
    s16 animDy;                     /* +0x83C: kind 5: y offset */
    u16 unk83E;
    struct DuelLoc from;            /* +0x840: kinds 3, 4 and 5: source location */
    struct DuelLoc to;              /* +0x844: kinds 3 and 4: destination */
    struct SprAnim sprAnim;         /* +0x848: the sprite animation stream slot (DuelSprAnim_*), used by kind 5
                                     *   and DuelCmd_ShuffleDeck; frameIndex (+0x852) is back at 0 when it ends */
    u8 unk858[4];
    void (*overlayCallback)(void);  /* +0x85C: optional per-frame callback run by DrawFieldOverlay */
};

/* gDuelScreen.animArg of animation kinds 1 and 2 as bytes (DuelAnim_Request packs
 * player | slot << 8 | isDefense << 16 | extra << 24). */
struct DuelAnimZoneArg {
    u8 player;                      /* +0: zone owner */
    u8 slot;                        /* +1: zone 0-10 */
    u8 isDefense;                   /* +2: defense position before the animation (selects the 90-degree pose) */
    u8 extra;                       /* +3: kind 1: also turn face up; kind 2: isFaceUp before the flip (the
                                     *   gCardFlipTiles row) */
};

/* Field-pixel position of an area (gDuelZonePositions[player][area], indexed like enum DuelArea plus the
 * zone index for the rows: 0-4 monsters, 5-9 spell/trap, 10 field, 11 hand, 12-15 the piles). */
struct DuelZonePos {
    s32 x;                          /* +0: field pixel x (/ 8 = tile column) */
    s32 y;                          /* +4: field pixel y; GetAreaY subtracts gDuelScreen.scroll */
};

/* One step of the 16-step arc of the card swap animation (gCardJumpArc, ROM 0x0819D27C). duel_cursor.c
 * reads the dy column through its own symbol gUnk_0819D280 (= &gCardJumpArc[0].dy): the matched code needs
 * separate x and y table pointers, so both symbols stay. */
struct CardJumpArcEntry {
    s32 dx;                         /* +0: horizontal offset (0..31) */
    s32 dy;                         /* +4: vertical offset (up to 32) */
};

/* gChainListScreen (0x020185B8): the chain-list overlay, which lists the last four links of a chain while
 * it is built (header 0) or resolved (header 1). ChainListScreen_Start arms it, ChainListScreen_Run runs it. */
struct ChainListScreen {
    struct ChainList *chain;        /* +0x0: the chain to list (gChain.links; entry count at +0x140, gChain.linkCount) */
    u8 resolving:1;                 /* +0x4 bit 0: header gChainListHeaders[resolving]: 0 "Chain : Activating",
                                     *   1 "Chain : Resolving" (the per-row Opposite/Yours comes from each entry) */
    u8 state:7;                     /* bits 1-7: enum ChainListScreenState */
    u8 timer;                       /* +0x5: open/close scale (0-16), then the hold counter (to 120) */
    u8 pad6[2];
};

/* One header of the chain-list overlay (gChainListHeaders[2], ROM 0x08198DE4). duel_card_actions.c reads
 * the table as u8[][0x4C] on purpose: with a byte-array row agbcc forms each field address as
 * (symbol + offset) + index * 0x4C, as the ROM does. */
struct ChainListHeader {
    u32 textColor;                  /* +0x00: colour of the header text (drawn at 2, 2) */
    u32 shadowColor;                /* +0x04: colour of the shadow pass (drawn at 3, 3) */
    u32 bgColor;                    /* +0x08: background colour for TextCanvasToTiles (read as a u16) */
    char text[0x40];                /* +0x0C: header string */
};

STATIC_ASSERT(sizeof(struct DuelScreen) == 0x860, DuelScreenSize);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, fieldBgScroll) == 0x002, DuelScreenFieldBgScroll);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, scroll) == 0x004, DuelScreenScroll);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, scrollTo) == 0x006, DuelScreenScrollTo);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, textTiles) == 0x008, DuelScreenTextTiles);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, unk80A) == 0x80A, DuelScreenUnk80A);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, cursorX) == 0x80C, DuelScreenCursorX);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, cursorToY) == 0x820, DuelScreenCursorToY);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, selPlayer) == 0x824, DuelScreenSelPlayer);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, selArea) == 0x828, DuelScreenSelArea);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, selIndex) == 0x82C, DuelScreenSelIndex);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, unk831) == 0x831, DuelScreenUnk831);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, animArg) == 0x834, DuelScreenAnimArg);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, animStep) == 0x838, DuelScreenAnimStep);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, animDx) == 0x83A, DuelScreenAnimDx);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, animDy) == 0x83C, DuelScreenAnimDy);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, from) == 0x840, DuelScreenFrom);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, to) == 0x844, DuelScreenTo);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, sprAnim) == 0x848, DuelScreenSprAnim);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, sprAnim.frameIndex) == 0x852, DuelScreenSprAnimFrame);
STATIC_ASSERT(OFFSET_OF(struct DuelScreen, overlayCallback) == 0x85C, DuelScreenOverlayCallback);
STATIC_ASSERT(sizeof(struct DuelAnimZoneArg) == 0x4, DuelAnimZoneArgSize);
STATIC_ASSERT(sizeof(struct DuelZonePos) == 0x8, DuelZonePosSize);
STATIC_ASSERT(sizeof(struct CardJumpArcEntry) == 0x8, CardJumpArcEntrySize);
STATIC_ASSERT(sizeof(struct ChainListScreen) == 0x8, ChainListScreenSize);
STATIC_ASSERT(OFFSET_OF(struct ChainListScreen, timer) == 0x5, ChainListScreenTimer);
STATIC_ASSERT(sizeof(struct ChainListHeader) == 0x4C, ChainListHeaderSize);
STATIC_ASSERT(OFFSET_OF(struct ChainListHeader, text) == 0xC, ChainListHeaderText);

/* ------------------------------------------------------------------------------------------------------ */
/* Data                                                                                                   */
/* ------------------------------------------------------------------------------------------------------ */

extern struct DuelScreen gDuelScreen;               /* 0x0201CFB0 */
/* Alias symbol of gDuelScreen.textTiles (0x0201CFB8), used by the text-cell code in battle_scene.c; byte
 * +0x800 is gDuelScreen+0x808, the dirty flags. */
extern u8 gDuelTextTiles[0x800];
extern struct ChainListScreen gChainListScreen;     /* 0x020185B8 */

/* 0x081A42A4: field-pixel position of every area of each player (struct DuelZonePos). */
extern const struct DuelZonePos gDuelZonePositions[2][16];

/* ------------------------------------------------------------------------------------------------------ */
/* Functions                                                                                              */
/* ------------------------------------------------------------------------------------------------------ */

/* ---- Screen setup, teardown and fades (duel_field_screen.c, duel_info_bar.c, card_canvas.c) ---- */

/* Builds the duel field screen (BG setup, graphics, board, life points, phase letters, field cells, text
 * cells), leaves it black and sets gDuelScreen.active; callers then run DuelScreen_FadeInStep. */
void DuelScreen_Init(void);
/* Turns BG0-3 and OBJ off, removes the HBlank handler and the VBlank callback; full != 0 also blacks the
 * screen and clears gDuelScreen.active and uiGfxLoaded. */
void DuelScreen_Exit(u16 full);
/* Turns all layers on and steps FadeFromBlack(4); returns 1 once the screen has faded in. */
u16 DuelScreen_FadeInStep(void);
/* Steps FadeToBlack(4); when it completes calls DuelScreen_Exit(0) and returns 1. */
u32 DuelScreen_FadeOutStep(void);
/* The field-only fades below blend BG0-2 and the backdrop (not the text box or sprites), level in
 * gMain.brightness (0-0x1F), `step` per call. This one darkens to black and hides BG0-2; no callers. */
u32 DuelFieldFadeToBlack(s32 step);
/* Darkens the field to BLDY 9 (behind the chain list) and leaves the layers on; returns 1 at level 9. */
u32 DuelFieldDim(s32 step);
/* Turns BG0-2 back on and undoes a darken; returns 1 at level 0 (blend cleared). */
u32 DuelFieldFadeFromBlack(s32 step);
/* Brightens the field to white; returns 1 at full white. */
u32 DuelFieldFadeToWhite(s32 step);
/* Undoes DuelFieldFadeToWhite; returns 1 at level 0 (blend cleared). */
u32 DuelFieldFadeFromWhite(s32 step);
/* VBlank callback: decrements gDuelScreen.fieldBgScroll and writes it to BG0 HOFS and VOFS. */
void DuelScreen_VBlank(void);
/* BG0-3 control: background texture (priority 3), board (256x512), card cells / LP / phases (256x512), text
 * box (priority 0). */
void DuelScreen_InitBgCnt(void);
/* Loads field background `background` (0 plain, 1-14 field magic) as a 32x32-px texture tiled over BG0 and
 * stores it in gDuelScreen.fieldBackground. */
void DuelScreen_LoadFieldBackground(u16 background);
/* Loads the duel OBJ palettes and tiles (hand cursor, mini cards, button icons, labels) with 1D mapping and
 * sets gDuelScreen.uiGfxLoaded. */
void LoadDuelUiGfx(void);
/* Loads the system font and the duel BG palettes and tiles (field graphics, LP digits, phase letters, text
 * box frame) into char block 1. */
void LoadDuelBgGfx(void);
/* Inverse of LoadDuelUiGfx: clears uiGfxLoaded, selects 2D OBJ mapping and clears OBJ VRAM for the 8bpp
 * card-image canvas. */
void UnloadDuelUiGfx(void);

/* ---- Scrolling and the per-frame update (duel_field_view.c, card_menu_input.c) ---- */

/* Starts a 4-step scroll interpolation from the current scroll to `target`. */
void DuelScreen_StartScroll(u32 target);
/* Scrolls to gDuelZoneScrollTargets[player][area] (20, 80 or 140) unless that is already the target. */
void DuelScreen_ScrollToZone(u32 player, u32 area);
/* Per-frame field update: uploads dirty text cells and text box tiles, steps the scroll and the cursor,
 * draws the cursor, runs DuelAnim_Update; returns 1 while the scroll, the cursor or an animation moves. */
u32 DuelScreen_Update(void);
/* The human player's field input while a phase waits: runs the card menu while it is open or confirmed,
 * else moves the free cursor and opens the menu (or the card list of a pile) on A. Returns 1 while the
 * menu is open or was opened this frame; callers treat B as "end this phase" only when it returns 0. */
int DuelScreen_HandleInput(void);

/* ---- Field cursor (duel_field_view.c, duel_cursor.c, duel_prompt_handlers.c, battle_scene.c) ---- */

/* Starts moving the cursor from its current position to (x, y) over 4 frames. */
void DuelCursor_MoveTo(u32 x, u32 y);
/* Stores the selection (normalising area 0 with index 5-9 or 10 to the spell row or field zone), moves the
 * cursor there and scrolls to it. Accepts (area, index) or (0, zone 0-10). */
void DuelCursor_Select(s32 player, s32 area, s32 index);
/* Re-applies the stored selection: DuelCursor_Select(selPlayer, selArea, selIndex). */
void DuelCursor_Refresh(void);
/* 1 if (player, area, index) matches `mask` (16 bits per player, player 1 << 16: 0x1 hand, 0x2 face-down
 * spell/trap or field, 0x4 face-up Magic or field, 0x8 face-up Trap, 0x10/0x20 face-down/face-up monster,
 * 0x40/0x80 attack/defense monster). Callers declare a u16 result. */
int DuelCursor_IsValidTarget(int player, int area, int index, u32 mask);
/* Steps the selection right (CURSOR_DIR_RIGHT) or left until DuelCursor_IsValidTarget accepts a slot, which
 * it stores through the pointers; returns 0 if the search comes back to the start. */
u16 DuelCursor_FindTargetHorizontal(u16 direction, int *player, int *area, int *index, u32 mask);
/* As DuelCursor_FindTargetHorizontal, walking the rows of both players for Up / Down (enum CursorDirection). */
u16 DuelCursor_FindTarget(u16 direction, int *player, int *area, int *index, u32 mask);
/* One frame of the masked cursor of effect prompts: it stops only on slots that match `mask`. Returns 1 on A
 * (the pick is in gDuelScreen.sel*; callers re-check it, since A also returns 1 when nothing matches). */
int DuelCursor_PickTarget(u32 mask);
/* One frame of the free cursor (Main Phase browsing), which also reaches the four piles; returns 1 on A. */
int DuelCursor_PickAny(void);
/* Card ID under the cursor: the zone card for areas 0/5/10, the hand card for area 11, else 0. */
u32 DuelCursor_GetCardId(void);

/* ---- Card animations (duel_field_view.c, duel_card_anim.c) ---- */

/* Requests animation `kind` (enum DuelAnimKind, 1 or 2) with animArg = arg (struct DuelAnimZoneArg). */
void DuelAnim_Request(u16 kind, u32 arg);
/* Requests DUEL_ANIM_MOVE_CARD: card cardId moves from `from` to `to`. */
void DuelAnim_MoveCard(u16 cardId, struct DuelLoc *from, struct DuelLoc *to);
/* Requests DUEL_ANIM_SWAP_CARDS: two face-down cards swap places between `a` and `b`. */
void DuelAnim_SwapCards(struct DuelLoc *a, struct DuelLoc *b);
/* Requests DUEL_ANIM_ZONE_EFFECT: plays sprite animation stream `anim` (an address) at the position of
 * `loc` plus (dx, dy) (explosion, negate, tribute whirlwind, smoke puff). */
void DuelAnim_PlayZoneEffect(struct DuelLoc *loc, u32 anim, u32 dx, u32 dy);
/* Runs the handler of the current animation; returns 1 if one ran this frame. */
u16 DuelAnim_Update(void);
/* Kind 1: rotates the card a quarter turn over 10 frames, flipping it too when DuelAnimZoneArg.extra is set. */
void DuelAnim_UpdateChangePosition(void);
/* Kind 2: 24 frames of gCardFlipTiles, kept at 90 degrees for a card in defense position. */
void DuelAnim_UpdateFlip(void);
/* Kind 3: moves the card sprite over 16 eased steps (7 frames while B is held or gDuelScreen.fast is set). */
void DuelAnim_UpdateMoveCard(void);
/* Kind 4: two card backs move in opposite directions over 16 eased steps. */
void DuelAnim_UpdateSwapCards(void);
/* Kind 5: plays the stream in animArg at the zone, then reloads the duel UI graphics it overwrote. */
void DuelAnim_UpdateZoneEffect(void);

/* The sprite animation stream slot gDuelScreen.sprAnim (sprite.h SprAnim*). */
/* Starts stream `anim` (an address) and clears uiGfxLoaded: the stream overwrites OBJ palette 15 and the OBJ
 * tiles from tile 1. */
void DuelSprAnim_Load(u32 anim);
/* Back to frame 0. */
void DuelSprAnim_Rewind(void);
/* Draws the current frame with each piece at its own position + (x, y); advance != 0 steps to the next. */
void DuelSprAnim_DrawAt(s16 x, s16 y, u16 advance);
/* Draws the current frame with every piece at (x, y); advance != 0 steps to the next. */
void DuelSprAnim_Draw(s16 x, s16 y, u16 advance);
/* As DuelSprAnim_Draw with a packed position (y << 16 | x) and horizontal flip = flip & 1. */
void DuelSprAnim_DrawFlip(u32 yx, u16 advance, u16 flip);

/* ---- Field cells, markers and hand cards (duel_field_screen.c) ---- */

/* Zeroes the 4x4-tile block (one 32x32 field cell) at tile (x, y) of the BG2 card layer. */
void ClearTileBlock4x4(u16 x, u16 y);
/* Fills the 4x4-tile block at tile (x, y) of the BG2 card layer with 16 consecutive map entries from
 * firstTile (palette bits included). */
void FillTileBlock4x4(u32 x, u32 y, u16 firstTile);
/* Redraws field zone (player, zone 0-10): empty, card back, or the card's icon, sideways in defense
 * position; an empty monster zone held for a banished monster gets FIELD_TILE_HELD_ZONE_MARK. */
void DrawZoneTiles(u32 player, u32 zone);
/* Clears the cell at gDuelZonePositions[player][area] (a zone 0-10 or a pile area 12-15). */
void ClearZoneTiles(u32 player, u32 area);
/* Redraws one area: the zones of rows 0/5/10, the deck and fusion-deck piles (thick above 5 cards), the top
 * card of the graveyard and of the banished pile. */
void DrawAreaTiles(u32 player, u32 area, u32 index);
/* DrawAreaTiles for every area of both players. */
void DrawAllAreaTiles(void);
/* Two animated 16x16 marker sprites on the cells of locA and locB (DUEL_LOC form), marker tile `tile`
 * (enum ZoneMarkerTile). */
void DrawLinkMarkerPair(u16 locA, u16 locB, u16 tile);
/* Marks the zone links of the selected zone (only a face-up or own card shows its links) and a zone held
 * for a banished monster. */
void DrawZoneLinkMarkers(u32 player, u32 zone, u32 area);
/* Per frame, while uiGfxLoaded and active: link markers of the card under the cursor, the can-attack
 * markers of the Battle Phase, then gDuelScreen.overlayCallback. */
void DrawFieldOverlay(void);
/* Per frame, while uiGfxLoaded and active: the hand cards of each player whose hand row is on screen (the
 * opponent's face down unless IsHandRevealed), except the card the open card menu shows. */
void DrawHandCards(void);
/* Empty in the retail ROM; called when the link partner's "Just a moment" pause starts. */
void LinkWaitStart_Nop(void);
/* Empty in the retail ROM; called when the pause ends (LINKMSG_INTERRUPT_END). */
void LinkWaitEnd_Nop(void);
/* Draws the blinking "Wait..." sprite while uiGfxLoaded and active. */
void DrawLinkWaitIndicator(void);

/* ---- Positions (card_canvas.c) ---- */

/* enum DuelArea of a zone index: 5-9 spell/trap row, 10 field zone, anything else the monster row. */
int GetZoneArea(int zone);
/* Field x of hand card `index` of `count` (32 px apart, squeezed into 160 px above 5 cards). */
s32 GetHandCardX(u32 player, u32 index, u32 count);
/* Field x of an area slot: gDuelZonePositions[player][area + index].x, or GetHandCardX for the hand. */
s32 GetAreaX(int player, int area, int index);
/* Screen y of an area (field y - scroll); for area 0 the row comes from GetZoneArea(index), so a zone index
 * 0-10 works there. */
s32 GetAreaY(u32 player, int area, int index);

/* ---- Info bar: text cells and card info (battle_scene.c, duel_info_bar.c, turn_order_steps.c) ---- */

/* Points the 64 BG3 map entries of the info bar (rows 18-19) at the text cells (tiles 0x28E-0x2CD,
 * palette 0). */
void TextCellsResetMap(void);
/* Fills every text cell with a blank glyph and marks them for upload and map reset. */
void TextCellsClear(void);
/* Draws a string of 2-byte (Shift-JIS, stored big-endian) characters, one per cell from firstCell. */
void TextCellsPutSjisString(int firstCell, u16 *str, u32 colour);
/* Draws a byte string with the 8x8 font, one character per cell from firstCell. */
void TextCellsPutString(int firstCell, const u8 *str, u32 colour);
/* Prints value right-aligned in cells firstCell..firstCell+digits-1 without leading zeros. It returns
 * before setting the dirty bits when the value is shorter than `digits`; callers rely on an earlier
 * TextCellsPutString or TextCellsClear. */
void TextCellsPutNumber(int firstCell, int value, u32 colour, int digits);
/* Copies BG tile `tile` of char block 1 into text cell `cell`. */
void TextCellsCopyBgTile(int cell, u16 tile);
/* Loads a 16x16 image-pack icon into cells cell, cell + 1 and the two below, with its palette in BG
 * palette palSlot; clears textMapReset so the upload keeps the icon's map palettes. */
void TextCellsLoadIcon(int cell, u16 palSlot, u16 *iconPack);
/* Draws a NUL-terminated string at (x, y) of the text canvas with a 1-pixel drop shadow; 2-byte
 * characters (advance `size`) in Shift-JIS mode, else bytes (advance size / 2). */
void TextDrawShadowedString(s32 x, s32 y, const u16 *str, s32 size);
/* Renders the card name centred in the info bar. */
void DuelInfo_DrawCardNameCentered(u16 cardId);
/* Card name, then for a monster with showStats the base ATK/DEF and the level. */
void DuelInfo_DrawCard(u16 cardId, u16 showStats);
/* Info of a spell/trap or field zone card: name, the turn counter of Cocoon of Evolution / Swords of
 * Revealing Light, the declared type or attribute icon, or the stats of a monster card kept there. */
void DuelInfo_DrawSpellZone(u16 cardId, u16 showStats, int player, int zone);
/* Renders `label` and, when labelLen > 0, `value` right after it. */
void DuelInfo_DrawLabelNumber(int glyphWidth, const u8 *label, int value, int labelLen);
/* Info of a monster zone: name, effective ATK/DEF (colour 6 when they differ from the printed values),
 * level, type and attribute icons. */
void DuelInfo_DrawMonsterZone(int player, int zone);
/* Redraws the info bar for the card or pile under the cursor (opponent cards only when face up, the
 * opponent's deck sizes hidden). */
void DuelScreen_DrawCursorInfo(void);

/* ---- Life points, phase letters and numbers (duel_info_bar.c, duel_cmd_moves.c) ---- */

/* Prints value right-aligned in 5 map cells from `cell` of gMain.bgMapBuffer[screenBlock], digit set
 * colorSet. */
void DrawBgNumber(u32 screenBlock, u32 cell, u32 colorSet, int value);
/* Draws a player's life points in the LP box on BG2. */
void DrawLifePoints(int player, int lifePoints);
/* Draws both players' D S M B M E phase letters, highlighting `phase` of `turnPlayer`. */
void DrawPhaseIndicator(int turnPlayer, int phase);
/* Draws a signed life-point change as 16x16 digit sprites right-aligned at x + 0x50 (a 0 has no sign). */
void DrawLpChangeAmount(u32 x, u32 y, s32 value, u32 colorSet);

/* ---- Card-image canvas and card icons (duel_field_screen.c, card_canvas.c, duel_card_actions.c) ---- */
/* The card image (104x144) is drawn into OBJ VRAM in 2D mapping as 8bpp tiles: LoadCardFrame +
 * LoadCardPicture + DrawCardInfo, after UnloadDuelUiGfx. */

/* Writes one 8bpp pixel of the canvas (read-modify-write of its word). */
void PlotCardImagePixel(u32 x, u32 y, u32 color);
/* Draws an 8x8 4bpp tile at yx (y << 16 | x) with colours pal + palBase (colour 0 transparent); nothing when
 * gfx or pal is NULL. */
void DrawCardImageTile(u32 yx, u32 palBase, const u16 *gfx, const void *pal);
/* Draws a 16x16 4bpp icon (four tiles) with DrawCardImageTile. */
void DrawCardImageIcon16(u32 yx, u16 palBase, const u16 *gfx, const void *pal);
/* Draws the card frame for the card's type or kind around the picture window. */
void LoadCardFrame(u16 cardId);
/* Unpacks the card's 72x80 6bpp picture and its 64-colour palette into the frame window. */
void LoadCardPicture(u16 cardId);
/* Draws the overlay icons: attribute, level stars, ATK/DEF digits, or the spell/trap subtype. */
void DrawCardInfo(u16 cardId);
/* OBJ tile of the card's 32x32 mini-card sheet loaded by LoadDuelUiGfx (by kind or Magic/Trap). */
int GetCardIconObjTile(u16 cardId);
/* BG tile of the same mini card loaded with the text box graphics (a Ticket returns its kind, 9). */
int GetCardIconBgTile(u16 cardId);
/* The card's 0x800-byte icon graphics in its frame colour (4 poses of 32x32 4bpp). */
const u8 *GetCardIconGfx(u16 cardId);

/* ---- Chain-list overlay (link_battle.c, duel_card_actions.c) ---- */

/* Arms the chain-list overlay: chain = (struct ChainList *)list, header resolving = player & 1. The
 * definition takes the chain pointer as a u32. */
void ChainListScreen_Start(u32 list, u16 player);
/* One frame of the overlay (enum ChainListScreenState); returns 1 when it has closed. */
int ChainListScreen_Run(void);
/* Adds the eight 32x16 sprites of the header text across the top of the screen. */
void ChainListScreen_DrawHeader(void);
/* Adds the four chain rows (card icon and seven text sprites each); with `scaled`, each row's y is
 * multiplied by scale / 16 (the open/close animation). The third argument is unused (callers pass -1). */
void ChainListScreen_DrawRows(int scale, u16 scaled, int unused);

#endif /* GUARD_DUEL_SCREEN_H */
