/*
 * sub_08000838 @ 0x08000838, size 0x1C bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, old_agbcc -O2 (no -mthumb-interwork)
 *   differs: agbcc -O2, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
typedef volatile u16 vu16;
#define REG_DISPCNT (*(vu16 *)0x04000000)
struct Unk08000838 {
    u8 filler0[0xC];
    void *unkC;
    u8 filler10[0x10];
    u8 unk20;
    u8 filler21;
    u16 unk22;
};
void sub_08000838(struct Unk08000838 *p) {
    u32 dispcnt = REG_DISPCNT & ~0x10;
    if (p->unk20)
        dispcnt |= 0x10;
    REG_DISPCNT = dispcnt;
}
