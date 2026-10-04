#ifndef GUARD_BUSTUP_H
#define GUARD_BUSTUP_H

/*
 * Bust-up dialogue scenes (units bustup_scene and bustup_runner): a character portrait (Mode-4 bitmap with
 * animated eyes and mouth) above a dialogue box that prints `$`-coded text from gDialogueTable.
 *
 * Usage: StartDialogue(eventId), then call CB_Bustup (or CB_AutoBustup) every frame until it returns 1. The
 * runner steps through gBustupSteps[gMain.seqIndex1] (enum BustupStep). The text codes are documented on
 * Bustup_UpdateTextBox. See wiki/game/text-system.md and wiki/data/scene-sets.md.
 */

#include "global.h"
#include "palette.h"
#include "sprite.h"
#include "util.h"

#define DIALOGUE_COUNT 490          /* records in gDialogueTable before the terminator */

/* Byte position, one entry of the cursor trail (also gBustupSlotPositions). */
struct BytePos {
    u8 x;
    u8 y;
    u8 pad[2];
};

/* One record of the dialogue table gDialogueTable (0x0813ADF4, DIALOGUE_COUNT records + terminator).
 * bustup_runner also reads the text through gDialogueTableText = gDialogueTable[0].text (index * 0x304). */
struct DialogueEntry {
    u16 eventId;                /* +0x000: duelist * 1000 + n, or < 1000 for story/system events */
    u16 speakerId;              /* +0x002: portrait character id when the record opens */
    char text[0x300];           /* +0x004: `$`-coded dialogue text */
};

/* Character name record of gDuelists (0x08139F64, 28 records; entry 0 is " "). See wiki/data/duelist-table.md. */
struct Duelist {
    u32 id;                     /* +0x00: character id (enum DuelistId for 1-24) */
    char name[0x40];            /* +0x04: full name, e.g. "Tea Gardner" */
    char shortName[0x40];       /* +0x44: short name, e.g. "Tea" */
};

/* Scene set: one speaker's scene (gSceneSets, 0x081976A0, 31 records). See wiki/data/scene-sets.md. */
struct SceneSet {
    const u16 *bitmapLz;        /* +0x00: LZSS 240x96 (240x80 for sets 0, 1, 3, 4) Mode-4 bitmap */
    const u16 *bgPal;           /* +0x04: 256 colours, entries 16-255 are loaded */
    const u16 *objPal;          /* +0x08: OBJ palette, or NULL */
    const u16 *objTilesLz;      /* +0x0C: LZSS 0x2000-byte OBJ tile sheet, or NULL */
    const void *const *anim;    /* +0x10: NULL-terminated animation track list (eyes, mouth), or NULL */
};

/* The dialogue box and text printer state (gBustup.textBox, 0x0201478C); the `tb` of the functions below. */
struct BustupTextBox {
    void *bitmap;               /* +0x00: decoded scene bitmap (gBustupBitmapBuffer) */
    const u16 *bgPal;           /* +0x04: scene BG palette */
    const u16 *objPal;          /* +0x08: scene OBJ palette, or NULL */
    void *boxBitmap;            /* +0x0C: decoded box bitmap (gBustupBoxBitmap) */
    const void *const *anim;    /* +0x10: scene set's animation track list */
    const u8 *text;             /* +0x14: current script position */
    u8 delay;                   /* +0x18: frames until the next glyph (2 = one glyph every 3 frames) */
    s8 state;                   /* +0x19: enum BustupTextState */
    u8 pad1A[2];
    u8 col;                     /* +0x1C: cursor column (6-px units, 30 per line) */
    u8 row;                     /* +0x1D: cursor row (13-px lines, 0-3) */
    u8 pad1E[2];
    u8 page;                    /* +0x20: Mode-4 page being written and displayed (DISPCNT bit 4) */
    u8 pad21;
    u16 boxDirty;               /* +0x22: 1 = clear the hidden box and show `page` next frame */
    u16 textColor;              /* +0x24: glyph colour (enum TextColor; `$rX` sets 0-15) */
    u8 pad26[2];
    struct Line cursor;         /* +0x28: cursor sparkle line stepper (also gBustupCursor) */
    u8 trailHead;               /* +0x3C: write index of trail[] (0xFF after init) */
    u8 pad3D[3];
    struct BytePos trail[30];   /* +0x40: cursor position history; x = 0xFF is empty */
    u8 nameIdx;                 /* +0xB8: read index into name[] */
    u8 name[0x43];              /* +0xB9: name being inserted by $q / $Q / $i */
};

/* Bust-up module state at 0x02013DE0 (0x1380 bytes), cleared by Bustup_InitState. */
struct BustupState {
    u8 unk0[0x810];             /* +0x0000: only touched by the clear */
    struct AnimState unk810[20]; /* +0x0810: only unk12 of each is set to 0xFF (Bustup_ResetBlink), and nothing
                                 * reads them; Bustup_ResetBlink matches only with this array type */
    u8 pad9A0[4];
    u8 animCount:3;             /* +0x09A4: animation tracks built by the scene loaders (AnimBlockInit) */
    u8 pad9A5;
    u8 scriptFlag;              /* +0x09A6: cleared by `$h` and Bustup_InitTextBox, set by `$k`; never read */
    u8 blinkIndex:5;            /* +0x09A7: index into gBlinkIntervals */
    u16 blinkTimer;             /* +0x09A8: frames to the next blink; also the cursor trail's wobble phase */
    u8 pad9AA[2];
    struct BustupTextBox textBox; /* +0x09AC */
    struct OamList sprites;     /* +0x0AA8: layered sprite list (also gBustupSprites) */
    struct AnimState anims[20]; /* +0x10C0: portrait animation tracks, 0 = eyes, 1 = mouth (gBustupMouthAnim) */
    u16 animsCount;             /* +0x1250: track count written by AnimBlockInit at block + 0x190 */
    u8 pad1252[0x12E8 - 0x1252];
    u8 unk12E8;                 /* +0x12E8: cleared by Bustup_InitState only */
    u8 selectSlot;              /* +0x12E9: 0-4, slot of the unused opponent preview (gBustupSlotPositions) */
    u8 selectPage;              /* +0x12EA: row of gBustupOpponentIds (page * 5 + slot) in that preview */
    u8 speaker;                 /* +0x12EB: current speaker / scene set character id (gMain.speaker, `$b`) */
    struct Fade fade;           /* +0x12EC: scene fade; param = gMain.seqIndex1 increment (also gBustupFade) */
    u8 pad12F4[5];
    u8 unk12F9;                 /* +0x12F9: set when the unused preview step ends; purpose unknown */
    u8 unk12FA;                 /* +0x12FA: 5 at init (hypothesis: slot count) */
    u8 pad12FB[0x136C - 0x12FB];
    u16 dialogueIndex;          /* +0x136C: current gDialogueTable record (copy of gMain.dialogueIndex) */
    u8 unk136E;                 /* +0x136E: copy of gMain.flag4884_3; never read */
    u8 slideDir;                /* +0x136F: BG2 slide direction in the unused preview (hypothesis) */
    u8 unk1370;                 /* +0x1370: cleared only */
    u8 pad1371[3];
    struct Timer timer0;        /* +0x1374 */
    struct Timer timer1;        /* +0x1378 */
    u8 autoNext:1;              /* +0x137C bit 0: auto mode reached the end of the text, go to the next record */
    u8 autoAdvance:1;           /* +0x137C bit 1: auto-advance mode (set every frame by CB_AutoBustup) */
    u8 pad137D[3];
};

typedef char bustup_h_check_pos[sizeof(struct BytePos) == 0x4 ? 1 : -1];
typedef char bustup_h_check_entry[sizeof(struct DialogueEntry) == 0x304 ? 1 : -1];
typedef char bustup_h_check_duelist[sizeof(struct Duelist) == 0x84 ? 1 : -1];
typedef char bustup_h_check_set[sizeof(struct SceneSet) == 0x14 ? 1 : -1];
typedef char bustup_h_check_tb[sizeof(struct BustupTextBox) == 0xFC ? 1 : -1];
typedef char bustup_h_check_tb_cursor[(u32)&((struct BustupTextBox *)0)->cursor == 0x28 ? 1 : -1];
typedef char bustup_h_check_tb_trail[(u32)&((struct BustupTextBox *)0)->trail == 0x40 ? 1 : -1];
typedef char bustup_h_check_tb_name[(u32)&((struct BustupTextBox *)0)->name == 0xB9 ? 1 : -1];
typedef char bustup_h_check_size[sizeof(struct BustupState) == 0x1380 ? 1 : -1];
typedef char bustup_h_check_timer[(u32)&((struct BustupState *)0)->blinkTimer == 0x9A8 ? 1 : -1];
typedef char bustup_h_check_textbox[(u32)&((struct BustupState *)0)->textBox == 0x9AC ? 1 : -1];
typedef char bustup_h_check_sprites[(u32)&((struct BustupState *)0)->sprites == 0xAA8 ? 1 : -1];
typedef char bustup_h_check_anims[(u32)&((struct BustupState *)0)->anims == 0x10C0 ? 1 : -1];
typedef char bustup_h_check_count[(u32)&((struct BustupState *)0)->animsCount == 0x1250 ? 1 : -1];
typedef char bustup_h_check_speaker[(u32)&((struct BustupState *)0)->speaker == 0x12EB ? 1 : -1];
typedef char bustup_h_check_fade[(u32)&((struct BustupState *)0)->fade == 0x12EC ? 1 : -1];
typedef char bustup_h_check_index[(u32)&((struct BustupState *)0)->dialogueIndex == 0x136C ? 1 : -1];
typedef char bustup_h_check_timers[(u32)&((struct BustupState *)0)->timer0 == 0x1374 ? 1 : -1];

/* Bust-up runner step: gMain.seqIndex1 for gBustupSteps (0x0813ADD4). The `$b` text code adds 4 to go from
 * UPDATE to CHANGE_SPEAKER, which then returns to UPDATE. */
enum BustupStep {
    BUSTUP_STEP_INIT = 0,
    BUSTUP_STEP_LOAD_SCENE = 1,
    BUSTUP_STEP_UPDATE = 2,
    BUSTUP_STEP_END = 3,
    BUSTUP_STEP_UNUSED_OPPONENT = 4,    /* unreachable opponent preview */
    BUSTUP_STEP_SELECT_END = 5,
    BUSTUP_STEP_CHANGE_SPEAKER = 6,
    BUSTUP_STEP_CHANGE_SCENE_END = 7,
};

/* Text printer state: BustupTextBox.state (s8), set by Bustup_UpdateTextBox, Bustup_InitTextBox and the
 * A-button handling of Bustup_Update. Negative states stop printing. */
enum BustupTextState {
    TEXT_STATE_BOX_FULL = -3,
    TEXT_STATE_END = -2,
    TEXT_STATE_WAIT_BUTTON = -1,        /* `$c`: wait for A */
    TEXT_STATE_PRINT = 0,
    TEXT_STATE_FAST = 1,                /* A pressed or auto mode: print without delay */
    TEXT_STATE_INSERT_NAME = 2,         /* copying name[] (`$q`, `$Q`, `$i`) */
    TEXT_STATE_NEXT_PAGE = 3,           /* `$p`: page flip */
};

/* Unused duplicate of enum BustupTextState (bustup_runner now uses the TEXT_STATE_* names). The header plan
 * still lists it (types.json); delete it together with that entry. */
enum TextBoxState {
    TEXTBOX_BOX_FULL = -3,
    TEXTBOX_END = -2,
    TEXTBOX_WAIT_BUTTON = -1,
    TEXTBOX_PRINT = 0,
    TEXTBOX_PRINT_FAST = 1,
    TEXTBOX_INSERT_NAME = 2,
    TEXTBOX_NEW_PAGE = 3,
};

/* Glyph colours of BustupTextBox.textColor that the code sets itself (`$rX` sets any of 0-15). */
enum TextColor {
    TEXT_COLOR_CARD_NAME = 3,           /* `$i` card names */
    TEXT_COLOR_SHORT_NAME = 4,          /* `$Q` short duelist names */
    TEXT_COLOR_FULL_NAME = 5,           /* `$q` full duelist names */
    TEXT_COLOR_DEFAULT = 7,
};

/* Bust-up module state. */
extern struct BustupState gBustup;

/* Address-suffixed aliases of gBustup members, declared on their own so units can name them directly. */
extern struct Line gBustupCursor;           /* 0x020147B4 = gBustup.textBox.cursor */
extern struct OamList gBustupSprites;       /* 0x02014888 = gBustup.sprites */
extern struct AnimState gBustupMouthAnim;   /* 0x02014EB4 = gBustup.anims[1], restarted after every glyph */
extern struct Fade gBustupFade;             /* 0x020150CC = gBustup.fade */

/* EWRAM decode buffer for the scene bitmap, the header strip and the OBJ tile sheet (240x160 bytes). */
extern u8 gBustupBitmapBuffer[0x9600];
/* Decoded dialogue box bitmap (240x64), the source for drawing and clearing the box. */
extern u8 gBustupBoxBitmap[0x3C00];

/* ---- Dialogue table and names ---- */

/* Index of the first gDialogueTable record with this event id, or DIALOGUE_COUNT if there is none. */
u32 GetDialogueIndex(u16 eventId);
/* gDialogueTable[index].eventId, or 0 if index > DIALOGUE_COUNT. */
u16 GetDialogueEventId(u32 index);
/* gDialogueTable[index].speakerId, or 0 if index > DIALOGUE_COUNT. */
u16 GetDialogueSpeaker(u32 index);
/* Selects the dialogue of an event: gMain.dialogueIndex, gMain.speaker, gMain.seqIndex1 = 0. Does nothing
 * if the event has no record. */
void StartDialogue(u16 eventId);
/* The scene set of a speaker (&gSceneSets[n]); unknown ids and 1 give Yugi's set 6. */
const struct SceneSet *GetSceneSet(u32 charId);
/* Full (full != 0, `$q`) or short (`$Q`) name of character `id` from gDuelists; entry 0 (" ") if absent. */
char *GetDuelistName(u32 id, u16 full);

/* ---- Runner and its steps (gBustupSteps, enum BustupStep) ---- */

/* Dialogue scene runner: runs gBustupSteps[gMain.seqIndex1]; at the NULL step hides the BG/OBJ layers and
 * returns 1, so other scenes call it as a sub-runner. */
u16 CB_Bustup(void);
/* CB_Bustup with gBustup.autoAdvance set every frame: fast text, records play back to back. */
u16 CB_AutoBustup(void);
/* BUSTUP_STEP_INIT: Bustup_InitState; returns 1. */
u16 Bustup_Init(void);
/* BUSTUP_STEP_LOAD_SCENE: starts the fade-in and loads the speaker's scene set and the record's text. */
s32 Bustup_LoadScene(void);
/* BUSTUP_STEP_UPDATE, every frame: the fade, A (fast text / continue / next page), B (skip the dialogue),
 * the text printer and the portrait animation. */
s32 Bustup_Update(void);
/* BUSTUP_STEP_UNUSED_OPPONENT: unreachable leftover of an opponent preview (one slot's animation, the cursor
 * sparkle and the opponent's record). */
s32 Bustup_UnusedOpponentPreview(void);
/* BUSTUP_STEP_CHANGE_SPEAKER: after `$b`, fades in the new speaker's scene set (keeping the box and text). */
s32 Bustup_ChangeSpeaker(void);
/* Draws the win/loss/draw counts of the previewed opponent as 3-digit sprites, with the banner if
 * drawBanner (unused preview step). */
void Bustup_DrawOpponentRecord(s32 drawBanner);

/* ---- Scene loading and Mode-4 pages ---- */

/* Clears gBustup, enables the OAM flush, hides all layers and resets BG1-3 scroll (callers pass an ignored
 * argument). */
void Bustup_InitState(void);
/* Loads a 240x96 scene set and dialogue box `box` into both Mode-4 pages, its palettes, OBJ tiles and the
 * animation tracks (anims = struct AnimState[20] block), and points tb->text at `text`. */
void Bustup_LoadSceneSet(const struct SceneSet *set, const u8 *text, struct BustupTextBox *tb,
                         void *anims, u8 box);
/* Bustup_LoadSceneSet for the 240x80 scene sets (speakers 0x20-0x23): the header strip fills rows 0-15. */
void Bustup_LoadSceneSetWithHeader(const struct SceneSet *set, const u8 *text, struct BustupTextBox *tb,
                                   void *anims, u8 box);
/* Loads a new 240x96 scene set into both pages but keeps the on-screen box and tb->text (`text` unused). */
void Bustup_ChangeSceneSet(const struct SceneSet *set, const u8 *text, struct BustupTextBox *tb,
                           void *anims, u8 box);
/* Sets DISPCNT bit 4 (Mode-4 frame select) from tb->page. */
void Bustup_ShowPage(struct BustupTextBox *tb);
/* If tb->boxDirty == 1, copies the clean box bitmap over the box area of the page that is not shown. */
void Bustup_ClearHiddenBox(struct BustupTextBox *tb);
/* Copies a full 240x160 8bpp screen from *bitmap to Mode-4 page 0 or 1. */
void CopyFullBitmapToPage(u8 page, void **bitmap);
/* Copies `size` bytes from *bitmap to Mode-4 page `page`, in four CpuSet quarters. */
void CopyBitmapToPage(u8 page, void **bitmap, u16 size);
/* CpuSets `rows` rows of `width` bytes from a 240-byte-stride bitmap to dest, `pitch` bytes apart. */
void CopyBitmapRows(u8 *src, u32 *dest, u16 width, u16 rows, u16 pitch);

/* ---- Text box ---- */

/* Text box init: delay 2, state PRINT (FAST in auto mode), scriptFlag cleared, cursor home, colour 7. */
void Bustup_InitTextBox(struct BustupTextBox *tb);
/* tb->boxDirty = 1: clear the box and show the page on the next frame. */
void Bustup_MarkBoxDirty(struct BustupTextBox *tb);
/* Per-frame text printer. Codes: $QNN short / $qNN full duelist name, $iNNNN card name by card number
 * (>= 2000: alternate art), $bNN change speaker scene, $c wait for A, $h/$k clear/set scriptFlag,
 * $n newline, $p next page, $rX colour 0-f. */
void Bustup_UpdateTextBox(struct BustupTextBox *tb);
/* Writes one 8bpp pixel through a 16-bit read-modify-write (VRAM ignores byte writes). */
void PlotPixel8bpp(u8 *dest, u32 color);
/* Plots one 10-row 1bpp glyph into a 240-px-wide 8bpp bitmap (kanji font in 2-byte text mode). */
void DrawGlyph8bpp(u8 *dest, u32 color, u16 glyph);

/* ---- Portrait animation and sprites ---- */

/* st->active = 1: replays a finished one-shot animation track from step 0. */
void AnimStateStart(struct AnimState *st);
/* Counts gBustup.blinkTimer down; when it wraps, loads the next gBlinkIntervals entry and restarts `eyes`. */
void Bustup_TickBlink(struct AnimState *eyes);
/* blinkTimer = blinkIndex = 0 (blink on the next tick); sets unk12 of the gBustup.unk810 entries. */
void Bustup_ResetBlink(void);
/* Byte-identical, uncalled copy of Bustup_ResetBlink. */
void Bustup_ResetBlinkUnused(void);
/* Adds 2n + 2 32x16 sprites side by side to gBustupSprites (OBJ tile 0x200 + tile, palette 2). */
void Bustup_DrawLabel(u16 tile, u16 x, u16 y, u16 n);
/* Draws value right-aligned in `digits` 8x8 digit sprites (leading zeros blank) into gBustupSprites. */
void Bustup_DrawNumber(u16 value, u16 x, u16 y, u16 digits, u8 pal);
/* Draws the cursor sparkle and its trail (unused preview step only). The ROM's callers pass two ignored
 * arguments, and it calls OamListAddSprite with too few arguments (kept as in the ROM). */
void Bustup_DrawCursorTrail(void);

#endif /* GUARD_BUSTUP_H */
