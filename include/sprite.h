#ifndef GUARD_SPRITE_H
#define GUARD_SPRITE_H

/*
 * Sprites (OBJs). The game has four ways to put sprites on screen:
 *   1. direct OAM emitters: AddSprite* / AddAffineSprite* append entries to gMain.oamBuffer (oamCount++);
 *   2. the OAM layer list (struct OamList): a scene allocates entries into 20 depth layers during the frame and
 *      OamListFlush copies them to gMain.oamBuffer in layer order;
 *   3. sprite animation streams (struct SprAnim): ROM streams of tiles and frames, drawn straight into
 *      gMain.oamBuffer;
 *   4. script animations (struct AnimBlock / AnimState / AnimSeq): per-step lists of OAM templates
 *      (struct OamTemplate), ticked once per frame and drawn into an OamList.
 * OBJ affine matrices live in the fourth halfword of every OAM entry (pa, pb, pc, pd of matrix n in entries
 * 4n..4n+3); struct ObjAffine drives one matrix. Defined in sprite, collection, text_render, bitmap_text,
 * gfx_util, link_sio and main.
 *
 * Every prototype is the function's definition as compiled. Some units call these functions through a
 * different local declaration (other widths, fewer or more arguments); they keep that view as a commented
 * local alias prototype (build/readability/proto_mismatches.txt).
 */

#include "global.h"

/* ---- OAM templates and the layer list ---- */

/* An 8-byte OAM template: the attributes of one sprite piece as stored in ROM (animation steps, digit sprites,
 * scene sprite tables). Stride 8, so the template list of a step is indexed like an OAM buffer. */
struct OamTemplate {
    u16 attr0; /* +0x0: Y in bits 0-7, shape/mode bits */
    u16 attr1; /* +0x2: X in bits 0-8, size bits */
    u16 attr2; /* +0x4: tile (format 0: 16-tile-wide sheet coordinates; modes 3/4/8: bits 8-9 pick the
                        sheet quadrant), see OamListAddSpriteGroup */
    u16 pad;   /* +0x6: not read */
};

/* One entry of an OamList; OamListAlloc returns it for the caller to fill. OamListFlush and OamListAddSprite
 * move attr0 and attr1 as one 32-bit word, so their units view the first 4 bytes as a u32. */
struct OamListEntry {
    u16 attr0;    /* +0x0: OAM attribute 0, copied by OamListFlush */
    u16 attr1;    /* +0x2: OAM attribute 1 */
    u16 attr2;    /* +0x4: OAM attribute 2 */
    u16 pad;      /* +0x6: not copied: that halfword of gMain.oamBuffer holds the affine parameters */
    s8 next;      /* +0x8: next entry of the same layer, -1 = end */
    u8 pad2[3];   /* +0x9 */
};

/* OAM layer list (0x618 bytes): 128 entries chained into 20 layers. Layer 0 is flushed first, so it is the
 * frontmost. Embedded at the start of most scene work areas (gSceneWork, gBustupSprites, the coin toss). */
struct OamList {
    s8 head[20];                       /* +0x000: first entry of each layer, -1 = empty */
    struct OamListEntry entries[128];  /* +0x014: allocated in order, entries[count] is the next free one */
    u8 count:7;                        /* +0x614 bits 0-6: entries allocated this frame. Only 7 bits wide, so
                                          the "list full" test in OamListAlloc never fires */
    u8 unused:1;                       /* +0x614 bit 7: kept by OamListClear, never used */
};

/* `mode` argument of OamListAddSpriteGroup: where each template piece is placed. The QUAD modes (format 0 only)
 * take the 128 x 128 sheet quadrant from template attr2 bits 8-9 (0x100 = +16 tiles, 0x200 = +0x200 tiles)
 * instead of sheetX / sheetY. */
enum OamGroupMode {
    OAM_GROUP_TEMPLATE_POS = 0,      /* at the template's own X/Y */
    OAM_GROUP_ABS_POS = 1,           /* every piece at (x, y) */
    OAM_GROUP_REL_POS = 2,           /* template X/Y offset by (x, y) */
    OAM_GROUP_QUAD_TEMPLATE_POS = 3, /* as 0, quadrant from attr2 */
    OAM_GROUP_QUAD_ABS_POS = 4,      /* as 1, quadrant from attr2 */
    OAM_GROUP_QUAD_REL_POS = 8       /* as 2, quadrant from attr2 */
};

/* `format` argument of OamListAddSpriteGroup: how template tile numbers are read. */
enum OamTileFormat {
    OAM_TILES_SHEET16 = 0, /* tile on a 16-tile-wide sheet, converted to the 32-wide 2D OBJ map */
    OAM_TILES_RAW = 1      /* tile used as is, plus sheetX * 16 and sheetY << 9 */
};

/* ---- Script animations ---- */

/* One step of a sprite animation script (ROM). A script is an array of steps ending with frames == 0. */
struct AnimSeq {
    u8 frames;                         /* +0x0: the step shows for frames + 1 ticks; 0 ends the script */
    u8 pieceCount;                     /* +0x1: OAM templates in this step */
    u16 pad;                           /* +0x2 */
    const struct OamTemplate *pieces;  /* +0x4: the step's templates */
};

/* AnimState.active. AnimStateTick advances a state only while it is ANIM_PLAYING. */
enum AnimActive {
    ANIM_FINISHED = 0,   /* script ended: rewound to step 0, still drawn but no longer ticked */
    ANIM_PLAYING = 1,
    ANIM_HIDDEN = 0xFF   /* skipped by AnimBlockDraw */
};

/* Player state of one script animation (0x14 bytes). */
struct AnimState {
    const struct AnimSeq *seq;           /* +0x00: the script */
    const struct OamTemplate *pieces;    /* +0x04: current step's templates (copied from seq[stepIdx]) */
    u16 x;                               /* +0x08: draw position; 0xFFFF after AnimBlockInit, overwritten by
                                            AnimBlockDraw in the positioned modes */
    u16 y;                               /* +0x0A */
    u8 pieceCount;                       /* +0x0C: templates in the current step */
    u8 stepIdx;                          /* +0x0D: current step */
    u8 active;                           /* +0x0E: enum AnimActive; some tests read it as a signed byte
                                            (ldrsb): bustup_runner, destiny_board_scene and turn_order_steps
                                            cast it to s8 */
    u8 timer;                            /* +0x0F: ticks left in the step; counts down through 0 */
    u8 layer;                            /* +0x10: OAM layer, 0x13 - index after AnimBlockInit; AnimBlockDraw
                                            uses its own layer argument instead */
    u8 pad;                              /* +0x11 */
    u8 unk12;                            /* +0x12: written only in gBustup's array (+0x810) */
    u8 pad13;                            /* +0x13 */
};

/* A group of up to 20 script animations: the AnimBlockInit/Tick/Draw argument, embedded in scene work areas
 * (gSceneWork +0x918, gDeckEdit, ...). */
struct AnimBlock {
    struct AnimState anims[20];  /* +0x000: the first `count` are in use */
    u16 count;                   /* +0x190: set by AnimBlockInit */
};

/* ---- Sprite animation streams ---- */

/*
 * A sprite animation stream in ROM (graphics bank A: 0x0868CAC0, 0x0868DB94, 0x0868EC38, 0x08690D0C,
 * 0x08694EA8, 0x0869771C). This struct covers the fixed start; the stream continues with
 *     blockCount x { u16 nTiles; u8 tiles[nTiles][32]; }       4bpp, OBJ 1D order, loaded from tile 1
 *     u16 frameCount;
 *     frameCount x { u16 pieceCount; struct SprAnimPiece pieces[pieceCount]; }
 */
struct SprAnimHeader {
    u16 palette[16];     /* +0x00: loaded to OBJ palette 15 */
    u16 blockCount;      /* +0x20: number of tile blocks */
    u16 blocks[0][2];    /* +0x22: blockCount x {size, unk2}: size = attr1 size bits (0, 0x4000, 0x8000,
                            0xC000); unk2 equals size in all six streams */
};

/* One piece of a stream frame. It takes 6 bytes in the stream, but agbcc pads this struct to 8: step through
 * the stream with a u16 pointer instead of indexing an array of these. */
struct SprAnimPiece {
    u16 block;  /* +0x0: tile block; OBJ tile = block * nTiles + 1 (all blocks of a stream are equal-sized) */
    s16 x;      /* +0x2: screen x; SprAnimDrawFrame adds dx, the ...At variants ignore it */
    s16 y;      /* +0x4: screen y */
};

/* Player state of a sprite animation stream (the instance is gDuelScreen + 0x848). */
struct SprAnim {
    const u8 *base;   /* +0x0: stream start (struct SprAnimHeader) */
    const u8 *cur;    /* +0x4: read pointer: header of the current frame */
    u16 frameCount;   /* +0x8: frames in the stream */
    u16 frameIndex;   /* +0xA: current frame; at frameCount the next draw call rewinds instead of drawing */
    u16 blockCount;   /* +0xC: tile blocks in the stream */
    u16 pieceCount;   /* +0xE: pieces in the frame being drawn (always 1 in the shipped streams) */
};

/* ---- OBJ affine ---- */

/* Rotation/scale record driving one OAM matrix (0x18 bytes); scenes keep 32 of them (gSceneWork + 0x618).
 * ObjAffineInit binds record i to matrix i, ObjAffineApply writes the matrix. */
struct ObjAffine {
    s16 scaleX;     /* +0x00: display scale, 8.8 (0x100 = 1.0): the matrix holds 1 / scale, so 0x200 shows
                       the sprite twice as wide */
    s16 scaleY;     /* +0x02: display scale, 8.8; read sign-extended by ObjAffineApply */
    u16 angle;      /* +0x04: 0x10000 = full turn; the high byte indexes gSineTable */
    u16 pad06;      /* +0x06 */
    s16 *param[4];  /* +0x08: pa, pb, pc, pd of the record's OAM matrix inside gMain.oamBuffer */
};

/* ---- Direct OAM emitters ----
 * `shape` is an enum SpriteShape: attr0 shape bits (0x4000 wide, 0x8000 tall) | attr1 size bits >> 8 (0x40,
 * 0x80, 0xC0). `yx` packs y << 16 | x. These append at gMain.oamBuffer[oamCount++] and do nothing once 128
 * entries are used (affine ones also once 32 matrices are used). */
enum SpriteShape {
    SPRITE_SHAPE_8x8 = 0x0000,
    SPRITE_SHAPE_16x16 = 0x0040,
    SPRITE_SHAPE_32x32 = 0x0080,
    SPRITE_SHAPE_64x64 = 0x00C0,
    SPRITE_SHAPE_16x8 = 0x4000,
    SPRITE_SHAPE_32x8 = 0x4040,
    SPRITE_SHAPE_32x16 = 0x4080,
    SPRITE_SHAPE_64x32 = 0x40C0,
    SPRITE_SHAPE_8x16 = 0x8000,
    SPRITE_SHAPE_8x32 = 0x8040,
    SPRITE_SHAPE_16x32 = 0x8080,
    SPRITE_SHAPE_32x64 = 0x80C0
};

/* 4bpp sprite; attr2 = tile | priority << 10 | palette << 12. */
void AddSprite(u32 yx, u16 shape, u16 attr2);
/* 4bpp sprite taking x and y separately. */
void AddSpriteXY(u16 x, s16 y, u16 shape, u16 attr2);
/* 4bpp semi-transparent sprite (attr0 |= 0x400, OBJ mode 1). */
void AddSpriteAlpha(u32 yx, u16 shape, u16 attr2);
/* 4bpp sprite with `flip` ORed into attr1 (0x1000 hflip, 0x2000 vflip). */
void AddSpriteFlip(u32 yx, u16 shape, u16 attr2, u16 flip);
/* 256-colour sprite: attr0 |= 0x2000, attr2 = tile << 1 (tile counted in 64-byte units). */
void AddSprite8bpp(u32 yx, u16 shape, u16 tile);
/* 256-colour sprite with `flip` ORed into attr1. */
void AddSprite8bppFlip(u32 yx, u16 shape, u16 tile, u16 flip);
/* 256-colour semi-transparent sprite (attr0 |= 0x2400). */
void AddSprite8bppAlpha(u32 yx, u16 shape, u16 tile);
/* Double-size rotate/scale 4bpp sprite on the next free matrix; scaleAngle = scale << 16 | angle (128 steps
 * per turn). (x, y) stays the top-left of the unrotated sprite, which turns about its centre. */
void AddAffineSprite(u32 yx, u16 shape, u16 attr2, u32 scaleAngle);
/* As AddAffineSprite for a 256-colour semi-transparent sprite (attr0 |= 0x2700). */
void AddAffineSprite8bppAlpha(u32 yx, u16 shape, u16 tile, u32 scaleAngle);
/* DMA-clears all OBJ palettes and the first 0x80 bytes of OBJ VRAM (4bpp tiles 0-3). */
void ClearObjPalettesAndFirstTiles(void);

/* ---- OAM affine matrices (matrix n = affineParam of gMain.oamBuffer[4n..4n+3]) ---- */

/* Matrix = uniform scale (pa = pd = scale, pb = pc = 0; 8.8). */
void SetOamAffineScale(u16 matrix, u16 scale);
/* Matrix = rotation by angle (128 steps per turn) times scale (8.8). */
void SetOamAffineRotScale(u16 matrix, u16 scale, u16 angle);
/* Matrix: pa = pd = scale, pb = shear, pc = -shear. */
void SetOamAffineShear(u16 matrix, u16 scale, u16 shear);
/* Unused. Rotation matrix from angleScale = scale nibble << 12 | angle (7 bits): nibble n <= 7 divides by
 * n + 1 (sprite n + 1 times larger), n >= 8 multiplies by n - 8 (n = 8 gives a zero matrix). */
void SetOamMatrixPacked(u16 group, u16 angleScale);
/* Binds the 32 records of `affines` to the 32 OAM matrices and resets them to scale 1.0, angle 0. */
void ObjAffineInit(struct ObjAffine *affines);
/* Writes the record's matrix: pa = cos / scaleX, pb = sin / scaleX, pc = -sin / scaleY, pd = cos / scaleY. */
void ObjAffineApply(struct ObjAffine *affine);

/* ---- OAM layer list ---- */

/* Empties every layer (heads = -1, count = 0); `list` is a struct OamList. */
void OamListClear(u8 *list);
/* Takes entries[count], links it at the head of `layer` and returns it. No full check (see OamList.count). */
struct OamListEntry *OamListAlloc(u8 layer, struct OamList *list);
/* Links the existing entry idx at the head of `layer`; count is not touched. */
void OamListLinkEntry(u8 idx, u8 layer, struct OamList *list);
/* Copies attr0-attr2 of every entry, layer 0 first, to consecutive gMain.oamBuffer slots (the affine
 * halfwords survive); returns the number copied. */
u8 OamListFlush(struct OamList *list);
/* Allocates an entry (`list` is a struct OamList) and copies attr0-attr2 of template `tmpl` unchanged. */
u16 *OamListAddTemplate(u16 *tmpl, u8 layer, u32 unused2, u32 unused3, void *list);
/* As OamListAddTemplate, with Y = y and X = x; attr2 is left to the caller. No NULL check. */
u16 *OamListAddTemplateAt(u16 *tmpl, u8 layer, s16 x, s16 y, u32 unused4, u32 unused5, u32 unused6, void *list);
/* As OamListAddTemplate, with (dx, dy) added to the template's X (9 bits) and Y (8 bits); attr2 left alone. */
u16 *OamListAddTemplateOffset(u16 *tmpl, u8 layer, s16 dx, s16 dy, u32 unused4, u32 unused5, u32 unused6,
                              void *list);
/* Adds `count` templates to `layer`, placed by `mode` (enum OamGroupMode), tiles by `format` (enum
 * OamTileFormat) and moved to sheet quadrant sheetX (+16 tiles) / sheetY (+0x200 tiles); attr0 |= attr0Flags.
 * Returns the last entry (undefined when count is 0 or the mode is unsupported). */
u16 *OamListAddSpriteGroup(u16 *tmpls, u8 layer, u8 count, u16 x, u16 y, u8 mode, u8 priority, u8 sheetX,
                           u8 sheetY, u8 format, u16 attr0Flags, void *list);
/* Allocates an entry for a width x height px sprite (8/16/32/64; 8x64, 16x64, 64x8 and 64x16 hang in
 * while (1)). bpp 8 sets the 256-colour bit, attr1Bits go to attr1 bits 9-15. The 9th argument is never read. */
struct OamListEntry *OamListAddSprite(u8 layer, u16 tile, u16 x, int y, u8 width, u8 height, u8 bpp,
                                      u8 palette, u32 unused, u16 attr0Flags, u8 attr1Bits, u8 priority,
                                      struct OamList *list);

/* `mode` argument of DrawNumberSprites (compare NumberMode in text.h). */
enum NumberSpriteMode {
    NUMSPRITE_ZERO_PAD = 0,          /* exactly numDigits digits, zero padded */
    NUMSPRITE_NO_LEADING_ZEROS = 1   /* stops at the leading zeros (a single 0 for value 0) */
};

/* Draws `value` right to left from x, one digit template (digitTemplates + d * 8 bytes) per digit, `spacing`
 * px apart, into layer 0 of `list` (a struct OamList *). */
void DrawNumberSprites(u16 value, u8 numDigits, u8 mode, u16 x, u16 y, u8 *digitTemplates, u32 unused,
                       u8 spacing, u8 sheetX, u8 sheetY, u8 priority, u32 list);

/* ---- Script animations ---- */

/* Starts one state per script of the NULL-terminated `scripts` list (at least one is read) in `block`
 * (a struct AnimBlock); returns the count. */
u8 AnimBlockInit(struct AnimSeq **scripts, u8 *block);
/* Steps the script: a step lasts frames + 1 ticks; a frames == 0 step rewinds to step 0 and finishes. */
void AnimStateTick(struct AnimState *anim);
/* AnimStateTick on every state of `block` (a struct AnimBlock). */
void AnimBlockTick(u8 *block);
/* Draws every non-hidden state of `block` (a struct AnimBlock) through OamListAddSpriteGroup into `oam`
 * (a struct OamList *). The positioned modes 1, 2, 4 and 8 first store (x, y) into each state. The int
 * return value is garbage (the ROM epilogue pops r1); callers ignore it. */
int AnimBlockDraw(u8 *block, u8 layer, u8 priority, u8 tileOffset, u8 palette, u8 format, u8 mode, u16 x,
                  u16 y, u32 oam);
/* Copies the state's current templates unchanged into `layer` of `oam` (a struct OamList *) with
 * OamListAddTemplate; tileOffset and palette are passed on but not used. */
void AnimStateDrawRaw(struct AnimState *anim, u8 layer, u32 unusedPriority, u8 tileOffset, u8 palette, u32 oam);

/* ---- Sprite animation streams ---- */

/* Starts a stream: 1D OBJ mapping on, palette to OBJ palette 15, tile blocks to OBJ VRAM from tile 1, then
 * SprAnimRewind. */
void SprAnimLoad(u8 *stream, struct SprAnim *anim);
/* Skips the header and tile blocks, reads frameCount and points cur at frame 0. */
void SprAnimRewind(struct SprAnim *anim);
/* Draws the current frame into gMain.oamBuffer[0..pieceCount - 1] at each piece's position + (dx, dy) and
 * steps to the next frame if `advance`. It does not bump oamCount, so it must run before any AddSprite of the
 * same frame. */
void SprAnimDrawFrame(u16 dx, u16 dy, struct SprAnim *anim, u16 advance);
/* As SprAnimDrawFrame, but every piece goes to (x, y). */
void SprAnimDrawFrameAt(u16 x, u16 y, struct SprAnim *anim, u16 advance);
/* As SprAnimDrawFrameAt with a packed position (y << 16 | x) and attr1 hflip = hflip & 1. */
void SprAnimDrawFrameAtFlip(u32 yx, struct SprAnim *anim, u16 advance, u16 hflip);

/* ---- ROM data ---- */

/* 16-colour OBJ palette for gHandCursorGfx, loaded to OBJ palette 0. */
extern const u8 gHandCursorPal[];
/* 4bpp OBJ sheet of pointing hands and a magnifier; its first 0x200 bytes (a 32 x 32 hand) are the
 * deck-choice cursor at OBJ tile 0x100. */
extern const u8 gHandCursorGfx[];

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char sprite_h_check_tmpl[sizeof(struct OamTemplate) == 0x8 ? 1 : -1];
typedef char sprite_h_check_entry[sizeof(struct OamListEntry) == 0xC ? 1 : -1];
typedef char sprite_h_check_entry_next[(u32)&((struct OamListEntry *)0)->next == 0x8 ? 1 : -1];
typedef char sprite_h_check_list[sizeof(struct OamList) == 0x618 ? 1 : -1];
typedef char sprite_h_check_list_entries[(u32)&((struct OamList *)0)->entries == 0x14 ? 1 : -1];
typedef char sprite_h_check_seq[sizeof(struct AnimSeq) == 0x8 ? 1 : -1];
typedef char sprite_h_check_state[sizeof(struct AnimState) == 0x14 ? 1 : -1];
typedef char sprite_h_check_state_active[(u32)&((struct AnimState *)0)->active == 0xE ? 1 : -1];
typedef char sprite_h_check_state_unk12[(u32)&((struct AnimState *)0)->unk12 == 0x12 ? 1 : -1];
typedef char sprite_h_check_block[sizeof(struct AnimBlock) == 0x194 ? 1 : -1];
typedef char sprite_h_check_block_count[(u32)&((struct AnimBlock *)0)->count == 0x190 ? 1 : -1];
typedef char sprite_h_check_hdr_blocks[(u32)&((struct SprAnimHeader *)0)->blocks == 0x22 ? 1 : -1];
typedef char sprite_h_check_piece_y[(u32)&((struct SprAnimPiece *)0)->y == 0x4 ? 1 : -1];
typedef char sprite_h_check_spranim[sizeof(struct SprAnim) == 0x10 ? 1 : -1];
typedef char sprite_h_check_affine[sizeof(struct ObjAffine) == 0x18 ? 1 : -1];
typedef char sprite_h_check_affine_param[(u32)&((struct ObjAffine *)0)->param == 0x8 ? 1 : -1];

#endif /* GUARD_SPRITE_H */
