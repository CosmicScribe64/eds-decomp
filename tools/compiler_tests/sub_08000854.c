/*
 * sub_08000854 @ 0x08000854, size 0x2C bytes (code + literal pool + padding)
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
struct Unk08000838 {
    u8 filler0[0xC];
    void *unkC;
    u8 filler10[0x10];
    u8 unk20;
    u8 filler21;
    u16 unk22;
};
void sub_0807ECFC(const void *src, void *dest, u32 control);
void sub_08000854(struct Unk08000838 *p) {
    if (p->unk22 == 1) sub_0807ECFC(p->unkC, p->unk20 == 1 ? (void *)0x06005A00 : (void *)0x0600FA00, 0x1E00);
}
