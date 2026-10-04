#ifndef GUARD_DUEL_LINK_H
#define GUARD_DUEL_LINK_H

/*
 * Duel link protocol: the messages two GBAs exchange during a Link Battle.
 *
 * Both consoles run the same duel. Every message starts with a halfword id (enum LinkMsgId) and is queued with
 * DuelLink_SendMessage (id + three halfwords) or DuelLink_SendMessageData (id + a block). DuelLink_PollMessage reads
 * at most one message per frame into gLinkState and sets the matching flag; the DuelLink_Run* steps then answer
 * the partner's requests. Data from the partner is in the partner's point of view: player 0 is the sender, so
 * player bits and card owner bits are flipped on receipt (DuelLink_MirrorCommand for duel commands).
 * Card lists use REQUEST = 0xF01x, LIST = REQUEST + 0x10, ACK = REQUEST + 0x20.
 */

#include "global.h"
#include "duel.h"
#include "chain.h"

/* First halfword of every duel link message. */
enum LinkMsgId {
    LINKMSG_ABORT = 0xEE00,                 /* link error: fade out the BGM and set gDuel.linkError */
    LINKMSG_UNK_EE01 = 0xEE01,              /* sets gLinkState.unk304_0; no reader found */
    LINKMSG_UNK_EE02 = 0xEE02,              /* sets gLinkState.unk304_1; no reader found */
    LINKMSG_FAST_MODE = 0xEE03,             /* the partner turned fast mode on (gDuelScreen.fast) */
    LINKMSG_NOP = 0xF000,                   /* seeded as rxId before each receive */
    LINKMSG_READY = 0xF001,                 /* partner ready (readyReceived) */
    LINKMSG_TURN_END = 0xF002,              /* partner ended its turn (turnEndReceived) */
    LINKMSG_DUEL_RESULT = 0xF003,           /* args[0]: gDuel.result (duelResultReceived) */
    LINKMSG_INTERRUPT_REQUEST = 0xF004,     /* partner pressed A to interrupt during our turn */
    LINKMSG_INTERRUPT_BEGIN = 0xF005,       /* interrupt accepted: sets gDuel.interruptActive (+0x1B14 bit 1) */
    LINKMSG_INTERRUPT_END = 0xF006,         /* clears it and interruptRequested */
    LINKMSG_REQUEST_HAND = 0xF011,          /* ask the partner for its hand list */
    LINKMSG_REQUEST_DECK = 0xF012,
    LINKMSG_REQUEST_GRAVEYARD = 0xF013,
    LINKMSG_REQUEST_FUSION_DECK = 0xF014,
    LINKMSG_REQUEST_BANISHED = 0xF015,
    LINKMSG_HAND_LIST = 0xF021,             /* {count | player << 8, card words}: replaces that list here */
    LINKMSG_DECK_LIST = 0xF022,
    LINKMSG_GRAVEYARD_LIST = 0xF023,
    LINKMSG_FUSION_DECK_LIST = 0xF024,
    LINKMSG_BANISHED_LIST = 0xF025,
    LINKMSG_HAND_ACK = 0xF031,              /* the partner stored our list (sets the matching *Acked flag) */
    LINKMSG_DECK_ACK = 0xF032,
    LINKMSG_GRAVEYARD_ACK = 0xF033,
    LINKMSG_FUSION_DECK_ACK = 0xF034,
    LINKMSG_BANISHED_ACK = 0xF035,
    LINKMSG_COMMAND = 0xF041,               /* a duel command {cmd, arg2, arg4, arg6} to run on this side */
    LINKMSG_COMMAND_RUNNING = 0xF042,       /* sent every 16 frames while it runs (resets cmdAckTimer) */
    LINKMSG_COMMAND_DONE = 0xF043,
    LINKMSG_ACTIVATE_QUERY = 0xF051,        /* a game event (EventResponse_Run: the open window's responseEntry):
                                             * does the partner respond? (activateQueryPending; the receiver
                                             * sets gChain.queryIsChainLink = 0) */
    LINKMSG_QUERY_REPLY = 0xF052,
    LINKMSG_ACTIVATE_CARD = 0xF053,         /* the partner's responding card (chain entry) */
    LINKMSG_CHAIN_QUERY = 0xF054,           /* a chain link (Chain_AskResponse): does the partner chain to it?
                                             * (activateQueryPending; queryIsChainLink = 1) */
    LINKMSG_QUERY_DECLINED = 0xF055,
    LINKMSG_CHAIN_CARD = 0xF056,            /* the partner's chained card (chain entry) */
    LINKMSG_CARD_PROMPT = 0xF057,           /* {card, arg1, arg2}: ask a Yes/No card question (cardPromptPending) */
    LINKMSG_CARD_PROMPT_ANSWER = 0xF058,    /* args[0]: the answer (cardPromptAnswer) */
    LINKMSG_TRIGGER_EVENT = 0xF059,         /* {1 - player, event, arg low half, arg high half}: open a response
                                             * window on the partner's side (EventResponse_Request; the block
                                             * sent is 0xA bytes, the receiver reads four halfwords) */
    LINKMSG_QUEUED_ACTION = 0xF05A,         /* 0x14-byte summon action to start on this side */
    LINKMSG_QUEUED_ACTION_DONE = 0xF05B,
    LINKMSG_CHAIN_LIST_COUNT = 0xF061,      /* args[0]: number of entries (rxChainListCount) */
    LINKMSG_CHAIN_LIST_ENTRY = 0xF062,      /* {index, chain entry}: stored in rxChainList */
    LINKMSG_SHOW_CHAIN_LIST_P0 = 0xF063,    /* show rxChainList on the chain-list screen (mode 0) */
    LINKMSG_SHOW_CHAIN_LIST_P1 = 0xF064,    /* same, mode 1 */
    LINKMSG_CHAIN_LIST_SHOWN = 0xF065,
    LINKMSG_REMOTE_RESOLVE = 0xF071,        /* run a card's resolve handler on remoteEntries */
    LINKMSG_ADD_ACTION = 0xF072,            /* add an action to the chain (Chain_Add) */
    LINKMSG_REMOTE_RESOLVE_DONE = 0xF073,
    LINKMSG_REMOTE_CHAIN_B = 0xF081,        /* run a card's chainB (target) handler on remoteEntries */
    LINKMSG_REMOTE_CHAIN_B_DONE = 0xF082,   /* the updated chain entry, copied back into gChain */
    LINKMSG_REMOTE_CHAIN_A = 0xF091,        /* run a card's chainA (cost) handler on remoteEntries */
    LINKMSG_REMOTE_CHAIN_A_DONE = 0xF092,   /* the updated chain entry, copied back into gChain */
    LINKMSG_PROMPT = 0xF0A1,                /* duel prompt data for the partner to answer */
    LINKMSG_PROMPT_RESULT = 0xF0A2,         /* 0x10-byte prompt result (promptResultReceived) */
    LINKMSG_ADD_ACTION_A = 0xF0B1           /* add a pending chain action (Chain_AddPending) */
};

/* A short link message: the id and three argument halfwords (DuelLink_SendMessage). The u32 containers are
 * the ROM's: it builds the message as two words with mask-and-or stores (u16 members give four strh). */
struct LinkMessage {
    u32 id:16;                          /* +0x0: enum LinkMsgId */
    u32 arg1:16;                        /* +0x2 */
    u32 arg2:16;                        /* +0x4 */
    u32 arg3:16;                        /* +0x6 */
};

/*
 * gLinkState: the receive buffer, the card-list send buffer and the flags and steps of the link protocol.
 * Cleared at duel start (0x494 bytes). Flags are set by DuelLink_PollMessage unless noted. The bitfield
 * container types (u32 or u8) are the ones the code accesses them with.
 */
struct LinkState {
    u16 rxId;                           /* +0x000: enum LinkMsgId of the message just received */
    u16 rxArgs[0xFF];                   /* +0x002: its payload (arguments, command, list header + cards) */
    u16 unk200;                         /* +0x200: cleared when a command is sent */
    u16 cmdAckTimer;                    /* +0x202: frames waited for LINKMSG_COMMAND_DONE (gives up at 120) */
    u16 txId;                           /* +0x204: card-list send buffer: LINKMSG_*_LIST id */
    u16 txHeader;                       /* +0x206: count | player << 8 */
    u32 txCards[0x3F];                  /* +0x208: card words being sent (also gLinkTxCards) */
    u32 unk304_0:1;                     /* +0x304 bit 0: LINKMSG_UNK_EE01 */
    u32 unk304_1:1;                     /* +0x304 bit 1: LINKMSG_UNK_EE02 */
    u32 readyReceived:1;                /* +0x304 bit 2: LINKMSG_READY */
    u32 unk304_3:5;
    u32 handAcked:1;                    /* +0x305 bit 0: partner stored our hand (cleared when sending it) */
    u32 deckAcked:1;                    /* +0x305 bit 1 */
    u32 graveAcked:1;                   /* +0x305 bit 2 */
    u32 fusionAcked:1;                  /* +0x305 bit 3 */
    u32 banishedAcked:1;                /* +0x305 bit 4 */
    u32 handReceived:1;                 /* +0x305 bit 5: the partner's hand list arrived */
    u32 deckReceived:1;                 /* +0x305 bit 6 */
    u32 graveReceived:1;                /* +0x305 bit 7 */
    u32 fusionReceived:1;               /* +0x306 bit 0: fusion deck (or banished) list arrived */
    u32 unk306_1:1;
    u8 turnEndReceived:1;               /* +0x306 bit 2: LINKMSG_TURN_END */
    u8 duelResultReceived:1;            /* +0x306 bit 3: LINKMSG_DUEL_RESULT */
    u8 unk306_4:1;
    u8 remoteDuelEnded:1;               /* +0x306 bit 5: the partner ran DUEL_CMD_SHOW_DUEL_RESULT */
    u32 interruptRequested:1;           /* +0x306 bit 6: LINKMSG_INTERRUPT_REQUEST */
    u32 queuedActionDone:1;             /* +0x306 bit 7: LINKMSG_QUEUED_ACTION_DONE */
    u32 remoteCmdPending:1;             /* +0x307 bit 0: remoteCmd waits for DuelCmd_RunRemote (it clears it) */
    u32 remoteCmdDone:1;                /* +0x307 bit 1: the partner finished our command */
    u8 activateQueryPending:1;          /* +0x307 bit 2: answer with DuelLink_AnswerActivateQuery */
    u8 queryReplyReceived:1;            /* +0x307 bit 3: the partner answered our query */
    u8 unk307_4:3;
    u32 promptResultReceived:1;         /* +0x307 bit 7: LINKMSG_PROMPT_RESULT */
    u32 remoteChainBPending:1;          /* +0x308 bit 0: run DuelLink_RunRemoteChainB */
    u32 remoteChainBDone:1;             /* +0x308 bit 1 */
    u32 remoteChainAPending:1;          /* +0x308 bit 2: run DuelLink_RunRemoteChainA */
    u32 remoteChainADone:1;             /* +0x308 bit 3 */
    u32 remoteResolvePending:1;         /* +0x308 bit 4: run DuelLink_RunRemoteResolve */
    u32 remoteResolveDone:1;            /* +0x308 bit 5 */
    u32 showChainListPending:1;         /* +0x308 bit 6: show rxChainList, then send CHAIN_LIST_SHOWN */
    u32 chainListShown:1;               /* +0x308 bit 7 */
    u32 unk308_8:24;
    struct ChainEntry rxChainList[16];  /* +0x30C: the partner's chain list (player bit mirrored) */
    u16 rxChainListCount;               /* +0x44C */
    u16 unk44E;
    u32 cardPromptPending:1;            /* +0x450 bit 0: run DuelLink_RunCardPrompt */
    u32 cardPromptAnswered:1;           /* +0x450 bit 1: cardPromptAnswer is valid */
    u32 unk450_2:14;
    u16 cardPromptStep;                 /* +0x452: step of DuelLink_RunCardPrompt */
    u16 cardPromptCard;                 /* +0x454: card of the question */
    u16 cardPromptArg1;                 /* +0x456 */
    u16 cardPromptArg2;                 /* +0x458 */
    u16 cardPromptAnswer;               /* +0x45A: gTextBox.result of the question */
    struct ChainEntry remoteEntries[2]; /* +0x45C: entries the partner asked us to run (also gLinkRemoteEntries) */
    u16 remoteCmd[4];                   /* +0x484: the partner's command in our point of view */
    u8 remoteCmdStep;                   /* +0x48C: step of DuelCmd_RunRemote */
    u8 remoteChainAStep;                /* +0x48D: step of DuelLink_RunRemoteChainA */
    u8 remoteChainBStep;                /* +0x48E: step of DuelLink_RunRemoteChainB */
    u8 remoteResolveStep;               /* +0x48F: step of DuelLink_RunRemoteResolve */
    u8 remoteResolveMode;               /* +0x490: bit 0: pass remoteEntries[1] to the resolve handler */
    u8 unk491[3];
};

/* RAM; the address aliases are other views of gLinkState that matched code uses. */
extern struct LinkState gLinkState;             /* 0x02017FB0 */
extern struct DuelCard gLinkRxCards[];          /* 0x02017FB4 = rxArgs + 2: cards of a received list */
extern u32 gLinkTxCards[0x3F];                  /* 0x020181B8 = gLinkState.txCards */
extern struct ChainEntry gLinkRemoteEntries[2]; /* 0x0201840C = gLinkState.remoteEntries */

/* Sending */
/* Queue {id, arg1, arg2, arg3}; returns the link queue result (0 = queue full). */
u16 DuelLink_SendMessage(u16 id, u16 arg1, u16 arg2, u16 arg3);
/* Queue {id, data[size]}, built in a 0x100-byte buffer (so size <= 0xFE). */
u16 DuelLink_SendMessageData(u16 id, const void *data, int size);
void DuelLink_SendHand(int player);         /* LINKMSG_HAND_LIST with the player's hand; clears handAcked */
void DuelLink_SendDeck(int player);         /* LINKMSG_DECK_LIST; clears deckAcked */
void DuelLink_SendGraveyard(int player);    /* LINKMSG_GRAVEYARD_LIST; clears graveAcked */
void DuelLink_SendFusionDeck(int player);   /* LINKMSG_FUSION_DECK_LIST; clears fusionAcked */
void DuelLink_SendBanished(int player);     /* LINKMSG_BANISHED_LIST; clears banishedAcked */

/* Receiving */
/* Receive and handle at most one message; 1 if a known message was handled, 0 if none or on a link error. */
u16 DuelLink_PollMessage(void);
/* Convert the received duel command to our point of view into gLinkState.remoteCmd. */
void DuelLink_MirrorCommand(void);

/* Answering the partner's requests (each returns 1 when finished) */
u16 DuelLink_RunPartnerRequests(void);      /* run the first pending request; 1 while one is handled */
int DuelLink_AnswerActivateQuery(void);     /* ask our player whether to respond, send the chosen card */
u16 DuelLink_RunCardPrompt(void);           /* Yes/No card question (Sanga/Kuriboh-type effects) */
u16 DuelLink_RunRemoteChainA(void);         /* run the chainA (cost) handler on remoteEntries */
u16 DuelLink_RunRemoteChainB(void);         /* run the chainB (target) handler on remoteEntries */
u16 DuelLink_RunRemoteResolve(void);        /* run the resolve handler on remoteEntries */

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char duel_link_h_check_msg[sizeof(struct LinkMessage) == 0x8 ? 1 : -1];
typedef char duel_link_h_check_tx[(u32)&((struct LinkState *)0)->txCards == 0x208 ? 1 : -1];
typedef char duel_link_h_check_chain[(u32)&((struct LinkState *)0)->rxChainList == 0x30C ? 1 : -1];
typedef char duel_link_h_check_count[(u32)&((struct LinkState *)0)->rxChainListCount == 0x44C ? 1 : -1];
typedef char duel_link_h_check_step[(u32)&((struct LinkState *)0)->cardPromptStep == 0x452 ? 1 : -1];
typedef char duel_link_h_check_remote[(u32)&((struct LinkState *)0)->remoteEntries == 0x45C ? 1 : -1];
typedef char duel_link_h_check_cmd[(u32)&((struct LinkState *)0)->remoteCmd == 0x484 ? 1 : -1];
typedef char duel_link_h_check_mode[(u32)&((struct LinkState *)0)->remoteResolveMode == 0x490 ? 1 : -1];
typedef char duel_link_h_check_size[sizeof(struct LinkState) == 0x494 ? 1 : -1];

#endif /* GUARD_DUEL_LINK_H */
