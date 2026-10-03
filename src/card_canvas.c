/*
 * card_canvas (0x080619E8-0x080629F4): the card-image canvas (frame/picture blits into OBJ tile memory,
 * the card info panel, card-class icon offsets, duel-zone pixel positions) and the pack-opening scene's
 * card sprites and detail rows (wiki/functions/card-canvas-c.md).
 *
 * LoadCardFrame + LoadCardPicture draw a card's image into OBJ VRAM in 2D mapping: the frame tiles picked
 * by card type/kind, then the 6bpp picture unpacked into the frame's window. DrawCardInfo adds the
 * attribute/type icon, the level stars and the ATK/DEF digits. UnloadDuelUiGfx tears the duel UI tiles
 * down again; GetCardIconObjTile / GetCardIconBgTile give the per-class icon tile offsets, and
 * GetZoneArea / GetHandCardX / GetAreaX / GetAreaY the field pixel positions of the duel zones. The
 * GetPack_* functions are the pack-opening scene's card display: the HBlank palette cycle, the BG3
 * scroll, the five flip-animation slot sprites and the text detail rows.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_ALT_ART, CARD_NAME_SIZE, CARD_ART_SIZE,
                                   * CARD_ART_PALETTE_SIZE, CARD_STATS_* extractors, gCardIconPals,
                                   * gCardIconGfx, gAttributeIconImages, gSpellSubtypeIconImages,
                                   * gMonsterTypeIconImages, gCardStatDigitsPal, gCardStatDigitsGfx,
                                   * gCardFrame*Gfx */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind */
#include "constants/cards.h"        /* CARD_OBELISK_THE_TORMENTOR, CARD_SLIFER_THE_SKY_DRAGON,
                                   * CARD_THE_WINGED_DRAGON_OF_RA */
#include "constants/duel.h"         /* enum DuelArea */
#include "gba.h"                    /* REG_DISPCNT, REG_VCOUNT, BG_PLTT, OBJ_PLTT, VRAM */
#include "main.h"                   /* struct Main gMain, bgVofs / bgHofs */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that duel_screen.h does not pull in the legacy header. After H0, replace the block (BEGIN to
 * END) with the include line of duel.h (build/readability/issues/card_canvas.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

/* Needed by duel_screen.h (DuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13 */
    u16 isDefense:1;                /* bit 14 */
    u16 isFaceUp:1;                 /* bit 15 */
    u16 unk2;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u8 unk6_6:2;
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 unk3[0x7 - 0x3];
    u8 unk7_0:6;
    u8 magicTrapLockTurns:2;        /* +0x007 bits 6-7: nonzero blocks Magic/Trap activation */
    u8 unk8[0x28 - 0x8];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
/* ---- END duel.h stand-in ---- */

#include "duel_screen.h"            /* struct DuelScreen gDuelScreen, struct DuelZonePos
                                   * gDuelZonePositions, DrawCardImageTile, DrawCardImageIcon16,
                                   * this unit's prototypes */
#include "booster.h"                /* struct PackOpenWork gPackOpenWork, the GetPack_* prototypes */
#include "util.h"                   /* MemClear16, MemCopy16 */

/* ---- Local views kept for matching (build/readability/HEADERS.md) ---- */

/* Card tables through their integer addresses, not the card_data.h symbols: GCC reloads the table
 * address at every use instead of keeping it in a register; the symbol forms give different code
 * (include/card_data.h). */
#define CARD_STATS_WORD(id)     (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* gCardStats[id] */
#define CARD_NUMBER(id)         (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber[id] */
#define CARD_ID_FROM_NUMBER(n)  (((const u16 *)0x08623DF4)[(n) & CARD_ID_MASK])    /* gCardNumberToId[n] */
#define CARD_TYPE(id)           CARD_STATS_TYPE(CARD_STATS_WORD(id))
#define CARD_KIND(id)           CARD_STATS_KIND(CARD_STATS_WORD(id))

/* Matching: this unit calls AddSpriteXY through int parameters; sprite.h declares u16/s16/u16/u16. */
extern void AddSpriteXYInt(int x, int y, int shape, u32 tile) asm("AddSpriteXY");

/* text.h declares TextCanvasToTiles(u16 *, u16) and TextDrawString(s32, s32, u16, const u8 *);
 * this unit's calls compile through these views. */
extern void TextCanvasInit(u32 widthTiles, u32 heightTiles);
extern void TextDrawString(u32 x, u32 y, u32 sizeColor, const void *str);
extern void TextCanvasToTiles(void *dst, u32 bgColor);
/* bg.h declares LoadBgImage4bpp(u16, u16, u16, u16 *) and SetBgMapEntry(u16, u16, u16); text.h declares
 * DrawBgDecimal(u32, u32, int, u16). This unit's calls compile through these views. */
extern void LoadBgImage4bpp(u32 mapOffset, u32 palStart, u32 tileBase, const void *pack);
extern void DrawBgDecimal(u32 cellColors, u32 tileDigits, u32 value, u32 zeroPad);
extern void SetBgMapEntry(u32 screenBlock, u32 cell, u32 entry);

/* 0x081A451C: the 8-entry BG palette colour cycle of the pack-opening scene. */
extern const u16 gPackSceneRasterColors[8];
/* 0x0300045C (= gMain.bgMapBuffer): kept as this symbol; the base load must come after the row * 4
 * copy in GetPack_DrawCardRow. */
extern u16 gUnk_0300045C[];

/* Card category (same inline as in card_detail): Obelisk (card number 1910) gives RITUAL, Slifer and
 * Ra (1911, 1912) give EFFECT, the Magic/Trap/Ticket types give their kinds, else the monster kind
 * (0 normal, 1 effect, 2 fusion, 3 ritual).
 * Matching: keep the switch form: an if-chain on the card number folds into an unsigned range test and
 * CSEs the stats-word read; the switch keeps the separate compares and the fresh read per arm. */
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((u8)CARD_TYPE(id)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_KIND(id);
    }
}
/* Turn off the OBJ layer of the duel screen and clear the OBJ tile memory. */
void UnloadDuelUiGfx(void)
{
    gDuelScreen.uiGfxLoaded = 0;
    REG_DISPCNT &= 0xFFBF;
    MemClear16((void *)(VRAM + 0x10000), 0x10000);
}

/* Draw the card frame (by card type / class) into OBJ tile memory (hypothesis). */
void LoadCardFrame(u16 id)
{
    u16 x;
    u16 y;
    u16 j;
    const u16 *frame;
    const u16 *src;
    u16 *dst;
    u16 pal;
    switch ((u8)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP: frame = (const u16 *)gCardFrameTrapGfx; break;
    case CARD_TYPE_MAGIC: frame = (const u16 *)gCardFrameMagicGfx; break;
    case CARD_TYPE_TICKET: frame = (const u16 *)gCardFrameTicketGfx; break;
    default:
        switch (GetCardSubtype(id)) {
        case CARD_KIND_EFFECT: frame = (const u16 *)gCardFrameEffectGfx; break;
        case CARD_KIND_FUSION: frame = (const u16 *)gCardFrameFusionGfx; break;
        case CARD_KIND_RITUAL: frame = (const u16 *)gCardFrameRitualGfx; break;
        default: frame = (const u16 *)gCardFrameNormalGfx; break;
        }
        break;
    }
    src = (const u16 *)((const u8 *)frame + 0x10 + frame[0] * 2);
    MemCopy16((void *)(OBJ_PLTT + 0x20), (const u8 *)frame + 8, 0x40);
    /* A variable palette offset, read as (u8)pal: the ROM builds 0x1000 as pal << 8 per loop nest. */
    pal = 0x10;
    for (y = 0; y <= 3; y++) {
        for (x = 0; x <= 0xC; x++) {
            dst = (u16 *)(VRAM + 0x10000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                u16 w = *src;
                u16 v = w;
                /* FAKEMATCH: keeps the loaded word w apart from the accumulator v (as in LoadBgImage) */
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
            dst = (u16 *)(VRAM + 0x10000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                u16 w = *src;
                u16 v = w;
                /* FAKEMATCH: keeps the loaded word w apart from the accumulator v (as in LoadBgImage) */
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
            dst = (u16 *)(VRAM + 0x10000 + (((u16)((x << 1) + 0x16)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                u16 w = *src;
                u16 v = w;
                /* FAKEMATCH: keeps the loaded word w apart from the accumulator v (as in LoadBgImage) */
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
            dst = (u16 *)(VRAM + 0x10000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
            for (j = 0; j <= 0x1F; j++) {
                u16 w = *src;
                u16 v = w;
                /* FAKEMATCH: keeps the loaded word w apart from the accumulator v (as in LoadBgImage) */
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
/* Card picture loader: copies card id's 64-colour palette to OBJ palette 0x05000260, unpacks the
 * 6-bit-per-pixel picture (8 pixels per 3 halfwords, as in BattleScene_LoadCardArt) into the 9x10 8bpp OBJ
 * tiles at columns 2-0xA, rows 4-0xD, then adds palette base 0x30 to every pixel byte (hypothesis). */
void LoadCardPicture(u16 id)
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

    src = (const u16 *)(0x08608360 + id * CARD_ART_PALETTE_SIZE);   /* gCardArtPalettes[id] */
    dst = (u16 *)(OBJ_PLTT + 0x60);
    MemCopy16(dst, src, 0x80);
    src = (const u16 *)(0x082A6500 + id * CARD_ART_SIZE);   /* gCardArtGfx[id] */
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
            dst = (u16 *)(VRAM + 0x10000 + (((u16)(x << 1)) + ((u16)(y + 2) << 5)) * 32);
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
/* ATK * 10 (0 for Magic/Trap/Ticket, 4000 for Divine). */
static inline int GetCardAtk10(u16 id)
{
    int result;
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        result = 0; break;
    case CARD_TYPE_DIVINE:
        result = 4000; break;
    default:
        result = CARD_STATS_ATK(CARD_STATS_WORD(id)) * 10; break;
    }
    return result;
}

/* DEF * 10 (same special cases). */
static inline int GetCardDef10(u16 id)
{
    int result;
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        result = 0; break;
    case CARD_TYPE_DIVINE:
        result = 4000; break;
    default:
        result = CARD_STATS_DEF(CARD_STATS_WORD(id)) * 10; break;
    }
    return result;
}

/* Draw the card info panel (frame icon, level stars, ATK/DEF digits, spell/trap icon) (hypothesis). */
static inline int CardLevelCached(u16 id, u32 stats)
{
    /* Reuse the type already read by the caller; every arm assigns the result. */
    register int level asm("r0");
    switch ((int)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP: case CARD_TYPE_MAGIC: case CARD_TYPE_TICKET: level = 0; break;
    case CARD_TYPE_DIVINE: level = 10; break;
    default: level = CARD_STATS_LEVEL(CARD_STATS_WORD(id)); break;
    }
    return level;
}
void DrawCardInfo(u16 id)
{
    int starRow;
    int right;
    int i;
    int lvl;
    int v;
    int x;
    /* The subtype is assigned only in the non-monster arm, before its use. */
    register int sub asm("r2");
    u32 stats = CARD_STATS_WORD(id);
    switch ((int)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP:
        DrawCardImageIcon16(0x0006004E, 0x70, (const u16 *)0x086366A8, (const void *)0x08636348);
        break;
    case CARD_TYPE_MAGIC:
        DrawCardImageIcon16(0x0006004E, 0x70, (const u16 *)0x08636728, (const void *)0x08636368);
        break;
    default:
        DrawCardImageIcon16(0x0006004E, 0x70, (const u16 *)gCardIconGfx[CARD_STATS_ATTR(stats)],
                     (const void *)gCardIconPals[CARD_STATS_ATTR(stats)]);
        break;
    }
    stats = CARD_STATS_WORD(id);
    if (CARD_STATS_TYPE(stats) <= CARD_TYPE_REPTILE) {
        lvl = CardLevelCached(id, stats);
        i = 0;
        if (i >= lvl)
            goto stars_done;
        right = 0x54;
        starRow = 0x140000;
        x = right;
        do {
            if (lvl <= 9)
                DrawCardImageTile(x | starRow, 0x80, (const u16 *)0x0822C360, (const void *)0x0822C300);
            else
                DrawCardImageTile((right - 0x4E * i / lvl) | starRow, 0x80, (const u16 *)0x0822C360, (const void *)0x0822C300);
            x -= 8;
            i++;
        } while (i < lvl);
    stars_done:
        DrawCardImageTile(0x0076003F, 0x90, (const u16 *)0x0863856C, (const void *)gCardStatDigitsPal);
        DrawCardImageTile(0x00760047, 0x90, (const u16 *)0x0863858C, (const void *)gCardStatDigitsPal);
        v = GetCardAtk10(id);
        x = 0x57;
        do {
            DrawCardImageTile(0x760000 | x, 0x90, (const u16 *)(gCardStatDigitsGfx + v % 10 * 32), (const void *)gCardStatDigitsPal);
            v = v / 10;
            x -= 4;
        } while (v != 0);
        DrawCardImageTile(0x007E003F, 0x90, (const u16 *)0x086385AC, (const void *)gCardStatDigitsPal);
        DrawCardImageTile(0x007E0047, 0x90, (const u16 *)0x086385CC, (const void *)gCardStatDigitsPal);
        v = GetCardDef10(id);
        x = 0x57;
        do {
            DrawCardImageTile(0x7E0000 | x, 0x90, (const u16 *)(gCardStatDigitsGfx + v % 10 * 32), (const void *)gCardStatDigitsPal);
            v = v / 10;
            x -= 4;
        } while (v != 0);
    } else {
        int type = CARD_TYPE(id);
        switch (type) {
        case CARD_TYPE_TRAP: case CARD_TYPE_MAGIC: sub = CARD_STATS_SUBTYPE(CARD_STATS_WORD(id)); break;
        default: sub = 0; break;
        }
        if (sub != 0)
            DrawCardImageTile(0x00140050, 0x90, (const u16 *)(0x08637374 + sub * 32), (const void *)0x08637454);
    }
}

/* Sprite/tile offset for the card art of `id` (hypothesis). */
int GetCardIconObjTile(u16 id)
{
    int c;
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR: return 0x140;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA: return 0xC0;
    }
    switch ((u8)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP: return 0x1C0;
    case CARD_TYPE_MAGIC: return 0x180;
    case CARD_TYPE_TICKET: return 0x80;
    }
    c = GetCardSubtype(id);
    switch (c) {
    case CARD_KIND_NORMAL: c = 0x80; break;
    case CARD_KIND_EFFECT: c = 0xC0; break;
    case CARD_KIND_FUSION: c = 0x100; break;
    case CARD_KIND_RITUAL: c = 0x140; break;
    }
    return c;
}

/* Sprite/tile offset for the card frame of `id` (hypothesis: frame graphic offset). */
int GetCardIconBgTile(u16 id)
{
    int c;
    switch ((u8)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP: return 0x1F0;
    case CARD_TYPE_MAGIC: return 0x1B0;
    }
    c = GetCardSubtype(id);
    switch (c) {
    case CARD_KIND_NORMAL: return 0xB0;
    case CARD_KIND_EFFECT: return 0xF0;
    case CARD_KIND_FUSION: return 0x130;
    case CARD_KIND_RITUAL: return 0x170;
    }
    return c;
}

/* Map a zone row index to its group base: the spell/trap row (5..9) gives DUEL_AREA_SPELL_TRAP, the
 * field zone gives DUEL_AREA_FIELD, anything else gives 0 (the monster row). */
int GetZoneArea(int zone)
{
    int r = 0;
    if ((u32)(zone - DUEL_AREA_SPELL_TRAP) <= 4)
        r = DUEL_AREA_SPELL_TRAP;
    if (zone == DUEL_AREA_FIELD)
        r = DUEL_AREA_FIELD;
    return r;
}

/* Pixel x of hand slot `index` of `player`, offset by its step of a spread over `count` cards.
 * Matching: keep the e/d split (d = index * 128 + e, then the divide); folding it into one expression
 * swaps the commutative operands and the register numbers. */
s32 GetHandCardX(u32 player, u32 index, u32 count)
{
    s32 x = gDuelZonePositions[player][DUEL_AREA_HAND].x;
    if (count != 0) {
        s32 e = index * 32;
        s32 d = e;
        if ((s32)count > 5) {
            d = index * 128 + e;
            d = d / (s32)count;
        }
        if (player == 0)
            x += d;
        else
            x -= d;
    }
    return x;
}

/* X pixel of zone (area + index) of a player; the hand area is spread over the hand size. */
s32 GetAreaX(int player, int area, int index)
{
    s32 x = gDuelZonePositions[player][area + index].x;
    if (area == DUEL_AREA_HAND)
        x = GetHandCardX(player, index, gDuelPlayers[player & 1].handCount);
    return x;
}

/* Y pixel of a zone; the monster area is first mapped through GetZoneArea(index). */
s32 GetAreaY(u32 player, int area, int index)
{
    if (area == DUEL_AREA_MONSTER)
        area = GetZoneArea(index);
    return gDuelZonePositions[player][area].y - gDuelScreen.scroll;
}

/* Sets BG palette entry 15 of the border to a rotating colour from the table. */
void GetPack_HBlank(void)
{
    *(u16 *)(BG_PLTT + 0x1E) = gPackSceneRasterColors[(REG_VCOUNT + (gPackOpenWork.colorCyclePhase >> 1)) & 7];
}

/* Push the two frame counters into gMain and decrement the third.
 * Matching: the post-increments through a struct Main * local fix the ROM's load order. */
void GetPack_ScrollBg(void)
{
    u16 scroll;
    struct Main *m = &gMain;
    scroll = gPackOpenWork.bgScrollX++;
    m->bgHofs[3] = scroll;
    scroll = gPackOpenWork.bgScrollY++;
    m->bgVofs[3] = scroll;
    gPackOpenWork.colorCyclePhase--;
}

/* Card number to card id: 0xFFFF means none, below CARD_NUMBER_ALT_ART is a direct table lookup,
 * anything else uses the alternate-art entry + 1. */
static inline u16 CardNumberToId(u16 n)
{
    if (n == 0xFFFF)
        return 0;
    if (n <= CARD_NUMBER_ALT_ART - 1)
        return CARD_ID_FROM_NUMBER(n);
    return CARD_ID_FROM_NUMBER(n - CARD_NUMBER_ALT_ART) + 1;
}

/* Draw the five slot sprites of the list at 0x02015160 (hypothesis). */
/* Matching: gCardFlipAnimTiles is an extern symbol, not a cast-address form: the ROM loads this table
 * through a constant-pool symbol_ref, which loop.c hoists in its first pass; that pushes the 0xFFFF
 * compare hoist to the second pass so it wins sl. */
extern const u16 gCardFlipAnimTiles[];   /* 0x0808659C: per-kind sprite attribute words */

void GetPack_DrawCardSprites(void)
{
    int i;
    for (i = 0; i <= 4; i++) {
        if (gPackOpenWork.revealFrame[i] <= 0x17) {
            u16 tile;
            if (gCardFlipAnimTiles[gPackOpenWork.revealFrame[i]] & 0x1000)
                tile = gCardFlipAnimTiles[gPackOpenWork.revealFrame[i]] + GetCardIconObjTile(CardNumberToId(gPackOpenWork.cardNumbers[i]));
            else
                tile = gCardFlipAnimTiles[gPackOpenWork.revealFrame[i]];
            AddSpriteXYInt(-4, i * 32, 0x80, tile);
        } else {
            AddSpriteXYInt(-4, i * 32, 0x80, 0x1000 | GetCardIconObjTile(CardNumberToId(gPackOpenWork.cardNumbers[i])));
        }
    }
}
/* Spell/trap subtype (stats bits 17-19) for Magic and Trap cards, else 0. */
static inline int GetSpellSubtype(u32 stats)
{
    switch ((int)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(stats);
    default:
        return 0;
    }
}

/* Draw the card detail layout for `id` at row `row`: palette/tile map setup, type icons, ATK/DEF, level stars (hypothesis). */
/* ATK * 10 / DEF * 10 as u16 (as in CardListView_DrawCardInfo): the result lands in r0 and is copied to r2. */
static inline u16 GetCardAtk10U16(const u32 *stats, u16 id)
{
    switch ((int)CARD_STATS_TYPE(*stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_ATK(CARD_STATS_WORD(id)) * 10;
    }
}

static inline u16 GetCardDef10U16(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_DEF(CARD_STATS_WORD(id)) * 10;
    }
}

/* Level (0 for Magic/Trap/Ticket, 10 for Divine). The u8 type local and u8 return add zero
 * extensions that combine deletes only after loop.c: they raise the first loop pass's insn
 * count so the card-stats address is hoisted in pass 2 and the (row * 4 + 2) << 5 chain stays
 * in the star loop, as in the ROM. */
static inline u8 GetCardLevelU8(u16 id)
{
    u8 type = CARD_TYPE(id);
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS_WORD(id));
    }
}

void GetPack_DrawCardRow(int row, u16 id)
{
    int i;
    int k;
    u32 color;
    const u32 *stats;
    color = 7;
    if (gPackOpenWork.rareCardNumber == CARD_NUMBER(id))
        color = 0xF;
    TextCanvasInit(0x18, 2);
    /* DrawText takes a u16 colour (as declared in campaign_select); this unit's prototype says u32,
     * so call through the real signature (the u16 conversion gives the ROM's constant copy). */
    ((void (*)(int, int, u16, const void *))TextDrawString)(2, 6, color | 0xA00, (const void *)(0x0822C720 + id * CARD_NAME_SIZE));   /* gCardNames[id] */
    /* Tile base 0x40 of char block 0x06004000: CSE keeps row * 0x30 for the fill loop below. */
    TextCanvasToTiles((void *)(VRAM + 0x4000 + (row * 0x30 + 0x40) * 32), 0);
    for (i = 0; i <= 1; i++) {
        for (k = 0; k < 24; k++) {
            /* The index in its own local puts the table base load after row * 4 in the loop body,
             * which gives the ROM's hoist order (row * 4 copy before the 0x0300045C base). */
            int idx = (row * 4 + i) * 32 + 3 + k;
            gUnk_0300045C[idx] = row * 0x30 + 0x40 + k + i * 24;
        }
    }
    stats = &CARD_STATS_WORD(id);
    switch ((int)CARD_STATS_TYPE(*stats)) {
    case CARD_TYPE_TRAP:
        LoadBgImage4bpp(((u16)(row * 4 + 2) << 5) + 3, (row + 5) * 16, row * 4 + 0x300, (const void *)0x08636CD8);
        if (GetSpellSubtype(*stats) != 0)
            LoadBgImage4bpp((((u16)(row * 4 + 2) << 5) + 5), (row + 10) * 16, row * 4 + 0x320, (const void *)gSpellSubtypeIconImages[GetSpellSubtype(CARD_STATS_WORD(id))]);
        break;
    case CARD_TYPE_MAGIC:
        LoadBgImage4bpp(((u16)(row * 4 + 2) << 5) + 3, (row + 5) * 16, row * 4 + 0x300, (const void *)0x08636DA0);
        if (GetSpellSubtype(*stats) != 0)
            LoadBgImage4bpp((((u16)(row * 4 + 2) << 5) + 5), (row + 10) * 16, row * 4 + 0x320, (const void *)gSpellSubtypeIconImages[GetSpellSubtype(CARD_STATS_WORD(id))]);
        break;
    case CARD_TYPE_DIVINE:
        break;
    default: {
        const u32 *statsPtr;
        u32 r = (u16)(row * 4 + 2) << 5;
        LoadBgImage4bpp(r + 3, (row + 5) * 16, row * 4 + 0x300, (const void *)gAttributeIconImages[CARD_STATS_ATTR(*(statsPtr = &CARD_STATS_WORD(id)))]);
        LoadBgImage4bpp(r + 5, (row + 10) * 16, row * 4 + 0x320, (const void *)gMonsterTypeIconImages[CARD_STATS_TYPE(*statsPtr)]);
        LoadBgImage4bpp(r + 8, 0xF0, 0x340, (const void *)0x0863CA1C);
        DrawBgDecimal((u16)(((row * 4 + 2) << 5) + 9) | 0x70000, (u16)(row * 8 + 0x1A0) | 0x40000, GetCardAtk10U16(statsPtr, id), 0);
        DrawBgDecimal((u16)(((row * 4 + 3) << 5) + 9) | 0x70000, (u16)(row * 8 + 0x1C8) | 0x40000, GetCardDef10U16(id), 0);
        for (i = 0; i < GetCardLevelU8(id); i++)
            SetBgMapEntry(0, (u16)(i + 0xE + ((row * 4 + 2) << 5)), 2);
        break;
    }
    }
}
