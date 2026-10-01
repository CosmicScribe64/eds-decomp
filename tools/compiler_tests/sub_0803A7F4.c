/*
 * sub_0803A7F4 @ 0x0803A7F4, size 0x28 bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, old_agbcc -O1
 *   differs: agbcc -O2 -fprologue-bugfix, agbcc -O2, agbcc -O1 -fprologue-bugfix, agbcc -O1, old_agbcc -O2 (no -mthumb-interwork)
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
struct Unk0803A7F4 {
    u16 unk0;
    u16 unk2_0:1;
    u16 unk2_1:15;
    u8 unk4_0:2;
    u8 unk4_2:1;
    u8 unk4_3:5;
};
void sub_08019860(u32 a, u32 b);
u32 sub_0803A7F4(struct Unk0803A7F4 *p) {
    if (!p->unk4_2)
        sub_08019860(1 - p->unk2_0, 500);
    return 0;
}
