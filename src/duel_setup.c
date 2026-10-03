/*
 * The duel command queue runner, duel setup and opening, and the building of the chain
 * (wiki/functions/duel-setup-c.md).
 *
 *  - DuelCmdQueue_Run pops the commands queued with DuelCmd_Push and runs them one at a time; in a link duel
 *    each one is mirrored to the partner, which runs it through DuelCmd_RunRemote and acknowledges it.
 *  - Duel_Setup clears every duel work area; SetupStartFieldCard and DuelPhase_Opening (duel step 1) deal the
 *    opening hands and put an event duel's Field Magic into play.
 *  - Chain_Add* queue activations (gChain.pending) and responses (gChain.links). Chain_Build runs each link's
 *    cost and target handlers, shows the chain and asks both players whether they respond
 *    (Chain_AskResponse); Chain_Resolve (duel_main.c) then resolves it.
 */
#include "global.h"
#include "constants/cards.h"        /* CARD_* card numbers */
#include "constants/card_stats.h"   /* CARD_TYPE_*, SPELL_*, CARD_STATS_TYPE_* / CARD_STATS_SUBTYPE_* */
#include "constants/duel.h"         /* enum DuelStep, DuelResult, DuelArea, ChainEntryKind, DuelField */
#include "constants/duel_cmds.h"    /* DUEL_CMD_* */
#include "constants/sound.h"        /* SE_* */
#include "card_data.h"              /* CARD_ID_MASK, CARD_NUMBER_ALT_ART, CARD_STATS_TYPE / _SUBTYPE */
#include "util.h"                   /* MemCopy16, MemClear16, FormatStr */

/* ---- BEGIN pre-H0 subset of gba.h, duel.h, main.h and sound.h ---- */
/*
 * include/gba.h, duel.h, main.h and sound.h still hold the legacy headers until the header switch (H0,
 * build/readability/HEADERS.md); chain.h, duel_cmd.h, duel_screen.h, duel_link.h, battle.h, summon.h and
 * card_list_view.h include duel.h. Until then this block repeats the part of the new headers that the unit
 * uses, with the same tags, field names, types and bitfield containers (structs are cut after the last field
 * used here and padded to their size). It defines GUARD_DUEL_H so that the headers below skip the legacy
 * file. After H0, replace the block (BEGIN to END) with the #include lines of these headers, in this order:
 *     gba.h duel.h main.h sound.h
 * (checked: that gives the same assembly with the new headers). The unit has no include line that the H0 sed
 * rewrites.
 */
#define GUARD_DUEL_H

#define B_BUTTON        0x0002                          /* gba.h */
#define R_BUTTON        0x0100
#define L_BUTTON        0x0200

struct DuelCard {
    u32 id:12;
    u32 owner:1;
    u32 unk13:19;
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
    u16 serial;
    u8 isDefense:1;                 /* +0x06 */
    u8 isFaceUp:1;
    u8 turnCounter:4;
    u8 unk6_6:2;
    u8 unk7[0x94 - 7];
};

struct DuelPlayer {
    u8 unk0[0xD64];
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
    u16 serial;
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004 */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 */
    u8 turnPlayer:1;
    u8 phase:3;
    u8 linkError:1;
    u8 result:2;
    u8 unk1B13[0x1B20 - 0x1B13];
    u8 phaseStep;                   /* +0x1B20 */
    u8 unk1B21[0x1B28 - 0x1B21];
    u16 cardMenuCard;               /* +0x1B28 */
    u16 summonTributes;
    struct CardMenu cardMenu;       /* +0x1B2C */
    u8 unk1B38[8];
    u8 cmdQueueStep;                /* +0x1B40 */
    u8 unk1B41[0x1B78 - 0x1B41];
};

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelState gDuel;
extern struct DuelZonesPlayer gDuelZones[2];

void AddCardNumberToDeckTop(int player, u16 number);
int CountActiveCardsOnField(int player, u16 cardNo);

struct Main {                                   /* main.h */
    u32 rngState;
    u16 heldKeys;                   /* +0x0004 */
    u16 newKeys;                    /* +0x0006 */
    u8 unk8[0x485E - 0x8];
    u16 frameCounter;               /* +0x485E */
    u8 unk4860[0x488A - 0x4860];
    u16 startField:4;               /* +0x488A */
    u16 subStep:8;
    u16 unk488A_12:4;
};
extern struct Main gMain;

void PlaySE(u32 seId);                          /* sound.h */
void FadeOutBGM(void);
/* ---- END pre-H0 subset ---- */

#include "chain.h"                  /* gChain, struct ChainEntry, the Chain_* functions defined here */
#include "duel_cmd.h"               /* gDuelCmd, DuelCmd_Push, DuelCmd_Dispatch */
#include "duel_flow.h"              /* gDuelCtrl, PlayDuelBGM, Duel_Setup, DuelPhase_Opening */
#include "duel_link.h"              /* gLinkState, enum LinkMsgId, DuelLink_SendMessage* */
#include "duel_screen.h"            /* gDuelScreen, ChainListScreen_*, DuelCursor_PickTarget */
#include "duel_scenes.h"            /* gDuelScene */
#include "text_box.h"               /* gTextBox, TextBoxOpen, TextBoxSetMenu */
#include "card_list_view.h"         /* gCardListView, CardListView_Open */
#include "card_menu.h"              /* CardMenu_Update, CardMenu_PlaySpellTrapFromHand */
#include "summon.h"                 /* gSummonAction */
#include "battle.h"                 /* gBattle */
#include "effect.h"                 /* gCardEffects, FindCardEffect, CanPlayerChain, CanChain*Card */
#include "duel_actions.h"           /* sub_080197C0 */
#include "ai.h"                     /* gAiState, AiTryChainResponse */

/* ---- Local views kept for matching ---- */

/* gCardStats and gCardIdToNumber through their constant addresses (the ROM loads the mask before the table). */
#define CARD_STATS_C(id)    (((const u32 *)0x08621DE0)[(id) & CARD_ID_MASK])
#define CARD_TYPE_C(id)     CARD_STATS_TYPE(CARD_STATS_C(id))
#define CARD_NUMBER_C(id)   (((const u16 *)0x08622AB4)[(id) & CARD_ID_MASK])
/* gCardNumberToId through its constant address. */
#define CARD_ID_TABLE       ((const u16 *)0x08623DF4)
/* gCardNames through its constant address, one 0x40-byte name per card ID (the table base is reloaded per
 * use). */
#define CARD_NAME_C(id)     (((const char (*)[CARD_NAME_SIZE])0x0822C720)[id])

/* Card number to card ID; 0xFFFF maps to 0, an alternate-art number (2000 + n) to n's ID + 1 (this copy does
 * not mask numbers below 2000). */
static inline u16 CardNumberToId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number < CARD_NUMBER_ALT_ART)
        return *(CARD_ID_TABLE + number);
    return *(CARD_ID_TABLE + ((number - CARD_NUMBER_ALT_ART) & CARD_ID_MASK)) + 1;
}

/* These are tested as u32 here (the definitions return u16); the narrower return type changes the tests. */
u32 CanChainFieldCard32(struct ChainEntry *chainLink, int player, int zone) asm("CanChainFieldCard");
u32 CanChainHandCard32(struct ChainEntry *chainLink, int player, int handIdx) asm("CanChainHandCard");

/* gLinkState.queryReplyReceived (+0x307 bit 3) in a u32 container (duel_link.h: u8): the ROM tests it as the
 * sign of the word shifted left by 28, and stores it with a word access. */
struct LinkStateQueryView {
    u8 unk0[0x304];
    u32 unk304_0:27;
    u32 queryReplyReceived:1;       /* +0x307 bit 3 */
    u32 unk307_4:4;
};
#define gLinkQueryReplyReceived (((struct LinkStateQueryView *)&gLinkState)->queryReplyReceived)

/* gDuel as Chain_AskResponse reads it: the card menu with its index in a u16 container (duel.h: u32). Same
 * bits as struct CardMenu. */
struct CardMenuIndexU16 {
    u16 open:1;
    u16 confirmed:1;
    u16 command:4;
    u16 slide:4;
    u32 available:16;
    u32 state:8;
    u32 step:8;
    u32 unk42:8;                    /* summonSeq, tributeSources */
    u16 timer:7;
    u16 player:1;                   /* bit 57 */
    u32 area:7;                     /* bits 58-64 */
    u16 index:8;                    /* bits 65-72 */
    u16 unk73:7;
};
struct DuelStateAskView {
    u8 unk0[0x1B12];
    u8 bgmOn:1;                     /* +0x1B12 */
    u8 turnPlayer:1;
    u8 unk1B12_2:6;
    u8 unk1B13[0x1B28 - 0x1B13];
    u16 cardMenuCard;               /* +0x1B28 */
    u16 summonTributes;
    struct CardMenuIndexU16 cardMenu; /* +0x1B2C */
};

/* Chain_AskResponse reaches gDuel, gChain and gTextBox through second symbols for the same addresses
 * (address-named aliases that the linker script resolves). Each one is a literal-pool entry of its own, as in
 * the ROM. */
extern struct DuelStateAskView gAliasFEA0_020192E0;     /* = gDuel */
extern struct ChainState gAliasFEA0_02017A40;           /* = gChain */
extern struct TextBox gAliasFEA0_0201AE60;              /* = gTextBox */
#define gDuelAlias      gAliasFEA0_020192E0
#define gChainAlias     gAliasFEA0_02017A40
#define gTextBoxAlias   gAliasFEA0_0201AE60

/* &gDuelZones[player].zones[zone] through the gDuel alias (gDuelZones = gDuel + 0x2C), summed in the ROM's
 * order. */
#define ALIAS_ZONE(player, zone) ((struct DuelZone *)((player) * sizeof(struct DuelPlayer) \
    + (zone) * sizeof(struct DuelZone) + (u32)((u8 *)&gDuelAlias + 0x2C)))

/* "<card>'s effect is activated. Resolve it as part of a Chain?" and "<card> is activated. ..." (FormatStr
 * templates for a monster effect and for a Magic/Trap card). */
extern const char gStrChainPromptEffect[];
extern const char gStrChainPromptCard[];

/* Run the duel command queue: pop the next command into gDuelCmd and run it until it finishes. A link duel
 * sends each command to the partner (LINKMSG_COMMAND) and waits for its LINKMSG_COMMAND_DONE; 120 frames
 * without a word from the partner (LINKMSG_COMMAND_RUNNING resets the timer) is a link error. After each
 * command the duel BGM is refreshed (not after the result banner, whose jingle must play). Returns 1 while
 * busy. */
u32 DuelCmdQueue_Run(void)
{
    int i;

    switch (gDuel.cmdQueueStep) {
    case 0:
        if (gDuelCmd.queueCount == 0)
            return 0;
        MemCopy16(&gDuelCmd, &gDuelCmd.queue[0], sizeof(struct DuelCmdEntry));
        gDuelCmd.queueCount--;
        for (i = 0; i < gDuelCmd.queueCount; i++)
            MemCopy16(&gDuelCmd.queue[i], &gDuelCmd.queue[i + 1], sizeof(struct DuelCmdEntry));
        gLinkState.remoteCmdDone = 1;
        gDuelCmd.running = 1;
        if (gDuelCtrl.isLinkDuel) {
            DuelLink_SendMessageData(LINKMSG_COMMAND, &gDuelCmd, sizeof(struct DuelCmdEntry));
            gLinkState.remoteCmdDone = 0;
            gLinkState.unk200 = 0;
            gLinkState.cmdAckTimer = 0;
        }
        gDuelCmd.step = 0;
        gDuelCmd.timer = 0;
        gDuel.cmdQueueStep++;
        /* fall through */
    case 1:
        DuelCmd_Dispatch();
        if (gDuelCmd.running)
            return 1;
        gDuel.cmdQueueStep++;
        /* fall through */
    case 2:
        if (!gLinkState.remoteCmdDone) {
            if (++gLinkState.cmdAckTimer < 120)
                return 1;
            gLinkState.cmdAckTimer = 0;
            DuelLink_SendMessage(LINKMSG_ABORT, 0, 0, 0);
            FadeOutBGM();
            gDuel.linkError = 1;
            return 0;
        }
        gLinkState.cmdAckTimer = 0;
        if ((gDuelCmd.cmd & DUEL_CMD_ID_MASK) != DUEL_CMD_SHOW_DUEL_RESULT)
            PlayDuelBGM();
        gDuel.cmdQueueStep = 0;
        if (gDuelCmd.queueCount)
            return 1;
        break;
    }
    return 0;
}

/* Link duel: run the duel command the partner sent (gLinkState.remoteCmd, already in our point of view).
 * While it runs, LINKMSG_COMMAND_RUNNING goes out every 16 frames (it resets the partner's wait timer);
 * LINKMSG_COMMAND_DONE ends it. Returns 1 while busy, 0 when done (remoteCmdPending cleared). */
u32 DuelCmd_RunRemote(void)
{
    switch (gLinkState.remoteCmdStep) {
    case 0:
        MemCopy16(&gDuelCmd, gLinkState.remoteCmd, sizeof(struct DuelCmdEntry));
        gDuelCmd.running = 1;
        gDuelCmd.step = 0;
        gDuelCmd.timer = 0;
        gLinkState.remoteCmdStep++;
        /* fall through */
    case 1:
        DuelCmd_Dispatch();
        if (!gDuelCmd.running)
            gLinkState.remoteCmdStep++;
        else if ((gMain.frameCounter & 0xF) == 0)
            DuelLink_SendMessage(LINKMSG_COMMAND_RUNNING, 0, 0, 0);
        return 1;
    case 2:
        if ((gDuelCmd.cmd & DUEL_CMD_ID_MASK) != DUEL_CMD_SHOW_DUEL_RESULT)
            PlayDuelBGM();
        DuelLink_SendMessage(LINKMSG_COMMAND_DONE, 0, 0, 0);
        gLinkState.remoteCmdStep++;
        return 1;
    }
    gLinkState.remoteCmdPending = 0;
    return 0;
}

/* Clear all duel work areas; the result starts as a draw. Holding L or R at the start turns on fast mode
 * (the card animations run as if B were held). Returns 1. */
u32 Duel_Setup(void)
{
    MemClear16(&gDuelCtrl, sizeof(gDuelCtrl));
    MemClear16(&gDuel, sizeof(gDuel));
    MemClear16(&gDuelScreen, sizeof(gDuelScreen));
    MemClear16(&gCardListView, sizeof(gCardListView));
    MemClear16(&gTextBox, sizeof(gTextBox));
    MemClear16(&gDuelCmd, sizeof(gDuelCmd));
    MemClear16(&gSummonAction, sizeof(gSummonAction));
    MemClear16(&gChain, sizeof(gChain));
    MemClear16(&gDuelScene, sizeof(gDuelScene));
    MemClear16(&gLinkState, sizeof(gLinkState));
    MemClear16(&gBattle, sizeof(gBattle));
    gDuel.result = DUEL_RESULT_DRAW;
    FadeOutBGM();
    if (gMain.heldKeys & (L_BUTTON | R_BUTTON))
        gDuelScreen.fast = 1;
    return 1;
}

/* An event duel's start field (gMain.startField, enum DuelField): put that Field Magic on top of the CPU's
 * deck, then queue for player 1: draw it, place it face up in the field zone, set the field background. */
void SetupStartFieldCard(void)
{
    u16 number;

    switch (gMain.startField) {
    case FIELD_FOREST:
        number = CARD_FOREST;
        break;
    case FIELD_WASTELAND:
        number = CARD_WASTELAND;
        break;
    case FIELD_MOUNTAIN:
        number = CARD_MOUNTAIN;
        break;
    case FIELD_SOGEN:
        number = CARD_SOGEN;
        break;
    case FIELD_UMI:
        number = CARD_UMI;
        break;
    case FIELD_YAMI:
        number = CARD_YAMI;
        break;
    case FIELD_CHORUS_OF_SANCTUARY:
        number = CARD_CHORUS_OF_SANCTUARY;
        break;
    case FIELD_GAIA_POWER:
        number = CARD_GAIA_POWER;
        break;
    case FIELD_UMIIRUKA:
        number = CARD_UMIIRUKA;
        break;
    case FIELD_MOLTEN_DESTRUCTION:
        number = CARD_MOLTEN_DESTRUCTION;
        break;
    case FIELD_RISING_AIR_CURRENT:
        number = CARD_RISING_AIR_CURRENT;
        break;
    case FIELD_LUMINOUS_SPARK:
        number = CARD_LUMINOUS_SPARK;
        break;
    case FIELD_MYSTIC_PLASMA_ZONE:
        number = CARD_MYSTIC_PLASMA_ZONE;
        break;
    default:
        return;
    }
    AddCardNumberToDeckTop(1, number);
    DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_DRAW_CARDS, 0, 1, 0);
    /* arg4: zone | hand index << 4 | faceUp << 8 */
    DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_PLACE_SPELL_TRAP_FROM_HAND, CardNumberToId(number),
                 ZONE_FIELD | 0 << 4 | 1 << 8, 0);
    DuelCmd_Push(DUEL_CMD_PLAYER | DUEL_CMD_SET_FIELD_BACKGROUND, gMain.startField, 1, 0);
}

/* Duel step 1. Step 0: reset the duel (8000 LP), open the duel screen, play the start field. Step 1: deal
 * five cards to the turn player, then to the other, and show "Start Duel" (which starts the BGM). Then a
 * duel the opponent starts goes to its turn (DUEL_STEP_OPPONENT_TURN); else returns 1 (on to step 2). */
u32 DuelPhase_Opening(void)
{
    switch (gDuel.phaseStep) {
    case 0:
        DuelCmd_Push(DUEL_CMD_RESET_DUEL_STATE, 0, 0, 0);
        DuelCmd_Push(DUEL_CMD_OPEN_DUEL_SCREEN, 0, 0, 0);
        SetupStartFieldCard();
        gDuel.phaseStep++;
        return 0;
    case 1:
        DuelCmd_Push(gDuel.turnPlayer ? DUEL_CMD_PLAYER | DUEL_CMD_DRAW_CARDS : DUEL_CMD_DRAW_CARDS, 0, 5, 0);
        DuelCmd_Push(!gDuel.turnPlayer ? DUEL_CMD_PLAYER | DUEL_CMD_DRAW_CARDS : DUEL_CMD_DRAW_CARDS, 0, 5, 0);
        DuelCmd_Push(DUEL_CMD_START_DUEL_BANNER, 0, 0, 0);
        gDuel.phaseStep++;
        return 0;
    default:
        if (gDuel.turnPlayer) {
            gAiState.turnPhase = 0;
            gAiState.step = 0;
            gDuelCtrl.phase = DUEL_STEP_OPPONENT_TURN;
            break;
        }
        return 1;
    }
    return 0;
}

/* 1 if the link runs its handlers on the link partner's GBA: a link duel, the partner's entry (player 1),
 * and not Time Machine. */
u32 Chain_IsPartnerEntry(struct ChainEntry *entry)
{
    if (gDuelCtrl.isLinkDuel && entry->player && CARD_NUMBER_C(entry->card) != CARD_TIME_MACHINE)
        return 1;
    return 0;
}

/*
 * Append an entry to gChain.pending (toChain 0) or gChain.links (toChain 1); card ID 0 is ignored.
 *   packed = card | zone << 16 | kind << 21 | event << 25 | player << 31,   locs = loc0 | loc1 << 16.
 * On the partner's turn of a link duel the partner owns the chain: the entry is sent to it instead
 * (LINKMSG_ADD_ACTION), with the player bit flipped to its point of view.
 */
void Chain_Add(u16 toChain, u32 packed, u32 locs)
{
    struct ChainEntry *entry;
    u16 msg[5];

    if (!(packed & 0xFFFF))
        return;
    if (gDuelCtrl.isLinkDuel && gDuel.turnPlayer) {
        if (packed & 0x80000000)
            packed &= ~0x80000000;
        else
            packed |= 0x80000000;
        msg[0] = toChain;
        msg[1] = packed;
        msg[2] = packed >> 16;
        msg[3] = locs;
        msg[4] = locs >> 16;
        DuelLink_SendMessageData(LINKMSG_ADD_ACTION, msg, sizeof(msg));
        return;
    }
    if (toChain)
        entry = &gChain.links[gChain.linkCount];
    else
        entry = &gChain.pending[gChain.pendingCount];
    entry->card = packed;
    entry->player = packed >> 31;
    entry->kind = (packed & 0x1E00000) >> 21;
    entry->zone = (packed & 0x1F0000) >> 16;
    entry->skipChainA = 0;
    entry->skipChainB = 0;
    entry->negated = 0;
    entry->destroyIfNegated = 0;
    entry->flag4_4 = 0;
    entry->event = (packed & 0x7E000000) >> 25;
    entry->loc0 = locs;
    entry->loc1 = locs >> 16;
    if (toChain)
        gChain.linkCount++;
    else
        gChain.pendingCount++;
}

/* Queue an activation or trigger: Chain_Update starts a new chain with it. */
void Chain_AddPending(u32 packed, u32 locs)
{
    Chain_Add(0, packed, locs);
}

/* Add a response link to the chain being built. */
void Chain_AddLink(u32 packed, u32 locs)
{
    Chain_Add(1, packed, locs);
}

/* Append a copy of an entry received from the link partner: it becomes player 1's, its cost and target
 * handlers already ran there (skipChainA/B), and the player byte of loc0 and loc1 is mirrored. */
void Chain_AddPartnerEntry(u16 toChain, struct ChainEntry *src)
{
    struct ChainEntry *entry;
    u16 loc;

    if (toChain)
        entry = &gChain.links[gChain.linkCount];
    else
        entry = &gChain.pending[gChain.pendingCount];
    MemCopy16(entry, src, sizeof(struct ChainEntry));
    entry->player = 1;
    entry->skipChainA = 1;
    entry->skipChainB = 1;
    entry->negated = 0;
    entry->destroyIfNegated = 0;
    entry->flag4_4 = 0;
    loc = src->loc0;
    entry->loc0 = (u8)(1 - loc) | ((loc >> 8) << 8);
    loc = src->loc1;
    entry->loc1 = (u8)(1 - loc) | ((loc >> 8) << 8);
    if (toChain)
        gChain.linkCount++;
    else
        gChain.pendingCount++;
}

/*
 * Does the activated card leave the field after resolving? 0 for monsters and for off-field entries that
 * are not Traps. Cards that stay on the field (Field, Equip and Continuous cards, Cocoon of Evolution,
 * Swords of Revealing Light, keys 1230, 1258 and 1545) go only when destroyIfNegated is set. Everything else
 * (Normal, Counter, Quick-Play and Ritual cards) goes: 1.
 */
u32 Chain_CardGoesToGrave(struct ChainEntry *entry)
{
    u32 ret = 1;
    u32 stats, type;
    int subtype;

    if (entry->kind == CHAIN_KIND_OFF_FIELD && CARD_STATS_TYPE(CARD_STATS_C(entry->card)) != CARD_TYPE_TRAP)
        return 0;
    stats = CARD_STATS_C(entry->card);
    type = CARD_STATS_TYPE(stats);
    if (type <= CARD_TYPE_REPTILE)
        return 0;
    switch ((int)type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        subtype = CARD_STATS_SUBTYPE(stats);
        break;
    default:
        subtype = 0;
        break;
    }
    switch (subtype) {
    case SPELL_FIELD:
    case SPELL_EQUIP:
    case SPELL_CONTINUOUS:
        goto staysOnField;
    }
    /* FAKEMATCH: an empty compiler barrier (no instructions) so that the card ID is loaded again after the
     * subtype test, as the ROM does. */
    asm volatile ("" : : : "memory");
    switch (CARD_NUMBER_C(entry->card)) {
    case CARD_COCOON_OF_EVOLUTION:
    case CARD_SWORDS_OF_REVEALING_LIGHT:
    case CARD_1230:
    case CARD_1258:
    case CARD_1545:
    staysOnField:
        ret = entry->destroyIfNegated;
        break;
    }
    return ret;
}

/*
 * The zone as a plain card word, for Chain_GetResponseCommands: struct DuelZone.card is a struct DuelCard,
 * whose 12-bit id agbcc reads with a halfword load; the ROM loads the whole word and masks it.
 */
struct DuelZoneWordView {
    u32 card;                       /* +0x00: struct DuelCard word, ID in bits 0-11 */
    u8 unk4[2];
    u8 isDefense:1;                 /* +0x06 */
    u8 isFaceUp:1;
    u8 unk6_2:6;
    u8 unk7[0x94 - 7];
};

/* Card-menu commands for answering `link` with the card at (player, area, index): 0x41 (Card View and
 * Activate) if that card can respond, else 1 (Card View only). A face-up key-1525 monster can answer a Magic
 * card, unless key 1418 is active on either field or the link is key 1539. */
u32 Chain_GetResponseCommands(struct ChainEntry *link, int player, int area, int index)
{
    u32 ret = CARDMENU_MASK_CARD_VIEW;
    struct DuelZoneWordView *zone;
    u32 id;

    switch (area) {
    case DUEL_AREA_HAND:
        if (CanChainHandCard32(link, player, index))
            ret = CARDMENU_MASK_CARD_VIEW | CARDMENU_MASK_ACTIVATE;
        break;
    case DUEL_AREA_SPELL_TRAP:
        if (CanChainFieldCard32(link, player, index + ZONE_SPELL_0))
            ret = CARDMENU_MASK_CARD_VIEW | CARDMENU_MASK_ACTIVATE;
        break;
    case DUEL_AREA_MONSTER:
        player &= 1; zone = (struct DuelZoneWordView *)((u8 *)gDuelZones + (index * 0x94 + player * 0xD64));
        id = zone->card << 20 >> 20;
        if (id != 0 && zone->isFaceUp && CARD_NUMBER_C(id) == CARD_1525
            && CARD_TYPE_C(link->card) == CARD_TYPE_MAGIC
            && !CountActiveCardsOnField(0, CARD_1418) && !CountActiveCardsOnField(1, CARD_1418)
            && CARD_NUMBER_C(link->card) != CARD_1539)
            ret = CARDMENU_MASK_CARD_VIEW | CARDMENU_MASK_ACTIVATE;
        break;
    }
    return ret;
}

/* Unreferenced: wait for the partner's reply to a response request (gLinkState.queryReplyReceived), then 60
 * frames; the steps are gTextBox.menuState and menuTimer. Returns 1 when done. */
u32 sub_0801FE54(void)
{
    struct TextBox *box = &gTextBox;
    u8 *stepPtr;
    u32 step;
    u32 stepCopy;

    stepPtr = &box->menuState;
    step = *stepPtr;
    stepCopy = step;
    /* FAKEMATCH: keeps the loaded step apart from the switch copy (an empty constraint, no instructions), so
     * the case values are not folded into it. */
    asm volatile ("" : "+r"(step));
    switch (stepCopy) {
    case 0:
        if (!gLinkQueryReplyReceived)
            break;
        goto next;
    case 1:
        if (box->menuTimer <= 59) {
            box->menuTimer++;
            break;
        }
        goto next;
    default:
        return 1;
    next:
        *stepPtr = step + 1;
        break;
    }
    return 0;
}

/* Chain_AskResponse steps (gChain.askStep; the same byte as EventResponse_Run's step). */
enum AskResponseStep {
    ASK_OPEN_SCREEN = 0,            /* once per chain: reopen the duel screen */
    ASK_PROMPT = 1,                 /* the CPU or the partner answers elsewhere; the human gets a Yes/No box */
    ASK_WAIT_ANSWER = 2,            /* No: settled */
    ASK_PICK_CARD = 10,             /* cursor and card menu (Chain_GetResponseCommands) */
    ASK_ACTIVATE = 11,              /* add the chosen card to the chain */
    ASK_LINK_QUERY = 100,           /* send the link to the partner (LINKMSG_CHAIN_QUERY) */
    ASK_LINK_WAIT = 101,            /* wait for its reply */
    ASK_CPU = 200                   /* the AI decides */
};

/*
 * Ask `player` (0 the human, 1 the CPU or the link partner) whether to respond to `link`; one step per call
 * (enum AskResponseStep). The human gets the prompt "<card> is activated. Resolve it as part of a Chain?"
 * with Yes/No; Yes lets them pick a card with the cursor and the card menu, and an activated card is added to
 * the chain (gChain.responseAdded). The link partner is asked over the link, the CPU through
 * AiTryChainResponse. Returns 1 when the question is settled.
 */
s32 Chain_AskResponse(struct ChainEntry *link, u32 player)
{
    u8 text[0x200];
    u16 card;
    int step = gChainAlias.askStep;

    /* The switch on the u8 copy keeps the loaded step in its own register, so the step++ of ASK_PICK_CARD
     * stays unfolded. */
    switch ((u8)step) {
    case ASK_OPEN_SCREEN:
        if (!gChainAlias.askScreenOpened) {
            gChainAlias.askScreenOpened = 1;
            DuelCmd_Push(DUEL_CMD_OPEN_DUEL_SCREEN, 0, 0, 0);
            gChainAlias.askStep++;
            return 0;
        }
        gChainAlias.askStep++;
        /* fall through */
    case ASK_PROMPT:
        if (player) {
            u8 nextStep;
            if (gDuelCtrl.isLinkDuel)
                nextStep = ASK_LINK_QUERY;
            else
                nextStep = ASK_CPU;
            gChainAlias.askStep = nextStep;
            return 0;
        }
        card = link->card;
        if (CARD_TYPE_C(card) <= CARD_TYPE_REPTILE)
            FormatStr((char *)text, gStrChainPromptEffect, CARD_NAME_C(card));
        else
            FormatStr((char *)text, gStrChainPromptCard, CARD_NAME_C(card));
        TextBoxOpen(0x206, 0x712, TEXTBOX_FLAGS_DEFAULT, text);     /* at cell (6, 2), 18 x 7 cells */
        TextBoxSetMenu(TEXTBOX_MENU_YES_NO, 0, 0);
        gChainAlias.askStep++;
        return 0;
    case ASK_WAIT_ANSWER:
        if (gTextBoxAlias.result == 0)
            return 1;
        gChainAlias.askStep = ASK_PICK_CARD;
        gDuelAlias.cardMenu.open = 0;
        gDuelAlias.cardMenu.confirmed = 0;
        return 0;
    case ASK_PICK_CARD:
        if (gDuelAlias.cardMenu.open) {
            CardMenu_Update();
            return 0;
        }
        if (gDuelAlias.cardMenu.confirmed) {
            gChainAlias.askStep++;
            return 0;
        }
        if (gMain.newKeys & B_BUTTON) {
            gChainAlias.askStep = ASK_PROMPT;
            return 0;
        }
        /* The cursor may pick the player's hand and Magic/Trap cards on their own turn; on the opponent's
         * turn the Magic/Trap cards and the face-up monsters. */
        if (DuelCursor_PickTarget(!gDuelAlias.turnPlayer
                ? PICK_HAND | PICK_FACE_DOWN_SPELL_TRAP | PICK_FACE_UP_MAGIC | PICK_FACE_UP_TRAP
                : PICK_FACE_DOWN_SPELL_TRAP | PICK_FACE_UP_MAGIC | PICK_FACE_UP_TRAP | PICK_FACE_UP_MONSTER
                      | PICK_ATTACK_POSITION | PICK_DEFENSE_POSITION) == 0)
            return 0;
        {
            u32 cursorPlayer = gDuelScreen.selPlayer;
            u32 area = gDuelScreen.selArea;
            u32 index = gDuelScreen.selIndex;
            u16 id = DuelCursor_GetCardId();
            switch (area) {
            case DUEL_AREA_MONSTER:
            case DUEL_AREA_SPELL_TRAP:
            case DUEL_AREA_FIELD:
            case DUEL_AREA_HAND:
                if (id != 0) {
                    gDuelAlias.cardMenu.open = 1;
                    gDuelAlias.cardMenu.state = 0;
                    gDuelAlias.cardMenu.available =
                        (u16)Chain_GetResponseCommands(link, cursorPlayer, area, index);
                    return 0;
                }
                PlaySE(SE_ERROR);
                return 0;
            case DUEL_AREA_DECK:
                PlaySE(SE_ERROR);
                return 0;
            case DUEL_AREA_FUSION_DECK:
            case DUEL_AREA_GRAVEYARD:
            case DUEL_AREA_BANISHED:
                CardListView_Open(cursorPlayer, area, 0, 0);
                PlaySE(SE_CONFIRM);
                return 0;
            }
        }
        return 0;
    case ASK_ACTIVATE:
        switch (gDuelAlias.cardMenu.area) {
        case DUEL_AREA_HAND:
            CardMenu_PlaySpellTrapFromHand(1, 1, link);
            if (gDuelAlias.cardMenu.confirmed)
                return 0;
            break;
        case DUEL_AREA_MONSTER:
        case DUEL_AREA_SPELL_TRAP:
            /* A card on the field: flip it face up if it is set, then add it as a link. */
            gDuelAlias.cardMenu.confirmed = 0;
            {
                u32 menuPlayer = 1 & gDuelAlias.cardMenu.player;
                if (!ALIAS_ZONE(menuPlayer, gDuelAlias.cardMenu.index + gDuelAlias.cardMenu.area)->isFaceUp) {
                    u16 cmd;
                    if (gDuelAlias.cardMenu.player)
                        cmd = DUEL_CMD_PLAYER | DUEL_CMD_FLIP_CARD;
                    else
                        cmd = DUEL_CMD_FLIP_CARD;
                    DuelCmd_Push(cmd, gDuelAlias.cardMenu.index + gDuelAlias.cardMenu.area, 0, 0);
                }
            }
            {
                /* packed = card | zone << 16 | kind << 21 | event << 25 | player << 31 (CHAIN_KIND_SPELL_TRAP);
                 * the response keeps the event and locations of the link it answers. */
                u32 playerAndEvent = ((1 & gDuelAlias.cardMenu.player) << 31) | (link->event << 25);
                u32 zoneAndKind = (((gDuelAlias.cardMenu.index + gDuelAlias.cardMenu.area) & 0x1F) << 16)
                                  | CHAIN_KIND_SPELL_TRAP << 21;
                Chain_AddLink(playerAndEvent | zoneAndKind | gDuelAlias.cardMenuCard,
                              (link->loc1 << 16) | link->loc0);
            }
            break;
        }
        gChainAlias.requestPending = 0;
        gChainAlias.responseAdded = 1;
        return 1;
    case ASK_LINK_QUERY:
        DuelLink_SendMessageData(LINKMSG_CHAIN_QUERY, link, sizeof(struct ChainEntry));
        gChainAlias.responseAdded = 0;
        gLinkQueryReplyReceived = 0;
        gChainAlias.askStep++;
        return 0;
    case ASK_LINK_WAIT:
        return gLinkQueryReplyReceived;
    case ASK_CPU:
        if (AiTryChainResponse(link))
            gChainAlias.responseAdded = 1;
        break;
    }
    return 1;
}

/* gChain.links followed by the build flags, through the symbol gUnk_02017CC0 (= gChain + 0x280), which the
 * ROM uses here for the list passed to ChainListScreen_Start and for one buildStep++ (+0x150 = gChain +0x3D0). */
struct ChainLinksView {
    struct ChainEntry links[16];
    u16 linkCount;                  /* +0x140 */
    u8 unk142[0x150 - 0x142];
    u8 building:1;                  /* +0x150 */
    u8 buildStep:7;
};
extern struct ChainLinksView gUnk_02017CC0;

/* gChain +0x3D2 as one byte: Chain_Build sets resolving = 1 and resolveStep = 0 with a single store. */
struct ChainResolveByteView {
    u8 unk0[0x3D2];
    u8 resolveFlags;                /* +0x3D2 */
};
#define gChainResolveByte (((struct ChainResolveByteView *)&gChain)->resolveFlags)

/* Chain_Build steps (gChain.buildStep). */
enum ChainBuildStep {
    BUILD_START = 0,
    BUILD_SETUP_LINK = 1,           /* look up the link's cost (chainA) and target (chainB) handlers */
    BUILD_START_CHAIN_A = 2,        /* a partner's link: let the partner run it */
    BUILD_RUN_CHAIN_A = 3,          /* until it returns nonzero (remoteChainADone) */
    BUILD_START_CHAIN_B = 4,
    BUILD_RUN_CHAIN_B = 5,          /* until it returns nonzero (remoteChainBDone) */
    BUILD_NEXT_LINK = 6,
    BUILD_SEND_CHAIN = 7,           /* link duel: send the chain to the partner */
    BUILD_SHOW_LIST = 8,            /* 'Chain : Activating' list */
    BUILD_WAIT_LIST = 9,
    BUILD_WAIT_PARTNER = 10,        /* link duel: until the partner has shown it too */
    BUILD_CHECK_OPPONENT = 11,      /* can the other player of the last link respond? */
    BUILD_ASK_OPPONENT = 12,
    BUILD_CHECK_OWNER = 13,         /* can the last link's player respond to it? */
    BUILD_ASK_OWNER = 14,
    BUILD_DONE = 15                 /* start resolving */
};

/*
 * Build the chain, one step per call while gChain.building is set (enum ChainBuildStep). From buildIndex on,
 * each link gets its gCardEffects cost and target handlers (skipped by skipChainA/B), shows its card, and runs
 * them until each returns nonzero; a partner's link runs them on the partner's GBA. Then the chain is shown,
 * and the opponent of the last link's player, then that player, may respond (Chain_AskResponse). A response
 * is appended to links and the build restarts at it (gChain.responseAdded); when nobody responds the chain
 * starts resolving. Every case ends in its own `return 1`, so each keeps its own copy of the step++ tail, as
 * in the ROM. Always returns 1.
 */
/* The type of gChain.chainA / chainB (struct CardEffect declares the handlers int-returning). */
typedef u16 (*ChainStepFunc)(struct ChainEntry *link, struct ChainEntry *chainedTo);

#define BUILD_LINK  (gChain.links[gChain.buildIndex])     /* the link being set up */
#define LAST_LINK   (gChain.links[gChain.linkCount - 1])    /* the last link: the one the players may answer */
int Chain_Build(void)
{
    u32 r;  /* one temporary for the effect row and for the link count, as in the ROM (r4) */

    switch (gChain.buildStep) {
    case BUILD_START:
        gChain.buildIndex = 0;
        gChain.buildStep++;
        /* fall through */
    case BUILD_SETUP_LINK:
        r = FindCardEffect(BUILD_LINK.card);
        if (r == -1) {
            gChain.chainA = NULL;
            gChain.chainB = NULL;
        } else {
            gChain.chainA = (ChainStepFunc)gCardEffects[r].chainA;
            gChain.chainB = (ChainStepFunc)gCardEffects[r].chainB;
        }
        if (BUILD_LINK.skipChainA)
            gChain.chainA = NULL;
        if (BUILD_LINK.skipChainB)
            gChain.chainB = NULL;
        sub_080197C0(BUILD_LINK.player, BUILD_LINK.card);     /* show the card (DUEL_CMD_SHOW_CARD_ZOOM_IN) */
        gLinkState.remoteChainBDone = 0;
        gLinkState.remoteChainADone = 0;
        gChain.costStep = 0;
        gChain.targetStep = 0;
        gChain.buildStep++;
        return 1;
    case BUILD_START_CHAIN_A:
        if (gChain.chainA == NULL) {
            gChain.buildStep += 2;
            return 1;
        }
        if ((u16)Chain_IsPartnerEntry(&BUILD_LINK))
            DuelLink_SendMessageData(LINKMSG_REMOTE_CHAIN_A, &BUILD_LINK, sizeof(struct ChainEntry));
        gChain.buildStep++;
        /* fall through */
    case BUILD_RUN_CHAIN_A:
        if ((u16)Chain_IsPartnerEntry(&BUILD_LINK) == 0) {
            {
                u16 *linkCount = &gChain.linkCount;
                /* FAKEMATCH: keeps r0/r1 busy while the count address is live, so local-alloc puts it in r2
                 * as the ROM does. Emits no instructions. */
                asm volatile("" ::: "r0", "r1");
                r = *linkCount;
            }
            /* chainedTo: the link before the last one (the one the last link answers), if any */
            if (r > 1) {
                if (gChain.chainA(&BUILD_LINK, &gChain.links[r - 2]))
                    gLinkState.remoteChainADone = 1;
            } else {
                if (gChain.chainA(&BUILD_LINK, NULL))
                    gLinkState.remoteChainADone = 1;
            }
        }
        if (gLinkState.remoteChainADone)
            gChain.buildStep++;
        return 1;
    case BUILD_START_CHAIN_B:
        if (gChain.chainB == NULL) {
            gChain.buildStep += 2;
            return 1;
        }
        if ((u16)Chain_IsPartnerEntry(&BUILD_LINK))
            DuelLink_SendMessageData(LINKMSG_REMOTE_CHAIN_B, &BUILD_LINK, sizeof(struct ChainEntry));
        gChain.buildStep++;
        /* fall through */
    case BUILD_RUN_CHAIN_B:
        if ((u16)Chain_IsPartnerEntry(&BUILD_LINK) == 0) {
            {
                u16 *linkCount = &gChain.linkCount;
                /* FAKEMATCH: as in BUILD_RUN_CHAIN_A. */
                asm volatile("" ::: "r0", "r1");
                r = *linkCount;
            }
            if (r > 1) {
                if (gChain.chainB(&BUILD_LINK, &gChain.links[r - 2]))
                    gLinkState.remoteChainBDone = 1;
            } else {
                if (gChain.chainB(&BUILD_LINK, NULL))
                    gLinkState.remoteChainBDone = 1;
            }
        }
        if (gLinkState.remoteChainBDone)
            gChain.buildStep++;
        return 1;
    case BUILD_NEXT_LINK:
        gChain.buildIndex++;
        if (gChain.buildIndex < gChain.linkCount) {
            gChain.buildStep = BUILD_SETUP_LINK;
            return 1;
        }
        gChain.buildStep++;
        /* fall through */
    case BUILD_SEND_CHAIN:
        if (gDuelCtrl.isLinkDuel) {
            int i;
            u16 packet[0x80];
            DuelLink_SendMessage(LINKMSG_CHAIN_LIST_COUNT, gChain.linkCount, 0, 0);
            for (i = 0; i < gChain.linkCount; i++) {
                packet[0] = i;
                MemCopy16(packet + 1, &gChain.links[i], sizeof(struct ChainEntry));
                DuelLink_SendMessageData(LINKMSG_CHAIN_LIST_ENTRY, packet, 2 + sizeof(struct ChainEntry));
            }
            DuelLink_SendMessage(LINKMSG_SHOW_CHAIN_LIST_P0, gChain.linkCount, 0, 0);
            gLinkState.chainListShown = 0;
        }
        gChain.buildStep++;
        return 1;
    case BUILD_SHOW_LIST:
        ChainListScreen_Start((u32)&gUnk_02017CC0, 0);
        gUnk_02017CC0.buildStep++;
        return 1;
    case BUILD_WAIT_LIST:
        if (ChainListScreen_Run())
            gChain.buildStep++;
        return 1;
    case BUILD_WAIT_PARTNER:
        if (gDuelCtrl.isLinkDuel && !gLinkState.chainListShown)
            return 1;
        gChain.askScreenOpened = 0;
        gChain.buildStep++;
        /* fall through */
    case BUILD_CHECK_OPPONENT:
        if (CanPlayerChain(&LAST_LINK, 1 - LAST_LINK.player)) {
            gChain.askStep = 0;
            gChain.aiZone = 0;
            gChain.responseAdded = 0;
        } else {
            gChain.buildStep++;     /* skip the question */
        }
        gChain.buildStep++;
        return 1;
    case BUILD_ASK_OPPONENT:
        if ((u16)Chain_AskResponse(&LAST_LINK, 1 - LAST_LINK.player)) {
            if (gChain.responseAdded)
                gChain.buildStep = BUILD_SETUP_LINK;
            else
                gChain.buildStep++;
        }
        return 1;
    case BUILD_CHECK_OWNER:
        if (CanPlayerChain(&LAST_LINK, LAST_LINK.player)) {
            gChain.askStep = 0;
            gChain.responseAdded = 0;
        } else {
            gChain.buildStep++;
        }
        gChain.buildStep++;
        return 1;
    case BUILD_ASK_OWNER:
        if ((u16)Chain_AskResponse(&LAST_LINK, LAST_LINK.player)) {
            if (gChain.responseAdded)
                gChain.buildStep = BUILD_SETUP_LINK;
            else
                gChain.buildStep++;
        }
        return 1;
    default:
        gChain.building = 0;
        /* resolving = 1 and resolveStep = 0 in one byte store. FAKEMATCH: the store goes through a u8 pointer
         * to the byte; as a plain member store (or as two bitfield stores) the zero constant of the next store
         * is hoisted above it. */
        *(u8 *)&gChainResolveByte = 1;
        gChain.unk3D3 = 0;
        return 1;
    }
}
#undef BUILD_LINK
#undef LAST_LINK
