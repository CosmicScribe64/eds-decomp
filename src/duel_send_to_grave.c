/*
 * duel_send_to_grave (0x08017314-0x080184D7): cards leaving the field, and the zone links that tie cards
 * together (wiki/functions/duel-send-to-grave-c.md).
 *
 * A zone's link list (DuelZone.links / linkKinds / numLinks) records the cards that affect its card: equips
 * (ZONE_LINK_EQUIP), continuous effects and targets (ZONE_LINK_CONTINUOUS), absorbed monsters
 * (ZONE_LINK_ABSORBED). When a card leaves the field, the cards linked to it go too (DestroyLinkedCards).
 *
 * The cards are not moved here: each move is queued as a duel command (DuelCmd_Push) whose handler
 * animates it and then updates the duel state; only some links are removed at once (RemoveZoneLink). The
 * leave-the-field triggers are queued with Chain_AddPending. SendFieldCardToGrave is the core of
 * DestroyFieldCard; TributeMonster and SendFusionMaterialToGrave are its tribute and Polymerization
 * variants.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE */
#include "constants/cards.h"        /* enum CardNumber */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* DuelZoneIndex, ZoneLinkKind, ResponseEventKind, ChainEntryKind */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */

/* ---- BEGIN duel.h subset (pre-H0) ---- */
/*
 * The part of include/duel.h this unit uses, with the header's tags, field names and bitfield containers.
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md),
 * so this block stands in for it: it defines GUARD_DUEL_H so that the headers included below do not pull
 * in the legacy file. After H0, replace this block (BEGIN to END) with #include "legacy/duel.h".
 */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:1;
    u32 unk14:1;                    /* bit 14: set for tribute, flip, Toon and special summons and tokens */
    u32 normalSummoned:1;           /* bit 15 */
    u32 specialSummoned:1;          /* bit 16 */
    u32 unk17:15;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0 */
    u8 isFaceUp:1;                  /* +0x06 bit 1 */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u16 destroyCountdown:4;         /* +0x06 bits 6-9 */
    u8 positionLocked:1;            /* +0x07 bit 2 */
    u8 unk7_3:1;
    u8 unk7_4:1;
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet */
    u8 revivedByMonsterReborn:1;    /* +0x07 bit 6 */
    u8 summonedFromGraveyard:1;     /* +0x07 bit 7 */
    u8 levelCheckDone:1;            /* +0x08 bit 0 */
    u8 unk8_1:7;
    u8 unk9;
    u16 links[32];                  /* +0x0A: DUEL_LOC of a card affecting this one (or a value / card ID) */
    u16 linkKinds[32];              /* +0x4A: low byte enum ZoneLinkKind, high byte stack count / value */
    u16 numLinks;                   /* +0x8A */
    u8 unk8C[4];                    /* +0x8C: battle flags */
    u32 unk90_0:6;                  /* +0x90 */
    u32 unk90_6:4;
    u32 canActivate:1;              /* +0x91 bit 2 */
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:1;
    u32 declaredValue:5;            /* +0x91 bits 5-9 */
    u32 unk92_2:14;
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 graveCount;                  /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 banishedCount;               /* +0x006 */
    u8 unk7[0xB - 0x7];             /* +0x007: player flags */
    u8 crushCardTurns:3;            /* +0x00B bits 0-2 */
    u8 monsterSentToGraveThisTurn:1;/* +0x00B bit 3: Last Will condition */
    u8 unkB_4:4;
    u8 unkC[2];
    u16 lpPaid[11];                 /* +0x00E: per zone, LP paid for the card there (Toon World) */
    u16 attackableMask;             /* +0x024 */
    u16 attackedMask;               /* +0x026 */
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4 */
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */

u32 IsToonMonster(u16 cardNo);
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int FindFreeMonsterZone(int player);
void RemoveZoneLink(u16 loc, u16 target, u16 kind);
u16 FindMonsterWithLinkTo(int player, int zone);
u16 FindMonsterLinkedToCard(s32 player, s32 slot);
/* ---- END duel.h subset ---- */

#include "duel_cmd.h"       /* DuelCmd_Push */
#include "chain.h"          /* gChain, Chain_AddPending, EventResponse_Request */
#include "duel_actions.h"   /* the functions defined here; DestroyFieldCard, MoveFieldCard, LP changes */
#include "effect.h"         /* LoseLpOnSendToGraveyard */

/*
 * Card tables read through integer-constant addresses: gCardIdToNumber (0x08622AB4) and gCardStats
 * (0x08621DE0). Matching: the symbol forms (card_data.h) load other literals.
 */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])   /* gCardIdToNumber[id] */
#define CARD_STATS(id)  (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])   /* gCardStats[id] */
#define CARD_TYPE(id)   CARD_STATS_TYPE(CARD_STATS(id))                     /* enum CardType */

/* Duel command id for `player`: DUEL_CMD_PLAYER marks player 1. */
#define PLAYER_CMD(player, id) ((player) ? DUEL_CMD_PLAYER | (id) : (id))

/* DUEL_LOC(player, zone) with both halves narrowed to u8. Matching: the casts give the ROM's packing
 * (lsl #24 for the player, lsl #24; lsr #8 for the zone). */
#define LOC_U8(player, zone) ((u8)(player) | ((u8)(zone) << 8))

/*
 * The card word of a zone read through a struct DuelCard pointer. Matching: this gives the ROM's word load
 * (ldr; lsl #20; lsr #20); a direct zone->card.id read loads a halfword.
 */
#define ZONE_CARD(zone) ((struct DuelCard *)(zone))

/*
 * Zone pointers. Matching: the matched code reaches a zone's fields through the zone's address,
 * ZONE(player, zone)->field; the direct gDuelPlayers[player & 1].zones[zone].field generates other code.
 * Some functions instead need the address spelled out as byte offsets from gDuelPlayers[0].zones, with a
 * given order of the two products (it decides the order of the adds and the registers):
 *   ZONE_OF(player, zone)      (player & 1) * 0xD64 + zone * 0x94
 *   ZONE_OF_SIDE(side, zone)   side = player & 1 already in a local: zone * 0x94 + side * 0xD64
 */
#define ZONE(player, zone) (&gDuelPlayers[(player) & 1].zones[zone])
#define ZONE_OF(player, zone)                                                                              \
    ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones                                                       \
                         + (((player) & 1) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone))))
#define ZONE_OF_SIDE(side, zone)                                                                           \
    ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones                                                       \
                         + ((zone) * sizeof(struct DuelZone) + (side) * sizeof(struct DuelPlayer))))

/*
 * Chain_AddPending word of a leave-the-field trigger without its owner and card ID: the event (enum
 * ResponseEventKind) in bits 25-30 and CHAIN_KIND_OFF_FIELD in bits 21-24 (struct ChainEntry, chain.h).
 */
#define OFF_FIELD_TRIGGER(event) (((event) << 25) | (CHAIN_KIND_OFF_FIELD << 21))

/*
 * Who controls the monster in (player, zone)? Its ZONE_LINK_EQUIP links to a face-up Snatch Steal give it
 * to Snatch Steal's controller, or back to the other side when that Snatch Steal is disabled or is
 * removedLink (the equip that is leaving); card 1514 (no EDS card) works like Snatch Steal but ignores the
 * disabled flag. If the controller changes, move the monster to that player's first free monster zone, or,
 * with none free, remove the link and destroy the monster.
 */
void UpdateMonsterControl(int player, int zone, u16 removedLink)
{
    int i, j;
    int controller;
    int side = player & 1;
    struct DuelZone *monster = ZONE_OF_SIDE(side, zone);

    if (ZONE_CARD(monster)->id == 0)
        return;
    controller = player;
    for (i = monster->numLinks; i > 0; i--) {
        u16 link;
        u8 kind;
        int linkPlayer, linkZone;
        struct DuelZone *zp = ZONE_OF_SIDE(side, zone);
        j = i - 1;
        link = zp->links[j];
        kind = zp->linkKinds[j];        /* low byte: enum ZoneLinkKind */
        linkPlayer = (u8)zp->links[j];
        linkZone = link >> 8;
        if (kind == ZONE_LINK_EQUIP) {
            int linkSide = linkPlayer & 1;
            struct DuelZone *equip = ZONE_OF_SIDE(linkSide, linkZone);
            u16 equipId = ZONE_CARD(equip)->id;
            if (equipId != 0 && equip->isFaceUp) {
                switch (CARD_NUMBER(equipId)) {
                case CARD_SNATCH_STEAL: {
                    int givesBack = equip->isDisabled;
                    if (link == removedLink)
                        givesBack = 1;
                    controller = linkPlayer;
                    if (givesBack)
                        controller = 1 - controller;
                    break;
                }
                case CARD_1514:
                    controller = linkPlayer;
                    if (link == removedLink)
                        controller = 1 - controller;
                    break;
                }
            }
        }
    }
    if (controller != player) {
        int freeZone = FindFreeMonsterZone(controller);
        if (freeZone == -1) {
            RemoveZoneLink(LOC_U8(player, zone), removedLink, ZONE_LINK_EQUIP);
            DestroyFieldCard(player, zone, 1);
        } else {
            MoveFieldCard(player, LOC_U8(player, zone), LOC_U8(controller, freeZone));
        }
    }
}

/*
 * The card in (player, zone) goes to the graveyard (the game logic of DestroyFieldCard). `destroyed` makes
 * DUEL_CMD_SEND_TO_GRAVEYARD play the destruction effect; `runTriggers` enables the leave-the-field
 * effects (0 when DestroyFieldCard cascades the Destiny Board letters). In order:
 *  - with runTriggers: Umi leaving the field zone face up destroys every face-up card 1423; Prohibition
 *    lifts its ban; a face-up Field Magic resets the field background.
 *  - Banisher of the Light face up on either side: the card is banished instead, and the cards linked to it
 *    are destroyed; nothing else happens.
 *  - a monster sets the player's monsterSentToGraveThisTurn (Last Will).
 *  - Cockroach Knight and Sword of Deep-Seated go back to the top of the deck instead.
 *  - otherwise: DUEL_CMD_SEND_TO_GRAVEYARD, a response window for the opponent (Magic / Trap / monster sent
 *    to the graveyard), Premature Burial and Call of the Haunted take their monster along, Snatch Steal
 *    gives its monster back, LoseLpOnSendToGraveyard.
 *  - with runTriggers: the graveyard triggers (Sangan, Witch of the Black Forest, the equips with a
 *    graveyard effect, The Immortal of Thunder, Toon World, Spear Cretin, Prohibition).
 *  - DestroyLinkedCards.
 */
void SendFieldCardToGrave(int player, int zone, u16 destroyed, u16 runTriggers)
{
    int side = player & 1;
    struct DuelZone *z = ZONE_OF_SIDE(side, zone);
    u32 id = ZONE_CARD(z)->id;
    struct DuelCard *card;
    int i;
    u16 linkedLoc;
    u16 number;

    if (id == 0)
        return;
    card = &gDuelPlayers[player & 1].zones[zone].card;
    if (runTriggers != 0) {
        switch (CARD_NUMBER(id)) {
        case CARD_UMI:
            if (z->isFaceUp && zone == ZONE_FIELD) {
                DestroyFaceUpCardsByNumber(player, CARD_1423);
                DestroyFaceUpCardsByNumber(1 - player, CARD_1423);
            }
            break;
        case CARD_PROHIBITION:
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_REMOVE_PROHIBITION), zone, 0, 0);
            break;
        }
        /* Matching: ZONE() again, not z (with zone known to be 10 the address is formed differently) */
        if (zone == ZONE_FIELD && ZONE(player, zone)->isFaceUp)
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SET_FIELD_BACKGROUND), 0, 0, 0);
    }
    if (CountFaceUpMonstersByNumber(0, CARD_BANISHER_OF_THE_LIGHT) > 0
        || CountFaceUpMonstersByNumber(1, CARD_BANISHER_OF_THE_LIGHT) > 0) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH), zone, destroyed, 0);
        DestroyLinkedCards(player, zone, 1);
        return;
    }
    if (CARD_TYPE(id) <= CARD_TYPE_REPTILE)
        gDuelPlayers[player & 1].monsterSentToGraveThisTurn = 1;
    if (CARD_NUMBER(id) == CARD_COCKROACH_KNIGHT || CARD_NUMBER(id) == CARD_SWORD_OF_DEEP_SEATED) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), id, 1, 0);
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_RETURN_TO_DECK), zone, destroyed, 0);
        if (CARD_TYPE(id) <= CARD_TYPE_REPTILE && zone <= ZONE_MONSTER_4)
            DestroyLinkedCards(player, zone, 1);
        return;
    }
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SEND_TO_GRAVEYARD), zone, destroyed, 0);
    switch (CARD_TYPE(id)) {
    case CARD_TYPE_MAGIC:
        EventResponse_Request(1 - player, RESPONSE_MAGIC_TO_GRAVE, (u16)LOC_U8(player, zone));
        break;
    case CARD_TYPE_TRAP:
        EventResponse_Request(1 - player, RESPONSE_TRAP_TO_GRAVE, (u16)LOC_U8(player, zone));
        break;
    default:
        EventResponse_Request(1 - player, RESPONSE_MONSTER_TO_GRAVE, (u16)LOC_U8(player, zone));
        break;
    }
    switch (CARD_NUMBER(id)) {
    case CARD_PREMATURE_BURIAL:
        /* the monster it revived (it holds a ZONE_LINK_EQUIP link to this card) is destroyed too */
        linkedLoc = FindMonsterWithLinkTo(player, zone);
        if (linkedLoc != 0xFFFF && runTriggers != 0) {
            RemoveZoneLink(linkedLoc, LOC_U8(player, zone), ZONE_LINK_EQUIP);
            SendFieldCardToGrave((u8)linkedLoc, linkedLoc >> 8, 1, 1);
        }
        break;
    case CARD_CALL_OF_THE_HAUNTED:
        /* likewise, unless this Call of the Haunted was disabled */
        linkedLoc = FindMonsterWithLinkTo(player, zone);
        if (linkedLoc != 0xFFFF && runTriggers != 0 && !ZONE(player, zone)->isDisabled) {
            RemoveZoneLink(linkedLoc, LOC_U8(player, zone), ZONE_LINK_CONTINUOUS);
            SendFieldCardToGrave((u8)linkedLoc, linkedLoc >> 8, 1, 1);
        }
        break;
    case CARD_SNATCH_STEAL:
    case CARD_1514:
        linkedLoc = FindMonsterWithLinkTo(player, zone);
        if (linkedLoc != 0xFFFF)
            UpdateMonsterControl((u8)linkedLoc, linkedLoc >> 8, LOC_U8(player, zone));
        break;
    }
    LoseLpOnSendToGraveyard(player, 1);
    if (runTriggers == 0)
        return;
    number = CARD_NUMBER(id);
    switch (number) {
    case CARD_SANGAN:
    case CARD_WITCH_OF_THE_BLACK_FOREST:
    case CARD_1241:
    case CARD_1257:
        /* only a monster that was summoned (not one destroyed while Set), or a card outside the monster
         * zones (agbcc tests the three bits as one mask) */
        if ((card->unk14 || card->normalSummoned || card->specialSummoned) || zone > ZONE_MONSTER_4)
            Chain_AddPending((card->owner << 31) | OFF_FIELD_TRIGGER(RESPONSE_MONSTER_TO_GRAVE) | id, 0);
        break;
    case CARD_1242:
        Chain_AddPending((card->owner << 31) | OFF_FIELD_TRIGGER(RESPONSE_MONSTER_TO_GRAVE) | id, 0);
        break;
    case CARD_AXE_OF_DESPAIR:
    case CARD_BLACK_PENDANT:
    case CARD_HORN_OF_LIGHT:
    case CARD_HORN_OF_THE_UNICORN:
    case CARD_MALEVOLENT_NUZZLER:
        /* equips with a graveyard effect: the trigger event is RESPONSE_BATTLE_DESTROYED while that
         * response window is open (the equipped monster died in battle), else RESPONSE_MAGIC_TO_GRAVE */
        Chain_AddPending((card->owner << 31)
                             | (gChain.responseEvent == RESPONSE_BATTLE_DESTROYED
                                    ? OFF_FIELD_TRIGGER(RESPONSE_BATTLE_DESTROYED)
                                    : OFF_FIELD_TRIGGER(RESPONSE_MAGIC_TO_GRAVE))
                             | id,
                         0);
        break;
    case CARD_THE_IMMORTAL_OF_THUNDER:
        /* the player loses 5000 LP when it leaves the field (not while its effectUnused flag is set) */
        if (!ZONE(player, zone)->effectUnused) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), id, 1, 0);
            LoseLifePoints(player, 5000);
        }
        break;
    case CARD_TOON_WORLD:
        if (ZONE(player, zone)->isFaceUp && !ZONE(player, zone)->isDisabled) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), id, 1, 0);
            /* the last Toon World of the player gone: every Toon monster on both fields is destroyed */
            if (CountActiveCardsOnFieldExcept(player, number, zone) == 0) {
                for (i = 0; i <= ZONE_MONSTER_4; i++) {
                    u16 toonId = ZONE_CARD(ZONE(player, i))->id;
                    if (toonId != 0 && IsToonMonster(CARD_NUMBER(toonId)))
                        SendFieldCardToGrave(player, i, 1, 1);
                }
                for (i = 0; i <= ZONE_MONSTER_4; i++) {
                    u16 toonId = ZONE_CARD(ZONE(1 - player, i))->id;
                    if (toonId != 0 && IsToonMonster(CARD_NUMBER(toonId)))
                        SendFieldCardToGrave(1 - player, i, 1, 1);
                }
            }
            /* the LP paid for this Toon World come back. Matching: read twice (CSE'd); a local changes the
             * register choice. */
            if (gDuelPlayers[player & 1].lpPaid[zone] != 0) {
                GainLifePoints(player, gDuelPlayers[player & 1].lpPaid[zone]);
                DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_UPDATE_ZONE_LP_PAID), zone, 0, 0);
            }
        }
        break;
    case CARD_SPEAR_CRETIN:
        if (!ZONE(player, zone)->effectUnused)
            Chain_AddPending(((card->owner & 1) << 31) | OFF_FIELD_TRIGGER(RESPONSE_MONSTER_TO_GRAVE) | id, 0);
        break;
    case CARD_PROHIBITION:
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_REMOVE_PROHIBITION), zone, 0, 0);
        break;
    }
    DestroyLinkedCards(player, zone, 1);
}

/* Queue DUEL_CMD_ADD_ZONE_LINK: the zone at DUEL_LOC `at` gets a link of `kind` to `target`. */
void QueueAddZoneLink(int player, u16 target, u16 at, u16 kind)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_ADD_ZONE_LINK), target, at, kind);
}

/* Queue DUEL_CMD_REMOVE_ZONE_LINK: remove the `kind` link to `target` from the zone at DUEL_LOC `at`. */
void QueueRemoveZoneLink(int player, u16 target, u16 at, u16 kind)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_REMOVE_ZONE_LINK), target, at, kind);
}

/*
 * Equip the card at equipLoc to the monster at targetLoc (DUEL_CMD_ADD_EQUIP_LINK). Card 1351 (no EDS card)
 * destroys any equip put on it, and itself too when the equip is Premature Burial; otherwise the opponent
 * gets a RESPONSE_EQUIP window.
 */
void EquipCard(int player, u16 equipLoc, u16 targetLoc)
{
    int equipPlayer = (u8)equipLoc;
    int equipZone = equipLoc >> 8;
    int targetPlayer = (u8)targetLoc;
    int targetZone = targetLoc >> 8;
    u16 targetId = ZONE_CARD(ZONE(targetPlayer, targetZone))->id;

    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_ADD_EQUIP_LINK), equipLoc, targetLoc, 0);
    if (CARD_NUMBER(targetId) == 1351) {     /* no EDS card; constants/cards.h has no CARD_1351 */
        DuelCmd_Push(PLAYER_CMD(targetPlayer, DUEL_CMD_SHOW_CARD_EFFECT), targetId, 1, 0);
        DestroyFieldCard(equipPlayer, equipZone, 1);
        if (CARD_NUMBER(ZONE_CARD(ZONE(equipPlayer, equipZone))->id) == CARD_PREMATURE_BURIAL)
            DestroyFieldCard(targetPlayer, targetZone, 1);
    } else {
        EventResponse_Request(1 - player, RESPONSE_EQUIP, equipLoc | (targetLoc << 16));
    }
}

/* Move the equip card at equipLoc from the monster it equips to the monster at newTargetLoc. */
void MoveEquipCard(u16 equipLoc, u16 newTargetLoc)
{
    u8 player = equipLoc;
    u16 monsterLoc = FindMonsterLinkedToCard(player, equipLoc >> 8);

    QueueRemoveZoneLink(player, equipLoc, monsterLoc, ZONE_LINK_EQUIP);
    EquipCard(player, equipLoc, newTargetLoc);
}

/*
 * Meant to queue the removal of the equip and continuous links that point at (player, zone). Original
 * bug, kept (it matches): for every link of every occupied zone of both players it reads
 * links[p] / linkKinds[p] of (player, zone) itself, p being the player loop index, and queues
 * QueueRemoveZoneLink(player, (player, zone), that link, kind) for kinds 1 and 2.
 */
void QueueRemoveLinksToZone(int player, int zone)
{
    int p, z, i;

    for (p = 0; p < 2; p++) {
        for (z = 0; z < DUEL_ZONE_COUNT; z++) {
            if (ZONE_CARD(ZONE(p, z))->id != 0) {
                for (i = 0; i < ZONE(p, z)->numLinks; i++) {
                    u16 link = ZONE(player, zone)->links[p];
                    u8 kind = ZONE(player, zone)->linkKinds[p];
                    switch (kind) {
                    case ZONE_LINK_EQUIP:
                    case ZONE_LINK_CONTINUOUS:
                        QueueRemoveZoneLink(player, LOC_U8(player, zone), link, kind);
                        break;
                    }
                }
            }
        }
    }
}

/*
 * Queue the removal of every ZONE_LINK_CONTINUOUS link of (player, zone) that points at
 * (targetPlayer, targetZone).
 */
void RemoveTargetLinksTo(int player, int zone, int targetPlayer, int targetZone)
{
    /* FAKEMATCH: the counter is pinned to r5 (the ROM's allocation); it must be an initialized local. */
    register int i asm("r5") = 0;

    for (; i < ZONE(player, zone)->numLinks; i++) {
        /* Matching: the side offset first, then the zone pointer, then the two entry addresses as separate
         * additions (base + field offset, then + i * 2). */
        int sideOffset = (player & 1) * sizeof(struct DuelPlayer);
        struct DuelZone *z =
            (struct DuelZone *)((u8 *)gDuelPlayers[0].zones + (zone * sizeof(struct DuelZone) + sideOffset));
        int offset = i * 2;
        u16 *linkPtr = z->links;
        u8 *kindPtr;
        u16 link;
        int linkPlayer;
        int linkZone;
        int kind;
        linkPtr = (u16 *)((u8 *)linkPtr + offset);
        kindPtr = (u8 *)z->linkKinds;
        kindPtr += offset;
        link = *linkPtr;
        linkPlayer = *(u8 *)linkPtr;
        linkZone = link >> 8;
        kind = *kindPtr;    /* low byte of linkKinds[i] */
        if (kind == ZONE_LINK_CONTINUOUS && linkPlayer == targetPlayer && linkZone == targetZone)
            QueueRemoveZoneLink(targetPlayer, link, LOC_U8(player, zone), ZONE_LINK_CONTINUOUS);
    }
}

/*
 * The card in (player, zone) leaves the field: destroy the cards linked to it. Equips (ZONE_LINK_EQUIP,
 * ZONE_LINK_EQUIP_ATK_200) that are still there, absorbed monsters (ZONE_LINK_ABSORBED) and kind 6 go
 * always; of the ZONE_LINK_CONTINUOUS cards, Spellbinding Circle, Call of the Haunted and card 1417 go
 * when `destroyed` is set and card 1244 always, unless the card is disabled. For a monster zone, then drop
 * the continuous links that all monsters hold to it.
 */
void DestroyLinkedCards(int player, int zone, u16 destroyed)
{
    int i;
    int z;

    for (i = 0; i < ZONE(player, zone)->numLinks; i++) {
        int side = player & 1;
        struct DuelZone *zp = ZONE_OF_SIDE(side, zone);
        int offset = i * 2;
        u16 *linkPtr = zp->links;
        u8 *kindPtr;
        int linkPlayer, linkZone;
        u16 linkedId;
        int destroy;
        linkPtr = (u16 *)((u8 *)linkPtr + offset);
        kindPtr = (u8 *)zp->linkKinds;
        kindPtr += offset;
        linkPlayer = (u8)*linkPtr;
        linkZone = *linkPtr >> 8;
        linkedId = ZONE_CARD(ZONE(linkPlayer, linkZone))->id;
        destroy = 0;

        switch (*kindPtr) {
        case ZONE_LINK_EQUIP:
        case ZONE_LINK_EQUIP_ATK_200:
            if (linkedId != 0)
                destroy = 1;
            break;
        case ZONE_LINK_CONTINUOUS:
            switch (CARD_NUMBER(linkedId)) {
            case CARD_SPELLBINDING_CIRCLE:
            case CARD_CALL_OF_THE_HAUNTED:
            case CARD_1417:
                if (!ZONE(linkPlayer, linkZone)->isDisabled)
                    destroy = destroyed;
                break;
            case CARD_1244:
                if (!ZONE(linkPlayer, linkZone)->isDisabled)
                    destroy = 1;
                break;
            }
            break;
        case ZONE_LINK_ABSORBED:
        case ZONE_LINK_6:
            destroy = 1;
            break;
        }
        if (destroy)
            DestroyFieldCard(linkPlayer, linkZone, destroyed != 0);
    }
    if (zone <= ZONE_MONSTER_4) {
        for (i = 0; i <= 1; i++)
            for (z = 0; z <= ZONE_MONSTER_4; z++)
                RemoveTargetLinksTo(i, z, player, zone);
    }
}

/* Destroy every monster absorbed by (player, zone): the targets of its ZONE_LINK_ABSORBED links. */
void DestroyAbsorbedMonsters(int player, int zone)
{
    int i = 0;

    if (i < ZONE(player, zone)->numLinks) {
        struct DuelZone *z = ZONE_OF(player, zone);
        u16 *count = &z->numLinks;  /* Matching: re-read through this pointer after each call */
        do {
            int offset = i * 2;
            u16 *linkPtr = z->links;
            u8 *kindPtr;
            int linkPlayer;
            int linkZone;
            int kind;
            linkPtr = (u16 *)((u8 *)linkPtr + offset);
            kindPtr = (u8 *)z->linkKinds;
            kindPtr += offset;
            linkPlayer = *(u8 *)linkPtr;
            linkZone = *linkPtr >> 8;
            kind = *kindPtr;
            if (kind == ZONE_LINK_ABSORBED)
                DestroyFieldCard(linkPlayer, linkZone, 1);
            i++;
        } while (i < *count);
    }
}

/*
 * Tribute the monster in (player, zone) (Tribute Summons and tribute costs). Returns 0 for a spell/trap
 * zone, an empty zone, or while card 1418 (no EDS card) forbids tributes on either field; else 1. With
 * Banisher of the Light face up the monster is banished instead (DUEL_CMD_BANISH) and the function returns
 * without a value, as in the ROM. Otherwise DUEL_CMD_TRIBUTE_MONSTER, the graveyard triggers (as in
 * SendFieldCardToGrave, without the summon check), DestroyLinkedCards and a RESPONSE_MONSTER_TO_GRAVE
 * window for the opponent.
 */
int TributeMonster(int player, int zone)
{
    int side = player & 1;
    int zoneOffset = zone * sizeof(struct DuelZone);
    int sideOffset = side * sizeof(struct DuelPlayer);
    u32 id = ZONE_CARD((u8 *)gDuelPlayers[0].zones + sideOffset + zoneOffset)->id;

    if (zone > ZONE_MONSTER_4)
        return 0;
    if (CountActiveCardsOnField(0, CARD_1418) > 0 || CountActiveCardsOnField(1, CARD_1418) > 0)
        return 0;
    if (id != 0) {
        struct DuelZone *z = &gDuelPlayers[player & 1].zones[zone];
        if (CountFaceUpMonstersByNumber(0, CARD_BANISHER_OF_THE_LIGHT) > 0
            || CountFaceUpMonstersByNumber(1, CARD_BANISHER_OF_THE_LIGHT) > 0) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH), zone, 1, 0);
            DestroyLinkedCards(player, zone, 1);
            return; /* Matching: no value (r0 is whatever DestroyLinkedCards left) */
        }
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_TRIBUTE_MONSTER), zone, 0, 0);
        if (CARD_TYPE(id) <= CARD_TYPE_REPTILE)
            gDuelPlayers[player & 1].monsterSentToGraveThisTurn = 1;
        LoseLpOnSendToGraveyard(player, 1);
        switch (CARD_NUMBER(id)) {
        case CARD_SANGAN:
        case CARD_WITCH_OF_THE_BLACK_FOREST:
        case CARD_1242:
        case CARD_1257:
            Chain_AddPending((ZONE_CARD(z)->owner << 31) | OFF_FIELD_TRIGGER(RESPONSE_MONSTER_TO_GRAVE) | id, 0);
            break;
        case CARD_AXE_OF_DESPAIR:
        case CARD_BLACK_PENDANT:
        case CARD_HORN_OF_LIGHT:
        case CARD_HORN_OF_THE_UNICORN:
        case CARD_MALEVOLENT_NUZZLER:
            Chain_AddPending((ZONE_CARD(z)->owner << 31) | OFF_FIELD_TRIGGER(RESPONSE_MAGIC_TO_GRAVE) | id, 0);
            break;
        case CARD_THE_IMMORTAL_OF_THUNDER:
            if (!ZONE(player, zone)->effectUnused) {
                DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), id, 1, 0);
                LoseLifePoints(player, 5000);
            }
            break;
        case CARD_SPEAR_CRETIN:
            if (!ZONE(player, zone)->effectUnused)
                Chain_AddPending(((ZONE_CARD(z)->owner & 1) << 31) | OFF_FIELD_TRIGGER(RESPONSE_MONSTER_TO_GRAVE)
                                     | id,
                                 0);
            break;
        }
        switch (CARD_NUMBER(id)) {
        case CARD_DARK_EYES_ILLUSIONIST:
        case CARD_1332:
            QueueRemoveLinksToZone(player, zone);
            break;
        }
        DestroyLinkedCards(player, zone, 0);
        EventResponse_Request(1 - player, RESPONSE_MONSTER_TO_GRAVE, (u16)LOC_U8(player, zone));
        return 1;
    }
    return 0;
}

/*
 * Send the monster in (player, zone) to the graveyard as a Fusion material (Polymerization). Like
 * TributeMonster with DUEL_CMD_FUSION_MATERIAL_TO_GRAVE, but without the card 1418 block and the response
 * window; returns 1, 0 for a spell/trap or empty zone, and no value on the Banisher of the Light path.
 */
int SendFusionMaterialToGrave(int player, int zone)
{
    int side = player & 1;  /* Matching: a separate local, for the ROM's order of the address terms */
    struct DuelZone *z = ZONE_OF_SIDE(side, zone);
    u32 id = ZONE_CARD(z)->id;

    if (zone > ZONE_MONSTER_4)
        return 0;
    if (id != 0) {
        struct DuelCard *card = &gDuelPlayers[player & 1].zones[zone].card;
        if (CountFaceUpMonstersByNumber(0, CARD_BANISHER_OF_THE_LIGHT) > 0
            || CountFaceUpMonstersByNumber(1, CARD_BANISHER_OF_THE_LIGHT) > 0) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH), zone, 1, 0);
            DestroyLinkedCards(player, zone, 1);
            return; /* Matching: no value, as in TributeMonster */
        }
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_FUSION_MATERIAL_TO_GRAVE), zone, 0, 0);
        if (CARD_TYPE(id) <= CARD_TYPE_REPTILE)
            gDuelPlayers[player & 1].monsterSentToGraveThisTurn = 1;
        LoseLpOnSendToGraveyard(player, 1);
        switch (CARD_NUMBER(id)) {
        case CARD_SANGAN:
        case CARD_WITCH_OF_THE_BLACK_FOREST:
        case CARD_1242:
        case CARD_1257:
            Chain_AddPending((card->owner << 31) | OFF_FIELD_TRIGGER(RESPONSE_MONSTER_TO_GRAVE) | id, 0);
            /* fall through (unlike TributeMonster): these also queue the RESPONSE_MAGIC_TO_GRAVE trigger */
        case CARD_AXE_OF_DESPAIR:
        case CARD_BLACK_PENDANT:
        case CARD_HORN_OF_LIGHT:
        case CARD_HORN_OF_THE_UNICORN:
        case CARD_MALEVOLENT_NUZZLER:
            Chain_AddPending((card->owner << 31) | OFF_FIELD_TRIGGER(RESPONSE_MAGIC_TO_GRAVE) | id, 0);
            break;
        case CARD_THE_IMMORTAL_OF_THUNDER:
            if (!ZONE(player, zone)->effectUnused) {
                DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), id, 1, 0);
                LoseLifePoints(player, 5000);
            }
            break;
        case CARD_SPEAR_CRETIN:
            if (!z->effectUnused)
                Chain_AddPending(((card->owner & 1) << 31) | OFF_FIELD_TRIGGER(RESPONSE_MONSTER_TO_GRAVE) | id, 0);
            break;
        }
        switch (CARD_NUMBER(id)) {
        case CARD_DARK_EYES_ILLUSIONIST:
        case CARD_1332:
            QueueRemoveLinksToZone(player, zone);
            break;
        }
        DestroyLinkedCards(player, zone, 0);
        return 1;
    }
    return 0;
}
