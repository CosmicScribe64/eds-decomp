#include "global.h"
#include "legacy/gba.h"                    /* REG_SIOCNT */
#include "util.h"                   /* MemCopy16 */
#include "link.h"                   /* gLinkBuf.timedOut, LinkRecvMessage */
#include "duel_prompt.h"            /* DuelPrompt_PostData */
#include "constants/duel.h"         /* enum DuelArea, enum ResponseEventKind, enum ZoneLinkKind */
#include "constants/duel_cmds.h"    /* enum DuelCmdId, DUEL_CMD_PLAYER, DUEL_CMD_ID_MASK */

/*
 * The receiving half of the duel link protocol (wiki/functions/duel-link-receive-c.md; the messages are
 * listed in include/duel_link.h).
 *
 * Both GBAs of a Link Battle run the same duel. DuelLink_PollMessage runs once per frame (CB_LinkBattle,
 * while gDuelCtrl.isLinkDuel is set): it reads at most one message into gLinkState and dispatches on its id.
 * It stores the partner's card lists and acknowledges them, answers list requests, sets the flags that the
 * duel step machines wait on, re-posts the partner's prompts and chain queries, and aborts the duel on
 * LINKMSG_ABORT, an unknown id or a receive timeout. Everything from the partner is in the partner's point
 * of view (player 0 is the sender), so player bits and card owner bits are flipped on receipt;
 * DuelLink_MirrorCommand does this for a received duel command.
 */

/* ---- BEGIN pre-H0 subset of duel.h and sound.h ---- */
/*
 * The part of those headers this unit uses, with their names, types and bitfield containers (unused bytes
 * are padding). include/duel.h and include/sound.h still hold the legacy headers until the header switch
 * (H0, build/readability/HEADERS.md), and chain.h, duel_link.h, duel_screen.h and summon.h include duel.h,
 * so this block also defines duel.h's include guard. After H0, replace this block (BEGIN to END) with
 *     #include "legacy/duel.h"
 *     #include "legacy/sound.h"
 * Checked: the unit compiles to the same assembly both ways.
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
    u8 unk4[0x94 - 4];              /* not used here; chain.h embeds a whole zone */
};

struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002: entries in hand[] */
    u8 deckCount;                   /* +0x003: entries in deck[] */
    u8 graveCount;                  /* +0x004: entries in graveyard[] */
    u8 fusionCount;                 /* +0x005: entries in fusionDeck[] */
    u8 banishedCount;               /* +0x006: entries in banished[] */
    u8 unk7[0x684 - 0x7];
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard banished[80];   /* +0xB84 */
    u16 banishedInfo[80];           /* +0xCC4 */
};

struct DuelState {
    u16 serial;                     /* +0x0000 */
    u16 unk2;
    struct DuelPlayer players[2];   /* +0x0004: = gDuelPlayers */
    u8 unk1ACC[0x1B12 - 0x1ACC];
    u8 bgmOn:1;                     /* +0x1B12 bit 0 */
    u8 turnPlayer:1;                /* +0x1B12 bit 1 */
    u8 phase:3;                     /* +0x1B12 bits 2-4 */
    u8 linkError:1;                 /* +0x1B12 bit 5: the link failed */
    u8 result:2;                    /* +0x1B12 bits 6-7: enum DuelResult */
    u8 unk1B13;
    u16 unk1B14_0:1;
    u16 interruptActive:1;          /* +0x1B14 bit 1: the non-turn link player is acting */
    u16 unk1B14_2:7;
    u8 unk1B16[0x1B50 - 0x1B16];
    u8 promptLinked:1;              /* +0x1B50 bit 0: the prompt is mirrored over the link */
    u8 promptActive:1;
    u8 promptPlayer:1;
    u8 unk1B50_3:1;
    u16 promptKind:6;
    u16 unk1B51_2:6;
    u16 promptArgs[8];              /* +0x1B52 */
    u8 promptStep;                  /* +0x1B62 */
    u8 unk1B63;
    u16 promptResult;               /* +0x1B64: the answer; the partner's 16-byte result is copied here */
    u8 unk1B66[0x1B78 - 0x1B66];
};

extern struct DuelState gDuel;                  /* 0x020192E0 */
extern struct DuelPlayer gDuelPlayers[2];       /* 0x020192E4 = gDuel.players */

void CopyDuelCard(u32 *dst, u32 *src);
void FadeOutBGM(void);
/* ---- END pre-H0 subset ---- */

#include "chain.h"                  /* gChain, Chain_Add, Chain_AddPartnerEntry, EventResponse_Request */
#include "duel_link.h"              /* gLinkState, gLinkRxCards, LINKMSG_*, DuelLink_* */
#include "duel_screen.h"            /* gDuelScreen.fast, DuelCursor_Select, DrawAllAreaTiles, ChainListScreen_Start */
#include "summon.h"                 /* gSummonAction, SummonAction_StartFromLink */

/* (zone << 8) | player with the player byte flipped: a location in the other GBA's point of view. */
#define FLIP_LOC_PLAYER(loc) ((u8)(1 - (loc)) | (((loc) >> 8) << 8))

/*
 * Local views kept for matching. They reach parts of gChain and gLinkState with another bitfield container
 * or another address form than the headers', which changes the code agbcc emits:
 * - struct ChainEntryU16: a chain entry whose player bit has a u16 container (chain.h: u8). With the u8
 *   container agbcc compiles `1 - player` to `player ^ 1`; the ROM subtracts and masks.
 * - QUERY_ENTRY_LOCS: gChain.queryEntry.loc0 / loc1 addressed from the gChain base (+0x4C2), not from
 *   &gChain.queryEntry (+0x4BC) as the rest of the entry is.
 * - REMOTE_PLAYERS_U32 (LINKMSG_REMOTE_CHAIN_B), struct RxChainPlayerView (LINKMSG_CHAIN_LIST_ENTRY):
 *   player bits of gLinkState.remoteEntries and rxChainList through u32 containers. The canonical u8 form
 *   changes the register allocation of the whole function (remoteEntries) or gives `player ^ 1`
 *   (rxChainList, which must also be addressed as gLinkState + index * 0x14 + 0x30E).
 */
struct ChainEntryU16 {
    u16 card;                       /* +0x00 */
    u16 player:1;                   /* +0x02 bit 0 */
    u16 kind:3;                     /* +0x02 bits 1-3 */
    u16 zone:6;                     /* +0x02 bits 4-9 */
    u16 event:6;                    /* +0x02 bits 10-15: enum ResponseEventKind */
    u8 flags4;                      /* +0x04 */
    u8 unk5;
    u16 loc0;                       /* +0x06 */
    u16 loc1;                       /* +0x08 */
    u8 unkA[10];
};
struct ChainEntryPairU16 {
    struct ChainEntryU16 entries[2];
};
#define QUERY_ENTRY             (*(struct ChainEntryU16 *)&gChain.queryEntry)
#define QUERY_LINK              (*(struct ChainEntryU16 *)&gChain.queryLink)
#define REMOTE_ENTRIES_U16      ((*(struct ChainEntryPairU16 *)gLinkState.remoteEntries).entries)

struct ChainQueryLocsView {
    u8 unk0[0x4C2];
    u16 loc0;                       /* +0x4C2: gChain.queryEntry.loc0 */
    u16 loc1;                       /* +0x4C4: gChain.queryEntry.loc1 */
};
#define QUERY_ENTRY_LOCS        (*(struct ChainQueryLocsView *)&gChain)

struct RemoteEntryPlayersView {
    u8 unk0[0x45E];
    u32 player0:1;                  /* +0x45E bit 0: gLinkState.remoteEntries[0].player */
    u32 unk45E_1:7;
    u8 unk45F[0x472 - 0x45F];
    u32 player1:1;                  /* +0x472 bit 0: gLinkState.remoteEntries[1].player */
    u32 unk472_1:7;
};
#define REMOTE_PLAYERS_U32      (*(struct RemoteEntryPlayersView *)&gLinkState)

/* Applied at gLinkState + index * 0x14, so that player is gLinkState.rxChainList[index].player. */
struct RxChainPlayerView {
    u8 unk0[0x30E];
    u32 player:1;                   /* +0x30E bit 0 */
    u32 unk30E_1:7;
};

/*
 * Convert the received duel command (LINKMSG_COMMAND: {cmd, arg2, arg4, arg6} in gLinkState.rxArgs) to
 * this side's point of view and store it in gLinkState.remoteCmd for DuelCmd_RunRemote: toggle the acting
 * player (DUEL_CMD_PLAYER) and flip each player byte, location and card owner bit in the arguments. For
 * DUEL_CMD_SHOW_DUEL_RESULT, YOU WIN and YOU LOSE trade places and remoteDuelEnded is set.
 */
void DuelLink_MirrorCommand(void)
{
    u16 cmd = gLinkState.rxArgs[0];
    u16 arg2 = gLinkState.rxArgs[1];
    u16 arg4 = gLinkState.rxArgs[2];
    u16 arg6 = gLinkState.rxArgs[3];
    struct DuelCard card1, card2;

    if (cmd & DUEL_CMD_PLAYER)
        cmd &= (u16)~DUEL_CMD_PLAYER;
    else
        cmd |= DUEL_CMD_PLAYER;
    switch (cmd & DUEL_CMD_ID_MASK) {
    case DUEL_CMD_POINT_AT_CARD:                    /* arg2: player */
        arg2 = 1 - arg2;
        break;
    case DUEL_CMD_MOVE_TO_ZONE:                     /* arg2, arg4: locations */
    case DUEL_CMD_ADD_EQUIP_LINK:
    case DUEL_CMD_SWAP_ZONES:
        arg2 = FLIP_LOC_PLAYER(arg2);
        arg4 = FLIP_LOC_PLAYER(arg4);
        break;
    case DUEL_CMD_ADD_ZONE_LINK:                    /* arg4: zone location; arg2: target, a location only */
    case DUEL_CMD_REMOVE_ZONE_LINK:                 /* for the link kinds below (else a value or card) */
        arg4 = FLIP_LOC_PLAYER(arg4);
        switch ((u8)arg6) {
        case ZONE_LINK_EQUIP:
        case ZONE_LINK_CONTINUOUS:
        case ZONE_LINK_ABSORBED:
            arg2 = FLIP_LOC_PLAYER(arg2);
        }
        break;
    case DUEL_CMD_START_BATTLE_SCENE:               /* player 0's and player 1's card: swap */
        arg2 = gLinkState.rxArgs[2];
        arg4 = gLinkState.rxArgs[1];
        break;
    case DUEL_CMD_PLAY_BATTLE_SCENE:                /* the two sides' values: swap; arg6: swap its bytes */
        arg2 = gLinkState.rxArgs[2];
        arg4 = gLinkState.rxArgs[1];
        arg6 = (gLinkState.rxArgs[3] >> 8) | ((u8)gLinkState.rxArgs[3] << 8);
        break;
    case DUEL_CMD_ADD_DECK_CARD_TO_HAND:            /* card word in arg2 | arg4 << 16: flip its owner */
    case DUEL_CMD_REMOVE_CARD_FROM_DECK:
    case DUEL_CMD_SUMMON_FROM_DECK:
    case DUEL_CMD_SEND_DECK_CARD_TO_GRAVEYARD:
    case DUEL_CMD_BANISH_DECK_CARD:
    case DUEL_CMD_ADD_CARD_TO_DECK_TOP:
    case DUEL_CMD_ADD_CARD_TO_GRAVEYARD:
    case DUEL_CMD_ADD_CARD_TO_BANISHED:
    case DUEL_CMD_REMOVE_CARD_FROM_HAND:
    case DUEL_CMD_ADD_CARD_TO_HAND:
    case DUEL_CMD_RETURN_GRAVEYARD_CARD_TO_HAND:
    case DUEL_CMD_REMOVE_CARD_FROM_GRAVEYARD:
    case DUEL_CMD_BANISH_GRAVEYARD_CARD:
    case DUEL_CMD_ADD_CARD_TO_GRAVEYARD_NO_REDRAW:
    case DUEL_CMD_MARK_GRAVEYARD_CARD:
    case DUEL_CMD_REMOVE_CARD_FROM_FUSION_DECK:
    case DUEL_CMD_SEND_FUSION_DECK_CARD_TO_GRAVEYARD:
    case DUEL_CMD_RETURN_BANISHED_CARD_TO_GRAVEYARD:
        /* FAKEMATCH: a pointer to each stack card word (and two separate words, card1 and card2) give the
         * ROM's 8-byte frame and address scheduling. */
        *(u32 *)&card1 = arg2 | (arg4 << 16);
        { struct DuelCard *card = &card1; card->owner = 1 - card->owner; }
        arg2 = *(u32 *)&card1;
        arg4 = *(u32 *)&card1 >> 16;
        break;
    case DUEL_CMD_PLACE_CARD:                       /* card word in arg4 | arg6 << 16: flip its owner */
    case DUEL_CMD_SET_MAGICAL_HATS_CARD:
        *(u32 *)&card2 = arg4 | (arg6 << 16);
        { struct DuelCard *card = &card2; card->owner = 1 - card->owner; }
        arg4 = *(u32 *)&card2;
        arg6 = *(u32 *)&card2 >> 16;
        break;
    case DUEL_CMD_SET_RETURN_AFTER_BATTLE:          /* arg2: location */
        arg2 = FLIP_LOC_PLAYER(arg2);
        break;
    case DUEL_CMD_SHOW_DUEL_RESULT:                 /* arg2: 0 YOU WIN, 1 YOU LOSE, 2 DRAW */
        if (gLinkState.rxArgs[1] <= 1)
            arg2 = 1 - gLinkState.rxArgs[1];
        gLinkState.remoteDuelEnded = 1;
        break;
    }
    gLinkState.remoteCmd[0] = cmd;
    gLinkState.remoteCmd[1] = arg2;
    gLinkState.remoteCmd[2] = arg4;
    gLinkState.remoteCmd[3] = arg6;
}

/*
 * Receive and handle at most one link message; returns 1 if a known message was handled, 0 if none arrived
 * or the link failed. A receive timeout or an unknown id sends LINKMSG_ABORT; those and a received
 * LINKMSG_ABORT fade out the BGM and set gDuel.linkError. Card lists arrive in the sender's point of view:
 * the list belongs to player 1 - (header >> 8), and every card's owner bit is flipped. Original quirks:
 * LINKMSG_BANISHED_LIST does not update banishedCount, and the fusion deck and banished lists both set
 * fusionReceived.
 */
u16 DuelLink_PollMessage(void)
{
    int received;
    int player, count, i;
    (void)REG_SIOCNT;               /* read and not used */
    gLinkState.rxId = LINKMSG_NOP;
    received = LinkRecvMessage((u8 *)&gLinkState);
    if (gLinkBuf.timedOut) {
        /* FAKEMATCH: the abort tail of the default case written out again; a goto to it changes the
         * register allocation. */
        DuelLink_SendMessage(LINKMSG_ABORT, 0, 0, 0);
        FadeOutBGM();
        gDuel.linkError = 1;
        return 0;
    }
    if (!received)
        return 0;
    switch (gLinkState.rxId) {
    case LINKMSG_READY: gLinkState.readyReceived = 1; return 1;
    case LINKMSG_UNK_EE01: gLinkState.unk304_0 = 1; return 1;
    case LINKMSG_UNK_EE02: gLinkState.unk304_1 = 1; return 1;
    case LINKMSG_FAST_MODE: gDuelScreen.fast = 1; return 1;
    case LINKMSG_NOP: return 1;
    case LINKMSG_TURN_END: gLinkState.turnEndReceived = 1; return 1;
    case LINKMSG_DUEL_RESULT:
        gLinkState.duelResultReceived = 1;
        gDuel.result = (u8)gLinkState.rxArgs[0];
        return 1;
    case LINKMSG_INTERRUPT_REQUEST: gLinkState.interruptRequested = 1; return 1;
    /* The non-turn player starts or ends acting during the turn; both put the cursor on player 0's
     * spell/trap row. */
    case LINKMSG_INTERRUPT_BEGIN:
        gDuel.interruptActive = 1;
        goto moveCursor;
    case LINKMSG_INTERRUPT_END:
        gDuel.interruptActive = 0;
        gLinkState.interruptRequested = 0;
        LinkWaitEnd_Nop();
    moveCursor:
        DuelCursor_Select(0, DUEL_AREA_SPELL_TRAP, 0);
        return 1;
    case LINKMSG_QUEUED_ACTION:
        MemCopy16(&gSummonAction, gLinkState.rxArgs, 0x14);
        SummonAction_StartFromLink();
        return 1;
    case LINKMSG_QUEUED_ACTION_DONE: gLinkState.queuedActionDone = 1; return 1;
    case LINKMSG_CARD_PROMPT:
        gLinkState.cardPromptPending = 1;
        gLinkState.cardPromptStep = 0;
        gLinkState.cardPromptCard = gLinkState.rxArgs[0];
        gLinkState.cardPromptArg1 = gLinkState.rxArgs[1];
        gLinkState.cardPromptArg2 = gLinkState.rxArgs[2];
        return 1;
    case LINKMSG_CARD_PROMPT_ANSWER:
        gLinkState.cardPromptAnswered = 1;
        gLinkState.cardPromptAnswer = gLinkState.rxArgs[0];
        return 1;
    case LINKMSG_ACTIVATE_QUERY:
        /* The partner's game event: does this player respond? Mirror it into gChain.queryEntry. */
        MemCopy16(&QUERY_ENTRY, gLinkState.rxArgs, 0x14);
        QUERY_ENTRY.player = 1 - QUERY_ENTRY.player;
        switch (QUERY_ENTRY.event) {
        case RESPONSE_SUMMONED:                     /* loc0 and loc1 are locations */
        case RESPONSE_FLIP_SUMMONED:
        case RESPONSE_SPECIAL_SUMMONED:
        case RESPONSE_SET:
        case RESPONSE_ATTACK_DECLARED:
        case RESPONSE_DAMAGE_STEP:
        case RESPONSE_MAGIC_TO_GRAVE:
        case RESPONSE_TRAP_TO_GRAVE:
        case RESPONSE_EQUIP:
        case RESPONSE_DREW:
        case RESPONSE_MONSTER_TO_HAND:
        case RESPONSE_DISCARDED:
        case RESPONSE_MONSTER_TO_GRAVE:
            QUERY_ENTRY_LOCS.loc0 = FLIP_LOC_PLAYER(QUERY_ENTRY_LOCS.loc0);
            QUERY_ENTRY_LOCS.loc1 = FLIP_LOC_PLAYER(QUERY_ENTRY_LOCS.loc1);
            break;
        case RESPONSE_LP_CHANGE:                    /* loc1: player */
            QUERY_ENTRY_LOCS.loc1 = 1 - QUERY_ENTRY_LOCS.loc1;
            break;
        case RESPONSE_BATTLE_DAMAGE:                /* loc1: 'you'/'opponent' selector, one per byte */
        case RESPONSE_BATTLE_DEFLECTED_DAMAGE:
            QUERY_ENTRY_LOCS.loc1 = (QUERY_ENTRY_LOCS.loc1 >> 8) | ((u8)QUERY_ENTRY_LOCS.loc1 << 8);
            break;
        case RESPONSE_BATTLE_DESTROYED:
            /* The ROM reloads both and stores them back unchanged (LINKMSG_TRIGGER_EVENT swaps them for
             * this event). A plain self-assignment would be optimised away. */
            {
                u16 loc0 = QUERY_ENTRY_LOCS.loc0;
                u16 loc1 = QUERY_ENTRY_LOCS.loc1;
                QUERY_ENTRY_LOCS.loc0 = loc0;
                QUERY_ENTRY_LOCS.loc1 = loc1;
            }
            break;
        }
        gChain.queryStep = LINKQUERY_ASK;
        gChain.queryIsChainLink = 0;
        gLinkState.activateQueryPending = 1;
        return 1;
    case LINKMSG_QUERY_REPLY: gLinkState.queryReplyReceived = 1; return 1;
    case LINKMSG_TRIGGER_EVENT:
        {
            u16 eventPlayer = gLinkState.rxArgs[0];
            u16 event = gLinkState.rxArgs[1], loc0 = gLinkState.rxArgs[2], loc1 = gLinkState.rxArgs[3];
            switch (event) {
            case RESPONSE_SUMMONED:
            case RESPONSE_FLIP_SUMMONED:
            case RESPONSE_SPECIAL_SUMMONED:
            case RESPONSE_SET:
            case RESPONSE_ATTACK_DECLARED:
            case RESPONSE_DAMAGE_STEP:
            case RESPONSE_EQUIP:
            case RESPONSE_DREW:
            case RESPONSE_MONSTER_TO_HAND:
            case RESPONSE_DISCARDED:
            case RESPONSE_MONSTER_TO_GRAVE:
                loc0 = FLIP_LOC_PLAYER(loc0);
                loc1 = FLIP_LOC_PLAYER(loc1);
                break;
            case RESPONSE_LP_CHANGE: loc1 = 1 - loc1; break;
            case RESPONSE_BATTLE_DAMAGE:
            case RESPONSE_BATTLE_DEFLECTED_DAMAGE: loc1 = (loc1 >> 8) | ((u8)loc1 << 8); break;
            case RESPONSE_BATTLE_DESTROYED: loc0 = gLinkState.rxArgs[3]; loc1 = gLinkState.rxArgs[2]; break;
            }
            EventResponse_Request(eventPlayer, event, loc0 | (loc1 << 16));
        }
        return 1;
    case LINKMSG_CHAIN_QUERY:
        /* The partner's chain link: does this player chain to it? Mirror it via gChain.queryLink. */
        MemCopy16(&QUERY_LINK, gLinkState.rxArgs, 0x14);
        QUERY_LINK.player = 1 - QUERY_LINK.player;
        QUERY_LINK.loc0 = FLIP_LOC_PLAYER(QUERY_LINK.loc0);
        QUERY_LINK.loc1 = FLIP_LOC_PLAYER(QUERY_LINK.loc1);
        MemCopy16(&QUERY_ENTRY, &QUERY_LINK, 0x14);
        gChain.queryStep = LINKQUERY_ASK;
        gChain.queryIsChainLink = 1;
        gLinkState.activateQueryPending = 1;
        return 1;
    case LINKMSG_QUERY_DECLINED: gLinkState.queryReplyReceived = 1; return 1;
    case LINKMSG_ACTIVATE_CARD:
        Chain_AddPartnerEntry(0, (struct ChainEntry *)gLinkState.rxArgs);
        gChain.responseAdded = 1;
        gLinkState.queryReplyReceived = 1;
        return 1;
    case LINKMSG_CHAIN_CARD:
        Chain_AddPartnerEntry(1, (struct ChainEntry *)gLinkState.rxArgs);
        gChain.responseAdded = 1;
        gLinkState.queryReplyReceived = 1;
        return 1;
    case LINKMSG_ADD_ACTION_A:
        /* {packed word}. The ROM passes the word as `locs` too; no sender of this message was found. */
        {
            u32 packed = gLinkState.rxArgs[0] | (gLinkState.rxArgs[1] << 16);
            Chain_AddPending(packed, packed);
        }
        return 1;
    case LINKMSG_PROMPT:
        /* {kind, args[8]}: the partner's prompt for its player 1, answered here by player 0. */
        DuelPrompt_PostData(0, gLinkState.rxArgs[0], &gLinkState.rxArgs[1], 8);
        gDuel.promptLinked = 1;
        return 1;
    case LINKMSG_PROMPT_RESULT:
        MemCopy16(&gDuel.promptResult, gLinkState.rxArgs, 0x10);
        gLinkState.promptResultReceived = 1;
        gLinkState.interruptRequested = 0;
        return 1;
    case LINKMSG_REMOTE_CHAIN_A:
        /* Run a chainA (cost) handler for the partner: entries[0] is the link to run (ours: player 0),
         * entries[1] the link it responds to (mirrored). LINKMSG_REMOTE_CHAIN_B does the same for chainB. */
        MemCopy16(gLinkState.remoteEntries, gLinkState.rxArgs, 0x28);
        REMOTE_ENTRIES_U16[0].player = 0;
        REMOTE_ENTRIES_U16[1].player = 1 - REMOTE_ENTRIES_U16[1].player;
        gLinkState.remoteChainAStep = 0;
        gLinkState.remoteChainAPending = 1;
        return 1;
    case LINKMSG_REMOTE_CHAIN_A_DONE:
        gLinkState.remoteChainADone = 1;
        gLinkState.interruptRequested = 0;
        MemCopy16(&gChain.links[gChain.buildIndex], gLinkState.rxArgs, 0x14);
        return 1;
    case LINKMSG_REMOTE_CHAIN_B:
        MemCopy16(gLinkState.remoteEntries, gLinkState.rxArgs, 0x28);
        REMOTE_PLAYERS_U32.player0 = 0;
        REMOTE_PLAYERS_U32.player1 = 1 - REMOTE_PLAYERS_U32.player1;
        gLinkState.remoteChainBStep = 0;
        gLinkState.remoteChainBPending = 1;
        return 1;
    case LINKMSG_REMOTE_CHAIN_B_DONE:
        gLinkState.remoteChainBDone = 1;
        gLinkState.interruptRequested = 0;
        MemCopy16(&gChain.links[gChain.buildIndex], gLinkState.rxArgs, 0x14);
        return 1;
    case LINKMSG_REMOTE_RESOLVE:
        /* {selector, mode, entries[2]}: remoteResolveMode = selector << 1 | (mode & 1). */
        MemCopy16(gLinkState.remoteEntries, &gLinkState.rxArgs[2], 0x28);
        {
            /* FAKEMATCH: the rx and dest temporaries order the loads and the store as in the ROM. */
            struct LinkState *rx = &gLinkState;
            u8 *dest = &gLinkState.remoteResolveMode;
            u32 low = (u8)rx->rxArgs[1] & 1;
            *dest = ((u8)rx->rxArgs[0] << 1) | low;
        }
        gLinkState.remoteResolveStep = 0;
        gLinkState.remoteResolvePending = 1;
        return 1;
    case LINKMSG_ADD_ACTION:
        Chain_Add(gLinkState.rxArgs[0], gLinkState.rxArgs[1] | (gLinkState.rxArgs[2] << 16),
                  gLinkState.rxArgs[3] | (gLinkState.rxArgs[4] << 16));
        return 1;
    case LINKMSG_REMOTE_RESOLVE_DONE: gLinkState.remoteResolveDone = 1; return 1;
    case LINKMSG_CHAIN_LIST_COUNT: gLinkState.rxChainListCount = gLinkState.rxArgs[0]; return 1;
    case LINKMSG_CHAIN_LIST_ENTRY:
        /* {index, chain entry}: store it with its player flipped. */
        MemCopy16(&gLinkState.rxChainList[gLinkState.rxArgs[0]], &gLinkState.rxArgs[1], 0x14);
        {
            struct RxChainPlayerView *entry = (struct RxChainPlayerView *)((u8 *)&gLinkState + gLinkState.rxArgs[0] * 0x14);
            entry->player = 1 - entry->player;
        }
        return 1;
    case LINKMSG_SHOW_CHAIN_LIST_P0:
        ChainListScreen_Start((u32)gLinkState.rxChainList, 0);
        gLinkState.showChainListPending = 1;
        return 1;
    case LINKMSG_SHOW_CHAIN_LIST_P1:
        ChainListScreen_Start((u32)gLinkState.rxChainList, 1);
        gLinkState.showChainListPending = 1;
        return 1;
    case LINKMSG_CHAIN_LIST_SHOWN: gLinkState.chainListShown = 1; return 1;
    /* The partner asks for one of our lists. */
    case LINKMSG_REQUEST_HAND: DuelLink_SendHand(0); return 1;
    case LINKMSG_REQUEST_DECK: DuelLink_SendDeck(0); return 1;
    case LINKMSG_REQUEST_GRAVEYARD: DuelLink_SendGraveyard(0); return 1;
    case LINKMSG_REQUEST_FUSION_DECK: DuelLink_SendFusionDeck(0); return 1;
    case LINKMSG_REQUEST_BANISHED: DuelLink_SendBanished(0); return 1;
    /*
     * One of the partner's lists: {count | player << 8, card words}. Store it for player 1 - player with
     * every owner bit flipped, then acknowledge it. (The order of the src and card declarations matters for
     * the deck loop's registers; the other loops follow it.)
     */
    case LINKMSG_HAND_LIST:
        player = gLinkState.rxArgs[0] >> 8; count = (u8)gLinkState.rxArgs[0];
        player = 1 - player;
        for (i = 0; i < count; i++) {
            const struct DuelCard *src = &gLinkRxCards[i];
            struct DuelCard *card = &gDuelPlayers[player & 1].hand[i];
            CopyDuelCard((u32 *)card, (u32 *)src);
            card->owner = 1 - card->owner;
        }
        gDuelPlayers[player & 1].handCount = count;
        DuelLink_SendMessage(LINKMSG_HAND_ACK, 0, 0, 0);
        gLinkState.handReceived = 1;
        return 1;
    case LINKMSG_DECK_LIST:
        player = gLinkState.rxArgs[0] >> 8; count = (u8)gLinkState.rxArgs[0];
        player = 1 - player;
        for (i = 0; i < count; i++) {
            const struct DuelCard *src = &gLinkRxCards[i];
            struct DuelCard *card = &gDuelPlayers[player & 1].deck[i];
            CopyDuelCard((u32 *)card, (u32 *)src);
            card->owner = 1 - card->owner;
        }
        gDuelPlayers[player & 1].deckCount = count;
        DuelLink_SendMessage(LINKMSG_DECK_ACK, 0, 0, 0);
        gLinkState.deckReceived = 1;
        return 1;
    case LINKMSG_GRAVEYARD_LIST:
        player = gLinkState.rxArgs[0] >> 8; count = (u8)gLinkState.rxArgs[0];
        player = 1 - player;
        for (i = 0; i < count; i++) {
            const struct DuelCard *src = &gLinkRxCards[i];
            struct DuelCard *card = &gDuelPlayers[player & 1].graveyard[i];
            CopyDuelCard((u32 *)card, (u32 *)src);
            card->owner = 1 - card->owner;
        }
        gDuelPlayers[player & 1].graveCount = count;
        DrawAllAreaTiles();
        DuelLink_SendMessage(LINKMSG_GRAVEYARD_ACK, 0, 0, 0);
        gLinkState.graveReceived = 1;
        return 1;
    case LINKMSG_FUSION_DECK_LIST:
        player = gLinkState.rxArgs[0] >> 8; count = (u8)gLinkState.rxArgs[0];
        player = 1 - player;
        for (i = 0; i < count; i++) {
            const struct DuelCard *src = &gLinkRxCards[i];
            struct DuelCard *card = &gDuelPlayers[player & 1].fusionDeck[i];
            CopyDuelCard((u32 *)card, (u32 *)src);
            card->owner = 1 - card->owner;
        }
        gDuelPlayers[player & 1].fusionCount = count;
        DrawAllAreaTiles();
        DuelLink_SendMessage(LINKMSG_FUSION_DECK_ACK, 0, 0, 0);
        gLinkState.fusionReceived = 1;
        return 1;
    case LINKMSG_BANISHED_LIST:
        player = gLinkState.rxArgs[0] >> 8; count = (u8)gLinkState.rxArgs[0];
        player = 1 - player;
        for (i = 0; i < count; i++) {
            const struct DuelCard *src = &gLinkRxCards[i];
            struct DuelCard *card = &gDuelPlayers[player & 1].banished[i];
            CopyDuelCard((u32 *)card, (u32 *)src);
            card->owner = 1 - card->owner;
        }
        /* The ROM does not update banishedCount here. */
        DrawAllAreaTiles();
        DuelLink_SendMessage(LINKMSG_BANISHED_ACK, 0, 0, 0);
        gLinkState.fusionReceived = 1;              /* sic: the fusion deck's flag */
        return 1;
    /* The partner stored one of our lists. */
    case LINKMSG_HAND_ACK: gLinkState.handAcked = 1; return 1;
    case LINKMSG_DECK_ACK: gLinkState.deckAcked = 1; return 1;
    case LINKMSG_GRAVEYARD_ACK: gLinkState.graveAcked = 1; return 1;
    case LINKMSG_FUSION_DECK_ACK: gLinkState.fusionAcked = 1; return 1;
    case LINKMSG_BANISHED_ACK: gLinkState.banishedAcked = 1; return 1;
    case LINKMSG_COMMAND:
        DuelLink_MirrorCommand();
        gLinkState.remoteCmdPending = 1;
        gLinkState.remoteCmdStep = 0;
        return 1;
    case LINKMSG_COMMAND_RUNNING: gLinkState.cmdAckTimer = 0; return 1;
    case LINKMSG_COMMAND_DONE: gLinkState.remoteCmdDone = 1; return 1;
    case LINKMSG_ABORT:
        FadeOutBGM();
        gDuel.linkError = 1;
        return 0;
    default:
        DuelLink_SendMessage(LINKMSG_ABORT, 0, 0, 0);
        FadeOutBGM();
        gDuel.linkError = 1;
        return 0;
    }
}
