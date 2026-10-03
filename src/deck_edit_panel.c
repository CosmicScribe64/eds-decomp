#include "global.h"
#include "gba.h"

extern s32 __modsi3(s32 a, s32 b);
extern u16 FadeToBlack(u32 a);
extern u16 FadeFromBlack(u32 a);
extern void PackList_DebugNop(u32 a);
extern u16 DeckEdit_GetListCard(u32 a, u32 b, u32 c);
extern void DeckEdit_DrawCardIcon(u8 kind, u8 idx, u16 *map, u8 col, u8 row, u8 pal, u16 tile);
extern void StrCopy(void *dst, const void *src);
extern void DrawTextStrip(void *str, void *dst, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern u16 sub_08065034_u16(void) asm("DeckEdit_GetCursorRowTile");

/* Scene state at 0x0201DB20; only the byte at +0x1C34 is used here. */
struct PageState {
    u8 pad0[0x620];
    u16 arr620[15];                 /* +0x620 */
    u16 scroll;                     /* +0x63E */
    u8 pad640[0x14A0 - 0x640];
    u8 arr14A0[0x1C1C - 0x14A0];    /* +0x14A0 */
    u8 cursor;                      /* +0x1C1C */
    u8 pad1C1D[0x1C34 - 0x1C1D];
    u8 page : 1;                    /* +0x1C34 bit 0: which of two pages is shown */
    u8 off : 4;                     /* bits 1-4: rotation offset 0..6 */
    u8 rest : 3;
    u8 pad1C35[0x1C3B - 0x1C35];
    u8 frameKind;                   /* +0x1C3B card frame graphic index (0..9) */
};
extern struct PageState gDeckEdit;

/* Pack-list slide state at 0x02020310 (see booster_get_pack). */
struct Slide {
    s32 state;                      /* +0x00 step of the scene runner */
    s32 unk4;
    s32 unk8;
    s32 current;                    /* +0x0C */
    s32 sel;                        /* +0x10 selected list index */
    s32 sel2;                       /* +0x14 candidate index after a key press */
    u8 flags;                       /* +0x18 bit 0: VRAM copy pending */
    u8 pad19;
    u16 unk1A;
    u16 unk1C;
    u16 pad1E;
    s32 pos;                        /* +0x20 */
    s32 posBase;                    /* +0x24 */
    u16 frame;                      /* +0x28 */
    u16 dir;                        /* +0x2A */
    u16 list[0x20];                 /* +0x2C */
    u16 count;                      /* +0x6C */
};
extern struct Slide gSceneWork;
struct PackInfo {
    u16 id;
    u8 pad2[2];
    const u8 *image;
    u8 name[0x40];
};
extern struct PackInfo gPackInfo[];
extern const u16 gPackListSlideEase[];
struct Main {
    u8 pad0[6];
    u16 keysNew;                    /* +0x06 */
    u8 pad8[0x41C - 8];
    u16 bgMap[8][0x400];            /* +0x41C BG map buffers */
    u8 pad441C[0x442A - 0x441C];
    u16 unk442A;                    /* +0x442A */
};
extern struct Main gMain;
extern void PackList_FlushVram(void);
extern void PackList_InitVideo(void);
extern void PackList_DrawBackground(u32 a, u32 b, u32 c);
extern void PackList_DrawCovers(u32 a);
extern void PackList_SetCoverAlpha(u32 a);
extern void PackList_LoadCoverGfx(u32 a, u32 b);
extern void PackList_DrawCoverTiles(u32 a, u32 b, u32 c);
extern void PlaySE(u32 a);

void PackList_DebugNop(u32 a) {}
u16 PackList_Init(void)
{
    /* Preserve the initialized scene base across the state-handler calls. */
    register struct Slide *s asm("r4") = &gSceneWork;
    u32 f = 1 & s->flags;
    if (f != 0) {
        PackList_FlushVram();
    done:
        return 0;
    }
    switch (s->state) {
    case 0:
        s->current = 1;
        s->sel = s->count - 1;
        PackList_InitVideo();
        goto next;
    case 1:
        PackList_DrawBackground(2, 0x11, 0x1FD);
        PackList_DrawCovers(s->sel);
        goto next;
    case 2:
        REG_DISPCNT |= 0x1440;
        PackList_DebugNop(0);
        if (FadeFromBlack(2) == 0)
            goto done;
        PackList_SetCoverAlpha(0);
        s->unk1A = f;
        s->unk1C = 8;
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
    PackList_DebugNop(gSceneWork.frame);
    if ((1 & gSceneWork.flags) != 0) {
        REG_DISPCNT |= 0x900;
        PackList_FlushVram();
        return 0;
    }
    if (gSceneWork.unk1A != gSceneWork.unk1C) {
        if (gSceneWork.unk1A > gSceneWork.unk1C)
            gSceneWork.unk1A--;
        else
            gSceneWork.unk1A++;
        PackList_SetCoverAlpha(gSceneWork.unk1A);
    }
    if (gSceneWork.frame != 0) {
        s32 t = gSceneWork.pos - gSceneWork.posBase;
        t *= gPackListSlideEase[--gSceneWork.frame];
        t /= 4096;
        t += gSceneWork.posBase;
        gMain.unk442A = t;
        REG_DISPCNT &= 0xFEFF;
        REG_DISPCNT &= 0xF7FF;
        if (gSceneWork.frame == 0) {
            gSceneWork.sel = gSceneWork.sel2;
            PackList_DrawCovers(gSceneWork.sel);
            gSceneWork.unk1C = 8;
            gSceneWork.dir = 0;
            gSceneWork.pos = 0;
            gSceneWork.posBase = 0;
            gMain.unk442A = 0;
        }
        return 0;
    }
    if (gMain.keysNew & 0x20) {
        vu32 *dma;
        s32 sel;
        s32 count;
        PlaySE(0);
        count = gSceneWork.count;
        sel = gSceneWork.sel;
        gSceneWork.sel2 = (sel + count - 1) % count;
        gSceneWork.unk1C = 0x10;
        gSceneWork.frame = 8;
        gSceneWork.dir = 1;
        gSceneWork.pos = gSceneWork.posBase - 0x50;
        PackList_LoadCoverGfx(3, gPackInfo[gSceneWork.list[(sel + gSceneWork.count - 1) % gSceneWork.count]].id);
        zero = 0;
        dma = (vu32 *)0x040000D4;
        dma[0] = (u32)&zero;
        dma[1] = (u32)gMain.bgMap[2];
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
    if (gMain.keysNew & 0x10) {
        vu32 *dma;
        s32 sel;
        PlaySE(0);
        sel = gSceneWork.sel;
        gSceneWork.sel2 = (sel + 1) % gSceneWork.count;
        gSceneWork.unk1C = 0x10;
        gSceneWork.frame = 8;
        gSceneWork.dir = 2;
        gSceneWork.pos = gSceneWork.posBase + 0x50;
        PackList_LoadCoverGfx(3, gPackInfo[gSceneWork.list[(sel + 3) % gSceneWork.count]].id);
        zero = 0;
        dma = (vu32 *)0x040000D4;
        dma[0] = (u32)&zero;
        dma[1] = (u32)gMain.bgMap[2];
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
    if (gMain.keysNew & 1) {
        PlaySE(1);
        return 1;
    }
    return 0;
}
u16 PackList_FadeOut(void)
{
    if (FadeToBlack(4) != 0) {
        REG_DISPCNT &= 0xEEFF;
        return 1;
    }
    PackList_DebugNop(0);
    return 0;
}
/* Fill a 9 x 10 block of the BG map at 0x0600F000 with ascending tiles starting at c*90 + e.
   d == 0: rows wrap (& 0x1F), columns run on; d == 1: both wrap. */
void DeckEdit_DrawPortraitTilemap(u8 a, u8 b, u8 c, u8 d, u8 e)
{
    u16 tile = c * 0x5A + e;
    u8 i;
    u8 j;
    switch (d) {
    case 0:
        for (i = 0; i < 10; i++) {
            u16 *p = &((u16 *)0x0600F000)[(a & 0x1F) + (((s8)b + i) & 0x1F) * 32];
            for (j = 0; j < 9; j++)
                *p++ = tile++;
        }
        break;
    case 1:
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 9; j++) {
                int n = ((s8)a + j) & 0x1F;
                n += (((s8)b + i) & 0x1F) * 32;
                ((u16 *)0x0600F000)[n] = tile++;
            }
        }
        break;
    }
}
extern void RenderShadowedGlyph(u32 dst, u32 ch, u32 a, u32 b, void *flags);
/* Clear 0x400 bytes at dst with DMA3, then render the 0xE0 glyphs 0x20..0xFF (0x20 bytes each) after it:
   codes 0x20-0x7F as-is, 0x80-0xBF from 0xA0 with flags bit 0 set, 0xC0-0xFF from 0xA0 with it cleared. */
struct Flag0 {
    u8 bit0 : 1;
    u8 rest : 7;
};
void RenderOutlinedFontTiles(u32 dst, u8 a, u8 b, struct Flag0 *flags)
{
    vu16 zero = 0;
    vu32 *dma = (vu32 *)0x040000D4;
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
            flags->bit0 = 1;
            break;
        case 0xC0:
            ch = 0xA0;
            flags->bit0 = 0;
            break;
        }
        RenderShadowedGlyph(dst, ch++, a, b, flags);
        dst += 0x20;
        i++;
    } while (i <= 0xFF);
}
/* Returns the VRAM address of the current slot: 0x06006180 + ((off + slot) % 7) * 0x2A0. */
u32 DeckEdit_GetListRowVram(u8 slot)
{
    s32 n = __modsi3(gDeckEdit.off + slot, 7);
    return 0x06006180 + n * 0x2A0;
}
u32 DeckEdit_GetCursorRowVram(void)
{
    return 0x06000000 + (gDeckEdit.page * 0x32 + 0x19B) * 32;
}
u16 DeckEdit_GetListRowTile(u8 slot)
{
    s32 n = __modsi3(gDeckEdit.off + slot, 7);
    return n * 21 + 0x30C;
}
u32 DeckEdit_GetCursorRowTile(void)
{
    return gDeckEdit.page * 0x32 + 0x19B;
}
void DeckEdit_RotateListRowRing(u8 dir)
{
    switch (dir) {
    case 1: {
        u8 n = (gDeckEdit.off - 1) & 0xF;
        gDeckEdit.off = n;
        dir = n; /* FAKEMATCH: reuse the dead direction argument. */
        if (dir == 0xF)
            gDeckEdit.off = 6;
        break;
    }
    case 2: {
        u8 n = (gDeckEdit.off + 1) & 0xF;
        gDeckEdit.off = n;
        if (n == 7)
            gDeckEdit.off = 0;
        break;
    }
    }
} /* 0x08065058 size 0x7C */
/* FAKEMATCH (decomp-permuter): the ROM stores the toggled bit twice; only a non-void return type with no
   return statement reproduces the extra mask. */
int DeckEdit_FlipCursorRowPage(void)
{
    int next = gDeckEdit.page + 1;
    int stored = (gDeckEdit.page = next);
    gDeckEdit.page = stored;
}

extern void DrawStringTiles(void *src, void *dst, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern const u8 gCardNames[];
/* Draw one 0x40-byte graphic from the table at 0x0822C720 into the map buffer `map` at (col + 4, row + 1). */
void DeckEdit_DrawListRowName(u16 idx, u8 *map, u16 col, u16 row, u32 unused, u8 slot)
{
    u32 a = DeckEdit_GetListRowVram(slot);
    u16 b = DeckEdit_GetListRowTile(slot);
    DrawStringTiles((void *)(gCardNames + idx * 64), map + (((col + 4) & 0x1F) + ((row + 1) & 0x1F) * 32) * 2, a, b, 2, 1, 0, 0);
}
extern int DeckEdit_FlipCursorRowPage(void);
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_KIND(id) ((int)((CARD_STATS(id) & 0x1F00000) >> 20))
#define CARD_NUM(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

/* Draw a card-sized graphic (table 0x0822C720, 0x40 bytes per id) and pick the frame kind byte (+0x1C3B) for card `id`. */
void DeckEdit_DrawCursorRowName(u16 id, u8 *map, u16 col, u16 row)
{
    char buf[0x80];
    const u8 *gfx = gCardNames + id * 64;
    u32 a;
    u16 b;
    int v;
    StrCopy(buf, gfx);
    a = DeckEdit_GetCursorRowVram();
    b = sub_08065034_u16();
    DrawTextStrip((void *)gfx, map + (((col + 4) & 0x1F) + ((row + 1) & 0x1F) * 32) * 2, a, b, 2, 1, 0, 0);
    DeckEdit_FlipCursorRowPage();
    switch (CARD_NUM(id)) {
    case 0x776:
        gDeckEdit.frameKind = 3;
        return;
    case 0x777:
    case 0x778:
        gDeckEdit.frameKind = 1;
        return;
    case 0x76D:
    case 0x76E:
    case 0x76F:
        gDeckEdit.frameKind = 0;
        return;
    default: {
        switch (CARD_KIND(id)) {
        case 0x15:
            gDeckEdit.frameKind = 5;
            return;
        case 0x16:
            gDeckEdit.frameKind = 4;
            return;
        }
        switch (CARD_NUM(id)) {
        case 0x776:
            v = 3;
            break;
        case 0x777:
        case 0x778:
            v = 1;
            break;
        default:
            switch (CARD_KIND(id)) {
            case 0x16:
                v = 7;
                break;
            case 0x15:
                v = 8;
                break;
            case 0x17:
                v = 9;
                break;
            default:
                v = (CARD_STATS(id) & 0xC0000) >> 18;
                break;
            }
            break;
        }
        break;
    }
    }
    gDeckEdit.frameKind = v;
}
extern s32 StrLen(const void *s);
extern const u8 gStrDeckEditNoCards[];
/* Draw the string gStrDeckEditNoCards (clamped to 100 chars) into the map buffer `map` at (col + 8, row + 3). */
void DeckEdit_DrawNoCardsText(u32 unused, u8 *map, u16 col, u16 row)
{
    char buf[0x40];
    u32 a;
    u16 b;
    StrCopy(buf, gStrDeckEditNoCards);
    if (StrLen(buf) > 0x64)
        buf[0x64] = 0;
    a = DeckEdit_GetCursorRowVram();
    b = sub_08065034_u16();
    DrawTextStrip(buf, map + (((col + 8) & 0x1F) + ((row + 3) & 0x1F) * 32) * 2, a, b, 2, 1, 0, 0);
}
extern const u8 gUnk_08704D48[];
extern const u8 gUnk_08704DE8[];
extern const u8 gUnk_08704E88[];
extern const u8 gUnk_08704FA8[];
extern const u8 gUnk_08705048[];
extern const u8 gUnk_08705188[];
extern const u8 gUnk_087052C8[];
extern const u8 gUnk_08705408[];
extern const u8 gUnk_08705628[];
extern const u8 gUnk_087056C8[];
extern const u8 gUnk_08705768[];
extern const u8 gUnk_08705808[];
extern const u8 gUnk_087058A8[];
extern const u8 gUnk_08705948[];
extern const u8 gUnk_087059E8[];
extern const u8 gUnk_08705A88[];
extern const u8 gUnk_08705B28[];
extern const u8 gUnk_08705BC8[];
extern const u8 gUnk_08705C68[];
extern const u8 gUnk_08705D08[];
extern const u8 gUnk_08705DA8[];
extern const u8 gUnk_08705E48[];
extern const u8 gUnk_08705EE8[];
extern const u8 gUnk_08705F88[];
extern const u8 gUnk_08706028[];
extern const u8 gUnk_087060C8[];
extern const u8 gUnk_08706168[];
extern const u8 gUnk_08706208[];
extern const u8 gUnk_087062A8[];
extern const u8 gUnk_08706348[];
extern const u8 gUnk_087063E8[];
extern const u8 gUnk_08706528[];
extern const u8 gUnk_08706668[];
extern const u8 gUnk_087067A8[];
extern const u8 gUnk_087068E8[];
extern const u8 gUnk_08706A28[];
extern const u8 gUnk_08706B68[];
extern const u8 gUnk_08706CA8[];
extern const u8 gUnk_08706DE8[];
/* Copy 40 palette/tile blocks (0x80 bytes each, the last 0x60) of the pack-list graphics (0x0870xxxx) to dst. */
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
/* kind is 0..3 at every ROM call site, selecting one of the four table pairs.
   Draw one 2x2-cell metatile `idx` at (col, row) of the BG map `map` (tile ids from a per-`kind` table plus
   `tile`, palette bank `pal`) and load its 16-colour palette into bank `pal`. */
void DeckEdit_DrawCardIcon(u8 kind, u8 idx, u16 *map, u8 col, u8 row, u8 pal, u16 tile)
{
    const u16 *tiles;
    const u32 *const *pals;
    if (idx != 0) {
        const u16 *t;
        u16 *p0;
        int x0, y0, x1, y1;
        switch (kind) {
        case 0:
            tiles = gAttributeIconTiles;
            pals = gAttributeIconPals;
            break;
        case 1:
            tiles = gTypeIconTiles;
            pals = gTypeIconPals;
            break;
        case 2:
            tiles = gSpellSubtypeIconTiles;
            pals = gSpellSubtypeIconPals;
            break;
        case 3:
            tiles = gCardKindIconTiles;
            pals = gCardKindIconPals;
            break;
        }
        x0 = col & 0x1F;
        y0 = (row & 0x1F) * 32;
        p0 = &map[x0 + y0];
        t = &tiles[idx];
        *p0 = ((*t + tile) & 0x3FF) | pal << 12;
        x1 = (col + 1) & 0x1F;
        map[x1 + y0] = ((*t + tile + 1) & 0x3FF) | pal << 12;
        y1 = ((row + 1) & 0x1F) * 32;
        map[x0 + y1] = ((*t + tile + 2) & 0x3FF) | pal << 12;
        map[x1 + y1] = ((*t + tile + 3) & 0x3FF) | pal << 12;
        CpuSet(pals[idx], (void *)(0x05000000 + pal * 32), 0x10);
    }
}

#define ROW(pos) (((((pos) + 7) * 8 + gDeckEdit.scroll) & 0xFF) >> 3)
/* Row macro for the default case: masks with the local `mask` (0xFF) instead of a literal (see below). */
#define ROW_M(pos) (((((pos) + 7) * 8 + gDeckEdit.scroll) & mask) >> 3)
/* Draw the three-part card header (attribute/type/level icons) for the list entry `pos` of the card view. */
void DeckEdit_DrawCardIcons(u16 pos)
{
    u16 id;
    int v;
    id = DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]);
    switch (CARD_KIND(id)) {
    case 0x15:
        DeckEdit_DrawCardIcon(0, 9, (u16 *)0x0600C000, 4, ROW(pos), 6, 0x100);
        switch (CARD_KIND(id)) {
        case 0x15:
        case 0x16:
            v = (CARD_STATS(id) & 0xE0000) >> 17;
            break;
        default:
            v = 0;
            break;
        }
        DeckEdit_DrawCardIcon(2, v, (u16 *)0x0600C000, 6, ROW(pos), 7, 0x100);
        break;
    case 0x16:
        DeckEdit_DrawCardIcon(0, 8, (u16 *)0x0600C000, 4, ROW(pos), 6, 0x100);
        switch (CARD_KIND(id)) {
        case 0x15:
        case 0x16:
            v = (CARD_STATS(id) & 0xE0000) >> 17;
            break;
        default:
            v = 0;
            break;
        }
        DeckEdit_DrawCardIcon(2, v, (u16 *)0x0600C000, 6, ROW(pos), 7, 0x100);
        break;
    case 0x17:
        break;
    case 0x18:
        DeckEdit_DrawCardIcon(0, 10, (u16 *)0x0600C000, 4, ROW(pos), 6, 0x100);
        break;
    default: {
        /* FAKEMATCH: with a literal 0xFF, CSE shares one 0xFF pseudo across the first two calls and local-alloc
           gives it sl, spilling pos+7; a block-scope mask variable starts its life earlier, so it loses sl to
           pos+7 and is rematerialised as `movs r3, #0xFF` at each use, as in the ROM. */
        int mask = 0xFF;
        int w;
        DeckEdit_DrawCardIcon(0, CARD_STATS(id) >> 29, (u16 *)0x0600C000, 4, ROW_M(pos), 6, 0x100);
        DeckEdit_DrawCardIcon(1, CARD_KIND(id), (u16 *)0x0600C000, 6, ROW_M(pos), 7, 0x100);
        switch (CARD_NUM(id)) {
        case 0x776:
            w = 3;
            break;
        case 0x777:
        case 0x778:
            w = 1;
            break;
        default:
            switch (CARD_KIND(id)) {
            case 0x16:
                w = 7;
                break;
            case 0x15:
                w = 8;
                break;
            case 0x17:
                w = 9;
                break;
            default:
                w = (CARD_STATS(id) & 0xC0000) >> 18;
                break;
            }
            break;
        }
        /* FAKEMATCH: the r1 clobber makes `w` conflict with r1, so global-alloc puts it in r0 (copied to r1 for
           the call) as in the ROM instead of taking the r1 copy preference. */
        asm volatile("" ::: "r1");
        DeckEdit_DrawCardIcon(3, w, (u16 *)0x0600C000, 8, ROW(pos), 1, 0x100);
        break;
    }
    }
}
extern void DrawNumberTiles(u16 val, u8 n, u8 mode, u16 *dst, u8 col, u8 row, u8 pal, u16 base, u8 m2);
extern void PutMapTileRun(u16 start, u16 *dst, u8 pal, u8 mode, u8 count);
extern void *memset(void *dst, int c, unsigned int n);
extern void *memcpy(void *dst, const void *src, unsigned int n);
extern const u8 gRaStatDigits[4];
static inline u16 CardAtk5AB4(u16 id)
{
    switch ((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return ((((const u32 *)0x08621DE0)[id & 0x7FF] << 14) >> 23) * 10;
    }
}
static inline u16 CardDef5AB4(u16 id)
{
    switch ((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1FF) * 10;
    }
}
/* FAKEMATCH: int-parameter views of PutMapTileRun / DrawNumberTiles. The ROM passes the u16 row and the
 * `0x300 | digit` / ATK values without narrowing them to the callees' u8/u16 parameter types, and the
 * u16 prototype would also reorder the `orr` operands in the digit loops. */
#define sub_080792A0_i ((void (*)(int start, u16 *dst, int pal, int mode, int count))PutMapTileRun)
#define sub_080794E0_i ((void (*)(int val, int n, int mode, u16 *dst, int col, int row, int pal, int base, int m2))DrawNumberTiles)

/* ATK and DEF shown for card `id`: 0 for kinds 21-23, 4000 for the Divine kind 24, else the stat * 10. */
static inline u16 CardAtk_08065AB4(u16 id)
{
    switch (CARD_KIND(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return ((CARD_STATS(id) << 14) >> 23) * 10;
    }
}

static inline u16 CardDef_08065AB4(u16 id)
{
    switch (CARD_KIND(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return (CARD_STATS(id) & 0x1FF) * 10;
    }
}

/* Draw the selected card's ATK/DEF box into the tilemap at (col, row). Kinds 21-23 draw nothing; the
 * Divine kind 24 draws a frame and a fixed 4-digit pattern chosen by card number (0x776-0x778). */
void DeckEdit_DrawAtkDef(u16 *map, u16 col, u16 row)
{
    u8 d[4];
    u16 id;
    u8 i;

    /* FAKEMATCH: three empty insns lengthen map's live range so global-alloc ranks it below the loop
     * temporary (col + 2) & 0x1F; map then gets r8 and that temporary r7 (spilled by reload), as in the ROM. */
    asm("");
    asm("");
    asm("");
    id = DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]);
    switch (CARD_KIND(id)) {
    case 21:
    case 22:
    case 23:
        break;
    case 24:
        map[col + (row << 5)] = 0x198;
        for (i = 0; i <= 1; i++) {
            int r = ((row + i) & 0x1F) << 5;
            map[((col + 1) & 0x1F) + r] = 0x2258;
            map[((col + 2) & 0x1F) + r] = 0x2230;
            map[((col + 3) & 0x1F) + r] = 0x2230;
            map[((col + 4) & 0x1F) + r] = 0x2230;
        }
        map[col + (((row + 1) & 0x1F) << 5)] = 0x199;
        switch (CARD_NUM(id)) {
        case 0x776:
            {
                u8 *p = d;
                memset(p, 0, 4);
                *p = 4;
            }
            for (i = 0; i <= 3; i++) {
                sub_080792A0_i(0x300 | d[i], &map[col + 1 + (row << 5)], 2, 0, 1);
                sub_080792A0_i(0x300 | d[i], &map[col++ + 1 + ((row + 1) << 5)], 2, 0, 1);
            }
            break;
        case 0x777:
            {
                u8 *p = d;
                memset(p, 0, 4);
                *p = 10;
            }
            for (i = 0; i <= 3; i++) {
                sub_080792A0_i(0x300 | d[i], &map[col + 1 + (row << 5)], 2, 0, 1);
                sub_080792A0_i(0x300 | d[i], &map[col++ + 1 + ((row + 1) << 5)], 2, 0, 1);
            }
            break;
        case 0x778:
            memcpy(d, gRaStatDigits, 4);
            for (i = 0; i <= 3; i++) {
                sub_080792A0_i(0x300 | d[i], &map[col + 1 + (row << 5)], 2, 0, 1);
                sub_080792A0_i(0x300 | d[i], &map[col++ + 1 + ((row + 1) << 5)], 2, 0, 1);
            }
            break;
        }
        break;
    default:
        map[col + (row << 5)] = 0x198;
        map[col + (((row + 1) & 0x1F) << 5)] = 0x199;
        sub_080794E0_i(CardAtk_08065AB4(id), 4, 1, map, (col + 4) & 0x1F, row, 2, 0x300, 0);
        sub_080794E0_i(CardDef_08065AB4(id), 4, 1, map, (col + 4) & 0x1F, (row + 1) & 0x1F, 2, 0x300, 0);
        break;
    }
}
#undef sub_080792A0_i
#undef sub_080794E0_i
