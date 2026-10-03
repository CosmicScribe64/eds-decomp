/*
 * ai_deck (0x080590E4-0x0805A30B): the CPU opponent's deck and its main-phase spell logic
 * (wiki/functions/ai-deck-c.md).
 *
 *  - RemoveAllDeckCardsByNumber, CountDeckCardsByNumber, RemoveOverLimitDeckCards and LoadOpponentDeck build the
 *    CPU's deck (player 1) for a Campaign duel from gOpponentDecks[gMain.opponent].
 *  - AiHasUsableSpellTrap, AiSelectUsableSpellTrap and AiTryPlaySpellTrap find a Magic or Trap card by card number,
 *    either as a set card on the CPU's field or in its hand, and start playing it.
 *  - AiActivateMonsterEffects, AiActivateExodiaTraps and AiPlaySpells are the sub-steps of the CPU's main phase
 *    (AiStepMainPhase in ai_steps.c): monster effects, the Exodia deck's traps, and the big spell state machine.
 *  - AiRiskSetCounter and AiIsOpponentThreatening are the conditions the spell logic asks.
 *
 * The CPU is always player 1; the human is player 0. "Card number" is the Konami number (constants/cards.h), "card
 * ID" the alphabetical index of the card tables (card_data.h).
 */
#include "global.h"
#include "card_data.h"              /* gCardIdToNumber, CARD_ID_MASK, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_*, SPELL_FIELD, CARD_STATS_* layout */
#include "constants/duel.h"         /* ZONE_*, CHAIN_KIND_* */
#include "constants/duel_cmds.h"    /* DUEL_CMD_PLAYER, DUEL_CMD_POINT_AT_CARD, DUEL_CMD_FLIP_CARD */
#include "constants/game.h"         /* DUELIST_RARE_HUNTER */
#include "main.h"                   /* gMain.opponent */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, summon.h, duel_cmd.h and card_menu.h do not pull in the legacy header.
 * After H0, replace the block (BEGIN to END) with #include "duel.h"
 * (see build/readability/issues/ai_deck.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 unk6_2:6;
    u8 unk7[0x8D - 7];
    u8 unk8D[3];                    /* +0x8D */
    u32 unk90_0:6;                  /* +0x90 */
    u32 unk90_6:4;
    u32 canActivate:1;              /* +0x91 bit 2: a set card that may be activated */
    u8 isDisabled:1;                /* +0x91 bit 3: card negated */
    u32 unk91_4:1;
    u32 declaredValue:5;            /* +0x91 bits 5-9 */
    u32 unk92_2:14;
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 unk6[0x28 - 6];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    u8 unk904[0xD64 - 0x904];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

/* The card command menu (gDuel.cardMenu, 0xC bytes). */
struct CardMenu {
    u16 open:1;                     /* bit 0 */
    u16 confirmed:1;                /* bit 1: a command was chosen (the AI sets it directly) */
    u16 command:4;                  /* bits 2-5: enum CardMenuCommand */
    u16 slide:4;                    /* bits 6-9 */
    u32 available:16;               /* bits 10-25 */
    u32 state:8;                    /* bits 26-33 */
    u32 step:8;                     /* bits 34-41: step of the command handler; reset before confirming */
    u8 summonSeq:4;                 /* bits 42-45 */
    u32 tributeSources:4;           /* bits 46-49 */
    u16 timer:7;                    /* bits 50-56 */
    u16 player:1;                   /* bit 57: player of the confirmed command */
    u32 area:7;                     /* bits 58-64 */
    u32 index:8;                    /* bits 65-72: zone index (field) or hand index (hand) */
    u32 placeZone:8;                /* bits 73-80 */
    u32 unk0A_1:15;
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B28 - 0x1ACC];
    u16 cardMenuCard;               /* +0x1B28: card ID under the cursor when the command was confirmed */
    u16 summonTributes;             /* +0x1B2A */
    struct CardMenu cardMenu;       /* +0x1B2C */
    u8 unk1B38[0x1B78 - 0x1B38];
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHandP1[80];         /* 0x0201A6CC = gDuelPlayers[1].hand */
extern struct DuelCard gDuelDeckP1[80];         /* 0x0201A80C = gDuelPlayers[1].deck */

u16 TakeDeckCardAt(int player, int idx, struct DuelCard *out);
void AddCardNumberToDeckTop(int player, u16 number);
void RemoveAllDeckCardsByNumber(int player, u16 number);
int CountDeckCardsByNumber(int player, u16 number);
void RemoveOverLimitDeckCards(int player);
int FindHandCardByNumber(int player, u16 cardNo);
int CountHandMonsters(int player);
int CountActiveCardsOnFieldExcept(int player, u16 cardNo, int skipZone);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountMonstersByNumber(int player, u16 cardNo);
int CountMonsters(int player);
int CountMonstersFiltered(int player, u16 faceUpOnly, u16 attackPosOnly);
int CountActivatableSetCards(int player, int cardNoWord);
int GetFaceUpFieldMagicNumber(void);
int CanPlaceSpellTrapCard(int player, u16 cardId);
u32 IsSpecialSummonOnly(u16 cardId);
u32 GetZoneCardAtk(u32 player, u32 slot);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* the Ai* functions defined here, struct AiState, enum AiSpellState */
#include "card_menu.h"              /* CardMenu_PlaySpellTrapFromHand */
#include "chain.h"                  /* struct ChainEntry, Chain_AddPending, CanActivateFieldCard */
#include "duel_actions.h"           /* FlipFieldCard */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl */
#include "effect.h"                 /* CanActivateEffect */
#include "effect_handlers.h"        /* EffectEquipTargetCheck, EffectBackupSoldierPrepare, ... */
#include "save.h"                   /* GetCardCopyLimit */
#include "util.h"                   /* MemClear16, Random */

#define PLAYER(p) (gDuelPlayers[(p) & 1])

/* The card word of a zone or pile entry as one u32. Matching: the ROM loads the whole word (ldr) and extracts the ID
 * with shifts; a .id bitfield read would load only the halfword that holds it. */
#define CARD_WORD(card) (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word) (((word) << 20) >> 20)

/* Matching: this unit calls DuelCmd_Push with u16 operands (duel_cmd.h takes arg4 and arg6 as int); the narrowing of
 * the packed operands is part of the ROM's code. */
extern void DuelCmdPush16(u32 cmd, u16 arg2, u16 arg4, u32 arg6) asm("DuelCmd_Push");

/* Matching: the card tables are read through integer-constant addresses (the ROM reloads the table address at every
 * use; the symbol forms generate other literal pools). */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])       /* gCardIdToNumber */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])        /* gCardStats */
#define gCardNumberToIdAddr ((const u16 *)0x08623DF4)                           /* gCardNumberToId */

/* Remove every card with card number `number` from the deck of `player`. */
void RemoveAllDeckCardsByNumber(int player, u16 number)
{
    int i;
    struct DuelCard removed;

    for (i = 0; i < PLAYER(player).deckCount;) {
        if (CARD_NUMBER(CARD_ID(CARD_WORD(PLAYER(player).deck[i]))) == number)
            TakeDeckCardAt(player, i, &removed);    /* the next card slides into slot i */
        else
            i++;
    }
}

/* Number of cards in the deck of `player` with card number `number`. */
int CountDeckCardsByNumber(int player, u16 number)
{
    int count = 0;
    int i;

    for (i = 0; i < PLAYER(player).deckCount; i++) {
        u32 id = CARD_ID(CARD_WORD(PLAYER(player).deck[i]));
        /* Matching: index term first (not gCardIdToNumber[...]), which gives the ROM's register order. */
        if (*((id & CARD_ID_MASK) + gCardIdToNumber) == number)
            count++;
    }
    return count;
}

/* Remove from the deck of `player` every card whose copy count is over its limit (GetCardCopyLimit, usually 3).
 * All copies of such a card go, not only the excess ones. */
void RemoveOverLimitDeckCards(int player)
{
    int i = 0;

    while (i < PLAYER(player).deckCount) {
        u16 id = CARD_ID(CARD_WORD(PLAYER(player).deck[i]));
        const u16 *number = (id & CARD_ID_MASK) + gCardIdToNumber;
        if (CountDeckCardsByNumber(player, *number) > GetCardCopyLimit(id))
            RemoveAllDeckCardsByNumber(player, *number);
        else
            i++;
    }
} /* 0x080591BC size 0x9C */

/* gOpponentDecks[duelist] (0x0819DC6C) and gOpponentAltDecks (0x0819DD34): the card numbers of each CPU deck. */
extern const struct DeckList gOpponentDecks[];
extern const struct DeckList gOpponentAltDecks[];
/* Matching: the deck is cleared through the deck symbol with byte offsets: the deck (0x140 bytes), the fusion
 * deck at +0x280 (0xA44 - 0x7C4) and the two counts at -0x7C1 (deckCount) and -0x7BF (fusionCount). */
extern u8 gDuelDeckP1Bytes[] asm("gDuelDeckP1");

/*
 * Build the CPU's deck for a Campaign duel: empty player 1's deck and fusion deck, add the card numbers of
 * gOpponentDecks[gMain.opponent] (gOpponentAltDecks when `useAltDeck`; the one caller, Campaign_SetupDuel, always
 * passes 0), then drop the cards over their copy limit. Rare Hunter, the Exodia deck, also gets AI_FLAG_EXODIA.
 */
void LoadOpponentDeck(u16 useAltDeck)
{
    int k;
    const struct DeckList *list;
    int opponent = gMain.opponent;
    int zero = 0;

    MemClear16(gDuelDeckP1Bytes, 0x140);
    MemClear16(gDuelDeckP1Bytes + 0x280, 0x140);
    gDuelDeckP1Bytes[-0x7C1] = zero;    /* deckCount */
    gDuelDeckP1Bytes[-0x7BF] = zero;    /* fusionCount */
    if (opponent != 0) {
        if (opponent == DUELIST_RARE_HUNTER)
            gDuelCtrl.aiFlags |= AI_FLAG_EXODIA;
        list = useAltDeck != 0 ? &gOpponentAltDecks[opponent] : &gOpponentDecks[opponent];
        /* FAKEMATCH: keep r5 live so the list pointer lands in r6 as in the ROM. */
        __asm__ __volatile__("" : : : "r5");
        for (k = 0; k < list->count; k++)
            AddCardNumberToDeckTop(1, list->cards[k]);
    }
    RemoveOverLimitDeckCards(1);
}

/*
 * 1 if one of the CPU's spell/trap zones (5-9) holds a card with number `number` that it can activate now (a set
 * card; CanActivateFieldCard decides).
 */
int AiHasUsableSpellTrap(u16 number)
{
    struct ChainEntry ref;
    int z;

    for (z = ZONE_SPELL_0; z <= ZONE_SPELL_4; z++) {
        struct DuelZone *zone = &gDuelZones[1].zones[z];
        if (CARD_ID(CARD_WORD(zone->card)) != 0) {
            ref.player = 1;
            ref.event = 0;
            if (CARD_NUMBER(CARD_ID(CARD_WORD(zone->card))) == number && CanActivateFieldCard(&ref, 1, z) != 0)
                return 1;
        }
    }
    return 0;
}

/*
 * Like AiHasUsableSpellTrap, but fills the whole chain entry first and, on success, selects the zone: it stores
 * the zone in gAiState.cardIndex and sets gAiState.subState to AI_SPELLS_CONFIRM_HAND_CARD, so AiPlaySpells (or
 * AiActivateExodiaTraps) activates it next. 1 if a zone was selected.
 */
int AiSelectUsableSpellTrap(u16 number)
{
    struct ChainEntry ref;
    int z;

    for (z = ZONE_SPELL_0; z <= ZONE_SPELL_4; z++) {
        struct DuelZone *zone = &gDuelZones[1].zones[z];
        if (CARD_ID(CARD_WORD(zone->card)) != 0) {
            ref.player = 1;
            ref.card = CARD_ID(CARD_WORD(zone->card));
            ref.zone = z;
            ref.event = 0;
            if (CARD_NUMBER(CARD_ID(CARD_WORD(zone->card))) == number && CanActivateFieldCard(&ref, 1, z) != 0) {
                gAiState.cardIndex = z;
                gAiState.subState = AI_SPELLS_CONFIRM_HAND_CARD;
                return 1;
            }
        }
    }
    return 0;
}

/* Converts a card number to a card ID: 0xFFFF means none, numbers up to 1999 map directly, and an alternate-art
 * number (2000 + n) uses the ID of n plus one. */
static inline u16 CardNumberToId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number <= CARD_NUMBER_ALT_ART - 1)
        return *((number & CARD_ID_MASK) + gCardNumberToIdAddr);
    return *(((number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK) + gCardNumberToIdAddr) + 1;
}

/* Spell/trap subtype (enum SpellSubtype) of a Magic or Trap card from its stats word, else 0. */
static inline int GetSpellSubtype(u32 stats)
{
    switch ((int)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(stats);
    default:
        return 0;
    }
}

/*
 * The location DUEL_LOC(1, z) (player 1, zone z) in the high halfword of a word; the caller shifts it down by 16
 * for EffectEquipTargetCheck's position argument.
 * FAKEMATCH: the empty asm keeps the flag in the register that receives the OR, as in the ROM.
 */
static inline u32 PackRefPosition(int z)
{
    u32 hi = (u32)z << 24;
    u32 flag = 0x10000;
    __asm__ __volatile__("" : "+r"(flag) : "r"(hi));
    return flag | hi;
}

/* The word Chain_AddPending takes for a CPU activation: card ID | kind << 21 | player 1 << 31 (chain.h). */
#define CPU_CHAIN_KIND(kind) (0x80000000 | ((kind) << 21))

/*
 * Try to play the Magic or Trap card `number` for the CPU, in this order:
 *  1. when the human has an activatable set Magic Jammer, back off at random (3 times in 4 with AI_FLAG_CAREFUL, else 1 time in 8);
 *  2. refuse a Field Magic that is already face up on the field;
 *  3. an Equip Magic needs a monster zone it can equip to (EffectEquipTargetCheck);
 *  4. a face-down copy in spell/trap zones 5-9 that may be activated: point at it, flip it and add it to the chain;
 *  5. otherwise select the copy in the hand: gAiState.cardIndex = its hand index and the caller confirms it.
 * Returns 1 when the card was activated or selected (the callers assume the hand path), 0 if it cannot be played.
 */
int AiTryPlaySpellTrap(u16 number)
{
    struct ChainEntry ref;
    int hand = FindHandCardByNumber(1, number);
    int ok;
    int z;
    u32 zoneBits;
    u32 cardWord;
    u32 pos;
    u16 cardId;

    if (CountActivatableSetCards(0, CARD_MAGIC_JAMMER) != 0) {
        if (gDuelCtrl.aiFlags & AI_FLAG_CAREFUL) {
            if ((Random() & 3) != 0)
                return 0;
        } else {
            if ((Random() & 7) == 0)
                return 0;
        }
    }
    cardId = CardNumberToId(number);
    if (GetSpellSubtype(CARD_STATS(cardId)) == SPELL_FIELD && GetFaceUpFieldMagicNumber() == number)
        return 0;
    switch (number) {
    /* Equip Magic: Legendary Sword .. Cyber Shield (300-316), Mystical Moon, Malevolent Nuzzler .. Power of Kaishin
     * (320-327), Magical Labyrinth, Salamandra, Bright Castle, Burning Spear, Gust Fan, Sword of Deep-Seated, Sword
     * of Dragon's Soul, Graceful Dice and key 1420. */
    case 0x12C: case 0x12D: case 0x12E: case 0x12F: case 0x130: case 0x131: case 0x132: case 0x133:
    case 0x134: case 0x135: case 0x136: case 0x137: case 0x138: case 0x139: case 0x13A: case 0x13B:
    case 0x13C:
    case CARD_MYSTICAL_MOON:
    case 0x140: case 0x141: case 0x142: case 0x143: case 0x144: case 0x145: case 0x146: case 0x147:
    case CARD_MAGICAL_LABYRINTH:
    case CARD_SALAMANDRA:
    case CARD_BRIGHT_CASTLE:
    case CARD_BURNING_SPEAR: case CARD_GUST_FAN:
    case CARD_SWORD_OF_DEEP_SEATED:
    case CARD_SWORD_OF_DRAGONS_SOUL:
    case CARD_GRACEFUL_DICE:
    case CARD_1420:
        ok = 1;
        for (z = ZONE_MONSTER_0; z <= ZONE_MONSTER_4; z++) {
            ref.card = CardNumberToId(number);
            ref.player = 1;
            pos = PackRefPosition(z);
            if (EffectEquipTargetCheck(&ref, pos >> 16) != 0)
                ok = 0;
        }
        if (ok != 0)
            return 0;
        break;
    }
    for (z = ZONE_SPELL_0; z <= ZONE_SPELL_4; z++) {
        struct DuelZone *zone;

        ref.player = 1;
        ref.event = 0;
        ref.zone = z;
        zone = &gDuelZones[1].zones[z];
        ref.card = CARD_ID(CARD_WORD(zone->card));
        /* Matching: byte tests of the zone flags (+0x91 bit 2 is canActivate; the header's u32 container reads a word). */
        if (ref.card != 0 && (((u8 *)zone)[0x91] & 4) && !zone->isFaceUp && CARD_NUMBER(ref.card) == number
            && CanActivateEffect(&ref, 0, 0) != 0) {
            DuelCmdPush16(DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD, 1, (z & 0xFF) << 8, 0);
            DuelCmdPush16(DUEL_CMD_PLAYER | DUEL_CMD_FLIP_CARD, z, 0, 0);
            zoneBits = (z & 0x1F) << 16;
            cardWord = CARD_ID(CARD_WORD(zone->card)) | CPU_CHAIN_KIND(CHAIN_KIND_SPELL_TRAP);
            Chain_AddPending(zoneBits | cardWord, 0);
            return 1;
        }
    }
    if (CanPlaceSpellTrapCard(1, CardNumberToId(number)) == 0)
        return 0;
    if (hand < 0)
        return 0;
    ref.player = 1;
    ref.event = 0;
    ref.card = CARD_ID(CARD_WORD(gDuelPlayers[1].hand[hand]));
    if (CanActivateEffect(&ref, 0, 1) == 0)
        return 0;
    gAiState.cardIndex = hand;
    gAiState.subState = AI_SPELLS_CONFIRM_HAND_CARD;
    return 1;
}

/* Byte view of gDuelZonesP1 (0x0201A070 = gDuelPlayers[1].zones). Matching: the ROM reaches player 1's zones, and
 * the card-menu words of gDuel (0x0201AE08 = gDuelZonesP1 + 0xD98), by byte offset from this one symbol. */
extern u8 gDuelZonesP1Bytes[] asm("gDuelZonesP1");
#define ZONE_STRIDE 0x94                /* sizeof(struct DuelZone) */
/* The card word of the CPU's zone z, read through the byte view. */
#define ZONE1_WORD(z) (*(u32 *)(gDuelZonesP1Bytes + (z) * ZONE_STRIDE))

/* gDuel.cardMenu.step (u16 container, bits 2-9 at +0x1B30) and gDuel.cardMenu.index (u16 container, bits 1-8 at
 * +0x1B34), the halfword views the ROM writes the CPU's menu confirmation with. */
struct CardMenuStepView {
    u16 lo:2;
    u16 step:8;
    u16 hi:6;
};
struct CardMenuIndexView {
    u16 lo:1;
    u16 index:8;
    u16 hi:7;
};

/*
 * The word of Chain_AddPending for the zone index z: zone << 16.
 * FAKEMATCH: the 31 mask is built in its own register first, as the ROM does.
 */
static inline u32 PackZoneIndex(u32 z)
{
    u8 mask = 31;
    __asm__ __volatile__("" : : "r"(mask));
    return (z & mask) << 16;
}

/* The CPU's table of monsters with an activated effect: gAiEffectMonsters (0x0819DD64): Time Wizard, Cannon
 * Soldier, Relinquished, Barrel Dragon. */
extern const u16 gAiEffectMonsters[];

/*
 * Activate the effect of a face-up monster of gAiEffectMonsters on the CPU's field (listIndex walks the table,
 * zoneIndex the monster zones). Cannon Soldier first tries to play key 1245 (Scapegoat by behaviour, not an EDS
 * card); if that selects a hand card it confirms it in the card menu and returns 0, so that the tokens arrive before
 * the effect, else it needs a second monster to tribute. An activation points at the zone and adds the card to the
 * chain. Returns 1 if an effect was activated, 0 if not (or while a hand card is being confirmed).
 */
int AiActivateMonsterEffects(void)
{
    struct ChainEntry ref;
    int ok;
    u16 id;
    u32 zoneBits;
    u32 cardWord;
    u16 handCardId;
    u16 *indexWord;

    for (gAiState.listIndex = 0; gAiState.listIndex <= 3; gAiState.listIndex++) {
        for (gAiState.zoneIndex = 0; gAiState.zoneIndex <= 4; gAiState.zoneIndex++) {
            ref.player = 1;
            id = CARD_ID(ZONE1_WORD(gAiState.zoneIndex));
            ref.card = id;
            ref.zone = gAiState.zoneIndex;
            ref.event = 0;
            if (id != 0 && CARD_NUMBER(id) == gAiEffectMonsters[gAiState.listIndex]
                && (((struct DuelZone *)(gDuelZonesP1Bytes + gAiState.zoneIndex * ZONE_STRIDE))->isFaceUp) != 0
                && CanActivateEffect(&ref, 0, 0) != 0) {
                ok = 1;
                if (gAiEffectMonsters[gAiState.listIndex] == CARD_CANNON_SOLDIER) {
                    if ((u16)AiTryPlaySpellTrap(CARD_1245) != 0) {
                        /* Confirm hand card cardIndex in the card menu: step = 0, cardMenuCard, player = 1,
                         * index = cardIndex, confirmed = 1. */
                        ((struct CardMenuStepView *)(gDuelZonesP1Bytes + 0xDA0))->step = 0;
                        handCardId = CARD_ID(CARD_WORD(gDuelPlayers[1].hand[gAiState.cardIndex]));
                        *(u16 *)(gDuelZonesP1Bytes + 0xD98) = handCardId;
                        *(u8 *)(gDuelZonesP1Bytes + 0xDA3) |= 2;
                        indexWord = (u16 *)(gDuelZonesP1Bytes + 0xDA4);
                        ((struct CardMenuIndexView *)indexWord)->index = gAiState.cardIndex;
                        *(u8 *)(gDuelZonesP1Bytes + 0xD9C) |= 2;
                        return 0;
                    }
                    if (CountMonsters(1) <= 1)
                        ok = 0;
                }
                if (ok != 0) {
                    DuelCmdPush16(DUEL_CMD_PLAYER | DUEL_CMD_POINT_AT_CARD, 1, gAiState.zoneIndex << 8, 0);
                    zoneBits = PackZoneIndex(gAiState.zoneIndex);
                    cardWord = CARD_ID(ZONE1_WORD(gAiState.zoneIndex)) | CPU_CHAIN_KIND(CHAIN_KIND_MONSTER);
                    Chain_AddPending(zoneBits | cardWord, 0);
                    return 1;
                }
            }
        }
    }
    return 0;
}

#define ZONE1(z) ((struct DuelZone *)((u32)gDuelZonesP1Bytes + (z) * ZONE_STRIDE))

/* Matching: the ROM calls this prepare handler with the three arguments of the prepare slot (r1 = r2 = 0); the
 * definition (effect_handlers.h) takes only the link. */
extern int EffectBackupSoldierPrepare3(struct ChainEntry *card, int chainLink, int fromHand)
    asm("EffectBackupSoldierPrepare");

/*
 * Step machine of the Exodia deck's traps (Rare Hunter), on gAiState.subState:
 *   AI_SPELLS_START           with AI_FLAG_EXODIA go to AI_SPELLS_EXODIA_CLEAR, else the step is done;
 *   AI_SPELLS_EXODIA_CLEAR    select a usable set key 1447 (a draw trap, not in EDS) or, if Backup Soldier can be
 *                             played and an Exodia piece is in the graveyard, a usable set Backup Soldier;
 *   AI_SPELLS_CONFIRM_HAND_CARD  flip the selected zone if it is face down, add it to the chain and restart the main
 *                             phase.
 * Returns 1 when there is nothing (more) to do, 0 while it is acting.
 */
int AiActivateExodiaTraps(void)
{
    struct ChainEntry ref;

    switch (gAiState.subState) {
    case AI_SPELLS_START:
        if (gDuelCtrl.aiFlags & AI_FLAG_EXODIA) {
            gAiState.subState = AI_SPELLS_EXODIA_CLEAR;
            return 0;
        }
        break;
    case AI_SPELLS_EXODIA_CLEAR:
        if ((u16)AiSelectUsableSpellTrap(CARD_1447) != 0)
            return 0;
        ref.player = 1;
        if (EffectBackupSoldierPrepare3(&ref, 0, 0) != 0 && AiCountExodiaInGraveyard() > 0
            && (u16)AiSelectUsableSpellTrap(CARD_BACKUP_SOLDIER) != 0)
            return 0;
        break;
    case AI_SPELLS_CONFIRM_HAND_CARD: {
        u8 zone = gAiState.cardIndex;
        u32 zoneBits;
        u32 cardWord;

        if (!ZONE1(zone)->isFaceUp)
            FlipFieldCard(1, zone, 0);
        zone = gAiState.cardIndex;
        zoneBits = PackZoneIndex(zone);
        cardWord = CARD_ID(CARD_WORD(ZONE1(zone)->card)) | CPU_CHAIN_KIND(CHAIN_KIND_SPELL_TRAP);
        Chain_AddPending(zoneBits | cardWord, 0);
        gAiState.phase = AI_MAIN_SUMMON;
        return 0;
    }
    }
    return 1;
}

/*
 * 1 = go ahead, 0 = hold back. If the human has an activatable set copy of card number `counterNo` the CPU holds back
 * at random: 3 times in 4 with AI_FLAG_CAREFUL, otherwise 1 time in 8.
 */
int AiRiskSetCounter(u16 counterNo)
{
    if (CountActivatableSetCards(0, counterNo) != 0) {
        if (gDuelCtrl.aiFlags & AI_FLAG_CAREFUL) {
            if ((Random() & 3) != 0)
                return 0;
        } else {
            if ((Random() & 7) == 0)
                return 0;
        }
    }
    return 1;
}

/*
 * 0 only when the human's strongest monster is no danger: its ATK is below the CPU's life points, its lead over the
 * CPU's strongest ATK is below the CPU's life points, it does not beat the CPU's strongest ATK, and the total ATK of
 * the human's monsters does not reach the CPU's life points. Otherwise 1.
 * ROM bug kept: the "== -1" tests read the uninitialised locals a and b instead of the zone indexes x and y, so an
 * empty field passes zone -1 on to GetZoneCardAtk.
 */
int AiIsOpponentThreatening(void)
{
    int humanAtk;
    int cpuAtk;
    int zone;
    int humanBest = AiFindStrongestMonster(0, -1, 1, 0);
    int cpuBest = AiFindStrongestMonster(1, -1, 1, 0);

    humanAtk = humanAtk == -1 ? 0 : GetZoneCardAtk(0, humanBest);
    cpuAtk = cpuAtk == -1 ? 0 : GetZoneCardAtk(1, cpuBest);
    if (humanAtk < gDuelPlayers[1].lifePoints && humanAtk - cpuAtk < gDuelPlayers[1].lifePoints && humanAtk <= cpuAtk) {
        humanAtk = 0;
        for (zone = 0; zone <= 4; zone++)
            humanAtk += GetZoneCardAtk(0, zone);
        if (humanAtk <= gDuelPlayers[1].lifePoints)
            return 0;
    }
    return 1;
}

struct DuelMenuBytes {
    u8 unk0[0x1B2C];
    u8 flags;                       /* +0x1B2C: cardMenu bits 0-7 (bit 1 = confirmed) */
};
extern struct DuelMenuBytes gDuelMenuBytes asm("gDuel");

/* The Magic cards AiPlaySpells state AI_SPELLS_LISTS plays, in this order (gAiGenericSpells, 0x08086394, 54 card
 * numbers: Graceful Charity, Pot of Greed, Fusion Sage, Upstart Goblin, Dark-Piercing Light, the Field Magics, the
 * healing and burn cards, Swords of Revealing Light, Mesmeric Control, Gravekeeper's Servant, Delinquent Duo, Chain
 * Energy, the Polymerizations and Ritual Magics, and keys 1230, 1426 and 1427) and, when the CPU has a face-up
 * monster, gAiEquipSpells (0x08086400, 35 numbers: the Equip Magic cards). */
extern const u16 gAiGenericSpells[];
extern const u16 gAiEquipSpells[];
#define AI_GENERIC_SPELL_COUNT 54
#define AI_EQUIP_SPELL_COUNT 35

/* Tributes a Normal Summon needs by level: levels 1-4 none, 5-6 one monster, 7 and up two. */
#define LEVEL_MAX_NO_TRIBUTE 4
#define LEVEL_MAX_ONE_TRIBUTE 6

/*
 * Level of a card as the AI counts it: 0 for Trap, Magic and Ticket cards, 10 for the Divine-Beasts, else the stars
 * (stats bits 25-28).
 */
static inline u32 AiCardLevel(u16 id)
{
    switch ((int)CARD_STATS_TYPE(CARD_STATS(id))) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS(id));
    }
}

/*
 * AiCardLevel with the loop-invariant masks as parameters.
 * FAKEMATCH: the hand scan keeps the 0x7FF and type masks in registers across the loop only when they arrive as
 * parameters.
 */
static inline u32 AiCardLevelMasks(u32 id, u32 mask, u32 typeMask)
{
    switch ((int)((((const u32 *)0x08621DE0)[id & mask] & typeMask) >> CARD_STATS_TYPE_SHIFT)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(((const u32 *)0x08621DE0)[id & mask]);
    }
}

/* gDuelPlayers[1].handCount, read through its address (base 0x020192E4 + 0xD66).
 * FAKEMATCH: the empty asm makes the address a fresh value at the loop test, so the ROM's reload happens
 * instead of retaining a loop pointer. */
static inline int AiCpuHandCountReload(void)
{
    u32 base = (u32)gDuelPlayers;
    u32 off = 0xD66;
    asm("" : "+r"(base), "+r"(off));
    return *(u8 *)(base + off);
}

/* Card number of card ID `id`, with the table address and the byte offset formed separately. */
static inline u16 AiCardNumberOfId(u32 id)
{
    u32 base;
    u32 off;

    off = (id & CARD_ID_MASK) * 2;
    base = (u32)gCardIdToNumber;
    return *(u16 *)(off + base);
}

/* gDuelPlayers[1].deckCount read through `ptr` (= base + 0xD67).
 * FAKEMATCH: the count is initialized into r3 and consumed by the empty asm, so it is still there at the loop test. */
static inline int AiCpuDeckCountAt(u8 *ptr)
{
    register int count asm("r3") = *ptr;
    asm("" : : "r"(count));
    return count;
}

/*
 * The CPU's main-phase spell machine, on gAiState.subState (enum AiSpellState). Each call plays at most one card
 * (AiTryPlaySpellTrap) or advances the state; returns 0 while it is still working and 1 when it is done.
 *  1  Pot of Greed, Fusion Sage, Dark Hole / Raigeki (when the CPU's field is empty, the human threatens it, it
 *     holds its own Sangan or Witch of the Black Forest, or the human has no set White Hole / Anti-Raigeki to answer),
 *     Tribute to the Doomed, Fissure.
 *  2  Change of Heart (a tribute for a level 5-6 monster, or level 5+ when the CPU has monsters), Monster Reborn,
 *     Premature Burial.
 *  3  Graceful Charity (with AI_FLAG_CAREFUL only with a revival card in hand or a key card in the top 3 of its deck).
 *  4  Card Destruction (key 1221) when Magic Thorn is active or the human holds key cards (the CPU reads the human's
 *     hand; a ROM bug makes the test true for "not in hand").
 *  5  the gAiGenericSpells list, then gAiEquipSpells when the CPU has a face-up monster.
 *  0x64-0x66 (AI_FLAG_EXODIA): Dark Hole to clear its own Sangan or Witch, Graceful Charity or Pot of Greed, Swords
 *     of Revealing Light.
 *  0xC8-0xC9 confirm the selected hand card in the card menu and run CardMenu_PlaySpellTrapFromHand until it is
 *     done; the card is Set instead of activated while Anti-Magic Fragrance is on the field. Then phase = 0.
 */
int AiPlaySpells(void)
{
    /* Matching: the (u16) casts on the results of AiTryPlaySpellTrap, AiRiskSetCounter and AiIsOpponentThreatening
     * stay; the ROM reads the returned halfword. */
    struct ChainEntry ref;
    int i;
    /* Matching: `found` starts as 1 (the ROM sets r4 = 1 on entry); it later serves as a flag and as the deck index. */
    int found = 1;
    int state = gAiState.subState;
    u16 number;
    u16 result;

    switch ((u8)state) {
    case AI_SPELLS_START:
        if (gDuelCtrl.aiFlags & AI_FLAG_EXODIA)
            gAiState.subState = AI_SPELLS_EXODIA_CLEAR;
        else
            gAiState.subState = state + 1;
        return 0;
    case AI_SPELLS_DRAW_AND_CLEAR:
        if ((u16)AiTryPlaySpellTrap(CARD_POT_OF_GREED)) return 0;
        if ((u16)AiTryPlaySpellTrap(CARD_FUSION_SAGE)) return 0;
        if (CountMonsters(0) > 0 && CountMonsters(1) == 0 && (u16)AiTryPlaySpellTrap(CARD_DARK_HOLE)) return 0;
        if ((u16)AiIsOpponentThreatening() && (u16)AiTryPlaySpellTrap(CARD_DARK_HOLE)) return 0;
        if ((CountMonstersByNumber(1, CARD_SANGAN) > 0 || CountMonstersByNumber(1, CARD_WITCH_OF_THE_BLACK_FOREST) > 0)
            && (u16)AiTryPlaySpellTrap(CARD_DARK_HOLE)) return 0;
        if (CountMonsters(0) > 0 && (u16)AiRiskSetCounter(CARD_WHITE_HOLE) && (u16)AiTryPlaySpellTrap(CARD_DARK_HOLE)) return 0;
        if (CountMonsters(0) > 0 && CountMonsters(1) == 0 && (u16)AiTryPlaySpellTrap(CARD_RAIGEKI)) return 0;
        if (CountMonsters(0) > 0 && (u16)AiRiskSetCounter(CARD_ANTI_RAIGEKI) && (u16)AiTryPlaySpellTrap(CARD_RAIGEKI)) return 0;
        if (CountMonstersFiltered(0, 0, 0) > 0 && CountHandMonsters(1) > 0 && (u16)AiTryPlaySpellTrap(CARD_TRIBUTE_TO_THE_DOOMED)) return 0;
        if (CountMonstersFiltered(0, 1, 0) > 0 && (u16)AiTryPlaySpellTrap(CARD_FISSURE)) return 0;
        gAiState.subState++;
        return 0;
    case AI_SPELLS_TAKE_AND_REVIVE:
        if (CountMonsters(0) > 0) {
            if (CountMonsters(1) == 0) {
                /* The CPU has no monster: Change of Heart pays for a level 5-6 monster in its hand. */
                i = 0;
                if (i < PLAYER(1).handCount) {
                    do {
                        u16 id = CARD_ID(*(u32 *)((u8 *)gDuelHandP1 + i * 4));
                        if (CARD_STATS_TYPE(CARD_STATS(id)) <= CARD_TYPE_REPTILE && IsSpecialSummonOnly(id) == 0
                            && AiCardLevel(id) > LEVEL_MAX_NO_TRIBUTE && AiCardLevel(id) <= LEVEL_MAX_ONE_TRIBUTE
                            && (u16)AiTryPlaySpellTrap(CARD_CHANGE_OF_HEART))
                            return 0;
                        i++;
                    } while (i < PLAYER(1).handCount);
                }
            }
            if (CountMonsters(1) > 0) {
                /* The CPU has monsters: any monster of level 5 or more in its hand will do. */
                i = 0;
                if (i < PLAYER(1).handCount) {
                    u32 typeMask = CARD_STATS_TYPE_MASK;
                    u32 loadMask = CARD_ID_MASK;
                    u32 mask;
                    /* FAKEMATCH: the initialized copy of the mask stays live before the scan. */
                    asm("" : : "r"(loadMask));
                    mask = loadMask;
                    do {
                        u32 id = CARD_ID(*(u32 *)((u8 *)gDuelHandP1 + i * 4));
                        if (((((const u32 *)0x08621DE0)[id & mask] & typeMask) >> CARD_STATS_TYPE_SHIFT) <= CARD_TYPE_REPTILE
                            && IsSpecialSummonOnly(id) == 0 && AiCardLevelMasks(id, mask, typeMask) > LEVEL_MAX_NO_TRIBUTE
                            && (u16)AiTryPlaySpellTrap(CARD_CHANGE_OF_HEART))
                            return 0;
                        i++;
                    } while (i < AiCpuHandCountReload());
                }
            }
        }
        number = CARD_MONSTER_REBORN;
        ref.card = CardNumberToId(number);
        ref.player = 1;
        if (EffectMonsterRebornPrepare(&ref, 0, 1) && (u16)AiRiskSetCounter(CARD_CALL_OF_THE_DARK) && (u16)AiTryPlaySpellTrap(number)) return 0;
        number = CARD_PREMATURE_BURIAL;
        ref.card = CardNumberToId(number);
        ref.player = 1;
        if (EffectPrematureBurialPrepare(&ref, 0, 1) && (u16)AiTryPlaySpellTrap(number)) return 0;
        gAiState.subState++;
        return 0;
    case AI_SPELLS_GRACEFUL_CHARITY:
        if (gDuelCtrl.aiFlags & AI_FLAG_CAREFUL) {
            /* With AI_FLAG_CAREFUL: a revival card in the hand makes the discard worthwhile ... */
            if (FindHandCardByNumber(1, CARD_MONSTER_REBORN) != -1 && (u16)AiTryPlaySpellTrap(CARD_GRACEFUL_CHARITY)) return 0;
            if (FindHandCardByNumber(1, CARD_PREMATURE_BURIAL) != -1 && (u16)AiTryPlaySpellTrap(CARD_GRACEFUL_CHARITY)) return 0;
            found = 0;
            {
                /* ... and so does a Dark Hole, Raigeki, Graceful Charity, Monster Reborn or Premature Burial among the
                 * first three cards of its deck.
                 * FAKEMATCH: initialized terms preserve the deck-count preheader (the count is read through
                 * gDuelPlayers + 0xD67 = gDuelPlayers[1].deckCount). */
                register u32 base asm("r0") = (u32)gDuelPlayers;
                register u32 off asm("r3") = 0xD67;
                int count;
                u8 *first;
                asm("" : "+r"(off));
                first = (u8 *)(base + off);
                count = *first;
                if (found < count) {
                    u8 *ptr = first;
                deckLoop:
                    {
                        switch (AiCardNumberOfId(CARD_ID(*(u32 *)(gDuelDeckP1Bytes + found * 4)))) {
                        case CARD_DARK_HOLE:
                        case CARD_RAIGEKI:
                        case CARD_GRACEFUL_CHARITY:
                        case CARD_MONSTER_REBORN:
                        case CARD_PREMATURE_BURIAL:
                            if ((u16)AiTryPlaySpellTrap(CARD_GRACEFUL_CHARITY)) return 0;
                            break;
                        }
                        found++;
                    }
                    if (found < 3 && found < AiCpuDeckCountAt(ptr))
                        goto deckLoop;
                }
            }
        } else if ((u16)AiTryPlaySpellTrap(CARD_GRACEFUL_CHARITY)) return 0;
        gAiState.subState++;
        return 0;
    case AI_SPELLS_CARD_DESTRUCTION:
        /* Does the human hold something worth stripping from it? */
        found = 0;
        if (CountActiveCardsOnFieldExcept(1, CARD_MAGIC_THORN, -1) > 0) found = 1;
        /* ROM bug: AiFindHandCardByNumber returns -1 for "not in hand", which counts as true here; only index 0
         * counts as false. */
        if (AiFindHandCardByNumber(0, CARD_RIGHT_LEG_OF_THE_FORBIDDEN_ONE)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_LEFT_LEG_OF_THE_FORBIDDEN_ONE)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_RIGHT_ARM_OF_THE_FORBIDDEN_ONE)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_LEFT_ARM_OF_THE_FORBIDDEN_ONE)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_EXODIA_THE_FORBIDDEN_ONE)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_DARK_HOLE)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_RAIGEKI)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_MEGAMORPH)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_MONSTER_REBORN)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_CHANGE_OF_HEART)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_MIRROR_FORCE)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_SNATCH_STEAL)) found = 1;
        if (AiFindHandCardByNumber(0, CARD_PREMATURE_BURIAL)) found = 1;
        if (found && (u16)AiTryPlaySpellTrap(CARD_1221))
            return 0;
        gAiState.subState++;
        gAiState.listIndex = 0;
        gAiState.zoneIndex = 0;
        gAiState.unk9 = 0;
        return 0;
    case AI_SPELLS_LISTS:
        for (gAiState.listIndex = 0; gAiState.listIndex < AI_GENERIC_SPELL_COUNT; gAiState.listIndex++) {
            result = (u16)AiTryPlaySpellTrap(gAiGenericSpells[gAiState.listIndex]);
            if (result) return 0;
        }
        if (CountMonstersFiltered(1, 1, 0) > 0) {
            for (gAiState.listIndex = 0; gAiState.listIndex < AI_EQUIP_SPELL_COUNT; gAiState.listIndex++) {
                if ((u16)AiTryPlaySpellTrap(gAiEquipSpells[gAiState.listIndex])) return 0;
            }
        }
        break;
    case AI_SPELLS_EXODIA_CLEAR:
        if (!AiCountExodiaOnField() && (CountMonstersByNumber(1, CARD_SANGAN) > 0 || CountMonstersByNumber(1, CARD_WITCH_OF_THE_BLACK_FOREST) > 0)
            && (u16)AiTryPlaySpellTrap(CARD_DARK_HOLE)) return 0;
        gAiState.subState++;
        return 0;
    case AI_SPELLS_EXODIA_DRAW:
        if (!(u16)AiTryPlaySpellTrap(CARD_GRACEFUL_CHARITY) && !(u16)AiTryPlaySpellTrap(CARD_POT_OF_GREED)) gAiState.subState++;
        return 0;
    case AI_SPELLS_EXODIA_SWORDS:
        if ((AiCountExodiaOnField() || (CountMonstersFiltered(0, 1, 0) > 0 && CountMonsters(1) == 0))
            && CountActiveCardsOnFieldExcept(1, CARD_SWORDS_OF_REVEALING_LIGHT, -1) == 0
            && (u16)AiTryPlaySpellTrap(CARD_SWORDS_OF_REVEALING_LIGHT)) return 0;
        break;
    case AI_SPELLS_CONFIRM_HAND_CARD:
        /* Confirm hand card cardIndex in the card menu as if the player had chosen it. */
        gDuel.cardMenu.step = 0;
        gDuel.cardMenuCard = CARD_ID(*(u32 *)((u8 *)&gDuel.players[1].hand[0] + gAiState.cardIndex * 4));
        gDuel.cardMenu.player = 1;
        gDuel.cardMenu.index = gAiState.cardIndex;
        gDuel.cardMenu.confirmed = 1;
        gAiState.subState++;
        /* fall through */
    case AI_SPELLS_WAIT_PLAY:
        if (CountActiveCardsOnField(0, CARD_ANTI_MAGIC_FRAGRANCE) || CountActiveCardsOnField(1, CARD_ANTI_MAGIC_FRAGRANCE))
            CardMenu_PlaySpellTrapFromHand(0, 0, 0);
        else
            CardMenu_PlaySpellTrapFromHand(1, 0, 0);
        {
            u8 confirmed = gDuelMenuBytes.flags & 2;
            if (!confirmed) gAiState.phase = confirmed;
        }
        return 0;
    }
    return 1;
}
