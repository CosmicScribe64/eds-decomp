#ifndef GUARD_GBA_H
#define GUARD_GBA_H

/* Memory regions */
#define EWRAM       0x02000000
#define IWRAM       0x03000000
#define REG_BASE    0x04000000
#define PLTT        0x05000000
#define BG_PLTT     0x05000000
#define OBJ_PLTT    0x05000200
#define VRAM        0x06000000
#define OBJ_VRAM0   0x06010000
#define OAM         0x07000000
#define SRAM        0x0E000000

#define REG16(off) (*(vu16 *)(REG_BASE + (off)))
#define REG32(off) (*(vu32 *)(REG_BASE + (off)))

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

/* DMA */
#define REG_DMA0SAD   REG32(0x0B0)
#define REG_DMA0DAD   REG32(0x0B4)
#define REG_DMA0CNT   REG32(0x0B8)
#define REG_DMA3SAD   REG32(0x0D4)
#define REG_DMA3DAD   REG32(0x0D8)
#define REG_DMA3CNT   REG32(0x0DC)

/* Timers */
#define REG_TM0CNT_L  REG16(0x100)
#define REG_TM0CNT_H  REG16(0x102)
#define REG_TM1CNT_L  REG16(0x104)
#define REG_TM1CNT_H  REG16(0x106)
#define REG_TM2CNT_L  REG16(0x108)
#define REG_TM2CNT_H  REG16(0x10A)
#define REG_TM3CNT_L  REG16(0x10C)
#define REG_TM3CNT_H  REG16(0x10E)

/* Serial / keys / interrupts */
#define REG_SIOCNT    REG16(0x128)
#define REG_KEYINPUT  REG16(0x130)
#define REG_KEYCNT    REG16(0x132)
#define REG_RCNT      REG16(0x134)
#define REG_IE        REG16(0x200)
#define REG_IF        REG16(0x202)
#define REG_WAITCNT   REG16(0x204)
#define REG_IME       REG16(0x208)

/* Keys (active-low in REG_KEYINPUT) */
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

/* BIOS calls (src/sdk/libagbsyscall.s) */
void CpuSet(const void *src, void *dest, u32 control);
void CpuFastSet(const void *src, void *dest, u32 control);
s32 Div(s32 num, s32 denom);

#endif /* GUARD_GBA_H */
