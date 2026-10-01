/*
 * sub_08000270 @ 0x08000270, size 0x50 bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, agbcc -O2
 *   differs: old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1, old_agbcc -O2 (no -mthumb-interwork)
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
void sub_0807ECFC(const void *src, void *dest, u32 control);
void sub_08000270(u8 *src, u32 *dest, u16 size, u16 count, u16 stride) {
    u8 i;
    for (i = 0; i < count; i++)
        sub_0807ECFC(src + i * 240, dest + ((i * stride) >> 2), size / 2);
}
