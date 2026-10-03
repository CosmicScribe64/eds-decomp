#include "global.h"

/* CPU turn step handlers (see ai_steps): gAiState is the AiState record,
 * +1 is the step index into the table gAiSteps, +2..+5 sub-step counters,
 * +6 a loop counter over the card-number table gAiSimpleSpells, +0xA a phase byte. */
struct AiState {
    u8 f0;
    u8 step;    /* +1: index into gAiSteps */
    u8 f2;      /* +2: step sub-state */
    u8 f3;      /* +3: hand index */
    u8 f4;
    u8 f5;      /* +5: zone index */
    u8 f6;      /* +6: index into gAiSimpleSpells */
    u8 f7;
    u8 f8;
    u8 f9;
    u8 phase;   /* +0xA */
    u8 fB;
};
extern struct AiState gAiState;
extern u8 gAiWork[];  /* AI work area, 0x1B28 bytes */
extern void MemClear16(void *dst, u32 size);
extern void DuelCmd_Push(u16 msg, u16 a, int b, int c);
extern int AiChooseStrategy(void);

/* One zone of a player (0x94 bytes). */
struct DuelZone {
    u32 card;       /* +0: card word, card id in the low 12 bits */
    u8 unk4[2];
    u8 flags6;      /* +6: bit 1 = face up */
    u8 unk7[0x8A];
    u8 f91;         /* +0x91: bit 2 */
    u8 unk92[2];
};

/* Per-player duel state (0xD64 bytes each at 0x020192E4); player 1 = the CPU. */
struct DuelPlayer {
    u16 lp;         /* +0x000 life points (hypothesis) */
    u8 handCount;   /* +0x002 */
    u8 deckCount;   /* +0x003 */
    u8 pad4[0x28 - 4];
    struct DuelZone zones[11];  /* +0x028 (0x0201930C for player 0) */
    u32 hand[80];   /* +0x684: u32 card words, card id = (w << 20) >> 20 */
    u8 pad7C4[0xD64 - 0x7C4];
};
extern struct DuelPlayer gDuelPlayers[2];

extern u8 gDuelZonesP1[];          /* player 1 zones */
extern u8 gDuelZones[];          /* player 0 zones (== 0x020192E4 + 0x28) */
extern u32 gDuelHandP1[];         /* player 1 hand (== 0x0201A048 + 0x684) */
extern u8 gUnk_0201A04A;            /* player 1 hand count (== 0x0201A048 + 2) */
extern const u16 gCardIdToNumber[];   /* maps card ID to card number */
extern const u32 gCardStats[];   /* card stats: type = bits 20-24, spell subtype = bits 17-19 */
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_NUMBER(id) (*((const u16 *)(((id) & 0x7FF) + gCardIdToNumber)))
#define ZONE(z) ((struct DuelZone *)(gDuelZonesP1 + (z) * 0x94))
extern u32 HasFlipEffect(u16 cardNo, u16 flag);
extern u16 CountMonsters(int player);
extern int CountActiveCardsOnField2(int player, u16 number);
extern int CountSpellTraps(int player);
extern int CountActivatableSetCards(int player, int number);
extern int CountGraveyardCardsOfType(int player, u16 type);
extern void QueueFlipSummon(int player, int zone);
extern int FindFreeSpellTrapZone(int player);
extern int CountHandCardsByNumber(int player, u16 number);
extern void PayChainEnergyCost(int player);

extern int CountFreeMonsterZones(int player);
extern int AiCountExodiaOnField(void);
extern int AiCountExodiaInDeck(void);
extern int FindHandCardByNumber(int player, u16 number);
extern int CountMonstersByNumber(int player, u16 number);
extern int GetZoneCardAtk(int player, int zone);
extern int CanSummonFromHand(int player, u16 id);
extern int AiIsKeyCard(int a, u16 number);
extern int IsSpecialSummonOnly(u16 id);
extern int AiHasTributesFor(u16 id);
extern u32 gDuelCtrl[];
/* Player 1's hand word i. The integer sum computes i << 2 before the symbol load, so the hand
 * address is a short-lived pseudo (not a reload) and loop.c leaves it in the loop. */
#define A30C_HAND(i) (*(u32 *)(((i) << 2) + (u32)gDuelHandP1))
/* Player 0's deck word i (0x020192E4 + 0x7C4): a constant address, reloaded inside the loop. */
#define A30C_DECK(i) (((u32 *)0x02019AA8)[i])
/* CountMonsters returns int (the unit header says u16, which adds narrowing). */
#define A30C_COUNT(p) (((int (*)(int))CountMonsters)(p))
/* FAKEMATCH: the ROM calls CountActivatableSetCards without setting r1 (one-argument call through a cast). */
#define A30C_9280(p) (((int (*)(int))CountActivatableSetCards)(p))
#define A30C_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define A30C_TYPE(id) ((A30C_STATS(id) & 0x1F00000) >> 20)
#define A30C_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

/* DEF-like value: 0 for types 0x15-0x17, 4000 for 0x18, else (stats & 0x1FF) * 10. The u16 AND makes
 * 0x1FF a halfword constant (ldr r2; adds r0, r2); the u16 return gives the r0 result + copy. */
static inline u16 A30C_Def(u16 id)
{
    int r;

    switch ((int)A30C_TYPE(id)) {
    case 21:
    case 22:
    case 23:
        r = 0;
        break;
    case 24:
        r = 4000;
        break;
    default:
        r = ((u16)A30C_STATS(id) & 0x1FF) * 10;
        break;
    }
    return r;
}

/* Level: 0 for types 0x15-0x17, 10 for 0x18, else bits 25-28. The u8 return adds RTL that combine
 * removes later; it keeps the second hand loop big enough that loop.c does not hoist the count address. */
static inline u8 A30C_Level(u16 id)
{
    switch ((int)A30C_TYPE(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return (A30C_STATS(id) & 0x1E000000) >> 25;
    }
}

/* Phase-10 hand choice for the CPU (player 1): scripted checks pick a hand index for specific card
 * numbers via FindHandCardByNumber(1, number); otherwise the best DEF-like value >= the opponent's strongest
 * zone value (GetZoneCardAtk) passing the playability filters, then the first level <= 4 monster that
 * passes them. Returns the hand index, or -1. Some checks use idx > -1 and others idx >= 0, as in the ROM. */
int AiPickMonsterToSet(void)
{
    int idx;
    int i;
    int max, best, w;
    u16 id;

    if (CountFreeMonsterZones(1) == 0)
        return -1;
    if (gDuelCtrl[1] & 0x200) {
        if (AiCountExodiaOnField() != 0) {
            idx = FindHandCardByNumber(1, 0x259);
            if (idx >= 0)
                return idx;
        }
        if (AiCountExodiaInDeck() != 0) {
            idx = FindHandCardByNumber(1, 0x2F);
            if (idx > -1)
                return idx;
            idx = FindHandCardByNumber(1, 0x23D);
            if (idx > -1)
                return idx;
            if (CountMonstersByNumber(1, 0x2F) > 0 || CountMonstersByNumber(1, 0x23D) > 0) {
                idx = FindHandCardByNumber(1, 0x23D);
                if (idx > -1)
                    return idx;
            }
            idx = FindHandCardByNumber(1, 0x463);
            if (idx >= 0)
                return idx;
        }
    }
    if (A30C_COUNT(0) > 1) {
        idx = FindHandCardByNumber(1, 0x259);
        if (idx >= 0)
            return idx;
    }
    if (A30C_COUNT(0) > 0) {
        idx = FindHandCardByNumber(1, 0x1F4);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0x21C);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0x259);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0xFF);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0x419);
        if (idx > -1)
            return idx;
    }
    if (CountGraveyardCardsOfType(1, 0x16) > 0) {
        idx = FindHandCardByNumber(1, 0x1AB);
        if (idx >= 0)
            return idx;
    }
    if (CountGraveyardCardsOfType(1, 0x15) > 0) {
        idx = FindHandCardByNumber(1, 0x65);
        if (idx >= 0)
            return idx;
    }
    if (gDuelPlayers[1].handCount <= 2 || gDuelPlayers[0].handCount > gDuelPlayers[1].handCount + 2) {
        idx = FindHandCardByNumber(1, 0x21B);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0x24E);
        if (idx > -1)
            return idx;
    }
    if (gDuelPlayers[0].deckCount <= 4) {
        idx = FindHandCardByNumber(1, 0x231);
        if (idx >= 0)
            return idx;
    }
    for (i = 0; i <= 4; i++) {
        switch (CARD_NUMBER(CARD_ID(A30C_DECK(i)))) {
        case 0x10:
        case 0x11:
        case 0x12:
        case 0x13:
        case 0x14:
        case 0x14F:
        case 0x150:
        case 0x1DA:
        case 0x290:
        case 0x29F:
        case 0x2EF:
        case 0x3AB:
        case 0x3C8:
        case 0x3F0:
        case 0x3F2:
        case 0x403:
        case 0x420:
        case 0x42C:
            idx = FindHandCardByNumber(1, 0x231);
            if (idx >= 0)
                goto ret;
            break;
        }
    }
    if (CountActiveCardsOnField2(0, 0x15B) > 0) {
        idx = FindHandCardByNumber(1, 0x246);
        if (idx >= 0)
            return idx;
    }
    if (CountActiveCardsOnField2(0, 0x148) > 0) {
        idx = FindHandCardByNumber(1, 0x27);
        if (idx >= 0)
            return idx;
    }
    if (CountSpellTraps(0) > 1) {
        idx = FindHandCardByNumber(1, 0x109);
        if (idx >= 0)
            return idx;
    }
    if (A30C_9280(0) > 1) {
        idx = FindHandCardByNumber(1, 0x249);
        if (idx >= 0)
            return idx;
    }
    if (A30C_9280(0) > 0) {
        idx = FindHandCardByNumber(1, 0xDF);
        if (idx >= 0)
            return idx;
    }
    if (A30C_COUNT(0) > 0 && A30C_COUNT(1) == 0) {
        idx = FindHandCardByNumber(1, 0x48B);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0x452);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0x45A);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0x45B);
        if (idx > -1)
            return idx;
        idx = FindHandCardByNumber(1, 0x51B);
        if (idx > -1)
            return idx;
    }
    max = 0;
    for (i = 0; i <= 4; i++) {
        int v = GetZoneCardAtk(0, i);
        if (max < v)
            max = v;
    }
    best = 0;
    idx = -1;
    for (i = 0; i < gDuelPlayers[1].handCount; i++) {
        id = CARD_ID(A30C_HAND(i));
        w = A30C_Def(id);
        if (best < w && max <= w && CanSummonFromHand(1, id) != 0
            && AiIsKeyCard(2, A30C_NUMBER(id)) == 0 && IsSpecialSummonOnly(id) == 0
            && AiHasTributesFor(id) != 0) {
            idx = i;
            best = w;
        }
    }
    if (idx >= 0) {
    ret:
        return idx;
    }
    for (i = 0; i < gDuelPlayers[1].handCount; i++) {
        u16 id2 = CARD_ID(A30C_HAND(i));
        if (A30C_Level(id2) <= 4 && A30C_TYPE(id2) <= 0x14
            && AiIsKeyCard(2, A30C_NUMBER(id2)) == 0 && IsSpecialSummonOnly(id2) == 0
            && AiHasTributesFor(id2) != 0)
            return i;
    }
    return -1;
}
/* Step 0: reset the AI work area, announce the turn and ask the scripted-strategy
 * picker (AiChooseStrategy). When one applies, jump to step 8, else let the runner
 * advance to step 1. */
int AiStepStartMainPhase(void)
{
    MemClear16(gAiWork, 0x1B28);
    DuelCmd_Push(0x8052, 0, 0, 0);
    if (AiChooseStrategy() != 0) {
        gAiState.step = 8;
        gAiState.f2 = 0;
        gAiState.f3 = 0;
        gAiState.f4 = 0;
        gAiState.f5 = 0;
        return 0;
    }
    return 1;
}


/* Scan the CPU hand, test opponent spell/trap targets, then queue a play.
 * FAKEMATCH: all register bindings and empty constraints below are initialized
 * lifetime/scheduling hints. Field and hand loop masks are distinct from the
 * outer mask; bound caller-saved values are dead before external calls.
 * Type bits are 20..24 (mask 0x01F00000). */
int AiStepSetSpellTraps(void)
{
    struct AiState *initial = &gAiState;
    u32 phase = initial->f2;
    register struct AiState *p asm("r9");
    p = initial;
    switch (phase) {
    case 0:
        {
            struct AiState *q = p;
            q->f3 = phase;
            {
                register u32 next asm("r0") = q->f2 + 1;
                struct AiState *nextp = p;
                nextp->f2 = next;
            }
            break;
        }
    case 1:
        break;
    default:
        return 1;
    }
    {
        u32 count;
        struct AiState *q;
        {
            register u32 base asm("r0") = (u32)&gDuelPlayers;
            register u32 off asm("r2") = 0xD66;
            asm("" : : "r"(base), "r"(off));
            count = *(u8 *)(base + off);
        }
        if (count == 0) goto done;
        q = p;
        {
            register u32 index asm("r3") = q->f3;
            asm("" : : "r"(index));
            if (index >= count) goto done;
        }
        {
            register u32 seed asm("r4") = 0x7FF;
            u32 lookupMask;
            asm("" : : "r"(seed));
            lookupMask = seed;
        hand_next:
            {
                register u32 w asm("r8");
                int c;
                u16 ok;
                {
                    u32 index = q->f3;
                    u32 off = index << 2;
                    u32 table = (u32)gDuelHandP1;
                    u32 raw;
                    raw = *(u32 *)(off + table);
                    w = CARD_ID(raw);
                }
                c = FindFreeSpellTrapZone(1);
                ok = 0;
                if (c < 0) return 1;
                switch (({
                    u32 index = w;
                    register u32 mask asm("r6");
                    asm("" : : "r"(index));
                    mask = lookupMask;
                    asm("" : "+r"(mask));
                    ((const u16 *)0x08622AB4)[index & mask];
                }
                )) {
                case 0x3FB:
                    {
                        register int player asm("r0") = 0;
                        ok = CountHandCardsByNumber(player, 0x14F) > 0;
                    }
                    break;
                case 0x3FE:
                    {
                        register int player asm("r0") = 0;
                        ok = CountHandCardsByNumber(player, 0x150) > 0;
                    }
                    break;
                case 0x402:
                    {
                        int player = 0;
                        asm("" : : "r"(player));
                        ok = CountHandCardsByNumber(player, 0x3F0) > 0;
                    }
                    break;
                case 0x405:
                case 0x482:
                    {
                        int z = 5;
                        register struct DuelPlayer *base asm("r5") = &gDuelPlayers[0];
                        register u32 off asm("r2") = 0x28;
                        register u8 *zb asm("r12");
                        u32 mask;
                        asm("" : : "r"(z), "r"(base), "r"(off));
                        zb = (u8 *)((u32)base + off);
                        mask = 0x7FF;
                    field22:
                        {
                            u32 off = 0x94 * z;
                            u8 *basecopy = zb;
                            struct DuelZone *zp;
                            u32 id;
                            zp = (struct DuelZone *)(off + (u32)basecopy);
                            id = CARD_ID(zp->card);
                            if (id != 0) {
                                id &= mask;
                                if ((((const u32 *)0x08621DE0)[id] & 0x1F00000) >> 20 == 0x16 && !(zp->flags6 & 2)) ok = 1;
                            }
                        }
                        if (++z <= 9) goto field22;
                        if (base->handCount != 0) {
                            u32 mask = 0x7FF;
                            register struct DuelPlayer *cur asm("r0") = &gDuelPlayers[0];
                            register u32 off asm("r2") = 0x684;
                            register u32 *hp asm("r1");
                            register u32 typeMask asm("r2");
                            int n;
                            asm("" : : "r"(mask), "r"(cur), "r"(off));
                            hp = (u32 *)((u32)cur + off);
                            typeMask = 0x1F00000;
                            n = cur->handCount;
                        hand22:
                            {
                                u32 id = CARD_ID(*hp);
                                register u32 table asm("r6");
                                id &= mask;
                                id <<= 2;
                                table = 0x08621DE0;
                                asm("" : : "r"(table));
                                if ((*(u32 *)(id + table) & typeMask) >> 20 == 0x16) ok = 1;
                            }
                            hp++;
                            if (--n != 0) goto hand22;
                        }
                        break;
                    }
                case 0x406:
                case 0x409:
                    {
                        int z = 5;
                        u8 *zb = gDuelZones;
                        u32 mask = 0x7FF;
                    field21:
                        {
                            u32 off = 0x94 * z;
                            u8 *basecopy = zb;
                            struct DuelZone *zp;
                            u32 id;
                            zp = (struct DuelZone *)(off + (u32)basecopy);
                            id = CARD_ID(zp->card);
                            if (id != 0) {
                                u32 off;
                                register u32 table asm("r6");
                                id &= mask;
                                off = id << 2;
                                table = 0x08621DE0;
                                asm("" : : "r"(off), "r"(table));
                                if ((*(u32 *)(off + table) & 0x1F00000) >> 20 == 0x15 && !(zp->flags6 & 2)) ok = 1;
                            }
                        }
                        if (++z <= 9) goto field21;
                        break;
                    }
                case 0x4DD:
                case 0x522:
                    goto skip;
                default:
                    {
                        u32 index = w;
                        u32 mask;
                        u32 stats, ty;
                        asm("" : : "r"(index));
                        mask = lookupMask;
                        index &= mask;
                        stats = ((const u32 *)0x08621DE0)[index];
                        ty = (stats & 0x1F00000) >> 20;
                        if (ty == 0x15) goto doAction;
                        if (ty != 0x16) break;
                        if (((stats & 0xE0000) >> 17) == 5) ok = 1;
                        {
                            u32 off = index << 1;
                            register u32 table asm("r2") = 0x08622AB4;
                            asm("" : : "r"(off), "r"(table));
                            if (*(const u16 *)(off + table) == 0x136) ok = 1;
                        }
                        break;
                    }
                }
                if (!ok) goto skip;
            doAction:
                {
                    register u32 packed asm("r2");
                    PayChainEnergyCost(1);
                    {
                        struct AiState *state = &gAiState;
                        register u32 nibble asm("r2");
                        nibble = state->f3 & 15;
                        nibble <<= 4;
                        c &= 15;
                        packed = nibble | c;
                    }
                    DuelCmd_Push(0x80C5, w, packed, 0);
                    return 0;
                }
            skip:
                q = &gAiState;
                {
                    u32 next = q->f3 + 1;
                    register u8 *cp asm("r3");
                    q->f3 = next;
                    cp = &gUnk_0201A04A;
                    count = *cp;
                    if (count != 0 && (u8)next < count) goto hand_next;
                }
            }
        }
    }
done:
    {
        register struct AiState *last asm("r4") = p;
        last->f2++;
    }
    return 0;
}

struct AiCardRef {
    u16 id;
    u8 player : 1;
    u8 reserved2 : 7;
    u8 positionHigh : 2;
    u8 kind : 6;
    u32 data[4];
};
extern const s16 gAiSimpleSpells[];
extern const u16 gCardNumberToId[];
int CountFaceUpSpellTrapsOfType(int player, u16 type);
int AiFindHandCardByNumber(int player, int number);
int CountActiveCardsOnField(int player, u16 number);
int CanPlaceSpellTrapCard(int player, u16 id);
int CanActivateFieldCard(struct AiCardRef *, int player, int zone);
int CanActivateEffect(struct AiCardRef *, struct AiCardRef *, u16);
void Chain_AddPending(u32 action, u32 flags);
static inline u16 AiScanCardId(u16 number)
{
    if (number == 0xFFFF) return 0;
    if (number <= 0x7CF) return ((const u16 *)0x08623DF4)[number & 0x7FF];
    return ((const u16 *)0x08623DF4)[(number - 0x7D0) & 0x7FF] + 1;
}
/* Scan field/hand spell candidates, apply LP gates, then queue the selected action. */
int AiStepPlaySimpleSpells(void)
{
    struct AiCardRef ref;
    switch (gAiState.phase) {
    case 0:
        gAiState.f6 = 0;
        do {
            if (CountActivatableSetCards(1, gAiSimpleSpells[gAiState.f6])) {
                int ok = 0;
                switch (gAiSimpleSpells[gAiState.f6]) {
                case 0x151: case 0x152: case 0x153: case 0x154: case 0x155:
                case 0x156: case 0x157: case 0x158: case 0x159:
                case 0x3EE: case 0x3EF:
                    ok = 1; break;
                case 0x15A:
                    if (gDuelPlayers[0].lp <= 1000 && gDuelPlayers[1].lp > 500)
                        ok = 1;
                    if (gDuelPlayers[0].lp < gDuelPlayers[1].lp + 500)
                        ok = 1;
                    if (gDuelPlayers[1].lp > 500) { ok = 1; break; }
                    break;
                case 0x40F:
                    if (200 * gDuelPlayers[0].handCount > gDuelPlayers[0].lp)
                        ok = 1;
                    if (gDuelPlayers[0].handCount > 2) { ok = 1; break; }
                    break;
                case 0x3F1:
                    if (CountFaceUpSpellTrapsOfType(0, 0x16) > 0) { ok = 1; break; }
                    break;
                case 0x3EC:
                    if (CountFaceUpSpellTrapsOfType(0, 0x15) > 0) { ok = 1; break; }
                    break;
                case 0x29F: case 0x437: case 0x438:
                    if (CountSpellTraps(0) > 0) { ok = 1; break; }
                    break;
                case 0x425:
                    if (CountSpellTraps(0) > 0 && CountSpellTraps(1) == 0) { ok = 1; break; }
                    break;
                case 0x42F:
                    if (gDuelPlayers[1].lp > 1000 && gDuelPlayers[0].handCount)
                        ok = 1;
                    break;
                }
                if (ok) {
                    int z;
                    u32 id;
                    ref.kind = 0;
                    for (z = 5; z <= 9; z++) {
                        {
                            u32 raw = ZONE(z)->card;
                            id = CARD_ID(raw);
                        }
                        /* Original code indexes this flag by table position, not z. */
                        if (id && ({
                            /* FAKEMATCH: initialized mask copies retain the lookup registers. */
                            register u32 loadMask asm("r1") = 0x7FF;
                            register u32 mask asm("r0");
                            u32 index;
                            asm("" : : "r"(loadMask));
                            mask = loadMask;
                            asm("" : "+r"(mask));
                            index = id;
                            ((const u16 *)0x08622AB4)[index & mask];
                        }) == (u16)gAiSimpleSpells[gAiState.f6]
                            && (ZONE(gAiState.f6)->f91 & 4)
                            && CanActivateFieldCard(&ref, 1, z)) {
                            DuelCmd_Push(0x807F, z, 0, 0);
                            {
                                u32 bits = (u32)(z & 31) << 16;
                                id |= 0x80200000;
                                Chain_AddPending(bits | id, 0);
                            }
                            return 0;
                        }
                    }
                }
            }
            gAiState.f6++;
        } while (gAiState.f6 <= 0x13);
        {
            /* FAKEMATCH: keep the initialized phase pointer separate from the next loop. */
            register struct AiState *state asm("r2") = &gAiState;
            state->phase++;
        }
        /* fall through */
    case 1: {
        /* FAKEMATCH: stage the initialized reset pointer before retaining state. */
        register struct AiState *initial asm("r1");
        struct AiState *state;
        const s16 *numbers;
        struct AiCardRef *record;
        int zero = 0;

        initial = &gAiState;

        initial->f6 = zero;
        numbers = gAiSimpleSpells;

        state = initial;
        record = &ref;

        do {
            if (AiFindHandCardByNumber(1, numbers[state->f6]) >= 0) {
                int ok = 0;
                switch (numbers[state->f6]) {
                case 0x151: case 0x152: case 0x153: case 0x154: case 0x155:
                case 0x156: case 0x157: case 0x158: case 0x159:
                case 0x3EE: case 0x3EF:
                    ok = 1; break;
                case 0x15A:
                    if (gDuelPlayers[0].lp <= 1999 && gDuelPlayers[1].lp > 1500)
                        { ok = 1; break; }
                    break;
                case 0x3F1:
                    if (CountFaceUpSpellTrapsOfType(0, 0x16) > 0) { ok = 1; break; }
                    break;
                case 0x3EC:
                    if (CountFaceUpSpellTrapsOfType(0, 0x15) > 0) { ok = 1; break; }
                    break;
                case 0x29F: case 0x437:
                    if (CountSpellTraps(0) > 0) { ok = 1; break; }
                    break;
                case 0x425: case 0x438:
                    if (CountSpellTraps(0) > 0 && CountSpellTraps(1) == 0) { ok = 1; break; }
                    break;
                case 0x42F:
                    if (gDuelPlayers[1].lp > 1000 && gDuelPlayers[0].handCount)
                        ok = 1;
                    break;
                }
                if (ok) {
                    record->player = 1;
                    record->kind = 0;
                    record->id = AiScanCardId(numbers[state->f6]);
                    if (CanActivateEffect(&ref, 0, 1) && CanPlaceSpellTrapCard(1, record->id)) {
                        gAiState.phase++;
                        return 0;
                    }
                }
            }
            state->f6++;
        } while (state->f6 <= 0x13);
        break;
    }
    case 2:
        if (CountActiveCardsOnField(0, 0x49C) || CountActiveCardsOnField(1, 0x49C)) {
            u16 id = AiScanCardId(gAiSimpleSpells[gAiState.f6]);
            int hand = AiFindHandCardByNumber(1, gAiSimpleSpells[gAiState.f6]);
            int freeZone = FindFreeSpellTrapZone(1);
            {
                int packed = ((hand & 15) << 4) | (freeZone & 15);
                /* FAKEMATCH: finish packing before loading the message constant. */
                asm("" : : "r"(packed));
                DuelCmd_Push(0x80C5, id, packed, 0);
            }
        } else {
            u16 id = AiScanCardId(gAiSimpleSpells[gAiState.f6]);
            int hand = AiFindHandCardByNumber(1, gAiSimpleSpells[gAiState.f6]);
            int freeZone = FindFreeSpellTrapZone(1);
            int zone;
            {
                int packed = ((hand & 15) << 4) | (freeZone & 15);
                /* FAKEMATCH: the old scratch is dead here; rematerialize the flag
                 * and retain its initialized copy before the message setup. */
                asm("" : : "r"(packed) : "r1");
                {
                    register u32 flag asm("r1") = 0x100;
                    asm("" : : "r"(flag));
                    {
                        u32 value = 0x100;
                        packed |= value;
                    }
                }
                asm("" : : "r"(packed));
                DuelCmd_Push(0x80C5, id, packed, 0);
            }
            zone = FindFreeSpellTrapZone(1);
            {
                /* FAKEMATCH: initialize the card word in the final packing register. */
                register u32 card asm("r1");
                u32 bits;
                u16 number = gAiSimpleSpells[gAiState.f6];
                if (number == 0xFFFF)
                    card = 0;
                else if (number <= 0x7CF) {
                    /* FAKEMATCH: preserve the initialized reverse-table scratch. */
                    register u32 table asm("r2");
                    u32 off = (number & 0x7FF) * 2;
                    table = 0x08623DF4;
                    asm("" : : "r"(table));
                    card = *(const u16 *)(off + table);
                } else
                    card = ((const u16 *)0x08623DF4)[(number - 0x7D0) & 0x7FF] + 1;
                bits = (u32)(zone & 31) << 16;
                card = (u16)card;
                card |= 0x80200000;
                Chain_AddPending(bits | card, 0);
            }
        }
        gAiState.phase = 0;
        return 0;
    }
    return 1;
}


/* Step 6: scan player 1's monster zones 0-4 (counter f5) for the card numbers that
 * let the CPU attack / activate from that zone; on a hit advance the sub-state (f6),
 * on a miss try the next zone.
 * NONMATCHING: structure/control flow decoded. The ROM uses dispatch ptr r4 (dead after the
 * dispatch, reused for 0x7FF), body ptr r5, zones base r6 and 0x94 in r7; the build keeps the
 * gCardIdToNumber table address and 0x7FF in r6/r7 across the HasFlipEffect call, so zones/0x94
 * spill to r8. Case 0x231 must use the body pointer (r5), not p. */
/* CountMonsters returns int (see duel_card_lists.c); the unit header declares u16, which
 * would add narrowing after each call, so call it through an int-returning cast. */
#define B184_Count(pl) (((int (*)(int))CountMonsters)(pl))
/* Step 6: scan player 1's monster zones 0-4 (counter f5) for card numbers that let the
 * CPU attack / activate from that zone; on a hit advance the sub-state (f6), on a miss
 * try the next zone. Sub-state 2 queues the action (QueueFlipSummon) and returns to 1.
 * The dispatch pointer (r4) and the body pointer (r1 -> r5) are separate locals, and the
 * success increments mix pointer and global forms, as the ROM's unmerged tails show. */
int AiStepFlipSummon(void)
{
    struct AiState *p = &gAiState;
    int phase = p->f6;
    struct AiState *dispatch = &gAiState;

    switch (phase) {
    case 0:
        p->f4 = 0;
        p->f5 = 0;
        p->f6++;
        /* fall through */
    case 1:
        {
        struct AiState *q = dispatch;
        int z;
        u32 id;
        u16 number;
        if (q->f5 > 4)
            return 1;
        z = q->f5;
        id = CARD_ID(ZONE(z)->card);
        if (id == 0) {
            q->f5 = z + 1;
            return 0;
        }
        if (ZONE(z)->flags6 & 2) {
            q->f5 = z + 1;
            return 0;
        }
        if (HasFlipEffect(((const u16 *)0x08622AB4)[id & 0x7FF], 0) == 0) {
            q->f5++;
            return 0;
        }
        number = ((const u16 *)0x08622AB4)[CARD_ID(ZONE(q->f5)->card) & 0x7FF];
        switch (number) {
        case 0x231:
            if (B184_Count(0) != 0)
                break;
            q->f6++;
            return 0;
        case 0x21B:
        case 0x24E:
            if (gDuelPlayers[1].handCount <= 2 || gDuelPlayers[0].handCount > gDuelPlayers[1].handCount + 2) {
                gAiState.f6++;
                return 0;
            }
            break;
        case 0x1F4:
        case 0x21C:
        case 0x259:
            if (B184_Count(0) > 0) {
                gAiState.f6++;
                return 0;
            }
            break;
        case 0x452:
        case 0x48B:
            if (B184_Count(1) == 1 && B184_Count(0) > 1) {
                gAiState.f6++;
                return 0;
            }
            break;
        case 0x280:
            if (B184_Count(0) > B184_Count(1) + 1) {
                q->f6++;
                return 0;
            }
            break;
        case 0x27:
            if (CountActiveCardsOnField2(0, 0x148) > 0) {
                q->f6++;
                return 0;
            }
            break;
        case 0x246:
            if (CountActiveCardsOnField2(0, 0x15B) > 0) {
                q->f6++;
                return 0;
            }
            break;
        case 0x109:
            if (CountSpellTraps(0) > 1) {
                q->f6++;
                return 0;
            }
            break;
        case 0x53:
        case 0xDF:
            if (CountActivatableSetCards(0, number) > 0) {
                gAiState.f6++;
                return 0;
            }
            break;
        case 0x262:
        case 0x2FA:
            gAiState.f6++;
            return 0;
        case 0x249:
            if (CountActivatableSetCards(0, number) > 1) {
                q->f6++;
                return 0;
            }
            break;
        case 0x1AB:
            if (CountGraveyardCardsOfType(1, 0x16) > 0) {
                q->f6++;
                return 0;
            }
            break;
        case 0x65:
            if (CountGraveyardCardsOfType(1, 0x15) > 0) {
                q->f6++;
                return 0;
            }
            break;
        }
        gAiState.f5++;
        return 0;
        }
    case 2:
        QueueFlipSummon(1, p->f5);
        p->f5++;
        p->f6 = 1;
        return 0;
    }
    return 1;
}

