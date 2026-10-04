#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_TOKEN_FIRST / _END, CARD_STATS_TYPE / _SUBTYPE */
#include "constants/cards.h"        /* CARD_1418 */
#include "constants/card_stats.h"   /* enum CardType, enum CardAttribute, SPELL_FIELD */
#include "constants/duel.h"         /* zone indexes, ZONE_LINK_*, BANISH_*, DUEL_LOC_ZONE */
#include "duel.h"                   /* struct DuelCard / DuelZone / DuelPlayer / DuelState, gDuel, gDuelPlayers, gDuelZones */

/*
 * Duel field helpers (wiki/functions/duel-zones-c.md):
 *  - queries over a player's 11 field zones (enum DuelZoneIndex: 0-4 monsters, 5-9 spells/traps, 10 the
 *    Field Magic): free zones, tributes, face-up / face-down counts, Toon and Prohibition checks;
 *  - the per-zone link lists: equips and other cards that affect a monster, stored on the monster's zone
 *    as a DUEL_LOC (zone << 8 | player) with a kind (enum ZoneLinkKind);
 *  - moving cards out of a zone into the graveyard, the banished pile, the hand or the deck.
 */

/* Every access masks the player with & 1, as the ROM does. */
#define PLAYER(p)       (gDuelPlayers[(p) & 1])
#define ZONE(p, z)      (gDuelPlayers[(p) & 1].zones[z])
/*
 * Card ID of the zone (p, z). Matching: DUEL_CARD_ID reads the card word through a pointer, as the ROM does
 * (ldr and shifts); a direct member read (zone.card.id) loads only the halfword that holds the field.
 */
#define ZONE_CARD_ID(p, z) DUEL_CARD_ID(&ZONE(p, z))
/* The zone reached through its card word's address. Matching: field reads then share one address
 * computation with the ZONE_CARD_ID read of the same zone (CSE). */
#define ZONEP(p, z)     ((struct DuelZone *)&ZONE(p, z).card)

/*
 * &gDuelZones[p].zones[z] as explicit address arithmetic (0xD64 = sizeof(struct DuelPlayer), 0x94 =
 * sizeof(struct DuelZone)). Matching: with this operand order old_agbcc emits zone * 0x94 first, as the
 * ROM's link walkers do; array indexing emits the player term first. Callers mask the player.
 */
#define ZONE_AT(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gDuelZones))

/*
 * Card tables read through integer-constant addresses. Matching: with the symbols (card_data.h) old_agbcc
 * loads the table address before the index math, the ROM after it. Same tables, same bytes.
 */
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])    /* gCardIdToNumber[id] */
#define CARD_STATS_WORD(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])    /* gCardStats[id] */
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS_WORD(id))                 /* enum CardType */

/* Card numbers 1920-1999 are monster tokens. */
#define IS_TOKEN(id) \
    ((u16)(CARD_NUMBER(id) - CARD_NUMBER_TOKEN_FIRST) < CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST)

/* Number of free monster zones (IsMonsterZoneFree) of the player. */
int CountFreeMonsterZones(int player)
{
    int count = 0;
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        /* Matching: the result is narrowed to u16 (lsl #16) before the test, as by a u16 prototype. */
        if ((u16)IsMonsterZoneFree(player, zone))
            count++;
    }
    return count;
}

/* First free monster zone (0-4) of the player, or -1. */
int FindFreeMonsterZone(int player)
{
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        /* Matching: the result is narrowed to u16 (lsl #16) before the test, as by a u16 prototype. */
        if ((u16)IsMonsterZoneFree(player, zone))
            return zone;
    }
    return -1;
}

/*
 * TRUE if the card in (player, zone) may be tributed: a monster (type 1-20) that is not a token, while
 * card number 1418 is active (face up, not negated) on neither field. 1418 is not an EDS card, but the
 * engine still checks it: it forbids all tributes.
 */
u16 IsTributableMonster(int player, int zone)
{
    u32 cardId = ZONE_CARD_ID(player, zone);

    if (cardId != 0 && !IS_TOKEN(cardId) && CARD_TYPE(cardId) <= CARD_TYPE_REPTILE
        && CountActiveCardsOnField(0, CARD_1418) <= 0 && CountActiveCardsOnField(1, CARD_1418) <= 0)
        return TRUE;
    return FALSE;
}

/* Number of the player's tributable monsters (IsTributableMonster), not counting excludeZone (-1 = none). */
int CountTributableMonsters(int player, int excludeZone)
{
    int count;
    int zone;

    /* card 1418 active on either field: no tributes at all */
    if (CountActiveCardsOnField(0, CARD_1418) > 0 || CountActiveCardsOnField(1, CARD_1418) > 0)
        return 0;
    count = 0;
    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        struct DuelZone *z = &ZONE(player, zone);

        if (DUEL_CARD_ID(z) && zone != excludeZone && IsTributableMonster(player, zone))
            count++;
    }
    return count;
}

/*
 * Number of occupied spell/trap zones 5-9 (and the Field Magic zone if includeField). faceUp counts the
 * face-up cards, faceDown the face-down ones; both set or both clear count every card.
 * Returns the u16 count zero-extended to a word, as the callers consume it.
 */
int CountSpellTrapsFiltered(int player, u16 faceUp, u16 faceDown, u16 includeField)
{
    u16 count = 0;
    int end = ZONE_FIELD;           /* int, not u16: the ROM's loop compare is signed */
    u16 zone;

    if (includeField)
        end = ZONE_FIELD + 1;
    for (zone = ZONE_SPELL_0; zone < end; zone++) {
        if (ZONE_CARD_ID(player, zone)) {
            int counted = FALSE;

            if (!faceUp && !faceDown)
                counted = TRUE;
            if (faceUp && ZONEP(player, zone)->isFaceUp)
                counted = TRUE;
            if (faceDown && !ZONEP(player, zone)->isFaceUp)
                counted = TRUE;
            if (counted)
                count++;
        }
    }
    return (u16)count;
}

/* TRUE if the spell/trap zone is empty and its bit in the player's lockedZones is clear. */
u16 IsSpellTrapZoneFree(int player, int zone)
{
    if (ZONE_CARD_ID(player, zone) == 0 && !((PLAYER(player).lockedZones >> zone) & 1))
        return TRUE;
    return FALSE;
}

/* First free spell/trap zone (5-9, IsSpellTrapZoneFree) of the player, or -1. */
int FindFreeSpellTrapZone(int player)
{
    int zone;

    for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
        if (IsSpellTrapZoneFree(player, zone))
            return zone;
    }
    return -1;
}

/*
 * TRUE if the player has room for the spell/trap card: always for a Field Magic (it goes to zone 10,
 * replacing any card there), else if a spell/trap zone 5-9 is empty (lockedZones is not consulted).
 */
int CanPlaceSpellTrapCard(int player, u16 cardId)
{
    u32 stats = CARD_STATS_WORD(cardId);
    int type = CARD_STATS_TYPE(stats);
    int subtype;
    int zone;

    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        subtype = CARD_STATS_SUBTYPE(stats);
        break;
    default:
        subtype = 0;                /* monsters have no spell subtype */
        break;
    }
    if (subtype == SPELL_FIELD)
        return TRUE;
    for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
        if (ZONE_CARD_ID(player, zone) == 0)
            return TRUE;
    }
    return FALSE;
}

/*
 * Banish the card in (player, zone) if banish is set, else send it to the graveyard; then clear the zone.
 * Unlike SendZoneCardToGraveyard it does not remove the links that point at the zone.
 */
void SendZoneCardToGraveyardOrBanished(int player, int zone, u16 banish)
{
    struct DuelCard *card = &ZONE(player, zone).card;

    if (banish)
        AddCardToBanished(card);
    else
        AddCardToGraveyard(card);
    ClearZone(player, zone);
}

/*
 * Remove the links that point at (player, zone) from both players' monster zones: links of kind
 * ZONE_LINK_EQUIP, CONTINUOUS, ABSORBED, 7 or EQUIP_ATK_200 whose target is DUEL_LOC(player, zone).
 * Original quirks (kept, they are the ROM's behaviour): a link of any other kind also skips the link after
 * it (i is incremented twice), and after a removal the link that moved into slot i is not checked.
 */
void RemoveLinksToZone(int player, int zone)
{
    int monPlayer, monZone, i;

    for (monPlayer = 0; monPlayer <= 1; monPlayer++) {
        for (monZone = ZONE_MONSTER_0; monZone <= ZONE_MONSTER_4; monZone++) {
            for (i = 0; i < ZONE_AT(monPlayer & 1, monZone)->numLinks; i++) {
                u16 target = ZONE_AT(monPlayer & 1, monZone)->links[i];
                u8 kind = ZONE_AT(monPlayer & 1, monZone)->linkKinds[i];

                switch (kind) {
                case ZONE_LINK_EQUIP:
                case ZONE_LINK_CONTINUOUS:
                case ZONE_LINK_ABSORBED:
                case 7:
                case ZONE_LINK_EQUIP_ATK_200:
                    /* DUEL_LOC(player, zone). Matching: written player first, with the u16 cast, for
                     * the ROM's lsl 24 / lsr 8 / orr / lsr 16 (DUEL_LOC's order differs). */
                    if (target == (u16)((u8)player | ((u8)zone << 8)))
                        RemoveZoneLinkAt(monPlayer, monZone, i);
                    break;
                default:
                    i++;
                    break;
                }
            }
        }
    }
}

/* Send the card in (player, zone) to the graveyard, remove the links to it and clear the zone. */
void SendZoneCardToGraveyard(int player, int zone)
{
    AddCardToGraveyard(&ZONE(player, zone).card);
    RemoveLinksToZone(player, zone);
    ClearZone(player, zone);
}

/* Banish the card in (player, zone) and clear the zone (the links to it stay). */
void BanishZoneCard(int player, int zone)
{
    AddCardToBanished(&ZONE(player, zone).card);
    ClearZone(player, zone);
}

/*
 * Return the card in (player, zone) to its owner's hand (a Fusion monster to the owner's fusion deck),
 * remove the links to it and clear the zone.
 */
void ReturnZoneCardToHand(int player, int zone)
{
    struct DuelCard *card = &ZONE(player, zone).card;

    if (IsFusionMonster(card->id))
        AddCardToFusionDeck(card->owner, card);
    else
        AddCardToHand(card->owner, card);
    RemoveLinksToZone(player, zone);
    ClearZone(player, zone);
}

/*
 * Return the card in (player, zone) to the top of its owner's deck (a Fusion monster to the owner's fusion
 * deck), remove the links to it and clear the zone.
 */
void ReturnZoneCardToDeck(int player, int zone)
{
    struct DuelCard *card = &ZONE(player, zone).card;

    if (IsFusionMonster(card->id))
        AddCardToFusionDeck(card->owner, card);
    else
        AddCardToDeckTop(card->owner, card);
    RemoveLinksToZone(player, zone);
    ClearZone(player, zone);
}

/* TRUE if one of the player's face-up monsters is a Toon (IsToonMonster). */
int HasFaceUpToonMonster(int player)
{
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        /* u16, not u32: the ROM hoists the 0x7FF mask out of the loop, not the table address. */
        u16 cardId = ZONE_CARD_ID(player, zone);

        if (cardId && ZONEP(player, zone)->isFaceUp && IsToonMonster(CARD_NUMBER(cardId)))
            return TRUE;
    }
    return FALSE;
}

/* FALSE if one of the player's face-up monsters is LIGHT, DARK or WIND (effective attribute), else TRUE. */
int HasNoFaceUpLightDarkWindMonster(int player)
{
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        if (ZONE_CARD_ID(player, zone) && ZONEP(player, zone)->isFaceUp) {
            /* Matching: switched on as an int (signed compares); the function returns u32. */
            switch ((int)GetZoneCardAttribute(player, zone)) {
            case ATTRIBUTE_LIGHT:
            case ATTRIBUTE_DARK:
            case ATTRIBUTE_WIND:
                return FALSE;
            }
        }
    }
    return TRUE;
}

/*
 * Number of active copies of card number cardNo in zones 0-10 of players[player & 1]: face up and not
 * negated. The emptiness of each zone is tested twice, through players and through gDuelPlayers.
 */
int CountActiveCardsOnFieldIn(struct DuelPlayer *players, int player, u16 cardNo)
{
    int count = 0;
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_FIELD; zone++) {
        struct DuelZone *z = &players[player & 1].zones[zone];
        u16 cardId = DUEL_CARD_ID(z);

        if (cardId && ZONE_CARD_ID(player, zone) && z->isFaceUp && !z->isDisabled
            && CARD_NUMBER(cardId) == cardNo)
            count++;
    }
    return count;
}

/* CountActiveCardsOnFieldIn(gDuelPlayers, ...): the same result as CountActiveCardsOnField. */
int CountActiveCardsOnField2(int player, u16 cardNo)
{
    return CountActiveCardsOnFieldIn(gDuelPlayers, player, cardNo);
}

/* Number of the player's face-up spell/trap cards (zones 5-9) of card type type (Trap or Magic). */
int CountFaceUpSpellTrapsOfType(int player, u16 type)
{
    int count = 0;
    int zone;

    for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
        u16 cardId = ZONE_CARD_ID(player, zone);

        if (cardId && ZONEP(player, zone)->isFaceUp && CARD_TYPE(cardId) == type)
            count++;
    }
    return count;
}

/*
 * CARD_TYPE of a card ID (enum CardType).
 * FAKEMATCH: the helper exists only for its u16 parameter: it keeps the ID narrowing apart from the
 * card-word read, so the 0x7FF mask is hoisted out of the caller's loop and the table address is reloaded
 * inside it, as in the ROM.
 */
static inline int GetCardTypeById(u16 cardId)
{
    return CARD_TYPE(cardId);
}

/* Number of cards of card type type in the player's graveyard. */
int CountGraveyardCardsOfType(int player, u16 type)
{
    int i;
    int count = 0;

    for (i = 0; i < PLAYER(player).graveCount; i++) {
        if ((u32)GetCardTypeById(DUEL_CARD_ID(&PLAYER(player).graveyard[i])) == type)
            count++;
    }
    return count;
}

/* Number of the player's occupied spell/trap zones (5-9). */
int CountSpellTraps(int player)
{
    int count = 0;
    int zone;

    for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
        if (ZONE_CARD_ID(player, zone))
            count++;
    }
    return count;
}

/*
 * Number of face-down copies of card number cardNo in zones 0-10 of players[player & 1] that may be
 * activated (canActivate) and are not negated (isDisabled).
 */
int CountActivatableSetCardsIn(struct DuelPlayer *players, int player, u16 cardNo)
{
    int count = 0;
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_FIELD; zone++) {
        struct DuelZone *z = &players[player & 1].zones[zone];
        u16 cardId = DUEL_CARD_ID(z);

        if (cardId && ZONE_CARD_ID(player, zone) && !z->isFaceUp && z->canActivate && !z->isDisabled
            && CARD_NUMBER(cardId) == cardNo)
            count++;
    }
    return count;
}

/*
 * CountActivatableSetCardsIn(gDuelPlayers, ...). The callers pass the card number as a word (a signed
 * 16-bit table load); it is narrowed to u16 here.
 */
int CountActivatableSetCards(int player, int cardNoWord)
{
    u16 cardNo = cardNoWord;

    return CountActivatableSetCardsIn(gDuelPlayers, player, cardNo);
}

/*
 * If (player, zone) holds a face-up card: the number of other face-up monsters on both fields (the
 * opponent's first) with the same card name (IsSameCardName). 0 if the zone is empty or face down.
 * Matching: each zone address is built as base + (zone * 0x94 + side * 0xD64) from a separate side
 * (= player & 1) local, in both scans.
 */
int CountOtherFaceUpSameNameMonsters(int player, int zone)
{
    int side;
    struct DuelZone *z;
    u32 cardId;
    int count, scan, otherZone;

    side = player & 1;
    z = (struct DuelZone *)((u32)gDuelPlayers[0].zones + (zone * 0x94 + side * 0xD64));
    cardId = DUEL_CARD_ID(z);
    if (!z->isFaceUp || cardId == 0)
        return 0;
    count = 0;
    for (scan = 0; scan <= 1; scan++) {
        for (otherZone = ZONE_MONSTER_0; otherZone <= ZONE_MONSTER_4; otherZone++) {
            int otherPlayer = player;

            if (scan == 0)
                otherPlayer = 1 - otherPlayer;
            if (player != otherPlayer || zone != otherZone) {
                struct DuelZone *other;
                u32 otherId;
                int otherSide;

                otherSide = otherPlayer & 1;
                other = (struct DuelZone *)((u32)gDuelPlayers[0].zones + (otherZone * 0x94 + otherSide * 0xD64));
                otherId = DUEL_CARD_ID(other);
                if (otherId && other->isFaceUp && IsSameCardName(otherId, cardId))
                    count++;
            }
        }
    }
    return count;
}

/*
 * Add a link of the given kind to the zone at loc (DUEL_LOC): target is usually the DUEL_LOC of the card
 * that affects it. Unless kind is ZONE_LINK_EQUIP_ATK_200 (always a new entry), an existing link with the
 * same target, whatever its kind, only gets its stack count (high byte of linkKinds[i]) incremented.
 * FAKEMATCH: the zone address is formed three times (z, the linkKinds[i] address inside the loop,
 * zAppend), each in the ROM's operand order, and zoneOfs / playerOfs are declared before z so that
 * side * 0xD64 gets the lower pseudo-register number (and r4).
 */
void AddZoneLink(u16 loc, u16 target, u16 kind)
{
    int player = (u8)loc;           /* DUEL_LOC_PLAYER(loc); the (u8) form matches, & 0xFF does not */
    int zone = DUEL_LOC_ZONE(loc);
    struct DuelZone *zAppend;
    int zoneOfs, playerOfs;
    struct DuelZone *z;
    int side, count, i;

    side = player & 1;
    zoneOfs = zone * 0x94;
    playerOfs = side * 0xD64;
    z = (struct DuelZone *)((u32)gDuelPlayers[0].zones + (zoneOfs + playerOfs));
    count = z->numLinks;
    if (kind != ZONE_LINK_EQUIP_ATK_200) {
        for (i = 0; i < count; i++) {
            if (z->links[i] == target) {
                u16 *kindAndCount = &((struct DuelZone *)((u32)gDuelPlayers[0].zones + zone * 0x94
                    + side * 0xD64))->linkKinds[i];

                /* high byte (stack count) + 1; the low byte (kind) is re-read with ldrb */
                *kindAndCount = ((u8)((*kindAndCount >> 8) + 1) << 8) | *(u8 *)kindAndCount;
                return;
            }
        }
    }
    side = player & 1;
    zAppend = (struct DuelZone *)((u32)gDuelPlayers[0].zones + (zone * 0x94 + side * 0xD64));
    zAppend->links[count] = target;
    zAppend->linkKinds[count] = kind;
    zAppend->numLinks++;
}

/*
 * Remove one link from the zone at loc (DUEL_LOC): the first link of the given kind and, unless the kind
 * is one of 4-7 or 9-12 (one link per kind), whose target is target.
 * Matching: player is an int (no u8 zero-extension of player & 1, so loop.c hoists zone * 0x94 first), and
 * the RemoveZoneLinkAt call is written twice: the larger loop keeps the default case's links[i] address
 * computed inside it, as in the ROM; cross-jumping merges the two calls afterwards.
 */
void RemoveZoneLink(u16 loc, u16 target, u16 kind)
{
    int player = (u8)loc;           /* DUEL_LOC_PLAYER(loc); the (u8) form matches */
    int zone = DUEL_LOC_ZONE(loc);
    int i;

    for (i = 0; i < ZONE_AT(player & 1, zone)->numLinks; i++) {
        if ((u8)ZONE_AT(player & 1, zone)->linkKinds[i] == kind) {
            switch (kind) {
            case ZONE_LINK_ATK_BONUS:
            case ZONE_LINK_ABSORBED:
            case ZONE_LINK_6:
            case 7:
            case ZONE_LINK_STATS_DOWN_500:
            case ZONE_LINK_EQUIP_ATK_200:
            case ZONE_LINK_ATK_300_PER_VALUE:
            case ZONE_LINK_ATK_DOWN_200:
                RemoveZoneLinkAt(player, zone, i);
                return;
            default:
                if (ZONE_AT(player & 1, zone)->links[i] == target) {
                    RemoveZoneLinkAt(player, zone, i);
                    return;
                }
                break;
            }
        }
    }
}

/*
 * Card number of the face-up Field Magic (zone 10), player 0's first, or 0. Returns the u16 card number
 * zero-extended to a word, as the callers consume it.
 */
int GetFaceUpFieldMagicNumber(void)
{
    int player;

    for (player = 0; player <= 1; player++) {
        u16 cardId = ZONE_CARD_ID(player, ZONE_FIELD);

        if (ZONEP(player, ZONE_FIELD)->isFaceUp && cardId)
            return (u16)CARD_NUMBER(cardId);
    }
    return 0;
}

/*
 * If (player, zone) holds a card: the DUEL_LOC of the first occupied, face-up monster (player 0's zones
 * first) with a link of kind ZONE_LINK_EQUIP, CONTINUOUS, ABSORBED, 7 or EQUIP_ATK_200 to it; else 0xFFFF.
 * FAKEMATCH: the two case groups have identical bodies (the ROM's jump table has two targets; cross-jumping
 * merges them). Matching: the loops sit inside the if with no early return, so loop.c does not move the
 * return block out of the loop; the DUEL_LOC values are written player first (see RemoveLinksToZone).
 */
u16 FindMonsterWithLinkTo(int player, int zone)
{
    int monPlayer, monZone, i;

    if (DUEL_CARD_ID(ZONE_AT(player & 1, zone)) != 0) {
        for (monPlayer = 0; monPlayer <= 1; monPlayer++) {
            for (monZone = ZONE_MONSTER_0; monZone <= ZONE_MONSTER_4; monZone++) {
                if (DUEL_CARD_ID(ZONE_AT(monPlayer & 1, monZone)) != 0 && ZONE_AT(monPlayer & 1, monZone)->isFaceUp) {
                    for (i = 0; i < ZONE_AT(monPlayer & 1, monZone)->numLinks; i++) {
                        u16 target = ZONE_AT(monPlayer & 1, monZone)->links[i];
                        u8 kind = ZONE_AT(monPlayer & 1, monZone)->linkKinds[i];

                        switch (kind) {
                        case ZONE_LINK_EQUIP:
                        case ZONE_LINK_ABSORBED:
                        case ZONE_LINK_EQUIP_ATK_200:
                            if (target == (u16)((u8)player | ((u8)zone << 8)))
                                return (u8)monPlayer | ((u8)monZone << 8);
                            break;
                        case ZONE_LINK_CONTINUOUS:
                        case 7:
                            if (target == (u16)((u8)player | ((u8)zone << 8)))
                                return (u8)monPlayer | ((u8)monZone << 8);
                            break;
                        }
                    }
                }
            }
        }
    }
    return 0xFFFF;
}

/*
 * TRUE if an active Prohibition declares a card with the same name as cardId: an entry of gDuel's
 * Prohibition list (DUEL_CMD_ADD_PROHIBITION / DUEL_CMD_REMOVE_PROHIBITION) whose Prohibition card is
 * still in its zone and not negated.
 */
u32 IsCardProhibited(u16 cardId)
{
    int i;

    for (i = 0; i < gDuel.prohibitionCount; i++) {
        if (IsSameCardName(gDuel.prohibitedCards[i], cardId)) {
            /* The DUEL_LOC is read in two parts: the zone with ldrh + lsr 8, the player with ldrb. */
            u16 *loc = &gDuel.prohibitionZones[i];
            int zone = DUEL_LOC_ZONE(*loc);
            int side = *(u8 *)loc & 1;
            struct DuelZone *z = (struct DuelZone *)((u8 *)gDuel.players[0].zones
                + (zone * 0x94 + side * 0xD64));

            if (DUEL_CARD_ID(z) && !z->isDisabled)
                return TRUE;
        }
    }
    return FALSE;
}

/* Append the card to its owner's graveyard, clearing graverobbed; empty slots and tokens are dropped. */
void AddCardToGraveyard(struct DuelCard *card)
{
    u32 owner = card->owner;
    struct DuelCard *slot = &gDuelPlayers[owner].graveyard[gDuelPlayers[owner].graveCount];

    if (card->id && !IS_TOKEN(card->id)) {
        card->graverobbed = 0;
        CopyDuelCard(slot, card);
        gDuelPlayers[owner].graveCount++;
    }
}

/* Append the card to its owner's banished pile (BANISH_NORMAL), clearing graverobbed; empty slots and
 * tokens are dropped. */
void AddCardToBanished(struct DuelCard *card)
{
    u32 owner = card->owner;
    struct DuelCard *slot = &gDuelPlayers[owner].banished[gDuelPlayers[owner].banishedCount];

    if (card->id && !IS_TOKEN(card->id)) {
        card->graverobbed = 0;
        CopyDuelCard(slot, card);
        gDuelPlayers[owner].banishedInfo[gDuelPlayers[owner].banishedCount] = BANISH_NORMAL;
        gDuelPlayers[owner].banishedCount++;
    }
}

/*
 * Banish the monster from zone until the End Phase (BANISH_UNTIL_END_PHASE): its banishedInfo entry
 * remembers the zone, and the zone's bit in the owner's removedMask keeps the zone free for its return
 * (ReturnTemporarilyBanishedCard). Empty slots and tokens are dropped; graverobbed is kept.
 * FAKEMATCH: the & 1 on the 1-bit owner makes the constant 1 live in r9 across the call, and the no-op
 * (u16) cast gives the ROM's lsl/lsr #16 before the store into removedMask, which straddles bytes
 * +0x0B/+0x0C.
 */
void AddCardToBanishedTemporarily(struct DuelCard *card, int zone)
{
    u32 owner = card->owner & 1;
    struct DuelCard *slot = &gDuelPlayers[owner].banished[gDuelPlayers[owner].banishedCount];

    if (card->id && !IS_TOKEN(card->id)) {
        CopyDuelCard(slot, card);
        gDuelPlayers[owner].banishedInfo[gDuelPlayers[owner].banishedCount]
            = ((u8)zone << 8) | BANISH_UNTIL_END_PHASE;
        gDuelPlayers[owner].banishedCount++;
        gDuelPlayers[owner].removedMask = (u16)(gDuelPlayers[owner].removedMask | 1 << zone);
    }
}

/*
 * Bring the monster banished until the End Phase from (player, zone) back: copy the first
 * BANISH_UNTIL_END_PHASE entry for that zone into the zone, close the gap in banished[] and clear the
 * zone's removedMask bit.
 * Original bug: the gap is closed in banished[] only, not in banishedInfo[], so the later info entries no
 * longer line up with their cards.
 * Matching: info is a u32 (not u16) and the compaction loop has its own index j (register allocation).
 */
void ReturnTemporarilyBanishedCard(int player, int zone)
{
    int i, j;

    for (i = 0; i < PLAYER(player).banishedCount; i++) {
        u32 info = PLAYER(player).banishedInfo[i];

        if ((u8)info == BANISH_UNTIL_END_PHASE && info >> 8 == zone) {
            CopyDuelCard(&ZONE(player, zone).card, &PLAYER(player).banished[i]);
            PLAYER(player).banishedCount--;
            for (j = i; j < PLAYER(player).banishedCount; j++)
                CopyDuelCard(&PLAYER(player).banished[j], &PLAYER(player).banished[j + 1]);
            PLAYER(player).removedMask &= ~(1 << zone);
            return;
        }
    }
}

/*
 * Append the card to its owner's banished pile face down (BANISH_FACE_DOWN, high byte 0). Empty slots and
 * tokens are dropped; graverobbed is kept.
 */
void AddCardToBanishedFaceDown(struct DuelCard *card)
{
    u32 owner = card->owner;
    struct DuelCard *slot = &gDuelPlayers[owner].banished[gDuelPlayers[owner].banishedCount];

    if (card->id && !IS_TOKEN(card->id)) {
        CopyDuelCard(slot, card);
        gDuelPlayers[owner].banishedInfo[gDuelPlayers[owner].banishedCount] = BANISH_FACE_DOWN;
        gDuelPlayers[owner].banishedCount++;
    }
}
