extern const unsigned int gUnk_08621DE0[];
#include "global.h"


/*
 * Duel AI helpers (zone evaluation, attack target selection, duel-state save/restore).
 * See wiki/functions/code-08056ecc.md.
 */

/* A card instance word: low 12 bits = card id (0 = none). */
struct DuelCard {
    u32 id : 12;
    u32 unk12 : 1;
    u32 unk13 : 19;
};

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4[2];
    u8 f6_0 : 1;            /* +0x06 bit 0 = face-up? */
    u8 f6_1 : 1;            /* bit 1 = temporary evaluation flag */
    u8 f6_rest : 6;
    u8 f7_0 : 1;            /* +0x07 */
    u8 f7_1 : 1;
    u8 f7_2 : 1;
    u8 f7_3 : 1;
    u8 f7_rest : 4;
    u8 filler8[2];
    u16 links[32];          /* +0x0A */
    u16 linkKinds[32];      /* +0x4A */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[5];
    u8 unk91;
    u8 filler92[2];
};

/* Per-player duel state, 0xD64 bytes, two of them at 0x020192E4. */
struct DuelPlayer {
    u8 unk0[2];
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 count904;                    /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 countB84;                    /* +0x006 */
    u8 unk7[2];
    u8 unk9_0 : 1;
    u8 unk9_1 : 7;
    u8 fillerA[0x1C];
    u16 zoneMask;                   /* +0x026 per-zone bitmask */
    struct DuelZone zones[11];      /* +0x028 */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard list904[80];    /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard listB84[80];    /* +0xB84 */
    u16 arrCC4[80];                 /* +0xCC4 */
};
extern struct DuelPlayer gUnk_020192E4[2];

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); callers pass player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
/* ROM tables through integer-constant pointers (the ROM reloads the table address at every use). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Card word of player 1's zone i (zones start at 0x0201930C + 0xD64). */
extern u8 gUnk_0201A070[];
#define ZONE1_WORD(i) (*(u32 *)((i) * 0x94 + (u32)gUnk_0201A070))

/* Monster level as the game computes it: Magic/Trap types 0x15-0x17 count as 0, type 0x18 as 10. */
#define CARD_LEVEL(id, r)                                     \
    switch ((int)CARD_TYPE(id)) {                             \
    case 0x15:                                                \
    case 0x16:                                                \
    case 0x17:                                                \
        r = 0;                                                \
        break;                                                \
    case 0x18:                                                \
        r = 10;                                               \
        break;                                                \
    default:                                                  \
        r = (CARD_STATS(id) & 0x1E000000) >> 25;              \
        break;                                                \
    }

/* AI value of a card: 0 for Magic/Trap/Ritual (types 0x15-0x17), 4000 for type 0x18, else ATK-like field * 10. */
static inline int CardValue(u16 id)
{
    int r;

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = ((CARD_STATS(id) << 14) >> 23) * 10;
        break;
    }
    return r;
}

/* Same with the DEF-like field (bits 0-8) for ordinary monsters. */
static inline int CardDefValue(u16 id)
{
    int r;

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = (CARD_STATS(id) & 0x1FF) * 10;
        break;
    }
    return r;
}

/* "Kind" key of a card (0 = ordinary): used to keep the AI from picking special cards first. */
static inline u8 CardKey(u16 id)
{
    int number = CARD_NUMBER(id);

    switch (number) {
    case 0x776:
        return 3;
    case 0x777:
    case 0x778:
        return 1;
    }
    switch ((int)CARD_TYPE(id)) {
    case 0x16:
        return 7;
    case 0x15:
        return 8;
    case 0x17:
        return 9;
    }
    return (CARD_STATS(id) & 0xC0000) >> 18;
}

struct ZoneCardInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
};
void sub_0800ABC8(u32 player, u32 slot, struct ZoneCardInfo *out);
int sub_08007994(u32 id);
int sub_080563B8(int skip, u16 flag);
extern const u16 gUnk_08622AB4[];

/* Duel action record at 0x0201CF90 (0x10 bytes), filled in and handed to sub_08055A00. */
struct ActRec {
    u32 player : 1;     /* bit 0 */
    u32 zone5 : 5;      /* bits 1-5 */
    u32 zone8 : 8;      /* bits 6-13 */
    u32 f14 : 1;        /* bit 14 */
    u32 f15 : 1;        /* bit 15 */
    u32 f16 : 3;        /* bits 16-18 */
    u32 f19 : 3;        /* bits 19-21 */
    u32 pad22 : 3;      /* bits 22-24 */
    u32 f25 : 1;
    u32 f26 : 1;
    u32 pad27 : 1;
    u32 f28 : 1;
    u32 f29 : 1;
    u32 pad30 : 1;
    u32 cardId : 16;    /* bits 31-46 (straddles the word boundary) */
    u32 pad47 : 17;
    struct DuelCard card;   /* +8 */
    u16 h0C;            /* +0xC */
    u8 f0E_0 : 2;
    u8 kind : 3;        /* bits 2-4 of +0xE */
    u8 f0E_5 : 3;
    u8 pad0F;
};
extern struct ActRec gUnk_0201CF90;
void sub_08055A00(void);
void sub_08007558(struct DuelCard *dst, struct DuelCard *src);

void sub_08055EB0(int player, int zone)
{
    struct ActRec *r = &gUnk_0201CF90;

    r->player = player;
    r->zone5 = zone;
    r->zone8 = zone;
    r->f14 = 1;
    r->f15 = 0;
    r->f16 = 0;
    r->f19 = 0;
    r->f25 = 0;
    r->f26 = 0;
    r->cardId = CARD_ID(*(u32 *)((player & 1) * 0xD64 + zone * 0x94 + (u32)gUnk_0201930C));
    r->kind = 3;
    r->h0C = 3;
    sub_08055A00();
}
int sub_08008524(int player, u16 number);
int sub_08007834(u16 id);
int sub_08008A44(int player);

void sub_08055F70(int player, struct DuelCard *card, u16 c, u16 d, u16 e)
{
    struct ActRec *r;

    if (sub_08008524(0, 0x47F) != 0 || sub_08008524(1, 0x47F) != 0)
        c = 1;
    if (sub_08007834(CARD_ID(CARD_WORD(*card))) != 0)
        c = 1;
    r = &gUnk_0201CF90;
    r->player = player;
    r->zone5 = sub_08008A44(player);
    r->zone8 = 0;
    r->f14 = c;
    r->f15 = d;
    r->f16 = 0;
    r->f19 = 0;
    r->f25 = 0;
    r->f26 = 0;
    r->cardId = CARD_ID(CARD_WORD(*card));
    r->h0C = e;
    sub_08007558(&r->card, card);
    r->kind = 4;
    r->h0C = e | 4;
    sub_08055A00();
}
void sub_08056094(int player, struct DuelCard *card, u16 c, u16 d)
{
    struct ActRec *r;

    if (sub_08008524(0, 0x47F) != 0 || sub_08008524(1, 0x47F) != 0)
        c = 1;
    if (sub_08007834(CARD_ID(CARD_WORD(*card))) != 0)
        c = 1;
    r = &gUnk_0201CF90;
    r->player = player;
    r->zone5 = sub_08008A44(player);
    r->zone8 = 0;
    r->f14 = c;
    r->f15 = 0;
    r->f16 = 0;
    r->f19 = 0;
    r->f25 = 0;
    r->f26 = 0;
    r->cardId = CARD_ID(CARD_WORD(*card));
    sub_08007558(&r->card, card);
    r->kind = 5;
    r->h0C = d | 4;
    sub_08055A00();
}
void sub_08046A74(int player);

/* The AI passes the packed tribute word directly in r3. The original callee
 * decodes only the low halfwords of that word and the fifth stack argument. */
void sub_080561A0(int player, int zone, int x, int packed, int faceUp)
{
    u16 y = packed;
    u16 z = faceUp;

    gUnk_0201CF90.player = player;
    gUnk_0201CF90.zone5 = x;
    gUnk_0201CF90.zone8 = zone;
    gUnk_0201CF90.f14 = 1;
    gUnk_0201CF90.f15 = z == 0;
    if (y != 0) {
        u8 lo = y;
        u8 hi = y >> 8;

        gUnk_0201CF90.f16 = lo & 7;
        gUnk_0201CF90.f19 = hi & 7;
        gUnk_0201CF90.f25 = lo >> 7;
        gUnk_0201CF90.f26 = hi >> 7;
        gUnk_0201CF90.f28 = (lo >> 4) & 1;
        gUnk_0201CF90.f29 = (hi >> 4) & 1;
    } else {
        gUnk_0201CF90.f16 = 0;
        gUnk_0201CF90.f19 = 0;
        gUnk_0201CF90.f25 = 0;
        gUnk_0201CF90.f26 = 0;
    }
    {
        /* FAKEMATCH: initialized address constraints and input barrier reproduce
         * the ROM register order without emitting instructions. */
        register u32 pl __asm__("r0");
        u32 off;
        register u32 stride __asm__("r2");
        pl = player & 1;
        off = zone * 4;
        stride = 0xD64;
        pl *= stride;
        off += pl;
        pl = 0x02019968;
        __asm__ volatile("" : : "r"(pl));
        off += pl;
        gUnk_0201CF90.cardId = CARD_ID(*(u32 *)off);
    }
    gUnk_0201CF90.kind = 6;
    gUnk_0201CF90.h0C = 5;
    sub_08046A74(player);
    sub_08055A00();
}
int sub_08056300(int a, u16 number)
{
    switch (number) {
    case 0x10:
    case 0x11:
    case 0x12:
    case 0x13:
    case 0x14:
        return 1;
    }
    if (a > 0) {
        switch (number) {
        case 0x14F:
        case 0x150:
        case 0x15B:
        case 0x29F:
        case 0x2DA:
        case 0x3BA:
        case 0x3F0:
        case 0x3F2:
        case 0x403:
        case 0x405:
        case 0x406:
        case 0x420:
        case 0x425:
        case 0x42C:
        case 0x49B:
            return 1;
        }
    }
    if (a > 1) {
        if (number == 0x39 || number == 0x1A3)
            return 1;
    }
    return 0;
}

int sub_080563B8(int skip, u16 flag)
{
    int i;
    int best;
    int bestIdx;

    for (i = 0; i <= 4; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != skip && CARD_NUMBER(id) == 0x2F)
            return i;
    }
    for (i = 0; i <= 4; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != skip && CARD_NUMBER(id) == 0x23D)
            return i;
    }
    best = 32000;
    bestIdx = -1;
    for (i = 0; i <= 4; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != skip) {
            struct ZoneCardInfo info;
            int ok = 1;
            int v;

            if (sub_08007994(id))
                ok = 0;
            if (ok == 0 && flag == 0)
                continue;
            sub_0800ABC8(1, i, &info);
            v = info.unk4 * 2 + info.unk8;
            if (v < best) {
                bestIdx = i;
                best = v;
            }
        }
    }
    return bestIdx;
}
int sub_080564A8(u16 id)
{
    s8 lvl;
    int a, b;

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        lvl = 0;
        break;
    case 0x18:
        lvl = 10;
        break;
    default:
        lvl = (CARD_STATS(id) & 0x1E000000) >> 25;
        break;
    }
    if (lvl < 0)
        goto slow;
    if (lvl <= 4)
        return 1;
    if (lvl > 6)
        goto slow;
    {
        int none = -1;
        if (sub_080563B8(none, 0) != none)
            return 1;
        return 0;
    }
slow:
    {
        int none = -1;
        a = sub_080563B8(none, 0);
        b = sub_080563B8(a, 0);
        if (a == none)
            return 0;
        if (b == none)
            return 0;
    }
    return 1;
}
int sub_08056544(int skip)
{
    int i;
    int best;
    int bestIdx;
    struct ZoneCardInfo info;

    for (i = 0; i <= 4; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && (u16)(CARD_NUMBER(id) - 0x780) <= 0x4F)
            return i;
    }
    for (i = 0; i <= 4; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != skip) {
            *(u16 *)&CARD_NUMBER(id) = 0x2F;
            return i;
        }
    }
    for (i = 0; i <= 4; i++) {
        u16 id = CARD_ID(ZONE1_WORD(i));
        if (id != 0 && i != skip) {
            *(u16 *)&CARD_NUMBER(id) = 0x23D;
            return i;
        }
    }
    best = 99999;
    bestIdx = -1;
    for (i = 0; i <= 4; i++) {
        if (CARD_ID(ZONE1_WORD(i)) != 0 && i != skip) {
            int v;
            sub_0800ABC8(1, i, &info);
            v = info.unk4 + info.unk8;
            if (best > v) {
                bestIdx = i;
                best = v;
            }
        }
    }
    return bestIdx;
}
/* Pick the hand card of `player` with the lowest ATK+DEF value (in three passes of decreasing
 * strictness: first only cards whose "kind" key is 0, then any that passes sub_08056300, then
 * even empty slots); returns the hand index or -1. */
static inline int WeakestValue(u16 id)
{
    register int r asm("r2");

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        r = ((CARD_STATS(id) << 14) >> 23) * 10;
        break;
    }
    return r;
}

static inline u8 WeakestKey(u16 id)
{
    int number = ({ u32 offset = (id & 0x7FF) * 2; register u32 base asm("r4") = 0x8622ab4; asm("" : : "r"(base)); *(const u16 *)(offset + base); });

    switch (number) {
    case 0x776:
        return 3;
    case 0x777:
    case 0x778:
        return 1;
    }
    switch ((int)((({ u32 offset = (id & 0x7FF) * 4; register u32 base asm("r1") = 0x8621de0; asm("" : : "r"(base)); *(const u32 *)(offset + base); }) & 0x1F00000) >> 20)) {
    case 0x16:
        return 7;
    case 0x15:
        return 8;
    case 0x17:
        return 9;
    }
    return (({ u32 offset = (id & 0x7FF) * 4; register u32 base asm("r3") = 0x8621de0; asm("" : : "r"(base)); *(const u32 *)(offset + base); }) & 0xC0000) >> 18;
}

#if 0 /* NONMATCHING: 13 bytes and 26 normalized lines differ. The ROM loads the hoisted table constants into r4 and then copies them (ldr r4; adds r6,r4), while the build uses r0. */
int sub_0805664C(struct DuelPlayer *duel, int player)
{
    int bestIdx = -1;
    int best = 9999;
    int i;

    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[player & 1].hand[i]));
        if (id != 0) {
            if (((CARD_STATS(id) & 0x1F00000) >> 20) <= 0x14) {
                if (WeakestKey(id) == 0 && (u16)sub_08056300(1, ({ u32 offset = (id & 0x7FF) * 2; u32 base = 0x8622ab4; asm("" : : "r"(base)); *(const u16 *)(offset + base); })) == 0) {
                    int attack = ({
    register int r asm("r2");

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        { u32 offset = (id & 0x7FF) * 4; register u32 base asm("r4") = 0x08621DE0; register u32 bits asm("r1"); bits = (*(const u32 *)(offset + base) << 14) >> 23;
            { register u32 scaled asm("r0") = bits * 5; asm("" : : "r"(scaled), "r"(base)); r = scaled * 2; } }
        break;
    }
    r;
});
                    int v = ({ int defense;
      switch ((int)CARD_TYPE(id)) {
      case 0x15: case 0x16: case 0x17: defense = 0; break;
      case 0x18: defense = 4000; break;
      default: {
        u32 offset = (id & 0x7FF) * 4;
        register u32 base asm("r4") = 0x08621DE0;
        u32 word;
        register u32 seed asm("r3");
        u32 mask;
        asm("" : : "r"(base));
        word = *(const u32 *)(offset + base);
        seed = 0x1FF; mask = seed;
        asm("" : : "r"(seed));
        defense = (word & mask) * 10; break;
      }} defense; }) + attack;
                    if (best > v) {
                        best = v;
                        bestIdx = i;
                    }
                }
            }
        }
    }
    { register int result asm("r4") = bestIdx;
      if (result >= 0) return result; }
    best = 9999;
    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        u32 off;
        u32 base = (u32)(duel + (player & 1));
        u16 id;
        
        off = i * 4 + 0x684;
        
        id = CARD_ID(*(u32 *)(base + off));
        if (id != 0) {
            const u32 *st = &CARD_STATS(id);
            if (((*st & 0x1F00000) >> 20) <= 0x14 && (u16)sub_08056300(1, CARD_NUMBER(id)) == 0) {
                int attack = ({
    register int r asm("r2");

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        { u32 offset = (id & 0x7FF) * 4; u32 base = 0x08621DE0; register u32 bits asm("r1");  bits = (*(const u32 *)(offset + base) << 14) >> 23;
            { register u32 scaled asm("r0") = bits * 5; asm("" : : "r"(scaled), "r"(base)); r = scaled * 2; } }
        break;
    }
    r;
});
                    int v = ({ int defense;
      switch ((int)CARD_TYPE(id)) {
      case 0x15: case 0x16: case 0x17: defense = 0; break;
      case 0x18: defense = 4000; break;
      default: {
        u32 offset = (id & 0x7FF) * 4;
        register u32 base asm("r4") = 0x08621DE0;
        u32 word;
        register u32 seed asm("r4");
        u32 mask;
        asm("" : : "r"(base));
        word = *(const u32 *)(offset + base);
        seed = 0x1FF; mask = seed;
        asm("" : : "r"(seed));
        defense = (word & mask) * 10; break;
      }} defense; }) + attack;
                if (best > v) {
                    best = v;
                    bestIdx = i;
                }
            }
        }
    }
    { register int result asm("r0") = bestIdx;
      if (result >= 0) return result; }
    { register int initial asm("r1") = 9999; asm("" : : "r"(initial)); best = initial; }
    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        u32 off;
        u32 base = (player & 1) * 0xD64 + ({ register u32 p asm("r0") = (u32)duel;  p; });
        u16 id;
        
        off = i * 4 + 0x684;
        
        id = CARD_ID(*(u32 *)(base + off));
        if ((u16)sub_08056300(1, CARD_NUMBER(id)) == 0) {
            int attack = ({
    register int r asm("r2");

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 4000;
        break;
    default:
        { u32 offset = (id & 0x7FF) * 4; register u32 base asm("r4") = 0x08621DE0; register u32 bits asm("r1"); bits = (*(const u32 *)(offset + base) << 14) >> 23;
            { register u32 scaled asm("r0") = bits * 5; asm("" : : "r"(scaled), "r"(base)); r = scaled * 2; } }
        break;
    }
    r;
});
                    int v = ({ int defense;
      switch ((int)CARD_TYPE(id)) {
      case 0x15: case 0x16: case 0x17: defense = 0; break;
      case 0x18: defense = 4000; break;
      default: {
        u32 offset = (id & 0x7FF) * 4;
        register u32 base asm("r4") = (u32)gUnk_08621DE0;
        u32 word;
        register u32 seed asm("r4");
        u32 mask;
        
        word = *(const u32 *)(offset + base);
        seed = 0x1FF; mask = seed;
        asm("" : : "r"(seed));
        defense = (word & mask) * 10; break;
      }} defense; }) + attack;
            if (best > v) {
                best = v;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08055EB0", sub_0805664C); /* 0x0805664C size 0x448 */

/* Picks the hand card index of `player` (duel is the base of the two DuelPlayers) to play: the
 * strongest (by CardValue) monster-like card that passes sub_08056300 and has a level above 4.
 * If none qualifies, picks the strongest without the level test. Returns -1 if there is none. */
int sub_08056A94(struct DuelPlayer *duel, int player)
{
    int bestIdx = -1;
    int best = -1;
    int i;

    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        struct DuelPlayer *pd = duel + (player & 1);
        u32 off = i * 4 + 0x684;
        u16 id = CARD_ID(*(u32 *)((u32)pd + off));
        if (id != 0) {
            const u32 *st = &CARD_STATS(id);
            if (((*st & 0x1F00000) >> 20) <= 0x14 && sub_08007834(id) == 0
                && (u16)sub_08056300(1, CARD_NUMBER(id)) == 0) {
                int v = CardValue(id);
                if (best < v) {
                    u32 lvl;
                    CARD_LEVEL(id, lvl);
                    if (lvl > 4) {
                        best = v;
                        bestIdx = i;
                    }
                }
            }
        }
    }
    if (bestIdx >= 0)
        return bestIdx;
    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        struct DuelPlayer *pd = duel + (player & 1);
        u32 off = i * 4 + 0x684;
        u16 id = CARD_ID(*(u32 *)((u32)pd + off));
        if (id != 0) {
            const u32 *st = &CARD_STATS(id);
            if (((*st & 0x1F00000) >> 20) <= 0x14 && sub_08007834(id) == 0
                && (u16)sub_08056300(1, CARD_NUMBER(id)) == 0) {
                int v = CardValue(id);
                if (best < v) {
                    best = v;
                    bestIdx = i;
                }
            }
        }
    }
    return bestIdx;
}
int sub_0800A304(int player, u16 number);
int sub_0805930C(u16 number);
int sub_08008A1C(int player);
int sub_0805664C(struct DuelPlayer *duel, int player);
int sub_08056CE8(void)
{
    int r = -1;
    int i;

    if (sub_0800A304(1, 0x3F0) > r || sub_0800A304(1, 0x488) > r || sub_0805930C(0x447) != 0) {
        if (sub_08008A1C(1) > 0) {
            r = sub_08056A94(gUnk_020192E4, 1);
            if (r >= 0)
                return r;
        }
    }
    for (i = 0; i < gUnk_020192E4[1].handCount; i++) {
        if (CARD_NUMBER(CARD_ID(CARD_WORD(gUnk_020192E4[1].hand[i]))) == 0x1DA)
            return i;
    }
    return sub_0805664C(gUnk_020192E4, 1);
}

/* First index of the list whose card number is `number`, or -1. */
int sub_08056D98(int player, u16 number, int limit)
{
    int i;

    for (i = 0; i < gUnk_020192E4[player & 1].deckCount && i < limit; i++) {
        u32 idx = CARD_ID(CARD_WORD(gUnk_020192E4[player & 1].deck[i])) & 0x7FF;
        if (gUnk_08622AB4[idx] == number)
            return i;
    }
    return -1;
}
/* Signed table entries arrive as words; preserve the original low-half decode. */
int sub_08056E04(int player, int numberWord)
{
    u16 number = numberWord;
    int i;

    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        u32 idx = CARD_ID(CARD_WORD(gUnk_020192E4[player & 1].hand[i])) & 0x7FF;
        if (gUnk_08622AB4[idx] == number)
            return i;
    }
    return -1;
}
int sub_08056E68(int player, u16 number)
{
    int i;

    for (i = 0; i < gUnk_020192E4[player & 1].fusionCount; i++) {
        u32 idx = CARD_ID(CARD_WORD(gUnk_020192E4[player & 1].fusionDeck[i])) & 0x7FF;
        if (gUnk_08622AB4[idx] == number)
            return i;
    }
    return -1;
}
