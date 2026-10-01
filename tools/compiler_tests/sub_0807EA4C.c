/*
 * sub_0807EA4C @ 0x0807EA4C, size 0x3C bytes (code + literal pool + padding)
 * Region: sound driver (0x0807D3D0-0x0807EACF)
 * Flags:  agbcc -mthumb-interwork -O2 -fhex-asm -fprologue-bugfix
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, agbcc -O2, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1
 *   differs: old_agbcc -O2 (no -mthumb-interwork)
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
struct Unk03005210 {
    u32 unk0;
    u8 pad4[0x184];
    u16 unk188;
    s16 unk18A;
    u16 unk18C;
    s16 unk18E;
    u8 unk190;
    u8 unk191;
    u8 unk192;
    u8 unk193;
    u8 unk194;
};
extern struct Unk03005210 gUnk_03005210;
void sub_0807E9C8(s32 a, s32 b);
void sub_0807EA4C(s32 a, s32 b, s32 c) {
    struct Unk03005210 *p = &gUnk_03005210;
    sub_0807E9C8(a, b);
    p->unk192 = c;
    p->unk193 = 0x10;
    p->unk191 = 0;
    p->unk190 = 0;
}
