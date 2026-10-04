#include "global.h"
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_REPTILE (the last monster type) */
#include "constants/duel.h"         /* enum DuelZoneIndex, ResponseEventKind, ChainEntryKind */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* ids, DUEL_CMD_PLAYER */
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_TOKEN_FIRST / _END, CARD_STATS_TYPE */
#include "duel_actions.h"           /* the field actions defined here and their siblings */
#include "effect.h"                 /* LoseLpOnSendToGraveyard */

/*
 * Field actions of the duel rules (wiki/functions/duel-field-moves-c.md): destroy, banish, return to the
 * hand or deck, flip, change the battle position, move or swap zones, and discard from the hand. None of
 * them changes the field directly: each one queues the animated duel command that does it (DuelCmd_Push;
 * the zone keeps its card until the duel command runner executes the command), destroys the cards linked
 * to the one that leaves (DestroyLinkedCards) and applies the card rules involved, queuing triggers with
 * Chain_AddPending: Banisher of the Light, Cockroach Knight, Sangan and the other "when destroyed in
 * battle" monsters, The Immortal of Thunder, Call of the Haunted, Crass Clown, Dream Clown, Ameba,
 * Griggle, Minar, Electric Snake, Magic Thorn.
 */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below (chain.h,
 * battle.h, duel_cmd.h) use, with the header's names, types and bitfield containers (unused bytes are
 * padding), and defines duel.h's include guard so that those headers do not pull in the legacy file.
 * After H0, replace the block (BEGIN to END) with #include "legacy/duel.h": that gives identical assembly
 * (checked against the staged header). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
    u32 owner:1;                    /* bit 12: owning player: whose graveyard, hand or deck the card returns to */
    u32 unk13:1;                    /* bit 13 */
    u32 unk14:1;                    /* bit 14 */
    u32 normalSummoned:1;           /* bit 15 */
    u32 specialSummoned:1;          /* bit 16 */
    u32 planted:1;                  /* bit 17: Parasite Paracide shuffled into the other player's deck */
    u32 graverobbed:1;              /* bit 18 */
    u32 unk19:1;
    u32 isFusionMaterial:1;         /* bit 20 */
    u32 destroyedInBattle:1;        /* bit 21: set before a battle-destroyed monster enters the graveyard */
    u32 destroyedByOpponent:1;      /* bit 22 */
    u32 flag23:1;                   /* bit 23 */
    u32 pendingEquip:1;             /* bit 24 */
    u32 equipZone:3;                /* bits 25-27 */
    u32 pendingOpponentSummon:1;    /* bit 28 */
    u32 unk29:3;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u16 destroyCountdown:4;         /* +0x06 bits 6-9; u16 container */
    u8 positionLocked:1;            /* +0x07 bit 2 */
    u8 unk7_3:1;                    /* +0x07 bit 3 */
    u8 unk7_4:1;                    /* +0x07 bit 4 */
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet (Ameba, Griggle) */
    u8 revivedByMonsterReborn:1;    /* +0x07 bit 6 */
    u8 summonedFromGraveyard:1;     /* +0x07 bit 7 */
    u8 levelCheckDone:1;            /* +0x08 bit 0 */
    u8 unk8_1:7;
    u8 unk9;
    u16 links[32];                  /* +0x0A */
    u16 linkKinds[32];              /* +0x4A */
    u16 numLinks;                   /* +0x8A */
    u8 unk8C_0:1;                   /* +0x8C: battle flags */
    u8 destroyAfterBattle:1;        /* +0x8C bit 1 */
    u32 returnAfterBattle:1;        /* +0x8C bit 2; u32 container */
    u8 cannotAttackNextTurn:1;      /* +0x8C bit 3 */
    u8 cannotAttack:1;              /* +0x8C bit 4 */
    u8 atkHalved:1;                 /* +0x8C bit 5 */
    u8 unk8C_6:2;
    u8 unk8D[3];
    u32 unk90_0:6;                  /* +0x90 */
    u32 unk90_6:4;                  /* +0x90 bits 6-9 */
    u32 canActivate:1;              /* +0x91 bit 2 */
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:1;
    u32 declaredValue:5;            /* +0x91 bits 5-9 */
    u32 unk92_2:14;
};

/* A card in a zone or pile, seen as status bytes (the bits of struct DuelCard, one byte access at a time);
 * applied to a whole zone, whose card word comes first. */
struct DuelCardStatusBytes {
    u8 cardIdLow;                   /* +0x00: card word bits 0-7 */
    u8 cardWordBits8to13:6;         /* +0x01: id (high bits), owner, unk13 */
    u8 unk14:1;                     /* +0x01 bit 6 = card bit 14 */
    u8 normalSummoned:1;            /* bit 15 */
    u8 specialSummoned:1;           /* +0x02 bit 0 = card bit 16 */
    u8 planted:1;                   /* bit 17 */
    u8 graverobbed:1;               /* bit 18 */
    u8 unk19:1;                     /* bit 19 */
    u8 isFusionMaterial:1;          /* bit 20 */
    u8 destroyedInBattle:1;         /* bit 21 */
    u8 destroyedByOpponent:1;       /* bit 22 */
    u8 flag23:1;                    /* bit 23 */
    u8 pendingEquip:1;              /* +0x03 bit 0 = card bit 24 */
    u8 equipZone:3;                 /* bits 25-27 */
    u8 pendingOpponentSummon:1;     /* bit 28 */
    u8 unk29:3;
    u8 restOfZone[0x94 - 4];
};

struct DuelPlayer {
    u8 unk0[0xB];
    u8 crushCardTurns:3;            /* +0x00B bits 0-2 */
    u8 monsterSentToGraveThisTurn:1;/* +0x00B bit 3: Last Will condition */
    u8 unkB_4:4;
    u8 unkC[0x28 - 0xC];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    u8 piles[0xD64 - 0x684];        /* +0x684: hand, deck, graveyard, fusion deck, banished */
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 linkError:1;                 /* +0x1B12 bit 5 */
    u8 result:2;                    /* +0x1B12 bits 6-7 */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */

u32 HasFlipEffect(u16 cardNo, int inBattle);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int CountMonsters(int player);
u16 FindMonsterLinkedToCard(s32 player, s32 slot);
/* ---- END duel.h stand-in ---- */

#include "chain.h"                  /* gChain.responseEvent, Chain_AddPending, EventResponse_Request */
#include "battle.h"                 /* gBattle.atkSlot / defSlot */
#include "duel_cmd.h"               /* DuelCmd_Push */

/* Local views of functions, kept on purpose (matching choices, see build/readability/HEADERS.md): the
 * definitions take a u16 card ID, but ChangeBattlePosition and SendBattleDestroyedCardToGraveyard pass
 * 32-bit IDs without the narrowing a u16 prototype adds at the call. */
int CanActivateEffectOfCardU32(int player, u32 cardId, int fromHand) asm("CanActivateEffectOfCard");
void sub_080197C0Int(int player, int cardId) asm("sub_080197C0");

/* Command id for DuelCmd_Push: DUEL_CMD_PLAYER (bit 15) marks a command of player 1. */
#define PLAYER_CMD(player, cmd) ((player) ? (DUEL_CMD_PLAYER | (cmd)) : (cmd))

/* Zone `zone` of player `player` (& 1): &gDuelPlayers[player & 1].zones[zone]. Matching: written inside
 * each memory reference, the array form keeps the zone base in one register that CSE shares. */
#define ZONE(player, zone) (&gDuelPlayers[(player) & 1].zones[zone])
/* The same address as a raw integer: gDuelZones (0x0201930C = &gDuelPlayers[0].zones[0]) plus the zone and
 * player strides. Matching: the zone term comes first, and the base stays an integer constant inside the
 * loops. */
#define DUEL_ZONES_ADDR 0x0201930C
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + ((player) & 1) * sizeof(struct DuelPlayer) \
                         + DUEL_ZONES_ADDR))
/* The card ID of a zone, read through a struct DuelCard pointer. Matching: `zp->card.id` takes another
 * expansion path and changes the code. */
#define ZONE_CARD_ID(zp) (((struct DuelCard *)(zp))->id)

/* The card tables through integer-constant addresses: gCardIdToNumber (0x08622AB4), gCardStats
 * (0x08621DE0) and gCardNumberToId (0x08623DF4). Matching: the literal form, not the symbols. */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER_TO_ID ((const u16 *)0x08623DF4)
/* Card numbers 1920-1999 are monster tokens: they cannot go to the hand or the deck. */
#define IS_TOKEN_NUMBER(no) ((u16)((no) - CARD_NUMBER_TOKEN_FIRST) < CARD_NUMBER_TOKEN_END - CARD_NUMBER_TOKEN_FIRST)

/* The ID (bits 0-11) of a card word read as a whole u32 (ldr, then lsl #20; lsr #20). */
#define CARD_WORD(ptr) (*(u32 *)(ptr))
#define CARD_ID(word) (((word) << 20) >> 20)

/* Fields of a Chain_AddPending trigger word: card | zone << 16 | kind << 21 | event << 25 | player << 31
 * (struct ChainEntry, chain.h). kind is an enum ChainEntryKind, event an enum ResponseEventKind. */
#define TRIGGER_ZONE(zone) ((zone) << 16)
#define TRIGGER_KIND(kind) ((kind) << 21)
#define TRIGGER_EVENT(event) ((event) << 25)
#define TRIGGER_PLAYER(player) ((player) << 31)

/* Destroy (with effects) every face-up card with card number cardNo in the player's zones 0-10.
 * SendFieldCardToGrave uses it for effect key 1423 when a face-up Umi leaves the field zone. */
void DestroyFaceUpCardsByNumber(int player, u16 cardNo)
{
    int zone;

    for (zone = 0; zone <= ZONE_FIELD; zone++) {
        struct DuelZone *z = ZONE_AT(player, zone);
        u16 id = ZONE_CARD_ID(z);
        if (id != 0 && z->isFaceUp && CARD_NUMBER(id) == cardNo)
            DestroyFieldCard(player, zone, 1);
    }
}

/* Destroy the card in (player, zone): SendFieldCardToGrave queues DUEL_CMD_SEND_TO_GRAVEYARD (or a banish
 * under Banisher of the Light) and, with withEffects, the leave-the-field triggers. The zone keeps its card
 * until the command runs. The group of effect keys 1528 and 1541-1544 (no EDS card) goes together: the
 * card is shown (cmd 0x74) and the others of the group in zones 5-9 are destroyed without effects. */
void DestroyFieldCard(int player, int zone, u16 withEffects)
{
    u32 id;
    u16 cardId;
    int i;

    id = ZONE_CARD_ID(ZONE(player, zone));
    cardId = id;

    SendFieldCardToGrave(player, zone, withEffects, withEffects != 0);
    if (id == 0)
        return;
    switch (CARD_NUMBER(id)) {
    case CARD_1528:
    case CARD_1541:
    case CARD_1542:
    case CARD_1543:
    case CARD_1544:
        break;
    default:
        return;
    }
    ShowDestroyedCard(player, cardId);
    for (i = ZONE_SPELL_0; i <= ZONE_SPELL_4; i++) {
        if (i != zone) {
            u32 otherId = ZONE_CARD_ID(ZONE(player, i));
            if (otherId != 0) {
                switch (CARD_NUMBER(otherId)) {
                case CARD_1528:
                case CARD_1541:
                case CARD_1542:
                case CARD_1543:
                case CARD_1544:
                    SendFieldCardToGrave(player, i, withEffects, 0);
                    break;
                }
            }
        }
    }
}

/* DestroyFieldCard on every occupied monster zone of the player (no caller in the ROM). */
void DestroyPlayerMonsters(int player, u16 withEffects)
{
    int zone;

    for (zone = 0; zone <= ZONE_MONSTER_4; zone++) {
        struct DuelZone *z = ZONE_AT(player, zone);
        if (ZONE_CARD_ID(z) != 0)
            DestroyFieldCard(player, zone, withEffects);
    }
}

/* Banish a monster destroyed in battle: card is the copy of its zone in gBattle.zones[player]. Queues
 * DUEL_CMD_ADD_CARD_TO_BANISHED with its card word, then destroys the linked cards. */
void BanishBattleDestroyedCard(int player, int zone, u16 *card)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_ADD_CARD_TO_BANISHED), card[0], card[1], 0);
    DestroyLinkedCards(player, zone, 1);
}

/*
 * A monster of `player` was destroyed in battle in monster zone `zone`; card is the copy of its zone in
 * gBattle.zones[player], and defender is the defending player.
 *  - Banisher of the Light face up on either field: banished instead (BanishBattleDestroyedCard).
 *  - Cockroach Knight: shown and put back on top of its owner's deck.
 *  - Otherwise the card is marked destroyedInBattle and queued for its owner's graveyard, the player's
 *    monsterSentToGraveThisTurn is set, the response event becomes RESPONSE_BATTLE_DESTROYED and the
 *    "destroyed in battle" triggers are queued (see the switch). Then the linked cards are destroyed.
 */
void SendBattleDestroyedCardToGraveyard(int defender, int player, int zone, u16 *card)
{
    /* Matching: a u16 first ID (ShowCardEffect sets r0 before r1) and a u32 ID for the dispatch. */
    u16 cardId;
    u32 dispatchId;
    u32 owner;
    u32 word;

    if (zone > ZONE_MONSTER_4)
        return;
    if (CountFaceUpMonstersByNumber(0, CARD_BANISHER_OF_THE_LIGHT) > 0
        || CountFaceUpMonstersByNumber(1, CARD_BANISHER_OF_THE_LIGHT) > 0) {
        BanishBattleDestroyedCard(player, zone, card);
        return;
    }
    /* The response event of the triggers queued below. */
    gChain.responseEvent = RESPONSE_BATTLE_DESTROYED;
    owner = ((struct DuelCard *)card)->owner;
    cardId = ((struct DuelCard *)card)->id;
    if (CARD_NUMBER(cardId) == CARD_COCKROACH_KNIGHT) {
        ShowCardEffect(player, cardId);
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_ADD_CARD_TO_DECK_TOP), card[0], card[1], 0);
        DestroyLinkedCards(player, zone, 1);
        return;
    }
    ((struct DuelCardStatusBytes *)card)->destroyedInBattle = 1;
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_ADD_CARD_TO_GRAVEYARD), card[0], card[1], 0);
    if (CARD_STATS_TYPE(CARD_STATS(((struct DuelCard *)card)->id)) <= CARD_TYPE_REPTILE)
        gDuelPlayers[player & 1].monsterSentToGraveThisTurn = 1;
    word = CARD_WORD(card);
    dispatchId = CARD_ID(word);
    switch (CARD_NUMBER(dispatchId)) {
    /* "When destroyed in battle" triggers. The second word holds the location of the destroyed monster
     * (the attacker belongs to the turn player, the target to the other one): low half the attacker's
     * DUEL_LOC, high half the target's, 0xFFFF for the other monster. */
    case CARD_SANGAN: case CARD_AXE_OF_DESPAIR: case CARD_BLACK_PENDANT: case CARD_HORN_OF_LIGHT:
    case CARD_HORN_OF_THE_UNICORN: case CARD_MALEVOLENT_NUZZLER: case CARD_WITCH_OF_THE_BLACK_FOREST:
    case CARD_THE_UNHAPPY_MAIDEN: case CARD_GIANT_RAT: case CARD_UFO_TURTLE:
    case CARD_GIANT_GERM: case CARD_NIMBLE_MOMONGA: case CARD_SHINING_FAIRY: case CARD_MOTHER_GRIZZLY:
    case CARD_FLYING_KAMAKIRI_1: case CARD_MYSTIC_TOMATO:
    case CARD_1241: case CARD_1242: case CARD_1257: case CARD_1405:
        {
            u32 trigger = TRIGGER_PLAYER(owner);
            u32 t, hi;
            trigger |= TRIGGER_EVENT(0x3F & gChain.responseEvent);
            /* FAKEMATCH: the ROM loads 0xFFFF between the two shifts of the re-read card ID, so the
             * extraction is split around it. Reusing `word` (the dispatch word) for the attacker location
             * makes its first set non-constant, so local-alloc does not double its live length and it
             * takes r3 before `trigger`. */
            t = CARD_WORD(card) << 20;
            word = 0xFFFF;
            t = (t >> 20) | TRIGGER_KIND(CHAIN_KIND_OFF_FIELD);
            trigger |= t;
            if (player == gDuel.turnPlayer)
                word = gDuel.turnPlayer | gBattle.atkSlot << 8;
            else
                hi = 0; /* FAKEMATCH: dead store; flow deletes it only after cse2, so CSE does not carry
                         * the turn bit past the join and the ROM's re-read of +0x1B12 is kept. */
            if (player == 1 - gDuel.turnPlayer)
                hi = ((u8)(1 - gDuel.turnPlayer) | gBattle.defSlot << 8) << 16;
            else
                hi = 0xFFFF0000;
            Chain_AddPending(trigger, hi | word);
        }
        break;
    case CARD_THE_IMMORTAL_OF_THUNDER:
        /* Its owner loses 5000 LP (the card is shown with cmd 0x73), unless effectUnused is set. */
        if (!ZONE(player, zone)->effectUnused) {
            DuelCmd_Push(PLAYER_CMD(owner, DUEL_CMD_SHOW_CARD_EFFECT), dispatchId, 1, 0);
            LoseLifePoints(owner, 5000);
        }
        break;
    case CARD_SPEAR_CRETIN:
        if (!ZONE(player, zone)->effectUnused) {
            u32 trigger = TRIGGER_PLAYER(((struct DuelCard *)card)->owner & 1)
                | TRIGGER_EVENT(0x3F & gChain.responseEvent);
            u32 t = dispatchId | TRIGGER_KIND(CHAIN_KIND_OFF_FIELD);
            Chain_AddPending(trigger | t, 0);
        }
        break;
    case CARD_1514:
        if (defender == player) {
            sub_080197C0Int(player, dispatchId);
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SET_DESTROYED_TRIGGER_PENDING), 1, 0, 0);
        }
        break;
    case CARD_DARK_EYES_ILLUSIONIST:
    case CARD_1332:
        QueueRemoveLinksToZone(player, zone);
        break;
    }
    LoseLpOnSendToGraveyard(player, 1);
    DestroyLinkedCards(player, zone, 1);
}

/* Banish the card in (player, zone) (DUEL_CMD_BANISH, or DUEL_CMD_BANISH_FLAGGED, which also sets card bit
 * 20, when flagged); nothing for an empty zone. A monster's linked cards are destroyed, a banished
 * Prohibition lifts its prohibition, and a face-up Field Magic resets the field background. */
void BanishFieldCard(int player, int zone, u16 flagged)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if (flagged != 0)
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH_FLAGGED), zone, 1, 0);
    else
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH), zone, 1, 0);
    if (zone <= ZONE_MONSTER_4)
        DestroyLinkedCards(player, zone, 1);
    if (CARD_NUMBER(id) == CARD_PROHIBITION)
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_REMOVE_PROHIBITION), zone, 0, 0);
    if (zone == ZONE_FIELD && ZONE(player, zone)->isFaceUp)
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SET_FIELD_BACKGROUND), 0, 0, 0);
}

/* Return the card in (player, zone) to its owner's hand (DUEL_CMD_RETURN_TO_HAND; Fusion monsters go to
 * the fusion deck); a token is destroyed instead. For a monster, the cards linked to it are destroyed and
 * the non-turn player gets a RESPONSE_MONSTER_TO_HAND window. A face-up Field Magic resets the field background, and
 * Call of the Haunted destroys the monster it revived unless it is disabled. */
void ReturnFieldCardToHand(int player, int zone, u16 cmdArg)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if (IS_TOKEN_NUMBER(CARD_NUMBER(id)))
        DestroyFieldCard(player, zone, 1);
    else
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_RETURN_TO_HAND), zone, cmdArg, 0);
    if (zone <= ZONE_MONSTER_4) {
        DestroyLinkedCards(player, zone, 0);
        EventResponse_Request(1 - gDuel.turnPlayer, RESPONSE_MONSTER_TO_HAND,
                              (u16)((u8)player | ((u8)zone << 8)));
    }
    if (zone == ZONE_FIELD && ZONE(player, zone)->isFaceUp)
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SET_FIELD_BACKGROUND), 0, 0, 0);
    if (CARD_NUMBER(id) == CARD_CALL_OF_THE_HAUNTED) {
        u32 loc = FindMonsterLinkedToCard(player, zone);
        u16 target = loc;
        if (target != 0xFFFF && !ZONE(player, zone)->isDisabled)
            DestroyFieldCard((u8)target, loc >> 8 & 0xFF, 1);
    }
}

/* Return the card in (player, zone) to the top of its owner's deck (DUEL_CMD_RETURN_TO_DECK; Fusion
 * monsters go to the fusion deck); a token is destroyed instead. As ReturnFieldCardToHand, without the
 * response window. */
void ReturnFieldCardToDeck(int player, int zone)
{
    u32 id = ZONE_CARD_ID(ZONE(player, zone));

    if (id == 0)
        return;
    if (IS_TOKEN_NUMBER(CARD_NUMBER(id)))
        DestroyFieldCard(player, zone, 1);
    else
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_RETURN_TO_DECK), zone, 0, 0);
    if (zone <= ZONE_MONSTER_4)
        DestroyLinkedCards(player, zone, 0);
    if (zone == ZONE_FIELD && ZONE(player, zone)->isFaceUp)
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SET_FIELD_BACKGROUND), 0, 0, 0);
    if (CARD_NUMBER(id) == CARD_CALL_OF_THE_HAUNTED) {
        u32 loc = FindMonsterLinkedToCard(player, zone);
        u16 target = loc;
        if (target != 0xFFFF && !ZONE(player, zone)->isDisabled)
            DestroyFieldCard((u8)target, loc >> 8 & 0xFF, 1);
    }
}

/* Return both players' monsters to the deck, the opponent's first (Morphing Jar #2). With shuffle, a side
 * that had monsters is shuffled (CountMonsters still counts them: the commands are deferred). */
void ReturnAllMonstersToDeck(int player, u16 shuffle)
{
    int pass;
    int p;
    int zone;

    for (pass = 0; pass <= 1; pass++) {
        if (pass == 0)
            p = 1 - player;
        else
            p = player;
        for (zone = 0; zone <= ZONE_MONSTER_4; zone++)
            ReturnFieldCardToDeck(p, zone);
        if (shuffle != 0 && CountMonsters(p) > 0)
            DuelCmd_Push(PLAYER_CMD(p, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
    }
}

/* Flip the card in (player, zone) (DUEL_CMD_FLIP_CARD); nothing for an empty zone. A face-down monster
 * turned face up gets ZONE_STATUS_NORMAL_SUMMONED (cmd 0x90, mask 2) and, with triggerFlip, its flip
 * effect is queued if it has one, may activate, and no card 1530 is on either field. A face-up monster
 * turned face down loses its linked cards. */
void FlipFieldCard(int player, int zone, u16 triggerFlip)
{
    int p = player & 1;
    int zoneBytes = zone * sizeof(struct DuelZone);
    struct DuelZone *z;
    u16 id;

    /* FAKEMATCH: an empty input constraint on the masked player before the zone address keeps the
     * ROM's register roles for the player and the narrowed card ID. */
    __asm__("" : : "r"(p));
    z = (struct DuelZone *)(zoneBytes + p * sizeof(struct DuelPlayer) + DUEL_ZONES_ADDR);
    id = ZONE_CARD_ID(z);

    if (id == 0)
        return;
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_FLIP_CARD), zone, 0, 0);
    if (zone <= ZONE_MONSTER_4) {
        if (!z->isFaceUp) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SET_ZONE_STATUS_FLAGS), zone, ZONE_STATUS_NORMAL_SUMMONED, 0);
            if (triggerFlip != 0 && HasFlipEffect(CARD_NUMBER(id), 0) != 0
                && CanActivateEffectOfCard(player, id, 0) != 0
                && CountActiveCardsOnField(0, CARD_1530) == 0 && CountActiveCardsOnField(1, CARD_1530) == 0) {
                /* The OR order of the trigger word matches only when built through two locals. */
                u32 trigger = TRIGGER_PLAYER(p);
                u32 zoneEvent = TRIGGER_ZONE(zone & 0x1F)
                    | (TRIGGER_EVENT(RESPONSE_FLIPPED) | TRIGGER_KIND(CHAIN_KIND_MONSTER));

                Chain_AddPending(trigger | zoneEvent | id, 0);
            }
        } else {
            DestroyLinkedCards(player, zone, 1);
        }
    }
}

/* Change the battle position of the monster in (player, zone) (DUEL_CMD_CHANGE_POSITION; with flipFaceUp
 * it is also turned face up); nothing outside the monster zones or for an empty zone. Defense to attack:
 * Crass Clown's trigger. Attack to defense: Dream Clown's, Tainted Wisdom's and Yado Karu's. Then, with
 * triggerFlip and flipFaceUp, a face-down monster with a flip effect queues it (unless card 1530 is on
 * either field). */
void ChangeBattlePosition(int player, int zone, u16 flipFaceUp, u16 triggerFlip)
{
    int one = 1;
    int p = player & one;
    int zoneBytes = zone * sizeof(struct DuelZone);
    s16 playerBytes = p * sizeof(struct DuelPlayer);
    struct DuelZone *z;
    u32 id;

    /* The masked player's offset (0 or 0xD64) fits in an s16, which gives the ROM's register priorities.
     * FAKEMATCH: the empty input constraint keeps `one` live, matching the lifetime the ROM gives it. */
    __asm__("" : : "r"(one));
    z = (struct DuelZone *)(zoneBytes + playerBytes + DUEL_ZONES_ADDR);
    id = ZONE_CARD_ID(z);

    if (zone > ZONE_MONSTER_4)
        return;
    if (id == 0)
        return;
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_CHANGE_POSITION), zone, flipFaceUp, 0);
    if (z->isDefense) {
        if (CARD_NUMBER(id) == CARD_CRASS_CLOWN && CanActivateEffectOfCardU32(player, id, 0) != 0) {
            u32 trigger = TRIGGER_PLAYER(player & 1);
            u32 zoneEvent = TRIGGER_ZONE(0x1F & zone)
                | (TRIGGER_EVENT(RESPONSE_POSITION_CHANGED) | TRIGGER_KIND(CHAIN_KIND_MONSTER));
            Chain_AddPending(trigger | zoneEvent | id, 0);
        }
    } else {
        switch (CARD_NUMBER(id)) {
        case CARD_DREAM_CLOWN:
        case CARD_TAINTED_WISDOM:
        case CARD_YADO_KARU:
            if (CanActivateEffectOfCardU32(player, id, 0) != 0) {
                u32 trigger = TRIGGER_PLAYER(player & 1);
                u32 zoneEvent = TRIGGER_ZONE(0x1F & zone)
                    | (TRIGGER_EVENT(RESPONSE_POSITION_CHANGED) | TRIGGER_KIND(CHAIN_KIND_MONSTER));
                Chain_AddPending(trigger | zoneEvent | id, 0);
            }
            break;
        }
    }
    if (triggerFlip == 0)
        return;
    if (flipFaceUp == 0)
        return;
    if (ZONE(player, zone)->isFaceUp)
        return;
    if (HasFlipEffect(CARD_NUMBER(id), 0) == 0)
        return;
    if (CountActiveCardsOnField(0, CARD_1530) != 0)
        return;
    if (CountActiveCardsOnField(1, CARD_1530) != 0)
        return;
    {
        u32 trigger = TRIGGER_PLAYER(player & 1);
        u32 zoneEvent = TRIGGER_ZONE(0x1F & zone)
            | (TRIGGER_EVENT(RESPONSE_POSITION_CHANGED) | TRIGGER_KIND(CHAIN_KIND_MONSTER));
        Chain_AddPending(trigger | zoneEvent | id, 0);
    }
}

/* Move the card at fromLoc to the empty zone toLoc (locations are player | zone << 8; player is unused):
 * DUEL_CMD_MOVE_TO_ZONE, for control changes. When the card changes sides with its one-shot effect unused:
 * Ameba deals 2000 damage to its new controller, Griggle gives its old controller 3000 LP, and the effect
 * is marked used at the destination (DUEL_CMD_SET_EFFECT_UNUSED, 0). The ID is re-read after the move
 * command, as the ROM does. */
void MoveFieldCard(int player, u16 fromLoc, u16 toLoc)
{
    int fromPlayer = (u8)fromLoc;
    int fromZone = fromLoc >> 8;
    int toPlayer = (u8)toLoc;
    int toZone = toLoc >> 8;
    u32 id;

    if (ZONE_CARD_ID(ZONE(fromPlayer, fromZone)) == 0)
        return;
    if (ZONE_CARD_ID(ZONE(toPlayer, toZone)) != 0)
        return;
    DuelCmd_Push(PLAYER_CMD(fromPlayer, DUEL_CMD_MOVE_TO_ZONE), fromLoc, toLoc, 0);
    if (fromPlayer == toPlayer)
        return;
    id = ZONE_CARD_ID(ZONE(fromPlayer, fromZone));
    switch (CARD_NUMBER(id)) {
    case CARD_AMEBA:
        if (ZONE(fromPlayer, fromZone)->effectUnused) {
            sub_080197C0(fromPlayer, id);
            LoseLifePoints(toPlayer, 2000);
            DuelCmd_Push(PLAYER_CMD(toPlayer, DUEL_CMD_SET_EFFECT_UNUSED), toZone, 0, 0);
            return;
        }
        break;
    case CARD_GRIGGLE:
        if (ZONE(fromPlayer, fromZone)->effectUnused) {
            sub_080197C0(fromPlayer, id);
            GainLifePoints(fromPlayer, 3000);
            DuelCmd_Push(PLAYER_CMD(toPlayer, DUEL_CMD_SET_EFFECT_UNUSED), toZone, 0, 0);
        }
        break;
    }
}

/* Swap the cards at loc1 and loc2 (player | zone << 8) when both zones hold one (DUEL_CMD_SWAP_ZONES),
 * then run the Ameba / Griggle control-change effects for the card of each location (without a same-side
 * check). */
void SwapFieldCards(int player, u16 loc1, u16 loc2)
{
    int player1 = (u8)loc1;
    int zone1 = loc1 >> 8;
    int player2 = (u8)loc2;
    int zone2 = loc2 >> 8;
    u16 id1 = ZONE_CARD_ID(ZONE(player1, zone1));
    u32 id2 = ZONE_CARD_ID(ZONE(player2, zone2));

    if (id1 == 0)
        return;
    if (id2 == 0)
        return;
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SWAP_ZONES), loc1, loc2, 0);
    switch (CARD_NUMBER(id1)) {
    case CARD_AMEBA:
        if (ZONE(player1, zone1)->effectUnused) {
            sub_080197C0(player1, ZONE_CARD_ID(ZONE(player1, zone1)));
            LoseLifePoints(player2, 2000);
            DuelCmd_Push(PLAYER_CMD(player2, DUEL_CMD_SET_EFFECT_UNUSED), zone2, 0, 0);
        }
        break;
    case CARD_GRIGGLE:
        if (ZONE(player1, zone1)->effectUnused) {
            sub_080197C0(player1, ZONE_CARD_ID(ZONE(player1, zone1)));
            GainLifePoints(player1, 3000);
            DuelCmd_Push(PLAYER_CMD(player2, DUEL_CMD_SET_EFFECT_UNUSED), zone2, 0, 0);
        }
        break;
    }
    switch (CARD_NUMBER(id2)) {
    case CARD_AMEBA:
        if (ZONE(player2, zone2)->effectUnused) {
            sub_080197C0(player2, ZONE_CARD_ID(ZONE(player2, zone2)));
            LoseLifePoints(player1, 2000);
            DuelCmd_Push(PLAYER_CMD(player1, DUEL_CMD_SET_EFFECT_UNUSED), zone1, 0, 0);
        }
        break;
    case CARD_GRIGGLE:
        if (ZONE(player2, zone2)->effectUnused) {
            sub_080197C0(player2, ZONE_CARD_ID(ZONE(player2, zone2)));
            GainLifePoints(player2, 3000);
            DuelCmd_Push(PLAYER_CMD(player1, DUEL_CMD_SET_EFFECT_UNUSED), zone1, 0, 0);
        }
        break;
    }
}

/* Return hand[handIdx] to the deck, on top if toTop (DUEL_CMD_RETURN_HAND_CARD_TO_DECK). */
void ReturnHandCardToDeck(int player, int handIdx, u16 toTop)
{
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_RETURN_HAND_CARD_TO_DECK), handIdx, toTop, 0);
}

/*
 * Discard hand[handIdx] to the graveyard (DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD); under Banisher of the
 * Light it is banished instead and nothing else happens. compactHand is the command's "compact the hand"
 * operand: pass 0 while discarding several cards by index, 1 with the last one. Then:
 *  - Minar discarded by an opponent's effect: the opponent loses 1000 LP;
 *  - Electric Snake discarded by an opponent's effect: the player draws 2 cards;
 *  - card 1242 (no EDS card) queues its RESPONSE_DISCARDED trigger;
 *  - each of the opponent's active Magic Thorns costs the player 500 LP.
 */
void DiscardHandCard(int player, int handIdx, u16 byOpponentEffect, u16 compactHand)
{
    u32 id;
    int thorns;
    u16 cardNo;

    if (CountFaceUpMonstersByNumber(0, CARD_BANISHER_OF_THE_LIGHT) > 0
        || CountFaceUpMonstersByNumber(1, CARD_BANISHER_OF_THE_LIGHT) > 0) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_BANISH_HAND_CARD), handIdx, compactHand, 0);
        return;
    }
    DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SEND_HAND_CARD_TO_GRAVEYARD), handIdx, compactHand, 0);
    {
        /* gDuelPlayers[player & 1].hand[handIdx], through gDuelHands (0x02019968 = gDuelPlayers[0].hand)
         * as an integer address. Matching: the masked player, then the index offset, then the player
         * offset, each in its own local. */
        int p = player & 1;
        int indexBytes = handIdx * sizeof(struct DuelCard);
        u32 offset = p * sizeof(struct DuelPlayer);
        u32 word;

        word = *(u32 *)(indexBytes + offset + 0x02019968);
        id = CARD_ID(word);
    }
    switch (CARD_NUMBER(id)) {
    case CARD_MINAR:
        if (byOpponentEffect != 0) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), id, 1, 0);
            LoseLifePoints(1 - player, 1000);
        }
        break;
    case CARD_ELECTRIC_SNAKE:
        if (byOpponentEffect != 0) {
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), id, 1, 0);
            DrawCards(player, 2);
        }
        break;
    case CARD_1242:
        {
            /* The zone field of this trigger holds the player. */
            u32 trigger = TRIGGER_PLAYER(player & 1);
            u32 zoneEvent = TRIGGER_ZONE(0x1F & player)
                | (TRIGGER_EVENT(RESPONSE_DISCARDED) | TRIGGER_KIND(CHAIN_KIND_OFF_FIELD));
            Chain_AddPending(trigger | zoneEvent | id, 0);
        }
        break;
    }
    /* Matching: the card number stays in one local, used for the count and the table index. */
    thorns = CountActiveCardsOnField(1 - player, cardNo = CARD_MAGIC_THORN);
    if (thorns > 0) {
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SHOW_CARD_EFFECT), CARD_NUMBER_TO_ID[cardNo], 1, 0);
        LoseLifePoints(player, thorns * 500);
    }
    LoseLpOnSendToGraveyard(player, 1);
}
