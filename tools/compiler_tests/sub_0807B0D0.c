/*
 * sub_0807B0D0 @ 0x0807B0D0, size 0x1C bytes (code + literal pool + padding)
 * Region: game code
 * Flags:  old_agbcc -mthumb-interwork -O2 -fhex-asm
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, old_agbcc -O1, agbcc -O1 -fprologue-bugfix, old_agbcc -O2 (no -mthumb-interwork)
 *   differs: agbcc -O2, agbcc -O1
 * Pipeline: cpp -nostdinc -undef -I/opt/agbcc/include | <cc> <flags> ; append ".text; .align 2, 0" ; arm-none-eabi-as -mcpu=arm7tdmi
 */
typedef unsigned char u8; typedef unsigned short u16; typedef unsigned int u32;
typedef signed char s8; typedef signed short s16; typedef signed int s32;
struct Timer { u8 state; u16 timer; };
struct Ramp { u8 state; s16 cur; s16 target; s16 step; };
void sub_0807B0D0(struct Timer *t) {
    if (t->state == 1) {
        if (--t->timer == 0)
            t->state = 2;
    }
}
