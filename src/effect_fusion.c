/*
 * effect_fusion (0x0803C838-0x0803DD7B): the Fusion rules and the Polymerization effect
 * (wiki/functions/effect-fusion-c.md).
 *
 *  - Three resolve handlers of effect keys that have no EDS card (1546, 1549, 1551): a Fusion monster is split
 *    back into its Graveyard materials, banished monsters return to the Graveyard, and Graveyard-banish summon
 *    costs may use monsters on the field.
 *  - The Fusion recipe rules: IsFusionSubstitute, the recipe checks over gFusionRecipes2 / gFusionRecipes3
 *    (struct FusionRecipe, include/effect.h), IsMaterialOfFusion, and FindFusionMaterials with its slot helpers.
 *    Polymerization's prepare handler, its target collector and its resolve handler use them.
 *  - The list of materials still to be picked (gChain.fusionMaterials) and EffectPolymerizationResolve.
 *
 * The handlers are the resolve slot (+0x04) of a gCardEffects row (struct CardEffect, include/effect.h).
 * Chain_Resolve calls them with the resolving link: its card ID, player (bit 1 = the CPU or the link
 * partner), zone, and the targets chosen by chainB (link->targets[], player | zone << 8). A negated link
 * does nothing. Multi-step handlers are step machines on gChain.effectStep: Chain_Resolve starts them at
 * EFFECT_STEP_START (0x80) and stores each return value as the next step, until the handler returns
 * EFFECT_STEP_DONE. A returned step that has no case ends the link on the next call.
 *
 * Slot words: a hand card or a monster is named by a Fusion slot, FUSION_SLOT_HAND | hand index or
 * FUSION_SLOT_FIELD | monster zone (enum FusionSlot, include/effect.h); FUSION_SLOT_NONE means "none found".
 * Card numbers 2000 and up are the alternate-art copies of card n - 2000 (CARD_NUMBER_ALT_ART).
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_ALT_ART, CARD_STATS_TYPE / CARD_STATS_KIND */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, enum CardKind */
#include "constants/duel.h"         /* enum DuelArea, MONSTER_ZONE_COUNT, ZONE_STATUS_*, PICK_* */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "duel.h"                   /* gDuel, gDuelPlayers, duel rules and helpers */

#include "ai.h"                     /* AiPickCardListEntry */
#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* ReturnFieldCardToDeck, SendFusionMaterialToGrave, BanishFieldCard */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* enum EffectStep, enum FusionSlot, struct FusionRecipe, CollectEffectTargets */
#include "effect_handlers.h"        /* the handlers defined here */
#include "summon.h"                 /* QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */

/*
 * Local views kept on purpose (matching choices, see build/readability/HEADERS.md).
 */
/* Matching: the resolve handlers call the prepare handler of their own effect with the three arguments of the
 * prepare slot (chainLink = their own chainedTo argument, fromHand = 0); the definitions in effect_handlers.h
 * take only the card, which would leave r1 and r2 unset. */
extern int EffectReturnBanishedToGravePrepare3(struct ChainEntry *card, int chainLink, int fromHand)
    asm("EffectReturnBanishedToGravePrepare");
extern int EffectPolymerizationPrepare3(struct ChainEntry *card, int chainLink, int fromHand)
    asm("EffectPolymerizationPrepare");

/* Matching: CollectEffectTargets is defined to return u16; the ROM uses the count as an int (no narrowing). */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");

/*
 * A card word as its two halfwords in memory (the operands arg2 / arg4 of the card-word duel commands).
 * Matching: members of an array element give the ROM's single address computation; indexing a u16 pointer
 * does not.
 */
struct CardWordHalves {
    u16 lo;                         /* card word bits 0-15 */
    u16 hi;                         /* card word bits 16-31 */
};

/*
 * gCardListView with its cards[] as a union of views of the card word. Matching: reaching a card through the
 * address of gCardListView itself keeps the symbol loaded once with +0xC as part of the member offset (the
 * gCardListView.cards[] member folds it into the literal and gives other code), and the Polymerization pick
 * reads the card ID through a u16 container (ldrh; lsl #20; lsr #20), as `half.id`, where a struct DuelCard
 * member read loads the whole word.
 */
struct CardListViewCards {
    u8 unk0[0xC];
    union {
        struct DuelCard card;       /* the card word */
        struct {
            u16 id:12;              /* card word bits 0-11 */
            u16 unk12:4;
            u16 hi;                 /* card word bits 16-31 */
        } half;
    } cards[0x80];                  /* +0x0C */
};
#define LIST_CARD(i)        (((struct CardListViewCards *)&gCardListView)->cards[i])

/*
 * The pending-material list of gChain (+0x502..+0x509) with the count in a u32 bitfield container.
 * Matching: IsPendingFusionMaterial reads fusionMaterialCount as `u32 count:2` (ldrb; lsl #30; lsr #30 for the
 * loop test); with the canonical u8 container the compiler folds the first test and allocates other registers.
 * RemovePendingFusionMaterial matches with gChain.fusionMaterialCount.
 */
struct PendingMaterialsView {
    u8 unk0[0x502];
    u32 count:2;                    /* +0x502 bits 0-1: gChain.fusionMaterialCount */
    u32 unk502_2:6;
    u8 unk503;
    u16 materials[3];               /* +0x504: gChain.fusionMaterials */
};
#define PENDING_MATERIALS   ((struct PendingMaterialsView *)&gChain)

/* The Fusion recipe tables (ROM; struct FusionRecipe, include/effect.h), each ending in a terminator recipe
 * whose result and materials are all 0x3E7. gFusionRecipes2 is followed directly by gFusionRecipes3. */
extern const struct FusionRecipe gFusionRecipes2[];     /* 0x0819A7C8: 52 recipes of 2 materials + terminator */
extern const struct FusionRecipe gFusionRecipes3[];     /* 0x0819A970: 3 recipes of 3 materials + terminator */
/* Index of each table's terminator: the fixed-length scans (IsMaterialOfFusion, FindFusionMaterials) visit it
 * too, which is harmless since no card has the result 0x3E7. */
#define FUSION_RECIPES2_LAST    0x34
#define FUSION_RECIPES3_LAST    3
#define FUSION_END              0x3E7       /* result and materials of a table's terminator recipe */
#define FUSION_END_WORD         0x03E703E7  /* result and materials[0] of the terminator, read as one word */

/* Prompts of this unit (ROM; the @n colour codes are left out here). */
extern const u8 gStrSummonFusionMaterialsQuestion[];        /* 0x08083AD0 "Do you wish to Special Summon a Fusion
                                                             * material monster to the Field?" */
extern const u8 gStrSelectBanishedCardToReturnToGrave[];    /* 0x08083B24 "Select from the list an out-of-play card
                                                             * that you wish to return to the Graveyard." */
extern const u8 gStrSelectFusionMonsterToSummon[];          /* 0x08083B78 "Select from the list a Fusion Monster that
                                                             * you wish to special Summon." */
extern const u8 gStrSelectTwoFusionMaterials[];             /* 0x08083BC8 "Select 2 monsters that you wish to use as
                                                             * Fusion material." */
extern const u8 gStrSelectThreeFusionMaterials[];           /* 0x08083C0C "Select 3 monsters that you wish to use as
                                                             * Fusion material." */

/* The card word of a zone or pile entry as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with shifts; a .id bitfield read would load only the halfword that holds it. */
#define CARD_WORD(card)     (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)       (((word) << 20) >> 20)

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber
 * (0x08622AB4). Matching: these give the ROM's literal pools; the symbol forms generate other code.
 */
#define CARD_STATS(id)      (((const u32 *)0x08621DE0)[CARD_ID_MASK & (id)])
#define CARD_NUMBER(id)     (((const u16 *)0x08622AB4)[CARD_ID_MASK & (id)])
#define CARD_TYPE(id)       CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */

/*
 * The card word of zone z of player p, and the address of that zone, by integer arithmetic over the player-stride
 * alias gDuelZones. Matching: ZONE_WORD and ZONE_AT_PLAYER_FIRST add the player term first, ZONE_AT the zone
 * term first, as the ROM does at each use; the array form gDuelZones[p].zones[z] gives other code in all three.
 * p must be 0 or 1.
 */
#define ZONE_WORD(p, z) \
    (*(u32 *)((p) * sizeof(struct DuelPlayer) + (z) * sizeof(struct DuelZone) + (u32)gDuelZones))
#define ZONE_AT(p, z) \
    ((struct DuelZone *)((z) * sizeof(struct DuelZone) + (p) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
#define ZONE_AT_PLAYER_FIRST(p, z) \
    ((struct DuelZone *)((p) * sizeof(struct DuelPlayer) + (z) * sizeof(struct DuelZone) + (u32)gDuelZones))

/* The number of targets of a chain entry (+0x0A bits 0-2) read as a byte. Matching: EffectSplitFusionResolve
 * keeps it as ldrb; and #7 in a variable; the ChainEntry.numTargets bitfield gives lsl/lsr. */
#define LINK_NUM_TARGETS_BYTE(link) (7 & ((u8 *)(link))[0xA])
/* The command id for the link's player (player 1's commands carry DUEL_CMD_PLAYER). */
#define CMD_FOR(link, cmd)          ((link)->player ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* Text box placement (TextBoxOpen pos = x | y << 8, size = width | height << 8, both in cells). */
#define BOX_POS(x, y)       ((x) | ((y) << 8))
#define BOX_SIZE(w, h)      ((w) | ((h) << 8))

/* The index of a Fusion slot word (hand index or monster zone) as GetFusionSlotCardId reads it. */
#define FUSION_SLOT_INDEX_MASK  0xFFF

/* Steps of EffectSplitFusionResolve. */
enum SplitFusionStep {
    SPLIT_STEP_START = EFFECT_STEP_START,   /* 0x80: conditions; return the Fusion monster to the Fusion Deck */
    SPLIT_STEP_ASK = 0x7F,                  /* open the Yes/No box */
    SPLIT_STEP_ANSWER = 0x7E,               /* No ends the effect; Yes collects the candidates */
    SPLIT_STEP_REMOVE = 0x7D,               /* take the next candidate out of the Graveyard */
    SPLIT_STEP_SUMMON = 0x7C                /* Special Summon it (the position is asked) */
};

/*
 * Key 1546 (hypothesis: OCG De-Fusion): return the targeted face-up Fusion monster to the Fusion Deck and
 * remember its card ID (gChain.scratch.effect.savedValue). If a human player then has at least as many free
 * monster zones as Graveyard cards that can be its Fusion materials, offer (Yes/No) to Special Summon those
 * cards from the Graveyard, one by one. For the CPU the effect ends after the return.
 *
 * Needs exactly one target that EffectFaceUpFusionMonsterCheck accepts. The candidates are the Graveyard cards
 * with their fusion-material mark (card word bit 20) that IsMaterialOfFusion accepts for the returned monster;
 * CollectEffectTargets(1546) collects them into gCardListView.cards[], and gChain.effectSubStep counts them
 * down (last entry first) while they are summoned.
 */
int EffectSplitFusionResolve(struct ChainEntry *link)
{
    int targetPlayer = DUEL_LOC_PLAYER(link->targets[0]);
    int targetZone = DUEL_LOC_ZONE(link->targets[0]);

    if (!link->negated) {
        switch (gChain.effectStep) {
        case SPLIT_STEP_START: {
            int numTargets = LINK_NUM_TARGETS_BYTE(link);
            int freeZones, candidates;

            if (numTargets != 1)
                return EFFECT_STEP_DONE;
            if (EffectFaceUpFusionMonsterCheck(link, link->targets[0]) == 0)
                return EFFECT_STEP_DONE;
            ReturnFieldCardToDeck(targetPlayer, targetZone);
            /* ReturnFieldCardToDeck only queues its command, so the zone still holds the card here. */
            gChain.scratch.effect.savedValue =
                CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(targetPlayer & 1, targetZone)->card));
            freeZones = CountFreeMonsterZones(link->player);
            candidates = CollectEffectTargetsInt(link->player, CARD_1546, gChain.scratch.effect.savedValue);
            if (candidates == 0)
                return EFFECT_STEP_DONE;
            if (freeZones < candidates)
                return EFFECT_STEP_DONE;
            if (link->player)               /* the CPU gets no offer */
                return EFFECT_STEP_DONE;
            return SPLIT_STEP_ASK;
        }
        case SPLIT_STEP_ASK:
            TextBoxOpen(BOX_POS(6, 2), BOX_SIZE(19, 6), TEXTBOX_FLAGS_DEFAULT, gStrSummonFusionMaterialsQuestion);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
            return SPLIT_STEP_ANSWER;
        case SPLIT_STEP_ANSWER:
            if (gTextBox.result == 0)       /* No */
                return EFFECT_STEP_DONE;
            gChain.effectSubStep = CollectEffectTargetsInt(link->player, CARD_1546, gChain.scratch.effect.savedValue);
            return SPLIT_STEP_REMOVE;
        case SPLIT_STEP_REMOVE:
            if (gChain.effectSubStep == 0)
                return EFFECT_STEP_DONE;
            gChain.effectSubStep--;
            /* Take the candidate out of the Graveyard (the command takes the card word as two halves). */
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD),
                         ((struct CardWordHalves *)gCardListViewCards)[gChain.effectSubStep].lo,
                         ((struct CardWordHalves *)gCardListViewCards)[gChain.effectSubStep].hi, 0);
            return SPLIT_STEP_SUMMON;
        case SPLIT_STEP_SUMMON:
            /* The list entry is the card word to summon: clear its fusion-material mark first. */
            LIST_CARD(gChain.effectSubStep).card.isFusionMaterial = 0;
            QueueSpecialSummonChoosePosition(link->player, &LIST_CARD(gChain.effectSubStep).card, 1,
                                             ZONE_STATUS_FROM_GRAVEYARD);
            return SPLIT_STEP_REMOVE;
        }
    }
    return EFFECT_STEP_DONE;
}

/* Steps of EffectReturnBanishedToGraveResolve. */
enum ReturnBanishedStep {
    RETURN_BANISHED_STEP_START = EFFECT_STEP_START, /* 0x80: re-check the activation condition; 3 picks */
    RETURN_BANISHED_STEP_PROMPT = 0x7F,             /* nothing eligible left ends the effect; show the prompt */
    RETURN_BANISHED_STEP_LIST = 0x7E,               /* open the list viewer */
    RETURN_BANISHED_STEP_RETURN = 0x7D,             /* send the picked card to the Graveyard */
    RETURN_BANISHED_STEP_END = 0x0A                 /* no case: ends on the next call */
};

/*
 * Key 1549: return up to three banished monsters to the Graveyard, one at a time. Each round checks that an
 * eligible banished monster is left (CollectEffectTargets(1549)), shows a prompt, opens the list viewer on the
 * link card's target list (CardListView_Open collects it itself) and sends the picked entry to the Graveyard.
 * gChain.effectSubStep holds the picks left. chainedTo is the link this effect answers; it is passed on to
 * the prepare handler, which re-checks the activation condition on the first call.
 */
int EffectReturnBanishedToGraveResolve(struct ChainEntry *link, int chainedTo)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case RETURN_BANISHED_STEP_START:
            if (EffectReturnBanishedToGravePrepare3(link, chainedTo, 0) == 0)
                return EFFECT_STEP_DONE;
            gChain.effectSubStep = 3;
            gChain.effectStep--;
            /* the stored step is now RETURN_BANISHED_STEP_PROMPT; fall through into it in this call */
        case RETURN_BANISHED_STEP_PROMPT:
            if (CollectEffectTargetsInt(link->player, CARD_1549, 0) == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(BOX_POS(6, 2), BOX_SIZE(19, 6), TEXTBOX_FLAGS_DEFAULT, gStrSelectBanishedCardToReturnToGrave);
            return RETURN_BANISHED_STEP_LIST;
        case RETURN_BANISHED_STEP_LIST:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return RETURN_BANISHED_STEP_RETURN;
        case RETURN_BANISHED_STEP_RETURN: {
            /* The picked entry of the viewer, as the two halves of its card word. */
            u16 *pickedCard = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow];

            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_RETURN_BANISHED_CARD_TO_GRAVEYARD), pickedCard[0], pickedCard[1], 0);
            gChain.effectSubStep--;
            if (gChain.effectSubStep != 0)
                return RETURN_BANISHED_STEP_PROMPT;
            return RETURN_BANISHED_STEP_END;
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1551 (the card behind it is unknown): set DuelPlayer.banishCostFromField for the player and return 0.
 * The flag is cleared at the turn change. (Hypothesis, from the code that reads the flag in other units: while
 * it is set, the Special Summon costs of keys 1514-1519, "remove N monsters in your Graveyard from play",
 * take the monsters from the player's own field instead.)
 */
int EffectBanishCostFromFieldResolve(struct ChainEntry *link)
{
    gDuelPlayers[link->player].banishCostFromField = 1;
    return 0;
}

/*
 * 1 if the card number is one of the four "you can substitute this card for any 1 Fusion-Material Monster"
 * cards: Goddess with the Third Eye, Beastking of the Swamps, Versago the Destroyer, Mystical Sheep #1.
 * At most one substitute is allowed per Fusion.
 */
u16 IsFusionSubstitute(u16 cardNumber)
{
    switch (cardNumber) {
    case CARD_GODDESS_WITH_THE_THIRD_EYE:
    case CARD_BEASTKING_OF_THE_SWAMPS:
    case CARD_VERSAGO_THE_DESTROYER:
    case CARD_MYSTICAL_SHEEP_1:
        return 1;
    default:
        return 0;
    }
}

/*
 * 1 if matA and matB (either order; one of them may be a substitute, not both) are the materials of a
 * gFusionRecipes2 recipe whose result is resultId. Compares the raw card numbers (no alternate-art
 * normalisation) and scans up to the terminator. Unused: only CheckFusionRecipe calls it.
 */
int CheckFusionRecipe2(int resultId, int matA, int matB)
{
    const struct FusionRecipe *recipe = gFusionRecipes2;
    u16 resultNum = CARD_NUMBER(resultId);
    u16 numA = CARD_NUMBER(matA);
    u16 numB = CARD_NUMBER(matB);

    if (IsFusionSubstitute(numA) != 0 && IsFusionSubstitute(numB) != 0)
        return 0;
    /* Matching: a goto loop; for (;;) hoists the terminator constants out of the loop, the ROM reloads them. */
again:
    if (*(u32 *)recipe == FUSION_END_WORD && recipe->materials[1] == FUSION_END)
        return 0;
    if (recipe->result == resultNum) {
        if (numA == recipe->materials[0] && numB == recipe->materials[1])
            return 1;
        if (numA == recipe->materials[1] && numB == recipe->materials[0])
            return 1;
        if (IsFusionSubstitute(numA) != 0 && (numB == recipe->materials[0] || numB == recipe->materials[1]))
            return 1;
        if (IsFusionSubstitute(numB) != 0 && (numA == recipe->materials[0] || numA == recipe->materials[1]))
            return 1;
    }
    recipe++;
    goto again;
}

/*
 * 1 if matA, matB and matC (any order, at most one substitute) are the materials of a gFusionRecipes3 recipe
 * whose result is resultId. Raw card numbers, as CheckFusionRecipe2. Once a recipe has the right result and
 * matA is one of its materials (or a substitute) the answer is final: later recipes are not tried (the three
 * results are distinct, so it makes no difference). Unused: only CheckFusionRecipe calls it.
 */
int CheckFusionRecipe3(u16 resultId, u16 matA, u16 matB, u16 matC)
{
    const struct FusionRecipe *recipe = gFusionRecipes3;
    u16 resultNum = CARD_NUMBER(resultId);
    u16 numA = CARD_NUMBER(matA);
    u16 numB = CARD_NUMBER(matB);
    u16 numC = CARD_NUMBER(matC);

    if (IsFusionSubstitute(numA) != 0 && IsFusionSubstitute(numB) != 0)
        return 0;
    if (IsFusionSubstitute(numA) != 0 && IsFusionSubstitute(numC) != 0)
        return 0;
    if (IsFusionSubstitute(numB) != 0 && IsFusionSubstitute(numC) != 0)
        return 0;
    /* Matching: a goto loop, as in CheckFusionRecipe2. */
again:
    if (*(u32 *)recipe == FUSION_END_WORD && recipe->materials[1] == FUSION_END)
        return 0;
    if (recipe->result == resultNum) {
        /* matA is the recipe's first, second or third material: matB and matC must be the other two (either
         * order), or a substitute may stand in for one of them. */
        if (numA == recipe->materials[0]) {
            if (numB == recipe->materials[1] && numC == recipe->materials[2])
                return 1;
            if (numB == recipe->materials[2] && numC == recipe->materials[1])
                return 1;
            if (IsFusionSubstitute(numB) != 0 && (numC == recipe->materials[1] || numC == recipe->materials[2]))
                return 1;
            if (IsFusionSubstitute(numC) != 0 && (numB == recipe->materials[1] || numB == recipe->materials[2]))
                return 1;
            return 0;
        }
        if (numA == recipe->materials[1]) {
            if (numB == recipe->materials[0] && numC == recipe->materials[2])
                return 1;
            if (numB == recipe->materials[2] && numC == recipe->materials[0])
                return 1;
            if (IsFusionSubstitute(numB) != 0 && (numC == recipe->materials[0] || numC == recipe->materials[2]))
                return 1;
            if (IsFusionSubstitute(numC) != 0 && (numB == recipe->materials[0] || numB == recipe->materials[2]))
                return 1;
            return 0;
        }
        if (numA == recipe->materials[2]) {
            if (numB == recipe->materials[0] && numC == recipe->materials[1])
                return 1;
            if (numB == recipe->materials[1] && numC == recipe->materials[0])
                return 1;
            if (IsFusionSubstitute(numB) != 0 && (numC == recipe->materials[0] || numC == recipe->materials[1]))
                return 1;
            if (IsFusionSubstitute(numC) != 0 && (numB == recipe->materials[0] || numB == recipe->materials[1]))
                return 1;
            return 0;
        }
        /* matA is a substitute for one material: matB and matC must be two of the recipe's materials. */
        if (IsFusionSubstitute(numA) != 0) {
            if (numB == recipe->materials[0] && (numC == recipe->materials[1] || numC == recipe->materials[2]))
                return 1;
            if (numB == recipe->materials[1] && numC == recipe->materials[2])
                return 1;
            return 0;
        }
    }
    recipe++;
    goto again;
}

/*
 * 1 if the card materialId can be a material of the Fusion monster fusionId: its card number (alternate art
 * counted as the original) is a material of a gFusionRecipes2 or gFusionRecipes3 recipe whose result is the
 * Fusion monster's number, or it is a Fusion substitute. Used by the target collector of key 1546.
 */
int IsMaterialOfFusion(int fusionId, int materialId)
{
    u16 fusionNum = CARD_NUMBER(fusionId);
    u16 materialNum = CARD_NUMBER(materialId);
    u32 i;
    const struct FusionRecipe *recipe;

    if (fusionNum >= CARD_NUMBER_ALT_ART)
        fusionNum -= CARD_NUMBER_ALT_ART;
    if (materialNum >= CARD_NUMBER_ALT_ART)
        materialNum -= CARD_NUMBER_ALT_ART;
    for (i = 0, recipe = gFusionRecipes2; i <= FUSION_RECIPES2_LAST; recipe++, i++) {
        if (fusionNum == recipe->result
            && (recipe->materials[0] == materialNum || recipe->materials[1] == materialNum))
            return 1;
    }
    for (i = 0, recipe = gFusionRecipes3; i <= FUSION_RECIPES3_LAST; recipe++, i++) {
        if (fusionNum == recipe->result
            && (recipe->materials[0] == materialNum || recipe->materials[1] == materialNum
                || recipe->materials[2] == materialNum))
            return 1;
    }
    if (IsFusionSubstitute(materialNum) != 0)
        return 1;
    return 0;
}

/*
 * The Fusion slot (enum FusionSlot) of a card with the given card number (or its alternate-art number) that
 * is not excludeA or excludeB: the player's monster zones 0-4 first, then the hand; if there is none, the
 * same two scans accept any Fusion substitute instead. FUSION_SLOT_NONE if nothing is left. Unlike the other
 * three scans, the exact-number hand scan does not skip empty entries (as in the ROM).
 */
u16 FindFusionMaterialSlot(int player, u16 cardNumber, u16 excludeA, u16 excludeB)
{
    int i;
    u16 id;     /* Matching: one variable for all four scans; a local per loop is allocated to r0/r1, not r2 */

    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        struct DuelZone *zone = ZONE_AT(player & 1, i);
        id = CARD_ID(CARD_WORD(zone->card));

        if (id != 0) {
            if (CARD_NUMBER(id) == cardNumber || CARD_NUMBER(id) == cardNumber + CARD_NUMBER_ALT_ART) {
                u16 slot = i + FUSION_SLOT_FIELD;

                if (excludeA != slot && excludeB != slot)
                    return slot;
            }
        }
    }
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));

        if (CARD_NUMBER(id) == cardNumber || CARD_NUMBER(id) == cardNumber + CARD_NUMBER_ALT_ART) {
            u16 slot = i + FUSION_SLOT_HAND;

            if (excludeA != slot && excludeB != slot)
                return slot;
        }
    }
    for (i = 0; i < MONSTER_ZONE_COUNT; i++) {
        struct DuelZone *zone = ZONE_AT(player & 1, i);
        id = CARD_ID(CARD_WORD(zone->card));

        if (id != 0 && IsFusionSubstitute(CARD_NUMBER(id)) != 0) {
            u16 slot = i + FUSION_SLOT_FIELD;

            if (excludeA != slot && excludeB != slot)
                return slot;
        }
    }
    for (i = 0; i < gDuelPlayers[player & 1].handCount; i++) {
        id = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].hand[i]));

        if (id != 0 && IsFusionSubstitute(CARD_NUMBER(id)) != 0) {
            u16 slot = i + FUSION_SLOT_HAND;

            if (excludeA != slot && excludeB != slot)
                return slot;
        }
    }
    return FUSION_SLOT_NONE;
}

/*
 * The card ID in a Fusion slot of the player: FUSION_SLOT_HAND | n is hand card n, FUSION_SLOT_FIELD | n is
 * monster zone n; 0 for any other value.
 */
u16 GetFusionSlotCardId(int player, u16 slot)
{
    if (slot & FUSION_SLOT_HAND) {
        int side = 1 & player;
        /* Matching: the offset is a u16 (lsl; lsr #16), as in the ROM. */
        u16 offset = (slot & FUSION_SLOT_INDEX_MASK) * sizeof(struct DuelCard) + side * sizeof(struct DuelPlayer);
        return CARD_ID(*(u32 *)(offset + (u32)gDuelHands));
    } else if (slot & FUSION_SLOT_FIELD) {
        int side = 1 & player;
        u32 offset = (slot & FUSION_SLOT_INDEX_MASK) * sizeof(struct DuelZone) + side * sizeof(struct DuelPlayer);
        return CARD_ID(*(u32 *)(offset + (u32)gDuelZones));
    }
    return 0;
}

/*
 * Dead code (no callers): CheckFusionRecipe3 when matC is non-zero, else CheckFusionRecipe2, as a boolean.
 * The game checks Fusions with FindFusionMaterials and IsMaterialOfFusion instead.
 */
int CheckFusionRecipe(u16 resultId, u16 matA, u16 matB, u16 matC)
{
    int result;

    if (matC != 0)
        result = CheckFusionRecipe3(resultId, matA, matB, matC);
    else
        result = CheckFusionRecipe2(resultId, matA, matB);
    if ((u16)result == 0)
        return 0;
    return 1;
}

/*
 * The card kind (enum CardKind) of a card ID: the Egyptian God cards give RITUAL (Obelisk) and EFFECT (Slifer,
 * Ra), Trap, Magic and Ticket cards give CARD_KIND_TRAP / _MAGIC / _TICKET, any other card its stats kind
 * (normal, effect, fusion or ritual monster). Matching: a switch on the number, not a range test.
 */
static inline int GetCardKind(u16 cardId)
{
    switch (CARD_NUMBER(cardId)) {
    case CARD_OBELISK_THE_TORMENTOR:
        return CARD_KIND_RITUAL;
    case CARD_SLIFER_THE_SKY_DRAGON:
    case CARD_THE_WINGED_DRAGON_OF_RA:
        return CARD_KIND_EFFECT;
    }
    switch ((int)CARD_TYPE(cardId)) {
    case CARD_TYPE_MAGIC:
        return CARD_KIND_MAGIC;
    case CARD_TYPE_TRAP:
        return CARD_KIND_TRAP;
    case CARD_TYPE_TICKET:
        return CARD_KIND_TICKET;
    }
    return CARD_STATS_KIND(CARD_STATS(cardId));
}

/* 1 if the Fusion slot holds a Fusion substitute. */
#define SLOT_IS_SUBSTITUTE(slot) IsFusionSubstitute(CARD_NUMBER(GetFusionSlotCardId(player, (slot))))

/*
 * 1 if the player holds the materials of the Fusion monster fusionId. Looks its recipe up (card number
 * normalised for alternate art) in gFusionRecipes2, then gFusionRecipes3, and fills slots[] with the Fusion
 * slots of the materials, the last material first (slots[0] is the last one of the recipe), each pick
 * excluding the earlier ones. Fails if a material is missing, if two picks are substitutes, or if every pick
 * is in the hand while all five monster zones are occupied (nowhere to put the Fusion monster).
 * Used by EffectPolymerizationPrepare, EffectPolymerizationResolve and the target collector.
 */
int FindFusionMaterials(int player, u16 fusionId, u16 *slots)
{
    const struct FusionRecipe *recipe;
    u16 fusionNum;
    u32 i;

    if (CARD_TYPE(fusionId) > CARD_TYPE_REPTILE)    /* not a monster */
        return 0;
    if (GetCardKind(fusionId) != CARD_KIND_FUSION)
        return 0;
    fusionNum = CARD_NUMBER(fusionId);
    if (fusionNum >= CARD_NUMBER_ALT_ART)
        fusionNum -= CARD_NUMBER_ALT_ART;

    for (i = 0, recipe = gFusionRecipes2; i <= FUSION_RECIPES2_LAST; recipe++, i++) {
        if (fusionNum == recipe->result) {
            u16 mat0 = recipe->materials[0];
            u16 mat1 = recipe->materials[1];
            slots[1] = FindFusionMaterialSlot(player, mat0, FUSION_SLOT_NONE, FUSION_SLOT_NONE);
            slots[0] = FindFusionMaterialSlot(player, mat1, slots[1], FUSION_SLOT_NONE);
            if (slots[0] == FUSION_SLOT_NONE)
                return 0;
            if (slots[1] == FUSION_SLOT_NONE)
                return 0;
            if (SLOT_IS_SUBSTITUTE(slots[0]) != 0 && SLOT_IS_SUBSTITUTE(slots[1]) != 0)
                return 0;
            if ((slots[0] & FUSION_SLOT_HAND) && (slots[1] & FUSION_SLOT_HAND)) {
                if (CountMonsters(player) == MONSTER_ZONE_COUNT)
                    return 0;
            }
            return 1;
        }
    }
    for (i = 0, recipe = gFusionRecipes3; i <= FUSION_RECIPES3_LAST; recipe++, i++) {
        if (fusionNum == recipe->result) {
            u16 mat0 = recipe->materials[0];
            u16 mat1 = recipe->materials[1];
            u16 mat2 = recipe->materials[2];
            slots[2] = FindFusionMaterialSlot(player, mat0, FUSION_SLOT_NONE, FUSION_SLOT_NONE);
            slots[1] = FindFusionMaterialSlot(player, mat1, slots[2], FUSION_SLOT_NONE);
            slots[0] = FindFusionMaterialSlot(player, mat2, slots[2], slots[1]);
            if (slots[0] == FUSION_SLOT_NONE)
                return 0;
            if (slots[1] == FUSION_SLOT_NONE)
                return 0;
            if (slots[2] == FUSION_SLOT_NONE)
                return 0;
            if (SLOT_IS_SUBSTITUTE(slots[0]) != 0 && SLOT_IS_SUBSTITUTE(slots[1]) != 0)
                return 0;
            if (SLOT_IS_SUBSTITUTE(slots[0]) != 0 && SLOT_IS_SUBSTITUTE(slots[2]) != 0)
                return 0;
            if (SLOT_IS_SUBSTITUTE(slots[1]) != 0 && SLOT_IS_SUBSTITUTE(slots[2]) != 0)
                return 0;
            if ((slots[0] & FUSION_SLOT_HAND) && (slots[1] & FUSION_SLOT_HAND) && (slots[2] & FUSION_SLOT_HAND)) {
                if (CountMonsters(player) == MONSTER_ZONE_COUNT)
                    return 0;
            }
            return 1;
        }
    }
    return 0;
}
#undef SLOT_IS_SUBSTITUTE

/*
 * gChain.fusionMaterials[0 .. fusionMaterialCount - 1] are the card IDs of the materials that the player still
 * has to pick for Polymerization; a picked one is cleared to 0 (EffectPolymerizationResolve fills the list).
 */

/* 1 if the card's number (alternate art counted as the original) is one of the materials still to pick. */
int IsPendingFusionMaterial(u16 cardId)
{
    int i;

    for (i = 0; i < PENDING_MATERIALS->count; i++) {
        if (PENDING_MATERIALS->materials[i] != 0) {
            int pendingNum = CARD_NUMBER(PENDING_MATERIALS->materials[i]);
            int cardNum = CARD_NUMBER(cardId);

            if (cardNum >= CARD_NUMBER_ALT_ART)
                cardNum -= CARD_NUMBER_ALT_ART;
            if (pendingNum >= CARD_NUMBER_ALT_ART)
                pendingNum -= CARD_NUMBER_ALT_ART;
            if (pendingNum == cardNum)
                return 1;
        }
    }
    return 0;
}

/*
 * Mark the card as picked: clear the first pending material with the same card number. If there is none and
 * the card is a Fusion substitute, clear the first pending material that is itself a substitute.
 */
void RemovePendingFusionMaterial(u16 cardId)
{
    int i;

    for (i = 0; i < gChain.fusionMaterialCount; i++) {
        if (gChain.fusionMaterials[i] != 0) {
            int pendingNum = CARD_NUMBER(gChain.fusionMaterials[i]);
            int cardNum = CARD_NUMBER(cardId);

            if (cardNum >= CARD_NUMBER_ALT_ART)
                cardNum -= CARD_NUMBER_ALT_ART;
            if (pendingNum >= CARD_NUMBER_ALT_ART)
                pendingNum -= CARD_NUMBER_ALT_ART;
            if (pendingNum == cardNum) {
                gChain.fusionMaterials[i] = 0;
                return;
            }
        }
    }
    if (IsFusionSubstitute(CARD_NUMBER(cardId)) == 0)
        return;
    for (i = 0; i < gChain.fusionMaterialCount; i++) {
        if (gChain.fusionMaterials[i] != 0 && IsFusionSubstitute(CARD_NUMBER(gChain.fusionMaterials[i])) != 0) {
            gChain.fusionMaterials[i] = 0;
            return;
        }
    }
}

/* Steps of EffectPolymerizationResolve. */
enum PolymerizationStep {
    POLY_STEP_START = EFFECT_STEP_START,    /* 0x80: re-check the activation condition; the CPU takes its pick, the
                                             * human gets the prompt */
    POLY_STEP_OPEN_LIST = 0x7F,             /* open the list of Fusion Deck monsters that can be fused now */
    POLY_STEP_PICKED = 0x7E,                /* read the pick, find its materials, prompt for them */
    POLY_STEP_PICK_MATERIAL = 0x7D,         /* human: cursor pick of the next material */
    POLY_STEP_CPU_SEND = EFFECT_STEP_END,   /* 0x64: CPU: send the next material away */
    POLY_STEP_COMPACT_HAND = 0x63,          /* close the gaps in the hand; take the Fusion monster out of its deck */
    POLY_STEP_SUMMON = 0x62,                /* Special Summon the Fusion monster (the position is asked) */
    POLY_STEP_END = 0x61                    /* no case: ends on the next call */
};

/*
 * Polymerization (keys 1003 and 1034): Special Summon a Fusion monster from the Fusion Deck by sending the
 * materials, from the hand and the player's monster zones, away. The card-menu Fusion command runs it too, with
 * the card of key 1547 (a Field Magic effect; hypothesis: OCG Fusion Gate): then the materials are banished
 * instead of sent to the Graveyard. chainedTo is the link this effect answers; it is passed on to the prepare
 * handler, which re-checks the activation condition on the first call.
 *
 * Human: pick the Fusion monster in the list viewer, then pick each material with the cursor on the hand and
 * monster row; a pick must be a pending material, or a substitute while none has been used yet.
 * CPU: AiPickCardListEntry picks the Fusion monster and the materials that FindFusionMaterials found are sent
 * without asking.
 *
 * State, in gChain (include/chain.h): fusionResult (the picked Fusion monster), fusionMaterialCount (2 or 3),
 * fusionPicksLeft and fusionSubstitutesUsed (the human's picks), fusionMaterials[] (card IDs still to pick)
 * and fusionMaterialSlots[] (where FindFusionMaterials found each material).
 */
int EffectPolymerizationResolve(struct ChainEntry *link, int chainedTo)
{
    if (link->negated)
        return EFFECT_STEP_DONE;
    switch (gChain.effectStep) {
    case POLY_STEP_START:
        if (EffectPolymerizationPrepare3(link, chainedTo, 0) == 0)
            return EFFECT_STEP_DONE;
        if (link->player) {
            int pick = AiPickCardListEntry(link->card);

            if (pick < 0)
                return EFFECT_STEP_DONE;
            gCardListView.cursorRow = 0;
            gCardListView.top = pick;
            return POLY_STEP_PICKED;
        }
        TextBoxOpen(BOX_POS(6, 2), BOX_SIZE(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrSelectFusionMonsterToSummon);
        return POLY_STEP_OPEN_LIST;
    case POLY_STEP_OPEN_LIST:
        CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
        return POLY_STEP_PICKED;
    case POLY_STEP_PICKED:
        gChain.fusionResult = LIST_CARD(gCardListView.top + gCardListView.cursorRow).half.id;
        if ((u16)FindFusionMaterials(link->player, gChain.fusionResult, gChain.fusionMaterialSlots) == 0)
            return EFFECT_STEP_DONE;
        /* The material count follows from the Fusion monster's card number: the 52 results of gFusionRecipes2
         * (plus the alternate-art numbers 2014 and 2068 of Flame Swordsman and Thousand Dragon) need 2
         * materials, the 3 results of gFusionRecipes3 need 3. */
        switch (CARD_NUMBER(gChain.fusionResult)) {
        case CARD_FLAME_SWORDSMAN: case CARD_ZOMBIE_WARRIOR: case CARD_GAIA_THE_DRAGON_CHAMPION:
        case CARD_KARBONALA_WARRIOR: case CARD_THOUSAND_DRAGON: case CARD_BAROX:
        case CARD_RABID_HORSEMAN: case CARD_KAMIONWIZARD: case CARD_CHARUBIN_THE_FIRE_KNIGHT:
        case CARD_DARKFIRE_DRAGON: case CARD_FUSIONIST: case CARD_FLAME_GHOST:
        case CARD_B_SKULL_DRAGON: case CARD_ROARING_OCEAN_SNAKE: case CARD_RARE_FISH:
        case CARD_MAVELUS: case CARD_DRAGONESS_THE_WICKED_KNIGHT: case CARD_LABYRINTH_TANK:
        case CARD_BICKURIBOX: case CARD_GILTIA_THE_D_KNIGHT: case CARD_METAL_DRAGON:
        case CARD_KAISER_DRAGON: case CARD_DEEPSEA_SHARK: case CARD_KAMINARI_ATTACK:
        case CARD_PUNISHED_EAGLE: case CARD_CRIMSON_SUNBIRD: case CARD_SOUL_HUNTER:
        case CARD_VERMILLION_SPARROW: case CARD_PRAGTICAL: case CARD_FLOWER_WOLF:
        case CARD_MUSICIAN_KING: case CARD_CYBER_SAURUS: case CARD_BRACCHIO_RAIDUS:
        case CARD_SKULLBIRD: case CARD_MYSTICAL_SAND: case CARD_KWAGAR_HERCULES:
        case CARD_SKELGON: case CARD_GREAT_MAMMOTH_OF_GOLDFINE: case CARD_EMPRESS_JUDGE:
        case CARD_ROSE_SPECTRE_OF_DUNN: case CARD_TWIN_HEADED_THUNDER_DRAGON: case CARD_MARINE_BEAST:
        case CARD_WARRIOR_OF_TRADITION: case CARD_AMPHIBIOUS_BUGROTH: case CARD_SKULL_KNIGHT:
        case CARD_METEOR_B_DRAGON: case CARD_ALLIGATORS_SWORD_DRAGON:
        case CARD_1241: case CARD_1334:     /* effect keys, no EDS card; 1404, 1445 and 1526 have no constant */
        case 1404: case 1445: case 1526:
        case CARD_FLAME_SWORDSMAN_ALT: case CARD_THOUSAND_DRAGON_ALT:
            gChain.fusionMaterialCount = 2;
            break;
        case CARD_BLUE_EYES_ULTIMATE_DRAGON: case CARD_AQUA_DRAGON: case CARD_MAN_EATING_BLACK_SHARK:
            gChain.fusionMaterialCount = 3;
            break;
        }
        if (link->player)
            return POLY_STEP_CPU_SEND;
        switch (gChain.fusionMaterialCount) {
        case 2:
            TextBoxOpen(BOX_POS(6, 2), BOX_SIZE(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrSelectTwoFusionMaterials);
            break;
        case 3:
            TextBoxOpen(BOX_POS(6, 2), BOX_SIZE(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrSelectThreeFusionMaterials);
            break;
        }
        gChain.fusionSubstitutesUsed = 0;
        gChain.fusionPicksLeft = gChain.fusionMaterialCount;
        {
            /* Record the card ID of each material (the hand card or monster named by its slot): the picks are
             * checked against these. The slot index is in the low byte. */
            int i;
            for (i = 0; i < gChain.fusionMaterialCount; i++) {
                u16 cardId = 0;
                /* FAKEMATCH: the do-while(0) puts the hand read one loop level deeper, which
                 * weights the hoisted hand base's refs so it wins r9 as in the ROM */
                if (gChain.fusionMaterialSlots[i] & FUSION_SLOT_HAND)
                    do {
                        cardId = CARD_ID(CARD_WORD(
                            gDuelPlayers[1 & link->player].hand[gChain.fusionMaterialSlots[i] & 0xFF]));
                    } while (0);
                if (gChain.fusionMaterialSlots[i] & FUSION_SLOT_FIELD)
                    cardId = CARD_ID(ZONE_WORD(1 & link->player, gChain.fusionMaterialSlots[i] & 0xFF));
                gChain.fusionMaterials[i] = cardId;
            }
        }
        gChain.fusionPicksLeft--;           /* picks left after the first one */
        return POLY_STEP_PICK_MATERIAL;
    case POLY_STEP_PICK_MATERIAL: {
        u16 confirmed = DuelCursor_PickTarget(PICK_HAND | PICK_ANY_MONSTER);   /* 1 once A is pressed */
        u16 pickedId;
        int valid;
        struct DuelScreen *screen;

        /* The card under the cursor: a hand card or a monster. */
        switch (gDuelScreen.selArea) {
        case DUEL_AREA_HAND:
            pickedId = CARD_ID(CARD_WORD(gDuelPlayers[link->player].hand[gDuelScreen.selIndex]));
            break;
        case DUEL_AREA_MONSTER:
            pickedId = CARD_ID(ZONE_WORD(link->player, gDuelScreen.selIndex));
            break;
        default:
            pickedId = 0;
            break;
        }
        /* It must be a material still to pick, or a substitute while none has been used yet. Matching: the
         * temporary and the two-step assignment of valid give the ROM's branches. */
        valid = 0;
        if (pickedId != 0) {
            valid = (u16)IsPendingFusionMaterial(pickedId) != 0;
            if (IsFusionSubstitute(CARD_NUMBER(pickedId)) != 0) {
                int substitutesUsed = gChain.fusionSubstitutesUsed;
                valid = 0;
                if (substitutesUsed == 0)
                    valid = 1;
            }
        }
        if (valid == 0 || confirmed == 0)
            return POLY_STEP_PICK_MATERIAL;
        RemovePendingFusionMaterial(pickedId);
        if (IsFusionSubstitute(CARD_NUMBER(pickedId)) != 0)
            gChain.fusionSubstitutesUsed++;
        /* Send the material away: banished for key 1547, else to the Graveyard. Matching: the second reads of
         * gDuelScreen go through a pointer. */
        screen = &gDuelScreen;
        switch (screen->selArea) {
        case DUEL_AREA_HAND:
            DuelCmd_Push(CARD_NUMBER(link->card) != CARD_1547
                             ? CMD_FOR(link, DUEL_CMD_SEND_HAND_FUSION_MATERIAL_TO_GRAVEYARD)
                             : CMD_FOR(link, DUEL_CMD_BANISH_HAND_FUSION_MATERIAL),
                         screen->selIndex, 0, 0);
            break;
        case DUEL_AREA_MONSTER:
            if (CARD_NUMBER(link->card) != CARD_1547)
                SendFusionMaterialToGrave(link->player, screen->selIndex);
            else
                BanishFieldCard(link->player, screen->selIndex, 1);
            break;
        }
        {
            /* Matching: the count is read into a temporary and written back as count - 1. */
            int picksLeft = gChain.fusionPicksLeft;
            if (picksLeft != 0) {
                gChain.fusionPicksLeft = picksLeft - 1;
                return POLY_STEP_PICK_MATERIAL;
            }
        }
        return POLY_STEP_COMPACT_HAND;
    }
    case POLY_STEP_CPU_SEND: {
        /* The CPU sends the materials that FindFusionMaterials chose, last slot first, one per call, counting
         * fusionMaterialCount down. Unlike the human's path it has no key 1547 branch: they always go to the
         * Graveyard. Matching: the count goes through a temporary. */
        u16 *slot;
        int count = gChain.fusionMaterialCount;

        if (count != 0) {
            gChain.fusionMaterialCount = count - 1;
            slot = &gChain.fusionMaterialSlots[gChain.fusionMaterialCount];
            if (*slot & FUSION_SLOT_HAND)
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SEND_HAND_FUSION_MATERIAL_TO_GRAVEYARD), *slot & 0xFF, 0, 0);
            else if (*slot & FUSION_SLOT_FIELD)
                SendFusionMaterialToGrave(link->player, *slot & 0xFF);
            return POLY_STEP_CPU_SEND;
        }
        return POLY_STEP_COMPACT_HAND;
    }
    case POLY_STEP_COMPACT_HAND: {
        u16 *pickedCard;

        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_COMPACT_HAND), 0, 0, 0);
        /* The picked Fusion monster, as the two halves of its card word: take it out of the Fusion Deck. */
        pickedCard = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow];
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_FUSION_DECK), pickedCard[0], pickedCard[1], 0);
        return POLY_STEP_SUMMON;
    }
    case POLY_STEP_SUMMON:
        /* Special Summon the picked entry of the list (the position is asked). */
        QueueSpecialSummonChoosePosition(
            link->player, (struct DuelCard *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow], 1,
            ZONE_STATUS_UNK14);
        return POLY_STEP_END;
    }
    return EFFECT_STEP_DONE;
}
