/*
 * sub_0807B4A8 @ 0x0807B4A8, size 0x18 bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, agbcc -O2, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1, old_agbcc -O2 (no -mthumb-interwork)
 *   differs: -
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
typedef volatile u16 vu16;
void sub_0807B4A8(u16 a) {
    *(vu16 *)0x04000052 = (a << 8) | (16 - a);
}
