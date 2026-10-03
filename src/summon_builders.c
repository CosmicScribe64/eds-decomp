extern const unsigned int gCardStats[];
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
extern struct DuelPlayer gDuelPlayers[2];

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); callers pass player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gDuelZones))

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
/* ROM tables through integer-constant pointers (the ROM reloads the table address at every use). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Card word of player 1's zone i (zones start at 0x0201930C + 0xD64). */
extern u8 gDuelZonesP1[];
#define ZONE1_WORD(i) (*(u32 *)((i) * 0x94 + (u32)gDuelZonesP1))

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

/* Same with the DEF-like field (bits 0-8) for ordinary monsters. The ROM masks the low halfword: the u16
 * AND makes 0x1FF a halfword constant, which reload loads and copies (ldr r3, =0x1FF; adds r0, r3). */
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
        r = ((u16)CARD_STATS(id) & 0x1FF) * 10;
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
void GetZoneCardStats(u32 player, u32 slot, struct ZoneCardInfo *out);
int IsFusionMonster(u32 id);
int AiPickTributeMonster(int skip, u16 flag);
extern const u16 gCardIdToNumber[];

/* Duel action record at 0x0201CF90 (0x10 bytes), filled in and handed to SummonAction_Start. */
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
extern struct ActRec gSummonAction;
void SummonAction_Start(void);
void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);

void QueueFlipSummon(int player, int zone)
{
    struct ActRec *r = &gSummonAction;

    r->player = player;
    r->zone5 = zone;
    r->zone8 = zone;
    r->f14 = 1;
    r->f15 = 0;
    r->f16 = 0;
    r->f19 = 0;
    r->f25 = 0;
    r->f26 = 0;
    r->cardId = CARD_ID(*(u32 *)((player & 1) * 0xD64 + zone * 0x94 + (u32)gDuelZones));
    r->kind = 3;
    r->h0C = 3;
    SummonAction_Start();
}
int CountActiveCardsOnField(int player, u16 number);
int IsSpecialSummonOnly(u16 id);
int FindFreeMonsterZone(int player);

void QueueSpecialSummon(int player, struct DuelCard *card, u16 c, u16 d, u16 e)
{
    struct ActRec *r;

    if (CountActiveCardsOnField(0, 0x47F) != 0 || CountActiveCardsOnField(1, 0x47F) != 0)
        c = 1;
    if (IsSpecialSummonOnly(CARD_ID(CARD_WORD(*card))) != 0)
        c = 1;
    r = &gSummonAction;
    r->player = player;
    r->zone5 = FindFreeMonsterZone(player);
    r->zone8 = 0;
    r->f14 = c;
    r->f15 = d;
    r->f16 = 0;
    r->f19 = 0;
    r->f25 = 0;
    r->f26 = 0;
    r->cardId = CARD_ID(CARD_WORD(*card));
    r->h0C = e;
    CopyDuelCard(&r->card, card);
    r->kind = 4;
    r->h0C = e | 4;
    SummonAction_Start();
}
void QueueSpecialSummonChoosePosition(int player, struct DuelCard *card, u16 c, u16 d)
{
    struct ActRec *r;

    if (CountActiveCardsOnField(0, 0x47F) != 0 || CountActiveCardsOnField(1, 0x47F) != 0)
        c = 1;
    if (IsSpecialSummonOnly(CARD_ID(CARD_WORD(*card))) != 0)
        c = 1;
    r = &gSummonAction;
    r->player = player;
    r->zone5 = FindFreeMonsterZone(player);
    r->zone8 = 0;
    r->f14 = c;
    r->f15 = 0;
    r->f16 = 0;
    r->f19 = 0;
    r->f25 = 0;
    r->f26 = 0;
    r->cardId = CARD_ID(CARD_WORD(*card));
    CopyDuelCard(&r->card, card);
    r->kind = 5;
    r->h0C = d | 4;
    SummonAction_Start();
}
void PayChainEnergyCost(int player);

/* The AI passes the packed tribute word directly in r3. The original callee
 * decodes only the low halfwords of that word and the fifth stack argument. */
void QueueSpecialSummonFromHand(int player, int zone, int x, int packed, int faceUp)
{
    u16 y = packed;
    u16 z = faceUp;

    gSummonAction.player = player;
    gSummonAction.zone5 = x;
    gSummonAction.zone8 = zone;
    gSummonAction.f14 = 1;
    gSummonAction.f15 = z == 0;
    if (y != 0) {
        u8 lo = y;
        u8 hi = y >> 8;

        gSummonAction.f16 = lo & 7;
        gSummonAction.f19 = hi & 7;
        gSummonAction.f25 = lo >> 7;
        gSummonAction.f26 = hi >> 7;
        gSummonAction.f28 = (lo >> 4) & 1;
        gSummonAction.f29 = (hi >> 4) & 1;
    } else {
        gSummonAction.f16 = 0;
        gSummonAction.f19 = 0;
        gSummonAction.f25 = 0;
        gSummonAction.f26 = 0;
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
        gSummonAction.cardId = CARD_ID(*(u32 *)off);
    }
    gSummonAction.kind = 6;
    gSummonAction.h0C = 5;
    PayChainEnergyCost(player);
    SummonAction_Start();
}
int AiIsKeyCard(int a, u16 number)
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

int AiPickTributeMonster(int skip, u16 flag)
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

            if (IsFusionMonster(id))
                ok = 0;
            if (ok == 0 && flag == 0)
                continue;
            GetZoneCardStats(1, i, &info);
            v = info.unk4 * 2 + info.unk8;
            if (v < best) {
                bestIdx = i;
                best = v;
            }
        }
    }
    return bestIdx;
}
int AiHasTributesFor(u16 id)
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
        if (AiPickTributeMonster(none, 0) != none)
            return 1;
        return 0;
    }
slow:
    {
        int none = -1;
        a = AiPickTributeMonster(none, 0);
        b = AiPickTributeMonster(a, 0);
        if (a == none)
            return 0;
        if (b == none)
            return 0;
    }
    return 1;
}
int AiPickEffectTribute(int skip)
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
            GetZoneCardStats(1, i, &info);
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
 * strictness: first only cards whose "kind" key is 0, then any that passes AiIsKeyCard, then
 * even empty slots); returns the hand index or -1. */
/* ATK-like value as a statement, so every arm writes the result local itself (CardValue's return copy
 * gives the ROM an extra move). */
#define CARD_ATK_VALUE(id, r)                                \
    switch ((int)CARD_TYPE(id)) {                            \
    case 0x15:                                               \
    case 0x16:                                               \
    case 0x17:                                               \
        r = 0;                                               \
        break;                                               \
    case 0x18:                                               \
        r = 4000;                                            \
        break;                                               \
    default:                                                 \
        r = ((CARD_STATS(id) << 14) >> 23) * 10;             \
        break;                                               \
    }

int AiPickWeakestHandCard(struct DuelPlayer *duel, int player)
{
    int bestIdx = -1;
    int best = 9999;
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));
        if (id != 0) {
            if (CARD_TYPE(id) <= 0x14 && CardKey(id) == 0 && (u16)AiIsKeyCard(1, CARD_NUMBER(id)) == 0) {
                int atk;
                int v;

                CARD_ATK_VALUE(id, atk);
                v = CardDefValue(id) + atk;
                if (best > v) {
                    best = v;
                    bestIdx = i;
                }
            }
        }
    }
    if (bestIdx >= 0)
        return bestIdx;
    best = 9999;
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        struct DuelPlayer *pd = duel + (player & 1);
        u32 off = i * 4 + 0x684;
        u16 id = CARD_ID(*(u32 *)((u32)pd + off));
        if (id != 0) {
            const u32 *st = &CARD_STATS(id);
            if (((*st & 0x1F00000) >> 20) <= 0x14 && (u16)AiIsKeyCard(1, CARD_NUMBER(id)) == 0) {
                int atk;
                int v;

                CARD_ATK_VALUE(id, atk);
                v = CardDefValue(id) + atk;
                if (best > v) {
                    best = v;
                    bestIdx = i;
                }
            }
        }
    }
    if (bestIdx >= 0)
        return bestIdx;
    best = 9999;
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        struct DuelPlayer *pd = duel + (player & 1);
        u32 off = i * 4 + 0x684;
        u16 id = CARD_ID(*(u32 *)((u32)pd + off));
        if ((u16)AiIsKeyCard(1, CARD_NUMBER(id)) == 0) {
            int atk;
            int v;

            CARD_ATK_VALUE(id, atk);
            v = CardDefValue(id) + atk;
            if (best > v) {
                best = v;
                bestIdx = i;
            }
        }
    }
    return bestIdx;
}

/* Picks the hand card index of `player` (duel is the base of the two DuelPlayers) to play: the
 * strongest (by CardValue) monster-like card that passes AiIsKeyCard and has a level above 4.
 * If none qualifies, picks the strongest without the level test. Returns -1 if there is none. */
int AiPickStrongestHandMonster(struct DuelPlayer *duel, int player)
{
    int bestIdx = -1;
    int best = -1;
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        struct DuelPlayer *pd = duel + (player & 1);
        u32 off = i * 4 + 0x684;
        u16 id = CARD_ID(*(u32 *)((u32)pd + off));
        if (id != 0) {
            const u32 *st = &CARD_STATS(id);
            if (((*st & 0x1F00000) >> 20) <= 0x14 && IsSpecialSummonOnly(id) == 0
                && (u16)AiIsKeyCard(1, CARD_NUMBER(id)) == 0) {
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
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        struct DuelPlayer *pd = duel + (player & 1);
        u32 off = i * 4 + 0x684;
        u16 id = CARD_ID(*(u32 *)((u32)pd + off));
        if (id != 0) {
            const u32 *st = &CARD_STATS(id);
            if (((*st & 0x1F00000) >> 20) <= 0x14 && IsSpecialSummonOnly(id) == 0
                && (u16)AiIsKeyCard(1, CARD_NUMBER(id)) == 0) {
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
int FindHandCardByNumber(int player, u16 number);
int AiHasUsableSpellTrap(u16 number);
int CountFreeMonsterZones(int player);
int AiPickWeakestHandCard(struct DuelPlayer *duel, int player);
int AiPickDiscard(void)
{
    int r = -1;
    int i;

    if (FindHandCardByNumber(1, 0x3F0) > r || FindHandCardByNumber(1, 0x488) > r || AiHasUsableSpellTrap(0x447) != 0) {
        if (CountFreeMonsterZones(1) > 0) {
            r = AiPickStrongestHandMonster(gDuelPlayers, 1);
            if (r >= 0)
                return r;
        }
    }
    for (i = 0; i < gDuelPlayers[1].handCount; i++) {
        if (CARD_NUMBER(CARD_ID(CARD_WORD(gDuelPlayers[1].hand[i]))) == 0x1DA)
            return i;
    }
    return AiPickWeakestHandCard(gDuelPlayers, 1);
}

/* First index of the list whose card number is `number`, or -1. */
int FindDeckCardByNumber(int player, u16 number, int limit)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].deckCount && i < limit; i++) {
        u32 idx = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].deck[i])) & 0x7FF;
        if (gCardIdToNumber[idx] == number)
            return i;
    }
    return -1;
}
/* Signed table entries arrive as words; preserve the original low-half decode. */
int AiFindHandCardByNumber(int player, int numberWord)
{
    u16 number = numberWord;
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u32 idx = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i])) & 0x7FF;
        if (gCardIdToNumber[idx] == number)
            return i;
    }
    return -1;
}
int FindFusionDeckCardByNumber(int player, u16 number)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].fusionCount; i++) {
        u32 idx = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].fusionDeck[i])) & 0x7FF;
        if (gCardIdToNumber[idx] == number)
            return i;
    }
    return -1;
}
