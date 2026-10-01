/*
 * sub_08000228 @ 0x08000228, size 0x48 bytes (code + literal pool + padding)
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
struct Unk84 { u32 id; u8 a[0x40]; u8 b[0x40]; };
extern const struct Unk84 gUnk_08139F64[];
const void *sub_08000228(u32 id, u16 flag) {
    u32 i;
    for (i = 1; i < 0x1c; i++) {
        if (gUnk_08139F64[i].id == id) {
            if (flag) return gUnk_08139F64[i].a;
            else return gUnk_08139F64[i].b;
        }
    }
    return gUnk_08139F64[0].a;
}
