#ifndef GUARD_MAIN_H
#define GUARD_MAIN_H

/*
 * System core (src/main.c): struct Main gMain, the interrupt table and handlers, the main loop, key input,
 * scene (main callback) switching and the per-frame sync of the shadow registers, BG maps and OAM.
 *
 * Frame: MainLoop waits for VBlankIntr to set gMain.intrCheck bit 0, runs FrameSyncUpdate (copies what
 * gMain.vblankFlags selects to the hardware), then the current scene gMain.callback. A scene returns non-zero
 * when it is done, and MainLoop then switches to CB_MainMenu.
 *
 * Migrating a unit from include/legacy/main.h: the legacy field names map to these by offset
 * (header_plan.json "legacy_renames"): unk441C -> textAreaStart, unk441E -> textAreaEnd,
 * unk4870_0 -> firstPlayer, mode4874 -> cardListMode, counter4888 -> matchDuelCount, score -> matchScore,
 * unk488A_0 -> startField, step488A -> subStep; legacy unk4888_0:4 is now unk4888_0:1, opponentFixed:1,
 * duelFormat:2; legacy unk4871[3] is now unk4871 + pickedCardId. Three field types differ from the legacy
 * header: intrCheck is vu16, callback returns u16, oamBuffer is struct OamEntry[128] (was u8[0x400]).
 * A unit that only matches with the old view keeps a commented local view.
 */

#include "global.h"
#include "gba.h"

/* ------------------------------------------------------------------------------------------------------ */
/* Interrupts                                                                                             */
/* ------------------------------------------------------------------------------------------------------ */

/* IntrTable index. IntrMain (crt0) checks the IF bits in this priority order, not in IE bit order. */
enum IntrSlot {
    INTR_SLOT_SERIAL,   /* 0: link cable (LinkSio) */
    INTR_SLOT_HBLANK,   /* 1: per-scene HBlank effects; cleared by SetMainCallback */
    INTR_SLOT_VBLANK,   /* 2: VBlankIntr */
    INTR_SLOT_VCOUNT,
    INTR_SLOT_TIMER0,
    INTR_SLOT_TIMER1,
    INTR_SLOT_TIMER2,   /* 6: Timer2Intr */
    INTR_SLOT_TIMER3,
    INTR_SLOT_DMA0,
    INTR_SLOT_DMA1,
    INTR_SLOT_DMA2,
    INTR_SLOT_DMA3,
    INTR_SLOT_KEYPAD,
    INTR_SLOT_GAMEPAK   /* 13: GamepakIntr */
};

/* 0x03000000: IRQ handlers, indexed by enum IntrSlot (filled by GameInit). */
extern void (*IntrTable[16])(void);

/* ------------------------------------------------------------------------------------------------------ */
/* struct Main                                                                                            */
/* ------------------------------------------------------------------------------------------------------ */

/* gMain.vblankFlags: what FrameSyncUpdate / FlushOamBuffer copy to the hardware after each VBlank. */
enum VBlankFlag {
    VBLANK_COPY_OAM     = 0x1,      /* gMain.oamBuffer -> OAM (then the buffer is reset) */
    VBLANK_COPY_BG_MAPS = 0x2,      /* gMain.bgMapBuffer -> VRAM screenblocks 0-7 */
    VBLANK_BG0_HOFS     = 0x10,     /* gMain.bgHofs[0] -> REG_BG0HOFS */
    VBLANK_BG1_HOFS     = 0x20,
    VBLANK_BG2_HOFS     = 0x40,
    VBLANK_BG3_HOFS     = 0x80,
    VBLANK_BG0_VOFS     = 0x100,    /* gMain.bgVofs[0] -> REG_BG0VOFS */
    VBLANK_BG1_VOFS     = 0x200,
    VBLANK_BG2_VOFS     = 0x400,
    VBLANK_BG3_VOFS     = 0x800
};

/*
 * One entry of gBgHofsRegs / gBgVofsRegs (ROM tables used by FrameSyncUpdate): the scroll register and the
 * VBlankFlag bit that enables writing its shadow value.
 */
struct BgScrollReg {
    vu16 *reg;          /* +0 REG_BGxHOFS or REG_BGxVOFS */
    u16 syncMask;       /* +4 enum VBlankFlag bit */
    u16 pad;            /* +6 */
};

STATIC_ASSERT(sizeof(struct BgScrollReg) == 0x8, BgScrollRegSize);

/*
 * gMain (IWRAM 0x03000040, 0x488C bytes): the system state. Input, interrupts, the scene callback, shadow
 * copies of the BG maps, scroll registers and OAM, frame counters, the scene step bytes and the state that
 * Campaign, Link Battle and the duel hand to each other. Unknown bytes are named unk<offset>.
 */
struct Main {
    u32 rngState;                       /* +0x0000 Random LCG state */
    u16 heldKeys;                       /* +0x0004 ~KEYINPUT (ReadKeys) */
    u16 newKeys;                        /* +0x0006 newly pressed, plus D-pad auto-repeat */
    u16 prevKeys;                       /* +0x0008 last differing key state */
    u16 keyRepeatTimer;                 /* +0x000A frames the same keys are held (repeat after 20) */
    u8 intrMainBuf[0x400];              /* +0x000C IWRAM copy of IntrMain; INTR_VECTOR points here */
    vu16 intrCheck;                     /* +0x040C bit 0 set by VBlankIntr, cleared and polled by MainLoop */
    u16 vblankFlags;                    /* +0x040E enum VBlankFlag */
    u16 (*callback)(void);              /* +0x0410 current scene; non-zero return -> CB_MainMenu */
    void (*vblankCallback)(void);       /* +0x0414 called last in VBlankIntr */
    void (*vblankCallbackEarly)(void);  /* +0x0418 called before SoundVBlank (link poll) */
    u16 bgMapBuffer[8][0x400];          /* +0x041C screenblocks 0-7, copied with VBLANK_COPY_BG_MAPS */
    u16 textAreaStart;                  /* +0x441C top-left map cell of the text area (SetTextArea; unread) */
    u16 textAreaEnd;                    /* +0x441E bottom-right cell, row * 32 + col; & 0x1F = wrap column */
    u16 bgVofs[4];                      /* +0x4420 BG0-3 VOFS shadows (VBLANK_BGn_VOFS) */
    u16 bgHofs[4];                      /* +0x4428 BG0-3 HOFS shadows (VBLANK_BGn_HOFS) */
    struct OamEntry oamBuffer[128];     /* +0x4430 shadow OAM, copied with VBLANK_COPY_OAM */
    u8 oamCount;                        /* +0x4830 next free oamBuffer entry */
    u8 affineCount;                     /* +0x4831 next free affine matrix (max 0x20) */
    u8 brightness:6;                    /* +0x4832 fade level 0..0x1F, mirrored to BLDY/BLDALPHA */
    u8 brightnessFlags:2;               /* +0x4832 bits 6-7, unused */
    u8 unk4833;
    s16 hblankY;                        /* +0x4834 line position of the Card Detail wave effect (-0x20..0xA0) */
    u16 hblankScroll[16];               /* +0x4836 HBlank HOFS values (title screen, Card Detail wave) */
    u8 unk4856;
    u8 seqIndexCampaign;                /* +0x4857 step index of the Campaign and debug-menu runners */
    u8 seqState0;                       /* +0x4858 shared sub-state; CB_LinkBattle's step index */
    u8 seqIndex1;                       /* +0x4859 step index of the menu/Password/Trading/Deck Edit runners */
    u8 seqState1;                       /* +0x485A sub-state of the seqIndex1 steps */
    u8 seqState2;                       /* +0x485B index or sub-state (Record runner index) */
    u16 currentBgm;                     /* +0x485C last BGM started by PlayBGM; 0xFFFF = none */
    u16 frameCounter;                   /* +0x485E +1 per MainLoop iteration */
    u8 frameCounter8;                   /* +0x4860 +1 per MainLoop iteration */
    u8 vblankCounter8;                  /* +0x4861 +1 per VBlank IRQ */
    u16 lastVcount;                     /* +0x4862 VCOUNT latched by the title-menu HBlank handler */
    u16 vblankCounter;                  /* +0x4864 +1 per VBlank IRQ */
    u16 lagCounter;                     /* +0x4866 VBlanks since the last MainLoop iteration */
    u16 lastSeFrame;                    /* +0x4868 frameCounter of the last PlaySE (one SE per frame) */
    u8 unk486A[6];
    u8 firstPlayer:1;                   /* +0x4870 bit 0: who takes the first turn, 0 = this player */
    u8 opponent:5;                      /* +0x4870 bits 1-5: duelist ID of the Campaign opponent */
    u8 result:2;                        /* +0x4870 bits 6-7: last duel result (enum DuelResult, from gDuel) */
    u8 unk4871;
    u16 pickedCardId;                   /* +0x4872 card ID picked in the trade/Prohibition lists; 0 = none */
    u8 cardListMode:2;                  /* +0x4874 enum CardListMode */
    u8 unk4874_2:6;
    u8 unk4875;
    u16 rewardPack;                     /* +0x4876 reward for the Get Pack screen, set after event duels */
    u8 seqIndexTop;                     /* +0x4878 step index of the License and Title runners */
    u8 seq4879;                         /* +0x4879 cleared by SetMainCallback; meaning unknown */
    u8 seq487A;                         /* +0x487A cleared by SetMainCallback; meaning unknown */
    u8 unk487B;
    u32 events;                         /* +0x487C CalendarEvent mask of the current duel (0 = ordinary) */
    u16 rewardCard;                     /* +0x4880 card given after a calendar-event duel (0 = none) */
    u16 unk4882;
    u16 unk4884_0:3;                    /* +0x4884 */
    u16 flag4884_3:1;                   /* +0x4884 bit 3, read by the bust-up scene */
    u16 speaker:8;                      /* +0x4884 bits 4-11: speaker of the current dialogue */
    u16 unk4884_12:4;
    u16 dialogueIndex;                  /* +0x4886 dialogue script index (StartDialogue) */
    u8 unk4888_0:1;                     /* +0x4888 bit 0 */
    u8 opponentFixed:1;                 /* +0x4888 bit 1: opponent already chosen, skip OpponentSelect_Run */
    u8 duelFormat:2;                    /* +0x4888 bits 2-3: enum DuelFormat (1 single, 3 best of 3) */
    u8 matchDuelCount:2;                /* +0x4888 bits 4-5: duels played in the current match */
    u8 unk4888_6:2;
    s8 matchScore;                      /* +0x4889 wins minus losses in the current match */
    u16 startField:4;                   /* +0x488A bits 0-3: enum DuelField in play from the start */
    u16 subStep:8;                      /* +0x488A bits 4-11: sub-state of the Campaign/Link/menu step */
    u16 unk488A_12:4;
};

STATIC_ASSERT(sizeof(struct Main) == 0x488C, MainSize);
STATIC_ASSERT(OFFSET_OF(struct Main, callback) == 0x410, MainCallback);
STATIC_ASSERT(OFFSET_OF(struct Main, bgMapBuffer) == 0x41C, MainBgMapBuffer);
STATIC_ASSERT(OFFSET_OF(struct Main, textAreaStart) == 0x441C, MainTextArea);
STATIC_ASSERT(OFFSET_OF(struct Main, oamBuffer) == 0x4430, MainOamBuffer);
STATIC_ASSERT(OFFSET_OF(struct Main, hblankScroll) == 0x4836, MainHblankScroll);
STATIC_ASSERT(OFFSET_OF(struct Main, seqIndex1) == 0x4859, MainSeqIndex1);
STATIC_ASSERT(OFFSET_OF(struct Main, lastSeFrame) == 0x4868, MainLastSeFrame);
STATIC_ASSERT(OFFSET_OF(struct Main, pickedCardId) == 0x4872, MainPickedCardId);
STATIC_ASSERT(OFFSET_OF(struct Main, events) == 0x487C, MainEvents);
STATIC_ASSERT(OFFSET_OF(struct Main, dialogueIndex) == 0x4886, MainDialogueIndex);
STATIC_ASSERT(OFFSET_OF(struct Main, matchScore) == 0x4889, MainMatchScore);

/* 0x03000040 */
extern struct Main gMain;

/*
 * Linker aliases of gMain fields (symbols.ld), declared separately because some units reach the field
 * through them to match. Prefer the gMain field in new code; keep a unit's alias access as it is.
 */
extern u8 gIntrMainBuf[0x400];                  /* 0x0300004C = gMain.intrMainBuf */
extern vu16 gMain_intrCheck;                    /* 0x0300044C = gMain.intrCheck */
extern u16 (*gMain_callback)(void);             /* 0x03000450 = gMain.callback */
extern struct OamEntry gMain_oamBuffer[128];    /* 0x03004470 = gMain.oamBuffer */

/*
 * Address-named views inside gMain that units declare locally with their own element types (u8[] or u16[]),
 * so they are not declared here: gUnk_0300045C = gMain.bgMapBuffer[0] (also gBgMaps), gUnk_03000C5C =
 * bgMapBuffer[1], gUnk_03001C5C = bgMapBuffer[3], gUnk_03002C5C = bgMapBuffer[5], gUnk_03004876 =
 * hblankScroll, gUnk_0300489E = frameCounter. Fold them into the field only where the code still matches.
 */

/* ------------------------------------------------------------------------------------------------------ */
/* Functions (src/main.c)                                                                                 */
/* ------------------------------------------------------------------------------------------------------ */

/* Boot */
/* Entry point from crt0: GameInit, then MainLoop. */
void AgbMain(void);
/* Clears EWRAM and IWRAM, loads the save, installs the IRQ handlers and IntrMain, starts the License scene. */
void GameInit(void);
/* Never returns: waits for VBlank, FrameSyncUpdate, runs gMain.callback (non-zero -> CB_MainMenu). */
void MainLoop(void);

/* Interrupt handlers (IntrTable) */
/* Sets gMain.intrCheck bit 0, counts the frame, calls vblankCallbackEarly, SoundVBlank, vblankCallback. */
void VBlankIntr(void);
/* Reloads Timer2 (one tick per 0.1875 s) and increments gLinkBuf.timer2Ticks. */
void Timer2Intr(void);
/* Cartridge removed: hangs. */
void GamepakIntr(void);

/* Input */
/* heldKeys = ~KEYINPUT, newKeys = pressed this frame; held D-pad keys repeat every 2nd frame after 20. */
void ReadKeys(void);

/* Scenes */
/* Switches scene: saves the game, clears both VBlank callbacks and the HBlank IRQ, zeroes the step bytes. */
void SetMainCallback(u16 (*callback)(void));

/* Frame sync */
/* With VBLANK_COPY_OAM: copies gMain.oamBuffer to OAM and resets the shadow entries and counters. */
void FlushOamBuffer(void);
/* After VBlank: enabled scroll shadows, BG map buffers, FlushOamBuffer, ReadKeys, SoundMain, one Random. */
void FrameSyncUpdate(void);
/* Zeroes gMain.bgHofs[0..3] and BG0-3HOFS. */
void ResetBgHofs(void);
/* Zeroes gMain.bgVofs[0..3] and BG0-3VOFS. */
void ResetBgVofs(void);
/* ResetBgHofs + ResetBgVofs. */
void ResetBgScroll(void);

#endif /* GUARD_MAIN_H */
