#ifndef GUARD_LINK_H
#define GUARD_LINK_H

/*
 * Link cable, in three layers:
 *
 *   LinkSio*  multi-player SIO driver (gLinkSio), modelled on the SDK's MultiSio: every frame each GBA sends
 *             one 10-halfword frame (sync word, checksum, packet header, 7 payload halfwords) and receives
 *             the partner's. LinkSioMain runs it; LinkSerialIntr is the serial / Timer3 IRQ handler. The GBA
 *             with SI low becomes the parent and clocks the transfers from Timer3.
 *   Link*     BASICSIO packet layer (gLinkBuf) used by the link duel: 12-byte packets (struct LinkPacket)
 *             with a 4-bit sequence number, an outgoing queue and a 64-entry receive ring. LinkVBlankHook
 *             runs once per frame from the VBlank IRQ: it receives at most one packet and answers with one
 *             (next queued packet, ACK, DUP_ACK or RESEND).
 *   LinkSync* two-player value exchange (struct LinkSync) for the turn-order screen and Card Trading: both
 *             sides send {msgId, REQUEST, value} until one ACKs.
 *   Sio*      thin SIOCNT/SIOMULTI register helpers.
 *
 * Code: src/bg_image.c, text_canvas.c and main.c (SIO driver), text_bg.c (packet layer), link_sio.c (LinkSync,
 * Sio*). Wiki: functions/bg-image-c.md, text-canvas-c.md, text-bg-c.md, link-sio-c.md, link-battle-c.md.
 */

#include "global.h"

/* --- Packet layer (gLinkBuf) --------------------------------------------------------------------------- */

/* LinkPacket.header bits 12-15. The code tests (header >> 8) & 0xF0 against 0x90..0xF0 and builds
 * headers such as 0x9000 | count. 0x9/0xA/0xB carry data and go to the receive ring. */
enum LinkPacketType {
    LINK_PKT_SINGLE = 0x9,      /* a whole message of up to 5 halfwords */
    LINK_PKT_PART = 0xA,        /* a middle part of a longer message */
    LINK_PKT_LAST = 0xB,        /* the part that completes a message (sent last, carries src[0..4]) */
    LINK_PKT_DUP_ACK = 0xD,     /* the partner saw this sequence number twice */
    LINK_PKT_RESEND = 0xE,      /* the partner asks for lastSentPacket again */
    LINK_PKT_ACK = 0xF,         /* nothing to send; acknowledges the partner's packet */
};

/* One 12-byte packet of the BASICSIO layer (gLinkBuf queues; gLinkPacketAck/DupAck/Resend in ROM). */
struct LinkPacket {
    u16 header;                 /* bits 0-7 count in halfwords (total for 0x9/0xB, the remainder before this
                                 * chunk for 0xA), bits 8-11 sequence (stamped by LinkSendPacket), bits 12-15
                                 * enum LinkPacketType */
    u16 data[5];                /* payload, 10 bytes */
};

/* State of the packet layer, gLinkBuf (0x030049D0). LinkInit and LinkShutdown clear all 0x840 bytes.
 * lastSentPacket and recvPacket hold struct LinkPacket data but sit at 2-byte offsets, where agbcc
 * cannot place a struct (it aligns every struct to 4), so they are declared as halfword arrays. */
struct LinkBuf {
    struct LinkPacket sendQueue[64];    /* +0x000: outgoing packets: LinkQueueMessage appends, LinkSendNextQueued
                                         *         sends the head and shifts the rest down */
    u16 sendCount;                      /* +0x300: packets in sendQueue */
    u16 lastSentPacket[6];              /* +0x302: copy of the last packet LinkSioSend accepted; resent when
                                         *         the partner asks (LINK_PKT_RESEND) */
    u16 recvPacket[6];                  /* +0x30E: packet received this frame (LinkSioRecv) */
    u8 unk31A[0x200];                   /* +0x31A: never accessed */
    u16 sendSeq;                        /* +0x51A: next sequence number (0..15) */
    u16 lastRecvSeq;                    /* +0x51C: sequence number of the last accepted packet (0xF after
                                         *         LinkInit); a repeat is answered with LINK_PKT_DUP_ACK */
    u16 timeoutFrames;                  /* +0x51E: frames without a valid packet; above 120 -> timedOut */
    u16 unk520;                         /* +0x520 */
    u16 timedOut;                       /* +0x522: 1 after a receive timeout; the link duel then aborts */
    u8 busy:1;                          /* +0x524 bit 0: LinkVBlankHook is running (LinkRecvMessage waits) */
    u8 unused1:1;                       /* +0x524 bit 1: never set */
    u8 msgReady:1;                      /* +0x524 bit 2: a 0x9 or 0xB packet arrived; LinkRecvMessage clears it */
    u16 recvSize;                       /* +0x526: LinkSioRecv's result this frame (bytes, 0 = nothing) */
    u16 recvWrite;                      /* +0x528: recvRing write index (mod 64) */
    u16 recvRead;                       /* +0x52A: recvRing read index (mod 64) */
    struct LinkPacket recvRing[64];     /* +0x52C: received data packets (types 0x9/0xA/0xB) */
    u16 timer2Ticks;                    /* +0x82C: incremented by Timer2Intr (about 5.3 Hz), never read */
    int queueIndex;                     /* +0x830: loop counter of LinkSendNextQueued's shift */
    struct LinkPacket sendBuf;          /* +0x834: staging copy handed to LinkSioSend */
};

typedef char link_h_check_packet_size[sizeof(struct LinkPacket) == 0xC ? 1 : -1];
typedef char link_h_check_buf_size[sizeof(struct LinkBuf) == 0x840 ? 1 : -1];
typedef char link_h_check_buf_count[(u32)&((struct LinkBuf *)0)->sendCount == 0x300 ? 1 : -1];
typedef char link_h_check_buf_last[(u32)&((struct LinkBuf *)0)->lastSentPacket == 0x302 ? 1 : -1];
typedef char link_h_check_buf_recv[(u32)&((struct LinkBuf *)0)->recvPacket == 0x30E ? 1 : -1];
typedef char link_h_check_buf_seq[(u32)&((struct LinkBuf *)0)->sendSeq == 0x51A ? 1 : -1];
typedef char link_h_check_buf_timeout[(u32)&((struct LinkBuf *)0)->timedOut == 0x522 ? 1 : -1];
typedef char link_h_check_buf_size526[(u32)&((struct LinkBuf *)0)->recvSize == 0x526 ? 1 : -1];
typedef char link_h_check_buf_ring[(u32)&((struct LinkBuf *)0)->recvRing == 0x52C ? 1 : -1];
typedef char link_h_check_buf_ticks[(u32)&((struct LinkBuf *)0)->timer2Ticks == 0x82C ? 1 : -1];
typedef char link_h_check_buf_index[(u32)&((struct LinkBuf *)0)->queueIndex == 0x830 ? 1 : -1];
typedef char link_h_check_buf_send[(u32)&((struct LinkBuf *)0)->sendBuf == 0x834 ? 1 : -1];

/* The packet layer. text_bg.c also reaches gLinkBuf.sendBuf through the alias symbol gUnk_03005204 and
 * sendSeq / lastSentPacket at negative offsets from it; keep that form (matching choice). */
extern struct LinkBuf gLinkBuf;

/* --- Multi-player SIO driver (gLinkSio) ------------------------------------------------------------------ */

/* High nibble of a SIO packet header (gLinkSio.txMsg[0], txBuf[2], received packet word 0); bits 0-8 hold
 * the length in bytes (names: medium confidence). */
enum LinkSioPacketType {
    LINKSIO_PKT_IDLE = 0x1000,      /* nothing to send */
    LINKSIO_PKT_FIRST = 0x2000,     /* first block of a multi-block message */
    LINKSIO_PKT_LAST = 0x3000,      /* last (or only) block */
    LINKSIO_PKT_MIDDLE = 0x4000,    /* a middle block */
    LINKSIO_PKT_RESEND = 0x5000,    /* checksum error: ask for the block again */
};

/* Bits returned by LinkSioMain and LinkSioCheckRecvData (bit i: player slot i). */
enum LinkSioStatus {
    LINK_SIO_RECV_OK_0 = 0x1,           /* slot 0 delivered a frame with a valid checksum */
    LINK_SIO_RECV_OK_1 = 0x2,
    LINK_SIO_CHECKSUM_ERROR_0 = 0x10,   /* slot 0's frame failed the checksum */
    LINK_SIO_CHECKSUM_ERROR_1 = 0x20,
    LINK_SIO_IS_PARENT = 0x80,          /* this GBA is the parent (LinkSioMain only) */
};

/* The same bits as stored in gLinkSio.stepResult and tested by the receive functions (LinkSioRecv*). */
enum LinkStepFlags {
    LINKSIO_RX_OK_0 = 0x1,
    LINKSIO_RX_OK_1 = 0x2,
    LINKSIO_RX_BAD_0 = 0x10,
    LINKSIO_RX_BAD_1 = 0x20,
    LINKSIO_RX_MASTER = 0x80,
};

/* Values of gLinkSio.master (names: medium confidence). */
enum LinkSioType {
    LINK_SIO_CHILD = 0,
    LINK_SIO_PARENT = 8,        /* this GBA clocks the transfers */
};

/* Multi-player SIO state, gLinkSio (0x03005B60, 0xB38 bytes; LinkSioInit clears it). Several members are
 * loop counters and temporaries of the driver functions; they must stay in memory for the match. */
struct LinkSio {
    u32 *serialIntrSlot;            /* +0x000: &IntrTable[0] from LinkSioInit; LinkSioStop clears the slot */
    u32 *timer3IntrSlot;            /* +0x004: &IntrTable[7] */
    u16 txMsg[0x102];               /* +0x008: outgoing message: [0] header (LinkSioPacketType | length),
                                     *         [1..] payload (LinkSioSend); read as 16-byte blocks by
                                     *         LinkSioRecvMultiBlock */
    u16 rxMsg[2][2][0x101];         /* +0x20C: received messages, [buffer][slot], double-buffered */
    u8 rxMsgLen[2][2];              /* +0xA14: [buffer][slot] length byte of the stored header
                                     *         (LinkSioRecvDebug only) */
    u8 rxReadBuf[2];                /* +0xA18: per slot, rxMsg buffer to read next (0/1) */
    u8 rxWriteBuf[2];               /* +0xA1A: per slot, rxMsg buffer to write next (0/1) */
    u16 rxPending;                  /* +0xA1C: messages stored but not yet output (LinkSioRecvDebug) */
    u8 master;                      /* +0xA1E: enum LinkSioType */
    u8 stage;                       /* +0xA1F: LinkSioMain stage: 0 waiting for the handshake, 1 running */
    u8 rxStatusAccum;               /* +0xA20: OR of every rxStatus */
    u8 dataReady;                   /* +0xA21: set by LinkSerialIntr when a full frame was swapped into
                                     *         rxDone; consumed by LinkSioCheckRecvData */
    u16 rxStatus;                   /* +0xA22: LinkSioCheckRecvData result (enum LinkSioStatus bits) */
    u8 transferEnabled;             /* +0xA24: set together with master; LinkSioStartTransfer requires it */
    u8 unkA25;                      /* +0xA25 */
    u16 unkA26;                     /* +0xA26: cleared by LinkSioInit and by LinkSioRecv; never read */
    u8 unkA28[2];                   /* +0xA28: set to 0xFF by LinkSioInit; never read */
    u8 unkA2A[2];                   /* +0xA2A */
    s32 state;                      /* +0xA2C: halfword position in the 10-transfer frame (-3..10);
                                     *         0xC = idle after init */
    u16 (*rxBuf)[12];               /* +0xA30: frame being filled by LinkSerialIntr (a row per player) */
    u16 (*rxDone)[12];              /* +0xA34: last complete frame (swapped with rxBuf by the IRQ) */
    u16 (*rxWork)[12];              /* +0xA38: frame taken by LinkSioCheckRecvData (swapped with rxDone) */
    u16 txBuf[12];                  /* +0xA3C: outgoing frame, one halfword per transfer ([0..9] sent):
                                     *         [1] ~checksum, [2] packet header (0x1000 idle), [3..9]
                                     *         payload. LinkSioSetSendData copies 12 halfwords to [2], so it
                                     *         overruns 4 bytes into rxFrames. */
    u16 rxFrames[3][2][12];         /* +0xA54: storage behind rxBuf / rxDone / rxWork */
    u16 recv[4];                    /* +0xAE4: SIOMULTI0-3 snapshot taken by LinkSerialIntr */
    u32 saved;                      /* +0xAEC: LinkSerialIntr swap temporary */
    u16 blockCount[2];              /* +0xAF0: per slot, blocks in the message in flight */
    u16 blockIndex[2];              /* +0xAF4: per slot, current block */
    u16 msgPending[2];              /* +0xAF8: per slot, non-zero while a message is in flight;
                                     *         LinkSioSend refuses to send while ours is set */
    s32 i;                          /* +0xAFC: slot loop counter (LinkSerialIntr, LinkSioCheckRecvData,
                                     *         LinkSioRecv) */
    s32 j;                          /* +0xB00: halfword loop counter of the checksum */
    u32 unkB04;                     /* +0xB04 */
    u16 *tmpFrame;                  /* +0xB08: swap temporary / current frame row (LinkSioCheckRecvData) */
    u32 sioCnt;                     /* +0xB0C: SIOCNT | SIOMLT_SEND << 16 snapshot from LinkSioMain stage 0
                                     *         (struct SioMultiCnt layout; bit 2 = SI) */
    u16 tmp0;                       /* +0xB10: scratch: data-ready copy, or LinkSioRecv's slot counter */
    u16 tmp1;                       /* +0xB12: scratch: checksum, or LinkSioRecv's received length */
    u16 stepResult;                 /* +0xB14: LinkSioMain result (enum LinkStepFlags) */
    u16 rxPackets[2][8];            /* +0xB16: per slot, the 16-byte packet (header + 7 halfwords) LinkSioMain
                                     *         copies out for LinkSioRecv */
};

typedef char link_h_check_sio_size[sizeof(struct LinkSio) == 0xB38 ? 1 : -1];
typedef char link_h_check_sio_rxmsg[(u32)&((struct LinkSio *)0)->rxMsg == 0x20C ? 1 : -1];
typedef char link_h_check_sio_rxlen[(u32)&((struct LinkSio *)0)->rxMsgLen == 0xA14 ? 1 : -1];
typedef char link_h_check_sio_master[(u32)&((struct LinkSio *)0)->master == 0xA1E ? 1 : -1];
typedef char link_h_check_sio_ready[(u32)&((struct LinkSio *)0)->dataReady == 0xA21 ? 1 : -1];
typedef char link_h_check_sio_status[(u32)&((struct LinkSio *)0)->rxStatus == 0xA22 ? 1 : -1];
typedef char link_h_check_sio_state[(u32)&((struct LinkSio *)0)->state == 0xA2C ? 1 : -1];
typedef char link_h_check_sio_rxbuf[(u32)&((struct LinkSio *)0)->rxBuf == 0xA30 ? 1 : -1];
typedef char link_h_check_sio_txbuf[(u32)&((struct LinkSio *)0)->txBuf == 0xA3C ? 1 : -1];
typedef char link_h_check_sio_frames[(u32)&((struct LinkSio *)0)->rxFrames == 0xA54 ? 1 : -1];
typedef char link_h_check_sio_recv[(u32)&((struct LinkSio *)0)->recv == 0xAE4 ? 1 : -1];
typedef char link_h_check_sio_blocks[(u32)&((struct LinkSio *)0)->blockCount == 0xAF0 ? 1 : -1];
typedef char link_h_check_sio_pending[(u32)&((struct LinkSio *)0)->msgPending == 0xAF8 ? 1 : -1];
typedef char link_h_check_sio_i[(u32)&((struct LinkSio *)0)->i == 0xAFC ? 1 : -1];
typedef char link_h_check_sio_frame[(u32)&((struct LinkSio *)0)->tmpFrame == 0xB08 ? 1 : -1];
typedef char link_h_check_sio_cnt[(u32)&((struct LinkSio *)0)->sioCnt == 0xB0C ? 1 : -1];
typedef char link_h_check_sio_result[(u32)&((struct LinkSio *)0)->stepResult == 0xB14 ? 1 : -1];
typedef char link_h_check_sio_packets[(u32)&((struct LinkSio *)0)->rxPackets == 0xB16 ? 1 : -1];

/* The SIO driver state. text_canvas.c declares gUnk_03006598 (= &gLinkSio.rxWork) without using it, and
 * LinkSyncClose clears it through the integer address 0x03005B60; keep those forms (matching choices). */
extern struct LinkSio gLinkSio;
/* Alias of &gLinkSio.txMsg[1] (0x03005B6A), the outgoing payload; LinkSioSend copies into it. */
extern u16 gLinkSioTxPayload[];
/* Alias of gLinkSio.recv (0x03006644), the SIOMULTI0-3 snapshot; used by LinkSerialIntr. */
extern u16 gSioMultiRecv[4];

/* --- Two-player value exchange (LinkSync) ---------------------------------------------------------------- */

/* LinkSyncPacket.phase (names: medium confidence). */
enum LinkSyncPhase {
    LINKSYNC_PHASE_NONE = 0,
    LINKSYNC_PHASE_REQUEST = 1,     /* {msgId, value}: answer with ACK if msgId matches */
    LINKSYNC_PHASE_RESTART = 3,     /* start over (only received, never sent) */
    LINKSYNC_PHASE_ACK = 4,         /* the partner received our request */
};

/* LinkSync.state, the LinkSyncStep state machine. */
enum LinkSyncState {
    LINKSYNC_OPEN = 0,              /* (re)initialise the SIO layer (LinkSyncOpen) */
    LINKSYNC_SEND = 1,              /* send the request */
    LINKSYNC_WAIT = 2,              /* wait for the partner's request or ACK */
    LINKSYNC_RESEND = 3,            /* back to SEND (never set by the ROM code) */
    LINKSYNC_CLOSE = 5,             /* done: LinkSyncClose, report success */
};

/* The 4-byte message LinkSyncStep sends with LinkSioSend. */
struct LinkSyncPacket {
    u8 msgId;                       /* exchange id chosen by the caller (0x51-0x53 on the turn-order screen);
                                     * a REQUEST with another id restarts the exchange */
    u8 phase;                       /* enum LinkSyncPhase */
    u16 data;                       /* the value exchanged; rx.data is the partner's value */
};

/* One exchange, owned by the caller (TurnOrderSceneWork.linkSync, the Card Trading state). Start it with
 * LinkSyncStart, then call LinkSyncStep every frame until it returns 1. */
struct LinkSync {
    struct LinkSyncPacket tx;       /* +0x0: our message */
    struct LinkSyncPacket rx;       /* +0x4: the partner's message */
    u8 state;                       /* +0x8: enum LinkSyncState */
    u8 pad;                         /* +0x9 */
    u16 timeout;                    /* +0xA: frames left (300 after each event); wrapping to 0xFFFF
                                     *       restarts from LINKSYNC_OPEN */
};

typedef char link_h_check_sync_packet_size[sizeof(struct LinkSyncPacket) == 4 ? 1 : -1];
typedef char link_h_check_sync_size[sizeof(struct LinkSync) == 0xC ? 1 : -1];
typedef char link_h_check_sync_state[(u32)&((struct LinkSync *)0)->state == 8 ? 1 : -1];
typedef char link_h_check_sync_timeout[(u32)&((struct LinkSync *)0)->timeout == 0xA ? 1 : -1];

/* --- Packet layer functions ------------------------------------------------------------------------------ */

/* Set up the packet layer: stop the SIO IRQs, clear gLinkBuf, LinkSioInit, install LinkVBlankHook as
 * gMain.vblankCallbackEarly. */
void LinkInit(void);
/* Stop the SIO IRQs, remove the VBlank hook and clear gLinkBuf. */
void LinkShutdown(void);
/* Per-frame step run from the VBlank IRQ: receive at most one packet and answer it. Returns 1 when a
 * 0x9 or 0xB data packet arrived. */
int LinkVBlankHook(void);
/* Split size bytes at src into packets and append them to the send queue; returns 0 if the queue is full. */
int LinkQueueMessage(u16 *src, u32 size);
/* Read one complete message from the receive ring into dest; returns its length byte, or 0. */
int LinkRecvMessage(u8 *dest);
/* 1 when the multi-part message starting at recvRing[ringIndex] has its closing 0xB packet in the ring. */
int LinkIsRecvMessageComplete(int ringIndex);
/* Stamp sendSeq into a copy of the 12-byte packet and pass it to LinkSioSend; 1 on success. */
int LinkSendPacket(void *pkt);
/* Send the head of the send queue and remove it, or send an ACK when the queue is empty. */
void LinkSendNextQueued(void);
/* Store a received data packet in recvRing and answer with an ACK. */
void LinkStoreRecvPacket(void *pkt);

/* --- SIO driver functions ---------------------------------------------------------------------------------- */

/* Clear gLinkSio, set up the frame buffers and install LinkSerialIntr in both IntrTable slots. */
void LinkSioInit(u32 *serialIntrSlot, u32 *timer3IntrSlot);
/* Disable the serial and Timer3 IRQs, clear both IntrTable slots and reset SIOCNT. */
void LinkSioStop(void);
/* Per-frame driver step (the SDK's MultiSioMain): handshake, parent election, then receive into recvBuf.
 * Returns enum LinkSioStatus bits. */
u16 LinkSioMain(u8 *recvBuf);
/* Parent only: send the 0xFEFE sync word and start a transfer and Timer3. */
void LinkSioStartTransfer(void);
/* Copy a packet into txBuf[2..] and store the checksum in txBuf[1]. */
void LinkSioSetSendData(void *packet);
/* Take the frame completed by the IRQ and check each player's checksum; returns enum LinkSioStatus bits. */
u16 LinkSioCheckRecvData(u8 *recvBuf);
/* SIO multi-player IRQ handler (serial and Timer3 slots): exchange one halfword of the frame. */
void LinkSerialIntr(void);
/* Queue size bytes (1..0x100) from src as our next message; returns 0 while the previous one is still in
 * flight. */
u32 LinkSioSend(void *src, int size);
/* Receive: when both players' frames are valid, store them and copy a finished message of player slot
 * `slot` to dst; returns its length in bytes, or 0. The only receive variant the game uses. */
u16 LinkSioRecv(int slot, void *dst);
/* Unused receive variant that reassembles FIRST/MIDDLE/LAST blocks and asks for resends. */
u32 LinkSioRecvMultiBlock(u32 slot, void *dst);
/* Unused receive variant for single-block (LAST) messages. */
u16 LinkSioRecvSingle(u32 slot, void *dst);
/* Unused receive variant of the debug build (prints each stored block). */
u32 LinkSioRecvDebug(int slot, void *dst);

/* --- Two-player value exchange ----------------------------------------------------------------------------- */

/* Reset the exchange (sync->state = LINKSYNC_OPEN). Takes a struct LinkSync * (defined with u8 *). */
void LinkSyncStart(u8 *sync);
/* One frame of the exchange of `data` under `msgId`; returns 1 when both sides have each other's value
 * (sync->rx.data). */
u32 LinkSyncStep(u16 msgId, u16 data, struct LinkSync *sync);
/* LinkSioInit and clear both packets' data; returns 1. tx and rx are struct LinkSyncPacket *. */
u16 LinkSyncOpen(void *tx, void *rx);
/* LinkSioStop and clear gLinkSio; returns 1. The arguments are not used. */
u32 LinkSyncClose(void *unusedTx, void *unusedRx);

/* --- SIO register helpers (multi-player mode) ------------------------------------------------------------ */

/* RCNT = 0, SIOCNT = multi-player at baud & 3, clear SIOMLT_SEND and SIOMULTI0-3. Unreferenced; irqEnable
 * lands in SIOCNT bit 15, which has no effect in this mode. */
int SioInitMultiPlayer(u8 baud, u8 irqEnable);
/* This GBA's multi-player id, SIOCNT bits 4-5 (0 = parent). */
u32 SioGetMultiPlayerId(void);
/* SIOMLT_SEND = data. */
void SioSetMultiSend(u16 data);
/* SIOMULTI[playerId]. */
u16 SioGetMultiRecv(u8 playerId);
/* SIOCNT & 0x40, the multi-player error flag. */
u16 SioGetError(void);
/* SIOCNT & 8, the SD terminal (all GBAs ready). */
u16 SioAllReady(void);
/* SIOCNT & 4, the SI terminal (0 = parent). */
u16 SioIsChild(void);
/* SIOCNT |= 0x80: start a transfer (parent only). */
void SioStart(void);
/* SIOCNT & 0x80: a transfer is running. */
u16 SioIsBusy(void);

#endif /* GUARD_LINK_H */
