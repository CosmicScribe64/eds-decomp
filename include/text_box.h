#ifndef GUARD_TEXT_BOX_H
#define GUARD_TEXT_BOX_H

/*
 * Duel text box: a framed message on BG3 that slides down from the top of the duel screen, waits for input
 * and slides back up, optionally with a Yes/No or two-line choice menu.
 *
 * Usage: TextBoxOpen(pos, size, TEXTBOX_FLAGS_DEFAULT, text), then TextBoxSetMenu(kind, draw, input). DuelMainStep
 * calls TextBoxUpdate every frame while the box is active; once it returns 0 the answer is in gTextBox.result.
 */

#include "global.h"

/* gTextBox.flags (third argument of TextBoxOpen): which TextBoxStep steps run. */
enum TextBoxFlags {
    TEXTBOX_FLAG_SLIDE_IN = 0x1,
    TEXTBOX_FLAG_SLIDE_OUT = 0x2,
    TEXTBOX_FLAG_WAIT_INPUT = 0x8,
    TEXTBOX_FLAGS_DEFAULT = 0xB         /* every call site passes this */
};

/* gTextBox.menuKind (first argument of TextBoxSetMenu). */
enum TextBoxMenuKind {
    TEXTBOX_MENU_MESSAGE = 0,           /* A closes the box */
    TEXTBOX_MENU_YES_NO = 1,            /* result: nonzero = Yes */
    TEXTBOX_MENU_TWO_CHOICE = 2,        /* result: chosen line 0/1 */
    TEXTBOX_MENU_CUSTOM = 5             /* caller-supplied draw and input callbacks */
};

/* gTextBox.menuState, used by the choice-menu callbacks (custom callbacks reuse the byte). */
enum TextBoxMenuState {
    TEXTBOX_MENU_STATE_SELECT = 0,
    TEXTBOX_MENU_STATE_CONFIRMED = 1,   /* the chosen line blinks */
    TEXTBOX_MENU_STATE_DONE = 2
};

/* gTextBox.step, advanced by TextBoxUpdate. */
enum TextBoxStep {
    TEXTBOX_STEP_SLIDE_IN = 0,
    TEXTBOX_STEP_INPUT = 1,
    TEXTBOX_STEP_SLIDE_OUT = 2,
    TEXTBOX_STEP_CLOSE = 3
};

/* gTextBox: the duel text box and its text tile buffer. Cleared at duel start (0x2124 bytes). */
struct TextBox {
    u8 active:1;                        /* +0x00 bit 0: set by TextBoxOpen, cleared by TextBoxUpdate at the end */
    u8 tilesDirty:1;                    /* +0x00 bit 1: upload tiles[] to VRAM 0x06009AE0 (DuelScreen_Update) */
    u8 unk0_2:6;
    u8 unk1;
    u16 unk2;                           /* +0x02: cleared by TextBoxOpen, never read */
    u16 menuKind;                       /* +0x04: enum TextBoxMenuKind */
    u16 flags;                          /* +0x06: enum TextBoxFlags */
    u16 x;                              /* +0x08: left text column in cells; the frame starts at x - 1 */
    u16 y;                              /* +0x0A: top cell */
    u16 width;                          /* +0x0C: text width in cells (5-24) */
    u16 height;                         /* +0x0E: text height in cells (2-11) */
    u16 unk10;                          /* +0x10: unused */
    u16 yesNoCursor;                    /* +0x12: highlighted label, 0 YES, 1 NO (TextBoxOpen keeps it) */
    u16 result;                         /* +0x14: menu answer: Yes/No nonzero = Yes, two-choice line 0/1 */
    u16 unk16;
    void (*drawCallback)(void);         /* +0x18: sprite callback while step <= 2; NULL = TextBoxDrawSprites */
    u16 (*inputCallback)(void);         /* +0x1C: step 1 input, nonzero when done; NULL = TextBoxHandleInput */
    u8 step;                            /* +0x20: enum TextBoxStep */
    u8 revealRow;                       /* +0x21: map row the sliding box has reached */
    u8 menuState;                       /* +0x22: enum TextBoxMenuState */
    u8 menuTimer;                       /* +0x23: blink timer of the menu callbacks */
    u8 tiles[0x1B00];                   /* +0x24: 216 4bpp text tiles (BG tiles 0x2D7..) drawn by TextBoxDrawText */
    u8 unk1B24[0x600];                  /* +0x1B24: no access found; the clear covers it (hypothesis: the text
                                           buffer is sized for 24 x 11 = 264 tiles, of which 216 are used) */
};

extern struct TextBox gTextBox;         /* 0x0201AE60 */

/* Open, menus and update */
/* Open the box: pos = x | y << 8, size = width | height << 8 (cells), flags = enum TextBoxFlags. */
void TextBoxOpen(u16 pos, u16 size, u16 flags, const u8 *text);
/* Choose the menu (enum TextBoxMenuKind): MESSAGE/YES_NO use the default callbacks, TWO_CHOICE the choice ones,
 * CUSTOM the callbacks passed here. */
void TextBoxSetMenu(u16 menuKind, void (*drawCallback)(void), u16 (*inputCallback)(void));
/* Per-frame driver run by DuelMainStep: slide in, input, slide out; 1 while the box is active. */
int TextBoxUpdate(void);

/* Default callbacks */
u16 TextBoxHandleInput(void);           /* default input: A closes a message; Yes/No: A answers, B = No */
void TextBoxDrawSprites(void);          /* default draw: A-button icon, or the Yes/No labels and pointer */
u16 TextBoxHandleChoiceInput(void);     /* two-line choice input: Up/Down picks, A confirms; 1 when done */
void TextBoxDrawChoiceCursor(void);     /* two-line choice draw: arrow sprite at the chosen line */
int TextBoxHandleChoiceInputCpu(void);  /* CPU stand-in for TextBoxHandleChoiceInput: picks a random line */

/* Rendering */
void TextBoxClearTiles(void);           /* fill the 216 text tiles with the blank frame-centre tile */
/* Set the box rectangle (pos/size as TextBoxOpen, clamped) and render the text into tiles[]. */
void TextBoxDrawText(u16 pos, u16 size, const u8 *text);
void TextBoxDrawTilemap(u32 row);       /* write the framed box into the BG3 map, ending above map row 'row' */
int TextBoxSlideIn(void);               /* draw one more row of the box; 1 when fully shown */
int TextBoxSlideOut(void);              /* remove one row of the box; 1 when gone */

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char text_box_h_check_flags[(u32)&((struct TextBox *)0)->flags == 0x6 ? 1 : -1];
typedef char text_box_h_check_result[(u32)&((struct TextBox *)0)->result == 0x14 ? 1 : -1];
typedef char text_box_h_check_draw[(u32)&((struct TextBox *)0)->drawCallback == 0x18 ? 1 : -1];
typedef char text_box_h_check_step[(u32)&((struct TextBox *)0)->step == 0x20 ? 1 : -1];
typedef char text_box_h_check_tiles[(u32)&((struct TextBox *)0)->tiles == 0x24 ? 1 : -1];
typedef char text_box_h_check_size[sizeof(struct TextBox) == 0x2124 ? 1 : -1];

#endif /* GUARD_TEXT_BOX_H */
