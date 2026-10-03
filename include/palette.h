#ifndef GUARD_PALETTE_H
#define GUARD_PALETTE_H

/*
 * Screen fades and blending.
 *
 * Two kinds of fade drive the colour-effect registers:
 *  - the global fade level gMain.brightness (0..0x1F), stepped by FadeToBlack/FadeFromBlack and friends
 *    (src/main.c) and mirrored to BLDY or BLDALPHA; a scene calls one of them per frame until it returns 1;
 *  - struct Fade objects (FadeStart/FadeTick, src/text_render.c) with an 8.8 level, owned by a scene's work
 *    area.
 * Palette fades (PalFade, PalFadeStrided; src/gfx_util.c) interpolate palette RAM colours towards one colour.
 *
 * BLDY/BLDALPHA coefficients are 0..16 (larger values act as 16).
 */

#include "global.h"

/* ------------------------------------------------------------------------------------------------------ */
/* Types                                                                                                  */
/* ------------------------------------------------------------------------------------------------------ */

/* FadeStart's color: the BLDCNT brightness effect used. */
enum FadeColor {
    FADE_BLACK,     /* 0: BLDCNT 0xFF (all layers, brightness decrease) */
    FADE_WHITE      /* 1 (any non-zero): BLDCNT 0xBF (brightness increase) */
};

/* struct Fade.state */
enum FadeState {
    FADE_STATE_IDLE,        /* 0: never started */
    FADE_STATE_RUNNING,     /* 1 */
    FADE_STATE_FADED_OUT,   /* 2: level reached 0x1000 (full colour) */
    FADE_STATE_FADED_IN     /* 3: level reached 0 */
};

/* A brightness fade (FadeStart/FadeTick), e.g. gDeckEdit+0x618, gBustup.fade (alias gBustupFade). */
struct Fade {
    u8 color;       /* +0 enum FadeColor */
    u8 pad;         /* +1 */
    u16 level;      /* +2 8.8 brightness 0..0x1000 (BLDY = level >> 8) */
    s16 step;       /* +4 per-tick delta: > 0 fades out to the colour, < 0 fades in; 0 when done */
    u8 state;       /* +6 enum FadeState */
    u8 param;       /* +7 caller-defined byte stored by FadeStart (e.g. the step increment at the end) */
};

/* Per-channel distance from a start colour to the target colour (BGR555 channels, -31..31). */
struct PalDelta {
    s8 r;           /* +0 bits 0-4 */
    s8 g;           /* +1 bits 5-9 */
    s8 b;           /* +2 bits 10-14 */
    s8 pad;         /* +3 */
};

/* A fade of up to 0x200 consecutive palette colours towards one colour (PalFade_Start/Apply). */
struct PalFade {
    u16 startColors[0x200];         /* +0x000 colours saved by PalFade_Start (the fade origin) */
    struct PalDelta delta[0x200];   /* +0x400 distance to the target colour */
    u8 step;                        /* +0xC00 0..0x20, advanced by the caller */
    u8 pad;                         /* +0xC01 */
    u16 count;                      /* +0xC02 number of colours */
    u16 *dst;                       /* +0xC04 palette RAM written by PalFade_Apply */
    u16 state;                      /* +0xC08 enum TickState (util.h) */
};

/* PalFade for up to 8 colours spaced `stride` palette entries apart (PalFadeStrided_*; no callers). */
struct PalFadeStrided {
    u16 startColors[8];             /* +0x00 saved colours */
    struct PalDelta delta[8];       /* +0x10 distance to the target colour */
    u8 step;                        /* +0x30 0..0x20, advanced by the caller */
    u8 pad;                         /* +0x31 */
    u16 count;                      /* +0x32 number of colours (max 8) */
    u16 *dst;                       /* +0x34 first palette entry */
    u8 stride;                      /* +0x38 palette entries between faded colours */
    u8 pad2;                        /* +0x39 */
    u16 state;                      /* +0x3A enum TickState (util.h) */
};

STATIC_ASSERT(sizeof(struct Fade) == 0x8, FadeSize);
STATIC_ASSERT(sizeof(struct PalDelta) == 0x4, PalDeltaSize);
STATIC_ASSERT(sizeof(struct PalFade) == 0xC0C, PalFadeSize);
STATIC_ASSERT(OFFSET_OF(struct PalFade, state) == 0xC08, PalFadeState);
STATIC_ASSERT(sizeof(struct PalFadeStrided) == 0x3C, PalFadeStridedSize);
STATIC_ASSERT(OFFSET_OF(struct PalFadeStrided, state) == 0x3A, PalFadeStridedState);

/* ------------------------------------------------------------------------------------------------------ */
/* Functions                                                                                              */
/* ------------------------------------------------------------------------------------------------------ */

/* Global fade level gMain.brightness (src/main.c). The Fade* functions return 1 when the fade is complete. */
/* Screen fully black: level 0x1F, BLDCNT 0x3FFF (all layers, darken), BLDY 0x1F. */
void SetBrightnessBlack(void);
/* Screen fully white: level 0x1F, BLDCNT 0x3FBF (all layers, brighten), BLDY 0x1F. */
void SetBrightnessWhite(void);
/* Level 0x1F with the given BLDCNT (SetBrightnessBlack/White with a parameter). */
void SetBrightnessFull(u16 bldcnt);
/* Level 0, BLDCNT 0, BLDY 0: no colour effect. */
void ClearBlend(void);
/* Darken: level += step (cap 0x1F); 1 when fully black. */
u32 FadeToBlack(s32 step);
/* Darken: level -= step; at 0 ClearBlend and return 1. */
u32 FadeFromBlack(s32 step);
/* Brighten: level += step (cap 0x1F); 1 when fully white. */
u32 FadeToWhite(s32 step);
/* Brighten: level -= step; at 0 ClearBlend and return 1. */
u32 FadeFromWhite(s32 step);
/* level -= 2 with the given BLDCNT, BLDY = level; at 0 ClearBlend and return 1. */
u32 FadeBrightnessDown(u16 bldcnt);
/* level += 2 (cap 0x1F) with the given BLDCNT, BLDY = level; 1 at 0x1F. */
u32 FadeBrightnessUp(u16 bldcnt);
/* level -= 2, BLDALPHA = EVA level / EVB 31 - level; at 0 ClearBlend and return 1. */
u32 FadeAlphaDown(u16 bldcnt);
/* level += 2 (cap 0x1F), BLDALPHA = EVA 31 - level / EVB level; 1 at 0x1F. */
u32 FadeAlphaUp(u16 bldcnt);

/* Fade objects (src/text_render.c) */
/* Starts a fade: a positive step fades out (level 0 -> 0x1000, to black for color 0, else white), a negative
 * step fades in; sets BLDCNT and BLDY. param is stored for the caller. */
void FadeStart(u8 color, s16 step, u8 param, struct Fade *fade);
/* Call once per frame: steps the level and BLDY; returns 1 only on the frame the fade ends. */
u32 FadeTick(struct Fade *fade);

/* Blend registers (src/gfx_util.c) */
/* REG_BLDALPHA = level << 8 | (16 - level): cross-fade weight 0..16 towards the 2nd target. */
void SetBldAlpha(u16 level);
/* REG_BLDY = level (0..16). */
void SetBldY(u16 level);

/* Palette fades (src/gfx_util.c). The caller advances f->step from 0 to 0x20, then calls the Apply. */
/* Saves count colours of pal and their distance to color; dst = pal, step = 0, state = TICK_RUNNING. */
void PalFade_Start(u16 *pal, u16 count, u16 color, struct PalFade *f);
/* Writes start + delta * step / 32 per channel; state = TICK_DONE after the frame that writes step 0x20. */
void PalFade_Apply(struct PalFade *f);
/* PalFade_Start for up to 8 colours pal[0], pal[stride], ... Unused. */
void PalFadeStrided_Start(u16 *pal, u16 count, u8 stride, u16 color, struct PalFadeStrided *f);
/* PalFade_Apply for a strided fade (step clamped to 0x20). Unused. */
void PalFadeStrided_Apply(struct PalFadeStrided *f);

#endif /* GUARD_PALETTE_H */
