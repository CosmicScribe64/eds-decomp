/*
 * sub_0807B114 @ 0x0807B114, size 0x3C bytes (code + literal pool + padding)
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
struct Ramp { u8 state; s16 cur; s16 target; s16 step; };
void sub_0807B114(struct Ramp *r) {
    if (r->state == 1) {
        r->cur += r->step;
        if (r->step > 0) {
            if (r->cur >= r->target) { r->state = 2; r->cur = r->target; }
        } else {
            if (r->cur <= r->target) { r->state = 2; r->cur = r->target; }
        }
    }
}
