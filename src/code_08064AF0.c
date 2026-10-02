#include "global.h"
#include "gba.h"

extern s32 __modsi3(s32 a, s32 b);
extern u16 sub_08075A6C(u32 a);
extern u16 sub_08075AE4(u32 a);
extern void sub_08064AF0(u32 a);
extern u16 sub_08068D1C(u32 a, u32 b, u32 c);
extern void sub_080656B4(u8 kind, u8 idx, u16 *map, u8 col, u8 row, u8 pal, u16 tile);
extern void sub_080752D0(void *dst, const void *src);
extern void sub_08079340(void *str, void *dst, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern u16 sub_08065034_u16(void) asm("sub_08065034");

/* Scene state at 0x0201DB20; only the byte at +0x1C34 is used here. */
struct PageState {
    u8 pad0[0x620];
    u16 arr620[15];                 /* +0x620 */
    u16 scroll;                     /* +0x63E */
    u8 pad640[0x14A0 - 0x640];
    u8 arr14A0[0x1C1C - 0x14A0];    /* +0x14A0 */
    u8 cursor;                      /* +0x1C1C */
    u8 pad1C1D[0x1C34 - 0x1C1D];
    u8 page : 1;                    /* +0x1C34 bit 0: which of two pages is shown */
    u8 off : 4;                     /* bits 1-4: rotation offset 0..6 */
    u8 rest : 3;
    u8 pad1C35[0x1C3B - 0x1C35];
    u8 frameKind;                   /* +0x1C3B card frame graphic index (0..9) */
};
extern struct PageState gUnk_0201DB20;

/* Pack-list slide state at 0x02020310 (see code_08063A28). */
struct Slide {
    s32 state;                      /* +0x00 step of the scene runner */
    s32 unk4;
    s32 unk8;
    s32 current;                    /* +0x0C */
    s32 sel;                        /* +0x10 selected list index */
    s32 sel2;                       /* +0x14 candidate index after a key press */
    u8 flags;                       /* +0x18 bit 0: VRAM copy pending */
    u8 pad19;
    u16 unk1A;
    u16 unk1C;
    u16 pad1E;
    s32 pos;                        /* +0x20 */
    s32 posBase;                    /* +0x24 */
    u16 frame;                      /* +0x28 */
    u16 dir;                        /* +0x2A */
    u16 list[0x20];                 /* +0x2C */
    u16 count;                      /* +0x6C */
};
extern struct Slide gUnk_02020310;
struct PackInfo {
    u16 id;
    u8 pad2[2];
    const u8 *image;
    u8 name[0x40];
};
extern struct PackInfo gUnk_080865DC[];
extern const u16 gUnk_080865CC[];
struct Main {
    u8 pad0[6];
    u16 keysNew;                    /* +0x06 */
    u8 pad8[0x41C - 8];
    u16 bgMap[8][0x400];            /* +0x41C BG map buffers */
    u8 pad441C[0x442A - 0x441C];
    u16 unk442A;                    /* +0x442A */
};
extern struct Main gUnk_03000040;
extern void sub_08064698(void);
extern void sub_08064290(void);
extern void sub_080647A4(u32 a, u32 b, u32 c);
extern void sub_080649D8(u32 a);
extern void sub_08064908(u32 a);
extern void sub_08064928(u32 a, u32 b);
extern void sub_08064984(u32 a, u32 b, u32 c);
extern void sub_08077AEC(u32 a);

void sub_08064AF0(u32 a) {}
u16 sub_08064AF4(void)
{
    /* Preserve the initialized scene base across the state-handler calls. */
    register struct Slide *s asm("r4") = &gUnk_02020310;
    u32 f = 1 & s->flags;
    if (f != 0) {
        sub_08064698();
    done:
        return 0;
    }
    switch (s->state) {
    case 0:
        s->current = 1;
        s->sel = s->count - 1;
        sub_08064290();
        goto next;
    case 1:
        sub_080647A4(2, 0x11, 0x1FD);
        sub_080649D8(s->sel);
        goto next;
    case 2:
        REG_DISPCNT |= 0x1440;
        sub_08064AF0(0);
        if (sub_08075AE4(2) == 0)
            goto done;
        sub_08064908(0);
        s->unk1A = f;
        s->unk1C = 8;
        REG_DISPCNT |= 0xB00;
    next:
        s->state++;
        goto done;
    default:
        sub_08064AF0(0);
        return 1;
    }
}
/* Pack-list input: slide animation (frame counter, eased with gUnk_080865CC) or LEFT/RIGHT to move the list, A to confirm. */
u16 sub_08064BA0(void)
{
    vu16 zero;
    sub_08064AF0(gUnk_02020310.frame);
    if ((1 & gUnk_02020310.flags) != 0) {
        REG_DISPCNT |= 0x900;
        sub_08064698();
        return 0;
    }
    if (gUnk_02020310.unk1A != gUnk_02020310.unk1C) {
        if (gUnk_02020310.unk1A > gUnk_02020310.unk1C)
            gUnk_02020310.unk1A--;
        else
            gUnk_02020310.unk1A++;
        sub_08064908(gUnk_02020310.unk1A);
    }
    if (gUnk_02020310.frame != 0) {
        s32 t = gUnk_02020310.pos - gUnk_02020310.posBase;
        t *= gUnk_080865CC[--gUnk_02020310.frame];
        t /= 4096;
        t += gUnk_02020310.posBase;
        gUnk_03000040.unk442A = t;
        REG_DISPCNT &= 0xFEFF;
        REG_DISPCNT &= 0xF7FF;
        if (gUnk_02020310.frame == 0) {
            gUnk_02020310.sel = gUnk_02020310.sel2;
            sub_080649D8(gUnk_02020310.sel);
            gUnk_02020310.unk1C = 8;
            gUnk_02020310.dir = 0;
            gUnk_02020310.pos = 0;
            gUnk_02020310.posBase = 0;
            gUnk_03000040.unk442A = 0;
        }
        return 0;
    }
    if (gUnk_03000040.keysNew & 0x20) {
        vu32 *dma;
        s32 sel;
        s32 count;
        sub_08077AEC(0);
        count = gUnk_02020310.count;
        sel = gUnk_02020310.sel;
        gUnk_02020310.sel2 = (sel + count - 1) % count;
        gUnk_02020310.unk1C = 0x10;
        gUnk_02020310.frame = 8;
        gUnk_02020310.dir = 1;
        gUnk_02020310.pos = gUnk_02020310.posBase - 0x50;
        sub_08064928(3, gUnk_080865DC[gUnk_02020310.list[(sel + gUnk_02020310.count - 1) % gUnk_02020310.count]].id);
        zero = 0;
        dma = (vu32 *)0x040000D4;
        dma[0] = (u32)&zero;
        dma[1] = (u32)gUnk_03000040.bgMap[2];
        dma[2] = 0x81000400;
        dma[2];
        while (dma[2] & 0x80000000)
            ;
        sub_08064984(2, 0x78, 3);
        {
            /* A u32 temporary (not |= on the u8 field) gives the ROM's register choice, as in sub_080649D8. */
            u32 f = gUnk_02020310.flags;
            f |= 1;
            gUnk_02020310.flags = f;
        }
    }
    if (gUnk_03000040.keysNew & 0x10) {
        vu32 *dma;
        s32 sel;
        sub_08077AEC(0);
        sel = gUnk_02020310.sel;
        gUnk_02020310.sel2 = (sel + 1) % gUnk_02020310.count;
        gUnk_02020310.unk1C = 0x10;
        gUnk_02020310.frame = 8;
        gUnk_02020310.dir = 2;
        gUnk_02020310.pos = gUnk_02020310.posBase + 0x50;
        sub_08064928(3, gUnk_080865DC[gUnk_02020310.list[(sel + 3) % gUnk_02020310.count]].id);
        zero = 0;
        dma = (vu32 *)0x040000D4;
        dma[0] = (u32)&zero;
        dma[1] = (u32)gUnk_03000040.bgMap[2];
        dma[2] = 0x81000400;
        dma[2];
        while (dma[2] & 0x80000000)
            ;
        sub_08064984(2, 0x60, 3);
        {
            /* A u32 temporary (not |= on the u8 field) gives the ROM's register choice, as in sub_080649D8. */
            u32 f = gUnk_02020310.flags;
            f |= 1;
            gUnk_02020310.flags = f;
        }
    }
    if (gUnk_03000040.keysNew & 1) {
        sub_08077AEC(1);
        return 1;
    }
    return 0;
}
u16 sub_08064DF8(void)
{
    if (sub_08075A6C(4) != 0) {
        REG_DISPCNT &= 0xEEFF;
        return 1;
    }
    sub_08064AF0(0);
    return 0;
}
/* Fill a 9 x 10 block of the BG map at 0x0600F000 with ascending tiles starting at c*90 + e.
   d == 0: rows wrap (& 0x1F), columns run on; d == 1: both wrap. */
void sub_08064E28(u8 a, u8 b, u8 c, u8 d, u8 e)
{
    u16 tile = c * 0x5A + e;
    u8 i;
    u8 j;
    switch (d) {
    case 0:
        for (i = 0; i < 10; i++) {
            u16 *p = &((u16 *)0x0600F000)[(a & 0x1F) + (((s8)b + i) & 0x1F) * 32];
            for (j = 0; j < 9; j++)
                *p++ = tile++;
        }
        break;
    case 1:
        for (i = 0; i < 10; i++) {
            for (j = 0; j < 9; j++) {
                int n = ((s8)a + j) & 0x1F;
                n += (((s8)b + i) & 0x1F) * 32;
                ((u16 *)0x0600F000)[n] = tile++;
            }
        }
        break;
    }
}
extern void sub_080788AC(u32 dst, u32 ch, u32 a, u32 b, void *flags);
/* Clear 0x400 bytes at dst with DMA3, then render the 0xE0 glyphs 0x20..0xFF (0x20 bytes each) after it:
   codes 0x20-0x7F as-is, 0x80-0xBF from 0xA0 with flags bit 0 set, 0xC0-0xFF from 0xA0 with it cleared. */
struct Flag0 {
    u8 bit0 : 1;
    u8 rest : 7;
};
void sub_08064F00(u32 dst, u8 a, u8 b, struct Flag0 *flags)
{
    vu16 zero = 0;
    vu32 *dma = (vu32 *)0x040000D4;
    u8 ch;
    u16 i;

    dma[0] = (u32)&zero;
    dma[1] = dst;
    dma[2] = 0x81000200;
    dma[2];
    dst += 0x400;
    ch = 0x20;
    i = 0x20;
    do {
        switch (i) {
        case 0x80:
            ch = 0xA0;
            flags->bit0 = 1;
            break;
        case 0xC0:
            ch = 0xA0;
            flags->bit0 = 0;
            break;
        }
        sub_080788AC(dst, ch++, a, b, flags);
        dst += 0x20;
        i++;
    } while (i <= 0xFF);
}
/* Returns the VRAM address of the current slot: 0x06006180 + ((off + slot) % 7) * 0x2A0. */
u32 sub_08064F90(u8 slot)
{
    s32 n = __modsi3(gUnk_0201DB20.off + slot, 7);
    return 0x06006180 + n * 0x2A0;
}
u32 sub_08064FCC(void)
{
    return 0x06000000 + (gUnk_0201DB20.page * 0x32 + 0x19B) * 32;
}
u16 sub_08064FF8(u8 slot)
{
    s32 n = __modsi3(gUnk_0201DB20.off + slot, 7);
    return n * 21 + 0x30C;
}
u32 sub_08065034(void)
{
    return gUnk_0201DB20.page * 0x32 + 0x19B;
}
void sub_08065058(u8 dir)
{
    switch (dir) {
    case 1: {
        u8 n = (gUnk_0201DB20.off - 1) & 0xF;
        gUnk_0201DB20.off = n;
        dir = n; /* FAKEMATCH: reuse the dead direction argument. */
        if (dir == 0xF)
            gUnk_0201DB20.off = 6;
        break;
    }
    case 2: {
        u8 n = (gUnk_0201DB20.off + 1) & 0xF;
        gUnk_0201DB20.off = n;
        if (n == 7)
            gUnk_0201DB20.off = 0;
        break;
    }
    }
} /* 0x08065058 size 0x7C */
/* FAKEMATCH (decomp-permuter): the ROM stores the toggled bit twice; only a non-void return type with no
   return statement reproduces the extra mask. */
int sub_080650D4(void)
{
    int next = gUnk_0201DB20.page + 1;
    int stored = (gUnk_0201DB20.page = next);
    gUnk_0201DB20.page = stored;
}

extern void sub_08079404(void *src, void *dst, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f);
extern const u8 gUnk_0822C720[];
/* Draw one 0x40-byte graphic from the table at 0x0822C720 into the map buffer `map` at (col + 4, row + 1). */
void sub_08065108(u16 idx, u8 *map, u16 col, u16 row, u32 unused, u8 slot)
{
    u32 a = sub_08064F90(slot);
    u16 b = sub_08064FF8(slot);
    sub_08079404((void *)(gUnk_0822C720 + idx * 64), map + (((col + 4) & 0x1F) + ((row + 1) & 0x1F) * 32) * 2, a, b, 2, 1, 0, 0);
}
extern int sub_080650D4(void);
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_KIND(id) ((int)((CARD_STATS(id) & 0x1F00000) >> 20))
#define CARD_NUM(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

/* Draw a card-sized graphic (table 0x0822C720, 0x40 bytes per id) and pick the frame kind byte (+0x1C3B) for card `id`. */
void sub_0806518C(u16 id, u8 *map, u16 col, u16 row)
{
    char buf[0x80];
    const u8 *gfx = gUnk_0822C720 + id * 64;
    u32 a;
    u16 b;
    int v;
    sub_080752D0(buf, gfx);
    a = sub_08064FCC();
    b = sub_08065034_u16();
    sub_08079340((void *)gfx, map + (((col + 4) & 0x1F) + ((row + 1) & 0x1F) * 32) * 2, a, b, 2, 1, 0, 0);
    sub_080650D4();
    switch (CARD_NUM(id)) {
    case 0x776:
        gUnk_0201DB20.frameKind = 3;
        return;
    case 0x777:
    case 0x778:
        gUnk_0201DB20.frameKind = 1;
        return;
    case 0x76D:
    case 0x76E:
    case 0x76F:
        gUnk_0201DB20.frameKind = 0;
        return;
    default: {
        switch (CARD_KIND(id)) {
        case 0x15:
            gUnk_0201DB20.frameKind = 5;
            return;
        case 0x16:
            gUnk_0201DB20.frameKind = 4;
            return;
        }
        switch (CARD_NUM(id)) {
        case 0x776:
            v = 3;
            break;
        case 0x777:
        case 0x778:
            v = 1;
            break;
        default:
            switch (CARD_KIND(id)) {
            case 0x16:
                v = 7;
                break;
            case 0x15:
                v = 8;
                break;
            case 0x17:
                v = 9;
                break;
            default:
                v = (CARD_STATS(id) & 0xC0000) >> 18;
                break;
            }
            break;
        }
        break;
    }
    }
    gUnk_0201DB20.frameKind = v;
}
extern s32 sub_080753CC(const void *s);
extern const u8 gUnk_08087554[];
/* Draw the string gUnk_08087554 (clamped to 100 chars) into the map buffer `map` at (col + 8, row + 3). */
void sub_08065384(u32 unused, u8 *map, u16 col, u16 row)
{
    char buf[0x40];
    u32 a;
    u16 b;
    sub_080752D0(buf, gUnk_08087554);
    if (sub_080753CC(buf) > 0x64)
        buf[0x64] = 0;
    a = sub_08064FCC();
    b = sub_08065034_u16();
    sub_08079340(buf, map + (((col + 8) & 0x1F) + ((row + 3) & 0x1F) * 32) * 2, a, b, 2, 1, 0, 0);
}
extern const u8 gUnk_08704D48[];
extern const u8 gUnk_08704DE8[];
extern const u8 gUnk_08704E88[];
extern const u8 gUnk_08704FA8[];
extern const u8 gUnk_08705048[];
extern const u8 gUnk_08705188[];
extern const u8 gUnk_087052C8[];
extern const u8 gUnk_08705408[];
extern const u8 gUnk_08705628[];
extern const u8 gUnk_087056C8[];
extern const u8 gUnk_08705768[];
extern const u8 gUnk_08705808[];
extern const u8 gUnk_087058A8[];
extern const u8 gUnk_08705948[];
extern const u8 gUnk_087059E8[];
extern const u8 gUnk_08705A88[];
extern const u8 gUnk_08705B28[];
extern const u8 gUnk_08705BC8[];
extern const u8 gUnk_08705C68[];
extern const u8 gUnk_08705D08[];
extern const u8 gUnk_08705DA8[];
extern const u8 gUnk_08705E48[];
extern const u8 gUnk_08705EE8[];
extern const u8 gUnk_08705F88[];
extern const u8 gUnk_08706028[];
extern const u8 gUnk_087060C8[];
extern const u8 gUnk_08706168[];
extern const u8 gUnk_08706208[];
extern const u8 gUnk_087062A8[];
extern const u8 gUnk_08706348[];
extern const u8 gUnk_087063E8[];
extern const u8 gUnk_08706528[];
extern const u8 gUnk_08706668[];
extern const u8 gUnk_087067A8[];
extern const u8 gUnk_087068E8[];
extern const u8 gUnk_08706A28[];
extern const u8 gUnk_08706B68[];
extern const u8 gUnk_08706CA8[];
extern const u8 gUnk_08706DE8[];
/* Copy 40 palette/tile blocks (0x80 bytes each, the last 0x60) of the pack-list graphics (0x0870xxxx) to dst. */
void sub_080653F0(u8 *dst)
{
    CpuSet(gUnk_08706528, dst, 0x40);
    CpuSet(gUnk_08706DE8, dst + 0x80, 0x40);
    CpuSet(gUnk_08706B68, dst + 0x100, 0x40);
    CpuSet(gUnk_08706668, dst + 0x180, 0x40);
    CpuSet(gUnk_087063E8, dst + 0x200, 0x40);
    CpuSet(gUnk_087068E8, dst + 0x280, 0x40);
    CpuSet(gUnk_08706A28, dst + 0x300, 0x40);
    CpuSet(gUnk_08706CA8, dst + 0x380, 0x40);
    CpuSet(gUnk_087067A8, dst + 0x400, 0x40);
    CpuSet(gUnk_08705808, dst + 0x480, 0x40);
    CpuSet(gUnk_08706348, dst + 0x500, 0x40);
    CpuSet(gUnk_08705768, dst + 0x580, 0x40);
    CpuSet(gUnk_087059E8, dst + 0x600, 0x40);
    CpuSet(gUnk_08705B28, dst + 0x680, 0x40);
    CpuSet(gUnk_087058A8, dst + 0x700, 0x40);
    CpuSet(gUnk_08705D08, dst + 0x780, 0x40);
    CpuSet(gUnk_08706028, dst + 0x800, 0x40);
    CpuSet(gUnk_08705E48, dst + 0x880, 0x40);
    CpuSet(gUnk_08705DA8, dst + 0x900, 0x40);
    CpuSet(gUnk_08705C68, dst + 0x980, 0x40);
    CpuSet(gUnk_08705A88, dst + 0xA00, 0x40);
    CpuSet(gUnk_08706168, dst + 0xA80, 0x40);
    CpuSet(gUnk_08705F88, dst + 0xB00, 0x40);
    CpuSet(gUnk_087060C8, dst + 0xB80, 0x40);
    CpuSet(gUnk_087062A8, dst + 0xC00, 0x40);
    CpuSet(gUnk_08706208, dst + 0xC80, 0x40);
    CpuSet(gUnk_08705EE8, dst + 0xD00, 0x40);
    CpuSet(gUnk_08705BC8, dst + 0xD80, 0x40);
    CpuSet(gUnk_08705948, dst + 0xE00, 0x40);
    CpuSet(gUnk_08705048, dst + 0xE80, 0x40);
    CpuSet(gUnk_087052C8, dst + 0xF00, 0x40);
    CpuSet(gUnk_08705628, dst + 0xF80, 0x40);
    CpuSet(gUnk_08705188, dst + 0x1000, 0x40);
    CpuSet(gUnk_087056C8, dst + 0x1080, 0x40);
    CpuSet(gUnk_08705408, dst + 0x1100, 0x40);
    CpuSet(gUnk_08704DE8, dst + 0x1180, 0x40);
    CpuSet(gUnk_08704FA8, dst + 0x1200, 0x40);
    CpuSet(gUnk_08704D48, dst + 0x1280, 0x40);
    CpuSet(gUnk_08704E88, dst + 0x1300, 0x30);
}
extern const u16 gUnk_0808733C[], gUnk_08087352[], gUnk_0808737C[], gUnk_0808738A[];
extern const u32 *const gUnk_08087394[], *const gUnk_080873C0[], *const gUnk_08087424[], *const gUnk_08087440[];
/* kind is 0..3 at every ROM call site, selecting one of the four table pairs.
   Draw one 2x2-cell metatile `idx` at (col, row) of the BG map `map` (tile ids from a per-`kind` table plus
   `tile`, palette bank `pal`) and load its 16-colour palette into bank `pal`. */
void sub_080656B4(u8 kind, u8 idx, u16 *map, u8 col, u8 row, u8 pal, u16 tile)
{
    const u16 *tiles;
    const u32 *const *pals;
    if (idx != 0) {
        const u16 *t;
        u16 *p0;
        int x0, y0, x1, y1;
        switch (kind) {
        case 0:
            tiles = gUnk_0808733C;
            pals = gUnk_08087394;
            break;
        case 1:
            tiles = gUnk_08087352;
            pals = gUnk_080873C0;
            break;
        case 2:
            tiles = gUnk_0808737C;
            pals = gUnk_08087424;
            break;
        case 3:
            tiles = gUnk_0808738A;
            pals = gUnk_08087440;
            break;
        }
        x0 = col & 0x1F;
        y0 = (row & 0x1F) * 32;
        p0 = &map[x0 + y0];
        t = &tiles[idx];
        *p0 = ((*t + tile) & 0x3FF) | pal << 12;
        x1 = (col + 1) & 0x1F;
        map[x1 + y0] = ((*t + tile + 1) & 0x3FF) | pal << 12;
        y1 = ((row + 1) & 0x1F) * 32;
        map[x0 + y1] = ((*t + tile + 2) & 0x3FF) | pal << 12;
        map[x1 + y1] = ((*t + tile + 3) & 0x3FF) | pal << 12;
        CpuSet(pals[idx], (void *)(0x05000000 + pal * 32), 0x10);
    }
}

#define ROW(pos) (((((pos) + 7) * 8 + gUnk_0201DB20.scroll) & 0xFF) >> 3)
/* Row macro for the default case: masks with the local `mask` (0xFF) instead of a literal (see below). */
#define ROW_M(pos) (((((pos) + 7) * 8 + gUnk_0201DB20.scroll) & mask) >> 3)
/* Draw the three-part card header (attribute/type/level icons) for the list entry `pos` of the card view. */
void sub_080657F8(u16 pos)
{
    u16 id;
    int v;
    id = sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]);
    switch (CARD_KIND(id)) {
    case 0x15:
        sub_080656B4(0, 9, (u16 *)0x0600C000, 4, ROW(pos), 6, 0x100);
        switch (CARD_KIND(id)) {
        case 0x15:
        case 0x16:
            v = (CARD_STATS(id) & 0xE0000) >> 17;
            break;
        default:
            v = 0;
            break;
        }
        sub_080656B4(2, v, (u16 *)0x0600C000, 6, ROW(pos), 7, 0x100);
        break;
    case 0x16:
        sub_080656B4(0, 8, (u16 *)0x0600C000, 4, ROW(pos), 6, 0x100);
        switch (CARD_KIND(id)) {
        case 0x15:
        case 0x16:
            v = (CARD_STATS(id) & 0xE0000) >> 17;
            break;
        default:
            v = 0;
            break;
        }
        sub_080656B4(2, v, (u16 *)0x0600C000, 6, ROW(pos), 7, 0x100);
        break;
    case 0x17:
        break;
    case 0x18:
        sub_080656B4(0, 10, (u16 *)0x0600C000, 4, ROW(pos), 6, 0x100);
        break;
    default: {
        /* FAKEMATCH: with a literal 0xFF, CSE shares one 0xFF pseudo across the first two calls and local-alloc
           gives it sl, spilling pos+7; a block-scope mask variable starts its life earlier, so it loses sl to
           pos+7 and is rematerialised as `movs r3, #0xFF` at each use, as in the ROM. */
        int mask = 0xFF;
        int w;
        sub_080656B4(0, CARD_STATS(id) >> 29, (u16 *)0x0600C000, 4, ROW_M(pos), 6, 0x100);
        sub_080656B4(1, CARD_KIND(id), (u16 *)0x0600C000, 6, ROW_M(pos), 7, 0x100);
        switch (CARD_NUM(id)) {
        case 0x776:
            w = 3;
            break;
        case 0x777:
        case 0x778:
            w = 1;
            break;
        default:
            switch (CARD_KIND(id)) {
            case 0x16:
                w = 7;
                break;
            case 0x15:
                w = 8;
                break;
            case 0x17:
                w = 9;
                break;
            default:
                w = (CARD_STATS(id) & 0xC0000) >> 18;
                break;
            }
            break;
        }
        /* FAKEMATCH: the r1 clobber makes `w` conflict with r1, so global-alloc puts it in r0 (copied to r1 for
           the call) as in the ROM instead of taking the r1 copy preference. */
        asm volatile("" ::: "r1");
        sub_080656B4(3, w, (u16 *)0x0600C000, 8, ROW(pos), 1, 0x100);
        break;
    }
    }
}
extern void sub_080794E0(u16 val, u8 n, u8 mode, u16 *dst, u8 col, u8 row, u8 pal, u16 base, u8 m2);
extern void sub_080792A0(u16 start, u16 *dst, u8 pal, u8 mode, u8 count);
extern void *memset(void *dst, int c, unsigned int n);
extern void *memcpy(void *dst, const void *src, unsigned int n);
extern const u8 gUnk_08087568[4];
static inline u16 CardAtk5AB4(u16 id)
{
    switch ((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return ((((const u32 *)0x08621DE0)[id & 0x7FF] << 14) >> 23) * 10;
    }
}
static inline u16 CardDef5AB4(u16 id)
{
    switch ((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    default:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1FF) * 10;
    }
}
#if 0 /* NONMATCHING: 282 lines; first 37% exact. Divine digit loops: target hoists ((row+1)<<5)+1 and keeps row<<5 on the stack */
/* Draw the selected card's ATK/DEF into the tilemap at (col, row); the Divine cards (type 24) get fixed
 * digit patterns by card number instead (hypothesis). */
void sub_08065AB4(u16 *map, u16 col, u16 row)
{
    u8 d[4];
    u16 id;
    u8 i;

    id = sub_08068D1C(gUnk_0201DB20.cursor, gUnk_0201DB20.arr14A0[gUnk_0201DB20.cursor], gUnk_0201DB20.arr620[gUnk_0201DB20.cursor]);
    switch (CARD_KIND(id)) {
    case 21:
    case 22:
    case 23:
        break;
    case 24:
        map[col + (row << 5)] = 0x198;
        for (i = 0; i <= 1; i++) {
            int r = ((row + i) & 0x1F) << 5;
            map[((col + 1) & 0x1F) + r] = 0x2258;
            map[((col + 2) & 0x1F) + r] = 0x2230;
            map[((col + 3) & 0x1F) + r] = 0x2230;
            map[((col + 4) & 0x1F) + r] = 0x2230;
        }
        map[col + (((row + 1) & 0x1F) << 5)] = 0x199;
        switch (CARD_NUM(id)) {
        case 0x776:
            {
                u8 *p = d;
                memset(p, 0, 4);
                *p = 4;
            }
            for (i = 0; i <= 3; i++) {
                sub_080792A0(0x300 | d[i], &map[col + (row << 5) + 1], 2, 0, 1);
                sub_080792A0(0x300 | d[i], &map[col++ + (((row + 1) << 5) + 1)], 2, 0, 1);
            }
            break;
        case 0x777:
            {
                u8 *p = d;
                memset(p, 0, 4);
                *p = 10;
            }
            for (i = 0; i <= 3; i++) {
                sub_080792A0(0x300 | d[i], &map[col + (row << 5) + 1], 2, 0, 1);
                sub_080792A0(0x300 | d[i], &map[col++ + (((row + 1) << 5) + 1)], 2, 0, 1);
            }
            break;
        case 0x778:
            memcpy(d, gUnk_08087568, 4);
            for (i = 0; i <= 3; i++) {
                sub_080792A0(0x300 | d[i], &map[col + (row << 5) + 1], 2, 0, 1);
                sub_080792A0(0x300 | d[i], &map[col++ + (((row + 1) << 5) + 1)], 2, 0, 1);
            }
            break;
        }
        break;
    default:
        map[col + (row << 5)] = 0x198;
        map[col + (((row + 1) & 0x1F) << 5)] = 0x199;
        sub_080794E0(CardAtk5AB4(id), 4, 1, map, (col + 4) & 0x1F, row, 2, 0x300, 0);
        sub_080794E0(CardDef5AB4(id), 4, 1, map, (col + 4) & 0x1F, (row + 1) & 0x1F, 2, 0x300, 0);
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08064AF0", sub_08065AB4); /* 0x08065AB4 size 0x3B8 */
