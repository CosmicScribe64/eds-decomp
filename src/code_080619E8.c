#include "global.h"
#include "gba.h"

extern void sub_08075278(void *dst, u32 size);
extern void sub_08075294(void *dst, const void *src, u32 size);
struct DuelFlags {
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
    u8 pad1[3];
    u8 b4;                      /* +4 */
};
extern struct DuelFlags gUnk_0201CFB0;
struct ZonePos { s32 x; s32 y; };
extern struct ZonePos gUnk_081A42A4[2][16];
struct PlayerState { u8 pad0[2]; u8 b2; u8 pad3[0xD64 - 3]; };
extern struct PlayerState gUnk_020192E4[2];
extern s32 __divsi3(s32 a, s32 b);
extern const u16 gUnk_081A451C[8];
struct Misc15160 {
    u8 pad0[0x102];
    u16 w102[5];                /* +0x102 card numbers shown in the five slots (0xFFFF = none) */
    u8 pad10C_[0];
    u8 b10C[5];                 /* +0x10C slot kinds (0..0x17) */
    u8 pad111;
    u16 w112;                   /* +0x112 card number of the current card */
    u8 pad114[2];
    u16 w116;
    u16 w118;
    u16 w11A;
};
extern struct Misc15160 gUnk_02015160;
struct Main {
    u8 pad0[6];
    u16 vcount;                 /* +6 (REG_VCOUNT mirror is 0x04000006; see code) */
    u8 pad8[0x41C - 8];
    u16 bgMap[8][0x400];        /* +0x41C BG map buffers */
    u8 pad441C[0x4426 - 0x441C];
    u16 w4426;
    u8 pad4428[6];
    u16 w442E;
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_KIND(id) ((CARD_STATS(id) & 0xC0000) >> 18)

/* Card category (same inline as in code_08006878): 3/1 for card numbers 1910/1911-1912,
 * 7 Magic, 8 Trap, 9 Ticket, else the monster kind (0 normal, 1 effect, 2 fusion, 3 ritual). */
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)CARD_TYPE(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return CARD_KIND(id);
    }
}
#define gUnk_0808659C ((const u16 *)0x0808659C)   /* per-kind sprite attribute words */
#define gUnk_08623DF4 ((const u16 *)0x08623DF4)   /* card number to card id */
extern void sub_080766A4(int a, int y, int b, u32 tile);
extern int sub_08062140(u16 id);
extern int sub_08062354(int x);
extern s32 sub_0806236C(u32 a, u32 b, u32 c);

/* Turn off the OBJ layer of the duel screen and clear the OBJ tile memory. */
void sub_080619E8(void)
{
    gUnk_0201CFB0.bit1 = 0;
    REG_DISPCNT &= 0xFFBF;
    sub_08075278((void *)0x06010000, 0x10000);
}

/* Draw the card frame (by card type / class) into OBJ tile memory (hypothesis). */
#if 0 /* NONMATCHING: type/class switch and palette blit match exactly. Caching the three loop
       * constants in locals (trick 4: maskHi=0xFF00, addHi=0x1000, addLo=0x10, assigned before the
       * blit loops) reproduces the ROM hoists into sl/r8/r9, but gcc also hoists the 0xFF
       * low-byte mask into ip (the ROM rematerialises it with `mov #0xFF`) and then spills
       * (y+2)<<5. Tried constants as literals (only 0xFF00 hoisted), 64-bit temps (dead double
       * loads), goto pixel loops (trick 3: no 0xFF hoist, but the src/dst registers move), and
       * u16/u32/`!= 0`/`(u8)v` forms. The 0xFF hoist versus the ip scratch is the only
       * remaining diff. */
void sub_08061A1C(u16 id)
{
    u16 x;
    u16 y;
    u16 j;
    const u16 *t;
    const u16 *src;
    u16 *dst;
    s16 maskHi;
    u32 addLo;
    u16 addHi;
    switch ((u8)CARD_TYPE(id)) {
    case 0x15: t = (const u16 *)0x08631558; break;
    case 0x16: t = (const u16 *)0x0862EEC0; break;
    case 0x17: t = (const u16 *)0x08633BF0; break;
    default:
        switch (GetCardSubtype(id)) {
        case 1: t = (const u16 *)0x08627AF8; break;
        case 2: t = (const u16 *)0x0862A190; break;
        case 3: t = (const u16 *)0x0862C828; break;
        default: t = (const u16 *)0x08625460; break;
        }
        break;
    }
    src = (const u16 *)((const u8 *)t + 0x10 + t[0] * 2);
    sub_08075294((void *)0x05000220, (const u8 *)t + 8, 0x40);
    maskHi = 0xFF00;
    addLo = 0x10;
    addHi = 0x1000;
    for (y = 0; y <= 3; y++) {
        for (x = 0; x <= 0xC; x++) {
            dst = (u16 *)(0x06010000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                s16 v = *src;
                if (v & maskHi)
                    v += addHi;
                if (v & 0xFF)
                    v += addLo;
                *dst = v;
                src++;
                dst++;
            }
        }
    }
    for (y = 4; y <= 0xD; y++) {
        for (x = 0; x <= 1; x++) {
            dst = (u16 *)(0x06010000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                u16 v = *src;
                if (v & maskHi)
                    v += addHi;
                if (v & 0xFF)
                    v += addLo;
                *dst = v;
                src++;
                dst++;
            }
        }
        for (x = 0; x <= 1; x++) {
            dst = (u16 *)(0x06010000 + (((u16)((x + 0xB) << 1)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                s16 v = *src;
                if (v & maskHi)
                    v += addHi;
                if (v & 0xFF)
                    v += addLo;
                *dst = v;
                src++;
                dst++;
            }
        }
    }
    for (y = 0xE; y <= 0x11; y++) {
        for (x = 0; x <= 0xC; x++) {
            dst = (u16 *)(0x06010000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                u16 v = *src;
                if (v & maskHi)
                    v += addHi;
                if (v & 0xFF)
                    v += addLo;
                *dst = v;
                src++;
                dst++;
            }
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080619E8", sub_08061A1C); /* 0x08061A1C size 0x308 */
/* Load the picture `id` (6 bits per pixel, packed) into OBJ tile memory and its 64-colour palette (hypothesis). */
#if 0 /* NONMATCHING: pixel repack and loop shapes agree, but gcc hoists (y+2)<<5 out of the x loop and
       * 0xFC0 out of the pixel loop (the ROM recomputes both, keeping only 0x3F in r8, k in a stack
       * slot, p in ip and next=src+0x30 in r9). Tried reading operands at each use (trick 1) and
       * local src/dst/p splits. A 64-bit temp for 0xFC0 (trick 2) and a goto x loop (trick 3) both
       * worsened the stack frame and register allocation. */
void sub_08061D24(u16 id)
{
    u16 y;
    u16 x;
    u16 j;
    s16 k;
    const u16 *src;
    u16 *dst;
    u16 *q;
    src = (const u16 *)(0x08608360 + id * 0x80);
    dst = (u16 *)0x05000260;
    sub_08075294(dst, src, 0x80);
    src = (const u16 *)(0x082A6500 + id * 0x10E0);
    for (y = 4; y <= 0xD; y++) {
        for (x = 2; x <= 0xA; x++) {
            u16 *p;
            dst = (u16 *)(0x06010000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            p = dst;
            for (k = 7; k >= 0; k--) {
                u32 a = src[0];
                u16 b = src[1];
                u32 c = src[2];
                u32 t;
                dst[0] = (a & 0x3F) | ((a & 0xFC0) << 2);
                dst[1] = (a >> 12) | ((b & 3) << 4) | ((b & 0xFC) << 6);
                t = b >> 8;
                dst[2] = (t & 0x3F) | (((t >> 6) | ((c & 0xF) << 2)) << 8);
                dst[3] = ((c >> 4) & 0x3F) | (((c >> 4) & 0xFC0) << 2);
                src += 3;
                dst += 4;
            }
            q = p;
            for (j = 0; j <= 0x1F; j++) {
                *q = (*q & 0x3F3F) + 0x3030;
                q++;
            }
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080619E8", sub_08061D24); /* 0x08061D24 size 0x130 */
extern void sub_0806196C(u32 yx, u16 palBase, const u16 *src, const void *pal);
extern void sub_080618C4(u32 yx, u16 palBase, const u16 *src, const void *pal);

static inline int GetCardLevel(u16 id)
{
    int result;
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        result = 0; break;
    case 0x18:
        result = 10; break;
    default:
        result = (CARD_STATS(id) & 0x1E000000) >> 25; break;
    }
    return result;
}

/* ATK * 10 (0 for Magic/Trap/Ticket, 4000 for Divine). */
static inline int GetCardAtk10(u16 id)
{
    int result;
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        result = 0; break;
    case 0x18:
        result = 4000; break;
    default:
        result = ((CARD_STATS(id) << 14) >> 23) * 10; break;
    }
    return result;
}

/* DEF * 10 (same special cases). */
static inline int GetCardDef10(u16 id)
{
    int result;
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        result = 0; break;
    case 0x18:
        result = 4000; break;
    default:
        result = (CARD_STATS(id) & 0x1FF) * 10; break;
    }
    return result;
}

extern const u32 gUnk_0819897C[];
extern const u32 gUnk_08198950[];

/* Draw the card info panel (frame icon, level stars, ATK/DEF digits, spell/trap icon) (hypothesis). */
static inline int CardLevelCached(u16 id, u32 stats)
{
    /* Reuse the type already read by the caller; every arm assigns the result. */
    register int level asm("r0");
    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 0x15: case 0x16: case 0x17: level = 0; break;
    case 0x18: level = 10; break;
    default: level = (CARD_STATS(id) & 0x1E000000) >> 25; break;
    }
    return level;
}
void sub_08061E54(u16 id)
{
    int starRow;
    int right;
    int i;
    int lvl;
    int v;
    int x;
    /* The subtype is assigned only in the non-monster arm, before its use. */
    register int sub asm("r2");
    u32 stats = CARD_STATS(id);
    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 0x15:
        sub_0806196C(0x0006004E, 0x70, (const u16 *)0x086366A8, (const void *)0x08636348);
        break;
    case 0x16:
        sub_0806196C(0x0006004E, 0x70, (const u16 *)0x08636728, (const void *)0x08636368);
        break;
    default:
        sub_0806196C(0x0006004E, 0x70, (const u16 *)gUnk_0819897C[stats >> 29],
                     (const void *)gUnk_08198950[stats >> 29]);
        break;
    }
    stats = CARD_STATS(id);
    if (((stats & 0x1F00000) >> 20) <= 0x14) {
        lvl = CardLevelCached(id, stats);
        i = 0;
        if (i >= lvl)
            goto stars_done;
        right = 0x54;
        starRow = 0x140000;
        x = right;
        do {
            if (lvl <= 9)
                sub_080618C4(x | starRow, 0x80, (const u16 *)0x0822C360, (const void *)0x0822C300);
            else
                sub_080618C4((right - 0x4E * i / lvl) | starRow, 0x80, (const u16 *)0x0822C360, (const void *)0x0822C300);
            x -= 8;
            i++;
        } while (i < lvl);
    stars_done:
        sub_080618C4(0x0076003F, 0x90, (const u16 *)0x0863856C, (const void *)0x0863840C);
        sub_080618C4(0x00760047, 0x90, (const u16 *)0x0863858C, (const void *)0x0863840C);
        v = GetCardAtk10(id);
        x = 0x57;
        do {
            sub_080618C4(0x760000 | x, 0x90, (const u16 *)(0x0863842C + v % 10 * 32), (const void *)0x0863840C);
            v = v / 10;
            x -= 4;
        } while (v != 0);
        sub_080618C4(0x007E003F, 0x90, (const u16 *)0x086385AC, (const void *)0x0863840C);
        sub_080618C4(0x007E0047, 0x90, (const u16 *)0x086385CC, (const void *)0x0863840C);
        v = GetCardDef10(id);
        x = 0x57;
        do {
            sub_080618C4(0x7E0000 | x, 0x90, (const u16 *)(0x0863842C + v % 10 * 32), (const void *)0x0863840C);
            v = v / 10;
            x -= 4;
        } while (v != 0);
    } else {
        int t = CARD_TYPE(id);
        switch (t) {
        case 0x15: case 0x16: sub = (CARD_STATS(id) & 0xE0000) >> 17; break;
        default: sub = 0; break;
        }
        if (sub != 0)
            sub_080618C4(0x00140050, 0x90, (const u16 *)(0x08637374 + sub * 32), (const void *)0x08637454);
    }
}

/* Sprite/tile offset for the card art of `id` (hypothesis). */
int sub_08062140(u16 id)
{
    int c;
    switch (CARD_NUMBER(id)) {
    case 1910: return 0x140;
    case 1911:
    case 1912: return 0xC0;
    }
    switch ((u8)CARD_TYPE(id)) {
    case 0x15: return 0x1C0;
    case 0x16: return 0x180;
    case 0x17: return 0x80;
    }
    c = GetCardSubtype(id);
    switch (c) {
    case 0: c = 0x80; break;
    case 1: c = 0xC0; break;
    case 2: c = 0x100; break;
    case 3: c = 0x140; break;
    }
    return c;
}

/* Sprite/tile offset for the card frame of `id0` (hypothesis: frame graphic offset). */
int sub_0806226C(u16 id)
{
    int c;
    switch ((u8)CARD_TYPE(id)) {
    case 0x15: return 0x1F0;
    case 0x16: return 0x1B0;
    }
    c = GetCardSubtype(id);
    switch (c) {
    case 0: return 0xB0;
    case 1: return 0xF0;
    case 2: return 0x130;
    case 3: return 0x170;
    }
    return c;
}

/* Map a zone row index to its group base: 5..9 gives 5, 10 gives 10, anything else gives 0. */
int sub_08062354(int x)
{
    int r = 0;
    if ((u32)(x - 5) <= 4)
        r = 5;
    if (x == 10)
        r = 10;
    return r;
}

/* Pixel x of slot 11 of `player`, offset by the `b`-th step of a spread over `c` items. */
s32 sub_0806236C(u32 a, u32 b, u32 c)
{
    s32 r = gUnk_081A42A4[a][11].x;
    if (c != 0) {
        s32 e = b * 32;
        s32 d = e;
        if ((s32)c > 5) {
            d = b * 128 + e;
            d = d / (s32)c;
        }
        if (a == 0)
            r += d;
        else
            r -= d;
    }
    return r;
}

/* X pixel of zone (row + idx) of a player; row 0xB (hand) is spread over the hand size. */
s32 sub_080623AC(int a, int b, int c)
{
    s32 r = gUnk_081A42A4[a][b + c].x;
    if (b == 0xB)
        r = sub_0806236C(a, c, gUnk_020192E4[a & 1].b2);
    return r;
}

/* Y pixel of a zone; row 0 is first mapped through sub_08062354(col). */
s32 sub_080623EC(u32 a, int b, int c)
{
    if (b == 0)
        b = sub_08062354(c);
    return gUnk_081A42A4[a][b].y - gUnk_0201CFB0.b4;
}

/* Sets BG palette entry 15 of the border to a rotating colour from the table. */
void sub_08062420(void)
{
    *(u16 *)0x0500001E = gUnk_081A451C[(REG_VCOUNT + (gUnk_02015160.w11A >> 1)) & 7];
}

/* Push the two frame counters into gMain and decrement the third. */
void sub_0806245C(void)
{
    u16 t;
    struct Main *m = &gMain;
    t = gUnk_02015160.w116++;
    m->w442E = t;
    t = gUnk_02015160.w118++;
    m->w4426 = t;
    gUnk_02015160.w11A--;
}

/* Card number to card id: 0xFFFF means none, <= 0x7CF is a direct table lookup, anything else uses the alternate entry + 1. */
static inline u16 CardNumberToId(u16 n)
{
    if (n == 0xFFFF)
        return 0;
    if (n <= 0x7CF)
        return gUnk_08623DF4[n & 0x7FF];
    return gUnk_08623DF4[(n - 0x7D0) & 0x7FF] + 1;
}

/* Draw the five slot sprites of the list at 0x02015160 (hypothesis). */
#if 0 /* NONMATCHING: register allocation differs. With explicit i2/y induction vars the index regs
       * match (r6=i, r8=2i, r7=32i, r9=-4, sl=0xFFFF, r5=else y copy), but gcc hoists the 0x7CF
       * compare (the target reloads it, keeping 0xFFFF). Tried reading b10C at each use
       * (trick 1). A `unsigned long long lim=0x7CF`/table-address temp (trick 2) un-hoists
       * 0x7CF but emits a dead double-word load. A goto loop with manual IVs (trick 3) loses
       * the base/0xFFFF hoists. Explicit i2/y locals (tricks 4/6) fix the index registers. The
       * 0x7CF hoist is the last diff. */
void sub_080624A4(void)
{
    int i;
    int i2 = 0;
    int y = 0;
    for (i = 0; i <= 4; i++) {
        u8 kind = gUnk_02015160.b10C[i];
        if (kind <= 0x17) {
            u16 e = gUnk_0808659C[kind];
            u32 tile;
            if (e & 0x1000) {
                u16 id = CardNumberToId(*(u16 *)((u8 *)&gUnk_02015160 + 0x102 + i2));
                tile = (u16)(gUnk_0808659C[gUnk_02015160.b10C[i]] + sub_08062140(id));
            } else {
                tile = e;
            }
            sub_080766A4(-4, y, 0x80, tile);
        } else {
            int a = -4;
            int y2 = y;
            u16 id = CardNumberToId(*(u16 *)((u8 *)&gUnk_02015160 + 0x102 + i2));
            sub_080766A4(a, y2, 0x80, 0x1000 | sub_08062140(id));
        }
        i2 += 2;
        y += 0x20;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080619E8", sub_080624A4); /* 0x080624A4 size 0x160 */
extern void sub_08074B08(u32 a, u32 b);
extern void sub_0807501C(u32 a, u32 b, u32 c, const void *d);
extern void sub_08075114(void *dst, u32 v);
extern void sub_0807326C(u32 a, u32 b, u32 c, const void *d);
extern void sub_08072C0C(u32 a, u32 b, u32 c, u32 d);
extern void sub_08072E98(u32 a, u32 b, u32 c);

/* Spell/trap subtype (stats bits 17-19) for Magic and Trap cards, else 0. */
static inline int GetSpellSubtype(u32 stats)
{
    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
        return (stats & 0xE0000) >> 17;
    default:
        return 0;
    }
}

/* Draw the card detail layout for `id` at slot `a`: palette/tile map setup, type icons, ATK/DEF, level stars (hypothesis). */
#if 0 /* NONMATCHING: the call sequence and first half agree. The target keeps `a` in sl and `id` in
       * a stack slot (reloaded per use) and hoists a*4 (r9)/a*0x30 (r8) plus the 0x0300045C base
       * (ip) out of the tile-map fill loop. The built code keeps `a` in r9 and `id` in sl and
       * allocates the high registers and frame differently. Tried local reuse, removing locals
       * and declaration order (tricks 4/6). The a/id swap is the first diff. */
void sub_08062604(int a, u16 id)
{
    s16 i;
    s16 k;
    u16 t;
    u16 *p;
    s16 sub;
    const u32 *st;
    t = 7;
    if (gUnk_02015160.w112 == CARD_NUMBER(id))
        t = 0xF;
    sub_08074B08(0x18, 2);
    sub_0807501C(2, 6, t | 0xA00, (const void *)(0x0822C720 + id * 0x40));
    sub_08075114((void *)(0x06004800 + a * 0x30 * 32), 0);
    for (i = 0; i <= 1; i++) {
        s16 v = a * 0x30 + 0x40;
        u16 *q = &gMain.bgMap[0][(a * 4 + i) * 3 + 32];
        for (k = 0x17; k >= 0; k--) {
            *q = v + i * 24;
            v++;
            q++;
        }
    }
    st = &CARD_STATS(id);
    switch ((int)((*st & 0x1F00000) >> 20)) {
    case 0x15:
        sub_0807326C(((u16)(a * 4 + 2) << 5) + 3, (a + 5) * 16, a * 4 + 0x300, (const void *)0x08636CD8);
        sub = GetSpellSubtype(*st);
        if (sub != 0)
            sub_0807326C((((u16)(a * 4 + 2) << 5) + 5), (a + 10) * 16, a * 4 + 0x320, (const void *)((const u32 *)0x081989D0)[sub]);
        break;
    case 0x16:
        sub = GetSpellSubtype(*st);
        sub_0807326C(((u16)(a * 4 + 2) << 5) + 3, (a + 5) * 16, a * 4 + 0x300, (const void *)0x08636DA0);
        if (sub != 0)
            sub_0807326C((((u16)(a * 4 + 2) << 5) + 5), (a + 10) * 16, a * 4 + 0x320, (const void *)((const u32 *)0x081989D0)[sub]);
        break;
    case 0x18:
        break;
    default: {
        u16 r = (a * 4 + 2) << 5;
        sub_0807326C(r + 3, (a + 5) * 16, a * 4 + 0x300, (const void *)((const u32 *)0x081989A8)[CARD_STATS(id) >> 29]);
        sub_0807326C(r + 5, (a + 10) * 16, a * 4 + 0x320, (const void *)((const u32 *)0x081989EC)[CARD_TYPE(id)]);
        sub_0807326C(r + 8, 0xF0, 0x340, (const void *)0x0863CA1C);
        sub_08072C0C((u16)(((a * 4 + 2) << 5) + 9) | 0x70000, (u16)(a * 8 + 0x1A0) | 0x40000, GetCardAtk10(id), 0);
        sub_08072C0C((u16)(((a * 4 + 3) << 5) + 9) | 0x70000, (u16)(a * 8 + 0x1C8) | 0x40000, GetCardDef10(id), 0);
        for (i = 0; i < GetCardLevel(id); i++)
            sub_08072E98(0, (u16)(i + 0xE + ((a * 4 + 2) << 5)), 2);
        break;
    }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080619E8", sub_08062604); /* 0x08062604 size 0x3EC */
