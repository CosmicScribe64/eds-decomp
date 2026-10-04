#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum ResponseEventKind, DuelPhase, DuelZoneIndex, DUEL_LOC_* */
#include "duel.h"                   /* gDuel, gDuelPlayers, duel rules and helpers */

/*
 * Activation conditions of card effects (wiki/functions/effect-prepare1-c.md).
 *
 * Every function here is the prepare handler (+0x0C) of a gCardEffects row (struct CardEffect,
 * include/effect.h). CanActivateEffect calls it through the table to ask "may this card's effect be
 * activated now?"; each one is a side-effect-free predicate returning 1 (yes) or 0 (no). Parameters:
 *   card       the activating card: its card ID, player and zone, the event that opened the response
 *              window (card->event, enum ResponseEventKind) and the event's locations loc0 / loc1
 *              (player | zone << 8);
 *   chainLink  the opponent's activation this card answers, or NULL (handlers that never read it
 *              declare it as int);
 *   fromHand   1 when the card is played from the hand, 0 when activated on the field.
 * Handlers that do not use the trailing parameters are defined without them.
 */

#include "chain.h"                  /* struct ChainEntry */
#include "summon.h"                 /* CanNormalSummon, CanSpecialSummon, CanSummonFromHand */
#include "effect.h"                 /* CollectEffectTargets, CountRedirectTargets, FindFusionMaterials */
#include "effect_handlers.h"        /* the prototypes of this unit's handlers */

/*
 * Local views of callees (HEADERS.md, "Keeping a deliberate local view"). Matching: the ROM compares the
 * results as signed ints (ATK <= 2000 is cmp; ble, the target count > 0 is cmp; ble); the header's u32 and
 * u16 return types would turn them into unsigned compares or add a narrowing of r0.
 */
int GetZoneCardAtkInt(int player, int zone) asm("GetZoneCardAtk");
int GetZoneCardDefInt(int player, int zone) asm("GetZoneCardDef");
int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");

/* The card word of a zone or pile entry as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with shifts; a .id bitfield read would load only the halfword that holds it. */
#define CARD_WORD(card)     (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)       (((word) << 20) >> 20)

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber
 * (0x08622AB4). Matching: these give the ROM's literal pools; the symbol forms generate other code.
 */
#define CARD_STATS(id)      (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */

/*
 * &gDuelZones[player].zones[zone] by integer arithmetic, zone term first. Matching: this is the ROM's
 * address order (array indexing emits the player term first). player must be 0 or 1: a ChainEntry's
 * player bit, or a location's player masked with & 1.
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))

/*
 * Jigen Bakudan: tribute it during its controller's Standby Phase. Needs a field activation answering the
 * player's own Standby Phase, a second monster, the zone's effectUnused flag clear (the opposite of
 * EffectOncePerTurnPrepare), and key 1418 (forbids Tributes) in effect on neither side.
 */
int EffectJigenBakudanPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (fromHand == 0 && card->event == RESPONSE_OWN_STANDBY && CountMonsters(card->player) > 1) {
        int player = card->player;
        struct DuelZone *zone = ZONE_AT(player, card->zone);

        if (!zone->effectUnused && CountActiveCardsOnField(0, CARD_1418) <= 0
            && CountActiveCardsOnField(1, CARD_1418) <= 0)
            return 1;
    }
    return 0;
}

/*
 * Shared once-per-turn ignition effect (Goddess of Whim, Barrel Dragon, Karate Man): on the field, returns
 * the effectUnused flag of the card's zone (set again at the start of the turn).
 */
int EffectOncePerTurnPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    int player;
    struct DuelZone *zone;

    if (fromHand != 0)
        return 0;
    player = card->player;
    zone = ZONE_AT(player, card->zone);
    return zone->effectUnused;
}

/*
 * Valkyrion the Magna Warrior: tribute it to Special Summon Alpha, Beta and Gamma The Magnet Warrior from
 * the graveyard. On the field only; two free monster zones (Valkyrion's own zone makes the third), no key
 * 1418 on either side, and all three magnets in the player's graveyard.
 */
int EffectValkyrionTheMagnaWarriorPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (fromHand == 0 && CountFreeMonsterZones(card->player) > 1 && CountActiveCardsOnField(0, CARD_1418) <= 0
        && CountActiveCardsOnField(1, CARD_1418) <= 0
        && CountGraveyardCardsByNumber(card->player, CARD_ALPHA_THE_MAGNET_WARRIOR) != 0
        && CountGraveyardCardsByNumber(card->player, CARD_BETA_THE_MAGNET_WARRIOR) != 0
        && CountGraveyardCardsByNumber(card->player, CARD_GAMMA_THE_MAGNET_WARRIOR) != 0)
        return 1;
    return 0;
}

/* Time Machine: on the field, answering a monster destroyed in battle. */
int EffectTimeMachinePrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (fromHand == 0 && card->event == RESPONSE_BATTLE_DESTROYED)
        return 1;
    return 0;
}

/* Toon World: the player can pay its 1000 LP cost. */
int EffectToonWorldPrepare(struct ChainEntry *card, int chainLink)
{
    return gDuelPlayers[card->player].lifePoints > 999;
}

/*
 * Shared (Lightforce Sword, The Inexperienced Spy, Confiscation, Delinquent Duo, The Forceful Sentry): the
 * opponent has a card in hand. Matching: the base pointer local comes first, then the opponent index.
 */
int EffectOpponentHasHandPrepare(struct ChainEntry *card)
{
    struct DuelPlayer *players = gDuelPlayers;
    int opponent = card->player;

    opponent ^= 1;
    return players[opponent].handCount != 0;
}

/*
 * The Flute of Summoning Dragon: Special Summon Dragons from the hand while Lord of D. is face up. Needs a
 * free monster zone, Special Summons allowed, Lord of D. face up on either side, and a Dragon in hand that
 * is not a Toon monster unless the player has a face-up Toon World.
 */
int EffectTheFluteOfSummoningDragonPrepare(struct ChainEntry *card)
{
    int i;

    if (CountFreeMonsterZones(card->player) != 0 && CanSpecialSummon(card->player) != 0
        && (CountFaceUpMonstersByNumber(0, CARD_LORD_OF_D) > 0 || CountFaceUpMonstersByNumber(1, CARD_LORD_OF_D) > 0)) {
        for (i = 0; i < gDuelPlayers[card->player].handCount; i++) {
            u32 id = CARD_ID(CARD_WORD(gDuelPlayers[card->player].hand[i]));

            if (CARD_TYPE(id) == CARD_TYPE_DRAGON
                && (IsToonMonster(CARD_NUMBER(id)) == 0 || HasFaceUpToonWorld(card->player) != 0))
                return 1;
        }
    }
    return 0;
}

/*
 * Chain Destruction: answering a summon (Normal, Flip or Special, by either player) of a face-up monster
 * with 2000 ATK or less. loc0 is the summoned monster.
 */
int EffectChainDestructionPrepare(struct ChainEntry *card)
{
    int player = DUEL_LOC_PLAYER(card->loc0);
    int zoneIndex = DUEL_LOC_ZONE(card->loc0);
    int p;
    struct DuelZone *zone;

    switch (card->event) {
    case RESPONSE_SUMMONED:
    case RESPONSE_FLIP_SUMMONED:
    case RESPONSE_SPECIAL_SUMMONED:
        break;
    default:
        return 0;
    }
    p = player & 1;
    zone = ZONE_AT(p, zoneIndex);
    if (!CARD_ID(CARD_WORD(zone->card)) || !zone->isFaceUp)
        return 0;
    return GetZoneCardAtkInt(player, zoneIndex) <= 2000;
}

/*
 * Shared by Trap Hole, House of Adhesive Tape and Eatgaboon: the opponent Normal or Flip Summoned (not
 * Special Summoned) a face-up monster (loc0). The activating card's number picks the stat test: Trap Hole
 * ATK 1000 or more, House of Adhesive Tape DEF 500 or less, Eatgaboon ATK 500 or less.
 */
int EffectTrapHolePrepare(struct ChainEntry *card)
{
    int player = DUEL_LOC_PLAYER(card->loc0);
    int zoneIndex = DUEL_LOC_ZONE(card->loc0);

    if (card->event == RESPONSE_SUMMONED || card->event == RESPONSE_FLIP_SUMMONED) {
        int p = player & 1;
        struct DuelZone *zone = ZONE_AT(p, zoneIndex);

        if (CARD_ID(CARD_WORD(zone->card)) && zone->isFaceUp && player != card->player) {
            switch (CARD_NUMBER(card->card)) {
            case CARD_TRAP_HOLE:
                return GetZoneCardAtkInt(player, zoneIndex) > 999;
            case CARD_HOUSE_OF_ADHESIVE_TAPE:
                return GetZoneCardDefInt(player, zoneIndex) <= 500;
            case CARD_EATGABOON:
                return GetZoneCardAtkInt(player, zoneIndex) <= 500;
            }
        }
    }
    return 0;
}

/*
 * Polymerization: Special Summons allowed and some Fusion Deck monster has its materials in the player's
 * hand or on the field (FindFusionMaterials).
 */
int EffectPolymerizationPrepare(struct ChainEntry *card)
{
    u16 slots[4];
    int i;

    if (CanSpecialSummon(card->player) == 0)
        return 0;
    /* Matching: the same '1 & card->player' in the loop condition and the body, and the player read
     * again inside the loop. */
    for (i = 0; i < gDuelPlayers[1 & card->player].fusionCount; i++) {
        int player = card->player;
        u16 fusionId = CARD_ID(CARD_WORD(gDuelPlayers[1 & card->player].fusionDeck[i]));

        if (FindFusionMaterials(player, fusionId, slots) != 0)
            return 1;
    }
    return 0;
}

/* Two-Pronged Attack: the player has two monsters to destroy and the opponent one. */
int EffectTwoProngedAttackPrepare(struct ChainEntry *card)
{
    if (CountMonsters(card->player) > 1 && CountMonsters(1 - card->player) > 0)
        return 1;
    return 0;
}

/*
 * Monster Reborn: a free monster zone, Special Summons allowed, Call of the Dark in effect on neither side,
 * and a graveyard monster to revive (CollectEffectTargets). chainLink and fromHand are unused (the
 * definition declares them as ints).
 */
int EffectMonsterRebornPrepare(struct ChainEntry *card, int chainLink, int fromHand)
{
    if (CountFreeMonsterZones(card->player) == 0 || CanSpecialSummon(card->player) == 0
        || CountActiveCardsOnField2(0, CARD_CALL_OF_THE_DARK) > 0
        || CountActiveCardsOnField2(1, CARD_CALL_OF_THE_DARK) > 0)
        return 0;
    return CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) > 0;
}

/*
 * Gravedigger Ghoul: key 1511 (which protects graveyards, hypothesis) not in effect on the opponent's side,
 * and a graveyard monster to banish (CollectEffectTargets).
 */
int EffectGravediggerGhoulPrepare(struct ChainEntry *card)
{
    if (CountActiveCardsOnField(1 - card->player, CARD_1511) > 0)
        return 0;
    return CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) > 0;
}

/*
 * Ultimate Offering: an extra Normal Summon for 500 LP. On the field only; the player has 500 LP, may
 * Normal Summon, and holds a monster that can be summoned now and is not Special-Summon-only.
 */
int EffectUltimateOfferingPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    int i;

    if (fromHand != 0 || gDuelPlayers[1 & card->player].lifePoints <= 499 || CanNormalSummon(card->player) == 0)
        return 0;
    /* Matching: the same '1 & card->player' in the loop condition and the body. */
    for (i = 0; i < gDuelPlayers[1 & card->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & card->player].hand[i]));

        if (CanSummonFromHand(card->player, id) != 0 && IsSpecialSummonOnly(id) == 0)
            return 1;
    }
    return 0;
}

/*
 * Shared by the traps that answer one opponent Magic card: on the field, chainLink is the opponent's, and
 * its card number is the one the trap counters. White Hole answers Dark Hole, Call of the Grave Monster
 * Reborn, Anti Raigeki Raigeki, Gryphon Wing Harpie's Feather Duster; key 1247 answers the listed
 * non-targeting normal Magic cards (healing, burn, draw, hand), key 1531 the Magic cards that target a
 * monster (equips, Change of Heart, Snatch Steal, ...).
 * Matching: the inner switches need their explicit 'default: return 0'.
 */
int EffectSpellResponsePrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand != 0)
        return 0;
    if (chainLink == NULL)
        return 0;
    if (chainLink->player == card->player)
        return 0;
    switch (CARD_NUMBER(card->card)) {
    case CARD_WHITE_HOLE:
        return CARD_NUMBER(chainLink->card) == CARD_DARK_HOLE;
    case CARD_CALL_OF_THE_GRAVE:
        return CARD_NUMBER(chainLink->card) == CARD_MONSTER_REBORN;
    case CARD_ANTI_RAIGEKI:
        return CARD_NUMBER(chainLink->card) == CARD_RAIGEKI;
    case CARD_GRYPHON_WING:
        return CARD_NUMBER(chainLink->card) == CARD_HARPIES_FEATHER_DUSTER;
    case CARD_1247:
        switch (CARD_NUMBER(chainLink->card)) {
        case CARD_MOOYAN_CURRY:
        case CARD_RED_MEDICINE:
        case CARD_GOBLINS_SECRET_REMEDY:
        case CARD_SOUL_OF_THE_PURE:
        case CARD_DIAN_KETO_THE_CURE_MASTER:
        case CARD_SPARKS:
        case CARD_HINOTAMA:
        case CARD_FINAL_FLAME:
        case CARD_OOKAZI:
        case CARD_GRACEFUL_CHARITY:
        case CARD_BLUE_MEDICINE:
        case CARD_RAIMEI:
        case CARD_POT_OF_GREED:
        case CARD_THE_CHEERFUL_COFFIN:
        case CARD_RESTRUCTER_REVOLUTION:
        case CARD_CONFISCATION:
        case CARD_DELINQUENT_DUO:
        case CARD_PAINFUL_CHOICE:
            return 1;
        default:
            return 0;
        }
    case CARD_1531:
        switch (CARD_NUMBER(chainLink->card)) {
        case CARD_LEGENDARY_SWORD:
        case CARD_SWORD_OF_DARK_DESTRUCTION:
        case CARD_DARK_ENERGY:
        case CARD_AXE_OF_DESPAIR:
        case CARD_LASER_CANNON_ARMOR:
        case CARD_INSECT_ARMOR_WITH_LASER_CANNON:
        case CARD_ELFS_LIGHT:
        case CARD_BEAST_FANGS:
        case CARD_STEEL_SHELL:
        case CARD_VILE_GERMS:
        case CARD_BLACK_PENDANT:
        case CARD_SILVER_BOW_AND_ARROW:
        case CARD_HORN_OF_LIGHT:
        case CARD_HORN_OF_THE_UNICORN:
        case CARD_DRAGON_TREASURE:
        case CARD_ELECTRO_WHIP:
        case CARD_CYBER_SHIELD:
        case CARD_MYSTICAL_MOON:
        case CARD_STOP_DEFENSE:
        case CARD_MALEVOLENT_NUZZLER:
        case CARD_VIOLET_CRYSTAL:
        case CARD_BOOK_OF_SECRET_ARTS:
        case CARD_INVIGORATION:
        case CARD_MACHINE_CONVERSION_FACTORY:
        case CARD_RAISE_BODY_HEAT:
        case CARD_FOLLOW_WIND:
        case CARD_POWER_OF_KAISHIN:
        case CARD_MAGICAL_LABYRINTH:
        case CARD_SALAMANDRA:
        case CARD_MEGAMORPH:
        case CARD_7_COMPLETED:
        case CARD_MONSTER_REBORN:
        case CARD_BURNING_SPEAR:
        case CARD_GUST_FAN:
        case CARD_TRIBUTE_TO_THE_DOOMED:
        case CARD_CHANGE_OF_HEART:
        case CARD_SWORD_OF_DEEP_SEATED:
        case CARD_BLOCK_ATTACK:
        case CARD_GERM_INFECTION:
        case CARD_PARALYZING_POTION:
        case CARD_RING_OF_MAGNETISM:
        case CARD_STIM_PACK:
        case CARD_SNATCH_STEAL:
        case CARD_DARKNESS_APPROACHES:
        case CARD_RUSH_RECKLESSLY:
        case CARD_THE_RELIABLE_GUARDIAN:
        case CARD_NOBLEMAN_OF_CROSSOUT:
        case CARD_PREMATURE_BURIAL:
        case CARD_SWORD_OF_DRAGONS_SOUL:
        case CARD_1211:
        case CARD_1220:
        case CARD_1313:
        case CARD_1419:
        case CARD_1420:
        case CARD_1422:
        case CARD_1448:
        case CARD_1449:
        case CARD_1450:
        case CARD_1451:
        case CARD_1540:
        case CARD_1546:
        case CARD_1548:
        case CARD_1550:
            return 1;
        default:
            return 0;
        }
    }
    return 0;
}

/* Tribute to The Doomed: the player has a card to discard for the cost. */
int EffectTributeToTheDoomedPrepare(struct ChainEntry *card)
{
    if (gDuelPlayers[card->player].handCount == 0)
        return 0;
    return 1;
}

/* Soul Release: key 1511 not in effect on the opponent's side, and either graveyard holds a card. */
int EffectSoulReleasePrepare(struct ChainEntry *card)
{
    if (CountActiveCardsOnField(1 - card->player, CARD_1511) > 0)
        return 0;
    if (gDuelPlayers[0].graveCount == 0 && gDuelPlayers[1].graveCount == 0)
        return 0;
    return 1;
}

/* The Cheerful Coffin: the player's hand holds a Monster Card to discard. */
int EffectTheCheerfulCoffinPrepare(struct ChainEntry *card)
{
    int i;

    /* Matching: the same '1 & card->player' in the loop condition and the body. */
    for (i = 0; i < gDuelPlayers[1 & card->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & card->player].hand[i]));

        if (CARD_TYPE(id) <= CARD_TYPE_REPTILE)   /* types 1-20 are monsters */
            return 1;
    }
    return 0;
}

/* Change of Heart: the opponent has a monster and the player a free monster zone to take it to. */
int EffectChangeOfHeartPrepare(struct ChainEntry *card)
{
    if (CountMonsters(1 - card->player) != 0 && CountFreeMonsterZones(card->player) != 0)
        return 1;
    return 0;
}

/*
 * Solemn Judgment: on the field. Without chainLink: answering a summon (Normal, Flip or Special) of the
 * monster at loc0, which is still there. With chainLink: it is a Magic or Trap activation other than key
 * 1539.
 */
int EffectSolemnJudgmentPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand == 0) {
        if (chainLink == NULL) {
            int player = DUEL_LOC_PLAYER(card->loc0) & 1;
            int zoneIndex = DUEL_LOC_ZONE(card->loc0);
            struct DuelZone *zone = ZONE_AT(player, zoneIndex);

            if (CARD_ID(CARD_WORD(zone->card)) != 0) {
                switch (card->event) {
                case RESPONSE_SUMMONED:
                case RESPONSE_FLIP_SUMMONED:
                case RESPONSE_SPECIAL_SUMMONED:
                    return 1;
                }
            }
        } else {
            u16 id = chainLink->card & CARD_ID_MASK;

            switch ((int)CARD_TYPE(id)) {
            case CARD_TYPE_TRAP:
            case CARD_TYPE_MAGIC:
                if (CARD_NUMBER(id) != CARD_1539)
                    return 1;
            }
        }
    }
    return 0;
}

/*
 * Magic Jammer: on the field, chainLink is a Magic card other than key 1539, and the player has a card to
 * discard for the cost.
 */
int EffectMagicJammerPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand == 0 && chainLink != NULL) {
        u32 id = chainLink->card & CARD_ID_MASK;

        if (CARD_TYPE(id) == CARD_TYPE_MAGIC && gDuelPlayers[card->player].handCount != 0
            && CARD_NUMBER(id) != CARD_1539)
            return 1;
    }
    return 0;
}

/* Seven Tools of the Bandit: on the field, chainLink is a Trap card, and the player can pay 1000 LP. */
int EffectSevenToolsOfTheBanditPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand == 0 && chainLink != NULL && CARD_TYPE(chainLink->card) == CARD_TYPE_TRAP
        && gDuelPlayers[card->player].lifePoints > 999)
        return 1;
    return 0;
}

/*
 * Horn of Heaven: on the field, answering a summon (Normal, Flip or Special) of the monster at loc0. The
 * player needs a monster to tribute for the cost: any monster when the opponent summoned, another monster
 * when the player summoned it.
 */
int EffectHornOfHeavenPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    int player = DUEL_LOC_PLAYER(card->loc0);
    int zoneIndex = DUEL_LOC_ZONE(card->loc0);

    if (fromHand == 0) {
        int p = player & 1;
        struct DuelZone *zone = ZONE_AT(p, zoneIndex);

        if (CARD_ID(CARD_WORD(zone->card)) != 0) {
            if (player == card->player && CountTributableMonsters(card->player, zoneIndex) == 0)
                return 0;
            if (player != card->player && CountTributableMonsters(card->player, -1) == 0)
                return 0;
            switch (card->event) {
            case RESPONSE_SUMMONED:
            case RESPONSE_FLIP_SUMMONED:
            case RESPONSE_SPECIAL_SUMMONED:
                return 1;
            }
        }
    }
    return 0;
}

/*
 * Restructer Revolution: the opponent has a card in hand (200 damage per card). A byte-identical copy of
 * EffectOpponentHasHandPrepare.
 */
int EffectRestructerRevolutionPrepare(struct ChainEntry *card)
{
    struct DuelPlayer *players = gDuelPlayers;
    int opponent = card->player;

    opponent ^= 1;
    return players[opponent].handCount != 0;
}

/* Fusion Sage: the deck holds a Polymerization to add to the hand (CollectEffectTargets). */
int EffectFusionSagePrepare(struct ChainEntry *card)
{
    return CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) > 0;
}

/*
 * Last Will: Special Summons allowed, a monster of the player's went from the field to the graveyard this
 * turn, a free monster zone, and a deck monster with 1500 ATK or less to summon (CollectEffectTargets).
 */
int EffectLastWillPrepare(struct ChainEntry *card)
{
    if (CanSpecialSummon(card->player) != 0) {
        struct DuelPlayer *players = gDuelPlayers;
        u32 playerBit = (u32)((u8 *)card)[2] << 31;     /* card->player in bit 31 */

        /* players[player].monsterSentToGraveThisTurn (+0x0B bit 3), tested as the sign of byte << 28 */
        if ((s32)((u32)((u8 *)&players[playerBit >> 31])[0xB] << 28) < 0) {
            /* FAKEMATCH: the ROM keeps playerBit and shifts it again (lsr r0, r3, #31) for the
             * CountFreeMonsterZones argument. Without the empty asm (with this test or the
             * monsterSentToGraveThisTurn bitfield) the compiler copies the player index register
             * instead (checked 2026-10-02). */
            __asm__("" : "+r"(playerBit));
            if (CountFreeMonsterZones(playerBit >> 31) != 0
                && CollectEffectTargetsInt(card->player, CARD_LAST_WILL, 0) != 0)
                return 1;
        }
    }
    return 0;
}

/* Waboku: on the field, during the opponent's turn. */
int EffectWabokuPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (fromHand == 0 && gDuel.turnPlayer != card->player)
        return 1;
    return 0;
}

/*
 * Shared by the traps that answer an attack declaration: on the field, the opponent declared an attack
 * (loc0 is the attacker, loc1 the attack target). Negate Attack, Enchanted Javelin and keys 1214, 1415,
 * 1424 need nothing else. Widespread Ruin and Mirror Force need an opponent card in attack position in
 * zones 0-9 (a quirk: the loop also accepts a spell/trap card, whose isDefense bit is clear). Magical
 * Hats and Magic-Arm Shield need a monster as the attack target (loc1 zone 0-4; a direct attack is
 * assumed to give a higher zone, hypothesis) and a monster of the player's; then Magical Hats needs two
 * free monster zones and two cards to hide the monster among (CollectEffectTargets), Magic-Arm Shield a
 * free monster zone and two face-up opponent monsters.
 * Matching: the two CountMonsters calls at the top discard their results; the always-true case group
 * comes first and every other case returns explicitly.
 */
int EffectAttackResponsePrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    u16 number;
    int zoneIndex;

    CountMonsters(card->player);
    CountMonsters(1 - card->player);
    if (fromHand != 0)
        return 0;
    if (DUEL_LOC_PLAYER(card->loc0) == card->player)
        return 0;
    if (card->event != RESPONSE_ATTACK_DECLARED)
        return 0;
    number = CARD_NUMBER(card->card);
    switch (number) {
    case CARD_NEGATE_ATTACK:
    case CARD_ENCHANTED_JAVELIN:
    case CARD_1214:
    case CARD_1415:
    case CARD_1424:
        return 1;
    case CARD_WIDESPREAD_RUIN:
    case CARD_MIRROR_FORCE:
        for (zoneIndex = ZONE_MONSTER_0; zoneIndex <= ZONE_SPELL_4; zoneIndex++) {
            /* Matching: each zone access computes its own address. */
            int opponent = (1 - card->player) & 1;
            struct DuelZone *zone = ZONE_AT(opponent, zoneIndex);

            if (CARD_ID(CARD_WORD(zone->card)) != 0) {
                int opponent2 = (1 - card->player) & 1;
                struct DuelZone *zone2 = ZONE_AT(opponent2, zoneIndex);

                if (!zone2->isDefense)
                    return 1;
            }
        }
        return 0;
    case CARD_MAGICAL_HATS:
        if (DUEL_LOC_ZONE(card->loc1) > ZONE_MONSTER_4)
            return 0;
        if (CountMonsters(card->player) == 0)
            return 0;
        if (CountFreeMonsterZones(card->player) <= 1)
            return 0;
        if (CollectEffectTargetsInt(card->player, number, 0) <= 1)
            return 0;
        return 1;
    case CARD_MAGIC_ARM_SHIELD:
        if (DUEL_LOC_ZONE(card->loc1) > ZONE_MONSTER_4)
            return 0;
        if (CountMonsters(card->player) == 0)
            return 0;
        if (CountFreeMonsterZones(card->player) == 0)
            return 0;
        if (CountMonstersFiltered(1 - card->player, TRUE, FALSE) <= 1)     /* face-up, any position */
            return 0;
        return 1;
    }
    return 0;
}

/* Share the Pain: both players have a monster to tribute. */
int EffectShareThePainPrepare(struct ChainEntry *card)
{
    if (CountTributableMonsters(card->player, -1) != 0 && CountTributableMonsters(1 - card->player, -1) != 0)
        return 1;
    return 0;
}

/* Curse of Fiend: during the Standby Phase, with a monster on either side. */
int EffectCurseOfFiendPrepare(struct ChainEntry *card)
{
    if (gDuel.phase != PHASE_STANDBY || (CountMonsters(card->player) <= 0 && CountMonsters(1 - card->player) <= 0))
        return 0;
    return 1;
}

/*
 * Final Destiny: five other cards in hand to discard (a hand activation counts the card itself, so
 * fromHand is subtracted), and a card in any zone of either player.
 */
int EffectFinalDestinyPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    int player, zoneIndex;

    if (gDuelPlayers[card->player].handCount - fromHand <= 4)
        return 0;
    for (player = 0; player < 2; player++) {
        for (zoneIndex = ZONE_MONSTER_0; zoneIndex <= ZONE_FIELD; zoneIndex++) {
            if (CARD_ID(CARD_WORD(gDuelPlayers[player & 1].zones[zoneIndex].card)) != 0)
                return 1;
        }
    }
    return 0;
}

/* Darkness Approaches: two other cards in hand to discard (fromHand: the card itself). */
int EffectDarknessApproachesPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (gDuelPlayers[card->player].handCount - fromHand <= 1)
        return 0;
    return 1;
}

/*
 * Fairy's Hand Mirror: on the field, chainLink is the opponent's, and its effect has another valid target
 * to switch to (CountRedirectTargets).
 */
int EffectFairysHandMirrorPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand != 0 || chainLink == NULL || chainLink->player == card->player)
        return 0;
    return CountRedirectTargets(CARD_FAIRYS_HAND_MIRROR, card->player, chainLink) > 0;
}

/* Painful Choice: the deck holds the five cards to pick from. */
int EffectPainfulChoicePrepare(struct ChainEntry *card)
{
    return gDuelPlayers[card->player].deckCount > 4;
}

/* Graverobber: the opponent's graveyard holds a Magic card to take. */
int EffectGraverobberPrepare(struct ChainEntry *card)
{
    int i;

    /* Matching: the same '(1 - card->player) & 1' in the loop condition and the body. */
    for (i = 0; i < gDuelPlayers[(1 - card->player) & 1].graveCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[(1 - card->player) & 1].graveyard[i]));

        if (CARD_TYPE(id) == CARD_TYPE_MAGIC)
            return 1;
    }
    return 0;
}

/*
 * Call of the Haunted: Special Summons allowed, a free monster zone, and a graveyard monster to revive
 * (CollectEffectTargets).
 */
int EffectCallOfTheHauntedPrepare(struct ChainEntry *card)
{
    if (CanSpecialSummon(card->player) == 0 || CountFreeMonsterZones(card->player) == 0)
        return 0;
    return CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) > 0;
}
