#include "global.h"
#include "main.h"     /* struct Main, gMain */
#include "duel.h"     /* DuelCard/DuelPlayer/DuelState, gDuel, gDuelPlayers */
#include "duel_ui.h"  /* gDuelScreen (struct DuelScreen) */

/*
 * Duel list queries (graveyard / deck / hand searches by card number), duel event
 * wrappers around DuelCmd_Push, and LP-change helpers.  See wiki/functions/code-08019554.md.
 *
 * Uses the shared duel structs.  Canonical DuelCard (duel.h) places the drawn-card flag at
 * bit 20, but this unit's ROM code tests bit 17 (DrawCards), so a unit-local word view is
 * kept for that one test (CARD17 below).
 */

#define CARD(word) (*(struct DuelCard *)&(word))
#define CARD17(word) (*(struct DuelCard17 *)&(word))

/* Unit-local view: canonical DuelCard has flag20:1 at bit 20; DrawCards tests bit 17. */
struct DuelCard17 {
    u32 id : 12;        /* card ID (index into gCardStats / gCardIdToNumber); 0 = none */
    u32 owner : 1;
    u32 unk13 : 4;
    u32 unk17 : 1;      /* tested when drawing card 762 */
    u32 unk18 : 14;
};

#define PLAYER(p) (gDuelPlayers[(p) & 1])

#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
extern const u16 gCardNumberToId[];

/* Unit-local byte view of the duel-state flags byte at +0x1B12: the ROM tests it as a raw mask
 * (0x1C), while canonical struct DuelState exposes it as the phase1B12:3 bitfield, so this view
 * is kept for that mask test (gDuel itself comes from duel.h). */
struct DuelFlagBytes {
    u8 filler0[0x1B12];
    u8 unk1B12;
};

/* Event message ids have bit 15 set when they concern player 1. */
#define EVT(p, id) ((p) ? (0x8000 | (id)) : (id))

void DuelCmd_Push(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void EventResponse_Request(int player, int kind, u32 arg);
void CopyDuelCard(u32 *dst, u32 *src);
void DiscardHandCard(int player, int idx, int a, int b);
void AddSprite(u32 yx, u16 shapeSize, u16 attr2);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
void ShowCardEffect(int player, u16 id);
void LoseLifePoints(int player, int lp);
void BanishTopDeckCards(int player, int arg);
void Chain_AddPending(u32 event, int arg);
int CountActiveCardsOnField(int player, u16 cardNo);
void DuelPrompt_PostRandomDiscard(int player, u16 arg, int value);
/* Pack a (zone << 8) | player location into a byte: player | zone << 4. */
#define PACK_LOC(loc) ((u8)((loc) & 0xF) | (u8)(((loc) >> 8) & 0xF) << 4)

int ReturnGraveyardCardToHand(int player, u16 no)
{
    int i;
    for (i = 0; i < PLAYER(player).graveCount; i++) {
        u32 *p = (u32 *)&PLAYER(player).graveyard[i];
        u32 card = *p;
        if (CARD_NUMBER(CARD(card).id) == no) {
            DuelCmd_Push(player ? 0x80D2 : 0xD2, card, card >> 16, 0);
            return 1;
        }
    }
    return 0;
}
int RemoveGraveyardCardByNumber(int player, u16 no, u32 *out)
{
    int i;
    for (i = 0; i < PLAYER(player).graveCount; i++) {
        u32 *card = (u32 *)&PLAYER(player).graveyard[i];
        if (CARD_NUMBER(CARD(*card).id) == no) {
            CopyDuelCard(out, card);
            DuelCmd_Push(player ? 0x80D3 : 0xD3, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 1;
        }
    }
    return 0;
}
void BanishGraveyardCard(int player, u16 *card)
{
    DuelCmd_Push(player ? 0x80D4 : 0xD4, card[0], card[1], 0);
}
int RemoveDeckCardByNumber(int player, u16 no, u32 *out)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        u32 *card = (u32 *)&PLAYER(player).deck[i];
        if (CARD_NUMBER(CARD(*card).id) == no) {
            CopyDuelCard(out, card);
            DuelCmd_Push(player ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return i;
        }
    }
    return -1;
}
int AddDeckCardToHand(int player, u16 no)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        u32 *p = (u32 *)&PLAYER(player).deck[i];
        u32 card = *p;
        if (CARD_NUMBER(CARD(card).id) == no) {
            DuelCmd_Push(player ? 0x8064 : 0x64, card, card >> 16, 0);
            return 1;
        }
    }
    return 0;
}

void ShowCardDetail(int player, u16 arg)
{
    DuelCmd_Push(player ? 0x8070 : 0x70, arg, 1, 0);
    DuelCmd_Push(player ? 0x8012 : 0x12, 1, 0, 0);
}
void sub_080197C0(int player, u16 arg)
{
    DuelCmd_Push(player ? 0x8072 : 0x72, arg, 1, 0);
}
void ShowCardEffect(int player, u16 arg)
{
    DuelCmd_Push(player ? 0x8073 : 0x73, arg, 1, 0);
}
void ShowDestroyedCard(int player, u16 arg)
{
    DuelCmd_Push(player ? 0x8074 : 0x74, arg, 1, 0);
}
void sub_08019820(int player, u16 arg)
{
    DuelCmd_Push(player ? 0x8075 : 0x75, arg, 1, 0);
}
void ShowRevealedCard(int player, u16 arg)
{
    DuelCmd_Push(player ? 0x8076 : 0x76, arg, 1, 0);
}
void LoseLifePoints(int player, int lp)
{
    if (lp != 0) {
        DuelCmd_Push(player ? 0x8043 : 0x43, lp, 1, 0);
        EventResponse_Request(player, 0xF, (player << 16) | (u16)lp);
    }
}
void InflictBattleDamage(int player, int lp, u16 from, u16 to)
{
    u16 no;
    int count = CountActiveCardsOnField(1 - player, no = 1045);

    if (lp != 0) {
        DuelCmd_Push(player ? 0x8043 : 0x43, lp, 1, 0);
        if (count > 0) {
            DuelCmd_Push((1 - player) ? 0x8073 : 0x73, ((const u16 *)0x08623DF4)[no], 1, 0);
            DuelPrompt_PostRandomDiscard(player, 1, count);
        }
        if ((u8)to == player)
            EventResponse_Request(player, 0xD, (u16)lp | (PACK_LOC(to) | PACK_LOC(from) << 8) << 16);
        else
            EventResponse_Request(player, 0xE, (u16)lp | (PACK_LOC(from) | PACK_LOC(to) << 8) << 16);
    }
}
void GainLifePoints(int player, int lp)
{
    u16 no = 1434;
    int count = CountFaceUpMonstersByNumber(player, no);
    if (lp != 0) {
        DuelCmd_Push(player ? 0x8042 : 0x42, lp, 1, 0);
        if (count > 0) {
            ShowCardEffect(player, ((const u16 *)0x08623DF4)[no]);
            LoseLifePoints(1 - player, count * 500);
        }
    }
}
int FindFreeMonsterZone(int player);
void ShowRevealedCard(int player, u16 id);
void TriggerAppropriate(int player);
void GainLifePoints(int player, int lp);

static inline u32 CardAttack(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((CARD_STATS(id) >> 9) & 0x1FF) * 10;
}

/* Draw n cards for player (hypothesis): events 0x61 per card, special handling for card 762
 * (goes straight to a monster zone) and for monsters with >= 1500 ATK when the player's
 * +0x0B low bits are set. */
void DrawCards(int player, int n)
{
    int handIdx = PLAYER(player).handCount;
    int i;
    u16 no = 1305;
    int count = CountActiveCardsOnField(player, no);

    if (count > 0) {
        DuelCmd_Push(player ? 0x8073 : 0x73, ((const u16 *)0x08623DF4)[no], 1, 0);
        GainLifePoints(player, count * 500);
    }
    for (i = 0; i < n; i++) {
        u32 *card = (u32 *)&PLAYER(player).deck[i];
        int handled = 0;
        int placed = 0;

        DuelCmd_Push(player ? 0x8061 : 0x61, 1, 1, 0);
        if (CARD(*card).owner != player && CARD17(*card).unk17 && CARD_NUMBER(CARD(*card).id) == 762) {
            int zone = FindFreeMonsterZone(player);
            handled = 1;
            placed = 1;
            DuelCmd_Push(player ? 0x8073 : 0x73, CARD(*card).id, 1, 0);
            if (zone >= 0) {
                DuelCmd_Push(player ? 0x80C4 : 0xC4, CARD(*card).id, ((handIdx & 0xF) << 4) | (zone & 0xF) | 0x300, 0);
                DuelCmd_Push(player ? 0x8090 : 0x90, zone, 0xD, 0);
                LoseLifePoints(player, 1000);
            } else {
                DuelCmd_Push(player ? 0x80C0 : 0xC0, handIdx, 1, 0);
            }
        }
        if (PLAYER(player).unkB_0 && !(((u8 *)gDuelPlayers)[0x1AC8] & 0x80) && !handled) {
            u16 id = CARD(*card).id;
            DuelCmd_Push(player ? 0x8073 : 0x73, ((const u16 *)0x08623DF4)[660], 1, 0);
            ShowRevealedCard(player, id);
            if (CARD_TYPE(id) <= 20 && CardAttack(id) > 1499) {
                DuelCmd_Push(player ? 0x80C0 : 0xC0, handIdx, 1, 0);
                if (CARD_NUMBER(id) == 1242)
                    { u32 ev = ((player & 0x1F) << 16) | 0x3A600000; ev |= (player & 1) << 31; Chain_AddPending(ev | id, 0); }
                placed = 1;
            }
        }
        if (!placed)
            handIdx++;
    }
    if (((struct DuelFlagBytes *)&gDuel)->unk1B12 & 0x1C) {
        TriggerAppropriate(1 - player);
        EventResponse_Request(1 - gDuel.linkSkip, 0x1A, (u8)player | ((u8)(1 - player) << 16));
    }
}
void BanishTopDeckCards(int player, int arg)
{
    DuelCmd_Push(player ? 0x8063 : 0x63, arg, 0, 0);
}
void SendTopDeckCardsToGraveyard(int player, int n, u16 flag)
{
    int i;

    if (CountFaceUpMonstersByNumber(0, 1107) > 0 || CountFaceUpMonstersByNumber(1, 1107) > 0) {
        BanishTopDeckCards(player, n);
        return;
    }
    DuelCmd_Push(player ? 0x8062 : 0x62, n, 0, 0);
    for (i = 0; i < n && i < PLAYER(player).deckCount; i++) {
        u32 id = CARD(PLAYER(player).deck[i]).id;
        switch (CARD_NUMBER(id)) {
        case 198:
            if (flag)
                Chain_AddPending(((player & 1) << 31) | 0x38600000 | (id & 0xFFFF), 0);
            break;
        case 1242:
            Chain_AddPending(((player & 1) << 31) | 0x38600000 | (id & 0xFFFF), 0);
            break;
        }
    }
}
static inline u16 ReadDeckCardId(int player, int index)
{
    int indexBytes = index * 4;
    u32 offset, word;

    offset = (player & 1) * 0xD64;
    word = *(u32 *)(indexBytes + offset + (u32)gDuelPlayers + 0x7C4);
    return (word << 20) >> 20;
}

static inline u16 ReadFusionCardId(int player, int index)
{
    int indexBytes = index * 4;
    u32 offset, word;

    offset = (player & 1) * 0xD64;
    word = *(u32 *)(indexBytes + offset + (u32)gDuelPlayers + 0xA44);
    return (word << 20) >> 20;
}

u32 IsSameCardName(u32 id1, u32 id2);

/* Card number to card ID (0xFFFF maps to 0; numbers >= 2000 map to ID(no - 2000) + 1). */
static inline u16 CardNumberToId(u16 no)
{
    if (no == 0xFFFF)
        return 0;
    if (no < 2000)
        return ((const u16 *)0x08623DF4)[no & 0x7FF];
    return ((const u16 *)0x08623DF4)[(no - 2000) & 0x7FF] + 1;
}

/* Queue event 0x67 / 0xDD for every deck / fusion-deck card that IsSameCardName matches with card
 * number `no`; with `flag`, card 198 also queues events 0xD6 and 0x60. */
void SendDeckCopiesToGraveyard(int player, u16 no, u16 flag)
{
    int i;

    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (IsSameCardName(ReadDeckCardId(player, i), CardNumberToId(no))) {
            u16 *h = (u16 *)&PLAYER(player).deck[i];
            DuelCmd_Push(player ? 0x8067 : 0x67, h[0], h[1], 0);
        }
    }
    for (i = 0; i < PLAYER(player).fusionCount; i++) {
        if (IsSameCardName(ReadFusionCardId(player, i), CardNumberToId(no))) {
            u16 *h = (u16 *)&PLAYER(player).fusionDeck[i];
            DuelCmd_Push(player ? 0x80DD : 0xDD, h[0], h[1], 0);
        }
    }
    if (flag && no == 198) {
        DuelCmd_Push(player ? 0x80D6 : 0xD6, 1, 0, 0);
        DuelCmd_Push(player ? 0x8060 : 0x60, 1, 0, 0);
    }
}
void BanishDeckCopies(int player, u16 no)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (CARD_NUMBER(ReadDeckCardId(player, i)) == no) {
            u16 *h = (u16 *)&PLAYER(player).deck[i];
            DuelCmd_Push(player ? 0x8068 : 0x68, h[0], h[1], 0);
        }
    }
}
int PlaceDeckCardOnField(int player, u16 no, int arg)
{
    int i;

    /* Keep the narrowed card number in ip while the message argument uses r8. */
    __asm__("" : : : "r8");
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (CARD_NUMBER(CARD(PLAYER(player).deck[i]).id) == no) {
            u16 *h = (u16 *)&PLAYER(player).deck[i];
            DuelCmd_Push(player ? 0x8066 : 0x66, h[0], h[1], arg);
            return 1;
        }
    }
    return 0;
}
/* Return 1 (after DiscardHandCard(player, i, 0, 1)) if the hand holds a card with number `no`. */
int DiscardHandCardByNumber(int player, u16 no)
{
    int i = 0;
    u8 *players = (u8 *)gDuelPlayers;
    u32 offset = (player & 1) * 0xD64;
    struct DuelPlayer *p = (struct DuelPlayer *)(players + offset);
    if (i < p->handCount) {
        u8 *hands = players + 0x684;
        u32 *hand = (u32 *)(offset + (u32)hands);
        do {
            if (CARD_NUMBER(CARD(*hand).id) == no) {
                DiscardHandCard(player, i, 0, 1);
                return 1;
            }
            hand++;
            i++;
        } while (i < p->handCount);
    }
    return 0;
}
void ChainListScreen_DrawHeader(void)
{
    int i;
    for (i = 0; i <= 7; i++)
        AddSprite(i << 5, 0x4080, i << 2);
}
/* Draw a 4-row x 8-column block of 8x8 OBJ tiles (rows 32 px apart, optionally scaled by scale/16). */
void ChainListScreen_DrawRows(int scale, u16 scaled, int unused)
{
    int i, j;
    for (i = 0; i < 4; i++) {
        int tile = i * 128 + 64;
        int y = i * 32 + 16;
        if (scaled) {
            y *= scale;
            y /= 16;
        }
        AddSprite(y << 16, 0x80, tile + 0x1400);
        for (j = 1; j < 8; j++)
            AddSprite((j << 5) | (y << 16), 0x80, tile + j * 4 + 0x400);
    }
}
extern const u8 gCardIconNormalGfx[];
extern const u8 gCardIconEffectGfx[];
extern const u8 gCardIconFusionGfx[];
extern const u8 gCardIconRitualGfx[];
extern const u8 gCardIconTrapGfx[];
extern const u8 gCardIconMagicGfx[];

static inline int CardFrameKind(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((int)CARD_TYPE(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    }
    return (CARD_STATS(id) & 0xC0000) >> 18;
}

/* Card frame graphics for a card ID (hypothesis). */
const u8 *GetCardIconGfx(u16 id)
{
    switch (CARD_TYPE(id)) {
    case 22:
        return gCardIconMagicGfx;
    case 21:
        return gCardIconTrapGfx;
    }
    switch (CardFrameKind(id)) {
    case 1:
        return gCardIconEffectGfx;
    case 2:
        return gCardIconFusionGfx;
    case 3:
        return gCardIconRitualGfx;
    }
    return gCardIconNormalGfx;
}
/* Card-list result screen (hypothesis: cards obtained after a duel), state machine in
 * gChainListScreen (see link_battle). */
struct CardListEntry {
    u16 id;                 /* +0x00 card ID */
    u8 flags2;              /* +0x02 bit 0 */
    u8 unk3;
    u8 flags4;              /* +0x04 bits 2, 3 */
    u8 unk5[0x14 - 5];
};
struct CardList {
    struct CardListEntry entries[16];
    u16 count;              /* +0x140 */
};
struct ListScreen {
    struct CardList *list;  /* +0x00 */
    u8 player : 1;          /* +0x04 */
    u8 state : 7;
    u8 timer;               /* +0x05 */
};
extern struct ListScreen gChainListScreen;
/* Per-player header, 0x4C bytes each: +0x00 colour (shadow), +0x04 colour, +0x08 u16 tile,
 * +0x0C title. A byte array, not a struct: with a 1-byte-aligned row, agbcc forms each field
 * address as (sym + off) + player * 0x4C, as the ROM does (a 4-aligned struct folds the offset
 * into the load instead). */
extern const u8 gChainListHeaders[][0x4C];
extern const u8 gSystemFontPal[];
extern const u8 gCardIconPal[];
extern const u8 gStrLink[];
extern const u8 gStrOpposite[];
extern const u8 gStrYours[];
extern const u8 gStrDestroyed[];
extern const u8 gStrInvalidated[];
extern const u8 gCardNames[];    /* card names, 0x40 bytes each */
void TextCellsClear(void);
void DuelScreen_ScrollToZone(u32 player, u32 zone);
int DuelFieldDim(int a);
void UnloadDuelUiGfx(void);
void MemCopy16(void *dest, const void *src, u32 size);
void CopyDoubleWords(void *dst, const void *src, u32 size);
void TextCanvasInit(u8 a, u8 b);
void TextDrawString(s32 x, s32 y, u16 attr, const u8 *str);
s32 StrLen(const u8 *str);
void TextDrawNumber(int x, int y, u16 attr, int value);
void TextCanvasToTiles(void *dest, u16 value);
void ChainListScreen_DrawHeader(void);
void ChainListScreen_DrawRows(int scale, u16 scaled, int unused);
const u8 *GetCardIconGfx(u16 id);
int DuelFieldFadeFromBlack(int a);
void LoadDuelUiGfx(void);
#define KEYS gMain
#define FAST() ((gMain.heldKeys & 2) || gDuelScreen.fast)

int ChainListScreen_Run(void)
{
    int i, row, x, j;
    struct CardListEntry *e;

    /* Every case returns (no break + trailing return 0): this puts the shared "return 0" block
     * before the default case, and lets case 5's two timer stores cross-jump into one. */
    switch (gChainListScreen.state) {
    case 0:
        TextCellsClear();
        DuelScreen_ScrollToZone(0, 0);
        gChainListScreen.state++;
        return 0;
    case 1:
        if (DuelFieldDim(1))
            gChainListScreen.state++;
        return 0;
    case 2:
        UnloadDuelUiGfx();
        gChainListScreen.state++;
        return 0;
    case 3:
        MemCopy16((void *)0x05000200, gSystemFontPal, 0x20);
        CopyDoubleWords((void *)0x05000220, gCardIconPal, 0x20);
        TextCanvasInit(0x20, 2);
        TextDrawString(3, 3, gChainListHeaders[gChainListScreen.player][4] | 0xC00, &gChainListHeaders[gChainListScreen.player][0xC]);
        TextDrawString(2, 2, gChainListHeaders[gChainListScreen.player][0] | 0xC00, &gChainListHeaders[gChainListScreen.player][0xC]);
        TextCanvasToTiles((void *)0x06010000, *(u16 *)&gChainListHeaders[gChainListScreen.player][8]);
        i = 0;
        if (gChainListScreen.list->count > 4)
            i = gChainListScreen.list->count - 4;
        for (row = 0; i < gChainListScreen.list->count && row <= 3; row++, i++) {
            e = &gChainListScreen.list->entries[i];
            TextCanvasInit(0x20, 4);
            TextDrawString(0x22, 5, 0xA01, gStrLink);
            TextDrawString(0x21, 4, 0xA07, gStrLink);
            x = StrLen(gStrLink) * 5;
            TextDrawNumber(x + 0x22, 5, 0xA01, i + 1);
            TextDrawNumber(x + 0x21, 4, 0xA07, i + 1);
            if (i <= 9)
                x += 10;
            else
                x += 15;
            if (e->flags2 & 1) {
                x += 4;
                TextDrawString(x + 0x22, 5, 0xA01, gStrOpposite);
                TextDrawString(x + 0x21, 4, 0xA04, gStrOpposite);
                x += StrLen(gStrOpposite) * 5;
            } else {
                x += 4;
                TextDrawString(x + 0x22, 5, 0xA01, gStrYours);
                TextDrawString(x + 0x21, 4, 0xA06, gStrYours);
                x += StrLen(gStrYours) * 5;
            }
            if (e->flags4 & 8) {
                x += 4;
                TextDrawString(x + 0x22, 5, 0xA0B, gStrDestroyed);
                TextDrawString(x + 0x21, 4, 0xA03, gStrDestroyed);
                x += StrLen(gStrDestroyed) * 5;
            }
            if (e->flags4 & 4) {
                x += 4;
                TextDrawString(x + 0x22, 5, 0xA0D, gStrInvalidated);
                TextDrawString(x + 0x21, 4, 0xA05, gStrInvalidated);
                x += StrLen(gStrInvalidated) * 5;
            }
            TextDrawString(0x23, 0x12, 0xA01, gCardNames + e->id * 64);
            TextDrawString(0x22, 0x11, 0xA07, gCardNames + e->id * 64);
            TextCanvasToTiles((void *)(0x06010000 + ((row * 128 + 64) << 5)), 0);
            /* tile-index form keeps VRAM + offset out of GCSE, so loop.c rebuilds it for the giv */
            for (j = 0; j <= 3; j++)
                CopyDoubleWords((void *)(0x06010000 + ((row * 128 + 64 + j * 32) << 5)), GetCardIconGfx(e->id) + j * 128, 0x80);
        }
        gChainListScreen.timer = 0;
        gChainListScreen.state++;
        return 0;
    case 4:
        ChainListScreen_DrawHeader();
        ChainListScreen_DrawRows(gChainListScreen.timer, 1, -1);
        if (gChainListScreen.timer <= 15) {
            gChainListScreen.timer++;
            if (FAST()) {
                if (gChainListScreen.timer <= 7)
                    gChainListScreen.timer += 7;
                else
                    gChainListScreen.timer = 16;
            }
        } else {
            gChainListScreen.timer = 0;
            gChainListScreen.state++;
        }
        return 0;
    case 5:
        ChainListScreen_DrawHeader();
        ChainListScreen_DrawRows(0, 0, -1);
        gChainListScreen.timer++;
        if ((KEYS.newKeys & 3) || gChainListScreen.timer > 120) {
            gChainListScreen.timer = 16;
            gChainListScreen.state++;
            return 0;
        }
        if (FAST()) {
            if (gChainListScreen.timer <= 103)
                gChainListScreen.timer += 16;
            else
                gChainListScreen.timer = 120;
        }
        return 0;
    case 6:
        ChainListScreen_DrawHeader();
        ChainListScreen_DrawRows(gChainListScreen.timer, 1, -1);
        if (gChainListScreen.timer != 0) {
            gChainListScreen.timer--;
            if (FAST()) {
                if (gChainListScreen.timer > 8)
                    gChainListScreen.timer -= 8;
                else
                    gChainListScreen.timer = 0;
            }
        } else {
            gChainListScreen.timer = 0;
            gChainListScreen.state++;
        }
        return 0;
    case 7:
        if (DuelFieldFadeFromBlack(1))
            gChainListScreen.state++;
        return 0;
    default:
        LoadDuelUiGfx();
        return 1;
    }
}
