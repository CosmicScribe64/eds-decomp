/*
 * sub_080261A8 @ 0x080261A8, size 0x38 bytes (code + literal pool + padding)
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
struct Unk080823B0 {
    s16 unk0;
    s16 unk2;
};
struct Unk080261A8 {
    u8 filler0[0x14];
};
extern const struct Unk080823B0 gUnk_080823B0[];
void sub_0807A398(s32 a, s32 b, u32 c, u32 d, struct Unk080261A8 *e);
void sub_080261A8(struct Unk080261A8 *p) {
    u8 i;
    for (i = 0; i < 5; i++)
        sub_0807A398(gUnk_080823B0[i].unk2, gUnk_080823B0[i].unk0, 0x68, 0x40, p++);
}
