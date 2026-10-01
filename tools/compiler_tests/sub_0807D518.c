/*
 * sub_0807D518 @ 0x0807D518, size 0x60 bytes (code + literal pool + padding)
 * Region: sound driver (0x0807D3D0-0x0807EACF)
 * Flags:  agbcc -mthumb-interwork -O2 -fhex-asm -fprologue-bugfix
 * Status: MATCHES (byte-identical .text vs baserom, relocations resolved)
 *
 * Flag matrix (all with -mthumb-interwork unless noted):
 *   matches: old_agbcc -O2, agbcc -O2 -fprologue-bugfix, agbcc -O2
 *   differs: old_agbcc -O1, agbcc -O1 -fprologue-bugfix, agbcc -O1, old_agbcc -O2 (no -mthumb-interwork)
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
typedef volatile u16 vu16;
typedef volatile u32 vu32;
#define REG_SOUND3CNT_L (*(vu16 *)0x04000070)
#define REG_WAVE_RAM0 (*(vu32 *)0x04000090)
#define REG_WAVE_RAM1 (*(vu32 *)0x04000094)
#define REG_WAVE_RAM2 (*(vu32 *)0x04000098)
#define REG_WAVE_RAM3 (*(vu32 *)0x0400009C)
extern const u32 gUnk_08139550[][4];
void sub_0807D518(struct Unk03005210 *p, u32 a, u32 b) {
    const u32 *src = gUnk_08139550[a * 16 + b];
    u16 bank;
    REG_WAVE_RAM0 = src[0]; REG_WAVE_RAM1 = src[1]; REG_WAVE_RAM2 = src[2]; REG_WAVE_RAM3 = src[3];
    bank = 0;
    if (!(p->unk188 & 0x200))
        bank = 0x40;
    p->unk188 ^= 0x200;
    REG_SOUND3CNT_L = bank | 0x80;
}
