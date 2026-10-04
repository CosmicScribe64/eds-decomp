/*
 * Duel card lists and field queries (0x08007994-0x08008A1B).
 *
 * - The decks: put a card on top of or under a player's deck, shuffle, draw, take a card by position or by
 *   card number, remove a given card; the same for the fusion deck; load player 0's decks from the save.
 * - The field: place a card into a monster zone or a spell/trap zone, clear a zone, remove a zone link.
 * - Field queries: find or count the cards of a given card number, monster type or attribute on a player's
 *   field. CountActiveCardsOnField ("is card X in effect for this player?") is one of the most-called
 *   routines of the game.
 *
 * Zones 0-4 hold monsters, 5-9 spells and traps, 10 the Field Magic (enum DuelZoneIndex). Cards are stored by
 * card ID; the "is it card X" tests compare card numbers (enum CardNumber) through gCardIdToNumber.
 * Wiki: wiki/functions/duel-card-lists-c.md.
 */
#include "global.h"
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_*, CARD_KIND_*, SPELL_*, gCardStats bit layout */
#include "constants/duel.h"         /* ZONE_*, ZONE_LINK_* */
#include "card_data.h"              /* gCardIdToNumber, CARD_ID_MASK, CARD_STATS_TYPE/SUBTYPE */
#include "legacy/duel.h"                   /* struct DuelCard/DuelZone/DuelPlayer, gDuel, gDuelPlayers, gDuelZones */
#include "save.h"                   /* gSaveData (saved decks), LoadPlayerDeckFromSave */
#include "util.h"                   /* MemClear16, Random */

/*
 * Before H0 (build/readability/HEADERS.md) include/duel.h is still the legacy header: it has no function
 * prototypes and none of the zone, player and card bit names used here. These prototypes are copied unchanged
 * from the canonical duel.h, and struct DuelZoneView / DuelPlayerView / DuelCardView and gDuelSerial below
 * give this unit the canonical field names. The unit compiles to the same code with either header; once the
 * canonical duel.h is installed, delete this bridge and use the canonical structs (the edits are listed in
 * build/readability/issues/duel_card_lists.md).
 */
void AddCardToHand(int player, struct DuelCard *card);
int CountActiveCardsOnField(int player, u16 cardNo);
u32 GetZoneCardType(s32 player, s32 slot);
u32 GetZoneCardAttribute(s32 player, s32 slot);

/* Local view of a field zone (struct DuelZone, 0x94 bytes) with the canonical names of the fields used here. */
struct DuelZoneView {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04: gDuel.serial when the card was placed */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5: turns a face-up card has been active */
    u16 destroyCountdown:4;         /* +0x06 bits 6-9 */
    u8 positionLocked:1;            /* +0x07 bit 2: cannot change position */
    u8 unk7_3:1;
    u8 unk7_4:1;
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet */
    u8 unk7_6:2;
    u8 unk8[2];
    u16 links[32];                  /* +0x0A: DUEL_LOC of a card affecting this one (or a value) */
    u16 linkKinds[32];              /* +0x4A: low byte enum ZoneLinkKind */
    u16 numLinks;                   /* +0x8A */
    u8 unk8C_0:4;                   /* +0x8C */
    u8 cannotAttack:1;              /* +0x8C bit 4 */
    u8 unk8C_5:3;
    u8 unk8D[3];
    u32 unk90_0:6;                  /* +0x90 */
    u32 unk90_6:4;                  /* +0x90 bits 6-9 */
    u32 canActivate:1;              /* +0x91 bit 2: a set card that may be activated */
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:1;
    u32 declaredValue:5;            /* +0x91 bits 5-9: value chosen when the card resolved */
    u32 unk92_2:8;                  /* +0x92 bits 2-9 (word bits 18-25): cleared as one 8-bit field. Also a
                                     * matching view: the canonical unk92_2:14 gives another mask. */
    u32 unk93_2:6;
};

/* Local view of a player (struct DuelPlayer) with the canonical names of the two masks used here. */
struct DuelPlayerView {
    u8 unk0[0xB];
    u8 unkB_0:4;
    u32 removedMask:5;              /* +0x0B bit 4 .. +0x0C bit 0: monster zones held for a monster banished
                                     * until the End Phase */
    u8 unkC_1:7;
    u8 unkD[0x26 - 0xD];
    u16 attackedMask;               /* +0x26: monster zones that have attacked this turn */
};

/* Local view of a card word (struct DuelCard) with the canonical name of bit 18. */
struct DuelCardView {
    u32 id:12;
    u32 owner:1;
    u32 unk13:5;                    /* bits 13-17 */
    u32 graverobbed:1;              /* bit 18: taken with Graverobber; cleared when it leaves the field */
    u32 unk19:13;
};

STATIC_ASSERT(sizeof(struct DuelZoneView) == 0x94, DuelZoneViewSize);
STATIC_ASSERT(OFFSET_OF(struct DuelZoneView, numLinks) == 0x8A, DuelZoneViewNumLinks);
STATIC_ASSERT(OFFSET_OF(struct DuelZoneView, unk8D) == 0x8D, DuelZoneViewUnk8D);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayerView, attackedMask) == 0x26, DuelPlayerViewAttackedMask);
STATIC_ASSERT(sizeof(struct DuelCardView) == 4, DuelCardViewSize);

/* gDuel.serial, the u16 placement counter at the start of gDuel. */
extern u16 gDuelSerial asm("gDuel");

/*
 * Callee views (same symbols; matching choices, not pre-H0 workarounds):
 * - CopyDuelCard and SwapDuelCards are defined with u32 * parameters (card_detail.c); this unit passes
 *   struct DuelCard pointers. Only the pointer types differ, the code is the same.
 * - IsToonMonster returns u32, but this unit reads the result as a u16 (lsl #16 before the test).
 */
void CopyDuelCardStruct(struct DuelCard *dst, struct DuelCard *src) asm("CopyDuelCard");
void SwapDuelCardsStruct(struct DuelCard *a, struct DuelCard *b) asm("SwapDuelCards");
u16 IsToonMonsterU16(u16 cardNo) asm("IsToonMonster");

/* gDuelPlayers[player] (player 0 or 1). */
#define PLAYER(player) (gDuelPlayers[(player) & 1])

#define DUEL_ZONE_SIZE   sizeof(struct DuelZone)     /* 0x94 */
#define DUEL_PLAYER_SIZE sizeof(struct DuelPlayer)   /* 0xD64: the player stride */

/*
 * Zone addresses as the ROM computes them from the gDuelZones base: base + (zone offset + player offset), or
 * with the two offsets added the other way round. The add order shows in the code, so each function uses
 * the one that matches.
 */
#define ZONE_ADDR(player, zone) \
    ((struct DuelZoneView *)((u8 *)gDuelZones + ((zone) * DUEL_ZONE_SIZE + ((player) & 1) * DUEL_PLAYER_SIZE)))
#define ZONE_ADDR_PLAYER_FIRST(player, zone) \
    ((struct DuelZoneView *)((u8 *)gDuelZones + (((player) & 1) * DUEL_PLAYER_SIZE + (zone) * DUEL_ZONE_SIZE)))

/* gDuelPlayers[player] and gDuel.serial, addressed from the gDuelZones base as some functions do (one literal
 * for everything): gDuelZones = gDuelPlayers[0].zones = (u8 *)gDuelPlayers + 0x28 = (u8 *)&gDuel + 0x2C. */
#define PLAYER_VIA_ZONES(player) \
    ((struct DuelPlayerView *)((u8 *)gDuelZones - 0x28 + ((player) & 1) * DUEL_PLAYER_SIZE))
#define DUEL_SERIAL_VIA_ZONES (*(u16 *)((u8 *)gDuelZones - 0x2C))

/* The card ID of the card word at ptr (a zone or a pile entry), read through a struct DuelCard pointer: the
 * ROM loads the whole word (ldr, then shifts), where an .id access on an array element gives ldrh. */
#define CARD_ID(ptr) (((struct DuelCard *)(ptr))->id)

/* gCardIdToNumber and gCardStats through their integer-constant addresses: this form keeps the
 * CARD_ID_MASK constant, not the table address, in a register (a matching choice; see card_data.h). */
#define CARD_NUMBER_C(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_STATS_C(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE_C(id) CARD_STATS_TYPE(CARD_STATS_C(id))

/*
 * enum CardKind of a card (the card frame): Obelisk is drawn as a Ritual monster, Slifer and Ra as Effect
 * monsters; Magic, Trap and Ticket cards have their own kinds; other monsters use the kind bits of their
 * stats. The same inline is repeated in card_detail.c and other units.
 */
static inline int GetCardKind(u16 cardId)
{
    switch (CARD_NUMBER_C(cardId)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((u8)CARD_TYPE_C(cardId)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_STATS_KIND(CARD_STATS_C(cardId));
    }
}

/* 1 if the card is a Fusion monster (a monster type and kind CARD_KIND_FUSION), else 0. */
u32 IsFusionMonster(u16 cardId)
{
    if (CARD_TYPE_C(cardId) <= CARD_TYPE_REPTILE && GetCardKind(cardId) == CARD_KIND_FUSION)
        return TRUE;
    return FALSE;
}

/*
 * Put a card into a monster zone: clear the zone, copy the card word, stamp the placement serial, set the
 * position (defense) and face (faceUp), and mark the card's one-shot effect unused and its position locked
 * for this turn. A face-up attack-position monster gets the zone's attackedMask bit cleared, so it may
 * attack this turn; a Toon monster cannot attack on the turn it is placed.
 */
void PlaceMonsterCard(int player, int zone, struct DuelCard *card, u16 defense, u16 faceUp)
{
    u8 *playerZones = (u8 *)gDuelZones + (player & 1) * DUEL_PLAYER_SIZE;
    struct DuelZoneView *z = (struct DuelZoneView *)(playerZones + zone * DUEL_ZONE_SIZE);
    u16 cardNo;
    /* Through a local, the table address is loaded after the card word, as in the ROM. */
    const u16 *idToNumber = gCardIdToNumber;

    MemClear16(z, DUEL_ZONE_SIZE);
    CopyDuelCardStruct(&z->card, card);
    z->serial = DUEL_SERIAL_VIA_ZONES++;
    z->isDefense = defense;
    z->isFaceUp = faceUp;
    z->effectUnused = TRUE;
    z->positionLocked = TRUE;
    if (!defense && faceUp)
        PLAYER_VIA_ZONES(player)->attackedMask &= ~(1 << zone);
    cardNo = idToNumber[CARD_ID(card) & CARD_ID_MASK];
    if (IsToonMonsterU16(cardNo))
        z->cannotAttack = TRUE;
}

/*
 * Put a Magic or Trap card into spell/trap zone ZONE_SPELL_0 + slot (a Field Magic goes to ZONE_FIELD),
 * stamp the serial and reset the zone's state. A card set face down (faceUp 0) may be activated at once if
 * it is a Magic card other than a Quick-Play, unless Anti-Magic Fragrance is active on either side; the
 * others get canActivate at their controller's next turn start (DuelCmd_TurnStart).
 */
void PlaceSpellTrapCard(int player, int slot, struct DuelCard *card, u16 faceUp)
{
    u8 *playerZones = (u8 *)gDuelZones + (player & 1) * DUEL_PLAYER_SIZE;
    struct DuelZoneView *z =
        (struct DuelZoneView *)(playerZones + (slot * DUEL_ZONE_SIZE + ZONE_SPELL_0 * DUEL_ZONE_SIZE));
    u32 id = CARD_ID(card);
    u32 stats = CARD_STATS_C(id);

    if (CARD_STATS_TYPE(stats) == CARD_TYPE_MAGIC && CARD_STATS_SUBTYPE(stats) == SPELL_FIELD)
        z = (struct DuelZoneView *)(playerZones + ZONE_FIELD * DUEL_ZONE_SIZE);
    CopyDuelCardStruct(&z->card, card);
    z->serial = gDuelSerial++;
    z->isDefense = FALSE;
    z->isFaceUp = faceUp & 1;
    z->turnCounter = 0;
    z->canActivate = FALSE;
    z->unk90_6 = 0xF;
    z->declaredValue = 0;
    z->isDisabled = FALSE;
    z->unk91_4 = 0;
    z->unk92_2 = 0;
    if (!faceUp) {
        stats = CARD_STATS_C(id);
        if (CARD_STATS_TYPE(stats) == CARD_TYPE_MAGIC && CARD_STATS_SUBTYPE(stats) != SPELL_QUICK_PLAY
            && CountActiveCardsOnField(0, CARD_ANTI_MAGIC_FRAGRANCE) == 0
            && CountActiveCardsOnField(1, CARD_ANTI_MAGIC_FRAGRANCE) == 0)
            z->canActivate = TRUE;
    }
}

/* Put a card on top of the deck (deck[0]), shifting the others down. Clears its graverobbed bit. */
void AddCardToDeckTop(int player, struct DuelCard *card)
{
    int i;

    for (i = PLAYER(player).deckCount; i > 0; i--)
        CopyDuelCardStruct(&PLAYER(player).deck[i], &PLAYER(player).deck[i - 1]);
    PLAYER(player).deckCount++;
    ((struct DuelCardView *)card)->graverobbed = FALSE;
    CopyDuelCardStruct(&PLAYER(player).deck[0], card);
}

/* Put a card at the bottom of the deck. */
void AddCardToDeckBottom(int player, struct DuelCard *card)
{
    CopyDuelCardStruct(&PLAYER(player).deck[PLAYER(player).deckCount], card);
    PLAYER(player).deckCount++;
}

/* Append a card to the fusion deck. */
void AddCardToFusionDeck(int player, struct DuelCard *card)
{
    int count = PLAYER(player).fusionCount;

    CopyDuelCardStruct(&PLAYER(player).fusionDeck[count], card);
    PLAYER(player).fusionCount++;
}

/* CopyDuelCard called without a prototype, so that a struct DuelCard can be passed by value (see below). */
typedef void (*CopyDuelCardUnprototyped)();

/*
 * Bring a copy of card number cardNo to the top of the deck: skip the run of copies already on top, then move
 * the next copy to deck[0], shifting the cards above it down. Unused (no callers).
 * Two bugs of the original are reproduced: the loop bounds read PLAYER(i).deckCount (the loop index, not the
 * player), and both CopyDuelCard calls pass a card word by value where a pointer is expected (the source
 * card, then the top card as the destination).
 */
void MoveDeckCardNumberToTop(int player, u16 cardNo)
{
    struct DuelCard picked;
    int i, j;
    register int r9 asm("r9"); /* FAKEMATCH: see the asm below */

    /* FAKEMATCH: an empty asm that "sets" r9 marks it as used, so global alloc puts the second loop's base
     * pointer in r9 (pass 0) instead of ip, leaving ip for the deck base. */
    asm volatile("" : "=r"(r9));
    for (i = 0; i < PLAYER(i).deckCount; i++) {
        /* The u16 id and the mask local keep 0x7FF in a register and the table load short-lived, so
         * loop.c hoists the deck-base loads in the ROM's order. */
        u16 id = CARD_ID(&PLAYER(player).deck[i]);
        u32 mask = CARD_ID_MASK;

        if (gCardIdToNumber[id & mask] != cardNo)
            break;
    }
    for (; i < PLAYER(i).deckCount; i++) {
        /* Read through *&: a plain deck[i] builds the address as base + offset + 0x7C4 and keeps the base
         * live, unlike the ROM. */
        struct DuelCard card = *&PLAYER(player).deck[i];

        if (gCardIdToNumber[card.id & CARD_ID_MASK] == cardNo) {
            ((CopyDuelCardUnprototyped)CopyDuelCardStruct)(&picked, card);
            for (j = i; j > 0; j--)
                SwapDuelCardsStruct(&PLAYER(player).deck[j], &PLAYER(player).deck[j - 1]);
            ((CopyDuelCardUnprototyped)CopyDuelCardStruct)(PLAYER(player).deck[0], &picked);
            return;
        }
    }
}

/* Shuffle the deck: passes * deckCount swaps of two random cards. The callers pass 8 (Campaign duel setup),
 * 4 (link duel setup) and 3 (DuelCmd_ShuffleDeck). */
void ShuffleDeck(int player, int passes)
{
    int i;

    if (PLAYER(player).deckCount == 0)
        return;
    passes *= PLAYER(player).deckCount;
    for (i = 0; i < passes; i++) {
        int a = Random() % PLAYER(player).deckCount;
        int b = Random() % PLAYER(player).deckCount;

        SwapDuelCardsStruct(&PLAYER(player).deck[a], &PLAYER(player).deck[b]);
    }
}

/* Remove deck[idx] and close the gap (inlined into its callers; each copy recomputes the player offset). */
static inline void DeckRemoveAt(int player, int idx)
{
    int i;

    PLAYER(player).deckCount--;
    for (i = idx; i < PLAYER(player).deckCount; i++)
        CopyDuelCardStruct(&PLAYER(player).deck[i], &PLAYER(player).deck[i + 1]);
}

/* Take deck[idx] out of the deck into *out. Returns 1, or 0 if idx >= deckCount. */
u16 TakeDeckCardAt(int player, int idx, struct DuelCard *out)
{
    if (idx < PLAYER(player).deckCount) {
        CopyDuelCardStruct(out, &PLAYER(player).deck[idx]);
        DeckRemoveAt(player, idx);
        return TRUE;
    }
    return FALSE;
}

/* Remove the first deck card whose whole card word equals *card. Returns 1 if one was found. */
int RemoveCardFromDeck(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < PLAYER(player).deckCount; i++) {
        struct DuelCard *entry = &PLAYER(player).deck[i];

        if (*(u32 *)card == *(u32 *)entry) {
            DeckRemoveAt(player, i);
            return TRUE;
        }
    }
    return FALSE;
}

/* Take the first deck card with card number cardNo out of the deck into *out. Returns 1 if one was found. */
u16 TakeDeckCardByNumber(int player, u16 cardNo, struct DuelCard *out)
{
    int i;

    for (i = 0; i < PLAYER(player).deckCount; i++) {
        if (CARD_NUMBER_C(CARD_ID(&PLAYER(player).deck[i])) == cardNo) {
            CopyDuelCardStruct(out, &PLAYER(player).deck[i]);
            DeckRemoveAt(player, i);
            return TRUE;
        }
    }
    return FALSE;
}

/* Move the top card of the deck to the hand (no animation, no triggers). */
void DuelDrawCard(int player)
{
    struct DuelCard card;

    if (TakeDeckCardAt(player, 0, &card))
        AddCardToHand(player, &card);
}

/* Remove fusionDeck[idx] and close the gap (inlined into its caller). */
static inline void FusionDeckRemoveAt(int player, int idx)
{
    int i;

    PLAYER(player).fusionCount--;
    for (i = idx; i < PLAYER(player).fusionCount; i++)
        CopyDuelCardStruct(&PLAYER(player).fusionDeck[i], &PLAYER(player).fusionDeck[i + 1]);
}

/* Remove the first fusion-deck card whose whole card word equals *card. Returns 1 if one was found. */
int RemoveCardFromFusionDeck(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < PLAYER(player).fusionCount; i++) {
        struct DuelCard *entry = &PLAYER(player).fusionDeck[i];

        if (*(u32 *)card == *(u32 *)entry) {
            FusionDeckRemoveAt(player, i);
            return TRUE;
        }
    }
    return FALSE;
}

/* Load player 0's deck and fusion deck from the saved Deck and Fusion Deck (card IDs, owner player 0). */
void LoadPlayerDeckFromSave(void)
{
    int i;

    MemClear16(gDuelPlayers[0].deck, sizeof(gDuelPlayers[0].deck));
    gDuelPlayers[0].deckCount = 0;
    for (i = 0; i < gSaveData.deckSize; i++) {
        struct DuelCard *entry = &gDuelPlayers[0].deck[i];

        entry->id = gSaveData.deck[i];
        entry->owner = 0;
        gDuelPlayers[0].deckCount++;
    }
    gDuelPlayers[0].fusionCount = 0;
    for (i = 0; i < gSaveData.fusionDeckSize; i++) {
        struct DuelCard *entry = &gDuelPlayers[0].fusionDeck[i];

        entry->id = gSaveData.fusionDeck[i];
        entry->owner = 0;
        gDuelPlayers[0].fusionCount++;
    }
}

/* Clear a field zone (card, flags and links). */
void ClearZone(int player, int zone)
{
    MemClear16(&PLAYER(player).zones[zone], DUEL_ZONE_SIZE);
}

/* Remove link idx of a zone, shifting the later links down. */
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

/* Field background index 1-14 of a Field Magic card number, else 0. CARD_1547 is not an EDS card. */
u32 GetFieldMagicIndex(u16 cardNo)
{
    switch (cardNo) {
    case CARD_FOREST:
        return 1;
    case CARD_WASTELAND:
        return 2;
    case CARD_MOUNTAIN:
        return 3;
    case CARD_SOGEN:
        return 4;
    case CARD_UMI:
        return 5;
    case CARD_YAMI:
        return 6;
    case CARD_CHORUS_OF_SANCTUARY:
        return 7;
    case CARD_GAIA_POWER:
        return 8;
    case CARD_UMIIRUKA:
        return 9;
    case CARD_MOLTEN_DESTRUCTION:
        return 10;
    case CARD_RISING_AIR_CURRENT:
        return 11;
    case CARD_LUMINOUS_SPARK:
        return 12;
    case CARD_MYSTIC_PLASMA_ZONE:
        return 13;
    case CARD_1547:
        return 14;
    }
    return 0;
}

/* First zone 0-10 other than skipZone with a face-up card of number cardNo, or -1 (a disabled card counts). */
int FindFaceUpCardOnField(int player, u16 cardNo, int skipZone)
{
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_FIELD; i++) {
        struct DuelZoneView *zone = ZONE_ADDR(player, i);
        u16 id = CARD_ID(zone);

        if (id != 0 && i != skipZone && zone->isFaceUp && CARD_NUMBER_C(id) == cardNo)
            return i;
    }
    return -1;
}

/* Byte-identical copy of FindFaceUpCardOnField (its callers pass only player and cardNo). */
int FindFaceUpCardOnField2(int player, u16 cardNo, int skipZone)
{
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_FIELD; i++) {
        struct DuelZoneView *zone = ZONE_ADDR(player, i);
        u16 id = CARD_ID(zone);

        if (id != 0 && i != skipZone && zone->isFaceUp && CARD_NUMBER_C(id) == cardNo)
            return i;
    }
    return -1;
}

/* Number of active (face-up, not disabled) cards of number cardNo in zones 0-10 other than skipZone. */
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone)
{
    int count = 0;
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_FIELD; i++) {
        struct DuelZoneView *zone = ZONE_ADDR(player, i);
        u16 id = CARD_ID(zone);

        if (id != 0 && i != skipZone && zone->isFaceUp && !zone->isDisabled && CARD_NUMBER_C(id) == cardNo)
            count++;
    }
    return count;
}

/* Number of active (face-up, not disabled) cards of number cardNo on the player's field: is card cardNo in
 * effect for this player. */
int CountActiveCardsOnField(int player, u16 cardNo)
{
    return CountActiveCardsOnFieldExcept(player, cardNo, -1);
}

/* Number of cards of number cardNo in spell/trap zones 5-9 that are not disabled (face up or down). Unused. */
int CountEnabledSpellTrapCards(int player, u16 cardNo)
{
    int count = 0;
    int i = ZONE_SPELL_0;
    /* Separate locals for the player offset, the base, the player's zones and the mask give the ROM's
     * operand order (see the wiki page). */
    u32 offset = (player & 1) * DUEL_PLAYER_SIZE;
    u8 *zoneBase = (u8 *)gDuelZones;
    struct DuelZone *zones = (struct DuelZone *)(zoneBase + offset);
    u32 mask = CARD_ID_MASK;

    for (; i <= ZONE_SPELL_4; i++) {
        u32 stride = i * DUEL_ZONE_SIZE;
        struct DuelCard *entry = (struct DuelCard *)((u32)zones + stride);
        u16 id = entry->id;

        /* Byte +0x91 bit 3 is the zone's isDisabled bit, read through its own address computation;
         * 0x08622AB4 is gCardIdToNumber. */
        if (id && !(zoneBase[offset + i * DUEL_ZONE_SIZE + 0x91] & 8)
            && ((const u16 *)0x08622AB4)[id & mask] == cardNo)
            count++;
    }
    return count;
}

/* Number of face-up monsters whose effective type (GetZoneCardType) is type. */
int CountFaceUpMonstersOfType(int player, u16 type)
{
    int count = 0;
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
        if (ZONE_ADDR_PLAYER_FIRST(player, i)->isFaceUp && GetZoneCardType(player, i) == type)
            count++;
    }
    return count;
}

/* Number of face-up monsters whose effective attribute (GetZoneCardAttribute) is attr. Unused. */
int CountFaceUpMonstersOfAttribute(int player, u16 attr)
{
    int count = 0;
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
        if (GetZoneCardAttribute(player, i) == attr && ZONE_ADDR_PLAYER_FIRST(player, i)->isFaceUp)
            count++;
    }
    return count;
}

/* 1 if Toon World is face up in one of the player's spell/trap zones (the disabled bit is not checked). */
int HasFaceUpToonWorld(int player)
{
    int i;

    for (i = ZONE_SPELL_0; i <= ZONE_SPELL_4; i++) {
        struct DuelZoneView *zone = ZONE_ADDR(player, i);
        u32 id = CARD_ID(zone);     /* u32 here (u16 in the other queries): a matching choice */

        if (id != 0 && zone->isFaceUp && CARD_NUMBER_C(id) == CARD_TOON_WORLD)
            return TRUE;
    }
    return FALSE;
}

/* Number of face-up monsters of number cardNo (the disabled bit is not checked). */
int CountFaceUpMonstersByNumber(int player, u16 cardNo)
{
    int count = 0;
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
        struct DuelZoneView *zone = ZONE_ADDR(player, i);
        u16 id = CARD_ID(zone);

        if (id != 0 && zone->isFaceUp && CARD_NUMBER_C(id) == cardNo)
            count++;
    }
    return count;
}

/* First monster zone with a face-up card of number cardNo, or -1. */
int FindFaceUpMonsterByNumber(int player, u16 cardNo)
{
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
        struct DuelZoneView *zone = ZONE_ADDR(player, i);
        u16 id = CARD_ID(zone);

        if (id != 0 && zone->isFaceUp && CARD_NUMBER_C(id) == cardNo)
            return i;
    }
    return -1;
}

/* Number of monsters of number cardNo, face up or face down. */
int CountMonstersByNumber(int player, u16 cardNo)
{
    int count = 0;
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
        u16 id = CARD_ID(ZONE_ADDR_PLAYER_FIRST(player, i));

        if (id != 0 && CARD_NUMBER_C(id) == cardNo)
            count++;
    }
    return count;
}

/* Number of face-up monsters of number cardNo in the given position (defense 1, attack 0). */
int CountFaceUpMonstersByNumberInPosition(int player, u16 cardNo, u16 defense)
{
    int count = 0;
    int i;

    for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
        struct DuelZoneView *zone = ZONE_ADDR(player, i);
        u32 id = CARD_ID(zone);

        if (id != 0 && zone->isFaceUp && zone->isDefense == defense && CARD_NUMBER_C(id) == cardNo)
            count++;
    }
    return count;
}

/* Number of occupied monster zones. The count is a u16, returned zero-extended. */
int CountMonsters(int player)
{
    u16 count = 0;
    u16 i;

    for (i = ZONE_MONSTER_0; i < MONSTER_ZONE_COUNT; i++) {
        if (CARD_ID(ZONE_ADDR_PLAYER_FIRST(player, i)))
            count++;
    }
    return count;
}

/* Number of occupied monster zones, optionally only face-up ones and/or only those in attack position. */
int CountMonstersFiltered(int player, u16 faceUpOnly, u16 attackPosOnly)
{
    u16 count = 0;
    u16 i;

    for (i = ZONE_MONSTER_0; i < MONSTER_ZONE_COUNT; i++) {
        if (CARD_ID(ZONE_ADDR_PLAYER_FIRST(player, i))) {
            int faceOk = FALSE;
            int positionOk = FALSE;

            if (!faceUpOnly || ZONE_ADDR_PLAYER_FIRST(player, i)->isFaceUp)
                faceOk = TRUE;
            if (!attackPosOnly || !ZONE_ADDR_PLAYER_FIRST(player, i)->isDefense)
                positionOk = TRUE;
            if (faceOk && positionOk)
                count++;
        }
    }
    return count;
}

/* IsMonsterZoneFree's zone address (as ZONE_ADDR, player not masked). As an inline, the player & 1 of the
 * argument is evaluated first, as in the ROM. */
static inline struct DuelZoneView *ZoneAddr(int player, int zone)
{
    return (struct DuelZoneView *)((u8 *)gDuelZones + (zone * DUEL_ZONE_SIZE + player * DUEL_PLAYER_SIZE));
}

/* The same address with the base loaded into a local first. FAKEMATCH: the early base load lets loop.c hoist
 * the whole linkKinds address in its first pass, so strength reduction walks linkKinds with a pointer
 * started at ((player * 0xD64 + 0x4A) + zone * 0x94) + base, as in the ROM. */
static inline struct DuelZoneView *ZoneAddrBaseFirst(int player, int zone)
{
    u8 *base = (u8 *)gDuelZones;

    return (struct DuelZoneView *)(base + (zone * DUEL_ZONE_SIZE + player * DUEL_PLAYER_SIZE));
}

/*
 * 1 if a monster may be placed in the zone: it is empty, not held for a monster banished until the End
 * Phase (removedMask), and none of its ZONE_LINK_CONTINUOUS links comes from a zone holding card number
 * 1320 (not an EDS card).
 */
u32 IsMonsterZoneFree(int player, int zone)
{
    struct DuelZoneView *z = ZoneAddr(player & 1, zone);
    int i;

    if (CARD_ID(z))
        return FALSE;
    if ((PLAYER_VIA_ZONES(player)->removedMask >> zone) & 1)
        return FALSE;
    /* FAKEMATCH: the loop test recomputes the zone address instead of using z; its movables match the
     * linkKinds ones, so loop.c hoists numLinks out of the loop. */
    for (i = 0; i < ZoneAddr(player & 1, zone)->numLinks; i++) {
        u8 linkPlayer = DUEL_LOC_PLAYER(z->links[i]);
        u16 linkZone = DUEL_LOC_ZONE(z->links[i]);

        if ((u8)ZoneAddrBaseFirst(player & 1, zone)->linkKinds[i] == ZONE_LINK_CONTINUOUS) {
            u16 id = CARD_ID(ZoneAddrBaseFirst(linkPlayer & 1, linkZone));

            if (id != 0 && CARD_NUMBER_C(id) == CARD_1320)
                return FALSE;
        }
    }
    return TRUE;
}
