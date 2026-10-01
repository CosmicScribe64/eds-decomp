/*
 * sub_080042D8 @ 0x080042D8, size 0x80 bytes (code + literal pool + padding)
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
extern const u8 gUnk_08198628[];
u32 sub_08004280(u32 year);
s32 sub_080042D8(u32 year, u32 month, u32 day) {
    s32 days = day - 1;
    u32 i;
    u32 r = year % 4;
    if (year >= 2000) year -= 2000;
    days += (year / 4) * 1461;
    days += 1 + r * 365;
    if (year % 4 == 0) days--;
    for (i = 1; i < month; i++) {
        days += gUnk_08198628[i - 1];
        if (i == 2) days += sub_08004280(year + 2000);
    }
    return (days + 6) % 7;
}
