#ifndef GUARD_DUEL_UI_H
#define GUARD_DUEL_UI_H

/*
 * Duel command runner (gDuelCmd) and duel screen state (gDuelScreen), merged from all units
 * (tools/structmap.py, 2026-09-30). Separate from duel.h so units can adopt them independently.
 */

#include "global.h"
#include "legacy/duel.h"

/* One duel command: the current one and each queue entry (8 bytes). */
struct DuelCmdEntry {
    u16 cmd;     /* bit 15: player (CMD_PLAYER) */
    u16 arg2;
    u16 arg4;
    u16 arg6;
};

/* gDuelCmd: the duel command runner (current command, queue, step machine). */
struct DuelCmd {
    u16 cmd;                            /* +0x000 (bit 15 = player) */
    u16 arg2;                           /* +0x002 */
    u16 arg4;                           /* +0x004 */
    u16 arg6;                           /* +0x006 */
    struct DuelCmdEntry queue[256];     /* +0x008 */
    u16 queueCount;                     /* +0x808 */
    u16 step:7;                         /* +0x80A bits 0..6 (some units read it as a u8 bitfield) */
    u16 counter:7;                      /* +0x80A bits 7..13 */
    u16 unk80A_14:2;
    u32 unk80C_0:5;                     /* +0x80C */
    u32 timer:7;                        /* +0x80C bits 5..11 */
    u32 unk80C_12:1;
    u32 running:1;                      /* +0x80D bit 5 (some units read it as a u8 bitfield) */
    u32 unk80C_14:18;
    u16 *hofsTable;                     /* +0x810 */
    struct DuelCard card;               /* +0x814: saved card */
    u16 savedCount3[2];                 /* +0x818: per-player deckCount copy (hypothesis) */
    u16 savedCount5[2];                 /* +0x81C: per-player fusionCount copy (hypothesis) */
    u32 savedList7C4[2][80];            /* +0x820: per-player copy of deck[] (hypothesis) */
    u32 savedListA44[2][80];            /* +0xAA0: per-player copy of fusionDeck[] (hypothesis) */
};

/* A card location (player, area, index) packed in 16 bits; padded to 4 bytes. */
struct DuelLoc {
    u16 player:1;
    u16 area:4;
    u16 index:9;
    u16 flag14:1;
    u16 flag15:1;
};

/* gDuelScreen: duel screen / field view state (cursor, scrolling, move animation). */
struct DuelScreen {
    u8 fast:1;                  /* +0x000 bit 0 */
    u8 flag0_1:1;               /* bit 1 (cursorOn? hypothesis) */
    u8 flag0_2:1;               /* bit 2 (active? hypothesis) */
    u8 unk0_3:5;
    u8 unk1;
    u16 unk2;                   /* +0x002 */
    u8 scroll;                  /* +0x004 */
    u8 scrollFrom;              /* +0x005 */
    u8 scrollTo;                /* +0x006 */
    u8 scrollSteps:4;           /* +0x007 bits 0..3 */
    u8 unk7_4:4;
    u8 tiles[0x800];            /* +0x008 */
    u16 tilesDirty:1;           /* +0x808 bit 0 */
    u16 flag808_1:1;
    u16 cursorDone:1;
    u16 busy:1;                 /* bit 3 */
    u16 cursorFlipA:1;
    u16 cursorFlipB:1;
    u16 cursorSteps:4;          /* bits 6..9 */
    u16 unk808_10:6;
    u16 unk80A;
    s32 cursorX;                /* +0x80C */
    s32 cursorY;                /* +0x810 */
    s32 cursorFromX;            /* +0x814 */
    s32 cursorFromY;            /* +0x818 */
    s32 cursorToX;              /* +0x81C */
    s32 cursorToY;              /* +0x820 */
    u32 player;                 /* +0x824 (most units: w824) */
    u32 zone;                   /* +0x828 (most units: w828) */
    u32 cursor;                 /* +0x82C (most units: w82C / idx82C) */
    u8 animActive:1;            /* +0x830 bit 0 */
    u8 animKind:7;
    u8 unk831[3];
    u32 animArg;                /* +0x834 (one unit reads it as u8 player, u8 zone, u16) */
    u8 animStep;                /* +0x838 */
    u8 animTimer;               /* +0x839 */
    u16 animArg83A;             /* +0x83A */
    u16 animArg83C;             /* +0x83C */
    u16 unk83E;
    struct DuelLoc from;        /* +0x840 */
    struct DuelLoc to;          /* +0x844 */
    u8 unk848[0xA];
    u16 unk852;                 /* +0x852 */
    u8 unk854[8];
    void (*cb85C)(void);        /* +0x85C */
};

extern struct DuelCmd gDuelCmd;
extern struct DuelScreen gDuelScreen;

typedef char duel_h_check_cmd_count[(u32)&((struct DuelCmd *)0)->queueCount == 0x808 ? 1 : -1];
typedef char duel_h_check_cmd_hofs[(u32)&((struct DuelCmd *)0)->hofsTable == 0x810 ? 1 : -1];
typedef char duel_h_check_cmd_size[sizeof(struct DuelCmd) == 0xD20 ? 1 : -1];
typedef char duel_h_check_scr_824[(u32)&((struct DuelScreen *)0)->player == 0x824 ? 1 : -1];
typedef char duel_h_check_scr_from[(u32)&((struct DuelScreen *)0)->from == 0x840 ? 1 : -1];
typedef char duel_h_check_scr_852[(u32)&((struct DuelScreen *)0)->unk852 == 0x852 ? 1 : -1];
typedef char duel_h_check_scr_cb[(u32)&((struct DuelScreen *)0)->cb85C == 0x85C ? 1 : -1];



#endif /* GUARD_DUEL_UI_H */
