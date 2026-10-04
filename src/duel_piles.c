#include "global.h"
#include "card_data.h"              /* gCardIdToNumber, CARD_ID_MASK, CARD_STATS_* extractors, token numbers */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardKind, SpellSubtype */
#include "constants/duel.h"         /* enum ZoneLinkKind, DUEL_LOC */
#include "duel.h"                   /* gDuel, gDuelPlayers, gDuelZones, struct DuelCard / DuelZone, CopyDuelCard, AddCardToFusionDeck */
#include "duel_actions.h"           /* DestroyFieldCard */
#include "effect_handlers.h"        /* EffectEquipTargetCheck */

/*
 * Duel piles and zone links (wiki/functions/duel-piles-c.md).
 *
 * The first half edits and searches the per-player piles of struct DuelPlayer: the graveyard, the banished
 * pile (with its parallel banishedInfo array) and the hand. AddCardToHand drops tokens and sends Fusion
 * monsters to the fusion deck; IsHandRevealed decides whether a player's hand is shown.
 *
 * The second half reads a zone's links: the list of cards that affect the card in that zone (struct
 * DuelZone links / linkKinds). A link's kind (low byte of linkKinds[i], enum ZoneLinkKind) says what
 * links[i] holds: the DUEL_LOC (zone << 8 | player) of an equip or of a card with a continuous effect,
 * or a card ID whose effect applies (ZONE_LINK_CARD_EFFECT).
 */

#include "chain.h"                  /* struct ChainEntry (EffectEquipTargetCheck's card argument) */

#define COPY_CARD(dst, src) CopyDuelCard((dst), (src))

/* The card word as one u32. Matching: the ROM always loads the whole word (ldr) for these compares and ID
 * extractions; a bitfield read of .id would load only the halfword holding it. */
#define CARD_WORD(card) (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word) (((word) << 20) >> 20)

/*
 * &gDuelZones[player].zones[zone] by byte arithmetic. Matching: this operand order emits the zone term
 * first, as the ROM's link walkers do; array indexing emits the player term first. Callers mask the
 * player with & 1.
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((u8 *)gDuelZones + (zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer)))

/*
 * Card tables read through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber
 * (0x08622AB4). Matching: old_agbcc then reloads the table address at every use, inside loops too,
 * instead of hoisting it as it does for the symbols. Same tables, same bytes.
 */
#define CARD_STATS(id)   (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id)  (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id)    CARD_STATS_TYPE(CARD_STATS(id))      /* enum CardType */
#define CARD_KIND(id)    CARD_STATS_KIND(CARD_STATS(id))      /* monster kind: enum CardKind 0-3 */
/* Card numbers 1920-1999 are monster tokens. */
#define IS_TOKEN(id) \
    ((u16)(CARD_NUMBER(id) - CARD_NUMBER_TOKEN_FIRST) < CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST)

/* The duel state seen from gDuelPlayers. Matching: IsHandRevealed reaches gDuel.turnPlayer as
 * gDuelPlayers + 0x1B0E, from the base register it already holds, not through a gDuel literal. */
#define DUEL_VIA_PLAYERS (*(struct DuelState *)((u8 *)gDuelPlayers - OFFSET_OF(struct DuelState, players)))

/*
 * enum CardKind of a card (the same inline as in card_detail): the Egyptian Gods first (Obelisk counts as
 * Ritual, Slifer and Ra as Effect), then Magic, Trap and Ticket cards, else the monster kind from the stats.
 */
static inline int GetCardKind(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((u8)CARD_TYPE(id)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    default:
        return CARD_KIND(id);
    }
}

/* Remove hand[index], shifting the later cards down (inlined into RemoveCardFromHand and CompactHand). */
static inline void RemoveHandCard(int player, int index)
{
    int i;

    gDuelPlayers[player & 1].handCount--;
    for (i = index; i < gDuelPlayers[player & 1].handCount; i++)
        COPY_CARD(&gDuelPlayers[player & 1].hand[i], &gDuelPlayers[player & 1].hand[i + 1]);
}

/* Remove graveyard[index] and close the gap. Returns 1, or 0 if index >= graveCount. */
int RemoveGraveyardCardAt(int player, int index)
{
    int i;

    if (index < gDuelPlayers[player & 1].graveCount) {
        gDuelPlayers[player & 1].graveCount--;
        for (i = index; i < gDuelPlayers[player & 1].graveCount; i++)
            COPY_CARD(&gDuelPlayers[player & 1].graveyard[i], &gDuelPlayers[player & 1].graveyard[i + 1]);
        return 1;
    }
    return 0;
}

/* Copy graveyard[index] to *out, then remove it and close the gap. Returns 1, or 0 if index >= graveCount. */
int TakeGraveyardCardAt(int player, int index, struct DuelCard *out)
{
    int i;

    if (index < gDuelPlayers[player & 1].graveCount) {
        COPY_CARD(out, &gDuelPlayers[player & 1].graveyard[index]);
        gDuelPlayers[player & 1].graveCount--;
        for (i = index; i < gDuelPlayers[player & 1].graveCount; i++)
            COPY_CARD(&gDuelPlayers[player & 1].graveyard[i], &gDuelPlayers[player & 1].graveyard[i + 1]);
        return 1;
    }
    return 0;
}

/* Remove the first graveyard entry whose whole card word equals *card. Returns 1 if found, else 0. */
u16 RemoveCardFromGraveyard(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        struct DuelCard *entry = &gDuelPlayers[player & 1].graveyard[i];

        if (CARD_WORD(*card) == CARD_WORD(*entry))
            return RemoveGraveyardCardAt(player, i);
    }
    return 0;
}

/* Remove the first graveyard entry with card ID cardId. Returns 1 if found, else 0. */
u16 RemoveGraveyardCardById(int player, u16 cardId)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        if (CARD_ID(CARD_WORD(gDuelPlayers[player & 1].graveyard[i])) == cardId)
            return RemoveGraveyardCardAt(player, i);
    }
    return 0;
}

/* Card ID of graveyard[index]. */
static inline u16 GetGraveyardCardId(int player, int index)
{
    struct DuelCard *card = &gDuelPlayers[player & 1].graveyard[index];

    return card->id;
}

/* Copy the first graveyard entry with card ID cardId to *out; the pile is unchanged. Returns 1 if found. */
int GetGraveyardCardById(int player, u16 cardId, struct DuelCard *out)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        struct DuelCard *entry = &gDuelPlayers[player & 1].graveyard[i];

        if (GetGraveyardCardId(player, i) == cardId) {
            COPY_CARD(out, entry);
            return 1;
        }
    }
    return 0;
}

/* 1 if some graveyard entry's whole card word equals *card, else 0. */
int IsCardInGraveyard(int player, struct DuelCard *card)
{
    struct DuelCard *wanted = card;
    int i = 0;
    u8 *players = (u8 *)gDuelPlayers;
    u32 playerOffset = (player & 1) * 0xD64;
    /* FAKEMATCH: graveCount is loaded into r2, where the ROM keeps it until the loop bound copy. */
    register int count asm("r2") = *((u8 *)(playerOffset + (u32)players) + OFFSET_OF(struct DuelPlayer, graveCount));

    if (i < count) {
        u8 *graveyards = players + OFFSET_OF(struct DuelPlayer, graveyard);
        int end = count;
        u32 word;
        struct DuelCard *entry;

        word = CARD_WORD(*wanted);
        entry = (struct DuelCard *)(playerOffset + (u32)graveyards);
        do {
            if (word == CARD_WORD(*entry))
                return 1;
            entry++;
            i++;
        } while (i < end);
    }
    return 0;
}

/* Number of graveyard cards whose card number is cardNo. */
int CountGraveyardCardsByNumber(int player, u16 cardNo)
{
    int i;
    int count = 0;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        u32 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].graveyard[i])) & CARD_ID_MASK;

        if (gCardIdToNumber[id] == cardNo)
            count++;
    }
    return count;
}

/* Remove banished[index] and its banishedInfo entry, closing both gaps. Returns 1, or 0 if out of range. */
int RemoveBanishedCardAt(int player, int index)
{
    if (index < gDuelPlayers[player & 1].banishedCount) {
        int i = index;

        gDuelPlayers[player & 1].banishedCount--;
        i++; /* FAKEMATCH: no-op pair that moves the copy of index one instruction later, as in the ROM */
        i--;
        for (; i < gDuelPlayers[player & 1].banishedCount; i++) {
            /* Matching: the source is written banished + i + 1, not &banished[i + 1]. */
            COPY_CARD(&gDuelPlayers[player & 1].banished[i], gDuelPlayers[player & 1].banished + i + 1);
            gDuelPlayers[player & 1].banishedInfo[i] = gDuelPlayers[player & 1].banishedInfo[i + 1];
        }
        return 1;
    }
    return 0;
}

/* Remove the first banished entry whose whole card word equals *card. Returns 1 if found, else 0. */
u16 RemoveCardFromBanished(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].banishedCount; i++) {
        struct DuelCard *entry = &gDuelPlayers[player & 1].banished[i];

        if (CARD_WORD(*card) == CARD_WORD(*entry))
            return RemoveBanishedCardAt(player, i);
    }
    return 0;
}

/* enum CardType of a card. Matching: the u16 parameter narrows the ID as in the ROM. */
static inline int GetCardType(u16 id)
{
    return CARD_TYPE(id);
}

/* Number of monsters (types 1-20) in the graveyard. */
int CountGraveyardMonsters(int player)
{
    int i;
    int count = 0;

    for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++) {
        if ((u32)GetCardType(CARD_ID(CARD_WORD(gDuelPlayers[player & 1].graveyard[i]))) <= CARD_TYPE_REPTILE)
            count++;
    }
    return count;
}

/* Number of monsters (types 1-20) in the hand. */
int CountHandMonsters(int player)
{
    int i;
    int count = 0;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        if ((u32)GetCardType(CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]))) <= CARD_TYPE_REPTILE)
            count++;
    }
    return count;
}

/*
 * Append *card to the player's hand. Empty cards and tokens are dropped; a Fusion monster goes to its
 * owner's fusion deck instead.
 */
void AddCardToHand(int player, struct DuelCard *card)
{
    int count = gDuelPlayers[player & 1].handCount;
    struct DuelCard *slot = &gDuelPlayers[player & 1].hand[count];

    /* FAKEMATCH: emits nothing; keeps card in r4 and the hand-offset scratch in r3, as in the ROM. */
    asm volatile ("" : : "r"(card) : "r3");

    if (card->id == 0 || IS_TOKEN(card->id))
        return;
    if (GetCardKind(card->id) == CARD_KIND_FUSION) {
        AddCardToFusionDeck(card->owner, card);
    } else {
        COPY_CARD(slot, card);
        gDuelPlayers[player & 1].handCount++;
    }
}

/* Clear hand[index].id if index < handCount, leaving a hole for CompactHand. No callers in the ROM. */
void ClearHandCardAt(int player, int index)
{
    int count = gDuelPlayers[player & 1].handCount;
    struct DuelCard *card = &gDuelPlayers[player & 1].hand[index];

    if (index < count)
        card->id = 0;
}

/* hand[index] as a whole card word. */
static inline u32 GetHandCardWord(int player, int index)
{
    struct DuelCard *card = &gDuelPlayers[player & 1].hand[index];

    return CARD_WORD(*card);
}

/* Remove the first hand entry whose whole card word equals *card. Returns 1 if found, else 0. */
int RemoveCardFromHand(int player, struct DuelCard *card)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        if (GetHandCardWord(player, i) == CARD_WORD(*card)) {
            RemoveHandCard(player, i);
            return 1;
        }
    }
    return 0;
}

/* Delete the empty (card ID 0) hand entries, keeping the order of the others. */
void CompactHand(int player)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount;) {
        if (CARD_ID(GetHandCardWord(player, i)) == 0)
            RemoveHandCard(player, i);
        else
            i++;
    }
}

/* Index of the first Trap card in the hand, or -1. */
int FindTrapInHand(int player)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));

        if (id != 0 && CARD_TYPE(id) == CARD_TYPE_TRAP)
            return i;
    }
    return -1;
}

/* Index of the first Magic card in the hand, or -1. */
int FindMagicInHand(int player)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));

        if (id != 0 && CARD_TYPE(id) == CARD_TYPE_MAGIC)
            return i;
    }
    return -1;
}

/* CARD_STATS with the ID mask passed in: used only by FindNonFieldMagicInHand (see its FAKEMATCH). */
#define CARD_STATS_M(id, mask)   (((const u32 *)0x08621DE0)[(id) & (mask)])
#define CARD_TYPE_M(id, mask)    CARD_STATS_TYPE(CARD_STATS_M(id, mask))
#define CARD_SUBTYPE_M(id, mask) CARD_STATS_SUBTYPE(CARD_STATS_M(id, mask))

/* Index of the first Magic card in the hand that is not a Field Magic, or -1. */
int FindNonFieldMagicInHand(int player)
{
    int i;
    u32 mask;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));

        mask = CARD_ID_MASK; /* FAKEMATCH: a variable mask makes the 0x7FF load land one instruction later */
        if (id != 0 && CARD_TYPE_M(id, mask) == CARD_TYPE_MAGIC && CARD_SUBTYPE_M(id, mask) != SPELL_FIELD)
            return i;
    }
    return -1;
}

/* Number of hand cards whose card number is cardNo. */
int CountHandCardsByNumber(int player, u16 cardNo)
{
    int i;
    int count = 0;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u32 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i])) & CARD_ID_MASK;

        if (gCardIdToNumber[id] == cardNo)
            count++;
    }
    return count;
}

/* Index of the first hand card whose card number is cardNo, or -1. */
int FindHandCardByNumber(int player, u16 cardNo)
{
    int i;

    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        u32 id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i])) & CARD_ID_MASK;

        if (gCardIdToNumber[id] == cardNo)
            return i;
    }
    return -1;
}

/*
 * 1 if the player's hand must be shown to the opponent, else 0:
 *  - the player's handRevealed flag is set;
 *  - The Eye of Truth is active on the opponent's field ("your opponent must show his/her hand");
 *  - Ceremonial Bell is active on either field (both players show their hands);
 *  - Respect Play is active on either field and it is this player's turn ("during their respective turns").
 */
int IsHandRevealed(int player)
{
    int opponent;

    if (gDuelPlayers[player & 1].handRevealed)
        return 1;
    opponent = 1 - player;
    if (CountActiveCardsOnField2(opponent, CARD_THE_EYE_OF_TRUTH) > 0
        || CountActiveCardsOnField2(player, CARD_CEREMONIAL_BELL) > 0
        || CountActiveCardsOnField2(opponent, CARD_CEREMONIAL_BELL) > 0)
        return 1;
    if (player == DUEL_VIA_PLAYERS.turnPlayer) {
        if (CountActiveCardsOnField2(player, CARD_RESPECT_PLAY) > 0
            || CountActiveCardsOnField2(opponent, CARD_RESPECT_PLAY) > 0)
            return 1;
    }
    return 0;
}

/*
 * Aqua Chorus: "If there are Monster Cards of the same name on the field, the ATK and DEF of those cards are
 * increased by 500." Returns the number of active Aqua Chorus on both fields if another face-up monster
 * shares the name of the monster in (player, zone), else 0. GetZoneCardStats adds 500 ATK/DEF per count.
 */
int CountAquaChorusBoosts(int player, int zone)
{
    int count = CountActiveCardsOnField(0, CARD_AQUA_CHORUS) + CountActiveCardsOnField(1, CARD_AQUA_CHORUS);

    if (count > 0 && CountOtherFaceUpSameNameMonsters(player, zone) > 0)
        return count;
    return 0;
}

/*
 * DUEL_LOC of the monster absorbed by the monster in (player, zone): links[i] of its first
 * ZONE_LINK_ABSORBED link (Relinquished treats the absorbed monster as an equip), or 0xFFFF if none.
 */
u16 FindAbsorbedMonsterLink(int player, int zone)
{
    int i;
    struct DuelZone *z;

    i = 0;
    player &= 1;
    /* Matching: the two offsets are summed before the base is added (ZONE_AT adds them one by one). */
    z = (struct DuelZone *)((u8 *)gDuelZones + (zone * sizeof(struct DuelZone) + player * sizeof(struct DuelPlayer)));
    for (; i < z->numLinks; i++) {
        u16 link = z->links[i];

        if ((u8)z->linkKinds[i] == ZONE_LINK_ABSORBED)
            return link;
    }
    return 0xFFFF;
}

/*
 * Run by DNA Surgery for each monster zone after the declared type changes: every equip on the monster in
 * (player, zone) that only suits some types or attributes (or one monster) is checked again with
 * EffectEquipTargetCheck, and destroyed if the monster no longer qualifies.
 */
void DestroyInvalidEquips(int player, int zone)
{
    struct ChainEntry equip;
    int i;

    for (i = 0; i < ZONE_AT(player & 1, zone)->numLinks; i++) {
        u16 link = ZONE_AT(player & 1, zone)->links[i];

        if ((u8)ZONE_AT(player & 1, zone)->linkKinds[i] == ZONE_LINK_EQUIP) {
            int equipPlayer = (u8)link;
            u16 equipZone = link >> 8;
            u16 id = CARD_ID(CARD_WORD(ZONE_AT(equipPlayer & 1, equipZone)->card));

            switch (CARD_NUMBER(id)) {
            case CARD_LEGENDARY_SWORD:
            case CARD_SWORD_OF_DARK_DESTRUCTION:
            case CARD_DARK_ENERGY:
            case CARD_LASER_CANNON_ARMOR:
            case CARD_INSECT_ARMOR_WITH_LASER_CANNON:
            case CARD_ELFS_LIGHT:
            case CARD_BEAST_FANGS:
            case CARD_STEEL_SHELL:
            case CARD_VILE_GERMS:
            case CARD_SILVER_BOW_AND_ARROW:
            case CARD_DRAGON_TREASURE:
            case CARD_ELECTRO_WHIP:
            case CARD_CYBER_SHIELD:
            case CARD_MYSTICAL_MOON:
            case CARD_VIOLET_CRYSTAL:
            case CARD_BOOK_OF_SECRET_ARTS:
            case CARD_INVIGORATION:
            case CARD_MACHINE_CONVERSION_FACTORY:
            case CARD_RAISE_BODY_HEAT:
            case CARD_FOLLOW_WIND:
            case CARD_POWER_OF_KAISHIN:
            case CARD_MAGICAL_LABYRINTH:
            case CARD_SALAMANDRA:
            case CARD_BRIGHT_CASTLE:
            case CARD_7_COMPLETED:
            case CARD_BURNING_SPEAR:
            case CARD_GUST_FAN:
            case CARD_GERM_INFECTION:
            case CARD_PARALYZING_POTION:
            case CARD_SWORD_OF_DRAGONS_SOUL:
            case CARD_1314:
            case CARD_1422:
            case CARD_1540:
            case CARD_1550:
                equip.player = equipPlayer;
                equip.zone = equipZone;
                equip.card = id;
                if (EffectEquipTargetCheck(&equip, (u8)player | ((u8)zone << 8)) == 0)
                    DestroyFieldCard(equipPlayer, equipZone, 1);
                break;
            }
        }
    }
}

/* Magic/Trap subtype (enum SpellSubtype) of a Magic or Trap card, 0 for any other card. */
static inline int GetSpellSubtype(u16 id)
{
    u32 stats = CARD_STATS(id);

    switch ((int)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(stats);
    default:
        return 0;
    }
}

/*
 * Number of equips on the monster in (player, zone): every ZONE_LINK_ABSORBED link, plus each
 * ZONE_LINK_EQUIP link whose source zone holds a card that passes either filter: (!requireMagic or a Magic
 * card) or (!requireEquipSubtype or an Equip spell). The filters are ORed, so one alone filters nothing; the
 * only caller (Eternal Rest) passes (0, 0).
 */
int CountZoneEquips(int player, int zone, u16 requireMagic, u16 requireEquipSubtype)
{
    int count = 0;
    int i;

    for (i = 0; i < ZONE_AT(player & 1, zone)->numLinks; i++) {
        u16 link = ZONE_AT(player & 1, zone)->links[i];

        switch ((u8)ZONE_AT(player & 1, zone)->linkKinds[i]) {
        case ZONE_LINK_EQUIP: {
            int counts = 0;
            int equipPlayer = (u8)link;
            u16 id = CARD_ID(CARD_WORD(ZONE_AT(equipPlayer & 1, link >> 8)->card));

            if (id == 0)
                break;
            if (!requireMagic || CARD_TYPE(id) == CARD_TYPE_MAGIC)
                counts = 1;
            if (!requireEquipSubtype || GetSpellSubtype(id) == SPELL_EQUIP)
                counts = 1;
            if (counts)
                count++;
            break;
        }
        case ZONE_LINK_ABSORBED:
            count++;
            break;
        }
    }
    return count;
}

/*
 * Number of links on the card in (player, zone) that come from card number cardNo: ZONE_LINK_EQUIP,
 * ZONE_LINK_CONTINUOUS and ZONE_LINK_EQUIP_ATK_200 links name the source zone, whose card is compared;
 * ZONE_LINK_CARD_EFFECT links hold the card ID itself. For Spellbinding Circle, Ring of Magnetism and card
 * 1244 a link whose source zone is disabled does not count; for other cards it does.
 * Quirk (ROM): the disabled test runs before the kind switch, so for a card-ID link it reads the zone that
 * the ID's bits happen to name.
 */
int CountZoneLinksFromCard(int player, int zone, u16 cardNo)
{
    int i;
    int count = 0;

    for (i = 0; i < ZONE_AT(player & 1, zone)->numLinks; i++) {
        u16 link = ZONE_AT(player & 1, zone)->links[i];
        u8 kind = ZONE_AT(player & 1, zone)->linkKinds[i];
        /* Matching: the source player is a u8 local (not int), which keeps loop.c from strength-reducing
         * linkKinds[i] in its first pass; the source zone is one local shared by both lookups. */
        u8 sourcePlayer = link;
        int sourceZone = link >> 8;
        u16 id = CARD_ID(CARD_WORD(ZONE_AT(sourcePlayer & 1, sourceZone)->card));
        int valid = 1;

        switch (cardNo) {
        case CARD_SPELLBINDING_CIRCLE:
        case CARD_RING_OF_MAGNETISM:
        case CARD_1244:
            if (ZONE_AT(sourcePlayer & 1, sourceZone)->isDisabled)
                valid = 0;
            break;
        }
        if (valid) {
            switch (kind) {
            case ZONE_LINK_EQUIP:
            case ZONE_LINK_CONTINUOUS:
            case ZONE_LINK_EQUIP_ATK_200:
                if (CARD_NUMBER(id) == cardNo)
                    count++;
                break;
            case ZONE_LINK_CARD_EFFECT:
                if (CARD_NUMBER(link) == cardNo)
                    count++;
                break;
            }
        }
    }
    return count;
}

/*
 * CountZoneLinksFromCard, but every link whose source zone is disabled is skipped, whatever the card.
 * Same quirk: card-ID links are also tested against the zone their bits name.
 */
int CountActiveZoneLinksFromCard(int player, int zone, u16 cardNo)
{
    int count = 0;
    int i;

    for (i = 0; i < ZONE_AT(player & 1, zone)->numLinks; i++) {
        u16 link = ZONE_AT(player & 1, zone)->links[i];
        u8 kind = ZONE_AT(player & 1, zone)->linkKinds[i];
        int sourcePlayer = (u8)link;
        u16 id = CARD_ID(CARD_WORD(ZONE_AT(sourcePlayer & 1, link >> 8)->card));
        int valid = 1;

        if (ZONE_AT(sourcePlayer & 1, link >> 8)->isDisabled)
            valid = 0;
        if (valid) {
            switch (kind) {
            case ZONE_LINK_EQUIP:
            case ZONE_LINK_CONTINUOUS:
            case ZONE_LINK_EQUIP_ATK_200:
                if (gCardIdToNumber[id & CARD_ID_MASK] == cardNo)
                    count++;
                break;
            case ZONE_LINK_CARD_EFFECT:
                if (gCardIdToNumber[link & CARD_ID_MASK] == cardNo)
                    count++;
                break;
            }
        }
    }
    return count;
}

/* 1 if the card in (player, zone) has a ZONE_LINK_CARD_EFFECT link from card number cardNo, else 0. */
int HasZoneCardEffectLink(int player, int zone, u16 cardNo)
{
    int i;

    for (i = 0; i < ZONE_AT(player & 1, zone)->numLinks; i++) {
        /* Matching: the body reaches the zone through the player row, so its address ((base + player) +
         * zone) is not shared with the loop bound's (zone first). */
        struct DuelZonesPlayer *row = &gDuelZones[player & 1];
        struct DuelZone *z = &row->zones[zone];
        u16 link = z->links[i];

        if (z->linkKinds[i] == ZONE_LINK_CARD_EFFECT && CARD_NUMBER(link) == cardNo)
            return 1;
    }
    return 0;
}

/*
 * Index of the first link on the card in (player, zone) from card number cardNo, or -1: ZONE_LINK_EQUIP and
 * ZONE_LINK_CONTINUOUS links compare the source zone's card, ZONE_LINK_CARD_EFFECT links their card ID.
 * No disabled test, and ZONE_LINK_EQUIP_ATK_200 links are not considered.
 */
int FindZoneLinkFromCard(int player, int zone, u16 cardNo)
{
    int i;

    for (i = 0; i < ZONE_AT(player & 1, zone)->numLinks; i++) {
        u16 link = ZONE_AT(player & 1, zone)->links[i];
        u8 kind = ZONE_AT(player & 1, zone)->linkKinds[i];
        int sourcePlayer = (u8)link;
        u16 id = CARD_ID(CARD_WORD(ZONE_AT(sourcePlayer & 1, link >> 8)->card));

        switch (kind) {
        case ZONE_LINK_EQUIP:
        case ZONE_LINK_CONTINUOUS:
            if (CARD_NUMBER(id) == cardNo)
                return i;
            break;
        case ZONE_LINK_CARD_EFFECT:
            if (CARD_NUMBER(link) == cardNo)
                return i;
            break;
        }
    }
    return -1;
}
