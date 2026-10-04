#include "global.h"
#include "legacy/gba.h"                    /* A_BUTTON, B_BUTTON */
#include "legacy/main.h"                   /* gMain.newKeys */
#include "util.h"                   /* Random, FormatStr, FormatInt */
#include "sprite.h"                 /* AddSprite */
#include "card_data.h"              /* gCardStats, gCardNames, gCardIdToNumber, CARD_ID_MASK */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_*, SPELL_* */
#include "constants/duel.h"         /* enum DuelArea, DuelPromptKind, ResponseEventKind */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* */
#include "constants/sound.h"        /* SE_* */

/*
 * The end of a turn, the opponent's turn, the partner's requests in a Link Battle, and the hand-discard
 * prompt (wiki/functions/duel-turn-end-c.md).
 *
 *  - DuelPhase_TurnEnd (duel step 7) removes the cards taken with Graverobber, shows the destroy countdowns of
 *    the turn player's monsters, queues the turn-end duel commands and hands the turn over.
 *  - DuelPhase_OpponentTurn (duel step 8) runs the CPU's turn (AiRunTurn) or, in a Link Battle, waits for the
 *    partner while serving its requests, until LINKMSG_TURN_END arrives; the duel then goes back to step 2.
 *  - DuelLink_Run* answer what the link partner asked this GBA to do: a Yes/No card question and the chainA,
 *    chainB and resolve handlers of a card, run on the partner's chain entries mirrored to this side
 *    (DuelLink_RunPartnerRequests picks the pending one).
 *  - DuelPrompt_Discard and DuelPrompt_DiscardCost (prompt kinds PROMPT_DISCARD and PROMPT_DISCARD_COST) make a
 *    player discard hand cards: the CPU picks them itself, the human picks with the text-box callbacks
 *    DiscardPrompt_DrawRemaining and DiscardPrompt_HandleInput.
 *  - CountDiscardableHandCards is dead code.
 */

/* ---- BEGIN pre-H0 subset of duel.h and sound.h ---- */
/*
 * The part of those headers this unit uses, with their names, types and bitfield containers (fields the
 * unit does not use keep the header's names). include/duel.h and sound.h still hold the legacy headers until
 * the header switch (H0, build/readability/HEADERS.md), and chain.h, duel_cmd.h, duel_link.h, duel_screen.h
 * and battle.h include duel.h, so this block also defines duel.h's include guard. After H0, replace this block
 * (BEGIN to END) with the include lines of duel.h and sound.h, in that order (checked: the unit compiles to
 * the same assembly with the new headers).
 */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID; 0 = empty */
    u32 owner:1;                    /* bit 12 */
    u32 unk13:1;
    u32 unk14:1;
    u32 normalSummoned:1;           /* bit 15 */
    u32 specialSummoned:1;          /* bit 16 */
    u32 planted:1;                  /* bit 17 */
    u32 graverobbed:1;              /* bit 18: taken with Graverobber; cleared when it leaves the field */
    u32 unk19:13;
};

struct DuelLoc {
    u16 player:1;
    u16 area:4;
    u16 index:9;
    u16 isDefense:1;
    u16 isFaceUp:1;
    u16 unk2;
};

struct DuelZone {
    struct DuelCard card;           /* +0x00 */
    u16 serial;                     /* +0x04 */
    u8 isDefense:1;                 /* +0x06 bit 0 */
    u8 isFaceUp:1;                  /* +0x06 bit 1 */
    u8 turnCounter:4;               /* +0x06 bits 2-5 */
    u16 destroyCountdown:4;         /* +0x06 bits 6-9: turns until the monster is destroyed */
    u8 positionLocked:1;            /* +0x07 bit 2 */
    u8 unk7_3:1;
    u8 unk7_4:1;
    u8 effectUnused:1;
    u8 revivedByMonsterReborn:1;
    u8 summonedFromGraveyard:1;
    u8 levelCheckDone:1;            /* +0x08 bit 0 */
    u8 unk8_1:7;
    u8 unk9;
    u16 links[32];                  /* +0x0A */
    u16 linkKinds[32];              /* +0x4A */
    u16 numLinks;                   /* +0x8A */
    u8 unk8C_0:1;                   /* +0x8C */
    u8 destroyAfterBattle:1;
    u32 returnAfterBattle:1;
    u8 cannotAttackNextTurn:1;
    u8 cannotAttack:1;
    u8 atkHalved:1;
    u8 unk8C_6:2;
    u8 unk8D[3];
    u32 unk90_0:6;
    u32 unk90_6:4;
    u32 canActivate:1;
    u8 isDisabled:1;
    u32 unk91_4:1;
    u32 declaredValue:5;
    u32 unk92_2:14;
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003 */
    u8 graveCount;                  /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 banishedCount;               /* +0x006 */
    u8 deckOut:1;                   /* +0x007 */
    u8 exodiaWin:1;
    u8 destinyBoardWin:1;
    u8 noNormalSummon:1;
    u8 noSpecialSummon:1;
    u8 positionChangeLocked:1;
    u8 magicTrapLockTurns:2;
    u8 noBattleDamage:1;            /* +0x008 */
    u8 battleProtected:1;
    u8 unk8_2:1;
    u8 insectQueenWonBattle:1;
    u8 normalSummonUsed:1;
    u8 summonedThisTurn:1;
    u8 extraBattlePhase:1;
    u8 unk8_7:1;
    u8 handRevealed:1;              /* +0x009 */
    u8 skipStandbyPhase:1;
    u8 skipDrawPhase:1;
    u8 skipTurn:1;
    u8 battlePhaseDone:1;
    u8 magicTrapActivatedThisTurn:1;
    u32 lockedZones:10;             /* +0x009 bit 6 .. +0x00A bit 7 */
    u8 crushCardTurns:3;            /* +0x00B */
    u8 monsterSentToGraveThisTurn:1;
    u32 removedMask:5;              /* +0x00B bit 4 .. +0x00C bit 0 */
    u8 delayedSummonCount:3;        /* +0x00C bits 1-3 */
    u8 destroyedTriggerPending:1;
    u8 banishCostFromField:1;
    u8 unkC_6:2;
    u8 unkD;
    u16 lpPaid[11];                 /* +0x00E */
    u16 attackableMask;             /* +0x024 */
    u16 attackedMask;               /* +0x026 */
    struct DuelZone zones[11];      /* +0x028 */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4 */
};

struct CardMenu {
    u16 open:1;
    u16 confirmed:1;
    u16 command:4;
    u16 slide:4;
    u32 available:16;
    u32 state:8;
    u32 step:8;
    u8 summonSeq:4;
    u32 tributeSources:4;
    u16 timer:7;
    u16 player:1;
    u32 area:7;
    u32 index:8;
    u32 placeZone:8;
    u32 unk0A_1:15;
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B10 - 0x1ACC];
    u16 turnCount;                  /* +0x1B10: ++ at the end of every turn */
    u8 bgmOn:1;                     /* +0x1B12 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 phase:3;
    u8 linkError:1;
    u8 result:2;
    u8 unk1B13_0:1;
    u8 unk1B13_1:7;
    u16 unk1B14_0:1;                /* +0x1B14 */
    u16 interruptActive:1;          /* +0x1B14 bit 1: the non-turn link player is acting */
    u16 unk1B14_2:7;
    u32 battleStage:8;
    u16 battleStep:8;
    u16 battleArg0:8;
    u16 battleArg1:8;
    u16 unk1B19_1:7;
    u8 unk1B1A[2];
    struct DuelCard battleCard;     /* +0x1B1C */
    u8 phaseStep;                   /* +0x1B20: step of the current duel-step handler */
    u8 phaseCounter;                /* +0x1B21: zone cursor / counter of the phase handlers */
    u8 phaseSubStep;                /* +0x1B22 */
    u8 phaseSubCounter;             /* +0x1B23 */
    u8 unk1B24[2];
    u8 endTurnAfterBattle:1;        /* +0x1B26 bit 0 */
    u8 unk1B26_1:7;
    u8 unk1B27;
    u16 cardMenuCard;               /* +0x1B28 */
    u16 summonTributes;             /* +0x1B2A */
    struct CardMenu cardMenu;       /* +0x1B2C */
    u8 unk1B38[0x1B50 - 0x1B38];
    u8 promptLinked:1;              /* +0x1B50 bit 0 */
    u8 promptActive:1;              /* +0x1B50 bit 1 */
    u8 promptPlayer:1;              /* +0x1B50 bit 2 */
    u8 unk1B50_3:1;
    u16 promptKind:6;               /* +0x1B50 bits 4-9: enum DuelPromptKind */
    u16 unk1B51_2:6;
    u16 promptArgs[8];              /* +0x1B52: [0] argument, [1] value (DuelPrompt_Post) */
    u8 promptStep;                  /* +0x1B62: step of the running prompt handler */
    u8 unk1B63;
    u16 promptResult;               /* +0x1B64 */
    u8 unk1B66[0x1B78 - 0x1B66];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */
extern struct DuelZonesPlayer gDuelZones[2];    /* 0x0201930C = gDuel.players[0].zones */

int FindMagicInHand(int player);
int FindTrapInHand(int player);

void PlaySE(u32 seId);
/* ---- END pre-H0 subset ---- */

#include "duel_flow.h"              /* gDuelCtrl, enum TurnEndStep, DuelPhase_TurnEnd, DuelPhase_OpponentTurn */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_link.h"              /* gLinkState, DuelLink_*, LINKMSG_* */
#include "duel_screen.h"            /* gDuelScreen, DuelScreen_ScrollToZone, DuelCursor_*, ChainListScreen_Run */
#include "duel_actions.h"           /* ShowCardEffect, DestroyFieldCard, DiscardHandCard */
#include "duel_prompt.h"            /* the DuelPrompt_Discard* and DiscardPrompt_* functions defined here */
#include "effect.h"                 /* gCardEffects, FindCardEffect, TriggerForcedRequisition */
#include "chain.h"                  /* gChain, EventResponse_Request */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "ai.h"                     /* gAiState, AiRunTurn, AiPickDiscard, AiPickWeakestHandCard */

/* ---- ROM data used only here ---- */

extern const char gStrTurnsUntilDestroyedFmt[];     /* "%d turn(s) remaining before '%s' is destroyed." */
extern const char gStrAttackTargetZeroAtkFmt[];     /* Sanga/Kazejin/Suijin: "%s has been designated as the attack
                                                     * target. Do you wish to ... reduce the attacking monster's ATK
                                                     * to 0?" */
extern const char gStrKuribohDiscardFmt[];          /* "You've suffered damage from battle. Do you wish to reduce the
                                                     * damage to 0 by discarding %s?" */
extern const char gStrAttackTargetSubstituteFmt[];  /* key 1243: "... select your opponent's monster to substitute as
                                                     * the target?" */
extern const char gStrAttackTargetRedirectFmt[];    /* key 1522: "... designate another monster as target?" */
extern const u8 gStrDiscardFromHand[];              /* "Discard from your hand." */
extern const u16 gUnk_0862467A;                     /* = gCardNumberToId[CARD_GRAVEROBBER]: Graverobber's ID */
extern const u16 gUnk_08623E66;                     /* = gCardNumberToId[CARD_KURIBOH]: Kuriboh's ID */

/* ---- Local views kept for matching (build/readability/HEADERS.md) ---- */

/*
 * Zone `zone` of `player` as a sum of integers on the gDuelZones address, zone term first or player term first.
 * Matching: the ROM adds the two products to the table address itself, and their order in the sum decides
 * agbcc's multiply order; gDuelZones[p].zones[z] folds them differently.
 */
#define ZONE_ZP(player, zone)   ((struct DuelZone *)((zone) * 0x94 + ((player) & 1) * 0xD64 + (u32)gDuelZones))
#define ZONE_PZ(player, zone)   ((struct DuelZone *)(((player) & 1) * 0xD64 + (zone) * 0x94 + (u32)gDuelZones))

/*
 * Matching: the flag byte at gLinkState +0x306 is read into a register and tested as the sign of the byte shifted
 * left (lsl; blt) instead of masking the bitfield (LinkState.turnEndReceived, duelResultReceived,
 * remoteDuelEnded). The byte is reached through a cast view so that the ROM's address form (the symbol and the
 * offset 0x306 as two literals) results.
 */
struct LinkStateFlagBytes { u8 pad[0x306]; u8 flags306; };
#define LINK_FLAGS_306                  (((struct LinkStateFlagBytes *)&gLinkState)->flags306)
#define LINK_FLAG_TURN_END_RECEIVED     2
#define LINK_FLAG_DUEL_RESULT_RECEIVED  3
#define LINK_FLAG_REMOTE_DUEL_ENDED     5
#define LINK_FLAG_IS_SET(flags, bit)    (((flags) << (31 - (bit))) < 0)

/* Byte +2 of remoteEntries[0] (bit 0 = player). Matching: set with a plain byte OR; the bitfield assignment
 * compiles to a mask and an or. */
#define REMOTE_ENTRY0_BYTE2             (((u8 *)&gLinkState.remoteEntries[0])[2])

/* The handler slots of gChain (u16 / u32 results, as Chain_Build and Chain_Resolve call them) and the rows of
 * gCardEffects (int results) declare the same handlers with different return types; a slot is filled with a
 * cast. */
typedef u16 (*ChainHandlerFn)(struct ChainEntry *link, struct ChainEntry *chainedTo);
typedef u32 (*ChainResolveFn)(struct ChainEntry *link, struct ChainEntry *chainedTo);

/* AiRunTurn is tested as an int; the header's u16 return adds a narrowing. */
int AiRunTurnInt(void) asm("AiRunTurn");

/* The card number (gCardIdToNumber) and the name record (gCardNames) of a card ID, through the tables' constant
 * addresses (the ROM loads the address as a literal instead of going through the symbol). */
#define CARD_NUMBER_OF(id)  (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
#define CARD_NAME_OF(id)    ((const char *)0x0822C720 + ((id) << 6))

/* gDuel.promptArgs[1] of a discard prompt (DuelPrompt_PostDiscard): bit 0 = only monsters may be discarded, bit 1 =
 * the discard is forced by the opponent's effect. */
#define DISCARD_FLAG_MONSTERS_ONLY  1
#define DISCARD_FLAG_BY_OPPONENT    2

/* gTextBox.menuTimer as the sub-step of DiscardPrompt_HandleInput. */
enum DiscardInputStep {
    DISCARD_INPUT_PICK = 0,         /* wait for a hand card to be discarded */
    DISCARD_INPUT_CURSOR_BACK = 1,  /* put the cursor back on the hand */
    DISCARD_INPUT_COUNT = 2         /* one card less to discard; done when none is left */
};

#define DISCARD_MARKER_ATTR2    0x431C      /* OBJ tile 0x31C, palette 4: one marker per card still to discard */

#define TEXTBOX_XY(x, y)    ((x) | ((y) << 8))      /* pos / size word of TextBoxOpen: low byte x, high byte y */

/*
 * The card type of a card ID (gCardStats bits 20-24) in the two forms the ROM uses here: through the gCardStats
 * symbol (pointer arithmetic, DiscardPrompt_TryDiscardSelected) and through its constant address.
 */
#define CARD_TYPE_OF_SYM(id)    CARD_STATS_TYPE(*(gCardStats + ((id) & CARD_ID_MASK)))
static inline int GetCardType(u16 cardId)
{
    return (((const u32 *)0x08621DE0)[cardId & CARD_ID_MASK] & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT;
}

/* Magic/Trap subtype (enum SpellSubtype) of a card ID, 0 for any other card. */
static inline int GetSpellSubtype(u16 cardId)
{
    u32 stats = ((const u32 *)0x08621DE0)[cardId & CARD_ID_MASK];
    switch ((int)((stats & CARD_STATS_TYPE_MASK) >> CARD_STATS_TYPE_SHIFT)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return (stats & CARD_STATS_SUBTYPE_MASK) >> CARD_STATS_SUBTYPE_SHIFT;
    default:
        return 0;
    }
}

/* gDuel +0x1B14 with interruptActive in a u8 container (duel.h: u16). Matching: padded past 4 bytes so that it
 * is read with ldrb. */
struct DuelStateInterruptView {
    u8 unk0:1;
    u8 interruptActive:1;
    u8 unk2:6;
    u8 pad[4];
};

/*
 * Duel step 7: the end of the turn (enum TurnEndStep; also gAiTurnPhases[5]). Steps 0 and 1 handle the turn
 * player's, then the opponent's, cards taken with Graverobber (card bit 18), one card per call: the first such
 * card in the hand is discarded (ShowCardEffect of Graverobber, DiscardHandCard); failing that, the first such
 * card in the spell/trap zones 5-10 (other than a face-up Field, Equip or Continuous card, which stay) loses
 * the bit and is destroyed. Step 1 skips the countdown messages on the CPU's turn. Step 2 shows, for each of
 * the turn player's monsters with a destroy countdown above 1, 'N turn(s) remaining before ... is destroyed.'.
 * Steps 3 and 4 queue DUEL_CMD_TURN_END and DUEL_CMD_SHOW_END_TURN_HAND. The last step resets the AI state
 * before the CPU's turn (or tells the link partner the turn is over) and counts the turn. Returns 1 when the
 * step is finished.
 */
int DuelPhase_TurnEnd(void)
{
    char text[0x80];
    int player = gDuel.turnPlayer;
    int i;
    struct DuelPlayer *playersForOpponent;
    struct DuelPlayer *playersForOwn; /* separate pointer per case: the ROM gives them different registers */
    u8 ownHandCount;
    u8 opponentHandCount;
    int opponent;

    switch (gDuel.phaseStep) {
    case TURN_END_STEP_GRAVEROBBED_OWN:
        i = 0;
        playersForOwn = gDuelPlayers;
        if (i < playersForOwn[player & 1].handCount) {
            struct DuelCard *hand = playersForOwn->hand;
            ownHandCount = playersForOwn[player & 1].handCount;
            do {
                /* Matching: the card is addressed as hand + player * 0xD64 + i * 4, not hand[i] of the player. */
                struct DuelCard card = *(struct DuelCard *)((player & 1) * 0xD64 + (u32)hand + i * 4);
                if (card.graverobbed) {
                    ShowCardEffect(player, gUnk_0862467A);
                    DiscardHandCard(player, i, 0, 1);
                    return 0;
                }
                i++;
            } while (i < ownHandCount);
        }
        for (i = ZONE_SPELL_0; i <= ZONE_FIELD; i++) {
            struct DuelZone *z = ZONE_ZP(player, i);
            struct DuelCard card = z->card;
            if (card.id != 0 && card.graverobbed) {
                int ok = 1;
                if (z->isFaceUp) {
                    switch (GetSpellSubtype(card.id)) {
                    case SPELL_FIELD:
                    case SPELL_EQUIP:
                    case SPELL_CONTINUOUS:
                        ok = 0;
                    }
                }
                if (ok) {
                    ZONE_PZ(player, i)->card.graverobbed = 0;
                    ShowCardEffect(player, gUnk_0862467A);
                    DestroyFieldCard(player, i, 0);
                    return 0;
                }
            }
        }
        gDuel.phaseStep++;
        return 0;
    case TURN_END_STEP_GRAVEROBBED_OPPONENT:
        i = 0;
        playersForOpponent = gDuelPlayers;
        if (i < playersForOpponent[(1 - player) & 1].handCount) {
            struct DuelCard *hand;
            /* FAKEMATCH: the first call takes this copy, the second recomputes 1 - player (as in the ROM). */
            opponent = 1 - player;
            hand = playersForOpponent->hand;
            opponentHandCount = playersForOpponent[(1 - player) & 1].handCount;
            do {
                struct DuelCard card = *(struct DuelCard *)((opponent & 1) * 0xD64 + (u32)hand + i * 4);
                if (card.graverobbed) {
                    ShowCardEffect(opponent, gUnk_0862467A);
                    DiscardHandCard(1 - player, i, 0, 1);
                    return 0;
                }
                i++;
            } while (i < opponentHandCount);
        }
        for (i = ZONE_SPELL_0; i <= ZONE_FIELD; i++) {
            struct DuelZone *z = ZONE_ZP(1 - player, i);
            struct DuelCard card = z->card;
            if (card.id != 0 && card.graverobbed) {
                int ok = 1;
                if (z->isFaceUp) {
                    switch (GetSpellSubtype(card.id)) {
                    case SPELL_FIELD:
                    case SPELL_EQUIP:
                    case SPELL_CONTINUOUS:
                        ok = 0;
                    }
                }
                if (ok) {
                    ZONE_PZ(1 - player, i)->card.graverobbed = 0;
                    ShowCardEffect(1 - player, gUnk_0862467A);
                    DestroyFieldCard(1 - player, i, 0);
                    return 0;
                }
            }
        }
        if (player)
            gDuel.phaseStep++;
        gDuel.phaseStep++;
        gDuel.phaseCounter = 0;
        return 0;
    case TURN_END_STEP_COUNTDOWN_MESSAGES:
        while (gDuel.phaseCounter <= ZONE_MONSTER_4) {
            u8 zone = gDuel.phaseCounter;
            /* gDuel.players[0].zones addressed through gDuel (the other scans use gDuelZones). */
            struct DuelZone *z = (struct DuelZone *)(zone * 0x94 + player * 0xD64 + (u32)gDuel.players[0].zones);
            struct DuelCard card = z->card;
            s16 id = card.id; /* FAKEMATCH: s16 adds loop insns so loop.c keeps movs #0x94 in the loop */
            if (id != 0 && z->destroyCountdown > 1) {
                /* FAKEMATCH: integer table address; the gCardNames symbol shifts reload registers */
                FormatStr(text, gStrTurnsUntilDestroyedFmt, CARD_NAME_OF(id));
                FormatInt(text, text, z->destroyCountdown - 1);
                TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(18, 7), TEXTBOX_FLAGS_DEFAULT, text);
                gDuel.phaseCounter++;
                return 0;
            }
            gDuel.phaseCounter = zone + 1;
        }
        gDuel.phaseStep++;
        return 0;
    case TURN_END_STEP_CMD_TURN_END:
        DuelCmd_Push(gDuel.turnPlayer ? DUEL_CMD_TURN_END | DUEL_CMD_PLAYER : DUEL_CMD_TURN_END, 0, 0, 0);
        gDuel.phaseStep++;
        return 0;
    case TURN_END_STEP_CMD_END_HAND:
        DuelCmd_Push(gDuel.turnPlayer ? DUEL_CMD_SHOW_END_TURN_HAND | DUEL_CMD_PLAYER : DUEL_CMD_SHOW_END_TURN_HAND,
                     0, 0, 0);
        gDuel.phaseStep++;
        return 0;
    default:
        /* Single player, the human's turn is over: reset the AI state for the CPU's turn that follows. */
        if (!gDuelCtrl.isLinkDuel) {
            if (!gDuel.turnPlayer) {
                gAiState.turnPhase = 0;
                gAiState.step = 0;
            }
        }
        if (gDuelCtrl.isLinkDuel)
            DuelLink_SendMessage(LINKMSG_TURN_END, 0, 0, 0);
        gDuel.turnCount++;
        return 1;
    }
}

/*
 * Called by DuelLink_RunPartnerRequests: the Yes/No question about a card that the partner asked
 * (LINKMSG_CARD_PROMPT; gLinkState.cardPromptStep is the step). Step 0 opens the question for the card's
 * number: Sanga/Kazejin/Suijin 'reduce the attacking monster's ATK to 0?', Kuriboh 'reduce the damage to 0 by
 * discarding Kuriboh?', key 1243 'select your opponent's monster to substitute as the target?', key 1522
 * 'designate another monster as target?'. Step 1 stores gTextBox.result as the answer and returns 1 for those
 * cards (0 for any other card, which would wait forever). Returns 1 when answered.
 */
u16 DuelLink_RunCardPrompt(void)
{
    char text[0x100];
    switch (gLinkState.cardPromptStep) {
    case 0:
        switch (CARD_NUMBER_OF(gLinkState.cardPromptCard)) {
        case CARD_SANGA_OF_THE_THUNDER:
        case CARD_KAZEJIN:
        case CARD_SUIJIN:
            FormatStr(text, gStrAttackTargetZeroAtkFmt, CARD_NAME_OF(gLinkState.cardPromptCard));
            TextBoxOpen(TEXTBOX_XY(4, 2), TEXTBOX_XY(20, 10), TEXTBOX_FLAGS_DEFAULT, text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            break;
        case CARD_KURIBOH:
            FormatStr(text, gStrKuribohDiscardFmt, CARD_NAME_OF(gUnk_08623E66));
            TextBoxOpen(TEXTBOX_XY(4, 2), TEXTBOX_XY(21, 9), TEXTBOX_FLAGS_DEFAULT, text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            break;
        case CARD_1243:
            FormatStr(text, gStrAttackTargetSubstituteFmt, CARD_NAME_OF(gLinkState.cardPromptCard));
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(19, 7), TEXTBOX_FLAGS_DEFAULT, text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            break;
        case CARD_1522:
            FormatStr(text, gStrAttackTargetRedirectFmt, CARD_NAME_OF(gLinkState.cardPromptCard));
            TextBoxOpen(TEXTBOX_XY(6, 2), TEXTBOX_XY(19, 7), TEXTBOX_FLAGS_DEFAULT, text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            break;
        }
        gLinkState.cardPromptStep++;
        return 0;
    case 1:
        switch (CARD_NUMBER_OF(gLinkState.cardPromptCard)) {
        case CARD_SANGA_OF_THE_THUNDER:
        case CARD_KAZEJIN:
        case CARD_SUIJIN:
        case CARD_KURIBOH:
        case CARD_1243:
        case CARD_1522:
            gLinkState.cardPromptAnswer = gTextBox.result;
            return 1;
        }
        return 0;
    default:
        return 1;
    }
}

/*
 * Called by DuelLink_RunPartnerRequests: run the cost (chainA) handler of the card in remoteEntries[0] on the
 * partner's chain entries (gLinkState.remoteChainAStep is the step). Step 0 looks the handler up; step 1 calls
 * it until it returns nonzero. Returns 1 when finished, also when the card has no chainA handler.
 */
u16 DuelLink_RunRemoteChainA(void)
{
    struct LinkState *link = &gLinkState;
    u8 *step = &link->remoteChainAStep;
    switch (*step) {
    case 0:
        gChain.effectIndex = FindCardEffect(link->remoteEntries[0].card);
        if (gChain.effectIndex < 0)
            return 1;
        gChain.chainA = (ChainHandlerFn)gCardEffects[gChain.effectIndex].chainA;
        if (gChain.chainA == NULL)
            return 1;
        gChain.costStep = 0;
        (*step)++;
        return 0;
    case 1:
        if (gChain.chainA(&link->remoteEntries[0], &link->remoteEntries[1]) == 0)
            return 0;
        (*step)++;
        return 0;
    default:
        return 1;
    }
}

/* Called by DuelLink_RunPartnerRequests: DuelLink_RunRemoteChainA for the target (chainB) handler. */
u16 DuelLink_RunRemoteChainB(void)
{
    struct LinkState *link = &gLinkState;
    u8 *step = &link->remoteChainBStep;
    switch (*step) {
    case 0:
        gChain.effectIndex = FindCardEffect(link->remoteEntries[0].card);
        if (gChain.effectIndex < 0)
            return 1;
        gChain.chainB = (ChainHandlerFn)gCardEffects[gChain.effectIndex].chainB;
        if (gChain.chainB == NULL)
            return 1;
        gChain.targetStep = 0;
        (*step)++;
        return 0;
    case 1:
        if (gChain.chainB(&link->remoteEntries[0], &link->remoteEntries[1]) == 0)
            return 0;
        (*step)++;
        return 0;
    default:
        return 1;
    }
}

/* Swap the low-byte player of a packed halfword: 1 - player, high byte kept. */
#define FLIP_LO_PLAYER(h)   ((u8)(1 - (h)) | ((h) >> 8 << 8))

/*
 * Called by DuelLink_RunPartnerRequests: run the resolve handler of the card in remoteEntries[0]. Step 0 mirrors both
 * remote entries to this GBA's point of view (player = 1 - player, also in the player byte of loc0 / loc1),
 * looks up the handler and starts it with gChain.effectStep = EFFECT_STEP_START. Step 1 calls it (with
 * remoteEntries[1] when remoteResolveMode bit 0 is set, else NULL) until it returns 0. Returns 1 when finished.
 */
u16 DuelLink_RunRemoteResolve(void)
{
    struct LinkState *link = &gLinkState;
    u8 *step = &link->remoteResolveStep;
    switch (*step) {
    case 0:
        /* FAKEMATCH: the u8 constant gives the minuend its own QImode register, as in the ROM */
        { u8 v = link->remoteEntries[0].player; u8 one = 1; link->remoteEntries[0].player = one - v; }
        { u8 v = link->remoteEntries[1].player; u8 one = 1; link->remoteEntries[1].player = one - v; }
        { u16 *hp = &link->remoteEntries[0].loc0; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &link->remoteEntries[0].loc1; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &link->remoteEntries[1].loc0; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &link->remoteEntries[1].loc1; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        gChain.effectIndex = FindCardEffect(gLinkState.remoteEntries[0].card);
        if (gChain.effectIndex < 0)
            return 1;
        gChain.resolve = (ChainResolveFn)gCardEffects[gChain.effectIndex].resolve;
        if (gChain.resolve == NULL)
            return 1;
        gChain.effectStep = EFFECT_STEP_START;
        gChain.effectSubStep = 0;
        (*step)++;
        return 0;
    case 1:
        if (link->remoteResolveMode & 1)
            gChain.effectStep = gChain.resolve(&link->remoteEntries[0], &link->remoteEntries[1]);
        else
            gChain.effectStep = gChain.resolve(&link->remoteEntries[0], NULL);
        if (gChain.effectStep == 0)
            gLinkState.remoteResolveStep++;
        return 0;
    default:
        return 1;
    }
}
#undef FLIP_LO_PLAYER

/*
 * Serve the first request the link partner has pending (one per call) and answer it; returns 1 while one is
 * being handled, else 0. In priority order: a Yes/No card question, the chainA handler, the chainB handler,
 * an 'activate?' query, the resolve handler, showing the chain list. Each pending flag is cleared when its
 * handler finishes.
 */
u16 DuelLink_RunPartnerRequests(void)
{
    if (gLinkState.cardPromptPending && !LINK_FLAG_IS_SET(LINK_FLAGS_306, LINK_FLAG_REMOTE_DUEL_ENDED)) {
        if (DuelLink_RunCardPrompt() != 0) {
            gLinkState.cardPromptPending = 0;
            DuelLink_SendMessage(LINKMSG_CARD_PROMPT_ANSWER, gLinkState.cardPromptAnswer, gLinkState.cardPromptArg1,
                                 gLinkState.cardPromptArg2);
        }
        return 1;
    }
    if (gLinkState.remoteChainAPending) {
        if (DuelLink_RunRemoteChainA() != 0) {
            gLinkState.remoteChainAPending = 0;
            REMOTE_ENTRY0_BYTE2 |= 1;
            DuelLink_SendMessageData(LINKMSG_REMOTE_CHAIN_A_DONE, &gLinkState.remoteEntries[0], sizeof(struct ChainEntry));
        }
        return 1;
    }
    if (gLinkState.remoteChainBPending) {
        if (DuelLink_RunRemoteChainB() != 0) {
            gLinkState.remoteChainBPending = 0;
            REMOTE_ENTRY0_BYTE2 |= 1;
            DuelLink_SendMessageData(LINKMSG_REMOTE_CHAIN_B_DONE, &gLinkState.remoteEntries[0], sizeof(struct ChainEntry));
        }
        return 1;
    }
    if (gLinkState.activateQueryPending && !gLinkState.remoteDuelEnded) {
        if (DuelLink_AnswerActivateQuery() != 0)
            gLinkState.activateQueryPending = 0;
        return 1;
    }
    if (gLinkState.remoteResolvePending) {
        if (DuelLink_RunRemoteResolve() != 0) {
            gLinkState.remoteResolvePending = 0;
            DuelLink_SendMessage(LINKMSG_REMOTE_RESOLVE_DONE, 0, 0, 0);
        }
        return 1;
    }
    if (gLinkState.showChainListPending) {
        if (ChainListScreen_Run() != 0) {
            gLinkState.showChainListPending = 0;
            DuelLink_SendMessage(LINKMSG_CHAIN_LIST_SHOWN, 0, 0, 0);
        }
        return 1;
    }
    return 0;
}

/*
 * Duel step 8: the opponent's turn. gDuel.turnPlayer is 1 for the whole turn. Single player: run AiRunTurn until
 * it finishes. Link duel: the partner's result ends the duel (the next duel step is DUEL_STEP_RESULT); otherwise
 * serve its requests, and while the non-turn player is interrupting ('Just a moment') let our player play on the
 * field until B ends the interrupt (DUEL_CMD_SHOW_END_TURN_HAND, LINKMSG_INTERRUPT_END), or send
 * LINKMSG_INTERRUPT_REQUEST when A is pressed. LINKMSG_TURN_END ends the turn. The turn then goes back to the
 * human player at DUEL_STEP_TURN_START (the step is moved back by 6 here, not by DuelMainStep). Returns 1 only
 * to move on to the result; 0 otherwise.
 */
int DuelPhase_OpponentTurn(void)
{
    struct DuelStateInterruptView *view;

    gDuel.turnPlayer = 1;
    if (gDuelCtrl.isLinkDuel) {
        u8 flags = LINK_FLAGS_306;
        if (LINK_FLAG_IS_SET(flags, LINK_FLAG_DUEL_RESULT_RECEIVED)) {
            gDuelCtrl.phase++;
            return 1;
        }
        if (LINK_FLAG_IS_SET(flags, LINK_FLAG_REMOTE_DUEL_ENDED))
            return 0;
        if (DuelLink_RunPartnerRequests() != 0)
            return 0;
        view = (struct DuelStateInterruptView *)((u8 *)&gDuel + 0x1B14);
        if (view->interruptActive) {
            if (DuelScreen_HandleInput() != 0)
                return 0;
            if (gMain.newKeys & B_BUTTON) {
                view->interruptActive = 0;
                DuelCmd_Push(DUEL_CMD_SHOW_END_TURN_HAND, 0, 0, 0);
                DuelLink_SendMessage(LINKMSG_INTERRUPT_END, 0, 0, 0);
            }
        } else if (gMain.newKeys & A_BUTTON) {
            DuelLink_SendMessage(LINKMSG_INTERRUPT_REQUEST, 0, 0, 0);
        }
        {
            u8 flagsNow = LINK_FLAGS_306;
            if (!LINK_FLAG_IS_SET(flagsNow, LINK_FLAG_TURN_END_RECEIVED))
                return 0;
            {
                int mask = -5;
                mask &= flagsNow;
                LINK_FLAGS_306 = mask;
            }
        }
    } else if (AiRunTurnInt() == 0) {
        return 0;
    }
    gDuel.turnPlayer = 0;
    gDuelCtrl.phase -= 6;
    gDuel.phaseStep = 0;
    gDuel.phaseCounter = 0;
    return 0;
}

/* Number of the player's hand cards that are neither planted nor graverobbed (bits 17 and 18 of the card
 * word); with monstersOnly also at most CARD_TYPE_REPTILE (a monster). monstersOnly 0 is the plain hand
 * count. Dead code. */
int CountDiscardableHandCards(int player, u16 monstersOnly)
{
    int six;
    int i;
    int count = 0;
    int n;

    if (monstersOnly == 0)
        return gDuelPlayers[player & 1].handCount;
    n = gDuelPlayers[player & 1].handCount;
    for (i = 0; i < n; i++) {
        u32 *p = (u32 *)&gDuelPlayers[player & 1].hand[i];
        if ((u32)GetCardType((*p << 20) >> 20) <= CARD_TYPE_REPTILE || monstersOnly == 0) {
            /* FAKEMATCH: the mask goes through a temporary (permuter find); it swaps count (r5) and the base (r6). */
            if (!(((u8 *)p)[2] & (six = 6)))
                count++;
        }
    }
    return count;
}

/*
 * The discard prompt's hand pick for player 0: 1 if the hand is empty. Otherwise the cursor picks a hand card
 * (DuelCursor_PickTarget(PICK_HAND)); the card at gDuelScreen.selIndex is discarded and 1 returned when it is
 * allowed (any card, or a monster when monstersOnly) and neither planted nor graverobbed; else the error
 * sound. Returns 0 while waiting.
 */
u16 DiscardPrompt_TryDiscardSelected(u16 monstersOnly, u16 byOpponent)
{
    struct DuelPlayer *player = gDuelPlayers;
    if (player->handCount == 0)
        return 1;
    if (DuelCursor_PickTarget(PICK_HAND) != 0) {
        u32 index = gDuelScreen.selIndex;
        u32 offset = index << 2;
        struct DuelCard *hand = gDuelPlayers->hand;
        struct DuelCard *card = (struct DuelCard *)((u8 *)hand + offset);
        if (monstersOnly == 0 || CARD_TYPE_OF_SYM((*(u32 *)card << 20) >> 20) <= CARD_TYPE_REPTILE) {
            struct DuelCard value = *card;
            int ok = 1;
            if (value.planted)
                ok = 0;
            if (value.graverobbed)
                ok = 0;
            if (ok != 0) {
                DiscardHandCard(0, index, byOpponent, 1);
                return 1;
            }
        }
        PlaySE(SE_ERROR);
    }
    return 0;
}

/* Text-box draw callback of the discard prompt: one marker sprite per card still to discard, 10 pixels apart,
 * at the text box's left edge two rows below its revealed top. */
void DiscardPrompt_DrawRemaining(void)
{
    int i;
    int x = (gTextBox.x + 1) << 3;
    int y = (gTextBox.revealRow - gTextBox.height + 2) << 3;
    for (i = 0; i < gChain.handPickCount; i++) {
        /* FAKEMATCH: no-op self-store (found by the permuter). It enlarges the loop body enough that loop.c
         * keeps y << 16 inside the loop, and the extra uses of the count address give that pointer r6 ahead of y. */
        gChain.handPickCount += 0;
        AddSprite((x + i * 10) | (y << 16), SPRITE_SHAPE_8x8, DISCARD_MARKER_ATTR2);
    }
}

/* Text-box input callback of the discard prompt; gTextBox.menuTimer is its sub-step. 0: pick a card until one is
 * discarded. 1: put the cursor back on the hand. 2: one card less to discard; 1 when none is left. */
int DiscardPrompt_HandleInput(void)
{
    struct TextBox *box = &gTextBox;
    u8 *state = &box->menuTimer;
    switch (*state) {
    case DISCARD_INPUT_PICK:
        if (DiscardPrompt_TryDiscardSelected(gDuel.promptArgs[1] & DISCARD_FLAG_MONSTERS_ONLY, gDuel.promptArgs[1] & DISCARD_FLAG_BY_OPPONENT)) {
        next:
            (*state)++;
        }
        break;
    case DISCARD_INPUT_CURSOR_BACK:
        DuelCursor_Select(0, DUEL_AREA_HAND, 0);
        goto next;
    case DISCARD_INPUT_COUNT:
        gChain.handPickCount--;
        if (gChain.handPickCount == 0)
            return 1;
        *state = 0;
        return 0;
    }
    return 0;
}

/*
 * Handler of PROMPT_DISCARD (enum DiscardPromptStep in gDuel.promptStep): make `player` discard `count` hand
 * cards. The CPU discards one per call, picking AiPickDiscard, else the weakest card, else a Magic, a Trap, or
 * (with more than 2 cards in player 1's hand) a random card, else the first. The human gets 'Discard from your
 * hand.' with the callbacks above. Then Forced Requisition (the opponent discards too) and the discard trigger
 * for the other player. monstersOnly is not read here: the input callback takes it from promptArgs[1]. Returns 1
 * when finished.
 */
int DuelPrompt_Discard(int player, int count, int monstersOnly, u16 byOpponent)
{
    struct DuelState *duel = &gDuel;
    u8 *step = &duel->promptStep;
    switch (*step) {
    case DISCARD_STEP_INIT:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gChain.handPickTimer = 0;
        gChain.handPickCount = count;
        (*step)++;
        return 0;
    case DISCARD_STEP_PICK:
        if (player != 0) {
            struct DuelPlayer *players;
            int pick;
            if (gChain.handPickCount == 0)
                goto next;
            players = duel->players;
            if (players[player & 1].handCount == 0)
                goto next;
            pick = AiPickDiscard();
            if (pick < 0) {
                pick = AiPickWeakestHandCard(players, 1);
                if (pick < 0) {
                    pick = FindMagicInHand(1);
                    if (pick < 0) {
                        pick = FindTrapInHand(1);
                        if (pick < 0) {
                            u8 *handCount = &duel->players[1].handCount;
                            if (*handCount > 2)
                                pick = Random() % *handCount;
                            else
                                pick = 0;
                        }
                    }
                }
            }
            DiscardHandCard(player, pick, byOpponent, 1);
            gChain.handPickCount--;
            return 0;
        }
        DuelCursor_Select(0, DUEL_AREA_HAND, 0);
        TextBoxOpen(TEXTBOX_XY(9, 2), TEXTBOX_XY(14, 5), TEXTBOX_FLAGS_DEFAULT, gStrDiscardFromHand);
        TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, DiscardPrompt_DrawRemaining, (u16 (*)(void))DiscardPrompt_HandleInput);
    next:
        gDuel.promptStep++;
        return 0;
    case DISCARD_STEP_FORCED_REQUISITION:
        TriggerForcedRequisition(player, count);
        (*step)++;
        return 0;
    case DISCARD_STEP_TRIGGER:
        EventResponse_Request(1 - duel->turnPlayer, RESPONSE_DISCARDED, (u8)player);
        (*step)++;
        return 0;
    default:
        return 1;
    }
}

/* Handler of PROMPT_DISCARD_COST: the first two steps of DuelPrompt_Discard (same CPU picks and human prompt),
 * without Forced Requisition and the discard trigger. Returns 1 when finished. */
int DuelPrompt_DiscardCost(int player, int count, int monstersOnly, u16 byOpponent)
{
    struct DuelState *duel = &gDuel;
    u8 *step = &duel->promptStep;
    switch (*step) {
    case DISCARD_STEP_INIT:
        DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
        gChain.handPickTimer = 0;
        gChain.handPickCount = count;
        (*step)++;
        return 0;
    case DISCARD_STEP_PICK:
        if (player != 0) {
            struct DuelPlayer *players;
            int pick;
            if (gChain.handPickCount == 0)
                goto next;
            players = duel->players;
            if (players[player & 1].handCount == 0)
                goto next;
            pick = AiPickDiscard();
            if (pick < 0) {
                pick = AiPickWeakestHandCard(players, 1);
                if (pick < 0) {
                    pick = FindMagicInHand(1);
                    if (pick < 0) {
                        pick = FindTrapInHand(1);
                        if (pick < 0) {
                            u8 *handCount = &duel->players[1].handCount;
                            if (*handCount > 2)
                                pick = Random() % *handCount;
                            else
                                pick = 0;
                        }
                    }
                }
            }
            DiscardHandCard(player, pick, byOpponent, 1);
            gChain.handPickCount--;
            return 0;
        }
        DuelCursor_Select(0, DUEL_AREA_HAND, 0);
        TextBoxOpen(TEXTBOX_XY(9, 2), TEXTBOX_XY(14, 5), TEXTBOX_FLAGS_DEFAULT, gStrDiscardFromHand);
        TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, DiscardPrompt_DrawRemaining, (u16 (*)(void))DiscardPrompt_HandleInput);
    next:
        gDuel.promptStep++;
        return 0;
    default:
        return 1;
    }
}
