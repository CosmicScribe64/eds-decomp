#include "global.h"

#include "gba.h"

/*
 * Tilemap / palette / easing utility library (0x0807A6AC-0x0807B6B8): BG
 * screenblock rectangle fills and copies (32-wide blocks, with the 64-wide
 * screenblock wrap), palette fades, a 4-slot callback queue, linear-step
 * counters, fixed-point helpers. See wiki/functions/code-0807a6ac.md.
 */

/* Fills a w x h rectangle of BG screenblock `bg` (at column x, row y) with
 * ascending tile numbers. Columns 0x20-0x3F continue in the next screenblock
 * (64-wide maps), columns >= 0x40 wrap back. */
void sub_0807A6AC(u16 tile, u8 bg, u8 x, u8 y, u8 w, u8 h)
{
    u16 *p = (u16 *)(VRAM + bg * 0x800 + x * 2 + y * 64);
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
void sub_0807A754(u16 tile, u8 bg, u8 x, u8 y, u8 w, u8 h)
{
    u32 *p = (u32 *)(VRAM + bg * 0x800 + x * 2 + y * 64);
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
/* Same as sub_0807A6AC but every entry gets the same tile. */
void sub_0807A808(u16 tile, u8 bg, u8 x, u8 y, u8 w, u8 h)
{
    u16 *p = (u16 *) (((0x06000000 + (bg * 0x800)) + (x * 2)) + (y * 64));
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
void sub_0807A898(u16 *dst, u16 tile, u8 pal, u8 w, u8 h)
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
void sub_0807A908(void *src, void *dst, u8 w, u8 h)
{
    u8 i;

    for (i = 0; i < h; i++) {
        s32 size = w * 2;

        CpuSet(src, dst, (size / 2) & 0x1FFFFF);
        src = (u8 *)src + size;
        dst = (u8 *)dst + 0x40;
    }
}
/* Same with an explicit destination row stride (in halfwords). */
void sub_0807A960(void *src, void *dst, u8 w, u8 h, u8 stride)
{
    u8 i;

    for (i = 0; i < h; i++) {
        u32 size = w * 2;

        CpuSet(src, dst, (size / 2) & 0x1FFFFF);
        src = (u8 *)src + size;
        dst = (u8 *)dst + stride * 2;
    }
}
/* Copies a w x h rectangle between maps, adding a palette nibble and a
 * high tile-number offset to every entry. */
void sub_0807A9C0(u16 *src, u16 *dst, u8 w, u8 h, u8 srcW, u8 pal, u8 hi)
{
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *dst = *src + (pal << 12) + (hi << 8);
            src++;
            dst++;
        }
        src += srcW - w;
        dst += 0x20 - w;
    }
}
/* Like sub_0807A9C0 but keeps only the low 10 bits of the source tile. */
void sub_0807AA4C(u16 *src, u16 *dst, u8 w, u8 h, u8 srcW, u8 pal, u8 hi)
{
    u8 i;
    u8 j;

    for (i = 0; i < h; i++) {
        for (j = 0; j < w; j++) {
            *dst = (*src & 0x3FF) | (pal << 12) | (hi << 8);
            src++;
            dst++;
        }
        src += srcW - w;
        dst += 0x20 - w;
    }
}
/* Copies h rows of w entries from a wide map to a 32-wide map, taking the
 * column modulo 32 and wrapping the destination every 32 rows. */
void sub_0807AAE4(u16 *src, u16 *dst, u8 w, u8 h, u16 srcX, u16 dstRow, u8 srcStride)
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
void sub_0807AB8C(u16 *src, u16 *dst, u8 w, u8 h)
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
void sub_0807AC00(u16 *dst, u8 w, u8 h, u8 pal)
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
void sub_0807AC5C(u8 bg, u8 x, u8 y, u8 tile)
{
    u16 *p = (u16 *)((x + y * 32) * 2 + bg * 0x800 + VRAM);

    *p = (*p & 0xFC00) | tile;
}
/* Draws a 3-digit decimal number right to left starting at (x, y).
 * mode 0: always draws 3 digits; mode 1: skips zero digits. */
void sub_0807AC88(u8 bg, u16 base, u8 x, u8 y, u16 num, u8 pal, u32 unused, u8 mode)
{
    u16 *p = (u16 *)((x + y * 32) * 2 + bg * 0x800 + VRAM);
    u32 ten = 10;
    u8 i;

    switch (mode) {
    case 0: {
        u32 bits;

        for (i = 0, bits = pal << 12; i < 3; i++) {
            *p = (num % ten + base) | bits;
            p--;
            num /= ten;
        }
        break;
    }
    case 1: {
        u32 bits;

        for (i = 0, bits = pal << 12; i < 3; i++) {
            u16 d = num % ten;

            if (d != 0) {
                *p = (d + base) | bits;
                p--;
            }
            num /= ten;
        }
        break;
    }
    }
}
u32 sub_0807A490(u16 x, u16 y, u8 shift);

/* Copies a w x h block of halfword rows between two bitmaps; the byte offset
 * of a (x, y) position comes from sub_0807A490(x, y, shift). */
#if 0 /* NONMATCHING: allocation and CSE differ. The target spills srcBase to
       * [sp] (keeps srcW in r9 and w in r7, reloads the 0xFFFE mask from the
       * literal pool for each call, and builds 0x1FFFFF inside the loop). The
       * build keeps srcBase in r7, spills srcW and CSEs the 0xFFFE mask. All
       * source-order and type variants tried leave the same allocation split. */
void sub_0807AD40(void *srcBase, u16 sx, u16 sy, u16 srcW, void *dstBase, u16 dx, u16 dy, u8 w, u8 h, u8 shift)
{
    u8 *src = (u8 *)srcBase + (sub_0807A490(sx, sy, shift) & 0xFFFE);
    u8 *dst = (u8 *)dstBase + (sub_0807A490(dx, dy, shift) & 0xFFFE);
    s16 i;

    for (i = 0; h > i; i++) {
        CpuSet(src, dst, w & 0x1FFFFF);
        src += srcW * 2;
        dst += 0x40;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807A6AC", sub_0807AD40); /* 0x0807AD40 size 0xA8 */
/* Same for a source that is a plain srcW-wide halfword array. */
#if 0 /* NONMATCHING: the same allocation and CSE split as sub_0807AD40. The
       * target spills srcW to [sp], keeps w in r7 and h in sl, keeps the
       * `w & 0x1FFFFF` mask (0x1FFFFF reloaded each iteration) and recomputes
       * srcW*2. The build keeps srcW in sl, spills w and folds the mask away.
       * All variants tried keep the same split. */
void sub_0807ADE8(u16 *srcBase, u16 sx, u16 sy, u16 srcW, void *dstBase, u16 dx, u16 dy, u8 w, u8 h, u8 shift)
{
    u16 *src = srcBase + sx + sy * srcW;
    u8 *dst = (u8 *)dstBase + (sub_0807A490(dx, dy, shift) & 0xFFFE);
    s16 i;

    for (i = 0; i < h; i++) {
        CpuSet(src, dst, w & 0x1FFFFF);
        src += srcW;
        dst += 0x40;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807A6AC", sub_0807ADE8); /* 0x0807ADE8 size 0xA0 */
/* Packs pairs of halfwords into bytes: dst[i] = lo | hi << 8. */
void sub_0807AE88(u16 *src, u16 *dst, u8 w, u8 h)
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
extern const u8 gUnk_08608360[];
extern const u16 gUnk_082A6500[];

/* Unpacks a 6-bit-per-pixel image (idx-th 0x10E0-byte record at 0x082A6500)
 * into 8-bit pixels at dst and loads its 64 colour palette to OBJ/BG palette
 * bank `bank`; the top two pixel bits select the sub-palette. */
#if 0 /* NONMATCHING: the 0x3F/0xFC0 masks do need to be locals (`u16 m6/m12`)
       * to hoist into r8/r9 as the target does (bare literals let the first
       * use stay an immediate). About 80 instruction lines of register
       * allocation in the two loops still differ: the target keeps the pixel
       * scratch in r0/r1/r2 and the second-loop counter in ip, while the build
       * rotates them. */
void sub_0807AEF0(u16 idx, u32 dstAddr, u16 bank)
{
    u16 *dst = (u16 *)dstAddr;
    u16 *in;
    u16 *out;
    s32 n;
    u32 pal = ((u32)bank << 22) + 0x800000 >> 16;
    s8 m6;
    s8 m12;
    s16 i;

    sub_08075294((void *)(PLTT + (((u32)bank << 22) + 0x800000 >> 15)), gUnk_08608360 + idx * 0x80, 0x80);
    m6 = 0x3F;
    m12 = 0xFC0;
    in = gUnk_082A6500 + idx * 0x870;
    out = dst;
    for (n = 0x2D0; n != 0; n--) {
        u16 a = in[0];
        u16 b = in[1];
        int c = in[2];

        out[0] = (a & m6) | (a & m12) << 2;
        out[1] = a >> 12 | (b & 3) << 4 | (b & 0xFC) << 6;
        out[2] = (b >> 8 & m6) | ((b >> 8 >> 6) | (c & 0xF) << 2) << 8;
        out[3] = (c >> 4 & m6) | (c >> 4 & m12) << 2;
        in += 3;
        out += 4;
    }
    for (i = 0; i < 0xB40; i++) {
        *dst = (*dst & 0x3F3F) + ((u8)pal << 8 | (u8)pal);
        dst++;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0807A6AC", sub_0807AEF0); /* 0x0807AEF0 size 0x10C */
void sub_0807AEF0(u16 a, u32 b, u16 c);

void sub_0807AFFC(u16 a, u32 b, u16 c)
{
    sub_0807AEF0(a, b, c);
}
/* 4-slot callback queue: slot[i] is a function returning non-zero when done. */
struct CallbackQueue {
    u8 head;                        /* +0x00 */
    u8 pad[3];
    u16 (*slot[4])(void);           /* +0x04 */
};

void sub_0807B010(struct CallbackQueue *q)
{
    u8 i;

    for (i = 0; i < 4; i++) {
        q->slot[i] = NULL;
    }
}
u8 sub_0807B028(u16 (*fn)(void), struct CallbackQueue *q)
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
void sub_0807B058(struct CallbackQueue *q)
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
/* Sequential list of callbacks: run entry [idx]; advance when it returns non-zero. */
struct CallbackList {
    u8 idx;                         /* +0x00 */
    u16 (**table)(void);            /* +0x04 */
};

void sub_0807B088(u16 (**table)(void), struct CallbackList *l)
{
    l->idx = 0;
    l->table = table;
}
u32 sub_0807B090(struct CallbackList *l)
{
    if (l->table[l->idx] != NULL) {
        if (l->table[l->idx]()) {
            l->idx++;
        }
        return 0;
    }
    return 1;
}
/* Countdown timer: state 0 idle, 1 running, 2 finished. */
struct Timer {
    u8 state;                       /* +0x00 */
    u16 count;                      /* +0x02 */
};

void sub_0807B0C0(struct Timer *t)
{
    t->state = 0;
    t->count = 0;
}
void sub_0807B0C8(struct Timer *t, u16 count)
{
    t->state = 1;
    t->count = count;
}
void sub_0807B0D0(struct Timer *t)
{
    if (t->state == 1) {
        t->count--;
        if (t->count == 0) {
            t->state = 2;
        }
    }
}
/* Linear interpolator: cur moves by step each tick until it reaches end. */
struct Ease {
    u8 state;                       /* +0x00 0 idle, 1 running, 2 done */
    u16 cur;                        /* +0x02 */
    u16 end;                        /* +0x04 */
    s16 step;                       /* +0x06 */
};

void sub_0807B0EC(u16 cur, u16 end, s16 step, struct Ease *e)
{
    e->state = 0;
    e->cur = cur;
    e->end = end;
    e->step = step;
}
void sub_0807B100(u16 cur, u16 end, s16 step, struct Ease *e)
{
    e->state = 1;
    e->cur = cur;
    e->end = end;
    e->step = step;
}
void sub_0807B114(struct Ease *e)
{
    if (e->state != 1) {
        return;
    }
    e->cur += e->step;
    if ((s16)e->step > 0) {
        if ((s16)e->cur >= (s16)e->end) {
            e->state = 2;
            e->cur = e->end;
        }
    } else if ((s16)e->cur <= (s16)e->end) {
        e->state = 2;
        e->cur = e->end;
    }
}
/* Palette fade toward a target colour (BGR555). cur[i] is the working copy of
 * colour i; delta[i] holds the per-channel distance to the target. `step`
 * (0..0x20) scales the deltas; the caller advances it. */
struct PalDelta {
    s8 r;
    s8 g;
    s8 b;
    s8 pad;
};

struct PalFade {
    u16 cur[0x200];                 /* +0x000 */
    struct PalDelta delta[0x200];   /* +0x400 */
    u8 step;                        /* +0xC00 */
    u8 pad0C01;
    u16 count;                      /* +0xC02 */
    u16 *dst;                       /* +0xC04 palette RAM being faded */
    u16 state;                      /* +0xC08 1 running, 2 done */
};

void sub_0807B150(u16 *pal, u16 count, u16 color, struct PalFade *f)
{
    u16 i;

    f->dst = pal;
    for (i = 0; i < count; i++) {
        f->cur[i] = *pal;
        pal++;
        f->delta[i].r = (color & 0x1F) - (f->cur[i] & 0x1F);
        f->delta[i].g = ((color & 0x3E0) >> 5) - ((f->cur[i] & 0x3E0) >> 5);
        f->delta[i].b = ((color & 0x7C00) >> 10) - ((f->cur[i] & 0x7C00) >> 10);
    }
    f->state = 1;
    f->count = count;
    f->step = 0;
}
void sub_0807B224(struct PalFade *f)
{
    u16 *dst = f->dst;

    if (f->state == 1) {
        u16 i;

        for (i = 0; i < f->count; i++) {
            *dst = (((f->cur[i] & 0x1F) + (f->delta[i].r * f->step >> 5)) & 0x1F)
                 | (((f->cur[i] & 0x3E0) + f->delta[i].g * f->step) & 0x3E0)
                 | (((f->cur[i] & 0x7C00) + (f->delta[i].b * f->step << 5)) & 0x7C00);
            dst++;
        }
        if (f->step == 0x20) {
            f->state = 2;
        }
    }
}
/* Small variant of PalFade (at most 8 colours, strided source). */
struct PalFadeSmall {
    u16 cur[8];                     /* +0x00 */
    struct PalDelta delta[8];       /* +0x10 */
    u8 step;                        /* +0x30 0..0x20 */
    u8 pad31;
    u16 count;                      /* +0x32 */
    u16 *dst;                       /* +0x34 */
    u8 stride;                      /* +0x38 palette entries between the faded colours */
    u8 pad39;
    u16 state;                      /* +0x3A 1 running, 2 done */
};

void sub_0807B31C(u16 *pal, u16 count, u8 stride, u16 color, struct PalFadeSmall *f)
{
    u16 j = 0;
    u16 i;

    f->dst = pal;
    for (i = 0; i < count; i++) {
        f->cur[i] = pal[j];
        f->delta[i].r = (color & 0x1F) - (pal[j] & 0x1F);
        f->delta[i].g = ((color & 0x3E0) >> 5) - ((f->cur[i] & 0x3E0) >> 5);
        f->delta[i].b = ((color & 0x7C00) >> 10) - ((f->cur[i] & 0x7C00) >> 10);
        j = j + stride;
    }
    f->state = 1;
    f->count = count;
    f->step = 0;
    f->stride = stride;
}
void sub_0807B3DC(struct PalFadeSmall *f)
{
    u16 *dst = f->dst;
    u16 idx = 0;

    if (f->state == 1) {
        u16 i;

        if (f->step > 0x1F) {
            f->step = 0x20;
            f->state = 2;
        }
        for (i = 0; i < f->count; i++) {
            dst[idx] = (((f->cur[i] & 0x1F) + (f->delta[i].r * f->step >> 5)) & 0x1F)
                     | (((f->cur[i] & 0x3E0) + f->delta[i].g * f->step) & 0x3E0)
                     | (((f->cur[i] & 0x7C00) + (f->delta[i].b * f->step << 5)) & 0x7C00);
            idx = f->stride + idx;
        }
    }
}
void sub_0807B4A8(u16 a)
{
    REG_BLDALPHA = (a << 8) | (0x10 - a);
}
void sub_0807B4C0(u16 a)
{
    REG_BLDY = a;
}
s16 sub_0807B4D0(s16 a, s16 b)
{
    return a * b >> 8;
}
s32 sub_0807B4E0(s32 a, s32 b)
{
    long long p = (long long)a * b;

    return (s32)(p >> 8);
}
/* Callers supply words; the original explicitly decodes signed halfwords
 * and sign-extends the narrowed quotient on return. */
int sub_0807B504(int aWord, int bWord)
{
    s16 a = aWord;
    s16 b = bWord;
    return (s16)Div(a * 256, b);
}
s16 sub_0807B51C(s16 a)
{
    return Div(0x10000, a);
}
/* Rotation/scale object: scale, angle and pointers to the four OAM affine
 * parameter slots (pa, pb, pc, pd) it drives. 32 of them, stride 0x18. */
struct ObjAffine {
    s16 scaleX;                     /* +0x00 8.8 fixed point */
    s16 scaleY;                     /* +0x02 */
    u16 angle;                      /* +0x04 high byte = 256-step angle */
    u16 pad06;
    s16 *param[4];                  /* +0x08 */
};

void sub_0807B534(struct ObjAffine *a)
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
extern const s16 gUnk_08087BA4[];   /* sine table, 256 steps per turn, 8.8 fixed point */

/* The originals are called through int-typed declarations (no s16
 * re-extension at the call site); asm labels reproduce that. */
extern int Reciprocal(int) asm("sub_0807B51C");
extern int MulFix(int, int) asm("sub_0807B4D0");

void sub_0807B5A0(struct ObjAffine *a)
{
    *a->param[0] = MulFix(Reciprocal(a->scaleX), gUnk_08087BA4[(a->angle >> 8) + 0x40]);
    *a->param[1] = MulFix(Reciprocal(a->scaleX), gUnk_08087BA4[a->angle >> 8]);
    *a->param[2] = MulFix(Reciprocal(a->scaleY), -gUnk_08087BA4[a->angle >> 8]);
    *a->param[3] = MulFix(Reciprocal(a->scaleY), gUnk_08087BA4[(a->angle >> 8) + 0x40]);
}
void sub_0807B628(u8 bg, s32 x, s32 y, s32 cx, s32 cy, struct ObjAffine *a)
{
    s32 sx;
    s32 sy;

    sx = sub_0807B4E0(*a->param[0], x -= cx) + sub_0807B4E0(*a->param[1], y -= cy) + cx;
    sy = sub_0807B4E0(*a->param[2], x) + sub_0807B4E0(*a->param[3], y) + cy;
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
