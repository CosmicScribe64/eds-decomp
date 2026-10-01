/*
 * sub_0800C894 @ 0x0800C894, size 0x14 bytes (code + literal pool + padding)
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
struct Unk0800ABC8 {
    u32 unk0;
    u32 unk4;
    u32 unk8;
};
void sub_0800ABC8(u32 a, u32 b, struct Unk0800ABC8 *out);
u32 sub_0800C894(u32 a, u32 b) {
    struct Unk0800ABC8 s;
    sub_0800ABC8(a, b, &s);
    return s.unk4;
}
