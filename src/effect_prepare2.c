/*
 * effect_prepare2 (0x0802EB58-0x0802FB63): card effect activation conditions, part 2
 * (wiki/functions/effect-prepare2-c.md; part 1 is effect_prepare1.c).
 *
 * 31 of the 33 functions are prepare handlers of gCardEffects (include/effect.h): side-effect-free "may this
 * effect be activated now?" tests that CanActivateEffect runs before the per-position Check handler. They
 * are called as f(card, chainLink, fromHand): card is the activating card's chain entry with the event that
 * triggered it (card->event, loc0, loc1), chainLink the opponent's link being answered (NULL for none), and
 * fromHand is 1 when the card is played from the hand. Handlers that ignore the trailing arguments are
 * defined with fewer parameters (include/effect_handlers.h). They return 1 when the effect may be used.
 *
 * The other two, CanRedirectEffectToZone and CountRedirectTargets, decide where Fairy's Hand Mirror (1073)
 * and effect key 1317 can move a single-target effect.
 *
 * Card numbers 1211-1552 are effect keys with no EDS card (CARD_<n>); the effect_handlers.h comments name the
 * OCG cards some of them behave like. Zones 0-4 hold monsters, 5-9 spells and traps, 10 the Field Magic.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, SpellSubtype */
#include "constants/duel.h"         /* enum DuelZoneIndex, DuelPhase, ResponseEventKind, DUEL_LOC_* */
#include "duel.h"                   /* gDuel, gDuelPlayers, duel rules and helpers */
#include "chain.h"                  /* struct ChainEntry */
#include "summon.h"                 /* CanSpecialSummon */
#include "effect.h"                 /* CanRedirectEffectToZone, CountRedirectTargets, CanEffectTargetZone */
#include "effect_handlers.h"        /* the prepare handlers defined here */

/* Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view"). */
/* Matching: CollectEffectTargets is defined (u16 return, u16 number); this unit calls it with an int number
 * and compares the result as an int (cmp; ble), with no narrowing at the call. */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: EffectCallOfTheHauntedPrepare is defined with one parameter and an int return; Premature Burial
 * passes its three arguments on and narrows the result to u16 (lsl #16; lsr #16). */
extern u16 EffectCallOfTheHauntedPrepare3(struct ChainEntry *card, int chainLink, u16 fromHand)
    asm("EffectCallOfTheHauntedPrepare");

/* The card word of a zone or pile slot as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with lsl #20; lsr #20; a bitfield read of .id loads only the halfword. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: the ROM reloads the table address at each use, which the symbols do not give.
 */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))         /* enum CardType; <= 20 is a monster */
#define CARD_SUBTYPE(id) CARD_STATS_SUBTYPE(CARD_STATS(id))   /* enum SpellSubtype (Magic and Trap cards) */

/*
 * &gDuelZones[player].zones[zone] by byte arithmetic, from three base symbols for the same address: the ROM
 * builds the pointer zone term first, then the player term, then adds the base. Callers pass player & 1.
 * ZONE_AT uses gDuelZones, ZONE_AT_P gDuelPlayers->zones and ZONE_AT_D gDuel.players->zones; each function
 * keeps the form it was matched with.
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
#define ZONE_AT_P(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) \
                         + (u32)gDuelPlayers->zones))
#define ZONE_AT_D(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) \
                         + (u32)gDuel.players->zones))

/* DuelZone +0x06 (isDefense bit 0, isFaceUp bit 1) as a whole byte. Matching: the ROM tests the flags with
 * ldrb and an and-mask; the 1-bit fields give lsl/lsr pairs. */
#define ZONE_POSITION_BYTE(zone) (((u8 *)(zone))[6])
#define ZONE_POSITION_DEFENSE 0x1   /* isDefense */
#define ZONE_POSITION_FACE_UP 0x2   /* isFaceUp */

/* Byte views of a chain entry for the two tests that the ROM does with ldrb. Matching: entry->event (a u16
 * bitfield) gives ldrh and other shifts in Sebek's Blessing, and entry->loc1 & 0xF an ldrh. */
#define ENTRY_EVENT_BYTE(entry) (((u8 *)(entry))[3] >> 2)   /* ChainEntry.event, enum ResponseEventKind */
#define LOC1_LOW_BYTE(entry) (((u8 *)(entry))[8])           /* DUEL_LOC_PLAYER(loc1) */

/* Earthshaker (1097): the opponent has a face-up monster. */
int EffectEarthshakerPrepare(struct ChainEntry *card)
{
    int zone;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        int side = (1 - card->player) & 1;
        struct DuelZone *slot = ZONE_AT(side, zone);

        if (CARD_ID(CARD_WORD(slot->card)) != 0) {
            /* Matching: a fresh pointer for the flag read (the ROM recomputes it). */
            struct DuelZone *flags;

            side = (1 - card->player) & 1;
            flags = ZONE_AT(side, zone);
            if (ZONE_POSITION_BYTE(flags) & ZONE_POSITION_FACE_UP)
                return 1;
        }
    }
    return 0;
}

/*
 * Event-triggered traps: does the event that card answers (card->event, loc0, loc1) or the link it is
 * chained to fit this trap? Never from the hand. "Own" is card->player; loc0 and loc1 hold the player of
 * the card concerned (or the damaged player) in their low byte.
 * The shared ret1/ret0 labels only pin the ROM's cross-jump survivors (block layout).
 */
int EffectEventResponsePrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    u8 loc0Player = DUEL_LOC_PLAYER(card->loc0);
    u8 loc1Player = DUEL_LOC_PLAYER(card->loc1);
    u16 number;
    u32 stats;

    if (fromHand != 0)
        return 0;
    number = CARD_NUMBER(card->card);
    switch (number) {
    case CARD_GUST:                     /* own Magic card destroyed */
        if (loc0Player != card->player)
            return 0;
        return card->event == RESPONSE_MAGIC_TO_GRAVE;
    case CARD_DRIVING_SNOW:             /* own Trap card destroyed */
        if (loc0Player != card->player)
            return 0;
        return card->event == RESPONSE_TRAP_TO_GRAVE;
    case CARD_ARMORED_GLASS:            /* a monster was equipped */
        return card->event == RESPONSE_EQUIP;
    case CARD_WORLD_SUPPRESSION:        /* answering a Field Magic, or a Field Magic was played */
        if (chainLink != NULL) {
            stats = CARD_STATS(chainLink->card);
            if (CARD_STATS_TYPE(stats) == CARD_TYPE_MAGIC && CARD_STATS_SUBTYPE(stats) == SPELL_FIELD)
                goto ret1;
        }
        return card->event == RESPONSE_FIELD_MAGIC_PLAYED;
    case CARD_MYSTIC_PROBE:             /* answering a Continuous Magic */
        if (chainLink == NULL)
            return 0;
        stats = CARD_STATS(chainLink->card);
        if (CARD_STATS_TYPE(stats) != CARD_TYPE_MAGIC)
            return 0;
        if (CARD_STATS_SUBTYPE(stats) == SPELL_CONTINUOUS)
            goto ret1;
        return 0;
    case CARD_METAL_DETECTOR:           /* answering a Continuous Trap (the EDS text says Magic) */
        if (chainLink == NULL)
            return 0;
        stats = CARD_STATS(chainLink->card);
        if (CARD_STATS_TYPE(stats) != CARD_TYPE_TRAP)
            return 0;
        if (CARD_STATS_SUBTYPE(stats) == SPELL_CONTINUOUS)
            goto ret1;
        return 0;
    case CARD_NUMINOUS_HEALER:          /* own player took damage */
    case CARD_1304:
        switch (card->event) {
        case RESPONSE_LP_CHANGE:        /* loc1: the damaged player */
            if (DUEL_LOC_PLAYER(card->loc1) != card->player)
                return 0;
        ret1:
            return 1;
        case RESPONSE_BATTLE_DAMAGE:    /* loc1 low nibble: the damaged player */
        case RESPONSE_BATTLE_DEFLECTED_DAMAGE:
            if ((LOC1_LOW_BYTE(card) & 0xF) == card->player)
                goto ret1;
            return 0;
        }
        return 0;
    case CARD_APPROPRIATE:              /* the opponent drew, and has no active Appropriate */
        if (loc0Player == card->player)
            return 0;
        if (CountActiveCardsOnFieldExcept(1 - card->player, number, -1) > 0)
            return 0;
        return card->event == RESPONSE_DREW;
    case CARD_FORCED_REQUISITION:       /* own discard, and the opponent has no active Forced Requisition */
        if (loc0Player != card->player)
            return 0;
        if (CountActiveCardsOnFieldExcept(1 - card->player, number, -1) > 0)
            return 0;
        return card->event == RESPONSE_DISCARDED;
    case CARD_MAJOR_RIOT:               /* own monster returned to the hand */
        if (loc0Player != card->player)
            return 0;
        if (card->event == RESPONSE_MONSTER_TO_HAND)
            goto ret1;
        return 0;
    case CARD_1301:                     /* own monster sent to the graveyard, or a battle on either own side */
        switch (card->event) {
        case RESPONSE_MONSTER_TO_GRAVE:
            if (loc0Player != card->player)
                goto ret0;
            goto ret1;
        case RESPONSE_BATTLE_DESTROYED:
            if (loc0Player == card->player || loc1Player == card->player)
                goto ret1;
            return 0;
        }
        return 0;
    }
ret0:
    return 0;
}

/* Backup Soldier (1147): at least 5 monster cards in the own graveyard. */
int EffectBackupSoldierPrepare(struct ChainEntry *card)
{
    int i;
    int monsters = 0;

    for (i = 0; i < gDuelPlayers[1 & card->player].graveCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & card->player].graveyard[i]));

        if (CARD_TYPE(id) <= CARD_TYPE_REPTILE)
            monsters++;
        if (monsters > 4)
            return 1;
    }
    return 0;
}

/* Ceasefire (1150): either player has a face-down monster. */
int EffectCeasefirePrepare(struct ChainEntry *card)
{
    int zone, player;

    for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
        for (player = 0; player <= 1; player++) {
            /* Matching: the masked player as its own statement. */
            int side = player & 1;
            struct DuelZone *slot = ZONE_AT(side, zone);

            if (CARD_ID(CARD_WORD(slot->card)) != 0 && !(ZONE_POSITION_BYTE(slot) & ZONE_POSITION_FACE_UP))
                return 1;
        }
    }
    return 0;
}

/* The Shallow Grave (1159): Special Summons allowed, a free monster zone on either side and a graveyard
 * monster (CollectEffectTargets). The target count does not depend on the loop variable. */
int EffectTheShallowGravePrepare(struct ChainEntry *card)
{
    int player;

    if (CanSpecialSummon(card->player) == 0)
        return 0;
    for (player = 0; player <= 1; player++) {
        if (CountFreeMonsterZones(player) > 0
            && CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) != 0)
            return 1;
    }
    return 0;
}

/* Premature Burial (1160): 800 LP for the cost, then the Call of the Haunted conditions (Special Summons
 * allowed, a free zone, a graveyard monster). */
int EffectPrematureBurialPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (gDuelPlayers[card->player].lifePoints < 800)
        return 0;
    return EffectCallOfTheHauntedPrepare3(card, chainLink, fromHand);
}

/* Sebek's Blessing (1179): an own monster dealt battle damage (the damaged player, the low nibble of loc1,
 * is the opponent). */
int EffectSebeksBlessingPrepare(struct ChainEntry *card)
{
    if (ENTRY_EVENT_BYTE(card) == RESPONSE_BATTLE_DAMAGE && (LOC1_LOW_BYTE(card) & 0xF) != card->player)
        return 1;
    return 0;
}

/* Riryoku (1181): at least two face-up monsters on the field. */
int EffectRiryokuPrepare(void)
{
    int faceUp = CountMonstersFiltered(0, TRUE, FALSE);

    faceUp += CountMonstersFiltered(1, TRUE, FALSE);
    if (faceUp <= 1)
        return 0;
    return 1;
}

/* Seal of the Ancients (1183): 1000 LP for the cost and a face-down card in any of the opponent's zones. */
int EffectSealOfTheAncientsPrepare(struct ChainEntry *card)
{
    int zone;

    if (gDuelPlayers[card->player].lifePoints < 1000)
        return 0;
    for (zone = ZONE_MONSTER_0; zone <= ZONE_FIELD; zone++) {
        int side = (1 - card->player) & 1;
        struct DuelZone *slot = ZONE_AT_P(side, zone);

        if (CARD_ID(CARD_WORD(slot->card)) != 0) {
            /* Matching: a fresh pointer for the flag read (the ROM recomputes it). */
            struct DuelZone *flags;

            side = (1 - card->player) & 1;
            flags = ZONE_AT_P(side, zone);
            if (!(ZONE_POSITION_BYTE(flags) & ZONE_POSITION_FACE_UP))
                return 1;
        }
    }
    return 0;
}

/* Graceful Dice (1203): the player has a face-up monster. */
int EffectGracefulDicePrepare(struct ChainEntry *card)
{
    return CountMonstersFiltered(card->player, TRUE, FALSE) > 0;
}

/* Skull Dice (1204): the opponent has a face-up monster. */
int EffectSkullDicePrepare(struct ChainEntry *card)
{
    return CountMonstersFiltered(1 - card->player, TRUE, FALSE) > 0;
}

/* Exchange (1205): the opponent has a hand card and the player keeps one besides this card (fromHand is 1
 * when Exchange itself is in the hand). */
int EffectExchangePrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    struct DuelPlayer *players = gDuelPlayers;

    if (players[(1 - card->player) & 1].handCount != 0 && players[1 & card->player].handCount - fromHand > 0)
        return 1;
    return 0;
}

/* Key 1211: the opponent has a monster and the player an active Dark Magician (either art, or the
 * non-EDS variant 1210). */
int EffectDarkMagicianOnFieldPrepare(struct ChainEntry *card)
{
    if (CountMonstersFiltered(1 - card->player, FALSE, FALSE) == 0
        || (CountActiveCardsOnField(card->player, CARD_DARK_MAGICIAN) <= 0
            && CountActiveCardsOnField(card->player, CARD_1210) <= 0
            && CountActiveCardsOnField(card->player, CARD_DARK_MAGICIAN_ALT) <= 0))
        return 0;
    return 1;
}

/* Key 1212: a free monster zone, Special Summons allowed, the Normal Summon unused and a Dark Magician in
 * the deck (CollectEffectTargets). */
int EffectDarkMagicianInDeckPrepare(struct ChainEntry *card)
{
    if (CountFreeMonsterZones(card->player) == 0 || CanSpecialSummon(card->player) == 0
        || gDuelPlayers[card->player].normalSummonUsed)
        return 0;
    return CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) > 0;
}

/* Key 1213: both players have a monster. */
int EffectBothPlayersHaveMonsterPrepare(struct ChainEntry *card)
{
    if (CountMonstersFiltered(1 - card->player, FALSE, FALSE) != 0
        && CountMonstersFiltered(card->player, FALSE, FALSE) != 0)
        return 1;
    return 0;
}

/*
 * Key 1220: Main Phase 1, the player has a monster, and no key 1418 (forbids tributes) is active. Then 1
 * if the player has a face-up Catapult Turtle, The Little Swordsman of Aile or Cannon Soldier (they use the
 * tribute); otherwise the Normal Summon must be unused and the hand must hold a summonable monster of level
 * 5 or more: level 5-6 needs a free zone (the opponent's monster is the one tribute), level 7+ one own
 * tribute besides it.
 */
int EffectCanTributeOpponentMonsterPrepare(struct ChainEntry *card)
{
    int i;
    u16 level;

    if (gDuel.phase != PHASE_MAIN1)
        return 0;
    if (CountMonsters(card->player) == 0)
        return 0;
    if (CountActiveCardsOnField(0, CARD_1418) > 0 || CountActiveCardsOnField(1, CARD_1418) > 0)
        return 0;
    for (i = ZONE_MONSTER_0; i <= ZONE_MONSTER_4; i++) {
        int side = 1 & card->player;
        struct DuelZone *slot = ZONE_AT_D(side, i);
        u16 id = CARD_ID(CARD_WORD(slot->card));

        if (id != 0) {
            /* Matching: a fresh pointer for the flag read (the ROM recomputes it). */
            int side2 = 1 & card->player;
            struct DuelZone *flags = ZONE_AT_D(side2, i);

            if (ZONE_POSITION_BYTE(flags) & ZONE_POSITION_FACE_UP) {
                switch (CARD_NUMBER(id)) {
                case CARD_CATAPULT_TURTLE:
                case CARD_THE_LITTLE_SWORDSMAN_OF_AILE:
                case CARD_CANNON_SOLDIER:
                    return 1;
                }
            }
        }
    }
    if (gDuelPlayers[1 & card->player].normalSummonUsed)
        return 0;
    for (i = 0; i < gDuelPlayers[1 & card->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & card->player].hand[i]));

        if (CARD_TYPE(id) <= CARD_TYPE_REPTILE && IsSpecialSummonOnly(id) == 0) {
            /* The printed level: 0 for Trap, Magic and Ticket cards, 10 for the Egyptian Gods, else the
             * stars. Computed again below for the level 5-6 / 7+ test. */
            switch ((int)CARD_TYPE(id)) {
            case CARD_TYPE_TRAP:
            case CARD_TYPE_MAGIC:
            case CARD_TYPE_TICKET:
                level = 0;
                break;
            case CARD_TYPE_DIVINE:
                level = 10;
                break;
            default:
                level = CARD_STATS_LEVEL(CARD_STATS(id));
                break;
            }
            if (level > 4) {
                switch ((int)CARD_TYPE(id)) {
                case CARD_TYPE_TRAP:
                case CARD_TYPE_MAGIC:
                case CARD_TYPE_TICKET:
                    /* FAKEMATCH (permuter): dead store. */
                    level = CARD_STATS_LEVEL(CARD_STATS(id));
                    level = 0;
                    break;
                case CARD_TYPE_DIVINE:
                    level = 10;
                    break;
                default:
                    level = CARD_STATS_LEVEL(CARD_STATS(id));
                    break;
                }
                if (level <= 6) {
                    if (FindFreeMonsterZone(card->player) != -1)
                        return 1;
                } else if (CountTributableMonsters(card->player, -1) > 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Key 1232: no key 1418 (forbids tributes) active and the player has a face-up Kuriboh. */
int EffectFaceUpKuribohPrepare(struct ChainEntry *card)
{
    if (CountActiveCardsOnField(0, CARD_1418) > 0 || CountActiveCardsOnField(1, CARD_1418) > 0
        || CountFaceUpMonstersByNumber(card->player, CARD_KURIBOH) <= 0)
        return 0;
    return 1;
}

/* Key 1245: no summon this turn and at least 4 free monster zones. */
int EffectNoSummonFourFreeZonesPrepare(struct ChainEntry *card)
{
    struct DuelPlayer *players = gDuelPlayers;
    u32 shifted = (u32)((u8 *)card)[2] << 31;   /* card->player in bit 31 */

    /* byte +0x08 << 26: bit 5 (summonedThisTurn) is the sign bit */
    if ((s32)((u32)((u8 *)&players[shifted >> 31])[8] << 26) >= 0) {
        /* FAKEMATCH: re-extract the player bit for the call as the ROM does (plain C reuses the
         * extracted player register). */
        __asm__("" : "+r"(shifted));
        if (CountFreeMonsterZones(shifted >> 31) > 3)
            return 1;
    }
    return 0;
}

/* Key 1248: Main Phase 1 only. */
int EffectMainPhase1Prepare(void)
{
    if (gDuel.phase == PHASE_MAIN1)
        return 1;
    return 0;
}

/* Key 1302: the opponent has 3000 LP or less. */
int EffectOpponentLifeAtMost3000Prepare(struct ChainEntry *card)
{
    int result = 0;
    struct DuelPlayer *players = gDuelPlayers;
    int opponent = card->player;

    opponent ^= 1;
    if (players[opponent].lifePoints <= 3000)
        result = 1;
    return result;
}

/* Key 1303: not from the hand; the opponent holds 6 or more cards and the player 2 or fewer. */
int EffectHandDisadvantagePrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (fromHand == 0 && gDuelPlayers[(1 - card->player) & 1].handCount > 5
        && gDuelPlayers[1 & card->player].handCount <= 2)
        return 1;
    return 0;
}

/* Key 1312: the start of Main Phase 1: no Magic/Trap activated and no Normal Summon yet this turn. */
int EffectStartOfMainPhase1Prepare(struct ChainEntry *card)
{
    if (gDuel.phase == PHASE_MAIN1) {
        struct DuelPlayer *players = gDuel.players;
        /* FAKEMATCH: shifted pinned to r3, the register the ROM keeps the shifted player byte in. */
        register u32 shifted __asm__("r3") = (u32)((u8 *)card)[2] << 31;   /* card->player in bit 31 */
        int one = 1;

        /* FAKEMATCH: keep the initialized mask as a separate register
         * value rather than folding it into the one-bit player range. */
        __asm__("" : "+r"(one));
        /* byte +0x09 << 26: bit 5 (magicTrapActivatedThisTurn) is the sign bit */
        if ((s32)((u32)((u8 *)&players[shifted >> 31])[9] << 26) < 0)
            return 0;
        /* FAKEMATCH: preserve the original shifted value through both
         * flag tests, so each player extraction uses the ROM's r0. */
        __asm__("" : : "r"(shifted));
        /* byte +0x08 << 27: bit 4 (normalSummonUsed) is the sign bit */
        if ((s32)((u32)((u8 *)&players[one & (shifted >> 31)])[8] << 27) < 0)
            return 0;
        __asm__("" : : "r"(shifted));
        return 1;
    }
    return 0;
}

/* Key 1316: the player has a monster and another hand card besides this one. */
int EffectOwnMonsterAndHandCardPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (CountMonsters(card->player) != 0 && gDuelPlayers[card->player].handCount != fromHand)
        return 1;
    return 0;
}

/*
 * May the single-target effect of link move onto (player, zone)? For Fairy's Hand Mirror the link must be
 * a Magic card. The link's card must be one of the single-target cards listed below; the new position must
 * hold a card and differ from link->targets[0], and link's own Check handler must accept it (1 when it has
 * none). Returns that result, 0 when the effect cannot move there.
 */
u16 CanRedirectEffectToZone(u16 effectNumber, struct ChainEntry *link, int player, int zone)
{
    if (link == NULL)
        return 0;
    if (effectNumber == CARD_FAIRYS_HAND_MIRROR) {
        u32 mask = CARD_ID_MASK;
        /* FAKEMATCH: the volatile read stops gcse from hoisting this load above the effectNumber test
         * (the ROM loads link->card in both places); the mask local loads the constant first. */
        u32 id = ((volatile struct ChainEntry *)link)->card & mask;

        if (CARD_TYPE(id) != CARD_TYPE_MAGIC)
            return 0;
    }
    switch (CARD_NUMBER(link->card)) {
    case CARD_LEGENDARY_SWORD ... CARD_CYBER_SHIELD:        /* 300-316: equip Magic */
    case CARD_MYSTICAL_MOON ... CARD_POWER_OF_KAISHIN:      /* 318-327: equip Magic */
    case CARD_MAGICAL_LABYRINTH:
    case CARD_SALAMANDRA:
    case CARD_MEGAMORPH:
    case CARD_BRIGHT_CASTLE:
    case CARD_7_COMPLETED:
    case CARD_BURNING_SPEAR ... CARD_GUST_FAN:
    case CARD_TRIBUTE_TO_THE_DOOMED:
    case CARD_CHANGE_OF_HEART:
    case CARD_SWORD_OF_DEEP_SEATED ... CARD_BLOCK_ATTACK:
    case CARD_GERM_INFECTION ... CARD_PARALYZING_POTION:
    case CARD_RING_OF_MAGNETISM:
    case CARD_STIM_PACK:
    case CARD_DARKNESS_APPROACHES:
    case CARD_RUSH_RECKLESSLY ... CARD_THE_RELIABLE_GUARDIAN:
    case CARD_NOBLEMAN_OF_CROSSOUT:
    case CARD_SWORD_OF_DRAGONS_SOUL:
    case CARD_1211:
    case CARD_1220:
    case CARD_1244:
    case CARD_1301:
    case CARD_1313:
    case CARD_1415:
    case CARD_1419 ... CARD_1420:
    case CARD_1422:
    case CARD_1448 ... CARD_1451:
    case CARD_1527:
    case CARD_1529:
    case CARD_1532:
    case CARD_1540:
    case CARD_1546:
    case CARD_1548:
    case CARD_1550:
    {
        int targetPlayer = DUEL_LOC_PLAYER(link->targets[0]);
        int targetZone = DUEL_LOC_ZONE(link->targets[0]);
        int side = player & 1;
        struct DuelZone *slot = ZONE_AT(side, zone);

        /* The gotos reproduce the ROM's block layout (the shared return-0 block after the card test). */
        if (CARD_ID(CARD_WORD(slot->card)) != 0)
            goto occupied;
    ret0:
        return 0;
    occupied:
        if (player == targetPlayer && zone == targetZone)
            goto ret0;
        return CanEffectTargetZone((u16 *)link, player, zone);   /* defined with a u16 * for the entry */
    }
    default:
        goto ret0;
    }
}

/* Number of positions CanRedirectEffectToZone accepts for effectNumber: both players' monster zones for
 * Fairy's Hand Mirror, player's own for key 1317, none for other numbers. */
int CountRedirectTargets(u16 effectNumber, int player, struct ChainEntry *link)
{
    int side, zone;
    int count = 0;

    switch (effectNumber) {
    case CARD_FAIRYS_HAND_MIRROR:
        for (side = 0; side <= 1; side++) {
            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                if (CanRedirectEffectToZone(effectNumber, link, side, zone))
                    count++;
            }
        }
        break;
    case CARD_1317:
        for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
            if (CanRedirectEffectToZone(effectNumber, link, player, zone))
                count++;
        }
        break;
    }
    return count;
}

/* Key 1317: answering an opponent's attack declaration with 2 or more monsters, or answering an
 * opponent's link whose single target can move to one of the player's monsters. */
int EffectCanRedirectTargetPrepare(struct ChainEntry *card, struct ChainEntry *chainLink)
{
    if (card->event == RESPONSE_ATTACK_DECLARED && DUEL_LOC_PLAYER(card->loc0) != card->player)
        return CountMonsters(card->player) > 1;
    if (chainLink == NULL || chainLink->player == card->player)
        return 0;
    return CountRedirectTargets(CARD_1317, card->player, chainLink) > 0;
}

/* Key 1320: at least 2 free monster zones on the whole field. */
int EffectTwoFreeMonsterZonesPrepare(void)
{
    return CountFreeMonsterZones(0) + CountFreeMonsterZones(1) > 1;
}

/* Key 1321: on the field, answering a Magic card other than key 1539. */
int EffectChainedToMagicPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand == 0 && chainLink != NULL) {
        u32 id = chainLink->card & CARD_ID_MASK;

        if (CARD_TYPE(id) == CARD_TYPE_MAGIC && CARD_NUMBER(id) != CARD_1539)
            return 1;
    }
    return 0;
}

/* Key 1324: on the field, with a Magic card in the hand. */
int EffectMagicCardInHandPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    int notMagic;
    int i;

    if (fromHand != 0)
        return 0;
    for (i = 0; i < gDuelPlayers[1 & card->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & card->player].hand[i]));

        /* FAKEMATCH (permuter): the comparison kept in a variable. */
        if ((notMagic = CARD_TYPE(id) != CARD_TYPE_MAGIC))
            continue;
        return 1;
    }
    return 0;
}

/* Key 1325: on the field, answering the opponent's Set of a monster that is still face down in defense
 * position (loc0 = its position). */
int EffectOpponentSetMonsterPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (fromHand == 0 && card->event == RESPONSE_SET) {
        int player = DUEL_LOC_PLAYER(card->loc0);
        int zone = DUEL_LOC_ZONE(card->loc0);

        if (player != card->player) {
            int side = player & 1;
            struct DuelZone *slot = ZONE_AT(side, zone);

            if (CARD_ID(CARD_WORD(slot->card)) != 0 && (ZONE_POSITION_BYTE(slot) & ZONE_POSITION_DEFENSE)
                && !(ZONE_POSITION_BYTE(slot) & ZONE_POSITION_FACE_UP))
                return 1;
        }
    }
    return 0;
}

/* Key 1414: a free monster zone, a hand monster that can be Normal Summoned, and two other hand cards
 * (besides this one when it is played from the hand). */
int EffectHandMonsterAndTwoCardsPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    int i;
    int others = 0;
    int monsters = 0;

    if (CountFreeMonsterZones(card->player) == 0)
        return 0;
    for (i = 0; i < gDuelPlayers[card->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gDuelPlayers[card->player].hand[i]));

        if (CARD_TYPE(id) <= CARD_TYPE_REPTILE) {
            if (IsSpecialSummonOnly(id) == 0)
                monsters++;
        } else
            others++;
    }
    if (fromHand != 0 && others != 0)
        others--;
    if (monsters != 0 && others > 1)
        return 1;
    return 0;
}

/* Key 1421: a monster in the hand and a graveyard monster destroyed in battle this turn
 * (CollectEffectTargets). */
int EffectHandMonsterAndGraveTargetPrepare(struct ChainEntry *card)
{
    if (CountHandMonsters(card->player) == 0 || CollectEffectTargetsInt(card->player, CARD_1421, 0) <= 0)
        return 0;
    return 1;
}

/* Key 1423: Umi is the face-up Field Magic. */
int EffectUmiOnFieldPrepare(void)
{
    if (GetFaceUpFieldMagicNumber() == CARD_UMI)
        return 1;
    return 0;
}
