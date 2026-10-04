#include "global.h"
#include "legacy/gba.h"                    /* A_BUTTON, B_BUTTON, DPAD_LEFT, DPAD_RIGHT, OBJ_PLTT, OBJ_VRAM0 */
#include "legacy/main.h"                   /* gMain.newKeys, gMain.heldKeys */
#include "util.h"                   /* StrCopy, StrCat, Random, MemCopy16 */
#include "sprite.h"                 /* AddSprite, SPRITE_SHAPE_* */
#include "text.h"                   /* TextCanvasInit, TextDrawString, TextCanvasToTiles, gSystemFontPal */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu, TextBoxMenuState */
#include "card_data.h"              /* gCardStats, CARD_STATS_*, CARD_ID_MASK */
#include "constants/cards.h"        /* CARD_SPEAR_CRETIN, CARD_THE_SHALLOW_GRAVE, CARD_1520 */
#include "constants/card_stats.h"   /* enum CardType, enum CardAttribute */
#include "constants/duel.h"         /* enum DuelArea, enum FieldPickMask */
#include "constants/duel_cmds.h"    /* DUEL_CMD_POINT_AT_CARD, ... */
#include "constants/sound.h"        /* SE_CONFIRM, SE_ERROR */

/*
 * Duel prompt handlers (wiki/functions/duel-prompt-handlers-c.md).
 *
 * A card effect posts a prompt (duel_prompt.h); every frame DuelPrompt_Run calls the handler of
 * gDuel.promptKind until it returns 1, with gDuel.promptStep as its step counter and the answer in
 * gDuel.promptResult. This unit holds twelve of the handlers: random hand discard and banish (kinds 3-5),
 * picking a card from the opponent's revealed hand (6), tribute (7), choosing a monster type or one or
 * two attributes (8-11: DNA Surgery, The Regulation of Tribe, Earthshaker), Setting a Level 4 or lower monster
 * from the hand (13) and choosing a Graveyard monster from the card list (14).
 *
 * It also has the text-box callbacks of the type and attribute menus, the CPU's auto-pick input of the
 * two-line choice menu, and DuelCursor_IsValidTarget, the filter behind the effect cursor (duel_cursor.c).
 *
 * Players: 0 is the human, 1 the CPU (or the link partner). The picks for the CPU are made at once; the human
 * gets a text box and a cursor.
 */

/* ---- BEGIN pre-H0 subset of duel.h ---- */
/*
 * The part of the staged duel.h that this unit and the headers it includes need (names, types and bitfield
 * containers as there; unused bytes are padding). include/duel.h still holds the legacy header until the
 * header switch (H0, build/readability/HEADERS.md), and chain.h, duel_screen.h, summon.h, card_list_view.h and
 * duel_cmd.h include it, so this block also defines its include guard. After H0, replace the block (BEGIN to
 * END) with #include "legacy/duel.h".
 */
#define GUARD_DUEL_H

struct DuelCard {
    u32 id:12;                      /* bits 0-11: card ID */
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
    u8 unk6_2:6;
    u8 unk7[3];                     /* +0x07 */
    u16 links[32];                  /* +0x0A: DUEL_LOC of a card affecting this one, or a value */
    u16 linkKinds[32];              /* +0x4A: low byte enum ZoneLinkKind */
    u16 numLinks;                   /* +0x8A: entries in links / linkKinds */
    u8 unk8C[8];                    /* +0x8C */
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 unk3[6];                     /* +0x003 */
    u8 handRevealed:1;              /* +0x009 bit 0: forces IsHandRevealed */
    u8 unk9_1:7;
    u8 unkA[0x28 - 0xA];            /* +0x00A */
    struct DuelZone zones[11];      /* +0x028: enum DuelZoneIndex */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0xD64 - 0x7C4];       /* deck, graveyard, fusion deck, banished */
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1: player whose turn it is */
    u8 unk1B12_2:6;
    u8 unk1B13[0x1B50 - 0x1B13];
    u8 promptLinked:1;              /* +0x1B50 bit 0: the prompt is mirrored over the link */
    u8 promptActive:1;              /* +0x1B50 bit 1: a prompt is pending */
    u8 promptPlayer:1;              /* +0x1B50 bit 2: player who answers (1 = CPU or link partner) */
    u8 unk1B50_3:1;
    u16 promptKind:6;               /* +0x1B50 bits 4-9: enum DuelPromptKind */
    u16 unk1B51_2:6;
    u16 promptArgs[8];              /* +0x1B52: [0] argument, [1] value (DuelPrompt_Post); all 8 for PostData */
    u8 promptStep;                  /* +0x1B62: step of the running prompt handler */
    u8 unk1B63;
    u16 promptResult;               /* +0x1B64: the answer (a hand slot, a card ID, an attribute - 1) */
    u8 unk1B66[0x1B78 - 0x1B66];
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */

u32 IsSpecialSummonOnly(u16 cardId);                    /* Fusion, Ritual and some effect monsters */
int FindFreeMonsterZone(int player);                    /* first free monster zone, or -1 */
int CountTributableMonsters(int player, int excludeZone); /* excludeZone -1 = none */
void PlaySE(u32 seId);                                  /* legacy sound.h lacks it */
/* ---- END pre-H0 subset ---- */

#include "chain.h"                  /* gChain.handPickTimer, gChain.handPickCount */
#include "duel_screen.h"            /* gDuelScreen, DuelCursor_PickTarget, DuelCursor_Select, ... */
#include "card_list_view.h"         /* gCardListView, gCardListViewCards, CardListView_Open */
#include "summon.h"                 /* CanSummonFromHand */
#include "duel_cmd.h"               /* DuelCmd_Push */
#include "duel_actions.h"           /* TributeMonster, DiscardHandCard */
#include "ai.h"                     /* AiPickOpponentHandCard, AiPickTributeMonster, AiPickCardListEntry */
#include "duel_prompt.h"            /* the handlers defined here */

/*
 * Local views kept for matching (build/readability/HEADERS.md):
 * - DuelPrompt_DiscardRandom, DuelPrompt_SetMonsterFromHand and the CPU branch of
 *   DuelPrompt_PickOpponentHandCard call DuelCmd_Push with u16 arguments (the header has int for the last two);
 * - CardListView_Open is called with u16 card number and argument (the header has int);
 * - AttributeMenu_HandleInputExcludeFirst reads promptResult through its own symbol gUnk_0201AE44;
 * - the second answer halfword of gDuel (+0x1B66), which the header does not name (DuelPromptAnswers);
 * - the random-pick handlers reach gDuel.promptStep and their player from the base of gDuelPlayers
 *   (PROMPT_STEP_VIA_PLAYERS).
 */
extern void DuelCmd_Push16(u16 cmd, u16 arg2, u16 arg4, u16 arg6) asm("DuelCmd_Push");
extern void CardListView_Open16(s32 player, s32 area, u16 cardNumber, u16 arg) asm("CardListView_Open");
extern u16 gUnk_0201AE44;       /* 0x0201AE44: gDuel.promptResult, through its own symbol in the second attribute menu */

/* Matching: the second answer halfword at gDuel +0x1B66 (promptResult2). The header declares only promptResult
 * (+0x1B64) and leaves the halfword after it as padding (build/readability/issues/duel_prompt_handlers.md); this
 * view gives the store the ROM's code. */
struct DuelPromptAnswers {
    u8 pad[OFFSET_OF(struct DuelState, promptResult)];
    u16 answer;                     /* +0x1B64: gDuel.promptResult */
    u16 answer2;                    /* +0x1B66 */
};
#define PROMPT_RESULT_2(duel)   (((struct DuelPromptAnswers *)(duel))->answer2)

/* The card-list entry under the cursor (cards[top + cursorRow]), as a card word. */
#define LIST_CURSOR_CARD    (gCardListView.cards[gCardListView.cursorRow + gCardListView.top])

/* ROM data used only here: the prompt texts, the menu strings and the name tables. */
extern const u8 gStrNewline[];                          /* a line break */
extern const u8 gStrMenuIndent[];                       /* four spaces */
extern const char *const gAttributeNames[];             /* 0x0819D264: LIGHT, DARK, WATER, FIRE, EARTH, WIND */
extern const char *const gMonsterTypeNames[];           /* 0x0819D214: the 20 monster type names, Dragon first */
extern const u8 gStrPromptSelectOpponentHandCard[];
extern const u8 gStrPromptSelectTribute[];              /* "Select a monster as @3Tribute@0." */
extern const u8 gStrPromptSelectMonsterToSet[];         /* "Select monster of Level 4 or below for setting on the Field." */
extern const u8 gStrPromptGraveMonsterToPlay[];         /* "... from the Graveyard" */
extern const u8 gStrPromptGraveMonsterToSet[];
extern const u8 gStrPromptGraveMonsterToSpecialSummon[];
extern const u8 gStrPromptSelectType[];                 /* "Select @3Type@0." */
extern const u8 gStrPromptSelectAttribute[];            /* "Select @3Attribute@0." */
extern const u8 gStrPromptSelectAnotherAttribute[];     /* "Select another @3Attribute@0." */

/* TextBoxOpen position and size of the prompts: x | y << 8 and width | height << 8, in cells. */
#define PROMPT_BOX_POS          0x206       /* cell (6, 2) */
#define PROMPT_BOX_18x7         0x712       /* 'Select ...' prompts with a cursor pick */
#define PROMPT_BOX_15x4         0x40F       /* the type and attribute menus */
#define PROMPT_BOX_17x4         0x411       /* 'Select another Attribute.' */
#define PROMPT_BOX_15x5         0x50F       /* 'Select Attribute.' plus two choice lines */

/* A card word as a u32 and its ID (bits 0-11); the ROM uses these shifts, not the bitfield. */
#define CARD_WORD(card)         (*(u32 *)&(card))
#define CARD_WORD_ID(word)      (((word) << 20) >> 20)

/* Card number of a card ID through the integer address of gCardIdToNumber (0x08622AB4); the symbol gives other
 * code. */
#define CARD_NUMBER_OF(id)      (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
/* gCardStats (0x08621DE0) by integer address, for the same reason. */
#define CARD_STATS_OF(id)       (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])

/* Matching: the random-pick handlers reach gDuel.promptStep from the base of gDuelPlayers (= &gDuel.players),
 * gDuelPlayers + 0x1B5E, and the player as gDuelPlayers + (player & 1) * sizeof(struct DuelPlayer). */
#define PROMPT_STEP_VIA_PLAYERS \
    ((u8 *)gDuelPlayers + (OFFSET_OF(struct DuelState, promptStep) - OFFSET_OF(struct DuelState, players)))

/* The first 10 frames of a random pick the cursor hops about the hand; then the card is picked. */
#define RANDOM_PICK_HOP_FRAMES  10

/* Entries of the menus: gMonsterTypeNames (CARD_TYPE_DRAGON..CARD_TYPE_REPTILE) and gAttributeNames
 * (ATTRIBUTE_LIGHT..ATTRIBUTE_WIND). The menu result is the index, type - 1 or attribute - 1. */
#define MONSTER_TYPE_COUNT      CARD_TYPE_REPTILE
#define ATTRIBUTE_COUNT         ATTRIBUTE_WIND

/* Timing of the CPU's choice (TextBoxHandleChoiceInputCpu), in frames of menuTimer. */
#define CPU_CHOICE_BLINK_END    0x3F        /* the cursor alternates every 4 frames until here */
#define CPU_CHOICE_PICK_FRAME   0x40        /* the CPU picks a line */
#define CPU_CHOICE_THINK_END    0xC0        /* then the choice is shown as confirmed */
#define CPU_CHOICE_HOLD_LAST    0x3B        /* the confirmation is held for 0x3C frames */
#define CPU_CHOICE_HOLD_SKIP    0x33        /* held B / fast-forward skips ahead by 8 while below this */

/* sizeColor argument of TextDrawString: font size in the high byte, colour index in the low byte. */
#define SIZE_COLOR(size, color) (((size) << 8) | (color))

/* arg4 of DUEL_CMD_PLACE_MONSTER_FROM_HAND: zone | hand index << 4 | isFaceUp << 8 | isDefense << 9. */
#define PLACE_ARG_SET           0x200       /* face down, defense position: a Set */

/* Highest level the Set prompt accepts: monsters of level 4 or lower need no Tribute. */
#define MAX_SET_LEVEL           4

/* OBJ palette 15 and the OBJ tile 0x364 where the menu labels are rendered (OBJ_VRAM0 + 0x364 * 0x20). */
#define MENU_LABEL_PALETTE      (OBJ_PLTT + 15 * 0x20)
#define MENU_LABEL_TILE         0x364
#define MENU_LABEL_TILES        ((u16 *)(OBJ_VRAM0 + MENU_LABEL_TILE * 0x20))
#define OBJ_PALETTE_15          0xF000      /* palette bits of an OBJ attr2 */

/*
 * PROMPT_DISCARD_RANDOM: discard `count` random cards of the player's hand. Step 0 scrolls to the hand and sets
 * handPickTimer = 0 and handPickCount = count. Each card then takes 10 frames of random cursor hops
 * (the cursor shows, DuelCursor_Select(player, hand, Random() % handCount)), followed by the pointer
 * command at the hop's slot and DiscardHandCard(player, slot, byOpponent, 1); handPickCount counts down.
 * Returns 1 when handPickCount is 0 or the hand is empty.
 */
int DuelPrompt_DiscardRandom(int player, u16 byOpponent, int count)
{
    u8 *step;
    if (gDuelPlayers[player & 1].handCount != 0) {
        step = PROMPT_STEP_VIA_PLAYERS;
        if (*step == 0) {
            DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
            gChain.handPickTimer = 0;
            gChain.handPickCount = count;
            (*step)++;
            return 0;
        }
        if (gChain.handPickCount != 0) {
            u8 *hopTimer = &gChain.handPickTimer;
            if (*hopTimer < RANDOM_PICK_HOP_FRAMES) {
                gDuelScreen.showCursor = 1;
                DuelCursor_Select(player, DUEL_AREA_HAND, Random() % gDuelPlayers[player & 1].handCount);
                (*hopTimer)++;
                return 0;
            } else {
                u16 msg = DUEL_CMD_POINT_AT_CARD;
                if (player != 0)
                    msg = DUEL_CMD_POINT_AT_CARD | DUEL_CMD_PLAYER;
                DuelCmd_Push16(msg, (u16)player, ((u8)gDuelScreen.selIndex << 8) | DUEL_AREA_HAND, 0);
                DiscardHandCard(player, gDuelScreen.selIndex, byOpponent, 1);
                gChain.handPickCount--;
                *hopTimer = 0;
                return 0;
            }
        }
    }
    return 1;
}

/*
 * PROMPT_BANISH_RANDOM: as DuelPrompt_DiscardRandom, but each pick pushes the pointer command and then
 * DUEL_CMD_BANISH_HAND_CARD (arg2 = the hand slot, arg4 = 1: compact the hand) instead of discarding.
 */
int DuelPrompt_BanishRandom(int player, int unused, int count)
{
    u8 *base = (u8 *)gDuelPlayers;
    struct DuelPlayer *ps = (struct DuelPlayer *)(base + (player & 1) * sizeof(struct DuelPlayer));
    u8 *step;
    if (ps->handCount != 0) {
        step = PROMPT_STEP_VIA_PLAYERS;
        if (*step == 0) {
            DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
            gChain.handPickTimer = 0;
            gChain.handPickCount = count;
            (*step)++;
            return 0;
        }
        if (gChain.handPickCount != 0) {
            u8 *hopTimer = &gChain.handPickTimer;
            if (*hopTimer < RANDOM_PICK_HOP_FRAMES) {
                gDuelScreen.showCursor = 1;
                DuelCursor_Select(player, DUEL_AREA_HAND, Random() % ps->handCount);
                (*hopTimer)++;
                return 0;
            } else {
                u16 msg = DUEL_CMD_POINT_AT_CARD;
                if (player != 0)
                    msg = DUEL_CMD_POINT_AT_CARD | DUEL_CMD_PLAYER;
                DuelCmd_Push(msg, (u16)player, ((u8)gDuelScreen.selIndex << 8) | DUEL_AREA_HAND, 0);
                DuelCmd_Push(player != 0 ? DUEL_CMD_BANISH_HAND_CARD | DUEL_CMD_PLAYER : DUEL_CMD_BANISH_HAND_CARD,
                             (u16)gDuelScreen.selIndex, 1, 0);
                gChain.handPickCount--;
                *hopTimer = 0;
                return 0;
            }
        }
    }
    return 1;
}

/*
 * PROMPT_BANISH_RANDOM_FACE_DOWN: as DuelPrompt_BanishRandom, with DUEL_CMD_BANISH_HAND_CARD_FACE_DOWN.
 */
int DuelPrompt_BanishRandomFaceDown(int player, int unused, int count)
{
    u8 *base = (u8 *)gDuelPlayers;
    struct DuelPlayer *ps = (struct DuelPlayer *)(base + (player & 1) * sizeof(struct DuelPlayer));
    u8 *step;
    if (ps->handCount != 0) {
        step = PROMPT_STEP_VIA_PLAYERS;
        if (*step == 0) {
            DuelScreen_ScrollToZone(player, DUEL_AREA_HAND);
            gChain.handPickTimer = 0;
            gChain.handPickCount = count;
            (*step)++;
            return 0;
        }
        if (gChain.handPickCount != 0) {
            u8 *hopTimer = &gChain.handPickTimer;
            if (*hopTimer < RANDOM_PICK_HOP_FRAMES) {
                gDuelScreen.showCursor = 1;
                DuelCursor_Select(player, DUEL_AREA_HAND, Random() % ps->handCount);
                (*hopTimer)++;
                return 0;
            } else {
                u16 msg = DUEL_CMD_POINT_AT_CARD;
                if (player != 0)
                    msg = DUEL_CMD_POINT_AT_CARD | DUEL_CMD_PLAYER;
                DuelCmd_Push(msg, (u16)player, ((u8)gDuelScreen.selIndex << 8) | DUEL_AREA_HAND, 0);
                DuelCmd_Push(player != 0 ? DUEL_CMD_BANISH_HAND_CARD_FACE_DOWN | DUEL_CMD_PLAYER
                                         : DUEL_CMD_BANISH_HAND_CARD_FACE_DOWN,
                             (u16)gDuelScreen.selIndex, 1, 0);
                gChain.handPickCount--;
                *hopTimer = 0;
                return 0;
            }
        }
    }
    return 1;
}

/*
 * PROMPT_TRIBUTE: the player tributes one of their monsters. Step 0 returns 1 at once when no monster can be
 * tributed. For the CPU, AiPickTributeMonster chooses the zone and TributeMonster runs when it found
 * one; for the human the text 'Select a monster as Tribute.' opens and step 1 waits for the cursor to pick a
 * monster zone (PICK_ANY_MONSTER). On A: SE_CONFIRM, the pointer command at the card and TributeMonster.
 */
int DuelPrompt_Tribute(int player)
{
    struct DuelState *duel = &gDuel;
    u8 *step = &duel->promptStep;
    if (*step == 0) {
        int noExclude = -1;     /* every monster counts */
        if (CountTributableMonsters(player, noExclude) == 0)
            return 1;
        if (player != 0) {
            int zone = AiPickTributeMonster(noExclude, 1);
            if (zone > noExclude)
                TributeMonster(player, zone);
            return 1;
        }
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_18x7, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectTribute);
        (*step)++;
        return 0;
    }
    if (DuelCursor_PickTarget(PICK_ANY_MONSTER)) {
        u32 selPlayer = gDuelScreen.selPlayer;
        u32 zone = gDuelScreen.selArea + gDuelScreen.selIndex;  /* the monster row: area 0 + zone */
        PlaySE(SE_CONFIRM);
        DuelCmd_Push(player != 0 ? DUEL_CMD_POINT_AT_CARD | DUEL_CMD_PLAYER : DUEL_CMD_POINT_AT_CARD,
                       (u16)gDuelScreen.selPlayer,
                       (u8)gDuelScreen.selArea | (((u8)gDuelScreen.selIndex) << 8), 0);
        TributeMonster(selPlayer, zone);
        return 1;
    }
    return 0;
}

/* Monster level of a card ID: 0 for Trap, Magic and Ticket cards, 10 for the Divine cards, else stats bits 25-28. */
static inline u32 GetCardLevel(u16 cardId)
{
    switch ((int)CARD_STATS_TYPE(CARD_STATS_OF(cardId))) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(CARD_STATS_OF(cardId));
    }
}

/*
 * PROMPT_SET_MONSTER_FROM_HAND: Set a monster of level 4 or lower from the hand. Step 0 shows the text,
 * step 1 runs the cursor over the hand (PICK_HAND). On A the hand card must pass CanSummonFromHand, not be
 * special-summon-only and have level <= 4; otherwise SE_ERROR plays and the pick goes on. A valid card pushes
 * DUEL_CMD_PLACE_MONSTER_FROM_HAND with arg4 = zone | hand index << 4 | 0x200 (face down, defense: a Set)
 * and advances the step. Later steps return 1.
 */
int DuelPrompt_SetMonsterFromHand(int player)
{
    struct DuelState *duel = &gDuel;
    u8 *step = &duel->promptStep;
    u8 st = *step;
    switch (st) {
    case 0:
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_18x7, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectMonsterToSet);
        (*step)++;
        return 0;
    case 1:
        if (DuelCursor_PickTarget(PICK_HAND)) {
            int p = 1 & player;
            u16 id = CARD_WORD_ID(CARD_WORD(gDuel.players[p].hand[gDuelScreen.selIndex]));
            if (CanSummonFromHand(player, id) != 0 && IsSpecialSummonOnly(id) == 0) {
                u32 level = GetCardLevel(id);
                if (level <= MAX_SET_LEVEL) {
                    DuelCmd_Push16(player != 0 ? DUEL_CMD_PLACE_MONSTER_FROM_HAND | DUEL_CMD_PLAYER
                                               : DUEL_CMD_PLACE_MONSTER_FROM_HAND,
                                   id,
                                   ((gDuelScreen.selIndex & 0xF) << 4) | (FindFreeMonsterZone(player) & 0xF) | PLACE_ARG_SET,
                                   0);
                    gDuel.promptStep++;
                    return 0;
                }
            }
            PlaySE(SE_ERROR);
        }
        return 0;
    default:
        return 1;
    }
}

/*
 * PROMPT_SELECT_GRAVEYARD_MONSTER: pick a Graveyard monster for the effect card cardId (Spear Cretin: play,
 * The Shallow Grave: Set, key 1520: Special Summon); `collectorArg` goes to the card-list collector. The CPU
 * takes the list entry AiPickCardListEntry chooses and finishes at once. The human: step 0 shows the text
 * for the effect (any other card returns 1 without an answer), step 1 opens the card list of effect
 * targets, and the later steps take cards[top + cursorRow]; on player 1's turn that card's owner bit is
 * flipped first. The chosen card word goes to promptResult (low half) and the halfword after it (high half); returns 1.
 */
int DuelPrompt_SelectGraveyardMonster(s32 player, u16 cardId, u16 collectorArg)
{
    struct DuelState *duel;
    struct DuelCard *chosen;
    u8 st;
    /* Reading the step before taking duel gives the ROM's `ldr r0; ...; ldrb; adds r7,r0,#0` copy. */
    st = gDuel.promptStep;
    duel = &gDuel;
    switch (st) {
    case 0:
        if (player != 0) {
            chosen = &gCardListViewCards[AiPickCardListEntry(cardId)];
            break;
        }
        switch (CARD_NUMBER_OF(cardId)) {
        case CARD_SPEAR_CRETIN:
            TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_18x7, TEXTBOX_FLAGS_DEFAULT, gStrPromptGraveMonsterToPlay);
            break;
        case CARD_THE_SHALLOW_GRAVE:
            TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_18x7, TEXTBOX_FLAGS_DEFAULT, gStrPromptGraveMonsterToSet);
            break;
        case CARD_1520:
            TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_18x7, TEXTBOX_FLAGS_DEFAULT,
                        gStrPromptGraveMonsterToSpecialSummon);
            break;
        default:
            return 1;
        }
        gDuel.promptStep++;
        return 0;
    case 1:
        CardListView_Open16(player, -1, CARD_NUMBER_OF(cardId), collectorArg);
        gDuel.promptStep++;
        return 0;
    default:
        chosen = (struct DuelCard *)&LIST_CURSOR_CARD;
        if (duel->turnPlayer)
            /* FAKEMATCH: `(...)[0]` keeps the byte store relative to the cards base ([r2,#1])
               instead of folding +0xC+1 into one offset off the cursor ([r2,#13]). */
            ((struct DuelCard *)&LIST_CURSOR_CARD)[0].owner = 1 - ((LIST_CURSOR_CARD << 19) >> 31);
        break;
    }
    duel->promptResult = *(u32 *)chosen;
    PROMPT_RESULT_2(duel) = ((u16 *)chosen)[1];
    return 1;
}

/*
 * Draw callback of the monster type menu. When gTextBox.menuTimer is 0 it loads gSystemFontPal into OBJ
 * palette 15, renders the name of the selected type (gMonsterTypeNames[result], with a shadow pass) into a 12x2
 * tile canvas, converts it to OBJ tiles at tile 0x364 and sets menuTimer = 1. Every frame it draws the
 * label as 3 x 2 sprites of 32x8 pixels (tiles 0x364.., palette 15) at x = (box x + 1) * 8, y = (revealRow - height) * 8.
 */
void TypeMenu_Draw(void)
{
    struct TextBox *box = &gTextBox;
    u32 x = (box->x + 1) << 3;
    u32 y = (box->revealRow - box->height) << 3;
    u8 *timer;
    u32 attr;
    asm("" : "+r"(y)); /* FAKEMATCH: keep combine from folding y<<16 into D<<19 */
    timer = &box->menuTimer;
    if (*timer == 0) {
        MemCopy16((void *)MENU_LABEL_PALETTE, gSystemFontPal, 0x20);
        TextCanvasInit(0xC, 2);
        TextDrawString(1, 1, SIZE_COLOR(10, 15), (const u8 *)gMonsterTypeNames[box->result]);   /* shadow: size 10, colour 15 */
        TextDrawString(0, 0, SIZE_COLOR(10, 2), (const u8 *)gMonsterTypeNames[box->result]);   /* text: size 10, colour 2 */
        TextCanvasToTiles(MENU_LABEL_TILES, 0);
        (*timer)++;
    }
    attr = MENU_LABEL_TILE;
    AddSprite(x | (y << 16), SPRITE_SHAPE_32x8, attr | OBJ_PALETTE_15);
    attr += 4;
    AddSprite((x + 0x20) | (y << 16), SPRITE_SHAPE_32x8, attr | OBJ_PALETTE_15);
    attr += 4;
    AddSprite((y << 16) | (x + 0x40), SPRITE_SHAPE_32x8, attr | OBJ_PALETTE_15);
    attr += 4;
    AddSprite(x | ((y + 8) << 16), SPRITE_SHAPE_32x8, attr | OBJ_PALETTE_15);
    attr += 4;
    AddSprite((x + 0x20) | ((y + 8) << 16), SPRITE_SHAPE_32x8, attr | OBJ_PALETTE_15);
    attr += 4;
    AddSprite((x + 0x40) | ((y + 8) << 16), SPRITE_SHAPE_32x8, attr | OBJ_PALETTE_15);
}

/*
 * Input callback of the monster type menu: returns 1 on A. It returns 0 while menuTimer is 0 (the label is
 * being redrawn) and skips the first frame (menuState 0 -> 1). Left sets result = (result + 19) % 20 and
 * Right result = (result + 1) % 20 (the 20 monster types, result = type - 1); both reset menuTimer to force the
 * redraw.
 */
int TypeMenu_HandleInput(void)
{
    struct TextBox *box = &gTextBox;
    u8 *timer = &box->menuTimer;
    if (*timer == 0)
        return 0;
    if (box->menuState == TEXTBOX_MENU_STATE_SELECT) {
        box->menuState++;
        return 0;
    }
    /* Matching: gMain.newKeys must be read in every test (no local): a local makes the
     * final `& 1` compile with the and's result in the key register. */
    if (gMain.newKeys & DPAD_LEFT) {
        box->result += MONSTER_TYPE_COUNT - 1;
        box->result = box->result % MONSTER_TYPE_COUNT;
        *timer = 0;
        return 0;
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        box->result += 1;
        box->result = box->result % MONSTER_TYPE_COUNT;
        *timer = 0;
        return 0;
    }
    if (gMain.newKeys & A_BUTTON)
        return 1;
    return 0;
}

/*
 * PROMPT_SELECT_TYPE: choose a monster type. Step 0 opens 'Select Type.' with the type menu callbacks; the next
 * call stores the menu's result (type - 1) in promptResult and returns 1.
 */
int DuelPrompt_SelectType(void)
{
    struct DuelState *duel = &gDuel;
    u8 *step = &duel->promptStep;
    if (*step == 0) {
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_15x4, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectType);
        TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, TypeMenu_Draw, (u16 (*)(void))TypeMenu_HandleInput);
        (*step)++;
        return 0;
    }
    duel->promptResult = gTextBox.result;
    return 1;
}

/*
 * Draw callback of the attribute menus: on menuTimer 0 it renders the name of the selected attribute
 * (gAttributeNames[result]) into an 8x4 tile canvas at OBJ tile 0x364 (palette 15) and sets menuTimer = 1. Every
 * frame it draws one 64x32 sprite at x = (box x + 1) * 8, y = (revealRow - height) * 8 - 16.
 */
void AttributeMenu_Draw(void)
{
    struct TextBox *box = &gTextBox;
    u32 x = (box->x + 1) << 3;
    int y = (box->revealRow - box->height) << 3;
    u8 *timer = &box->menuTimer;
    if (*timer == 0) {
        MemCopy16((void *)MENU_LABEL_PALETTE, gSystemFontPal, 0x20);
        TextCanvasInit(8, 4);
        TextDrawString(4, 0x12, SIZE_COLOR(12, 15), (const u8 *)gAttributeNames[box->result]);  /* shadow: size 12, colour 15 */
        TextDrawString(3, 0x11, SIZE_COLOR(12, 1), (const u8 *)gAttributeNames[box->result]);  /* text: size 12, colour 1 */
        TextCanvasToTiles(MENU_LABEL_TILES, 0);
        (*timer)++;
    }
    AddSprite(x | ((y - 0x10) << 16), SPRITE_SHAPE_64x32, MENU_LABEL_TILE | OBJ_PALETTE_15);
}

/*
 * Input callback of the attribute menu: as TypeMenu_HandleInput with the six attributes
 * (result = attribute - 1): Left = (result + 5) % 6, Right = (result + 1) % 6; returns 1 on A.
 */
int AttributeMenu_HandleInput(void)
{
    struct TextBox *box = &gTextBox;
    u8 *timer = &box->menuTimer;
    if (*timer == 0)
        return 0;
    if (box->menuState == TEXTBOX_MENU_STATE_SELECT) {
        box->menuState++;
        return 0;
    }
    if (gMain.newKeys & DPAD_LEFT) {
        box->result += ATTRIBUTE_COUNT - 1;
        box->result = box->result % ATTRIBUTE_COUNT;
        *timer = 0;
        return 0;
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        box->result += 1;
        box->result = box->result % ATTRIBUTE_COUNT;
        *timer = 0;
        return 0;
    }
    if (gMain.newKeys & A_BUTTON)
        return 1;
    return 0;
}

/*
 * Input callback of the second attribute pick of DuelPrompt_SelectTwoAttributes: Left and Right step through the
 * six attributes as in AttributeMenu_HandleInput but skip the value already in promptResult (the first pick);
 * A returns 1 only when result differs from it.
 */
int AttributeMenu_HandleInputExcludeFirst(void)
{
    u16 firstPick;
    if (gTextBox.menuTimer == 0)
        return 0;
    if (gTextBox.menuState == TEXTBOX_MENU_STATE_SELECT) {
        gTextBox.menuState++;
        return 0;
    }
    if (gMain.newKeys & DPAD_LEFT) {
        struct TextBox *box = &gTextBox;
        firstPick = gUnk_0201AE44;
        do {
            box->result += ATTRIBUTE_COUNT - 1;
            box->result = box->result % ATTRIBUTE_COUNT;
        } while (box->result == firstPick);
        gTextBox.menuTimer = 0;
        return 0;
    }
    if (gMain.newKeys & DPAD_RIGHT) {
        struct TextBox *box = &gTextBox;
        firstPick = gUnk_0201AE44;
        do {
            box->result += 1;
            box->result = box->result % ATTRIBUTE_COUNT;
        } while (box->result == firstPick);
        gTextBox.menuTimer = 0;
        return 0;
    }
    if (gMain.newKeys & A_BUTTON) {
        if (gTextBox.result != gDuel.promptResult)
            return 1;
    }
    return 0;
}

/*
 * PROMPT_SELECT_ATTRIBUTE: choose an attribute. Step 0 opens 'Select Attribute.' with the attribute menu
 * callbacks; the next call stores the menu's result (attribute - 1) in promptResult and returns 1.
 */
int DuelPrompt_SelectAttribute(void)
{
    struct DuelState *duel = &gDuel;
    u8 *step = &duel->promptStep;
    if (*step == 0) {
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_15x4, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectAttribute);
        TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, AttributeMenu_Draw, (u16 (*)(void))AttributeMenu_HandleInput);
        (*step)++;
        return 0;
    }
    duel->promptResult = gTextBox.result;
    return 1;
}

/*
 * PROMPT_SELECT_TWO_ATTRIBUTES: choose two different attributes (Earthshaker). Step 0 opens the attribute menu with
 * 'Select Attribute.'; step 1 stores the pick in promptResult and reopens the menu with 'Select another
 * Attribute.' and AttributeMenu_HandleInputExcludeFirst; step 2 stores the second pick in the halfword after promptResult.
 * Returns 1 at the end.
 */
int DuelPrompt_SelectTwoAttributes(void)
{
    struct DuelState *duel = &gDuel;
    u8 *step = &duel->promptStep;
    /* A switch rather than an if/else chain keeps both step blocks out of line. */
    switch (*step) {
    case 0:
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_15x4, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectAttribute);
        TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, AttributeMenu_Draw, (u16 (*)(void))AttributeMenu_HandleInput);
        (*step)++;
        return 0;
    case 1:
        duel->promptResult = gTextBox.result;
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_17x4, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectAnotherAttribute);
        TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, AttributeMenu_Draw,
                       (u16 (*)(void))AttributeMenu_HandleInputExcludeFirst);
        (*step)++;
        return 0;
    }
    PROMPT_RESULT_2(duel) = gTextBox.result;
    return 1;
}

/*
 * CPU stand-in for the two-line choice input (TextBoxHandleChoiceInput); returns 1 when finished. It runs
 * the choice menu's states:
 *   SELECT: the cursor alternates between the two lines every 4 frames up to menuTimer 0x3F; at 0x40 the CPU
 *     picks result = Random() & 1; after menuTimer 0xC0 it goes on to CONFIRMED;
 *   CONFIRMED: holds for 0x3C frames (eight at a time while B is held or gDuelScreen.fast is set), then DONE;
 *   DONE: returns 1.
 */
int TextBoxHandleChoiceInputCpu(void)
{
    struct TextBox *box = &gTextBox;
    u8 *state = &box->menuState;
    int st = *state; /* the (u8) switch index is a separate pseudo, so CSE does not fold st == 1 into the case 1 increment */
    switch ((u8)st) {
    case TEXTBOX_MENU_STATE_SELECT:
        if (box->menuTimer <= CPU_CHOICE_BLINK_END)
            box->result = (box->menuTimer >> 2) & 1;
        if (box->menuTimer == CPU_CHOICE_PICK_FRAME)
            box->result = Random() & 1;
        if (box->menuTimer > CPU_CHOICE_THINK_END) {
            box->menuTimer = 0;
            (*state)++;
        } else
            box->menuTimer++;
        break;
    case TEXTBOX_MENU_STATE_CONFIRMED: {
        u8 timer = box->menuTimer;
        if (timer <= CPU_CHOICE_HOLD_LAST) {
            box->menuTimer = timer + 1;
            if ((gMain.heldKeys & B_BUTTON) || gDuelScreen.fast) {
                if (box->menuTimer <= CPU_CHOICE_HOLD_SKIP)
                    box->menuTimer = timer + 8;
            }
        } else
            (*state)++;
        break;
    }
    case TEXTBOX_MENU_STATE_DONE:
        return 1;
    }
    return 0;
}

/*
 * PROMPT_PICK_ONE_OF_TWO_ATTRIBUTES: choose one of the two attributes in promptArgs[0..1] (Earthshaker's
 * second player). Step 0 builds 'Select Attribute.' with one indented line per attribute name in a 0x80-byte
 * buffer and opens it. The CPU's prompt (promptPlayer) gets the choice cursor with
 * TextBoxHandleChoiceInputCpu; the human gets the standard two-line menu. The next call stores
 * promptArgs[result] in promptResult and returns 1.
 */
int DuelPrompt_PickOneOfTwoAttributes(void)
{
    char buf[0x80];
    if (gDuel.promptStep == 0) {
        const char *const *names;
        u16 *attributes;
        int i;
        StrCopy(buf, (const char *)gStrPromptSelectAttribute);
        StrCat(buf, (const char *)gStrNewline);
        names = gAttributeNames;
        attributes = gDuel.promptArgs;
        for (i = 1; i >= 0; i--) {
            StrCat(buf, (const char *)gStrMenuIndent);
            StrCat(buf, names[*attributes]);
            StrCat(buf, (const char *)gStrNewline);
            attributes++;
        }
        TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_15x5, TEXTBOX_FLAGS_DEFAULT, (const u8 *)buf);
        if (gDuel.promptPlayer) {
            TextBoxSetMenu(TEXTBOX_MENU_CUSTOM, TextBoxDrawChoiceCursor, (u16 (*)(void))TextBoxHandleChoiceInputCpu);
        } else {
            TextBoxSetMenu(TEXTBOX_MENU_TWO_CHOICE, NULL, NULL);
            gDuel.promptStep++;
        }
        gDuel.promptStep++;
        return 0;
    }
    gDuel.promptResult = gDuel.promptArgs[gTextBox.result];
    return 1;
}

/*
 * PROMPT_PICK_OPPONENT_HAND_CARD: pick a card in the opponent's hand (Confiscation, The Forceful Sentry).
 * `player` is the answering player. The CPU takes AiPickOpponentHandCard's slot in player 0's hand, stores it in
 * promptResult, points at it and returns 1. The human: step 0 shows the text; step 1 reveals player 1's hand
 * (handRevealed) and waits for DuelCursor_PickTarget(PICK_PLAYER1(PICK_HAND)); on A it plays SE_CONFIRM and
 * points at the card; step 2 hides the hand again, stores the slot in promptResult and returns 1.
 */
int DuelPrompt_PickOpponentHandCard(int player)
{
    if (player != 0) {
        int slot = AiPickOpponentHandCard();
        gDuel.promptResult = slot;
        DuelCmd_Push16(DUEL_CMD_POINT_AT_CARD | DUEL_CMD_PLAYER, 0, (u8)slot << 8 | DUEL_AREA_HAND, 0);
        return 1;
    } else {
        struct DuelState *duel = &gDuel;
        u8 *step = &duel->promptStep;
        switch (*step) {
        case 0:
            TextBoxOpen(PROMPT_BOX_POS, PROMPT_BOX_18x7, TEXTBOX_FLAGS_DEFAULT, gStrPromptSelectOpponentHandCard);
            (*step)++;
            return 0;
        case 1:
            {
                /* Matching: a pointer to player 1 keeps handRevealed a +9 displacement off gDuel + 0xD68. */
                struct DuelPlayer *opponent = &duel->players[1];
                opponent->handRevealed = 1;
            }
            if (DuelCursor_PickTarget(PICK_PLAYER1(PICK_HAND))) {
                PlaySE(SE_CONFIRM);
                DuelCmd_Push(DUEL_CMD_POINT_AT_CARD, (u16)gDuelScreen.selPlayer,
                               (u8)gDuelScreen.selArea | (((u8)gDuelScreen.selIndex) << 8), 0);
                (*step)++;
            }
            return 0;
        default:
            {
                struct DuelPlayer *opponent = &duel->players[1];
                opponent->handRevealed = 0;
            }
            duel->promptResult = gDuelScreen.selIndex;
            return 1;
        }
    }
}

/* Zone `zone` of `player` as base + zone * 0x94 + player * 0xD64 (the order of the ROM's address arithmetic). */
#define ZONE_PTR(player, zone) \
    ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones + (zone) * sizeof(struct DuelZone) + (player) * sizeof(struct DuelPlayer)))

/*
 * Is (player, area + index) a valid pick for the effect cursor's mask (16 bits per player, player 1 shifted by
 * 16; enum FieldPickMask)? The area decides:
 *   hand (11): PICK_HAND and index < handCount;
 *   monster row (0): a card, one face bit (face-up 0x20 / face-down 0x10) and one position bit (defense 0x80 /
 *     attack 0x40) must both match;
 *   spell/trap row (5): a card, then PICK_FACE_DOWN_SPELL_TRAP if it is face down; a face-up Magic needs
 *     PICK_FACE_UP_MAGIC and a face-up Trap PICK_FACE_UP_TRAP; any other face-up card there counts only if a
 *     face-up monster of either side links to it with link kind 1, 5 or 6 (an equip or an absorbed card;
 *     link = DUEL_LOC(player, index + 5));
 *   Field Magic zone (10): a card, PICK_FACE_UP_MAGIC if face up, else PICK_FACE_DOWN_SPELL_TRAP.
 * Any other area (the piles) is not valid. Returns 1 or 0.
 */
int DuelCursor_IsValidTarget(int player, int area, int index, u32 mask)
{
    struct DuelZone *zone = &gDuelPlayers[player & 1].zones[area + index];
    /* The signed shift keeps the id extraction apart from the later `id != 0`
     * tests, so only zone->card << 20 is shared (CSE), as in the ROM. */
    int type = CARD_STATS_TYPE(CARD_STATS_OF(((s32)(CARD_WORD(zone->card) << 20) >> 20) & CARD_ID_MASK));
    int p, i, k;

    switch (area) {
    case DUEL_AREA_HAND:
        if (!((PICK_HAND << (player << 4)) & mask)) return 0;
        if (index < gDuelPlayers[player & 1].handCount) return 1;
        return 0;
    case DUEL_AREA_MONSTER: {
        int faceMatches, positionMatches;
        if (!((PICK_ANY_MONSTER << (player << 4)) & mask)) return 0;
        faceMatches = 0;
        positionMatches = 0;
        if (CARD_WORD_ID(CARD_WORD(zone->card)) == 0) return 0;
        if (((PICK_FACE_UP_MONSTER << (player << 4)) & mask) && zone->isFaceUp) faceMatches = 1;
        if (((PICK_FACE_DOWN_MONSTER << (player << 4)) & mask) && !zone->isFaceUp) faceMatches = 1;
        if (((PICK_DEFENSE_POSITION << (player << 4)) & mask) && zone->isDefense) positionMatches = 1;
        if (((PICK_ATTACK_POSITION << (player << 4)) & mask) && !zone->isDefense) positionMatches = 1;
        if (positionMatches && faceMatches) return 1;
        return 0;
    }
    case DUEL_AREA_SPELL_TRAP: {
        u32 spellTrapBits = ((PICK_FACE_DOWN_SPELL_TRAP | PICK_FACE_UP_MAGIC | PICK_FACE_UP_TRAP) << (player << 4)) & mask;
        if (!spellTrapBits) return 0;
        if (CARD_WORD_ID(CARD_WORD(zone->card)) == 0) return 0;
        if (!zone->isFaceUp) {
            if ((PICK_FACE_DOWN_SPELL_TRAP << (player << 4)) & mask) return 1;
            return 0;
        }
        if (spellTrapBits == (u32)(PICK_FACE_DOWN_SPELL_TRAP << (player << 4))) return 0;
        switch (type) {
        case CARD_TYPE_MAGIC:
            if ((PICK_FACE_UP_MAGIC << (player << 4)) & mask) return 1;
            return 0;
        case CARD_TYPE_TRAP:
            if ((PICK_FACE_UP_TRAP << (player << 4)) & mask) return 1;
            return 0;
        }
        for (p = 0; p <= 1; p++) {
            for (i = 0; i <= 4; i++) {
                if (CARD_WORD_ID(CARD_WORD(ZONE_PTR(p & 1, i)->card)) && ZONE_PTR(p & 1, i)->isFaceUp) {
                    for (k = 0; k < ZONE_PTR(p & 1, i)->numLinks; k++) {
                        u16 link = ZONE_PTR(p & 1, i)->links[k];
                        switch (ZONE_PTR(p & 1, i)->linkKinds[k]) {
                        case ZONE_LINK_EQUIP:
                        case ZONE_LINK_ABSORBED:
                        case ZONE_LINK_6:
                            if (link == (u16)((u8)player | ((u8)(index + 5) << 8))) return 1;
                            break;
                        }
                    }
                }
            }
        }
        return 0;
    }
    case DUEL_AREA_FIELD: {
        int valid;
        struct DuelZone *fieldZone = ZONE_PTR(player & 1, ZONE_FIELD);
        if (!CARD_WORD_ID(CARD_WORD(fieldZone->card))) return 0;
        valid = 0;
        if (fieldZone->isFaceUp) {
            if ((PICK_FACE_UP_MAGIC << (player << 4)) & mask)
                valid = 1;
        } else if ((PICK_FACE_DOWN_SPELL_TRAP << (player << 4)) & mask)
            valid = 1;
        if (valid) return 1;
        return 0;
    }
    }
    return 0;
}
