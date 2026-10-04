#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_TYPE, CARD_STATS_LEVEL */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum ChainEntryKind, ResponseEventKind, DuelArea, DuelPromptKind, ZoneLinkKind */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */

/*
 * Card effect handlers: the Resolve slot of gCardEffects (include/effect.h) for card numbers 1116-1181
 * (wiki/functions/effect-resolve7-c.md).
 *
 * Chain_Resolve calls a link's resolve handler once per frame with gChain.effectStep as the handler's step:
 * EFFECT_STEP_START (0x80) on the first call, then whatever the previous call returned, until a handler
 * returns EFFECT_STEP_DONE (0). The steps count down from 0x80 (enum EffectStep, include/effect.h) and
 * mean something different in each handler; the comment above each one lists them. A returned step the
 * handler has no case for (often EFFECT_STEP_END, 0x64) ends the effect on the next call. Every handler
 * does nothing when the link was negated (ChainEntry.negated).
 *
 * Players: 0 is the human, 1 the CPU (or the link partner). Duel commands queued for player 1 carry
 * DUEL_CMD_PLAYER (bit 15). Zones 0-4 hold monsters, 5-9 spells and traps; a target is player | zone << 8.
 */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that chain.h, card_list_view.h, duel_cmd.h, duel_screen.h and summon.h do not pull in the legacy
 * header. After H0, replace the block (BEGIN to END) with #include "legacy/duel.h" (see
 * build/readability/issues/effect_resolve7.md). */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                    /* bit 12: owning player */
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
    u8 unk6_2:6;
    u8 unk7[0x94 - 0x7];
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 unk4[0x28 - 0x4];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4: deck[0] is the top card */
    u8 unk904[0xD64 - 0x904];
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;                     /* +0x1B12 bits 2-4: enum DuelPhase */
    u8 unk1B12_5:3;
    u8 unk1B13[0x1B64 - 0x1B13];
    u16 promptResult;               /* +0x1B64: the answer of the last duel prompt */
    u8 unk1B66[0x1B78 - 0x1B66];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

u32 HasFlipEffect(u16 cardNo, int inBattle);
u32 IsEffectMonster(u16 cardId);
u32 IsSpecialSummonOnly(u16 cardId);
void CopyDuelCard(u32 *dst, u32 *src);
int CountGraveyardCardsByNumber(int player, u16 cardNo);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFreeMonsterZones(int player);
void DestroyInvalidEquips(int player, int zone);
u32 GetZoneCardAtk(u32 player, u32 slot);
u32 GetZoneCardType(s32 player, s32 slot);
/* ---- END duel.h stand-in ---- */

#include "ai.h"                     /* gAiWork.listPick, AiPickCardListEntry */
#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain, gChainEffectWork */
#include "duel_actions.h"           /* life points, flips, banishing, discards, QueueAddZoneLink */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl.isLinkDuel */
#include "duel_prompt.h"            /* DuelPrompt_Post, DuelPrompt_TryPostSetMonster */
#include "duel_screen.h"            /* DuelCursor_Select */
#include "effect.h"                 /* CollectEffectTargets */
#include "effect_handlers.h"        /* the prototypes of this unit's handlers */
#include "summon.h"                 /* QueueSpecialSummon, QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* Random, HalveRoundUp */

/*
 * Local views of callees (build/readability/HEADERS.md, "Keeping a deliberate local view").
 */
/* Matching: CollectEffectTargets is defined to return u16; the ROM compares the result as an int (cmp; ble)
 * without narrowing r0. */
int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: UpdateSpellTrapNegation takes no parameter, but EffectNegateCardsThisTurnResolve calls it with
 * r0 = 0 (movs r0, #0 before the bl). */
void UpdateSpellTrapNegationArg(int unused) asm("UpdateSpellTrapNegation");
/* Matching: EffectNoblemanResolve calls FlipFieldCard with a fourth argument (r3 = 0). */
void FlipFieldCard4(int player, int zone, u16 triggerFlip, int unused) asm("FlipFieldCard");
/* Matching: the definition takes a u16 target; Riryoku passes HalveRoundUp's int result without the
 * narrowing a u16 parameter adds (lsl #16; lsr #16). */
void QueueAddZoneLinkInt(int player, int target, u16 at, u16 kind) asm("QueueAddZoneLink");

/* Text box prompts (not in a header: only this unit uses them). */
extern const u8 gStrPromptAddGraveMonsterToHand[];  /* 0x08083164: "Do you wish to add a Monster from the
                                                     * Graveyard to your hand?" */
extern const u8 gStrSelectMonsterToAddToHand[];     /* 0x080831A8: "Select a Monster card that you wish to
                                                     * add to your hand." */

/* Text box position and size (x | y << 8, width | height << 8, in cells) of the effect prompts. */
#define EFFECT_TEXTBOX_POS      0x206
#define EFFECT_TEXTBOX_SIZE     0x712

/* The command id for player's side: player 1's commands carry DUEL_CMD_PLAYER. */
#define PLAYER_CMD(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* The card word of a zone or pile entry as one u32. Matching: the ROM loads the whole word (ldr) and
 * extracts the ID with shifts; a .id bitfield read would load only the halfword that holds it. */
#define CARD_WORD(card)     (*(u32 *)&(card))
/* Card ID (bits 0-11) of a card word: lsl #20; lsr #20. */
#define CARD_ID(word)       (((word) << 20) >> 20)
/* The low 11 bits of a card word (lsl #21; lsr #21), the form some table lookups use for the ID. */
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
 * address order (array indexing emits the player term first). player must be 0 or 1.
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
/* The zone holds a card: its card word's ID bits are nonzero. Matching: a whole-word load and lsl #20;
 * a card.id != 0 test generates other code. */
#define ZONE_HAS_CARD(zone) (CARD_WORD((zone)->card) << 20 != 0)

/*
 * Spear Cretin, The Shallow Grave: each player, the turn player first, may Special Summon a monster from
 * their own graveyard (prompt PROMPT_SELECT_GRAVEYARD_MONSTER). Spear Cretin summons in a position of the
 * player's choice, The Shallow Grave face-down in defense position. Spear Cretin's effect only resolves when
 * the card was sent off the field (kind CHAIN_KIND_OFF_FIELD); otherwise it only marks its effect used.
 * gChain.effectSubStep is the side being asked.
 *   EFFECT_STEP_START  start with the turn player
 *   EFFECT_STEP_2      prompt the side if it has a free monster zone and a graveyard monster to pick
 *   EFFECT_STEP_3      take the picked card from the graveyard and summon it
 *   EFFECT_STEP_4      next side; finish once both sides had their turn
 */
int EffectEachPlayerReviveResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (CARD_NUMBER(link->card) == CARD_SPEAR_CRETIN && link->kind != CHAIN_KIND_OFF_FIELD) {
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
            return EFFECT_STEP_DONE;
        }
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = gDuel.turnPlayer;
            gChain.effectStep--;
            /* -> EFFECT_STEP_2, fall through */
        case EFFECT_STEP_2:
            if (CountFreeMonsterZones(gChain.effectSubStep) > 0
                && CollectEffectTargetsInt(gChain.effectSubStep, CARD_NUMBER(link->card), 0) > 0) {
                DuelPrompt_Post(gChain.effectSubStep, PROMPT_SELECT_GRAVEYARD_MONSTER, link->card, link->player);
                return EFFECT_STEP_3;
            }
            return EFFECT_STEP_4;
        case EFFECT_STEP_3: {
            /* The picked card word: promptResult and the halfword after it. Matching: the card is modified
             * through a second pointer, which gives the ROM's early address of the stack slot. */
            struct DuelCard card;
            struct DuelCard *cardp;

            *(u32 *)&card = gDuel.promptResult | (&gDuel.promptResult)[1] << 16;
            cardp = &card;
            /* In a link duel on the partner's turn the picked card's owner is seen from the other side. */
            if (gDuelCtrl.isLinkDuel && gDuel.turnPlayer)
                cardp->owner = 1 - cardp->owner;
            DuelCmd_Push(PLAYER_CMD(gChain.effectSubStep, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD),
                         *(u32 *)&card & 0xFFFF, *(u32 *)&card >> 16, 0);
            switch (CARD_NUMBER(link->card)) {
            case CARD_SPEAR_CRETIN:
                QueueSpecialSummonChoosePosition(gChain.effectSubStep, &card, 0, ZONE_STATUS_FROM_GRAVEYARD);
                break;
            case CARD_THE_SHALLOW_GRAVE:
                QueueSpecialSummon(gChain.effectSubStep, &card, 0, 1, ZONE_STATUS_FROM_GRAVEYARD);
                break;
            }
            return EFFECT_STEP_4;
        }
        case EFFECT_STEP_4: {
            int turnPlayer;

            gChain.effectSubStep = 1 - gChain.effectSubStep;
            turnPlayer = gDuel.turnPlayer;
            /* FAKEMATCH: the volatile read makes the ROM's reload of the byte after the store (plain C
             * reuses the stored register and adds a zero-extension). */
            if (*(volatile u8 *)&gChain.effectSubStep != turnPlayer)
                return EFFECT_STEP_2;
            return EFFECT_STEP_END;
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Armored Glass, World Suppression, Mystic Probe, Metal Detector: queue the command that negates the card
 * class this turn (equips, the field, continuous Magic, continuous Traps) and return EFFECT_STEP_2; on the next call
 * (or when negated) refresh which spell/trap cards are disabled.
 */
int EffectNegateCardsThisTurnResolve(struct ChainEntry *link)
{
    if (gChain.effectStep == EFFECT_STEP_START && !link->negated) {
        int cmd;

        switch (CARD_NUMBER(link->card)) {
        case CARD_ARMORED_GLASS:
            cmd = PLAYER_CMD(link->player, DUEL_CMD_NEGATE_EQUIP_THIS_TURN);
            break;
        case CARD_WORLD_SUPPRESSION:
            cmd = PLAYER_CMD(link->player, DUEL_CMD_NEGATE_FIELD_THIS_TURN);
            break;
        case CARD_MYSTIC_PROBE:
            cmd = PLAYER_CMD(link->player, DUEL_CMD_NEGATE_CONT_MAGIC_THIS_TURN);
            break;
        case CARD_METAL_DETECTOR:
            cmd = PLAYER_CMD(link->player, DUEL_CMD_NEGATE_CONT_TRAP_THIS_TURN);
            break;
        default:
            goto refresh;
        }
        DuelCmd_Push(cmd, 1, 0, 0);
        return EFFECT_STEP_2;
    }
refresh:
    UpdateSpellTrapNegationArg(0);
    return EFFECT_STEP_DONE;
}

/*
 * Numinous Healer: gain 1000 LP plus 500 per Numinous Healer in your graveyard. Also the resolver of effect
 * key 1304 (no EDS card): the opponent loses 700 LP plus 300 per copy in your graveyard.
 */
int EffectNuminousHealerResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        u16 cardNo = CARD_NUMBER(link->card);

        switch (cardNo) {
        case CARD_NUMINOUS_HEALER:
            GainLifePoints(link->player, CountGraveyardCardsByNumber(link->player, cardNo) * 500 + 1000);
            break;
        case CARD_1304:
            LoseLifePoints(1 - link->player, CountGraveyardCardsByNumber(link->player, cardNo) * 300 + 700);
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * DNA Surgery, The Regulation of Tribe: EFFECT_STEP_START stores the declared monster type (targets[0], set by
 * the ChainB handler) on the card's zone. On the next call DNA Surgery re-checks the equips of all monsters,
 * whose type-restricted equips may no longer apply.
 */
int EffectDeclareTypeResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        if (gChain.effectStep == EFFECT_STEP_START) {
            if (link->numTargets == 1 && link->targets[0] != 0) {
                DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_ZONE_DECLARED_VALUE), link->zone,
                             link->targets[0], 0);
                return EFFECT_STEP_2;
            }
        } else if (CARD_NUMBER(link->card) == CARD_DNA_SURGERY) {
            int player, zone;

            for (player = 0; player <= 1; player++)
                for (zone = 0; zone <= 4; zone++)
                    DestroyInvalidEquips(player, zone);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Backup Soldier: add up to three monsters from the graveyard to the hand (the target collector of card
 * 1147 lists the non-effect monsters with ATK 1500 or less). gChain.effectSubStep counts the cards left.
 *   EFFECT_STEP_START  three cards
 *   EFFECT_STEP_2      stop if nothing is left to pick; the CPU picks at once (-> EFFECT_STEP_5), the human
 *                      gets a Yes/No box
 *   EFFECT_STEP_3      No ends the effect; Yes shows 'Select a Monster card ...'
 *   EFFECT_STEP_4      open the card list
 *   EFFECT_STEP_5      return the chosen card to the hand; repeat while cards are left
 */
int EffectBackupSoldierResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = 3;
            gChain.effectStep--;
            /* -> EFFECT_STEP_2, fall through */
        case EFFECT_STEP_2:
            if (!CollectEffectTargetsInt(link->player, CARD_BACKUP_SOLDIER, 0))
                goto done;
            if (link->player) {
                AiPickCardListEntry(link->card);
                gCardListView.cursorRow = 0;
                gCardListView.top = gAiWork.listPick;
                return EFFECT_STEP_5;
            } else {
                TextBoxOpen(EFFECT_TEXTBOX_POS, EFFECT_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT,
                            gStrPromptAddGraveMonsterToHand);
                TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
                return EFFECT_STEP_3;
            }
        case EFFECT_STEP_3:
            if (gTextBox.result == 0)
                goto done;
            TextBoxOpen(EFFECT_TEXTBOX_POS, EFFECT_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectMonsterToAddToHand);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:
            CardListView_Open(link->player, -1, CARD_BACKUP_SOLDIER, 0);
            return EFFECT_STEP_5;
        case EFFECT_STEP_5: {
            struct CardListView *view;

            /* The chosen entry is cards[top + cursorRow]. Matching: view is assigned inside the argument,
             * after link->player is loaded. */
            ReturnGraveyardCardToHand(link->player,
                CARD_NUMBER(CARD_ID11((view = &gCardListView)->cards[view->top + view->cursorRow])));
            sub_08019820(link->player, CARD_ID(view->cards[view->top + view->cursorRow]));
            if (--gChain.effectSubStep != 0)
                return EFFECT_STEP_2;
            return EFFECT_STEP_END;
        }
        }
    }
done:
    return EFFECT_STEP_DONE;
}

/*
 * Major Riot: return every monster on the field to its owner's hand; then each player, the activating player
 * first, may set one monster from the hand (DuelPrompt_TryPostSetMonster) per monster returned from their
 * side. targets[player] counts the monsters each side has left to set.
 *   EFFECT_STEP_START  return the monsters and count them
 *   EFFECT_STEP_2      the activating player sets
 *   EFFECT_STEP_3      the opponent sets
 */
int EffectMajorRiotResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            int player, zone;

            for (player = 0; player <= 1; player++) {
                link->targets[player] = 0;
                for (zone = 0; zone <= 4; zone++) {
                    struct DuelZone *z = ZONE_AT(player & 1, zone);

                    if (ZONE_HAS_CARD(z)) {
                        DuelCmd_Push(PLAYER_CMD(player, DUEL_CMD_RETURN_TO_HAND), zone, 0, 0);
                        link->targets[player]++;
                    }
                }
            }
            gChain.effectSubStep = link->player;
            return EFFECT_STEP_2;
        }
        case EFFECT_STEP_2:
            if (link->targets[link->player] != 0 && DuelPrompt_TryPostSetMonster(link->player)) {
                link->targets[link->player]--;
                return EFFECT_STEP_2;
            }
            goto opponentSets;
        case EFFECT_STEP_3:
            if (link->targets[1 - link->player] != 0 && DuelPrompt_TryPostSetMonster(1 - link->player)) {
                link->targets[1 - link->player]--;
            opponentSets:
                return EFFECT_STEP_3;
            }
            return EFFECT_STEP_4;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Ceasefire: flip every face-down defense-position monster of both players face up without its flip effect,
 * then the opponent loses 500 LP per effect monster on the field.
 */
int EffectCeasefireResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int player, zone;
        int count;

        for (player = 0; player <= 1; player++) {
            for (zone = 0; zone <= 4; zone++) {
                struct DuelZone *z = ZONE_AT(player & 1, zone);

                if (ZONE_HAS_CARD(z) && z->isDefense && !z->isFaceUp)
                    FlipFieldCard(player, zone, 0);
            }
        }
        count = 0;
        for (player = 0; player <= 1; player++) {
            for (zone = 0; zone <= 4; zone++) {
                struct DuelZone *z = ZONE_AT(player & 1, zone);
                int cardId = CARD_ID(CARD_WORD(z->card));

                if (cardId != 0 && IsEffectMonster(cardId) != 0)
                    count++;
            }
        }
        if (count > 0)
            LoseLifePoints(1 - link->player, count * 500);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Nobleman of Crossout, Nobleman of Extermination: flip the face-down target face up without its flip
 * effect, show it and banish it. If it was a flip-effect monster (Crossout) or a Trap (Extermination), all
 * copies are also banished from both players' decks. targets[1] keeps the banished card's ID.
 *   EFFECT_STEP_START  banish the target (a face-up target is left alone)
 *   EFFECT_STEP_2      banish the copies in your deck
 *   EFFECT_STEP_3      banish the copies in the opponent's deck
 *   EFFECT_STEP_4, 5   queue DUEL_CMD_SHUFFLE_DECK, one per step
 */
int EffectNoblemanResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            u8 targetPlayer;
            int targetZone;
            struct DuelZone *z;
            int cardId;
            int side;

            if (link->numTargets != 1)
                goto done;
            targetPlayer = link->targets[0];
            targetZone = link->targets[0] >> 8;
            side = targetPlayer & 1;
            z = ZONE_AT(side, targetZone);
            cardId = CARD_ID(CARD_WORD(z->card));
            if (cardId == 0)
                goto done;
            if (z->isFaceUp)
                goto done;
            link->targets[1] = cardId;
            FlipFieldCard4(targetPlayer, targetZone, 0, 0);
            ShowRevealedCard(targetPlayer, link->targets[1]);
            BanishFieldCard(targetPlayer, targetZone, 0);
            switch (CARD_NUMBER(link->card)) {
            case CARD_NOBLEMAN_OF_CROSSOUT:
                if (CARD_TYPE(link->targets[1]) > CARD_TYPE_REPTILE)  /* not a monster */
                    goto done;
                if (HasFlipEffect(CARD_NUMBER(link->targets[1]), 1) != 0
                    || HasFlipEffect(CARD_NUMBER(link->targets[1]), 0) != 0)
                    goto banishCopies;
                goto done;
            case CARD_NOBLEMAN_OF_EXTERMINATION:
                if (CARD_TYPE(link->targets[1]) != CARD_TYPE_TRAP)
                    goto done;
            banishCopies:
                return EFFECT_STEP_2;
            default:
                goto done;
            }
        }
        case EFFECT_STEP_2:
            BanishDeckCopies(link->player, CARD_NUMBER(link->targets[1]));
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:
            BanishDeckCopies(1 - link->player, CARD_NUMBER(link->targets[1]));
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
            return EFFECT_STEP_5;
        case EFFECT_STEP_5:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
            return EFFECT_STEP_6;
        }
    }
done:
    return EFFECT_STEP_DONE;
}

/*
 * Inspection: on the opponent's Standby Phase, while the card is still on the field, show a random card of
 * the opponent's hand. The cursor first hops over the hand at random (2 hops per card, cosmetic);
 * gChain.effectSubStep counts the hops and gChain.effectCounter is the shown card's hand index.
 *   EFFECT_STEP_START  set up the hops (a single card is shown at once)
 *   EFFECT_STEP_2      one hop per call, then pick the card
 *   EFFECT_STEP_9      point at the card and show it
 */
int EffectInspectionResolve(struct ChainEntry *link)
{
    if (!link->negated && link->event == RESPONSE_OPPONENT_STANDBY) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            int side = 1 & link->player;
            struct DuelPlayer *players;
            /* &gDuelPlayers[side].zones[zone] from the gDuelPlayers symbol. Matching: the ROM derives this
             * address and the players pointer below from one literal. */
            struct DuelZone *z = (struct DuelZone *)(link->zone * sizeof(struct DuelZone)
                                                     + side * sizeof(struct DuelPlayer) + (u32)gDuelPlayers
                                                     + OFFSET_OF(struct DuelPlayer, zones));

            if (!ZONE_HAS_CARD(z))
                goto done;
            players = gDuelPlayers;
            gChain.effectSubStep = players[(1 - link->player) & 1].handCount * 2;
            if (players[(1 - link->player) & 1].handCount == 1) {
                gChain.effectCounter = 0;
                return EFFECT_STEP_9;
            }
            gChain.effectStep--;
        }
        /* -> EFFECT_STEP_2, fall through */
        case EFFECT_STEP_2:
            if (gChain.effectSubStep != 0) {
                int opponent;
                int rnd;
                struct DuelPlayer *players;

                gChain.effectSubStep--;
                opponent = 1 - link->player;
                rnd = Random();
                players = gDuelPlayers;
                DuelCursor_Select(opponent, DUEL_AREA_HAND, rnd % players[(1 - link->player) & 1].handCount);
                return EFFECT_STEP_2;
            } else {
                int rnd;
                struct DuelPlayer *players;

                rnd = Random();
                players = gDuelPlayers;
                gChain.effectCounter = rnd % players[(1 - link->player) & 1].handCount;
                return EFFECT_STEP_9;
            }
        case EFFECT_STEP_9: {
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_POINT_AT_CARD), 1 - link->player,
                         gChain.effectCounter << 8 | DUEL_AREA_HAND, 0);
            ShowCardDetail(link->player,
                           CARD_ID(CARD_WORD(gDuelPlayers[(1 - link->player) & 1].hand[gChain.effectCounter])));
        }
        }
    }
done:
    return EFFECT_STEP_DONE;
}

/* Prohibition: show the declared card (targets[0], chosen by the ChainB handler) and add it to the
 * prohibited cards. */
int EffectProhibitionResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        ShowDestroyedCard(link->player, link->targets[0]);
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_ADD_PROHIBITION), link->zone, link->targets[0], 0);
    }
    return EFFECT_STEP_DONE;
}

/* Level of a card of the given type: 0 for Trap, Magic and Ticket cards, 10 for the Divine cards, else the
 * stats' level. */
static inline u32 GetCardLevel(int type, int cardId)
{
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS(cardId));
    }
}
/* gDuelPlayers index of a player byte. */
static inline int PlayerIndex(int player) { return (u8)player & 1; }
/* Number of cards in the player's hand. */
static inline int HandCount(int player) { return gDuelPlayers[(u8)player & 1].handCount; }

/*
 * gChain.effectCard and gChain.effectSubStep through the alias symbol gChainEffectWork (= &gChain.effectStep).
 * Matching: the ROM derives these addresses from the one gChainEffectWork literal (subs r5, r1, #7).
 */
#define EFFECT_CARD_WORD    (*(u32 *)((u32)gChainEffectWork + 8))   /* gChain.effectCard */
#define EFFECT_CARD_LO      (*(u16 *)((u32)gChainEffectWork + 8))   /* its low halfword */
#define EFFECT_CARD_HI      (*(u16 *)((u32)gChainEffectWork + 10))  /* its high halfword */
#define EFFECT_SIDE         (*(u8 *)((u32)gChainEffectWork + 1))    /* gChain.effectSubStep */

/*
 * Morphing Jar #2: return all monsters on the field to their owners' decks (shuffled); then each player,
 * the activating player first, turns up cards from the top of the deck until as many monsters have come up
 * as they returned. A monster of level 4 or lower that can be Normal Summoned is Special Summoned face down
 * in defense position; every other card goes to the graveyard. A Parasite Paracide that the other player
 * planted in the deck is summoned on its owner's side instead. targets[player] counts the monsters still
 * to turn up; gChain.effectSubStep is the side digging and gChain.effectCard the card turned up.
 *   EFFECT_STEP_START  count the monsters per side and return them
 *   EFFECT_STEP_2      draw the top card (or switch sides; done when both sides are through)
 *   EFFECT_STEP_3      decide what happens to the drawn card
 *   EFFECT_STEP_4      summon a planted Parasite Paracide for its owner
 *   EFFECT_STEP_5      summon the drawn monster
 */
int EffectMorphingJar2Resolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            int player, zone;

            for (player = 0; player <= 1; player++) {
                link->targets[player] = 0;
                for (zone = 0; zone <= 4; zone++) {
                    u16 cardId = CARD_ID(CARD_WORD(gDuelPlayers[player & 1].zones[zone].card));

                    if (cardId != 0 && CARD_TYPE(cardId) <= CARD_TYPE_REPTILE)
                        link->targets[player]++;
                }
            }
            ReturnAllMonstersToDeck(link->player, 1);
            gChain.effectSubStep = link->player;
        nextCard:
            return EFFECT_STEP_2;
        }
        case EFFECT_STEP_2: {
            if (gDuelPlayers[PlayerIndex(gChain.effectSubStep)].deckCount != 0
                && link->targets[gChain.effectSubStep] != 0) {
                struct DuelCard *deck = gDuelPlayers[PlayerIndex(gChain.effectSubStep)].deck;

                CopyDuelCard((u32 *)&gChain.effectCard, (u32 *)deck);
                DuelCmd_Push(PLAYER_CMD(gChain.effectSubStep, DUEL_CMD_DRAW_CARDS), 1, 1, 0);
                ShowRevealedCard(gChain.effectSubStep, CARD_ID(CARD_WORD(*deck)));
                return EFFECT_STEP_3;
            }
            gChain.effectSubStep = 1 - gChain.effectSubStep;
            /* FAKEMATCH: the memory barrier makes the ROM's reload of effectSubStep after the store */
            asm("" ::: "memory");
            if (gChain.effectSubStep != link->player)
                goto nextCard;
            return EFFECT_STEP_9;
        }
        case EFFECT_STEP_3: {
            int side;
            u32 word;

            /* bit 12 owner, bit 17 planted (struct DuelCard) */
            word = EFFECT_CARD_WORD;
            if ((int)((word << 19) >> 31) != (side = EFFECT_SIDE) && (int)(word << 14) < 0
                && CARD_NUMBER(CARD_ID11(word)) == CARD_PARASITE_PARACIDE) {
                if (CountFreeMonsterZones(1 - side) > 0) {
                    DuelCmd_Push(PLAYER_CMD(EFFECT_SIDE, DUEL_CMD_REMOVE_CARD_FROM_HAND), EFFECT_CARD_LO,
                                 EFFECT_CARD_HI, 0);
                    return EFFECT_STEP_4;
                }
                /* the drawn card is the last one in the hand */
                DiscardHandCard(EFFECT_SIDE, HandCount(EFFECT_SIDE) - 1, 0, 1);
                /* FAKEMATCH: the empty asm keeps this call; without it the compiler merges it with the
                 * DiscardHandCard call below (cross-jump), 8 bytes shorter than the ROM. */
                asm("");
                goto nextCard;
            }
            {
                int cardId = CARD_ID(EFFECT_CARD_WORD);
                u32 type = CARD_TYPE(cardId);

                if (type <= CARD_TYPE_REPTILE) {
                    if (GetCardLevel(type, cardId) <= 4 && IsSpecialSummonOnly(cardId) == 0) {
                        u16 *halves = &EFFECT_CARD_LO;

                        DuelCmd_Push(PLAYER_CMD(EFFECT_SIDE, DUEL_CMD_REMOVE_CARD_FROM_HAND), halves[0], halves[1],
                                     0);
                        /* Matching: the same byte as gChain.effectSubStep, through the alias */
                        link->targets[EFFECT_SIDE]--;
                        return EFFECT_STEP_5;
                    }
                    link->targets[gChain.effectSubStep]--;
                }
            }
            {
                int digger = gChain.effectSubStep;

                /* byOpponentEffect: the digging side is not Morphing Jar #2's controller */
                DiscardHandCard(digger, HandCount(digger) - 1, link->player != digger, 1);
            }
            goto nextCard;
        }
        case EFFECT_STEP_4:
            QueueSpecialSummon(1 - gChain.effectSubStep, &gChain.effectCard, 0, 1, 0);
            goto nextCard;
        case EFFECT_STEP_5:
            QueueSpecialSummon(gChain.effectSubStep, &gChain.effectCard, 0, 1, 0);
            goto nextCard;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Windstorm of Etaqua: change the battle position of every face-up monster of the opponent. While Dragon
 * Capture Jar is active on either side, Dragons in defense position stay (the Jar keeps them there).
 */
int EffectWindstormOfEtaquaResolve(struct ChainEntry *link)
{
    int dragonCaptureJar = 0;

    if (!link->negated) {
        int zone;

        if (CountActiveCardsOnField(0, CARD_DRAGON_CAPTURE_JAR) != 0
            || CountActiveCardsOnField(1, CARD_DRAGON_CAPTURE_JAR) != 0)
            dragonCaptureJar = 1;
        for (zone = 0; zone <= 4; zone++) {
            int side = (1 - link->player) & 1;
            struct DuelZone *z = ZONE_AT(side, zone);

            /* Matching: the card word and the position byte are read through separate zone pointers. */
            if (ZONE_HAS_CARD(z)) {
                struct DuelZone *z2 = ZONE_AT((1 - link->player) & 1, zone);

                if (z2->isFaceUp) {
                    u8 change = 1;

                    if (dragonCaptureJar) {
                        struct DuelZone *z3 = ZONE_AT((1 - link->player) & 1, zone);

                        if (z3->isDefense)
                            change = GetZoneCardType(1 - link->player, zone) != CARD_TYPE_DRAGON;
                    }
                    if (change)
                        ChangeBattlePosition(1 - link->player, zone, 0, 0);
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Sebek's Blessing: gain LP equal to the battle damage your monster dealt (the event's loc0). */
int EffectSebeksBlessingResolve(struct ChainEntry *link)
{
    if (!link->negated)
        GainLifePoints(link->player, link->loc0);
    return EFFECT_STEP_DONE;
}

/*
 * Riryoku: with both targets face up, halve the first target's ATK until the end of the turn and add that
 * amount (rounded up) to the second target (a ZONE_LINK_ATK_BONUS link).
 */
int EffectRiryokuResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 2) {
        int fromPlayer = (u8)link->targets[0];
        int fromZone = link->targets[0] >> 8;
        int toPlayer = (u8)link->targets[1];
        int toZone = link->targets[1] >> 8;
        int fromSide = fromPlayer & 1;
        struct DuelZone *from = ZONE_AT(fromSide, fromZone);

        if (ZONE_HAS_CARD(from) && from->isFaceUp) {
            int toSide = toPlayer & 1;
            struct DuelZone *to = ZONE_AT(toSide, toZone);

            if (ZONE_HAS_CARD(to) && to->isFaceUp) {
                DuelCmd_Push(PLAYER_CMD(fromPlayer, DUEL_CMD_HALVE_ATTACK), fromZone, 0, 0);
                QueueAddZoneLinkInt(link->player, HalveRoundUp(GetZoneCardAtk(fromPlayer, fromZone)), link->targets[1],
                                    ZONE_LINK_ATK_BONUS);
            }
        }
    }
    return EFFECT_STEP_DONE;
}
