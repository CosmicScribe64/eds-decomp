/*
 * sub_0805FCF4 @ 0x0805FCF4, size 0x34 bytes (code + literal pool + padding)
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
extern u8 gUnk_0201AE60;
extern u8 gUnk_0201AE84[];
void sub_080752B0(void *src, void *dest, u32 size);
void sub_0805FCF4(void) {
    s32 i;
    u8 *src = gUnk_0201AE84;
    for (i = 0; i < 0xd8; i++) {
        sub_080752B0(src, (void *)0x06009A40, 0x20);
        src += 0x20;
    }
    gUnk_0201AE60 |= 2;
}
