#include "global.h"
#include "gba.h"

extern int DivFix8(int a, int b);
extern u16 *OamListAddSpriteGroup(const void *a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l);
extern const void *gCardFrameSprites[];
extern u16 gFrameSlotY[];
extern int MulFix8(int a, int b);
extern u16 gFrameSlotScale[];
extern u8 GetCardFrameIndex(u16 id);
extern const u8 gUnk_08706F28[];
extern const u8 gUnk_087070A8[];
extern const u8 gUnk_08707228[];
extern const u8 gUnk_087076A8[];
extern const u8 gUnk_087073A8[];
extern const u8 gUnk_08707528[];
extern const u8 gUnk_08707828[];
extern const u8 gUnk_087078A8[];
extern const u8 gUnk_08707928[];
extern const u8 gUnk_08707AA8[];
extern const u8 gUnk_087079A8[];
extern const u8 gUnk_08707A28[];
/* Scene state at 0x0201DB20 (see wiki code-08064af0). */
struct PageState {
    u8 pad0[0x620];
    u16 arr620[15];                 /* +0x620 */
    u16 scroll;                     /* +0x63E */
    u8 pad640[0x14A0 - 0x640];
    u8 arr14A0[0x1C1C - 0x14A0];    /* +0x14A0 */
    u8 cursor;                      /* +0x1C1C */
};
extern struct PageState gDeckEdit;
extern u8 gCardFrameAnimIds[];
extern void PlaySE(u16 id);
extern void Ease_Start(int a, int b, int c, void *d);
extern void Ease_Tick(void *p);
extern u16 IsBelowCardCopyLimit(u16 id);
extern void RemoveCardFromTrunk(u16 id);
extern void RemoveCardFromSavedFusionDeck(u16 id);
extern void RemoveCardFromSavedDeck(u16 id);
extern void RemoveCardFromSavedSideDeck(u16 id);
extern void AddCardToTrunk(u16 id);
extern void AddCardToSavedFusionDeck(u16 id);
extern void AddCardToSavedDeck(u16 id);
extern void AddCardToSavedSideDeck(u16 id);
extern void DeckEdit_BuildCardLists(void);
extern void DeckEdit_StartListSlide(u32 a);
extern void DeckEdit_CountSideDeckMonsters(void);
extern void DeckEdit_BeginCardMove(u8 *p);
extern u16 DeckEdit_IsFusionMonster(u16 id);
struct SaveM {
    u8 pad0[0x20C8];
    u16 f20C8;
    u16 f20CA;
    u16 f20CC;
};
extern struct SaveM gUnk_02011C20_s asm("gSaveData");
extern const u8 gCardMoveSprite[];
extern u16 gDeckEditEaseCurve[];
struct St2 {
    u8 pad0[0x1726];
    u8 f1726[0x1BB8 - 0x1726];      /* 20-byte cells; byte 0 is a "touched" flag */
    u8 f1BB8;
    u8 pad1BB9[0x1C1C - 0x1BB9];
    u8 cursor;
};
extern struct St2 gUnk_0201DB20_b asm("gDeckEdit");
struct Cnt { u8 pad8[8]; u16 n : 10; };
struct Cnt9 { u8 pad9[9]; u8 b9; };
#define TRUNK ((u8 *)0x02011C20)
#define TRUNK_N(t, id) (((struct Cnt *)((t) + (u16)(id) * 4))->n)
#define TRUNK_B9(t, id) (((struct Cnt9 *)((t) + (u16)(id) * 4))->b9)
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 col);
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUM(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_KIND(id) ((int)((CARD_STATS(id) & 0x1F00000) >> 20))

extern void OamListAddSprite(int a, int b, int c, int d, int e, int f, int g, int h, int i, int j, int k, int l, int m);
extern void ObjAffineApply(void *p);
extern const u8 gScrollArrowTiles[];
struct Ctx {
    u8 pad0[0x18B0];
    u16 f18B0;
    u16 f18B2;
};
extern struct Ctx gUnk_0201DB20_c asm("gDeckEdit");
struct Pair {
    u16 a;
    u16 b;
    u8 flagA : 1;
    u8 padA : 7;
    u8 idxA;
    u8 flagB : 1;
    u8 padB : 7;
    u8 idxB;
};
struct Ent16 { u8 b[16]; };
struct XY { u16 x, y; };
extern struct XY gUnk_08087488_s[] asm("gCardMoveTargets");


/* Draw the card's level stars (tile 0x19A) into map, `perRow` per row starting at (col, row). */
void DeckEdit_DrawLevelStars(u8 *map, u16 col, u16 row, u8 perRow)
{
    u16 col0 = col;
    u8 i = 0;
    u32 id;
    const u32 *st;
    int kind;
    id = DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor]);
    st = &((const u32 *)0x08621DE0)[(id << 21) >> 21];
    kind = (*st & 0x1F00000) >> 20;
    for (;;) {
        u8 n = i;
        u32 cnt;
        int idx;
        i++;
        switch (kind) {
        case 0x15:
        case 0x16:
        case 0x17:
            cnt = 0;
            break;
        case 0x18:
            cnt = 10;
            break;
        default:
            cnt = (*st & 0x1E000000) >> 25;
            break;
        }
        if (!(n < cnt))
            break;
        /* FAKEMATCH (permuter): the mask goes through the dead `id` */
        id = 0x1F;
        idx = col & id;
        idx += (row & 0x1F) * 32;
        *(u16 *)(map + idx * 2) = 0x19A;
        col++;
        if (i == perRow) {
            col = col0;
            row++;
        }
    }
}

void DeckEdit_CalcScrollBar(u16 a, u16 b, u16 *out)
{
    u16 n = a - 1;
    u16 x;
    int y;
    if (n != 0) {
        x = DivFix8(0xC0, n);
        y = DivFix8((0x5800 - x) >> 8, n);
    } else {
        x = 0xC0;
        y = 0x57;
    }
    out[1] = y * b;
    out[0] = x;
}
void DeckEdit_DrawScrollBar(u16 a, u16 b, struct Pair *p)
{
    int x = p->a >> 8;
    int y = p->b >> 8;
    int yy;
    if (b == a + 1 && y + x <= 0x57)
        y++;
    if (x != 0 && b > 3) {
        int r5 = x * 8;
        int z;
        int t = MulFix8(0x10, 0x100 - r5);
        z = y - 4;
        z -= t;
        OamListAddSprite(0, 0x89, 0xE4, z & 0xFF, 8, 0x20, 4, 7, 0, 0x300, 0, 0, (int)&gUnk_0201DB20_c);
        gUnk_0201DB20_c.f18B2 = r5 + 0x10;
        ObjAffineApply(&gUnk_0201DB20_c.f18B0);
    } else if (b <= 3) {
        OamListAddSprite(0, 0x89, 0xE4, 0xC, 8, 0x20, 4, 7, 0, 0x300, 0, 0, (int)&gUnk_0201DB20_c);
        OamListAddSprite(0, 0x89, 0xE4, 0x24, 8, 0x20, 4, 7, 0, 0x300, 0, 0, (int)&gUnk_0201DB20_c);
        gUnk_0201DB20_c.f18B2 = 0x200;
        ObjAffineApply(&gUnk_0201DB20_c.f18B0);
        y = 0;
        x = 0x58;
    }
    OamListAddSprite(0, 0xF, 0xE8, (y + 8) & 0xFF, 8, 8, 4, 7, 0, 0, 0, 0, (int)&gUnk_0201DB20_c);
    yy = y + 0xC;
    OamListAddSprite(0, 0x2F, 0xE8, (yy + x) & 0xFF, 8, 8, 4, 7, 0, 0, 0, 0, (int)&gUnk_0201DB20_c);
    if (p->flagA) {
        p->flagA = 0;
        *(u16 *)0x0600E03A = 0x5000 | gScrollArrowTiles[p->idxA];
    }
    if (p->flagB) {
        p->flagB = 0;
        *(u16 *)0x0600E37A = 0x5000 | gScrollArrowTiles[p->idxB + 3];
    }
}
/* Copy 12 graphics blocks (0x0870xxxx) into the tile area at dst. */
void sub_08066164(u8 *dst)
{
    u8 *p;
    CpuSet(gUnk_08706F28, dst, 0xC0);
    CpuSet(gUnk_087070A8, dst + 0x180, 0xC0);
    CpuSet(gUnk_08707228, dst + 0x300, 0xC0);
    CpuSet(gUnk_087076A8, dst + 0x480, 0xC0);
    CpuSet(gUnk_087073A8, dst + 0x600, 0xC0);
    CpuSet(gUnk_08707528, dst + 0x780, 0xC0);
    CpuSet(gUnk_08707828, dst + 0x900, 0x40);
    CpuSet(gUnk_087078A8, dst + 0x980, 0x40);
    CpuSet(gUnk_08707928, p = dst + 0xA00, 0x40);
    CpuSet(gUnk_08707AA8, p, 0x40);
    CpuSet(gUnk_087079A8, dst + 0xA80, 0x40);
    CpuSet(gUnk_08707A28, dst + 0xB00, 0x40);
}
void DeckEdit_ResetFrameSlots(u8 *p)
{
    u8 i;
    for (i = 0; i <= 5; i++)
        *(u8 *)((u32)p + (i << 4) + 0xC) = 0;
    p[0] = 5;
}
void DeckEdit_TweenFrameSlots(u8 a, u8 b, u8 c, u8 *d, u8 *e)
{
    u8 m = a;
    u8 i;
    switch (c) {
    case 1:
        m = 6 - m;
    case 2:
        a = 6 - a;
        break;
    default:
        return;
    }
    switch (b) {
    case 1:
        for (i = 0; i <= 5; i++) {
            u8 *q = e + i * 16;
            if (q[0xC] != 0) {
                u16 *o;
                *(u16 *)(q + 6) = *(u16 *)(q + 8) + MulFix8(*(s16 *)(q + 0xA), gDeckEditEaseCurve[a]);
                o = (u16 *)(d + (i + 1) * 24);
                o[0] = o[1] = *(u16 *)(q + 0xE) + *(u16 *)(q + 0x10) * m;
            }
        }
        break;
    case 2:
        for (i = 0; i <= 5; i++) {
            u8 *q = e + i * 16;
            if (q[0xC] != 0) {
                s16 *o;
                *(u16 *)(q + 6) = *(u16 *)(q + 8) + MulFix8(*(s16 *)(q + 0xA), gDeckEditEaseCurve[a]);
                o = (s16 *)(d + (i + 1) * 24);
                o[0] = o[1] = *(u16 *)(q + 0xE) + *(u16 *)(q + 0x10) * m;
                {
                    int t = *o;
                    *(u16 *)(q + 0xE) = t;
                }
            }
        }
        e[e[0] * 16 + 0xC] = 0;
        break;
    }
}

/* Card frame kind (0..9) for card `id`; same logic as the tail of DeckEdit_DrawCursorRowName. */
u8 GetCardFrameIndex(u16 id)
{
    switch (CARD_NUM(id)) {
    case 0x76D:
    case 0x76E:
    case 0x76F:
        return 0;
    case 0x776:
        return 3;
    case 0x777:
    case 0x778:
        return 1;
    default:
        switch (CARD_KIND(id)) {
        case 0x15:
            return 5;
        case 0x16:
            return 4;
        }
        switch (CARD_NUM(id)) {
        case 0x776:
            return 3;
        case 0x777:
        case 0x778:
            return 1;
        default:
            switch (CARD_KIND(id)) {
            case 0x16:
                return 7;
            case 0x15:
                return 8;
            case 0x17:
                return 9;
            default:
                return (CARD_STATS(id) & 0xC0000) >> 18;
            }
        }
    }
}
void DeckEdit_ScrollFrameSlots(u8 dir, u16 x, u8 unused, u8 *s, u8 *arr)
{
    u8 n = s[0];
    u8 i;
    u8 *e;
    u16 v;
    int m;
    u8 *o;
    switch (dir) {
    case 1:
    {
        if (x != 0xFFFF) {
            u8 *t = (u8 *)&((struct Ent16 *)s)[n];
            t[4] = 0;
            t[0xC] = 1;
            *(u16 *)(t + 6) = gFrameSlotY[0];
            t[5] = GetCardFrameIndex(x);
            v = gFrameSlotScale[0];
            *(u16 *)(t + 0xE) = v;
            m = n + 1;
            t[0x12] = m;
            o = arr + m * 24;
            *(u16 *)(o + 2) = v;
            *(u16 *)o = v;
        }
        if (s[0] != 0)
            s[0] = s[0] - 1;
        else
            s[0] = 5;
        for (i = 0; i <= 5; i++) {
            e = s + i * 16;
            if (e[0xC] != 0) {
                e[4] = e[4] + 1;
                *(u16 *)(e + 0xA) = gFrameSlotY[e[4]] - *(u16 *)(e + 6);
                *(u16 *)(e + 8) = *(u16 *)(e + 6);
                *(u16 *)(e + 0x10) = (gFrameSlotScale[e[4]] - *(u16 *)(e + 0xE)) / 6;
            }
        }
    break;
    }
    case 2:
    {
        if (x != 0xFFFF) {
            u8 *t = (u8 *)&((struct Ent16 *)s)[n];
            t[4] = 6;
            t[0xC] = 1;
            *(u16 *)(t + 6) = gFrameSlotY[6];
            t[5] = GetCardFrameIndex(x);
            v = gFrameSlotScale[6];
            *(u16 *)(t + 0xE) = v;
            m = n + 1;
            t[0x12] = m;
            o = arr + m * 24;
            *(u16 *)(o + 2) = v;
            *(u16 *)o = v;
        }
        s[0] = (s[0] + 1) % 6;
        for (i = 0; i <= 5; i++) {
            e = s + i * 16;
            if (e[0xC] != 0) {
                e[4] = e[4] - 1;
                *(u16 *)(e + 0xA) = *(u16 *)(e + 6) - gFrameSlotY[e[4]];
                *(u16 *)(e + 8) = *(u16 *)(e + 6) - *(u16 *)(e + 0xA);
                *(u16 *)(e + 0x10) = (gFrameSlotScale[e[4]] - *(u16 *)(e + 0xE)) / 6;
            }
        }
        break;
    }
    }
}
void DeckEdit_DrawFrameSlots(u8 *p, int arg)
{
    u8 i;
    for (i = 0; i <= 5; i++) {
        u8 *e = (u8 *)(i * 16 + (u32)p);
        if (e[8] != 0) {
            u16 *o = OamListAddSpriteGroup(gCardFrameSprites[e[1]], 5, 1, -3, *(s16 *)(e + 2), 4, 0, 0, 0, 0, 0, arg);
            o[0] |= 0x100;
            o[1] |= e[0xE] << 9;
        }
    }
}
void DeckEdit_InitFrameSlot(u8 slot, u16 x, u8 kind, u8 *base, u8 *arr)
{
    u8 *e;
    u16 v;
    int n;
    u8 *o;
    u8 r = GetCardFrameIndex(x);
    e = base + slot * 16;
    e[5] = r;
    e[0xC] = 1;
    e[4] = kind;
    *(u16 *)(e + 6) = gFrameSlotY[kind];
    v = gFrameSlotScale[kind];
    *(u16 *)(e + 0xE) = v;
    n = slot + 1;
    e[0x12] = n;
    o = arr + n * 24;
    *(u16 *)(o + 2) = v;
    *(u16 *)o = v;
}
void DeckEdit_ResetCardMove(u8 *p)
{
    *p = 0;
}
void DeckEdit_StartCardMove(u8 a, u8 b, u8 *s)
{
    s[0] = 1;
    s[0xC] = a;
    s[0xD] = b;
    *(u16 *)(s + 0xE) = gUnk_08087488_s[b].x + 3;
    *(u16 *)(s + 0x10) = gUnk_08087488_s[b].y - 0x28;
}
/* The deck editor keeps cursor in 0..2; each selector initializes copies.
 * Materialize the trunk symbol before selecting its card, and use a separate
 * ring base for the final flag write. No compiler hints are needed. */
void DeckEdit_BeginCardMove(u8 *p)
{
    u32 copies;
    u8 *trunk;
    u32 id;
    int frameKind;
    u32 stateBase = (u32)&gDeckEdit;
    u32 categoryIndex;
    u8 *category;

    categoryIndex = gCardFrameAnimIds[p[0xC]];
    category = (u8 *)(stateBase + categoryIndex * 20);
    category[0x1726] = 1;
    switch (gDeckEdit.cursor) {
    case 0:
        trunk = (u8 *)&gUnk_02011C20_s;

        copies = TRUNK_N(trunk, DeckEdit_GetListCard(0, gDeckEdit.arr14A0[0], gDeckEdit.arr620[0]));
        break;
    case 1:
        id = (u16)DeckEdit_GetListCard(1, gDeckEdit.arr14A0[1], gDeckEdit.arr620[1]);
        switch (CARD_NUM(id)) {
        case 0x776:
            frameKind = 3;
            break;
        case 0x777:
        case 0x778:
            frameKind = 1;
            break;
        default:
            switch (CARD_KIND(id)) {
            case 0x16:
                frameKind = 7;
                break;
            case 0x15:
                frameKind = 8;
                break;
            case 0x17:
                frameKind = 9;
                break;
            default:
                frameKind = (CARD_STATS(id) & 0xC0000) >> 18;
                break;
            }
            break;
        }
        if (frameKind == 2) {
            trunk = (u8 *)&gUnk_02011C20_s;

            copies = TRUNK_B9(trunk, DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor])) >> 6;
        } else {
            trunk = (u8 *)&gUnk_02011C20_s;

            copies = ((u32)TRUNK_B9(trunk, DeckEdit_GetListCard(gDeckEdit.cursor, gDeckEdit.arr14A0[gDeckEdit.cursor], gDeckEdit.arr620[gDeckEdit.cursor])) << 28) >> 30;
        }
        break;
    case 2:
        trunk = (u8 *)&gUnk_02011C20_s;

        copies = ((u32)TRUNK_B9(trunk, DeckEdit_GetListCard(2, gDeckEdit.arr14A0[2], gDeckEdit.arr620[2])) << 26) >> 30;
        break;
    }
    if (copies == 1) {
        struct St2 *ring = &gUnk_0201DB20_b;
        u32 address = ((ring->f1BB8 + 3) % 6) * 16;
        address += (u32)ring;
        address += 0x1BC4;
        *(u8 *)address = 0;
    }
    p[0]++;
}

/* 1 if card `id` (not a monster-frame kind 0x15..0x17) has frame kind 2, else 0. */
u16 DeckEdit_IsFusionMonster(u16 id)
{
    int v;
    switch (CARD_KIND(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
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
    if (v == 2)
        return 1;
    return 0;
}
struct DE699C {
    u8 pad0[0x620];
    u16 col[3];                 /* +0x620 */
    u8 pad626[0x1494 - 0x626];
    u16 count[2][3];            /* +0x1494 */
    u8 row[3];                  /* +0x14A0 */
    u8 pad14A3[0x1724 - 0x14A3];
    struct { u8 pre[2]; s8 b0; u8 b1; u8 pad4[16]; } cells[63];  /* +0x1724, 20 bytes each; b0 at +0x1726 */
    u8 pad1C10[0x1C1C - 0x1C10];
    u8 cursor;                  /* +0x1C1C */
    u8 pad1C1D[0x1C3C - 0x1C1D];
    u8 menu0;                   /* +0x1C3C */
    u8 phase : 3;               /* +0x1C3D */
    u8 rest : 5;
    u8 b1C3E;
    u8 markA[3];                /* +0x1C3F */
    u8 markB[3];                /* +0x1C42 */
};
struct FB699C { u8 f : 8; };
#define DE (*(struct DE699C *)&gDeckEdit)
#define CARDC() DeckEdit_GetListCard(DE.cursor, DE.row[DE.cursor], DE.col[DE.cursor])
/* Deck-edit add/remove step machine (s[0] = step): 1 checks the per-category limits and either starts the move
 * (DeckEdit_BeginCardMove + SE 1) or cancels (SE 3); 2 claims the category cell; 3 animates the card sprite; 4 marks cell
 * s[0xD] + 10; 5 applies the change and refreshes the lists.  Cells are 20 bytes from +0x1726; agbcc aligns the
 * struct to 4, hence the 2-byte `pre`.  The `|= 0xFF` through a u8 bitfield keeps the ROM's dead ldrb; trunk
 * pointers are scoped per case so &row gets r5 and trunk r4. */
void DeckEdit_UpdateCardMove(u8 *s)
{
    u16 r;
    u32 cnt;
    u32 id;
    int frameKind;

    switch (s[0]) {
    case 0:
        return;
    case 1:
        switch (s[0xD]) {
        case 0:
            DeckEdit_BeginCardMove(s);
            PlaySE(1);
            break;
        case 1:
            r = DeckEdit_IsFusionMonster(CARDC());
            if (r != 0) {
                if (gUnk_02011C20_s.f20CC < 0x14) {
                    switch (DE.cursor) {
                    case 0:
                        if (IsBelowCardCopyLimit(DeckEdit_GetListCard(0, DE.row[0], DE.col[0]))) {
                            DeckEdit_BeginCardMove(s);
                            PlaySE(1);
                        } else {
                            s[0] = 0;
                            PlaySE(3);
                        }
                        break;
                    case 1:
                    case 2:
                        DeckEdit_BeginCardMove(s);
                        PlaySE(1);
                        break;
                    }
                } else {
                    s[0] = 0;
                    PlaySE(3);
                }
            } else {
                if (gUnk_02011C20_s.f20C8 < 0x3C) {
                    switch (DE.cursor) {
                    case 0:
                        if (IsBelowCardCopyLimit(DeckEdit_GetListCard(0, DE.row[0], DE.col[0]))) {
                            DeckEdit_BeginCardMove(s);
                            PlaySE(1);
                        } else {
                            s[0] = 0;
                            PlaySE(3);
                        }
                        break;
                    case 1:
                    case 2:
                        DeckEdit_BeginCardMove(s);
                        PlaySE(1);
                        break;
                    }
                } else {
                    s[0] = 0;
                    PlaySE(3);
                }
            }
            break;
        case 2:
            if (gUnk_02011C20_s.f20CA < 0xF) {
                switch (DE.cursor) {
                case 0:
                    if (IsBelowCardCopyLimit(DeckEdit_GetListCard(0, DE.row[0], DE.col[0]))) {
                        DeckEdit_BeginCardMove(s);
                        PlaySE(1);
                    } else {
                        s[0] = 0;
                        PlaySE(3);
                    }
                    break;
                case 1:
                case 2:
                    DeckEdit_BeginCardMove(s);
                    PlaySE(1);
                    break;
                }
            } else {
                s[0] = 0;
                PlaySE(3);
            }
            break;
        }
    case 2:
        {
            u32 stateBase = (u32)&DE;
            u32 k = gCardFrameAnimIds[s[0xC]];
            s8 *cell = (s8 *)(stateBase + k * 20) + 0x1726;
            if (*cell != 0)
                return;
            ((struct FB699C *)cell)->f |= 0xFF;
        }
        Ease_Start(0, 6, 1, s + 4);
        s[0]++;
    case 3: {
        int x, y;
        x = (MulFix8(*(s16 *)(s + 0xE) << 8, gDeckEditEaseCurve[*(s16 *)(s + 6)]) >> 8) - 3;
        y = (MulFix8(*(s16 *)(s + 0x10) << 8, gDeckEditEaseCurve[*(s16 *)(s + 6)]) >> 8) + 0x28;
        OamListAddSpriteGroup(gCardMoveSprite, 0, 1, x, y, 4, 0, 0, 0, 0, 0, (int)&DE);
        Ease_Tick(s + 4);
        if (s[4] != 2)
            return;
        s[0]++;
        break;
    }
    case 4:
        DE.cells[s[0xD] + 10].b0 = 1;
        DE.cells[s[0xD] + 10].b1 = 0;
        s[0]++;
        break;
    case 5:
        if (DE.cells[s[0xD] + 10].b0 != 0)
            return;
        s[0] = 0;
        switch (DE.cursor) {
        case 0:
            RemoveCardFromTrunk(CARDC());
            break;
        case 1:
            if (DeckEdit_IsFusionMonster(CARDC()))
                RemoveCardFromSavedFusionDeck(CARDC());
            else
                RemoveCardFromSavedDeck(CARDC());
            break;
        case 2:
            RemoveCardFromSavedSideDeck(CARDC());
            break;
        }
        switch (s[0xD]) {
        case 0:
            AddCardToTrunk(CARDC());
            break;
        case 1:
            if (DeckEdit_IsFusionMonster(CARDC()))
                AddCardToSavedFusionDeck(CARDC());
            else
                AddCardToSavedDeck(CARDC());
            break;
        case 2:
            AddCardToSavedSideDeck(CARDC());
            break;
        }
        DE.row[s[0xD]] = 0;
        DE.markA[s[0xD]] = 0;
        DE.markB[s[0xD]] = 0;
        DE.phase = 3;
        switch (DE.cursor) {
        case 0:
            {
                u8 *trunk = (u8 *)&gUnk_02011C20_s;
                cnt = TRUNK_N(trunk, DeckEdit_GetListCard(0, DE.row[0], DE.col[0]));
            }
            break;
        case 1:
            id = (u16)DeckEdit_GetListCard(1, DE.row[1], DE.col[1]);
            switch (CARD_NUM(id)) {
            case 0x776:
                frameKind = 3;
                break;
            case 0x777:
            case 0x778:
                frameKind = 1;
                break;
            default:
                switch (CARD_KIND(id)) {
                case 0x16:
                    frameKind = 7;
                    break;
                case 0x15:
                    frameKind = 8;
                    break;
                case 0x17:
                    frameKind = 9;
                    break;
                default:
                    frameKind = (CARD_STATS(id) & 0xC0000) >> 18;
                    break;
                }
                break;
            }
            if (frameKind == 2) {
                u8 *trunk = (u8 *)&gUnk_02011C20_s;
                cnt = TRUNK_B9(trunk, CARDC()) >> 6;
            } else {
                u8 *trunk = (u8 *)&gUnk_02011C20_s;
                cnt = ((u32)TRUNK_B9(trunk, CARDC()) << 28) >> 30;
            }
            break;
        case 2:
            {
                u8 *trunk = (u8 *)&gUnk_02011C20_s;
                cnt = ((u32)TRUNK_B9(trunk, DeckEdit_GetListCard(2, DE.row[2], DE.col[2])) << 26) >> 30;
            }
            break;
        }
        DeckEdit_BuildCardLists();
        if (DE.count[DE.row[DE.cursor]][DE.cursor] == DE.col[DE.cursor]) {
            if (DE.count[DE.row[DE.cursor]][DE.cursor] == 0)
                DE.col[DE.cursor] = 0;
            else
                DE.col[DE.cursor] = DE.count[DE.row[DE.cursor]][DE.cursor] - 1;
        }
        if (cnt == 0)
            DeckEdit_StartListSlide(2);
        DeckEdit_CountSideDeckMonsters();
        break;
    default:
        s[0] = 0;
        break;
    }
}
#undef DE
#undef CARDC
