#include "global.h"
#include "gba.h"

/* gMain (0x03000040), only the fields used here. */
struct Main {
    u8 filler0[0x40E];
    u16 vblankFlags;                /* +0x40E */
    u8 filler410[4];
    void (*vblankCallback)(void);   /* +0x414 */
    u8 filler418[0x20E6 - 0x418];
    u16 unk20E6[2];                 /* +0x20E6 */
    u8 filler20EA[0x2140 - 0x20EA];
    u16 unk2140[2];                 /* +0x2140 */
    u8 filler2144[0x4832 - 0x2144];
    u8 brightness : 6;              /* +0x4832 fade level 0..0x1F */
    u8 brightnessFlags : 2;
    u8 filler4833[0x485E - 0x4833];
    u16 frameCounter;               /* +0x485E */
};
extern struct Main gMain;
#define gMain gMain

/* Duel screen flags at 0x0201CFB0 (byte 0). */
struct DuelFlags {
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
    u8 pad1[0x824 - 1];
    u32 w824;                   /* cursor player */
    u32 w828;                   /* cursor zone (row) */
    u32 w82C;                   /* cursor zone (column) */
    u8 pad830[0x85C - 0x830];
    void (*cb85C)(void);        /* +0x85C optional per-frame callback */
};
extern struct DuelFlags gDuelScreen;
struct DuelGlobals {
    u8 pad0[4];
    u16 w4;                     /* +4 */
    u8 pad6[0xD68 - 6];
    u16 wD68;                   /* +0xD68 */
    u8 padD6A[0x1ACC - 0xD6A];
    u8 v1ACC : 4;               /* +0x1ACC */
    u8 v1ACC_hi : 4;
    u8 pad1ACD[0x1B12 - 0x1ACD];
    u8 f1B12_0 : 1;             /* +0x1B12 */
    u8 f1B12_1 : 1;
    u8 f1B12_2 : 3;
    u8 f1B12_5 : 3;
    u8 pad1B13[0x1B2C - 0x1B13];
    u8 f1B2C_0 : 1;             /* +0x1B2C */
    u8 f1B2C_rest : 7;
};
extern struct DuelGlobals gDuel;
extern u16 gUnk_03001C5C[];       /* BG tilemap buffer in IWRAM, 32 columns */
extern u8 gDuelFieldImage[];
struct DuelCard { u32 id : 12; u32 unk12 : 20; };
struct DuelZone {
    struct DuelCard card;   /* +0 */
    u8 unk4;
    u8 unk5;
    u8 flags6;              /* bit 0 / bit 1 tested by the board renderer, bit 1 = face-down */
    u8 unk7;
    u8 pad8[0xA - 8];
    u16 ids[0x20];          /* +0xA attached-effect card ids */
    u8 types[0x40];         /* +0x4A attached-effect types */
    u16 count;              /* +0x8A attached-effect count */
    u8 pad8C[0x94 - 0x8C];
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))
#define ZB3(p, z) ((struct DuelZone *)((p) * 0xD64 + (u32)gDuelZones + (z) * 0x94))
struct ZonePos { s32 x; s32 y; };         /* pixel position of a zone, per player (0x10 entries each) */
extern struct ZonePos gDuelZonePositions[2][16];
extern int IsMonsterZoneFree(int player, int zone);
extern int GetCardIconBgTile(u32 id);
extern u32 GetAreaX(u32 player, u32 a, u32 b);
extern u32 GetAreaY(u32 player, u32 a, u32 b);
extern void sub_08060DD8_i(s32 x, s32 y) asm("ClearTileBlock4x4");
extern void sub_08060E2C_i(s32 x, s32 y, u32 t) asm("FillTileBlock4x4");
extern const u32 gZoneMarkerAnimTiles[];
extern u32 IsHandRevealed(u32 player);
extern u32 GetHandCardX(u32 a, u32 b, u32 c);
extern int GetCardIconObjTile(u32 id);
extern int CanMonsterAttack(u32 a, u32 b, u32 c);
extern void DrawZoneLinkMarkers(u32 player, u32 zone, u32 kind);
extern void DrawZoneTiles(u32 player, u32 zone);
struct PlayerState {            /* 0xD64 bytes, at 0x020192E4 + player * 0xD64 */
    u8 pad0[2];
    u8 b2;                      /* +2 */
    u8 b3;                      /* +3 */
    u8 b4;                      /* +4 */
    u8 b5;                      /* +5 */
    u8 b6;                      /* +6 */
    u8 pad7[0xB - 7];
    u8 unkB;                    /* +0xB high nibble in the +0x1B2C bitmask */
    u8 unkC;                    /* +0xC bit 0 in the bitmask */
    u8 padD[0x26 - 0xD];
    u16 u26;                    /* +0x26 bitmask */
    u8 pad28[0x904 - 0x28];
    u32 a904[0x280 / 4];        /* +0x904 card words */
    u32 aB84[0x140 / 4];        /* +0xB84 card words */
    u8 aCC4[0x100];             /* +0xCC4 2-byte entries */
};
extern struct PlayerState gDuelPlayers[2];
struct IntrVectors {
    u32 unk0;
    void (*hblankCallback)(void);
};
extern struct IntrVectors IntrTable;

extern void ResetVideo(void);
extern void DuelScreen_InitBgCnt(void);
extern void LoadDuelBgGfx(void);
extern void LoadDuelUiGfx(void);
extern void LoadBgImage4bppMap1(u32 a, u32 b, u32 c, const void *d);
extern void DuelScreen_LoadFieldBackground(u32 a);
extern void DrawLifePoints(u32 a, u32 b);
extern void DrawPhaseIndicator(u32 a, u32 b);
extern void DrawAllAreaTiles(void);
extern void TextCellsClear(void);
extern void TextCellsResetMap(void);
extern void SetBrightnessBlack(void);
extern void ResetBgScroll(void);
extern void DuelScreen_VBlank(void);
extern void ClearBlend(void);
extern u16 FadeToBlack(u8 step);
extern u16 FadeFromBlack(u8 step);
extern void AddSprite(u32 yx, u16 shape, u16 attr2);
extern void DrawAreaTiles(u32 a, u32 b, u32 c);
extern void PlotCardImagePixel(u32 x, u32 y, u32 color);
extern void DrawCardImageTile(u32 yx, u32 palBase, const u16 *src, const void *pal);
extern void MemCopy16(void *dst, const void *src, u32 size);

void DuelScreen_Init(void)
{
    s32 i;
    /* FAKEMATCH: keep the initialized base tile in r3 during the two stores. */
    register u16 tile asm("r3");
    struct DuelGlobals *g;
    u16 *a;
    u16 *b;

    REG_DISPCNT = 0;
    gMain.vblankFlags = 0x603;
    ResetVideo();
    DuelScreen_InitBgCnt();
    LoadDuelBgGfx();
    LoadDuelUiGfx();
    LoadBgImage4bppMap1(0, 0x60, 0x10, gDuelFieldImage);
    i = 0;
    a = gMain.unk20E6;
    tile = 0x4280;
    b = gMain.unk2140;
    for (; i <= 1; a++, b++, i++) {
        /* Stage the halfword sum; agbcc combines this into one store. */
        *a = i;
        *a += tile;
        *b = tile + i;
    }
    g = &gDuel;
    DuelScreen_LoadFieldBackground(g->v1ACC);
    DrawLifePoints(0, g->w4);
    DrawLifePoints(1, g->wD68);
    DrawPhaseIndicator(g->f1B12_1, g->f1B12_2);
    DrawAllAreaTiles();
    TextCellsClear();
    TextCellsResetMap();
    SetBrightnessBlack();
    ResetBgScroll();
    gMain.vblankCallback = DuelScreen_VBlank;
    gDuelScreen.bit2 = 1;
}

void DuelScreen_Exit(u16 arg)
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
    if (arg != 0) {
        SetBrightnessBlack();
        gDuelScreen.bit2 = 0;
        gDuelScreen.bit1 = 0;
    }
}
u16 DuelScreen_FadeInStep(void)
{
    REG_DISPCNT |= 0x1F00;
    return FadeFromBlack(4);
}
u32 DuelScreen_FadeOutStep(void)
{
    if (FadeToBlack(4) != 0) {
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
    struct DuelZonesPlayer *pp = &gDuelZones[1 & player];
    struct DuelZone *z = &pp->zones[zone];
    u16 id = (*(u32 *)z << 20) >> 20;
    s32 x = gDuelZonePositions[player][zone].x / 8;
    s32 y = gDuelZonePositions[player][zone].y / 8;
    u16 t;
    u16 *p;
    u16 xx;
    u16 yy;

    if (id == 0) {
        sub_08060DD8_i(x, y);
        if ((s32)zone <= 4 && IsMonsterZoneFree(player, zone) == 0) {
            xx = x + 1;
            yy = y + 1;
            p = gUnk_03001C5C + (xx + (yy << 5));
            p[0] = 0x1240;
            p[1] = 0x1241;
            p[0x20] = 0x1242;
            p[0x21] = 0x1243;
        }
    } else {
        t = 0x1070;
        if (z->flags6 & 2)
            t = GetCardIconBgTile(id) + 0x2000;
        if (z->flags6 & 1)
            t += 0x30;
        sub_08060E2C_i(x, y, t);
    }
}


void ClearZoneTiles(u32 player, u32 zone)
{
    sub_08060DD8_i(gDuelZonePositions[player][zone].x / 8, gDuelZonePositions[player][zone].y / 8);
}
/* Redraw the board tile block for one slot `kind` (0-4 / 5-9 / 10 zone rows, 12-15 special slots) of `player`. */
/* Private 0xD64-byte view of the per-player block (the unit's struct PlayerState is 0xDC4 bytes). */
struct PlayerState_08061004 {   /* 0xD64 bytes, at 0x020192E4 + player * 0xD64 */
    u8 pad0[3];
    u8 b3;                      /* +3 */
    u8 b4;                      /* +4 count of a904 */
    u8 b5;                      /* +5 */
    u8 b6;                      /* +6 count of aB84 */
    u8 pad7[0x904 - 7];
    u32 a904[0xA0];             /* +0x904 card words */
    u32 aB84[0x50];             /* +0xB84 card words */
    u8 aCC4[0xA0];              /* +0xCC4 2-byte entries */
};
extern struct PlayerState_08061004 gPS1004_020192E4[2];
#define PS04 gPS1004_020192E4

void DrawAreaTiles(u32 player, u32 kind, u32 idx)
{
    s32 x = gDuelZonePositions[player][kind].x / 8;
    s32 y = gDuelZonePositions[player][kind].y / 8;
    u16 t;
    u8 v;

    switch (kind) {
    case 0:
    case 5:
    case 10:
        DrawZoneTiles(player, kind + idx);
        break;
    case 12:
        v = PS04[1 & player].b5;
        if (v != 0) {
            if (v > 5)
                sub_08060E2C_i(x, y, 0x1230);
            else
                sub_08060E2C_i(x, y, 0x1070);
        } else {
            sub_08060DD8_i(x, y);
        }
        break;
    case 13:
        v = PS04[1 & player].b3;
        if (v != 0) {
            if (v > 5)
                sub_08060E2C_i(x, y, 0x1230);
            else
                sub_08060E2C_i(x, y, 0x1070);
        } else {
            sub_08060DD8_i(x, y);
        }
        break;
    case 14:
        if (PS04[1 & player].b4 != 0) {
            u32 *list;
            s32 o;

            /* FAKEMATCH: the byte-offset variable gives the ROM's "lsl; sub #4; add list" order;
             * list[count - 1] is distributed into (list + count*4) - 4 instead. */
            list = PS04[1 & player].a904;
            o = PS04[1 & player].b4 * 4 - 4;
            list = (u32 *)((u8 *)list + o);
            sub_08060E2C_i(x, y, (u16)(GetCardIconBgTile((*list << 20) >> 20) + 0x2000));
        } else {
            sub_08060DD8_i(x, y);
        }
        break;
    case 15:
        if (PS04[1 & player].b6 != 0) {
            u32 *list;
            s32 o;

            /* FAKEMATCH: same byte-offset form as case 14. */
            list = PS04[1 & player].aB84;
            o = PS04[1 & player].b6 * 4 - 4;
            list = (u32 *)((u8 *)list + o);
            t = GetCardIconBgTile((*list << 20) >> 20) + 0x2000;
            if (PS04[1 & player].aCC4[(PS04[1 & player].b6 - 1) * 2] == 2)
                sub_08060E2C_i(x, y, 0x1070);
            else
                sub_08060E2C_i(x, y, t);
        } else {
            sub_08060DD8_i(x, y);
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
            DrawAreaTiles(i, 0, j);
            DrawAreaTiles(i, 5, j);
        }
        DrawAreaTiles(i, 10, 0);
        DrawAreaTiles(i, 12, 0);
        DrawAreaTiles(i, 13, 0);
        DrawAreaTiles(i, 14, 0);
        DrawAreaTiles(i, 15, 0);
    }
}
/* Draw two animated 8x8 cursor sprites at the screen positions of two zones (a, b = player | zone << 8); the tile cycles with frameCounter / 8. */
void DrawLinkMarkerPair(u16 a, u16 b, u16 c)
{
    u8 p1, z1, p2, z2;
    u32 x1, y1, x2, y2;
    u16 t;

    p1 = a;
    z1 = a >> 8;
    x1 = GetAreaX(p1, 0, z1) + 8;
    y1 = GetAreaY(p1, 0, z1) + 8;
    p2 = b;
    z2 = b >> 8;
    x2 = GetAreaX(p2, 0, z2) + 8;
    y2 = GetAreaY(p2, 0, z2) + 8;
    x1 |= y1 << 16;
    t = gZoneMarkerAnimTiles[(gMain.frameCounter >> 3) & 7] + c + 0x5400;
    AddSprite(x1, 0x40, t);
    x2 |= y2 << 16;
    t = gZoneMarkerAnimTiles[(gMain.frameCounter >> 3) & 7] + c + 0x5400;
    AddSprite(x2, 0x40, t);
}
/* Private views of one zone (0x94 bytes) and of the per-player block (0xD64 bytes) for DrawZoneLinkMarkers. */
struct Zone12F4 {
    u32 card;               /* +0 card word, id in the low 12 bits */
    u8 unk4;
    u8 unk5;
    u8 flags6;              /* +6 bit 1 = face-up */
    u8 unk7;
    u8 pad8[2];
    u16 ids[0x20];          /* +0xA linked zone refs (player | zone << 8) */
    u8 types[0x40];         /* +0x4A link types, stride 2 */
    u16 count;              /* +0x8A link count */
    u8 pad8C[0x94 - 0x8C];
};
struct PS12F4 {
    u8 pad0[0xB];
    u32 lowB : 4;
    u32 mask : 5;           /* +0xB bit 4 .. +0xC bit 0: straddles the byte, read as two ldrb + shift/or */
    u32 restC : 7;
    u8 padD[0xD64 - 0xD];
} __attribute__((packed));
extern struct PS12F4 gPS12F4_020192E4[2];
extern u8 gZ8_12F4_0201930C[];
/* Byte-array form: in a dereference the offset is expanded as (1 & p), z * 0x94, then * 0xD64, as in the ROM. */
#define ZA12F4(p, z) ((struct Zone12F4 *)&gZ8_12F4_0201930C[(1 & (p)) * 0xD64 + (z) * 0x94])
#define PACK12F4(p, z) ((u16)((u8)(p) | ((u8)(z) << 8)))

/* Draw cursor pairs for the links of zone (player, zone): kind 5 first scans every face-up occupied zone for links
   pointing at (player, zone); kinds 0, 5 and 10 then draw the zone's own links (types 1/5/10 -> 0x20C, 2/7 -> 0x218)
   and, when bit `zone` of the player's mask is set, a 0x218 cursor from (player, 0xF). */
void DrawZoneLinkMarkers(u32 player, u32 zone, u32 kind)
{
    s32 p;
    s32 z;
    s32 e;
    u16 id;
    u8 type;

    switch ((s32)kind) {
    case 5:
        for (p = 0; p <= 1; p++) {
            for (z = 0; z <= 10; z++) {
                if ((*(u32 *)ZA12F4(p, z) << 20) != 0 && (ZA12F4(p, z)->flags6 & 2)) {
                    for (e = 0; e < ZA12F4(p, z)->count; e++) {
                        id = ZA12F4(p, z)->ids[e];
                        switch (ZA12F4(p, z)->types[e * 2]) {
                        case 1:
                        case 5:
                        case 10:
                            if (id == PACK12F4(player, zone))
                                DrawLinkMarkerPair(id, PACK12F4(p, z), 0x20C);
                            break;
                        case 2:
                        case 7:
                            if (id == PACK12F4(player, zone))
                                DrawLinkMarkerPair(id, PACK12F4(p, z), 0x218);
                            break;
                        }
                    }
                }
            }
        }
        /* fallthrough */
    case 0:
    case 10:
        for (e = 0; e < ZA12F4(player, zone)->count; e++) {
            id = ZA12F4(player, zone)->ids[e];
            type = ZA12F4(player, zone)->types[e * 2];
            if ((ZA12F4(player, zone)->flags6 & 2) || player == 0) {
                switch (type) {
                case 1:
                case 5:
                case 10:
                    DrawLinkMarkerPair(id, PACK12F4(player, zone), 0x20C);
                    break;
                case 2:
                case 7:
                    DrawLinkMarkerPair(id, PACK12F4(player, zone), 0x218);
                    break;
                }
            }
        }
        if ((gPS12F4_020192E4[1 & player].mask >> zone) & 1)
            DrawLinkMarkerPair((u8)player | 0xF00, (u8)player | ((u8)zone << 8), 0x218);
        break;
    }
}
/* Per-frame draw of the duel board markers: zone cursor effects, hand strip cursor sprites, then the optional callback. */
/* Per-player state block (0xD64 bytes) at gDuel + 4; only the +0x26 bitmask is used here. */
struct PS580 {
    u8 pad0[0x26];
    u16 u26;
    u8 pad28[0xD64 - 0x28];
};
#define G580 ((struct DuelGlobals *)&gDuel)
/* Per-frame board overlay: act on the cursor zone (row 0 occupied / row 5) via DrawZoneLinkMarkers, draw an animated cursor
   sprite on each monster zone not flagged in the player's +0x26 mask, then run the optional callback at +0x85C. */
void DrawFieldOverlay(void)
{
    u32 p;
    u32 z0;
    u32 zn;
    s32 i;
    u32 x;
    u32 y;
    u16 t;

    if ((*(u8 *)&gDuelScreen & 6) == 6) {
        p = gDuelScreen.w824;
        z0 = gDuelScreen.w828;
        zn = z0 + gDuelScreen.w82C;
        switch (z0) {
        case 0:
            /* Explicit (p * 0xD64 + z * 0x94 + base): the ZB() operand order multiplies p first. */
            if (*(u32 *)((1 & p) * 0xD64 + zn * 0x94 + (u32)gDuelZones) << 20 != 0)
                DrawZoneLinkMarkers(p, zn, 0);
            break;
        case 5:
            DrawZoneLinkMarkers(p, zn, 5);
            break;
        }
        if (G580->f1B12_2 == 3) {
            for (i = 0; i <= 4; i++) {
                if (CanMonsterAttack(G580->f1B12_1, i, 0) != 0) {
                    /* Base through the gDuel cast so CSE derives it as (sym + 4) and loop hoists it. */
                    if (((((struct PS580 *)((u8 *)&gDuel + 4))[1 & G580->f1B12_1].u26 >> i) & 1) == 0) {
                        x = GetAreaX(G580->f1B12_1, 0, i);
                        y = GetAreaY(G580->f1B12_1, 0, i);
                        x += 8;
                        y += 8;
                        y <<= 16; /* separate shift keeps the OR result in x's register */
                        x |= y;
                        t = gZoneMarkerAnimTiles[(gMain.frameCounter >> 3) & 7] + 0x5600;
                        AddSprite(x, 0x40, t);
                    }
                }
            }
        }
        if (gDuelScreen.cb85C != NULL)
            gDuelScreen.cb85C();
    }
}
/* Draw the small card sprites of each player's hand/deck strip (row 0xB), skipping the one under the cursor. */
struct HandRow616 {
    struct DuelCard c[80];
    u8 pad[0xD64 - 80 * 4];
};
extern struct HandRow616 gDuelHands[2];
struct PlayerHdr616 {           /* 0xD64 bytes at 0x020192E4 + player * 0xD64 */
    u8 pad0[2];
    u8 handCount;               /* +2 */
    u8 pad3[0xD64 - 3];
};
extern struct PlayerHdr616 gHandHdr616_020192E4[2];
void DrawHandCards(void)
{
    s32 pl;
    u16 sel;
    u32 y;
    u32 x;
    u32 id;
    u16 t;
    s32 n;
    s32 i;
    struct DuelCard *p;

    if ((*(u8 *)&gDuelScreen & 6) == 6) {
        for (pl = 0; pl <= 1; pl++) {
            /* Ternary (not if/else): sel then has 5 refs and loses r9 to the hoisted player offset. */
            sel = pl != 0 ? IsHandRevealed(pl) : 1;
            y = GetAreaY(pl, 0xB, 0);
            if (y + 0x20 <= 0xBF) {
                n = gHandHdr616_020192E4[1 & pl].handCount;
                for (i = 0; i < n; i++) {
                    /* Two statements so fold keeps (base + player offset) + i * 4. */
                    p = gDuelHands[1 & pl].c;
                    p += i;
                    x = GetHandCardX(pl, i, n);
                    id = p->id;
                    t = sel ? GetCardIconObjTile(id) + 0x1000 : 0x40;
                    if (id != 0) {
                        /* Through a cast pointer the flag stays gDuel + 0x1B2C (two literals);
                           a plain gDuel.f1B2C_0 folds into one 0x0201AE0C literal. */
                        if (!(((struct DuelGlobals *)&gDuel)->f1B2C_0 && gDuelScreen.w824 == pl && gDuelScreen.w828 == 0xB
                              && gDuelScreen.w82C == i))
                            AddSprite((y << 16) | x, 0x80, t + 0x400);
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
    u16 t = 0x324;

    if (gMain.frameCounter & 0x20)
        t += 0x20;
    if ((*(u8 *)&gDuelScreen & 6) == 6) {
        AddSprite(0x00400058, 0x40C0, t |= 0x6000);
    }
}
/* Plot one pixel of colour `color` at (x, y) into 8bpp OBJ tile data at 0x06010000 (tile rows of 64 bytes, 2 tiles per 8 pixels of x, starting two rows down). */
void PlotCardImagePixel(u32 x, u32 y, u32 color)
{
    /* FAKEMATCH: retain the ROM's tile-column/address register. */
    register u32 tx __asm__("r4") = (x << 13) >> 16;
    u32 ty = (y << 13) >> 16;
    u32 xl = (x & 7) << 16;
    u32 yl = y & 7;
    u32 *p;
    u32 w;
    u8 buf[4];
    u32 px;
    u8 *bp;

    tx = (u16)(tx << 1);
    tx = (tx + ((u16)(ty + 2) << 5)) << 5;
    p = (u32 *)(0x06010000 + tx);
    p = (u32 *)((u32)p + ((xl >> 18) << 2));
    p = (u32 *)((u32)p + (yl << 3));
    w = *p;
    buf[0] = w;
    buf[1] = (u16)w >> 8;
    bp = buf;
    w >>= 16;
    bp[2] = w;
    w >>= 8;
    bp[3] = w;
    px = 0x30000;
    px &= xl;
    px >>= 16;
    buf[px] = color;
    *p = buf[0] | (buf[1] << 8) | ((buf[2] | (buf[3] << 8)) << 16);
}
/* Blit an 8x8 4bpp tile (`src`, 16 halfwords, 2 per row) as pixels through PlotCardImagePixel at yx, colours offset by palBase; also loads the 16-colour palette `pal` at the matching OBJ palette slot. */
void DrawCardImageTile(u32 yx, u32 palArg, const u16 *srcArg, const void *pal)
{
    /* FAKEMATCH: preserve the ROM's source-pointer register before narrowing palette base. */
    register const u16 *src __asm__("r5") = srcArg;
    u16 palBase = palArg;
    u16 i;
    u16 j;
    u16 k;
    u16 w;
    u16 x0 = yx;
    u32 y0 = yx >> 16;

    if (src == 0 || pal == 0)
        return;
    MemCopy16((void *)(0x05000200 + ((palBase >> 4) << 5)), pal, 0x40);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 2; j++) {
            w = *src;
            for (k = 0, src++; k < 4; k++) {
                u32 c = (w >> (k * 4)) & 0xF;

                if (c != 0)
                    PlotCardImagePixel(x0 + k + j * 4, y0 + i, (u8)(c + palBase));
            }
        }
    }
}
/* Blit a 16x16 image made of four 8x8 tiles (0x20 bytes apart) at yx. */
void DrawCardImageIcon16(u32 yx, u16 palBase, const u16 *src, const void *pal)
{
    u16 x = yx;
    u32 y = yx >> 16;

    DrawCardImageTile(x | (y << 16), palBase, src, pal);
    src += 0x10;
    DrawCardImageTile((x + 8) | (y << 16), palBase, src, pal);
    src += 0x10;
    DrawCardImageTile(x | ((y + 8) << 16), palBase, src, pal);
    src += 0x10;
    DrawCardImageTile((x + 8) | ((y + 8) << 16), palBase, src, pal);
}
