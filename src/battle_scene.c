/*
 * battle_scene (0x0805E788-0x0805F96B): the card-flip battle scene, the info-bar text cells and the
 * card info panels (wiki/functions/battle-scene-c.md).
 *
 * BattleScene_Update runs the battle scene (duel_card_anim.c draws the two battling cards) one frame
 * at a time: the cards roll open, their values show, the damaged side shakes and flashes its damage,
 * the destroyed side darkens, and the scene fades out; DuelCmd_PlayBattleScene calls it until it
 * returns 1. Holding B (or the duel screen's fast flag) speeds every timer up.
 *
 * The rest of the unit is the duel screen's info bar: a buffer of 64 text cells (8x8 4bpp tiles at
 * gDuelTextTiles, uploaded to BG3 by DuelScreen_Update) that strings, numbers and icons are drawn
 * into, and the card info panels built on it: DuelCursor_GetCardId (the card under the field cursor)
 * and the DuelInfo_Draw* panels (name box, ATK/DEF/level, the turn counter of Cocoon of Evolution
 * and Swords of Revealing Light, and the declared type / attribute icons).
 */
#include "global.h"
#include "card_data.h"            /* gCardNames */
#include "constants/card_stats.h" /* enum CardType, CARD_STATS_*_MASK, CARD_STATS_*_SHIFT */
#include "constants/cards.h"      /* CARD_COCOON_OF_EVOLUTION, CARD_SWORDS_OF_REVEALING_LIGHT, CARD_DNA_SURGERY */
#include "constants/duel.h"       /* enum DuelArea */
#include "duel.h"                 /* gDuel, duel structs, struct ZoneCardStats, GetZoneCardStats */
#include "gba.h"                  /* B_BUTTON */
#include "main.h"                 /* struct Main gMain, heldKeys / newKeys */
#include "sound.h"                /* PlaySE */

/* ---- Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view") ---- */

/* The duel screen block (duel_screen.h) as this unit reads it: the dirty bits at +0x808 are set
 * with byte read-modify-writes by the text-cell code, so they stay in a u8 container (the canonical
 * header uses a u16 and would compile to halfword accesses). */
struct DuelScreenView {
    u8 fast:1;                      /* +0x000 bit 0: fast-forward card animations, as if B were held */
    u8 unk0_1:7;
    u8 unk1[0x808 - 0x1];
    u8 textTilesDirty:1;            /* +0x808 bit 0: upload the text cells this frame */
    u8 textMapReset:1;              /* +0x808 bit 1: after the upload, reset the cell map */
    u8 unk808_2:6;
    u8 unk809[0x824 - 0x809];
    s32 selPlayer;                  /* +0x824: player of the cursor selection */
    s32 selArea;                    /* +0x828: enum DuelArea of the selection */
    s32 selIndex;                   /* +0x82C: zone index in the row, or hand index */
};
extern struct DuelScreenView gDuelScreen;   /* 0x0201CFB0 */

/* The text cell buffer (gDuelScreen.textTiles in duel_screen.h), viewed by TextCellsLoadIcon: two
 * 0x400-byte tile sets, then the dirty byte (the same byte as gDuelScreen + 0x808). The second set
 * and the flag are addressed from the global itself, not from a local pointer, so that CSE rebuilds
 * them from the register holding the buffer base (`add r0, sl`). */
struct TextTilesView {
    u8 tilesA[0x400];
    u8 tilesB[0x400];
    u8 textTilesDirty:1;            /* +0x800 bit 0 */
    u8 textMapReset:1;              /* +0x800 bit 1 */
    u8 unk800_2:6;
};
extern u8 gDuelTextTiles[0x800];            /* 0x0201CFB8 */

/* gDuelZones (duel.h) read as raw bytes: the cursor and panel code walks the zones with 0x94-byte
 * strides and pinned registers, so the canonical struct DuelZonesPlayer view is not used here. */
extern u8 gDuelZoneBytes[] asm("gDuelZones");   /* 0x0201930C */

/* One duel field zone (0x94 bytes; struct DuelZone in duel.h): only byte +6 and the word at +0x90
 * are used here. */
struct UiZone {
    u8 unk0[6];
    u8 flag6_0:1;                   /* +0x06 bit 0 */
    u8 flag6_1:1;                   /* +0x06 bit 1: the zone shows its counter / declared icon */
    u8 counter6:4;                  /* +0x06 bits 2-5: turn counter shown as number sprites */
    u8 unk6_6:2;
    u8 unk7[0x90 - 0x7];
    u32 unk90;                      /* +0x90: declared type / attribute bits (DNA Surgery, card 1448) */
};

/* gBattle as the battle scene sees it (struct Battle in battle.h, which cannot be included here:
 * it pulls in duel.h, and its BattleScene_DrawValues / BattleScene_DrawDamage prototypes have
 * other parameter widths than the matched calls in this unit). Only the scene state at +0x154
 * (gBattle.scene) is touched. */
struct Battle {
    u8 unk0[0x15C];
    u8 state;                       /* +0x15C: enum BattleSceneState */
    u8 subState;                    /* +0x15D: fade frame (OPEN) or enum BattleResultStep (RESULT) */
    u8 unk15E;                      /* +0x15E: zeroed when the scene opens, never read */
    u8 timer;                       /* +0x15F: frame counter of the steps */
};
extern struct Battle gBattle;               /* 0x02018450 */

/* The info-bar map in gMain (main.h: bgMapBuffer[5]): TextCellsLoadIcon forms the entry addresses
 * from the gMain base at run time, as the ROM does. */
struct MainInfoBarMap {
    u8 unk0[0x2C1C];
    u16 map[0x400];                 /* +0x2C1C */
};
extern struct MainInfoBarMap gMainInfoBarMap asm("gMain");

/* Save data header: byte +4 picks the label set and the character width. */
struct SaveHdr {
    u8 unk0[4];
    u8 flags;                       /* +0x4: bits 0-6 == 0: Japanese labels; bit 7: 2-byte characters */
};
extern struct SaveHdr gSaveData;            /* 0x02011C20 */

/* gBattle.scene states (battle_scene.h). */
enum BattleSceneState {
    BATTLE_SCENE_SHOW = 0,          /* show BG2/BG3/OBJ, darken everything */
    BATTLE_SCENE_OPEN = 1,          /* cards roll open while the screen fades in (subState 0-15) */
    BATTLE_SCENE_END_OPEN = 2,      /* remove the HBlank effect */
    BATTLE_SCENE_RESULT = 3,        /* values, hit and destroy sub-steps (enum BattleResultStep) */
    BATTLE_SCENE_FADE_OUT = 4,
    BATTLE_SCENE_DONE = 5           /* BattleScene_Update returns 1 */
};

enum BattleResultStep {
    BATTLE_RESULT_START = 0,
    BATTLE_RESULT_SHOW_VALUES = 1,  /* show both values for 30 frames */
    BATTLE_RESULT_HIT = 2,          /* shake the damaged sides and flash the damage */
    BATTLE_RESULT_DESTROY = 3,      /* darken and hide the destroyed sides */
    BATTLE_RESULT_HOLD = 4          /* hold 60 frames or until B */
};

/* Per-side flags of BattleScene_Update / BattleScene_DrawValues (player 0 in bits 0-7, player 1
 * in bits 8-15). */
enum BattleSideFlags {
    BATTLE_SIDE_DEFENSE = 0x1,      /* show DEF instead of ATK */
    BATTLE_SIDE_DESTROYED = 0x2,
    BATTLE_SIDE_DAMAGE = 0x4        /* this side takes life-point damage */
};

/* B held or the fast flag: every scene timer runs fast. */
#define BATTLE_SCENE_FAST ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast)
/* ---- Card table reads (card_data.h) ---- */

/* Card ID (bits 0-11) of a struct DuelCard word. */
#define CARD_ID(word) (((word) << 20) >> 20)
/* ROM card stats table through an integer-constant pointer (the ROM reloads the address at every
 * use; reading through gCardStats would compile differently). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) ((CARD_STATS(id) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)
/* Card number of a card ID (gCardIdToNumber, read through its address like the stats table). */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
/* Printed ATK of a card: 0 for Trap/Magic/Ticket, 4000 for a Divine card, else the stats field. */
static inline u16 CardAtk(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    }
    return ((CARD_STATS(id) << 14) >> 23) * 10;
}
static inline u16 CardDef(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    }
    return (CARD_STATS(id) & 0x1FF) * 10;
}
/* Monster level: Trap/Magic/Ticket count as 0, a Divine card as 10. */
static inline u16 CardLevel(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    }
    return (CARD_STATS(id) & 0x1E000000) >> 25;
}
/* ---- ROM data used only here ---- */
extern const u8 gMonsterInfoAtkLabelJp[];
extern const u8 gMonsterInfoDefLabelJp[];
extern const u8 gMonsterInfoAtkLabel[];
extern const u8 gMonsterInfoDefLabel[];
extern u16 *const gCardTypeIcons[];
extern u16 *const gCardAttributeIcons[];
/* Per-side pointers to the BG offset words shaken during the scene. */
extern s32 *const gBattleSceneShakeRegs[];
extern void (*IntrTable[])(void);

void DuelInfo_DrawCard(u16 cardId, u16 showStats);
void TextCellsLoadIcon(int cell, u16 palSlot, u16 *iconPack);
extern const u8 gTurnCounterLabel[];
extern const u8 gInfoAtkLabelJp[];
extern const u8 gInfoDefLabelJp[];
extern const u8 gInfoAtkLabel[];
extern const u8 gInfoDefLabel[];
/* Helpers from other units (memory copies, glyph rendering, the text canvas). */
void CopyDoubleWords(void *dst, const void *src, u32 size);
void MemCopy16(void *dst, const void *src, u32 size);
void RenderBoldGlyphTile(u32 ch, void *dst, u32 pal, u32 bpp);
void RenderSjisGlyphTile(u32 ch, void *dst, u32 pal, u32 bpp);
int StrLenWide(const u8 *s);
void TextCanvasInit(u32 w, u32 h);
void TextDrawString(int x, int y, u16 attr, const u8 *str);
void TextDrawNumber(int x, int y, u16 attr, int val);
void TextCanvasToTiles(void *dst, u16 value);
void TextCellsPutString(int firstCell, const u8 *str, u32 colour);
void TextCellsPutNumber(int firstCell, int value, u32 colour, int digits);
void TextCellsCopyBgTile(int cell, u16 tile);
/* Battle scene helpers (duel_card_anim.c, duel_field_screen.c). The parameter widths are the
 * matched call-site views: battle_scene.h declares BattleScene_DrawValues(u16, u32 *, u16) and
 * BattleScene_DrawDamage(int, int, u16) (build/readability/HEADERS.md). */
void ClearBlend(void);
void BattleScene_ResetBgAffine(void);
void BattleScene_DrawValues(u16 flags, s32 *values, int hideDestroyed);
void BattleScene_DrawDamage(int side, int damage, int flash);
s32 Random(void);
u16 FadeToBlack(int speed);
/* One frame of the battle scene (fade, shake and blend); returns 1 when finished (BATTLE_SCENE_DONE).
 * Holding B (or the fast flag) speeds up every timer. */
u32 BattleScene_Update(u16 value0, u16 value1, u16 flags)
{
    s32 pos[2];
    int k;
    u8 v;
    u32 t3;

    pos[0] = value0;
    pos[1] = value1;
    switch (gBattle.state) {
    case BATTLE_SCENE_SHOW:
        *(vu16 *)0x04000000 |= 0x1C00;
        *(vu16 *)0x04000050 = 0x3FFF;
        gBattle.state++;
    case BATTLE_SCENE_OPEN:
        *(vu16 *)0x04000054 = 0xF - gBattle.subState;
        v = gBattle.subState;
        if (v <= 0xE) {
            gBattle.subState++;
            if (BATTLE_SCENE_FAST && gBattle.subState <= 0xB)
                gBattle.subState += 3;
            return 0;
        }
        gBattle.state++;
        return 0;
    case BATTLE_SCENE_END_OPEN:
        ClearBlend();
        *(vu16 *)0x04000208 = 0;
        *(vu16 *)0x04000200 &= ~2;
        *(vu16 *)0x04000208 = 1;
        *(vu16 *)0x04000208 = 0;
        *(vu16 *)0x04000200 &= ~2;
        IntrTable[1] = 0;
        *(vu16 *)0x04000208 = 1;
        BattleScene_ResetBgAffine();
        gBattle.subState = 0;
        gBattle.unk15E = 0;
        gBattle.timer = 0;
        gBattle.state++;
    case BATTLE_SCENE_RESULT:
        switch (gBattle.subState) {
        case BATTLE_RESULT_START:
            if (flags == 0) {
                gBattle.state++;
                return 0;
            }
            gBattle.subState++;
        case BATTLE_RESULT_SHOW_VALUES:
            if (gBattle.timer <= 0x1D) {
                BattleScene_DrawValues(flags, pos, 0);
                gBattle.timer++;
                if (BATTLE_SCENE_FAST && gBattle.timer <= 0x15)
                    gBattle.timer += 7;
                return 0;
            }
            PlaySE(9); /* SE 9, unnamed in enum SoundEffect */
            gBattle.timer = 0;
            gBattle.subState++;
        case BATTLE_RESULT_HIT:
            BattleScene_DrawValues(flags, pos, 1);
            if (gBattle.timer <= 0x1D) {
                for (k = 0; k <= 1; k++) {
                    if (((BATTLE_SIDE_DESTROYED | BATTLE_SIDE_DAMAGE) << (k * 8)) & flags) {
                        *gBattleSceneShakeRegs[k * 2] = ((Random() % 16) - 8) << 8;
                        *gBattleSceneShakeRegs[k * 2 + 1] = ((Random() % 16) - 8) << 8;
                    }
                    if ((BATTLE_SIDE_DAMAGE << (k * 8)) & flags)
                        BattleScene_DrawDamage(k, pos[1 - k] - pos[k], 1);
                }
                gBattle.timer++;
                if (BATTLE_SCENE_FAST && gBattle.timer <= 0x15)
                    gBattle.timer += 7;
                return 0;
            }
            for (k = 0; k <= 1; k++) {
                if (((BATTLE_SIDE_DESTROYED | BATTLE_SIDE_DAMAGE) << (k * 8)) & flags) {
                    *gBattleSceneShakeRegs[k * 2] = 0;
                    *gBattleSceneShakeRegs[k * 2 + 1] = 0;
                }
            }
            gBattle.timer = 0;
            gBattle.subState++;
        case BATTLE_RESULT_DESTROY:
            BattleScene_DrawValues(flags, pos, 1);
            t3 = gBattle.timer;
            if (t3 <= 0x1F) {
                *(vu16 *)0x04000050 = 0xC0;
                if (flags & BATTLE_SIDE_DESTROYED)
                    *(vu16 *)0x04000050 |= 0x404;
                if (flags & (BATTLE_SIDE_DESTROYED << 8))
                    *(vu16 *)0x04000050 |= 0x808;
                *(vu16 *)0x04000054 = gBattle.timer;
                gBattle.timer = t3 + 1;
                if (BATTLE_SCENE_FAST && gBattle.timer <= 0x15)
                    gBattle.timer += 7;
                if (gBattle.timer == 0x20) {
                    if (flags & BATTLE_SIDE_DESTROYED)
                        *(vu16 *)0x04000000 &= 0xFBFF;
                    if (flags & (BATTLE_SIDE_DESTROYED << 8))
                        *(vu16 *)0x04000000 &= 0xF7FF;
                }
                for (k = 0; k <= 1; k++) {
                    if ((BATTLE_SIDE_DAMAGE << (k * 8)) & flags)
                        BattleScene_DrawDamage(k, pos[1 - k] - pos[k], 0);
                }
                return 0;
            }
            *(vu16 *)0x04000050 = 0;
            *(vu16 *)0x04000054 = 0;
            gBattle.timer = 0;
            gBattle.subState++;
        case BATTLE_RESULT_HOLD:
            BattleScene_DrawValues(flags, pos, 1);
            for (k = 0; k <= 1; k++) {
                if ((BATTLE_SIDE_DAMAGE << (k * 8)) & flags)
                    BattleScene_DrawDamage(k, pos[1 - k] - pos[k], 0);
            }
            if (gMain.newKeys & 2) {
                gBattle.state++;
                return 0;
            } else {
                v = gBattle.timer;
                if (v <= 0x3B) {
                    gBattle.timer++;
                    if (BATTLE_SCENE_FAST && gBattle.timer <= 0x33)
                        gBattle.timer += 7;
                } else {
                    gBattle.state++;
                }
            }
            break;
        }
        return 0;
    case BATTLE_SCENE_FADE_OUT:
        BattleScene_DrawValues(flags, pos, 1);
        for (k = 0; k <= 1; k++) {
            if ((BATTLE_SIDE_DAMAGE << (k * 8)) & flags)
                BattleScene_DrawDamage(k, pos[1 - k] - pos[k], 0);
        }
        if (FadeToBlack(BATTLE_SCENE_FAST ? 4 : 1))
            gBattle.state++;
        return 0;
    default:
        return 1;
    }
}
/* Card id (12 bits) of the card the duel screen cursor points at: zone `area + index` of `player`
 * for the monster, spell/trap and field areas, hand slot `index` for the hand, else 0. */
u32 DuelCursor_GetCardId(void)
{
    struct DuelScreenView *sc = &gDuelScreen;
    int player = sc->selPlayer;
    int area = sc->selArea;
    int index = sc->selIndex;
    int playerOff = (player & 1) * 0xD64;
    u8 *zones = gDuelZoneBytes;
    /* FAKEMATCH: pinning the base and the byte offset to r2/r0 makes agbcc emit the ROM's
     * `adds r0, r2, r0` operand order for the final zone-address add. */
    register u8 *base asm("r2") = playerOff + zones;
    register u32 zoneOff asm("r0") = (area + index) * 0x94;
    u32 id;

    id = CARD_ID(*(u32 *)((u32)base + zoneOff));

    switch (area) {
    case DUEL_AREA_MONSTER:
    case DUEL_AREA_SPELL_TRAP:
    case DUEL_AREA_FIELD:
        return id;
    case DUEL_AREA_HAND: {
        u8 *hand = zones + 11 * 0x94;
        u8 *handPtr = playerOff + hand;
        int handOff = index * 4;
        id = CARD_ID(*(u32 *)(handPtr + handOff));
        return id;
    }
    }
    return 0;
}
/* Fills 64 halfwords with consecutive values 0x28E..0x2CD. */
void TextCellsResetMap(void)
{
    u8 *b = (u8 *)&gMain;
    int i = 0x3F;
    int v = 0x2CD;
    u16 *p = (u16 *)(b + 0x311A);

    for (; i >= 0; i--) {
        *p = v;
        v--;
        p--;
    }
}
/* Clears all 64 cells of the text buffer and marks the duel screen dirty. */
void TextCellsClear(void)
{
    int i;
    u8 *cell = gDuelTextTiles;

    for (i = 0x3F; i >= 0; i--) {
        RenderBoldGlyphTile(0x20, cell, 0, 9);
        cell += 0x20;
    }
    gDuelScreen.textTilesDirty = 1;
    gDuelScreen.textMapReset = 1;
    TextCellsResetMap();
}
/* Draws a halfword-character string (byte-swapped chars) into the buffer from cell `first`. */
void TextCellsPutSjisString(int first, u16 *str, u32 pal)
{
    while (*(u8 *)str != 0) {
        u16 c = *str;
        u32 hi = c >> 8;
        u32 lo = (u8)c << 8;
        RenderSjisGlyphTile(lo | hi, gDuelTextTiles + first++ * 0x20, pal, 9);
        str++;
    }
    gDuelScreen.textTilesDirty = 1;
    gDuelScreen.textMapReset = 1;
}
/* Draws a byte-character string into the buffer from cell `first`. */
void TextCellsPutString(int first, const u8 *str, u32 pal)
{
    while (*str != 0) {
        RenderBoldGlyphTile(*str, gDuelTextTiles + first++ * 0x20, pal, 9);
        str++;
    }
    gDuelScreen.textTilesDirty = 1;
    gDuelScreen.textMapReset = 1;
}
/* Draws `val` as up to `digits` decimal digits right-aligned at cell `first + digits - 1`. */
void TextCellsPutNumber(int first, int val, u32 pal, int digits)
{
    first += digits;
    while (digits > 0) {
        RenderBoldGlyphTile(val % 10 + 0x30, gDuelTextTiles + --first * 0x20, pal, 9);
        val /= 10;
        if (val == 0)
            return;
        digits--;
    }
    gDuelScreen.textTilesDirty = 1;
    gDuelScreen.textMapReset = 1;
}
/* Copies BG tile `tile` of char block 1 (0x06004000) into text cell `cell`. */
void TextCellsCopyBgTile(int cell, u16 tile)
{
    MemCopy16(gDuelTextTiles + cell * 0x20, (void *)(0x06004000 + tile * 0x20), 0x20);
}
/* Loads an image-pack icon `iconPack` (u16 count of palette halfwords at +0, palette at +8, then
 * a tile count and two tile sets) into the text buffer cell `cell`, palette slot `palSlot`, and
 * patches the four BG map entries of that cell to palette `palSlot & 15`. */
void TextCellsLoadIcon(int cell, u16 palSlot, u16 *iconPack)
{
    int off = iconPack[0] * 2;
    u16 *cnt = (u16 *)((u8 *)iconPack + (off + 8));
    u8 *src = (u8 *)iconPack + (off + 0x10);
    int cellOff = cell << 5;
    u8 *buf = gDuelTextTiles;
    u16 n = *cnt;

    if (iconPack != 0) {
        u16 pal;

        MemCopy16(buf + cellOff, src, n * 16);
        MemCopy16(&((struct TextTilesView *)gDuelTextTiles)->tilesB[cellOff], src + *cnt * 16, *cnt * 16);
        CopyDoubleWords((void *)(0x05000000 + palSlot * 32), iconPack + 4, iconPack[0] * 2);
        pal = palSlot << 12;
        /* BG map entries of the 2x2 block at rows 18-19, column cell: palette nibble := palSlot. */
        gMainInfoBarMap.map[(u16)cell + 0x240] &= 0xFFF;
        gMainInfoBarMap.map[(u16)(cell + 1) + 0x240] &= 0xFFF;
        gMainInfoBarMap.map[(u16)cell + 0x260] &= 0xFFF;
        gMainInfoBarMap.map[(u16)(cell + 1) + 0x260] &= 0xFFF;
        gMainInfoBarMap.map[(u16)cell + 0x240] |= pal;
        gMainInfoBarMap.map[(u16)(cell + 1) + 0x240] |= pal;
        gMainInfoBarMap.map[(u16)cell + 0x260] |= pal;
        gMainInfoBarMap.map[(u16)(cell + 1) + 0x260] |= pal;
        ((struct TextTilesView *)gDuelTextTiles)->textMapReset = 0;
    }
}
/* Draws a centred 2-row message box: the string `n` of the table at 0x0822C720 (64 bytes each). */
void DuelInfo_DrawCardNameCentered(u16 cardId)
{
    const u8 *str = gCardNames + (cardId << 6);
    int len = StrLenWide(str);
    u32 w = 12;
    int x;

    if (len > 0xF)
        w = 10;
    TextCanvasInit(0x20, 2);
    x = (int)(len * w) >> 1;
    TextDrawString(0x78 - x, 9 - (w >> 1), (w << 8) | 8, str);
    TextDrawString(0x77 - x, 8 - (w >> 1), (w << 8) | 7, str);
    TextCanvasToTiles(gDuelTextTiles, 9);
}
/* Card info header: name box (string `cardId`), then for monster cards with `showStats` the
 * ATK / DEF / level numbers and labels. */
void DuelInfo_DrawCard(u16 cardId, u16 showStats)
{
    /* FAKEMATCH: pinning the byte offset to r0 makes agbcc emit the ROM's lsls-before-ldr order
     * (harmless: it only fixes register/schedule allocation of the name-string address). */
    register u32 off asm("r0") = cardId << 6;
    const u8 *tbl = gCardNames;
    const u8 *str = tbl + off;
    int len = StrLenWide(str);
    u32 w = 12;
    int v;

    if (len > 0xC)
        w = 10;
    TextCanvasInit(0x20, 2);
    TextDrawString(3, 9 - (w >> 1), (w << 8) | 8, str);
    TextDrawString(2, 8 - (w >> 1), (w << 8) | 7, str);
    TextCanvasToTiles(gDuelTextTiles, 9);
    if (CARD_TYPE(cardId) <= CARD_TYPE_REPTILE && showStats != 0) {
        if ((gSaveData.flags & 0x7F) == 0) {
            TextCellsPutString(0x17, gInfoAtkLabelJp, 5);
            TextCellsPutString(0x37, gInfoDefLabelJp, 4);
        } else {
            TextCellsPutString(0x17, gInfoAtkLabel, 5);
            TextCellsPutString(0x37, gInfoDefLabel, 4);
        }
        v = CardAtk(cardId);
        TextCellsPutNumber(0x1A, v, 7, 4);
        v = CardDef(cardId);
        TextCellsPutNumber(0x3A, v, 7, 4);
        TextCellsCopyBgTile(0x18, 3);
        v = CardLevel(cardId);
        TextCellsPutNumber(0x37, v, 7, 2);
    }
}
/* Zone of a player as this unit reads it: a fresh address computation per call (the ROM uses
 * separate register temporaries per use, which a shared local would not reproduce). */
static inline struct UiZone *ZonePtr(int player, int zone)
{
    int pl = player & 1;
    return (struct UiZone *)(zone * 0x94 + pl * 0xD64 + (u32)gDuelZoneBytes);
}
/* Duel card detail (zone view): name box, a counter box for Cocoon of Evolution, Swords of
 * Revealing Light and card 1230, and the ATK/DEF/level numbers; DNA Surgery and card 1448 also
 * show their declared type / attribute icon. */
void DuelInfo_DrawSpellZone(u16 cardId, u16 showStats, int player, int zone)
{
    /* FAKEMATCH: offset pinned to r0 as in DuelInfo_DrawCard */
    register u32 off asm("r0") = cardId << 6;
    const u8 *tbl = gCardNames;
    const u8 *str = tbl + off;
    int len = StrLenWide(str);
    u32 w = 12;
    int x = 0xEC;
    int shown = 0;
    struct UiZone *z;
    int v;

    if (len > 0xF)
        w = 10;
    TextCanvasInit(0x20, 2);
    TextDrawString(3, 9 - (w >> 1), (w << 8) | 8, str);
    TextDrawString(2, 8 - (w >> 1), (w << 8) | 7, str);
    if (ZonePtr(player, zone)->flag6_1) {
        int cardNumber = CARD_NUMBER(cardId);
        switch (cardNumber) {
        case CARD_COCOON_OF_EVOLUTION:
        case CARD_SWORDS_OF_REVEALING_LIGHT:
        case CARD_1230: {
            u32 t;
            x -= 0x3C;
            t = ZonePtr(player, zone)->counter6;
            if ((int)t <= 9) {
                TextDrawNumber(x + 1, 4, 0xA08, t);
                TextDrawNumber(x, 3, 0xA07, t);
                TextDrawString(x + 0xB, 4, 0xA08, gTurnCounterLabel);
                TextDrawString(x + 0xA, 3, 0xA04, gTurnCounterLabel);
            } else {
                x -= 0xA;
                TextDrawNumber(x + 0xB, 4, 0xA08, t);
                TextDrawNumber(x + 0xA, 3, 0xA07, t);
                TextDrawString(x + 0x15, 4, 0xA08, gTurnCounterLabel);
                TextDrawString(x + 0x14, 3, 0xA04, gTurnCounterLabel);
            }
            shown = 1;
            break;
        }
        }
    }
    TextCanvasToTiles(gDuelTextTiles, 9);
    if (CARD_TYPE(cardId) <= CARD_TYPE_REPTILE && showStats != 0 && shown == 0) {
        if ((gSaveData.flags & 0x7F) == 0) {
            TextCellsPutString(0x17, gInfoAtkLabelJp, 5);
            TextCellsPutString(0x37, gInfoDefLabelJp, 4);
        } else {
            TextCellsPutString(0x17, gInfoAtkLabel, 5);
            TextCellsPutString(0x37, gInfoDefLabel, 4);
        }
        v = CardAtk(cardId);
        TextCellsPutNumber(0x1A, v, 7, 4);
        v = CardDef(cardId);
        TextCellsPutNumber(0x3A, v, 7, 4);
        TextCellsCopyBgTile(0x18, 3);
        v = CardLevel(cardId);
        TextCellsPutNumber(0x37, v, 7, 2);
    }
    z = ZonePtr(player, zone);
    if (z->flag6_1) {
        int cardNumber = CARD_NUMBER(cardId);
        switch (cardNumber) {
        case CARD_DNA_SURGERY:
            TextCellsLoadIcon(0x1C, 9, gCardTypeIcons[(z->unk90 << 14) >> 27]);
            break;
        case CARD_1448:
            TextCellsLoadIcon(0x1C, 9, gCardAttributeIcons[(z->unk90 << 14) >> 27]);
            break;
        }
    }
}
/* Draws a two-row text box frame (rows of string `label`, palette/size byte `glyphWidth`) and the
 * number `value` centred: `labelLen` is the extra width in characters, each digit of `value` adds
 * 1 (2 with 2-byte chars). */
void DuelInfo_DrawLabelNumber(int glyphWidth, const u8 *label, int value, int labelLen)
{
    TextCanvasInit(0x20, 2);
    TextDrawString(3, 9 - glyphWidth / 2, ((u8)glyphWidth << 8) | 0xD, label);
    TextDrawString(2, 8 - glyphWidth / 2, ((u8)glyphWidth << 8) | 5, label);
    if (labelLen > 0) {
        int rest = value;
        int x;

        if (rest > 9) {
            u8 doubleWidth = gSaveData.flags & 0x80;
            do {
                labelLen++;
                if (doubleWidth)
                    labelLen++;
                rest /= 10;
            } while (rest > 9);
        }
        x = glyphWidth * labelLen / 2;
        TextDrawNumber(x + 3, 9 - glyphWidth / 2, ((u8)glyphWidth << 8) | 8, value);
        x += 2;
        TextDrawNumber(x, 8 - glyphWidth / 2, ((u8)glyphWidth << 8) | 7, value);
    }
    TextCanvasToTiles(gDuelTextTiles, 9);
}
/* Card detail panel for the card in (player, zone): name, labels, ATK/DEF (palette 7 when equal
 * to the printed value, else 6), level and the type / attribute icons. */
void DuelInfo_DrawMonsterZone(int player, int zone)
{
    struct ZoneCardStats info;
    int atk;
    int def;
    int x;
    u32 pal;

    GetZoneCardStats(player, zone, &info);
    DuelInfo_DrawCard(info.id, 0);
    if ((gSaveData.flags & 0x7F) == 0) {
        TextCellsPutString(0x18, gMonsterInfoAtkLabelJp, 5);
        TextCellsPutString(0x38, gMonsterInfoDefLabelJp, 4);
    } else {
        TextCellsPutString(0x18, gMonsterInfoAtkLabel, 5);
        TextCellsPutString(0x38, gMonsterInfoDefLabel, 4);
    }
    atk = info.atk;
    pal = CardAtk(info.id) != info.atk ? 6 : 7;
    TextCellsPutNumber(0x19, atk, pal, 5);
    def = info.def;
    pal = CardDef(info.id) != info.def ? 6 : 7;
    TextCellsPutNumber(0x39, def, pal, 5);
    TextCellsCopyBgTile(0x17, 3);
    TextCellsPutNumber(0x36, CardLevel(info.id), 7, 2);
    x = 0x13;
    if (CardLevel(info.id) > 9)
        x--;
    if (info.type != 0 && info.type <= CARD_TYPE_REPTILE)
        TextCellsLoadIcon(x, 9, gCardTypeIcons[info.type]);
    if (info.attribute != 0 && info.attribute <= 6)
        TextCellsLoadIcon(x + 2, 10, gCardAttributeIcons[info.attribute]);
}
