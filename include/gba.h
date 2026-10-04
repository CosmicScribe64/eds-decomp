#ifndef GUARD_GBA_H
#define GUARD_GBA_H

/*
 * GBA hardware: memory map, IO registers and their bit values, the OAM attribute and SIOCNT (multi-player)
 * layouts, and the three BIOS calls the game links (src/sdk/libagbsyscall.s).
 *
 * Register names follow GBATEK / pret. tools/xref.py reads the register names from the lines of the form
 * `#define REG_X REG16(0x...)` / `REG32(0x...)` below (the last name for an offset wins): keep that form.
 *
 * Matching note: many units write registers through an integer address (`*(vu16 *)0x04000050`) or through a
 * pointer to one register (`vu16 *reg = &REG_BG0HOFS; reg[2] = ...`). Those forms are matching choices; keep
 * them when a unit switches to this header.
 */

#include "global.h"

/* ------------------------------------------------------------------------------------------------------ */
/* Memory map                                                                                             */
/* ------------------------------------------------------------------------------------------------------ */

#define EWRAM       0x02000000  /* 256 KiB on-board work RAM */
#define IWRAM       0x03000000  /* 32 KiB on-chip work RAM (gMain, IntrTable, sound driver, link buffers) */
#define REG_BASE    0x04000000  /* IO registers */
#define PLTT        0x05000000  /* palette RAM: 256 BG colours, then 256 OBJ colours (BGR555) */
#define BG_PLTT     0x05000000
#define OBJ_PLTT    0x05000200
#define VRAM        0x06000000  /* 96 KiB video RAM */
#define BG_VRAM     0x06000000
#define OBJ_VRAM0   0x06010000  /* OBJ tiles (tile modes) */
#define OBJ_VRAM1   0x06014000  /* OBJ tiles 0x200-0x3FF: the only OBJ tiles in the bitmap modes 3-5 */
#define OAM         0x07000000  /* 128 OAM entries (struct OamEntry) */
#define SRAM        0x0E000000  /* 32 KiB battery SRAM (save game) */

#define PLTT_SIZE       0x400
#define BG_PLTT_SIZE    0x200
#define OBJ_PLTT_SIZE   0x200
#define OAM_SIZE        0x400
#define BG_CHAR_SIZE    0x4000  /* one BGxCNT character base block */
#define BG_SCREEN_SIZE  0x800   /* one BGxCNT screen base block (32x32 text map) */

#define BG_CHAR_ADDR(n)   (BG_VRAM + BG_CHAR_SIZE * (n))
#define BG_SCREEN_ADDR(n) (BG_VRAM + BG_SCREEN_SIZE * (n))

/* Mode 4: two 240x160 8bpp frames; DISPCNT_FRAME_SELECT shows the second. */
#define MODE4_FRAME0        (VRAM)
#define MODE4_FRAME1        (VRAM + 0xA000)
#define MODE4_FRAME_SIZE    (240 * 160)

/* BIOS work RAM at the top of IWRAM: the IRQ handler address (GameInit points it at gMain.intrMainBuf). */
#define INTR_VECTOR (*(void **)0x03007FFC)
#define INTR_CHECK  (*(vu16 *)0x03007FF8)

/* ------------------------------------------------------------------------------------------------------ */
/* IO registers                                                                                           */
/* ------------------------------------------------------------------------------------------------------ */

#define REG16(off) (*(vu16 *)(REG_BASE + (off)))
#define REG32(off) (*(vu32 *)(REG_BASE + (off)))
#define REG_ADDR(off) (REG_BASE + (off))

/* LCD */
#define REG_DISPCNT   REG16(0x000)
#define REG_DISPSTAT  REG16(0x004)
#define REG_VCOUNT    REG16(0x006)
#define REG_BG0CNT    REG16(0x008)
#define REG_BG1CNT    REG16(0x00A)
#define REG_BG2CNT    REG16(0x00C)
#define REG_BG3CNT    REG16(0x00E)
#define REG_BG0HOFS   REG16(0x010)
#define REG_BG0VOFS   REG16(0x012)
#define REG_BG1HOFS   REG16(0x014)
#define REG_BG1VOFS   REG16(0x016)
#define REG_BG2HOFS   REG16(0x018)
#define REG_BG2VOFS   REG16(0x01A)
#define REG_BG3HOFS   REG16(0x01C)
#define REG_BG3VOFS   REG16(0x01E)
#define REG_BG2PA     REG16(0x020)
#define REG_BG2PB     REG16(0x022)
#define REG_BG2PC     REG16(0x024)
#define REG_BG2PD     REG16(0x026)
#define REG_BG2X_L    REG16(0x028)  /* 16-bit halves of the BG2/BG3 reference points (written as halfwords */
#define REG_BG2X_H    REG16(0x02A)  /* by the bust-up and Destiny Board scenes); the REG32 names come last */
#define REG_BG2Y_L    REG16(0x02C)  /* so that tools/xref.py names 0x028 / 0x02C after them */
#define REG_BG2Y_H    REG16(0x02E)
#define REG_BG2X      REG32(0x028)
#define REG_BG2Y      REG32(0x02C)
#define REG_BG3PA     REG16(0x030)
#define REG_BG3PB     REG16(0x032)
#define REG_BG3PC     REG16(0x034)
#define REG_BG3PD     REG16(0x036)
#define REG_BG3X_L    REG16(0x038)
#define REG_BG3X_H    REG16(0x03A)
#define REG_BG3Y_L    REG16(0x03C)
#define REG_BG3Y_H    REG16(0x03E)
#define REG_BG3X      REG32(0x038)
#define REG_BG3Y      REG32(0x03C)
#define REG_WIN0H     REG16(0x040)
#define REG_WIN1H     REG16(0x042)
#define REG_WIN0V     REG16(0x044)
#define REG_WIN1V     REG16(0x046)
#define REG_WININ     REG16(0x048)
#define REG_WINOUT    REG16(0x04A)
#define REG_MOSAIC    REG16(0x04C)
#define REG_BLDCNT    REG16(0x050)
#define REG_BLDALPHA  REG16(0x052)
#define REG_BLDY      REG16(0x054)

/* Sound */
#define REG_SOUND1CNT_L REG16(0x060)
#define REG_SOUND1CNT_H REG16(0x062)
#define REG_SOUND1CNT_X REG16(0x064)
#define REG_SOUND2CNT_L REG16(0x068)
#define REG_SOUND2CNT_H REG16(0x06C)
#define REG_SOUND3CNT_L REG16(0x070)
#define REG_SOUND3CNT_H REG16(0x072)
#define REG_SOUND3CNT_X REG16(0x074)
#define REG_SOUND4CNT_L REG16(0x078)
#define REG_SOUND4CNT_H REG16(0x07C)
#define REG_SOUNDCNT_L  REG16(0x080)
#define REG_SOUNDCNT_H  REG16(0x082)
#define REG_SOUNDCNT_X  REG16(0x084)
#define REG_SOUNDBIAS   REG16(0x088)
#define REG_WAVE_RAM0   REG32(0x090)
#define REG_WAVE_RAM1   REG32(0x094)
#define REG_WAVE_RAM2   REG32(0x098)
#define REG_WAVE_RAM3   REG32(0x09C)
#define REG_FIFO_A      REG32(0x0A0)
#define REG_FIFO_B      REG32(0x0A4)

/* DMA: SAD, DAD, then CNT (CNT_L = word count, CNT_H = control; written together as one u32) */
#define REG_DMA0SAD   REG32(0x0B0)
#define REG_DMA0DAD   REG32(0x0B4)
#define REG_DMA0CNT_L REG16(0x0B8)
#define REG_DMA0CNT_H REG16(0x0BA)
#define REG_DMA0CNT   REG32(0x0B8)
#define REG_DMA1SAD   REG32(0x0BC)
#define REG_DMA1DAD   REG32(0x0C0)
#define REG_DMA1CNT_L REG16(0x0C4)
#define REG_DMA1CNT_H REG16(0x0C6)
#define REG_DMA1CNT   REG32(0x0C4)
#define REG_DMA2SAD   REG32(0x0C8)
#define REG_DMA2DAD   REG32(0x0CC)
#define REG_DMA2CNT_L REG16(0x0D0)
#define REG_DMA2CNT_H REG16(0x0D2)
#define REG_DMA2CNT   REG32(0x0D0)
#define REG_DMA3SAD   REG32(0x0D4)
#define REG_DMA3DAD   REG32(0x0D8)
#define REG_DMA3CNT_L REG16(0x0DC)
#define REG_DMA3CNT_H REG16(0x0DE)
#define REG_DMA3CNT   REG32(0x0DC)

/* Timers: CNT_L = reload/counter, CNT_H = control */
#define REG_TM0CNT_L  REG16(0x100)
#define REG_TM0CNT_H  REG16(0x102)
#define REG_TM1CNT_L  REG16(0x104)
#define REG_TM1CNT_H  REG16(0x106)
#define REG_TM2CNT_L  REG16(0x108)
#define REG_TM2CNT_H  REG16(0x10A)
#define REG_TM3CNT_L  REG16(0x10C)
#define REG_TM3CNT_H  REG16(0x10E)

/* Serial (link cable) */
#define REG_SIOMULTI0   REG16(0x120)
#define REG_SIOMULTI1   REG16(0x122)
#define REG_SIOMULTI2   REG16(0x124)
#define REG_SIOMULTI3   REG16(0x126)
#define REG_SIOCNT      REG16(0x128)
#define REG_SIOMLT_SEND REG16(0x12A)

/* Keys */
#define REG_KEYINPUT  REG16(0x130)
#define REG_KEYCNT    REG16(0x132)
#define REG_RCNT      REG16(0x134)

/* Interrupts, waitstates */
#define REG_IE        REG16(0x200)
#define REG_IF        REG16(0x202)
#define REG_WAITCNT   REG16(0x204)
#define REG_IME       REG16(0x208)

/* ------------------------------------------------------------------------------------------------------ */
/* Register bit values                                                                                    */
/* ------------------------------------------------------------------------------------------------------ */

/* DISPCNT */
#define DISPCNT_MODE_0          0x0000  /* BG0-3 text */
#define DISPCNT_MODE_1          0x0001  /* BG0-1 text, BG2 affine */
#define DISPCNT_MODE_2          0x0002  /* BG2-3 affine */
#define DISPCNT_MODE_3          0x0003
#define DISPCNT_MODE_4          0x0004
#define DISPCNT_MODE_5          0x0005
#define DISPCNT_FRAME_SELECT    0x0010  /* modes 4/5: display frame 1 (VRAM + 0xA000) */
#define DISPCNT_HBLANK_INTERVAL 0x0020  /* OAM accessible during HBlank */
#define DISPCNT_OBJ_1D_MAP      0x0040
#define DISPCNT_FORCED_BLANK    0x0080
#define DISPCNT_BG0_ON          0x0100
#define DISPCNT_BG1_ON          0x0200
#define DISPCNT_BG2_ON          0x0400
#define DISPCNT_BG3_ON          0x0800
#define DISPCNT_BG_ALL_ON       0x0F00
#define DISPCNT_OBJ_ON          0x1000
#define DISPCNT_WIN0_ON         0x2000
#define DISPCNT_WIN1_ON         0x4000
#define DISPCNT_OBJWIN_ON       0x8000

/* DISPSTAT */
#define DISPSTAT_VBLANK         0x0001  /* in VBlank (read only) */
#define DISPSTAT_HBLANK         0x0002
#define DISPSTAT_VCOUNT         0x0004
#define DISPSTAT_VBLANK_INTR    0x0008
#define DISPSTAT_HBLANK_INTR    0x0010
#define DISPSTAT_VCOUNT_INTR    0x0020

/* BGxCNT */
#define BGCNT_PRIORITY(n)       (n)             /* 0 = front, 3 = back */
#define BGCNT_CHARBASE(n)       ((n) << 2)      /* tile data at VRAM + n * 0x4000 */
#define BGCNT_MOSAIC            0x0040
#define BGCNT_16COLOR           0x0000          /* 4bpp tiles */
#define BGCNT_256COLOR          0x0080          /* 8bpp tiles */
#define BGCNT_SCREENBASE(n)     ((n) << 8)      /* map at VRAM + n * 0x800 */
#define BGCNT_WRAP              0x2000          /* affine BGs only */
#define BGCNT_TXT256x256        0x0000
#define BGCNT_TXT512x256        0x4000
#define BGCNT_TXT256x512        0x8000
#define BGCNT_TXT512x512        0xC000
#define BGCNT_AFF128x128        0x0000
#define BGCNT_AFF256x256        0x4000
#define BGCNT_AFF512x512        0x8000
#define BGCNT_AFF1024x1024      0xC000

/* WININ / WINOUT (per window: one byte) */
#define WININ_WIN0_BG0          0x0001
#define WININ_WIN0_BG1          0x0002
#define WININ_WIN0_BG2          0x0004
#define WININ_WIN0_BG3          0x0008
#define WININ_WIN0_BG_ALL       0x000F
#define WININ_WIN0_OBJ          0x0010
#define WININ_WIN0_CLR          0x0020  /* colour special effects enabled */
#define WININ_WIN0_ALL          0x003F
#define WININ_WIN1_ALL          0x3F00
#define WINOUT_WIN01_ALL        0x003F
#define WINOUT_WINOBJ_ALL       0x3F00

/* BLDCNT: 1st target bits 0-5, effect bits 6-7, 2nd target bits 8-13 */
#define BLDCNT_TGT1_BG0         0x0001
#define BLDCNT_TGT1_BG1         0x0002
#define BLDCNT_TGT1_BG2         0x0004
#define BLDCNT_TGT1_BG3         0x0008
#define BLDCNT_TGT1_OBJ         0x0010
#define BLDCNT_TGT1_BD          0x0020  /* backdrop */
#define BLDCNT_TGT1_ALL         0x003F
#define BLDCNT_EFFECT_NONE      0x0000
#define BLDCNT_EFFECT_BLEND     0x0040  /* alpha blend (BLDALPHA) */
#define BLDCNT_EFFECT_LIGHTEN   0x0080  /* brightness increase (BLDY) */
#define BLDCNT_EFFECT_DARKEN    0x00C0  /* brightness decrease (BLDY) */
#define BLDCNT_TGT2_BG0         0x0100
#define BLDCNT_TGT2_BG1         0x0200
#define BLDCNT_TGT2_BG2         0x0400
#define BLDCNT_TGT2_BG3         0x0800
#define BLDCNT_TGT2_OBJ         0x1000
#define BLDCNT_TGT2_BD          0x2000
#define BLDCNT_TGT2_ALL         0x3F00

/* BLDALPHA: EVA (1st target weight) bits 0-4, EVB (2nd target weight) bits 8-12, each 0..16. The operand
 * order is the ROM's for variable weights (eva first); with constants either order folds to the same value.
 * duel_cmd_screen narrows each weight to u8 as well and keeps its own macro. */
#define BLDALPHA_BLEND(eva, evb) ((eva) | ((evb) << 8))

/* DMA control (CNT_H) */
#define DMA_DEST_INC            0x0000
#define DMA_DEST_DEC            0x0020
#define DMA_DEST_FIXED          0x0040
#define DMA_DEST_RELOAD         0x0060
#define DMA_SRC_INC             0x0000
#define DMA_SRC_DEC             0x0080
#define DMA_SRC_FIXED           0x0100
#define DMA_REPEAT              0x0200
#define DMA_16BIT               0x0000
#define DMA_32BIT               0x0400
#define DMA_DREQ_ON             0x0800
#define DMA_START_NOW           0x0000
#define DMA_START_VBLANK        0x1000
#define DMA_START_HBLANK        0x2000
#define DMA_START_SPECIAL       0x3000  /* sound FIFO (DMA1/2), video capture (DMA3) */
#define DMA_INTR_ENABLE         0x4000
#define DMA_ENABLE              0x8000

/* Timer control (CNT_H) */
#define TIMER_1CLK              0x00
#define TIMER_64CLK             0x01
#define TIMER_256CLK            0x02
#define TIMER_1024CLK           0x03
#define TIMER_COUNTUP           0x04    /* cascade: count overflows of the previous timer */
#define TIMER_INTR_ENABLE       0x40
#define TIMER_ENABLE            0x80

/* IE / IF bits (the IntrTable slot order is different: see enum IntrSlot in main.h) */
#define INTR_FLAG_VBLANK        0x0001
#define INTR_FLAG_HBLANK        0x0002
#define INTR_FLAG_VCOUNT        0x0004
#define INTR_FLAG_TIMER0        0x0008
#define INTR_FLAG_TIMER1        0x0010
#define INTR_FLAG_TIMER2        0x0020
#define INTR_FLAG_TIMER3        0x0040
#define INTR_FLAG_SERIAL        0x0080
#define INTR_FLAG_DMA0          0x0100
#define INTR_FLAG_DMA1          0x0200
#define INTR_FLAG_DMA2          0x0400
#define INTR_FLAG_DMA3          0x0800
#define INTR_FLAG_KEYPAD        0x1000
#define INTR_FLAG_GAMEPAK       0x2000

/* SIOCNT, multi-player mode (see struct SioMultiCnt for the bitfield view) */
#define SIO_MULTI_MODE          0x2000  /* mode bits 12-13 = 2 */
#define SIO_MULTI_SI            0x0004  /* SI terminal: 0 = parent */
#define SIO_MULTI_SD            0x0008  /* SD terminal: 1 = all connected and ready */
#define SIO_ID                  0x0030  /* multi-player ID (bits 4-5) */
#define SIO_ERROR               0x0040
#define SIO_START               0x0080  /* start / busy */
#define SIO_MULTI_BUSY          0x0080
#define SIO_ENABLE              0x0080
#define SIO_INTR_ENABLE         0x4000
#define SIO_115200_BPS          0x0003

/* Keys (KEYINPUT is active low: ReadKeys stores ~KEYINPUT in gMain.heldKeys) */
#define A_BUTTON      0x0001
#define B_BUTTON      0x0002
#define SELECT_BUTTON 0x0004
#define START_BUTTON  0x0008
#define DPAD_RIGHT    0x0010
#define DPAD_LEFT     0x0020
#define DPAD_UP       0x0040
#define DPAD_DOWN     0x0080
#define R_BUTTON      0x0100
#define L_BUTTON      0x0200
#define DPAD_ANY      0x00F0
#define KEYS_MASK     0x03FF

/* ------------------------------------------------------------------------------------------------------ */
/* OAM                                                                                                    */
/* ------------------------------------------------------------------------------------------------------ */

/*
 * One OAM entry (8 bytes), the layout of OAM and of the shadow copy gMain.oamBuffer. The fourth halfword of
 * every entry belongs to the affine matrices: entries 4k..4k+3 hold PA, PB, PC, PD of matrix k (8.8 fixed).
 * Some units use a bitfield view of the first three halfwords (sprite.c SprAnimDraw*).
 */
struct OamEntry {
    u16 attr0;          /* +0 y:8, affineMode:2, objMode:2, mosaic:1, bpp:1, shape:2 */
    u16 attr1;          /* +2 x:9, matrixNum:5 (or 3 unused + hflip:1 + vflip:1), size:2 */
    u16 attr2;          /* +4 tileNum:10, priority:2, paletteNum:4 */
    s16 affineParam;    /* +6 PA/PB/PC/PD of affine matrix (index / 4) */
};

STATIC_ASSERT(sizeof(struct OamEntry) == 0x8, OamEntrySize);

/* attr0 */
#define OAM_ATTR0_Y(y)              ((y) & 0xFF)
#define OAM_ATTR0_AFFINE            0x0100  /* affine on (matrix in attr1) */
#define OAM_ATTR0_HIDE              0x0200  /* affine off: entry not drawn */
#define OAM_ATTR0_AFFINE_DOUBLE     0x0300  /* affine on, double-size canvas */
#define OAM_ATTR0_BLEND             0x0400  /* semi-transparent (BLDCNT 1st target) */
#define OAM_ATTR0_WINDOW            0x0800  /* OBJ window mask */
#define OAM_ATTR0_MOSAIC            0x1000
#define OAM_ATTR0_8BPP              0x2000  /* 256 colours (else 16) */
#define OAM_ATTR0_H_RECTANGLE       0x4000  /* shape: wide */
#define OAM_ATTR0_V_RECTANGLE       0x8000  /* shape: tall */
/* attr1 */
#define OAM_ATTR1_X(x)              ((x) & 0x1FF)
#define OAM_ATTR1_MATRIX(n)         ((n) << 9)  /* affine matrix 0..31 */
#define OAM_ATTR1_HFLIP             0x1000  /* without affine */
#define OAM_ATTR1_VFLIP             0x2000
#define OAM_ATTR1_SIZE(n)           ((n) << 14) /* 0..3; the pixel size depends on the shape */
/* attr2 */
#define OAM_ATTR2_TILE(n)           ((n) & 0x3FF)
#define OAM_ATTR2_PRIORITY(n)       ((n) << 10) /* 0 = front, 3 = back */
#define OAM_ATTR2_PALETTE(n)        ((n) << 12) /* 16-colour palette 0..15 */

/* ------------------------------------------------------------------------------------------------------ */
/* Serial                                                                                                 */
/* ------------------------------------------------------------------------------------------------------ */

/*
 * SIOCNT in multi-player mode, followed by SIOMLT_SEND (REG_SIOCNT as a struct). Used for the volatile
 * register view in LinkSioMain and for the saved copy gLinkSio.sioCntBak.
 */
struct SioMultiCnt {
    u16 baudRate:2;     /* +0 bits 0-1: 3 = 115200 bps */
    u16 si:1;           /* bit 2: SI terminal, 0 = parent */
    u16 sd:1;           /* bit 3: SD terminal, 1 = all connected and ready */
    u16 id:2;           /* bits 4-5: multi-player ID (0 = parent) */
    u16 error:1;        /* bit 6: transfer error */
    u16 enable:1;       /* bit 7: start / busy */
    u16 unused8:4;      /* bits 8-11 */
    u16 mode:2;         /* bits 12-13: SIO mode (2 = multi-player) */
    u16 ifEnable:1;     /* bit 14: serial IRQ enable */
    u16 unused15:1;     /* bit 15 */
    u16 data;           /* +2 SIOMLT_SEND: halfword to send */
};

STATIC_ASSERT(sizeof(struct SioMultiCnt) == 0x4, SioMultiCntSize);

/* ------------------------------------------------------------------------------------------------------ */
/* BIOS calls (libagbsyscall; Thumb stubs in src/sdk/libagbsyscall.s)                                     */
/* ------------------------------------------------------------------------------------------------------ */

/* CpuSet / CpuFastSet control: word/halfword count in bits 0-20 (CpuFastSet: words, multiple of 8). */
#define CPU_SET_SRC_FIXED       0x01000000  /* fill: repeat the first source unit */
#define CPU_SET_16BIT           0x00000000
#define CPU_SET_32BIT           0x04000000
#define CPU_FAST_SET_SRC_FIXED  0x01000000

/* SWI 0x0B: copy or fill (control: count | CPU_SET_* flags). */
void CpuSet(const void *src, void *dest, u32 control);
/* SWI 0x0C: copy or fill in 32-byte blocks (control: word count | CPU_FAST_SET_SRC_FIXED). */
void CpuFastSet(const void *src, void *dest, u32 control);
/* SWI 0x06: signed division, returns num / denom (used by the 8.8 fixed-point helpers in util.h). */
s32 Div(s32 num, s32 denom);

/* Fill size bytes at dest with value through a CpuSet fill, as the SDK macros do. The source has to be a
 * volatile stack temporary: with a plain local the compiler moves the store of the value ahead of the
 * address computation (duel_field_view checked this for CpuFill16). */
#define CpuFill16(value, dest, size)                                \
    {                                                               \
        vu16 fill_ = (value);                                       \
        CpuSet((void *)&fill_, (dest), CPU_SET_SRC_FIXED | (size) / 2); \
    }
#define CpuFill32(value, dest, size)                                \
    {                                                               \
        vu32 fill_ = (value);                                       \
        CpuSet((void *)&fill_, (dest), CPU_SET_SRC_FIXED | CPU_SET_32BIT | (size) / 4); \
    }

#endif /* GUARD_GBA_H */
