/*
 * sub_0807EE60 @ 0x0807EE60, size 0x38 bytes (code + literal pool + padding)
 * Region: SDK AgbSram library (SRAM_V112)
 * Flags:  agbcc -mthumb-interwork -O1
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, agbcc -O2, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1
 *   differs: old_agbcc -O2 (no -mthumb-interwork)
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
void sub_0807ED8C(const u8 *src, u8 *dest, u32 size);
u32 sub_0807EDFC(const u8 *src, u8 *dest, u32 size);
u32 sub_0807EE60(const u8 *src, u8 *dest, u32 size) {
    u8 i;
    u32 errorAddr;
    for (i = 0; i < 3; i++) {
        sub_0807ED8C(src, dest, size);
        errorAddr = sub_0807EDFC(src, dest, size);
        if (errorAddr == 0)
            break;
    }
    return errorAddr;
}
