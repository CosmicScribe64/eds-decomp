#include "global.h"
#include "card_data.h"              /* CARD_ID_MASK, CARD_NAME_SIZE, CARD_STATS_TYPE, CARD_STATS_LEVEL */
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* enum CardType */
#include "constants/duel.h"         /* enum DuelPromptKind, ZoneLinkKind, FieldPickMask */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER */
#include "constants/sound.h"        /* enum SoundEffect */

/*
 * Card effect handlers: the Resolve slot of gCardEffects (include/effect.h) for Seal of the Ancients, the
 * two Dice, Exchange, Time Wizard, Goddess of Whim, Barrel Dragon and effect keys 1212, 1213 and 1220 (no
 * EDS cards) (wiki/functions/effect-resolve8-c.md).
 *
 * Chain_Resolve calls a link's resolve handler once per frame with gChain.effectStep as the handler's step:
 * EFFECT_STEP_START (0x80) on the first call, then whatever the previous call returned, until a handler
 * returns EFFECT_STEP_DONE (0). The steps count down from 0x80 (enum EffectStep, include/effect.h) and
 * mean something different in each handler; Time Wizard and key 1220 name theirs in local enums. A returned
 * step the handler has no case for ends the effect on the next call; the coin-toss handlers return
 * COIN_EFFECT_STEP_USED (0x0A) so that their default case marks the once-per-turn effect used. When the
 * link was negated (ChainEntry.negated) every handler but key 1212's does nothing.
 *
 * Players: 0 is the human, 1 the CPU (or the link partner). Duel commands queued for player 1 carry
 * DUEL_CMD_PLAYER (bit 15). Zones 0-4 hold monsters, 5-9 spells and traps, 10 the Field Magic; a target is
 * player | zone << 8.
 */

/* ---- BEGIN duel.h stand-in (pre-H0) ----
 * include/duel.h still holds the legacy header until the header switch (H0, build/readability/HEADERS.md).
 * This block declares the part of the canonical duel.h that this unit and the headers below use, with the
 * header's names, types and bitfield containers (unused bytes are padding), and defines duel.h's include
 * guard so that card_list_view.h, chain.h, duel_cmd.h, duel_screen.h and summon.h do not pull in the legacy
 * header. After H0, replace the block (BEGIN to END) with #include "duel.h" and #include "sound.h" (see
 * build/readability/issues/effect_resolve8.md). */
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
    u8 unk4[0x8 - 0x4];
    u8 noBattleDamage:1;            /* +0x008 bit 0 */
    u8 battleProtected:1;           /* +0x008 bit 1 */
    u8 unk8_2:1;
    u8 insectQueenWonBattle:1;      /* +0x008 bit 3 */
    u8 normalSummonUsed:1;          /* +0x008 bit 4: the Normal Summon of this turn is done */
    u8 summonedThisTurn:1;          /* +0x008 bit 5 */
    u8 extraBattlePhase:1;          /* +0x008 bit 6 */
    u8 unk8_7:1;
    u8 unk9[0x28 - 0x9];
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B64 - 0x1ACC];
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
extern struct DuelCard gDuelHands[];            /* 0x02019968 = gDuelPlayers[0].hand (player stride 0xD64) */

u32 IsSpecialSummonOnly(u16 cardId);
int CountActiveCardsOnField(int player, u16 cardNo);
int CountFaceUpMonstersByNumber(int player, u16 cardNo);
int CountFreeMonsterZones(int player);
int FindFreeMonsterZone(int player);
u16 IsTributableMonster(int player, int zone);
int CountTributableMonsters(int player, int excludeZone);
u32 GetZoneCardAtk(u32 player, u32 slot);

/* sound.h (staged) declares this; the legacy include/sound.h does not. */
void PlaySE(u32 seId);
/* ---- END duel.h stand-in ---- */

#include "card_list_view.h"         /* gCardListView, gCardListViewCards, CardListView_Open */
#include "chain.h"                  /* struct ChainEntry, gChain, gChainProxyLink */
#include "duel_actions.h"           /* TributeMonster, MoveFieldCard, AddDeckCardToHand, QueueAddZoneLink */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_prompt.h"            /* DuelPrompt_Post */
#include "duel_screen.h"            /* gDuelScreen (cursor selection), DuelCursor_PickTarget */
#include "effect.h"                 /* CollectEffectTargets, DestroyFieldCardByEffect, prompt strings */
#include "effect_handlers.h"        /* the prototypes of this unit's handlers */
#include "summon.h"                 /* QueueSpecialSummonChoosePosition, QueueNormalSummonChoosePosition */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "util.h"                   /* Random, HalveRoundDown, FormatStr, MemCopy16 */

/*
 * Local views of callees (build/readability/HEADERS.md, "Keeping a deliberate local view").
 */
/* Matching: CollectEffectTargets is defined to return u16; the ROM compares the result as an int (cmp; ble)
 * without narrowing r0. */
int CollectEffectTargetsInt(int player, int cardNumber, int param) asm("CollectEffectTargets");
/* Matching: ShowCardDetail takes a u16 card ID; Seal of the Ancients passes the extracted ID as an int (the
 * u16 parameter changes the register allocation). */
void ShowCardDetailInt(int player, int cardId) asm("ShowCardDetail");
/* Matching: IsTributableMonster returns u16; key 1220 tests r0 without narrowing it (no lsl #16). */
int IsTributableMonsterInt(int player, int zone) asm("IsTributableMonster");
/* Matching: key 1220 calls these handlers with more arguments than their definitions take (r1, r2 set). */
int EffectCanTributeOpponentMonsterPrepare3(struct ChainEntry *card, int chainLink, int fromHand)
    asm("EffectCanTributeOpponentMonsterPrepare");
int EffectCatapultTurtleResolve2(struct ChainEntry *link, int chainedTo) asm("EffectCatapultTurtleResolve");
int EffectTheLittleSwordsmanOfAileResolve2(struct ChainEntry *link, int chainedTo)
    asm("EffectTheLittleSwordsmanOfAileResolve");

/* Prompts used only by this unit (the coin toss and key 1220 prompts are in effect.h). */
extern const char gStrPromptTributeToSpecialSummonFmt[];  /* 0x08083214: "Do you wish to tribute %s and
                                                           * Special Summon %s?" */
extern const char gStrPromptSummonFromHandOrDeck[];       /* 0x08083250: "Where do you Summon from ?: Hand /
                                                           * Deck" */
extern const char gStrTimeWizardSelectTributeFmt[];       /* 0x08083288: "Please select %s as Tribute" */
extern const char gStrSelectMagicFromDeck[];              /* 0x080832AC: "Select a Magic card from the list
                                                           * that you wish to add to your hand from the Deck." */
/* Card IDs of Dark Magician and Dark Sage: address-suffixed aliases of single gCardNumberToId entries
 * (card_data.h). Matching: the ROM loads each entry through its own literal. */
extern const u16 gUnk_08623E38;     /* gCardNumberToId[CARD_DARK_MAGICIAN] */
extern const u16 gUnk_08624758;     /* gCardNumberToId[CARD_DARK_SAGE] */

/* Text box positions and sizes (x | y << 8, width | height << 8, in cells) of the effect prompts. */
#define EFFECT_TEXTBOX_POS          0x206
#define EFFECT_TEXTBOX_POS_HIGH     0x204   /* two rows higher, for the three-line key 1220 menu */
#define COIN_TEXTBOX_SIZE           0x613
#define EFFECT_TEXTBOX_SIZE         0x712
#define TRIBUTE_USE_TEXTBOX_SIZE    0x717
#define SELECT_EFFECT_TEXTBOX_SIZE  0x411

/* Coin faces: the call in gTextBox.result (two-choice menu line 0/1) and each Random() & 1 toss. */
#define COIN_HEADS  0
#define COIN_TAILS  1

/* The step a coin-toss handler returns when it is done: no case matches it, so the next call runs the
 * default case, which marks the once-per-turn effect used (DUEL_CMD_SET_EFFECT_UNUSED with 0). */
#define COIN_EFFECT_STEP_USED   10

/* The command id for player's side: player 1's commands carry DUEL_CMD_PLAYER. */
#define PLAYER_CMD(player, cmd) ((player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))
/* The command id for the other side (the bit is set when player is 0). */
#define OPPONENT_CMD(player, cmd) (!(player) ? DUEL_CMD_PLAYER | (cmd) : (cmd))

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
/* The name record of a card ID in gCardNames (card_data.h declares it as bytes). */
#define CARD_NAME(id)       ((const char *)gCardNames + (id) * CARD_NAME_SIZE)
/* The same record through gCardNames' integer address (0x0822C720). Matching: another literal pool entry. */
#define CARD_NAME_AT_ADDR(id) (((const char (*)[CARD_NAME_SIZE])0x0822C720)[id])

/*
 * &gDuelZones[player].zones[zone] by integer arithmetic, zone term first. Matching: this is the ROM's
 * address order (array indexing emits the player term first). player must be 0 or 1.
 */
#define ZONE_AT(player, zone) \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer) + (u32)gDuelZones))
/* The same address with the player term first, as the ROM adds them in some places. */
#define ZONE_AT_PLAYER_FIRST(player, zone) \
    ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) + (zone) * sizeof(struct DuelZone) + (u32)gDuelZones))
/* The zone holds a card: its card word's ID bits are nonzero. Matching: a whole-word load and lsl #20;
 * a card.id != 0 test generates other code. */
#define ZONE_HAS_CARD(zone) (CARD_WORD((zone)->card) << 20 != 0)

/*
 * Seal of the Ancients: look at every face-down card of the opponent (zones 0-10). Each one is pointed at,
 * turned face up, shown in the Card Detail view to the activating player and turned face down again.
 */
int EffectSealOfTheAncientsResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int zone;

        for (zone = 0; zone <= 10; zone++) {
            int side = (1 - link->player) & 1;
            struct DuelZone *z = ZONE_AT(side, zone);

            /* Matching: the card word and the position byte are read through separate zone pointers. */
            if (ZONE_HAS_CARD(z)) {
                struct DuelZone *z2 = ZONE_AT((1 - link->player) & 1, zone);

                if (!z2->isFaceUp) {
                    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_POINT_AT_CARD), 1 - link->player,
                                 (u8)zone << 8, 0);
                    DuelCmd_Push(OPPONENT_CMD(link->player, DUEL_CMD_FLIP_CARD), zone, 0, 0);
                    ShowCardDetailInt(link->player,
                                      CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST((1 - link->player) & 1, zone)->card)));
                    DuelCmd_Push(OPPONENT_CMD(link->player, DUEL_CMD_FLIP_CARD), zone, 0, 0);
                }
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Graceful Dice, Skull Dice: roll a die (1-6) and link the result to every face-up monster of one side, as a
 * ZONE_LINK_CARD_EFFECT link from the dice card with the roll in the high byte: Graceful Dice on your
 * monsters, Skull Dice on the opponent's.
 */
int EffectDiceResolve(struct ChainEntry *link)
{
    int roll = Random() % 6 + 1;

    if (!link->negated) {
        int cmd;
        int side;
        int zone;
        u8 sideByte;

        switch (CARD_NUMBER(link->card)) {
        case CARD_GRACEFUL_DICE:
            side = link->player;
            cmd = DUEL_CMD_ROLL_GRACEFUL_DICE;
            break;
        case CARD_SKULL_DICE:
            side = 1 - link->player;
            cmd = DUEL_CMD_ROLL_SKULL_DICE;
            break;
        }
        /* FAKEMATCH: the assignment in the arm keeps DUEL_CMD_PLAYER an SImode constant (cmd | 0x8000 would
         * be narrowed to u16) and gives an if/else, so CSE does not share the first player test. */
        DuelCmd_Push(link->player ? (cmd |= DUEL_CMD_PLAYER) : cmd, roll, 0, 0);
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_OPEN_DUEL_SCREEN), 0, 0, 0);

        for (zone = 0; zone <= 4; zone++) {
            struct DuelZone *z = ZONE_AT(side & 1, zone);

            if (z->isFaceUp && ZONE_HAS_CARD(z)) {
                /* FAKEMATCH: the copy is hoisted by loop.c after side & 1, as in the ROM */
                sideByte = side;
                QueueAddZoneLink(link->player, link->card, (u8)zone << 8 | sideByte,
                                 (u8)roll << 8 | ZONE_LINK_CARD_EFFECT);
            }
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Exchange: each player picks a card from the other's hand (PROMPT_PICK_OPPONENT_HAND_CARD), then the two
 * cards change hands. targets[0] and targets[1] keep the two picks.
 *   EFFECT_STEP_START  the activating player picks
 *   EFFECT_STEP_2      keep the pick; the opponent picks
 *   EFFECT_STEP_3      keep the pick and swap the cards
 */
int EffectExchangeResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            DuelPrompt_Post(link->player, PROMPT_PICK_OPPONENT_HAND_CARD, 0, 0);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:
            link->targets[0] = gDuel.promptResult;
            DuelPrompt_Post(1 - link->player, PROMPT_PICK_OPPONENT_HAND_CARD, 0, 0);
            return EFFECT_STEP_3;
        case EFFECT_STEP_3:
            link->targets[1] = gDuel.promptResult;
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_EXCHANGE_HAND_CARDS), gDuel.promptResult,
                         link->targets[0], 0);
            return EFFECT_STEP_END;
        }
    }
    return EFFECT_STEP_DONE;
}

/* EffectTimeWizardResolve's steps (gChain.effectStep). */
enum TimeWizardStep {
    TIME_WIZARD_END = COIN_EFFECT_STEP_USED, /* (default case) mark the effect used */
    TIME_WIZARD_ADD_MAGIC = 0x60,           /* add the chosen Magic card to the hand */
    TIME_WIZARD_OPEN_MAGIC_LIST = 0x61,
    TIME_WIZARD_OFFER_MAGIC = 0x62,         /* 'Select a Magic card ...' if the deck has one */
    TIME_WIZARD_WAIT_SUMMON = 0x63,
    TIME_WIZARD_SUMMON_DARK_SAGE = 0x64,
    TIME_WIZARD_PICK_TRIBUTE = 0x6D,        /* cursor on a face-up Dark Magician */
    TIME_WIZARD_PROMPT_TRIBUTE = 0x6E,
    TIME_WIZARD_PICK_SOURCE = 0x76,         /* Hand / Deck answer */
    TIME_WIZARD_ANSWER = 0x77,              /* Yes/No answer; where can Dark Sage come from */
    TIME_WIZARD_OFFER_DARK_SAGE = 0x78,
    TIME_WIZARD_TOSS = EFFECT_STEP_2,
    TIME_WIZARD_CALL = EFFECT_STEP_START,
};

/*
 * Time Wizard: call a coin toss (the human picks Heads or Tails, the CPU at random). A right call destroys
 * all of the opponent's monsters; a wrong one destroys your own and you lose half their total ATK.
 * After a right call, with a free monster zone, Dark Sage in the hand or deck, no key 1418 (forbids
 * tributes) in effect and a face-up Dark Magician, you may tribute the Dark Magician to Special Summon Dark
 * Sage, and then add a Magic card from the deck to the hand. targets[0] is the source Dark Sage comes from
 * (CARDLIST_SRC_HAND or CARDLIST_SRC_DECK).
 */
int EffectTimeWizardResolve(struct ChainEntry *link)
{
    char text[0x100];
    char format[0x100];
    int i;

    if (!link->negated) {
        switch (gChain.effectStep) {
        case TIME_WIZARD_CALL:
            if (!link->player) {
                TextBoxOpen(EFFECT_TEXTBOX_POS, COIN_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrCoinTossSelection);
                TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
            } else {
                gTextBox.result = Random() & 1;
            }
            return TIME_WIZARD_TOSS;
        case TIME_WIZARD_TOSS: {
            u8 toss = Random() & 1;

            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_TOSS_COIN), gTextBox.result, toss, 0);
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_OPEN_DUEL_SCREEN), 0, 0, 0);
            if (toss == gTextBox.result) {
                for (i = 0; i <= 4; i++) {
                    int side = (1 - link->player) & 1;
                    struct DuelZone *z = ZONE_AT(side, i);

                    if (ZONE_HAS_CARD(z)) {
                        DestroyFieldCardByEffect(1 - link->player, i);
                        OnCardDestroyedByEffect(link->player, 1 - link->player, i);
                    }
                }
                if (CountFreeMonsterZones(link->player) <= 0)
                    goto done;
                /* the target collector of Time Wizard lists Dark Sage in the hand and deck */
                if (CollectEffectTargetsInt(link->player, CARD_TIME_WIZARD, 0) <= 0)
                    goto done;
                if (CountActiveCardsOnField(0, CARD_1418) != 0 || CountActiveCardsOnField(1, CARD_1418) != 0)
                    goto done;
                if (CountFaceUpMonstersByNumber(link->player, CARD_DARK_MAGICIAN) == 0
                    && CountFaceUpMonstersByNumber(link->player, CARD_1210) == 0
                    && CountFaceUpMonstersByNumber(link->player, CARD_DARK_MAGICIAN_ALT) == 0)
                    goto done;
                return TIME_WIZARD_OFFER_DARK_SAGE;
            } else {
                int totalAtk = 0;

                for (i = 0; i <= 4; i++) {
                    int player = link->player;
                    struct DuelZone *z = ZONE_AT(player, i);

                    if (ZONE_HAS_CARD(z)) {
                        totalAtk += GetZoneCardAtk(link->player, i);
                        DestroyFieldCardByEffect(link->player, i);
                    }
                }
                LoseLifePoints(link->player, HalveRoundDown(totalAtk));
                goto done;
            }
        }
        case TIME_WIZARD_OFFER_DARK_SAGE:
            /* "Do you wish to tribute Dark Magician and Special Summon Dark Sage?" */
            FormatStr(format, gStrPromptTributeToSpecialSummonFmt, CARD_NAME(gUnk_08623E38));
            FormatStr(text, format, CARD_NAME(gUnk_08624758));
            TextBoxOpen(EFFECT_TEXTBOX_POS, COIN_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, text);
            TextBoxSetMenu(TEXTBOX_MENU_YES_NO, NULL, NULL);
            return TIME_WIZARD_ANSWER;
        case TIME_WIZARD_ANSWER: {
            int inHand;
            int inDeck;

            if (gTextBox.result == 0)
                goto done;
            inHand = 0;
            inDeck = 0;
            for (i = 0; i < gCardListView.count; i++) {
                if (gCardListView.sources[i] != CARDLIST_SRC_HAND) {
                    if (gCardListView.sources[i] == CARDLIST_SRC_DECK)
                        inDeck = 1;
                } else {
                    inHand = 1;
                }
            }
            if (inHand) {
                if (inDeck) {
                    TextBoxOpen(EFFECT_TEXTBOX_POS, COIN_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT,
                                gStrPromptSummonFromHandOrDeck);
                    TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
                    return TIME_WIZARD_PICK_SOURCE;
                }
                link->targets[0] = CARDLIST_SRC_HAND;
                return TIME_WIZARD_PROMPT_TRIBUTE;
            }
            if (inDeck) {
                link->targets[0] = CARDLIST_SRC_DECK;
                return TIME_WIZARD_PROMPT_TRIBUTE;
            }
            return TIME_WIZARD_END;
        }
        case TIME_WIZARD_PICK_SOURCE:
            switch (gTextBox.result) {
            case 0:
                link->targets[0] = CARDLIST_SRC_HAND;
                break;
            case 1:
                link->targets[0] = CARDLIST_SRC_DECK;
                break;
            }
            return TIME_WIZARD_PROMPT_TRIBUTE;
        case TIME_WIZARD_PROMPT_TRIBUTE:
            /* "Please select Dark Magician as Tribute" */
            FormatStr(text, gStrTimeWizardSelectTributeFmt, CARD_NAME_AT_ADDR(gUnk_08623E38));
            TextBoxOpen(EFFECT_TEXTBOX_POS, COIN_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, text);
        pickTribute:
            return TIME_WIZARD_PICK_TRIBUTE;
        case TIME_WIZARD_PICK_TRIBUTE:
            if (DuelCursor_PickTarget(PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION) != 0) {
                int player = gDuelScreen.selPlayer;
                int zone = gDuelScreen.selArea + gDuelScreen.selIndex;
                int side = player & 1;
                struct DuelZone *z = ZONE_AT(side, zone);
                int cardNo = ((const u16 *)0x08622AB4)[CARD_ID11(CARD_WORD(z->card))];

                switch (cardNo) {
                case CARD_DARK_MAGICIAN:
                case CARD_1210:
                case CARD_DARK_MAGICIAN_ALT:
                    TributeMonster(player, zone);
                    return TIME_WIZARD_SUMMON_DARK_SAGE;
                default:
                    PlaySE(SE_ERROR);
                    goto pickTribute;
                }
            }
            goto pickTribute;
        case TIME_WIZARD_SUMMON_DARK_SAGE: {
            /* the first Dark Sage of the chosen source */
            for (i = 0; i < gCardListView.count; i++) {
                if (gCardListView.sources[i] == link->targets[0]) {
                    u32 *card = &gCardListView.cards[i];

                    DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_REMOVE_CARD_FROM_HAND), ((u16 *)card)[0],
                                 ((u16 *)card)[1], 0);
                    QueueSpecialSummonChoosePosition(link->player, (struct DuelCard *)card, 1, 1);
                    return TIME_WIZARD_WAIT_SUMMON;
                }
            }
        done:
            return TIME_WIZARD_END;
        }
        case TIME_WIZARD_WAIT_SUMMON:
            return TIME_WIZARD_OFFER_MAGIC;
        case TIME_WIZARD_OFFER_MAGIC:
            /* the target collector of Dark Sage lists the deck's Magic cards */
            if (CollectEffectTargetsInt(link->player, CARD_DARK_SAGE, 0) > 0) {
                TextBoxOpen(EFFECT_TEXTBOX_POS, COIN_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectMagicFromDeck);
                return TIME_WIZARD_OPEN_MAGIC_LIST;
            }
            goto done;
        case TIME_WIZARD_OPEN_MAGIC_LIST:
            CardListView_Open(link->player, -1, CARD_DARK_SAGE, 0);
            return TIME_WIZARD_ADD_MAGIC;
        case TIME_WIZARD_ADD_MAGIC: {
            int player = link->player;
            u32 card = gCardListView.cards[gCardListView.top + gCardListView.cursorRow];

            if (AddDeckCardToHand(player, ((const u16 *)0x08622AB4)[CARD_ID11(card)]))
                DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SHUFFLE_DECK), 0, 0, 0);
            goto done;
        }
        default:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Goddess of Whim: call a coin toss (the human picks, the CPU at random). The result is kept as a
 * ZONE_LINK_CARD_EFFECT link on the card's own zone, with 1 in the high byte for a right call and 0 for a
 * wrong one. Uses up the once-per-turn effect.
 *   EFFECT_STEP_START  the call
 *   EFFECT_STEP_2      toss and link the result
 *   (default)          mark the effect used
 */
int EffectGoddessOfWhimResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            if (!link->player) {
                TextBoxOpen(EFFECT_TEXTBOX_POS, COIN_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrCoinTossSelection);
                TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
            } else {
                gTextBox.result = Random() & 1;
            }
            return EFFECT_STEP_2;
        case EFFECT_STEP_2: {
            u8 toss = Random() & 1;

            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_TOSS_COIN), gTextBox.result, toss, 0);
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_OPEN_DUEL_SCREEN), 0, 0, 0);
            QueueAddZoneLink(link->player, link->card, link->player | link->zone << 8,
                             toss == gTextBox.result ? 1 << 8 | ZONE_LINK_CARD_EFFECT : ZONE_LINK_CARD_EFFECT);
            return COIN_EFFECT_STEP_USED;
        }
        default:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Barrel Dragon: toss three coins; with two or more heads, destroy the targeted monster of the opponent.
 * Uses up the once-per-turn effect. The toss command gets the tails as a bit mask.
 */
int EffectBarrelDragonResolve(struct ChainEntry *link)
{
    if (!link->negated) {
        int tailsMask = 0;
        int heads = 0;
        int i;

        for (i = 0; i <= 2; i++) {
            if ((Random() & 1) == COIN_HEADS)
                heads++;
            else
                tailsMask |= 1 << i;
        }
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_TOSS_THREE_COINS), tailsMask, 0, 0);
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_OPEN_DUEL_SCREEN), 0, 0, 0);
        if (heads > 1) {
            DestroyFieldCardByEffect(1 - link->player, link->targets[0] >> 8);
            OnCardDestroyedByEffect(link->player, 1 - link->player, link->targets[0] >> 8);
        }
        DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_EFFECT_UNUSED), link->zone, 0, 0);
    }
    return EFFECT_STEP_DONE;
}

/*
 * Effect key 1212 (no EDS card): Special Summon a Dark Magician from the deck (the target collector puts it
 * in gCardListView.cards[0]), then lock the player's summons (DUEL_CMD_SET_SUMMON_LOCKS: no Normal and no
 * Special Summon). The locks also apply when the activation was negated, unless the card is destroyed with
 * the negation.
 *   EFFECT_STEP_START  take the Dark Magician out of the deck
 *   EFFECT_STEP_2      Special Summon it (position chosen)
 *   (next call)        lock summons
 */
int EffectSummonDarkMagicianFromDeckResolve(struct ChainEntry *link)
{
    /* gCardListView.cards[0] as two halfwords, through the alias symbol gCardListViewCards */
    u16 *found = (u16 *)gCardListViewCards;

    if (link->negated) {
        if (!link->destroyIfNegated)
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_SUMMON_LOCKS), 1, 1, 0);
    } else {
        switch (gChain.effectStep) {
        case EFFECT_STEP_START:
            if (CollectEffectTargetsInt(link->player, CARD_NUMBER(link->card), 0) == 0)
                break;
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_REMOVE_CARD_FROM_DECK), found[0], found[1], 0);
            return EFFECT_STEP_2;
        case EFFECT_STEP_2:
            QueueSpecialSummonChoosePosition(link->player, (struct DuelCard *)found, 1, 0);
            return EFFECT_STEP_3;
        default:
            DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_SET_SUMMON_LOCKS), 1, 1, 0);
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/*
 * Effect key 1213 (no EDS card): with targets[0] an opponent's monster and targets[1] one of yours, destroy
 * the opponent's monster, then move yours into its zone (control passes to the opponent; hypothesis: OCG
 * Mystic Box).
 *   EFFECT_STEP_START  destroy the opponent's monster
 *   EFFECT_STEP_2      move your monster
 */
int EffectDestroyAndGiveMonsterResolve(struct ChainEntry *link)
{
    if (!link->negated && link->numTargets == 2) {
        u8 destroyPlayer = link->targets[0];
        int destroyZone = link->targets[0] >> 8;
        u8 givePlayer = link->targets[1];
        int giveZone = link->targets[1] >> 8;

        switch (gChain.effectStep) {
        case EFFECT_STEP_START: {
            int destroySide = destroyPlayer & 1;
            struct DuelZone *destroyed = ZONE_AT(destroySide, destroyZone);

            if (ZONE_HAS_CARD(destroyed)) {
                int giveSide = givePlayer & 1;
                struct DuelZone *given = ZONE_AT(giveSide, giveZone);

                if (ZONE_HAS_CARD(given)) {
                    if (destroyPlayer != link->player && givePlayer == link->player) {
                        DestroyFieldCardByEffect(destroyPlayer, destroyZone);
                        OnCardDestroyedByEffect(link->player, destroyPlayer, destroyZone);
                        return EFFECT_STEP_2;
                    }
                }
            }
            break;
        }
        case EFFECT_STEP_2:
            MoveFieldCard(link->player, link->targets[1], link->targets[0]);
            break;
        }
    }
    return EFFECT_STEP_DONE;
}

/* &gDuelPlayers[player].zones[zone] from the gDuelPlayers symbol, zone term first. */
#define PLAYER_ZONE_AT(player, zone)                                                                          \
    ((struct DuelZone *)((zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer)              \
                         + ((u32)gDuelPlayers + OFFSET_OF(struct DuelPlayer, zones))))
/* The card word of hand[index] of the player, through the gDuelHands symbol (= gDuelPlayers[0].hand). */
#define HAND_CARD_WORD(player, index) \
    (*(u32 *)((player) * sizeof(struct DuelPlayer) + (index) * sizeof(struct DuelCard) + (u32)gDuelHands))

/* enum CardType of a card ID, as a u8. */
static inline u8 GetCardType(u16 cardId)
{
    return CARD_TYPE(cardId);
}

/* Level of a card: 0 for Trap, Magic and Ticket cards, 10 for the Divine cards, else the stats' level. */
static inline u8 GetCardLevel(u16 cardId)
{
    u8 level;

    switch ((int)CARD_TYPE(cardId)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        level = 0;
        break;
    case CARD_TYPE_DIVINE:
        level = 10;
        break;
    default:
        level = CARD_STATS_LEVEL(CARD_STATS(cardId));
        break;
    }
    return level;
}

/* EffectTributeOpponentMonsterResolve's steps (gChain.effectStep). */
enum TributeOpponentStep {
    TRIBUTE_OPP_EFFECT_DONE = 0x61,         /* no case: ends */
    TRIBUTE_OPP_RUN_EFFECT = 0x62,          /* run the chosen monster's effect with the target as tribute */
    TRIBUTE_OPP_PICK_EFFECT_MONSTER = 0x63,
    TRIBUTE_OPP_PROMPT_EFFECT = 0x64,       /* 'Select the Effect Monster ...' */
    TRIBUTE_OPP_SUMMON_DONE = 0x6E,         /* no case: ends */
    TRIBUTE_OPP_PICK_SECOND_TRIBUTE = 0x76, /* level 7 or higher: one more own monster */
    TRIBUTE_OPP_PICK_HAND_MONSTER = 0x77,
    TRIBUTE_OPP_PROMPT_SUMMON = 0x78,       /* 'Select a high-level monster ...' */
    TRIBUTE_OPP_ANSWER = EFFECT_STEP_3,     /* Summon (0) or effect (1) */
    TRIBUTE_OPP_CHOOSE_USE = EFFECT_STEP_2,
    TRIBUTE_OPP_CHECK = EFFECT_STEP_START,
};

/*
 * Effect key 1220 (no EDS card; hypothesis: OCG Soul Exchange): the targeted monster of the opponent
 * (targets[0]) may be used as a tribute, either for a Tribute Summon of a level 5 or higher monster from the
 * hand, or for the effect of a face-up Catapult Turtle, The Little Swordsman of Aile or Cannon Soldier.
 * With both possible, a menu asks which. The summon path sets normalSummonUsed.
 * ROM bug (hypothesis): both summon paths call TributeMonster(targets[0], 0), passing the packed
 * player | zone << 8 as the player and zone 0, so the monster in that side's zone 0 is tributed; the effect
 * path hands the target to the effect correctly. chainedTo is passed on to the Prepare check.
 */
int EffectTributeOpponentMonsterResolve(struct ChainEntry *link, int chainedTo)
{
    char text[0x80]; /* unused: the ROM reserves these 0x80 stack bytes and never touches them */

    if (!link->negated) {
        switch (gChain.effectStep) {
        case TRIBUTE_OPP_CHECK:
            if (EffectCanTributeOpponentMonsterPrepare3(link, chainedTo, 0) != 0)
                return TRIBUTE_OPP_CHOOSE_USE;
            break;
        case TRIBUTE_OPP_CHOOSE_USE: {
            int canSummon = 0;
            int hasEffectMonster;
            int i;

            /* A Tribute Summon is possible with the Normal Summon unused and a level 5-6 monster in the hand
             * and a free zone, or a level 7+ monster and one more tributable monster of the player. */
            if (!gDuelPlayers[1 & link->player].normalSummonUsed) {
                for (i = 0; i < gDuelPlayers[1 & link->player].handCount; i++) {
                    u16 cardId = CARD_ID(HAND_CARD_WORD(link->player, i));

                    if (GetCardType(cardId) <= CARD_TYPE_REPTILE && IsSpecialSummonOnly(cardId) == 0) {
                        if (GetCardLevel(cardId) > 4) {
                            if (GetCardLevel(cardId) <= 6) {
                                if (FindFreeMonsterZone(link->player) != -1)
                                    canSummon = 1;
                            } else {
                                if (CountTributableMonsters(link->player, -1) > 0)
                                    canSummon = 1;
                            }
                        }
                    }
                }
            }
            if (gDuelPlayers[link->player].normalSummonUsed)
                canSummon = 0;
            hasEffectMonster = 0;
            for (i = 0; i <= 4; i++) {
                int side = 1 & link->player;
                struct DuelZone *z = PLAYER_ZONE_AT(side, i);
                u16 cardId = CARD_ID(CARD_WORD(z->card));

                if (cardId != 0) {
                    /* Matching: the position byte is read through a second zone pointer. */
                    struct DuelZone *z2 = PLAYER_ZONE_AT(1 & link->player, i);

                    if (z2->isFaceUp) {
                        switch (CARD_NUMBER(cardId)) {
                        case CARD_CATAPULT_TURTLE:
                        case CARD_THE_LITTLE_SWORDSMAN_OF_AILE:
                        case CARD_CANNON_SOLDIER:
                            hasEffectMonster = 1;
                            break;
                        }
                    }
                }
            }
            if (canSummon != 0) {
                if (hasEffectMonster == 0) {
                promptSummon:
                    return TRIBUTE_OPP_PROMPT_SUMMON;
                }
            } else {
                if (hasEffectMonster != 0)
                    goto promptEffect;
                break;
            }
            TextBoxOpen(EFFECT_TEXTBOX_POS_HIGH, TRIBUTE_USE_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrPromptTributeUse);
            TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
            return TRIBUTE_OPP_ANSWER;
        }
        case TRIBUTE_OPP_ANSWER:
            if (gTextBox.result == 0)
                goto promptSummon;
        promptEffect:
            return TRIBUTE_OPP_PROMPT_EFFECT;
        case TRIBUTE_OPP_PROMPT_SUMMON:
            TextBoxOpen(EFFECT_TEXTBOX_POS, EFFECT_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectHighLevelMonster);
            gDuelPlayers[link->player].normalSummonUsed = 1;
        pickHandMonster:
            return TRIBUTE_OPP_PICK_HAND_MONSTER;
        case TRIBUTE_OPP_PICK_HAND_MONSTER: {
            int handIndex;
            u16 cardId;

            if (DuelCursor_PickTarget(PICK_HAND) == 0)
                goto pickHandMonster;
            handIndex = gDuelScreen.selIndex;
            cardId = CARD_ID(HAND_CARD_WORD(link->player, handIndex));
            if (GetCardType(cardId) <= CARD_TYPE_REPTILE && IsSpecialSummonOnly(cardId) == 0) {
                if (GetCardLevel(cardId) > 4) {
                    if (GetCardLevel(cardId) <= 6) {
                        TributeMonster(link->targets[0], 0);
                        QueueNormalSummonChoosePosition(link->player, handIndex, FindFreeMonsterZone(link->player),
                                                        0);
                        return TRIBUTE_OPP_SUMMON_DONE;
                    }
                    TextBoxOpen(EFFECT_TEXTBOX_POS, EFFECT_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT,
                                gStrSelectSecondTribute);
                    link->targets[1] = handIndex;
                pickSecondTribute:
                    return TRIBUTE_OPP_PICK_SECOND_TRIBUTE;
                }
            }
            PlaySE(SE_ERROR);
            goto pickHandMonster;
        }
        case TRIBUTE_OPP_PICK_SECOND_TRIBUTE:
            if (DuelCursor_PickTarget(PICK_ANY_MONSTER) == 0)
                goto pickSecondTribute;
            if (IsTributableMonsterInt(link->player, gDuelScreen.selIndex) != 0) {
                TributeMonster(link->targets[0], 0);
                TributeMonster(link->player, gDuelScreen.selIndex);
                /* the second tribute's zone is free now */
                QueueNormalSummonChoosePosition(link->player, link->targets[1], gDuelScreen.selIndex, 0);
                return TRIBUTE_OPP_SUMMON_DONE;
            }
            PlaySE(SE_ERROR);
            goto pickSecondTribute;
        case TRIBUTE_OPP_PROMPT_EFFECT:
            TextBoxOpen(EFFECT_TEXTBOX_POS, SELECT_EFFECT_TEXTBOX_SIZE, TEXTBOX_FLAGS_DEFAULT, gStrSelectEffectMonster);
        pickEffectMonster:
            return TRIBUTE_OPP_PICK_EFFECT_MONSTER;
        case TRIBUTE_OPP_PICK_EFFECT_MONSTER: {
            int player;
            int zone;

            if (DuelCursor_PickTarget(PICK_FACE_UP_MONSTER | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION) == 0)
                goto pickEffectMonster;
            player = gDuelScreen.selPlayer;
            zone = gDuelScreen.selArea + gDuelScreen.selIndex;
            switch (((const u16 *)0x08622AB4)[CARD_ID11(CARD_WORD(ZONE_AT_PLAYER_FIRST(1 & player, zone)->card))]) {
            case CARD_CATAPULT_TURTLE:
            case CARD_THE_LITTLE_SWORDSMAN_OF_AILE:
            case CARD_CANNON_SOLDIER:
                PlaySE(SE_CONFIRM);
                DuelCmd_Push(PLAYER_CMD(link->player, DUEL_CMD_POINT_AT_CARD), (u16)gDuelScreen.selPlayer,
                             (u8)gDuelScreen.selArea | (u8)gDuelScreen.selIndex << 8, 0);
                /* Run the chosen monster's effect as a copy of this link with its card and zone. */
                MemCopy16(&gChainProxyLink, link, sizeof(struct ChainEntry));
                gChainProxyLink.card = CARD_ID(CARD_WORD(ZONE_AT_PLAYER_FIRST(1 & player, zone)->card));
                gChainProxyLink.zone = zone;
                return TRIBUTE_OPP_RUN_EFFECT;
            }
            PlaySE(SE_ERROR);
            goto pickEffectMonster;
        }
        case TRIBUTE_OPP_RUN_EFFECT:
            /* Matching: the switch reads the copy through gChain, the Catapult Turtle call passes the alias
             * symbol gChainProxyLink, and the Little Swordsman call &gChain.proxyLink. */
            switch (CARD_NUMBER(gChain.proxyLink.card)) {
            case CARD_CATAPULT_TURTLE:
            case CARD_CANNON_SOLDIER:
                EffectCatapultTurtleResolve2(&gChainProxyLink, 0);
                return TRIBUTE_OPP_EFFECT_DONE;
            case CARD_THE_LITTLE_SWORDSMAN_OF_AILE:
                EffectTheLittleSwordsmanOfAileResolve2(&gChain.proxyLink, 0);
                return TRIBUTE_OPP_EFFECT_DONE;
            }
            break;
        }
    }
    return EFFECT_STEP_DONE;
}
