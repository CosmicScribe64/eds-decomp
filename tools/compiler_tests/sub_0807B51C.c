/*
 * sub_0807B51C @ 0x0807B51C, size 0x18 bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, agbcc -O2, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1
 *   differs: old_agbcc -O2 (no -mthumb-interwork)
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
s32 sub_0807ED00(s32 num, s32 denom);
s16 sub_0807B51C(s16 a) {
    return sub_0807ED00(0x10000, a);
}
