/*
 * ai_summon (0x08057EE0-0x080590E3): the CPU's summon choice, set-card activation and chain answers
 * (wiki/functions/ai-summon-c.md).
 *
 * The CPU is always player 1, the human is player 0. This unit holds:
 *  - the simulation steps: AiSimSummon puts a hand monster on the field and AiSimBattlePhase plays out the
 *    CPU's whole Battle Phase on the live duel state, returning the predicted damage. They only run inside
 *    the AiBackupDuelState / AiRestoreDuelState sandbox (src/ai_picks.c), as do AiSimBattleDamage and
 *    AiPlanAttack, which wrap the sandbox around a whole simulation;
 *  - AiChooseSummonNoTribute / AiChooseSummonWithTribute: pick the hand monster whose simulated summon gives
 *    the best battle result;
 *  - AiShouldActivateSetCard (the CPU's activation window) and AiTryChainResponse (its answer to a chain
 *    link): decided card by card, by card number. Set Magic/Trap cards are activated through AiChainSetCard;
 *  - AddCardNumberToDeckTop: puts a card on top of a deck (builds the CPU's deck and the start field).
 */
#include "global.h"
#include "card_data.h"              /* gCardNumberToId, CARD_ID_MASK, CARD_NUMBER_*, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind, CARD_STATS_* layout */
#include "constants/duel.h"         /* enum DuelZoneIndex, enum ChainEntryKind, MONSTER_ZONE_COUNT */
#include "constants/duel_cmds.h"    /* DUEL_CMD_CHAIN_BANNER, DUEL_CMD_PLAYER */
#include "duel.h"               /* struct DuelState, struct DuelPlayer, struct DuelZone, gDuel, gDuelPlayers */
#include "ai.h"                     /* struct AiWork, struct AttackPlan, AI_FLAG_EXODIA, the Ai* functions */
#include "battle.h"                 /* CanEnterBattlePhase, BuildAttackableMask */
#include "chain.h"                  /* struct ChainEntry, Chain_AddLink */
#include "duel_actions.h"           /* FlipFieldCard */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl */
#include "summon.h"                 /* CanSummonFromHand */
#include "util.h"                   /* Random */

/* ---- Local views (matching choices) ---- */

/* A card word read whole, then shifted by hand. */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_ID_OF(c) CARD_ID(CARD_WORD(c))
/* "The slot holds no card", tested as the ROM does: the word shifted left by 20 keeps only the ID bits
 * (`card.id == 0` compiles to different code). */
#define CARD_EMPTY(c) ((CARD_WORD(c) << 20) == 0)
/* The card tables through integer-constant addresses: the ROM reloads the table address at every use. These
 * are gCardStats (0x08621DE0), gCardIdToNumber (0x08622AB4) and gCardNumberToId (0x08623DF4). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_ID_OF_NUMBER(no) (((const u16 *)0x08623DF4)[(no) & (CARD_NUMBER_COUNT - 1)])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))
/* Zone pointer by byte arithmetic with the zone term first (the ROM's address order); callers pass player & 1. */
#define ZONE_AT(p, z) \
    ((struct DuelZone *)((z) * sizeof(struct DuelZone) + (p) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
/* The same with the player term first, where the ROM expands `player & 1` before the zone. */
#define ZONE_AT_PLAYER_FIRST(p, z) \
    ((struct DuelZone *)((p) * sizeof(struct DuelPlayer) + (z) * sizeof(struct DuelZone) + (u32)gDuelZones))
/* DuelZone +0x91 bit 2 (canActivate): a set Magic/Trap card that may be activated. Matching: read as a byte
 * (the header declares it in a u32 bitfield container, which loads differently). */
#define ZONE_CAN_ACTIVATE(z) (((const u8 *)(z))[0x91] & 0x4)
/* The hand of a player as reached from a pointer to its zone 0 (hand - zones = 0x65C bytes), and the first
 * zone of player 1 / player 0 as reached from gDuelPlayers (the ROM's constants +0xD8C and +0x28). */
#define HAND_FROM_ZONE0(zone0) \
    ((u32 *)((u32)(zone0) + (OFFSET_OF(struct DuelPlayer, hand) - OFFSET_OF(struct DuelPlayer, zones))))
#define PLAYER1_ZONES_ADDR ((u32)gDuelPlayers + sizeof(struct DuelPlayer) + OFFSET_OF(struct DuelPlayer, zones))
#define PLAYER0_ZONES_ADDR ((u32)gDuelPlayers + OFFSET_OF(struct DuelPlayer, zones))

/* Level of a card for the Tribute rules: 0 for Magic/Trap/Ticket cards, 10 for a Divine card, else the star
 * count (gCardStats bits 25-28). */
static inline u32 CardLevel(u16 id)
{
    u32 r;

    switch ((int)CARD_TYPE(id)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        r = 0;
        break;
    case CARD_TYPE_DIVINE:
        r = 10;
        break;
    default:
        r = CARD_STATS_LEVEL(CARD_STATS(id));
        break;
    }
    return r;
}

/* Simulation: remove the tributes named by the nibbles of `tributeMask`, then put hand card `handIndex` of
 * player 1 onto a free monster zone as a face-up Attack Position monster. Each of the four nibbles that is
 * nonzero names a tribute (the packers set bit 3 as a presence marker; bits 0-2 are its monster zone), and
 * that zone's card ID is cleared. The hand itself is not changed. ROM quirks: only the card ID is written into the new zone, and a -1
 * from FindFreeMonsterZone is not checked, so call it only inside the backup/restore sandbox, which also
 * covers a stray write to zones[-1]. */
void AiSimSummon(int handIndex, u16 tributeMask)
{
    struct DuelZone *zone;
    int i;
    register int freeZone __asm__("r2");

    /* FAKEMATCH: keep the mask out of r2, which the ROM uses for the counter. */
    __asm__ __volatile__("" : : "r"(tributeMask) : "r2");
    for (i = 0; i < 4; i++) {
        if (tributeMask & 0xF)
            ZONE_AT(1, tributeMask & 7)->card.id = 0;
        tributeMask >>= 4;
    }
    freeZone = FindFreeMonsterZone(1);
    /* FAKEMATCH: retain the ROM's r2 copy before the stride multiplication. */
    __asm__ __volatile__("" : : "r"(freeZone));
    zone = ZONE_AT(1, freeZone);
    zone->card.id = CARD_ID(HAND_FROM_ZONE0(ZONE_AT(1, 0))[handIndex]);
    zone->isFaceUp = 1;
    zone->isDefense = 0;
}

/* Simulation: play out the CPU's whole Battle Phase on the live duel state and return the total predicted
 * battle damage to the human (negative: the CPU takes more than it deals). It zeroes the destroyed-monster
 * counters; if the CPU may enter the Battle Phase it rebuilds the can-attack mask, then repeats AiChooseAttack:
 * each plan marks its attacker in attackedMask (so the loop ends once nobody can attack), removes the
 * destroyed cards, counts them in gAiWork (simAttackersLost, simTargetsDestroyed) and adds the plan's damage.
 * ROM quirk: the destroyed cards are cleared at zones[attackerDestroyed] of player 1 and
 * zones[targetDestroyed] of player 0, indexed by the plan's 1-bit flags, so the code always hits zone 1
 * and never attackerZone / targetZone. */
int AiSimBattlePhase(void)
{
    int total = 0;

    gAiWork.simAttackersLost = 0;
    gAiWork.simTargetsDestroyed = 0;
    if (CanEnterBattlePhase(1)) {
        BuildAttackableMask(gDuelPlayers, 1, 0, 0);
        /* Matching: the (u16) cast is the ROM's zero-extension of the result (ai.h declares it as int). */
        while ((u16)AiChooseAttack()) {
            gDuelPlayers[1].attackedMask |= 1 << gAiWork.bestAttack.attackerZone;
            if (gAiWork.bestAttack.attackerDestroyed) {
                ((struct DuelZone *)(gAiWork.bestAttack.attackerDestroyed * sizeof(struct DuelZone)
                    + PLAYER1_ZONES_ADDR))->card.id = 0;
                gAiWork.simAttackersLost++;
            }
            if (gAiWork.bestAttack.targetDestroyed) {
                ((struct DuelZone *)(gAiWork.bestAttack.targetDestroyed * sizeof(struct DuelZone)
                    + PLAYER0_ZONES_ADDR))->card.id = 0;
                gAiWork.simTargetsDestroyed++;
            }
            total += gAiWork.bestAttack.damage;
        }
    }
    return total;
}

/* Predicted battle damage of the CPU's whole Battle Phase, simulated inside the backup sandbox.
 * assumeAttackPos: first turn the CPU's monsters (those without a flip effect) to face-up Attack Position
 * (AiSimSetAttackPositions). No caller in this ROM. */
int AiSimBattleDamage(u16 assumeAttackPos)
{
    int damage;

    AiBackupDuelState();
    if (assumeAttackPos != 0)
        AiSimSetAttackPositions();
    damage = AiSimBattlePhase();
    AiRestoreDuelState();
    return damage;
}

/* Run AiChooseAttack inside the backup sandbox: 1 if the CPU has a winning attack (the plan survives the
 * restore in gAiWork.bestAttack). assumeAttackPos: as for AiSimBattleDamage. AiStepMainPhase calls it with
 * 1 to ask whether an attack would work if its monsters switched to Attack Position; the battle step calls
 * it with 0 and then uses gAiWork.bestAttack. */
int AiPlanAttack(u16 assumeAttackPos)
{
    u16 found;  /* u16: the result is zero-extended, as the AI callers read it as a word */

    AiBackupDuelState();
    if (assumeAttackPos != 0)
        AiSimSetAttackPositions();
    found = AiChooseAttack();
    AiRestoreDuelState();
    return found;
}

/* Pick the hand monster of player 1 to summon this turn, for every level; returns its hand index (-1 if
 * none) and stores the best simulated battle damage in *outDamage.
 * It first simulates the Battle Phase of the field as it is (baseDamage). Then each hand card that can be
 * summoned (not a key card, not special-summon-only) gets the tributes it needs and is simulated inside the
 * backup sandbox: AiSimSummon, the CPU's monsters to Attack Position, the Battle Phase, then the CPU's monster
 * count. A card is better if its damage beats the best so far, or equals it while destroying more human
 * monsters and leaving the CPU a monster. With AI_FLAG_EXODIA, Sangan, the Witch of the Black Forest and Mystic
 * Tomato are also taken when life points + damage > 1000, Exodia pieces are still in the deck and none are in
 * the graveyard or on the field.
 *  - Levels 1-4: no tribute.
 *  - Levels 5-6: one tribute from AiPickTributeMonster, mask = zone | 8.
 *  - Level 0, 7+ and Divine (10): two tributes packed into the mask: the first tribute's zone in bits 4-6
 *    and the second as (zone & 7) | 8 in the low nibble. ROM quirk: the first tribute gets no `| 8` marker,
 *    so a first tribute in zone 0 is not removed by AiSimSummon.
 * A card whose tributes cannot be found is skipped. */
int AiChooseSummonWithTribute(int *outDamage)
{
    int bestHandIndex = -1;
    int bestKills = 0;
    int baseDamage;
    int handIndex;

    AiBackupDuelState();
    AiSimSetAttackPositions();
    baseDamage = AiSimBattlePhase();
    AiRestoreDuelState();
    for (handIndex = 0; handIndex < gDuelPlayers[1].handCount; handIndex++) {
        u16 cardId = CARD_ID_OF(gDuelPlayers[1].hand[handIndex]);
        int skip;
        int better;
        u16 tributeMask;
        int damage;
        int tribute1, tribute2;
        u16 tributeNibble2;

        if (!CanSummonFromHand(1, cardId))
            continue;
        /* Key cards the CPU keeps (tier 2: Exodia pieces, staple Magic/Traps, Kuriboh, Cyber-Stein). */
        if (AiIsKeyCard(2, CARD_NUMBER(cardId)))
            continue;
        if (IsSpecialSummonOnly(cardId))
            continue;
        skip = 0;
        better = 0;
        tributeMask = 0;
        switch ((int)CardLevel(cardId)) {
        case 1:
        case 2:
        case 3:
        case 4:
            break;                          /* no tribute */
        case 5:
        case 6:                             /* one tribute */
            tribute1 = AiPickTributeMonster(-1, 0);
            if (tribute1 == -1)
                skip = 1;
            tributeMask = tribute1 | 8;
            break;
        default:                            /* two tributes (level 0, 7+, Divine) */
            tribute1 = AiPickTributeMonster(-1, 0);
            tribute2 = AiPickTributeMonster(tribute1, 0);
            if (tribute1 == -1 || tribute2 == -1)
                skip = 1;
            tributeNibble2 = (tribute2 & 7) | 8;
            tributeMask = (((u32)tribute1 << 20) >> 16) | tributeNibble2;
            break;
        }
        if (skip)
            continue;
        AiBackupDuelState();
        AiSimSummon(handIndex, tributeMask);
        AiSimSetAttackPositions();
        damage = AiSimBattlePhase();
        gAiWork.simOwnMonsters = CountMonsters(1);
        gAiWork.simOwnMonsters2 = CountMonsters(1);
        AiRestoreDuelState();
        if (baseDamage < damage)
            better = 1;
        if (damage == baseDamage && bestKills < gAiWork.simTargetsDestroyed && gAiWork.simOwnMonsters != 0)
            better = 1;
        if (gDuelCtrl.aiFlags & AI_FLAG_EXODIA) {
            /* Exodia strategy: a card that digs for pieces, if the CPU survives the battle with more than
             * 1000 life points and no piece is out of the deck yet. */
            switch (CARD_NUMBER(cardId)) {
            case CARD_SANGAN:
            case CARD_WITCH_OF_THE_BLACK_FOREST:
            case CARD_MYSTIC_TOMATO:
                if (gDuelPlayers[1].lifePoints + damage > 1000 && AiCountExodiaInDeck() > 0
                 && AiCountExodiaInGraveyard() == 0 && AiCountExodiaOnField() == 0)
                    better = 1;
                break;
            }
        }
        if (better) {
            if (damage > 0)
                baseDamage = damage;
            bestKills = gAiWork.simTargetsDestroyed;
            bestHandIndex = handIndex;
        }
    }
    *outDamage = baseDamage;
    return bestHandIndex;
}

/* Pick the hand monster of player 1 to Normal Summon without a tribute (level 1-4); returns its hand index,
 * or -1 if none improves on the current field. The baseline is the simulated damage of the field as it is
 * (monsters in Attack Position, inside the backup sandbox). Each summonable hand card (not a key card, not
 * special-summon-only, level <= 4) is summoned in a simulation of its own; it replaces the best so far if its
 * damage is higher, or equal while it destroys more human monsters and leaves the CPU at least one monster. */
int AiChooseSummonNoTribute(void)
{
    int bestHandIndex = -1;
    int bestKills = 0;
    int baseDamage;
    int handIndex;

    AiBackupDuelState();
    AiSimSetAttackPositions();
    baseDamage = AiSimBattlePhase();
    AiRestoreDuelState();
    for (handIndex = 0; handIndex < gDuelPlayers[1].handCount; handIndex++) {
        u16 cardId = CARD_ID_OF(gDuelPlayers[1].hand[handIndex]);
        int skip;
        int better;
        int damage;

        if (!CanSummonFromHand(1, cardId))
            continue;
        /* Key cards the CPU keeps (tier 2: Exodia pieces, staple Magic/Traps, Kuriboh, Cyber-Stein). */
        if (AiIsKeyCard(2, CARD_NUMBER(cardId)))
            continue;
        if (IsSpecialSummonOnly(cardId))
            continue;
        skip = 0;
        better = 0;
        if (CardLevel(cardId) > 4)      /* needs a tribute: not considered here */
            skip = 1;
        if (skip)
            continue;
        AiBackupDuelState();
        AiSimSummon(handIndex, 0);
        AiSimSetAttackPositions();
        damage = AiSimBattlePhase();
        gAiWork.simOwnMonsters = CountMonsters(1);
        gAiWork.simOwnMonsters2 = CountMonsters(1);
        AiRestoreDuelState();
        if (baseDamage < damage)
            better = 1;
        if (damage == baseDamage && bestKills < gAiWork.simTargetsDestroyed && gAiWork.simOwnMonsters != 0)
            better = 1;
        if (better) {
            if (damage > 0)
                baseDamage = damage;
            bestKills = gAiWork.simTargetsDestroyed;
            bestHandIndex = handIndex;
        }
    }
    return bestHandIndex;
}

/* Effective ATK of the card in zone 0 of the side `side`. ROM quirk: AiShouldActivateSetCard passes the
 * entry's loc0 halfword (player | zone << 8) as the player and always asks for zone 0, not for the zone
 * of the attacker. */
static inline int CallZoneZero(int side)
{
    int zero = 0;
    /* FAKEMATCH: set r1 before the index is copied into r0, as in both ROM calls (an empty asm with an r0
     * clobber). */
    __asm__ __volatile__("" : : "r"(zero) : "r0");
    return GetZoneCardAtk(side, zero);
}

/* AI: should the CPU activate the set card described by `entry` (an activation window; NULL gives 0)?
 * Decided by the card number of entry->card (a Trap, mostly): some always, others by the board.
 * Always: House of Adhesive Tape, Eatgaboon, Widespread Ruin, Negate Attack, Mesmeric Control, Magic-Arm
 * Shield, Trap Hole, Call of the Haunted, Enchanted Javelin, Mirror Wall, Numinous Healer, Appropriate, Backup
 * Soldier, Light of Intervention, Graceful Dice, Skull Dice and the effect keys 1303, 1304, 1323, 1325, 1415,
 * 1423, 1424, 1447, 1528 and 1536. By the board: Bell of Destruction, Just Desserts, Magic Thorn, Gift of the
 * Mystical Elf, Mirror Force, key 1214, Ceasefire, Aqua Chorus and key 1425. Everything else gives 0.
 * Card numbers 1214 and 1425 are effect keys with no EDS card. */
int AiShouldActivateSetCard(struct ChainEntry *entry)
{
    int a;
    int r;
    int z;
    int zone;
    int zz;

    if (entry == 0)
        return 0;
    switch (CARD_NUMBER(entry->card)) {
    case CARD_HOUSE_OF_ADHESIVE_TAPE:
    case CARD_EATGABOON:
    case CARD_WIDESPREAD_RUIN:
    case CARD_NEGATE_ATTACK:
    case CARD_MESMERIC_CONTROL:
    case CARD_MAGIC_ARM_SHIELD:
    case CARD_TRAP_HOLE:
    case CARD_CALL_OF_THE_HAUNTED:
    case CARD_ENCHANTED_JAVELIN:
    case CARD_MIRROR_WALL:
    case CARD_NUMINOUS_HEALER:
    case CARD_APPROPRIATE:
    case CARD_BACKUP_SOLDIER:
    case CARD_LIGHT_OF_INTERVENTION:
    case CARD_GRACEFUL_DICE:
    case CARD_SKULL_DICE:
    case CARD_1303:
    case CARD_1304:
    case CARD_1323:
    case CARD_1325:
    case CARD_1415:
    case CARD_1423:
    case CARD_1424:
    case CARD_1447:
    case CARD_1528:
    case CARD_1536:
        return 1;
    case CARD_BELL_OF_DESTRUCTION:
        /* a = the CPU's strongest ATK: go if it exceeds the human's life points but not the CPU's own.
         * Otherwise a = the human's strongest ATK: no if there is none or it reaches the CPU's life points;
         * go if it exceeds the human's life points, or if the human would still be below the CPU's life
         * points after losing a. */
        a = AiGetStrongestMonsterScore(1, -1, 1, 0);
        if (a > -1 && a < gDuelPlayers[1].lifePoints && a > gDuelPlayers[0].lifePoints)
            return 1;
        a = AiGetStrongestMonsterScore(0, -1, 1, 0);
        if (a <= -1)
            return 0;
        if (a >= gDuelPlayers[1].lifePoints)
            return 0;
        if (a > gDuelPlayers[0].lifePoints)
            return 1;
        if (gDuelPlayers[0].lifePoints - a < gDuelPlayers[1].lifePoints)
            return 1;
        return 0;
    case CARD_JUST_DESSERTS:
        /* 500 damage per human monster: go if it is lethal, else by chance. */
        if (CountMonsters(0) * 500 >= gDuelPlayers[0].lifePoints)
            return 1;
        {
            int t = CountMonsters(0);

            if (t > Random() % 3 + 1)
                return 1;
        }
        return 0;
    case CARD_MAGIC_THORN:
        /* ROM quirk: AiFindHandCardByNumber returns -1 for a missing card, which these truthiness tests
         * count as found, so this nearly always gives 1. */
        if (AiFindHandCardByNumber(1, CARD_1221))
            return 1;
        r = AiFindHandCardByNumber(1, CARD_DELINQUENT_DUO);
        goto nonzero;
    case CARD_GIFT_OF_THE_MYSTICAL_ELF:
        /* More than two face-up monsters on the field. */
        {
            int t = CountMonstersFiltered(0, 1, 0);

            t += CountMonstersFiltered(1, 1, 0);
            if (t > 2)
                return 1;
        }
        return 0;
    case CARD_MIRROR_FORCE:
        /* zone = entry->loc0, used as the attacker's player number (see CallZoneZero). Go if its ATK, or its
         * side's total ATK, reaches the CPU's life points; if the CPU has no monster and the human has two
         * or more; or if its side's total ATK is at least the other side's. */
        zone = entry->loc0;
        if (CallZoneZero(zone) >= gDuelPlayers[1].lifePoints)
            return 1;
        if (CountMonsters(1) == 0 && CountMonsters(0) > 1)
            return 1;
        a = SumMonsterAtk(zone);
        if (a >= gDuelPlayers[1].lifePoints)
            return 1;
        if (a >= SumMonsterAtk(1 - zone))
            return 1;
        return 0;
    case CARD_CEASEFIRE:
        /* 500 damage per Effect Monster: count them on both fields. */
        a = 0;
        for (zone = 0; zone <= 1; zone++) {
            for (z = 0; z < MONSTER_ZONE_COUNT; z++) {
                u32 id = CARD_ID_OF(ZONE_AT_PLAYER_FIRST(zone & 1, z)->card);

                if (id != 0 && IsEffectMonster(id))
                    a++;
            }
        }
        if (gDuelPlayers[0].lifePoints < a * 500)
            return 1;
        /* Otherwise go if the human has at least two more Effect Monsters than the CPU. */
        a = 0;
        for (z = 0; z < MONSTER_ZONE_COUNT; z++) {
            if (CARD_ID_OF(ZONE_AT(0, z)->card) != 0 && IsEffectMonster(CARD_ID_OF(ZONE_AT(0, z)->card)))
                a++;
            if (CARD_ID_OF(ZONE_AT(1, z)->card) != 0 && IsEffectMonster(CARD_ID_OF(ZONE_AT(1, z)->card)))
                a--;
        }
        if (a > 1)
            return 1;
        return 0;
    case CARD_AQUA_CHORUS:
        /* A face-up CPU monster with a same-name partner on the field. */
        for (zz = 0; zz < MONSTER_ZONE_COUNT; zz++) {
            struct DuelZone *monster = ZONE_AT(1, zz);

            if (!CARD_EMPTY(monster->card) && (monster->isFaceUp)) {
                if (CountOtherFaceUpSameNameMonsters(1, zz) > 0)
                    return 1;
            }
        }
        return 0;
    case CARD_1214:
        /* a = the ATK of zone 0 of the side loc0 names (see CallZoneZero). Go if it reaches half the human's
         * life points or all of the CPU's, if the human would still have more life points than the CPU
         * after taking it, or if the human has exactly one monster. */
        zone = entry->loc0;
        a = CallZoneZero(zone);
        if (a >= gDuelPlayers[0].lifePoints / 2)
            return 1;
        if (a >= gDuelPlayers[1].lifePoints)
            return 1;
        if (gDuelPlayers[0].lifePoints - a > gDuelPlayers[1].lifePoints)
            return 1;
        if (CountMonsters(0) == 1)
            return 1;
        return 0;
    case CARD_1425:
        r = CountMonsters(0);
    nonzero:
        if (r != 0)
            return 1;
        return 0;
    }
    return 0;
}

/* Find the first face-down, activatable set card with card number `number` in player 1's spell/trap zones
 * (5-9) and activate it in answer to the same event as `entry`: queue the chain banner, flip the card, and
 * add a chain link {player 1, kind spell/trap, the event of entry, that zone, the card ID} with the two
 * event locations of entry. Returns 1 if there was such a card, else 0. */
u16 AiChainSetCard(struct ChainEntry *entry, int number)
{
    int i;

    for (i = ZONE_SPELL_0; i <= ZONE_SPELL_4; i++) {
        struct DuelZone *zone = ZONE_AT(1, i);
        u32 id = CARD_ID_OF(zone->card);

        if (id != 0 && !zone->isFaceUp && ZONE_CAN_ACTIVATE(zone) && CARD_NUMBER(id) == number) {
            u32 eventBits;
            u32 zoneBits;

            DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_CHAIN_BANNER, 0, 0, 0);
            FlipFieldCard(1, i, 0);
            eventBits = entry->event << 25;
            zoneBits = (i & 0x1F) << 16;
            zoneBits |= (1u << 31) | (CHAIN_KIND_SPELL_TRAP << 21);     /* player 1, kind spell/trap */
            Chain_AddLink(eventBits | zoneBits | CARD_ID_OF(zone->card), (entry->loc1 << 16) | entry->loc0);
            return 1;
        }
    }
    return 0;
}

/* AI: the CPU's answer to the chain link `entry`: returns 1 if it chained a set card (AiChainSetCard).
 * By the card number of entry->card, for the human's cards (entry->player == 0) unless noted:
 *  - Field Magic and LP-recovery cards: never answered (0).
 *  - Dark Hole -> White Hole; Raigeki -> Anti Raigeki; Monster Reborn -> Call of the Grave or Call of the Dark.
 *  - Harpie's Feather Duster: nothing without set cards; else Gryphon Wing, then Magic Jammer (two or more set
 *    cards and a hand card), then any set Reinforcements, Castle Walls, Rush Recklessly, The Reliable
 *    Guardian, Snake Fang, Graceful Dice, Skull Dice or key 1447.
 *  - De-Spell / Mystical Space Typhoon aimed at Black Pendant or Magic Jammer: no answer.
 *  - Swords of Revealing Light -> Ceasefire when 500 per Effect Monster exceeds the human's life points, or by
 *    chance (no player check).
 *  - Burn cards (the thresholds below): answered only when they would be lethal.
 * Then by type, for the human's cards: Magic -> Imperial Order, then Magic Jammer (if the CPU has a hand card);
 * Trap -> Royal Decree, then Seven Tools of the Bandit (if the CPU's life points exceed 1000). Last, any link
 * may be answered with key 1447. */
int AiTryChainResponse(struct ChainEntry *entry)
{
    int a;
    int z;
    int zone;
    int number;
    int x;
    u32 m;

    /* FAKEMATCH: the 0x7FF mask lives in a variable that the Magic Thorn case reuses for its whole index
     * chain, so the mask, switch value and table base get r1/r2/r3 as in the ROM. */
    m = CARD_ID_MASK;
    number = ((const u16 *)0x08622AB4)[entry->card & m];
    switch (number) {
    case CARD_FOREST:
    case CARD_WASTELAND:
    case CARD_MOUNTAIN:
    case CARD_SOGEN:
    case CARD_UMI:
    case CARD_YAMI:
    case CARD_MOOYAN_CURRY:
    case CARD_RED_MEDICINE:
    case CARD_GOBLINS_SECRET_REMEDY:
    case CARD_SOUL_OF_THE_PURE:
    case CARD_DIAN_KETO_THE_CURE_MASTER:
    case CARD_BLUE_MEDICINE:
    case CARD_CHORUS_OF_SANCTUARY:
    case CARD_GAIA_POWER:
    case CARD_UMIIRUKA:
    case CARD_MOLTEN_DESTRUCTION:
    case CARD_RISING_AIR_CURRENT:
    case CARD_LUMINOUS_SPARK:
    case CARD_MYSTIC_PLASMA_ZONE:
        return 0;
    case CARD_DARK_HOLE:
        if (entry->player)
            return 0;
        if (AiChainSetCard(entry, CARD_WHITE_HOLE))
            return 1;
        break;
    case CARD_RAIGEKI:
        if (entry->player)
            return 0;
        if (AiChainSetCard(entry, CARD_ANTI_RAIGEKI))
            return 1;
        break;
    case CARD_SWORDS_OF_REVEALING_LIGHT:
        /* a = Effect Monsters on both fields. */
        a = 0;
        for (zone = 0; zone <= 1; zone++) {
            for (z = 0; z < MONSTER_ZONE_COUNT; z++) {
                u32 id = CARD_ID_OF(ZONE_AT_PLAYER_FIRST(zone & 1, z)->card);

                if (id != 0 && IsEffectMonster(id))
                    a++;
            }
        }
        if (gDuelPlayers[0].lifePoints < a * 500) {
            if (AiChainSetCard(entry, CARD_CEASEFIRE))
                return 1;
        }
        if (a > Random() % 3 + 1) {
            if (AiChainSetCard(entry, CARD_CEASEFIRE))
                return 1;
        }
        break;
    case CARD_MONSTER_REBORN:
        if (entry->player)
            return 0;
        if (AiChainSetCard(entry, CARD_CALL_OF_THE_GRAVE))
            return 1;
        if (AiChainSetCard(entry, CARD_CALL_OF_THE_DARK))
            return 1;
        break;
    case CARD_HARPIES_FEATHER_DUSTER:
        if (entry->player)
            return 0;
        if (!CountSpellTrapsFiltered(1, 0, 0, 0))
            return 0;
        if (AiChainSetCard(entry, CARD_GRYPHON_WING))
            return 1;
        if (CountSpellTrapsFiltered(1, 0, 0, 0) > 1 && gDuelPlayers[1].handCount != 0
         && AiChainSetCard(entry, CARD_MAGIC_JAMMER))
            return 1;
        for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
            struct DuelZone *setCard = ZONE_AT(1, zone);
            u16 id = CARD_ID_OF(setCard->card);

            if (setCard->isFaceUp)
                continue;
            switch (CARD_NUMBER(id)) {
            case CARD_REINFORCEMENTS:
            case CARD_CASTLE_WALLS:
            case CARD_RUSH_RECKLESSLY:
            case CARD_THE_RELIABLE_GUARDIAN:
            case CARD_SNAKE_FANG:
            case CARD_GRACEFUL_DICE:
            case CARD_SKULL_DICE:
            case CARD_1447:
                if (AiChainSetCard(entry, CARD_NUMBER(id)))
                    return 1;
                break;
            }
        }
        return 0;
    case CARD_DE_SPELL:
    case CARD_MYSTICAL_SPACE_TYPHOON: {
        u16 target;
        /* FAKEMATCH: a hard-register copy makes reload tie the constant 1 (not target) to the and's output,
         * and the empty asm keeps it live so the (u8) zero-extension gets a fresh register. */
        register int targetPlayer asm("r3");
        int targetPlayerByte;

        if (entry->player)
            return 0;
        target = entry->targets[0];     /* player | zone << 8 of the card aimed at */
        targetPlayer = target;
        if (CARD_EMPTY(ZONE_AT_PLAYER_FIRST(targetPlayer & 1, target >> 8)->card))
            break;
        targetPlayerByte = (u8)targetPlayer;
        asm("" : : "r"(targetPlayer));
        x = CARD_NUMBER(CARD_ID_OF(ZONE_AT_PLAYER_FIRST(targetPlayerByte & 1, target >> 8)->card));
        if (x == CARD_BLACK_PENDANT || x == CARD_MAGIC_JAMMER)
            return 0;
        break;
    }
    case CARD_MAGIC_THORN:
        /* ROM quirk: this indexes gCardIdToNumber with the player bit instead of the card ID, so x is the
         * number of card ID 0 or 1 (0xFFFF or 439), neither of which matches: the case always returns 0. */
        m &= entry->player;
        m <<= 1;
        m += 0x08622AB4;
        x = *(u16 *)m;
        if (x == CARD_MORPHING_JAR)
            return 1;
        if (x != CARD_1221)
            return 0;
        if (gDuelPlayers[0].handCount != 0)
            return 1;
        return 0;
    /* The five burn cards' damage as LP thresholds: answer only when the CPU's life points are at or below it.
     * They are keyed by the numbers 200 and 227-230, whereas the burn Magic cards Sparks (200 damage),
     * Hinotama (500), Final Flame (600), Ookazi (800) and Tremendous Fire (1000) are numbers 342-346
     * (hypothesis: stale card numbering, so these cases do not apply to those cards). */
    case CARD_FRENZIED_PANDA:
        if (gDuelPlayers[1].lifePoints > 200)
            return 0;
        break;
    case CARD_WOOD_REMAINS:
        if (gDuelPlayers[1].lifePoints > 500)
            return 0;
        break;
    case CARD_HOURGLASS_OF_LIFE:
        if (gDuelPlayers[1].lifePoints > 600)
            return 0;
        break;
    case CARD_RARE_FISH:
        if (gDuelPlayers[1].lifePoints > 800)
            return 0;
        break;
    case CARD_230:
        if (gDuelPlayers[1].lifePoints > 1000)
            return 0;
        break;
    case CARD_RAIMEI:
        if (gDuelPlayers[1].lifePoints > 300)
            return 0;
        break;
    case CARD_RESTRUCTER_REVOLUTION:
        /* 200 damage per card in the CPU's hand. */
        if (gDuelPlayers[1].lifePoints > gDuelPlayers[1].handCount * 200)
            return 0;
        break;
    }
    switch ((int)CARD_TYPE(entry->card)) {
    case CARD_TYPE_MAGIC:
        if (!(entry->player)) {
            if (AiChainSetCard(entry, CARD_IMPERIAL_ORDER))
                return 1;
            if (gDuelPlayers[1].handCount != 0) {
                if (AiChainSetCard(entry, CARD_MAGIC_JAMMER))
                    return 1;
            }
        }
        break;
    case CARD_TYPE_TRAP:
        if (!(entry->player)) {
            if (AiChainSetCard(entry, CARD_ROYAL_DECREE))
                return 1;
            if (gDuelPlayers[1].lifePoints > 1000) {
                if (AiChainSetCard(entry, CARD_SEVEN_TOOLS_OF_THE_BANDIT))
                    return 1;
            }
        }
        break;
    }
    if (AiChainSetCard(entry, CARD_1447))
        return 1;
    return 0;
}

/* Card number to card ID: 0xFFFF = none (ID 0); numbers below 2000 through gCardNumberToId; 2000 + n is the
 * alternate art of card n, whose ID is n's ID + 1. */
static inline u16 CardNumberToId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number < CARD_NUMBER_ALT_ART)
        return CARD_ID_OF_NUMBER(number);
    return CARD_ID_OF_NUMBER(number - CARD_NUMBER_ALT_ART) + 1;
}

#define PLAYER(p) gDuelPlayers[(p) & 1]

/* Put the card with card number `number` on top of player `player`'s deck: a Fusion monster at the front of
 * the fusion deck (+0xA44, fusionCount), any other card at the front of the deck (+0x7C4, deckCount; deck[0]
 * is the top). Numbers above 0xFFF are ignored. The list is shifted up one slot with CopyDuelCard, then only
 * the card ID and owner bits of slot 0 are written (its other bits keep what the previous top card had).
 * Used to build the CPU's deck from the deck lists and to put the start-of-duel Field Magic on top. */
void AddCardNumberToDeckTop(int player, u16 number)
{
    u16 id;
    int kind;
    int n;
    struct DuelCard *front;

    id = CardNumberToId(number);
    if (number > 0xFFF)
        return;
    /* Monsters only: the Magic/Trap/Ticket cases of the kind classifier below are in the ROM's code but can
     * never be reached after this test. */
    if (CARD_TYPE(id) <= CARD_TYPE_REPTILE) {
        switch (CARD_NUMBER(id)) {
        case CARD_OBELISK_THE_TORMENTOR:
            kind = CARD_KIND_RITUAL;
            break;
        case CARD_SLIFER_THE_SKY_DRAGON:
        case CARD_THE_WINGED_DRAGON_OF_RA:
            kind = CARD_KIND_EFFECT;
            break;
        default:
            switch ((int)CARD_TYPE(id)) {
            case CARD_TYPE_MAGIC:
                kind = CARD_KIND_MAGIC;
                break;
            case CARD_TYPE_TRAP:
                kind = CARD_KIND_TRAP;
                break;
            case CARD_TYPE_TICKET:
                kind = CARD_KIND_TICKET;
                break;
            default:
                kind = CARD_STATS_KIND(CARD_STATS(id));
                break;
            }
            break;
        }
        if (kind == CARD_KIND_FUSION) {
            for (n = PLAYER(player).fusionCount; n > 0; n--)
                CopyDuelCard((u32 *)&PLAYER(player).fusionDeck[n], (u32 *)&PLAYER(player).fusionDeck[n - 1]);
            PLAYER(player).fusionCount++;
            front = &PLAYER(player).fusionDeck[0];
            front->id = id;
            front->owner = player;
            return;
        }
    }
    for (n = PLAYER(player).deckCount; n > 0; n--)
        CopyDuelCard((u32 *)&PLAYER(player).deck[n], (u32 *)&PLAYER(player).deck[n - 1]);
    PLAYER(player).deckCount++;
    front = &PLAYER(player).deck[0];
    front->id = id;
    front->owner = player;
}
