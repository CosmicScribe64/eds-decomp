#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_TYPE / LEVEL, gCardNames */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum ResponseEventKind, ChainEntryKind, ZoneLinkKind, ZoneStatusFlag */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* ids, DUEL_CMD_PLAYER */

/*
 * Card effect handlers (wiki/functions/effect-prepare3-c.md). Three groups, all reached through gCardEffects
 * (struct CardEffect, include/effect.h) except DestroyFieldCardByEffect:
 *
 *  - Prepare handlers (+0x0C) of effect keys 1425-1549, which have no EDS card: side-effect-free "may this
 *    card be activated now?" predicates run by CanActivateEffect. Parameters: card is the activating card
 *    (its ID, player and zone, the event that opened the response window and the event's locations loc0 /
 *    loc1); chainLink is the activation it answers, or NULL; fromHand is 1 when it is played from the hand.
 *    Handlers that do not use the trailing parameters are defined without them.
 *  - DestroyFieldCardByEffect, the helper behind every destruction by a card effect.
 *  - Resolve handlers (+0x04) of EDS cards, run by Chain_Resolve with the link to resolve. Most do nothing
 *    when the activation was negated. One-shot handlers return EFFECT_STEP_DONE; the multi-frame ones are
 *    state machines on gChain.effectStep, which Chain_Resolve starts at EFFECT_STEP_START (0x80) and then
 *    sets to each return value, until a handler returns EFFECT_STEP_DONE.
 */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit uses, with the header's names, types
 * and bitfield containers (unused bytes are padding), and defines duel.h's include guard so that chain.h,
 * duel_cmd.h, duel_link.h and card_list_view.h do not pull in the legacy header. After H0, replace the block
 * (BEGIN to END) with #include "legacy/duel.h". */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:1;
    u32 unk14:1;                    /* bit 14: summon status bit (ZONE_STATUS_UNK14) */
    u32 unk15:17;
};

/* The status bits of a card word as single-bit u8 fields (byte accesses). */
struct DuelCardStatusBytes {
    u8 cardIdLow;                   /* +0x00: card word bits 0-7 */
    u8 cardWordBits8to13:6;         /* +0x01: id (high bits), owner, unk13 */
    u8 unk14:1;                     /* +0x01 bit 6 = card bit 14 */
    u8 normalSummoned:1;            /* bit 15 */
    u8 unk2[2];
    u8 restOfZone[0x94 - 4];
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0: defense position */
    u8 isFaceUp:1;                  /* +0x06 bit 1: face up */
    u8 unk6_2:6;
    u8 unk7[3];
    u16 links[32];                  /* +0x0A */
    u16 linkKinds[32];              /* +0x4A */
    u16 numLinks;                   /* +0x8A */
    u8 unk8C[8];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 unk4[0x28 - 0x4];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4 */
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

void CopyDuelCard(u32 *dst, u32 *src);
int CountGraveyardCardsByNumber(int player, u16 cardNo);
int CountGraveyardMonsters(int player);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int CountMonstersFiltered(int player, u16 faceUpOnly, u16 attackPosOnly);
int CountSpellTrapsFiltered(int player, u16 faceUp, u16 faceDown, u16 includeField);
int CountTributableMonsters(int player, int excludeZone);
u32 GetZoneCardAtk(u32 player, u32 slot);
u32 GetZoneCardType(s32 player, s32 slot);
/* ---- END duel.h stand-in ---- */

#include "chain.h"                  /* struct ChainEntry, gChain */
#include "effect.h"                 /* enum EffectStep, enum SevenCompletedStat */
#include "effect_handlers.h"        /* the prototypes of this unit's handlers, EffectTributeTargetChainB */
#include "duel_actions.h"           /* DestroyFieldCard, TributeMonster, EquipCard, LoseLifePoints, ... */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl */
#include "duel_link.h"              /* gLinkState, DuelLink_SendDeck */
#include "duel_prompt.h"            /* DeckReorder_Run */
#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "ai.h"                     /* gAiWork, AiPickCardListEntry */
#include "util.h"                   /* FormatStr, HalveRoundUp */

/* Prompt texts used only here. */
extern const u8 gStrNoDeckCardsToAdd[];             /* 'There are no cards to be added from your Deck.' */
extern const u8 gStrSelectDeckMonsterToAdd[];       /* 'Please select a monster in the list to be added ...' */
extern const char gStrTributeToReturnToDeckPrompt[];/* '%s has been sent to the Graveyard. ... tribute ...' */
extern const char gStrPayLpToReturnToDeckPrompt[];  /* '%s has been sent to the Graveyard. ... pay 500LP ...' */

/* gCardNumberToId[CARD_AXE_OF_DESPAIR] (0x08624052), an alias symbol the ROM loads directly. */
extern const u16 gCardNumberToId_AxeOfDespair;

/*
 * Local views of callees (HEADERS.md, "Keeping a deliberate local view"). Matching: the ROM uses the
 * results without narrowing them (and compares them as signed ints); the header's u16 return types would
 * add a narrowing of r0. EffectDarkHolePrepare is defined with one parameter but called here with three
 * (r1 and r2 are set) and its result narrowed to u16.
 */
int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
int IsValidEquipTargetInt(int equipPlayer, int equipSlot, int targetPlayer, int targetSlot)
    asm("IsValidEquipTarget");
int FindMonsterLinkedToCardInt(int player, int slot) asm("FindMonsterLinkedToCard");
u16 EffectDarkHolePrepare3(struct ChainEntry *card, struct ChainEntry *chainLink, int fromHand)
    asm("EffectDarkHolePrepare");
/* Matching: two handlers pass a command id held in an int register; the header's u16 cmd parameter would
 * narrow it at the call (lsl/lsr #16). */
void DuelCmd_PushInt(int cmd, u16 arg2, u16 arg4, int arg6) asm("DuelCmd_Push");

/* The card word of a zone as one u32. Matching: the ROM loads the whole word (ldr) and extracts the ID with
 * shifts; a .id bitfield read would load only the halfword that holds it. */
#define CARD_WORD(card)     (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)       (((word) << 20) >> 20)
/* The card ID masked to 11 bits (lsl #21; lsr #21), as the table index of EffectSanganResolve. */
#define CARD_ID11(word)     (((word) << 21) >> 21)

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber
 * (0x08622AB4). Matching: these give the ROM's literal pools; the symbol forms generate other code.
 */
#define CARD_STATS(id)      (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */

/*
 * &gDuelZones[player].zones[zone] by integer arithmetic, zone term first. Matching: this is the ROM's
 * address order (array indexing emits the player term first). Callers mask the player with & 1.
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) \
                         + (u32)gDuelZones))

/*
 * Byte +2 of a struct ChainEntry: bit 0 player, bits 1-3 kind. Matching: Mask of Darkness and the equip
 * handlers at the end of the file test the fields of the whole byte with masks (and keep the byte in a
 * register), where link->player / link->kind give other code there.
 */
#define LINK_BYTE2(link)        (((u8 *)(link))[2])
#define BYTE2_PLAYER_MASK       1
#define BYTE2_KIND_MASK         0xE
#define BYTE2_KIND_OFF_FIELD    (CHAIN_KIND_OFF_FIELD << 1)     /* kind CHAIN_KIND_OFF_FIELD: left the field */

/*
 * The location player | zone << 8 (DUEL_LOC), written player first. Matching: this is the ROM's operand
 * order; DUEL_LOC (constants/duel.h) puts the zone first, which changes the register use.
 */
#define LOC_PLAYER_ZONE(player, zone) ((player) | ((zone) << 8))

/* Duel command id for player's side: DUEL_CMD_PLAYER (bit 15) marks a command of player 1. */
#define PLAYER_CMD(player, cmd) ((player) ? (DUEL_CMD_PLAYER | (cmd)) : (cmd))

/* Text box of the prompts here: x 5, y 2, 20 x 9 cells. */
#define PROMPT_POS  0x205
#define PROMPT_SIZE 0x914

/*
 * Key 1425: on the field, answering a summon (Normal, Flip or Special), with a monster on the field
 * (the Dark Hole condition). Its resolve destroys every monster.
 */
int EffectDestroyAllOnSummonPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand != 0)
        return 0;
    switch (card->event) {
    case RESPONSE_SUMMONED:
    case RESPONSE_FLIP_SUMMONED:
    case RESPONSE_SPECIAL_SUMMONED:
        return EffectDarkHolePrepare3(card, chainLink, 0);
    }
    return 0;
}

/*
 * Key 1428: on the field, answering an attack declaration (not another link) whose target (loc1) is one
 * of the player's monsters, with a face-up key 1405 that can take the attack instead.
 */
int EffectRedirectAttackPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand == 0 && chainLink == NULL && card->event == RESPONSE_ATTACK_DECLARED) {
        int player = DUEL_LOC_PLAYER(card->loc1);
        int zone = DUEL_LOC_ZONE(card->loc1);
        int p = player & 1;
        struct DuelZone *target = ZONE_AT(p, zone);

        if (CARD_ID(CARD_WORD(target->card)) != 0 && player == card->player
            && CountFaceUpMonstersByNumber(player, CARD_1405) > 0)
            return 1;
    }
    return 0;
}

/*
 * Keys 1430, 1433 and 1444: tributes are allowed (no key 1418 in effect on either side) and the player's
 * graveyard holds a monster destroyed in battle (CollectEffectTargets for key 1421).
 */
int EffectTributeRecoverGraveMonsterPrepare(struct ChainEntry *card)
{
    if (CountActiveCardsOnField(0, CARD_1418) > 0 || CountActiveCardsOnField(1, CARD_1418) > 0
        || CollectEffectTargetsInt(card->player, CARD_1421, 0) <= 0)
        return 0;
    return 1;
}

/* Keys 1432 and 1442: the player has at least two monsters to tribute. */
int EffectTributeTwoMonstersPrepare(struct ChainEntry *card)
{
    if (CountTributableMonsters(card->player, -1) > 1)
        return 1;
    return 0;
}

/*
 * Key 1439: tributes are allowed (no key 1418 on either side) and the player's graveyard holds a Magic card
 * destroyed by the opponent (CollectEffectTargets for key 1439).
 */
int EffectTributeRecoverGraveMagicPrepare(struct ChainEntry *card)
{
    if (CountActiveCardsOnField(0, CARD_1418) > 0 || CountActiveCardsOnField(1, CARD_1418) > 0
        || CollectEffectTargetsInt(card->player, CARD_1439, 0) <= 0)
        return 0;
    return 1;
}

/* Key 1436: at least two hand cards (the cost banishes two at random). */
int EffectHasTwoHandCardsPrepare(struct ChainEntry *card)
{
    if (gDuelPlayers[card->player].handCount <= 1)
        return 0;
    return 1;
}

/* Key 1510: on the field, with a hand card (the cost discards one at random). */
int EffectHasHandCardPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (fromHand != 0)
        return 0;
    if (gDuelPlayers[card->player].handCount != 0)
        return 1;
    return 0;
}

/* Key 1521: at least two cards in the spell/trap and field zones of both players together. */
int EffectTwoSpellTrapsOnFieldPrepare(void)
{
    int count = CountSpellTrapsFiltered(0, 0, 0, 1);

    count += CountSpellTrapsFiltered(1, 0, 0, 1);
    return count > 1;
}

/*
 * Key 1525: on the field, answering a Magic card (but not key 1539, which is never negated), with tributes
 * allowed (no key 1418 on either side). Its resolve tributes a monster to negate the Magic card.
 */
int EffectTributeNegateMagicPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    if (fromHand == 0 && chainLink != NULL && CARD_TYPE(chainLink->card) == CARD_TYPE_MAGIC
        && CountActiveCardsOnField(0, CARD_1418) <= 0 && CountActiveCardsOnField(1, CARD_1418) <= 0
        && CARD_NUMBER(chainLink->card) != CARD_1539)
        return 1;
    return 0;
}

/*
 * Key 1527: on the field, with an equip card that could move to another monster. Searches every face-up
 * monster (monsterPlayer, monsterZone) on both fields and every spell/trap position (equipPlayer,
 * equipZone): the equip card there must pass this card's check (EffectTailorOfTheFickleCheck), be a valid
 * equip for that monster, and not already be equipped to it.
 */
int EffectMoveEquipPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    int monsterPlayer, monsterZone, equipPlayer, equipZone;

    if (fromHand != 0)
        return 0;
    for (monsterPlayer = 0; monsterPlayer <= 1; monsterPlayer++) {
        int p;

        for (monsterZone = ZONE_MONSTER_0; monsterZone <= ZONE_MONSTER_4; monsterZone++) {
            struct DuelZone *monster;

            /* Matching: the mask is written inside the inner loop (agbcc hoists it either way, but this
             * placement gives the ROM's instruction order). */
            p = monsterPlayer & 1;
            monster = ZONE_AT(p, monsterZone);

            if (CARD_ID(CARD_WORD(monster->card)) != 0 && monster->isFaceUp) {
                for (equipPlayer = 0; equipPlayer <= 1; equipPlayer++) {
                    for (equipZone = ZONE_SPELL_0; equipZone <= ZONE_SPELL_4; equipZone++) {
                        /* Matching: a u8 flag gives the ROM's neg / orr / lsr #31 for "!= 0". */
                        u8 canMove = EffectTailorOfTheFickleCheck(card,
                                                                  LOC_PLAYER_ZONE((u8)equipPlayer, (u8)equipZone))
                                     != 0;

                        if (IsValidEquipTargetInt(equipPlayer, equipZone, monsterPlayer, monsterZone) == 0)
                            canMove = 0;
                        if (FindMonsterLinkedToCardInt(equipPlayer, equipZone)
                            == (u16)LOC_PLAYER_ZONE((u8)monsterPlayer, (u8)monsterZone))
                            canMove = 0;
                        if (canMove)
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}

/*
 * Key 1529: answering an attack declared by the opponent (the attacker, loc0, is not the player's), when
 * the opponent has at least two face-up monsters (one of them can attack instead).
 */
int EffectSwitchAttackerPrepare(struct ChainEntry *card)
{
    /* Matching: separate tests, not one ||-chain (the player bit is extracted again for 1 - player). */
    if (card->event != RESPONSE_ATTACK_DECLARED)
        return 0;
    if (DUEL_LOC_PLAYER(card->loc0) == card->player)    /* the attacker is the player's own */
        return 0;
    if (CountMonstersFiltered(1 - card->player, 1, 0) <= 1)
        return 0;
    return 1;
}

/*
 * Key 1532: on the field and not answering another link, no key 1511 on the opponent's field, and some
 * face-up monster on either field whose level is at most the number of monsters in the player's graveyard
 * (the cost banishes that many of them).
 */
int EffectBanishGraveToDestroyPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    /* FAKEMATCH: a 64-bit temp holding the gCardStats address stops agbcc from hoisting the table
     * address out of the loop; the ROM reloads it inside. */
    unsigned long long cardStats = 0x08621DE0;
    int graveMonsters;
    int player, zone;
    int level;

    if (fromHand != 0 || chainLink != NULL)
        return 0;
    if (CountActiveCardsOnField(1 - card->player, CARD_1511) > 0)
        return 0;
    graveMonsters = CountGraveyardMonsters(card->player);
    if (graveMonsters == 0)
        return 0;
    for (player = 0; player <= 1; player++) {
        for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
            struct DuelZone *monster = ZONE_AT(player & 1, zone);
            u16 id = CARD_ID(CARD_WORD(monster->card));

            if (monster->isFaceUp && id != 0) {
                u32 stats = ((const u32 *)(u32)cardStats)[id & CARD_ID_MASK];

                /* Level as the game counts it: Trap, Magic and Ticket cards 0, the Egyptian Gods 10. */
                switch ((int)CARD_STATS_TYPE(stats)) {
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
                if (level <= graveMonsters)
                    return 1;
            }
        }
    }
    return 0;
}

/*
 * Key 1535: on the field, after battle damage to the attacker's controller (RESPONSE_BATTLE_DEFLECTED_DAMAGE)
 * when the attacker is not the player's. loc1 holds two nibble-packed locations (player | zone << 4): the
 * low byte the attacker, the high byte the monster it attacked. Returns the attacked monster's isDefense
 * bit: the card answers an opponent's monster that attacked a defense-position monster and lost.
 */
int EffectRepelledAttackerPrepare(struct ChainEntry *card, int chainLink, u16 fromHand)
{
    if (fromHand == 0 && card->event == RESPONSE_BATTLE_DEFLECTED_DAMAGE) {
        /* Matching: card->loc1 is read inline (a local copy changes the register choice). */
        if ((card->loc1 & 0xF) != card->player) {
            u16 attacked = card->loc1 >> 8;
            int attackedPlayer = attacked & 0xF;
            int attackedZone = attacked >> 4;
            int p = attackedPlayer & 1;
            struct DuelZone *defender = ZONE_AT(p, attackedZone);

            return defender->isDefense;
        }
    }
    return 0;
}

/*
 * Key 1549: no face-up Banisher of the Light on either side (it would banish the returned cards again) and
 * more than 4 banished monsters of the player (CollectEffectTargets for key 1549).
 */
int EffectReturnBanishedToGravePrepare(struct ChainEntry *card)
{
    if (CountFaceUpMonstersByNumber(0, CARD_BANISHER_OF_THE_LIGHT) <= 0
        && CountFaceUpMonstersByNumber(1, CARD_BANISHER_OF_THE_LIGHT) <= 0
        && CollectEffectTargetsInt(card->player, CARD_1549, 0) > 4)
        return 1;
    return 0;
}

/*
 * Destroy the card in (player, zone) by a card effect (Dark Hole, Raigeki, Fissure, Trap Hole, Mirror
 * Force, ...). Sangan, Witch of the Black Forest and keys 1241/1257 first get the ZONE_STATUS_UNK14 status
 * (DUEL_CMD_SET_ZONE_STATUS_FLAGS, and card-word bit 14 set at once) so that their graveyard trigger runs.
 * An empty zone is left alone.
 */
void DestroyFieldCardByEffect(int player, int zone)
{
    int p = 1 & player;
    struct DuelZone *target;
    struct DuelZone *sameZone;
    u16 id;

    target = ZONE_AT(p, zone);
    /* FAKEMATCH: an empty asm input keeps the zone address in r0 without an index copy. */
    __asm__("" : : "r"(target));
    id = CARD_ID(CARD_WORD(target->card));

    if (id != 0) {
        switch (CARD_NUMBER(id)) {
        case CARD_SANGAN:
        case CARD_WITCH_OF_THE_BLACK_FOREST:
        case CARD_1241:
        case CARD_1257:
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_SET_ZONE_STATUS_FLAGS), zone, ZONE_STATUS_UNK14, 0);
            /* Also set card-word bit 14 directly, before DestroyFieldCard (the queued command runs later). */
            p = 1 & player;
            /* FAKEMATCH: an empty asm input keeps the recomputed player mask in r2. */
            __asm__("" : : "r"(p));
            sameZone = ZONE_AT(p, zone);
            ((struct DuelCardStatusBytes *)sameZone)->unk14 = 1;
        }
        DestroyFieldCard(player, zone, 1);
    }
}

/*
 * Dragon Piper (FLIP): unless negated, destroy every face-up Dragon Capture Jar on the field, then switch
 * every face-up defense-position Dragon to attack position.
 */
int EffectDragonPiperResolve(struct ChainEntry *link)
{
    int player, zone;

    if (link->negated)
        return EFFECT_STEP_DONE;
    for (player = 0; player <= 1; player++) {
        for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
            struct DuelZone *z = ZONE_AT(player & 1, zone);
            /* Matching: an int ID gives the signed "> 0" test (ble). */
            int id = CARD_ID(CARD_WORD(z->card));

            if (id > 0 && z->isFaceUp && CARD_NUMBER(id) == CARD_DRAGON_CAPTURE_JAR)
                DestroyFieldCard(player, zone, 1);
        }
    }
    for (player = 0; player <= 1; player++) {
        for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
            struct DuelZone *z = ZONE_AT(player & 1, zone);

            if (CARD_ID(CARD_WORD(z->card)) != 0 && z->isDefense && z->isFaceUp
                && GetZoneCardType(player, zone) == CARD_TYPE_DRAGON)
                ChangeBattlePosition(player, zone, 0, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Sangan and Witch of the Black Forest: add a monster from the deck to the hand (the candidates come from
 * CollectEffectTargets for the card's number: ATK <= 1500 for Sangan, DEF <= 1500 for the Witch).
 * Player 1 (the CPU) picks with AiPickCardListEntry; the human picks in the card-list viewer.
 */
int EffectSanganResolve(struct ChainEntry *link)
{
    if (link->negated)
        return EFFECT_STEP_DONE;
    switch (gChain.effectStep) {
    case EFFECT_STEP_START:
        if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) == 0) {
            /* No candidate: the human is told so, the CPU is done at once. */
            if (link->player)
                return EFFECT_STEP_DONE;
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrNoDeckCardsToAdd);
            return EFFECT_STEP_END;
        }
        if (link->player) {
            /* CPU: point the viewer at its pick and take it at once. */
            AiPickCardListEntry(link->card);
            gCardListView.cursorRow = 0;
            gCardListView.top = gAiWork.listPick;
            return EFFECT_STEP_3;
        }
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectDeckMonsterToAdd);
        return EFFECT_STEP_2;
    case EFFECT_STEP_2:
        /* Human: list the candidates (area -1 = effect targets of this card number). */
        CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
        return EFFECT_STEP_3;
    case EFFECT_STEP_3:
        /* Show the picked card, add it to the hand and shuffle the deck. */
        ShowPickedCard(link->player,
                     CARD_ID(gCardListView.cards[gCardListView.top + gCardListView.cursorRow]));
        if (AddDeckCardToHand(link->player,
                              ((const u16 *)0x08622AB4)[CARD_ID11(gCardListView.cards[gCardListView.top
                                                                                   + gCardListView.cursorRow])])
            != 0)
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
        return EFFECT_STEP_END;
    }
    return EFFECT_STEP_DONE;
}

/*
 * Castle of Dark Illusions: unless negated, mark its one-shot effect used (DUEL_CMD_SET_EFFECT_UNUSED with
 * 0) and give every face-up Zombie on the field a ZONE_LINK_CONTINUOUS link to the Castle (its ATK/DEF
 * boost).
 */
int EffectCastleOfDarkIllusionsResolve(struct ChainEntry *link)
{
    int player, zone;

    if (link->negated)
        return EFFECT_STEP_DONE;
    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
    for (player = 0; player <= 1; player++) {
        for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
            struct DuelZone *z = ZONE_AT(player & 1, zone);

            if (CARD_ID(CARD_WORD(z->card)) != 0 && z->isFaceUp
                && GetZoneCardType(player, zone) == CARD_TYPE_ZOMBIE)
                QueueAddZoneLink(link->player, LOC_PLAYER_ZONE(link->player, link->zone),
                                 LOC_PLAYER_ZONE((u8)player, (u8)zone), ZONE_LINK_CONTINUOUS);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Reaper of the Cards and Trap Master: unless negated, destroy the target (targets[0]) if it is a Trap.
 * A face-down target is flipped up and shown first, and flipped back down when it is not a Trap.
 */
int EffectReaperOfTheCardsResolve(struct ChainEntry *link)
{
    if (link->negated)
        return EFFECT_STEP_DONE;
    if (link->numTargets != 1)
        return EFFECT_STEP_DONE;
    {
        int player = DUEL_LOC_PLAYER(link->targets[0]);
        int zone = DUEL_LOC_ZONE(link->targets[0]);
        int p = 1 & player;
        struct DuelZone *target = ZONE_AT(p, zone);
        u16 id = CARD_ID(CARD_WORD(target->card));

        if (id == 0)
            return EFFECT_STEP_DONE;
        if (target->isFaceUp) {
            if (CARD_TYPE(id) == CARD_TYPE_TRAP)
                DestroyFieldCard(player, zone, 1);
            /* Matching: this return stops old_agbcc from cross-jumping the two DestroyFieldCard calls. */
            return EFFECT_STEP_DONE;
        }
        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_FLIP_CARD), zone, 0, 0);
        ShowPickedCard(player, id);
        if (CARD_TYPE(id) == CARD_TYPE_TRAP)
            DestroyFieldCard(player, zone, 1);
        else
            DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_FLIP_CARD), zone, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Catapult Turtle and Cannon Soldier: tribute the target (targets[0]); if that succeeds the opponent loses
 * half its ATK, rounded up (Catapult Turtle), or 500 (Cannon Soldier). No negation check.
 */
int EffectCatapultTurtleResolve(struct ChainEntry *link)
{
    if (link->numTargets != 1)
        return EFFECT_STEP_DONE;
    {
        int player = DUEL_LOC_PLAYER(link->targets[0]);
        int zone = DUEL_LOC_ZONE(link->targets[0]);
        int p = 1 & player;
        struct DuelZone *target = ZONE_AT(p, zone);
        int damage;

        if (CARD_ID(CARD_WORD(target->card)) == 0)
            return EFFECT_STEP_DONE;
        damage = 0;
        switch (CARD_NUMBER(link->card)) {
        case CARD_CATAPULT_TURTLE:
            damage = HalveRoundUp(GetZoneCardAtk(player, zone));
            break;
        case CARD_CANNON_SOLDIER:
            damage = 500;
            break;
        }
        if (TributeMonster(player, zone) != 0)
            LoseLifePoints(1 - link->player, damage);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Mask of Darkness and Magician of Faith (FLIP): unless negated, return the chosen graveyard card to the
 * hand. targets[0] and targets[1] hold the picked card word; its ID (bits 0-11) must still be in the
 * graveyard.
 */
int EffectMaskOfDarknessResolve(struct ChainEntry *link)
{
    u32 id;

    if (link->negated)
        return EFFECT_STEP_DONE;
    if (link->numTargets != 2)
        return EFFECT_STEP_DONE;
    /* Matching: the assignment inside the argument fixes the evaluation order; u32 shifts give lsr. */
    if (CountGraveyardCardsByNumber(link->player, CARD_NUMBER(id = (u32)(link->targets[0] << 20) >> 20)) > 0) {
        ShowPickedCard(link->player, id);
        {
            /* Matching: the player bit as the raw byte & 1 (the bitfield read gives other code here). */
            int player = LINK_BYTE2(link) & BYTE2_PLAYER_MASK;
            /* FAKEMATCH: the command id is pinned to r3 (the ROM picks it there, then copies it to r0). */
            register int cmd __asm__("r3") = DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND;

            if (player)
                cmd = DUEL_CMD_PLAYER | DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND;
            {
                u16 cardWordLow = link->targets[0];
                int zero = 0;

                /* FAKEMATCH: an empty asm with r0 clobbered sets up r1/r2 before the id is copied to r0. */
                __asm__("" : : "r"(cardWordLow), "r"(zero) : "r0");
                DuelCmd_PushInt(cmd, cardWordLow, zero, 0);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Tainted Wisdom: unless negated, shuffle the player's deck. */
int EffectTaintedWisdomResolve(struct ChainEntry *link)
{
    if (link->negated)
        return EFFECT_STEP_DONE;
    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
    return EFFECT_STEP_DONE;
}

/*
 * Big Eye (FLIP): look at the top 5 cards of the deck and put them back in any order. Needs more than 4
 * deck cards; no negation check.
 *   EFFECT_STEP_START  copy deck[0..4] to gChain.scratch.deckReorder.cards, reset the reorder state and
 *                      fall through to the next step at once
 *   EFFECT_STEP_2      run the reorder prompt (DeckReorder_Run) until it is confirmed
 *   EFFECT_STEP_3      write the 5 cards back; in a link duel send the new deck to the partner
 *   EFFECT_STEP_4      (link duel) wait until the partner acknowledges it (gLinkState.deckAcked)
 */
int EffectBigEyeResolve(struct ChainEntry *link)
{
    int i;

    switch (gChain.effectStep) {
    case EFFECT_STEP_START:
        if (gDuelPlayers[link->player].deckCount <= 4)
            return EFFECT_STEP_DONE;
        for (i = 0; i <= 4; i++)
            CopyDuelCard((u32 *)&gChain.scratch.deckReorder.cards[i],
                         (u32 *)&gDuelPlayers[link->player].deck[i]);
        gChain.scratch.deckReorder.mode = DECK_REORDER_INIT;
        gChain.scratch.deckReorder.phase = 0;
        gChain.scratch.deckReorder.timer = 0;
        gChain.scratch.deckReorder.unk36 = 0;
        gChain.effectStep--;                    /* EFFECT_STEP_2; fall through */
    case EFFECT_STEP_2:
        if (DeckReorder_Run(link->player) != 0)
            return EFFECT_STEP_3;
        return EFFECT_STEP_2;
    case EFFECT_STEP_3:
        for (i = 0; i <= 4; i++)
            CopyDuelCard((u32 *)&gDuelPlayers[link->player & 1].deck[i],
                         (u32 *)&gChain.scratch.deckReorder.cards[i]);
        if (gDuelCtrl.isLinkDuel) {
            DuelLink_SendDeck(link->player);
        waitForPartner:
            return EFFECT_STEP_4;
        }
        return EFFECT_STEP_DONE;
    case EFFECT_STEP_4:
        if (!gLinkState.deckAcked)
            goto waitForPartner;
        return EFFECT_STEP_DONE;
    }
    return EFFECT_STEP_DONE;
}

/* Penguin Knight: return the player's graveyard to the deck and shuffle it. No negation check. */
int EffectPenguinKnightResolve(struct ChainEntry *link)
{
    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_RETURN_GRAVEYARD_TO_DECK), 1, 0, 0);
    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
    return EFFECT_STEP_DONE;
}

/* Dimensional Warrior: banish both battling monsters (the event's loc0 and loc1). No negation check. */
int EffectDimensionalWarriorResolve(struct ChainEntry *link)
{
    /* Matching: (u8) here; DUEL_LOC_PLAYER's & 0xFF gives other code in this function. */
    int player0 = (u8)link->loc0;
    int zone0 = DUEL_LOC_ZONE(link->loc0);
    int player1 = (u8)link->loc1;
    int zone1 = DUEL_LOC_ZONE(link->loc1);

    BanishFieldCard(player0, zone0, 0);
    BanishFieldCard(player1, zone1, 0);
    return EFFECT_STEP_DONE;
}

/*
 * The Little Swordsman of Aile: tribute the target (targets[0]); if that succeeds, link the card's own
 * ZONE_LINK_CARD_EFFECT (+700 ATK this turn) to its zone. No negation check.
 */
int EffectTheLittleSwordsmanOfAileResolve(struct ChainEntry *link)
{
    if (link->numTargets != 1)
        return EFFECT_STEP_DONE;
    {
        int player = DUEL_LOC_PLAYER(link->targets[0]);
        int zone = DUEL_LOC_ZONE(link->targets[0]);
        int p = 1 & player;
        struct DuelZone *target = ZONE_AT(p, zone);

        if (CARD_ID(CARD_WORD(target->card)) == 0)
            return EFFECT_STEP_DONE;
        if (TributeMonster(player, zone) != 0)
            QueueAddZoneLink(link->player, link->card, LOC_PLAYER_ZONE(link->player, link->zone),
                             ZONE_LINK_CARD_EFFECT);
    }
    return EFFECT_STEP_DONE;
}

/* Princess of Tsurugi: unless negated, the opponent loses 500 LP per card in its spell/trap zones. */
int EffectPrincessOfTsurugiResolve(struct ChainEntry *link)
{
    int count = CountSpellTrapsFiltered(1 - link->player, 0, 0, 0);

    if (!link->negated && count > 0)
        LoseLifePoints(1 - link->player, count * 500);
    return EFFECT_STEP_DONE;
}

/*
 * The resolve of every equip card (45 rows): while the card is still on the field, equip it to targets[0].
 * 7 Completed has a second target, the chosen stat (enum SevenCompletedStat), stored as the zone's
 * declaredValue. Always returns 0.
 */
u16 EffectEquipResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    int p = 1 & link->player;
    struct DuelZone *equip = ZONE_AT(p, link->zone);

    if (CARD_ID(CARD_WORD(equip->card)) != 0) {
        /* Matching: numTargets read as the raw byte & 7 (the bitfield read gives other code here). */
        int numTargets = ((u8 *)link)[0xA] & 7;

        if (numTargets == 1) {
            EquipCard(link->player, LOC_PLAYER_ZONE(link->player, link->zone), link->targets[0]);
        } else if (CARD_NUMBER(link->card) == CARD_7_COMPLETED && numTargets == 2) {
            EquipCard(link->player, LOC_PLAYER_ZONE(link->player, link->zone), link->targets[0]);
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_ZONE_DECLARED_VALUE), link->zone,
                         link->targets[1], 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Axe of Despair. On the field: EffectEquipResolve. Sent to the graveyard (kind CHAIN_KIND_OFF_FIELD): the
 * human may tribute a monster to put it back on top of the deck.
 *   EFFECT_STEP_START  CPU: done; no monster to tribute: end; else ask Yes/No
 *   EFFECT_STEP_2      No: end; Yes: reset the target-selection step (gChain.targetStep)
 *   EFFECT_STEP_3      pick the monster to tribute (EffectTributeTargetChainB) until done
 *   EFFECT_STEP_4      tribute it; on success return the Axe from the graveyard to the top of the deck
 */
u16 EffectAxeOfDespairResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    char text[0x100];

    if (link->kind != CHAIN_KIND_OFF_FIELD)
        return EffectEquipResolve(link, chainedTo);
    switch (gChain.effectStep) {
    case EFFECT_STEP_START:
        if (link->player)
            return EFFECT_STEP_DONE;
        if (CountTributableMonsters(link->player, -1) == 0)
            return EFFECT_STEP_END;
        /* gCardNumberToId_AxeOfDespair = gCardNumberToId[CARD_AXE_OF_DESPAIR]: the name is the Axe's own. */
        FormatStr(text, gStrTributeToReturnToDeckPrompt,
                  (const char *)gCardNames + gCardNumberToId_AxeOfDespair * CARD_NAME_SIZE);
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        return EFFECT_STEP_2;
    case EFFECT_STEP_2:
        if (gTextBox.result == 0)
            return EFFECT_STEP_END;
    {
        int offset = OFFSET_OF(struct ChainState, targetStep);

        ((u8 *)&gChain)[offset] = 0;        /* gChain.targetStep */
        /* FAKEMATCH: an empty asm input keeps the offset alive through the store, so that the zero
         * goes in r0 and the offset in r2, as in the ROM. */
        __asm__("" : : "r"(offset));
    }
    pickTribute:
        return EFFECT_STEP_3;
    case EFFECT_STEP_3:
        if (EffectTributeTargetChainB(link) == 0)
            goto pickTribute;
        return EFFECT_STEP_4;
    case EFFECT_STEP_4:
    {
        int player = DUEL_LOC_PLAYER(link->targets[0]);
        u16 target = link->targets[0];
        int zone;

        zone = target >> 8;
        /* FAKEMATCH: an empty asm input with r2 clobbered keeps the target in r3 and leaves r1 for the
         * later read of byte +2, as in the ROM. */
        __asm__("" : : "r"(target) : "r2");

        if (TributeMonster(player, zone) != 0)
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_TOP), link->card,
                         0, 0);
        return EFFECT_STEP_END;
    }
    }
    return EFFECT_STEP_DONE;
}

/* Black Pendant. On the field: EffectEquipResolve. Sent to the graveyard: unless negated, the opponent
 * loses 500 LP. */
u16 EffectBlackPendantResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    if (link->kind != CHAIN_KIND_OFF_FIELD)
        return EffectEquipResolve(link, chainedTo);
    if (!link->negated)
        LoseLifePoints(1 - link->player, 500);
    return EFFECT_STEP_DONE;
}

/*
 * Horn of Light and Malevolent Nuzzler. On the field: EffectEquipResolve. Sent to the graveyard: the
 * player may pay 500 LP to put the card back on top of the deck (the CPU is asked too).
 *   EFFECT_STEP_START  less than 500 LP: done; else ask Yes/No
 *   EFFECT_STEP_2      Yes: pay 500 LP and return the card from the graveyard to the top of the deck; end
 */
u16 EffectHornOfLightResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    char text[0x100];

    if (link->kind != CHAIN_KIND_OFF_FIELD)
        return EffectEquipResolve(link, chainedTo);
    switch (gChain.effectStep) {
    case EFFECT_STEP_START:
        if (gDuelPlayers[link->player].lifePoints < 500)
            return EFFECT_STEP_DONE;
        FormatStr(text, gStrPayLpToReturnToDeckPrompt, (const char *)gCardNames + link->card * CARD_NAME_SIZE);
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
        return EFFECT_STEP_2;
    case EFFECT_STEP_2:
        if (gTextBox.result != 0) {
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_LOSE_LP), 500, 1, 0);
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_TOP), link->card,
                         0, 0);
        }
        return EFFECT_STEP_END;
    }
    return EFFECT_STEP_DONE;
}

/* Horn of the Unicorn. On the field: EffectEquipResolve. Sent to the graveyard: it goes back on top of the
 * deck, without a prompt. */
u16 EffectHornOfTheUnicornResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    if (link->kind != CHAIN_KIND_OFF_FIELD)
        return EffectEquipResolve(link, chainedTo);
    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_TOP), link->card, 0, 0);
    return EFFECT_STEP_DONE;
}
