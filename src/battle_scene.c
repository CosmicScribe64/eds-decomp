#include "global.h"

/* Duel screen block at 0x0201CFB0 (see code-08051a9c / code-0805d58c). */
struct DuelScreen {
    u8 pad0[0x808];
    u8 f808_0 : 1;
    u8 f808_1 : 1;
    u8 f808_rest : 6;
    u8 pad809[0x824 - 0x809];
    u32 player;     /* +0x824 */
    u32 mode;       /* +0x828 */
    u32 index;      /* +0x82C */
};
extern struct DuelScreen gDuelScreen;
extern u8 gDuelZones[];
struct ZoneW {
    u32 w;
    u8 pad[0x90];
};
#define CARD_ID(w) (((w) << 20) >> 20)
/* ROM card stats table through an integer-constant pointer (the ROM reloads the address at every use). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* AI/UI value of a card: 0 for Magic/Trap/Ritual (types 0x15-0x17), 4000 for type 0x18, else field * 10. */
static inline u16 CardAtk(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    }
    return ((CARD_STATS(id) << 14) >> 23) * 10;
}
static inline u16 CardDef(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    }
    return (CARD_STATS(id) & 0x1FF) * 10;
}
/* Monster level: Magic/Trap types count as 0, type 0x18 as 10. */
static inline u16 CardLevel(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    }
    return (CARD_STATS(id) & 0x1E000000) >> 25;
}
/* Card info block filled by GetZoneCardStats (12 bytes). */
struct CardInfo {
    u16 id;         /* +0 */
    u8 attr : 5;    /* byte +2 bits 0-4 (monster attribute? index into gCardTypeIcons) */
    u8 kind : 3;    /* byte +2 bits 5-7 (index into gCardAttributeIcons) */
    u32 atk;
    u32 def;
};
void GetZoneCardStats(int player, int slot, struct CardInfo *out);
void DuelInfo_DrawCard(u16 id, u16 flag);
void TextCellsLoadIcon(int a, u16 b, u16 *hdr);
extern const u8 gMonsterInfoAtkLabelJp[];
extern const u8 gMonsterInfoDefLabelJp[];
extern const u8 gMonsterInfoAtkLabel[];
extern const u8 gMonsterInfoDefLabel[];
extern u16 *const gCardTypeIcons[];
extern u16 *const gCardAttributeIcons[];
/* One duel field zone (0x94 bytes); only byte +6 and the word at +0x90 are used here. */
struct UiZone {
    u8 pad0[6];
    u8 f6_0 : 1;
    u8 f6_1 : 1;        /* bit 1: "face-down" / evaluation flag (hypothesis) */
    u8 f6_2 : 4;        /* bits 2-5: counter shown as number sprites */
    u8 f6_6 : 2;
    u8 pad7[0x90 - 7];
    u32 w90;
};
extern u8 gDuelZones[];
extern const u8 gTurnCounterLabel[];
extern const u8 gInfoAtkLabelJp[];
extern const u8 gInfoDefLabelJp[];
extern const u8 gInfoAtkLabel[];
extern const u8 gInfoDefLabel[];
/* gMain (0x03000040): 64 halfwords at +0x309C (used by TextCellsResetMap). */
struct MainTbl {
    u8 pad[0x309C];
    u16 tbl[0x40];
};
extern struct MainTbl gMain;
struct MainMap {
    u8 pad[0x2C1C];
    u16 map[0x400];
};
extern struct MainMap gUnk_03000040_m asm("gMain");
void CopyDoubleWords(void *dst, const void *src, u32 size);
/* Text/tile buffer of 64 cells of 0x20 bytes (8x8 4bpp tiles) at 0x0201CFB8. */
extern u8 gDuelTextTiles[];
void RenderBoldGlyphTile(u32 ch, void *dst, u32 pal, u32 bpp);
void RenderSjisGlyphTile(u32 ch, void *dst, u32 pal, u32 bpp);
void MemCopy16(void *dst, const void *src, u32 size);
extern const u8 gCardNames[];
int StrLenWide(const u8 *s);
void TextCanvasInit(u32 w, u32 h);
void TextDrawString(int x, int y, u16 attr, const u8 *str);
void TextCanvasToTiles(void *dst, u16 value);
void TextCellsPutString(int first, const u8 *str, u32 pal);
void TextCellsPutNumber(int first, int val, u32 pal, int digits);
void TextCellsCopyBgTile(int a, u16 b);
void TextDrawNumber(int x, int y, u16 attr, int val);
/* Save-mirror header at 0x02011C20: bit 7 of byte +4 selects double-width (2-byte) characters. */
struct SaveHdr {
    u8 pad[4];
    u8 flags;
};
extern struct SaveHdr gSaveData;

/* Transition state at 0x02018450 (+0x15C step, +0x15D sub-step, +0x15F timer). */
struct ScnE788 { u8 pad[0x15C]; u8 step; u8 sub; u8 unk15E; u8 t; };
extern struct ScnE788 gBattle;
struct KeysE788 { u8 pad[4]; u16 held; u16 pressed; };
extern struct KeysE788 gKeysE788 asm("gMain");
/* Per-side pointers to the BG offset words shaken during the transition. */
extern s32 *const gBattleSceneShakeRegs[];
extern void (*IntrTable[])(void);
void ClearBlend(void);
void BattleScene_ResetBgAffine(void);
void BattleScene_DrawValues(u16 flags, s32 *pos, int mode);
void BattleScene_DrawDamage(int side, int delta, int mode);
s32 Random(void);
u16 FadeToBlack(int speed);
void PlaySE(int se);
#define E788_FAST ((gKeysE788.held & 2) || (((u8 *)&gDuelScreen)[0] & 1))
/* Screen transition (fade, shake and blend) run once per frame; returns 1 when finished (hypothesis).
 * Holding B (or the fast flag) speeds up every timer. */
u32 BattleScene_Update(u16 a, u16 b, u16 flags)
{
    s32 pos[2];
    int k;
    u8 v;
    u32 t3;

    pos[0] = a;
    pos[1] = b;
    switch (gBattle.step) {
    case 0:
        *(vu16 *)0x04000000 |= 0x1C00;
        *(vu16 *)0x04000050 = 0x3FFF;
        gBattle.step++;
    case 1:
        *(vu16 *)0x04000054 = 0xF - gBattle.sub;
        v = gBattle.sub;
        if (v <= 0xE) {
            gBattle.sub++;
            if (E788_FAST && gBattle.sub <= 0xB)
                gBattle.sub += 3;
            return 0;
        }
        gBattle.step++;
        return 0;
    case 2:
        ClearBlend();
        *(vu16 *)0x04000208 = 0;
        *(vu16 *)0x04000200 &= ~2;
        *(vu16 *)0x04000208 = 1;
        *(vu16 *)0x04000208 = 0;
        *(vu16 *)0x04000200 &= ~2;
        IntrTable[1] = 0;
        *(vu16 *)0x04000208 = 1;
        BattleScene_ResetBgAffine();
        gBattle.sub = 0;
        gBattle.unk15E = 0;
        gBattle.t = 0;
        gBattle.step++;
    case 3:
        switch (gBattle.sub) {
        case 0:
            if (flags == 0) {
                gBattle.step++;
                return 0;
            }
            gBattle.sub++;
        case 1:
            if (gBattle.t <= 0x1D) {
                BattleScene_DrawValues(flags, pos, 0);
                gBattle.t++;
                if (E788_FAST && gBattle.t <= 0x15)
                    gBattle.t += 7;
                return 0;
            }
            PlaySE(9);
            gBattle.t = 0;
            gBattle.sub++;
        case 2:
            BattleScene_DrawValues(flags, pos, 1);
            if (gBattle.t <= 0x1D) {
                for (k = 0; k <= 1; k++) {
                    if ((6 << (k * 8)) & flags) {
                        *gBattleSceneShakeRegs[k * 2] = ((Random() % 16) - 8) << 8;
                        *gBattleSceneShakeRegs[k * 2 + 1] = ((Random() % 16) - 8) << 8;
                    }
                    if ((4 << (k * 8)) & flags)
                        BattleScene_DrawDamage(k, pos[1 - k] - pos[k], 1);
                }
                gBattle.t++;
                if (E788_FAST && gBattle.t <= 0x15)
                    gBattle.t += 7;
                return 0;
            }
            for (k = 0; k <= 1; k++) {
                if ((6 << (k * 8)) & flags) {
                    *gBattleSceneShakeRegs[k * 2] = 0;
                    *gBattleSceneShakeRegs[k * 2 + 1] = 0;
                }
            }
            gBattle.t = 0;
            gBattle.sub++;
        case 3:
            BattleScene_DrawValues(flags, pos, 1);
            t3 = gBattle.t;
            if (t3 <= 0x1F) {
                *(vu16 *)0x04000050 = 0xC0;
                if (flags & 2)
                    *(vu16 *)0x04000050 |= 0x404;
                if (flags & 0x200)
                    *(vu16 *)0x04000050 |= 0x808;
                *(vu16 *)0x04000054 = gBattle.t;
                gBattle.t = t3 + 1;
                if (E788_FAST && gBattle.t <= 0x15)
                    gBattle.t += 7;
                if (gBattle.t == 0x20) {
                    if (flags & 2)
                        *(vu16 *)0x04000000 &= 0xFBFF;
                    if (flags & 0x200)
                        *(vu16 *)0x04000000 &= 0xF7FF;
                }
                for (k = 0; k <= 1; k++) {
                    if ((4 << (k * 8)) & flags)
                        BattleScene_DrawDamage(k, pos[1 - k] - pos[k], 0);
                }
                return 0;
            }
            *(vu16 *)0x04000050 = 0;
            *(vu16 *)0x04000054 = 0;
            gBattle.t = 0;
            gBattle.sub++;
        case 4:
            BattleScene_DrawValues(flags, pos, 1);
            for (k = 0; k <= 1; k++) {
                if ((4 << (k * 8)) & flags)
                    BattleScene_DrawDamage(k, pos[1 - k] - pos[k], 0);
            }
            if (gKeysE788.pressed & 2) {
                gBattle.step++;
                return 0;
            } else {
                v = gBattle.t;
                if (v <= 0x3B) {
                    gBattle.t++;
                    if (E788_FAST && gBattle.t <= 0x33)
                        gBattle.t += 7;
                } else {
                    gBattle.step++;
                }
            }
            break;
        }
        return 0;
    case 4:
        BattleScene_DrawValues(flags, pos, 1);
        for (k = 0; k <= 1; k++) {
            if ((4 << (k * 8)) & flags)
                BattleScene_DrawDamage(k, pos[1 - k] - pos[k], 0);
        }
        if (FadeToBlack(E788_FAST ? 4 : 1))
            gBattle.step++;
        return 0;
    default:
        return 1;
    }
}
/* Card id (12 bits) of the card the duel screen cursor points at: zone `mode + index` of `player`
 * for modes 0/5/10, hand slot `index` for mode 11, else 0. */
u32 DuelCursor_GetCardId(void)
{
    struct DuelScreen *sc = &gDuelScreen;
    int player = sc->player;
    int mode = sc->mode;
    int index = sc->index;
    int off = (player & 1) * 0xD64;
    u8 *z = gDuelZones;
    /* FAKEMATCH: pinning the base and the byte offset to r2/r0 makes agbcc emit the ROM's
     * `adds r0, r2, r0` operand order for the final zone-address add. */
    register u8 *bp asm("r2") = off + z;
    register u32 mi asm("r0") = (mode + index) * 0x94;
    u32 id;

    id = CARD_ID(*(u32 *)((u32)bp + mi));

    switch (mode) {
    case 0:
    case 5:
    case 10:
        return id;
    case 11: {
        u8 *h = z + 11 * 0x94;
        u8 *hp = off + h;
        int hoff = index * 4;
        id = CARD_ID(*(u32 *)(hp + hoff));
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
    gDuelScreen.f808_0 = 1;
    gDuelScreen.f808_1 = 1;
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
    gDuelScreen.f808_0 = 1;
    gDuelScreen.f808_1 = 1;
}
/* Draws a byte-character string into the buffer from cell `first`. */
void TextCellsPutString(int first, const u8 *str, u32 pal)
{
    while (*str != 0) {
        RenderBoldGlyphTile(*str, gDuelTextTiles + first++ * 0x20, pal, 9);
        str++;
    }
    gDuelScreen.f808_0 = 1;
    gDuelScreen.f808_1 = 1;
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
    gDuelScreen.f808_0 = 1;
    gDuelScreen.f808_1 = 1;
}
/* Copies one cell of the text buffer to tile `b` of the OBJ/BG tile area at 0x06004000. */
void TextCellsCopyBgTile(int a, u16 b)
{
    MemCopy16(gDuelTextTiles + a * 0x20, (void *)(0x06004000 + b * 0x20), 0x20);
}
/* Loads a palette+tile block `hdr` (u16 count of palette halfwords at +0, palette at +8, then a
 * tile count and two tile sets) into the text buffer cell `a`, palette slot `b` and patches the four
 * BG map entries of that cell to palette `b & 15`. */
/* The text/tile buffer at 0x0201CFB8: two 0x400-byte tile sets, then the dirty-flag byte (the same
 * byte as gDuelScreen.f808). The second set and the flag are addressed from the global itself, not
 * from `buf`, so that CSE rebuilds them from the register holding the buffer base (`add r0, sl`). */
struct TileBufEF00 {
    u8 a[0x400];
    u8 b[0x400];
    u8 f0 : 1;
    u8 f1 : 1;
    u8 rest : 6;
};
void TextCellsLoadIcon(int a, u16 b, u16 *hdr)
{
    int off = hdr[0] * 2;
    u16 *cnt = (u16 *)((u8 *)hdr + (off + 8));
    u8 *src = (u8 *)hdr + (off + 0x10);
    int cell = a << 5;
    u8 *buf = gDuelTextTiles;
    u16 n = *cnt;

    if (hdr != 0) {
        u16 pal;

        MemCopy16(buf + cell, src, n * 16);
        MemCopy16(&((struct TileBufEF00 *)gDuelTextTiles)->b[cell], src + *cnt * 16, *cnt * 16);
        CopyDoubleWords((void *)(0x05000000 + b * 32), hdr + 4, hdr[0] * 2);
        pal = b << 12;
        /* BG map entries of the 2x2 block at rows 18-19, column a: palette nibble := b. */
        gUnk_03000040_m.map[(u16)a + 0x240] &= 0xFFF;
        gUnk_03000040_m.map[(u16)(a + 1) + 0x240] &= 0xFFF;
        gUnk_03000040_m.map[(u16)a + 0x260] &= 0xFFF;
        gUnk_03000040_m.map[(u16)(a + 1) + 0x260] &= 0xFFF;
        gUnk_03000040_m.map[(u16)a + 0x240] |= pal;
        gUnk_03000040_m.map[(u16)(a + 1) + 0x240] |= pal;
        gUnk_03000040_m.map[(u16)a + 0x260] |= pal;
        gUnk_03000040_m.map[(u16)(a + 1) + 0x260] |= pal;
        ((struct TileBufEF00 *)gDuelTextTiles)->f1 = 0;
    }
}
/* Draws a centred 2-row message box: the string `n` of the table at 0x0822C720 (64 bytes each). */
void DuelInfo_DrawCardNameCentered(u16 n)
{
    const u8 *str = gCardNames + (n << 6);
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
/* Card info header: name box (string `id`), then for monster cards (type <= 0x14) with `flag` the
 * ATK / DEF / level numbers and labels. */
void DuelInfo_DrawCard(u16 id, u16 flag)
{
    /* FAKEMATCH: pinning the byte offset to r0 makes agbcc emit the ROM's lsls-before-ldr order
     * (harmless: it only fixes register/schedule allocation of the name-string address). */
    register u32 off asm("r0") = id << 6;
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
    if (CARD_TYPE(id) <= 0x14 && flag != 0) {
        if ((gSaveData.flags & 0x7F) == 0) {
            TextCellsPutString(0x17, gInfoAtkLabelJp, 5);
            TextCellsPutString(0x37, gInfoDefLabelJp, 4);
        } else {
            TextCellsPutString(0x17, gInfoAtkLabel, 5);
            TextCellsPutString(0x37, gInfoDefLabel, 4);
        }
        v = CardAtk(id);
        TextCellsPutNumber(0x1A, v, 7, 4);
        v = CardDef(id);
        TextCellsPutNumber(0x3A, v, 7, 4);
        TextCellsCopyBgTile(0x18, 3);
        v = CardLevel(id);
        TextCellsPutNumber(0x37, v, 7, 2);
    }
}
extern const u16 gCardIdToNumber[];
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
/* Duel card detail (zone view): name box, a counter box for the special cards 0x47/0x15B/0x4CE, and the
 * ATK/DEF/level numbers; cards 0x479/0x5A8 also show an icon block. */
static inline struct UiZone *ZoneF270(int player, int slot)
{
    int pl = player & 1;
    return (struct UiZone *)(slot * 0x94 + pl * 0xD64 + (u32)gDuelZones);
}
#define ZONE_F270(p, s) ZoneF270(p, s)
void DuelInfo_DrawSpellZone(u16 id, u16 flag, int player, int slot)
{
    /* FAKEMATCH: offset pinned to r0 as in DuelInfo_DrawCard */
    register u32 off asm("r0") = id << 6;
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
    if (ZONE_F270(player, slot)->f6_1) {
        int n = CARD_NUMBER(id);
        switch (n) {
        case 0x47:
        case 0x15B:
        case 0x4CE: {
            u32 t;
            x -= 0x3C;
            t = ZONE_F270(player, slot)->f6_2;
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
    if (CARD_TYPE(id) <= 0x14 && flag != 0 && shown == 0) {
        if ((gSaveData.flags & 0x7F) == 0) {
            TextCellsPutString(0x17, gInfoAtkLabelJp, 5);
            TextCellsPutString(0x37, gInfoDefLabelJp, 4);
        } else {
            TextCellsPutString(0x17, gInfoAtkLabel, 5);
            TextCellsPutString(0x37, gInfoDefLabel, 4);
        }
        v = CardAtk(id);
        TextCellsPutNumber(0x1A, v, 7, 4);
        v = CardDef(id);
        TextCellsPutNumber(0x3A, v, 7, 4);
        TextCellsCopyBgTile(0x18, 3);
        v = CardLevel(id);
        TextCellsPutNumber(0x37, v, 7, 2);
    }
    z = ZONE_F270(player, slot);
    if (z->f6_1) {
        int n = CARD_NUMBER(id);
        switch (n) {
        case 0x479:
            TextCellsLoadIcon(0x1C, 9, gCardTypeIcons[(z->w90 << 14) >> 27]);
            break;
        case 0x5A8:
            TextCellsLoadIcon(0x1C, 9, gCardAttributeIcons[(z->w90 << 14) >> 27]);
            break;
        }
    }
}
/* Draws a two-row text box frame (rows of string `str`, palette/size byte `a`) and the number `val`
 * centred: `cnt` is the extra width in characters, each digit of `val` adds 1 (2 with 2-byte chars). */
void DuelInfo_DrawLabelNumber(int a, const u8 *str, int val, int cnt)
{
    TextCanvasInit(0x20, 2);
    TextDrawString(3, 9 - a / 2, ((u8)a << 8) | 0xD, str);
    TextDrawString(2, 8 - a / 2, ((u8)a << 8) | 5, str);
    if (cnt > 0) {
        int v = val;
        int x;

        if (v > 9) {
            u8 dbl = gSaveData.flags & 0x80;
            do {
                cnt++;
                if (dbl)
                    cnt++;
                v /= 10;
            } while (v > 9);
        }
        x = a * cnt / 2;
        TextDrawNumber(x + 3, 9 - a / 2, ((u8)a << 8) | 8, val);
        x += 2;
        TextDrawNumber(x, 8 - a / 2, ((u8)a << 8) | 7, val);
    }
    TextCanvasToTiles(gDuelTextTiles, 9);
}
/* Card detail panel for the card in (player, slot): name, labels, ATK/DEF (palette 7 when equal to the
 * printed value, else 6), level and the attribute / kind icons. */
void DuelInfo_DrawMonsterZone(int player, int slot)
{
    struct CardInfo info;
    int atk;
    int def;
    int x;
    u32 pal;

    GetZoneCardStats(player, slot, &info);
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
    if (info.attr != 0 && info.attr <= 0x14)
        TextCellsLoadIcon(x, 9, gCardTypeIcons[info.attr]);
    if (info.kind != 0 && info.kind <= 6)
        TextCellsLoadIcon(x + 2, 10, gCardAttributeIcons[info.kind]);
}
