#include "global.h"
#include "main.h"     /* struct Main, gUnk_03000040 */
#include "duel.h"     /* DuelCard/DuelPlayer/DuelState, gUnk_020192E0, gUnk_020192E4 */
#include "duel_ui.h"  /* gUnk_0201CFB0 (struct DuelScreen) */

/*
 * Duel list queries (graveyard / deck / hand searches by card number), duel event
 * wrappers around sub_0801EC58, and LP-change helpers.  See wiki/functions/code-08019554.md.
 *
 * Uses the shared duel structs.  Canonical DuelCard (duel.h) places the drawn-card flag at
 * bit 20, but this unit's ROM code tests bit 17 (sub_080199E0), so a unit-local word view is
 * kept for that one test (CARD17 below).
 */

#define CARD(word) (*(struct DuelCard *)&(word))
#define CARD17(word) (*(struct DuelCard17 *)&(word))

/* Unit-local view: canonical DuelCard has flag20:1 at bit 20; sub_080199E0 tests bit 17. */
struct DuelCard17 {
    u32 id : 12;        /* card ID (index into gUnk_08621DE0 / gUnk_08622AB4); 0 = none */
    u32 owner : 1;
    u32 unk13 : 4;
    u32 unk17 : 1;      /* tested when drawing card 762 */
    u32 unk18 : 14;
};

#define PLAYER(p) (gUnk_020192E4[(p) & 1])

#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
extern const u16 gUnk_08623DF4[];

/* Unit-local byte view of the duel-state flags byte at +0x1B12: the ROM tests it as a raw mask
 * (0x1C), while canonical struct DuelState exposes it as the phase1B12:3 bitfield, so this view
 * is kept for that mask test (gUnk_020192E0 itself comes from duel.h). */
struct DuelFlagBytes {
    u8 filler0[0x1B12];
    u8 unk1B12;
};

/* Event message ids have bit 15 set when they concern player 1. */
#define EVT(p, id) ((p) ? (0x8000 | (id)) : (id))

void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void sub_08042AB0(int player, int kind, u32 arg);
void sub_08007558(u32 *dst, u32 *src);
void sub_080193D4(int player, int idx, int a, int b);
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);
int sub_080086CC(int player, u16 cardNo);
void sub_080197E0(int player, u16 id);
void sub_08019860(int player, int lp);
void sub_08019CD0(int player, int arg);
void sub_0801FBCC(u32 event, int arg);
int sub_08008524(int player, u16 cardNo);
void sub_08022784(int player, u16 arg, int value);
/* Pack a (zone << 8) | player location into a byte: player | zone << 4. */
#define PACK_LOC(loc) ((u8)((loc) & 0xF) | (u8)(((loc) >> 8) & 0xF) << 4)

int sub_08019554(int player, u16 no)
{
    int i;
    for (i = 0; i < PLAYER(player).graveCount; i++) {
        u32 *p = (u32 *)&PLAYER(player).graveyard[i];
        u32 card = *p;
        if (CARD_NUMBER(CARD(card).id) == no) {
            sub_0801EC58(player ? 0x80D2 : 0xD2, card, card >> 16, 0);
            return 1;
        }
    }
    return 0;
}
int sub_080195D0(int player, u16 no, u32 *out)
{
    int i;
    for (i = 0; i < PLAYER(player).graveCount; i++) {
        u32 *card = (u32 *)&PLAYER(player).graveyard[i];
        if (CARD_NUMBER(CARD(*card).id) == no) {
            sub_08007558(out, card);
            sub_0801EC58(player ? 0x80D3 : 0xD3, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 1;
        }
    }
    return 0;
}
void sub_0801965C(int player, u16 *card)
{
    sub_0801EC58(player ? 0x80D4 : 0xD4, card[0], card[1], 0);
}
int sub_0801967C(int player, u16 no, u32 *out)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        u32 *card = (u32 *)&PLAYER(player).deck[i];
        if (CARD_NUMBER(CARD(*card).id) == no) {
            sub_08007558(out, card);
            sub_0801EC58(player ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return i;
        }
    }
    return -1;
}
int sub_0801970C(int player, u16 no)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        u32 *p = (u32 *)&PLAYER(player).deck[i];
        u32 card = *p;
        if (CARD_NUMBER(CARD(card).id) == no) {
            sub_0801EC58(player ? 0x8064 : 0x64, card, card >> 16, 0);
            return 1;
        }
    }
    return 0;
}

void sub_08019788(int player, u16 arg)
{
    sub_0801EC58(player ? 0x8070 : 0x70, arg, 1, 0);
    sub_0801EC58(player ? 0x8012 : 0x12, 1, 0, 0);
}
void sub_080197C0(int player, u16 arg)
{
    sub_0801EC58(player ? 0x8072 : 0x72, arg, 1, 0);
}
void sub_080197E0(int player, u16 arg)
{
    sub_0801EC58(player ? 0x8073 : 0x73, arg, 1, 0);
}
void sub_08019800(int player, u16 arg)
{
    sub_0801EC58(player ? 0x8074 : 0x74, arg, 1, 0);
}
void sub_08019820(int player, u16 arg)
{
    sub_0801EC58(player ? 0x8075 : 0x75, arg, 1, 0);
}
void sub_08019840(int player, u16 arg)
{
    sub_0801EC58(player ? 0x8076 : 0x76, arg, 1, 0);
}
void sub_08019860(int player, int lp)
{
    if (lp != 0) {
        sub_0801EC58(player ? 0x8043 : 0x43, lp, 1, 0);
        sub_08042AB0(player, 0xF, (player << 16) | (u16)lp);
    }
}
void sub_08019894(int player, int lp, u16 from, u16 to)
{
    u16 no;
    int count = sub_08008524(1 - player, no = 1045);

    if (lp != 0) {
        sub_0801EC58(player ? 0x8043 : 0x43, lp, 1, 0);
        if (count > 0) {
            sub_0801EC58((1 - player) ? 0x8073 : 0x73, ((const u16 *)0x08623DF4)[no], 1, 0);
            sub_08022784(player, 1, count);
        }
        if ((u8)to == player)
            sub_08042AB0(player, 0xD, (u16)lp | (PACK_LOC(to) | PACK_LOC(from) << 8) << 16);
        else
            sub_08042AB0(player, 0xE, (u16)lp | (PACK_LOC(from) | PACK_LOC(to) << 8) << 16);
    }
}
void sub_08019980(int player, int lp)
{
    u16 no = 1434;
    int count = sub_080086CC(player, no);
    if (lp != 0) {
        sub_0801EC58(player ? 0x8042 : 0x42, lp, 1, 0);
        if (count > 0) {
            sub_080197E0(player, ((const u16 *)0x08623DF4)[no]);
            sub_08019860(1 - player, count * 500);
        }
    }
}
int sub_08008A44(int player);
void sub_08019840(int player, u16 id);
void sub_08046BA8(int player);
void sub_08019980(int player, int lp);

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
void sub_080199E0(int player, int n)
{
    int handIdx = PLAYER(player).handCount;
    int i;
    u16 no = 1305;
    int count = sub_08008524(player, no);

    if (count > 0) {
        sub_0801EC58(player ? 0x8073 : 0x73, ((const u16 *)0x08623DF4)[no], 1, 0);
        sub_08019980(player, count * 500);
    }
    for (i = 0; i < n; i++) {
        u32 *card = (u32 *)&PLAYER(player).deck[i];
        int handled = 0;
        int placed = 0;

        sub_0801EC58(player ? 0x8061 : 0x61, 1, 1, 0);
        if (CARD(*card).owner != player && CARD17(*card).unk17 && CARD_NUMBER(CARD(*card).id) == 762) {
            int zone = sub_08008A44(player);
            handled = 1;
            placed = 1;
            sub_0801EC58(player ? 0x8073 : 0x73, CARD(*card).id, 1, 0);
            if (zone >= 0) {
                sub_0801EC58(player ? 0x80C4 : 0xC4, CARD(*card).id, ((handIdx & 0xF) << 4) | (zone & 0xF) | 0x300, 0);
                sub_0801EC58(player ? 0x8090 : 0x90, zone, 0xD, 0);
                sub_08019860(player, 1000);
            } else {
                sub_0801EC58(player ? 0x80C0 : 0xC0, handIdx, 1, 0);
            }
        }
        if (PLAYER(player).unkB_0 && !(((u8 *)gUnk_020192E4)[0x1AC8] & 0x80) && !handled) {
            u16 id = CARD(*card).id;
            sub_0801EC58(player ? 0x8073 : 0x73, ((const u16 *)0x08623DF4)[660], 1, 0);
            sub_08019840(player, id);
            if (CARD_TYPE(id) <= 20 && CardAttack(id) > 1499) {
                sub_0801EC58(player ? 0x80C0 : 0xC0, handIdx, 1, 0);
                if (CARD_NUMBER(id) == 1242)
                    { u32 ev = ((player & 0x1F) << 16) | 0x3A600000; ev |= (player & 1) << 31; sub_0801FBCC(ev | id, 0); }
                placed = 1;
            }
        }
        if (!placed)
            handIdx++;
    }
    if (((struct DuelFlagBytes *)&gUnk_020192E0)->unk1B12 & 0x1C) {
        sub_08046BA8(1 - player);
        sub_08042AB0(1 - gUnk_020192E0.linkSkip, 0x1A, (u8)player | ((u8)(1 - player) << 16));
    }
}
void sub_08019CD0(int player, int arg)
{
    sub_0801EC58(player ? 0x8063 : 0x63, arg, 0, 0);
}
void sub_08019CF0(int player, int n, u16 flag)
{
    int i;

    if (sub_080086CC(0, 1107) > 0 || sub_080086CC(1, 1107) > 0) {
        sub_08019CD0(player, n);
        return;
    }
    sub_0801EC58(player ? 0x8062 : 0x62, n, 0, 0);
    for (i = 0; i < n && i < PLAYER(player).deckCount; i++) {
        u32 id = CARD(PLAYER(player).deck[i]).id;
        switch (CARD_NUMBER(id)) {
        case 198:
            if (flag)
                sub_0801FBCC(((player & 1) << 31) | 0x38600000 | (id & 0xFFFF), 0);
            break;
        case 1242:
            sub_0801FBCC(((player & 1) << 31) | 0x38600000 | (id & 0xFFFF), 0);
            break;
        }
    }
}
static inline u16 ReadDeckCardId(int player, int index)
{
    int indexBytes = index * 4;
    u32 offset, word;

    offset = (player & 1) * 0xD64;
    word = *(u32 *)(indexBytes + offset + (u32)gUnk_020192E4 + 0x7C4);
    return (word << 20) >> 20;
}

static inline u16 ReadFusionCardId(int player, int index)
{
    int indexBytes = index * 4;
    u32 offset, word;

    offset = (player & 1) * 0xD64;
    word = *(u32 *)(indexBytes + offset + (u32)gUnk_020192E4 + 0xA44);
    return (word << 20) >> 20;
}

u32 sub_080074A0(u32 id1, u32 id2);

/* Card number to card ID (0xFFFF maps to 0; numbers >= 2000 map to ID(no - 2000) + 1). */
static inline u16 CardNumberToId(u16 no)
{
    if (no == 0xFFFF)
        return 0;
    if (no < 2000)
        return ((const u16 *)0x08623DF4)[no & 0x7FF];
    return ((const u16 *)0x08623DF4)[(no - 2000) & 0x7FF] + 1;
}

/* Queue event 0x67 / 0xDD for every deck / fusion-deck card that sub_080074A0 matches with card
 * number `no`; with `flag`, card 198 also queues events 0xD6 and 0x60. */
void sub_08019E0C(int player, u16 no, u16 flag)
{
    int i;

    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (sub_080074A0(ReadDeckCardId(player, i), CardNumberToId(no))) {
            u16 *h = (u16 *)&PLAYER(player).deck[i];
            sub_0801EC58(player ? 0x8067 : 0x67, h[0], h[1], 0);
        }
    }
    for (i = 0; i < PLAYER(player).fusionCount; i++) {
        if (sub_080074A0(ReadFusionCardId(player, i), CardNumberToId(no))) {
            u16 *h = (u16 *)&PLAYER(player).fusionDeck[i];
            sub_0801EC58(player ? 0x80DD : 0xDD, h[0], h[1], 0);
        }
    }
    if (flag && no == 198) {
        sub_0801EC58(player ? 0x80D6 : 0xD6, 1, 0, 0);
        sub_0801EC58(player ? 0x8060 : 0x60, 1, 0, 0);
    }
}
void sub_0801A010(int player, u16 no)
{
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (CARD_NUMBER(ReadDeckCardId(player, i)) == no) {
            u16 *h = (u16 *)&PLAYER(player).deck[i];
            sub_0801EC58(player ? 0x8068 : 0x68, h[0], h[1], 0);
        }
    }
}
int sub_0801A09C(int player, u16 no, int arg)
{
    int i;

    /* Keep the narrowed card number in ip while the message argument uses r8. */
    __asm__("" : : : "r8");
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (CARD_NUMBER(CARD(PLAYER(player).deck[i]).id) == no) {
            u16 *h = (u16 *)&PLAYER(player).deck[i];
            sub_0801EC58(player ? 0x8066 : 0x66, h[0], h[1], arg);
            return 1;
        }
    }
    return 0;
}
#if 0 /* NONMATCHING (score 8): BYTE-IDENTICAL: whole unit reports 25/25 functions match, unit bytes MATCH with
       * this C enabled. wf.py apply refuses only because check.py's size line compares the .size symbol (0x66, code
       * only) with the table size 0x68, which includes the 2-byte trailing .align pad after bx r1 (matched sub_0801A09C
       * shows the same -2 artifact). To enable: replace the INCLUDE_ASM with this C. Key: the hand pointer must be a
       * fresh pseudo born as offset + (players + 0x684) (separate base temp), so offset dies there and shares r1 with
       * hand; staging hand as two assignments to one variable made it conflict with offset and pushed the product into
       * r0. */
/* Return 1 (after sub_080193D4(player, i, 0, 1)) if the hand holds a card with number `no`. */
int sub_0801A130(int player, u16 no)
{
    int i = 0;
    u8 *players = (u8 *)gUnk_020192E4;
    u32 offset = (player & 1) * 0xD64;
    struct DuelPlayer *p = (struct DuelPlayer *)(players + offset);
    if (i < p->handCount) {
        u8 *hands = players + 0x684;
        u32 *hand = (u32 *)(offset + (u32)hands);
        do {
            if (CARD_NUMBER(CARD(*hand).id) == no) {
                sub_080193D4(player, i, 0, 1);
                return 1;
            }
            hand++;
            i++;
        } while (i < p->handCount);
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08019554", sub_0801A130); /* 0x0801A130 size 0x68 */
void sub_0801A198(void)
{
    int i;
    for (i = 0; i <= 7; i++)
        sub_080761F0(i << 5, 0x4080, i << 2);
}
/* Draw a 4-row x 8-column block of 8x8 OBJ tiles (rows 32 px apart, optionally scaled by scale/16). */
void sub_0801A1B8(int scale, u16 scaled, int unused)
{
    int i, j;
    for (i = 0; i < 4; i++) {
        int tile = i * 128 + 64;
        int y = i * 32 + 16;
        if (scaled) {
            y *= scale;
            y /= 16;
        }
        sub_080761F0(y << 16, 0x80, tile + 0x1400);
        for (j = 1; j < 8; j++)
            sub_080761F0((j << 5) | (y << 16), 0x80, tile + j * 4 + 0x400);
    }
}
extern const u8 gUnk_0867897C[];
extern const u8 gUnk_0867917C[];
extern const u8 gUnk_0867997C[];
extern const u8 gUnk_0867A17C[];
extern const u8 gUnk_0867A97C[];
extern const u8 gUnk_0867B17C[];

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
const u8 *sub_0801A238(u16 id)
{
    switch (CARD_TYPE(id)) {
    case 22:
        return gUnk_0867B17C;
    case 21:
        return gUnk_0867A97C;
    }
    switch (CardFrameKind(id)) {
    case 1:
        return gUnk_0867917C;
    case 2:
        return gUnk_0867997C;
    case 3:
        return gUnk_0867A17C;
    }
    return gUnk_0867897C;
}
#if 0 /* NONMATCHING: faithful 32-bit DrawText coordinates and shared next-index
 * staging reduce the draft to 0x490 bytes versus 0x488. Header-field addressing,
 * text-colour lifetimes and the shared animation-state tails still differ. */
/* Card-list result screen (hypothesis: cards obtained after a duel), state machine in
 * gUnk_020185B8 (see code_0801A7B4). */
struct CardListEntry {
    u16 id;                 /* +0x00 card ID */
    u8 flags2;              /* +0x02 bit 0 */
    u8 unk3;
    u8 flags4;              /* +0x04 bits 2, 3 */
    u8 unk5[0x14 - 5];
};
struct CardList {
    struct CardListEntry entries[16];
    int count;              /* +0x140 */
};
struct ListScreen {
    struct CardList *list;  /* +0x00 */
    u8 player : 1;          /* +0x04 */
    u8 state : 7;
    u8 timer;               /* +0x05 */
};
extern struct ListScreen gUnk_020185B8;
struct ListScreenHeader {
    u8 color0;              /* +0x00 */
    u8 unk1[3];
    u8 color4;              /* +0x04 */
    u8 unk5[3];
    u16 tile;               /* +0x08 */
    u8 unkA[2];
    u8 title[0x40];         /* +0x0C */
};
extern const struct ListScreenHeader gUnk_08198DE4[2];
extern const u8 gUnk_0822C300[];
extern const u8 gUnk_0867795C[];
extern const u8 gUnk_080817C8[];
extern const u8 gUnk_080817D0[];
extern const u8 gUnk_080817DC[];
extern const u8 gUnk_080817E4[];
extern const u8 gUnk_080817F0[];
extern const u8 gUnk_0822C720[];    /* card names, 0x40 bytes each */
void sub_0805ED9C(void);
void sub_080240A8(u32 player, u32 zone);
int sub_08060BF0(int a);
void sub_080619E8(void);
void sub_08075294(void *dest, const void *src, u32 size);
void sub_080752B0(void *dst, const void *src, u32 size);
void sub_08074B08(u8 a, u8 b);
void sub_0807501C(s32 x, s32 y, u16 attr, const u8 *str);
s32 sub_080753CC(const u8 *str);
void sub_080750E0(int x, int y, u16 attr, int value);
void sub_08075114(void *dest, u16 value);
void sub_0801A198(void);
void sub_0801A1B8(int scale, u16 scaled, int unused);
const u8 *sub_0801A238(u16 id);
int sub_08060C68(int a);
void sub_08060578(void);
#define KEYS gUnk_03000040
#define FAST() ((gUnk_03000040.heldKeys & 2) || gUnk_0201CFB0.fast)

int sub_0801A32C(void)
{
    int i, row, x, j;
    int nextIndex;
    struct CardListEntry *e;
    u8 *vram;
    const u8 *str;

    switch (gUnk_020185B8.state) {
    case 0:
        sub_0805ED9C();
        sub_080240A8(0, 0);
        gUnk_020185B8.state++;
        break;
    case 1:
        if (sub_08060BF0(1))
            gUnk_020185B8.state++;
        break;
    case 2:
        sub_080619E8();
        gUnk_020185B8.state++;
        break;
    case 3:
        sub_08075294((void *)0x05000200, gUnk_0822C300, 0x20);
        sub_080752B0((void *)0x05000220, gUnk_0867795C, 0x20);
        sub_08074B08(0x20, 2);
        sub_0807501C(3, 3, gUnk_08198DE4[gUnk_020185B8.player].color4 | 0xC00, gUnk_08198DE4[gUnk_020185B8.player].title);
        sub_0807501C(2, 2, gUnk_08198DE4[gUnk_020185B8.player].color0 | 0xC00, gUnk_08198DE4[gUnk_020185B8.player].title);
        sub_08075114((void *)0x06010000, gUnk_08198DE4[gUnk_020185B8.player].tile);
        i = 0;
        if (gUnk_020185B8.list->count > 4)
            i = gUnk_020185B8.list->count - 4;
        row = 0;
        if (i < gUnk_020185B8.list->count) {
        next_row:
            e = &gUnk_020185B8.list->entries[i];
            sub_08074B08(0x20, 4);
            sub_0807501C(0x22, 5, 0xA01, gUnk_080817C8);
            sub_0807501C(0x21, 4, 0xA07, gUnk_080817C8);
            x = sub_080753CC(gUnk_080817C8) * 5;
            nextIndex = i + 1;
            sub_080750E0(x + 0x22, 5, 0xA01, nextIndex);
            sub_080750E0(x + 0x21, 4, 0xA07, nextIndex);
            if (i <= 9)
                x += 10;
            else
                x += 15;
            if (e->flags2 & 1) {
                x += 4;
                str = gUnk_080817D0;
                sub_0807501C(x + 0x22, 5, 0xA01, str);
                sub_0807501C(x + 0x21, 4, 0xA04, str);
            } else {
                x += 4;
                str = gUnk_080817DC;
                sub_0807501C(x + 0x22, 5, 0xA01, str);
                sub_0807501C(x + 0x21, 4, 0xA06, str);
            }
            x += sub_080753CC(str) * 5;
            if (e->flags4 & 8) {
                x += 4;
                str = gUnk_080817E4;
                sub_0807501C(x + 0x22, 5, 0xA0B, str);
                sub_0807501C(x + 0x21, 4, 0xA03, str);
                x += sub_080753CC(str) * 5;
            }
            if (e->flags4 & 4) {
                x += 4;
                str = gUnk_080817F0;
                sub_0807501C(x + 0x22, 5, 0xA0D, str);
                sub_0807501C(x + 0x21, 4, 0xA05, str);
                x += sub_080753CC(str) * 5;
            }
            sub_0807501C(0x23, 0x12, 0xA01, gUnk_0822C720 + e->id * 64);
            sub_0807501C(0x22, 0x11, 0xA07, gUnk_0822C720 + e->id * 64);
            sub_08075114((void *)(0x06010000 + ((row * 128 + 64) << 5)), 0);
            vram = (u8 *)(0x06010000 + ((row * 128 + 64) << 5));
            for (j = 0; j <= 3; j++) {
                sub_080752B0(vram, sub_0801A238(e->id) + j * 128, 0x80);
                vram += 0x400;
            }
            row++;
            i = nextIndex;
            if (i < gUnk_020185B8.list->count && row <= 3)
                goto next_row;
        }
        gUnk_020185B8.timer = 0;
        gUnk_020185B8.state++;
        break;
    case 4:
        sub_0801A198();
        sub_0801A1B8(gUnk_020185B8.timer, 1, -1);
        if (gUnk_020185B8.timer > 15) {
            gUnk_020185B8.timer = 0;
            break;
            gUnk_020185B8.state++;
        }
        gUnk_020185B8.timer++;
        if (FAST()) {
            if (gUnk_020185B8.timer <= 7)
                gUnk_020185B8.timer += 7;
            else
                gUnk_020185B8.timer = 16;
        }
        break;
    case 5:
        sub_0801A198();
        sub_0801A1B8(0, 0, -1);
        gUnk_020185B8.timer++;
        if ((KEYS.newKeys & 3) || gUnk_020185B8.timer > 120) {
            gUnk_020185B8.timer = 16;
            gUnk_020185B8.state++;
            break;
        }
        if (FAST()) {
            if (gUnk_020185B8.timer <= 103)
                gUnk_020185B8.timer += 16;
            else
                gUnk_020185B8.timer = 120;
        }
        break;
    case 6:
        sub_0801A198();
        sub_0801A1B8(gUnk_020185B8.timer, 1, -1);
        if (gUnk_020185B8.timer == 0) {
            gUnk_020185B8.timer = 0;
            gUnk_020185B8.state++;
            break;
        }
        gUnk_020185B8.timer--;
        if (FAST()) {
            if (gUnk_020185B8.timer > 8)
                gUnk_020185B8.timer -= 8;
            else
                gUnk_020185B8.timer = 0;
        }
        break;
    case 7:
        if (sub_08060C68(1))
            gUnk_020185B8.state++;
        break;
    default:
        sub_08060578();
        return 1;
    }
    return 0;
}
#else
INCLUDE_ASM("asm/nonmatching/code_08019554", sub_0801A32C); /* 0x0801A32C size 0x488 */
#endif
