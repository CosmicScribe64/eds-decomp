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
void sub_08061A1C(u16 id)
{
    u16 x;
    u16 y;
    u16 j;
    const u16 *t;
    const u16 *src;
    u16 *dst;
    u16 pal;
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
    /* A variable palette offset, read as (u8)pal: the ROM builds 0x1000 as pal << 8 per loop nest. */
    pal = 0x10;
    for (y = 0; y <= 3; y++) {
        for (x = 0; x <= 0xC; x++) {
            dst = (u16 *)(0x06010000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                u16 w = *src;
                u16 v = w;
                /* FAKEMATCH: keeps the loaded word w apart from the accumulator v (as in sub_08072FAC) */
                __asm__ __volatile__("" : : "r"(w));
                if (w & 0xFF00)
                    v += (u8)pal << 8;
                if (v & 0xFF)
                    v += (u8)pal;
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
                u16 w = *src;
                u16 v = w;
                /* FAKEMATCH: keeps the loaded word w apart from the accumulator v (as in sub_08072FAC) */
                __asm__ __volatile__("" : : "r"(w));
                if (w & 0xFF00)
                    v += (u8)pal << 8;
                if (v & 0xFF)
                    v += (u8)pal;
                *dst = v;
                src++;
                dst++;
            }
        }
            for (x = 0; x <= 1; x++) {
            dst = (u16 *)(0x06010000 + (((u16)((x << 1) + 0x16)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                u16 w = *src;
                u16 v = w;
                /* FAKEMATCH: keeps the loaded word w apart from the accumulator v (as in sub_08072FAC) */
                __asm__ __volatile__("" : : "r"(w));
                if (w & 0xFF00)
                    v += (u8)pal << 8;
                if (v & 0xFF)
                    v += (u8)pal;
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
                u16 w = *src;
                u16 v = w;
                /* FAKEMATCH: keeps the loaded word w apart from the accumulator v (as in sub_08072FAC) */
                __asm__ __volatile__("" : : "r"(w));
                if (w & 0xFF00)
                    v += (u8)pal << 8;
                if (v & 0xFF)
                    v += (u8)pal;
                *dst = v;
                src++;
                dst++;
            }
        }
    }
}
/* Load the picture `id` (6 bits per pixel, packed) into OBJ tile memory and its 64-colour palette (hypothesis). */
/* Card picture loader: copies card id's 64-colour palette to OBJ palette 0x05000260, unpacks the
 * 6-bit-per-pixel picture (8 pixels per 3 halfwords, as in sub_0805DF34) into the 9x10 8bpp OBJ
 * tiles at columns 2-0xA, rows 4-0xD, then adds palette base 0x30 to every pixel byte. */
void sub_08061D24(u16 id)
{
    u16 y;
    u16 x;
    u16 j;
    int k;
    const u16 *src;
    u16 *dst;
    const u16 *s;
    u16 *d;
    u16 *q;
    u16 m6, m12, c30;

    src = (const u16 *)(0x08608360 + id * 0x80);
    dst = (u16 *)0x05000260;
    sub_08075294(dst, src, 0x80);
    src = (const u16 *)(0x082A6500 + id * 0x10E0);
    /* m12 and c30 lose the register contest and are rematerialized by reload (ROM: movs/lsls
     * 0xFC0 into r7 in the pixel loop, 0x30 into r6 before the fix-up loop). */
    m12 = 0xFC0;
    c30 = 0x30;
    y = 4;
    m6 = 0x3F; /* after y = 4, as in the ROM */
    for (; y <= 0xD; y++) {
        x = 2;
        /* FAKEMATCH: a goto x loop keeps loop.c from hoisting (u16)(y + 2) << 5 out of it, and the
         * do-while(0) around the unpack restores the pixel loop's nesting depth (local-alloc refs:
         * s0 in r2, s1 in r3). src += 24 outside it keeps next (r9) below m6 (r8) in global-alloc
         * priority. */
    xloop:
        do {
            dst = (u16 *)(0x06010000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            s = src;
            d = dst;
            for (k = 0; k < 8; k++) {
                u16 s0 = s[0];
                u32 s1 = s[1];
                u32 s2 = s[2];
                u16 t, xx;
                d[0] = (s0 & m6) | ((s0 & m12) << 2);
                d[1] = (s0 >> 12) | ((s1 & 3) << 4) | ((s1 & 0xFC) * 64);
                t = s1 >> 8;
                d[2] = (t & m6) | (((t >> 6) | ((s2 & 0xF) << 2)) << 8);
                xx = s2 >> 4;
                d[3] = (xx & m6) | ((xx & m12) << 2);
                s += 3;
                d += 4;
            }
        } while (0);
        src += 24;
        q = dst;
        for (j = 0; j <= 0x1F; j++) {
            *q = (*q & 0x3F3F) + ((u8)c30 << 8 | (u8)c30);
            q++;
        }
        x++;
        if (x <= 0xA)
            goto xloop;
    }
}
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
/* The ROM loads this table through a symbol (a constant-pool symbol_ref), which loop.c hoists in
 * its first pass; that pushes the 0xFFFF compare hoist to the second pass so it wins sl.
 * The cast-address macro above is used only here. */
#undef gUnk_0808659C
extern const u16 gUnk_0808659C[];

void sub_080624A4(void)
{
    int i;
    for (i = 0; i <= 4; i++) {
        if (gUnk_02015160.b10C[i] <= 0x17) {
            u16 tile;
            if (gUnk_0808659C[gUnk_02015160.b10C[i]] & 0x1000)
                tile = gUnk_0808659C[gUnk_02015160.b10C[i]] + sub_08062140(CardNumberToId(gUnk_02015160.w102[i]));
            else
                tile = gUnk_0808659C[gUnk_02015160.b10C[i]];
            sub_080766A4(-4, i * 32, 0x80, tile);
        } else {
            sub_080766A4(-4, i * 32, 0x80, 0x1000 | sub_08062140(CardNumberToId(gUnk_02015160.w102[i])));
        }
    }
}
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
extern const u32 gUnk_081989A8[];
extern const u32 gUnk_081989D0[];
extern const u32 gUnk_081989EC[];
extern u16 gUnk_0300045C[];

/* ATK * 10 / DEF * 10 as u16 (as in sub_0802A188): the result lands in r0 and is copied to r2. */
static inline u16 F604_Atk10(const u32 *p, u16 id)
{
    switch ((int)((*p & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    default:
        return ((CARD_STATS(id) << 14) >> 23) * 10;
    }
}

static inline u16 F604_Def10(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    default:
        return (CARD_STATS(id) & 0x1FF) * 10;
    }
}

/* Level (0 for Magic/Trap/Ticket, 10 for Divine). The u8 type local and u8 return add zero
 * extensions that combine deletes only after loop.c: they raise the first loop pass's insn
 * count so the card-stats address is hoisted in pass 2 and the (a * 4 + 2) << 5 chain stays
 * in the star loop, as in the ROM. */
static inline u8 F604_Level(u16 id)
{
    u8 type = CARD_TYPE(id);
    switch (type) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (CARD_STATS(id) & 0x1E000000) >> 25;
    }
}

void sub_08062604(int a, u16 id)
{
    int i;
    int k;
    u32 t;
    const u32 *st;
    t = 7;
    if (gUnk_02015160.w112 == CARD_NUMBER(id))
        t = 0xF;
    sub_08074B08(0x18, 2);
    /* DrawText takes a u16 colour (as declared in code_08002388); this unit's prototype says u32,
     * so call through the real signature (the u16 conversion gives the ROM's constant copy). */
    ((void (*)(int, int, u16, const void *))sub_0807501C)(2, 6, t | 0xA00, (const void *)(0x0822C720 + id * 0x40));
    /* Tile base 0x40 of char block 0x06004000: CSE keeps a * 0x30 for the fill loop below. */
    sub_08075114((void *)(0x06004000 + (a * 0x30 + 0x40) * 32), 0);
    for (i = 0; i <= 1; i++) {
        for (k = 0; k < 24; k++) {
            /* The index in its own local puts the table base load after a * 4 in the loop body,
             * which gives the ROM's hoist order (a * 4 copy before the 0x0300045C base). */
            int idx = (a * 4 + i) * 32 + 3 + k;
            gUnk_0300045C[idx] = a * 0x30 + 0x40 + k + i * 24;
        }
    }
    st = &CARD_STATS(id);
    switch ((int)((*st & 0x1F00000) >> 20)) {
    case 0x15:
        sub_0807326C(((u16)(a * 4 + 2) << 5) + 3, (a + 5) * 16, a * 4 + 0x300, (const void *)0x08636CD8);
        if (GetSpellSubtype(*st) != 0)
            sub_0807326C((((u16)(a * 4 + 2) << 5) + 5), (a + 10) * 16, a * 4 + 0x320, (const void *)gUnk_081989D0[GetSpellSubtype(CARD_STATS(id))]);
        break;
    case 0x16:
        sub_0807326C(((u16)(a * 4 + 2) << 5) + 3, (a + 5) * 16, a * 4 + 0x300, (const void *)0x08636DA0);
        if (GetSpellSubtype(*st) != 0)
            sub_0807326C((((u16)(a * 4 + 2) << 5) + 5), (a + 10) * 16, a * 4 + 0x320, (const void *)gUnk_081989D0[GetSpellSubtype(CARD_STATS(id))]);
        break;
    case 0x18:
        break;
    default: {
        const u32 *p;
        u32 r = (u16)(a * 4 + 2) << 5;
        sub_0807326C(r + 3, (a + 5) * 16, a * 4 + 0x300, (const void *)gUnk_081989A8[*(p = &CARD_STATS(id)) >> 29]);
        sub_0807326C(r + 5, (a + 10) * 16, a * 4 + 0x320, (const void *)gUnk_081989EC[(*p & 0x1F00000) >> 20]);
        sub_0807326C(r + 8, 0xF0, 0x340, (const void *)0x0863CA1C);
        sub_08072C0C((u16)(((a * 4 + 2) << 5) + 9) | 0x70000, (u16)(a * 8 + 0x1A0) | 0x40000, F604_Atk10(p, id), 0);
        sub_08072C0C((u16)(((a * 4 + 3) << 5) + 9) | 0x70000, (u16)(a * 8 + 0x1C8) | 0x40000, F604_Def10(id), 0);
        for (i = 0; i < F604_Level(id); i++)
            sub_08072E98(0, (u16)(i + 0xE + ((a * 4 + 2) << 5)), 2);
        break;
    }
    }
}
