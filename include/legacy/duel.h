#ifndef GUARD_DUEL_H
#define GUARD_DUEL_H

/*
 * Canonical duel-state structures, merged from how all C units declare them
 * (tools/structmap.py + tools/mkheader.py, 2026-09-30). Field names are the most-used ones; "(hypothesis)"
 * marks meanings that are not verified. Unknown bytes are named unk<offset>.
 *
 * Memory: gDuel (struct DuelState): a u32 then the two players; gDuelPlayers = &players[0] (stride
 * 0xD64); gDuelZones = &players[0].zones[0] (each zone 0x94 bytes).
 *
 * Migrating a unit: replace its local struct definitions with #include "legacy/duel.h", adapt field names, and verify
 * with tools/check.py. If a unit only matches with a different declared type for some field (for example u8
 * instead of u16), keep a local view for that unit and say why; do not change this header to fit one unit.
 */

#include "global.h"

/* A card reference inside a zone or list (32 bits). */
struct DuelCard {
    u32 id:12;       /* card id (alphabetical index into the card tables) */
    u32 owner:1;     /* +0x1 bit 4 */
    u32 unk13:7;
    u32 flag20:1;
    u32 unk21:11;
};

/* One field zone (0x94 bytes); each player has 11 (zones[0..10]). */
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u16 serial;             /* +0x04 */
    u8 flag6_0:1;           /* +0x06 bit 0: face-down? (hypothesis; units disagree on bits 0/1) */
    u8 flag6_1:1;           /* +0x06 bit 1 */
    u8 counter6:4;          /* +0x06 bits 2..5 */
    u8 unk6_6:2;
    u8 unk7;                /* +0x07 */
    u8 unk8[2];             /* +0x08 */
    u16 links[32];          /* +0x0A */
    u16 linkKinds[32];      /* +0x4A */
    u16 numLinks;           /* +0x8A */
    u8 unk8C[8];            /* +0x8C */
};

/* One player's duel state (0xD64 bytes); two of them in struct DuelState. */
struct DuelPlayer {
    u16 lifePoints;             /* +0x000 */
    u8 handCount;               /* +0x002: entries in hand[] */
    u8 deckCount;               /* +0x003: entries in deck[] */
    u8 graveCount;              /* +0x004: entries in graveyard[] (a.k.a. count904) */
    u8 fusionCount;             /* +0x005: entries in fusionDeck[] */
    u8 countB84;                /* +0x006: entries in listB84[] (banished? hypothesis) */
    u8 deckOut:1;               /* +0x007 bit 0 */
    u8 winA:1;                  /* +0x007 bit 1 */
    u8 winExodia:1;             /* +0x007 bit 2 */
    u8 flag7_3:1;               /* +0x007 bit 3 */
    u8 flag7_4:1;
    u8 flag7_5:1;
    u8 turns7_6:2;              /* +0x007 bits 6..7 */
    u8 unk8;                    /* +0x008 */
    u8 unk9;                    /* +0x009 */
    u8 unkA;                    /* +0x00A */
    u8 unkB_0:3;                /* +0x00B bits 0..2 */
    u8 flagB_3:1;               /* +0x00B bit 3 */
    u8 unkB_4:4;
    u8 flagsC;                  /* +0x00C */
    u8 unkD[0x19];              /* +0x00D */
    u16 zoneMask;               /* +0x026 */
    struct DuelZone zones[11];  /* +0x028 */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 (hypothesis: deck, from deckCount) */
    struct DuelCard graveyard[80];  /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 (hypothesis, from fusionCount) */
    struct DuelCard listB84[80];    /* +0xB84 */
    u16 arrCC4[80];                 /* +0xCC4 */
};

/* gDuel: the duel (players, then the marked-card queue and duel flags; known up to +0x1B20). */
struct DuelState {
    u32 unk0;                       /* +0x0000 */
    struct DuelPlayer players[2];   /* +0x0004 */
    u32 unk1ACC_0:15;               /* +0x1ACC (bitfields; units disagree on the split) */
    u32 queueCount:4;               /* +0x1ACD bit 7 .. +0x1ACE bit 2 (a.k.a. numMarked) */
    u32 unk1ACC_19:13;
    u16 queueZone[16];              /* +0x1AD0 (a.k.a. markedZones) */
    u16 queueArg[16];               /* +0x1AF0 (a.k.a. markedCards) */
    u16 unk1B10;                    /* +0x1B10 */
    u8 flag1B12_0:1;                /* +0x1B12 bit 0 */
    u8 linkSkip:1;                  /* bit 1 */
    u8 phase1B12:3;                 /* bits 2..4 */
    u8 linkError:1;                 /* bit 5 */
    u8 result:2;                    /* bits 6..7 */
    u8 unk1B13;
    u32 unk1B14;                    /* +0x1B14: bitfields; units disagree (surrender = bit 1, stage = bits 9..16?) */
    u8 unk1B18[8];                  /* +0x1B18 */
    u8 phaseStep;                   /* +0x1B20 */
};

/* gDuelZones is &players[0].zones[0]: this view (used by most units) indexes zones per player. */
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 rest[0xD64 - 11 * 0x94];
};

extern struct DuelState gDuel;
extern struct DuelPlayer gDuelPlayers[2];
extern struct DuelZonesPlayer gDuelZones[2];

/* Compile-time layout checks (agbcc: structs are padded to 4 bytes). */
typedef char duel_h_check_zone[sizeof(struct DuelZone) == 0x94 ? 1 : -1];
typedef char duel_h_check_player[sizeof(struct DuelPlayer) == 0xD64 ? 1 : -1];
typedef char duel_h_check_hand[(u32)&((struct DuelPlayer *)0)->hand == 0x684 ? 1 : -1];
typedef char duel_h_check_b84[(u32)&((struct DuelPlayer *)0)->listB84 == 0xB84 ? 1 : -1];
typedef char duel_h_check_q[(u32)&((struct DuelState *)0)->queueZone == 0x1AD0 ? 1 : -1];
typedef char duel_h_check_step[(u32)&((struct DuelState *)0)->phaseStep == 0x1B20 ? 1 : -1];
typedef char duel_h_check_zp[sizeof(struct DuelZonesPlayer) == 0xD64 ? 1 : -1];
typedef char duel_h_check_zones[(u32)&((struct DuelPlayer *)0)->zones == 0x28 ? 1 : -1];

#endif /* GUARD_DUEL_H */

