#include "global.h"
#include "duel.h"

/*
 * Duel-state helpers: per-player deck / fusion-deck lists, zone setup, and
 * "is card X on my field" queries. See wiki/functions/code-08007994.md.
 *
 * The card/zone/player structures (struct DuelCard / DuelZone / DuelPlayer /
 * DuelState / DuelZonesPlayer) and gDuel / gDuelPlayers / gDuelZones
 * come from include/duel.h (canonical layouts and field names).
 */

/*
 * Local views kept because the canonical declarations in duel.h differ from what
 * this unit needs to match (duel.h is shared, so it is not changed here):
 *
 * - struct DuelCardBit18 (below): canonical struct DuelCard has unk13:7 spanning
 *   bits 13-19 and flag20 at bit 20, so the bit-18 flag that AddCardToDeckTop clears
 *   has no canonical field name.
 * - struct DuelZone90 (below): canonical struct DuelZone stops at +0x8C, but
 *   PlaceSpellTrapCard clears the u32 bitfield at +0x90 bit 18 (the ROM's `+0x92 &= 0xFC03`).
 * - struct DuelZoneFlags (below): canonical struct DuelZone declares +0x06 as the
 *   bitfields flag6_0/flag6_1, but the zone searches test those bits as plain byte
 *   masks (mov #2; ldrb; and).
 * - struct DuelSerial (below): canonical struct DuelState starts with `u32 unk0`, but
 *   this unit increments the placement serial as a u16 at +0x00.
 */
struct DuelCardBit18 {
    u32 id : 12;
    u32 owner : 1;
    u32 unk13 : 5;
    u32 bit18 : 1;
    u32 rest : 13;
};

struct DuelZone90 {
    u8 filler[0x90];
    u32 unk90_0 : 6;
    u32 unk90_6 : 4;
    u32 unk90_10 : 1;
    u32 unk90_11 : 1;
    u32 unk90_12 : 1;
    u32 unk90_13 : 5;
    u32 unk90_18 : 8;
    u32 unk90_26 : 6;
};

/* struct DuelZoneFlags: the +0x06 flags as a plain byte (see the list at the top of the
 * file). A struct member keeps the ROM's [rn, #6] displacement. */
struct DuelZoneFlags {
    u8 filler0[6];
    u8 flags6;
};

/* Save image (gSaveData, 0x02011C20): only the deck fields used here. */
struct SaveData {
    u8 filler0[0x2008];
    u16 deck[75];           /* +0x2008: card IDs of the player's deck (hypothesis: max 75) */
    u16 fusionDeck[21];     /* +0x209E: card IDs of the fusion deck (hypothesis) */
    u16 deckSize;           /* +0x20C8 */
    u8 filler20CA[2];
    u16 fusionDeckSize;     /* +0x20CC */
};
extern struct SaveData gSaveData;

/* Local view of the placement serial as a u16 at +0x00 (see the list at the top of the
 * file). PlaceMonsterCard writes it as a raw u16 too. */
struct DuelSerial {
    u16 serial;                     /* 0x020192E0 */
    u8 filler2[2];
    struct DuelPlayer players[2];   /* 0x020192E4 */
};
extern struct DuelSerial gUnk_020192E0_serial asm("gDuel");

/* The same zones addressed from their own base (player + 0x28); struct DuelZonesPlayer
 * comes from duel.h. */
#define ZONE(p, z) (gDuelZones[(p) & 1].zones[z])
/* ID of the card word at ptr (a zone or a list entry), read as a whole u32 (ldr + shifts)
 * through a struct DuelCard pointer; a plain .id member access would use ldrh. */
#define CARD_ID(zone) (((struct DuelCard *)(zone))->id)
/* Flags tested as plain byte masks (mov #2; ldrb; and), not as bitfields (see struct
 * DuelZoneFlags). */
#define ZONE_FLAGS(zone) (((struct DuelZoneFlags *)(zone))->flags6)
#define ZONE_FACEUP(zone) (ZONE_FLAGS(zone) & 2)
#define ZONE_FLAG90_11(zone) (((u8 *)(zone))[0x91] & 8)    /* unk90_11 */
/* Zone address as the ROM computes it: base + (zone * 0x94 + (player & 1) * 0xD64). */
#define ZONE_PTR(p, z) ((struct DuelZone *)((u8 *)gDuelZones + ((z) * 0x94 + ((p) & 1) * 0xD64)))
/* Same with the two offsets added the other way round (the add order shows in the code). */
#define ZONE_PTR2(p, z) ((struct DuelZone *)((u8 *)gDuelZones + (((p) & 1) * 0xD64 + (z) * 0x94)))
#define PLAYER(p) (gDuelPlayers[(p) & 1])

extern const u32 gCardStats[];   /* card stats, indexed by card ID */
extern const u16 gCardIdToNumber[];   /* card ID to card number */

/* A zone's card word, read through a struct DuelCard pointer (this is what the original code does). */
#define ZONE_CARD(p, z) ((struct DuelCard *)&PLAYER(p).zones[z])
#define CARD_NUMBER(id) (gCardIdToNumber[(id) & 0x7FF])
/* The same table through a constant address: GCC then keeps the 0x7FF mask, not the table, in a register. */
#define CARD_NUMBER_C(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS_C(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS_C(id) & 0x1F00000) >> 20)

/* Card "subtype" (same helper as in card_detail): 3 for card 1910, 1 for 1911-1912,
 * 7/8/9 for types 22/21/23, else stats bits 18-19. */
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER_C(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)CARD_TYPE(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return (CARD_STATS_C(id) & 0xC0000) >> 18;
    }
}

void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);
void SwapDuelCards(struct DuelCard *a, struct DuelCard *b);
u16 IsToonMonster(u16 cardNo);
void MemClear16(void *dst, int size);
u16 TakeDeckCardAt(int player, int idx, struct DuelCard *out);
void AddCardToHand(int player, struct DuelCard *card);
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone);
int CountActiveCardsOnField(int player, u16 cardNo);
int Random(void);                /* Random */
int GetZoneCardType(int player, int zone);
int GetZoneCardAttribute(int player, int zone);

/* Monster (type <= 20) with subtype 2. */
u32 IsFusionMonster(u16 id)
{
    if (CARD_TYPE(id) <= 20 && GetCardSubtype(id) == 2)
        return 1;
    return 0;
}
/* Put a card into a field zone: clear the zone, copy the card word, stamp the serial and flags. */
void PlaceMonsterCard(int player, int zone, struct DuelCard *card, u16 flag0, u16 faceUp)
{
    u8 *zbase = (u8 *)gDuelZones + (player & 1) * 0xD64;
    struct DuelZone *z = (struct DuelZone *)(zbase + zone * 0x94);
    u16 number;
    const u16 *tab = gCardIdToNumber;

    MemClear16(z, 0x94);
    CopyDuelCard(&z->card, card);
    z->serial = (*(u16 *)((u8 *)gDuelZones - 0x2C))++;
    z->flag6_0 = flag0;
    z->flag6_1 = faceUp;
    ((u8 *)z)[7] |= 0x20;
    ((u8 *)z)[7] |= 0x04;
    if (flag0 == 0 && faceUp != 0)
        ((struct DuelPlayer *)((u8 *)gDuelZones - 0x28 + (player & 1) * 0xD64))->zoneMask
            &= ~(1 << zone);
    number = tab[CARD_ID(card) & 0x7FF];
    if (IsToonMonster(number)) {
        u8 *flag = (u8 *)z + 0x8C;

        *flag |= 0x10;
    }
}
/* Place a card in field zone 5+zone, or the field zone (10) if it is a kind-2 type-22 card;
 * clears/initialises the +0x90 flag bits and stamps the serial. */
void PlaceSpellTrapCard(int player, int zone, struct DuelCard *card, u16 flag)
{
    u8 *zp = (u8 *)gDuelZones + (player & 1) * 0xD64;
    struct DuelZone *z = (struct DuelZone *)(zp + (zone * 0x94 + 0x2E4));
    u32 id = CARD_ID(card);
    u32 stats = CARD_STATS_C(id);

    if (((stats & 0x1F00000) >> 20) == 22 && ((stats & 0xE0000) >> 17) == 2)
        z = (struct DuelZone *)(zp + 0x5C8);
    CopyDuelCard(&z->card, card);
    z->serial = gUnk_020192E0_serial.serial++;
    z->flag6_0 = 0;
    z->flag6_1 = flag & 1;
    ((s8 *)z)[6] &= ~0x3C;
    ((s8 *)z)[0x91] &= ~4;
    *(u16 *)((u8 *)z + 0x90) |= 0x3C0;
    *(u32 *)((u8 *)z + 0x90) &= 0xFFFC1FFF;
    ((s8 *)z)[0x91] &= ~8;
    ((s8 *)z)[0x91] &= ~0x10;
    ((struct DuelZone90 *)z)->unk90_18 = 0;
    if (flag == 0) {
        stats = CARD_STATS_C(id);
        if (((stats & 0x1F00000) >> 20) == 22 && ((stats & 0xE0000) >> 17) != 5
            && CountActiveCardsOnField(0, 0x49C) == 0 && CountActiveCardsOnField(1, 0x49C) == 0)
            ((s8 *)z)[0x91] |= 4;
    }
}
/* Put a card on top of the deck (deck[0]), shifting the others down. */
void AddCardToDeckTop(int player, struct DuelCard *card)
{
    int i;

    for (i = PLAYER(player).deckCount; i > 0; i--)
        CopyDuelCard(&PLAYER(player).deck[i], &PLAYER(player).deck[i - 1]);
    PLAYER(player).deckCount++;
    ((struct DuelCardBit18 *)card)->bit18 = 0;
    CopyDuelCard(&PLAYER(player).deck[0], card);
}
/* Append a card to the bottom of the deck. */
void AddCardToDeckBottom(int player, struct DuelCard *card)
{
    CopyDuelCard(&PLAYER(player).deck[PLAYER(player).deckCount], card);
    PLAYER(player).deckCount++;
}
/* Append a card to the fusion deck (hypothesis). */
void AddCardToFusionDeck(int player, struct DuelCard *card)
{
    int n = PLAYER(player).fusionCount;

    CopyDuelCard(&PLAYER(player).fusionDeck[n], card);
    PLAYER(player).fusionCount++;
}
/* CopyDuelCard called without a prototype so a struct DuelCard can be passed by value. */
typedef void (*CopyByValFn_D50)();
/* Move a card with number cardNo to the top of the deck: skip the run of such cards already on
 * top, then bring the next one up. Original bugs reproduced: the loop bounds use PLAYER(i) (the
 * loop index) instead of PLAYER(player), and both CopyDuelCard calls pass a card word by value
 * where a pointer is expected (the source card, and the top card as the destination). */
void MoveDeckCardNumberToTop(int player, u16 cardNo)
{
    struct DuelCard tmp;
    int i, j;
    register int r9 asm("r9"); /* FAKEMATCH: see the asm below */

    /* FAKEMATCH: an empty asm that "sets" r9 marks it as used, so global alloc puts the
     * loop-2 base pointer in r9 (pass 0) instead of ip, leaving ip for the deck base. */
    asm volatile("" : "=r"(r9));
    for (i = 0; i < PLAYER(i).deckCount; i++) {
        /* The u16 id and the mask local keep 0x7FF in a register (r9) and the table load
         * short-lived, so loop.c hoists the deck-base loads in the ROM's order. */
        u16 id = CARD_ID(&PLAYER(player).deck[i]);
        u32 mask = 0x7FF;
        if (gCardIdToNumber[id & mask] != cardNo)
            break;
    }
    for (; i < PLAYER(i).deckCount; i++) {
        struct DuelCard c = *&PLAYER(player).deck[i];
        if (CARD_NUMBER(c.id) == cardNo) {
            ((CopyByValFn_D50)CopyDuelCard)(&tmp, c);
            for (j = i; j > 0; j--)
                SwapDuelCards(&PLAYER(player).deck[j], &PLAYER(player).deck[j - 1]);
            ((CopyByValFn_D50)CopyDuelCard)(PLAYER(player).deck[0], &tmp);
            return;
        }
    }
}
/* Shuffle the deck: n * deckCount random swaps. */
void ShuffleDeck(int player, int n)
{
    int i;

    if (PLAYER(player).deckCount == 0)
        return;
    n *= PLAYER(player).deckCount;
    for (i = 0; i < n; i++) {
        int a = Random() % PLAYER(player).deckCount;
        int b = Random() % PLAYER(player).deckCount;
        SwapDuelCards(&PLAYER(player).deck[a], &PLAYER(player).deck[b]);
    }
}
/* Remove deck[idx], closing the gap (inlined into several callers). */
static inline void DeckRemoveAt(int player, int idx)
{
    int i;

    PLAYER(player).deckCount--;
    for (i = idx; i < PLAYER(player).deckCount; i++)
        CopyDuelCard(&PLAYER(player).deck[i], &PLAYER(player).deck[i + 1]);
}

/* Take card idx out of the deck into *out. Returns 0 if idx is out of range. */
u16 TakeDeckCardAt(int player, int idx, struct DuelCard *out)
{
    if (idx < PLAYER(player).deckCount) {
        CopyDuelCard(out, &PLAYER(player).deck[idx]);
        DeckRemoveAt(player, idx);
        return 1;
    }
    return 0;
}
/* Remove the first deck card equal to *card (whole word). Returns 1 if found. */
int RemoveCardFromDeck(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < PLAYER(player).deckCount; i++) {
        struct DuelCard *entry = &PLAYER(player).deck[i];

        if (*(u32 *)card == *(u32 *)entry) {
            DeckRemoveAt(player, i);
            return 1;
        }
    }
    return 0;
}
/* 0x08007F48 size 0xA4 */
/* Take the first deck card with number cardNo into *out. Returns 1 if found. */
u16 TakeDeckCardByNumber(int player, u16 cardNo, struct DuelCard *out)
{
    int i;

    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (CARD_NUMBER_C(CARD_ID(&PLAYER(player).deck[i])) == cardNo) {
            CopyDuelCard(out, &PLAYER(player).deck[i]);
            DeckRemoveAt(player, i);
            return 1;
        }
    }
    return 0;
}
/* Draw: take the top card of the deck and add it to the hand. */
void DuelDrawCard(int player)
{
    struct DuelCard card;

    if (TakeDeckCardAt(player, 0, &card))
        AddCardToHand(player, &card);
}
/* Remove fusionDeck[idx], closing the gap (inlined into its callers). */
static inline void FusionRemoveAt(int player, int idx)
{
    int i;

    PLAYER(player).fusionCount--;
    for (i = idx; i < PLAYER(player).fusionCount; i++)
        CopyDuelCard(&PLAYER(player).fusionDeck[i], &PLAYER(player).fusionDeck[i + 1]);
}
/* Remove the first fusion-deck card equal to *card (whole word). Returns 1 if found. */
int RemoveCardFromFusionDeck(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < PLAYER(player).fusionCount; i++) {
        struct DuelCard *entry = &PLAYER(player).fusionDeck[i];

        if (*(u32 *)card == *(u32 *)entry) {
            FusionRemoveAt(player, i);
            return 1;
        }
    }
    return 0;
}
/* Load player 0's deck and fusion deck from the save image. */
void LoadPlayerDeckFromSave(void)
{
    int i;

    MemClear16(gDuelPlayers[0].deck, sizeof(gDuelPlayers[0].deck));
    gDuelPlayers[0].deckCount = 0;
    for (i = 0; i < gSaveData.deckSize; i++) {
        struct DuelCard *c = &gDuelPlayers[0].deck[i];
        c->id = gSaveData.deck[i];
        c->owner = 0;
        gDuelPlayers[0].deckCount++;
    }
    gDuelPlayers[0].fusionCount = 0;
    for (i = 0; i < gSaveData.fusionDeckSize; i++) {
        struct DuelCard *c = &gDuelPlayers[0].fusionDeck[i];
        c->id = gSaveData.fusionDeck[i];
        c->owner = 0;
        gDuelPlayers[0].fusionCount++;
    }
}
/* Clear one field zone. */
void ClearZone(int player, int zone)
{
    MemClear16(&PLAYER(player).zones[zone], 0x94);
}
/* Remove link idx from a zone's link list. */
void RemoveZoneLinkAt(int player, int zone, int idx)
{
    struct DuelZone *z = &PLAYER(player).zones[zone];

    if (idx < z->numLinks) {
        z->numLinks--;
        for (; idx < z->numLinks; idx++) {
            z->links[idx] = z->links[idx + 1];
            z->linkKinds[idx] = z->linkKinds[idx + 1];
        }
    }
}
/* Index 1-14 of a card number in a fixed list (329-334, 1069, 1125-1130, 1547), else 0. */
u32 GetFieldMagicIndex(u16 cardNo)
{
    switch (cardNo) {
    case 329:
        return 1;
    case 330:
        return 2;
    case 331:
        return 3;
    case 332:
        return 4;
    case 333:
        return 5;
    case 334:
        return 6;
    case 1069:
        return 7;
    case 1125:
        return 8;
    case 1126:
        return 9;
    case 1127:
        return 10;
    case 1128:
        return 11;
    case 1129:
        return 12;
    case 1130:
        return 13;
    case 1547:
        return 14;
    }
    return 0;
}
/* First face-up zone (0-10, except skipZone) with number cardNo, or -1. */
int FindFaceUpCardOnField(int player, u16 cardNo, int skipZone)
{
    int i;

    for (i = 0; i <= 10; i++) {
        struct DuelZone *zone = ZONE_PTR(player, i);
        u16 id = CARD_ID(zone);
        if (id != 0 && i != skipZone && ZONE_FACEUP(zone) && CARD_NUMBER_C(id) == cardNo)
            return i;
    }
    return -1;
}
/* Identical copy of FindFaceUpCardOnField. */
int FindFaceUpCardOnField2(int player, u16 cardNo, int skipZone)
{
    int i;

    for (i = 0; i <= 10; i++) {
        struct DuelZone *zone = ZONE_PTR(player, i);
        u16 id = CARD_ID(zone);
        if (id != 0 && i != skipZone && ZONE_FACEUP(zone) && CARD_NUMBER_C(id) == cardNo)
            return i;
    }
    return -1;
}
/* Number of face-up zones (0-10, except skipZone, not flagged unk91_3) with number cardNo. */
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone)
{
    int count = 0;
    int i;

    for (i = 0; i <= 10; i++) {
        struct DuelZone *zone = ZONE_PTR(player, i);
        u16 id = CARD_ID(zone);
        if (id != 0 && i != skipZone && ZONE_FACEUP(zone) && !ZONE_FLAG90_11(zone)
            && CARD_NUMBER_C(id) == cardNo)
            count++;
    }
    return count;
}
/* Count cards with number cardNo on player's field (all zones). */
int CountActiveCardsOnField(int player, u16 cardNo)
{
    return CountActiveCardsOnFieldExcept(player, cardNo, -1);
}
/* Count magic/trap zones with this card number and flag 0x91 bit 3 clear. */
int CountEnabledSpellTrapCards(int player, u16 cardNo)
{
    int count = 0;
    int i = 5;
    u32 offset = (player & 1) * 0xD64;
    u8 *zoneBase = (u8 *)gDuelZones;
    struct DuelZone *zones = (struct DuelZone *)(zoneBase + offset);
    u32 mask = 0x7FF;

    for (; i <= 9; i++) {
        u32 stride = i * 0x94;
        struct DuelCard *entry = (struct DuelCard *)((u32)zones + stride);
        u16 id = entry->id;

        if (id && !(zoneBase[offset + i * 0x94 + 0x91] & 8) && ((const u16 *)0x08622AB4)[id & mask] == cardNo)
            count++;
    }
    return count;
}
/* Number of face-up monsters (zones 0-4) for which GetZoneCardType(player, zone) == value. */
int CountFaceUpMonstersOfType(int player, u16 value)
{
    int count = 0;
    int i;

    for (i = 0; i <= 4; i++) {
        if (ZONE_FACEUP(ZONE_PTR2(player, i)) && GetZoneCardType(player, i) == value)
            count++;
    }
    return count;
}
/* Number of face-up monsters (zones 0-4) for which GetZoneCardAttribute(player, zone) == value. */
int CountFaceUpMonstersOfAttribute(int player, u16 value)
{
    int count = 0;
    int i;

    for (i = 0; i <= 4; i++) {
        if (GetZoneCardAttribute(player, i) == value && ZONE_FACEUP(ZONE_PTR2(player, i)))
            count++;
    }
    return count;
}
/* True if card number 954 is face up in one of the magic/trap zones (5-9). */
int HasFaceUpToonWorld(int player)
{
    int i;

    for (i = 5; i <= 9; i++) {
        struct DuelZone *zone = ZONE_PTR(player, i);
        u32 id = CARD_ID(zone);
        if (id != 0 && ZONE_FACEUP(zone) && CARD_NUMBER_C(id) == 954)
            return 1;
    }
    return 0;
}
/* Number of face-up monsters (zones 0-4) with number cardNo. */
int CountFaceUpMonstersByNumber(int player, u16 cardNo)
{
    int count = 0;
    int i;

    for (i = 0; i <= 4; i++) {
        struct DuelZone *zone = ZONE_PTR(player, i);
        u16 id = CARD_ID(zone);
        if (id != 0 && ZONE_FACEUP(zone) && CARD_NUMBER_C(id) == cardNo)
            count++;
    }
    return count;
}
/* First face-up monster zone (0-4) with number cardNo, or -1. */
int FindFaceUpMonsterByNumber(int player, u16 cardNo)
{
    int i;

    for (i = 0; i <= 4; i++) {
        struct DuelZone *zone = ZONE_PTR(player, i);
        u16 id = CARD_ID(zone);
        if (id != 0 && ZONE_FACEUP(zone) && CARD_NUMBER_C(id) == cardNo)
            return i;
    }
    return -1;
}
/* Number of monster zones (0-4) holding a card with number cardNo. */
int CountMonstersByNumber(int player, u16 cardNo)
{
    int count = 0;
    int i;

    for (i = 0; i <= 4; i++) {
        u16 id = CARD_ID(ZONE_PTR2(player, i));
        if (id != 0 && CARD_NUMBER_C(id) == cardNo)
            count++;
    }
    return count;
}
/* Number of face-up monsters (zones 0-4) with number cardNo and unk6_0 == flag. */
int CountFaceUpMonstersByNumberInPosition(int player, u16 cardNo, u16 flag)
{
    int count = 0;
    int i;

    /* FAKEMATCH: do/while(0) groups the whole body so GCC schedules the
     * zone pointer and card ID into the ROM's registers (found with the permuter). */
    do {
        for (i = 0; i <= 4; i++) {
            struct DuelZone *zone = ZONE_PTR(player, i);
            u32 id = CARD_ID(zone);
            u8 flags;
            if (id != 0 && ((flags = ZONE_FLAGS(zone)) & 2) && ((u32)flags << 31) >> 31 == flag
                && CARD_NUMBER_C(id) == cardNo)
                count++;
        }
        return count;
    } while (0);
}
/* Count occupied monster zones. */
/* Return the zero-extended halfword as a word, as the ROM callers consume it. */
int CountMonsters(int player)
{
    u16 count = 0;
    u16 i;

    for (i = 0; i < 5; i++) {
        if (CARD_ID(ZONE_PTR2(player, i)))
            count++;
    }
    return (u16)(count);
}
/* Count occupied monster zones, optionally only face-up ones and/or only those with flags6 bit 0 clear. */
/* The count remains zero-extended while word consumers compare it directly. */
int CountMonstersFiltered(int player, u16 needFaceUp, u16 needBit0Clear)
{
    u16 count = 0;
    u16 i;

    for (i = 0; i < 5; i++) {
        if (CARD_ID(ZONE_PTR2(player, i))) {
            int ok1 = 0;
            int ok2 = 0;

            if (!needFaceUp || (ZONE_FLAGS(ZONE_PTR2(player, i)) & 2))
                ok1 = 1;
            if (!needBit0Clear || !(ZONE_FLAGS(ZONE_PTR2(player, i)) & 1))
                ok2 = 1;
            if (ok1 && ok2)
                count++;
        }
    }
    return (u16)count;
}
/* IsMonsterZoneFree's view of a player's header (it is addressed from gDuelZones - 0x28): a
 * 5-bit zone mask that straddles +0x0B bits 4-7 and +0x0C bit 0. The packed bitfield gives the
 * ROM's split read (ldrb +0x0B >> 4, then (ldrb +0x0C & 1) << 4 ORed in that operand order). */
struct PlayerMask8940 {
    u8 filler[0xB];
    u16 lo:4;
    u16 zoneMask:5;
} __attribute__((packed));
/* Zone address with the offsets summed first (the ROM's add order). */
static inline struct DuelZone *ZoneAt8940(int p, int z)
{
    return (struct DuelZone *)((u8 *)gDuelZones + (z * 0x94 + p * 0xD64));
}
/* The same address with the base loaded into a local first. FAKEMATCH: the early base load
 * lets loop.c hoist the whole linkKinds address in its first pass, so strength reduction
 * walks linkKinds with a pointer started at ((p * 0xD64 + 0x4A) + z * 0x94) + base. */
static inline struct DuelZone *ZoneAtB8940(int p, int z)
{
    u8 *base = (u8 *)gDuelZones;
    return (struct DuelZone *)(base + (z * 0x94 + p * 0xD64));
}
/* Whether a zone is free for use (hypothesis): empty, its bit in the player's 5-bit zone mask
 * clear, and no kind-2 link to a zone holding card number 1320. */
u32 IsMonsterZoneFree(int player, int zone)
{
    struct DuelZone *z = ZoneAt8940(player & 1, zone);
    struct PlayerMask8940 *pl;
    int i;

    if (CARD_ID(z))
        return 0;
    pl = (struct PlayerMask8940 *)((u8 *)gDuelZones - 0x28 + (player & 1) * 0xD64);
    if ((pl->zoneMask >> zone) & 1)
        return 0;
    /* FAKEMATCH: the loop test recomputes the zone address instead of using z; its movables
     * match the linkKinds ones, so loop.c hoists numLinks out of the loop. */
    for (i = 0; i < ZoneAt8940(player & 1, zone)->numLinks; i++) {
        u8 lp = z->links[i];
        u16 lz = z->links[i] >> 8;
        if ((u8)ZoneAtB8940(player & 1, zone)->linkKinds[i] == 2) {
            u16 id = CARD_ID(ZoneAtB8940(lp & 1, lz));
            if (id != 0 && CARD_NUMBER_C(id) == 1320)
                return 0;
        }
    }
    return 1;
}
