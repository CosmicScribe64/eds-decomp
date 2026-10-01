/*
 * sub_0802C890 @ 0x0802C890, size 0x24 bytes (code + literal pool + padding)
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
struct Unk0802C890 {
    u16 unk0;
    u8 unk2_0:1;
    u8 unk2_1:3;
    u16 unk2_4:6;
    u8 unk3_2:6;
    u16 unk4;
};
void sub_08017FF4(u32 a, u32 b);
u32 sub_0802C890(struct Unk0802C890 *p) {
    if (p->unk3_2 == 2)
        sub_08017FF4(p->unk2_0, p->unk2_4);
    return 1;
}
