/*
 * deck_edit_panel (0x08064AF0-0x08065E6C): pack-list scene steps and deck-edit detail panel drawing
 * (wiki/functions/deck-edit-panel-c.md).
 *
 * PackList_Init, PackList_HandleInput and PackList_FadeOut are steps of the Get-a-pack pack list
 * (the scene runner is in booster_get_pack.c, the card drawing in booster_pack.c); they slide the
 * pack covers in gSceneWork (struct PackListWork) with the gPackListSlideEase factors. The rest of
 * the unit draws the deck-edit list view around the cursor card in gDeckEdit (struct DeckEdit): its
 * name and frame kind (DeckEdit_DrawCursorRowName), the attribute/type/kind icon header
 * (DeckEdit_DrawCardIcons) and the ATK/DEF box (DeckEdit_DrawAtkDef). The seven row-name buffers
 * in VRAM form a ring over two text pages (DeckEdit_RotateListRowRing, DeckEdit_FlipCursorRowPage).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, gCardNames */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind, enum CardFrame, CARD_STATS_* */
#include "constants/cards.h"        /* CARD_THE_MONARCHY, CARD_SET_SAIL_FOR_THE_KINGDOM, CARD_OBELISK_THE_TORMENTOR, ... */
#include "constants/sound.h"        /* SE_CURSOR, SE_CONFIRM */
#include "gba.h"                    /* REG_DISPCNT, REG_DMA3SAD, PLTT, VRAM, DPAD_LEFT, DPAD_RIGHT, A_BUTTON, CpuSet */
#include "main.h"                   /* struct Main gMain, newKeys, bgMapBuffer, bgHofs */
#include "booster.h"                /* struct PackListWork, struct PackInfo gPackInfo, PackList_* prototypes */
#include "deck_edit.h"              /* struct DeckEdit gDeckEdit, prototypes of the DeckEdit_* functions defined here */

/* gSceneWork (0x02020310) under the pack-list view; the starter-deck screen uses the second view of
 * the same area (booster_get_pack.c). */
extern struct PackListWork gSceneWork;      /* 0x02020310 */

/* ---- Local views kept for matching (build/readability/issues/deck_edit_panel.md) ---- */

/* deck_edit.h declares DeckEdit_GetListCardWide(u8, u8, u16); this unit passes the three arguments
 * un-narrowed, as the ROM does. */
extern u16 DeckEdit_GetListCardWide(u32 list, u32 row, u32 index) asm("DeckEdit_GetListCard");
/* deck_edit.h declares DeckEdit_GetCursorRowTile(void) returning u32; the text drawers below take
 * the value in a u16, without the narrowing the u32 prototype would add at the call. */
extern u16 DeckEdit_GetCursorRowTileU16(void) asm("DeckEdit_GetCursorRowTile");
/* palette.h declares the fades returning u32 (and taking s32); the callers here test the u16
 * result, so the ROM's narrowing stays visible. */
extern u16 FadeFromBlackU16(u32 step) asm("FadeFromBlack");
extern u16 FadeToBlackU16(u32 step) asm("FadeToBlack");
/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);

extern s32 __modsi3(s32 num, s32 denom);
extern void *memset(void *dst, int c, unsigned int n);
extern void *memcpy(void *dst, const void *src, unsigned int n);

/* ---- ROM data used only here ---- */

extern const u16 gPackListSlideEase[];      /* 0x080865CC: [8] slide easing factors (8.8) */
extern const u8 gStrDeckEditNoCards[];      /* 0x08087554: 'There are no cards.' */
extern const u8 gRaStatDigits[4];           /* 0x08087568: ATK/DEF digits of The Winged Dragon of Ra */
/* Card icon tile and palette blocks, 0x08704D48-0x08706DE8 (see DeckEdit_LoadCardIconTiles). */
extern const u8 gUnk_08704D48[], gUnk_08704DE8[], gUnk_08704E88[], gUnk_08704FA8[], gUnk_08705048[];
extern const u8 gUnk_08705188[], gUnk_087052C8[], gUnk_08705408[], gUnk_08705628[], gUnk_087056C8[];
extern const u8 gUnk_08705768[], gUnk_08705808[], gUnk_087058A8[], gUnk_08705948[], gUnk_087059E8[];
extern const u8 gUnk_08705A88[], gUnk_08705B28[], gUnk_08705BC8[], gUnk_08705C68[], gUnk_08705D08[];
extern const u8 gUnk_08705DA8[], gUnk_08705E48[], gUnk_08705EE8[], gUnk_08705F88[], gUnk_08706028[];
extern const u8 gUnk_087060C8[], gUnk_08706168[], gUnk_08706208[], gUnk_087062A8[], gUnk_08706348[];
extern const u8 gUnk_087063E8[], gUnk_08706528[], gUnk_08706668[], gUnk_087067A8[], gUnk_087068E8[];
extern const u8 gUnk_08706A28[], gUnk_08706B68[], gUnk_08706CA8[], gUnk_08706DE8[];

void PackList_DebugNop(u32 value) {}
/* Pack-list scene step: video setup, draw the covers, then fade in and enable the cover blend. */
u16 PackList_Init(void)
{
    /* Preserve the initialized scene base across the state-handler calls. */
    register struct PackListWork *s asm("r4") = &gSceneWork;
    u32 f = 1 & s->flags;
    if (f != 0) {
        PackList_FlushVram();
    done:
        return 0;
    }
    switch (s->state) {
    case 0:
        s->centerSlot = 1;
        s->firstIndex = s->packCount - 1;
        PackList_InitVideo();
        goto next;
    case 1:
        PackList_DrawBackground(2, 0x11, 0x1FD);
        PackList_DrawCovers(s->firstIndex);
        goto next;
    case 2:
        REG_DISPCNT |= 0x1440;
        PackList_DebugNop(0);
        if (FadeFromBlackU16(2) == 0)
            goto done;
        PackList_SetCoverAlpha(0);
        s->coverAlpha = f; /* f is always 0 here; the flag test above returned otherwise */
        s->coverAlphaTarget = 8;
        REG_DISPCNT |= 0xB00;
    next:
        s->state++;
        goto done;
    default:
        PackList_DebugNop(0);
        return 1;
    }
}
/* Pack-list input: slide animation (frame counter, eased with gPackListSlideEase) or LEFT/RIGHT to move the list, A to confirm. */
u16 PackList_HandleInput(void)
{
    vu16 zero;
    PackList_DebugNop(gSceneWork.slideFrame);
    if ((1 & gSceneWork.flags) != 0) {
        REG_DISPCNT |= 0x900;
        PackList_FlushVram();
        return 0;
    }
    if (gSceneWork.coverAlpha != gSceneWork.coverAlphaTarget) {
        if (gSceneWork.coverAlpha > gSceneWork.coverAlphaTarget)
            gSceneWork.coverAlpha--;
        else
            gSceneWork.coverAlpha++;
        PackList_SetCoverAlpha(gSceneWork.coverAlpha);
    }
    if (gSceneWork.slideFrame != 0) {
        s32 t = gSceneWork.slideTo - gSceneWork.slideFrom;
        t *= gPackListSlideEase[--gSceneWork.slideFrame];
        t /= 4096;
        t += gSceneWork.slideFrom;
        gMain.bgHofs[1] = t;
        REG_DISPCNT &= 0xFEFF;
        REG_DISPCNT &= 0xF7FF;
        if (gSceneWork.slideFrame == 0) {
            gSceneWork.firstIndex = gSceneWork.nextFirstIndex;
            PackList_DrawCovers(gSceneWork.firstIndex);
            gSceneWork.coverAlphaTarget = 8;
            gSceneWork.slideDir = 0;
            gSceneWork.slideTo = 0;
            gSceneWork.slideFrom = 0;
            gMain.bgHofs[1] = 0;
        }
        return 0;
    }
    if (gMain.newKeys & DPAD_LEFT) {
        vu32 *dma;
        s32 firstIndex;
        s32 packCount;
        PlaySE(SE_CURSOR);
        packCount = gSceneWork.packCount;
        firstIndex = gSceneWork.firstIndex;
        gSceneWork.nextFirstIndex = (firstIndex + packCount - 1) % packCount;
        gSceneWork.coverAlphaTarget = 0x10;
        gSceneWork.slideFrame = 8;
        gSceneWork.slideDir = 1;
        gSceneWork.slideTo = gSceneWork.slideFrom - 0x50;
        PackList_LoadCoverGfx(3, gPackInfo[gSceneWork.packRows[(firstIndex + gSceneWork.packCount - 1) % gSceneWork.packCount]].id);
        zero = 0;
        dma = (vu32 *)&REG_DMA3SAD;
        dma[0] = (u32)&zero;
        dma[1] = (u32)gMain.bgMapBuffer[2];
        dma[2] = 0x81000400;
        dma[2];
        while (dma[2] & 0x80000000)
            ;
        PackList_DrawCoverTiles(2, 0x78, 3);
        {
            /* A u32 temporary (not |= on the u8 field) gives the ROM's register choice, as in PackList_DrawCovers. */
            u32 f = gSceneWork.flags;
            f |= 1;
            gSceneWork.flags = f;
        }
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        vu32 *dma;
        s32 firstIndex;
        PlaySE(SE_CURSOR);
        firstIndex = gSceneWork.firstIndex;
        gSceneWork.nextFirstIndex = (firstIndex + 1) % gSceneWork.packCount;
        gSceneWork.coverAlphaTarget = 0x10;
        gSceneWork.slideFrame = 8;
        gSceneWork.slideDir = 2;
        gSceneWork.slideTo = gSceneWork.slideFrom + 0x50;
        PackList_LoadCoverGfx(3, gPackInfo[gSceneWork.packRows[(firstIndex + 3) % gSceneWork.packCount]].id);
        zero = 0;
        dma = (vu32 *)&REG_DMA3SAD;
        dma[0] = (u32)&zero;
        dma[1] = (u32)gMain.bgMapBuffer[2];
        dma[2] = 0x81000400;
        dma[2];
        while (dma[2] & 0x80000000)
            ;
        PackList_DrawCoverTiles(2, 0x60, 3);
        {
            /* A u32 temporary (not |= on the u8 field) gives the ROM's register choice, as in PackList_DrawCovers. */
            u32 f = gSceneWork.flags;
            f |= 1;
            gSceneWork.flags = f;
        }
    }
    if (gMain.newKeys & A_BUTTON) {
        PlaySE(SE_CONFIRM);
        return 1;
    }
    return 0;
}
/* Pack-list scene step: fade to black; returns 1 when the fade has finished. */
u16 PackList_FadeOut(void)
{
    if (FadeToBlackU16(4) != 0) {
        REG_DISPCNT &= 0xEEFF;
        return 1;
    }
    PackList_DebugNop(0);
    return 0;
}
/* Fill a 9 x 10 block of the BG map at 0x0600F000 with ascending tiles starting at page*90 + tileBase.
   wrap == 0: rows wrap (& 0x1F), columns run on; wrap == 1: both wrap. */
void DeckEdit_DrawPortraitTilemap(u8 col, u8 row, u8 page, u8 wrap, u8 tileBase)
{
    u16 tile = page * 0x5A + tileBase;
    u8 i;
    u8 j;
    switch (wrap) {
    case 0:
        for (i = 0; i < 10; i++) {
            u16 *p = &((u16 *)(VRAM + 0xF000))[(col & 0x1F) + (((s8)row + i) & 0x1F) * 32];
            for (j = 0; j < 9; j++)
                *p++ = tile++;
        }
        break;
    case 1:
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 9; j++) {
                int n = ((s8)col + j) & 0x1F;
                n += (((s8)row + i) & 0x1F) * 32;
                ((u16 *)(VRAM + 0xF000))[n] = tile++;
            }
        }
        break;
    }
}
/* Clear 0x400 bytes at dst with DMA3, then render the 0xE0 glyphs 0x20..0xFF (0x20 bytes each) after it:
   codes 0x20-0x7F as-is, 0x80-0xBF from 0xA0 with flags->katakana set, 0xC0-0xFF from 0xA0 with it cleared. */
void RenderOutlinedFontTiles(u32 dst, u8 color, u8 outlineColor, struct TextFlags *flags)
{
    vu16 zero = 0;
    vu32 *dma = (vu32 *)&REG_DMA3SAD;
    u8 ch;
    u16 i;

    dma[0] = (u32)&zero;
    dma[1] = dst;
    dma[2] = 0x81000200;
    dma[2];
    dst += 0x400;
    ch = 0x20;
    i = 0x20;
    do {
        switch (i) {
        case 0x80:
            ch = 0xA0;
            flags->katakana = 1;
            break;
        case 0xC0:
            ch = 0xA0;
            flags->katakana = 0;
            break;
        }
        RenderShadowedGlyph((u32 *)dst, ch++, color, outlineColor, (u8 *)flags);
        dst += 0x20;
        i++;
    } while (i <= 0xFF);
}
/* The seven list-row name buffers sit at VRAM + 0x6180, 0x2A0 bytes apart, addressed by
 * (gDeckEdit.listRowRing + slot) % 7; the cursor row names use two pages of 0x32 tiles. */
u32 DeckEdit_GetListRowVram(u8 slot)
{
    s32 n = __modsi3(gDeckEdit.listRowRing + slot, 7);
    return VRAM + 0x6180 + n * 0x2A0;
}
u32 DeckEdit_GetCursorRowVram(void)
{
    return VRAM + (gDeckEdit.cursorRowPage * 0x32 + 0x19B) * 32;
}
u16 DeckEdit_GetListRowTile(u8 slot)
{
    s32 n = __modsi3(gDeckEdit.listRowRing + slot, 7);
    return n * 21 + 0x30C;
}
u32 DeckEdit_GetCursorRowTile(void)
{
    return gDeckEdit.cursorRowPage * 0x32 + 0x19B;
}
/* Scroll the seven-entry list-row ring one step (1 = up, 2 = down), wrapping at the ends. */
void DeckEdit_RotateListRowRing(u8 dir)
{
    switch (dir) {
    case DECKEDIT_SCROLL_UP: {
        u8 n = (gDeckEdit.listRowRing - 1) & 0xF;
        gDeckEdit.listRowRing = n;
        dir = n; /* FAKEMATCH: reuse the dead direction argument. */
        if (dir == 0xF)
            gDeckEdit.listRowRing = 6;
        break;
    }
    case DECKEDIT_SCROLL_DOWN: {
        u8 n = (gDeckEdit.listRowRing + 1) & 0xF;
        gDeckEdit.listRowRing = n;
        if (n == 7)
            gDeckEdit.listRowRing = 0;
        break;
    }
    }
} /* 0x08065058 size 0x7C */
/* FAKEMATCH (decomp-permuter): the ROM stores the toggled bit twice; only a non-void return type with no
   return statement reproduces the extra mask. */
int DeckEdit_FlipCursorRowPage(void)
{
    int next = gDeckEdit.cursorRowPage + 1;
    int stored = (gDeckEdit.cursorRowPage = next);
    gDeckEdit.cursorRowPage = stored;
}

/* Draw the name of card `cardId` (gCardNames, 0x40 bytes per id) into the map buffer `map` at (col + 4, row + 1). */
void DeckEdit_DrawListRowName(u16 cardId, u8 *map, u16 col, u16 row, u32 unused, u8 slot)
{
    u32 tiles = DeckEdit_GetListRowVram(slot);
    u16 firstTile = DeckEdit_GetListRowTile(slot);
    DrawStringTiles((u8 *)(gCardNames + cardId * CARD_NAME_SIZE), (u32)(map + (((col + 4) & 0x1F) + ((row + 1) & 0x1F) * 32) * 2), tiles, firstTile, 2, 1, 0, 0);
}
/* Constant-address table reads (see include/card_data.h): the ROM reloads the table address at
 * every use, so these stay macros over the literal addresses (gCardStats / gCardIdToNumber)
 * instead of the symbols, whose form generates different code. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) ((int)((CARD_STATS(id) & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT))
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/* Draw the cursor card's name (gCardNames, 0x40 bytes per id) and pick gDeckEdit.cursorCardFrame
 * for card `cardId`: the three tickets and the Egyptian Gods by card number, everything else by
 * type and kind. The number is tested twice (see wiki/functions/deck-edit-panel-c.md). */
void DeckEdit_DrawCursorRowName(u16 cardId, u8 *map, u16 col, u16 row)
{
    char buf[0x80];
    const u8 *gfx = gCardNames + cardId * CARD_NAME_SIZE;
    u32 tiles;
    u16 firstTile;
    int frame;
    StrCopy(buf, (const char *)gfx);
    tiles = DeckEdit_GetCursorRowVram();
    firstTile = DeckEdit_GetCursorRowTileU16();
    DrawTextStrip((u8 *)gfx, map + (((col + 4) & 0x1F) + ((row + 1) & 0x1F) * 32) * 2, tiles, firstTile, 2, 1, 0, 0);
    DeckEdit_FlipCursorRowPage();
    switch (CARD_NUMBER(cardId)) {
    case CARD_OBELISK_THE_TORMENTOR:
        gDeckEdit.cursorCardFrame = CARD_FRAME_RITUAL;
        return;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        gDeckEdit.cursorCardFrame = CARD_FRAME_EFFECT;
        return;
    case CARD_THE_MONARCHY:
    case CARD_SET_SAIL_FOR_THE_KINGDOM:
    case CARD_GLORY_OF_THE_KINGS_HAND:
        gDeckEdit.cursorCardFrame = CARD_FRAME_NORMAL;
        return;
    default: {
        switch (CARD_TYPE(cardId)) {
        case CARD_TYPE_TRAP:
            gDeckEdit.cursorCardFrame = CARD_FRAME_TRAP;
            return;
        case CARD_TYPE_MAGIC:
            gDeckEdit.cursorCardFrame = CARD_FRAME_MAGIC;
            return;
        }
        switch (CARD_NUMBER(cardId)) {
        case CARD_OBELISK_THE_TORMENTOR:
            frame = CARD_FRAME_RITUAL;
            break;
        case CARD_SLIFER_THE_SKY_DRAGON:
        case CARD_THE_WINGED_DRAGON_OF_RA:
            frame = CARD_FRAME_EFFECT;
            break;
        default:
            switch (CARD_TYPE(cardId)) {
            case CARD_TYPE_MAGIC:
                frame = CARD_KIND_MAGIC;
                break;
            case CARD_TYPE_TRAP:
                frame = CARD_KIND_TRAP;
                break;
            case CARD_TYPE_TICKET:
                frame = CARD_KIND_TICKET;
                break;
            default:
                frame = CARD_STATS_KIND(CARD_STATS(cardId));
                break;
            }
            break;
        }
        break;
    }
    }
    gDeckEdit.cursorCardFrame = frame;
}
/* Draw the string gStrDeckEditNoCards (clamped to 100 chars) into the map buffer `map` at (col + 8, row + 3). */
void DeckEdit_DrawNoCardsText(u32 unusedCardId, u8 *map, u16 col, u16 row)
{
    char buf[0x40];
    u32 tiles;
    u16 firstTile;
    StrCopy(buf, (const char *)gStrDeckEditNoCards);
    if (StrLen(buf) > 100)
        buf[100] = 0;
    tiles = DeckEdit_GetCursorRowVram();
    firstTile = DeckEdit_GetCursorRowTileU16();
    DrawTextStrip(buf, map + (((col + 8) & 0x1F) + ((row + 3) & 0x1F) * 32) * 2, tiles, firstTile, 2, 1, 0, 0);
}
/* Copy the card icon tile/palette blocks (gUnk_0870xxxx above, 0x40 u32 words each) to dst. */
void DeckEdit_LoadCardIconTiles(u8 *dst)
{
    CpuSet(gUnk_08706528, dst, 0x40);
    CpuSet(gUnk_08706DE8, dst + 0x80, 0x40);
    CpuSet(gUnk_08706B68, dst + 0x100, 0x40);
    CpuSet(gUnk_08706668, dst + 0x180, 0x40);
    CpuSet(gUnk_087063E8, dst + 0x200, 0x40);
    CpuSet(gUnk_087068E8, dst + 0x280, 0x40);
    CpuSet(gUnk_08706A28, dst + 0x300, 0x40);
    CpuSet(gUnk_08706CA8, dst + 0x380, 0x40);
    CpuSet(gUnk_087067A8, dst + 0x400, 0x40);
    CpuSet(gUnk_08705808, dst + 0x480, 0x40);
    CpuSet(gUnk_08706348, dst + 0x500, 0x40);
    CpuSet(gUnk_08705768, dst + 0x580, 0x40);
    CpuSet(gUnk_087059E8, dst + 0x600, 0x40);
    CpuSet(gUnk_08705B28, dst + 0x680, 0x40);
    CpuSet(gUnk_087058A8, dst + 0x700, 0x40);
    CpuSet(gUnk_08705D08, dst + 0x780, 0x40);
    CpuSet(gUnk_08706028, dst + 0x800, 0x40);
    CpuSet(gUnk_08705E48, dst + 0x880, 0x40);
    CpuSet(gUnk_08705DA8, dst + 0x900, 0x40);
    CpuSet(gUnk_08705C68, dst + 0x980, 0x40);
    CpuSet(gUnk_08705A88, dst + 0xA00, 0x40);
    CpuSet(gUnk_08706168, dst + 0xA80, 0x40);
    CpuSet(gUnk_08705F88, dst + 0xB00, 0x40);
    CpuSet(gUnk_087060C8, dst + 0xB80, 0x40);
    CpuSet(gUnk_087062A8, dst + 0xC00, 0x40);
    CpuSet(gUnk_08706208, dst + 0xC80, 0x40);
    CpuSet(gUnk_08705EE8, dst + 0xD00, 0x40);
    CpuSet(gUnk_08705BC8, dst + 0xD80, 0x40);
    CpuSet(gUnk_08705948, dst + 0xE00, 0x40);
    CpuSet(gUnk_08705048, dst + 0xE80, 0x40);
    CpuSet(gUnk_087052C8, dst + 0xF00, 0x40);
    CpuSet(gUnk_08705628, dst + 0xF80, 0x40);
    CpuSet(gUnk_08705188, dst + 0x1000, 0x40);
    CpuSet(gUnk_087056C8, dst + 0x1080, 0x40);
    CpuSet(gUnk_08705408, dst + 0x1100, 0x40);
    CpuSet(gUnk_08704DE8, dst + 0x1180, 0x40);
    CpuSet(gUnk_08704FA8, dst + 0x1200, 0x40);
    CpuSet(gUnk_08704D48, dst + 0x1280, 0x40);
    CpuSet(gUnk_08704E88, dst + 0x1300, 0x30);
}
extern const u16 gAttributeIconTiles[], gTypeIconTiles[], gSpellSubtypeIconTiles[], gCardKindIconTiles[];
extern const u32 *const gAttributeIconPals[], *const gTypeIconPals[], *const gSpellSubtypeIconPals[], *const gCardKindIconPals[];
/* `set` is an enum CardIconSet at every ROM call site, selecting one of the four table pairs.
   Draw one 2x2-cell metatile `idx` at (col, row) of the BG map `map` (tile ids from the per-`set`
   table plus `tileBase`, palette bank `pal`) and load its 16-colour palette into bank `pal`. */
void DeckEdit_DrawCardIcon(u8 set, u8 idx, u16 *map, u8 col, u8 row, u8 pal, u16 tileBase)
{
    const u16 *tiles;
    const u32 *const *pals;
    if (idx != 0) {
        const u16 *t;
        u16 *p0;
        int x0, y0, x1, y1;
        switch (set) {
        case CARD_ICON_SET_ATTRIBUTE:
            tiles = gAttributeIconTiles;
            pals = gAttributeIconPals;
            break;
        case CARD_ICON_SET_TYPE:
            tiles = gTypeIconTiles;
            pals = gTypeIconPals;
            break;
        case CARD_ICON_SET_SPELL_SUBTYPE:
            tiles = gSpellSubtypeIconTiles;
            pals = gSpellSubtypeIconPals;
            break;
        case CARD_ICON_SET_KIND:
            tiles = gCardKindIconTiles;
            pals = gCardKindIconPals;
            break;
        }
        x0 = col & 0x1F;
        y0 = (row & 0x1F) * 32;
        p0 = &map[x0 + y0];
        t = &tiles[idx];
        *p0 = ((*t + tileBase) & 0x3FF) | pal << 12;
        x1 = (col + 1) & 0x1F;
        map[x1 + y0] = ((*t + tileBase + 1) & 0x3FF) | pal << 12;
        y1 = ((row + 1) & 0x1F) * 32;
        map[x0 + y1] = ((*t + tileBase + 2) & 0x3FF) | pal << 12;
        map[x1 + y1] = ((*t + tileBase + 3) & 0x3FF) | pal << 12;
        CpuSet(pals[idx], (void *)(PLTT + pal * 32), 0x10);
    }
}

#define ROW(pos) (((((pos) + 7) * 8 + gDeckEdit.bg0Vofs) & 0xFF) >> 3)
/* Row macro for the default case: masks with the local `mask` (0xFF) instead of a literal (see below). */
#define ROW_M(pos) (((((pos) + 7) * 8 + gDeckEdit.bg0Vofs) & mask) >> 3)
/* Draw the three-part card header (attribute/type/kind icons) for the list entry `rowOffset` of the card view. */
void DeckEdit_DrawCardIcons(u16 rowOffset)
{
    u16 cardId;
    int subtype;
    cardId = DeckEdit_GetListCardWide(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]);
    switch (CARD_TYPE(cardId)) {
    case CARD_TYPE_TRAP:
        DeckEdit_DrawCardIcon(CARD_ICON_SET_ATTRIBUTE, ATTRIBUTE_ICON_TRAP, (u16 *)(VRAM + 0xC000), 4, ROW(rowOffset), 6, 0x100);
        switch (CARD_TYPE(cardId)) {
        case CARD_TYPE_TRAP:
        case CARD_TYPE_MAGIC:
            subtype = (CARD_STATS(cardId) & CARD_STATS_SUBTYPE_MASK) >> CARD_STATS_SUBTYPE_SHIFT;
            break;
        default:
            subtype = 0;
            break;
        }
        DeckEdit_DrawCardIcon(CARD_ICON_SET_SPELL_SUBTYPE, subtype, (u16 *)(VRAM + 0xC000), 6, ROW(rowOffset), 7, 0x100);
        break;
    case CARD_TYPE_MAGIC:
        DeckEdit_DrawCardIcon(CARD_ICON_SET_ATTRIBUTE, ATTRIBUTE_ICON_MAGIC, (u16 *)(VRAM + 0xC000), 4, ROW(rowOffset), 6, 0x100);
        switch (CARD_TYPE(cardId)) {
        case CARD_TYPE_TRAP:
        case CARD_TYPE_MAGIC:
            subtype = (CARD_STATS(cardId) & CARD_STATS_SUBTYPE_MASK) >> CARD_STATS_SUBTYPE_SHIFT;
            break;
        default:
            subtype = 0;
            break;
        }
        DeckEdit_DrawCardIcon(CARD_ICON_SET_SPELL_SUBTYPE, subtype, (u16 *)(VRAM + 0xC000), 6, ROW(rowOffset), 7, 0x100);
        break;
    case CARD_TYPE_TICKET:
        break;
    case CARD_TYPE_DIVINE:
        DeckEdit_DrawCardIcon(CARD_ICON_SET_ATTRIBUTE, ATTRIBUTE_ICON_DIVINE, (u16 *)(VRAM + 0xC000), 4, ROW(rowOffset), 6, 0x100);
        break;
    default: {
        /* FAKEMATCH: with a literal 0xFF, CSE shares one 0xFF pseudo across the first two calls and local-alloc
           gives it sl, spilling rowOffset+7; a block-scope mask variable starts its life earlier, so it loses sl to
           rowOffset+7 and is rematerialised as `movs r3, #0xFF` at each use, as in the ROM. */
        int mask = 0xFF;
        int frame;
        DeckEdit_DrawCardIcon(CARD_ICON_SET_ATTRIBUTE, CARD_STATS(cardId) >> CARD_STATS_ATTR_SHIFT, (u16 *)(VRAM + 0xC000), 4, ROW_M(rowOffset), 6, 0x100);
        DeckEdit_DrawCardIcon(CARD_ICON_SET_TYPE, CARD_TYPE(cardId), (u16 *)(VRAM + 0xC000), 6, ROW_M(rowOffset), 7, 0x100);
        switch (CARD_NUMBER(cardId)) {
        case CARD_OBELISK_THE_TORMENTOR:
            frame = CARD_FRAME_RITUAL;
            break;
        case CARD_SLIFER_THE_SKY_DRAGON:
        case CARD_THE_WINGED_DRAGON_OF_RA:
            frame = CARD_FRAME_EFFECT;
            break;
        default:
            switch (CARD_TYPE(cardId)) {
            case CARD_TYPE_MAGIC:
                frame = CARD_KIND_MAGIC;
                break;
            case CARD_TYPE_TRAP:
                frame = CARD_KIND_TRAP;
                break;
            case CARD_TYPE_TICKET:
                frame = CARD_KIND_TICKET;
                break;
            default:
                frame = CARD_STATS_KIND(CARD_STATS(cardId));
                break;
            }
            break;
        }
        /* FAKEMATCH: the r1 clobber makes `frame` conflict with r1, so global-alloc puts it in r0 (copied to r1 for
           the call) as in the ROM instead of taking the r1 copy preference. */
        asm volatile("" ::: "r1");
        DeckEdit_DrawCardIcon(CARD_ICON_SET_KIND, frame, (u16 *)(VRAM + 0xC000), 8, ROW(rowOffset), 1, 0x100);
        break;
    }
    }
}
/* FAKEMATCH: int-parameter views of PutMapTileRun / DrawNumberTiles. The ROM passes the u16 row and the
 * `0x300 | digit` / ATK values without narrowing them to the callees' u8/u16 parameter types, and the
 * u16 prototype would also reorder the `orr` operands in the digit loops. */
#define PutMapTileRunInt ((void (*)(int start, u16 *dst, int pal, int mode, int count))PutMapTileRun)
#define DrawNumberTilesInt ((void (*)(int val, int n, int mode, u16 *dst, int col, int row, int pal, int base, int m2))DrawNumberTiles)

/* ATK shown for card `cardId`: 0 for the Trap/Magic/Ticket types, 4000 for Divine, else the stat * 10. */
static inline u16 CardAtkValue(u16 cardId)
{
    switch (CARD_TYPE(cardId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_ATK(CARD_STATS(cardId)) * CARD_STATS_POINTS_SCALE;
    }
}

/* DEF shown for card `cardId`: same rule as CardAtkValue. */
static inline u16 CardDefValue(u16 cardId)
{
    switch (CARD_TYPE(cardId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_DEF(CARD_STATS(cardId)) * CARD_STATS_POINTS_SCALE;
    }
}

/* Draw the selected card's ATK/DEF box into the tilemap at (col, row). The Trap/Magic/Ticket types
 * draw nothing; Divine draws a frame and a fixed 4-digit pattern chosen by card number. */
void DeckEdit_DrawAtkDef(u16 *map, u16 col, u16 row)
{
    u8 d[4];
    u16 cardId;
    u8 i;

    /* FAKEMATCH: three empty insns lengthen map's live range so global-alloc ranks it below the loop
     * temporary (col + 2) & 0x1F; map then gets r8 and that temporary r7 (spilled by reload), as in the ROM. */
    asm("");
    asm("");
    asm("");
    cardId = DeckEdit_GetListCardWide(gDeckEdit.curList, gDeckEdit.listRow[gDeckEdit.curList], gDeckEdit.listPos[gDeckEdit.curList]);
    switch (CARD_TYPE(cardId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        break;
    case CARD_TYPE_DIVINE:
        map[col + (row << 5)] = 0x198;
        for (i = 0; i <= 1; i++) {
            int r = ((row + i) & 0x1F) << 5;
            map[((col + 1) & 0x1F) + r] = 0x2258;
            map[((col + 2) & 0x1F) + r] = 0x2230;
            map[((col + 3) & 0x1F) + r] = 0x2230;
            map[((col + 4) & 0x1F) + r] = 0x2230;
        }
        map[col + (((row + 1) & 0x1F) << 5)] = 0x199;
        switch (CARD_NUMBER(cardId)) {
        case CARD_OBELISK_THE_TORMENTOR:
            {
                u8 *p = d;
                memset(p, 0, 4);
                *p = 4;
            }
            for (i = 0; i <= 3; i++) {
                PutMapTileRunInt(0x300 | d[i], &map[col + 1 + (row << 5)], 2, 0, 1);
                PutMapTileRunInt(0x300 | d[i], &map[col++ + 1 + ((row + 1) << 5)], 2, 0, 1);
            }
            break;
        case CARD_SLIFER_THE_SKY_DRAGON:
            {
                u8 *p = d;
                memset(p, 0, 4);
                *p = 10;
            }
            for (i = 0; i <= 3; i++) {
                PutMapTileRunInt(0x300 | d[i], &map[col + 1 + (row << 5)], 2, 0, 1);
                PutMapTileRunInt(0x300 | d[i], &map[col++ + 1 + ((row + 1) << 5)], 2, 0, 1);
            }
            break;
        case CARD_THE_WINGED_DRAGON_OF_RA:
            memcpy(d, gRaStatDigits, 4);
            for (i = 0; i <= 3; i++) {
                PutMapTileRunInt(0x300 | d[i], &map[col + 1 + (row << 5)], 2, 0, 1);
                PutMapTileRunInt(0x300 | d[i], &map[col++ + 1 + ((row + 1) << 5)], 2, 0, 1);
            }
            break;
        }
        break;
    default:
        map[col + (row << 5)] = 0x198;
        map[col + (((row + 1) & 0x1F) << 5)] = 0x199;
        DrawNumberTilesInt(CardAtkValue(cardId), 4, 1, map, (col + 4) & 0x1F, row, 2, 0x300, 0);
        DrawNumberTilesInt(CardDefValue(cardId), 4, 1, map, (col + 4) & 0x1F, (row + 1) & 0x1F, 2, 0x300, 0);
        break;
    }
}
#undef PutMapTileRunInt
#undef DrawNumberTilesInt
