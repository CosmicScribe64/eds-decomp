/*
 * sub_08001BC8 @ 0x08001BC8, size 0x24 bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, old_agbcc -O2 (no -mthumb-interwork)
 *   differs: agbcc -O2, agbcc -O1
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
struct Unk0813ADF4 {
    u16 unk0;
    u16 unk2;
    u8 filler4[0x300];
};
extern const struct Unk0813ADF4 gUnk_0813ADF4[];
u16 sub_08001BC8(u32 id) {
    if (id <= 490) return gUnk_0813ADF4[id].unk0; else return 0;
}
