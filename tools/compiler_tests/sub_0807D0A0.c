/*
 * sub_0807D0A0 @ 0x0807D0A0, size 0x78 bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2
 *   differs: agbcc -O2 -fprologue-bugfix, agbcc -O2, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1, old_agbcc -O2 (no -mthumb-interwork)
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
struct Unk03000040 {
    u8 filler0[0x4859];
    u8 unk4859;
    u8 unk485A;
    u8 unk485B;
    u8 filler485C[0x16];
    u16 unk4872;
};
struct Unk0201F780 {
    u16 unk0;
    u8 unk2_0:1;
    u8 unk2_1:1;
    u8 unk2_2:6;
    u8 unk3;
    u32 unk4;
};
extern struct Unk03000040 gUnk_03000040;
extern struct Unk0201F780 gUnk_0201F780;
u16 sub_0806F01C(void);
u32 sub_0807D0A0(void) {
    if (sub_0806F01C()) {
        if (gUnk_03000040.unk4872 != 0) {
            gUnk_0201F780.unk0 = gUnk_03000040.unk4872;
            gUnk_0201F780.unk2_0 = 1;
            gUnk_0201F780.unk2_1 = 1;
        } else {
            gUnk_0201F780.unk0 = 0;
            gUnk_0201F780.unk2_0 = 0;
            gUnk_0201F780.unk2_1 = 0;
        }
        gUnk_03000040.unk4859 = 1;
        gUnk_03000040.unk485A = 0;
        gUnk_03000040.unk485B = 0;
    }
    return 0;
}
