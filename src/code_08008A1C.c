#include "global.h"

/*
 * Duel field helpers: queries over a player's 11 field zones (0-4 monster,
 * 5-9 magic/trap, 10 field), zone "link" lists, and moving zone cards to the
 * graveyard / banished lists.  See wiki/functions/code-08008a1c.md.
 */

/*
 * A card instance word as stored in the duel state (zones, graveyard, ...).
 * The containers hold plain u32 words; reads go through CARD() so that the
 * whole word is loaded (ldr + shifts), which is what the ROM does.
 */
struct DuelCard {
    u32 id : 12;        /* card ID (index into gUnk_08621DE0 / gUnk_08622AB4); 0 = none */
    u32 owner : 1;      /* bit 12: owning player (whose graveyard it goes to) */
    u32 unk13 : 5;
    u32 unk18 : 1;      /* bit 18: cleared when the card leaves the field */
    u32 unk19 : 13;
};
#define CARD(word) (*(struct DuelCard *)&(word))

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    u32 card;               /* +0x00: struct DuelCard */
    u8 filler4[2];
    u8 flags6;              /* +0x06: bit 1 = face up (hypothesis) */
    u8 filler7[3];
    u16 links[32];          /* +0x0A: (zone << 8) | player of a linked card */
    u16 linkInfo[32];       /* +0x4A: low byte = link kind, high byte = count */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[5];
    u8 flags91;             /* +0x91 */
    u8 filler92[2];
};

/* Per-player duel state, 0xD64 bytes, two of them at 0x020192E4. */
struct DuelPlayer {
    u8 unk0[4];
    u8 numGrave;            /* +0x004: count of grave[] */
    u8 unk5;
    u8 numBanished;         /* +0x006: count of banished[] */
    u8 unk7;
    u32 unk8_0 : 14;
    u32 zoneLock : 10;      /* +0x008 bits 14-23: per-zone bit, zones 0-9 */
    u32 unk8_24 : 4;
    u32 removedMask : 5;    /* +0x008 bits 28-32 (straddles into +0x00C): monster zones whose card is temporarily banished */
    u32 unkC_1 : 27;
    u8 filler10[0x18];
    struct DuelZone zones[11];      /* +0x028 */
    u32 list684[160];               /* +0x684 */
    u32 grave[160];                 /* +0x904 */
    u32 banished[80];               /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4: low byte = kind, high byte = zone */
};

extern struct DuelPlayer gUnk_020192E4[2];

/* The whole duel state: serial counter, both players, then a small list of (zone, card) pairs. */
struct Duel {
    u16 serial;                     /* 0x020192E0 */
    u8 filler2[2];
    struct DuelPlayer players[2];   /* 0x020192E4 */
    u32 unk1ACC_0 : 15;             /* +0x1ACC */
    u32 numMarked : 4;              /* +0x1ACC bits 15-18: entries in marked* */
    u32 unk1ACC_19 : 13;
    u16 markedZones[16];            /* +0x1AD0: (zone << 8) | player */
    u16 markedCards[16];            /* +0x1AF0: card IDs */
};
extern struct Duel gUnk_020192E0;
extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */
extern const u16 gUnk_08622AB4[];   /* card ID to card number */

#define PLAYER(p) (gUnk_020192E4[(p) & 1])
#define ZONE(p, z) (gUnk_020192E4[(p) & 1].zones[z])
#define ZONE_CARD(p, z) CARD(ZONE(p, z).card)
/* Zone pointer derived from the card word's address, so that field accesses
 * share one address computation with ZONE_CARD (CSE). */
#define ZONEP(p, z) ((struct DuelZone *)&ZONE(p, z).card)
/* Zone address computed as base + (zone * 0x94 + (player & 1) * 0xD64), the order some loops use. */
#define ZONE_PTR(p, z) ((struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + (((p) & 1) * 0xD64 + (z) * 0x94)))
/* Same address in the order matched shim code uses, zone * 0x94 + (player & 1) * 0xD64. */
extern u8 gUnk_0201930C[];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

/*
 * The ROM tables are indexed through integer-constant pointers. With the extern
 * arrays (SYMBOL_REF), old_agbcc loads the table address before the index math,
 * while the ROM loads it after. The bytes are the same as gUnk_08622AB4[] / gUnk_08621DE0[].
 */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Card numbers 1920-1999 are tokens. */
#define IS_TOKEN(id) ((u16)(CARD_NUMBER(id) - 1920) < 80)

#define CARD_NUMBER_1418 1418   /* Not an EDS card number. Possibly an effect key (hypothesis). */

u16 sub_08008940(int player, int zone);
u16 sub_08008A6C(int player, int zone);
u16 sub_08008C24(int player, int zone);
int sub_08009038(struct DuelPlayer *players, int player, u16 number);
int sub_080091F0(struct DuelPlayer *players, int player, u16 number);
void sub_080096F4(struct DuelCard *card);
void sub_08009768(struct DuelCard *card);
void sub_08008278(int player, int zone);
void sub_080082A0(int player, int zone, int idx);
void sub_08008D3C(int player, int zone);
int sub_08008524(int player, u16 number);
int sub_08007994(u32 id);
void sub_08007D18(u32 owner, struct DuelCard *card);
void sub_08009EAC(u32 owner, struct DuelCard *card);
void sub_08007C58(u32 owner, struct DuelCard *card);
int sub_0800756C(u16 number);
int sub_0800CAF0(int player, int zone);
void sub_08007558(struct DuelCard *dst, struct DuelCard *src);
u32 sub_080074A0(u32 id1, u32 id2);

/* Number of monster zones (0-4) for which sub_08008940 is true. */
int sub_08008A1C(int player)
{
    int count = 0;
    int i;

    for (i = 0; i <= 4; i++) {
        if (sub_08008940(player, i))
            count++;
    }
    return count;
}

/* First monster zone (0-4) for which sub_08008940 is true, or -1. */
int sub_08008A44(int player)
{
    int i;

    for (i = 0; i <= 4; i++) {
        if (sub_08008940(player, i))
            return i;
    }
    return -1;
}

/* Zone holds a non-token monster and card 1418 is on neither side of the field. */
u16 sub_08008A6C(int player, int zone)
{
    u32 id = ZONE_CARD(player, zone).id;

    if (id != 0 && !IS_TOKEN(id) && CARD_TYPE(id) <= 20
        && sub_08008524(0, CARD_NUMBER_1418) <= 0 && sub_08008524(1, CARD_NUMBER_1418) <= 0)
        return TRUE;
    return FALSE;
}

/* Number of monster zones other than `exclude` for which sub_08008A6C is true. */
int sub_08008AF8(int player, int exclude)
{
    int count;
    int i;

    if (sub_08008524(0, CARD_NUMBER_1418) > 0 || sub_08008524(1, CARD_NUMBER_1418) > 0)
        return 0;
    count = 0;
    for (i = 0; i <= 4; i++) {
        struct DuelZone *zone = &ZONE(player, i);

        if (CARD(zone->card).id && i != exclude && sub_08008A6C(player, i))
            count++;
    }
    return count;
}

/* Count occupied magic/trap zones (5-9, plus the field zone if includeField), all of them or
 * only the face-up and/or face-down ones. */
/* Return the zero-extended halfword as a word, as the ROM callers consume it. */
int sub_08008B70(int player, u16 faceUp, u16 faceDown, u16 includeField)
{
    u16 count = 0;
    int end = 10;
    u16 i;

    if (includeField)
        end = 11;
    for (i = 5; i < end; i++) {
        if (ZONE_CARD(player, i).id) {
            int ok = 0;

            if (!faceUp && !faceDown)
                ok = 1;
            if (faceUp && (ZONEP(player, i)->flags6 & 2))
                ok = 1;
            if (faceDown && !(ZONEP(player, i)->flags6 & 2))
                ok = 1;
            if (ok)
                count++;
        }
    }
    return (u16)(count);
}

/* True if the zone is empty and not locked. */
u16 sub_08008C24(int player, int zone)
{
    if (ZONE_CARD(player, zone).id == 0 && !((PLAYER(player).zoneLock >> zone) & 1))
        return TRUE;
    return FALSE;
}

/* First free magic/trap zone (5-9), or -1. */
int sub_08008C6C(int player)
{
    int i;

    for (i = 5; i <= 9; i++) {
        if (sub_08008C24(player, i))
            return i;
    }
    return -1;
}

/* Is there room to play card `id`: always for a Field magic, else a free magic/trap zone. */
int sub_08008C94(int player, u16 id)
{
    u32 stats = CARD_STATS(id);
    int type = (stats & 0x1F00000) >> 20;
    int subtype;
    int i;

    switch (type) {
    case 21:    /* Trap */
    case 22:    /* Magic */
        subtype = (stats & 0xE0000) >> 17;
        break;
    default:
        subtype = 0;
        break;
    }
    if (subtype == 2)
        return TRUE;
    for (i = 5; i <= 9; i++) {
        if (ZONE_CARD(player, i).id == 0)
            return TRUE;
    }
    return FALSE;
}

/* Send a zone's card to the banished list (flag) or the graveyard, then clear the zone. */
void sub_08008CFC(int player, int zone, u16 banish)
{
    struct DuelCard *card = &ZONE_CARD(player, zone);

    if (banish)
        sub_08009768(card);
    else
        sub_080096F4(card);
    sub_08008278(player, zone);
}

#if 0 /* NONMATCHING: the instruction sequence matches apart from register allocation. The
       * ROM keeps p in r7, z in r6, base in r2; GCC uses p=r6, z=r5, base=r7 (and
       * computes p+1 earlier). The jump-table switch and link/linkInfo reads match. */
/* Drop every link pointing to zone (player, zone): scan both players' monster zones
 * and remove links whose kind is 1, 2, 5, 7 or 10 and whose target word is (player, zone). */
void sub_08008D3C(int player, int zone)
{
    int p, z, i;

    for (p = 0; p <= 1; p++) {
        for (z = 0; z <= 4; z++) {
            for (i = 0; i < ZB(p & 1, z)->numLinks; i++) {
                u16 link = ZB(p & 1, z)->links[i];
                u8 kind = ZB(p & 1, z)->linkInfo[i];

                switch (kind) {
                case 1: case 2: case 5: case 7: case 10:
                    if (link == ((u8)player | ((u8)zone << 8)))
                        sub_080082A0(p, z, i);
                    break;
                default:
                    i++;
                    break;
                }
            }
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08008A1C", sub_08008D3C); /* 0x08008D3C size 0x108 */

/* Send a zone's card to the graveyard, drop links to it, then clear the zone. */
void sub_08008E44(int player, int zone)
{
    sub_080096F4(&ZONE_CARD(player, zone));
    sub_08008D3C(player, zone);
    sub_08008278(player, zone);
}

/* Banish a zone's card, then clear the zone. */
void sub_08008E80(int player, int zone)
{
    sub_08009768(&ZONE_CARD(player, zone));
    sub_08008278(player, zone);
}

void sub_08008EB4(int player, int zone)
{
    struct DuelCard *card = &ZONE_CARD(player, zone);

    if (sub_08007994(card->id))
        sub_08007D18(card->owner, card);
    else
        sub_08009EAC(card->owner, card);
    sub_08008D3C(player, zone);
    sub_08008278(player, zone);
}

void sub_08008F14(int player, int zone)
{
    struct DuelCard *card = &ZONE_CARD(player, zone);

    if (sub_08007994(card->id))
        sub_08007D18(card->owner, card);
    else
        sub_08007C58(card->owner, card);
    sub_08008D3C(player, zone);
    sub_08008278(player, zone);
}

/* Any face-up monster whose card number satisfies sub_0800756C. */
int sub_08008F74(int player)
{
    int i;

    for (i = 0; i <= 4; i++) {
        u16 id = ZONE_CARD(player, i).id;

        if (id && (ZONEP(player, i)->flags6 & 2) && sub_0800756C(CARD_NUMBER(id)))
            return TRUE;
    }
    return FALSE;
}

/* False if any face-up monster has sub_0800CAF0 state 1, 2 or 6. */
int sub_08008FDC(int player)
{
    int i;

    for (i = 0; i <= 4; i++) {
        if (ZONE_CARD(player, i).id && (ZONEP(player, i)->flags6 & 2)) {
            switch (sub_0800CAF0(player, i)) {
            case 1:
            case 2:
            case 6:
                return FALSE;
            }
        }
    }
    return TRUE;
}

/* Count face-up cards (all 11 zones, flags91 bit 3 clear) with card number `number`. */
int sub_08009038(struct DuelPlayer *players, int player, u16 number)
{
    int count = 0;
    int i;

    for (i = 0; i <= 10; i++) {
        struct DuelZone *zone = &players[player & 1].zones[i];
        u16 id = CARD(zone->card).id;

        if (id && ZONE_CARD(player, i).id && (zone->flags6 & 2) && !(zone->flags91 & 8)
            && CARD_NUMBER(id) == number)
            count++;
    }
    return count;
}

int sub_080090C8(int player, u16 number)
{
    return sub_08009038(gUnk_020192E4, player, number);
}

/* Count face-up magic/trap zone cards of card type `type`. */
int sub_080090E0(int player, u16 type)
{
    int count = 0;
    int i;

    for (i = 5; i <= 9; i++) {
        u16 id = ZONE_CARD(player, i).id;

        if (id && (ZONEP(player, i)->flags6 & 2) && CARD_TYPE(id) == type)
            count++;
    }
    return count;
}

/* Keep the inline ID narrowing separate from the card-word read. */
static inline int GetGraveCardType(u16 id)
{
    return CARD_TYPE(id);
}

/* Count graveyard cards of a card type. */
int sub_08009150(int player, u16 type)
{
    int i;
    int count = 0;

    for (i = 0; i < PLAYER(player).numGrave; i++) {
        if ((u32)GetGraveCardType(CARD(PLAYER(player).grave[i]).id) == type)
            count++;
    }
    return count;
}

/* Number of occupied magic/trap zones (5-9). */
int sub_080091B4(int player)
{
    int count = 0;
    int i;

    for (i = 5; i <= 9; i++) {
        if (ZONE_CARD(player, i).id)
            count++;
    }
    return count;
}

/* Count face-down cards (flags91 bits 2-3 == 1) with card number `number`. */
int sub_080091F0(struct DuelPlayer *players, int player, u16 number)
{
    int count = 0;
    int i;

    for (i = 0; i <= 10; i++) {
        struct DuelZone *zone = &players[player & 1].zones[i];
        u16 id = CARD(zone->card).id;

        if (id && ZONE_CARD(player, i).id && !(zone->flags6 & 2) && (zone->flags91 & 0xC) == 4
            && CARD_NUMBER(id) == number)
            count++;
    }
    return count;
}

/* Signed table entries arrive as words; preserve the original low-half decode. */
int sub_08009280(int player, int numberWord)
{
    u16 number = numberWord;
    return sub_080091F0(gUnk_020192E4, player, number);
}

/* Number of other face-up monsters on either side (opponent first) that are the same card
 * (sub_080074A0) as the face-up card in (player, zone); 0 if that zone is empty/face down. */
int sub_08009298(int player, int zone)
{
    int pp;
    struct DuelZone *z;
    u32 id;
    int count, side, i;

    pp = player & 1;
    z = (struct DuelZone *)((u32)gUnk_020192E4[0].zones + (zone * 0x94 + pp * 0xD64));
    id = CARD(z->card).id;
    if (!(z->flags6 & 2) || id == 0)
        return 0;
    count = 0;
    for (side = 0; side <= 1; side++) {
        for (i = 0; i <= 4; i++) {
            int p = player;

            if (side == 0)
                p = 1 - p;
            if (player != p || zone != i) {
                struct DuelZone *other;
                u32 id2;
                int pp2;

                pp2 = p & 1;
                other = (struct DuelZone *)((u32)gUnk_020192E4[0].zones + (i * 0x94 + pp2 * 0xD64));
                id2 = CARD(other->card).id;
                if (id2 && (other->flags6 & 2) && sub_080074A0(id2, id))
                    count++;
            }
        }
    }
    return count;
}
/* Add a link from zone `at` ((zone << 8) | player) to `target` with the given kind. Unless kind
 * is 10, an existing link to the same target just gets its count (high byte of linkInfo) incremented. */
void sub_0800935C(u16 at, u16 target, u16 kind)
{
    int p = (u8)at;
    int zn = at >> 8;
    struct DuelZone *z2;
    int zoneOfs, playerOfs;
    struct DuelZone *z;
    int pp, n, i;

    pp = p & 1;
    zoneOfs = zn * 0x94;
    playerOfs = pp * 0xD64;
    z = (struct DuelZone *)((u32)gUnk_020192E4[0].zones + (zoneOfs + playerOfs));
    n = z->numLinks;
    if (kind != 10) {
        for (i = 0; i < n; i++) {
            if (z->links[i] == target) {
                /* The ROM re-derives this address as base + zone * 0x94 + player * 0xD64 (a separate
                 * loop giv from links[i]). */
                u16 *info = &((struct DuelZone *)((u32)gUnk_020192E4[0].zones + zn * 0x94 + pp * 0xD64))->linkInfo[i];

                /* The low byte (kind) is re-read with ldrb. */
                *info = ((u8)((*info >> 8) + 1) << 8) | *(u8 *)info;
                return;
            }
        }
    }
    pp = p & 1;
    z2 = (struct DuelZone *)((u32)gUnk_020192E4[0].zones + (zn * 0x94 + pp * 0xD64));
    z2->links[n] = target;
    z2->linkInfo[n] = kind;
    z2->numLinks++;
}
#if 0 /* NONMATCHING: the switch tree and all loads match (each field read where used, no
       * shared pointer). Only register allocation differs: the ROM keeps target in sl and
       * player in r8, and GCC swaps them. Advisor confirmed this is the right shape. */
/* Remove one link from zone `loc` ((zone << 8) | player): the first link whose kind
 * (low byte of linkInfo) is `kind` and, unless `kind` is a special kind, whose target
 * is `target`. */
void sub_08009424(u16 loc, u16 target, u16 kind)
{
    u16 player = (u8)loc;
    int zone = loc >> 8;
    int i;

    for (i = 0; i < ZB(player & 1, zone)->numLinks; i++) {
        struct DuelZone *z = ZB(player & 1, zone);

        if ((u8)z->linkInfo[i] == kind) {
            switch (kind) {
            case 4: case 5: case 6: case 7:
            case 9: case 10: case 11: case 12:
                break;
            default:
                if (z->links[i] != target)
                    continue;
                break;
            }
            sub_080082A0(player, zone, i);
            return;
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08008A1C", sub_08009424); /* 0x08009424 size 0xC0 */

/* Card number of the face-up Field card (zone 10) of either player, or 0. */
/* The ROM callers consume the zero-extended card number as a word. */
int sub_080094E4(void)
{
    int p;

    for (p = 0; p <= 1; p++) {
        u16 id = ZONE_CARD(p, 10).id;

        if ((ZONEP(p, 10)->flags6 & 2) && id)
            return (u16)CARD_NUMBER(id);
    }
    return 0;
}

/* Zone address as (player & 1) * 0xD64 + zone * 0x94 + base; this operand order makes
 * old_agbcc emit zone * 0x94 first, as the ROM does here (ZB's order emits the player term first). */
#define ZB_PZ(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))

/* Find a zone (either player, monster zones only, occupied and face-up) holding a link
 * (kind 1, 2, 5, 7 or 10) whose target is (player, zone); return its loc (zone << 8 | player)
 * or 0xFFFF. The two case groups have identical bodies; cross-jumping merges them. The
 * loops sit inside an if (no early return), so loop.c finds no barrier to move the
 * return block out of the loop. */
u16 sub_08009538(int player, int zone)
{
    int p, z, i;

    if (CARD(ZB_PZ(player & 1, zone)->card).id != 0) {
        for (p = 0; p <= 1; p++) {
            for (z = 0; z <= 4; z++) {
                if (CARD(ZB_PZ(p & 1, z)->card).id != 0 && (ZB_PZ(p & 1, z)->flags6 & 2)) {
                    for (i = 0; i < ZB_PZ(p & 1, z)->numLinks; i++) {
                        u16 link = ZB_PZ(p & 1, z)->links[i];
                        u8 kind = ZB_PZ(p & 1, z)->linkInfo[i];

                        switch (kind) {
                        case 1: case 5: case 10:
                            if (link == (u16)((u8)player | ((u8)zone << 8)))
                                return (u8)p | ((u8)z << 8);
                            break;
                        case 2: case 7:
                            if (link == (u16)((u8)player | ((u8)zone << 8)))
                                return (u8)p | ((u8)z << 8);
                            break;
                        }
                    }
                }
            }
        }
    }
    return 0xFFFF;
}
/* True if the marked-zone list has an entry for card `id` whose zone still holds a card
 * (with +0x91 bit 3 clear). */
u32 sub_0800966C(u16 id)
{
    int i;

    for (i = 0; i < gUnk_020192E0.numMarked; i++) {
        if (sub_080074A0(gUnk_020192E0.markedCards[i], id)) {
            u16 *mz = &gUnk_020192E0.markedZones[i];
            int z = *mz >> 8;
            int p = *(u8 *)mz & 1;
            struct DuelZone *zone = (struct DuelZone *)((u8 *)gUnk_020192E0.players[0].zones
                + (z * 0x94 + p * 0xD64));

            if (CARD(zone->card).id && !(zone->flags91 & 8))
                return 1;
        }
    }
    return 0;
}

/* Put a (non-token) card into its owner's graveyard. */
void sub_080096F4(struct DuelCard *card)
{
    u32 owner = card->owner;
    struct DuelCard *dst = &CARD(gUnk_020192E4[owner].grave[gUnk_020192E4[owner].numGrave]);

    if (card->id && !IS_TOKEN(card->id)) {
        card->unk18 = 0;
        sub_08007558(dst, card);
        gUnk_020192E4[owner].numGrave++;
    }
}

/* Put a (non-token) card into its owner's banished list (kind 0). */
void sub_08009768(struct DuelCard *card)
{
    u32 owner = card->owner;
    struct DuelCard *dst = &CARD(gUnk_020192E4[owner].banished[gUnk_020192E4[owner].numBanished]);

    if (card->id && !IS_TOKEN(card->id)) {
        card->unk18 = 0;
        sub_08007558(dst, card);
        gUnk_020192E4[owner].banishedInfo[gUnk_020192E4[owner].numBanished] = 0;
        gUnk_020192E4[owner].numBanished++;
    }
}

/* Temporarily banish a monster from `zone`: banished list entry of kind 1 remembering the zone,
 * and the zone's bit set in the owner's removedMask. The `& 1` on the owner leaves the SImode 1
 * that the ROM keeps in r9; the (u16) gives the ROM's lsl/lsr #16 before the field split. */
void sub_080097F0(struct DuelCard *card, int zone)
{
    u32 owner = card->owner & 1;
    struct DuelCard *dst = &CARD(gUnk_020192E4[owner].banished[gUnk_020192E4[owner].numBanished]);

    if (card->id && !IS_TOKEN(card->id)) {
        sub_08007558(dst, card);
        gUnk_020192E4[owner].banishedInfo[gUnk_020192E4[owner].numBanished] = ((u8)zone << 8) | 1;
        gUnk_020192E4[owner].numBanished++;
        gUnk_020192E4[owner].removedMask = (u16)(gUnk_020192E4[owner].removedMask | 1 << zone);
    }
}
#if 0 /* NONMATCHING: the semantics are right and the direct gUnk_020192E4[p] indexing is closest
       * (96 vs the ROM's 99 instructions), but the ROM spills arg0 and two address bases
       * to its 12-byte frame while GCC keeps them in registers, so register allocation and spills differ. */
/* Return the temporarily banished monster of `zone` (a kind-1 banished entry whose high
 * byte is the zone) to its field zone, compact the banished card list, and clear the zone's
 * bit in the owner's removedMask. */
void sub_080098C0(int player, int zone)
{
    u16 p = player & 1;
    int i;

    for (i = 0; gUnk_020192E4[p].numBanished > i; i++) {
        s8 info = gUnk_020192E4[p].banishedInfo[i];

        if ((u8)info == 1 && (info >> 8) == zone) {
            sub_08007558(&ZONE_CARD(p, zone), &CARD(gUnk_020192E4[p].banished[i]));
            gUnk_020192E4[p].numBanished--;
            for (; i < gUnk_020192E4[p].numBanished; i++)
                sub_08007558(&CARD(gUnk_020192E4[p].banished[i]), &CARD(gUnk_020192E4[p].banished[i + 1]));
            gUnk_020192E4[p].removedMask &= ~(1 << zone);
            return;
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08008A1C", sub_080098C0); /* 0x080098C0 size 0x128 */

/* Put a (non-token) card into its owner's banished list (kind 2). */
void sub_080099E8(struct DuelCard *card)
{
    u32 owner = card->owner;
    struct DuelCard *dst = &CARD(gUnk_020192E4[owner].banished[gUnk_020192E4[owner].numBanished]);

    if (card->id && !IS_TOKEN(card->id)) {
        sub_08007558(dst, card);
        gUnk_020192E4[owner].banishedInfo[gUnk_020192E4[owner].numBanished] = 2;
        gUnk_020192E4[owner].numBanished++;
    }
}
