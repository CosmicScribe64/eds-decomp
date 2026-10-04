#include "global.h"
#include "card_data.h"              /* gCardIdToNumber, CARD_ID_MASK, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardAttribute */
#include "constants/duel.h"         /* enum ZoneLinkKind, DuelPhase, DuelZoneIndex */
#include "util.h"                   /* HalveRoundUp */

/*
 * Effective card stats (wiki/functions/card-stats-c.md).
 *
 * GetZoneCardStats computes what the card in a zone currently is: its card ID, effective type and
 * attribute, and its ATK and DEF after equips, zone links, its own effect, the auras of other monsters,
 * Field Magic, Aqua Chorus, Reverse Trap, Shield & Sword and the doubling/halving effects. The ATK/DEF
 * wrappers, the battle code and the AI read it. The two small functions before it find the face-up
 * monsters that a given card affects (Ring of Magnetism's attack redirection).
 */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit uses, with the header's names,
 * types and bitfield containers (unused bytes are padding), and defines duel.h's include guard. After H0,
 * replace the block (BEGIN to END) with #include "legacy/duel.h": that gives identical assembly (checked against
 * the staged header). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12 */
    u32 unk13:4;
    u32 planted:1;                  /* bit 17: Parasite Paracide shuffled into the other player's deck */
    u32 unk18:14;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04: gDuel.serial when the card was placed */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5: turns a face-up card has been active */
    u8 unk6_6:2;
    u8 unk7_0:5;
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet (Ameba, Griggle) */
    u8 unk7_6:2;
    u8 unk8[2];
    u16 links[32];                  /* +0x0A: DUEL_LOC of a card affecting this one, or a value / card ID */
    u16 linkKinds[32];              /* +0x4A: low byte enum ZoneLinkKind, high byte stack count / value */
    u16 numLinks;                   /* +0x8A: entries in links / linkKinds */
    u8 unk8C_0:5;
    u8 atkHalved:1;                 /* +0x8C bit 5: Riryoku halving until end of turn */
    u8 unk8C_6:2;
    u8 unk8D[3];
    u32 unk90_0:11;
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:1;
    u32 declaredValue:5;            /* +0x91 bits 5-9: value chosen when the card resolved (DNA Surgery type,
                                     * an ATK/DEF choice, an attribute) */
    u32 unk92_2:14;
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003 */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 unk5[0x28 - 0x5];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard graveyard[80];  /* +0x904 */
    u8 unkA44[0xD64 - 0xA44];
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 fieldBackground:4;           /* +0x1ACC bits 0-3 */
    u8 duelOver:1;                  /* +0x1ACC bit 4 */
    u8 unk1ACC_5:1;
    u8 magicNegated:1;              /* +0x1ACC bit 6: Imperial Order */
    u8 trapsNegated:1;              /* +0x1ACC bit 7: Jinzo / Royal Decree */
    u8 equipMagicNegated:1;         /* +0x1ACD bit 0 */
    u8 equipMagicNegatedThisTurn:1; /* +0x1ACD bit 1: Armored Glass */
    u8 fieldMagicNegatedThisTurn:1; /* +0x1ACD bit 2: World Suppression */
    u8 contMagicNegatedThisTurn:1;  /* +0x1ACD bit 3 */
    u8 contTrapNegatedThisTurn:1;   /* +0x1ACD bit 4 */
    u8 statChangesReversed:1;       /* +0x1ACD bit 5: Reverse Trap: stat modifiers are subtracted */
    u8 atkDefSwapped:1;             /* +0x1ACD bit 6: Shield & Sword */
    u8 unk1ACD_7:1;
    u8 unk1ACE[0x1B12 - 0x1ACE];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 unk1B12_5:3;
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

/* Effective stats of the card in a zone (GetZoneCardStats). */
struct ZoneCardStats {
    u16 id;                         /* +0x0: card ID */
    u8 type:5;                      /* +0x2 bits 0-4: effective enum CardType */
    u8 attribute:3;                 /* +0x2 bits 5-7: effective enum CardAttribute */
    u8 unk3;
    s32 atk;                        /* +0x4: effective ATK */
    s32 def;                        /* +0x8: effective DEF */
};

extern struct DuelState gDuel;
extern struct DuelPlayer gDuelPlayers[2];
extern struct DuelZonesPlayer gDuelZones[2];
extern struct DuelZone gDuelSpellTrapZones;     /* 0x020195F0 = gDuelPlayers[0].zones[ZONE_SPELL_0] */

int CountActiveCardsOnField(int player, u16 cardNo);
int CountFaceUpMonstersOfType(int player, u16 type);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int CountFaceUpMonstersByNumberInPosition(int player, u16 cardNo, u16 defense);
int CountMonsters(int player);
int CountMonstersFiltered(int player, u16 faceUpOnly, u16 attackPosOnly);
int CountSpellTrapsFiltered(int player, u16 faceUp, u16 faceDown, u16 includeField);
int CountAquaChorusBoosts(int player, int zone);
int GetFaceUpFieldMagicNumber(void);
int CountZoneLinksFromCard(int player, int zone, u16 cardNo);
/* ---- END duel.h stand-in ---- */

/* 0x080815A8: ATK/DEF change per enum CardType for Forest, Wasteland, Mountain, Sogen, Umi and Yami
 * (row = card number - CARD_FOREST). */
extern const s16 gFieldTypeBonuses[][24];
/* 0x080816C8: per enum CardAttribute, 1 for the attribute that Gaia Power, Umiiruka, Molten Destruction,
 * Rising Air Current, Luminous Spark and Mystic Plasma Zone boost (row = card number - CARD_GAIA_POWER). */
extern const s16 gFieldAttributeBonuses[][8];

/*
 * Addressing. The ROM forms most duel addresses by byte arithmetic, in a fixed operand order, and reads the
 * card fields of a zone or pile through a struct DuelCard pointer (one whole-word ldr; a member access such
 * as zone->card.id loads only the halfword holding the ID). Each macro below keeps one such form; array
 * indexing (gDuelZones[player].zones[zone]) generates different code.
 */

/* &gDuelZones[player].zones[zone]: base, then the zone term, then the player term. */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((u8 *)gDuelZones + (zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer)))
/* The card word at the start of a zone, read through a struct DuelCard pointer. */
#define ZONE_CARD(zonePtr) (*(struct DuelCard *)(zonePtr))
#define ZONE_CARD_ID(player, zone) (ZONE_CARD(ZONE_AT(player, zone)).id)
/* gDuelPlayers[player].graveyard[index]: gDuelPlayers, then the player term, then the pile offset. */
#define GRAVEYARD_CARD(player, index) \
    (((struct DuelCard *)((u8 *)gDuelPlayers + (player) * sizeof(struct DuelPlayer) \
                          + OFFSET_OF(struct DuelPlayer, graveyard)))[index])
/* The card in the player's Field Magic zone, from gDuel. */
#define FIELD_ZONE_CARD(player) \
    (*(struct DuelCard *)((u8 *)&gDuel + OFFSET_OF(struct DuelState, players[0].zones[ZONE_FIELD]) \
                          + (player) * sizeof(struct DuelPlayer)))
/* gDuelPlayers reached from the gDuelZones literal (Megamorph's first LP comparison). */
#define PLAYERS_VIA_ZONES ((struct DuelPlayer *)((u8 *)gDuelZones - OFFSET_OF(struct DuelPlayer, zones)))

/*
 * Flag groups read as whole bytes or halfwords. Matching: the ROM tests several flags with one load and
 * one mask (and tests bit 0 with and #1, where a 1-bit field gives lsl #31).
 */
/* Zone +0x06 as one byte: bit 0 isDefense, bit 1 isFaceUp. */
struct ZoneFlagsByte { u8 unk0[6]; u8 flags; u8 unk7; };
#define ZONE_FLAG_DEFENSE 0x01
/* The rule flags of gDuel (fieldBackground .. atkDefSwapped, +0x1ACC) as one u16. */
struct DuelRuleFlagsView { u8 unk0[0x1ACC]; u16 flags; };
#define DUEL_RULE_FLAGS (((struct DuelRuleFlagsView *)&gDuel)->flags)
#define RULE_MAGIC_NEGATED      0x0040  /* magicNegated: Imperial Order */
#define RULE_TRAPS_NEGATED      0x0080  /* trapsNegated: Jinzo, Royal Decree */
#define RULE_STATS_REVERSED     0x2000  /* statChangesReversed: Reverse Trap */
#define RULE_ATK_DEF_SWAPPED    0x4000  /* atkDefSwapped: Shield & Sword */
/* Its second byte (gDuel +0x1ACD), reached from the gDuelZones literal. */
#define DUEL_RULE_FLAGS_HI (*((u8 *)gDuelZones - OFFSET_OF(struct DuelState, players[0].zones) + 0x1ACD))
#define RULE_HI_EQUIP_MAGIC_NEGATED 0x03    /* equipMagicNegated | equipMagicNegatedThisTurn (Armored Glass) */

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: the ROM reloads the table address at each use. The inline functions keep a u16 card-ID boundary,
 * so the repeated ID and table loads are not folded into one expression.
 */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
static inline int GetCardType(u16 id) { return CARD_STATS_TYPE(CARD_STATS(id)); }
#define CARD_TYPE(id) GetCardType(id)
static inline u16 GetCardNumber(int id) { return ((const u16 *)0x08622AB4)[id & CARD_ID_MASK]; }
#define CARD_NUMBER(id) GetCardNumber(id)
/* The equip switch reads the number table through its symbol, so one register holds the table address
 * across the case bodies; elsewhere the ROM reloads a constant address. */
static inline u16 GetCardNumberSym(int id) { return *(gCardIdToNumber + (id & CARD_ID_MASK)); }
/* Matching: the u16 return keeps the ROM's extra 5-bit mask on the value. */
static inline u16 GetDeclaredValue(struct DuelZone *zone) { return zone->declaredValue; }
/* FAKEMATCH: the ROM reads the whole stats word here; ordinary non-volatile expressions narrow the load to
 * the top byte. volatile keeps the word read; the original qualifier is unknown. */
static inline int GetCardAttribute(u16 id)
{
    return CARD_STATS_ATTR(((const volatile u32 *)0x08621DE0)[id & CARD_ID_MASK]);
}

/* Printed ATK of a card: 0 for Trap, Magic and Ticket cards, 4000 for the Egyptian Gods. */
static inline int BaseAttack(u16 id)
{
    switch (CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_ATK(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE;
    }
}

/* Printed DEF of a card: 0 for Trap, Magic and Ticket cards, 4000 for the Egyptian Gods. */
static inline int BaseDefense(u16 id)
{
    switch (CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_DEF(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE;
    }
}

/*
 * Number of the player's occupied, face-up monster zones that have a link from card number cardNo
 * (CountZoneLinksFromCard). The attack code uses it for Ring of Magnetism: with exactly one such monster,
 * attacks are redirected to it.
 */
int CountMonstersAffectedByCard(u32 player, u16 cardNo)
{
    int count = 0;
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        if (ZONE_CARD_ID(player & 1, zone)
            && ZONE_AT(player & 1, zone)->isFaceUp
            && CountZoneLinksFromCard(player, zone, cardNo))
            count++;
    }
    return count;
}

/* First occupied, face-up monster zone of the player with a link from card number cardNo, or -1. */
int FindMonsterAffectedByCard(u32 player, u16 cardNo)
{
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        if (ZONE_CARD_ID(player & 1, zone)
            && ZONE_AT(player & 1, zone)->isFaceUp
            && CountZoneLinksFromCard(player, zone, cardNo))
            return zone;
    }
    return -1;
}

/*
 * Fill *out with the card ID, effective type, attribute, ATK and DEF of the card in (player, zone).
 * An empty zone gives all 0; spell/trap zones and face-down monsters get the printed values. A face-up
 * monster's stats are built in this order:
 *  1. Printed stats; its own one-shot effects (Karate Man, card 1254) and Magic immunity (cards 1326/1329
 *     while Umi is the face-up Field Magic).
 *  2. Type overrides: a planted Parasite Paracide on the monster's side makes it an Insect, a face-up DNA
 *     Surgery on either side gives the declared type; the most recently placed card wins.
 *  3. The zone's links (enum ZoneLinkKind): resolved card effects, continuous effects, equips, the monster
 *     absorbed by Relinquished, and value links.
 *  4. The monster's own effect, Plant support, and the attribute auras of both fields.
 *  5. Field Magic of both players, Chorus of Sanctuary, card 1515 and Aqua Chorus.
 *  6. The modifiers are added (subtracted while Reverse Trap is active), the stats are clamped at 0, the
 *     net doublings/halvings are applied (a halving rounds up), and Shield & Sword swaps ATK and DEF.
 * Three modifier sums are kept: equipAtk/equipDef (equips and the equip-like value links), otherAtk/otherDef
 * (resolved card effects, ZONE_LINK_CARD_EFFECT) and addAtk/addDef (everything else).
 */
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out)
{
    int i, p;
    int atkHalves = 0, atkDoubles = 0, defHalves = 0, defDoubles = 0;
    int equipAtk = 0, equipDef = 0, equipCount = 0;
    int otherAtk = 0, otherDef = 0, addAtk = 0, addDef = 0;
    /* Matching: the zone pointer is built in these steps, so its offsets are shared with the card ID read. */
    int playerIndex = player & 1;
    u32 playerOffset = playerIndex * sizeof(struct DuelPlayer);
    u8 *playerZones = (u8 *)gDuelZones + playerOffset;
    u32 zoneOffset = zone * sizeof(struct DuelZone);
    struct DuelZone *thisZone = (struct DuelZone *)(playerZones + zoneOffset);
    u16 newestSerial = 0, immuneToMagic = 0;

    out->type = 0;
    out->attribute = 0;
    out->atk = 0;
    out->def = 0;
    out->id = ZONE_CARD_ID(player & 1, zone);
    if (!out->id)
        return;
    /* Printed type and attribute. Matching: the ROM writes the packed byte +2 twice (type, then type |
     * attribute << 5); bitfield stores to out->type and out->attribute generate other code. */
    {
        int type = CARD_TYPE(out->id);

        type &= 31;
        ((u8 *)out)[2] = type;
        {
            int attribute = GetCardAttribute(out->id) << 5;

            ((u8 *)out)[2] = (type & 31) | attribute;
        }
    }
    out->atk = BaseAttack(out->id);
    out->def = BaseDefense(out->id);
    /* FAKEMATCH: forces the card ID to be reloaded from *out after the printed stats are read. */
    asm("" : "+m"(out->id));
    if (zone > ZONE_MONSTER_4 || !ZONE_AT(player & 1, zone)->isFaceUp)
        return;

    /* 1. Karate Man's doubled ATK (and card 1254's +500 ATK/DEF) while effectUnused is clear, i.e. after its
     *    effect was used, and the Magic immunity of cards 1326/1329 under Umi (1254, 1326, 1329: no EDS cards). */
    switch (CARD_NUMBER(out->id)) {
    case CARD_KARATE_MAN:
        if (!ZONE_AT(player & 1, zone)->effectUnused)
            out->atk *= 2;
        break;
    case CARD_1254:
        if (!ZONE_AT(player & 1, zone)->effectUnused) {
            addAtk += 500;
            addDef += 500;
        }
        break;
    case CARD_1326:
    case 1329:
        if (GetFaceUpFieldMagicNumber() == CARD_UMI)
            immuneToMagic = 1;
        break;
    }

    /* 2. Type overrides: the most recently placed Parasite Paracide or DNA Surgery wins. */
    for (i = 0; i <= 4; i++) {
        /* Matching: the offsets are summed before gDuelZones is added (ZONE_AT adds them to the base). */
        struct DuelZone *monster = (struct DuelZone *)(i * sizeof(struct DuelZone)
                                                       + (player & 1) * sizeof(struct DuelPlayer) + (u32)gDuelZones);

        /* a face-up planted Parasite Paracide (summoned when drawn from this player's deck) among this
         * player's monsters: Insect */
        if (ZONE_CARD(monster).id
            && CARD_NUMBER(ZONE_CARD(monster).id) == CARD_PARASITE_PARACIDE
            && ZONE_CARD(monster).planted && monster->isFaceUp
            && monster->serial > (u32)newestSerial) {
            newestSerial = monster->serial;
            out->type = CARD_TYPE_INSECT;
        }
        /* a face-up, enabled DNA Surgery in spell/trap zone i on either side: the declared type */
        for (p = 0; p <= 1; p++) {
            /* Matching: from the gDuelSpellTrapZones literal (= gDuelZones + ZONE_SPELL_0 zones). */
            struct DuelZone *spell = (struct DuelZone *)((p & 1) * sizeof(struct DuelPlayer) + i * sizeof(struct DuelZone)
                                                         + (u32)&gDuelSpellTrapZones);

            if (ZONE_CARD(spell).id
                && CARD_NUMBER(ZONE_CARD(spell).id) == CARD_DNA_SURGERY
                && spell->isFaceUp && !spell->isDisabled
                && spell->serial > (u32)newestSerial) {
                newestSerial = spell->serial;
                out->type = GetDeclaredValue(spell);
            }
        }
    }

    /* 3. The zone's links. value = high byte of linkKinds[i]; links[i] is a DUEL_LOC for the zone kinds. */
    for (i = 0; i < ZONE_AT(player & 1, zone)->numLinks; i++) {
        u16 link = ZONE_AT(player & 1, zone)->links[i];
        u8 kind = ZONE_AT(player & 1, zone)->linkKinds[i];
        int value = ZONE_AT(player & 1, zone)->linkKinds[i] >> 8;
        u8 sourcePlayer = link;
        int sourceZone = link >> 8;
        u16 sourceId = ZONE_CARD_ID(sourcePlayer & 1, sourceZone);

        switch (kind) {
        case ZONE_LINK_CARD_EFFECT:
            /* link is the card ID of a resolved card effect; Magic cards do not affect an immune monster */
            if (immuneToMagic && CARD_TYPE(link) == CARD_TYPE_MAGIC)
                break;
            switch (CARD_NUMBER(link)) {
            case CARD_THE_LITTLE_SWORDSMAN_OF_AILE:
                otherAtk += (value + 1) * 700;
                break;
            case CARD_GODDESS_OF_WHIM:
                /* the coin toss: value 1 doubles ATK, 0 halves it. Matching: this case and the dice re-read
                 * the value from the zone instead of using value. */
                if (ZONE_AT(player & 1, zone)->linkKinds[i] >> 8)
                    atkDoubles++;
                else
                    atkHalves++;
                break;
            case CARD_REINFORCEMENTS: otherAtk += (value + 1) * 500; break;
            case CARD_CASTLE_WALLS: otherDef += (value + 1) * 500; break;
            case CARD_RUSH_RECKLESSLY: otherAtk += (value + 1) * 700; break;
            case CARD_THE_RELIABLE_GUARDIAN: otherDef += (value + 1) * 700; break;
            case CARD_SNAKE_FANG: otherDef -= (value + 1) * 500; break;
            case CARD_GRACEFUL_DICE:
                /* value: the die roll */
                otherAtk += (ZONE_AT(player & 1, zone)->linkKinds[i] >> 8) * 100;
                otherDef += (ZONE_AT(player & 1, zone)->linkKinds[i] >> 8) * 100;
                break;
            case CARD_SKULL_DICE:
                otherAtk -= (ZONE_AT(player & 1, zone)->linkKinds[i] >> 8) * 100;
                otherDef -= (ZONE_AT(player & 1, zone)->linkKinds[i] >> 8) * 100;
                break;
            case 1231: otherAtk -= (value + 1) * 500; break;    /* card number 1231: no EDS card */
            case CARD_1314: atkDoubles++; break;
            case CARD_1415: otherAtk -= (value + 1) * 700; break;
            case CARD_1534:
                /* +100 ATK per monster in the graveyard. Quirk (ROM): the scan reuses the link-loop index,
                 * so the link loop goes on from graveCount + 1. */
                for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++)
                    if ((u32)CARD_TYPE(GRAVEYARD_CARD(player & 1, i).id) <= CARD_TYPE_REPTILE)
                        otherAtk += 100;
                break;
            }
            break;
        case ZONE_LINK_CONTINUOUS:
            /* the continuous effect of the card in the source zone, unless that card is disabled */
            if (!sourceId || ZONE_AT(sourcePlayer & 1, sourceZone)->isDisabled)
                break;
            switch (CARD_NUMBER(sourceId)) {
            case CARD_CASTLE_OF_DARK_ILLUSIONS:
                /* +200 ATK/DEF per step (value + 1). Matching: the value is re-read from the zone. */
                addAtk += ((ZONE_AT(player & 1, zone)->linkKinds[i] >> 8) + 1) * 200;
                addDef += ((ZONE_AT(player & 1, zone)->linkKinds[i] >> 8) + 1) * 200;
                break;
            case CARD_1244: addAtk -= 700; break;
            case CARD_MIRROR_WALL:
                /* halves ATK when the Mirror Wall belongs to the other player */
                if (sourcePlayer != player)
                    atkHalves++;
                break;
            }
            break;
        case ZONE_LINK_EQUIP: {
            /*
             * The equip card in the source zone. Its effect is skipped while it is disabled, while card 1537
             * (negates Equip Magic; no EDS card) is active on either field, while gDuel +0x1ACD bit 0 or 1 is set
             * (equipMagicNegated, equipMagicNegatedThisTurn: Armored Glass) or while the monster is immune
             * to Magic; it still counts for equipCount.
             */
            if (!sourceId)
                break;
            if (!ZONE_AT(sourcePlayer & 1, sourceZone)->isDisabled && !CountActiveCardsOnField(0, CARD_1537)
                && !CountActiveCardsOnField(1, CARD_1537) && !(DUEL_RULE_FLAGS_HI & RULE_HI_EQUIP_MAGIC_NEGATED)
                && (!immuneToMagic || CARD_TYPE(sourceId) != CARD_TYPE_MAGIC)) {
                switch (GetCardNumberSym(sourceId)) {
                case CARD_COCOON_OF_EVOLUTION:
                    /* Petit Moth in the Cocoon */
                    if (GetCardNumberSym(out->id) == CARD_PETIT_MOTH) {
                        out->atk = 0;
                        out->def = 2000;
                    }
                    break;
                case CARD_LEGENDARY_SWORD:
                    if (out->type == CARD_TYPE_WARRIOR) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_SWORD_OF_DARK_DESTRUCTION:
                    if (out->attribute == ATTRIBUTE_DARK) {
                        equipAtk += 400;
                        equipDef -= 200;
                    }
                    break;
                case CARD_DARK_ENERGY:
                    if (out->type == CARD_TYPE_FIEND) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_AXE_OF_DESPAIR: equipAtk += 1000; break;
                case CARD_LASER_CANNON_ARMOR:
                    if (out->type == CARD_TYPE_INSECT) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_INSECT_ARMOR_WITH_LASER_CANNON:
                    if (out->type == CARD_TYPE_INSECT)
                        equipAtk += 700;
                    break;
                case CARD_ELFS_LIGHT:
                    if (out->attribute == ATTRIBUTE_LIGHT) {
                        equipAtk += 400;
                        equipDef -= 200;
                    }
                    break;
                case CARD_BEAST_FANGS:
                    if (out->type == CARD_TYPE_BEAST) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_STEEL_SHELL:
                    if (out->attribute == ATTRIBUTE_WATER) {
                        equipAtk += 400;
                        equipDef -= 200;
                    }
                    break;
                case CARD_VILE_GERMS:
                    if (out->type == CARD_TYPE_PLANT) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_BLACK_PENDANT: equipAtk += 500; break;
                case CARD_SILVER_BOW_AND_ARROW:
                    if (out->type == CARD_TYPE_FAIRY) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_HORN_OF_LIGHT: equipDef += 800; break;
                case CARD_HORN_OF_THE_UNICORN:
                    equipAtk += 700;
                    equipDef += 700;
                    break;
                case CARD_DRAGON_TREASURE:
                    if (out->type == CARD_TYPE_DRAGON) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_ELECTRO_WHIP:
                    if (out->type == CARD_TYPE_THUNDER) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_CYBER_SHIELD:
                    /* Harpie Lady or Harpie Lady Sisters (number - CARD_HARPIE_LADY <= 1), or card number 1249
                     * (no EDS card) */
                    if ((u16)(GetCardNumberSym(out->id) - CARD_HARPIE_LADY) <= 1 || GetCardNumberSym(out->id) == 1249)
                        equipAtk += 500;
                    break;
                case CARD_MYSTICAL_MOON:
                    if (out->type == CARD_TYPE_BEAST_WARRIOR) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_MALEVOLENT_NUZZLER: equipAtk += 700; break;
                case CARD_VIOLET_CRYSTAL:
                    if (out->type == CARD_TYPE_ZOMBIE) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_BOOK_OF_SECRET_ARTS:
                    if (out->type == CARD_TYPE_SPELLCASTER) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_INVIGORATION:
                    if (out->attribute == ATTRIBUTE_EARTH) {
                        equipAtk += 400;
                        equipDef -= 200;
                    }
                    break;
                case CARD_MACHINE_CONVERSION_FACTORY:
                    if (out->type == CARD_TYPE_MACHINE) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_RAISE_BODY_HEAT:
                    if (out->type == CARD_TYPE_DINOSAUR) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_FOLLOW_WIND:
                    if (out->type == CARD_TYPE_WINGED_BEAST) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_POWER_OF_KAISHIN:
                    if (out->type == CARD_TYPE_AQUA) {
                        equipAtk += 300;
                        equipDef += 300;
                    }
                    break;
                case CARD_KUNAI_WITH_CHAIN: equipAtk += 500; break;
                case CARD_SALAMANDRA:
                    if (out->attribute == ATTRIBUTE_FIRE)
                        equipAtk += 700;
                    break;
                case CARD_MEGAMORPH:
                    /*
                     * Printed ATK doubled while the equipping player has less LP than the opponent, halved
                     * while more. Quirk (ROM): it also clears addAtk and otherAtk collected so far.
                     */
                    if (PLAYERS_VIA_ZONES[sourcePlayer & 1].lifePoints < PLAYERS_VIA_ZONES[(1 - sourcePlayer) & 1].lifePoints)
                        out->atk = BaseAttack(out->id) * 2;
                    if (gDuelPlayers[sourcePlayer & 1].lifePoints > gDuelPlayers[(1 - sourcePlayer) & 1].lifePoints)
                        out->atk = HalveRoundUp(BaseAttack(out->id));
                    addAtk = 0;
                    otherAtk = 0;
                    break;
                case CARD_METALMORPH:
                    equipAtk += 300;
                    equipDef += 300;
                    break;
                case CARD_BRIGHT_CASTLE:
                    if (out->attribute == ATTRIBUTE_LIGHT)
                        equipAtk += 700;
                    break;
                case CARD_7_COMPLETED:
                    /* the declared value picks ATK (1) or DEF (2) */
                    if (out->type == CARD_TYPE_MACHINE) {
                        switch (ZONE_AT(sourcePlayer & 1, sourceZone)->declaredValue) {
                        case 1: equipAtk += 700; break;
                        case 2: equipDef += 700; break;
                        }
                    }
                    break;
                case CARD_BURNING_SPEAR:
                    if (out->attribute == ATTRIBUTE_FIRE) {
                        equipAtk += 400;
                        equipDef -= 200;
                    }
                    break;
                case CARD_GUST_FAN:
                    if (out->attribute == ATTRIBUTE_WIND) {
                        equipAtk += 400;
                        equipDef -= 200;
                    }
                    break;
                case CARD_SWORD_OF_DEEP_SEATED:
                    equipAtk += 500;
                    equipDef += 500;
                    break;
                case CARD_GERM_INFECTION:
                    /* -300 ATK per turn on the field; Machines are not affected */
                    if (out->type != CARD_TYPE_MACHINE)
                        equipAtk -= ZONE_AT(sourcePlayer & 1, sourceZone)->turnCounter * 300;
                    break;
                case CARD_RING_OF_MAGNETISM:
                    equipAtk -= 500;
                    equipDef -= 500;
                    break;
                case CARD_STIM_PACK:
                    /* +700 ATK, then -200 per turn on the field */
                    equipAtk += 700 - ZONE_AT(sourcePlayer & 1, sourceZone)->turnCounter * 200;
                    break;
                case CARD_SWORD_OF_DRAGONS_SOUL:
                    if (out->type == CARD_TYPE_WARRIOR)
                        equipAtk += 700;
                    break;
                case CARD_1242: equipAtk -= 500; break;
                case CARD_1420:
                    equipAtk += 1000;
                    equipDef -= 1000;
                    break;
                case CARD_1422:
                    if (out->type == CARD_TYPE_WARRIOR)
                        equipAtk += 800;
                    break;
                case CARD_1448:
                    /* the declared attribute */
                    out->attribute = ZONE_AT(sourcePlayer & 1, sourceZone)->declaredValue;
                    break;
                case CARD_1449:
                    /* +800 ATK/DEF per face-up monster of the equipping player */
                    equipAtk += CountMonstersFiltered(sourcePlayer, 1, 0) * 800;
                    equipDef += CountMonstersFiltered(sourcePlayer, 1, 0) * 800;
                    break;
                case CARD_1450:
                    /* +500 ATK/DEF per spell/trap card of the equipping player */
                    equipAtk += CountSpellTrapsFiltered(sourcePlayer, 0, 0, 0) * 500;
                    equipDef += CountSpellTrapsFiltered(sourcePlayer, 0, 0, 0) * 500;
                    break;
                case CARD_1540:
                    if (GetCardNumberSym(out->id) == 1339)
                        equipAtk += 300;
                    break;
                case CARD_1550:
                    /* a Warrior becomes a Dragon with +500 ATK/DEF; the type also wins over an older type override */
                    if (out->type == CARD_TYPE_WARRIOR) {
                        out->type = CARD_TYPE_DRAGON;
                        equipAtk += 500;
                        equipDef += 500;
                    }
                    /* Matching: sourcePlayer % 2, not & 1: the byte-wide AND would share its constant 1 with
                     * the store below. */
                    if (ZONE_AT(sourcePlayer % 2, sourceZone)->serial > (u32)newestSerial)
                        out->type = CARD_TYPE_DRAGON;
                    break;
                }
            }
            if (sourceId)
                equipCount++;
            break;
        }
        case ZONE_LINK_STATS_UP_100:
            addAtk += value * 100;
            addDef += value * 100;
            break;
        case ZONE_LINK_ABSORBED:
            /* Relinquished (or card 1334) takes the printed ATK/DEF of the monster it absorbed, 0/0 while that
             * monster is face down */
            if (CARD_NUMBER(out->id) == CARD_RELINQUISHED || CARD_NUMBER(out->id) == CARD_1334) {
                out->atk = 0;
                out->def = 0;
                if (ZONE_AT(sourcePlayer & 1, sourceZone)->isFaceUp) {
                    out->atk = BaseAttack(sourceId);
                    out->def = BaseDefense(sourceId);
                }
            }
            break;
        case ZONE_LINK_ATK_BONUS:
            /* link is the ATK bonus itself */
            if (!immuneToMagic)
                out->atk += link;
            break;
        case ZONE_LINK_ADD_CARD_STATS:
            /* link is a card ID whose printed ATK/DEF are added.
             * Matching: the sums go through a temporary: printed stat + total, in that order. */
            {
                int sum = BaseAttack(link) + addAtk;
                addAtk = sum;
            }
            {
                int sum = BaseDefense(link) + addDef;
                addDef = sum;
            }
            break;
        case ZONE_LINK_STATS_DOWN_500:
            addAtk -= (value + 1) * 500;
            addDef -= (value + 1) * 500;
            break;
        case ZONE_LINK_EQUIP_ATK_200: equipAtk += 200; break;
        case ZONE_LINK_ATK_300_PER_VALUE: equipAtk += value * 300; break;
        case ZONE_LINK_ATK_DOWN_200: addAtk -= value * 200; break;
        }
    }

    /* 4a. The monster's own effect. */
    switch (CARD_NUMBER(out->id)) {
    case CARD_SWAMP_BATTLEGUARD:
        addAtk += CountActiveCardsOnField(player, CARD_LAVA_BATTLEGUARD) * 500;
        break;
    case CARD_SHADOW_GHOUL:
        /* +100 ATK per monster in its controller's graveyard */
        for (i = 0; i < gDuelPlayers[player & 1].graveCount; i++)
            if ((u32)CARD_TYPE(GRAVEYARD_CARD(player & 1, i).id) <= CARD_TYPE_REPTILE)
                addAtk += 100;
        break;
    case CARD_WODAN_THE_RESIDENT_OF_THE_FOREST:
        addAtk += CountFaceUpMonstersOfType(0, CARD_TYPE_PLANT) * 100;
        addAtk += CountFaceUpMonstersOfType(1, CARD_TYPE_PLANT) * 100;
        break;
    case CARD_HARPIES_PET_DRAGON: {
        /* +300 ATK/DEF per Harpie Lady (or card number 1249) on either field */
        int harpies0 = CountActiveCardsOnField(0, CARD_HARPIE_LADY), harpies1 = CountActiveCardsOnField(1, CARD_HARPIE_LADY);
        int others0 = CountActiveCardsOnField(0, 1249), others1 = CountActiveCardsOnField(1, 1249);

        addAtk += (harpies0 + harpies1 + others0 + others1) * 300;
        addDef += (harpies0 + harpies1 + others0 + others1) * 300;
        break;
    }
    case CARD_MACHINE_KING:
        addAtk += CountFaceUpMonstersOfType(0, CARD_TYPE_MACHINE) * 100;
        addAtk += CountFaceUpMonstersOfType(1, CARD_TYPE_MACHINE) * 100;
        break;
    case CARD_MAHA_VAILO:
        addAtk += equipCount * 500;
        break;
    case CARD_MUKA_MUKA:
        addAtk += gDuelPlayers[player & 1].handCount * 300;
        addDef += gDuelPlayers[player & 1].handCount * 300;
        break;
    case CARD_LAVA_BATTLEGUARD:
        addAtk += CountActiveCardsOnField(player, CARD_SWAMP_BATTLEGUARD) * 500;
        break;
    case CARD_HOURGLASS_OF_COURAGE:
        /* halved while its turnCounter is 0 or 1, doubled afterwards */
        if (ZONE_AT(player & 1, zone)->turnCounter <= 1) {
            atkHalves++;
            defHalves++;
        } else {
            atkDoubles++;
            defDoubles++;
        }
        break;
    case CARD_DARK_MAGICIAN_GIRL:
        /* +300 ATK per Dark Magician or Magician of Black Chaos in both graveyards */
        for (i = 0; i <= 1; i++)
            for (p = 0; p < gDuelPlayers[i & 1].graveCount; p++) {
                int cardNo = CARD_NUMBER(GRAVEYARD_CARD(i & 1, p).id);

                switch (cardNo) {
                case CARD_DARK_MAGICIAN:
                case CARD_MAGICIAN_OF_BLACK_CHAOS:
                case CARD_1210:
                case CARD_DARK_MAGICIAN_ALT:
                    addAtk += 300;
                    break;
                }
            }
        break;
    case CARD_INSECT_QUEEN:
        addAtk += CountFaceUpMonstersOfType(0, CARD_TYPE_INSECT) * 200;
        addAtk += CountFaceUpMonstersOfType(1, CARD_TYPE_INSECT) * 200;
        break;
    case CARD_BUSTER_BLADER:
        /* +500 ATK per Dragon in the opponent's graveyard and on the opponent's field */
        for (i = 0; i < gDuelPlayers[(1 - player) & 1].graveCount; i++)
            if (CARD_TYPE(GRAVEYARD_CARD((1 - player) & 1, i).id) == CARD_TYPE_DRAGON)
                addAtk += 500;
        addAtk += CountFaceUpMonstersOfType(1 - player, CARD_TYPE_DRAGON) * 500;
        break;
    case CARD_FLASH_ASSAILANT:
        addAtk -= gDuelPlayers[player & 1].handCount * 400;
        addDef -= gDuelPlayers[player & 1].handCount * 400;
        break;
    case CARD_BOAR_SOLDIER:
        /* -1000 ATK while the opponent has a monster */
        if (CountMonsters(1 - player) > 0)
            addAtk -= 1000;
        break;
    case 1413:
        /* card number 1413 (no EDS card): -200 ATK per opponent's monster */
        addAtk -= CountMonsters(1 - player) * 200;
        break;
    case 1516:
        /* card number 1516 (no EDS card): +300 ATK during its controller's Battle Phase */
        if (player == gDuel.turnPlayer && gDuel.phase == PHASE_BATTLE)
            addAtk += 300;
        break;
    case 1518:
        /* card number 1518 (no EDS card): +300 ATK during the opponent's Battle Phase */
        if (player != gDuel.turnPlayer && gDuel.phase == PHASE_BATTLE)
            addAtk += 300;
        break;
    }

    /* 4b. Plant support: +500 ATK/DEF per face-up defense-position card 1252 (no EDS card) of the player. */
    switch (out->type) {
    case CARD_TYPE_PLANT:
        addAtk += CountFaceUpMonstersByNumberInPosition(player, 1252, 1) * 500;
        addDef += CountFaceUpMonstersByNumberInPosition(player, 1252, 1) * 500;
        break;
    }

    /* 4c. Attribute auras of the monsters on both fields (ATK only): +500 for the boosted attribute, -400
     * for the opposite one. */
    switch (out->attribute) {
    case ATTRIBUTE_LIGHT:
        addAtk += CountActiveCardsOnField(0, CARD_HOSHININGEN) * 500;
        addAtk += CountActiveCardsOnField(1, CARD_HOSHININGEN) * 500;
        addAtk -= CountActiveCardsOnField(0, CARD_WITCHS_APPRENTICE) * 400;
        addAtk -= CountActiveCardsOnField(1, CARD_WITCHS_APPRENTICE) * 400;
        break;
    case ATTRIBUTE_DARK:
        addAtk -= CountActiveCardsOnField(0, CARD_HOSHININGEN) * 400;
        addAtk -= CountActiveCardsOnField(1, CARD_HOSHININGEN) * 400;
        addAtk += CountActiveCardsOnField(0, CARD_WITCHS_APPRENTICE) * 500;
        addAtk += CountActiveCardsOnField(1, CARD_WITCHS_APPRENTICE) * 500;
        break;
    case ATTRIBUTE_WATER:
        addAtk += CountActiveCardsOnField(0, CARD_STAR_BOY) * 500;
        addAtk += CountActiveCardsOnField(1, CARD_STAR_BOY) * 500;
        addAtk -= CountActiveCardsOnField(0, CARD_LITTLE_CHIMERA) * 400;
        addAtk -= CountActiveCardsOnField(1, CARD_LITTLE_CHIMERA) * 400;
        addAtk -= CountActiveCardsOnField(0, CARD_1422) * 500;
        addAtk -= CountActiveCardsOnField(1, CARD_1422) * 500;
        break;
    case ATTRIBUTE_FIRE:
        addAtk -= CountActiveCardsOnField(0, CARD_STAR_BOY) * 400;
        addAtk -= CountActiveCardsOnField(1, CARD_STAR_BOY) * 400;
        addAtk += CountActiveCardsOnField(0, CARD_LITTLE_CHIMERA) * 500;
        addAtk += CountActiveCardsOnField(1, CARD_LITTLE_CHIMERA) * 500;
        break;
    case ATTRIBUTE_EARTH:
        addAtk += CountActiveCardsOnField(0, CARD_MILUS_RADIANT) * 500;
        addAtk += CountActiveCardsOnField(1, CARD_MILUS_RADIANT) * 500;
        addAtk -= CountActiveCardsOnField(0, CARD_BLADEFLY) * 400;
        addAtk -= CountActiveCardsOnField(1, CARD_BLADEFLY) * 400;
        break;
    case ATTRIBUTE_WIND:
        addAtk -= CountActiveCardsOnField(0, CARD_MILUS_RADIANT) * 400;
        addAtk -= CountActiveCardsOnField(1, CARD_MILUS_RADIANT) * 400;
        addAtk += CountActiveCardsOnField(0, CARD_BLADEFLY) * 500;
        addAtk += CountActiveCardsOnField(1, CARD_BLADEFLY) * 500;
        break;
    }

    /* 5. The Field Magic of both players, unless gDuel.fieldMagicNegatedThisTurn (+0x1ACD bit 2) is set or
     * the monster is immune to Magic. */
    if (!gDuel.fieldMagicNegatedThisTurn && !immuneToMagic) {
        for (i = 0; i <= 1; i++) {
            struct DuelZone *field = &gDuel.players[i & 1].zones[ZONE_FIELD];

            if (FIELD_ZONE_CARD(i & 1).id
                && !field->isDisabled && field->isFaceUp) {
                int cardNo = CARD_NUMBER(FIELD_ZONE_CARD(i & 1).id);

                switch (cardNo) {
                case CARD_FOREST:
                case CARD_WASTELAND:
                case CARD_MOUNTAIN:
                case CARD_SOGEN:
                case CARD_UMI:
                case CARD_YAMI:
                    addAtk += gFieldTypeBonuses[cardNo - CARD_FOREST][out->type];
                    addDef += gFieldTypeBonuses[cardNo - CARD_FOREST][out->type];
                    break;
                case CARD_GAIA_POWER:
                case CARD_UMIIRUKA:
                case CARD_MOLTEN_DESTRUCTION:
                case CARD_RISING_AIR_CURRENT:
                case CARD_LUMINOUS_SPARK:
                case CARD_MYSTIC_PLASMA_ZONE:
                    addAtk += gFieldAttributeBonuses[cardNo - CARD_GAIA_POWER][out->attribute] * 500;
                    addDef -= gFieldAttributeBonuses[cardNo - CARD_GAIA_POWER][out->attribute] * 400;
                    break;
                case CARD_CHORUS_OF_SANCTUARY:
                    /* +500 DEF for monsters in defense position */
                    if (((struct ZoneFlagsByte *)&gDuel.players[player & 1].zones[zone])->flags & ZONE_FLAG_DEFENSE)
                        addDef += 500;
                    break;
                }
            }
        }
    }
    /* -300 ATK during its controller's Battle Phase while the opponent has a face-up card 1515 (no EDS card) */
    if (player == gDuel.turnPlayer && gDuel.phase == PHASE_BATTLE && CountFaceUpMonstersByNumber(1 - player, 1515))
        addAtk -= 300;
    addAtk += CountAquaChorusBoosts(player, zone) * 500;
    addDef += CountAquaChorusBoosts(player, zone) * 500;

    /* 6. Reverse Trap (statChangesReversed without trapsNegated) subtracts the modifiers instead. */
    if ((DUEL_RULE_FLAGS & (RULE_STATS_REVERSED | RULE_TRAPS_NEGATED)) == RULE_STATS_REVERSED) {
        out->atk -= addAtk + equipAtk + otherAtk;
        out->def -= addDef + equipDef + otherDef;
    } else {
        out->atk += addAtk + equipAtk + otherAtk;
        out->def += addDef + equipDef + otherDef;
    }
    if (out->atk < 0)
        out->atk = 0;
    if (out->def < 0)
        out->def = 0;
    /* Riryoku's halving (atkHalved) counts as one more ATK halving */
    if (thisZone->atkHalved)
        atkHalves++;
    /* the doublings and halvings cancel out; the rest are applied one by one */
    if (atkDoubles > atkHalves)
        for (i = 0; i < atkDoubles - atkHalves; i++)
            out->atk *= 2;
    if (atkDoubles < atkHalves)
        for (i = 0; i < atkHalves - atkDoubles; i++)
            out->atk = HalveRoundUp(out->atk);
    if (defDoubles > defHalves)
        for (i = 0; i < defDoubles - defHalves; i++)
            out->def *= 2;
    if (defDoubles < defHalves)
        for (i = 0; i < defHalves - defDoubles; i++)
            out->def = HalveRoundUp(out->def);
    /* Shield & Sword (atkDefSwapped without magicNegated) swaps ATK and DEF, except for an immune monster */
    if ((DUEL_RULE_FLAGS & (RULE_ATK_DEF_SWAPPED | RULE_MAGIC_NEGATED)) == RULE_ATK_DEF_SWAPPED && !immuneToMagic) {
        int def = out->def, atk = out->atk;

        out->atk = def;
        out->def = atk;
    }
}
