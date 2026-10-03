#include "global.h"

struct DeckState {
    u8 pad[0x618];
    u8 anim[6];
    u8 animState;
    u8 pad61F[0x1C1C - 0x61F];
    u8 cursor;
    u8 pad1C1D[0x1C3D - 0x1C1D];
    u8 phase:3; u8 rest3D:5;
    u8 pad1C3E[0x1C48 - 0x1C3E];
    u8 low48:1; u8 mode:4; u8 high48:3;
    u8 b49, b4A, b4B;
    u16 h4C, h4E;
    u8 b50;
    s8 b51;
    u8 b52, b53;
};
extern struct DeckState gDeckEdit;
struct Main { u8 pad[6]; u16 keys; u8 pad8[0x485A - 8]; u8 step; };
extern struct Main gMain;
extern u16 (*const gDeckStatsSteps[])(void);
struct TrunkEntry {
    u8 pad[8];
    u16 owned : 10;
    u16 main : 2;
    u16 side : 2;
    u16 extra : 2;
};
extern u8 gSaveData[];
extern u32 gUnk_02011C20_words[] asm("gSaveData");

u16 DeckStats_ClearState(void)
{
    gDeckEdit.h4E = 0;
    gDeckEdit.h4C = 0;
    gDeckEdit.b49 = 0;
    gDeckEdit.b4A = 0;
    gDeckEdit.b52 = 0;
    gDeckEdit.b53 = 0;
    gDeckEdit.b4B = 0;
    return 1;
}

/* Word arguments are explicitly narrowed, including the default selector
 * return preserved by the ROM. FAKEMATCH: the initialized selector binding
 * retains its original scratch register; no instructions are supplied. */
u32 GetCardCopiesInList(int listWord, int cardWord)
{
    int list = (u16)listWord;
    register int selector asm("r3") = list;
    u32 id;

    id = (u16)cardWord;

    if (list == 1) goto main_count;
    if (list > 1) goto above_one;
    if (list == 0) goto owned_count;
    return list;
above_one:
    if (selector == 2) goto side_count;
    return list;
owned_count:
    {
        u32 base = (u32)gSaveData;
        struct TrunkEntry *entry;

        entry = (struct TrunkEntry *)(id * 4 + base);
        return entry->owned;
    }
main_count:
    {
        u32 base = (u32)gSaveData;
        u8 *entry;
        u32 packed;
        u32 main;

        entry = (u8 *)(id * 4 + base);
        packed = entry[9];
        main = (packed << 28) >> 30;

        return main + (packed >> 6);
    }
side_count:
    {
        u32 base = (u32)gSaveData;
        struct TrunkEntry *entry;

        entry = (struct TrunkEntry *)(id * 4 + base);
        return entry->side;
    }
}

struct CountState {
    u8 pad[0x644];
    union {
        u16 l0[1][0x394];
        struct { u8 pad[0x66A]; u16 cards[1][0x394]; } l1;
        struct { u8 pad[0x70A]; u16 cards[1][0x394]; } l2;
    } lists;
    u8 pad1478[0x1C];
    u16 count[2][3];
    u8 row[3];
};
#define CS_STATE (*(struct CountState *)&gDeckEdit)
u32 GetCardCopiesInList(int, int);
#define CS_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CS_NUM(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CS_KIND(id) ((int)((CS_STATS(id) & 0x1F00000) >> 20))
/* Card frame kind: 3 ritual (0x776), 1 effect (0x777/0x778), 7/8/9 Magic/Trap/Ritual-magic, else the
 * stats monster kind. Direct returns (not `v = ...; break;`) keep jump2 from threading `kind == 0` past the test. */
static inline u8 CS_FrameKind(u16 id)
{
    switch (CS_NUM(id)) {
    case 0x776: return 3;
    case 0x777:
    case 0x778: return 1;
    default:
        switch (CS_KIND(id)) {
        case 0x16: return 7;
        case 0x15: return 8;
        case 0x17: return 9;
        default: return (CS_STATS(id) & 0xC0000) >> 18;
        }
    }
}
#define COUNT_KIND(kindValue) \
    for (i = 0; i < CS_STATE.count[row][list]; i++) { \
        u16 card = cards[i]; \
        switch (CS_KIND(card)) { \
        case 0x15: \
        case 0x16: \
            break; \
        default: \
            if (CS_FrameKind(cards[i]) == (kindValue)) \
                total += GetCardCopiesInList(list, cards[i]); \
            break; \
        } \
    }
#define COUNT_TYPE(typeValue) \
    for (i = 0; i < CS_STATE.count[row][list]; i++) { \
        u16 card = cards[i]; \
        if (CS_KIND(card) == (typeValue)) total += GetCardCopiesInList(list, card); \
    }
/* Sums GetCardCopiesInList copy counts over list `list`'s current row for category 1-6
 * (frame kind 0/1/2, Magic, Trap, frame kind 3). */
u32 DeckStats_CountCategory(u8 list, u8 category)
{
    u16 total = 0;
    u8 row = CS_STATE.row[list];
    u16 *cards;
    u16 i;
    switch (list) {
    case 0: cards = CS_STATE.lists.l0[row]; break;
    case 1: cards = CS_STATE.lists.l1.cards[row]; break;
    case 2: cards = CS_STATE.lists.l2.cards[row]; break;
    }
    switch (category) {
    case 1: COUNT_KIND(0); break;
    case 2: COUNT_KIND(1); break;
    case 3: COUNT_KIND(2); break;
    case 4: COUNT_TYPE(0x16); break;
    case 5: COUNT_TYPE(0x15); break;
    case 6: COUNT_KIND(3); break;
    }
    return total;
}
struct Save { u8 pad[0x20C6]; u16 total, main, side, extra; };
extern struct Save gUnk_02011C20_s asm("gSaveData");
struct CountRow { u16 count, percent; };
extern struct CountRow gScratchBuffer[];
void DeckEdit_BuildCardLists(void);
u32 DeckStats_CountCategory(u8, u8);
int DivFix8(int, int);

/* FAKEMATCH: initialized bindings and empty allocation hints preserve the
 * original count-load and sum order. Case 0/2 share a halfword load; case 1
 * derives the extra-copy address from the main-copy offset. No instructions
 * are supplied by the hints. Caller-saved bindings are dead before calls. */
void DeckStats_Compute(void)
{
    struct CountRow *rows = gScratchBuffer;
    int value, percent;
    u16 quotient;
    u32 last;
    u8 *cursor;
    DeckEdit_BuildCardLists();
    {
        u16 *count;
        switch (gDeckEdit.cursor) {
        case 0: {
            register u32 base asm("r0") = (u32)&gUnk_02011C20_s;
            register u32 off asm("r2") = 0x20C6;
            asm("" : : "r"(base), "r"(off));
            base += off;
            count = (u16 *)base;
            goto read_count;
        }
        case 1: {
            register u32 base asm("r0") = (u32)&gUnk_02011C20_s;
            register u32 off asm("r2") = 0x20C8;
            u16 *m = (u16 *)(base + off);
            off += 4;
            base += off;
            rows[6].count = *m + *(u16 *)base;
            break;
        }
        case 2: {
            u32 base = (u32)&gUnk_02011C20_s;
            u32 off = 0x20CA;
            base += off;
            count = (u16 *)base;
        }
        read_count:
            rows[6].count = *count;
            break;
        }
    }
    rows[6].percent = 100;
    {
        register u32 base asm("r4") = (u32)&gDeckEdit;
        register u32 off asm("r2") = 0x1C1C;
        asm("" : : "r"(off));
        base += off;
        cursor = (u8 *)base;
    }
    rows[0].count = DeckStats_CountCategory(*cursor, 1);
    rows[1].count = DeckStats_CountCategory(*cursor, 2);
    rows[2].count = DeckStats_CountCategory(*cursor, 3);
    rows[3].count = DeckStats_CountCategory(*cursor, 4);
    rows[4].count = DeckStats_CountCategory(*cursor, 5);
    last = DeckStats_CountCategory(*cursor, 6);
    rows[5].count = last;
    {
        register int a asm("r1") = rows[1].count;
        register int b asm("r2") = rows[0].count;
        int c;
        asm("" : : "r"(a), "r"(b));
        a += b;
        b = rows[2].count;
        asm("" : : "r"(b));
        b += a;
        c = rows[3].count;
        asm("" : : "r"(c));
        c += b;
        a = rows[4].count;
        asm("" : : "r"(a));
        a += c;
        last += a;
        rows[6].count = last;
    }
    {
        int input = rows[0].count;
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[0].percent = percent;
    }
    {
        int input = rows[1].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[1].percent = percent;
    }
    {
        int input = rows[2].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[2].percent = percent;
    }
    {
        int input = rows[3].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[3].percent = percent;
    }
    {
        int input = rows[4].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[4].percent = percent;
    }
    {
        int input = rows[5].count;
        asm("" : : "r"(input));
        quotient = DivFix8(input * 4, rows[6].count);
        value = quotient * 25;
        percent = value >> 8;
        if ((value & 0xFF) > 0x7F) percent++;
        rows[5].percent = percent;
    }
}

extern const u8 gDeckEditDigitSprites[];
void DrawNumberSprites();
/* FAKEMATCH: the do-while(0) wrapper changes agbcc's loop-invariant hoisting
   so the loop constants land in the same registers as the ROM. */
void DeckStats_DrawNumbers(void)
{
    u8 i;
    do {
        for (i = 0; i <= 5; i++) {
            struct CountRow *row = &gScratchBuffer[i];
            DrawNumberSprites(row->count, 4, 1, 0xA8, i * 16 + 0x24, gDeckEditDigitSprites, 1, 8, 0, 0, 0, &gDeckEdit);
            DrawNumberSprites(row->percent, 3, 1, 0xC8, i * 16 + 0x24, gDeckEditDigitSprites, 1, 8, 0, 0, 0, &gDeckEdit);
        }
    } while (0);
    DrawNumberSprites(gScratchBuffer[6].count, 4, 1, 0x98, 0x8C, gDeckEditDigitSprites, 1, 8, 0, 0, 0, &gDeckEdit);
}
void CpuFastSet(const void *, void *, u32);
void CpuSet(const void *, void *, u32);
void CopyMapRectAddOffset(u16 *, u16 *, u8, u8, u8, u8, u8);
void CopyMapRect(void *, void *, u8, u8);
/* ASM crop helper decodes its word-valued scalar arguments on entry. */
void CropMapBlock(u16 *, int, int, int, void *, int, int, int, int, int);
void CopyTileSheetTo2D(u8 *, u8 *, u16);
struct Fade;
void FadeStart(u8, s16, u8, struct Fade *);
extern const u8 gDeckStatsPatternMap[], gDeckStatsPanelMap[], gDeckStatsBg1Map[], gListFilterSortPageMap[], gDeckStatsListIconMap[];
extern const u8 gDeckStatsBgTiles[], gDeckStatsLabelTiles[], gDeckEditObjTiles[], gDeckStatsBgPal[], gDeckEditObjPal[];
extern const u16 gUnk_08623326;
extern const u32 gCardStats[];
extern u8 gUnk_0201F770;
#define REG16(off) (*(volatile u16 *)(0x04000000 + (off)))
void DeckStats_Compute(void);
/* FAKEMATCH: two initialized bindings retain the shared tile source and
 * fade-speed store allocation. Three empty constraints preserve the original
 * discarded card-type computation and its range tests without instructions. */
int DeckStats_Init(void)
{
    u32 zero0 = 0, zero1;
    u16 x, y;
    register const u8 *tiles asm("r10");
    CpuFastSet(&zero0, (void *)0x06000000, 0x01004000);
    zero1 = 0;
    CpuFastSet(&zero1, (void *)0x06010000, 0x01002000);
    for (y = 0; y <= 3; y++) {
        for (x = 0; x <= 3; x++)
            CopyMapRectAddOffset((u16 *)gDeckStatsPatternMap, (void *)(0x0600F000 + (x * 8 + y * 256) * 2), 8, 8, 8, 0, 0);
    }
    CopyMapRect((void *)gDeckStatsPanelMap, (void *)0x0600E000, 30, 20);
    CopyMapRect((void *)gDeckStatsBg1Map, (void *)0x0600D000, 30, 20);
    CopyMapRect((void *)gListFilterSortPageMap, (void *)0x0600C000, 30, 20);
    tiles = gDeckStatsListIconMap;
    CropMapBlock((u16 *)tiles, 0, gDeckEdit.cursor * 5, 7, (void *)0x0600D000, 20, 0, 7, 5, 0);
    CropMapBlock((u16 *)tiles, 0, gDeckEdit.cursor * 5, 7, (void *)0x0600C000, 20, 0, 7, 5, 0);
    CpuFastSet(gDeckStatsBgTiles, (void *)0x06000000, 0x800);
    CpuFastSet(gDeckStatsLabelTiles, (void *)0x06002000, 0x800);
    CopyTileSheetTo2D((u8 *)gDeckEditObjTiles, (void *)0x06010000, 16);
    CpuFastSet(gDeckStatsBgPal, (void *)0x05000000, 0x80);
    CpuSet(gDeckEditObjPal, (void *)0x05000200, 0x100);
    REG16(8) = 0x1800;
    REG16(10) = 0x1A01;
    REG16(12) = 0x1C02;
    REG16(14) = 0x1E02;
    {
        int step = -0x180;
        u32 base = (u32)&gDeckEdit;
        u32 off = 0x618;

        FadeStart(0, step, 0, (void *)(base + off));
    }
    REG16(0x10) = 0; REG16(0x12) = 0;
    REG16(0x14) = 0; REG16(0x16) = 0;
    REG16(0x18) = 0; REG16(0x1A) = 0;
    REG16(0x1C) = 0; REG16(0x1E) = 0;
    {
        register int speed asm("r0") = 16;
        gUnk_0201F770 = speed;
    }
    REG16(0) = 0x1A00;
    DeckStats_Compute();
    {
        u32 card = 0x439;
        int number;
        int threshold = 0x776;
        asm("" : "+r"(card));
        number = gUnk_08623326;
        asm("" : "+r"(threshold));
        if (number == threshold) goto done_type;
        if (number < threshold) goto compute_type;
        if (number <= 0x778) goto done_type;
    compute_type:
        {
            u32 off = card * 4;
            u32 value = (*(const u32 *)((u32)gCardStats + off) & 0x1F00000) >> 20;
            asm("" : : "r"(value));
        }
    done_type: ;
    }
    return 1;
}

void DeckStats_DrawNumbers(void);
struct Animation {
    u8 pad[6]; u8 state;
    u8 pad7[0x1634 - 7];
    u16 scrollX, scrollY;
    u8 pad1638, close, pad163A, busy;
};
extern struct Animation gUnk_0201E138;
extern u8 gUnk_0201F770;
void FadeTick(void *);
void PlaySE(u16);
void SetBldAlpha(u16);
struct Fade;
void FadeStart(u8, s16, u8, struct Fade *);
void OamListFlush(void *);
void OamListClear(void *);

u16 DeckStats_Update(void)
{
    u32 keys = gMain.keys & 0x3FF;
    FadeTick(&gUnk_0201E138);
    DeckStats_DrawNumbers();
    gUnk_0201E138.scrollX += 0x80;
    gUnk_0201E138.scrollY += 0x80;
    REG16(0x1C) = gUnk_0201E138.scrollX >> 8;
    REG16(0x1E) = gUnk_0201E138.scrollY >> 8;
    if (gUnk_0201E138.state == 0 && gUnk_0201E138.busy == 0) {
        switch (keys) {
        case 1:
        case 2:
            gUnk_0201E138.close = 1;
            PlaySE(2);
            break;
        }
    }
    if (gDeckEdit.animState == 2) {
        gDeckEdit.mode = 4;
        gDeckEdit.phase = 3;
        return 1;
    }
    if (gDeckEdit.animState == 3) {
        REG16(0x50) = 0x3F44;
        SetBldAlpha(16);
        REG16(0) |= 0x400;
        gDeckEdit.animState = 0;
        gDeckEdit.b51 = -1;
        gDeckEdit.b52 = 1;
    }
    if (gDeckEdit.b51 != 0) {
        gDeckEdit.b50 += gDeckEdit.b51;
        if (gDeckEdit.b50 == 8)
            gDeckEdit.b51 = 0;
        if (gDeckEdit.b50 == 16) {
            REG16(0) &= 0xFBFF;
            FadeStart(0, 0x180, 0, (struct Fade *)((u8 *)&gDeckEdit + 0x618));
            gDeckEdit.b51 = 0;
            gDeckEdit.b52 = 0;
        }
        SetBldAlpha(gDeckEdit.b50);
    }
    OamListFlush(&gDeckEdit);
    OamListClear(&gDeckEdit);
    return 0;
}
u16 DeckEdit_RunStatistics(void)
{
    if (gDeckStatsSteps[gMain.step]) {
        if (gDeckStatsSteps[gMain.step]())
            gMain.step++;
        return 0;
    }
    return 1;
}
struct InitState {
    u8 pad[0x620]; u16 col[3];
    u8 pad626[0x630 - 0x626];
    u16 h630, h632; u8 b634, b635;
    u16 h636, h638, h63A, h63C, h63E;
    u8 pad640[0x1494 - 0x640];
    u16 count[2][3]; u8 row[3];
    u8 pad14A3[0x1710 - 0x14A3];
    u8 active:1; u8 rest:7;
    u8 pad1711[0x18AC - 0x1711]; u16 h18AC;
    u8 pad18AE[0x1BB0 - 0x18AE]; u16 slide[2];
    u8 dirty4:1; u8 rest4:7; u8 b1BB5;
    u8 dirty6:1; u8 rest6:7; u8 b1BB7;
    u8 pad1BB8[0x1C1C - 0x1BB8]; u8 cursor, previous;
};
struct InitMain { u8 pad[0x40E]; u16 flags; };
extern u8 gUnk_0201F6D8[];
struct PanelFlags { u8 active:1; u8 mode:4; u8 rest:3; u8 pad[7]; };
void MemClear16(void *, u32);
void Ease_Init(int, int, int, void *);
void ClearKatakanaFlag(void *);
void ObjAffineInit(void *);
void DeckEdit_SetListCard(u16, u8, u8, u16);
void DeckEdit_CountSideDeckMonsters(void);
void DeckEdit_CalcScrollBar(u16, u16, u16 *);
void DeckEdit_ResetFrameSlots(void *);
void DeckEdit_ResetCardMove(void *);
#define INIT (*(struct InitState *)&gDeckEdit)
struct TrunkEntryInit { u16 owned : 10; u8 f1 : 2; u8 f2 : 2; u8 f3 : 2; };
struct TrunkInit { u8 pad0[8]; struct TrunkEntryInit e[1]; };
#define TRUNK_INIT ((struct TrunkInit *)gSaveData)
/* Integer-address indexing, as in the twin TradeCardSelect_Init (deck_edit_prohibit). */
#define CARD_NUMBER_INIT(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
int DeckEdit_Init(void)
{
    u16 i, j;
    struct PanelFlags *panel;
    MemClear16(&INIT, 0x1C5C);
    ((struct InitMain *)&gMain)->flags = 1;
    REG16(0x12) = 0; REG16(0x10) = 0;
    REG16(0x16) = 0; REG16(0x14) = 0;
    REG16(0x1A) = 0; REG16(0x18) = 0;
    REG16(0x1E) = 0; REG16(0x1C) = 0;
    REG16(0x28) = 0; REG16(0x2A) = 0;
    REG16(0x3C) = 0; REG16(0x3E) = 0;
    REG16(0) &= 0xE0FF;
    OamListClear(&INIT);
    INIT.h632 = 0; INIT.h630 = 0;
    INIT.h63A = 0; INIT.h638 = 0;
    INIT.h63E = 0; INIT.h63C = 0;
    for (i = 0; i <= 2; i++) {
        INIT.col[i] = 0; INIT.row[i] = 0;
        for (j = 0; j <= 1; j++) INIT.count[j][i] = 0;
    }
    INIT.previous = 0; INIT.cursor = 0;
    INIT.b634 = 0; INIT.b635 = 0;
    INIT.active = 0; INIT.h18AC = 0xFC00;
    Ease_Init(0, 0, 0, (u8 *)&INIT + 0x628);
    ClearKatakanaFlag((u8 *)&INIT + 0x640);
    ObjAffineInit((u8 *)&INIT + 0x18B0);
    for (i = 1; i <= 0x334 && CARD_NUMBER_INIT(i) != 0xFFFF; i++) {
        if ((u16)(CARD_NUMBER_INIT(i) - 0x780) > 0x4F) {
            if (TRUNK_INIT->e[i].owned)
                DeckEdit_SetListCard(i, 0, INIT.row[0], INIT.count[INIT.row[0]][0]++);
            if (TRUNK_INIT->e[i].f1 || TRUNK_INIT->e[i].f3)
                DeckEdit_SetListCard(i, 1, INIT.row[1], INIT.count[INIT.row[1]][1]++);
            if (TRUNK_INIT->e[i].f2)
                DeckEdit_SetListCard(i, 2, INIT.row[2], INIT.count[INIT.row[2]][2]++);
        }
    }
    DeckEdit_CountSideDeckMonsters();
    DeckEdit_CalcScrollBar(INIT.count[INIT.row[INIT.cursor]][INIT.cursor], INIT.col[INIT.cursor], INIT.slide);
    INIT.dirty4 = 1; INIT.dirty6 = 1;
    if (INIT.count[INIT.row[INIT.cursor]][INIT.cursor] > 5)
        INIT.b1BB7 = INIT.b1BB5 = 1;
    else INIT.b1BB7 = INIT.b1BB5 = 0;
    DeckEdit_ResetFrameSlots(gUnk_0201F6D8);
    DeckEdit_ResetCardMove(gUnk_0201F6D8 + 0x68);
    panel = (struct PanelFlags *)(gUnk_0201F6D8 + 0x7C);
    panel->active = 0; panel->mode = 0;
    return 1;
}
