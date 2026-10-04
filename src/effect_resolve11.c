/*
 * effect_resolve11 (0x0803B670-0x0803C837): resolve handlers of the effect keys 1448-1545
 * (wiki/functions/effect-resolve11-c.md). The keys 1546-1551 and Polymerization follow in effect_fusion.c.
 *
 * Every function here is the resolve slot (+0x04) of a gCardEffects row (struct CardEffect, include/effect.h).
 * None of these keys is an EDS card (constants/cards.h names them CARD_<number>), so the handlers are named
 * after what they do; where a key behaves like an OCG card, the handler comment names it (hypothesis).
 *
 * Chain_Resolve calls a handler with gChain.effectStep = EFFECT_STEP_START (0x80), stores the value it returns
 * in gChain.effectStep and calls it again next frame, until it returns EFFECT_STEP_DONE (0). The multi-step
 * handlers count down from 0x80; a step waits for a Yes/No answer, a pick in the card list or a cursor pick.
 * A returned step that the handler has no case for (0x78, 0x0A) ends the link on the next call.
 * gChain.effectSubStep and gChain.effectCounter are the handlers' scratch bytes.
 *
 * Most handlers do nothing when link->negated is set. link->targets[] holds the positions player | zone << 8
 * that the chainB handler chose; the handlers test the target again, because the board can change before the
 * chain resolves. Player 1 is the CPU or the link partner: a duel command that acts for player 1 carries
 * DUEL_CMD_PLAYER.
 */
#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_STATS_* field macros */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType, enum SpellSubtype */
#include "constants/duel.h"         /* zones, ZoneLinkKind, ZoneStatusFlag, ChainEntryKind, DuelPromptKind, PICK_* */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* enum SoundEffect */

#include "duel.h"                /* duel state, zones, players */
#include "sound.h"               /* PlaySE */
#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain, Chain_AddPending */
#include "duel_actions.h"           /* field actions: EquipCard, TributeMonster, ChangeBattlePosition, ... */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_flow.h"              /* gDuelCtrl */
#include "duel_prompt.h"            /* DuelPrompt_Post */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget */
#include "effect.h"                 /* enum EffectStep, CanActivateEffect, DestroyFieldCardByEffect, prompt texts */
#include "effect_handlers.h"        /* the handlers defined here and the check handlers they call */
#include "summon.h"                 /* gSummonAction, QueueSpecialSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */

/* Local views kept for matching (build/readability/HEADERS.md, "Keeping a deliberate local view"). */
/* Matching: CollectEffectTargets is defined with a u16 return; these handlers use the count as an int (no
 * narrowing of r0 after the call; key 1513 tests it with <= 0). */
extern int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: IsTributableMonster is defined with a u16 return; key 1512 tests the result as an int (no lsl #16
 * before the compare). */
extern int IsTributableMonsterInt(int player, int zone) asm("IsTributableMonster");
/* Matching: IsValidEquipTarget is defined with a u16 return; key 1527 tests the result as an int. */
extern int IsValidEquipTargetInt(int equipPlayer, int equipSlot, int targetPlayer, int targetSlot)
    asm("IsValidEquipTarget");
/* Matching: CanActivateEffect is defined with an int return; key 1545 calls it through a u16 view (the
 * result is narrowed before the test). */
extern u16 CanActivateEffectU16(struct ChainEntry *card, struct ChainEntry *chainLink, u16 fromHand)
    asm("CanActivateEffect");
/* Matching: these take a u16 card ID; key 1545 passes its u32 id with no narrowing, which keeps the ROM's
 * register allocation. */
extern void ShowRevealedCardInt(int player, int cardId) asm("ShowRevealedCard");
extern void ShowDestroyedCardInt(int player, int cardId) asm("ShowDestroyedCard");
extern void ShowActivatedCardInt(int player, int cardId) asm("ShowActivatedCard");

/* The command id for the acting player: player 1's commands carry DUEL_CMD_PLAYER. */
#define PLAYER_CMD(isPlayer1, cmd) ((isPlayer1) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

/* A packed location, player term first. Matching: DUEL_LOC (constants/duel.h) puts the zone term first, which
 * changes the operand order of the orr. */
#define LOC(player, zone) ((player) | (zone) << 8)

/* The card word of a zone as one u32. Matching: the ROM loads the whole word (ldr) and extracts the ID with
 * lsl #20; lsr #20; a bitfield read of .id loads only the halfword. */
#define CARD_WORD(card) (*(u32 *)&(card))
#define CARD_ID(word) (((word) << 20) >> 20)
#define ZONE_HAS_CARD(zone) (CARD_WORD((zone)->card) << 20 != 0)

/* gCardStats (0x08621DE0) and gCardIdToNumber (0x08622AB4) through integer-constant addresses. Matching: the
 * ROM reloads the table address at each use, which the symbols do not give. */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_TYPE(id) CARD_STATS_TYPE(CARD_STATS(id))     /* enum CardType; <= 20 is a monster */

/* &gDuelZones[player].zones[zone] by byte arithmetic. Matching: the ROM adds the zone term, then the player
 * term, then the gDuelZones literal. Callers pass player & 1. */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))

/* Text box rectangles: TextBoxOpen takes x | y << 8 and width | height << 8, in cells. */
#define TEXTBOX_XY(x, y) ((x) | (y) << 8)

/* The level of a card for the level comparisons: Trap, Magic and Ticket cards count 0, Divine-Beasts 10. */
#define GET_CARD_LEVEL(id, level)                               \
    switch ((int)CARD_TYPE(id)) {                               \
    case CARD_TYPE_TRAP:                                        \
    case CARD_TYPE_MAGIC:                                       \
    case CARD_TYPE_TICKET:                                      \
        level = 0;                                              \
        break;                                                  \
    case CARD_TYPE_DIVINE:                                      \
        level = 10;                                             \
        break;                                                  \
    default:                                                    \
        level = CARD_STATS_LEVEL(CARD_STATS(id));               \
        break;                                                  \
    }

/* gChain.scratch.effect.effectCards[0] (0x02017F84) as two halfwords, the DuelCmd_Push operands. */
#define EFFECT_CARD_HALF(i) (((u16 *)gChain.scratch.effect.effectCards)[i])

/* gDuel as bytes, for step 3 of EffectOpponentGraveSummonResolve. Matching: the ROM forms gDuel + offset from
 * one base register; the struct members fold into one literal per field. */
extern u8 gDuelBytes[] asm("gDuel");
#define DUEL_TURN_FLAGS_OFFSET 0x1B12       /* the byte holding gDuel.bgmOn (bit 0) and gDuel.turnPlayer (bit 1) */
#define DUEL_TURN_PLAYER_BIT   0x2

/*
 * Key 1448 (an Equip Magic that declares an Attribute): store the declared value (targets[0], the
 * Attribute + 1 that the chainB prompt chose) in this card's zone (GetZoneCardStats reads it as the equipped
 * monster's Attribute) and equip the card to targets[1]. No negated test.
 */
int EffectEquipSetAttributeResolve(struct ChainEntry *link)
{
    if (link->numTargets == 2) {
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_ZONE_DECLARED_VALUE), link->zone,
                     link->targets[0], 0);
        EquipCard(link->player, LOC(link->player, link->zone), link->targets[1]);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1510: destroy every Special Summoned monster on the field (EffectSpecialSummonedMonsterCheck), one per
 * call: the opponent's side first, then the player's own. Then neither player may Special Summon this turn
 * (DUEL_CMD_SET_SUMMON_LOCKS keeps each player's Normal Summon lock and sets the Special Summon lock).
 *   START  start with the opponent's side (gChain.effectSubStep).
 *   2      destroy the first matching monster of the side and come back; when the side has none, switch
 *          sides, and go on to the end once both sides are clear.
 */
int EffectDestroySpecialSummonedResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = 1 - link->player;
            gChain.effectStep--;
            /* fall through */
        case EFFECT_STEP_2: {
            int zone;
            int player;

            for (zone = ZONE_MONSTER_0; zone <= ZONE_MONSTER_4; zone++) {
                if (EffectSpecialSummonedMonsterCheck(link, (u8)zone << 8 | gChain.effectSubStep) != 0) {
                    DestroyFieldCardByEffect(gChain.effectSubStep, zone);
                    OnCardDestroyedByEffect(link->player, gChain.effectSubStep, zone);
                    return EFFECT_STEP_2;
                }
            }
            gChain.effectSubStep = 1 - gChain.effectSubStep;
            player = link->player;  /* Matching: compared through a local copy */
            if (gChain.effectSubStep == player)
                return EFFECT_STEP_2;
        }
            /* fall through: both sides are clear */
        default:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_SUMMON_LOCKS),
                         gDuelPlayers[1 & link->player].noNormalSummon, 1, 0);
            DuelCmd_Push(PLAYER_CMD(!link->player, DUEL_CMD_SET_SUMMON_LOCKS),
                         gDuelPlayers[(1 - link->player) & 1].noNormalSummon, 1, 0);
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1511 (OCG Kycoo the Ghost Destroyer by behaviour, hypothesis): banish up to 2 monsters from the
 * opponent's graveyard, asking before each one. gChain.effectSubStep counts the picks left.
 *   START  2 picks.
 *   2      no picks left or no monster in the opponent's graveyard: done. Else ask Yes/No.
 *   3      No: done. Yes: prompt for the pick.
 *   4      open the list of the opponent's graveyard monsters.
 *   5      banish the picked card (for the opponent) and go back to 2.
 */
int EffectBanishOpponentGraveMonstersResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = 2;
            gChain.effectStep--;
            /* fall through */
        case EFFECT_STEP_2:
            if (gChain.effectSubStep == 0)
                return EFFECT_STEP_DONE;
            if (CollectEffectTargetsInt(link->player, CARD_1511, 0) == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT,
                        gChain.effectSubStep == 2 ? gStrBanishOpponentGraveMonsterQuestion
                                                  : gStrBanishAnotherOpponentGraveMonsterQuestion);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:
            if (gTextBox.result == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT,
                        gStrSelectOpponentGraveMonsterToBanish);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return EFFECT_STEP_5;
        case EFFECT_STEP_5: {
            /* the picked card word as its two halves (the DuelCmd_Push operands) */
            u16 *picked = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow];

            DuelCmd_Push(PLAYER_CMD(!link->player, DUEL_CMD_BANISH_GRAVEYARD_CARD), picked[0], picked[1], 0);
            gChain.effectSubStep--;
            return EFFECT_STEP_START;
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1512 (a FLIP effect): the human player tributes one of their other monsters and Special Summons a
 * monster from the Fusion Deck. The summoned monster gets a ZONE_LINK_CARD_EFFECT link to this card, by which
 * DuelCmd_TurnEnd destroys it at the end of the turn. Player 1 does nothing.
 *   START  needs a Fusion Deck card and another monster that can be tributed; prompt for the tribute.
 *   2      cursor pick on the player's monsters; a pick that is this card or cannot be tributed buzzes.
 *   3      prompt for the Fusion Deck pick.
 *   4      open the list of the Fusion Deck.
 *   5      take the picked card out of the Fusion Deck.
 *   6      Special Summon it, choosing the position.
 *   7      link it to this card (zone from gSummonAction); returns 0x78, which ends the link.
 */
int EffectTributeToSummonFusionResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            if (link->player)
                return EFFECT_STEP_DONE;
            if (CollectEffectTargetsInt(link->player, CARD_1512, 0) == 0)
                return EFFECT_STEP_DONE;
            if (CountTributableMonsters(link->player, link->zone) == 0)
                return EFFECT_STEP_DONE;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrDesignateOwnMonsterToTribute);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:
            if (DuelCursor_PickTarget(PICK_ANY_MONSTER) != 0) {
                /* The picked zone, gDuelScreen.selIndex, through a byte pointer. Matching: the ROM adds the
                 * offset to the gDuelScreen literal; &gDuelScreen.selIndex folds into one literal. */
                u8 *screen = (u8 *)&gDuelScreen;
                u32 *selIndex = (u32 *)(screen + OFFSET_OF(struct DuelScreen, selIndex));

                if (*selIndex != link->zone && IsTributableMonsterInt(link->player, *selIndex) != 0) {
                    TributeMonster(link->player, *selIndex);
                    return EFFECT_STEP_3;
                }
                PlaySE(SE_ERROR);
            }
            return EFFECT_STEP_2;
        case EFFECT_STEP_3:
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrSelectFusionToSummonForTribute);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return EFFECT_STEP_5;
        case EFFECT_STEP_5: {
            u16 *picked = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow];

            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_REMOVE_CARD_FROM_FUSION_DECK), picked[0],
                         picked[1], 0);
            return EFFECT_STEP_6;
        }
        case EFFECT_STEP_6:
            QueueSpecialSummonChoosePosition(link->player,
                (struct DuelCard *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow], 1, 0);
            return EFFECT_STEP_7;
        case EFFECT_STEP_7:
            /* gSummonAction.zone: the monster zone the queued summon uses */
            QueueAddZoneLink(link->player, link->card, LOC(link->player, gSummonAction.zone), ZONE_LINK_CARD_EFFECT);
            return EFFECT_STEP_9;   /* no case: ends the link */
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1513 (OCG Bazoo the Soul-Eater by behaviour, hypothesis): banish up to 3 monsters from the player's
 * graveyard, asking before each one; this card gains 300 ATK per banished card (a ZONE_LINK_ATK_300_PER_VALUE
 * link with the count as its value). Once per turn: the effect is marked used at the end.
 * gChain.effectSubStep counts the picks left, gChain.effectCounter the cards banished.
 *   START  3 picks, none made.
 *   2      no graveyard monster left: go to 9. Else ask Yes/No ("a card" first, then "another card").
 *   3      No: go to 9. Yes: prompt for the pick.
 *   4      open the list of the graveyard monsters.
 *   5      banish the picked card (for its owner, card word bit 12); back to 2, or to 9 after 3 picks.
 *   9      add the ATK link if any card was banished, then (also from any other step) mark the effect used.
 */
int EffectBanishGraveForAtkResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            gChain.effectSubStep = 3;
            gChain.effectCounter = 0;
            gChain.effectStep--;
            /* fall through */
        case EFFECT_STEP_2:
            if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) <= 0)
                goto finish;
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT,
                        gChain.effectCounter != 0 ? gStrBanishAnotherGraveCardQuestion : gStrBanishGraveCardQuestion);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:
            if (gTextBox.result == 0) {
                /* Matching: the shared exit sits here, inside the No branch, as in the ROM. */
finish:
                return EFFECT_STEP_9;
            }
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, gStrSelectGraveCardToBanishForAtk);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return EFFECT_STEP_5;
        case EFFECT_STEP_5: {
            u16 *picked = (u16 *)&gCardListView.cards[gCardListView.cursorRow + gCardListView.top];

            /* the command acts for the card's owner: bit 12 of the card word (sign bit after << 19) */
            DuelCmd_Push((int)(gCardListView.cards[gCardListView.cursorRow + gCardListView.top] << 19) < 0
                             ? DUEL_CMD_PLAYER | DUEL_CMD_BANISH_GRAVEYARD_CARD
                             : DUEL_CMD_BANISH_GRAVEYARD_CARD,
                         picked[0], picked[1], 0);
            gChain.effectSubStep--;
            gChain.effectCounter++;
            if (gChain.effectSubStep == 0)
                goto finish;
            return EFFECT_STEP_2;
        }
        case EFFECT_STEP_9:
            if (gChain.effectCounter != 0)
                QueueAddZoneLink(link->player, link->card, LOC(link->player, link->zone),
                                 gChain.effectCounter << 8 | ZONE_LINK_ATK_300_PER_VALUE);
            /* fall through */
        default:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1514 (OCG Dark Necrofear by behaviour, hypothesis; OnCardDestroyedByEffect arms it when the opponent's
 * effect destroys it): from the graveyard, this card is placed face up in a free spell/trap zone as an equip
 * of the targeted face-up opponent monster, and the player takes control of that monster (MoveFieldCard to a
 * free monster zone, skipped when negated). Needs the card in the graveyard and a free spell/trap and monster
 * zone. The graveyard card word is copied to gChain.scratch.effect.effectCards[0] for the two commands.
 */
int EffectEquipFromGraveTakeControlResolve(struct ChainEntry *link)
{
    if (CountGraveyardCardsByNumber(link->player, CARD_1514) != 0 && FindFreeSpellTrapZone(link->player) != -1
        && FindFreeMonsterZone(link->player) != -1 && link->numTargets == 1) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = link->targets[0] >> 8;
        int monsterZone;
        /* FAKEMATCH: an extra use of targetPlayer raises its global-alloc priority above the 0x77/0x8077
         * ternary temp, so targetPlayer gets r7 and the ternary falls to ip as in the ROM */
        asm("" :: "r"(targetPlayer));
        monsterZone = FindFreeMonsterZone(link->player);

        if (targetPlayer != link->player) {
            int side = targetPlayer & 1;
            struct DuelZone *target = ZONE_AT(side, targetZone);

            if (ZONE_HAS_CARD(target) && target->isFaceUp) {
                int spellZone = FindFreeSpellTrapZone(link->player);

                GetGraveyardCardById(link->player, link->card, gChain.scratch.effect.effectCards);
                DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD),
                             EFFECT_CARD_HALF(0), EFFECT_CARD_HALF(1), 0);
                /* arg2: the zone, face up (bit 8) */
                DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_PLACE_CARD), (u8)spellZone | 0x100,
                             EFFECT_CARD_HALF(0), EFFECT_CARD_HALF(1));
                EquipCard(link->player, LOC(link->player, (u8)spellZone), LOC(targetPlayer, targetZone));
                if (!link->negated && monsterZone != -1)
                    MoveFieldCard(link->player, link->targets[0], LOC(link->player, (u8)monsterZone));
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Keys 1517 and 1519: the player picks a face-up monster of the opponent; it changes its battle position
 * and is locked in it (DUEL_CMD_SET_POSITION_LOCKED, for the opponent).
 *   START  prompt.
 *   2      cursor pick on the opponent's face-up monsters; then the commands, and step 3 ends the link.
 */
int EffectChangeOpponentPositionResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT,
                        gStrSelectOpponentMonsterToChangePosition);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:
            if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION))
                != 0) {
                /* Matching: the command id before the gDuelScreen address gives the ROM's literal-pool order. */
                u16 cmd = PLAYER_CMD(!link->player, DUEL_CMD_SET_POSITION_LOCKED);
                u8 *screen = (u8 *)&gDuelScreen;
                u32 *selIndex = (u32 *)(screen + OFFSET_OF(struct DuelScreen, selIndex));  /* as in key 1512 */

                DuelCmd_Push(cmd, *(u16 *)selIndex, 1, 0);  /* arg2: the zone (low half), arg4: locked */
                ChangeBattlePosition(1 - link->player, *selIndex, 0, 0);
                return EFFECT_STEP_3;   /* no case: ends the link */
            }
            return EFFECT_STEP_2;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1520: the opponent may Special Summon a monster from their own graveyard (the Call of the Haunted
 * candidates), through two duel prompts answered by the opponent.
 *   START  needs a candidate, a free monster zone and Special Summons allowed; asks the opponent Yes/No.
 *   2      No: done. Yes: the opponent picks the graveyard monster (PROMPT_SELECT_GRAVEYARD_MONSTER).
 *   3      take the picked card word (gDuel.promptResult and the halfword after it) out of the graveyard
 *          and Special Summon it, choosing the position; step 4 ends the link. In a link duel on player
 *          1's turn the owner bit of the word is flipped for the removal command and back for the summon.
 */
int EffectOpponentGraveSummonResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            if (CollectEffectTargetsInt(1 - link->player, CARD_CALL_OF_THE_HAUNTED, 0) == 0)
                return EFFECT_STEP_DONE;
            if (CountFreeMonsterZones(1 - link->player) == 0)
                return EFFECT_STEP_DONE;
            if (CanSpecialSummon(1 - link->player) == 0)
                return EFFECT_STEP_DONE;
            DuelPrompt_Post(1 - link->player, PROMPT_CONFIRM_GRAVEYARD_SUMMON, 0, 0);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:
            if (gDuel.promptResult == 0)
                return EFFECT_STEP_DONE;
            DuelPrompt_Post(1 - link->player, PROMPT_SELECT_GRAVEYARD_MONSTER, link->card, 0);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3: {
            struct DuelCard card;
            struct DuelCard *cardPtr;
            u8 *duel = gDuelBytes;

            CARD_WORD(card) = *(u16 *)(duel + OFFSET_OF(struct DuelState, promptResult))
                            | *(u16 *)(duel + OFFSET_OF(struct DuelState, promptResult) + 2) << 16;
            cardPtr = &card;    /* Matching: the owner flips go through a pointer */
            /* a link duel on player 1's turn */
            if (gDuelCtrl.isLinkDuel && (duel[DUEL_TURN_FLAGS_OFFSET] & DUEL_TURN_PLAYER_BIT))
                cardPtr->owner = 1 - cardPtr->owner;
            DuelCmd_Push(PLAYER_CMD(!link->player, DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD),
                         CARD_WORD(card) & 0xFFFF, CARD_WORD(card) >> 16, 0);
            if (gDuelCtrl.isLinkDuel && (gDuelBytes[DUEL_TURN_FLAGS_OFFSET] & DUEL_TURN_PLAYER_BIT))
                cardPtr->owner = 1 - cardPtr->owner;
            QueueSpecialSummonChoosePosition(1 - link->player, &card, 1, ZONE_STATUS_FROM_GRAVEYARD);
            return EFFECT_STEP_4;
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/* Key 1521 (a FLIP effect): return both targeted cards (two spell/trap cards) to their owners' hands. */
int EffectReturnTwoCardsToHandResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 2) {
        int i;

        for (i = 0; i < link->numTargets; i++) {
            int targetPlayer = (u8)link->targets[i];
            int targetZone = link->targets[i] >> 8;
            int side = targetPlayer & 1;
            struct DuelZone *target = ZONE_AT(side, targetZone);

            if (ZONE_HAS_CARD(target))
                ReturnFieldCardToHand(targetPlayer, targetZone, 1);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1524 (a FLIP effect): add two monsters from the player's graveyard that were used as Fusion materials
 * (card word bit 20) to the hand. gChain.effectSubStep counts the picks left.
 *   START  needs at least 2 candidates; 2 picks.
 *   2      prompt.
 *   3      open the list of the candidates.
 *   4      return the picked card to the hand; back to 2, or return 0x0A (ends the link) after the second.
 */
int EffectReturnFusionMaterialsToHandResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            int count = CollectEffectTargetsInt(link->player, CARD_1524, 0);

            gChain.effectSubStep = count;
            if ((u8)count <= 1)
                return EFFECT_STEP_DONE;
            gChain.effectSubStep = 2;
            gChain.effectStep--;
        }
            /* fall through */
        case EFFECT_STEP_2:
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(19, 6), TEXTBOX_FLAGS_DEFAULT, gStrSelectFusionMaterialToAddToHand);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:
            CardListView_Open(link->player, -1, CARD_NUMBER(link->card), 0);
            return EFFECT_STEP_4;
        case EFFECT_STEP_4: {
            u16 *picked = (u16 *)&gCardListView.cards[gCardListView.top + gCardListView.cursorRow];

            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND), picked[0],
                         picked[1], 0);
            gChain.effectSubStep--;
            if (gChain.effectSubStep != 0)
                return EFFECT_STEP_2;
            return 0x0A;    /* no case: ends the link */
        }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1525: tribute this monster to negate the Magic card it answers (chainedTo, whose first halfword is the
 * card ID) and destroy it. Nothing happens while key 1418 (no tributes) is active on either field.
 */
int EffectTributeNegateMagicResolve(struct ChainEntry *link, u16 *chainedTo)
{
    if (CountActiveCardsOnField(0, CARD_1418) <= 0 && CountActiveCardsOnField(1, CARD_1418) <= 0) {
        TributeMonster(link->player, link->zone);
        if (chainedTo != NULL && CARD_TYPE(*chainedTo) == CARD_TYPE_MAGIC)
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_NEGATE_ACTIVATION), 1, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1527: move every face-up Equip card on the field (both players' spell/trap zones) to the targeted
 * monster. An equip that EffectTailorOfTheFickleCheck refuses or that may not equip the target is destroyed.
 */
int EffectMoveAllEquipsResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        int player;

        for (player = 0; player <= 1; player++) {
            int zone;

            for (zone = ZONE_SPELL_0; zone <= ZONE_SPELL_4; zone++) {
                /* Matching: the copies of zone and player, the u8 side and the u8 subtype shape the ROM's
                 * register allocation (wiki: "Opponent-zone search matched"). */
                int zoneCopy = zone;
                int playerCopy = player;
                int targetPlayer = (u8)link->targets[0];
                int targetZone = link->targets[0] >> 8;
                int canMove = TRUE;
                u8 side = player & 1;
                struct DuelZone *equip = ZONE_AT(side, zone);
                u16 id = CARD_ID(CARD_WORD(equip->card));

                if (id != 0 && equip->isFaceUp) {
                    u8 subtype;

                    switch ((int)CARD_TYPE(id)) {
                    case CARD_TYPE_TRAP:
                    case CARD_TYPE_MAGIC:
                        subtype = CARD_STATS_SUBTYPE(CARD_STATS(id));
                        break;
                    default:
                        subtype = 0;    /* not a Magic/Trap card */
                        break;
                    }
                    if (subtype == SPELL_EQUIP) {
                        if (EffectTailorOfTheFickleCheck(link, (u8)playerCopy | (u8)zoneCopy << 8) == 0)
                            canMove = FALSE;
                        if (IsValidEquipTargetInt(playerCopy, zoneCopy, targetPlayer, targetZone) == 0)
                            canMove = FALSE;
                        if (canMove)
                            MoveEquipCard((u8)player | (u8)zone << 8, link->targets[0]);
                        else
                            DestroyFieldCard(player, zone, 1);
                    }
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1529 (answers an attack declaration): the targeted face-up monster of the opponent attacks in place of
 * the current attacker (link->loc0). The old attacker is marked as having attacked, the new one is pointed at,
 * switched to Attack Position if needed, and set as the attacker with the battle steps restarted.
 */
int EffectSwitchAttackerResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 1) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = link->targets[0] >> 8;
        int side = 1 & targetPlayer;
        struct DuelZone *target = ZONE_AT(side, targetZone);

        if (target->isFaceUp && ZONE_HAS_CARD(target)) {
            DuelCmd_Push(PLAYER_CMD((u8)link->loc0 != 0, DUEL_CMD_MARK_ATTACKED), link->loc0 >> 8, 1, 0);
            /* arg2: player, arg4: area (the monster row, 0) | index << 8 */
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_POINT_AT_CARD), (u8)link->targets[0],
                         (link->targets[0] >> 8) << 8, 0);
            if (target->isDefense)
                ChangeBattlePosition(targetPlayer, targetZone, 0, 0);
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_ATTACKER), link->targets[0], 1, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1532: destroy the targeted face-up monster if its level still equals targets[1], the number of
 * graveyard monsters the chainB handler banished for it.
 */
int EffectBanishGraveToDestroyResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 2) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = link->targets[0] >> 8;
        u16 banishedCount = link->targets[1];
        int side = 1 & targetPlayer;
        struct DuelZone *target = ZONE_AT(side, targetZone);
        u32 id = CARD_ID(CARD_WORD(target->card));

        if (target->isFaceUp && id != 0) {
            u32 level;

            GET_CARD_LEVEL(id, level);
            if (level == banishedCount) {
                DestroyFieldCardByEffect(targetPlayer, targetZone);
                OnCardDestroyedByEffect(link->player, targetPlayer, targetZone);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1535 (answers battle damage taken by the opponent's attacker against a defense-position monster):
 * destroy that attacker. The low byte of link->loc1 holds its player (bits 0-3) and zone (bits 4-7).
 */
int EffectDestroyRepelledAttackerResolve(struct ChainEntry *link)
{
    u8 attacker = ((u8 *)link)[8];     /* low byte of loc1 */
    int player = attacker & 0xF;
    int zone = attacker >> 4;

    if (!link->negated) {
        int side = player & 1;
        struct DuelZone *target = ZONE_AT(side, zone);

        if (ZONE_HAS_CARD(target)) {
            DestroyFieldCardByEffect(player, zone);
            OnCardDestroyedByEffect(link->player, player, zone);
        }
    }
    return EFFECT_STEP_DONE;
}

/* Key 1539: return the targeted Magic/Trap card of the opponent to their hand. No negated test. */
int EffectReturnOpponentSpellTrapResolve(struct ChainEntry *link)
{
    if (link->numTargets == 1) {
        int targetPlayer = (u8)link->targets[0];
        int targetZone = link->targets[0] >> 8;
        int side = 1 & targetPlayer;
        struct DuelZone *target = ZONE_AT(side, targetZone);

        if (ZONE_HAS_CARD(target) && targetPlayer != link->player)
            ReturnFieldCardToHand(targetPlayer, targetZone, 1);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Key 1545 (OCG Bait Doll by behaviour, hypothesis): flip the targeted face-down spell/trap card face up. A
 * card that is not a Trap is shown and set face down again. A Trap that works only in answer to an event (the
 * list below) or whose activation conditions fail is destroyed; any other Trap is activated: it is queued as
 * a pending activation. In every case this card then goes back into its owner's deck, which is shuffled.
 * When the link was negated, this card is only destroyed.
 */
int EffectForceActivateTrapResolve(struct ChainEntry *link)
{
    int targetPlayer = (u8)link->targets[0];
    int targetZone = link->targets[0] >> 8;
    int side = 1 & targetPlayer;
    struct DuelZone *target = ZONE_AT(side, targetZone);
    u32 id = CARD_ID(CARD_WORD(target->card));

    if (link->negated) {
        DestroyFieldCard(link->player, link->zone, 1);
        return EFFECT_STEP_DONE;
    }
    if (id != 0 && !target->isFaceUp) {
        FlipFieldCard(targetPlayer, targetZone, 0);
        if (CARD_TYPE(id) != CARD_TYPE_TRAP) {
            ShowRevealedCardInt(targetPlayer, id);
            FlipFieldCard(targetPlayer, targetZone, 0);
        } else {
            switch (CARD_NUMBER(id)) {
            /* Traps that can only answer an event: activating them now fails, so they are destroyed. */
            case CARD_HOUSE_OF_ADHESIVE_TAPE:
            case CARD_EATGABOON:
            case CARD_WIDESPREAD_RUIN:
            case CARD_FAKE_TRAP:
            case CARD_MAGICAL_HATS:
            case CARD_TIME_MACHINE:
            case CARD_NEGATE_ATTACK:
            case CARD_CHAIN_DESTRUCTION:
            case CARD_MAGIC_ARM_SHIELD:
            case CARD_TRAP_HOLE:
            case CARD_WHITE_HOLE:
            case CARD_CALL_OF_THE_GRAVE:
            case CARD_ANTI_RAIGEKI:
            case CARD_SOLEMN_JUDGMENT:
            case CARD_MAGIC_JAMMER:
            case CARD_SEVEN_TOOLS_OF_THE_BANDIT:
            case CARD_HORN_OF_HEAVEN:
            case CARD_MAGIC_THORN:
            case CARD_MIRROR_FORCE:
            case CARD_GRYPHON_WING:
            case CARD_FAIRYS_HAND_MIRROR:
            case CARD_ENCHANTED_JAVELIN:
            case CARD_GUST:
            case CARD_DRIVING_SNOW:
            case CARD_ARMORED_GLASS:
            case CARD_WORLD_SUPPRESSION:
            case CARD_MYSTIC_PROBE:
            case CARD_METAL_DETECTOR:
            case CARD_NUMINOUS_HEALER:
            case CARD_APPROPRIATE:
            case CARD_FORCED_REQUISITION:
            case CARD_MAJOR_RIOT:
            case CARD_1214:
            case CARD_1247:
            case CARD_1301:
            case CARD_1304:
            case CARD_1317:
            case CARD_1321:
            case CARD_1325:
            case CARD_1424:
            case CARD_1425:
            case CARD_1428:
            case CARD_1529:
            case CARD_1531:
            case CARD_1535:
                goto destroyTarget;
            default: {
                struct ChainEntry entry;
                u32 zoneBits, playerBit;

                entry.card = id;
                entry.player = targetPlayer;
                entry.zone = targetZone;
                entry.event = RESPONSE_NONE;
                if (CanActivateEffectU16(&entry, NULL, 0) == 0)
                    goto destroyTarget;
                ShowActivatedCardInt(targetPlayer, id);
                /* Chain_AddPending word: card | zone << 16 | kind << 21 | player << 31 */
                playerBit = targetPlayer << 31;
                zoneBits = (targetZone & 0x1F) << 16;
                zoneBits |= CHAIN_KIND_SPELL_TRAP << 21;
                Chain_AddPending(playerBit | zoneBits | id, 0);
                goto returnToDeck;
            }
            }
        }
    }
    goto returnToDeck;
destroyTarget:
    ShowDestroyedCardInt(targetPlayer, id);
    DestroyFieldCard(targetPlayer, targetZone, 1);
returnToDeck:
    ReturnFieldCardToDeck(link->player, link->zone);
    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 1, 0, 0);
    return EFFECT_STEP_DONE;
}
