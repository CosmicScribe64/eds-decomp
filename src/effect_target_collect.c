/*
 * effect_target_collect (0x08044224-0x08046737): the card-effect target collector
 * (wiki/functions/effect-target-collect-c.md).
 *
 * CollectEffectTargets(player, cardNumber, param) is the one function here, and with 0x2514 bytes the largest
 * in the game. A card effect that lets the player pick a card from a list (a search, a revival, Polymerization's
 * fusion monsters, Magical Hats' hidden cards, ...) calls it to build that list in gCardListView. The Prepare
 * handlers test the count (> 0 means the effect can be activated) and CardListView_Open(area -1) shows the
 * list to the player.
 *
 * How it works:
 *   1. gCardListView.count is cleared and the effect's card number (or effect key, for the numbers with no EDS
 *      card) is looked up in a switch of 42 bodies for 60 numbers. A number with no body ends with an empty
 *      list.
 *   2. A body scans one or two of the player's piles (hand, deck, graveyard, fusion deck, banished cards) and
 *      appends the card words that qualify to gCardListView.cards[], each with the pile it came from in
 *      gCardListView.sources[] (enum CardListSource). The words are copied whole, flags included.
 *   3. If anything was found, three filters remove entries again: Toon monsters (unless Toon World is face up
 *      or the case skips the filter), cards with the planted bit, and cards that Prohibition names.
 *
 * The piles scanned are the player's own (player & 1) unless a comment says "opponent" ((1 - player) & 1). A
 * "monster" is a card whose type is at most CARD_TYPE_REPTILE (the last monster type); Trap, Magic, Ticket and
 * Divine cards have higher types. Keys 1211-1552 are effect keys with no EDS card (see include/effect.h).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_* extractors */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, CardKind, CardAttribute, SpellSubtype */
#include "constants/duel.h"         /* BANISH_FACE_DOWN */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that card_list_view.h does not pull in the legacy header. After H0, replace the block (BEGIN to
 * END) with #include "legacy/duel.h" (see build/readability/issues/effect_target_collect.md). */
#define GUARD_DUEL_H

/* A card in a zone or pile: one 32-bit word. id 0 is an empty slot. */
struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
    u32 owner:1;                    /* bit 12: owning player: whose graveyard, hand or deck the card returns to */
    u32 unk13:1;
    u32 unk14:1;
    u32 normalSummoned:1;           /* bit 15 */
    u32 specialSummoned:1;          /* bit 16 */
    u32 planted:1;                  /* bit 17: Parasite Paracide shuffled into the other player's deck */
    u32 graverobbed:1;              /* bit 18 */
    u32 unk19:1;
    u32 isFusionMaterial:1;         /* bit 20: one of Polymerization's materials */
    u32 destroyedInBattle:1;        /* bit 21: set before a battle-destroyed monster enters the graveyard */
    u32 destroyedByOpponent:1;      /* bit 22 */
    u32 flag23:1;
    u32 pendingEquip:1;
    u32 equipZone:3;
    u32 pendingOpponentSummon:1;
    u32 unk29:3;
};

STATIC_ASSERT(sizeof(struct DuelCard) == 4, DuelCardSize);

/* Player 0's deck and graveyard (the aliases are symbols at gDuelPlayers + 0x7C4 / + 0x904); player 1's
 * piles follow at the player stride 0xD64. */
extern struct DuelCard gDuelDecks[];            /* 0x02019AA8 = gDuelPlayers[0].deck */
extern struct DuelCard gDuelGraveyards[];       /* 0x02019BE8 = gDuelPlayers[0].graveyard */

/* 1 if card number cardNo is one of the four Toon effect monsters (Toon Alligator is a Normal Monster). */
u32 IsToonMonster(u16 cardNo);
/* 1 if the card is an Effect Monster. */
u32 IsEffectMonster(u16 cardId);
/* 1 if the monster cannot be Normal Summoned or Set (Fusion, Ritual and some effect monsters). */
u32 IsSpecialSummonOnly(u16 cardId);
/* *dst = *src for a duel card word. */
void CopyDuelCard(u32 *dst, u32 *src);
/* 1 if Toon World is face up in the player's spell/trap zones. */
int HasFaceUpToonWorld(int player);
/* 1 if an active Prohibition declares the same card name. */
u32 IsCardProhibited(u16 cardId);
/* ---- END duel.h stand-in ---- */

#include "card_list_view.h"         /* gCardListView, enum CardListSource */
#include "effect.h"                 /* CollectEffectTargets, IsMaterialOfFusion, FindFusionMaterials,
                                     * CanReviveGraveyardCard */

/*
 * Local views kept on purpose (matching choices, see build/readability/HEADERS.md).
 */

/*
 * Matching: gDuelPlayers as this unit reads it. struct DuelPlayer (include/duel.h) keeps its piles as
 * struct DuelCard, but the matched code needs two other views of the same memory:
 *   words  the piles as raw u32 card words: whole words, flags included, are copied into gCardListView,
 *          and the loops index them as u32 array members
 *   bits   the graveyard with bit 20 (isFusionMaterial) as a signed 1-bit field: the ROM tests it with
 *          ldrb; lsl #27; cmp; bge, which only a signed field read straight from a struct-typed array
 *          element gives (a u32 field loads a word and shifts by 11; a u8 field or a mask gives movs/ands)
 * Both are members of one union, declared once under another name that is bound to the symbol, and used for
 * every access to gDuelPlayers in this unit: the ROM's address arithmetic only comes out when all accesses
 * share one declaration (a second declaration of the symbol, or a cast of gDuelPlayers, changes which
 * partial sums the compiler shares). Field offsets are those of struct DuelPlayer.
 */
struct DuelPlayerWords {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 graveCount;                  /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 banishedCount;               /* +0x006 */
    u8 unk7[0x684 - 0x7];
    u32 hand[80];                   /* +0x684 */
    u32 deck[80];                   /* +0x7C4 */
    u32 graveyard[80];              /* +0x904 */
    u32 fusionDeck[80];             /* +0xA44 */
    u32 banished[80];               /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4: low byte enum BanishKind */
};
struct DuelCardSignedBit20 {
    u32 unk0_0:20;
    s32 isFusionMaterial:1;         /* bit 20 */
    u32 unk0_21:11;
};
struct DuelPlayerBits {
    u8 unk0[0x904];
    struct DuelCardSignedBit20 graveyard[80];   /* +0x904 */
    u8 unkA44[0xD64 - 0xA44];
};
union DuelPlayerView {
    struct DuelPlayerWords words;
    struct DuelPlayerBits bits;
};
extern union DuelPlayerView gDuelPlayerView[2] asm("gDuelPlayers");

STATIC_ASSERT(sizeof(union DuelPlayerView) == 0xD64, DuelPlayerViewSize);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayerWords, hand) == 0x684, DuelPlayerWordsHand);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayerWords, deck) == 0x7C4, DuelPlayerWordsDeck);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayerWords, graveyard) == 0x904, DuelPlayerWordsGraveyard);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayerWords, fusionDeck) == 0xA44, DuelPlayerWordsFusionDeck);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayerWords, banished) == 0xB84, DuelPlayerWordsBanished);
STATIC_ASSERT(OFFSET_OF(struct DuelPlayerWords, banishedInfo) == 0xCC4, DuelPlayerWordsBanishedInfo);

/* Player pl's side as card words (pl is masked with & 1) or with the signed graveyard bit. */
#define PLAYER(pl)      (gDuelPlayerView[(pl) & 1].words)
#define PLAYER_BITS(pl) (gDuelPlayerView[(pl) & 1].bits)

/* One card of a pile as a struct DuelCard array element: gDuelDecks and gDuelGraveyards are only read through
 * byte arithmetic. Matching: the zone read `pile->cards[i]` (a member array) adds the base before the index,
 * as the ROM does; `((struct DuelCard *)pile)[i]` adds the index first. */
struct DuelCardPile {
    struct DuelCard cards[80];
};

/* ---- Pile access ---- */

/* Size of struct DuelPlayer: the distance between player 0's and player 1's copy of every pile. */
#define PLAYER_STRIDE 0xD64

/*
 * A pointer to card word i of a pile of player pl (pl is masked with & 1 here). `pile` is the pile of
 * player 0, either as gDuelDecks / gDuelGraveyards or as one of the PLAYERS_* bases below.
 * Matching: the ROM computes these addresses in three different orders, and each case matches only with its
 * own, because the order decides which partial sum the compiler keeps in a register.
 *   PILE_WORD       pile + (i * 4 + pl * 0xD64)    the sum of both terms is added to the base
 *   PILE_WORD_FLAT  pile + i * 4 + pl * 0xD64      the index term is added to the base first
 *   PILE_WORD_PL_I  pile + (pl * 0xD64 + i * 4)    as PILE_WORD with the terms swapped
 * The two spellings of a pile base (the gDuelDecks alias or gDuelPlayers + 0x7C4) load different
 * literals, so they are not interchangeable either. The copies into gCardListView read the pile as a
 * u32 array member instead (PLAYER(pl).deck[i]), which gives the ROM's address order for the word loaded.
 */
#define PILE_WORD(pile, i, pl)      ((u32 *)((u8 *)(pile) + ((i) * 4 + ((pl) & 1) * PLAYER_STRIDE)))
#define PILE_WORD_FLAT(pile, i, pl) ((u32 *)((u8 *)(pile) + (i) * 4 + ((pl) & 1) * PLAYER_STRIDE))
#define PILE_WORD_PL_I(pile, i, pl) ((u32 *)((u8 *)(pile) + (((pl) & 1) * PLAYER_STRIDE + (i) * 4)))

/* The piles of player 0 reached through gDuelPlayers. */
#define PLAYERS_HAND        ((u8 *)gDuelPlayerView + OFFSET_OF(struct DuelPlayerWords, hand))
#define PLAYERS_DECK        ((u8 *)gDuelPlayerView + OFFSET_OF(struct DuelPlayerWords, deck))
#define PLAYERS_GRAVEYARD   ((u8 *)gDuelPlayerView + OFFSET_OF(struct DuelPlayerWords, graveyard))
#define PLAYERS_FUSION_DECK ((u8 *)gDuelPlayerView + OFFSET_OF(struct DuelPlayerWords, fusionDeck))
#define PLAYERS_BANISHED    ((u8 *)gDuelPlayerView + OFFSET_OF(struct DuelPlayerWords, banished))

/* ---- Card tables ---- */

/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)       (((u32)(word) << 20) >> 20)

/*
 * gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4) through integer-constant addresses.
 * Matching: these give the ROM's literal pools; the symbol forms load other code. CARD_STATS_VOLATILE reads
 * the table through a volatile pointer so that each use loads again: the ROM does not share the repeated
 * lookups in most cases (the Nv helpers below are the cases where it does).
 */
#define CARD_STATS_VOLATILE(id)     (((volatile const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_STATS(id)              (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id)             (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])

/*
 * Stat helpers for the target filters. The variants differ only in the integer width of the parameter and
 * result and in whether the stats word is read through a volatile pointer; each case uses the one that
 * reproduces the ROM's register allocation and loop-invariant hoisting (loop.c moves a constant out of a
 * loop only when the loop is small enough, and the narrowing casts decide how many insns it has):
 *   (no suffix)  volatile read, u32 or int parameters
 *   Nv           non-volatile read: the stats word may be shared between a type test and an ATK test
 *   8 / 16       u8 result / u16 parameter
 * Trap, Magic and Ticket cards have no ATK, DEF or level; the Divine-Beast cards count as 4000 ATK / DEF
 * and level 10.
 */
#define DIVINE_STAT     4000
#define DIVINE_LEVEL    10

/* Card type (enum CardType), volatile read. */
static inline u32 CardType(u16 id)
{
    return CARD_STATS_TYPE(CARD_STATS_VOLATILE(id));
}

/* Card type as a u8: the narrowing adds pass-1 loop insns (removed later by combine), which keeps loop.c
   from hoisting &count in pass 1 (Call of the Haunted group and Spear Cretin: the sources[] pointer before the
   count pointer, as in the ROM). */
static inline u8 CardType8(u16 id)
{
    return CARD_STATS_TYPE(CARD_STATS_VOLATILE(id));
}

/* Card type, non-volatile read (cases that share the stats load between the type and ATK tests). */
static inline u32 CardTypeNv(u16 id)
{
    return CARD_STATS_TYPE(CARD_STATS(id));
}

/* ATK of a card whose type the caller has already read (non-volatile stats read). */
static inline u32 CardAttackOfType(u32 id, u32 type)
{
    switch ((s32)type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return DIVINE_STAT;
    }
    return CARD_STATS_ATK(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE;
}

/* ATK with a u16 id: the parameter's zero-extension adds pass-1 loop insns, so the sources[] pointer of
   Sangan's loop is hoisted in loop pass 2 (after the 1500 constant), as in the ROM. */
static inline u16 CardAttack16(u16 id)
{
    u32 type = CARD_STATS_TYPE(CARD_STATS_VOLATILE(id));

    switch ((s32)type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return DIVINE_STAT;
    }
    return CARD_STATS_ATK(CARD_STATS_VOLATILE(id)) * CARD_STATS_POINTS_SCALE;
}

/* ATK, non-volatile read, u16 id and result (Last Will, Backup Soldier). */
static inline u16 CardAttackNv16(u16 id)
{
    u32 type = CARD_STATS_TYPE(CARD_STATS(id));

    switch ((s32)type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return DIVINE_STAT;
    }
    return CARD_STATS_ATK(CARD_STATS(id)) * CARD_STATS_POINTS_SCALE;
}

static inline u16 CardDefense(u16 id)
{
    u32 type = CARD_STATS_TYPE(CARD_STATS_VOLATILE(id));

    switch ((s32)type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return DIVINE_STAT;
    }
    return CARD_STATS_DEF(CARD_STATS_VOLATILE(id)) * CARD_STATS_POINTS_SCALE;
}

static inline s8 CardLevel(u16 id)
{
    u32 type = CARD_STATS_TYPE(CARD_STATS_VOLATILE(id));

    switch ((s32)type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return DIVINE_LEVEL;
    }
    return CARD_STATS_LEVEL(CARD_STATS_VOLATILE(id));
}

/* Card kind (enum CardKind): the three Egyptian-God-like numbers first (Obelisk counts as Ritual, Slifer
 * and Ra as Effect), then Magic / Trap / Ticket by type, else the kind bits of the stats word. */
static inline s8 CardKind(u32 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((s32)CARD_STATS_TYPE(CARD_STATS_VOLATILE(id))) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    }
    return CARD_STATS_KIND(CARD_STATS_VOLATILE(id));
}

/* CardKind with a u16 id (Senju of the Thousand Hands): all three `id & 0x7FF` ANDs are then HImode, so
   their constants match and loop.c hoists 0x7FF in pass 1 (ROM preheader order). The attribute searches need
   the u32 CardKind. */
static inline s8 CardKind16(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((s32)CARD_STATS_TYPE(CARD_STATS_VOLATILE(id))) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    }
    return CARD_STATS_KIND(CARD_STATS_VOLATILE(id));
}

/* Magic/Trap subtype (enum SpellSubtype) of a card, 0 for any other type; non-volatile stats read. */
static inline u32 CardSpellSubtypeNv(u16 id)
{
    u32 stats = CARD_STATS(id);

    switch ((s32)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(stats);
    }
    return 0;
}

/* ---- The list being built ---- */

/* Search limit shared by most "ATK/DEF <= 1500" searches (Sangan, Witch of the Black Forest, the attribute
 * floaters, Last Will, Backup Soldier). */
#define STAT_LIMIT 1500

/*
 * Append a card word and its source pile to gCardListView.
 * Matching: ADD_TARGET is a plain { } block; a do { } while (0) would add loop notes that weight every
 * reference inside one loop level deeper, so ADD_TARGET_DOWHILE is used only in the three groups that match
 * with it: the attribute floaters (keys 1108-1123), the graveyard revivals of Call of the Haunted and its
 * kind, and the graveyard monsters by attribute (keys 1515-1519).
 */
#define ADD_TARGET(card, source) { \
    gCardListView.cards[gCardListView.count] = (card); \
    gCardListView.sources[gCardListView.count] = (source); \
    gCardListView.count++; \
}
#define ADD_TARGET_DOWHILE(card, source) do { \
    gCardListView.cards[gCardListView.count] = (card); \
    gCardListView.sources[gCardListView.count] = (source); \
    gCardListView.count++; \
} while (0)

/*
 * Delete cards[i] from the list: shift the later words down one place and decrement the count. Matching: the
 * copy loop runs through j == count, so it reads cards[count], one word past the last entry, as the ROM does;
 * sources[] is not shifted, so after a removal the tags no longer line up with the words.
 */
#define REMOVE_CARD(i) { \
    for (j = (i); j < gCardListView.count; j++) \
        gCardListView.cards[j] = gCardListView.cards[j + 1]; \
    gCardListView.count--; \
}

/*
 * Fill gCardListView with the cards player can pick for the effect of cardNumber and return how many there
 * are (0 when the effect has no candidate or the number is not handled here).
 *
 * param is an extra argument of some effects: the level to search for (key 1318), the first argument of
 * IsMaterialOfFusion (key 1546), the player that Spear Cretin compares against (1116). The cardNumber is the
 * effect key: a card number, or a number with no EDS card (see enum CardNumber).
 */
u16 CollectEffectTargets(int player, u16 cardNumber, int param)
{
    u16 materials[4];           /* scratch for FindFusionMaterials; only its result is used */
    int forceToonFilter = 0;    /* run the Toon filter even though Toon World is face up (Last Will) */
    int skipToonFilter = 0;     /* never run the Toon filter (graveyard pickers, Painful Choice, ...) */
    int searching;              /* Spear Cretin: still looking for its own copy in the list */
    int i;
    int j;
    u16 toonWorldUp;

    gCardListView.count = 0;
    switch (cardNumber) {
    case CARD_SANGAN:
    {
        /* Deck monsters with ATK of at most 1500. */
        i = 0;
        /* FAKEMATCH: the empty clobber right after i = 0 makes global allocation give i r8, as in the ROM,
           while i stays a basic induction variable (a register ... asm("r8") pin stops the strength reduction
           of the i-indexed addresses). */
        asm volatile("" ::: "r4", "r5", "r6", "r7");
        for (; i < PLAYER(player).deckCount; i++) {
            /* Matching: copying the card as a bitfield struct gives the ROM's single lsl #20 and two lsr #20
               (one ID for the type test, another for the ATK test). */
            struct DuelCard card = *(struct DuelCard *)PILE_WORD(gDuelDecks, i, player);

            if (CardType(card.id) <= CARD_TYPE_REPTILE && CardAttack16(card.id) <= STAT_LIMIT)
                ADD_TARGET(PLAYER(player).deck[i], CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_MASK_OF_DARKNESS:
    {
        /* Graveyard Traps. */
        u32 *word;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, player);
            if (CardType(CARD_ID(*word)) == CARD_TYPE_TRAP)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_ELEGANT_EGOTIST:
    {
        /* Hand, then deck: Harpie Lady, Harpie Lady Sisters and key 1249 (no EDS card). */
        u32 *word;

        for (i = 0; i < PLAYER(player).handCount; i++) {
            word = PILE_WORD(PLAYERS_HAND, i, player);
            switch (CARD_NUMBER(CARD_ID(*word))) {
            case CARD_HARPIE_LADY:
            case CARD_HARPIE_LADY_SISTERS:
            case 1249:
                ADD_TARGET(*word, CARDLIST_SRC_HAND);
                break;
            }
        }
        for (i = 0; i < PLAYER(player).deckCount; i++) {
            word = PILE_WORD(gDuelDecks, i, player);
            switch (CARD_NUMBER(CARD_ID(*word))) {
            case CARD_HARPIE_LADY:
            case CARD_HARPIE_LADY_SISTERS:
            case 1249:
                ADD_TARGET(*word, CARDLIST_SRC_DECK);
                break;
            }
        }
        break;
    }
    case CARD_TIME_WIZARD:
    {
        /* Hand, then deck: Dark Sage. */
        int cardNo;     /* Matching: the card number is kept in a variable, as in the ROM, even though only
                           the comparison reads it. */

        for (i = 0; i < PLAYER(player).handCount; i++) {
            u32 *wp = PILE_WORD(PLAYERS_HAND, i, player);

            if ((cardNo = CARD_NUMBER(CARD_ID(*wp))) == CARD_DARK_SAGE)
                ADD_TARGET(*wp, CARDLIST_SRC_HAND);
        }
        for (i = 0; i < PLAYER(player).deckCount; i++) {
            u32 *wp = PILE_WORD(PLAYERS_DECK, i, player);

            if ((cardNo = CARD_NUMBER(CARD_ID(*wp))) == CARD_DARK_SAGE)
                ADD_TARGET(*wp, CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_MONSTER_EYE:
    {
        /* Graveyard: Polymerization (either number). */
        u32 *word;
        int cardNo;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(gDuelGraveyards, i, player);
            cardNo = CARD_NUMBER(CARD_ID(*word));
            if (cardNo == CARD_POLYMERIZATION || cardNo == CARD_POLYMERIZATION_ALT)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_CYBER_STEIN:
    case CARD_GALE_DOGRA:
    case CARD_1512:
    {
        /* Every card of the fusion deck. No source tags are written: the unfiltered fusion-deck copy
           leaves sources[] as it was. */
        for (i = 0; i < PLAYER(player).fusionCount; i++) {
            gCardListView.cards[gCardListView.count] = PLAYER(player).fusionDeck[i];
            gCardListView.count++;
        }
        break;
    }
    case CARD_THUNDER_DRAGON:
    {
        /* Deck: other Thunder Dragons. */
        u32 *word;

        for (i = 0; i < PLAYER(player).deckCount; i++) {
            word = PILE_WORD(PLAYERS_DECK, i, player);
            if (CARD_NUMBER(CARD_ID(*word)) == CARD_THUNDER_DRAGON)
                ADD_TARGET(*word, CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_MAGICIAN_OF_FAITH:
    {
        /* Graveyard Magic cards. */
        u32 *word;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, player);
            if (CardType(CARD_ID(*word)) == CARD_TYPE_MAGIC)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_WITCH_OF_THE_BLACK_FOREST:
    {
        /* Deck monsters with DEF of at most 1500. */
        for (i = 0; i < PLAYER(player).deckCount; i++) {
            struct DuelCard card = *(struct DuelCard *)PILE_WORD(gDuelDecks, i, player);

            if (CardType(card.id) <= CARD_TYPE_REPTILE && CardDefense(card.id) <= STAT_LIMIT)
                ADD_TARGET(PLAYER(player).deck[i], CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_MAGICAL_HATS:
    {
        /* Deck cards that are not monsters (Trap, Magic, Ticket, Divine). */
        u32 *word;

        for (i = 0; i < PLAYER(player).deckCount; i++) {
            word = PILE_WORD(PLAYERS_DECK, i, player);
            if (CardType(CARD_ID(*word)) > CARD_TYPE_REPTILE)
                ADD_TARGET(*word, CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_THE_FLUTE_OF_SUMMONING_DRAGON:
    {
        /* Hand cards that are Dragons. ROM quirk, kept: the test reads hand[i] but the word added is
           deck[i], the deck word at the same index, tagged as a hand card. */
        u32 id;
        u32 type;

        for (i = 0; i < PLAYER(player).handCount; i++) {
            id = CARD_ID(*PILE_WORD(PLAYERS_HAND, i, player));
            if (id != 0) {
                type = CardType(id);
                if (type == CARD_TYPE_DRAGON)
                    ADD_TARGET(*PILE_WORD(PLAYERS_DECK, i, player), CARDLIST_SRC_HAND);
            }
        }
        break;
    }
    case CARD_POLYMERIZATION:
    case CARD_POLYMERIZATION_ALT:
    case CARD_1547:
    {
        /* Fusion deck monsters whose materials the player holds. */
        u32 *word;

        for (i = 0; i < PLAYER(player).fusionCount; i++) {
            word = PILE_WORD(PLAYERS_FUSION_DECK, i, player);
            if (FindFusionMaterials(player, CARD_ID(*word), materials) != 0)
                ADD_TARGET(*word, CARDLIST_SRC_FUSION_DECK);
        }
        break;
    }
    case CARD_MONSTER_REBORN:
    {
        /* Monsters in either graveyard (player 0's, then player 1's) that CanReviveGraveyardCard accepts. */
        u32 *word;

        for (j = 0; j <= 1; j++) {
            for (i = 0; i < PLAYER(j).graveCount; i++) {
                word = PILE_WORD(gDuelGraveyards, i, j);
                if (CardType(CARD_ID(*word)) <= CARD_TYPE_REPTILE
                    && (u16)CanReviveGraveyardCard(j, i) != 0)
                    ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
            }
        }
        break;
    }
    case CARD_GRAVEDIGGER_GHOUL:
    {
        /* Monsters in the opponent's graveyard. */
        u32 *word;

        skipToonFilter = 1;
        for (i = 0; i < PLAYER(1 - player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, 1 - player);
            if (CardType(CARD_ID(*word)) <= CARD_TYPE_REPTILE)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_ANCIENT_TELESCOPE:
    {
        /* The top five cards of the opponent's deck. */
        skipToonFilter = 1;
        for (i = 0; i < PLAYER(1 - player).deckCount && i <= 4; i++) {
            ADD_TARGET(PLAYER(1 - player).deck[i], CARDLIST_SRC_DECK);
            /* FAKEMATCH: the empty asm emits nothing; it only makes loop.c's second pass count 27 insns
               (26 without it). threshold(26) * savings(1) * life(1) < insn_count then keeps the cards base
               (list + 12) inside the loop, while the body/latch use the same (1 - player) & 1 so the latch
               pointer is hoisted in pass 1, before the deck-offset giv init (ROM order). */
            asm("");
        }
        break;
    }
    case CARD_SOUL_RELEASE:
    {
        /* Every card in both graveyards, own first. */
        skipToonFilter = 1;
        for (i = 0; i < PLAYER(player).graveCount; i++) {
            ADD_TARGET(PLAYER(player).graveyard[i], CARDLIST_SRC_GRAVEYARD);
            /* FAKEMATCH: three empty asms pad loop.c's second pass from 24 to 27 insns so the cards base
               (list + 12) stays in the loop; same index as the test, so the latch pointer copy is hoisted in
               pass 1 before the giv init, as in the ROM (see CARD_ANCIENT_TELESCOPE). Two are not enough. */
            asm("");
            asm("");
            asm("");
        }
        for (i = 0; i < PLAYER(1 - player).graveCount; i++) {
            ADD_TARGET(PLAYER(1 - player).graveyard[i], CARDLIST_SRC_GRAVEYARD);
            /* FAKEMATCH: loop padding, as above. */
            asm("");
            asm("");
            asm("");
        }
        break;
    }
    case CARD_FUSION_SAGE:
    {
        /* Deck: Polymerization (either number). */
        u32 *word;
        int cardNo;

        for (i = 0; i < PLAYER(player).deckCount; i++) {
            word = PILE_WORD(PLAYERS_DECK, i, player);
            cardNo = CARD_NUMBER(CARD_ID(*word));
            if (cardNo == CARD_POLYMERIZATION || cardNo == CARD_POLYMERIZATION_ALT)
                ADD_TARGET(*word, CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_LAST_WILL:
    {
        /* Deck monsters with ATK of at most 1500 that can be Special Summoned from the deck (not
           special-summon-only). The Toon filter always runs, even with Toon World face up. */
        for (i = 0; i < PLAYER(player).deckCount; i++) {
            /* The test read folds to the constant gDuelPlayers + 0x7C4; loop.c matches the copy's
               `base + 0x7C4` with it, so that add stays in the loop next to the hoisted base (sl). */
            u16 id = CARD_ID(*PILE_WORD_FLAT(PLAYERS_DECK, i, player));

            if (CardTypeNv(id) <= CARD_TYPE_REPTILE && CardAttackNv16(id) <= STAT_LIMIT
                && IsSpecialSummonOnly(id) == 0)
                ADD_TARGET(PLAYER(player).deck[i], CARDLIST_SRC_DECK);
        }
        forceToonFilter = 1;
        break;
    }
    case CARD_PAINFUL_CHOICE:
    {
        /* Every card of the deck. */
        skipToonFilter = 1;
        for (i = 0; i < PLAYER(player).deckCount; i++) {
            ADD_TARGET(PLAYER(player).deck[i], CARDLIST_SRC_DECK);
            /* FAKEMATCH: three dead stores (nothing reads j/searching/toonWorldUp before the tail rewrites
               them, so flow deletes them after loop.c). They keep the loop at 27 insns in the second
               loop pass, so loop.c leaves the `list + 12` cards address in the loop
               (threshold 26 * savings 1 * life 1 < 27), as the ROM does, while the plain
               `player & 1` latch test still lets the first pass hoist the latch pointer copy. */
            j = i;
            searching = i;
            toonWorldUp = i;
        }
        break;
    }
    case CARD_GRAVEROBBER:
    {
        /* Magic cards in the opponent's graveyard. */
        u32 *word;

        for (i = 0; i < PLAYER(1 - player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, 1 - player);
            if (CardType(CARD_ID(*word)) == CARD_TYPE_MAGIC)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_GIANT_RAT:        /* EARTH */
    case CARD_UFO_TURTLE:       /* FIRE */
    case CARD_SHINING_FAIRY:    /* LIGHT */
    case CARD_MOTHER_GRIZZLY:   /* WATER */
    case CARD_FLYING_KAMAKIRI_1:/* WIND */
    case CARD_MYSTIC_TOMATO:    /* DARK */
    {
        /* The "floaters": deck monsters with ATK of at most 1500 and the attribute of the card, except
           Fusion and Ritual monsters and nine monsters that have their own summon procedure (Larvae Moth,
           Great Moth, Harpie Lady Sisters, Perfectly Ultimate Great Moth, Wall Shadow, Gate Guardian,
           Metalzoa, Red-Eyes Black Metal Dragon, Valkyrion the Magna Warrior). */
        u32 id;
        u32 type;
        u32 attribute;
        u8 ok;

        for (i = 0; i < PLAYER(player).deckCount; i++) {
            id = CARD_ID(*PILE_WORD_FLAT(gDuelDecks, i, player));
            type = CARD_STATS_TYPE(CARD_STATS(id));
            if (type > CARD_TYPE_REPTILE || CardAttackOfType(id, type) > STAT_LIMIT)
                continue;
            ok = 0;
            switch (cardNumber) {
            case CARD_GIANT_RAT:
                attribute = CARD_STATS_ATTR(CARD_STATS_VOLATILE(id));
                ok = attribute == ATTRIBUTE_EARTH;
                break;
            case CARD_UFO_TURTLE:
                attribute = CARD_STATS_ATTR(CARD_STATS_VOLATILE(id));
                ok = attribute == ATTRIBUTE_FIRE;
                break;
            case CARD_SHINING_FAIRY:
                attribute = CARD_STATS_ATTR(CARD_STATS_VOLATILE(id));
                ok = attribute == ATTRIBUTE_LIGHT;
                break;
            case CARD_MOTHER_GRIZZLY:
                attribute = CARD_STATS_ATTR(CARD_STATS_VOLATILE(id));
                ok = attribute == ATTRIBUTE_WATER;
                break;
            case CARD_FLYING_KAMAKIRI_1:
                attribute = CARD_STATS_ATTR(CARD_STATS_VOLATILE(id));
                ok = attribute == ATTRIBUTE_WIND;
                break;
            case CARD_MYSTIC_TOMATO:
                attribute = CARD_STATS_ATTR(CARD_STATS_VOLATILE(id));
                ok = attribute == ATTRIBUTE_DARK;
                break;
            }
            if (CardKind(id) == CARD_KIND_RITUAL || CardKind(id) == CARD_KIND_FUSION)
                ok = 0;
            switch (CARD_NUMBER(id)) {
            case CARD_LARVAE_MOTH:
            case CARD_GREAT_MOTH:
            case CARD_HARPIE_LADY_SISTERS:
            case CARD_PERFECTLY_ULTIMATE_GREAT_MOTH:
            case CARD_WALL_SHADOW:
            case CARD_GATE_GUARDIAN:
            case CARD_METALZOA:
            case CARD_RED_EYES_BLACK_METAL_DRAGON:
            case CARD_VALKYRION_THE_MAGNA_WARRIOR:
                ok = 0;
                break;
            }
            if (ok != 0)
                ADD_TARGET_DOWHILE(PLAYER(player).deck[i], CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_SENJU_OF_THE_THOUSAND_HANDS:
    {
        /* Deck Ritual monsters. */
        for (i = 0; i < PLAYER(player).deckCount; i++) {
            u16 id = CARD_ID(*PILE_WORD_FLAT(gDuelDecks, i, player));

            if (CardType(id) <= CARD_TYPE_REPTILE && CardKind16(id) == CARD_KIND_RITUAL)
                ADD_TARGET(PLAYER(player).deck[i], CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_GIANT_GERM:
    case CARD_NIMBLE_MOMONGA:
    case CARD_1307:
    {
        /* Deck: other copies of the card itself. */
        u32 *word;

        for (i = 0; i < PLAYER(player).deckCount; i++) {
            word = PILE_WORD(PLAYERS_DECK, i, player);
            if (CARD_NUMBER(CARD_ID(*word)) == cardNumber)
                ADD_TARGET(*word, CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_SONIC_BIRD:
    {
        /* Deck Ritual Magic cards. */
        for (i = 0; i < PLAYER(player).deckCount; i++) {
            struct DuelCard card = *(struct DuelCard *)PILE_WORD(PLAYERS_DECK, i, player);

            if (CardType(card.id) == CARD_TYPE_MAGIC && CardSpellSubtypeNv(card.id) == SPELL_RITUAL)
                ADD_TARGET(PLAYER(player).deck[i], CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_CALL_OF_THE_HAUNTED:
    case CARD_THE_SHALLOW_GRAVE:
    case CARD_PREMATURE_BURIAL:
    case CARD_1520:
    {
        /* Graveyard monsters that can be revived (CanReviveGraveyardCard tests the Normal / Effect / Fusion /
           Ritual rules). */
        u32 *word;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(gDuelGraveyards, i, player);
            if (CardType8(CARD_ID(*word)) <= CARD_TYPE_REPTILE
                && (u16)CanReviveGraveyardCard(player, i) != 0)
                ADD_TARGET_DOWHILE(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_SPEAR_CRETIN:
    {
        /* As the revivals above, then, when param is the player, the first Spear Cretin in the list is taken
           out again (hypothesis: so that it cannot revive itself). */
        u32 *word;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(gDuelGraveyards, i, player);
            if (CardType8(CARD_ID(*word)) <= CARD_TYPE_REPTILE
                && (u16)CanReviveGraveyardCard(player, i) != 0)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        if (player == param) {
            searching = 1;
            for (i = 0; i < gCardListView.count && searching; i++) {
                u32 *t = gCardListView.cards;

                if (CARD_NUMBER(CARD_ID(t[i])) == CARD_SPEAR_CRETIN) {
                    searching = 0;
                    /* Unlike REMOVE_CARD, this one moves the tags too and stops at the new count. */
                    gCardListView.count--;
                    for (j = i; j < gCardListView.count; j++) {
                        CopyDuelCard(&gCardListView.cards[j], &gCardListView.cards[j + 1]);
                        gCardListView.sources[j] = gCardListView.sources[j + 1];
                    }
                }
            }
        }
        break;
    }
    case CARD_BACKUP_SOLDIER:
    {
        /* Graveyard monsters with ATK of at most 1500 that are not Effect Monsters. */
        for (i = 0; i < PLAYER(player).graveCount; i++) {
            u16 id = CARD_ID(*PILE_WORD_FLAT(PLAYERS_GRAVEYARD, i, player));

            if (CardTypeNv(id) <= CARD_TYPE_REPTILE && CardAttackNv16(id) <= STAT_LIMIT
                && IsEffectMonster(id) == 0)
                ADD_TARGET(PLAYER(player).graveyard[i], CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_DARK_SAGE:
    {
        /* Deck Magic cards. */
        u32 *word;

        for (i = 0; i < PLAYER(player).deckCount; i++) {
            word = PILE_WORD(PLAYERS_DECK, i, player);
            if (CardType(CARD_ID(*word)) == CARD_TYPE_MAGIC)
                ADD_TARGET(*word, CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_1212:
    {
        /* Deck: Dark Magician (either art) and key 1210. */
        u32 *word;

        for (i = 0; i < PLAYER(player).deckCount; i++) {
            /* Matching: the address of player 0's deck[i] plus the player offset. */
            word = (u32 *)((u8 *)&gDuelPlayerView[0].words.deck[i] + (player & 1) * PLAYER_STRIDE);
            switch (CARD_NUMBER(CARD_ID(*word))) {
            case CARD_DARK_MAGICIAN:
            case CARD_1210:
            case CARD_DARK_MAGICIAN_ALT:
                ADD_TARGET(*word, CARDLIST_SRC_DECK);
                break;
            }
        }
        break;
    }
    case CARD_1240:
    {
        /* Deck: Gazelle the King of Mythical Beasts. */
        u32 *word;

        for (i = 0; i < PLAYER(player).deckCount; i++) {
            word = PILE_WORD(PLAYERS_DECK, i, player);
            if (CARD_NUMBER(CARD_ID(*word)) == CARD_GAZELLE_THE_KING_OF_MYTHICAL_BEASTS)
                ADD_TARGET(*word, CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_1241:
    {
        /* Graveyard: Gazelle the King of Mythical Beasts and key 1240. */
        u32 *word;
        int cardNo;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, player);
            cardNo = CARD_NUMBER(CARD_ID(*word));
            if (cardNo == CARD_GAZELLE_THE_KING_OF_MYTHICAL_BEASTS || cardNo == CARD_1240)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_1318:
    {
        /* Deck Insects whose level is param and that can be Special Summoned from the deck. */
        for (i = 0; i < PLAYER(player).deckCount; i++) {
            struct DuelCard card = *(struct DuelCard *)PILE_WORD_PL_I(PLAYERS_DECK, i, player);

            if (CardType(card.id) == CARD_TYPE_INSECT && CardLevel(card.id) == param
                && IsSpecialSummonOnly(CARD_ID(*PILE_WORD_PL_I(PLAYERS_DECK, i, player))) == 0)
                ADD_TARGET(*PILE_WORD(PLAYERS_DECK, i, player), CARDLIST_SRC_DECK);
        }
        break;
    }
    case CARD_1421:
    {
        /* Graveyard monsters that were destroyed in battle (struct DuelCard.destroyedInBattle); read by
           EffectRecoverGraveMonsterResolve. */
        u32 off;
        u8 *b;
        u32 *g;

        i = 0;
        if (i < PLAYER(player).graveCount) {
            off = (player & 1) * PLAYER_STRIDE;
            b = (u8 *)gDuelPlayerView;
            g = (u32 *)(b + OFFSET_OF(struct DuelPlayerWords, graveyard));
            do {
                /* FAKEMATCH: off + g must stay in the loop ahead of the i << 2 (ROM 0x08045F6E); as a pseudo,
                   loop pass 2 hoists it (lifetime 2, 49 insns) and the read becomes a giv. Pinning it keeps it. */
                register u32 t asm("r0") = off + (u32)g;
                struct DuelCard w = ((struct DuelCardPile *)t)->cards[i];

                if (CardType(w.id) <= CARD_TYPE_REPTILE && w.destroyedInBattle)
                    ADD_TARGET(*(u32 *)((u8 *)g + (off + i * 4)), CARDLIST_SRC_GRAVEYARD);
                i++;
            } while (i < PLAYER(player).graveCount);
        }
        break;
    }
    case CARD_1439:
    {
        /* Magic cards in the graveyard that were destroyed by the opponent (destroyedByOpponent); read by
           EffectTributeRecoverGraveMagicResolve. */
        u32 off;
        u8 *b;
        u32 *g;

        i = 0;
        if (i < PLAYER(player).graveCount) {
            off = (player & 1) * PLAYER_STRIDE;
            b = (u8 *)gDuelPlayerView;
            g = (u32 *)(b + OFFSET_OF(struct DuelPlayerWords, graveyard));
            do {
                /* FAKEMATCH: off + g must stay in the loop ahead of the i << 2 (ROM 0x08046012); as a pseudo,
                   loop pass 2 hoists it (lifetime 2, 49 insns) and the read becomes a giv. Pinning it keeps it. */
                register u32 t asm("r0") = off + (u32)g;
                struct DuelCard w = ((struct DuelCardPile *)t)->cards[i];

                if (CardType(w.id) == CARD_TYPE_MAGIC && w.destroyedByOpponent)
                    ADD_TARGET(*(u32 *)((u8 *)g + (off + i * 4)), CARDLIST_SRC_GRAVEYARD);
                i++;
            } while (i < PLAYER(player).graveCount);
        }
        break;
    }
    case CARD_1511:
    {
        /* Monsters in the opponent's graveyard. */
        u32 *word;

        for (i = 0; i < PLAYER(1 - player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, 1 - player);
            if (CardType(CARD_ID(*word)) <= CARD_TYPE_REPTILE)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_1513:
    {
        /* Monsters in the graveyard; no Toon filter. */
        u32 *word;

        skipToonFilter = 1;
        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, player);
            if (CardType(CARD_ID(*word)) <= CARD_TYPE_REPTILE)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_1514:
    {
        /* Fiends in the graveyard. */
        u32 *word;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, player);
            if (CardType(CARD_ID(*word)) == CARD_TYPE_FIEND)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case 1515:  /* LIGHT */
    case 1516:  /* FIRE */
    case 1517:  /* WATER */
    case 1518:  /* EARTH */
    case 1519:  /* WIND */
    {
        /* Graveyard monsters of one attribute: the keys of the monsters that are Special Summoned by
           banishing monsters of an attribute. */
        for (i = 0; i < PLAYER(player).graveCount; i++) {
            /* ROM order: attribute = 0 after the id read. The u16 id and the u8 type/attribute values add
               flow-time extension insns (combined away later) inside attribute's live range, which keeps
               its global-alloc priority (refs 14 / live 70, doubled by the REG_EQUIV of its first set)
               below number's (81 / 681), so number keeps r4 and attribute gets r5 as in the ROM. */
            u32 stats;
            u16 id16;
            u8 attr8;

            id16 = CARD_ID(*PILE_WORD_FLAT(gDuelGraveyards, i, player));
            attr8 = 0;
            switch (cardNumber) {
            case 1515:
                attr8 = ATTRIBUTE_LIGHT;
                break;
            case 1516:
                attr8 = ATTRIBUTE_FIRE;
                break;
            case 1517:
                attr8 = ATTRIBUTE_WATER;
                break;
            case 1518:
                attr8 = ATTRIBUTE_EARTH;
                break;
            case 1519:
                attr8 = ATTRIBUTE_WIND;
                break;
            }
            stats = CARD_STATS(id16);
            if ((u8)CARD_STATS_TYPE(stats) <= CARD_TYPE_REPTILE && (u8)CARD_STATS_ATTR(stats) == attr8)
                ADD_TARGET_DOWHILE(PLAYER(player).graveyard[i], CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_1524:
    {
        /* Graveyard cards that are Polymerization materials (isFusionMaterial); read by
           EffectReturnFusionMaterialsToHandResolve. */
        for (i = 0; i < PLAYER(player).graveCount; i++) {
            if (PLAYER_BITS(player).graveyard[i].isFusionMaterial)
                ADD_TARGET(PLAYER(player).graveyard[i], CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_1532:
    {
        /* Monsters in the graveyard. */
        u32 *word;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, player);
            if (CardType(CARD_ID(*word)) <= CARD_TYPE_REPTILE)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_1546:
    {
        /* Graveyard cards that are Polymerization materials (isFusionMaterial) and also a material of the
           fusion monster param (a card ID); read by the fusion handlers of effect_fusion.c. */
        u32 *word;
        u32 id;

        for (i = 0; i < PLAYER(player).graveCount; i++) {
            word = PILE_WORD(PLAYERS_GRAVEYARD, i, player);
            id = CARD_ID(*word);
            if (IsMaterialOfFusion((u16)param, id) != 0
                && PLAYER_BITS(player).graveyard[i].isFusionMaterial)
                ADD_TARGET(*word, CARDLIST_SRC_GRAVEYARD);
        }
        break;
    }
    case CARD_1549:
    {
        /* Banished monsters that are not face down (BANISH_FACE_DOWN cards stay hidden); read by the fusion
           handlers of effect_fusion.c. */
        for (i = 0; i < PLAYER(player).banishedCount; i++) {
            u32 w = *PILE_WORD(PLAYERS_BANISHED, i, player);

            if (CardType(CARD_ID(w)) <= CARD_TYPE_REPTILE
                && (u8)PLAYER(player).banishedInfo[i] != BANISH_FACE_DOWN)
                ADD_TARGET(w, CARDLIST_SRC_BANISHED);
        }
        break;
    }
    }
    if (gCardListView.count == 0)
        return 0;

    /* Filter 1, Toon monsters: they cannot be picked unless Toon World is face up, so they are removed
       when it is not (always for Last Will). The graveyard pickers skip it. */
    toonWorldUp = HasFaceUpToonWorld(player);
    if ((forceToonFilter || toonWorldUp == 0) && !skipToonFilter) {
        for (i = 0; i < gCardListView.count;) {
            u32 *t = gCardListView.cards;

            if (IsToonMonster(CARD_NUMBER(CARD_ID(t[i]))) != 0) {
                REMOVE_CARD(i);
            } else {
                i++;
            }
        }
    }
    /* Filter 2, planted cards: card word bit 17 (Parasite Paracide shuffled into this deck) is the sign
       bit after << 14. */
    for (i = 0; i < gCardListView.count;) {
        u32 *t = gCardListView.cards;

        if ((s32)(t[i] << 14) < 0) {
            REMOVE_CARD(i);
        } else {
            i++;
        }
    }
    /* Filter 3, cards that an active Prohibition names. */
    for (i = 0; i < gCardListView.count;) {
        u32 *t = gCardListView.cards;

        if (IsCardProhibited(CARD_ID(t[i])) != 0) {
            REMOVE_CARD(i);
        } else {
            i++;
        }
    }
    return gCardListView.count;
}
