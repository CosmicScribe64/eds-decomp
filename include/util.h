#ifndef GUARD_UTIL_H
#define GUARD_UTIL_H

/*
 * Small general-purpose helpers: memory and string routines, rounding, Random, 8.8 fixed-point maths and the
 * sine tables, frame steppers (Timer, Ease, Line, Tween), callback queues and step lists, and Coords16.
 * Defined in src/main.c (memory, strings, rounding), src/sprite.c (Random), src/bitmap_text.c (Line),
 * src/gfx_util.c (steppers, fixed point) and src/link_sio.c (Tween).
 *
 * Fixed point: "8.8" values use 0x100 = 1.0.
 *
 * Many units call these through a local prototype with other parameter widths than the definition (listed
 * per function in build/readability/protos.json). Such a unit keeps its commented local view when it
 * migrates, because the call-site narrowing is part of the matched code.
 */

#include "global.h"

/* ------------------------------------------------------------------------------------------------------ */
/* Types                                                                                                  */
/* ------------------------------------------------------------------------------------------------------ */

/* A screen position in pixels (pret's name; replaces the local struct Pos16 views). */
struct Coords16 {
    s16 x;  /* +0 */
    s16 y;  /* +2 */
};

/* State of a Timer, an Ease and the palette fades of palette.h; callers poll for TICK_DONE. */
enum TickState {
    TICK_IDLE,      /* 0: not started (Timer_Reset, Ease_Init) */
    TICK_RUNNING,   /* 1 */
    TICK_DONE       /* 2: finished */
};

/* A frame countdown (Timer_Reset/Start/Tick). */
struct Timer {
    u8 state;       /* +0 enum TickState */
    u16 count;      /* +2 frames left */
};

/*
 * A linear stepper: cur moves by step each Ease_Tick until it reaches end. Despite the name there is no
 * curve here; callers use cur as an index into easing tables (gDeckEditEaseCurve, gPackListSlideEase).
 */
struct Ease {
    u8 state;       /* +0 enum TickState */
    u16 cur;        /* +2 current value (compared as s16) */
    u16 end;        /* +4 target value */
    s16 step;       /* +6 added to cur each tick; its sign selects the stop test */
};

/* struct Line.state */
enum LineState {
    LINE_DONE,      /* 0: reached the end point */
    LINE_X_MAJOR,   /* 1: |dx| >= |dy|: x advances every step */
    LINE_Y_MAJOR    /* 2: y advances every step */
};

/* A Bresenham line walker, one pixel per LineStep (bust-up text cursor, Exodia scene pieces). */
struct Line {
    u16 x;          /* +0x00 current x */
    u16 y;          /* +0x02 current y (x and y are compared with endX/endY as one u32) */
    u16 endX;       /* +0x04 */
    u16 endY;       /* +0x06 */
    u16 stepX;      /* +0x08 +1 or -1 (0xFFFF) */
    u16 stepY;      /* +0x0A +1 or -1 */
    s16 dx;         /* +0x0C |endX - x0| */
    s16 dy;         /* +0x0E |endY - y0| */
    u16 error;      /* +0x10 Bresenham accumulator, starts at 0 */
    u8 state;       /* +0x12 enum LineState */
};

/* struct Tween.mode, TweenInit's mode argument (every caller in the ROM uses TWEEN_APPROACH). */
enum TweenMode {
    TWEEN_APPROACH,     /* 0: constant speed, slowing to 1 px/frame within 7 px of the end */
    TWEEN_EASE_IN_OUT,  /* 1: (1 - cos) / 2 over half a turn */
    TWEEN_EASE_OUT,     /* 2: sine over a quarter turn (no break in the original: also runs mode 3) */
    TWEEN_EASE_IN       /* 3: 1 - cos over a quarter turn */
};

/* struct Tween.state */
enum TweenState {
    TWEEN_RUNNING = 1,
    TWEEN_DONE = 2
};

/* A 2D move from a start to an end point (TweenInit/TweenUpdate; turn-order scene work +0xADC). */
struct Tween {
    u16 x;          /* +0x00 current x */
    u16 y;          /* +0x02 current y */
    u16 startX;     /* +0x04 (modes 1-3) */
    u16 startY;     /* +0x06 */
    u16 endX;       /* +0x08 target */
    u16 endY;       /* +0x0A */
    s16 step;       /* +0x0C mode 0: x speed in px/frame; modes 1-3: phase increment per frame */
    s16 phase;      /* +0x0E mode 0: y speed; modes 1-3: phase (mode 1 0..0x7F, modes 2-3 0..0x3F00) */
    s16 dx;         /* +0x10 endX - startX (modes 1-3) */
    s16 dy;         /* +0x12 endY - startY (modes 1-3) */
    u8 state;       /* +0x14 enum TweenState */
    u8 mode;        /* +0x15 enum TweenMode */
};

/* Four callbacks run every frame until they return non-zero (CallbackQueue_*; unused in the ROM). */
struct CallbackQueue {
    u8 head;                    /* +0x00 index of the last slot written (pre-incremented, never masked) */
    u8 pad[3];
    u16 (*slot[4])(void);       /* +0x04 NULL = free; a non-zero return clears the slot */
};

/* A NULL-terminated list of steps run in order (StepList_*; unused in the ROM). */
struct StepList {
    u8 idx;                     /* +0x00 current step */
    u16 (**steps)(void);        /* +0x04 step table; a step returns non-zero to advance */
};

STATIC_ASSERT(sizeof(struct Coords16) == 0x4, Coords16Size);
STATIC_ASSERT(sizeof(struct Timer) == 0x4, TimerSize);
STATIC_ASSERT(sizeof(struct Ease) == 0x8, EaseSize);
STATIC_ASSERT(sizeof(struct Line) == 0x14, LineSize);
STATIC_ASSERT(OFFSET_OF(struct Line, state) == 0x12, LineState);
STATIC_ASSERT(sizeof(struct Tween) == 0x18, TweenSize);
STATIC_ASSERT(OFFSET_OF(struct Tween, mode) == 0x15, TweenMode);
STATIC_ASSERT(sizeof(struct CallbackQueue) == 0x14, CallbackQueueSize);
STATIC_ASSERT(sizeof(struct StepList) == 0x8, StepListSize);

/* ------------------------------------------------------------------------------------------------------ */
/* Data                                                                                                   */
/* ------------------------------------------------------------------------------------------------------ */

/*
 * 0x02030000: general scratch area, used with different element types: the LZSS ring buffer, the
 * QuickSortS16 range stack, the Deck Edit statistics rows. Units keep their own typed views.
 */
extern u8 gScratchBuffer[];

/* 0x08087BA4: sine, 8.8 fixed, 256 steps per turn; [i + 0x40] is the cosine. ObjAffineApply uses angle >> 8. */
extern const s16 gSineTable[];
/* 0x081A77A8: sine, 8.8 fixed, 128 steps per turn; [i + 32] is the cosine (SetOamMatrix*). main.c reads it
 * through a local const u16 view (ldrh), sprite.c through this s16 type. */
extern const s16 gSineTable128[];
/* 0x08082712: k * k for k = 0..16 (17 entries), the drop curve of the turn-order DUEL logo. */
extern const u16 gSquareTable[];

/* ------------------------------------------------------------------------------------------------------ */
/* Functions                                                                                              */
/* ------------------------------------------------------------------------------------------------------ */

/* Memory (src/main.c; halfword/doubleword copies, safe for VRAM and palette RAM) */
/* Zeroes (size + 1) / 2 halfwords at dst. */
void MemClear16(void *dst, s32 size);
/* Copies (size + 1) / 2 halfwords. */
void MemCopy16(void *dst, const void *src, u32 size);
/* Copies (size + 7) / 8 blocks of 8 bytes (ldmia/stmia pairs). */
void CopyDoubleWords(void *dst, const void *src, u32 size);

/* Strings (byte strings; Shift-JIS text uses 2 bytes per character) */
/* strcpy: copies src including the NUL. */
void StrCopy(char *dst, const char *src);
/* strcat. */
void StrCat(char *dst, const char *src);
/* Appends n in decimal (up to 10 digits). n <= 0 appends nothing, so 0 prints as an empty string. */
void StrCatNumber(char *dst, s32 n);
/* StrCatNumber with full-width Shift-JIS digits (0x824F + d); same n <= 0 gotcha. Unused. */
void StrCatNumberFullwidth(char *dst, s32 n);
/* Length in bytes. */
s32 StrLen(const char *s);
/* Number of 2-byte characters before the first one whose low byte is 0 (full-width string length). */
s32 StrLenWide(const u16 *s);
/* Minimal sprintf: fmt with its first "%s" replaced by arg. No NUL in dst when fmt has no "%s". */
void FormatStr(char *dst, const char *fmt, const char *arg);
/* Minimal sprintf: fmt with its first "%d" replaced by arg (StrCatNumber: 0 prints as nothing). */
void FormatInt(char *dst, const char *fmt, s32 arg);

/* Rounding (ATK/DEF, damage and life points) */
/* (x + 5) / 10 * 10: nearest multiple of 10, halves up. */
s32 RoundTo10(s32 x);
/* (x * 5 + 5) / 10: x / 2 rounded up for x >= 0 (1501 -> 751). Halves ATK/DEF and damage. */
s32 HalveRoundUp(s32 x);
/* (x * 5 + 4) / 10: x / 2 rounded down for x >= 0 (1501 -> 750). Halves life points. */
s32 HalveRoundDown(s32 x);

/* Random numbers (src/sprite.c) */
/* gMain.rngState = rotl16(state * 0x343FD + 0x269EC3); returns 0..32767 (bits 16-30). */
int Random(void);

/* 8.8 fixed point (src/gfx_util.c) */
/* (a * b) >> 8. */
s16 MulFix8(s16 a, s16 b);
/* (a * b) >> 8 with a 64-bit product. */
s32 MulFix8Wide(s32 a, s32 b);
/* (s16)Div(a * 256, b): s16 operands passed as words; no zero check. */
int DivFix8(int a, int b);
/* Div(0x10000, a): 1 / a in 8.8. */
s16 ReciprocalFix8(s16 a);

/* Timers (src/gfx_util.c) */
/* state = TICK_IDLE, count = 0. */
void Timer_Reset(struct Timer *t);
/* state = TICK_RUNNING, count = frames. */
void Timer_Start(struct Timer *t, u16 frames);
/* While running: --count; at 0 state = TICK_DONE. */
void Timer_Tick(struct Timer *t);

/* Linear steppers (src/gfx_util.c) */
/* Stores cur/end/step with state = TICK_IDLE (not running). */
void Ease_Init(u16 cur, u16 end, s16 step, struct Ease *e);
/* Stores cur/end/step with state = TICK_RUNNING. */
void Ease_Start(u16 cur, u16 end, s16 step, struct Ease *e);
/* While running: cur += step; at or past end: cur = end, state = TICK_DONE. */
void Ease_Tick(struct Ease *e);

/* Lines (src/bitmap_text.c) */
/* Bresenham setup from (x0, y0) to (x1, y1): picks the major axis, error = 0. */
void LineInit(s16 x0, s16 y0, s16 x1, s16 y1, struct Line *line);
/* One Bresenham step; sets LINE_DONE at the end point, does nothing once done. */
void LineStep(struct Line *line);

/* Tweens (src/link_sio.c) */
/* Starts a 2D tween in mode (enum TweenMode). Mode 0: speeds vxOrFrames/vy; modes 1-3: vxOrFrames frames. */
void TweenInit(u16 startX, u16 startY, u16 endX, u16 endY, u16 vxOrFrames, u16 vy, struct Tween *tween, u8 mode);
/* One frame while running; snaps to the end and sets TWEEN_DONE when it arrives. */
void TweenUpdate(struct Tween *tween);

/* Callback queues and step lists (src/gfx_util.c; no callers in the ROM) */
/* Sets the 4 slots to NULL (head is not reset). */
void CallbackQueue_Init(struct CallbackQueue *q);
/* Stores fn in slot[++head & 3] if slot[head] is free; returns the old head, or 0 when busy. */
u8 CallbackQueue_Add(u16 (*fn)(void), struct CallbackQueue *q);
/* Calls every non-NULL slot; a slot whose callback returns non-zero is cleared. */
void CallbackQueue_Run(struct CallbackQueue *q);
/* l->idx = 0, l->steps = steps. */
void StepList_Init(u16 (**steps)(void), struct StepList *l);
/* Returns 1 at the NULL end of the list; otherwise runs steps[idx] (idx++ on non-zero) and returns 0. */
u32 StepList_Run(struct StepList *l);

#endif /* GUARD_UTIL_H */
