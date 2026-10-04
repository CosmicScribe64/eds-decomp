#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_* extractors, gCardNames */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum DuelZoneIndex, DuelArea, ZoneLinkKind, ResponseEventKind, DuelPromptKind */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* enum SoundEffect */

/*
 * Card effect handlers: the Resolve slot of gCardEffects (include/effect.h) for the effect keys 1319-1444
 * (wiki/functions/effect-resolve10-c.md). No EDS card has these numbers (gCardNumberToId[key] is 0), so the
 * game never reaches them: they are leftovers of the shared Duel Monsters engine, named after what they do
 * (include/effect_handlers.h names the OCG card each key behaves like, as a hypothesis).
 *
 * Chain_Resolve calls a link's resolve handler once per frame with gChain.effectStep as the handler's step:
 * 0x80 (EFFECT_STEP_START) on the first call, then whatever the previous call returned, until a handler
 * returns 0. The steps count down from 0x80 and are private to each handler; a returned step without a case
 * (0x64, 0x78, 0x77, 0x0A, ...) ends the effect on the next call. Most handlers do nothing when the link was
 * negated.
 *
 * Players: 0 is the human, 1 the CPU (or the link partner); the CPU skips the Yes/No questions. Duel commands
 * queued for player 1 carry DUEL_CMD_PLAYER (bit 15). Zones 0-4 hold monsters, 5-9 spells and traps, 10 the
 * Field Magic; a target or location is player | zone << 8 (DUEL_LOC).
 */

#include "duel.h"                /* duel state, zones, players */
#include "sound.h"               /* PlaySE */
#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain */
#include "duel_actions.h"           /* discards, flips, position changes, life points, QueueAddZoneLink */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_Post */
#include "duel_screen.h"            /* gDuelScreen.selIndex, DuelCursor_PickTarget */
#include "effect.h"                 /* the target and destruction helpers, gStrCoinTossMenu */
#include "effect_handlers.h"        /* the prototypes of this unit's handlers, EffectHandMonsterAndTwoCardsPrepare */
#include "summon.h"                 /* QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* FormatStr, Random */

/* ---- Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view") ---- */

/* Matching: CollectEffectTargets is defined to return u16; the ROM tests the result as an int, without the
 * narrowing of r0 that the u16 return adds. */
int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");

/* Alias symbols (address-suffixed; card_data.h lists them). Matching: the ROM loads these addresses from
 * their own literals. */
extern const u16 gCardNumberToId_ParasiteParacide[];   /* &gCardNumberToId[CARD_PARASITE_PARACIDE] */
extern const u16 gCardNumberToId_1405[];   /* &gCardNumberToId[CARD_1405] (0: no EDS card) */
extern u8 gChainEffectCounter[];          /* &gChain.effectCounter */

/* Prompts used only by this unit. */
extern const u8 gStrPlaceOnDeckTopPrompt[];         /* 0x08083604 "Do you wish to place %s at the top of the
                                                     * Deck?" */
extern const u8 gStrPayToReviveNextStandbyPrompt[]; /* 0x08083634 "Do you wish to pay 1000LP to Special-Summon
                                                     * %s in your next Standby Phase?" */
extern const u8 gStrSelectHandMonster[];            /* 0x08083690 "Select a Monster card from your hand." */
extern const u8 gStrSelectHandMagicTrap[];          /* 0x080836BC "Select 1 Magic or Trap card from your hand." */
extern const u8 gStrSelectAnotherHandMagicTrap[];   /* 0x080836F0 "Select another Magic or Trap card from your
                                                     * hand." */
extern const u8 gStrSelectGraveMonsterToDeck[];     /* 0x0808372C "Select a Monster to be returned to your Deck." */
extern const u8 gStrSelectGraveMonsterToHand[];     /* 0x08083764 "... to be retruned to your hand." (sic) */
extern const u8 gStrSelectGraveMagicToDeck[];       /* 0x0808379C "Select a Magic card to be returned your Deck." */

/* ---- Helpers ---- */

/* The command id for a player: player 1's commands carry DUEL_CMD_PLAYER. CMD_FOR is the link's player. */
#define CMD_FOR_PLAYER(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))
#define CMD_FOR(link, cmd)          CMD_FOR_PLAYER((link)->player, cmd)

/* Text box placement: TextBoxOpen takes pos = x | y << 8 and size = width | height << 8, in cells. */
#define TEXT_BOX_CELLS(x, y)        ((x) | (y) << 8)
#define PROMPT_POS                  TEXT_BOX_CELLS(6, 2)
#define PROMPT_SIZE_18x7            TEXT_BOX_CELLS(18, 7)
#define PROMPT_SIZE_19x6            TEXT_BOX_CELLS(19, 6)

/* The card word of a zone or pile entry as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with shifts. */
#define CARD_WORD(card)             (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)               (((word) << 20) >> 20)

/*
 * Card tables through integer-constant addresses: gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4).
 * Matching: these give the ROM's literal pools and reloads; the symbol forms (card_data.h) generate other
 * code.
 */
#define CARD_STATS(id)              (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id)             (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id)               CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType */
/* enum CardType of a card word, indexing gCardStats with the word's low 11 bits (lsl #21; lsr #21). */
#define CARD_WORD_TYPE(word)        CARD_STATS_TYPE(((const u32 *)0x08621DE0)[(u32)(word) << 21 >> 21])

/*
 * &gDuelZones[side].zones[zone] by byte arithmetic, side being 0 or 1. Matching: the two forms add the zone
 * and player terms in the orders the ROM uses.
 */
#define ZONE_AT(side, zone) /* zone term first */ \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (side) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
#define ZONE_AT_PLAYER_FIRST(side, zone) /* player term first */ \
    ((struct DuelZone *)((side) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))
/* DuelZone +0x06 as a byte: isDefense (bit 0) and isFaceUp (bit 1). */
#define ZONE_POSITION(zone)         (((u8 *)(zone))[6])
#define ZONE_POSITION_DEFENSE       0x1
#define ZONE_POSITION_FACE_UP       0x2

/* gDuelPlayers[player].hand[index] as one u32, through gDuelHands with the player term first (the ROM's
 * address order). */
#define HAND_CARD_WORD(player, index) \
    (*(u32 *)((player) * sizeof(struct DuelPlayer) + (index) * sizeof(struct DuelCard) + (u32)gDuelHands))
/* gDuelPlayers[side].hand through gDuelHands, as card words. */
#define HAND_WORDS(side)            ((u32 *)((side) * sizeof(struct DuelPlayer) + (u32)gDuelHands))

/* ChainEntry.targets as a struct (EffectHandRouletteSummonResolve). */
struct ChainTargets {
    u16 loc[3];
};

/* The card the player picked in gCardListView. */
#define LIST_PICK                   (&gCardListView.cards[gCardListView.cursorRow + gCardListView.top])
/* gCardListView.cards[] as halfword pairs, and the card ID in the picked entry's low halfword (lsl #20;
 * lsr #20). Matching: indexing this view folds the +0x0C of cards into the ldrh offset. */
struct CardListViewHalves {
    u8 unk0[0xC];
    struct { u16 lo; u16 hi; } cards[0x80];     /* +0x0C */
};
#define LIST_PICK_ID \
    ((u32)((struct CardListViewHalves *)&gCardListView)->cards[gCardListView.cursorRow + gCardListView.top].lo << 20 >> 20)

/* ---- Resolve handlers ---- */

/* Key 1319 (hypothesis: OCG Interdimensional Matter Transporter): banish the target until the End Phase
 * (DUEL_CMD_BANISH_UNTIL_END_PHASE); its zone stays reserved for it. */
int EffectBanishOwnMonsterUntilEndPhaseResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (link->numTargets == 1) {
            int targetPlayer = (u8)link->targets[0];
            int targetZone = DUEL_LOC_ZONE(link->targets[0]);

            if (CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(targetPlayer & 1, targetZone)->card)))
                DuelCmd_Push(CMD_FOR_PLAYER(targetPlayer, DUEL_CMD_BANISH_UNTIL_END_PHASE), targetZone, 1, 0);
        }
    }
    return 0;
}

/* Key 1320 (hypothesis: OCG Ground Collapse): link both target zones, if still empty, to this card
 * (ZONE_LINK_CONTINUOUS); IsMonsterZoneFree then refuses them while the card stays on the field. */
int EffectBlockTwoMonsterZonesResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 2) {
        int i;
        u16 *target = link->targets;

        for (i = 0; i <= 1; target++, i++) {
            int zone = DUEL_LOC_ZONE(*target);

            if (!CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(1 & *(u8 *)target, zone)->card)))
                QueueAddZoneLink(link->player, link->player | link->zone << 8, *target, ZONE_LINK_CONTINUOUS);
        }
    }
    return 0;
}

/*
 * Key 1321 (hypothesis: OCG Magic Drain): negate the Magic card chainedTo unless the opponent discards a Magic
 * card.
 *   0x80  an opponent with an empty hand cannot discard: go to 0x7E; else offer the discard
 *         (PROMPT_OFFER_DISCARD_MAGIC)
 *   0x7F  a discard (gDuel.promptResult nonzero) ends the effect; otherwise step on into 0x7E
 *   0x7E  negate chainedTo if it is a Magic card (DUEL_CMD_NEGATE_ACTIVATION, also destroying it)
 */
int EffectNegateMagicUnlessDiscardResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    if (!link->negated) {
        int step = gChain.effectStep;

        switch ((short)step) { /* FAKEMATCH: the (short) cast changes no semantics and gives the ROM's r2/`adds r0,r2,#0` codegen. */
        case 0x80:
            if (gDuelPlayers[(1 - link->player) & 1].handCount == 0)
                return 0x7E;
            DuelPrompt_Post(1 - link->player, PROMPT_OFFER_DISCARD_MAGIC, 0, 0);
            return 0x7F;
        case 0x7F:
            if (gDuel.promptResult != 0)
                return 0;
            gChain.effectStep = step - 1;
            /* fallthrough */
        case 0x7E:
            if (chainedTo == NULL)
                return 0;
            if (CARD_TYPE(chainedTo->card) == CARD_TYPE_MAGIC)
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_NEGATE_ACTIVATION), TRUE, 0, 0);
            return 0x64;
        }
    }
    return 0;
}

/* Key 1324: the opponent loses 500 LP (the cost, discarding a Magic card, is EffectDiscardMagicCardChainA). */
int EffectDiscardMagicDamageResolve(struct ChainEntry *link)
{
    if (!link->negated)
        LoseLifePoints(1 - link->player, 500);
    return 0;
}

/* Key 1325 (hypothesis: OCG Shadow of Eyes): answering the opponent's Set of a monster (event RESPONSE_SET at
 * loc0), turn the monster, if still face down in defense position, face up in attack position without its
 * flip effect. */
int EffectFlipSetMonsterToAttackResolve(struct ChainEntry *link)
{
    if (!link->negated && link->event == RESPONSE_SET) {
        int setPlayer = (u8)link->loc0;
        int setZone = DUEL_LOC_ZONE(link->loc0);

        if (setPlayer != link->player) {
            int side = setPlayer & 1;
            struct DuelZone *z = ZONE_AT(side, setZone);

            /* Face down in defense position. Matching: two byte tests; the bitfield tests z->isDefense &&
             * !z->isFaceUp merge into one (byte & 3) == 1 compare. */
            if (CARD_ID(CARD_WORD(z->card)) && (ZONE_POSITION(z) & ZONE_POSITION_DEFENSE)
                && !(ZONE_POSITION(z) & ZONE_POSITION_FACE_UP))
                ChangeBattlePosition(setPlayer, setZone, TRUE, FALSE);
        }
    }
    return 0;
}

/*
 * Key 1328: offer to put Parasite Paracide from the deck on top of it (the CPU always accepts).
 *   0x80  ask "Do you wish to place Parasite Paracide at the top of the Deck?" (Yes/No)
 *   0x7F  on Yes: take the card out of the deck into gChain.effectCard, shuffle, and put it back on top
 */
int EffectPlaceParasiteParacideOnDeckResolve(struct ChainEntry *link)
{
    char text[0x80];

    if (!link->negated) {
        if (gChain.effectStep == 0x80) {
            if (link->player) {
                gTextBox.result = TRUE;
                return 0x7F;
            }
            FormatStr(text, (const char *)gStrPlaceOnDeckTopPrompt,
                      (const char *)gCardNames + gCardNumberToId_ParasiteParacide[0] * CARD_NAME_SIZE);   /* Parasite Paracide */
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return 0x7F;
        } else {
            u16 *card;
            register u8 *chain __asm__("r1");
            register int offset __asm__("r2");

            if (gTextBox.result == 0)
                return 0;
            /* FAKEMATCH: &gChain.effectCard as base and offset pinned to r1 / r2, with an empty asm, to keep the
             * ROM's base/offset registers and add operand order. */
            chain = (u8 *)&gChain;
            offset = OFFSET_OF(struct ChainState, effectCard);
            __asm__("" : "+r"(offset), "+r"(chain));
            card = (u16 *)(chain + offset);
            if (RemoveDeckCardByNumber(link->player, CARD_PARASITE_PARACIDE, (u32 *)card) != -1) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_ADD_CARD_TO_DECK_TOP), card[0], card[1], 0);
            }
        }
    }
    return 0;
}

/* Key 1330, flip: change the target's battle position. Does not test negated. */
int EffectChangeTargetPositionResolve(struct ChainEntry *link)
{
    if (link->numTargets == 1) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = DUEL_LOC_ZONE(link->targets[0]);

        if (CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(targetPlayer & 1, targetZone)->card)))
            ChangeBattlePosition(targetPlayer, targetZone, FALSE, FALSE);
    }
    return 0;
}

/* Key 1337, flip: turn the target, a face-down defense-position card, face up without its flip effect. An
 * effect monster is destroyed; any other card is shown and turned face down again. Does not test negated. */
int EffectDestroySetEffectMonsterResolve(struct ChainEntry *link)
{
    if (link->numTargets == 1) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = DUEL_LOC_ZONE(link->targets[0]);
        int side = targetPlayer & 1;
        struct DuelZone *z = ZONE_AT(side, targetZone);
        int id = CARD_ID(CARD_WORD(z->card));
        u16 cardId = id;    /* Matching: the u16 copy for the calls, made before the test */

        if (id > 0 && z->isDefense && !z->isFaceUp) {
            FlipFieldCard(targetPlayer, targetZone, FALSE);
            if (IsEffectMonster(cardId)) {
                ShowDestroyedCard(targetPlayer, cardId);
                /* Card bit 14 is set while it is destroyed, as DestroyFieldCardByEffect does for Sangan and
                 * Witch of the Black Forest (hypothesis: so that the destroy triggers run). */
                ((struct DuelCardStatusBytes *)z)->unk14 = 1;
                DestroyFieldCardByEffect(targetPlayer, targetZone);
                OnCardDestroyedByEffect(link->player, targetPlayer, targetZone);
                ((struct DuelCardStatusBytes *)z)->unk14 = 0;
            } else {
                ShowRevealedCard(targetPlayer, cardId);
                FlipFieldCard(targetPlayer, targetZone, FALSE);
            }
        }
    }
    return 0;
}

/* Key 1338, flip: destroy each of the opponent's face-up monsters of level 4. */
int EffectDestroyOpponentLevel4Resolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int zone;
        u32 level;

        for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
            u16 id = CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST((1 - link->player) & 1, zone)->card));

            if (ZONE_AT_PLAYER_FIRST((1 - link->player) & 1, zone)->isFaceUp && id != 0) {
                /* The card's level: 0 for Trap, Magic and Ticket cards, 10 for the Egyptian Gods, else the
                 * stars. Matching: written out here; an inline function returning it generates other code. */
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
                if (level == 4) {
                    DestroyFieldCardByEffect(1 - link->player, zone);
                    OnCardDestroyedByEffect(link->player, 1 - link->player, zone);
                }
            }
        }
    }
    return 0;
}

/*
 * Key 1405 (hypothesis: OCG Revival Jam): offer to pay 1000 LP to Special Summon the card in the next Standby
 * Phase (DUEL_CMD_ADJUST_DELAYED_SUMMON_COUNT; the Standby Phase does the summon).
 *   0x80  ask "Do you wish to pay 1000LP to Special-Summon %s in your next Standby Phase?" (Yes/No)
 *   0x7F  on Yes: pay and count the delayed summon
 */
int EffectPayToReviveNextStandbyResolve(struct ChainEntry *link)
{
    char text[0x80];

    if (!link->negated) {
        if (gChain.effectStep == 0x80) {
            FormatStr(text, (const char *)gStrPayToReviveNextStandbyPrompt,
                      (const char *)gCardNames + gCardNumberToId_1405[0] * CARD_NAME_SIZE);   /* key 1405's name */
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, (const u8 *)text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return 0x7F;
        } else {
            if (gTextBox.result == 0)
                return 0;
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_LOSE_LP), 1000, 0, 0);
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_ADJUST_DELAYED_SUMMON_COUNT), 1, 0, 0);
        }
    }
    return 0;
}

/*
 * Key 1414: hand roulette. The player picks a monster and two Magic/Trap cards from the hand (hand indices in
 * targets[0..2]); a roulette picks one of the three at random. The other two are discarded; a picked monster
 * is Special Summoned, a picked Magic/Trap card is discarded too.
 *   0x80  needs a free zone, a summonable hand monster and two other hand cards
 *   0x7F  pick the monster (not special-summon-only; a wrong pick buzzes)
 *   0x7E / 0x7D  pick the first Magic/Trap card
 *   0x7C / 0x7B  pick a second, different one; start the roulette: 16 hops (gChain.effectSubStep)
 *   0x7A  one hop: move the cursor to a random pick, never the same twice in a row (gChain.effectCounter);
 *         after the last hop point at the final pick
 *   0x79  discard the two cards that were not picked
 *   0x78  the picked monster leaves the hand and is Special Summoned, or the picked card is discarded
 */
int EffectHandRouletteSummonResolve(struct ChainEntry *link, struct ChainEntry *chainedTo)
{
    int i;

    if (!link->negated) {
        switch (gChain.effectStep) {
        case 0x80:
            if (EffectHandMonsterAndTwoCardsPrepare(link, (int)chainedTo, FALSE) == 0)
                return 0;
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, gStrSelectHandMonster);
        pickMonster:
            return 0x7F;
        case 0x7F: {
            int id;
            int side;

            if (DuelCursor_PickTarget(PICK_HAND) == 0)
                goto pickMonster;
            side = link->player & 1; /* the `& 1` leaves the constant 1 in a register that the later `1 & byte` reuses */
            id = CARD_ID(HAND_CARD_WORD(side, gDuelScreen.selIndex));
            if (CARD_TYPE(id) <= CARD_TYPE_REPTILE && IsSpecialSummonOnly(id) == 0) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), link->player,
                             (u16)((u8)gDuelScreen.selIndex << 8 | DUEL_AREA_HAND), 0);
                link->targets[0] = gDuelScreen.selIndex;
                return 0x7E;
            }
            PlaySE(SE_ERROR);
            goto pickMonster;
        }
        case 0x7E:
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, gStrSelectHandMagicTrap);
        pickFirstSpell:
            return 0x7D;
        case 0x7D:
            if (DuelCursor_PickTarget(PICK_HAND) == 0)
                goto pickFirstSpell;
            if (CARD_WORD_TYPE(HAND_CARD_WORD(link->player & 1, gDuelScreen.selIndex)) > CARD_TYPE_REPTILE) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), link->player,
                             (u16)((u8)gDuelScreen.selIndex << 8 | DUEL_AREA_HAND), 0);
                link->targets[1] = gDuelScreen.selIndex;
                return 0x7C;
            }
            PlaySE(SE_ERROR);
            goto pickFirstSpell;
        case 0x7C:
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, gStrSelectAnotherHandMagicTrap);
        pickSecondSpell:
            return 0x7B;
        case 0x7B:
            if (DuelCursor_PickTarget(PICK_HAND) == 0)
                goto pickSecondSpell;
            if (CARD_WORD_TYPE(HAND_CARD_WORD(link->player & 1, gDuelScreen.selIndex)) > CARD_TYPE_REPTILE
                && gDuelScreen.selIndex != link->targets[1]) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), link->player,
                             (u16)((u8)gDuelScreen.selIndex << 8 | DUEL_AREA_HAND), 0);
                link->targets[2] = gDuelScreen.selIndex;
                gChain.effectSubStep = 16;  /* roulette hops */
                gChain.effectCounter = 0;
                return 0x7A;
            }
            PlaySE(SE_ERROR);
            goto pickSecondSpell;
        case 0x7A: {
            /* Matching: targets[] indexed through a struct view keeps the base first in the address add (a
             * plain u16 pointer adds the index first). */
            struct ChainTargets *picks = (struct ChainTargets *)link->targets;

            do {
                i = Random() % 3;
            } while (i == gChainEffectCounter[0]);    /* gChain.effectCounter: the previous pick */
            gChain.effectCounter = i;
            if (gChain.effectSubStep != 0) {
                gChain.effectSubStep--;
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_MOVE_CURSOR), DUEL_AREA_HAND, picks->loc[gChain.effectCounter], 0);
                return 0x7A;
            }
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), link->player,
                         (u16)((u8)picks->loc[gChain.effectCounter] << 8 | DUEL_AREA_HAND), 0);
            return 0x79;
        }
        case 0x79: {
            u8 *pick;

            /* Matching: pick is set after i = 0, like a hoisted invariant. Discard the two cards that were not
             * picked (compactHand FALSE: the hand indices must stay valid). */
            for (i = 0, pick = gChainEffectCounter; i <= 2; i++) {
                if (i != *pick)
                    DiscardHandCard(link->player, link->targets[i], FALSE, FALSE);
            }
            return 0x78;
        }
        case 0x78:
            if (gChain.effectCounter == 0) {
                /* The monster won: remove it from the hand and Special Summon it face up. Matching: the row
                 * pointer in its own variable gives the (player * 0xD64 + base) + index * 4 order. */
                u32 *row = HAND_WORDS(link->player & 1);
                u32 *card = row + link->targets[0];

                ShowRevealedCard(link->player, CARD_ID(*card));
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_REMOVE_CARD_FROM_HAND), ((u16 *)card)[0], ((u16 *)card)[1], 0);
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_COMPACT_HAND), 0, 0, 0);
                QueueSpecialSummonChoosePosition(link->player, (struct DuelCard *)card, TRUE, 0);
            } else {
                ShowDestroyedCard(link->player, CARD_ID(HAND_CARD_WORD(link->player, link->targets[gChain.effectCounter])));
                DiscardHandCard(link->player, link->targets[gChain.effectCounter], FALSE, TRUE);
            }
            return 0x77;
        }
    }
    return 0;
}

/* Key 1417 (hypothesis: OCG Mask of Dispel): link the target, a face-up Magic card, to this card
 * (ZONE_LINK_CONTINUOUS; the Standby Phase deals the damage). With no valid target the card sets its own
 * destroyIfNegated bit, so it leaves the field. */
int EffectCurseFaceUpMagicResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (link->numTargets == 1) {
            int targetZone = DUEL_LOC_ZONE(link->targets[0]);
            int side = 1 & *(u8 *)&link->targets[0];
            struct DuelZone *z = ZONE_AT(side, targetZone);
            int id = CARD_ID(CARD_WORD(z->card));

            if (id && z->isFaceUp && CARD_TYPE(id) == CARD_TYPE_MAGIC) {
                QueueAddZoneLink(link->player, link->player | link->zone << 8, link->targets[0], ZONE_LINK_CONTINUOUS);
                return 0;
            }
        }
        link->destroyIfNegated = TRUE;
    }
    return 0;
}

/*
 * Key 1424 (hypothesis: OCG Fairy Box): coin toss; a right call makes the attacker's ATK 0 for this battle.
 *   0x80  the human calls Heads or Tails (two-choice menu); the CPU calls at random
 *   0x7F  toss (DUEL_CMD_TOSS_COIN shows it); on a right call DUEL_CMD_ZERO_ATTACKER_ATK
 * Returns 0x0A (no case: the effect ends on the next call).
 */
int EffectCoinTossZeroAttackerResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case 0x80:
            if (!link->player) {
                TextBoxOpen(PROMPT_POS, PROMPT_SIZE_19x6, TEXTBOX_FLAGS_DEFAULT, gStrCoinTossMenu);
                TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
            } else {
                gTextBox.result = Random() & 1;     /* enum CoinFace */
            }
            return 0x7F;
        case 0x7F: {
            u8 result = Random() & 1;   /* enum CoinFace */

            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_TOSS_COIN), gTextBox.result, result, 0);
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_OPEN_DUEL_SCREEN), 0, 0, 0);
            if (result == gTextBox.result)
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_ZERO_ATTACKER_ATK), 0, 0, 0);
            return 0x0A;
        }
        }
    }
    return 0;
}

/* Key 1428 (hypothesis: OCG Jam Defender): if the target is a face-up key 1405 (Revival Jam by behaviour),
 * point at it and make it the attack target. */
int EffectRedirectAttackResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (link->numTargets == 1) {
            int targetZone = DUEL_LOC_ZONE(link->targets[0]);
            int side = 1 & *(u8 *)&link->targets[0];
            struct DuelZone *z = ZONE_AT(side, targetZone);
            int id = CARD_ID(CARD_WORD(z->card));

            if (z->isFaceUp && id && CARD_NUMBER(id) == CARD_1405) {
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_POINT_AT_CARD), link->player, targetZone << 8 | DUEL_AREA_MONSTER, 0);
                DuelCmd_Push(CMD_FOR(link, DUEL_CMD_SET_ATTACK_TARGET), link->targets[0], TRUE, 0);
            }
        }
    }
    return 0;
}

/*
 * Keys 1421, 1430, 1433 and 1444: pick a graveyard monster destroyed in battle (target collector, key 1421).
 * 1430 returns it to the top of the deck, 1433 to the bottom, 1444 to the hand; 1421 only marks it
 * (DUEL_CMD_MARK_GRAVEYARD_CARD). Does not test negated.
 *   0x80  needs a candidate; "Select a Monster to be returned to your Deck / hand."
 *   0x7F  open the card list
 *   0x7E  act on the picked card
 */
int EffectRecoverGraveMonsterResolve(struct ChainEntry *link)
{
    switch (gChain.effectStep) {
    case 0x80:
        if (CollectEffectTargetsInt(link->player, CARD_1421, 0) == 0)
            return 0;
        switch (CARD_NUMBER(link->card)) {
        case CARD_1430:
        case CARD_1433:
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_18x7, TEXTBOX_FLAGS_DEFAULT, gStrSelectGraveMonsterToDeck);
            return 0x7F;
        case CARD_1421:
        case CARD_1444:
            TextBoxOpen(PROMPT_POS, PROMPT_SIZE_18x7, TEXTBOX_FLAGS_DEFAULT, gStrSelectGraveMonsterToHand);
            return 0x7F;
        }
        return 0;
    case 0x7F:
        CardListView_Open(link->player, -1, CARD_1421, 0);
        return 0x7E;
    case 0x7E:
        switch (CARD_NUMBER(link->card)) {
        case CARD_1430:
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_TOP), LIST_PICK_ID, 0, 0);
            return 0x78;
        case CARD_1433:
            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_BOTTOM), LIST_PICK_ID, 0, 0);
            return 0x78;
        case CARD_1444: {
            u16 *card = (u16 *)LIST_PICK;

            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND), card[0], card[1], 0);
            return 0x78;
        }
        case CARD_1421: {
            u16 *card = (u16 *)LIST_PICK;

            DuelCmd_Push(CMD_FOR(link, DUEL_CMD_MARK_GRAVEYARD_CARD), card[0], card[1], 0);
            return 0x78;
        }
        }
        return 0;
    }
    return 0;
}

/* Keys 1432 and 1442: tribute both targets (two of the player's occupied monster zones), unless key 1418
 * (forbids tributes) is active on either side; then key 1432 deals 1200 damage to the opponent, key 1442 gains
 * 1000 LP. */
int EffectTributeTwoMonstersResolve(struct ChainEntry *link)
{
    if (!link->negated && CountActiveCardsOnField(0, CARD_1418) <= 0 && CountActiveCardsOnField(1, CARD_1418) <= 0
        && link->numTargets == 2) {
        int i;
        register u16 *targets __asm__("r3");
        u16 *target;

        i = 0;
        targets = link->targets;
        for (; i <= 1; i++) {
            register int offset __asm__("r0") = i * 2;
            u16 *loc;
            int targetPlayer, targetZone;

            /* FAKEMATCH: initialized offset constraint retains indexed checks
             * with the r3 base. The later call loop uses its own walking pointer;
             * targets is no longer needed once that pointer is assigned. */
            asm("" : "+r"(offset));
            loc = (u16 *)((u8 *)targets + offset);
            targetPlayer = *(u8 *)loc;
            targetZone = DUEL_LOC_ZONE(*loc);

            if (targetPlayer != link->player)
                return 0;
            if (!CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(1 & targetPlayer, targetZone)->card)))
                return 0;
        }
        target = targets;
        for (i = 0; i <= 1; target++, i++)
            TributeMonster((u8)*target, DUEL_LOC_ZONE(*target));
        switch (CARD_NUMBER(link->card)) {
        case CARD_1432:
            LoseLifePoints(1 - link->player, 1200);
            break;
        case CARD_1442:
            GainLifePoints(link->player, 1000);
            break;
        }
    }
    return 0;
}

/* Key 1435, flip: banish the top 3 cards of the player's deck; the opponent loses 800 LP. */
int EffectBanishDeckTopDamageResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        BanishTopDeckCards(link->player, 3);
        LoseLifePoints(1 - link->player, 800);
    }
    return 0;
}

/* Key 1436 (hypothesis: OCG Fire Sorcerer), flip: the opponent loses 800 LP (the cost, banishing 2 random hand
 * cards, is EffectBanishRandomHandCardsChainA). */
int EffectDamageOpponent800Resolve(struct ChainEntry *link)
{
    if (!link->negated)
        LoseLifePoints(1 - link->player, 800);
    return 0;
}

/*
 * Key 1439: put a graveyard Magic card destroyed by the opponent (target collector, key 1439) on the bottom of
 * the deck. Does not test negated.
 *   0x80  needs a candidate; "Select a Magic card to be returned your Deck."
 *   0x7F  open the card list
 *   0x7E  return the picked card
 */
int EffectTributeRecoverGraveMagicResolve(struct ChainEntry *link)
{
    switch (gChain.effectStep) {
    case 0x80:
        if (CollectEffectTargetsInt(link->player, CARD_1439, 0) == 0)
            return 0;
        TextBoxOpen(PROMPT_POS, PROMPT_SIZE_18x7, TEXTBOX_FLAGS_DEFAULT, gStrSelectGraveMagicToDeck);
        return 0x7F;
    case 0x7F:
        CardListView_Open(link->player, -1, CARD_1439, 0);
        return 0x7E;
    case 0x7E:
        DuelCmd_Push(CMD_FOR(link, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_DECK_BOTTOM), LIST_PICK_ID, 0, 0);
        return 0x78;
    }
    return 0;
}
