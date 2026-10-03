#ifndef GUARD_CARD_DETAIL_H
#define GUARD_CARD_DETAIL_H

/*
 * Full-screen Card Detail viewer (units card_detail and title_menu): card art frame, name, icons, stats and
 * the scrolling description. Used by Deck Edit, the duel, campaign rewards, Password and Card Trading.
 *
 * Usage: CardDetail_Init(cardId, timer, 0), then call CardDetail_Run every frame until it returns 1. Pack
 * opening runs the individual steps (InitVideo, FadeIn, HandleInput, FadeOut) itself.
 */

#include "global.h"

/* Card Detail viewer state at 0x02013D90 (0x44 bytes), cleared by CardDetail_Reset. */
struct CardDetail {
    u8 waveIntro:1;             /* +0x00 bit 0: wavy HBlank/mosaic intro and the alternate layout (name
                                 * centred, sprites shifted right 0x48, no type/description). No
                                 * CardDetail_Init caller sets it in retail (hypothesis: unused). Older code
                                 * calls it useHBlank. */
    u16 state:15;               /* +0x00 bits 1-15: enum CardDetailState. Deck Edit resets it with
                                 * `gCardDetail &= 1`, which keeps waveIntro. */
    u16 cardId;                 /* +0x02: card ID being shown */
    u16 timer;                  /* +0x04: auto-close countdown in frames; 0 = wait for A/B (the duel uses 300) */
    u8 unk06[0x26];             /* +0x06: no accesses found */
    s32 atk;                    /* +0x2C: displayed ATK (stat * 10; 0 for Trap/Magic/Ticket, 4000 for Divine) */
    s32 def;                    /* +0x30: displayed DEF */
    s32 scrollPos;              /* +0x34: current BG3 (description) vertical scroll */
    s32 scrollTarget;           /* +0x38: scroll target, 0 or textHeight - 0x70 */
    s32 textHeight;             /* +0x3C: rendered description height in px (0x70 visible), set by
                                 * CardDetail_RenderText */
    u16 browseCardId;           /* +0x40: debug browser cursor (card ID 1..820) */
    u8 unk42[2];                /* +0x42: padding */
};

typedef char card_detail_h_check_size[sizeof(struct CardDetail) == 0x44 ? 1 : -1];
typedef char card_detail_h_check_card[(u32)&((struct CardDetail *)0)->cardId == 0x02 ? 1 : -1];
typedef char card_detail_h_check_atk[(u32)&((struct CardDetail *)0)->atk == 0x2C ? 1 : -1];
typedef char card_detail_h_check_height[(u32)&((struct CardDetail *)0)->textHeight == 0x3C ? 1 : -1];
typedef char card_detail_h_check_browse[(u32)&((struct CardDetail *)0)->browseCardId == 0x40 ? 1 : -1];

/* CardDetail_Run state machine: gCardDetail.state. */
enum CardDetailState {
    CARD_DETAIL_SETUP = 0,      /* CardDetail_InitVideo + CardDetail_DrawCard */
    CARD_DETAIL_FADE_IN = 1,
    CARD_DETAIL_INPUT = 2,
    CARD_DETAIL_FADE_OUT = 3,
    CARD_DETAIL_DONE = 4,
};

/* Card Detail viewer state. */
extern struct CardDetail gCardDetail;

/* ---- Viewer control (card_detail) ---- */

/* MemClear16(&gCardDetail, 0x44). */
void CardDetail_Reset(void);
/* Prepares the viewer for a card: Reset, cardId, displayed ATK/DEF, auto-close timer (0 = none) and the
 * wave intro flag (every caller passes 0). */
void CardDetail_Init(u16 cardId, u16 timer, u16 waveIntro);
/* Runs the viewer (enum CardDetailState) for one frame; returns 1 when it has closed. */
u16 CardDetail_Run(void);

/* ---- Viewer steps (card_detail; each returns 1 when done) ---- */

/* Video setup: all layers off, BG0-3CNT, system font, OBJ palette 3 and the digit tiles. */
u16 CardDetail_InitVideo(void);
/* Shows BG0-3 and OBJ, draws the stat sprites and fades in from black. */
u16 CardDetail_FadeIn(void);
/* Draws the stat sprites and fades to black; hides BG0-3 and OBJ when black. */
u16 CardDetail_FadeOut(void);
/* Per frame: A/B close (SE 2) and the auto-close timer counts down; Up/Down jump the description's scroll
 * target to the top/bottom and BG3 scrolls 2 px per frame towards it. */
u16 CardDetail_HandleInput(void);
/* Draws the text and icons (CardDetail_DrawInfo), the card frame picked by card type or enum CardKind (not
 * for tokens) and the card art. */
void CardDetail_DrawCard(void);

/* ---- Drawing (title_menu) ---- */

/* HBlank handler: BG1HOFS/BG2HOFS from gMain.hblankScroll[VCOUNT & 15] (the wave intro). */
void CardDetail_HBlank(void);
/* Renders str into the text canvas: size = width | height << 8, pos = x | y << 16, colors = colour |
 * shadow << 8. Draws with a drop shadow (+1, +1) and, while the text does not fit (400+ chars or past
 * y 0xC0), retries with tighter spacing, then without shadow at line height 8. Sets gCardDetail.textHeight. */
void CardDetail_RenderText(u16 size, u32 pos, const u8 *str, u16 colors, int lineHeight, u16 wrap);
/* CardDetail_RenderText, then converts the canvas to BG tiles from firstTile (background pixels =
 * bgColor) and writes their map entries into gMain.bgMapBuffer[bg] at mapPos = x | y << 8 (in tiles). */
void CardDetail_DrawTextBox(u32 bg, u16 mapPos, u16 size, u16 firstTile, u16 colors, u16 lineHeight,
                            u32 textPos, const u8 *str, u16 wrap, u16 bgColor);
/* Draws the card name, the attribute/Magic/Trap/Divine and subtype icons, then the type line with its
 * /Effect, /Fusion or /Ritual suffix and the description. */
void CardDetail_DrawInfo(u16 cardId);
/* Adds digit sprites (tile 0x30 + digit, palette 3) right to left from x + 0x14, 4 px apart. */
void CardDetail_DrawNumber(int x, int y, int value);
/* Adds the "ATK" label sprite and gCardDetail.atk. */
void CardDetail_DrawAtk(void);
/* Adds the "DEF" label sprite and gCardDetail.def. */
void CardDetail_DrawDef(void);
/* Per-frame stat sprites (nothing for tokens): level stars, attribute icon and ATK/DEF; fixed glyphs for
 * the Divine cards. */
void CardDetail_DrawSprites(void);

#endif /* GUARD_CARD_DETAIL_H */
