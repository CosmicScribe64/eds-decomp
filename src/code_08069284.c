#include "global.h"
#include "gba.h"

/* Deck-edit scene, part 3 (helpers after the card-list screens). See wiki/functions/code-08069284.md. */

/* Card-list/scene state at 0x0201DB20 (see code-08068180); only the flag block at +0x1C49.. is modelled here. */
struct SceneFlags {
    u8 pad0[0x1C1C];
    u8 cursor;                  /* +0x1C1C */
    u8 pad1C1D[0x1C49 - 0x1C1D];
    u8 f1C49;
    u8 f1C4A;
    u8 f1C4B;
    u16 f1C4C;
    u16 f1C4E;
    u8 f1C50;
    u8 f1C51;
    u8 f1C52;
    u8 f1C53;
    u8 f1C54;
};
extern struct SceneFlags gUnk_0201DB20;

/* Sprite descriptor table entry (8 bytes). */
struct SpriteDesc {
    const void *gfx;
    u8 b4;
    u8 pad5[3];
};
extern const struct SpriteDesc gUnk_081A715C[];
extern const struct SpriteDesc gUnk_081A71CC[];
extern void *sub_08077EF4(const void *a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l);
void sub_0807A908(const void *src, void *dst, u32 w, u32 h);
extern const u8 gUnk_086FD580[];
extern const u8 gUnk_086FD0D0[];
extern const u8 gUnk_086F41A0[];
struct MainLite {
    u8 pad[0x414];
    u32 f414;
};
extern struct MainLite gUnk_03000040;
void sub_08069F40(const void *src, void *dst, u8 n);
extern void CpuFastSet(const void *src, void *dst, u32 cnt);
void sub_0807A9C0(const void *src, void *dst, int w, int h, int a, int b, int c);
void sub_0807ADE8(const void *tbl, int a, int b, int c, int d, int e, int f, int g, int h, int i);
void sub_08077CEC(const void *src, void *dst, u32 n);
void sub_08078670(const void *a, void *b);
void sub_080787F4(u32 a, u32 b, u32 c, void *p);
extern const u8 gUnk_086FC060[], gUnk_086FC0E0[], gUnk_086FC590[], gUnk_086FCA40[], gUnk_086FD850[];
extern const u8 gUnk_086F2060[], gUnk_086F4060[], gUnk_086F6060[], gUnk_086F8060[], gUnk_086FA060[];
extern const u8 gUnk_086F1C60[], gUnk_086F1E60[], gUnk_081A6118[];
struct FB { u8 f : 8; };
#define OBJF(off) (((struct FB *)((u8 *)&gUnk_0201DB20 + (off)))->f)

#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((int)((CARD_STATS(id) & 0x1F00000) >> 20))

/* Counters of the three card lists: [0][list] = entries, [1][list] = entries after filtering; +0x14A0 u8[list]; +0x620 u16[list]. */
struct ListState {
    u8 pad0[0x620];
    u16 arr620[3];
    u8 pad626[0x1494 - 0x626];
    u16 cnt1494[2][3];
    u8 arr14A0[3];
};
extern struct ListState gUnk_0201DB20_ls asm("gUnk_0201DB20");
extern u16 gUnk_0201EFC4[];
void sub_080686E8(void);
void CpuSet(const void *src, void *dst, u32 cnt);
void sub_080690C4(int n, s16 *arr, u16 (*cmp)(s16, s16));
u16 sub_08068E44(s16 a, s16 b);
u16 sub_08068EFC(s16 a, s16 b);
u16 sub_08068FBC(s16 a, s16 b);
u16 sub_08068FEC(s16 a, s16 b);
u16 sub_08069014(s16 a, s16 b);

/* Monster category (0-3 = normal/effect/fusion/ritual-like by number or the stats bits 18-19), 7/8/9 for Magic/Trap/Ticket. */
static inline int CardKind(u16 id)
{
    int num = CARD_NUMBER(id);
    int k;

    if (num == 0x776)
        return 3;
    if (num >= 0x776 && num <= 0x778)
        return 1;
    switch (CARD_TYPE(id)) {
    case 0x16:
        k = 7;
        break;
    case 0x15:
        k = 8;
        break;
    case 0x17:
        k = 9;
        break;
    default:
        k = (CARD_STATS(id) & 0xC0000) >> 18;
        break;
    }
    return k;
}

/* Frame kind (as CS_FrameKind in code_0806C4E4): direct returns keep jump2 from threading the `== k` test. */
static inline u8 FS_FrameKind(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 0x776: return 3;
    case 0x777:
    case 0x778: return 1;
    default:
        switch (CARD_TYPE(id)) {
        case 0x16: return 7;
        case 0x15: return 8;
        case 0x17: return 9;
        default: return (CARD_STATS(id) & 0xC0000) >> 18;
        }
    }
}
#define FS_COUNT gUnk_0201DB20_ls.cnt1494[0][list]
#define FS_KIND(kindValue) \
    for (i = 0; i < FS_COUNT; i++) { \
        u16 card = src[i]; \
        switch ((u8)CARD_TYPE(card)) { \
        case 0x15: \
        case 0x16: \
            break; \
        default: \
            if (FS_FrameKind(src[i]) == (kindValue)) \
                dst[n++] = src[i]; \
            break; \
        } \
    }
#define FS_TYPE(typeValue) \
    for (i = 0; i < FS_COUNT; i++) { \
        u16 card = src[i]; \
        if ((u8)CARD_TYPE(card) == (typeValue)) \
            dst[n++] = card; \
    }
/* Deck-edit filter and sort of card list `list`: rebuilds the lists, copies list `list` (u16 ids) to a scratch array,
   keeps the cards of category `filter` (0 = all, 1-3 monster categories, 4 = Magic, 5 = Trap, 6 = category 3, 7 = monsters
   only) and sorts by `sort` (0 none, 1 ATK, 2 DEF, 3 type, 4 attribute, 5 level). */
void sub_08069284(u8 list, u8 filter, u8 sort)
{
    u16 *src;
    u16 *dst;
    int n = 0;
    u16 n2 = 0;
    u16 i;
    u16 *base = gUnk_0201EFC4;

    sub_080686E8();
    switch (list) {
    case 0:
        src = base - 0x730;
        dst = base - 0x39C;
        break;
    case 1:
        src = base - 0x3FB;
        dst = base - 0x67;
        break;
    case 2:
        src = base - 0x3AB;
        dst = base - 0x17;
        break;
    }
    CpuSet(src, dst, gUnk_0201DB20_ls.cnt1494[0][list]);
    switch (filter) {
    case 0:
        if (sort == 0)
            break;
        for (i = 0; i < FS_COUNT; i++) {
            u16 card = src[i];

            switch (CARD_TYPE(card)) {
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
                base[n2++] = card;
                break;
            default:
                dst[n++] = src[i];
                break;
            }
        }
        break;
    case 1:
        FS_KIND(0);
        break;
    case 2:
        FS_KIND(1);
        break;
    case 3:
        FS_KIND(2);
        break;
    case 4:
        FS_TYPE(0x16);
        break;
    case 5:
        FS_TYPE(0x15);
        break;
    case 6:
        for (i = 0; i < FS_COUNT; i++) {
            if (FS_FrameKind(src[i]) == 3)
                dst[n++] = src[i];
        }
        break;
    case 7:
        for (i = 0; i < FS_COUNT; i++) {
            u16 card = src[i];

            switch (CARD_TYPE(card)) {
            case 0x15:
            case 0x16:
            case 0x17:
            case 0x18:
                break;
            default:
                dst[n++] = src[i];
                break;
            }
        }
        break;
    }
    if (filter == 0) {
        if (sort == 0) {
            gUnk_0201DB20_ls.cnt1494[1][list] = gUnk_0201DB20_ls.cnt1494[0][list];
            n = gUnk_0201DB20_ls.cnt1494[0][list];
        } else {
            gUnk_0201DB20_ls.cnt1494[1][list] = n;
        }
    } else {
        gUnk_0201DB20_ls.cnt1494[1][list] = n;
    }
    switch (sort) {
    case 0:
        gUnk_0201DB20_ls.arr14A0[list] = 1;
        break;
    case 1:
        sub_080690C4(n, (s16 *)dst, (u16 (*)(s16, s16))sub_08068E44);
        gUnk_0201DB20_ls.arr14A0[list] = 1;
        break;
    case 2:
        sub_080690C4(n, (s16 *)dst, (u16 (*)(s16, s16))sub_08068EFC);
        gUnk_0201DB20_ls.arr14A0[list] = 1;
        break;
    case 3:
        sub_080690C4(n, (s16 *)dst, (u16 (*)(s16, s16))sub_08068FBC);
        gUnk_0201DB20_ls.arr14A0[list] = 1;
        break;
    case 4:
        sub_080690C4(n, (s16 *)dst, (u16 (*)(s16, s16))sub_08068FEC);
        gUnk_0201DB20_ls.arr14A0[list] = 1;
        break;
    case 5:
        sub_080690C4(n, (s16 *)dst, (u16 (*)(s16, s16))sub_08069014);
        gUnk_0201DB20_ls.arr14A0[list] = 1;
        break;
    }
    if (filter == 0 && sort != 0) {
        int total;

        i = 0;
        total = n2 + n;
        for (; i < n2; i++)
            dst[n + i] = base[i];
        gUnk_0201DB20_ls.cnt1494[1][list] = total;
    }
    gUnk_0201DB20_ls.arr620[list] = 0;
}
/* Clears the flag block at +0x1C49..+0x1C53 of the scene state; returns 1. */
int sub_08069A90(void)
{
    gUnk_0201DB20.f1C4E = 0;
    gUnk_0201DB20.f1C4C = 0;
    gUnk_0201DB20.f1C49 = 0;
    gUnk_0201DB20.f1C4A = 0;
    gUnk_0201DB20.f1C52 = 0;
    gUnk_0201DB20.f1C53 = 0;
    gUnk_0201DB20.f1C4B = 0;
    return 1;
}
/* Scene init: clears VRAM, loads the tile maps / graphics / palettes of the screen, marks 14 objects and sets up the BG registers. */
int sub_08069AE0(void)
{
    vu32 z1;
    vu32 z2;
    u16 i;
    u16 j;

    z1 = 0;
    CpuFastSet((void *)&z1, (void *)0x06000000, 0x01004000);
    z2 = 0;
    CpuFastSet((void *)&z2, (void *)0x06010000, 0x01002000);
    for (i = 0; i <= 3; i++) {
        for (j = 0; j <= 3; j++)
            sub_0807A9C0(gUnk_086FC060, (u16 *)0x0600F000 + (j * 8 + i * 256), 8, 8, 8, 0, 0);
    }
    sub_0807A908(gUnk_086FC0E0, (void *)0x0600E000, 0x1E, 0x14);
    sub_0807A908(gUnk_086FC590, (void *)0x0600D000, 0x1E, 0x14);
    sub_0807A908(gUnk_086FCA40, (void *)0x0600C000, 0x1E, 0x14);
    sub_0807ADE8(gUnk_086FD850, 0, gUnk_0201DB20.cursor * 5, 7, 0x0600D000, 0x14, 0, 7, 5, 0);
    sub_0807ADE8(gUnk_086FD850, 0, gUnk_0201DB20.cursor * 5, 7, 0x0600C000, 0x14, 0, 7, 5, 0);
    CpuFastSet(gUnk_086F2060, (void *)0x06000000, 0x800);
    CpuFastSet(gUnk_086F4060, (void *)0x06002000, 0x800);
    CpuFastSet(gUnk_086F6060, (void *)0x06004000, 0x800);
    sub_08077CEC(gUnk_086F8060, (void *)0x06010000, 0x10);
    sub_08077CEC(gUnk_086FA060, (void *)0x06010200, 0x10);
    sub_08078670(gUnk_081A6118, (u8 *)&gUnk_0201DB20 + 0x1718);
    OBJF(0x1726) |= 0xFF;
    OBJF(0x173A) |= 0xFF;
    OBJF(0x174E) |= 0xFF;
    OBJF(0x1762) |= 0xFF;
    OBJF(0x1776) |= 0xFF;
    OBJF(0x178A) |= 0xFF;
    OBJF(0x179E) |= 0xFF;
    OBJF(0x17B2) |= 0xFF;
    OBJF(0x17C6) |= 0xFF;
    OBJF(0x17DA) |= 0xFF;
    OBJF(0x17EE) |= 0xFF;
    OBJF(0x1802) |= 0xFF;
    OBJF(0x1816) |= 0xFF;
    OBJF(0x182A) |= 0xFF;
    CpuFastSet(gUnk_086F1C60, (void *)0x05000000, 0x80);
    CpuFastSet(gUnk_086F1E60, (void *)0x05000200, 0x80);
    REG_BG0CNT = 0x1800;
    REG_BG1CNT = 0x1A01;
    REG_BG2CNT = 0x1C02;
    REG_BG3CNT = 0x1E02;
    sub_080787F4(0, -0x180, 0, (u8 *)&gUnk_0201DB20 + 0x618);
    REG_BG0HOFS = 0;
    REG_BG0VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG1VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG3HOFS = 0;
    REG_BG3VOFS = 0;
    gUnk_0201DB20.f1C50 = 0x10;
    REG_DISPCNT = 0x1A00;
    return 1;
}
/* Creates the sprite group for descriptor `idx` of table 0x081A715C (priority word 0x400). */
void sub_08069DD8(u8 idx)
{
    sub_08077EF4(gUnk_081A715C[idx].gfx, 1, gUnk_081A715C[idx].b4, -1, -1, 3, 2, 0, 0, 0, 0x400, (int)&gUnk_0201DB20);
}
/* Same with table 0x081A71CC and priority 0. */
void sub_08069E20(u8 idx)
{
    sub_08077EF4(gUnk_081A71CC[idx].gfx, 1, gUnk_081A71CC[idx].b4, -1, -1, 3, 2, 0, 0, 0, 0, (int)&gUnk_0201DB20);
}
/* Loads two 30x8 tile maps into BG map rows and clears gMain+0x414. */
void sub_08069E64(void)
{
    sub_0807A908(gUnk_086FD580, (void *)0x0600C200, 30, 8);
    sub_0807A908(gUnk_086FD0D0, (void *)0x0600E200, 30, 8);
    gUnk_03000040.f414 = 0;
}
/* Converts 8 entries of 4 bytes from `src` to `dst` (u16 pairs) by `mode` (1..8): mask/zero-extend the first half-word
   and optionally swap it behind the second one. */
void sub_08069EA4(u16 *src, u16 *dst, u8 mode)
{
    int i;

    for (i = 0; i <= 7; i++) {
        switch (mode) {
        case 1:
            dst[0] = 0xF & src[0];
            break;
        case 2:
            dst[0] = (u8)src[0];
            break;
        case 3:
            dst[0] = 0xFFF & src[0];
            break;
        case 4:
            dst[0] = src[0];
            break;
        case 5:
            dst[0] = src[1];
            dst[1] = 0xF & src[0];
            break;
        case 6:
            dst[0] = src[1];
            dst[1] = (u8)src[0];
            break;
        case 7:
            dst[0] = src[1];
            dst[1] = 0xFFF & src[0];
            break;
        case 8:
            dst[0] = src[1];
            dst[1] = src[0];
            break;
        }
        dst += 2;
        src += 2;
    }
}
/* Copies a block of (n >> 3) groups of 8 entries plus (n & 7) entries with sub_08069EA4, in two planes 0x200 bytes apart. */
void sub_08069F40(const void *src, void *dst, u8 n)
{
    u8 groups = n >> 3;
    u8 rest = n & 7;
    u8 i;

    for (i = 0; i < groups; i++) {
        sub_08069EA4((u16 *)src, dst, 8);
        sub_08069EA4((u16 *)((u8 *)src + 0x200), (u16 *)((u8 *)dst + 0x200), 8);
        dst = (u8 *)dst + 0x20;
    }
    sub_08069EA4((u16 *)src, dst, rest);
    sub_08069EA4((u16 *)((u8 *)src + 0x200), (u16 *)((u8 *)dst + 0x200), rest);
}
/* Advances the animation counter at +0x1C54 and redraws with (counter * 3) & 0x7F groups. */
void sub_08069FAC(void)
{
    gUnk_0201DB20.f1C54++;
    sub_08069F40(gUnk_086F41A0, (void *)0x06003C00, (gUnk_0201DB20.f1C54 * 3) & 0x7F);
}
/* View of the scene state 0x0201DB20 used by the filter/sort menu handler. */
struct FMObject {
    u8 pad0[0xE];
    u8 mark;                    /* +0xE */
    u8 padF[0x14 - 0xF];
};
struct FilterMenu {
    u8 pad0[0x618];
    u8 transition[6];           /* +0x618 */
    u8 transitionState;         /* +0x61E */
    u8 pad61F[0x1718 - 0x61F];
    struct FMObject objs[14];   /* +0x1718 */
    u8 pad1830[0x1C1C - 0x1830];
    u8 cursor;                  /* +0x1C1C */
    u8 pad1C1D[0x1C3D - 0x1C1D];
    u8 dirty : 3;               /* +0x1C3D */
    u8 dirtyRest : 5;
    u8 pad1C3E;
    u8 filter[3];               /* +0x1C3F */
    u8 sort[3];                 /* +0x1C42 */
    u8 pad1C45[3];
    u8 f1C48Low : 1;            /* +0x1C48 */
    u8 f1C48 : 4;
    u8 f1C48Rest : 3;
    u8 filterSel;               /* +0x1C49 */
    u8 sortSel;                 /* +0x1C4A */
    u8 phase;                   /* +0x1C4B */
    u16 scrollX;                /* +0x1C4C */
    u16 scrollY;                /* +0x1C4E */
    u8 blend;                   /* +0x1C50 */
    s8 blendStep;               /* +0x1C51 */
    u8 fading;                  /* +0x1C52 */
    u8 timer;                   /* +0x1C53 */
    u8 anim;                    /* +0x1C54 */
    u8 pad1C55[0x1C5A - 0x1C55];
    u8 flags;                   /* +0x1C5A */
    u8 special;                 /* +0x1C5B */
};
#define FM (*(struct FilterMenu *)&gUnk_0201DB20)
struct FMMain {
    u8 pad0[6];
    u16 keys;                   /* +0x6 */
    u8 pad8[0x414 - 8];
    u32 callback;               /* +0x414 */
};
#define FM_MAIN (*(struct FMMain *)&gUnk_03000040)
extern const u8 gUnk_0808756C[][4];
extern const u8 gUnk_08087588[][4];
extern const u8 gUnk_080875A4[][4];
extern const u8 gUnk_080875BC[];
void sub_08077AEC(int sound);
void sub_0807883C(void *state);
void sub_0807B4A8(u32 level);
void sub_0807871C(void *objects);
void sub_08078534(void *objects, int a, int b, int c, int d, int e, int f, int g, int h, void *state);
void sub_0807A298(void *state);
void sub_0807A2EC(void *state);

/* Filter/sort menu frame handler: scrolls BG3, moves the filter (phase 0) and sort (phase 1) cursors through the
   neighbour tables, applies the choice with sub_08069284 (phase 2), runs the 13-frame confirm timer and the fades.
   Returns 1 when the exit transition is done. The `case 3..5` / `case 4..5` switches give the ROM's signed bound tests. */
int sub_08069FE4(void)
{
    u32 keys;

    keys = FM_MAIN.keys & 0x3FF;
    sub_0807883C(FM.transition);
    FM.scrollX += 0x80;
    FM.scrollY += 0x80;
    REG_BG3HOFS = FM.scrollX >> 8;
    REG_BG3VOFS = FM.scrollY >> 8;
    if (FM.timer == 0) {
        switch (FM.phase) {
        case 0:
            switch (keys) {
            case 0x40:
                FM.objs[FM.filterSel].mark = 0xFF;
                if (FM.flags & 0x20)
                    FM.filterSel = gUnk_08087588[FM.filterSel][0];
                else
                    FM.filterSel = gUnk_0808756C[FM.filterSel][0];
                FM.objs[FM.filterSel].mark = 1;
                sub_08077AEC(0);
                break;
            case 0x80:
                FM.objs[FM.filterSel].mark = 0xFF;
                if (FM.flags & 0x20)
                    FM.filterSel = gUnk_08087588[FM.filterSel][1];
                else
                    FM.filterSel = gUnk_0808756C[FM.filterSel][1];
                FM.objs[FM.filterSel].mark = 1;
                sub_08077AEC(0);
                break;
            case 0x10:
                FM.objs[FM.filterSel].mark = 0xFF;
                if (FM.flags & 0x20)
                    FM.filterSel = gUnk_08087588[FM.filterSel][2];
                else
                    FM.filterSel = gUnk_0808756C[FM.filterSel][2];
                FM.objs[FM.filterSel].mark = 1;
                sub_08077AEC(0);
                break;
            case 0x20:
                FM.objs[FM.filterSel].mark = 0xFF;
                if (FM.flags & 0x20)
                    FM.filterSel = gUnk_08087588[FM.filterSel][3];
                else
                    FM.filterSel = gUnk_0808756C[FM.filterSel][3];
                FM.objs[FM.filterSel].mark = 1;
                sub_08077AEC(0);
                break;
            case 2:
                if (FM.transitionState == 0)
                    FM.blendStep = 1;
                sub_08077AEC(2);
                break;
            default:
                FM.objs[FM.filterSel].mark = 1;
                break;
            case 1:
                FM.timer = 1;
                sub_08077AEC(1);
                break;
            }
            if (FM.fading != 0 && FM.blend <= 8)
                sub_08069DD8(FM.filterSel);
            break;
        case 1:
            switch (keys) {
            case 0x40:
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 0xFF;
                FM.sortSel = gUnk_080875A4[FM.sortSel][0];
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 1;
                sub_08077AEC(0);
                break;
            case 0x80:
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 0xFF;
                FM.sortSel = gUnk_080875A4[FM.sortSel][1];
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 1;
                sub_08077AEC(0);
                break;
            case 0x10:
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 0xFF;
                FM.sortSel = gUnk_080875A4[FM.sortSel][2];
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 1;
                sub_08077AEC(0);
                break;
            case 0x20:
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 0xFF;
                FM.sortSel = gUnk_080875A4[FM.sortSel][3];
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 1;
                sub_08077AEC(0);
                break;
            case 2:
                FM.phase = 0;
                REG_DISPCNT |= 0x200;
                REG_DISPCNT &= 0xFEFF;
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 0xFF;
                FM.objs[FM.filterSel].mark = 0;
                sub_08077AEC(0);
                break;
            case 1:
                FM.timer = 1;
                sub_08077AEC(1);
                break;
            default:
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 1;
                break;
            }
            if (FM.fading != 0 && FM.blend <= 8)
                sub_08069DD8(gUnk_080875BC[FM.sortSel]);
            break;
        case 2:
            switch (keys) {
            case 2:
                if (FM.transitionState == 0) {
                    FM.blendStep = 1;
                    sub_08077AEC(2);
                }
                break;
            case 1:
                sub_08069F40(gUnk_086F41A0, (void *)0x06003C00, (FM.scrollX >> 8) & 0x7F);
                sub_08077AEC(1);
                break;
            default:
                FM.objs[10].mark = 1;
                FM.anim = 0;
                FM_MAIN.callback = (u32)sub_08069FAC;
                switch (FM.special) {
                case 3:
                case 4:
                case 5:
                    sub_08069284(FM.cursor, FM.filterSel, FM.special);
                    FM.special = 0;
                    FM.sort[FM.cursor] = 1;
                    FM.dirty = 3;
                    break;
                default:
                    sub_08069284(FM.cursor, FM.filterSel, FM.sortSel);
                    break;
                }
                FM_MAIN.callback = 0;
                sub_08069F40(gUnk_086F41A0, (void *)0x06003C00, 0x80);
                FM.blendStep = 1;
                FM.phase = 3;
                break;
            }
            break;
        }
    }
    if (FM.timer != 0) {
        switch (FM.phase) {
        case 0:
            if (FM.timer == 13) {
                FM.timer = 0;
                FM.objs[FM.filterSel].mark = 0xFF;
                REG_DISPCNT &= 0xFDFF;
                REG_DISPCNT |= 0x100;
                switch (FM.filterSel) {
                case 4:
                case 5:
                    FM.filter[FM.cursor] = FM.filterSel;
                    FM.dirty = 3;
                    FM.sort[FM.cursor] = FM.sortSel;
                    FM.dirty = 3;
                    FM.timer = 0;
                    sub_08069E64();
                    FM.phase = 2;
                    break;
                default:
                    FM.phase = 1;
                    FM.objs[gUnk_080875BC[FM.sortSel]].mark = 0;
                    break;
                }
            } else if (++FM.timer <= 11) {
                sub_08069E20(FM.filterSel);
            }
            break;
        case 1:
            if (FM.timer == 13) {
                FM.filter[FM.cursor] = FM.filterSel;
                FM.dirty = 3;
                FM.sort[FM.cursor] = FM.sortSel;
                FM.dirty = 3;
                FM.timer = 0;
                FM.objs[gUnk_080875BC[FM.sortSel]].mark = 0xFF;
                sub_08069E64();
                FM.phase = 2;
            } else if (++FM.timer <= 11) {
                sub_08069E20(gUnk_080875BC[FM.sortSel]);
            }
            break;
        case 2:
            break;
        }
    }
    if (FM.transitionState == 2) {
        FM.f1C48 = 4;
        FM.dirty = 3;
        return 1;
    }
    if (FM.transitionState == 3) {
        REG_BLDCNT = 0x3F44;
        sub_0807B4A8(0x10);
        REG_DISPCNT |= 0x400;
        FM.transitionState = 0;
        FM.blendStep = -1;
        FM.fading = 1;
    }
    if (FM.blendStep != 0) {
        FM.blend += FM.blendStep;
        if (FM.blend == 8)
            FM.blendStep = 0;
        if (FM.blend == 16) {
            REG_DISPCNT &= 0xFBFF;
            sub_080787F4(0, 0x180, 0, FM.transition);
            FM.blendStep = 0;
            FM.fading = 0;
        }
        sub_0807B4A8(FM.blend);
    }
    sub_0807871C(FM.objs);
    sub_08078534(FM.objs, 0, 0, 0, 0, 0, 3, 0, 0, &gUnk_0201DB20);
    sub_0807A298(&gUnk_0201DB20);
    sub_0807A2EC(&gUnk_0201DB20);
    return 0;
}
