/*
 * deck_edit_cards (0x08068180-0x08069284): the deck-edit card lists, the list builder, the card
 * comparators and quicksort, and the side-panel / name-index drawing of the list screens
 * (wiki/functions/deck-edit-cards-c.md).
 *
 * The Deck Edit screen and the pickers built from the same code (Campaign side-deck swap, Card
 * Trading, Prohibition) share the state block gDeckEdit (0x0201DB20) and its three card lists:
 * list 0 is the trunk, list 1 the main deck, list 2 the side deck, each with a full row 0 and a
 * filtered/sorted row 1. This unit holds the list accessors (DeckEdit_GetListCard,
 * DeckEdit_SetListCard, DeckEdit_GetActiveListRow), the builder that refills the lists from the
 * trunk counts (DeckEdit_BuildCardLists, filtered by enum CardListMode), the copy counter of the
 * card under the cursor (DeckEdit_GetSelectedCardCopies), the five card comparators and
 * QuickSortS16 that the List Filter sort runs on, and the side-panel drawing
 * (DeckEdit_DrawCardCounts, DeckEdit_UpdateNameIndexLetters, DeckEdit_DrawNameIndexTab).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_NUMBER_TOKEN_FIRST;
                                       * the tables below are read through their integer addresses,
                                       * the form this unit matches with (include/card_data.h) */
#include "constants/card_stats.h"   /* CARD_STATS_* masks and shifts, CARD_STATS_POINTS_SCALE, enum CardType */

/* gCardStats (0x08621DE0) read through its address. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
/* The card's type (gCardStats bits 20-24, enum CardType). */
#define CARD_TYPE_OF(id) ((int)CARD_STATS_TYPE(CARD_STATS(id)))
/* ---- Local views kept for matching (build/readability/HEADERS.md) ----
 * include/deck_edit.h declares the canonical struct DeckEdit, struct DeckEditListRow and this
 * unit's prototypes, but it cannot be included here: it pulls in sprite.h and text.h, whose
 * DrawNumberSprites, OamListAddSpriteGroup and RenderStringToTiles prototypes have other
 * parameter widths than the matched calls in this unit, and DeckEdit_DrawPortraitTilemap's
 * tileBase argument is a u32 at the call below. The views here keep deck_edit.h's and save.h's
 * field names and offsets; build/readability/issues/deck_edit_cards.md has the details. */

/* gDeckEdit (0x0201DB20, struct DeckEdit) as this unit reads it. The three card lists start at
 * +0x644, +0xCAE and +0xD4E (0x728 bytes per row) and are modelled as a union so that every list
 * access is a field of the one symbol (the ROM shares the base register across the list switch). */
struct DeckEditView {
    u8 unk0[0x620];
    u16 listPos[3];                 /* +0x620: selected card index per list */
    u8 unk626[0x63E - 0x626];
    u16 bg0Vofs;                    /* +0x63E */
    u8 unk640[0x644 - 0x640];
    union {
        u16 trunk[1][0x394];                                    /* +0x644: list 0 (trunk) */
        struct { u8 pad[0xCAE - 0x644]; u16 cards[1][0x394]; } deck;  /* +0xCAE: list 1 (main deck) */
        struct { u8 pad[0xD4E - 0x644]; u16 cards[1][0x394]; } side;  /* +0xD4E: list 2 (side deck) */
    } lists;
    u8 unk1478[0x1C];               /* (agbcc rounds the union member structs up to 4 bytes) */
    u16 listCount[2][3];            /* +0x1494: [row][list] number of cards */
    u8 listRow[3];                  /* +0x14A0: row shown per list (0 full, 1 filtered/sorted) */
    u8 unk14A3[0x1712 - 0x14A3];
    u16 sideMonsterCount[2];        /* +0x1712: [row] monster copies in the side deck */
    u8 unk1716[0x1BB0 - 0x1716];
    u16 scrollThumbLen;             /* +0x1BB0: scrollBar.thumbLen (8.8 px) */
    u16 scrollThumbPos;             /* +0x1BB2: scrollBar.thumbPos (8.8 px) */
    u8 unk1BB4[0x1C1C - 0x1BB4];
    u8 curList;                     /* +0x1C1C: list shown (0 trunk, 1 main deck, 2 side deck) */
    u8 unk1C1D[0x1C42 - 0x1C1D];
    u8 sort[3];                     /* +0x1C42: commandMenu.sort, the List Filter sort of each list */
    u8 unk1C45[0x1C58 - 0x1C45];
    s16 nameTabTimer;               /* +0x1C58: frames the name-index tab stays visible (u16 in
                                       deck_edit.h; decremented as an s16 here) */
};
extern struct DeckEditView gDeckEdit;   /* 0x0201DB20 */

/* Complete two-row views of the overlapping card lists (same bases as the union above). */
struct TrunkListView { u8 pad[0x644]; u16 cards[2][0x394]; };
struct DeckListView { u8 pad[0xCAE]; u16 cards[2][0x394]; };
struct SideListView { u8 pad[0xD4E]; u16 cards[2][0x394]; };

/* Call views whose parameter widths differ from the headers (see the note above). */
extern void RenderStringToTiles(void *str, void *tiles, u32 fg, u32 bg, u32 unused);
extern u16 *OamListAddSpriteGroup(const void *tmpls, int layer, int count, int x, int y, int mode,
                                  int priority, int sheetX, int sheetY, int format, int attr0Flags, int list);
extern const u8 gNameIndexTabSprite[];      /* 0x081A70F4 */
extern const u8 gNameIndexLettersSprite[];  /* 0x080874A0 */
extern u16 gNameIndexCache asm("gUnk_0201F775"); /* 0x0201F775: gDeckEdit.nameIndexCache read as a u16
                                                    (the odd address makes the cache never hit) */

/* gSaveData's trunk entries read through the save base as u32 words (the ROM's access form):
 * entry `id` is the word at gSaveData + 8 + id * 4 (struct TrunkEntry in include/save.h). */
struct TrunkCountView { u8 pad8[8]; u16 count:10; };        /* bits 0-9: copies in the trunk */
struct TrunkCopiesView { u8 pad9[9]; u8 copiesByte; };      /* entry byte 1: bits 2-3 deckCopies,
                                                               bits 4-5 sideCopies, bits 6-7 fusionCopies */
extern u32 gSaveDataWords[] asm("gSaveData");   /* 0x02011C20: gSaveData as u32 words */
#define TRUNK_COUNT(id) (((struct TrunkCountView *)(gSaveDataWords + (id)))->count)
#define TRUNK_COPIES(id) (((struct TrunkCopiesView *)(gSaveDataWords + (id)))->copiesByte)

/* gDeckEdit.listRow and the two save-deck sizes, reached through their own symbols as the ROM
 * loads them. */
extern u8 gDeckEditListRow[] asm("gUnk_0201EFC0");          /* 0x0201EFC0: &gDeckEdit.listRow */
extern u16 gSaveDeckSize[] asm("gUnk_02013CE8");            /* 0x02013CE8: &gSaveData.deckSize */
extern u16 gSaveFusionDeckSize[] asm("gUnk_02013CEC");      /* 0x02013CEC: &gSaveData.fusionDeckSize */
extern const u8 gDeckEditDigitSprites[];                    /* 0x081A6EB4: digit sprite descriptors */
extern void DrawNumberSprites(int value, int numDigits, int mode, int x, int y,
                              const void *digitTemplates, int unused, int spacing, int sheetX,
                              int sheetY, int priority, int list);
extern u16 DeckEdit_GetListCard(u8 list, u8 row, u16 index);
extern u16 DeckEdit_IsFusionMonster(u16 cardId);
extern void DeckEdit_DrawPortraitTilemap(u8 col, u8 row, u8 page, u8 wrap, u32 tileBase);

#define DRAW_COUNT(value, numDigits, mode, x, y) \
    DrawNumberSprites(value, numDigits, mode, x, y, gDeckEditDigitSprites, 1, 8, 0, 0, 0, (int)state)
#define CUR_CARD() DeckEdit_GetListCard(list, gDeckEditListRow[list], state->listPos[list])
/* u16 at byte offset `off` of gSaveData; the same symbol as TRUNK_COUNT/TRUNK_COPIES, so CSE shares its base. */
#define SAVE_U16(off) (*(u16 *)((u8 *)gSaveDataWords + (off)))
#define SAVE_TRUNK_SIZE SAVE_U16(0x20C6)        /* gSaveData.trunkSize */
#define SAVE_SIDE_DECK_SIZE SAVE_U16(0x20CA)    /* gSaveData.sideDeckSize */
/* Draw the side panel's card-count numbers for list `list`: the cursor card's trunk count and its
 * deck/fusion/side copy counts (the trunk entry's 2-bit fields), the deck and side sizes, and per
 * list the totals or split counts. */
void DeckEdit_DrawCardCounts(u8 list)
{
    struct DeckEditView *state = &gDeckEdit;
    DRAW_COUNT(TRUNK_COUNT(CUR_CARD()), 2, 1, 0x30, 0x88);
    DRAW_COUNT(((u32)(TRUNK_COPIES(CUR_CARD()) << 28) >> 30) + (TRUNK_COPIES(CUR_CARD()) >> 6), 2, 1, 0x68, 0x77);
    DRAW_COUNT(gSaveDeckSize[0] + gSaveFusionDeckSize[0], 2, 1, 0x68, 0x88);
    CUR_CARD(); /* result unused in the ROM too */
    DRAW_COUNT((u32)(TRUNK_COPIES(CUR_CARD()) << 26) >> 30, 2, 1, 0xA8, 0x77);
    DRAW_COUNT(SAVE_SIDE_DECK_SIZE, 2, 1, 0xA8, 0x88);
    switch (list) {
    case 0:
        DRAW_COUNT(SAVE_TRUNK_SIZE + gSaveDeckSize[0] + SAVE_SIDE_DECK_SIZE + gSaveFusionDeckSize[0], 4, 1, 0xE0, 0x88);
        break;
    case 1:
        DRAW_COUNT(gSaveDeckSize[0], 2, 1, 0xE0, 0x78);
        DRAW_COUNT(gSaveFusionDeckSize[0], 2, 1, 0xE0, 0x8B);
        break;
    case 2:
        DRAW_COUNT(state->sideMonsterCount[state->listRow[list]], 2, 1, 0xE0, 0x79);
        DRAW_COUNT(SAVE_SIDE_DECK_SIZE - state->sideMonsterCount[state->listRow[list]], 2, 1, 0xE0, 0x8A);
        break;
    }
}
#undef DRAW_COUNT
#undef CUR_CARD
#undef SAVE_U16
#undef SAVE_TRUNK_SIZE
#undef SAVE_SIDE_DECK_SIZE
#define ST gDeckEdit
/* Copies of the cursor card held for the current list, read from the card's trunk entry: the
 * trunk count for list 0, the deck copies (fusion-deck copies for a Fusion Monster) for list 1,
 * the side-deck copies for list 2. Editor rows are 0..1 and each selected column belongs to its
 * list; DeckEdit_IsFusionMonster is pure, so the nested selectors remain in 0..2. */
u32 DeckEdit_GetSelectedCardCopies(void)
{
    struct DeckEditView *state = &ST;
    u8 *curList = &state->curList;
    switch (*curList) {
    case 0: {
        u32 row = ST.listRow[0];
        u32 *saveWords;
        if (ST.listCount[row][0] == 0)
            return 0;
        saveWords = gSaveDataWords;
        return ((struct TrunkCountView *)(saveWords + ((struct TrunkListView *)&ST)->cards[row][ST.listPos[0]]))->count;
    }
    case 1: {
        u32 rowOffset = 0x14A1;             /* &ST.listRow[1] as a byte offset */
        u8 *rows = (u8 *)((u32)&ST + rowOffset);
        u16 pos = ST.listPos[1];
        if (DeckEdit_IsFusionMonster(((struct DeckListView *)&ST)->cards[*rows][pos])) {
            u32 *saveWords;
            u32 id;
            u8 cur = *curList;
            u32 twice = cur * 2;
            u16 pos;
            u8 row;
            row = ST.listRow[cur];
            if (ST.listCount[row][cur] == 0)
                return 0;
            saveWords = gSaveDataWords;
            pos = *(u16 *)((u32)&ST.listPos + twice);
            switch (cur) {
            case 0:
                id = ((struct TrunkListView *)&ST)->cards[row][pos];
                break;
            case 1:
                id = ((struct DeckListView *)&ST)->cards[row][pos];
                break;
            case 2:
                id = ((struct SideListView *)&ST)->cards[row][pos];
                break;
            }
            return ((struct TrunkCopiesView *)(saveWords + (u16)id))->copiesByte >> 6;
        } else {
            u32 *saveWords;
            u32 id;
            u8 cur = *curList;
            u32 twice = cur * 2;
            u16 pos;
            u8 row;
            row = ST.listRow[cur];
            if (ST.listCount[row][cur] == 0)
                return 0;
            saveWords = gSaveDataWords;
            pos = *(u16 *)((u32)&ST.listPos + twice);
            switch (cur) {
            case 0:
                id = ((struct TrunkListView *)&ST)->cards[row][pos];
                break;
            case 1:
                id = ((struct DeckListView *)&ST)->cards[row][pos];
                break;
            case 2:
                id = ((struct SideListView *)&ST)->cards[row][pos];
                break;
            }
            {
                /* FAKEMATCH: retain the selected ID while forming its trunk offset. */
                register u32 copy asm("r1") = id;
                register u32 offset asm("r0");
                asm("" : : "r"(copy));
                offset = (u16)copy * 4;
                return ((u32)((struct TrunkCopiesView *)((u32)saveWords + offset))->copiesByte << 28) >> 30;
            }
        }
    }
    case 2: {
        /* FAKEMATCH: staged state addresses and the live row-selector offset. */
        register u32 count asm("r0");
        u32 co;
        u32 *saveWords;
        register u32 ro asm("r2") = 0x14A2;     /* &ST.listRow[2] as a byte offset */
        u32 row;
        count = (u32)&ST;
        count += ro;
        row = *(u8 *)count;
        asm("" : : "r"(ro));
        count = (u32)&ST;
        count += row * 6;
        co = 0x1498;                            /* &ST.listCount[row][2] relative to the row base */
        if (*(u16 *)(count + co) == 0)
            return 0;
        saveWords = gSaveDataWords;
        {
            u32 off = 0x624;                    /* &ST.listPos[2] as a byte offset */
            u32 pos;
            count = (u32)&ST;
            count += off;
            pos = *(u16 *)count;
            return ((u32)((struct TrunkCopiesView *)(saveWords + ((struct SideListView *)&ST)->cards[row][pos]))->copiesByte << 26) >> 30;
        }
    }
    }
    return 0;
}

#undef ST
/* Pointer to the row (0x728 bytes) of card list `list` selected by the per-list row index listRow[list]. */
u16 *DeckEdit_GetActiveListRow(int list)
{
    switch ((u8)list) {
    case 0:
        return gDeckEdit.lists.trunk[gDeckEdit.listRow[0]];
    case 1:
        return gDeckEdit.lists.deck.cards[gDeckEdit.listRow[1]];
    case 2:
        return gDeckEdit.lists.side.cards[gDeckEdit.listRow[2]];
    }
}
/* Deck-edit list builder: rebuild row 0 of the three card lists (trunk / deck / side)
 * from the trunk counts, filtered by the list mode, then compact row 1 of every list whose row
 * selector is 1. Views local to this function: the trunk entries at 0x02011C28 (4 bytes per card
 * id) and the list state at 0x0201DB20 (rows of 821 + 80 + 15 ids, 0x728 bytes each). */
struct TrunkEntryView { u16 count : 10; u16 deckCopies : 2; u16 sideCopies : 2; u16 fusionCopies : 2; u16 unk2; };
struct SaveTrunkView { u8 unk0[8]; struct TrunkEntryView trunk[0x335]; };
extern u8 gSaveData[];   /* 0x02011C20 */
#define gSaveTrunk (*(struct SaveTrunkView *)gSaveData)
struct ListRowView { u16 trunk[821]; u16 deck[80]; u16 side[15]; };
struct DeckEditBuildView {
    u8 unk0[0x644];
    struct ListRowView rows[2];   /* +0x644 */
    u16 listCount[2][3];          /* +0x1494 */
    u8 listRow[3];                /* +0x14A0 */
    u8 unk14A3[0x1C5A - 0x14A3];
    u8 swapFlags;                 /* +0x1C5A: bit 5 = sideSwapMode (fusion copies stay out of list 1) */
};
#define gListsWork (*(struct DeckEditBuildView *)&gDeckEdit)
struct MainModeView { u8 unk0[0x4874]; u8 cardListMode : 2; u8 unk4874_2 : 6; };
extern u8 gMain[];   /* 0x03000040 */
#define gMainMode (*(struct MainModeView *)gMain)
#define W gListsWork
#define CARD_NUMBER(i) (((const u16 *)0x08622AB4)[(i) & CARD_ID_MASK])   /* gCardIdToNumber */
#define OWNED_COUNT(i) (gSaveTrunk.trunk[i].count)
#define DECK_COPIES(i) (gSaveTrunk.trunk[i].deckCopies)
#define FUSION_COPIES(i) (gSaveTrunk.trunk[i].fusionCopies)
#define SIDE_COPIES(i) (gSaveTrunk.trunk[i].sideCopies)
static inline u16 ReadListEntry(u8 list, u8 row, u16 index)
{
    switch (list) {
    case 0: return W.rows[row].trunk[index];
    case 1: return W.rows[row].deck[index];
    case 2: return W.rows[row].side[index];
    }
}
static inline void WriteListEntry(u16 cardId, u8 list, u8 row, u16 index)
{
    switch (list) {
    case 0: W.rows[row].trunk[index] = cardId; break;
    case 1: W.rows[row].deck[index] = cardId; break;
    case 2: W.rows[row].side[index] = cardId; break;
    }
}
void DeckEdit_BuildCardLists(void)
{
    u16 i;
    for (i = 0; i <= 2; i++)
        W.listCount[0][i] = 0;
    switch (gMainMode.cardListMode) {
    case 1:   /* Card Trading: card numbers 1210-1899 and 1920-1999 are hidden */
        for (i = 1; i <= 0x334; i++) {
            u16 number = CARD_NUMBER(i);
            if ((u16)(number - 0x4BA) > 0x315 || (u16)(number - 0x76C) <= 0x13) {
                if (OWNED_COUNT(i))
                    WriteListEntry(i, 0, 0, W.listCount[0][0]++);
                if ((s32)((u32)W.swapFlags << 26) < 0) { /* sideSwapMode: main-deck copies only */
                    if (DECK_COPIES(i))
                        WriteListEntry(i, 1, 0, W.listCount[0][1]++);
                } else if (DECK_COPIES(i) || FUSION_COPIES(i)) {
                    WriteListEntry(i, 1, 0, W.listCount[0][1]++);
                }
                if (SIDE_COPIES(i))
                    WriteListEntry(i, 2, 0, W.listCount[0][2]++);
            }
        }
        break;
    case 0:   /* Deck Edit: token numbers 1920-1999 are hidden */
        for (i = 1; i <= 0x334; i++) {
            if ((u16)(CARD_NUMBER(i) - CARD_NUMBER_TOKEN_FIRST) > 0x4F) {
                if (OWNED_COUNT(i))
                    WriteListEntry(i, 0, 0, W.listCount[0][0]++);
                if ((s32)((u32)W.swapFlags << 26) < 0) {
                    if (DECK_COPIES(i))
                        WriteListEntry(i, 1, 0, W.listCount[0][1]++);
                } else if (DECK_COPIES(i) || FUSION_COPIES(i)) {
                    WriteListEntry(i, 1, 0, W.listCount[0][1]++);
                }
                if (SIDE_COPIES(i))
                    WriteListEntry(i, 2, 0, W.listCount[0][2]++);
            }
        }
        break;
    case 2:   /* Prohibition picker: list 0 only, card numbers 1900-1999 excluded */
        for (i = 1; i <= 0x334; i++) {
            if ((u16)(CARD_NUMBER(i) - 0x76C) > 0x63)
                WriteListEntry(i, 0, 0, W.listCount[0][0]++);
        }
        break;
    }
    for (i = 0; i <= 2; i++) {
        if (W.listRow[i] == 1) {
            u16 n = 0;
            u16 j;
            for (j = 0; j < W.listCount[1][i]; j++) {
                switch (i) {
                case 0:
                    if (OWNED_COUNT(ReadListEntry(i, 1, j)))
                        WriteListEntry(ReadListEntry(i, 1, j), i, 1, n++);
                    break;
                case 1:
                    if (DECK_COPIES(ReadListEntry(i, 1, j)) || FUSION_COPIES(ReadListEntry(i, 1, j)))
                        WriteListEntry(ReadListEntry(i, 1, j), i, 1, n++);
                    break;
                case 2:
                    if (SIDE_COPIES(ReadListEntry(i, 1, j)))
                        WriteListEntry(ReadListEntry(i, 1, j), i, 1, n++);
                    break;
                }
            }
            W.listCount[1][i] = n;
        }
    }
}
#undef W
#undef CARD_NUMBER
#undef OWNED_COUNT
#undef DECK_COPIES
#undef FUSION_COPIES
#undef SIDE_COPIES
#undef gSaveTrunk
#undef gListsWork
#undef gMainMode
/* Load the graphic of the currently selected card into VRAM 0x06012FE0 if it changed. The
 * selected list is 0, 1 or 2, as in the adjacent list accessors. */
void DeckEdit_UpdateNameIndexLetters(void)
{
    u32 id;
    u16 *cache;
    const u8 *nameRecord;
    int cached;
    u8 row = gDeckEdit.listRow[gDeckEdit.curList];
    u16 index = gDeckEdit.listPos[gDeckEdit.curList];
    u16 firstHalf;
    u16 buf[2];
    switch (gDeckEdit.curList) {
    case 0:
        id = gDeckEdit.lists.trunk[row][index];
        break;
    case 1:
        id = gDeckEdit.lists.deck.cards[row][index];
        break;
    case 2:
        id = gDeckEdit.lists.side.cards[row][index];
        break;
    }
    nameRecord = ((const u8 *)0x0822C720) + (u16)id * CARD_NAME_SIZE;   /* gCardNames + id * 0x40 */
    cache = &gNameIndexCache;
    firstHalf = *(u16 *)nameRecord;
    cached = *cache;
    if (firstHalf != cached) {
        *cache = firstHalf;
        buf[0] = firstHalf;
        buf[1] = 0;
        RenderStringToTiles(buf, (void *)0x06012FE0, 1, 0, 0);
    }
}

void DeckEdit_DrawNameIndexTab(void)
{
    int y;
    if (gDeckEdit.sort[gDeckEdit.curList] == 0) {
        u16 count = gDeckEdit.listCount[*((u8 *)&gDeckEdit + 0x14A0 + gDeckEdit.curList)][gDeckEdit.curList];
        if (count <= 3)
            y = 0x30;
        else
            y = (u32)(gDeckEdit.scrollThumbPos + (gDeckEdit.scrollThumbLen >> 1)) >> 8;
        if (gDeckEdit.nameTabTimer != 0) {
            gDeckEdit.nameTabTimer--;
            OamListAddSpriteGroup(gNameIndexTabSprite, 0, 1, 0xD8, y + 0x1FD, 4, 0, 0, 0, 0, 0, (int)&gDeckEdit);
            OamListAddSpriteGroup(gNameIndexLettersSprite, 0, 1, 0xDB, y + 1, 4, 0, 0, 0, 0, 0, (int)&gDeckEdit);
        }
    }
}
/* Read entry `index` of row `row` of card list `list` (0..2) of the list state: 0x728 bytes per row. */
u16 DeckEdit_GetListCard(u8 list, u8 row, u16 index)
{
    switch (list) {
    case 0:
        return ((struct TrunkListView *)&gDeckEdit)->cards[row][index];
    case 1:
        return ((struct DeckListView *)&gDeckEdit)->cards[row][index];
    case 2:
        return ((struct SideListView *)&gDeckEdit)->cards[row][index];
    }
}
/* Write `cardId` into the same cell DeckEdit_GetListCard reads. */
void DeckEdit_SetListCard(u16 cardId, u8 list, u8 row, u16 index)
{
    switch (list) {
    case 0:
        gDeckEdit.lists.trunk[row][index] = cardId;
        break;
    case 1:
        gDeckEdit.lists.deck.cards[row][index] = cardId;
        break;
    case 2:
        gDeckEdit.lists.side.cards[row][index] = cardId;
        break;
    }
}
/* Draw the 9x10 card portrait map on BG3 with tile base 0. */
void DeckEdit_PlaceCardArt(u8 col, u8 row, u8 page, u8 wrap)
{
    DeckEdit_DrawPortraitTilemap(col, row, page, wrap, 0);
}
/* 1 if card a is worth more than card b (monster ATK * 10 in stats bits 9..17; 0 for Trap/Magic/Ticket, 4000 for Divine). */
int CompareCardsByAtk(int aCard, int bCard)
{
    u16 bId = bCard;
    u16 aId = aCard;
    int valueA;
    int valueB;
    switch (CARD_TYPE_OF(aId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        valueA = 0;
        break;
    case CARD_TYPE_DIVINE:
        valueA = 4000;
        break;
    default:
        valueA = CARD_STATS_ATK(CARD_STATS(aId)) * CARD_STATS_POINTS_SCALE;
        break;
    }
    switch (CARD_TYPE_OF(bId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        valueB = 0;
        break;
    case CARD_TYPE_DIVINE:
        valueB = 4000;
        break;
    default:
        valueB = CARD_STATS_ATK(CARD_STATS(bId)) * CARD_STATS_POINTS_SCALE;
        break;
    }
    return valueA - valueB > 0;
}
/* 1 if card a is worth more than card b by the low 9 stat bits * 10 (same type rules as CompareCardsByAtk). */
int CompareCardsByDef(int aCard, int bCard)
{
    u16 bId = bCard;
    u16 aId = aCard;
    int valueA;
    int valueB;
    switch (CARD_TYPE_OF(aId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        valueA = 0;
        break;
    case CARD_TYPE_DIVINE:
        valueA = 4000;
        break;
    default:
        valueA = (CARD_STATS(aId) & CARD_STATS_DEF_MASK) * CARD_STATS_POINTS_SCALE;
        break;
    }
    switch (CARD_TYPE_OF(bId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        valueB = 0;
        break;
    case CARD_TYPE_DIVINE:
        valueB = 4000;
        break;
    default:
        valueB = (CARD_STATS(bId) & CARD_STATS_DEF_MASK) * CARD_STATS_POINTS_SCALE;
        break;
    }
    return valueA - valueB > 0;
}
/* 1 if card a has a lower type than card b. */
u32 CompareCardsByType(int aId, int bId)
{
    unsigned long long base = 0x08621DE0; /* FAKEMATCH (decomp-permuter): a 64-bit temp stops agbcc from CSEing the table address */
    int typeA = (int)((((const u32 *)(u32)base)[aId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT);
    return (u32)(typeA - CARD_TYPE_OF(bId)) >> 31;
}

/* 1 if card a has a lower attribute (stats >> 29) than card b. */
u32 CompareCardsByAttribute(int aId, int bId)
{
    unsigned long long base = 0x08621DE0; /* FAKEMATCH (decomp-permuter): see CompareCardsByType */
    return (u32)(((((const u32 *)(u32)base)[aId & CARD_ID_MASK]) >> CARD_STATS_ATTR_SHIFT) - CARD_STATS_ATTR(CARD_STATS(bId))) >> 31;
}

/* 1 if card a has a higher level (stats >> 25 & 0xF; 10 for Divine, 0 for Trap/Magic/Ticket) than card b. */
int CompareCardsByLevel(int aCard, int bCard)
{
    u16 bId = bCard;
    u16 aId = aCard;
    int valueA;
    int valueB;
    switch (CARD_TYPE_OF(aId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        valueA = 0;
        break;
    case CARD_TYPE_DIVINE:
        valueA = 10;
        break;
    default:
        valueA = CARD_STATS_LEVEL(CARD_STATS(aId));
        break;
    }
    switch (CARD_TYPE_OF(bId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        valueB = 0;
        break;
    case CARD_TYPE_DIVINE:
        valueB = 10;
        break;
    default:
        valueB = CARD_STATS_LEVEL(CARD_STATS(bId));
        break;
    }
    return valueA - valueB > 0;
}
struct SortRange { s16 lo; s16 hi; };
extern struct SortRange gScratchBuffer[];   /* 0x02030000 */
/* The original swap macro has no braces, so under an unbraced `if` only `t = a` is conditional.
   The median-of-three below relies on that (the pivot is not a true median). */
#define SORT_SWAP(a, b) t = a; a = b; b = t
/* Quicksort of n s16 values in arr (insertion sort for ranges of <= 20); cmp(a, b) != 0 means a orders before b.
   The pending (lo, hi) ranges live in an explicit stack at 0x02030000. */
void QuickSortS16(int n, s16 *arr, u16 (*cmp)(s16, s16))
{
    struct SortRange *stack = gScratchBuffer;
    s16 lo, hi, i, j, x;
    s16 top;
    u16 t;

    stack[0].lo = 0;
    stack[0].hi = n - 1;
    top = 1;
    do {
        top--;
        lo = stack[top].lo;
        hi = stack[top].hi;
        if (lo < hi) {
            if (hi - lo > 20) {
                s16 y, z;
                i = lo - 1;
                j = hi + 1;
                x = arr[(lo + hi) / 2];
                y = arr[lo];
                z = arr[hi];
                if (x < y) SORT_SWAP(x, y);
                if (x > z) {
                    SORT_SWAP(x, z);
                    if (x > y) SORT_SWAP(x, y);
                }
                for (;;) {
                    while (cmp(arr[++i], x))
                        ;
                    while (cmp(x, arr[--j]))
                        ;
                    if (i >= j)
                        break;
                    SORT_SWAP(arr[i], arr[j]);
                }
                stack[top].lo = j + 1;
                stack[top].hi = hi;
                top++;
                stack[top].lo = lo;
                stack[top].hi = i - 1;
                top++;
            } else {
                for (i = lo + 1; i <= hi; i++) {
                    x = arr[i];
                    j = i - 1;
                    while (j >= lo && cmp(x, arr[j])) {
                        arr[j + 1] = arr[j];
                        j--;
                    }
                    arr[j + 1] = x;
                }
            }
        }
    } while (top > 0);
}
