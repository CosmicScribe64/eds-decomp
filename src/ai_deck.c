#include "global.h"
#include "gba.h"

/* One card instance word (see duel_card_lists). */
struct DuelCard {
    u32 id : 12;
    u32 unk12 : 20;
};
struct DuelZone {
    struct DuelCard card;   /* +0 */
    u8 unk4[2];
    u8 flags6;              /* +6 */
    u8 unk7[0x94 - 7];
};
/* Per-player duel state, 0xD64 bytes, two of them at 0x020192E4 (see duel_card_lists). */
struct DuelPlayer {
    u16 w0;                         /* +0x000 */
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 count904;                    /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 unk6[0x20];
    u16 unk26;                      /* +0x026 */
    struct DuelZone zones[11];      /* +0x028 */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] = top */
    struct DuelCard list904[80];    /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard listB84[80];    /* +0xB84 */
    u16 arrCC4[80];                 /* +0xCC4 */
};
extern struct DuelPlayer gDuelPlayers[2];
#define PLAYER(p) (gDuelPlayers[(p) & 1])
#define CARD_ID(word) (((struct DuelCard *)(word))->id)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])   /* card ID to card number */

/* Card reference record passed to the usability tests (see duel_response). */
struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;
    u8 unk4_3 : 5;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;  /* +0x0A bits 0-2 */
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[3];     /* +0x0C */
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gDuelZones[2];
extern int CanActivateFieldCard(struct CardRef *ref, int player, int idx);
extern u16 TakeDeckCardAt(int player, int idx, struct DuelCard *out);
extern int GetCardCopyLimit(u32 id);
extern int CountActivatableSetCards(int a, u16 b);
extern u32 Random(void);
extern void Chain_AddPending(u32 a, int b);

/* Remove every card with card number `number` from the deck of `player`. */
void RemoveAllDeckCardsByNumber(int player, u16 number)
{
    int i;
    struct DuelCard out;
    for (i = 0; i < PLAYER(player).deckCount;) {
        if (CARD_NUMBER(CARD_ID(&PLAYER(player).deck[i])) == number)
            TakeDeckCardAt(player, i, &out);
        else
            i++;
    }
}

extern const u16 gCardIdToNumber[];   /* card ID to card number (extern form: the table address is hoisted) */

/* Number of cards in the deck of `player` with card number `number`. */
int CountDeckCardsByNumber(int player, u16 number)
{
    int n = 0;
    int i;
    for (i = 0; i < PLAYER(player).deckCount; i++) {
        u32 id = CARD_ID(&PLAYER(player).deck[i]);
        if (*((id & 0x7FF) + gCardIdToNumber) == number)
            n++;
    }
    return n;
}
void RemoveOverLimitDeckCards(int player)
{
    int i = 0;
    while (i < PLAYER(player).deckCount) {
        u16 id = CARD_ID(&PLAYER(player).deck[i]);
        const u16 *p = (id & 0x7FF) + gCardIdToNumber;
        if (CountDeckCardsByNumber(player, *p) > GetCardCopyLimit(id))
            RemoveAllDeckCardsByNumber(player, *p);
        else
            i++;
    }
} /* 0x080591BC size 0x9C */
/* gMain (0x03000040), only the byte at +0x4870 is used here. */
struct Main {
    u8 pad0[0x4870];
    u8 unk4870_0 : 1;
    u8 mode : 5;                    /* +0x4870 bits 1-5: scenario / opponent index */
    u8 unk4870_6 : 2;
};
extern struct Main gMain;
struct Flags5EE8 { u32 w0; u32 w4; };
extern struct Flags5EE8 gDuelCtrl;
extern u8 gDuelDeckP1[];
struct DeckList { const u16 *ids; u16 n; u16 pad; };
extern const struct DeckList gOpponentDecks[];
extern const struct DeckList gOpponentAltDecks[];
extern void MemClear16(void *dst, u32 size);
extern void AddCardNumberToDeckTop(int a, int b);
extern void RemoveOverLimitDeckCards(int player);

/* Reset the two 0x140-byte work buffers and add the starting cards for scenario `mode` (hypothesis). */
void LoadOpponentDeck(u16 alt)
{
    int k;
    const struct DeckList *l;
    int mode = gMain.mode;
    int zero = 0;
    MemClear16(gDuelDeckP1, 0x140);
    MemClear16(gDuelDeckP1 + 0x280, 0x140);
    gDuelDeckP1[-0x7C1] = zero;
    gDuelDeckP1[-0x7BF] = zero;
    if (mode != 0) {
        if (mode == 0xB)
            gDuelCtrl.w4 |= 0x200;
        l = alt != 0 ? &gOpponentAltDecks[mode] : &gOpponentDecks[mode];
        /* Preserve the reset-size register and put the list pointer in r6. */
        __asm__ __volatile__("" : : : "r5");
        for (k = 0; k < l->n; k++)
            AddCardNumberToDeckTop(1, l->ids[k]);
    }
    RemoveOverLimitDeckCards(1);
}

/* 1 if a spell/trap zone (5-9) of player 1 holds card number `number` and it is usable as an effect source. */
int AiHasUsableSpellTrap(u16 number)
{
    struct CardRef ref;
    int z;
    for (z = 5; z <= 9; z++) {
        struct DuelZone *zone = &gDuelZones[1].zones[z];
        if (CARD_ID(zone) != 0) {
            ref.player = 1;
            ref.kind = 0;
            if (CARD_NUMBER(CARD_ID(zone)) == number && CanActivateFieldCard(&ref, 1, z) != 0)
                return 1;
        }
    }
    return 0;
}
/* AI step state at 0x02015EF0 (hypothesis): state machine byte, loop counters, chosen zone / hand slot. */
struct Sel5EF0 {
    u8 pad0[6];
    u8 state;                   /* +6: 0, 0x64, 0xC8 ... */
    u8 b7;                      /* +7 */
    u8 b8;                      /* +8 */
    u8 pad9;
    u8 bA;                      /* +0xA */
    u8 bB;                      /* +0xB: zone / hand index */
};
extern struct Sel5EF0 gAiState;

/* Like AiHasUsableSpellTrap, but fills the whole CardRef (id, zone) and on success records the zone in `gAiState`. */
int AiSelectUsableSpellTrap(u16 number)
{
    struct CardRef ref;
    int z;
    for (z = 5; z <= 9; z++) {
        struct DuelZone *zone = &gDuelZones[1].zones[z];
        if (CARD_ID(zone) != 0) {
            ref.player = 1;
            ref.id = CARD_ID(zone);
            ref.zone = z;
            ref.kind = 0;
            if (CARD_NUMBER(CARD_ID(zone)) == number && CanActivateFieldCard(&ref, 1, z) != 0) {
                gAiState.bB = z;
                gAiState.state = 0xC8;
                return 1;
            }
        }
    }
    return 0;
}
extern int FindHandCardByNumber(int player, u16 number);
extern int GetFaceUpFieldMagicNumber(void);
extern int EffectEquipTargetCheck(struct CardRef *ref, u16 pos);
extern int CanActivateEffect(struct CardRef *ref, int a, int b);
extern int CanPlaceSpellTrapCard(int player, u16 id);
extern void DuelCmd_Push(u32 a, u16 b, u16 c, u32 d);
#define gCardNumberToId ((const u16 *)0x08623DF4)   /* card number to card ID */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])

/* Converts a card number to a card ID: 0xFFFF means none, numbers up to 0x7CF map directly, and anything else uses the alternate entry + 1. */
static inline u16 CardNumberToId(u16 n)
{
    if (n == 0xFFFF)
        return 0;
    if (n <= 0x7CF)
        return *((n & 0x7FF) + gCardNumberToId);
    return *(((n - 0x7D0) & 0x7FF) + gCardNumberToId) + 1;
}

/* Spell/trap subtype (stats bits 17-19) for Magic and Trap cards, else 0. */
static inline int GetSpellSubtype(u32 stats)
{
    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
        return (stats & 0xE0000) >> 17;
    default:
        return 0;
    }
}

static inline u32 PackRefPosition(int z)
{
    u32 hi = (u32)z << 24;
    u32 flag = 0x10000;
    /* Keep the flag as the destination of the packed-position OR. */
    __asm__ __volatile__("" : "+r"(flag) : "r"(hi));
    return flag | hi;
}

/* AI: tries to activate or set the spell or trap card with number `number` (hypothesis). Returns 1 if an action was queued. */
/* Return the zero-extended halfword as a word, as the ROM callers consume it. */
int AiTryPlaySpellTrap(u16 number)
{
    struct CardRef ref;
    int hand = FindHandCardByNumber(1, number);
    int ok;
    int z;
    u32 a;
    u32 t;
    u32 pos;
    u16 id0;
    if (CountActivatableSetCards(0, 0x405) != 0) {
        if (gDuelCtrl.w4 & 1) {
            if ((Random() & 3) != 0)
                return 0;
        } else {
            if ((Random() & 7) == 0)
                return 0;
        }
    }
    id0 = CardNumberToId(number);
    if (GetSpellSubtype(CARD_STATS(id0)) == 2 && GetFaceUpFieldMagicNumber() == number)
        return 0;
    switch (number) {
    case 0x12C: case 0x12D: case 0x12E: case 0x12F: case 0x130: case 0x131: case 0x132: case 0x133:
    case 0x134: case 0x135: case 0x136: case 0x137: case 0x138: case 0x139: case 0x13A: case 0x13B:
    case 0x13C:
    case 0x13E:
    case 0x140: case 0x141: case 0x142: case 0x143: case 0x144: case 0x145: case 0x146: case 0x147:
    case 0x28B:
    case 0x28D:
    case 0x29B:
    case 0x3F4: case 0x3F5:
    case 0x412:
    case 0x49E:
    case 0x4B3:
    case 0x58C:
        ok = 1;
        for (z = 0; z <= 4; z++) {
            ref.id = CardNumberToId(number);
            ref.player = 1;
            pos = PackRefPosition(z);
            if (EffectEquipTargetCheck(&ref, pos >> 16) != 0)
                ok = 0;
        }
        if (ok != 0)
            return 0;
        break;
    }
    for (z = 5; z <= 9; z++) {
        struct DuelZone *zone;
        ref.player = 1;
        ref.kind = 0;
        ref.zone = z;
        zone = &gDuelZones[1].zones[z];
        ref.id = CARD_ID(zone);
        if (ref.id != 0 && (((u8 *)zone)[0x91] & 4) && !(zone->flags6 & 2) && CARD_NUMBER(ref.id) == number
            && CanActivateEffect(&ref, 0, 0) != 0) {
            DuelCmd_Push(0x8008, 1, (z & 0xFF) << 8, 0);
            DuelCmd_Push(0x807F, z, 0, 0);
            a = (z & 0x1F) << 16;
            t = CARD_ID(zone) | 0x80200000;
            Chain_AddPending(a | t, 0);
            return 1;
        }
    }
    if (CanPlaceSpellTrapCard(1, CardNumberToId(number)) == 0)
        return 0;
    if (hand < 0)
        return 0;
    ref.player = 1;
    ref.kind = 0;
    ref.id = CARD_ID(&gDuelPlayers[1].hand[hand]);
    if (CanActivateEffect(&ref, 0, 1) == 0)
        return 0;
    gAiState.bB = hand;
    gAiState.state = 0xC8;
    return 1;
}
/* Selection widget at 0x0201AE0C (see campaign), only the fields written here. */
struct SelW {
    u16 flag0 : 1;
    u16 active : 1;
    u16 cursor : 4;
    u16 rows : 4;
    u32 mask : 16;
    u32 state : 8;
    u32 unk34 : 8;
    u32 unk42 : 8;
    u16 timer : 7;
    u16 player : 1;
    u32 zone : 7;
    u32 unk65 : 8;
    u32 unk73 : 23;
};
extern u8 gDuelZonesP1[];
#define SELW (*(struct SelW *)(gDuelZonesP1 + 0xD9C))
#define SEL_CARD (*(u16 *)(gDuelZonesP1 + 0xD98))
extern const u16 gAiEffectMonsters[];
extern int CountMonsters(int player);
extern int AiTryPlaySpellTrap(u16 number);

/* AI: scan the four card numbers in `gAiEffectMonsters` against player 1's zones 0-4 (hypothesis). Returns 1 when an activation was queued. */
static inline u32 PackZoneIndex(u32 z)
{
    u8 mask = 31;
    /* Keep the mask separate from the still-live zone index. */
    __asm__ __volatile__("" : : "r"(mask));
    return (z & mask) << 16;
}

struct SelectionCursor {
    u16 lo : 2;
    u16 cursor : 8;
    u16 hi : 6;
};
struct SelectionSlot {
    u16 lo : 1;
    u16 slot : 8;
    u16 hi : 7;
};

int AiActivateMonsterEffects(void)
{
    struct CardRef ref;
    int ok;
    u16 id;
    u32 a;
    u32 t;
    u16 cid;
    u16 *p;
    for (gAiState.b7 = 0; gAiState.b7 <= 3; gAiState.b7++) {
        for (gAiState.b8 = 0; gAiState.b8 <= 4; gAiState.b8++) {
            ref.player = 1;
            id = CARD_ID(gDuelZonesP1 + gAiState.b8 * 0x94);
            ref.id = id;
            ref.zone = gAiState.b8;
            ref.kind = 0;
            if (id != 0 && CARD_NUMBER(id) == gAiEffectMonsters[gAiState.b7]
                && (((struct DuelZone *)(gDuelZonesP1 + gAiState.b8 * 0x94))->flags6 & 2) != 0
                && CanActivateEffect(&ref, 0, 0) != 0) {
                ok = 1;
                if (gAiEffectMonsters[gAiState.b7] == 0x1FF) {
                    if ((u16)AiTryPlaySpellTrap(0x4DD) != 0) {
                        ((struct SelectionCursor *)(gDuelZonesP1 + 0xDA0))->cursor = 0;
                        cid = CARD_ID(&gDuelPlayers[1].hand[gAiState.bB]);
                        *(u16 *)(gDuelZonesP1 + 0xD98) = cid;
                        *(u8 *)(gDuelZonesP1 + 0xDA3) |= 2;
                        p = (u16 *)(gDuelZonesP1 + 0xDA4);
                        ((struct SelectionSlot *)p)->slot = gAiState.bB;
                        *(u8 *)(gDuelZonesP1 + 0xD9C) |= 2;
                        return 0;
                    }
                    if (CountMonsters(1) <= 1)
                        ok = 0;
                }
                if (ok != 0) {
                    DuelCmd_Push(0x8008, 1, gAiState.b8 << 8, 0);
                    a = PackZoneIndex(gAiState.b8);
                    t = CARD_ID(gDuelZonesP1 + gAiState.b8 * 0x94) | 0x80400000;
                    Chain_AddPending(a | t, 0);
                    return 1;
                }
            }
        }
    }
    return 0;
}
extern u8 gDuelZonesP1[];
#define Z1(z) ((struct DuelZone *)((u32)gDuelZonesP1 + (z) * 0x94))
extern int EffectBackupSoldierPrepare(struct CardRef *ref, int a, int b);
extern int AiCountExodiaInGraveyard(void);
extern void FlipFieldCard(int player, int zone, int c);

/* AI step machine for a special summon combo (hypothesis): state 0 waits for a flag, 0x64 checks cards 0x5A7 / 0x47B, 0xC8 acts on the zone. Returns 1 when the step is finished without action. */
int AiActivateExodiaTraps(void)
{
    struct CardRef ref;
    switch (gAiState.state) {
    case 0:
        if (gDuelCtrl.w4 & 0x200) {
            gAiState.state = 0x64;
            return 0;
        }
        break;
    case 0x64:
        if ((u16)AiSelectUsableSpellTrap(0x5A7) != 0)
            return 0;
        ref.player = 1;
        if (EffectBackupSoldierPrepare(&ref, 0, 0) != 0 && AiCountExodiaInGraveyard() > 0 && (u16)AiSelectUsableSpellTrap(0x47B) != 0)
            return 0;
        break;
    case 0xC8: {
        u8 z = gAiState.bB;
        u32 a;
        u32 t;
        if (!(Z1(z)->flags6 & 2))
            FlipFieldCard(1, z, 0);
        z = gAiState.bB;
        a = PackZoneIndex(z);
        t = CARD_ID(Z1(z)) | 0x80200000;
        Chain_AddPending(a | t, 0);
        gAiState.bA = 0;
        return 0;
    }
    }
    return 1;
}

/* Random gate: if `CountActivatableSetCards(0, x)`, returns 0 with probability 3/4 (1/8 when `gDuelCtrl.w4` bit 0 is clear), else 1. */
int AiRiskSetCounter(u16 x)
{
    if (CountActivatableSetCards(0, x) != 0) {
        if (gDuelCtrl.w4 & 1) {
            if ((Random() & 3) != 0)
                return 0;
        } else {
            if ((Random() & 7) == 0)
                return 0;
        }
    }
    return 1;
}
extern int AiFindStrongestMonster(int a, int b, int c, int d);
extern int GetZoneCardAtk(int player, int idx);

/* Compares two sums of zone values of the players against the limit at `PLAYER(1)+0` (reads two uninitialised locals, as the original does). */
int AiIsOpponentThreatening(void)
{
    int a;
    int b;
    int i;
    int x = AiFindStrongestMonster(0, -1, 1, 0);
    int y = AiFindStrongestMonster(1, -1, 1, 0);
    a = a == -1 ? 0 : GetZoneCardAtk(0, x);
    b = b == -1 ? 0 : GetZoneCardAtk(1, y);
    if (a < gDuelPlayers[1].w0 && a - b < gDuelPlayers[1].w0 && a <= b) {
        a = 0;
        for (i = 0; i <= 4; i++)
            a += GetZoneCardAtk(0, i);
        if (a <= gDuelPlayers[1].w0)
            return 0;
    }
    return 1;
}
extern int IsSpecialSummonOnly(u32 id);
extern int CountActiveCardsOnFieldExcept(int player, u16 number, int zone);
extern int CountActiveCardsOnField(int player, u16 number);
extern int CountMonstersByNumber(int player, u16 number);
extern int CountMonstersFiltered(int player, u16 a, u16 b);
extern int CountHandMonsters(int player);
extern int EffectMonsterRebornPrepare(struct CardRef *ref, int a, int b);
extern int EffectPrematureBurialPrepare(struct CardRef *ref, int a, u16 flag);
extern void CardMenu_PlaySpellTrapFromHand(u16 a, u16 b, struct CardRef *ref);
extern int AiFindHandCardByNumber(int player, u16 number);
extern int AiCountExodiaOnField(void);
struct AiSelectionState {
 u8 pad[0x13EC]; struct DuelCard hand[80]; u8 gap[0x1B28-0x152C];
 u16 card; u8 gap2[2]; union { u8 raw; struct { u8 lo:1; u8 active:1; u8 hi:6; } bits; } active;
 u16 cursorLo:2; u16 cursor:8; u16 cursorHi:6;
 u8 byte32; u8 flags33;
 u16 slotLo:1; u16 slot:8; u16 slotHi:7;
};
extern struct AiSelectionState gAiSelectionState asm("gDuel");

extern struct DuelCard gDuelHandP1[];
extern const u16 gAiGenericSpells[], gAiEquipSpells[];

static inline u32 AiCardLevel(u16 id)
{
    switch ((int)((CARD_STATS(id) & 0x1F00000) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (CARD_STATS(id) & 0x1E000000) >> 25;
    }
}

static inline u32 AiCardLevelMasks(u32 id, u32 mask, u32 typeMask)
{
    switch ((int)((((const u32 *)0x08621DE0)[id & mask] & typeMask) >> 20)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    default:
        return (((const u32 *)0x08621DE0)[id & mask] & 0x1E000000) >> 25;
    }
}
/* FAKEMATCH: reload the hand-count address instead of retaining a loop pointer. */
static inline int AiCountReload(void)
{
    u32 base = (u32)gDuelPlayers;
    u32 off = 0xD66;
    asm("" : "+r"(base), "+r"(off));
    return *(u8 *)(base + off);
}
static inline u16 AiDeckNumber(u32 id)
{
    u32 base;
    u32 off;
    off = (id & 0x7FF) * 2;
    base = (u32)gCardIdToNumber;
    return *(u16 *)(off + base);
}
/* FAKEMATCH: preserve the initialized count scratch at the loop test. */
static inline int AiDeckCountAt(u8 *ptr)
{
    register int count asm("r3") = *ptr;
    asm("" : : "r"(count));
    return count;
}
/* CPU spell/monster selection state machine; usability records set only id/player. */
int AiPlaySpells(void)
{
    struct CardRef ref;
    int i;
    int found = 1;
    int state = gAiState.state;
    u16 number;
    u16 result;

    switch ((u8)state) {
    case 0:
        if (gDuelCtrl.w4 & 0x200)
            gAiState.state = 0x64;
        else
            gAiState.state = state + 1;
        return 0;
    case 1:
        if ((u16)AiTryPlaySpellTrap(0x3F2)) return 0;
        if ((u16)AiTryPlaySpellTrap(0x410)) return 0;
        if (CountMonsters(0) > 0 && CountMonsters(1) == 0 && (u16)AiTryPlaySpellTrap(0x14F)) return 0;
        if ((u16)AiIsOpponentThreatening() && (u16)AiTryPlaySpellTrap(0x14F)) return 0;
        if ((CountMonstersByNumber(1, 0x2F) > 0 || CountMonstersByNumber(1, 0x23D) > 0) && (u16)AiTryPlaySpellTrap(0x14F)) return 0;
        if (CountMonsters(0) > 0 && (u16)AiRiskSetCounter(0x3FB) && (u16)AiTryPlaySpellTrap(0x14F)) return 0;
        if (CountMonsters(0) > 0 && CountMonsters(1) == 0 && (u16)AiTryPlaySpellTrap(0x150)) return 0;
        if (CountMonsters(0) > 0 && (u16)AiRiskSetCounter(0x3FE) && (u16)AiTryPlaySpellTrap(0x150)) return 0;
        if (CountMonstersFiltered(0, 0, 0) > 0 && CountHandMonsters(1) > 0 && (u16)AiTryPlaySpellTrap(0x3FF)) return 0;
        if (CountMonstersFiltered(0, 1, 0) > 0 && (u16)AiTryPlaySpellTrap(0x3E9)) return 0;
        gAiState.state++;
        return 0;
    case 2:
        if (CountMonsters(0) > 0) {
            if (CountMonsters(1) == 0) {
                i = 0;
                if (i < PLAYER(1).handCount) {
                    do {
                        u16 id = CARD_ID((u8 *)gDuelHandP1 + i * 4);
                        if (((CARD_STATS(id) & 0x1F00000) >> 20) <= 20 && IsSpecialSummonOnly(id) == 0 && AiCardLevel(id) > 4 && AiCardLevel(id) <= 6 && (u16)AiTryPlaySpellTrap(0x403))
                            return 0;
                        i++;
                    } while (i < PLAYER(1).handCount);
                }
            }
            if (CountMonsters(1) > 0) {
                i = 0;
                if (i < PLAYER(1).handCount) {
                    u32 typeMask = 0x1F00000;
                    u32 loadMask = 0x7FF;
                    u32 mask;
                    /* FAKEMATCH: retain the initialized mask copy before the scan. */
                    asm("" : : "r"(loadMask));
                    mask = loadMask;
                    do {
                        u32 id = CARD_ID((u8 *)gDuelHandP1 + i * 4);
                        if (((((const u32 *)0x08621DE0)[id & mask] & typeMask) >> 20) <= 20 && IsSpecialSummonOnly(id) == 0 && AiCardLevelMasks(id, mask, typeMask) > 4 && (u16)AiTryPlaySpellTrap(0x403))
                            return 0;
                        i++;
                    } while (i < AiCountReload());
                }
            }
        }
        number = 0x3F0;
        ref.id = CardNumberToId(number);
        ref.player = 1;
        if (EffectMonsterRebornPrepare(&ref, 0, 1) && (u16)AiRiskSetCounter(0x402) && (u16)AiTryPlaySpellTrap(number)) return 0;
        number = 0x488;
        ref.id = CardNumberToId(number);
        ref.player = 1;
        if (EffectPrematureBurialPrepare(&ref, 0, 1) && (u16)AiTryPlaySpellTrap(number)) return 0;
        gAiState.state++;
        return 0;
    case 3:
        if (gDuelCtrl.w4 & found) {
            if (FindHandCardByNumber(1, 0x3F0) != -1 && (u16)AiTryPlaySpellTrap(0x3C8)) return 0;
            if (FindHandCardByNumber(1, 0x488) != -1 && (u16)AiTryPlaySpellTrap(0x3C8)) return 0;
            found = 0;
            {
                /* FAKEMATCH: initialized terms preserve the deck-count preheader. */
                register u32 base asm("r0") = (u32)gDuelPlayers;
                register u32 off asm("r3") = 0xD67;
                int count;
                u8 *first;
                asm("" : "+r"(off));
                first = (u8 *)(base + off);
                count = *first;
                if (found < count) {
                    u8 *ptr = first;
                deckLoop:
                    {
                        switch (AiDeckNumber(CARD_ID(gDuelDeckP1 + found * 4))) {
                        case 0x14F:
                        case 0x150:
                        case 0x3C8:
                        case 0x3F0:
                        case 0x488:
                            if ((u16)AiTryPlaySpellTrap(0x3C8)) return 0;
                            break;
                        }
                        found++;
                    }
                    if (found < 3 && found < AiDeckCountAt(ptr))
                        goto deckLoop;
                }
            }
        } else if ((u16)AiTryPlaySpellTrap(0x3C8)) return 0;
        gAiState.state++;
        return 0;
    case 4:
        found = 0;
        if (CountActiveCardsOnFieldExcept(1, 0x40E, -1) > 0) found = 1;
        if (AiFindHandCardByNumber(0, 0x10)) found = 1;
        if (AiFindHandCardByNumber(0, 0x11)) found = 1;
        if (AiFindHandCardByNumber(0, 0x12)) found = 1;
        if (AiFindHandCardByNumber(0, 0x13)) found = 1;
        if (AiFindHandCardByNumber(0, 0x14)) found = 1;
        if (AiFindHandCardByNumber(0, 0x14F)) found = 1;
        if (AiFindHandCardByNumber(0, 0x150)) found = 1;
        if (AiFindHandCardByNumber(0, 0x290)) found = 1;
        if (AiFindHandCardByNumber(0, 0x3F0)) found = 1;
        if (AiFindHandCardByNumber(0, 0x403)) found = 1;
        if (AiFindHandCardByNumber(0, 0x420)) found = 1;
        if (AiFindHandCardByNumber(0, 0x42C)) found = 1;
        if (AiFindHandCardByNumber(0, 0x488)) found = 1;
        if (found && (u16)AiTryPlaySpellTrap(0x4C5))
            return 0;
        gAiState.state++;
        gAiState.b7 = 0;
        gAiState.b8 = 0;
        gAiState.pad9 = 0;
        return 0;
    case 5:
        for (gAiState.b7 = 0; gAiState.b7 < 54; gAiState.b7++) {
            result = (u16)AiTryPlaySpellTrap(gAiGenericSpells[gAiState.b7]);
            if (result) return 0;
        }
        if (CountMonstersFiltered(1, 1, 0) > 0) {
            for (gAiState.b7 = 0; gAiState.b7 < 35; gAiState.b7++) {
                if ((u16)AiTryPlaySpellTrap(gAiEquipSpells[gAiState.b7])) return 0;
            }
        }
        break;
    case 0x64:
        if (!AiCountExodiaOnField() && (CountMonstersByNumber(1, 0x2F) > 0 || CountMonstersByNumber(1, 0x23D) > 0) && (u16)AiTryPlaySpellTrap(0x14F)) return 0;
        gAiState.state++;
        return 0;
    case 0x65:
        if (!(u16)AiTryPlaySpellTrap(0x3C8) && !(u16)AiTryPlaySpellTrap(0x3F2)) gAiState.state++;
        return 0;
    case 0x66:
        if ((AiCountExodiaOnField() || (CountMonstersFiltered(0, 1, 0) > 0 && CountMonsters(1) == 0)) && CountActiveCardsOnFieldExcept(1, 0x15B, -1) == 0 && (u16)AiTryPlaySpellTrap(0x15B)) return 0;
        break;
    case 0xC8:
        gAiSelectionState.cursor = 0;
        gAiSelectionState.card = CARD_ID((u8 *)&gAiSelectionState.hand[0] + gAiState.bB * 4);
        gAiSelectionState.flags33 |= 2;
        gAiSelectionState.slot = gAiState.bB;
        gAiSelectionState.active.bits.active = 1;
        gAiState.state++;
        /* fall through */
    case 0xC9:
        if (CountActiveCardsOnField(0, 0x49C) || CountActiveCardsOnField(1, 0x49C)) CardMenu_PlaySpellTrapFromHand(0, 0, 0);
        else CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        {
            u8 active = gAiSelectionState.active.raw & 2;
            if (!active) gAiState.bA = active;
        }
        return 0;
    }
    return 1;
}


