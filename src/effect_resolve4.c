/*
 * effect_resolve4 (0x08033DAC-0x08035197): resolve handlers of seventeen more effects
 * (wiki/functions/effect-resolve4-c.md), continuing effect_resolve3.
 *
 * Every function here is the resolve slot (+0x04) of a gCardEffects row (struct CardEffect, include/effect.h).
 * Chain_Resolve calls it with the resolving link (its card ID, player and zone, the triggering event and its
 * location loc0, and the targets chosen by the chainB handler: link->targets[], player | zone << 8) and, for
 * the handlers that take it, chainedTo: the link this one answers. A negated link does nothing. Multi-step
 * effects are step machines on gChain.effectStep: Chain_Resolve starts it at EFFECT_STEP_START (0x80) and
 * stores the handler's return value until the handler returns EFFECT_STEP_DONE. A returned step that has no
 * case (0x78, 0x7E, 0x0A) ends the link on the next call.
 *
 * Cards: Shield & Sword, Graceful Charity, Chain Destruction, Mesmeric Control, Magic-Arm Shield, Fissure,
 * Trap Hole / House of Adhesive Tape / Eatgaboon, Remove Trap, Two-Pronged Attack, Monster Reborn, Pot of
 * Greed / Skelengel, Gravedigger Ghoul / Soul Release, The Inexperienced Spy, the until-end-of-turn stat
 * cards, Ultimate Offering, Ancient Telescope and the counter traps (White Hole, Call of the Grave, Anti
 * Raigeki, Magic Jammer, Seven Tools of the Bandit, Gryphon Wing, keys 1530/1531).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* DUEL_LOC_ZONE, enum DuelZoneIndex, ResponseEventKind, ZoneLinkKind, ... */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* SE_CONFIRM, SE_ERROR */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, card_list_view.h, duel_cmd.h, duel_screen.h and summon.h do not pull in the legacy
 * header. After H0, replace the block (BEGIN to END) with #include "legacy/duel.h"
 * (see build/readability/issues/effect_resolve4.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player: whose graveyard the card returns to */
    u32 unk13:19;
};

/* Needed by duel_screen.h (DuelScreen.from / .to). */
struct DuelLoc {
    u16 player:1;                   /* bit 0: side of the field */
    u16 area:4;                     /* bits 1-4: enum DuelArea */
    u16 index:9;                    /* bits 5-13 */
    u16 isDefense:1;                /* bit 14 */
    u16 isFaceUp:1;                 /* bit 15 */
    u16 unk2;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u8 unk6_6:2;
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 unk3[0x28 - 0x3];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC;                     /* +0x1ACC: field background, duel over, magic/trap negation */
    u8 equipMagicNegated:1;         /* +0x1ACD bit 0 */
    u8 equipMagicNegatedThisTurn:1; /* +0x1ACD bit 1 */
    u8 fieldMagicNegatedThisTurn:1; /* +0x1ACD bit 2 */
    u8 contMagicNegatedThisTurn:1;  /* +0x1ACD bit 3 */
    u8 contTrapNegatedThisTurn:1;   /* +0x1ACD bit 4 */
    u8 statChangesReversed:1;       /* +0x1ACD bit 5 */
    u8 atkDefSwapped:1;             /* +0x1ACD bit 6: Shield & Sword */
    u8 unk1ACD_7:1;
    u8 unk1ACE[0x1B78 - 0x1ACE];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

u32 IsSameCardName(u32 cardId1, u32 cardId2);
u32 HasFlipEffect(u16 cardNo, int inBattle);
u32 IsSpecialSummonOnly(u16 cardId);
int GetGraveyardCardById(int player, u16 cardId, struct DuelCard *out);
int CountActiveCardsOnField2(int player, u16 cardNo);
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* DestroyFieldCard, DiscardHandCard, DrawCards, MoveFieldCard, ... */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_PostDiscard */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* enum EffectStep, CanCardTargetZone, IsZoneTargetable, gTributeSummonPrompts */
#include "effect_handlers.h"        /* the handlers defined here */
#include "summon.h"                 /* CanSummonFromHand, QueueNormalSummonChoosePosition, ... */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */

/*
 * Local views kept on purpose (matching choices, see build/readability/HEADERS.md).
 */
/* DuelCmd_Push (duel_cmd.h) takes arg4 and arg6 as int. This unit's matched code narrowed the cursor operand
 * of DUEL_CMD_POINT_AT_CARD to u16 (it called through a u16 prototype): CURSOR_AREA_INDEX casts it. */
/* Matching: CollectEffectTargets is defined to return u16; the ROM compares the count as an int (cmp; ble). */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: the ROM compares the ATK / DEF results as signed ints (the definitions return u32). */
extern int GetZoneCardAtkInt(int player, int zone) asm("GetZoneCardAtk");
extern int GetZoneCardDefInt(int player, int zone) asm("GetZoneCardDef");
/* Matching: the ROM tests these results as whole registers (cmp #0); the definitions return u16, which adds
 * a narrowing lsl #16 before the test. */
extern int CanCardTargetZoneInt(u16 cardId, int player, int zone) asm("CanCardTargetZone");
extern int IsTributableMonsterInt(int player, int zone) asm("IsTributableMonster");

/* Prompts of this unit (ROM). */
extern const u8 gStrSelectGraveyardCardToBanish[];          /* 0x08082CA8: "Select a card in Graveyard to be
                                                             * removed from play." */
extern const u8 gStrBanishAnotherGraveyardCardQuestion[];   /* 0x08082CE0: "Do you wish to remove from play
                                                             * another card in your Graveyard?" */
extern const u8 gStrSelectMonsterToSummonFromHand[];        /* 0x08082D24: "Select a monster that you wish to
                                                             * Summon from your hand." */

/* The card word of a zone or pile entry as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with shifts; a .id bitfield read would load only the halfword that holds it. */
#define CARD_WORD(card)     (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)       (((word) << 20) >> 20)
/* 1 if the card word holds a card (ID != 0), tested with a single lsl #20; the ROM uses both forms. */
#define HAS_CARD(card)      (CARD_WORD(card) << 20 != 0)
/* Low and high halfword of a card word in memory: the operands of the card-word duel commands. */
#define CARD_WORD_LO(card)  (((u16 *)&(card))[0])
#define CARD_WORD_HI(card)  (((u16 *)&(card))[1])

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber
 * (0x08622AB4). Matching: these give the ROM's literal pools; the symbol forms generate other code.
 */
#define CARD_STATS(id)      (((const u32 *)0x08621DE0)[CARD_ID_MASK & (id)])
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[CARD_ID_MASK & (id)])
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */

/*
 * &gDuelZones[player].zones[zone] by integer arithmetic, zone term first (ZONE_AT_PLAYER_FIRST: player term
 * first). Matching: these are the ROM's address orders (array indexing emits the player term first).
 * player must be 0 or 1: a ChainEntry's player bit, or a location's player masked with & 1.
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
#define ZONE_AT_PLAYER_FIRST(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))

/* A packed location player | zone << 8 (DUEL_LOC). Matching: the ROM evaluates the player first; DUEL_LOC's
 * zone-first order gives other registers. */
#define LOC(player, zone)   ((player) | ((zone) << 8))
/* The player byte of a packed location (DUEL_LOC) in memory: its low byte, read with ldrb. */
#define LOC_PLAYER_BYTE(loc) (((u8 *)&(loc))[0])
/* The low byte of a cursor word (gDuelScreen.selArea / selIndex), read with ldrb. */
#define LOW_BYTE(x)         (*(u8 *)&(x))
/* The DUEL_CMD_POINT_AT_CARD operand area | index << 8 of the cursor selection, as the u16 the command takes. */
#define CURSOR_AREA_INDEX   (u16)(LOW_BYTE(gDuelScreen.selArea) | (LOW_BYTE(gDuelScreen.selIndex) << 8))

/*
 * The player bit of a chain entry read as the raw byte at +0x02. Matching: the duel-command selectors test
 * it as ldrb; and #1 (the ChainEntry.player bitfield gives lsl/lsr).
 */
#define LINK_PLAYER_BYTE(link) (1 & ((u8 *)(link))[2])
/* The number of targets (ChainEntry.numTargets, +0x0A bits 0-2) as ldrb; and #7. Matching: where the count
 * is kept in a variable, a bitfield read gives other code (compares of link->numTargets match either way). */
#define LINK_NUM_TARGETS_BYTE(link) (7 & ((u8 *)(link))[0xA])
/* The command id for the link's player (player 1's commands carry DUEL_CMD_PLAYER), and for its opponent. */
#define CMD_FOR(link, cmd)          (LINK_PLAYER_BYTE(link) ? DUEL_CMD_PLAYER | (cmd) : (cmd))
#define CMD_FOR_OPPONENT(link, cmd) (LINK_PLAYER_BYTE(link) == 0 ? DUEL_CMD_PLAYER | (cmd) : (cmd))
/* The command id for a player number. */
#define CMD_FOR_PLAYER(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* Text box placement of the effect prompts (TextBoxOpen pos = x | y << 8, size = width | height << 8). */
#define PROMPT_POS          0x206   /* x 6, y 2 */
#define PROMPT_SIZE         0x712   /* 18 x 7 cells */
#define PICK_PROMPT_SIZE    0x412   /* 18 x 4 cells: the tribute picks, which leave the field visible */

/* Marks a used byte of the tribute word (targets[2]): (zone | TRIBUTE_ZONE_SET), one tribute per byte. */
#define TRIBUTE_ZONE_SET    0x80

/* Shield & Sword: toggle the duel-wide swap of original ATK and DEF (gDuel.atkDefSwapped). */
int EffectShieldAndSwordResolve(struct ChainEntry *link)
{
    if (!link->negated)
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_ATK_DEF_SWAPPED), !gDuel.atkDefSwapped, 0, 0);
    return EFFECT_STEP_DONE;
}

/* Steps of EffectGracefulCharityResolve. */
enum GracefulCharityStep {
    CHARITY_STEP_DRAW = EFFECT_STEP_START,  /* 0x80: draw 3 */
    CHARITY_STEP_DISCARD = 0x7F,            /* discard 2 (the prompt runs on its own) */
    CHARITY_STEP_END = 0x7E,                /* no case: done on the next call */
};

/* Graceful Charity: draw 3 cards, then discard 2. */
int EffectGracefulCharityResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case CHARITY_STEP_DRAW:
            DrawCards(link->player, 3);
            return CHARITY_STEP_DISCARD;
        case CHARITY_STEP_DISCARD:
            DuelPrompt_PostDiscard(link->player, 2, 0, 0);
            return CHARITY_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectChainDestructionResolve. */
enum ChainDestructionStep {
    CHAIN_DESTRUCTION_STEP_START = EFFECT_STEP_START,   /* 0x80: the summoned monster must still be there */
    CHAIN_DESTRUCTION_STEP_HAND = 0x7F,     /* discard the next same-name hand card (gChain.effectSubStep) */
    CHAIN_DESTRUCTION_STEP_DECK = 0x7E,     /* send the deck copies to the graveyard */
    CHAIN_DESTRUCTION_STEP_SHUFFLE = 0x7D,
    CHAIN_DESTRUCTION_STEP_END = 0x78,      /* no case: done on the next call */
};

/*
 * Chain Destruction: send every copy of the monster just summoned (loc0) from its summoner's hand and deck to
 * the graveyard, then shuffle that deck. The hand is walked from gChain.effectSubStep; a discard closes the
 * gap, so the same index is tested again on the next call.
 */
int EffectChainDestructionResolve(struct ChainEntry *link)
{
    int player = LOC_PLAYER_BYTE(link->loc0);
    int zone = DUEL_LOC_ZONE(link->loc0);
    int side = 1 & player;
    struct DuelZone *summoned = ZONE_AT(side, zone);
    u16 id = CARD_ID(CARD_WORD(summoned->card));

    if (!link->negated) {
        switch (gChain.effectStep) {
        case CHAIN_DESTRUCTION_STEP_START:
            if (id == 0)
                return EFFECT_STEP_DONE;
            gChain.effectSubStep = 0;
            gChain.effectStep--;
            /* fall through */
        case CHAIN_DESTRUCTION_STEP_HAND:
            while (gChain.effectSubStep < gDuelPlayers[1 & player].handCount) {
                if (IsSameCardName(CARD_ID(CARD_WORD(gDuelPlayers[1 & player].hand[gChain.effectSubStep])), id) != 0) {
                    DiscardHandCard(player, gChain.effectSubStep, 1, 1);
                    return CHAIN_DESTRUCTION_STEP_HAND;
                }
                gChain.effectSubStep++;
            }
            return CHAIN_DESTRUCTION_STEP_DECK;
        case CHAIN_DESTRUCTION_STEP_DECK:
            SendDeckCopiesToGraveyard(player, CARD_NUMBER(id), 1);
            return CHAIN_DESTRUCTION_STEP_SHUFFLE;
        case CHAIN_DESTRUCTION_STEP_SHUFFLE:
            DuelCmd_Push(CMD_FOR_PLAYER(player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
            return CHAIN_DESTRUCTION_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Mesmeric Control: the opponent cannot change battle positions (the lock is cleared at the end of a turn). */
int EffectMesmericControlResolve(struct ChainEntry *link)
{
    if (!link->negated)
        DuelCmd_Push(CMD_FOR_OPPONENT(link, DUEL_CMD_SET_POSITION_CHANGE_LOCK), 1, 0, 0);
    return EFFECT_STEP_DONE;
}

/*
 * Magic-Arm Shield: if the attack it answers still stands and the target passes EffectMagicArmShieldCheck,
 * take control of the target until the battle ends (moved into a free monster zone of the player, which is
 * kept in targets[1]) and make it the attack target.
 */
int EffectMagicArmShieldResolve(struct ChainEntry *link, int chainedTo)
{
    if (!link->negated && EffectAttackResponsePrepare(link, chainedTo, 0) != 0) {
        if (link->numTargets == 1 && EffectMagicArmShieldCheck(link, link->targets[0]) != 0) {
            int zone = FindFreeMonsterZone(link->player);

            link->targets[1] = LOC(link->player, (u8)zone);
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_RETURN_AFTER_BATTLE), link->targets[0], 1, 0);
            MoveFieldCard(link->player, link->targets[0], link->targets[1]);
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_ATTACK_TARGET), link->targets[1], 1, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Fissure: destroy the opponent's face-up monster with the lowest ATK (the first one on a tie). If it cannot
 * be targeted (IsZoneTargetable), the card is only shown.
 */
int EffectFissureResolve(struct ChainEntry *link)
{
    int lowestAtk = 99999;  /* above any ATK */
    int lowestZone = -1;

    if (!link->negated) {
        int zone;

        for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
            int side = (1 - link->player) & 1;
            struct DuelZone *monster = ZONE_AT(side, zone);

            /* FAKEMATCH: the r1 clobber keeps the player shift and the zone product in the ROM's scratch
             * registers. No code emitted. */
            __asm__("" : : : "r1");
            if (HAS_CARD(monster->card)) {
                int side2 = (1 - link->player) & 1;
                struct DuelZone *monster2 = ZONE_AT(side2, zone);

                if (monster2->isFaceUp) {
                    int atk = GetZoneCardAtkInt(1 - link->player, zone);

                    if (atk < lowestAtk) {
                        lowestAtk = atk;
                        lowestZone = zone;
                    }
                }
            }
        }
        if (lowestZone != -1) {
            if (IsZoneTargetable(1 - link->player, lowestZone) != 0) {
                DestroyFieldCardByEffect(1 - link->player, lowestZone);
                OnCardDestroyedByEffect(link->player, 1 - link->player, lowestZone);
            } else {
                int side3 = (1 - link->player) & 1;
                struct DuelZone *monster3 = ZONE_AT(side3, lowestZone);

                if (HAS_CARD(monster3->card)) {
                    ShowCardEffect(link->player,
                                   CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST((1 - link->player) & 1, lowestZone)->card)));
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* 1 if value <= 500, else 0. Matching: the if form gives the ROM's movs #0; cmp; bgt; movs #1. */
static inline int IsAtMost500(int value)
{
    int result = 0;

    if (value <= 500)
        result = 1;
    return result;
}

/* 1 if value > 999 (1000 or more), else 0. */
static inline int IsAtLeast1000(int value)
{
    int result = 0;

    if (value > 999)
        result = 1;
    return result;
}

/*
 * Trap Hole, House of Adhesive Tape, Eatgaboon: destroy the monster just summoned or flip summoned (loc0) if
 * it can be targeted and passes the card's test: Trap Hole ATK 1000 or more, House of Adhesive Tape DEF 500
 * or less, Eatgaboon ATK 500 or less.
 */
int EffectTrapHoleResolve(struct ChainEntry *link)
{
    int player = LOC_PLAYER_BYTE(link->loc0);
    int zone = DUEL_LOC_ZONE(link->loc0);
    int passes = 1;

    /* FAKEMATCH: preserve the initialized default result separately
     * from the later player mask, matching the ROM's two constants. */
    __asm__("" : "+r"(passes));
    if (!link->negated && (link->event == RESPONSE_SUMMONED || link->event == RESPONSE_FLIP_SUMMONED)) {
        int side = 1 & player;
        struct DuelZone *summoned = ZONE_AT(side, zone);

        if (HAS_CARD(summoned->card) && CanCardTargetZoneInt(link->card, player, zone) != 0) {
            switch (CARD_NUMBER(link->card)) {
            case CARD_TRAP_HOLE:
                passes = IsAtLeast1000(GetZoneCardAtkInt(player, zone));
                break;
            case CARD_HOUSE_OF_ADHESIVE_TAPE:
                passes = IsAtMost500(GetZoneCardDefInt(player, zone));
                break;
            case CARD_EATGABOON:
                passes = IsAtMost500(GetZoneCardAtkInt(player, zone));
                break;
            }
            if (passes != 0) {
                DestroyFieldCardByEffect(player, zone);
                OnCardDestroyedByEffect(link->player, player, zone);
            }
        }
    }
    /* FAKEMATCH: reserve r5 at the return so link/zone use r5/r4. */
    __asm__("" : : : "r5");
    return EFFECT_STEP_DONE;
}

/* Remove Trap: destroy the targeted card if it is still a face-up Trap. */
int EffectRemoveTrapResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int numTargets = LINK_NUM_TARGETS_BYTE(link);

        if (numTargets == 1) {
            int player = LOC_PLAYER_BYTE(link->targets[0]);
            int zone = DUEL_LOC_ZONE(link->targets[0]);
            /* FAKEMATCH: numTargets is 1 here; the ROM reuses that register as the player mask */
            int side = numTargets & player;
            struct DuelZone *target = ZONE_AT(side, zone);
            u32 id = CARD_ID(CARD_WORD(target->card));

            if (id != 0 && CARD_TYPE(id) == CARD_TYPE_TRAP && target->isFaceUp)
                DestroyFieldCard(player, zone, 1);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Two-Pronged Attack: destroy the three targets (two of the player's monsters and one of the opponent's),
 * but only if all three are still on the field. */
int EffectTwoProngedAttackResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 3) {
        int i;

        for (i = 0; i < link->numTargets; i++) {
            register int offset __asm__("r1") = i * 2;
            register u8 *base __asm__("r0") = (u8 *)link;
            u16 *target;
            int zone, side;
            struct DuelZone *card;

            /* FAKEMATCH: initialized index/base constraints retain the ROM's
             * per-iteration address setup rather than a walking pointer.
             * The second input keeps +0xC on the base. No code emitted. */
            asm("" : "+r"(offset), "+r"(base));
            base += OFFSET_OF(struct ChainEntry, targets);
            asm("" : : "r"(base));
            target = (u16 *)(base + offset);     /* &link->targets[i] */
            zone = DUEL_LOC_ZONE(*target);
            side = 1 & LOC_PLAYER_BYTE(*target);
            card = ZONE_AT(side, zone);

            if (!CARD_ID(CARD_WORD(card->card)))
                return EFFECT_STEP_DONE;
        }
        for (i = 0; i < link->numTargets; i++) {
            register int offset __asm__("r1") = i * 2;
            register u8 *base __asm__("r0") = (u8 *)link;
            u16 *target;
            int player, zone;

            /* FAKEMATCH: same initialized address setup as the check loop. */
            asm("" : "+r"(offset), "+r"(base));
            base += OFFSET_OF(struct ChainEntry, targets);
            asm("" : : "r"(base));
            target = (u16 *)(base + offset);
            player = LOC_PLAYER_BYTE(*target);
            zone = DUEL_LOC_ZONE(*target);

            DestroyFieldCardByEffect(player, zone);
            OnCardDestroyedByEffect(link->player, player, zone);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Monster Reborn (also key 1241): Special Summon the chosen graveyard monster to the player's field. Its card
 * word is stored in targets[0] (low half) and targets[1] (high half); the owner bit says whose graveyard it is
 * in. Monster Reborn does nothing while Call of the Dark is active on either field.
 */
int EffectMonsterRebornResolve(struct ChainEntry *link)
{
    struct DuelCard found;
    struct DuelCard card;

    if (!link->negated) {
        if (CARD_NUMBER(link->card) == CARD_MONSTER_REBORN) {
            if (CountActiveCardsOnField2(0, CARD_CALL_OF_THE_DARK) > 0
                || CountActiveCardsOnField2(1, CARD_CALL_OF_THE_DARK) > 0)
                return EFFECT_STEP_DONE;
        }
        if (link->numTargets == 2 && CountFreeMonsterZones(link->player) > 0) {
            CARD_WORD(card) = link->targets[1] << 16 | link->targets[0];
            if (GetGraveyardCardById(card.owner, card.id, &found) != 0) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD), link->targets[0], link->targets[1], 0);
                QueueSpecialSummonChoosePosition(link->player, &card, 1,
                                                 ZONE_STATUS_MONSTER_REBORN | ZONE_STATUS_FROM_GRAVEYARD);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Pot of Greed (2 cards), Skelengel and key 1447 (1 card): draw. */
int EffectDrawCardsResolve(struct ChainEntry *link)
{
    int count = 0;

    if (!link->negated) {
        switch (CARD_NUMBER(link->card)) {
        case CARD_SKELENGEL:
        case CARD_1447:
            count = 1;
            break;
        case CARD_POT_OF_GREED:
            count = 2;
            break;
        }
        if (count > 0)
            DrawCards(link->player, count);
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectBanishGraveyardCardsResolve; gChain.effectSubStep counts the cards still allowed. */
enum BanishGraveyardStep {
    BANISH_GRAVE_STEP_START = EFFECT_STEP_START,    /* 0x80: up to 2 (Gravedigger Ghoul) or 5 (Soul Release) */
    BANISH_GRAVE_STEP_PROMPT = 0x7F,        /* a graveyard card left? "Select a card in Graveyard ..." */
    BANISH_GRAVE_STEP_OPEN_LIST = 0x7E,     /* the card-list viewer with the candidates */
    BANISH_GRAVE_STEP_BANISH = 0x7D,        /* banish the picked card */
    BANISH_GRAVE_STEP_ASK_MORE = 0x7C,      /* "remove from play another card?" (Yes/No) */
    BANISH_GRAVE_STEP_ANSWER = 0x7B,        /* Yes: back to BANISH_GRAVE_STEP_PROMPT */
};

/* Gravedigger Ghoul (up to 2) and Soul Release (up to 5): banish graveyard cards one by one, asking after each
 * one whether to go on. */
int EffectBanishGraveyardCardsResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case BANISH_GRAVE_STEP_START:
            switch (CARD_NUMBER(link->card)) {
            case CARD_GRAVEDIGGER_GHOUL:
                gChain.effectSubStep = 2;
                break;
            case CARD_SOUL_RELEASE:
                gChain.effectSubStep = 5;
                break;
            }
            gChain.effectStep--;
            /* fall through */
        case BANISH_GRAVE_STEP_PROMPT:
            if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) <= 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectGraveyardCardToBanish);
            return BANISH_GRAVE_STEP_OPEN_LIST;
        case BANISH_GRAVE_STEP_OPEN_LIST:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return BANISH_GRAVE_STEP_BANISH;
        case BANISH_GRAVE_STEP_BANISH: {
            u32 *pick = &gCardListView.cards[gCardListView.cursorRow + gCardListView.top];

            /* the command's player is the card's owner: bit 12 tested as the sign bit (lsl #19) */
            DuelCmd_Push((int)(gCardListView.cards[gCardListView.cursorRow + gCardListView.top] << 19) < 0
                             ? DUEL_CMD_PLAYER | DUEL_CMD_BANISH_GRAVEYARD_CARD
                             : DUEL_CMD_BANISH_GRAVEYARD_CARD,
                         CARD_WORD_LO(*pick), CARD_WORD_HI(*pick), 0);
            gChain.effectSubStep--;
            if (gChain.effectSubStep == 0)
                return EFFECT_STEP_DONE;
            return BANISH_GRAVE_STEP_ASK_MORE;
        }
        case BANISH_GRAVE_STEP_ASK_MORE:
            if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrBanishAnotherGraveyardCardQuestion);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return BANISH_GRAVE_STEP_ANSWER;
        case BANISH_GRAVE_STEP_ANSWER:
            if (gTextBox.result == 0)
                return EFFECT_STEP_DONE;
            return BANISH_GRAVE_STEP_PROMPT;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * The Inexperienced Spy (human player): pick a card in the opponent's hand with the cursor and show it.
 * Returns EFFECT_STEP_START (the same step again) until the pick is confirmed. Quirk: the hand tested is the
 * player's own (EffectOpponentHasHandPrepare checked the opponent's).
 */
int EffectTheInexperiencedSpyResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (gDuelPlayers[1 & link->player].handCount != 0) {
            if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_HAND)) != 0) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), (u16)gDuelScreen.selPlayer,
                             CURSOR_AREA_INDEX, 0);
                ShowCardDetail(link->player,
                               CARD_ID(CARD_WORD(gDuelPlayers[(1 - link->player) & 1].hand[gDuelScreen.selIndex])));
            } else
                goto pick_again;    /* Matching: the ROM's branch layout */
        }
    }
    return EFFECT_STEP_DONE;
pick_again:
    return EFFECT_STEP_START;
}

/*
 * Reinforcements, Castle Walls, Rush Recklessly, The Reliable Guardian, Snake Fang, key 1534: link the card's
 * ID to the targeted face-up monster (ZONE_LINK_CARD_EFFECT). GetZoneCardStats applies the bonus for each such
 * link (+500/+700 ATK or DEF, -500 DEF) until the end of the turn.
 */
int EffectAddStatModifierResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int numTargets = LINK_NUM_TARGETS_BYTE(link);

        if (numTargets == 1) {
            u16 target = link->targets[0];
            int zone = DUEL_LOC_ZONE(target);
            /* FAKEMATCH: numTargets is 1 here; the ROM reuses that register as the player mask */
            int side = numTargets & LOC_PLAYER_BYTE(link->targets[0]);
            struct DuelZone *monster = ZONE_AT(side, zone);

            if (monster->isFaceUp && CARD_ID(CARD_WORD(monster->card)))
                QueueAddZoneLink(link->player, link->card, target, ZONE_LINK_CARD_EFFECT);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectUltimateOfferingResolve. targets[0] = hand index, targets[1] = monster zone (or first
 * tribute), targets[2] = tribute word (zone | TRIBUTE_ZONE_SET per byte). */
enum UltimateOfferingStep {
    OFFERING_STEP_START = EFFECT_STEP_START,    /* 0x80: a summonable hand monster? "Select a monster ..." */
    OFFERING_STEP_PICK_MONSTER = 0x7F,      /* hand pick; branches on the level */
    OFFERING_STEP_ASK_TWO = 0x78,           /* level 7+: "requires 2 Tributes. Summon?" (Yes/No) */
    OFFERING_STEP_ANSWER_TWO = 0x77,        /* No: back to OFFERING_STEP_START */
    OFFERING_STEP_PICK_FIRST = 0x76,        /* pick the first tribute */
    OFFERING_STEP_PROMPT_SECOND = 0x75,
    OFFERING_STEP_PICK_SECOND = 0x74,       /* pick another monster as the second tribute */
    OFFERING_STEP_ASK_ONE = 0x6E,           /* level 5-6: "requires 1 Tribute. Summon?" (Yes/No) */
    OFFERING_STEP_ANSWER_ONE = 0x6D,        /* No: back to OFFERING_STEP_START */
    OFFERING_STEP_PICK_ONE = 0x6C,          /* pick the tribute */
    OFFERING_STEP_SUMMON = EFFECT_STEP_END, /* 0x64: queue the Normal Summon */
    OFFERING_STEP_END = 0x0A,               /* no case: done on the next call */
};

/*
 * Ultimate Offering: an extra Normal Summon from the hand, picked with the cursor, with the tribute prompts
 * for level 5 and up (gTributeSummonPrompts[0-4]). Answering No to a tribute prompt starts over.
 */
int EffectUltimateOfferingResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case OFFERING_STEP_START: {
            int i;

            for (i = 0; i < gDuelPlayers[1 & link->player].handCount; i++) {
                u16 id = CARD_ID(CARD_WORD(gDuelPlayers[1 & link->player].hand[i]));

                if (CanSummonFromHand(link->player, id) != 0 && IsSpecialSummonOnly(id) == 0) {
                    TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectMonsterToSummonFromHand);
                    return OFFERING_STEP_PICK_MONSTER;
                }
            }
            break;
        }
        case OFFERING_STEP_PICK_MONSTER:
            if (DuelCursor_PickTarget(PICK_HAND) != 0) {
                u32 handIndex = gDuelScreen.selIndex;
                u16 id = CARD_ID(CARD_WORD(gDuelPlayers[link->player].hand[handIndex]));

                if (CanSummonFromHand(link->player, id) != 0 && IsSpecialSummonOnly(id) == 0) {
                    int level;

                    link->targets[0] = handIndex;
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
                    switch (level) {
                    case 1:
                    case 2:
                    case 3:
                    case 4:
                        link->targets[1] = FindFreeMonsterZone(link->player);
                        link->targets[2] = 0;
                        return OFFERING_STEP_SUMMON;
                    case 5:
                    case 6:
                        return OFFERING_STEP_ASK_ONE;
                    default:
                        return OFFERING_STEP_ASK_TWO;
                    }
                }
                PlaySE(SE_ERROR);
            }
            return OFFERING_STEP_PICK_MONSTER;
        case OFFERING_STEP_ASK_TWO:
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gTributeSummonPrompts[1]);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return OFFERING_STEP_ANSWER_TWO;
        case OFFERING_STEP_ANSWER_TWO:
            if (gTextBox.result == 0)
                return OFFERING_STEP_START;
            TextBoxOpen(PROMPT_POS, PICK_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gTributeSummonPrompts[2]);
            return OFFERING_STEP_PICK_FIRST;
        case OFFERING_STEP_PICK_FIRST:
            if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
                if (IsTributableMonsterInt(gDuelScreen.selPlayer, gDuelScreen.selIndex) != 0) {
                    PlaySE(SE_CONFIRM);
                    DuelCmd_Push(DUEL_CMD_POINT_AT_CARD, (u16)gDuelScreen.selPlayer,
                                 CURSOR_AREA_INDEX, 0);
                    link->targets[1] = gDuelScreen.selIndex;
                    link->targets[2] = (u8)((int)gDuelScreen.selIndex | TRIBUTE_ZONE_SET) << 8;
                    return OFFERING_STEP_PROMPT_SECOND;
                }
                PlaySE(SE_ERROR);
            }
            return OFFERING_STEP_PICK_FIRST;
        case OFFERING_STEP_PROMPT_SECOND:
            TextBoxOpen(PROMPT_POS, PICK_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gTributeSummonPrompts[3]);
            return OFFERING_STEP_PICK_SECOND;
        case OFFERING_STEP_PICK_SECOND:
            if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
                if (IsTributableMonsterInt(gDuelScreen.selPlayer, gDuelScreen.selIndex) != 0
                    && gDuelScreen.selIndex != link->targets[1]) {
                    PlaySE(SE_CONFIRM);
                    DuelCmd_Push(DUEL_CMD_POINT_AT_CARD, (u16)gDuelScreen.selPlayer,
                                 CURSOR_AREA_INDEX, 0);
                    link->targets[2] = (u8)((int)gDuelScreen.selIndex | TRIBUTE_ZONE_SET) | link->targets[2];
                    return OFFERING_STEP_SUMMON;
                }
                PlaySE(SE_ERROR);
            }
            return OFFERING_STEP_PICK_SECOND;
        case OFFERING_STEP_ASK_ONE:
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gTributeSummonPrompts[0]);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return OFFERING_STEP_ANSWER_ONE;
        case OFFERING_STEP_ANSWER_ONE:
            if (gTextBox.result == 0)
                return OFFERING_STEP_START;
            TextBoxOpen(PROMPT_POS, PICK_PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gTributeSummonPrompts[4]);
            return OFFERING_STEP_PICK_ONE;
        case OFFERING_STEP_PICK_ONE:
            if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
                if (IsTributableMonsterInt(gDuelScreen.selPlayer, gDuelScreen.selIndex) != 0) {
                    PlaySE(SE_CONFIRM);
                    DuelCmd_Push(DUEL_CMD_POINT_AT_CARD, (u16)gDuelScreen.selPlayer,
                                 CURSOR_AREA_INDEX, 0);
                    link->targets[1] = gDuelScreen.selIndex;
                    link->targets[2] = (u8)((int)gDuelScreen.selIndex | TRIBUTE_ZONE_SET);
                    return OFFERING_STEP_SUMMON;
                }
                PlaySE(SE_ERROR);
            }
            return OFFERING_STEP_PICK_ONE;
        case OFFERING_STEP_SUMMON:
            QueueNormalSummonChoosePosition(link->player, link->targets[0], link->targets[1], link->targets[2]);
            return OFFERING_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Ancient Telescope: show the cards collected for its effect (the opponent's top deck cards) in the
 * card-list viewer. */
int EffectAncientTelescopeResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        CollectEffectTargets(link->player, CARD_NUMBER(link->card), 0);
        CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Counter traps: negate and destroy chainedTo (DUEL_CMD_NEGATE_ACTIVATION, then DUEL_CMD_SET_SPELL_TRAP_DISABLED
 * on its zone) when it is the card this one counters:
 *   White Hole       the opponent's Dark Hole; then destroys the opponent's monsters
 *   Call of the Grave  the opponent's Monster Reborn
 *   Anti Raigeki     the opponent's Raigeki; then destroys the opponent's monsters
 *   Magic Jammer     any Magic card
 *   Seven Tools of the Bandit  any Trap card
 *   Gryphon Wing     the opponent's Harpie's Feather Duster; then destroys the opponent's spell/trap and
 *                    field cards
 *   key 1530         a monster with a flip effect (negation only)
 *   key 1531         the equip spells and the other targeting Magic cards listed below
 */
int EffectNegateChainedCardResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    int zone;

    if (!link->negated && chainedTo != NULL) {
        switch (CARD_NUMBER(link->card)) {
        case CARD_WHITE_HOLE:
            if (CARD_NUMBER(chainedTo->card) == CARD_DARK_HOLE && LINK_PLAYER_BYTE(chainedTo) != LINK_PLAYER_BYTE(link)) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
                DuelCmd_Push(CMD_FOR(chainedTo, DUEL_CMD_SET_SPELL_TRAP_DISABLED), chainedTo->zone, 1, 0);
                for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                    int side = (1 - link->player) & 1;
                    struct DuelZone *monster = ZONE_AT(side, zone);

                    if (HAS_CARD(monster->card)) {
                        DestroyFieldCardByEffect(1 - link->player, zone);
                        OnCardDestroyedByEffect(link->player, 1 - link->player, zone);
                    }
                }
            }
            return EFFECT_STEP_DONE;
        case CARD_CALL_OF_THE_GRAVE:
            if (CARD_NUMBER(chainedTo->card) == CARD_MONSTER_REBORN
                && LINK_PLAYER_BYTE(chainedTo) != LINK_PLAYER_BYTE(link)) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
                DuelCmd_Push(CMD_FOR(chainedTo, DUEL_CMD_SET_SPELL_TRAP_DISABLED), chainedTo->zone, 1, 0);
            }
            break;
        case CARD_ANTI_RAIGEKI:
            if (CARD_NUMBER(chainedTo->card) == CARD_RAIGEKI && LINK_PLAYER_BYTE(chainedTo) != LINK_PLAYER_BYTE(link)) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
                DuelCmd_Push(CMD_FOR(chainedTo, DUEL_CMD_SET_SPELL_TRAP_DISABLED), chainedTo->zone, 1, 0);
                for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                    int side = (1 - link->player) & 1;
                    struct DuelZone *monster = ZONE_AT(side, zone);

                    if (HAS_CARD(monster->card)) {
                        DestroyFieldCardByEffect(1 - link->player, zone);
                        OnCardDestroyedByEffect(link->player, 1 - link->player, zone);
                    }
                }
            }
            return EFFECT_STEP_DONE;
        case CARD_MAGIC_JAMMER:
            if (CARD_TYPE(chainedTo->card) == CARD_TYPE_MAGIC) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
                DuelCmd_Push(CMD_FOR(chainedTo, DUEL_CMD_SET_SPELL_TRAP_DISABLED), chainedTo->zone, 1, 0);
            }
            return EFFECT_STEP_DONE;
        case CARD_SEVEN_TOOLS_OF_THE_BANDIT:
            if (CARD_TYPE(chainedTo->card) == CARD_TYPE_TRAP) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
                DuelCmd_Push(CMD_FOR(chainedTo, DUEL_CMD_SET_SPELL_TRAP_DISABLED), chainedTo->zone, 1, 0);
            }
            return EFFECT_STEP_DONE;
        case CARD_GRYPHON_WING:
            if (CARD_NUMBER(chainedTo->card) == CARD_HARPIES_FEATHER_DUSTER
                && LINK_PLAYER_BYTE(chainedTo) != LINK_PLAYER_BYTE(link)) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
                DuelCmd_Push(CMD_FOR(chainedTo, DUEL_CMD_SET_SPELL_TRAP_DISABLED), chainedTo->zone, 1, 0);
                for (zone = ZONE_SPELL_0; zone <= ZONE_FIELD; zone++) {
                    int side = (1 - link->player) & 1;
                    struct DuelZone *card = ZONE_AT(side, zone);

                    if (HAS_CARD(card->card)) {
                        DuelCmd_Push(CMD_FOR_OPPONENT(link, DUEL_CMD_SET_DESTROYED_BY_OPPONENT_FLAG), zone, 1, 0);
                        DestroyFieldCard(1 - link->player, zone, 1);
                    }
                }
            }
            return EFFECT_STEP_DONE;
        case CARD_1530:
            if (CARD_TYPE(chainedTo->card) <= CARD_TYPE_REPTILE     /* a monster */
                && (HasFlipEffect(CARD_NUMBER(chainedTo->card), 1) != 0
                    || HasFlipEffect(CARD_NUMBER(chainedTo->card), 0) != 0))
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
            break;
        case CARD_1531:
            switch (CARD_NUMBER(chainedTo->card)) {
            case CARD_LEGENDARY_SWORD ... CARD_CYBER_SHIELD:        /* equip spells 300-316 */
            case CARD_MYSTICAL_MOON ... CARD_POWER_OF_KAISHIN:      /* equip spells 318-327 */
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
            case CARD_1448 ... CARD_1451:
            case CARD_1540:
            case CARD_1546:
            case CARD_1548:
            case CARD_1550:
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
                DuelCmd_Push(CMD_FOR(chainedTo, DUEL_CMD_SET_SPELL_TRAP_DISABLED), chainedTo->zone, 1, 0);
                break;
            }
            break;
        }
    }
    return EFFECT_STEP_DONE;
}
