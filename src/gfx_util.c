/*
 * gfx_util (0x0807A6AC-0x0807B6B8): tilemap, palette and stepping utilities shared by the
 * menus and duel screens (wiki/functions/gfx-util-c.md).
 *
 * BG screenblock rectangle fills and copies (32-entry rows, with the 64-wide second
 * screenblock wrap at column 0x20), bitmap block copies between tile maps, the card art
 * unpacker, palette fades towards a target colour (PalFade, PalFadeStrided), a callback
 * queue and step lists, frame Timers and linear Eases, 8.8 fixed-point helpers, and the
 * ObjAffine rotation/scale records. Every type and prototype used here is canonical
 * (bg.h, util.h, palette.h, sprite.h); the few local views kept for matching are listed
 * in build/readability/issues/gfx_util.md.
 */
#include "global.h"
#include "bg.h"       /* FillVramMapRect*, CopyMap*, SetVramMapTile, DrawVramMapNumber3, enum NumberDrawMode, GetTilemapOffset, UnpackCardArt8bpp, LoadCardArt8bpp */
#include "legacy/gba.h"      /* VRAM, PLTT, IWRAM, REG_BLDALPHA, REG_BLDY, CpuSet, Div */
#include "palette.h"  /* struct PalDelta, struct PalFade, struct PalFadeStrided, SetBldAlpha, SetBldY, PalFade_Start/Apply, PalFadeStrided_Start/Apply */
#include "sprite.h"   /* struct ObjAffine, ObjAffineInit, ObjAffineApply */
#include "util.h"     /* struct Timer, struct Ease, struct CallbackQueue, struct StepList, enum TickState, MulFix8, MulFix8Wide, DivFix8, ReciprocalFix8, MemCopy16, gSineTable */

/* Fills a w x h rectangle of BG screenblock `screenBlock` (at column x, row y) with
 * ascending tile numbers. Columns 0x20-0x3F continue in the next screenblock
 * (64-wide maps), columns >= 0x40 wrap back. */
void FillVramMapRectSeq(u16 tile, u8 screenBlock, u8 x, u8 y, u8 w, u8 h)
{
    u16 *p = (u16 *)(VRAM + screenBlock * 0x800 + x * 2 + y * 64);
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            s32 xx = x + j;

            if ((u32)(xx - 0x20) <= 0x1F) {
                *(u16 *)((u8 *)p + 0x7C0) = tile++;
            } else if (xx > 0x3F) {
                u16 *q;
                u16 value;
                q = (u16 *)((u8 *)p - 0x80);
                value = tile++;
                /* FAKEMATCH: keep the initialized old tile live through the
                 * increment so the wrap store has the ROM register order. */
                __asm__ volatile("" : : "r"(value));
                *q = value;
            } else {
                *p = tile++;
            }
            p++;
        }
        p += 0x20 - w;
    }
}
/* Fills a w x h rectangle (w even) with one tile using 32-bit stores. */
void FillVramMapRect32(u16 tile, u8 screenBlock, u8 x, u8 y, u8 w, u8 h)
{
    u32 *p = (u32 *)(VRAM + screenBlock * 0x800 + x * 2 + y * 64);
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w / 2; j++) {
            s32 xx = x + j * 2;

            if ((u32)(xx - 0x20) <= 0x1F) {
                *(u32 *)((u8 *)p + 0x7C0) = tile | tile << 16;
                p++;
            } else if (xx > 0x3F) {
                *(u32 *)((u8 *)p - 0x80) = tile | tile << 16;
                p++;
            } else {
                *p++ = tile | tile << 16;
            }
        }
        p += 0x10 - w / 2;
    }
}
/* Same as FillVramMapRectSeq but every entry gets the same tile. */
void FillVramMapRect(u16 tile, u8 screenBlock, u8 x, u8 y, u8 w, u8 h)
{
    u16 *p = (u16 *) (((VRAM + (screenBlock * 0x800)) + (x * 2)) + (y * 64));
    u8 i;
    u8 j;
    u32 next;
    for (i = 0; i < h; i = next)
    {
      for (j = 0; j < w; j++)
      {
        s32 xx = x + j;
        if (((u32) (xx - 0x20)) <= 0x1F)
        {
          *((u16 *) (((u8 *) p) + 0x7C0)) = tile;
        }
        else
          if (xx > 0x3F)
        {
          *((u16 *) (((u8 *) p) - 0x80)) = tile;
        }
        /* FAKEMATCH: duplicate store preserves separate wrap branches. */
        else
          if (j)
        {
          *p = tile;
        }
        else
        {
          *p = tile;
        }
        p++;
      }

      p += 0x20 - w;
      next = i + 1;
    }

} /* 0x0807A808 size 0x90 */
/* Fills a w x h rectangle at dst (32-entry rows) with ascending tile
 * numbers (10 bits) combined with a palette nibble. */
void FillMapRectSeqPal(u16 *dst, u16 tile, u8 pal, u8 w, u8 h)
{
    u8 i;
    u8 j;
    u8 nib;

    tile &= 0x3FF;
    nib = pal & 0xF;
    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *dst++ = tile++ | nib << 12;
        }
        dst += 0x20 - w;
    }
}
/* Copies h rows of w halfwords from a packed source to a 32-entry-wide map. */
void CopyMapRect(const void *src, void *dst, u8 w, u8 h)
{
    u8 i;

    for (i = 0; i < h; i++) {
        s32 size = w * 2;

        CpuSet(src, dst, (size / 2) & 0x1FFFFF);
        src = (const u8 *)src + size;
        dst = (u8 *)dst + 0x40;
    }
}
/* Same with an explicit destination row stride (in halfwords). */
void CopyMapRectStride(const void *src, void *dst, u8 w, u8 h, u8 dstStride)
{
    u8 i;

    for (i = 0; i < h; i++) {
        u32 size = w * 2;

        CpuSet(src, dst, (size / 2) & 0x1FFFFF);
        src = (const u8 *)src + size;
        dst = (u8 *)dst + dstStride * 2;
    }
}
/* Copies a w x h rectangle between maps, adding a palette nibble and a
 * high tile-number offset to every entry. */
void CopyMapRectAddOffset(u16 *src, u16 *dst, u8 w, u8 h, u8 srcStride, u8 pal, u8 tileHi)
{
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *dst = *src + (pal << 12) + (tileHi << 8);
            src++;
            dst++;
        }
        src += srcStride - w;
        dst += 0x20 - w;
    }
}
/* Like CopyMapRectAddOffset but keeps only the low 10 bits of the source tile. */
void CopyMapRectSetPalette(u16 *src, u16 *dst, u8 w, u8 h, u8 srcStride, u8 pal, u8 tileHi)
{
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *dst = (*src & 0x3FF) | (pal << 12) | (tileHi << 8);
            src++;
            dst++;
        }
        src += srcStride - w;
        dst += 0x20 - w;
    }
}
/* Copies h rows of w entries from a wide map to a 32-wide map, taking the
 * column modulo 32 and wrapping the destination every 32 rows. */
void CopyMapRectWrapped(u16 *src, u16 *dst, u8 w, u8 h, u16 srcX, u16 dstRow, u8 srcStride)
{
    u16 i;
    u16 j;
    u32 row = (u8)dstRow;

    for (i = 0; i < h; i++) {
        for (j = 0, row++; j < w; j++) {
            dst[(srcX + j) & 0x1F] = src[srcX + j];
        }
        src += srcStride;
        if ((row = (u8)row) == 0x20) {
            dst -= 0x3E0;
            row = 0;
        } else {
            dst += 0x20;
        }
    }
}
void CopyMapRectRemapTiles16To32(u16 *src, u16 *dst, u8 w, u8 h)
{
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *dst = (*src & 0xFC0F) | (*src & 0x3F0) << 1;
            src++;
            dst++;
        }
        dst += 0x20 - w;
    }
}
/* Sets the palette nibble of every entry in a w x h rectangle. */
void SetMapRectPalette(u16 *dst, u8 w, u8 h, u8 pal)
{
    u8 i;
    u8 j;
    u16 bits = (pal & 0xF) << 12;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *dst = (*dst & 0xFFF) | bits;
            dst++;
        }
        dst += 0x20 - w;
    }
}
void SetVramMapTile(u8 screenBlock, u8 x, u8 y, u8 tile)
{
    u16 *p = (u16 *)((x + y * 32) * 2 + screenBlock * 0x800 + VRAM);

    *p = (*p & 0xFC00) | tile;
}
/* Draws a 3-digit decimal number right to left starting at (x, y).
 * NUMBER_DRAW_ALL_DIGITS: always draws 3 digits; NUMBER_DRAW_SKIP_ZEROS: skips zero digits. */
void DrawVramMapNumber3(u8 screenBlock, u16 digitTile, u8 x, u8 y, u16 num, u8 pal, u32 unused, u8 mode)
{
    u16 *p = (u16 *)((x + y * 32) * 2 + screenBlock * 0x800 + VRAM);
    u32 ten = 10;
    u8 i;

    switch (mode) {
    case NUMBER_DRAW_ALL_DIGITS: {
        u32 bits;

        for (i = 0, bits = pal << 12; i < 3; i++) {
            *p = (num % ten + digitTile) | bits;
            p--;
            num /= ten;
        }
        break;
    }
    case NUMBER_DRAW_SKIP_ZEROS: {
        u32 bits;

        for (i = 0, bits = pal << 12; i < 3; i++) {
            u16 d = num % ten;

            if (d != 0) {
                *p = (d + digitTile) | bits;
                p--;
            }
            num /= ten;
        }
        break;
    }
    }
}
/* Copies a w x h block of halfword rows between two bitmaps; the byte offset
 * of a (x, y) position comes from GetTilemapOffset(x, y, shift). The u32 return
 * type (no value) gives the `pop {r1}` epilogue. */
u32 CopyMapBlock(u16 *srcMap, u16 sx, u16 sy, u16 srcStride, u16 *dstMap, u16 dx, u16 dy, u8 w, u8 h, u8 shift)
{
    u16 *src = srcMap + GetTilemapOffset(sx, sy, shift) / 2;
    u16 *dst = dstMap + GetTilemapOffset(dx, dy, shift) / 2;
    u8 i;

    for (i = 0; i < h; i++) {
        s32 size = w * 2;
        CpuSet(src, dst, (size / 2) & 0x1FFFFF);
        src += srcStride;
        dst += 0x20;
    }
}
/* Same for a source that is a plain srcStride-wide halfword array. */
u32 CropMapBlock(u16 *srcMap, u16 sx, u16 sy, u16 srcStride, u16 *dstMap, u16 dx, u16 dy, u8 w, u8 h, u8 shift)
{
    u16 *src = srcMap + sx + sy * srcStride;
    u16 *dst = dstMap + GetTilemapOffset(dx, dy, shift) / 2;
    u8 i;

    for (i = 0; i < h; i++) {
        s32 size = w * 2;
        CpuSet(src, dst, (size / 2) & 0x1FFFFF);
        src += srcStride;
        dst += 0x20;
    }
}
/* Packs pairs of halfwords into bytes: dst[i] = lo | hi << 8. */
void PackMapRectBytes(u16 *src, u16 *dst, u8 w, u8 h)
{
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *dst++ = (src[0] & 0xFF) | (src[1] & 0xFF) << 8;
            src += 2;
        }
        dst += 0x20 - w;
    }
}
/* Twin of BattleScene_LoadCardArt (portrait loader): palette cardId -> PLTT + (pal >> 4) * 32 with
 * pal = page * 64 + 0x80, unpacks record cardId's 720 x 3 halfwords of packed 6-bit pixels to one
 * pixel per byte at dst, then adds (pal & 0xFF) to all 0xB40 halfwords. The ROM tables are
 * integer addresses so that both bases are rematerialized by reload (as in BattleScene_LoadCardArt). */
void UnpackCardArt8bpp(u16 cardId, u32 dst, u16 page)
{
    u16 pal = page * 64 + 0x80;
    const u16 *src;
    u16 *out;
    int n;
    /* FAKEMATCH: the ROM keeps the second loop's counter in ip (r2/r3 hold 0x3F3F/0xB3F);
     * no ordinary form reproduced that allocation, so the counter is pinned. With the pin
     * the loop must be a do-while (a pinned `for` keeps its entry test), and the bound is a
     * local set before the loop so its literal is loaded first, as in the ROM. */
    register u32 i asm("ip");
    u32 lim;
    u16 m6, m12;

    MemCopy16((void *)(PLTT + (pal >> 4) * 0x20), (const void *)(0x08608360 + cardId * 0x80), 0x80);
    src = (const u16 *)(0x082A6500 + cardId * 0x10E0);
    out = (u16 *)dst;
    m6 = 0x3F;
    m12 = 0xFC0;
    for (n = 720; n != 0; n--) {
        u16 s0 = src[0];
        u32 s1 = src[1];
        u32 s2 = src[2];
        u16 t, x;
        out[0] = (s0 & m6) | ((s0 & m12) << 2);
        out[1] = (s0 >> 12) | ((s1 & 3) << 4) | ((s1 & 0xFC) * 64);
        t = s1 >> 8;
        out[2] = (t & m6) | (((t >> 6) | ((s2 & 0xF) << 2)) << 8);
        x = s2 >> 4;
        out[3] = (x & m6) | ((x & m12) << 2);
        src += 3;
        out += 4;
    }
    i = 0;
    lim = 0xB3F;
    do {
        *(u16 *)dst = (*(u16 *)dst & 0x3F3F) + ((u8)pal << 8 | (u8)pal);
        dst += 2;
        i++;
    } while (i <= lim);
}
void LoadCardArt8bpp(u16 cardId, u32 dst, u16 page)
{
    UnpackCardArt8bpp(cardId, dst, page);
}
/* 4-slot callback queue: slot[i] is a function returning non-zero when done (util.h). */
void CallbackQueue_Init(struct CallbackQueue *q)
{
    u8 i;

    for (i = 0; i < 4; i++) {
        q->slot[i] = NULL;
    }
}
u8 CallbackQueue_Add(u16 (*fn)(void), struct CallbackQueue *q)
{
    u8 head = q->head;
    u8 ret;

    if (q->slot[head] != NULL) {
        ret = 0;
    } else {
        q->slot[++q->head & 3] = fn;
        ret = head;
    }
    return ret;
}
void CallbackQueue_Run(struct CallbackQueue *q)
{
    u8 i;

    for (i = 0; i < 4; i++) {
        if (q->slot[i] != NULL) {
            if (q->slot[i]()) {
                q->slot[i] = NULL;
            }
        }
    }
}
/* Sequential list of callbacks: run entry [idx]; advance when it returns non-zero (util.h). */
void StepList_Init(u16 (**steps)(void), struct StepList *l)
{
    l->idx = 0;
    l->steps = steps;
}
u32 StepList_Run(struct StepList *l)
{
    if (l->steps[l->idx] != NULL) {
        if (l->steps[l->idx]()) {
            l->idx++;
        }
        return 0;
    }
    return 1;
}
/* Countdown timer: state 0 idle, 1 running, 2 finished (enum TickState, util.h). */
void Timer_Reset(struct Timer *t)
{
    t->state = TICK_IDLE;
    t->count = 0;
}
void Timer_Start(struct Timer *t, u16 count)
{
    t->state = TICK_RUNNING;
    t->count = count;
}
void Timer_Tick(struct Timer *t)
{
    if (t->state == TICK_RUNNING) {
        t->count--;
        if (t->count == 0) {
            t->state = TICK_DONE;
        }
    }
}
/* Linear interpolator: cur moves by step each tick until it reaches end (util.h). */
void Ease_Init(u16 cur, u16 end, s16 step, struct Ease *e)
{
    e->state = TICK_IDLE;
    e->cur = cur;
    e->end = end;
    e->step = step;
}
void Ease_Start(u16 cur, u16 end, s16 step, struct Ease *e)
{
    e->state = TICK_RUNNING;
    e->cur = cur;
    e->end = end;
    e->step = step;
}
void Ease_Tick(struct Ease *e)
{
    if (e->state != TICK_RUNNING) {
        return;
    }
    e->cur += e->step;
    if ((s16)e->step > 0) {
        if ((s16)e->cur >= (s16)e->end) {
            e->state = TICK_DONE;
            e->cur = e->end;
        }
    } else if ((s16)e->cur <= (s16)e->end) {
        e->state = TICK_DONE;
        e->cur = e->end;
    }
}
/* Palette fade toward a target colour (BGR555). startColors[i] is the working copy of
 * colour i; delta[i] holds the per-channel distance to the target. `step`
 * (0..0x20) scales the deltas; the caller advances it (palette.h). */
void PalFade_Start(u16 *pal, u16 count, u16 color, struct PalFade *f)
{
    u16 i;

    f->dst = pal;
    for (i = 0; i < count; i++) {
        f->startColors[i] = *pal;
        pal++;
        f->delta[i].r = (color & 0x1F) - (f->startColors[i] & 0x1F);
        f->delta[i].g = ((color & 0x3E0) >> 5) - ((f->startColors[i] & 0x3E0) >> 5);
        f->delta[i].b = ((color & 0x7C00) >> 10) - ((f->startColors[i] & 0x7C00) >> 10);
    }
    f->state = TICK_RUNNING;
    f->count = count;
    f->step = 0;
}
void PalFade_Apply(struct PalFade *f)
{
    u16 *dst = f->dst;

    if (f->state == TICK_RUNNING) {
        u16 i;

        for (i = 0; i < f->count; i++) {
            *dst = (((f->startColors[i] & 0x1F) + (f->delta[i].r * f->step >> 5)) & 0x1F)
                 | (((f->startColors[i] & 0x3E0) + f->delta[i].g * f->step) & 0x3E0)
                 | (((f->startColors[i] & 0x7C00) + (f->delta[i].b * f->step << 5)) & 0x7C00);
            dst++;
        }
        if (f->step == 0x20) {
            f->state = TICK_DONE;
        }
    }
}
/* Small variant of PalFade (at most 8 colours, strided source). */
void PalFadeStrided_Start(u16 *pal, u16 count, u8 stride, u16 color, struct PalFadeStrided *f)
{
    u16 j = 0;
    u16 i;

    f->dst = pal;
    for (i = 0; i < count; i++) {
        f->startColors[i] = pal[j];
        f->delta[i].r = (color & 0x1F) - (pal[j] & 0x1F);
        f->delta[i].g = ((color & 0x3E0) >> 5) - ((f->startColors[i] & 0x3E0) >> 5);
        f->delta[i].b = ((color & 0x7C00) >> 10) - ((f->startColors[i] & 0x7C00) >> 10);
        j = j + stride;
    }
    f->state = TICK_RUNNING;
    f->count = count;
    f->step = 0;
    f->stride = stride;
}
void PalFadeStrided_Apply(struct PalFadeStrided *f)
{
    u16 *dst = f->dst;
    u16 idx = 0;

    if (f->state == TICK_RUNNING) {
        u16 i;

        if (f->step > 0x1F) {
            f->step = 0x20;
            f->state = TICK_DONE;
        }
        for (i = 0; i < f->count; i++) {
            dst[idx] = (((f->startColors[i] & 0x1F) + (f->delta[i].r * f->step >> 5)) & 0x1F)
                     | (((f->startColors[i] & 0x3E0) + f->delta[i].g * f->step) & 0x3E0)
                     | (((f->startColors[i] & 0x7C00) + (f->delta[i].b * f->step << 5)) & 0x7C00);
            idx = f->stride + idx;
        }
    }
}
void SetBldAlpha(u16 level)
{
    REG_BLDALPHA = (level << 8) | (0x10 - level);
}
void SetBldY(u16 level)
{
    REG_BLDY = level;
}
s16 MulFix8(s16 a, s16 b)
{
    return a * b >> 8;
}
s32 MulFix8Wide(s32 a, s32 b)
{
    long long p = (long long)a * b;

    return (s32)(p >> 8);
}
/* Callers supply words; the original explicitly decodes signed halfwords
 * and sign-extends the narrowed quotient on return. */
int DivFix8(int aWord, int bWord)
{
    s16 a = aWord;
    s16 b = bWord;
    return (s16)Div(a * 256, b);
}
s16 ReciprocalFix8(s16 a)
{
    return Div(0x10000, a);
}
/* Rotation/scale objects (sprite.h): scale, angle and pointers to the four OAM affine
 * parameter slots (pa, pb, pc, pd) each record drives. 32 of them, stride 0x18. */
void ObjAffineInit(struct ObjAffine *a)
{
    u8 i;
    u8 j;
    s16 **pp;

    i = 0;
    pp = a->param;
    for (; i < 32; i++) {
        for (j = 0; j < 4; j++) {
            pp[(j * 4 + i * 24) >> 2] = (s16 *)(IWRAM + 0x4476 + (i * 4 + j) * 8);
        }
        a[i].scaleX = 0x100;
        a[i].scaleY = 0x100;
        a[i].angle = 0;
    }
}
/* ---- Local views kept for matching (build/readability/issues/gfx_util.md) ---- */

/* The originals are called through int-typed declarations (no s16
 * re-extension at the call site); asm labels reproduce that. */
extern int Reciprocal(int) asm("ReciprocalFix8");
extern int MulFix(int, int) asm("MulFix8");

void ObjAffineApply(struct ObjAffine *a)
{
    *a->param[0] = MulFix(Reciprocal(a->scaleX), gSineTable[(a->angle >> 8) + 0x40]);
    *a->param[1] = MulFix(Reciprocal(a->scaleX), gSineTable[a->angle >> 8]);
    *a->param[2] = MulFix(Reciprocal(a->scaleY), -gSineTable[a->angle >> 8]);
    *a->param[3] = MulFix(Reciprocal(a->scaleY), gSineTable[(a->angle >> 8) + 0x40]);
}
void SetBgAffineRefPoint(u8 bg, s32 x, s32 y, s32 cx, s32 cy, struct ObjAffine *a)
{
    s32 sx;
    s32 sy;

    sx = MulFix8Wide(*a->param[0], x -= cx) + MulFix8Wide(*a->param[1], y -= cy) + cx;
    sy = MulFix8Wide(*a->param[2], x) + MulFix8Wide(*a->param[3], y) + cy;
    switch (bg) {
    case 2:
        *(vu32 *)0x04000028 = sx;
        *(vu32 *)0x0400002C = sy;
        break;
    case 3:
        *(vu32 *)0x04000038 = sx;
        *(vu32 *)0x0400003C = sy;
        break;
    }
}
