#ifndef GUARD_AGB_SRAM_H
#define GUARD_AGB_SRAM_H

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned int   u32;
typedef volatile u16   vu16;

#define REG_WAITCNT (*(vu16 *)0x04000204)
#define SRAM_ADR    0x0E000000
#define SRAM_SIZE   0x8000

void ReadSram(u8 *src, u8 *dst, u32 size);
void WriteSram(u8 *src, u8 *dst, u32 size);
u32  VerifySram(u8 *src, u8 *tgt, u32 size);
u32  WriteSramEx(u8 *src, u8 *dst, u32 size);

#endif // GUARD_AGB_SRAM_H
