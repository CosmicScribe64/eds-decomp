/*
 * sub_08004358 @ 0x08004358, size 0x13C bytes (code + literal pool + padding)
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
s32 sub_080042D8(u32 year, u32 month, u32 day);
u32 sub_08004358(u32 year, u32 month, u32 day) {
    u32 flags = 0;
    switch (month) {
    case 1:
        if (day == 1) flags |= 1;
        sub_080042D8(year, month, 1);
        sub_080042D8(year, month, day);
        if ((day - 1) / 7 == 1 && sub_080042D8(year, month, day) == 1)
            flags |= 0x800;
        break;
    case 2:
        if (day == 11) flags |= 2;
        if (day == 24) flags |= 0x2000;
        break;
    case 4:
        if (day == 29) flags |= 4;
        break;
    case 5:
        switch (day) {
        case 3: flags |= 8; break;
        case 4: flags |= 0x10; break;
        case 5: flags |= 0x20; break;
        }
        break;
    case 7:
        if (day == 20) flags |= 0x40;
        if (day == 7) flags |= 0x4000;
        break;
    case 9:
        if (day == 15) flags |= 0x80;
        break;
    case 10:
        sub_080042D8(year, month, 1);
        sub_080042D8(year, month, day);
        if ((day - 1) / 7 == 1 && sub_080042D8(year, month, day) == 1)
            flags |= 0x1000;
        break;
    case 11:
        switch (day) {
        case 3: flags |= 0x100; break;
        case 23: flags |= 0x200; break;
        }
        break;
    case 12:
        if (day == 23) flags |= 0x400;
        break;
    }
    return flags;
}
