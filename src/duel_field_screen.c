/*
 * duel_field_screen (0x080609C4-0x08061A18): duel screen setup and teardown, the screen fades, and the
 * board redraw routines (field cells, link and attack markers, hand cards)
 * (wiki/functions/duel-field-screen-c.md).
 *
 * DuelScreen_Init builds the field screen (BG setup, graphics, board, life points, phase letters, field
 * cells, text cells) and leaves it black; DuelScreen_FadeInStep / DuelScreen_FadeOutStep step the fade
 * around it and DuelScreen_Exit tears it down. The DuelFieldFade* helpers blend only the field layers
 * (BG0-2 and the backdrop) through gMain.brightness. ClearTileBlock4x4 / FillTileBlock4x4 write the
 * 4x4-tile field cells of the BG2 card layer in its IWRAM shadow (gUnk_03001C5C, 32 columns);
 * DrawZoneTiles, DrawAreaTiles and DrawAllAreaTiles redraw the cells from the duel state. Per frame,
 * DrawFieldOverlay draws the link markers of the card under the cursor plus the Battle Phase
 * can-attack markers and runs gDuelScreen.overlayCallback, DrawHandCards draws both players' hand
 * strips, and DrawLinkWaitIndicator the blinking "Wait..." sprite of a link duel. PlotCardImagePixel,
 * DrawCardImageTile and DrawCardImageIcon16 are the pixel blitters of the 8bpp card-image canvas.
 */
#include "global.h"
#include "constants/duel.h"     /* enum DuelArea, enum DuelPhase */
#include "legacy/gba.h"                /* REG_DISPCNT, REG_IME, REG_IE, REG_BLDCNT, REG_BLDY */
#include "legacy/main.h"               /* struct Main gMain (vblankFlags, vblankCallback, brightness, frameCounter) */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that duel_screen.h does not pull in the legacy header.
 * After H0, replace the block (BEGIN to END) with the include lines of duel.h and sound.h, in that order
 * (build/readability/issues/duel_field_screen.md). */
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
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 countB84;                    /* +0x006: entries in listB84[] */
    u8 unk7[0x26 - 0x7];
    u16 zoneMask;                   /* +0x026: per-zone bitmask (DrawFieldOverlay) */
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard listB84[80];    /* +0xB84 */
    u16 arrCC4[80];                 /* +0xCC4 */
};

/* The card command menu (gDuel.cardMenu, 0xC bytes): the human's card pick in a response window. */
struct CardMenu {
    u16 open:1;                     /* bit 0: menu open, the caller runs CardMenu_Update */
    u16 confirmed:1;                /* bit 1: a command was chosen */
    u16 command:4;                  /* bits 2-5: enum CardMenuCommand */
    u16 slide:4;                    /* bits 6-9: slide/zoom animation step 0-8 */
    u32 available:16;               /* bits 10-25: enum CardMenuCommandMask bits */
    u32 state:8;                    /* bits 26-33: CardMenu_Update state */
    u32 step:8;                     /* bits 34-41: step of the command handler */
    u8 summonSeq:4;                 /* bits 42-45 */
    u32 tributeSources:4;           /* bits 46-49 */
    u16 timer:7;                    /* bits 50-56: pulse timer of the selected icon */
    u16 player:1;                   /* bit 57: player of the confirmed command */
    u32 area:7;                     /* bits 58-64: enum DuelArea of the cursor at confirm */
    u32 index:8;                    /* bits 65-72: zone index (field) or hand index (hand) */
    u32 placeZone:8;                /* bits 73-80 */
    u32 unk0A_1:15;
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 fieldBackground:4;           /* +0x1ACC bits 0-3: field background index (DuelScreen_LoadFieldBackground) */
    u8 unk1ACC_4:4;
    u8 unk1ACD[0x1B12 - 0x1ACD];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 linkError:1;                 /* +0x1B12 bit 5 */
    u8 result:2;                    /* +0x1B12 bits 6-7 */
    u8 unk1B13[0x1B28 - 0x1B13];
    u16 cardMenuCard;               /* +0x1B28: card ID under the cursor when the command was confirmed */
    u16 summonTributes;             /* +0x1B2A */
    struct CardMenu cardMenu;       /* +0x1B2C */
    u8 unk1B38[0x1B78 - 0x1B38];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */
/* ---- END duel.h stand-in ---- */

#include "bg.h"                   /* ResetVideo */
#include "duel_screen.h"          /* struct DuelScreen gDuelScreen, gDuelZonePositions, the prototypes of the functions defined here, GetAreaX / GetAreaY, GetHandCardX, GetCardIconBgTile / GetCardIconObjTile */
#include "palette.h"              /* SetBrightnessBlack, ClearBlend */
#include "util.h"                 /* MemCopy16 */

/* ---- Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view") ---- */

/* gDuel as the three readers here use it: the players' life points, the field background and the
 * +0x1B12 / +0x1B2C flag bytes, reached through a cast pointer so the ROM derives the field addresses
 * from the gDuel base (a plain gDuel.cardMenu.open folds into one 0x0201AE0C literal). */
struct DuelStateView {
    u8 unk0[4];
    u16 lifePoints0;                /* +0x004 = players[0].lifePoints */
    u8 unk6[0xD68 - 0x6];
    u16 lifePoints1;                /* +0xD68 = players[1].lifePoints */
    u8 unkD6A[0x1ACC - 0xD6A];
    u8 fieldBackground:4;           /* +0x1ACC bits 0-3: field background index */
    u8 unk1ACC_4:4;
    u8 unk1ACD[0x1B12 - 0x1ACD];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 unk1B12_5:3;
    u8 unk1B13[0x1B2C - 0x1B13];
    u8 cardMenuOpen:1;              /* +0x1B2C bit 0 = cardMenu.open */
    u8 unk1B2C_1:7;
};
#define DUEL_VIEW ((struct DuelStateView *)&gDuel)

/* The frame tile slots at +0x20E6/+0x2140 sit inside struct Main's bgMapBuffer (include/main.h).
 * Matching: they are reached through a cast of &gMain so the base stays the gMain symbol; routing
 * the same address through an asm-aliased second symbol splits agbcc's base-pointer CSE in
 * DuelScreen_Init and changes its codegen. DuelScreen_Init fills the two halfword pairs with
 * 0x4280 + i (two BG map entries of the field screen). */
struct MainTileView {
    u8 unk0[0x20E6];
    u16 unk20E6[2];                 /* +0x20E6 */
    u8 unk20EA[0x2140 - 0x20EA];
    u16 unk2140[2];                 /* +0x2140 */
};
#define MAIN_TILE_VIEW ((struct MainTileView *)&gMain)

/* The interrupt handler table (0x03000000); only the HBlank slot is touched here. */
struct IntrTable {
    u32 unk0;                       /* +0x00 */
    void (*hblankCallback)(void);   /* +0x04: handler slot 1 */
};
extern struct IntrTable IntrTable;  /* 0x03000000 */

extern u16 gUnk_03001C5C[];                         /* 0x03001C5C: BG2 card-layer map shadow, 32 columns */
extern u8 gDuelFieldImage[];                        /* 0x0867BB7C: the field board image loaded by DuelScreen_Init */
extern const u32 gZoneMarkerAnimTiles[8];           /* 0x081A427C: OBJ tile of the marker animation frames */

/* Matching: bg.h declares LoadBgImage4bppMap1 with u16 parameters and a u16 *pack; this unit calls it
 * with u32 operands and the u8 image data. */
extern void LoadBgImage4bppMap1Wide(u32 mapOffset, u32 palStart, u32 tileBase, const void *pack) asm("LoadBgImage4bppMap1");
/* Matching: palette.h declares the fades as u32 (s32 step); this unit calls them u16-returning with a
 * u8 step, and the ROM narrows the result. */
extern u16 FadeToBlackU16(u8 step) asm("FadeToBlack");
extern u16 FadeFromBlackU16(u8 step) asm("FadeFromBlack");
/* battle.h declares u16 CanMonsterAttack(int, int, u16); this unit calls it through an int result. */
extern int CanMonsterAttack(u32 player, u32 zone, u32 checkCost);
extern int IsMonsterZoneFree(int player, int zone);
extern u32 IsHandRevealed(u32 player);
extern void ResetBgScroll(void);

/* Matching: ClearTileBlock4x4 / FillTileBlock4x4 are called through s32 parameters here. */
extern void ClearTileBlock4x4Int(s32 x, s32 y) asm("ClearTileBlock4x4");
extern void FillTileBlock4x4Int(s32 x, s32 y, u32 t) asm("FillTileBlock4x4");
/* duel_screen.h declares int GetCardIconBgTile(u16); the ROM's DrawAreaTiles was compiled against a
 * u32 parameter, and narrowing the shifted card word perturbs agbcc's register allocation there. */
extern int GetCardIconBgTileInt(u32 id) asm("GetCardIconBgTile");

void DuelScreen_Init(void)
{
    s32 i;
    /* FAKEMATCH: keep the initialized base tile in r3 during the two stores. */
    register u16 tile asm("r3");
    struct DuelStateView *g;
    u16 *a;
    u16 *b;

    REG_DISPCNT = 0;
    gMain.vblankFlags = 0x603;
    ResetVideo();
    DuelScreen_InitBgCnt();
    LoadDuelBgGfx();
    LoadDuelUiGfx();
    LoadBgImage4bppMap1Wide(0, 0x60, 0x10, gDuelFieldImage);
    i = 0;
    a = MAIN_TILE_VIEW->unk20E6;
    tile = 0x4280;
    b = MAIN_TILE_VIEW->unk2140;
    for (; i <= 1; a++, b++, i++) {
        /* Stage the halfword sum; agbcc combines this into one store. */
        *a = i;
        *a += tile;
        *b = tile + i;
    }
    g = DUEL_VIEW;
    DuelScreen_LoadFieldBackground(g->fieldBackground);
    DrawLifePoints(0, g->lifePoints0);
    DrawLifePoints(1, g->lifePoints1);
    DrawPhaseIndicator(g->turnPlayer, g->phase);
    DrawAllAreaTiles();
    TextCellsClear();
    TextCellsResetMap();
    SetBrightnessBlack();
    ResetBgScroll();
    gMain.vblankCallback = DuelScreen_VBlank;
    gDuelScreen.active = 1;
}

void DuelScreen_Exit(u16 full)
{
    REG_DISPCNT &= 0xE0FF;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    IntrTable.hblankCallback = NULL;
    REG_IME = 1;
    gMain.vblankCallback = NULL;
    if (full != 0) {
        SetBrightnessBlack();
        gDuelScreen.active = 0;
        gDuelScreen.uiGfxLoaded = 0;
    }
}
u16 DuelScreen_FadeInStep(void)
{
    REG_DISPCNT |= 0x1F00;
    return FadeFromBlackU16(4);
}
u32 DuelScreen_FadeOutStep(void)
{
    if (FadeToBlackU16(4) != 0) {
        DuelScreen_Exit(0);
        return 1;
    }
    return 0;
}
u32 DuelFieldFadeToBlack(s32 step)
{
    REG_BLDCNT = 0x27E7;
    if (gMain.brightness <= 0x1E) {
        gMain.brightness += step;
        if (gMain.brightness > 0x1F)
            gMain.brightness = 0x1F;
    }
    REG_BLDY = gMain.brightness;
    if (gMain.brightness <= 0x1E)
        return 0;
    REG_DISPCNT &= 0xF8FF;
    return 1;
}
u32 DuelFieldDim(s32 step)
{
    REG_BLDCNT = 0x27E7;
    if (gMain.brightness <= 0x8) {
        gMain.brightness += step;
        if (gMain.brightness > 0x9)
            gMain.brightness = 0x9;
    }
    REG_BLDY = gMain.brightness;
    if (gMain.brightness <= 0x8)
        return 0;
    return 1;
}
u32 DuelFieldFadeFromBlack(s32 step)
{
    REG_DISPCNT |= 0x700;
    if (gMain.brightness > step)
        gMain.brightness -= step;
    else
        gMain.brightness = 0;
    if (gMain.brightness != 0) {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = 0x27E7;
        return 0;
    }
    ClearBlend();
    return 1;
}
u32 DuelFieldFadeToWhite(s32 step)
{
    REG_BLDCNT = 0x27A7;
    if (gMain.brightness <= 0x1E) {
        gMain.brightness += step;
        if (gMain.brightness > 0x1F)
            gMain.brightness = 0x1F;
    }
    REG_BLDY = gMain.brightness;
    if (gMain.brightness <= 0x1E)
        return 0;
    return 1;
}
u32 DuelFieldFadeFromWhite(s32 step)
{
    if (gMain.brightness > step)
        gMain.brightness -= step;
    else
        gMain.brightness = 0;
    if (gMain.brightness != 0) {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = 0x27A7;
        return 0;
    }
    ClearBlend();
    return 1;
}
/* Clear a 4x4 block of the IWRAM tilemap buffer at (x, y). */
void ClearTileBlock4x4(u16 x, u16 y)
{
    u16 *p = gUnk_03001C5C + (x + (y << 5));

    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[0x20] = 0;
    p[0x21] = 0;
    p[0x22] = 0;
    p[0x23] = 0;
    p[0x40] = 0;
    p[0x41] = 0;
    p[0x42] = 0;
    p[0x43] = 0;
    p[0x60] = 0;
    p[0x61] = 0;
    p[0x62] = 0;
    p[0x63] = 0;
}
/* Fill a 4x4 block of the tilemap buffer at (x, y) with consecutive tiles starting at t. */
void FillTileBlock4x4(u32 x, u32 y, u16 t)
{
    u16 *p = gUnk_03001C5C + ((u16)x + ((u16)y << 5));

    p[0x0] = t++;
    p[0x1] = t++;
    p[0x2] = t++;
    p[0x3] = t++;
    p[0x20] = t++;
    p[0x21] = t++;
    p[0x22] = t++;
    p[0x23] = t++;
    p[0x40] = t++;
    p[0x41] = t++;
    p[0x42] = t++;
    p[0x43] = t++;
    p[0x60] = t++;
    p[0x61] = t++;
    p[0x62] = t++;
    p[0x63] = t;
}
/* Redraw the board tile block of zone (player, zone): an empty zone clears it (and draws the 2x2 empty marker for monster zones), otherwise a card back/face tile block. */
void DrawZoneTiles(u32 player, u32 zone)
{
    struct DuelZonesPlayer *playerZones = &gDuelZones[1 & player];
    struct DuelZone *zonePtr = &playerZones->zones[zone];
    u16 id = (*(u32 *)zonePtr << 20) >> 20;
    s32 x = gDuelZonePositions[player][zone].x / 8;
    s32 y = gDuelZonePositions[player][zone].y / 8;
    u16 baseTile;
    u16 *map;
    u16 markerX;
    u16 markerY;

    if (id == 0) {
        ClearTileBlock4x4Int(x, y);
        if ((s32)zone <= 4 && IsMonsterZoneFree(player, zone) == 0) {
            markerX = x + 1;
            markerY = y + 1;
            map = gUnk_03001C5C + (markerX + (markerY << 5));
            map[0] = FIELD_TILE_HELD_ZONE_MARK;
            map[1] = FIELD_TILE_HELD_ZONE_MARK + 1;
            map[0x20] = FIELD_TILE_HELD_ZONE_MARK + 2;
            map[0x21] = FIELD_TILE_HELD_ZONE_MARK + 3;
        }
    } else {
        baseTile = FIELD_TILE_CARD_BACK;
        if (zonePtr->isFaceUp)
            baseTile = GetCardIconBgTile(id) + FIELD_TILE_ICON_PAL;
        if (zonePtr->isDefense)
            baseTile += FIELD_TILE_POSE_SIDEWAYS;
        FillTileBlock4x4Int(x, y, baseTile);
    }
}


void ClearZoneTiles(u32 player, u32 area)
{
    ClearTileBlock4x4Int(gDuelZonePositions[player][area].x / 8, gDuelZonePositions[player][area].y / 8);
}
/* Redraw the board tile block for one area of `player`: the zone rows (MONSTER/SPELL_TRAP/FIELD with
 * `index`), the fusion deck and deck piles (thick above 5 cards), the top card of the graveyard and
 * of the banished pile. The pile fields are struct DuelPlayer's (graveCount/graveyard at +0x904,
 * countB84/listB84 at +0xB84, arrCC4 at +0xCC4). */
/* Banished-pile fields of struct DuelPlayer through a scalar-array view (countB84 +0x6, the pile as
 * u32 card words at +0xB84, the +0xCC4 table as bytes). Matching: the canonical DuelPlayer spelling
 * for these (struct DuelCard listB84 / u16 arrCC4 behind casts) changes agbcc's register allocation
 * in DrawAreaTiles' DUEL_AREA_BANISHED case, so the case reads this view; the extern keeps its
 * address-suffixed alias symbol, which the link resolves to gDuelPlayers (0x020192E4). */
struct DuelPlayerBanishedView {
    u8 unk0[6];
    u8 countB84;                    /* +0x006: entries in listB84 */
    u8 unk7[0xB84 - 0x7];
    u32 listB84[80];                /* +0xB84: card words */
    u8 arrCC4[0xA0];                /* +0xCC4: byte form of the 2-byte entries */
};
extern struct DuelPlayerBanishedView gPS1004_020192E4[2];   /* = gDuelPlayers */

void DrawAreaTiles(u32 player, u32 area, u32 index)
{
    s32 x = gDuelZonePositions[player][area].x / 8;
    s32 y = gDuelZonePositions[player][area].y / 8;
    u16 baseTile;
    u8 pileCount;

    switch (area) {
    case DUEL_AREA_MONSTER:
    case DUEL_AREA_SPELL_TRAP:
    case DUEL_AREA_FIELD:
        DrawZoneTiles(player, area + index);
        break;
    case DUEL_AREA_FUSION_DECK:
        pileCount = gDuelPlayers[1 & player].fusionCount;
        if (pileCount != 0) {
            if (pileCount > 5)
                FillTileBlock4x4Int(x, y, FIELD_TILE_THICK_PILE);
            else
                FillTileBlock4x4Int(x, y, FIELD_TILE_CARD_BACK);
        } else {
            ClearTileBlock4x4Int(x, y);
        }
        break;
    case DUEL_AREA_DECK:
        pileCount = gDuelPlayers[1 & player].deckCount;
        if (pileCount != 0) {
            if (pileCount > 5)
                FillTileBlock4x4Int(x, y, FIELD_TILE_THICK_PILE);
            else
                FillTileBlock4x4Int(x, y, FIELD_TILE_CARD_BACK);
        } else {
            ClearTileBlock4x4Int(x, y);
        }
        break;
    case DUEL_AREA_GRAVEYARD:
        if (gDuelPlayers[1 & player].graveCount != 0) {
            u32 *list;
            s32 offset;

            /* FAKEMATCH: the byte-offset variable gives the ROM's "lsl; sub #4; add list" order;
             * list[count - 1] is distributed into (list + count*4) - 4 instead. */
            list = (u32 *)gDuelPlayers[1 & player].graveyard;
            offset = gDuelPlayers[1 & player].graveCount * 4 - 4;
            list = (u32 *)((u8 *)list + offset);
            FillTileBlock4x4Int(x, y, (u16)(GetCardIconBgTileInt((*list << 20) >> 20) + FIELD_TILE_ICON_PAL));
        } else {
            ClearTileBlock4x4Int(x, y);
        }
        break;
    case DUEL_AREA_BANISHED:
        if (gPS1004_020192E4[1 & player].countB84 != 0) {
            u32 *list;
            s32 offset;

            /* FAKEMATCH: same byte-offset form as DUEL_AREA_GRAVEYARD. */
            list = gPS1004_020192E4[1 & player].listB84;
            offset = gPS1004_020192E4[1 & player].countB84 * 4 - 4;
            list = (u32 *)((u8 *)list + offset);
            baseTile = GetCardIconBgTileInt((*list << 20) >> 20) + FIELD_TILE_ICON_PAL;
            if (gPS1004_020192E4[1 & player].arrCC4[(gPS1004_020192E4[1 & player].countB84 - 1) * 2] == 2)
                FillTileBlock4x4Int(x, y, FIELD_TILE_CARD_BACK);
            else
                FillTileBlock4x4Int(x, y, baseTile);
        } else {
            ClearTileBlock4x4Int(x, y);
        }
        break;
    }
}

void DrawAllAreaTiles(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            DrawAreaTiles(i, DUEL_AREA_MONSTER, j);
            DrawAreaTiles(i, DUEL_AREA_SPELL_TRAP, j);
        }
        DrawAreaTiles(i, DUEL_AREA_FIELD, 0);
        DrawAreaTiles(i, DUEL_AREA_FUSION_DECK, 0);
        DrawAreaTiles(i, DUEL_AREA_DECK, 0);
        DrawAreaTiles(i, DUEL_AREA_GRAVEYARD, 0);
        DrawAreaTiles(i, DUEL_AREA_BANISHED, 0);
    }
}
/* Draw two animated 8x8 cursor sprites at the screen positions of two zones (locA, locB = player | zone << 8); the tile cycles with frameCounter / 8. */
void DrawLinkMarkerPair(u16 locA, u16 locB, u16 markerTile)
{
    u8 playerA, zoneA, playerB, zoneB;
    u32 xA, yA, xB, yB;
    u16 tile;

    playerA = locA;
    zoneA = locA >> 8;
    xA = GetAreaX(playerA, DUEL_AREA_MONSTER, zoneA) + 8;
    yA = GetAreaY(playerA, DUEL_AREA_MONSTER, zoneA) + 8;
    playerB = locB;
    zoneB = locB >> 8;
    xB = GetAreaX(playerB, DUEL_AREA_MONSTER, zoneB) + 8;
    yB = GetAreaY(playerB, DUEL_AREA_MONSTER, zoneB) + 8;
    xA |= yA << 16;
    tile = gZoneMarkerAnimTiles[(gMain.frameCounter >> 3) & 7] + markerTile + 0x5400;
    AddSprite(xA, 0x40, tile);
    xB |= yB << 16;
    tile = gZoneMarkerAnimTiles[(gMain.frameCounter >> 3) & 7] + markerTile + 0x5400;
    AddSprite(xB, 0x40, tile);
}
/* Private views of one zone (0x94 bytes) and of the per-player block (0xD64 bytes) for DrawZoneLinkMarkers:
 * the zone's link kinds are read as bytes (the low bytes of DuelZone.linkKinds) and the player's held-zone
 * mask straddles the +0xB/+0xC byte boundary. */
struct ZoneLinkView {
    u32 cardWord;               /* +0x00: card word, id in the low 12 bits */
    u8 unk4;
    u8 unk5;
    u8 flags6;                  /* +0x06: bit 1 = face up */
    u8 unk7;
    u8 unk8[2];
    u16 links[0x20];            /* +0x0A: linked zone refs (player | zone << 8) = DuelZone.links */
    u8 linkKindBytes[0x40];     /* +0x4A: low bytes of DuelZone.linkKinds, stride 2 */
    u16 numLinks;               /* +0x8A = DuelZone.numLinks */
    u8 unk8C[0x94 - 0x8C];
};
struct DuelPlayerLinkMaskView {
    u8 unk0[0xB];
    u32 unkB_0:4;
    u32 heldZoneMask:5;         /* +0xB bit 4 .. +0xC bit 0: zones held for a banished card (hypothesis); straddles the byte, read as two ldrb + shift/or */
    u32 unkC_1:7;
    u8 unkD[0xD64 - 0xD];
} __attribute__((packed));
extern struct DuelPlayerLinkMaskView gPS12F4_020192E4[2];   /* = gDuelPlayers */
extern u8 gZ8_12F4_0201930C[];                              /* = gDuelZones, byte form */
/* Byte-array form: in a dereference the offset is expanded as (1 & p), z * 0x94, then * 0xD64, as in the ROM. */
#define ZONE_LINK_VIEW(p, z) ((struct ZoneLinkView *)&gZ8_12F4_0201930C[(1 & (p)) * 0xD64 + (z) * 0x94])
#define PACK_LOC(p, z) ((u16)((u8)(p) | ((u8)(z) << 8)))

/* Draw cursor pairs for the links of zone (player, zone): the SPELL_TRAP area first scans every face-up occupied zone
   for links pointing at (player, zone); areas MONSTER, SPELL_TRAP and FIELD then draw the zone's own links (kinds
   1/5/10 -> MARKER_TILE_LINK_EQUIP, 2/7 -> MARKER_TILE_LINK_OTHER) and, when bit `zone` of the player's held-zone
   mask is set, a MARKER_TILE_LINK_OTHER cursor from (player, DUEL_AREA_BANISHED). */
void DrawZoneLinkMarkers(u32 player, u32 zone, u32 area)
{
    s32 scanPlayer;
    s32 scanZone;
    s32 linkIdx;
    u16 linkLoc;
    u8 linkKind;

    switch ((s32)area) {
    case DUEL_AREA_SPELL_TRAP:
        for (scanPlayer = 0; scanPlayer <= 1; scanPlayer++) {
            for (scanZone = 0; scanZone <= 10; scanZone++) {
                if ((*(u32 *)ZONE_LINK_VIEW(scanPlayer, scanZone) << 20) != 0 && (ZONE_LINK_VIEW(scanPlayer, scanZone)->flags6 & 2)) {
                    for (linkIdx = 0; linkIdx < ZONE_LINK_VIEW(scanPlayer, scanZone)->numLinks; linkIdx++) {
                        linkLoc = ZONE_LINK_VIEW(scanPlayer, scanZone)->links[linkIdx];
                        switch (ZONE_LINK_VIEW(scanPlayer, scanZone)->linkKindBytes[linkIdx * 2]) {
                        case 1:
                        case 5:
                        case 10:
                            if (linkLoc == PACK_LOC(player, zone))
                                DrawLinkMarkerPair(linkLoc, PACK_LOC(scanPlayer, scanZone), MARKER_TILE_LINK_EQUIP);
                            break;
                        case 2:
                        case 7:
                            if (linkLoc == PACK_LOC(player, zone))
                                DrawLinkMarkerPair(linkLoc, PACK_LOC(scanPlayer, scanZone), MARKER_TILE_LINK_OTHER);
                            break;
                        }
                    }
                }
            }
        }
        /* fallthrough */
    case DUEL_AREA_MONSTER:
    case DUEL_AREA_FIELD:
        for (linkIdx = 0; linkIdx < ZONE_LINK_VIEW(player, zone)->numLinks; linkIdx++) {
            linkLoc = ZONE_LINK_VIEW(player, zone)->links[linkIdx];
            linkKind = ZONE_LINK_VIEW(player, zone)->linkKindBytes[linkIdx * 2];
            if ((ZONE_LINK_VIEW(player, zone)->flags6 & 2) || player == 0) {
                switch (linkKind) {
                case 1:
                case 5:
                case 10:
                    DrawLinkMarkerPair(linkLoc, PACK_LOC(player, zone), MARKER_TILE_LINK_EQUIP);
                    break;
                case 2:
                case 7:
                    DrawLinkMarkerPair(linkLoc, PACK_LOC(player, zone), MARKER_TILE_LINK_OTHER);
                    break;
                }
            }
        }
        if ((gPS12F4_020192E4[1 & player].heldZoneMask >> zone) & 1)
            DrawLinkMarkerPair((u8)player | (DUEL_AREA_BANISHED << 8), (u8)player | ((u8)zone << 8), MARKER_TILE_LINK_OTHER);
        break;
    }
}
/* Per-player state block (0xD64 bytes) at gDuel + 4; only the +0x26 zone mask is used here. */
struct PlayerZoneMaskView {
    u8 unk0[0x26];
    u16 zoneMask;                   /* +0x26 = DuelPlayer.zoneMask */
    u8 unk28[0xD64 - 0x28];
};
/* Per-frame board overlay: act on the cursor zone (row 0 occupied / row 5) via DrawZoneLinkMarkers, draw an animated cursor
   sprite on each monster zone not flagged in the player's +0x26 mask, then run the optional callback at +0x85C. */
void DrawFieldOverlay(void)
{
    u32 cursorPlayer;
    u32 cursorArea;
    u32 cursorZone;
    s32 i;
    u32 x;
    u32 y;
    u16 tile;

    if ((*(u8 *)&gDuelScreen & 6) == 6) {
        cursorPlayer = gDuelScreen.selPlayer;
        cursorArea = gDuelScreen.selArea;
        cursorZone = cursorArea + gDuelScreen.selIndex;
        switch (cursorArea) {
        case DUEL_AREA_MONSTER:
            /* Explicit (p * 0xD64 + z * 0x94 + base): the operand order multiplies p first. */
            if (*(u32 *)((1 & cursorPlayer) * 0xD64 + cursorZone * 0x94 + (u32)gDuelZones) << 20 != 0)
                DrawZoneLinkMarkers(cursorPlayer, cursorZone, DUEL_AREA_MONSTER);
            break;
        case DUEL_AREA_SPELL_TRAP:
            DrawZoneLinkMarkers(cursorPlayer, cursorZone, DUEL_AREA_SPELL_TRAP);
            break;
        }
        if (DUEL_VIEW->phase == PHASE_BATTLE) {
            for (i = 0; i <= 4; i++) {
                if (CanMonsterAttack(DUEL_VIEW->turnPlayer, i, 0) != 0) {
                    /* Base through the gDuel cast so CSE derives it as (sym + 4) and loop hoists it. */
                    if (((((struct PlayerZoneMaskView *)((u8 *)&gDuel + 4))[1 & DUEL_VIEW->turnPlayer].zoneMask >> i) & 1) == 0) {
                        x = GetAreaX(DUEL_VIEW->turnPlayer, DUEL_AREA_MONSTER, i);
                        y = GetAreaY(DUEL_VIEW->turnPlayer, DUEL_AREA_MONSTER, i);
                        x += 8;
                        y += 8;
                        y <<= 16; /* separate shift keeps the OR result in x's register */
                        x |= y;
                        tile = gZoneMarkerAnimTiles[(gMain.frameCounter >> 3) & 7] + MARKER_TILE_CAN_ATTACK + 0x5400;
                        AddSprite(x, 0x40, tile);
                    }
                }
            }
        }
        if (gDuelScreen.overlayCallback != NULL)
            gDuelScreen.overlayCallback();
    }
}
/* Draw the small card sprites of each player's hand strip (area DUEL_AREA_HAND), skipping the one under the cursor. */
void DrawHandCards(void)
{
    s32 player;
    u16 revealed;
    u32 y;
    u32 x;
    u32 cardId;
    u16 tile;
    s32 handCount;
    s32 cardIdx;
    struct DuelCard *card;

    if ((*(u8 *)&gDuelScreen & 6) == 6) {
        for (player = 0; player <= 1; player++) {
            /* Ternary (not if/else): revealed then has 5 refs and loses r9 to the hoisted player offset. */
            revealed = player != 0 ? IsHandRevealed(player) : 1;
            y = GetAreaY(player, DUEL_AREA_HAND, 0);
            if (y + 0x20 <= 0xBF) {
                handCount = gDuelPlayers[1 & player].handCount;
                for (cardIdx = 0; cardIdx < handCount; cardIdx++) {
                    /* Two statements so fold keeps (base + player offset) + i * 4. */
                    card = gDuelPlayers[1 & player].hand;
                    card += cardIdx;
                    x = GetHandCardX(player, cardIdx, handCount);
                    cardId = card->id;
                    tile = revealed ? GetCardIconObjTile(cardId) + 0x1000 : 0x40;
                    if (cardId != 0) {
                        /* Through the cast view the flag stays gDuel + 0x1B2C (two literals);
                           a plain gDuel.cardMenu.open folds into one 0x0201AE0C literal. */
                        if (!(DUEL_VIEW->cardMenuOpen && gDuelScreen.selPlayer == player && gDuelScreen.selArea == DUEL_AREA_HAND
                              && gDuelScreen.selIndex == cardIdx))
                            AddSprite((y << 16) | x, 0x80, tile + 0x400);
                    }
                }
            }
        }
    }
}
void LinkWaitStart_Nop(void)
{
}
void LinkWaitEnd_Nop(void)
{
}
void DrawLinkWaitIndicator(void)
{
    u16 tile = 0x324;

    if (gMain.frameCounter & 0x20)
        tile += 0x20;
    if ((*(u8 *)&gDuelScreen & 6) == 6) {
        AddSprite(0x00400058, 0x40C0, tile |= 0x6000);
    }
}
/* Plot one pixel of colour `color` at (x, y) into 8bpp OBJ tile data at 0x06010000 (tile rows of 64 bytes, 2 tiles per 8 pixels of x, starting two rows down). */
void PlotCardImagePixel(u32 x, u32 y, u32 color)
{
    /* FAKEMATCH: retain the ROM's tile-column/address register. */
    register u32 tileOffset __asm__("r4") = (x << 13) >> 16;
    u32 tileY = (y << 13) >> 16;
    u32 xLow = (x & 7) << 16;
    u32 yLow = y & 7;
    u32 *wordPtr;
    u32 word;
    u8 buf[4];
    u32 pixelIdx;
    u8 *bytePtr;

    tileOffset = (u16)(tileOffset << 1);
    tileOffset = (tileOffset + ((u16)(tileY + 2) << 5)) << 5;
    wordPtr = (u32 *)(0x06010000 + tileOffset);
    wordPtr = (u32 *)((u32)wordPtr + ((xLow >> 18) << 2));
    wordPtr = (u32 *)((u32)wordPtr + (yLow << 3));
    word = *wordPtr;
    buf[0] = word;
    buf[1] = (u16)word >> 8;
    bytePtr = buf;
    word >>= 16;
    bytePtr[2] = word;
    word >>= 8;
    bytePtr[3] = word;
    pixelIdx = 0x30000;
    pixelIdx &= xLow;
    pixelIdx >>= 16;
    buf[pixelIdx] = color;
    *wordPtr = buf[0] | (buf[1] << 8) | ((buf[2] | (buf[3] << 8)) << 16);
}
/* Blit an 8x8 4bpp tile (`gfx`, 16 halfwords, 2 per row) as pixels through PlotCardImagePixel at yx, colours offset by palBase; also loads the 16-colour palette `pal` at the matching OBJ palette slot. */
void DrawCardImageTile(u32 yx, u32 palBase, const u16 *gfx, const void *pal)
{
    /* FAKEMATCH: preserve the ROM's source-pointer register before narrowing palette base. */
    register const u16 *src __asm__("r5") = gfx;
    u16 palBaseNarrow = palBase;
    u16 i;
    u16 j;
    u16 k;
    u16 halfword;
    u16 x0 = yx;
    u32 y0 = yx >> 16;

    if (src == 0 || pal == 0)
        return;
    MemCopy16((void *)(0x05000200 + ((palBaseNarrow >> 4) << 5)), pal, 0x40);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 2; j++) {
            halfword = *src;
            for (k = 0, src++; k < 4; k++) {
                u32 color = (halfword >> (k * 4)) & 0xF;

                if (color != 0)
                    PlotCardImagePixel(x0 + k + j * 4, y0 + i, (u8)(color + palBaseNarrow));
            }
        }
    }
}
/* Blit a 16x16 image made of four 8x8 tiles (0x20 bytes apart) at yx. */
void DrawCardImageIcon16(u32 yx, u16 palBase, const u16 *gfx, const void *pal)
{
    u16 x = yx;
    u32 y = yx >> 16;

    DrawCardImageTile(x | (y << 16), palBase, gfx, pal);
    gfx += 0x10;
    DrawCardImageTile((x + 8) | (y << 16), palBase, gfx, pal);
    gfx += 0x10;
    DrawCardImageTile(x | ((y + 8) << 16), palBase, gfx, pal);
    gfx += 0x10;
    DrawCardImageTile((x + 8) | ((y + 8) << 16), palBase, gfx, pal);
}
