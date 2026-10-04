#ifndef GUARD_MAIN_H
#define GUARD_MAIN_H

/*
 * gMain (gMain): the game's main state in IWRAM, merged from how all C units declare it
 * (tools/structmap.py + tools/mkheader.py, 2026-09-30). Field names are the most-used ones; unknown bytes are
 * named unk<offset>. Only offsets up to 0x488C are known.
 *
 * Migrating a unit: replace its local struct with #include "legacy/main.h" and verify with tools/check.py. If a unit
 * only matches with a different declared type for some field, keep a local view for that unit and say why.
 */

#include "global.h"

struct Main {
    u32 rngState;                   /* +0x0000 */
    u16 heldKeys;                   /* +0x0004 */
    u16 newKeys;                    /* +0x0006 */
    u16 prevKeys;                   /* +0x0008 */
    u16 keyRepeatTimer;             /* +0x000A */
    u8 intrMainBuf[0x400];          /* +0x000C: IntrMain is copied here */
    u16 intrCheck;                  /* +0x040C */
    u16 vblankFlags;                /* +0x040E */
    void (*callback)(void);         /* +0x0410 */
    void (*vblankCallback)(void);   /* +0x0414 */
    void (*vblankCallbackEarly)(void); /* +0x0418 */
    u16 bgMapBuffer[8][0x400];      /* +0x041C */
    u16 unk441C;                    /* +0x441C */
    u16 unk441E;                    /* +0x441E */
    u16 bgVofs[4];                  /* +0x4420 */
    u16 bgHofs[4];                  /* +0x4428 */
    u8 oamBuffer[0x400];            /* +0x4430 */
    u8 oamCount;                    /* +0x4830 */
    u8 affineCount;                 /* +0x4831 */
    u8 brightness:6;                /* +0x4832 bits 0..5 */
    u8 brightnessFlags:2;           /* +0x4832 bits 6..7 */
    u8 unk4833;
    s16 hblankY;                    /* +0x4834 */
    u16 hblankScroll[16];           /* +0x4836 */
    u8 unk4856;                     /* +0x4856 */
    u8 seqIndexCampaign;            /* +0x4857 */
    u8 seqState0;                   /* +0x4858 */
    u8 seqIndex1;                   /* +0x4859 */
    u8 seqState1;                   /* +0x485A */
    u8 seqState2;                   /* +0x485B */
    u16 currentBgm;                 /* +0x485C */
    u16 frameCounter;               /* +0x485E */
    u8 frameCounter8;               /* +0x4860 */
    u8 vblankCounter8;              /* +0x4861 */
    u16 lastVcount;                 /* +0x4862 */
    u16 vblankCounter;              /* +0x4864 */
    u16 lagCounter;                 /* +0x4866 */
    u16 lastSeFrame;                /* +0x4868 */
    u8 unk486A[6];                  /* +0x486A */
    u8 unk4870_0:1;                 /* +0x4870 bit 0 */
    u8 opponent:5;                  /* +0x4870 bits 1..5 */
    u8 result:2;                    /* +0x4870 bits 6..7 */
    u8 unk4871[3];                  /* +0x4871 */
    u8 mode4874:2;                  /* +0x4874 bits 0..1 */
    u8 unk4874_2:6;
    u8 unk4875;
    u16 rewardPack;                 /* +0x4876 */
    u8 seqIndexTop;                 /* +0x4878 */
    u8 seq4879;                     /* +0x4879 */
    u8 seq487A;                     /* +0x487A */
    u8 unk487B;
    u32 events;                     /* +0x487C */
    u16 rewardCard;                 /* +0x4880 */
    u16 unk4882;                    /* +0x4882 */
    u16 unk4884_0:3;                /* +0x4884 */
    u16 flag4884_3:1;
    u16 speaker:8;                  /* +0x4884 bits 4..11 */
    u16 unk4884_12:4;
    u16 dialogueIndex;              /* +0x4886 */
    u8 unk4888_0:4;                 /* +0x4888 */
    u8 counter4888:2;               /* +0x4888 bits 4..5 */
    u8 unk4888_6:2;
    s8 score;                       /* +0x4889 */
    u16 unk488A_0:4;                /* +0x488A */
    u16 step488A:8;                 /* +0x488A bits 4..11 */
    u16 unk488A_12:4;
};

extern struct Main gMain;

typedef char main_h_check_oam[(u32)&((struct Main *)0)->oamBuffer == 0x4430 ? 1 : -1];
typedef char main_h_check_seq[(u32)&((struct Main *)0)->seqIndex1 == 0x4859 ? 1 : -1];
typedef char main_h_check_events[(u32)&((struct Main *)0)->events == 0x487C ? 1 : -1];
typedef char main_h_check_score[(u32)&((struct Main *)0)->score == 0x4889 ? 1 : -1];

#endif /* GUARD_MAIN_H */
