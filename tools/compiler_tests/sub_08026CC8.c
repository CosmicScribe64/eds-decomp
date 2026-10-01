/*
 * sub_08026CC8 @ 0x08026CC8, size 0x68 bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, agbcc -O2
 *   differs: old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1, old_agbcc -O2 (no -mthumb-interwork)
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
typedef volatile u16 vu16;
#define REG_VCOUNT   (*(vu16 *)0x04000006)
#define REG_BG0HOFS  (*(vu16 *)0x04000010)
#define REG_BG3HOFS  (*(vu16 *)0x0400001C)
struct Unk02020310 {
    u8 filler0[0xB06];
    u8 unkB06;
};
extern struct Unk02020310 gUnk_02020310;
extern const s8 gUnk_080823C4[];
void sub_08026CC8(void) {
    REG_BG0HOFS = gUnk_080823C4[(gUnk_02020310.unkB06 + (REG_VCOUNT >> 1)) % 64] + gUnk_02020310.unkB06;
    REG_BG3HOFS = gUnk_02020310.unkB06 - gUnk_080823C4[(gUnk_02020310.unkB06 + (REG_VCOUNT >> 2)) % 64];
}
