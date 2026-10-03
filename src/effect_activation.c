/*
 * effect_activation (0x0802CAE8-0x0802DB2F): may a card's effect be activated now?
 * (wiki/functions/effect-activation-c.md)
 *
 * CanActivateEffect is the rule every activation goes through, for the human menus, the CPU and the chain:
 * the card must not be under Prohibition, must be fast enough to answer the chain link it would respond to
 * (enum SpellSpeed, GetCardSpellSpeed), must be one of the damage-step cards during the damage step, and must
 * pass its gCardEffects row: the prepare handler (activation condition) and, if the row has one, the check
 * handler (target filter) for at least one of the 22 board positions. Around it are the wrappers for a card
 * ID and for a field zone, the chain-response scans (a set Magic/Trap in zones 5-9, a Quick-Play Magic in the
 * hand), sixteen prepare handlers (Time Wizard ... Relinquished) and six chainA/chainB activation-cost steps
 * of effect keys 1220-1510, which have no EDS card.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_* extractors, gCardNames */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardKind, CardAttribute, SpellSubtype */
#include "constants/duel.h"         /* enum DuelZoneIndex, ResponseEventKind, FieldPickMask, DUEL_LOC */
#include "constants/sound.h"        /* SE_ERROR */

/* ---- BEGIN pre-H0 subset of duel.h and sound.h ---- */
/*
 * The part of those headers this unit uses, with their names, types and bitfield containers (unused bytes
 * are padding). include/duel.h and sound.h still hold the legacy headers until the header switch (H0,
 * build/readability/HEADERS.md), and chain.h, duel_screen.h and summon.h include duel.h, so this block
 * also defines duel.h's include guard. After H0, replace this block (BEGIN to END) with
 *     #include "duel.h"
 *     #include "sound.h"
 * Checked: with the block replaced, the unit compiles to the same assembly against the staged headers
 * (the shadow include/ of build/readability/integrate/check.sh).
 */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
    u32 unk13:19;
};

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
    u16 destroyCountdown:4;         /* +0x06 bits 6-9 */
    u8 positionLocked:1;            /* +0x07 bit 2 */
    u8 unk7_3:1;
    u8 unk7_4:1;
    u8 effectUnused:1;              /* +0x07 bit 5: one-shot effect not used yet */
    u8 revivedByMonsterReborn:1;    /* +0x07 bit 6 */
    u8 summonedFromGraveyard:1;     /* +0x07 bit 7 */
    u8 unk8[0x8C - 0x8];
    u8 unk8C[4];                    /* +0x8C: battle flags */
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
    u8 deckCount;                   /* +0x003 */
    u8 graveCount;                  /* +0x004 */
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 banishedCount;               /* +0x006 */
    u8 unk7;                        /* +0x007: player flags */
    u8 unk8_0:4;                    /* +0x008 */
    u8 normalSummonUsed:1;          /* +0x008 bit 4: the Normal Summon of this turn is done */
    u8 unk8_5:3;
    u8 unk9_0:4;                    /* +0x009 */
    u8 battlePhaseDone:1;           /* +0x009 bit 4: set when the Battle Phase ends */
    u8 unk9_5:3;
    u8 unkA[0x684 - 0xA];
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];
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
    u8 result:2;                    /* +0x1B12 bits 6-7: enum DuelResult */
    u8 unk1B13[0x1B78 - 0x1B13];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];     /* the rest of the player stride */
};

struct ZoneCardStats {
    u16 id;                         /* +0x0: card ID */
    u8 type:5;                      /* +0x2 bits 0-4: effective enum CardType */
    u8 attribute:3;                 /* +0x2 bits 5-7: effective enum CardAttribute */
    u8 unk3;
    s32 atk;                        /* +0x4: effective ATK */
    s32 def;                        /* +0x8: effective DEF */
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */

u32 HasFlipEffect(u16 cardNo, int inBattle);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountMonstersFiltered(int player, u16 faceUpOnly, u16 attackPosOnly);
int CountSpellTrapsFiltered(int player, u16 faceUp, u16 faceDown, u16 includeField);
u32 IsCardProhibited(u16 cardId);
int CountFreeMonsterZones(int player);
int FindFreeSpellTrapZone(int player);
int CanPlaceSpellTrapCard(int player, u16 cardId);
u16 FindAbsorbedMonsterLink(int player, int zone);
void GetZoneCardStats(int player, int zone, struct ZoneCardStats *out);

void PlaySE(u32 seId);
/* ---- END pre-H0 subset ---- */

#include "util.h"                   /* MemCopy16, FormatStr */
#include "text_box.h"               /* TextBoxOpen, TEXTBOX_FLAGS_DEFAULT */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* TributeMonster, DiscardHandCard */
#include "duel_prompt.h"            /* DuelPrompt_PostDiscardCost, PostRandomBanish, PostRandomDiscard */
#include "duel_screen.h"            /* gDuelScreen.selPlayer / selIndex, DuelCursor_PickTarget */
#include "summon.h"                 /* CanSpecialSummon */
#include "effect.h"                 /* gCardEffects, FindCardEffect, enum SpellSpeed; the rules defined here */
#include "effect_handlers.h"        /* the handlers defined here, EffectBlastJugglerCheck */

/* ---- Local views kept for matching ---- */

/* Matching: the ROM compares these results as int (cmp #0; ble, and cmp with 0xFFFF, with no narrowing);
 * the definitions return u16, which adds lsl #16; lsr #16 at the call. */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
extern int FindAbsorbedMonsterLinkInt(int player, int zone) asm("FindAbsorbedMonsterLink");

/* 0x08623E66 = &gCardNumberToId[CARD_KURIBOH] (card_data.h). Matching: the ROM loads this element address
 * from its own literal. */
extern const u16 gUnk_08623E66[];

/* Prompts used only by this unit. */
extern const char gStrSelectTributeFmt[];       /* 0x080827EC "Please select @3%s@0 as @2Tribute@0" */
extern const u8 gStrSelectMagicToDiscard[];     /* 0x08082810 "Please select a Magic Card to be discarded
                                                 * from your hand." */

/*
 * Card tables read through integer-constant addresses: gCardIdToNumber (0x08622AB4) and gCardStats
 * (0x08621DE0). Matching: the symbol forms (card_data.h) load other literals.
 */
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[CARD_ID_MASK & (id)])   /* gCardIdToNumber[id] */
#define CARD_STATS(id)  (((const u32 *)0x08621DE0)[CARD_ID_MASK & (id)])   /* gCardStats[id] */
#define CARD_TYPE(id)   CARD_STATS_TYPE(CARD_STATS(id))                     /* enum CardType */

/*
 * The card of a zone or a hand slot, read through a struct DuelCard pointer. Matching: this loads the whole
 * card word (ldr; lsl #20; lsr #20 for the ID), as the ROM does; a direct zone->card.id access compiles to
 * other loads.
 */
#define ZONE_CARD(z) ((struct DuelCard *)(z))

/*
 * Zone pointers by byte arithmetic on gDuelZones; p is already masked to 0/1. Matching: the two forms add
 * the zone and player terms in the orders the ROM uses (&gDuelZones[p].zones[z] adds the base first).
 */
#define ZONE_ZP(p, z) /* zone term first */ \
    ((struct DuelZone *)((z) * sizeof(struct DuelZone) + (p) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
#define ZONE_PZ(p, z) /* player term first */ \
    ((struct DuelZone *)((p) * sizeof(struct DuelPlayer) + (z) * sizeof(struct DuelZone) + (u32)gDuelZones))

/* gDuelPlayers[p].hand[i] (p masked) through the gDuelHands alias, index term first. Matching: as above. */
#define HAND_CARD(p, i) \
    ((struct DuelCard *)((u32)gDuelHands + (i) * sizeof(struct DuelCard) + (p) * sizeof(struct DuelPlayer)))

/* The zone byte at +0x91 read whole for its canActivate bit (bit 2). Matching: the header's canActivate is
 * a bit of a u32 container, whose read loads a word. */
#define ZONE_BYTE91(z)  (((u8 *)(z))[0x91])
#define ZONE_BYTE91_CAN_ACTIVATE 0x04

/* Position word of the check handlers: player | zone << 8, both narrowed to u8 as the ROM does. */
#define CHECK_POS(player, zone) ((u8)(player) | ((u8)(zone) << 8))

/* ---- ChainA / ChainB activation-cost steps (effect keys without an EDS card) ---- */

/*
 * ChainA of keys 1220 and 1248: the activation cost is this turn's Battle Phase. Sets battlePhaseDone for
 * the link's player, so that player cannot enter a Battle Phase this turn. Returns 1 (done).
 */
int EffectSkipBattlePhaseChainA(struct ChainEntry *link)
{
    gDuelPlayers[link->player].battlePhaseDone = TRUE;
    return 1;
}

/*
 * ChainA of key 1232: tribute a face-up Kuriboh as the cost. Step 0 (gChain.costStep) opens the prompt;
 * step 1 waits for a cursor pick of a face-up monster and tributes it if it is Kuriboh, else plays the
 * error sound. Returns 1 once Kuriboh was tributed, else 0 (call again next frame).
 */
int EffectTributeKuribohChainA(struct ChainEntry *link)
{
    char buf[256];
    /* Matching (also below): &gChain.costStep as gChain's address plus the offset in a register; the
     * plain &gChain.costStep loads the address from one literal. */
    u8 *base = (u8 *)&gChain;
    u8 *step = base + OFFSET_OF(struct ChainState, costStep);

    switch (*step) {
    case 0: {
        const char *fmt = gStrSelectTributeFmt;

        /* ROM bug: the prompt is formatted with Kuriboh's name into buf, but the box shows fmt with its
         * raw %s. */
        FormatStr(buf, fmt, (const char *)gCardNames + gUnk_08623E66[0] * CARD_NAME_SIZE);  /* Kuriboh's name */
        TextBoxOpen(0x206, 0x712, TEXTBOX_FLAGS_DEFAULT, (const u8 *)fmt);   /* at cell (6, 2), 18 x 7 cells */
        (*step)++;
        return 0;
    }
    case 1:
        if (DuelCursor_PickTarget(PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION)) {
            int player = gDuelScreen.selPlayer;
            int zone = gDuelScreen.selIndex;
            int p = player & 1;
            struct DuelZone *z = ZONE_ZP(p, zone);
            int cardId = ZONE_CARD(z)->id;

            if (cardId && z->isFaceUp && CARD_NUMBER(cardId) == CARD_KURIBOH) {
                TributeMonster(player, zone);
                return 1;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    }
    return 0;
}

/*
 * ChainA of key 1324: discard a Magic card from the hand as the cost. Step 0 opens the prompt; step 1 waits
 * for a hand pick and discards it if it is a Magic card, else plays the error sound. Returns 1 once a card
 * was discarded, else 0.
 */
int EffectDiscardMagicCardChainA(struct ChainEntry *link)
{
    u8 *base = (u8 *)&gChain;
    u8 *step = base + OFFSET_OF(struct ChainState, costStep);

    switch (*step) {
    case 0:
        TextBoxOpen(0x206, 0x712, TEXTBOX_FLAGS_DEFAULT, gStrSelectMagicToDiscard);
        (*step)++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(PICK_HAND)) {
            int player = gDuelScreen.selPlayer;
            int handIdx = gDuelScreen.selIndex;
            int p = player & 1;

            if (CARD_TYPE(HAND_CARD(p, handIdx)->id) == CARD_TYPE_MAGIC) {
                DiscardHandCard(player, handIdx, FALSE, TRUE);
                return 1;
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    }
    return 0;
}

/* ChainB of key 1421: post the cost prompt in which the link's player discards 1 monster card from the hand
 * (PROMPT_DISCARD_COST, monsters only). Returns 1 at once. */
int EffectDiscardHandCardChainB(struct ChainEntry *link)
{
    DuelPrompt_PostDiscardCost(link->player, 1, TRUE, FALSE);
    return 1;
}

/* ChainA of key 1436: on step 0 post the prompt that banishes 2 random hand cards and return 0; then 1. */
int EffectBanishRandomHandCardsChainA(struct ChainEntry *link)
{
    u8 *base = (u8 *)&gChain;
    u8 *step = base + OFFSET_OF(struct ChainState, costStep);

    if (*step == 0) {
        DuelPrompt_PostRandomBanish(link->player, 2);
        (*step)++;
        return 0;
    }
    return 1;
}

/* ChainA of key 1510: on step 0 post the prompt that discards 1 random hand card (by the player's own
 * effect) and return 0; then 1. */
int EffectDiscardRandomHandCardChainA(struct ChainEntry *link)
{
    u8 *base = (u8 *)&gChain;
    u8 *step = base + OFFSET_OF(struct ChainState, costStep);

    if (*step == 0) {
        DuelPrompt_PostRandomDiscard(link->player, FALSE, 1);
        (*step)++;
        return 0;
    }
    return 1;
}

/* ---- Activation rules ---- */

/*
 * enum CardKind of a card: Obelisk counts as a Ritual monster, Slifer and Ra as Effect monsters; Magic, Trap
 * and Ticket cards have their own kinds; other monsters use the kind bits of their stats. The same inline is
 * repeated in card_detail.c and other units. Matching: here it needs the single return at the end; the
 * return-per-case form compiles differently.
 */
static inline int GetCardKind(u16 cardId)
{
    int kind;

    switch (CARD_NUMBER(cardId)) {
    case CARD_OBELISK_THE_TORMENTOR:
        kind = CARD_KIND_RITUAL;
        break;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        kind = CARD_KIND_EFFECT;
        break;
    default:
        switch ((int)CARD_TYPE(cardId)) {
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
            kind = CARD_STATS_KIND(CARD_STATS(cardId));
            break;
        }
        break;
    }
    return kind;
}

/* enum SpellSubtype of a Magic or Trap card from its gCardStats word; SPELL_NORMAL for other cards. */
static inline int GetSpellSubtype(u32 stats)
{
    switch ((int)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(stats);
    default:
        return SPELL_NORMAL;
    }
}

/*
 * Spell speed of a card (enum SpellSpeed): Counter Trap 3, other Traps 2; Quick-Play Magic 2, other Magic 1;
 * an Effect monster 2 if it has a flip effect, else 1; any other card 0.
 */
int GetCardSpellSpeed(u16 cardId)
{
    u16 number = CARD_NUMBER(cardId);
    int type = CARD_TYPE(cardId);
    int kind = GetCardKind(cardId);
    int subtype = GetSpellSubtype(CARD_STATS(cardId));

    switch (type) {
    case CARD_TYPE_TRAP:
        if (subtype == SPELL_COUNTER)
            return SPELL_SPEED_3;
        return SPELL_SPEED_2;
    case CARD_TYPE_MAGIC:
        if (subtype == SPELL_QUICK_PLAY)
            return SPELL_SPEED_2;
        return SPELL_SPEED_1;
    default:
        if (kind == CARD_KIND_EFFECT) {
            if (HasFlipEffect(number, FALSE))
                return SPELL_SPEED_2;
            return SPELL_SPEED_1;
        }
        return SPELL_SPEED_NONE;
    }
}

/*
 * 1 if the effect of `card` may be activated now, in answer to `chainLink` (NULL: not answering a link);
 * fromHand is 1 when the card is played from the hand. The rules, in order:
 *   - the card is not under Prohibition;
 *   - when answering a link: its spell speed is at least the link's, and not 1 (speed 1 cannot answer);
 *   - in the damage step (card->event == RESPONSE_DAMAGE_STEP) only the cards listed below may activate;
 *   - the card has a gCardEffects row; its prepare handler accepts, and if it has a check handler, that
 *     accepts at least one position (player 0-1, zone 0-10). A row with neither handler always passes.
 * The result is a full-word 0 or 1 (the CPU callers use it as int).
 */
int CanActivateEffect(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    int idx = FindCardEffect(card->card);
    u16 (*check)(struct ChainEntry *, u16);
    /* Matching: the ROM tests only the low halfword of prepare's result (lsl #16), as for a u16 return;
     * struct CardEffect declares the slot int-returning, so this local pointer has the u16 type. */
    u16 (*prepare)(struct ChainEntry *, struct ChainEntry *, u16);
    int player;
    int zone;

    if (IsCardProhibited(card->card))
        return FALSE;
    if (chainLink) {
        if (GetCardSpellSpeed(card->card) < GetCardSpellSpeed(chainLink->card))
            return FALSE;
        if (GetCardSpellSpeed(card->card) == SPELL_SPEED_1)
            return FALSE;
    }
    if (card->event == RESPONSE_DAMAGE_STEP) {
        switch (CARD_NUMBER(card->card)) {
        case CARD_KUNAI_WITH_CHAIN:
        case CARD_METALMORPH:
        case CARD_REVERSE_TRAP:
        case CARD_REINFORCEMENTS:
        case CARD_CASTLE_WALLS:
        case CARD_RUSH_RECKLESSLY:
        case CARD_THE_RELIABLE_GUARDIAN:
        case CARD_SNAKE_FANG:
        case CARD_MIRROR_WALL:
        case CARD_AQUA_CHORUS:
        case CARD_GRACEFUL_DICE:
        case CARD_SKULL_DICE:
        case CARD_1314:
        case CARD_1415:
        case CARD_1534:
            break;
        default:
            return FALSE;
        }
    }
    if (idx < 0)
        return FALSE;
    check = gCardEffects[idx].check;
    prepare = (u16 (*)(struct ChainEntry *, struct ChainEntry *, u16))gCardEffects[idx].prepare;
    if (check == NULL && prepare == NULL)
        return TRUE;
    if (prepare != NULL && !prepare(card, chainLink, fromHand))
        return FALSE;
    if (check == NULL)
        return TRUE;
    for (player = 0; player <= 1; player++) {
        for (zone = 0; zone < DUEL_ZONE_COUNT; zone++) {
            if (check(card, CHECK_POS(player, zone)))
                return TRUE;
        }
    }
    return FALSE;
}

/*
 * CanActivateEffect(&entry, NULL, fromHand) for card ID cardId of player, with no link to answer. Only the
 * card and player of the temporary entry are set; its zone and event are stack garbage. Returns the result
 * narrowed to u16 (the callers declare int).
 */
int CanActivateEffectOfCard(int player, u16 cardId, u16 fromHand)
{
    struct ChainEntry entry;

    entry.player = player;
    entry.card = cardId;
    return (u16)CanActivateEffect(&entry, NULL, fromHand);
}

/* CanActivateEffect for the card in (player, zone) answering `event` (enum ResponseEventKind), with no link;
 * 0 for an empty zone. */
u16 CanActivateEffectInZone(int player, int zone, u16 event)
{
    struct ChainEntry entry;

    entry.player = player;
    entry.card = ZONE_CARD(ZONE_PZ(player & 1, zone))->id;
    entry.zone = zone;
    entry.event = event;
    if (entry.card == 0)
        return FALSE;
    return CanActivateEffect(&entry, NULL, 0);
}

/* 1 if the chain (gChain.links) or the pending list (gChain.pending) already holds an entry from
 * (player, zone): that card is activated or queued and cannot respond again. */
u16 IsZoneInActionLists(int player, int zone)
{
    int i;

    for (i = 0; i < gChain.linkCount; i++) {
        if (gChain.links[i].player == player && gChain.links[i].zone == zone)
            return TRUE;
    }
    for (i = 0; i < gChain.pendingCount; i++) {
        if (gChain.pending[i].player == player && gChain.pending[i].zone == zone)
            return TRUE;
    }
    return FALSE;
}

/*
 * Can the set Magic/Trap in spell/trap zone (player, zone) be activated in answer to chainLink? The card must
 * not be a monster, must be at least as fast as the link, and must not be on the chain or pending yet. It
 * must be face down (keys 1324, 1428, 1532 and Ultimate Offering may also answer face up), its zone must
 * have canActivate set and isDisabled clear, and a Trap cannot answer while Jinzo is active on either field.
 * Then CanActivateEffect on a copy of the link re-pointed at the card.
 */
u16 CanChainFieldCard(struct ChainEntry *chainLink, int player, int zone)
{
    struct ChainEntry entry;
    int p = player & 1;
    struct DuelZone *z = ZONE_ZP(p, zone);
    int cardId = ZONE_CARD(z)->id;
    int faceUp = z->isFaceUp;
    int p2;
    struct DuelZone *z2;

    MemCopy16(&entry, chainLink, sizeof(struct ChainEntry));
    entry.player = player;
    entry.card = cardId;
    if (cardId == 0 || CARD_TYPE(cardId) <= CARD_TYPE_REPTILE
        || GetCardSpellSpeed(cardId) < GetCardSpellSpeed(chainLink->card) || IsZoneInActionLists(player, zone))
        return FALSE;
    switch (CARD_NUMBER(cardId)) {
    case CARD_1324:
    case CARD_ULTIMATE_OFFERING:
    case CARD_1428:
    case CARD_1532:
        faceUp = FALSE;
        break;
    }
    if (faceUp)
        return FALSE;
    /* Matching: the ROM computes the zone address a second time here. */
    p2 = player & 1;
    z2 = ZONE_ZP(p2, zone);
    if (!(ZONE_BYTE91(z2) & ZONE_BYTE91_CAN_ACTIVATE))
        return FALSE;
    if (z2->isDisabled)
        return FALSE;
    if (CARD_TYPE(cardId) == CARD_TYPE_TRAP
        && (CountActiveCardsOnField(0, CARD_JINZO) || CountActiveCardsOnField(1, CARD_JINZO)))
        return FALSE;
    return CanActivateEffect(&entry, chainLink, 0);
}

/*
 * Can hand card handIdx of player be activated in answer to chainLink? It must be a Quick-Play Magic at
 * least as fast as the link, with room on the field (CanPlaceSpellTrapCard); then CanActivateEffect on a
 * copy of the link re-pointed at the card.
 */
u16 CanChainHandCard(struct ChainEntry *chainLink, int player, int handIdx)
{
    struct ChainEntry entry;
    int cardId = HAND_CARD(player & 1, handIdx)->id;
    int tableIdx = CARD_ID_MASK & cardId;
    u32 stats;

    MemCopy16(&entry, chainLink, sizeof(struct ChainEntry));
    entry.player = player;
    entry.card = cardId;
    stats = ((const u32 *)0x08621DE0)[tableIdx];    /* gCardStats[cardId]; the mask is applied above */
    if (CARD_STATS_TYPE(stats) == CARD_TYPE_MAGIC && CARD_STATS_SUBTYPE(stats) == SPELL_QUICK_PLAY
        && GetCardSpellSpeed(cardId) >= GetCardSpellSpeed(chainLink->card)
        && CanPlaceSpellTrapCard(player, cardId))
        return CanActivateEffect(&entry, chainLink, 0);
    return FALSE;
}

/*
 * 1 if player can answer chainLink at all: a set card in spell/trap zones 5-9 (CanChainFieldCard); on
 * player's own turn also a Quick-Play Magic in the hand (CanChainHandCard); otherwise, against a Magic card,
 * a face-up key 1525 in a monster zone that is not already on the chain.
 */
int CanPlayerChain(struct ChainEntry *chainLink, int player)
{
    int i;
    int p;

    for (i = ZONE_SPELL_0; i <= ZONE_SPELL_4; i++) {
        if (CanChainFieldCard(chainLink, player, i))
            return TRUE;
    }
    /* Matching: the statement order of both loops (i = 0 first, the card number in a variable) gives the
     * ROM's register use. */
    if (player == gDuel.turnPlayer) {
        struct DuelPlayer *players;

        i = 0;
        players = gDuel.players;
        p = player & 1;
        for (; i < players[p].handCount; i++) {
            if (CanChainHandCard(chainLink, player, i))
                return TRUE;
        }
        return FALSE;
    }
    if (CARD_TYPE(chainLink->card) == CARD_TYPE_MAGIC) {
        int wanted;

        i = 0;
        p = player & 1;
        wanted = CARD_1525;
        for (; i <= ZONE_MONSTER_4; i++) {
            struct DuelZone *z = ZONE_ZP(p, i);
            u16 cardId = ZONE_CARD(z)->id;

            if (z->isFaceUp && cardId && !IsZoneInActionLists(player, i) && CARD_NUMBER(cardId) == wanted)
                return TRUE;
        }
    }
    return FALSE;
}

/* ---- Prepare handlers (activation conditions) ---- */

/* 15 Time Wizard: on the field, its once-per-turn effect unused, and the opponent has a monster. */
int EffectTimeWizardPrepare(struct ChainEntry *card, int unused, u16 fromHand)
{
    int zone;

    if (fromHand != 0)
        return FALSE;
    if (!ZONE_PZ(card->player, card->zone)->effectUnused)
        return FALSE;
    for (zone = 0; zone <= ZONE_MONSTER_4; zone++) {
        if (ZONE_CARD(ZONE_PZ((1 - card->player) & 1, zone))->id)
            return TRUE;
    }
    return FALSE;
}

/* 71 Cocoon of Evolution: from the hand only, while the player's Normal Summon of this turn is unused. */
int EffectCocoonOfEvolutionPrepare(struct ChainEntry *card, int unused, u16 fromHand)
{
    if (fromHand != 0 && !gDuelPlayers[card->player].normalSummonUsed)
        return TRUE;
    return FALSE;
}

/*
 * 317 Elegant Egotist: the player can Special Summon into a free monster zone, Harpie Lady (or key 1249) is
 * active on either field, and CollectEffectTargets finds a candidate.
 */
int EffectElegantEgotistPrepare(struct ChainEntry *card)
{
    int count;

    if (!CanSpecialSummon(card->player) || !CountFreeMonsterZones(card->player))
        return FALSE;
    count = CountActiveCardsOnField(0, CARD_HARPIE_LADY) + CountActiveCardsOnField(1, CARD_HARPIE_LADY)
        + CountActiveCardsOnField(0, 1249) + CountActiveCardsOnField(1, 1249);   /* 1249: effect key */
    if (count == 0)
        return FALSE;
    return CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) > 0;
}

/* 335 Dark Hole: either player has a monster. */
int EffectDarkHolePrepare(struct ChainEntry *card)
{
    int player;
    int zone;

    for (player = 0; player <= 1; player++) {
        for (zone = 0; zone <= ZONE_MONSTER_4; zone++) {
            if (ZONE_CARD(ZONE_PZ(player & 1, zone))->id)
                return TRUE;
        }
    }
    return FALSE;
}

/* 336 Raigeki: the opponent has a monster. */
int EffectRaigekiPrepare(struct ChainEntry *card)
{
    int zone;

    for (zone = 0; zone <= ZONE_MONSTER_4; zone++) {
        if (ZONE_CARD(ZONE_PZ((1 - card->player) & 1, zone))->id)
            return TRUE;
    }
    return FALSE;
}

/* 349 Dark-Piercing Light: the opponent has a face-down monster. */
int EffectDarkPiercingLightPrepare(struct ChainEntry *card)
{
    int zone;

    for (zone = 0; zone <= ZONE_MONSTER_4; zone++) {
        int p = (1 - card->player) & 1;
        struct DuelZone *z = ZONE_ZP(p, zone);
        struct DuelZone *z2;

        if (ZONE_CARD(z)->id) {
            z2 = ZONE_ZP((1 - card->player) & 1, zone);     /* Matching: the ROM computes it again */
            if (!z2->isFaceUp)
                return TRUE;
        }
    }
    return FALSE;
}

/* 401 Monster Eye: the player has at least 1000 LP for the cost, and CollectEffectTargets finds a
 * candidate. */
int EffectMonsterEyePrepare(struct ChainEntry *card)
{
    if (gDuelPlayers[card->player].lifePoints < 1000)
        return FALSE;
    return CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) > 0;
}

/*
 * 416 Blast Juggler: on the field, in the Standby Phase window (card->event == RESPONSE_OWN_STANDBY), with
 * at least two monsters that its check handler (EffectBlastJugglerCheck) accepts.
 */
int EffectBlastJugglerPrepare(struct ChainEntry *card, int unused, u16 fromHand)
{
    int count;
    int player;
    int zone;

    if (fromHand != 0)
        return FALSE;
    if (card->event != RESPONSE_OWN_STANDBY)
        return FALSE;
    count = 0;
    for (player = 0; player <= 1; player++) {
        for (zone = 0; zone <= ZONE_MONSTER_4; zone++) {
            if (EffectBlastJugglerCheck(card, CHECK_POS(player, zone))) {
                count++;
                if (count > 1)
                    return TRUE;
            }
        }
    }
    return FALSE;
}

/*
 * 419 Cyber-Stein and 505 Gale Dogra (shared prepare handler). Cyber-Stein needs 5000 LP, a card in the
 * Fusion Deck, CanSpecialSummon and a free monster zone; Gale Dogra needs 3000 LP and a card in the Fusion
 * Deck. Any other card: 0.
 */
int EffectCyberSteinPrepare(struct ChainEntry *card)
{
    int result;

    switch (CARD_NUMBER(card->card)) {
    default:
    fail:
        return FALSE;
    case CARD_CYBER_STEIN: {
        struct DuelPlayer *players = gDuelPlayers;
        register u32 playerBit __asm__("r3") = (u32)((u8 *)card)[2] << 31;   /* card->player in bit 31 */
        int one = 1;

        /* FAKEMATCH: keeps the constant 1 (the & 1 mask) in its own register and the shifted player
         * byte live in r3, from which the ROM extracts the player three times. */
        __asm__("" : "+r"(one) : "r"(playerBit));
        if (players[playerBit >> 31].lifePoints < 5000)
            goto fail;
        __asm__("" : : "r"(playerBit));
        if (players[one & (playerBit >> 31)].fusionCount == 0)
            goto fail;
        /* FAKEMATCH: keeps this extraction after the fusionCount test. */
        __asm__("" : "+r"(playerBit));
        if (!CanSpecialSummon(playerBit >> 31))
            goto fail;
        result = CountFreeMonsterZones(card->player);
        break;
    }
    case CARD_GALE_DOGRA: {
        struct DuelPlayer *players = gDuelPlayers;
        register u32 playerBit __asm__("r3") = (u32)((u8 *)card)[2] << 31;
        int one = 1;

        /* FAKEMATCH: the same register lifetimes as in the Cyber-Stein case. */
        __asm__("" : "+r"(one) : "r"(playerBit));
        if (players[playerBit >> 31].lifePoints < 3000)
            goto fail;
        __asm__("" : : "r"(playerBit));
        result = players[one & (playerBit >> 31)].fusionCount;
        __asm__("" : : "r"(playerBit));
        break;
    }
    }
    if (result == 0)
        goto fail;
    return TRUE;
}

/* 424 Thunder Dragon: from the hand (it discards itself), and CollectEffectTargets finds a candidate. */
int EffectThunderDragonPrepare(struct ChainEntry *card, int unused, u16 fromHand)
{
    if (fromHand == 0)
        return FALSE;
    return CollectEffectTargetsInt(card->player, CARD_NUMBER(card->card), 0) > 0;
}

/* 579 Patrol Robo: on the field with its once-per-turn effect unused. */
int EffectPatrolRoboPrepare(struct ChainEntry *card, int unused, u16 fromHand)
{
    if (fromHand != 0)
        return FALSE;
    return ZONE_PZ(card->player, card->zone)->effectUnused;
}

/* 585 Greenkappa: at least two face-down Magic/Trap cards on the field (field zones included). */
int EffectGreenkappaPrepare(struct ChainEntry *card)
{
    int count;

    count = CountSpellTrapsFiltered(0, FALSE, TRUE, TRUE);
    count += CountSpellTrapsFiltered(1, FALSE, TRUE, TRUE);
    return count > 1;
}

/*
 * 650 Kunai with Chain: on the field. Answering an attack declaration (card->event ==
 * RESPONSE_ATTACK_DECLARED), 1 if the attacker (card->loc0 = player | zone << 8) is still there, in attack
 * position, and the opponent's. Otherwise 1 if the player has a face-up monster to equip.
 */
int EffectKunaiWithChainPrepare(struct ChainEntry *card, int unused, u16 fromHand)
{
    if (fromHand != 0)
        return FALSE;
    if (card->event == RESPONSE_ATTACK_DECLARED) {
        u8 attackerPlayer = DUEL_LOC_PLAYER(card->loc0);
        int zone = DUEL_LOC_ZONE(card->loc0);
        int p = attackerPlayer & 1;
        struct DuelZone *z = ZONE_ZP(p, zone);

        if (ZONE_CARD(z)->id && !z->isDefense && card->player != attackerPlayer)
            return TRUE;
    }
    if (CountMonstersFiltered(card->player, TRUE, FALSE) > 0)
        return TRUE;
    return FALSE;
}

/*
 * 660 Crush Card: key 1418 (which forbids tributes) is not active on either field, and the player has a DARK
 * monster with 1000 ATK or less (a monster type, card number below 1900) to tribute.
 */
int EffectCrushCardPrepare(struct ChainEntry *card)
{
    struct ZoneCardStats stats;
    int zone;

    if (CountActiveCardsOnField(0, CARD_1418) > 0 || CountActiveCardsOnField(1, CARD_1418) > 0)
        return FALSE;
    for (zone = 0; zone <= ZONE_MONSTER_4; zone++) {
        struct DuelZonesPlayer *zones = &gDuelZones[card->player];
        struct DuelZone *z = &zones->zones[zone];
        u16 cardId = ZONE_CARD(z)->id;

        if (cardId && CARD_NUMBER(cardId) <= 1899 && CARD_TYPE(cardId) <= CARD_TYPE_REPTILE) {
            GetZoneCardStats(card->player, zone, &stats);
            if (stats.atk <= 1000 && stats.attribute == ATTRIBUTE_DARK)
                return TRUE;
        }
    }
    return FALSE;
}

/* 671 Harpie's Feather Duster: the opponent has a Magic/Trap card (field zone included). */
int EffectHarpiesFeatherDusterPrepare(struct ChainEntry *card)
{
    return CountSpellTrapsFiltered(1 - card->player, FALSE, FALSE, TRUE) > 0;
}

/*
 * 689 Fake Trap: on the field, answering a link of the opponent. Against a single-target removal (Reaper of
 * the Cards, Trap Master, Remove Trap, Mystical Space Typhoon, Gust, Driving Snow), the link's first target
 * must be a Trap other than this card. Against a mass removal (Harpie's Feather Duster, Heavy Storm, Gryphon
 * Wing, Final Destiny), the player must have another Trap in zones 5-9.
 */
int EffectFakeTrapPrepare(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
{
    int zone;

    if (fromHand != 0)
        return FALSE;
    if (chainLink == NULL)
        return FALSE;
    if (chainLink->player == card->player)
        return FALSE;
    switch (CARD_NUMBER(chainLink->card)) {
    case CARD_REAPER_OF_THE_CARDS:
    case CARD_TRAP_MASTER:
    case CARD_REMOVE_TRAP:
    case CARD_MYSTICAL_SPACE_TYPHOON:
    case CARD_GUST:
    case CARD_DRIVING_SNOW: {
        u8 targetPlayer = DUEL_LOC_PLAYER(chainLink->targets[0]);
        int targetZone = DUEL_LOC_ZONE(chainLink->targets[0]);
        int p = targetPlayer & 1;
        struct DuelZone *z = ZONE_ZP(p, targetZone);
        int cardId = ZONE_CARD(z)->id;

        if (cardId == 0)
            return FALSE;
        if (targetPlayer == card->player && targetZone == card->zone)
            return FALSE;
        if (CARD_TYPE(cardId) != CARD_TYPE_TRAP)
            return FALSE;
        return TRUE;
    }
    case CARD_HARPIES_FEATHER_DUSTER:
    case CARD_HEAVY_STORM:
    case CARD_GRYPHON_WING:
    case CARD_FINAL_DESTINY:
        for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
            int p = card->player;
            struct DuelZone *z = ZONE_ZP(p, zone);
            u16 cardId = ZONE_CARD(z)->id;

            if (cardId && zone != card->zone && CARD_TYPE(cardId) == CARD_TYPE_TRAP)
                return TRUE;
        }
        break;
    }
    return FALSE;
}

/*
 * 730 Relinquished: on the field, it has not absorbed a monster yet, the player has a free spell/trap zone
 * (the absorbed monster goes there), and its once-per-turn effect is unused.
 */
int EffectRelinquishedPrepare(struct ChainEntry *card, int unused, u16 fromHand)
{
    int player = card->player;
    int zone = card->zone;

    if (fromHand == 0 && FindAbsorbedMonsterLinkInt(player, zone) == 0xFFFF
        && FindFreeSpellTrapZone(player) != -1)
        return ZONE_PZ(player, zone)->effectUnused;
    return FALSE;
}
