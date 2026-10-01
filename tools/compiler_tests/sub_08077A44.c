/*
 * sub_08077A44 @ 0x08077A44, size 0x18 bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, old_agbcc -O2 (no -mthumb-interwork)
 *   differs: agbcc -O2 -fprologue-bugfix, agbcc -O2, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
struct Unk02011C20 {
    u8 pad0[0x2152];
    u16 unk2152;
};
extern struct Unk02011C20 gUnk_02011C20;
u32 sub_08077A44(void) {
    return gUnk_02011C20.unk2152 & 1;
}
