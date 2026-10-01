/*
 * sub_0807EDCC @ 0x0807EDCC, size 0x30 bytes (code + literal pool + padding)
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
u32 sub_0807EDCC(const u8 *src, u8 *dest, u32 size) {
    while (size--) {
        if (*dest++ != *src++)
            return (u32)(dest - 1);
    }
    return 0;
}
