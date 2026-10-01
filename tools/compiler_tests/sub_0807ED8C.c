/*
 * sub_0807ED8C @ 0x0807ED8C, size 0x40 bytes (code + literal pool + padding)
 * Region: SDK AgbSram library (SRAM_V112)
 * Flags:  agbcc -mthumb-interwork -O1
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1
 *   differs: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, agbcc -O2, old_agbcc -O2 (no -mthumb-interwork)
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
typedef volatile u16 vu16;
#define REG_WAITCNT (*(vu16 *)0x04000204)
void sub_0807ED8C(const u8 *src, u8 *dest, u32 size) {
    REG_WAITCNT = (REG_WAITCNT & ~3) | 3;
    while (size--)
        *dest++ = *src++;
}
