#include "global.h"
#include "gba.h"

/* gMain (0x03000040), only the fields used here. */
struct Main {
    u8 filler0[0x40E];
    u16 vblankFlags;                /* +0x40E */
    u8 filler410[4];
    void (*vblankCallback)(void);   /* +0x414 */
    u8 filler418[0x20E6 - 0x418];
    u16 unk20E6[2];                 /* +0x20E6 */
    u8 filler20EA[0x2140 - 0x20EA];
    u16 unk2140[2];                 /* +0x2140 */
    u8 filler2144[0x4832 - 0x2144];
    u8 brightness : 6;              /* +0x4832 fade level 0..0x1F */
    u8 brightnessFlags : 2;
    u8 filler4833[0x485E - 0x4833];
    u16 frameCounter;               /* +0x485E */
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040

/* Duel screen flags at 0x0201CFB0 (byte 0). */
struct DuelFlags {
    u8 bit0 : 1;
    u8 bit1 : 1;
    u8 bit2 : 1;
    u8 rest : 5;
    u8 pad1[0x824 - 1];
    u32 w824;                   /* cursor player */
    u32 w828;                   /* cursor zone (row) */
    u32 w82C;                   /* cursor zone (column) */
    u8 pad830[0x85C - 0x830];
    void (*cb85C)(void);        /* +0x85C optional per-frame callback */
};
extern struct DuelFlags gUnk_0201CFB0;
struct DuelGlobals {
    u8 pad0[4];
    u16 w4;                     /* +4 */
    u8 pad6[0xD68 - 6];
    u16 wD68;                   /* +0xD68 */
    u8 padD6A[0x1ACC - 0xD6A];
    u8 v1ACC : 4;               /* +0x1ACC */
    u8 v1ACC_hi : 4;
    u8 pad1ACD[0x1B12 - 0x1ACD];
    u8 f1B12_0 : 1;             /* +0x1B12 */
    u8 f1B12_1 : 1;
    u8 f1B12_2 : 3;
    u8 f1B12_5 : 3;
    u8 pad1B13[0x1B2C - 0x1B13];
    u8 f1B2C_0 : 1;             /* +0x1B2C */
    u8 f1B2C_rest : 7;
};
extern struct DuelGlobals gUnk_020192E0;
extern u16 gUnk_03001C5C[];       /* BG tilemap buffer in IWRAM, 32 columns */
extern u8 gUnk_0867BB7C[];
struct DuelCard { u32 id : 12; u32 unk12 : 20; };
struct DuelZone {
    struct DuelCard card;   /* +0 */
    u8 unk4;
    u8 unk5;
    u8 flags6;              /* bit 0 / bit 1 tested by the board renderer, bit 1 = face-down */
    u8 unk7;
    u8 pad8[0xA - 8];
    u16 ids[0x20];          /* +0xA attached-effect card ids */
    u8 types[0x40];         /* +0x4A attached-effect types */
    u16 count;              /* +0x8A attached-effect count */
    u8 pad8C[0x94 - 0x8C];
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
#define ZB3(p, z) ((struct DuelZone *)((p) * 0xD64 + (u32)gUnk_0201930C + (z) * 0x94))
struct ZonePos { s32 x; s32 y; };         /* pixel position of a zone, per player (0x10 entries each) */
extern struct ZonePos gUnk_081A42A4[2][16];
extern int sub_08008940(int player, int zone);
extern int sub_0806226C(u32 id);
extern u32 sub_080623AC(u32 player, u32 a, u32 b);
extern u32 sub_080623EC(u32 player, u32 a, u32 b);
extern void sub_08060DD8_i(s32 x, s32 y) asm("sub_08060DD8");
extern void sub_08060E2C_i(s32 x, s32 y, u32 t) asm("sub_08060E2C");
extern const u32 gUnk_081A427C[];
extern u32 sub_0800A368(u32 player);
extern u32 sub_0806236C(u32 a, u32 b, u32 c);
extern int sub_08062140(u32 id);
extern int sub_0804A528(u32 a, u32 b, u32 c);
extern void sub_080612F4(u32 player, u32 zone, u32 kind);
extern void sub_08060ECC(u32 player, u32 zone);
struct PlayerState {            /* 0xD64 bytes, at 0x020192E4 + player * 0xD64 */
    u8 pad0[2];
    u8 b2;                      /* +2 */
    u8 b3;                      /* +3 */
    u8 b4;                      /* +4 */
    u8 b5;                      /* +5 */
    u8 b6;                      /* +6 */
    u8 pad7[0xB - 7];
    u8 unkB;                    /* +0xB high nibble in the +0x1B2C bitmask */
    u8 unkC;                    /* +0xC bit 0 in the bitmask */
    u8 padD[0x26 - 0xD];
    u16 u26;                    /* +0x26 bitmask */
    u8 pad28[0x904 - 0x28];
    u32 a904[0x280 / 4];        /* +0x904 card words */
    u32 aB84[0x140 / 4];        /* +0xB84 card words */
    u8 aCC4[0x100];             /* +0xCC4 2-byte entries */
};
extern struct PlayerState gUnk_020192E4[2];
struct IntrVectors {
    u32 unk0;
    void (*hblankCallback)(void);
};
extern struct IntrVectors gUnk_03000000;

extern void sub_08073574(void);
extern void sub_0806041C(void);
extern void sub_0806075C(void);
extern void sub_08060578(void);
extern void sub_080731D0(u32 a, u32 b, u32 c, const void *d);
extern void sub_0806044C(u32 a);
extern void sub_08060934(u32 a, u32 b);
extern void sub_08060964(u32 a, u32 b);
extern void sub_080611AC(void);
extern void sub_0805ED9C(void);
extern void sub_0805ED78(void);
extern void sub_080759F4(void);
extern void sub_080757AC(void);
extern void sub_08060400(void);
extern void sub_080757F4(void);
extern u16 sub_08075A6C(u8 step);
extern u16 sub_08075AE4(u8 step);
extern void sub_080761F0(u32 yx, u16 shape, u16 attr2);
extern void sub_08061004(u32 a, u32 b, u32 c);
extern void sub_08061848(u32 x, u32 y, u32 color);
extern void sub_080618C4(u32 yx, u32 palBase, const u16 *src, const void *pal);
extern void sub_08075294(void *dst, const void *src, u32 size);

void sub_080609C4(void)
{
    s32 i;
    /* FAKEMATCH: keep the initialized base tile in r3 during the two stores. */
    register u16 tile asm("r3");
    struct DuelGlobals *g;
    u16 *a;
    u16 *b;

    REG_DISPCNT = 0;
    gMain.vblankFlags = 0x603;
    sub_08073574();
    sub_0806041C();
    sub_0806075C();
    sub_08060578();
    sub_080731D0(0, 0x60, 0x10, gUnk_0867BB7C);
    i = 0;
    a = gMain.unk20E6;
    tile = 0x4280;
    b = gMain.unk2140;
    for (; i <= 1; a++, b++, i++) {
        /* Stage the halfword sum; agbcc combines this into one store. */
        *a = i;
        *a += tile;
        *b = tile + i;
    }
    g = &gUnk_020192E0;
    sub_0806044C(g->v1ACC);
    sub_08060934(0, g->w4);
    sub_08060934(1, g->wD68);
    sub_08060964(g->f1B12_1, g->f1B12_2);
    sub_080611AC();
    sub_0805ED9C();
    sub_0805ED78();
    sub_080759F4();
    sub_080757AC();
    gMain.vblankCallback = sub_08060400;
    gUnk_0201CFB0.bit2 = 1;
}

void sub_08060AAC(u16 arg)
{
    REG_DISPCNT &= 0xE0FF;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    REG_IME = 1;
    REG_IME = 0;
    REG_IE &= 0xFFFD;
    gUnk_03000000.hblankCallback = NULL;
    REG_IME = 1;
    gMain.vblankCallback = NULL;
    if (arg != 0) {
        sub_080759F4();
        gUnk_0201CFB0.bit2 = 0;
        gUnk_0201CFB0.bit1 = 0;
    }
}
u16 sub_08060B2C(void)
{
    REG_DISPCNT |= 0x1F00;
    return sub_08075AE4(4);
}
u32 sub_08060B4C(void)
{
    if (sub_08075A6C(4) != 0) {
        sub_08060AAC(0);
        return 1;
    }
    return 0;
}
u32 sub_08060B6C(s32 step)
{
    REG_BLDCNT = 0x27E7;
    if (gMain.brightness <= 0x1E) {
        gMain.brightness += step;
        if (gMain.brightness > 0x1F)
            gMain.brightness = 0x1F;
    }
    REG_BLDY = gMain.brightness;
    if (gMain.brightness <= 0x1E)
        return 0;
    REG_DISPCNT &= 0xF8FF;
    return 1;
}
u32 sub_08060BF0(s32 step)
{
    REG_BLDCNT = 0x27E7;
    if (gMain.brightness <= 0x8) {
        gMain.brightness += step;
        if (gMain.brightness > 0x9)
            gMain.brightness = 0x9;
    }
    REG_BLDY = gMain.brightness;
    if (gMain.brightness <= 0x8)
        return 0;
    return 1;
}
u32 sub_08060C68(s32 step)
{
    REG_DISPCNT |= 0x700;
    if (gMain.brightness > step)
        gMain.brightness -= step;
    else
        gMain.brightness = 0;
    if (gMain.brightness != 0) {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = 0x27E7;
        return 0;
    }
    sub_080757F4();
    return 1;
}
u32 sub_08060CEC(s32 step)
{
    REG_BLDCNT = 0x27A7;
    if (gMain.brightness <= 0x1E) {
        gMain.brightness += step;
        if (gMain.brightness > 0x1F)
            gMain.brightness = 0x1F;
    }
    REG_BLDY = gMain.brightness;
    if (gMain.brightness <= 0x1E)
        return 0;
    return 1;
}
u32 sub_08060D64(s32 step)
{
    if (gMain.brightness > step)
        gMain.brightness -= step;
    else
        gMain.brightness = 0;
    if (gMain.brightness != 0) {
        REG_BLDY = gMain.brightness;
        REG_BLDCNT = 0x27A7;
        return 0;
    }
    sub_080757F4();
    return 1;
}
/* Clear a 4x4 block of the IWRAM tilemap buffer at (x, y). */
void sub_08060DD8(u16 x, u16 y)
{
    u16 *p = gUnk_03001C5C + (x + (y << 5));

    p[0] = 0;
    p[1] = 0;
    p[2] = 0;
    p[3] = 0;
    p[0x20] = 0;
    p[0x21] = 0;
    p[0x22] = 0;
    p[0x23] = 0;
    p[0x40] = 0;
    p[0x41] = 0;
    p[0x42] = 0;
    p[0x43] = 0;
    p[0x60] = 0;
    p[0x61] = 0;
    p[0x62] = 0;
    p[0x63] = 0;
}
/* Fill a 4x4 block of the tilemap buffer at (x, y) with consecutive tiles starting at t. */
void sub_08060E2C(u32 x, u32 y, u16 t)
{
    u16 *p = gUnk_03001C5C + ((u16)x + ((u16)y << 5));

    p[0x0] = t++;
    p[0x1] = t++;
    p[0x2] = t++;
    p[0x3] = t++;
    p[0x20] = t++;
    p[0x21] = t++;
    p[0x22] = t++;
    p[0x23] = t++;
    p[0x40] = t++;
    p[0x41] = t++;
    p[0x42] = t++;
    p[0x43] = t++;
    p[0x60] = t++;
    p[0x61] = t++;
    p[0x62] = t++;
    p[0x63] = t;
}
/* Redraw the board tile block of zone (player, zone): an empty zone clears it (and draws the 2x2 empty marker for monster zones), otherwise a card back/face tile block. */
void sub_08060ECC(u32 player, u32 zone)
{
    struct DuelZonesPlayer *pp = &gUnk_0201930C[1 & player];
    struct DuelZone *z = &pp->zones[zone];
    u16 id = (*(u32 *)z << 20) >> 20;
    s32 x = gUnk_081A42A4[player][zone].x / 8;
    s32 y = gUnk_081A42A4[player][zone].y / 8;
    u16 t;
    u16 *p;
    u16 xx;
    u16 yy;

    if (id == 0) {
        sub_08060DD8_i(x, y);
        if ((s32)zone <= 4 && sub_08008940(player, zone) == 0) {
            xx = x + 1;
            yy = y + 1;
            p = gUnk_03001C5C + (xx + (yy << 5));
            p[0] = 0x1240;
            p[1] = 0x1241;
            p[0x20] = 0x1242;
            p[0x21] = 0x1243;
        }
    } else {
        t = 0x1070;
        if (z->flags6 & 2)
            t = sub_0806226C(id) + 0x2000;
        if (z->flags6 & 1)
            t += 0x30;
        sub_08060E2C_i(x, y, t);
    }
}


void sub_08060FD0(u32 player, u32 zone)
{
    sub_08060DD8_i(gUnk_081A42A4[player][zone].x / 8, gUnk_081A42A4[player][zone].y / 8);
}
/* Redraw the board tile block for one slot `kind` (0-4 / 5-9 / 10 zone rows, 12-15 special slots) of `player`. */
#if 0 /* NONMATCHING: register allocation differs. The ROM keeps idx in r2, table in r6, x in r5,
       * y in r7 (saves r8); ours moves idx to r6, table to r2, x to r7, y to r5. Tried a zp
       * local, an xy swap and u32 copies. */
void sub_08061004(u32 player, u32 kind, u32 idx)
{
    s32 x = gUnk_081A42A4[player][kind].x / 8;
    s32 y = gUnk_081A42A4[player][kind].y / 8;
    struct PlayerState *ps;
    s16 t;
    s8 v;

    switch (kind) {
    case 0:
    case 5:
    case 10:
        sub_08060ECC(player, kind + idx);
        break;
    case 12:
        v = gUnk_020192E4[1 & player].b5;
        if (v == 0)
            goto clear;
        if (v <= 5)
            goto t1070;
    t1230:
        t = 0x1230;
        goto draw;
    case 13:
        v = gUnk_020192E4[1 & player].b3;
        if (v == 0)
            goto clear;
        if (v > 5)
            goto t1230;
        goto t1070;
    case 14:
        ps = &gUnk_020192E4[1 & player];
        if (ps->b4 == 0)
            goto clear;
        t = sub_0806226C((ps->a904[ps->b4 - 1] << 20) >> 20) + 0x2000;
        goto draw;
    clear:
        sub_08060DD8_i(x, y);
        break;
    case 15:
        ps = &gUnk_020192E4[1 & player];
        if (ps->b6 == 0) {
            break;
            sub_08060DD8_i(x, y);
        }
        t = sub_0806226C((ps->aB84[ps->b6 - 1] << 20) >> 20) + 0x2000;
        if (ps->aCC4[(ps->b6 - 1) * 2] != 2)
            goto draw;
    t1070:
        t = 0x1070;
    draw:
        sub_08060E2C_i(x, y, t);
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080609C4", sub_08061004);
void sub_080611AC(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < 2; i++) {
        for (j = 0; j < 5; j++) {
            sub_08061004(i, 0, j);
            sub_08061004(i, 5, j);
        }
        sub_08061004(i, 10, 0);
        sub_08061004(i, 12, 0);
        sub_08061004(i, 13, 0);
        sub_08061004(i, 14, 0);
        sub_08061004(i, 15, 0);
    }
}
/* Draw two animated 8x8 cursor sprites at the screen positions of two zones (a, b = player | zone << 8); the tile cycles with frameCounter / 8. */
void sub_0806120C(u16 a, u16 b, u16 c)
{
    u8 p1, z1, p2, z2;
    u32 x1, y1, x2, y2;
    u16 t;

    p1 = a;
    z1 = a >> 8;
    x1 = sub_080623AC(p1, 0, z1) + 8;
    y1 = sub_080623EC(p1, 0, z1) + 8;
    p2 = b;
    z2 = b >> 8;
    x2 = sub_080623AC(p2, 0, z2) + 8;
    y2 = sub_080623EC(p2, 0, z2) + 8;
    x1 |= y1 << 16;
    t = gUnk_081A427C[(gMain.frameCounter >> 3) & 7] + c + 0x5400;
    sub_080761F0(x1, 0x40, t);
    x2 |= y2 << 16;
    t = gUnk_081A427C[(gMain.frameCounter >> 3) & 7] + c + 0x5400;
    sub_080761F0(x2, 0x40, t);
}
#if 0 /* NONMATCHING: hand translation from the m2c draft. The semantics follow the ROM (nested
       * switch over attached-effect types), but codegen differs wholesale: no jump tables,
       * player/zone kept in r6/r7 instead of r8/r9, and a different branch tree. Best-effort
       * parked base only. */
void sub_080612F4(u32 player, u32 zone, u32 kind)
{
    s32 sp0;
    s16 e;
    u32 p;
    s32 z;
    struct PlayerState *ps;
    struct DuelZone *zd;
    u16 id;
    u32 type;
    u32 mask;
    u32 a;

    switch (kind) {
    case 0:
    case 10:
block_24:
        e = 0;
loop_32:
        if (e < (s32)ZB(1 & player, zone)->count) {
            zd = ZB(1 & player, zone);
            id = zd->ids[e];
            if ((zd->flags6 & 2) || (player == 0)) {
                type = *((u8 *)zd + 0x4A + e * 2) - 1;
                switch (type) {
                case 0:
                case 4:
                case 9:
                    if (id == (u16)((((player << 24) >> 8) | (zone << 24)) >> 16))
                        sub_0806120C(id, (u16)((((player << 24) >> 8) | (zone << 24)) >> 16), 0x20C);
                    break;
                case 1:
                case 6:
                    if (id == (u16)((((player << 24) >> 8) | (zone << 24)) >> 16))
                        sub_0806120C(id, (u16)((((player << 24) >> 8) | (zone << 24)) >> 16), 0x218);
                    break;
                }
            }
            e++;
            goto loop_32;
        }
        ps = &gUnk_020192E4[1 & player];
        mask = ((ps->unkC & 1) << 4) | (ps->unkB >> 4);
        if ((mask >> zone) & 1) {
            a = (u8)player;
            sub_0806120C(a | 0xF00, (u16)((zone << 8) | a), 0x218);
        }
        return;
    case 5:
        p = 0;
loop_9:
        z = 0;
        sp0 = p + 1;
loop_10:
        zd = ZB(p, z);
        if (((*(u32 *)zd << 20) != 0) && (zd->flags6 & 2)) {
            e = 0;
            if ((s32)zd->count > 0) {
                do {
                    zd = ZB(p, z);
                    id = zd->ids[e];
                    type = *((u8 *)zd + 0x4A + e * 2) - 1;
                    switch (type) {
                    case 0:
                    case 4:
                    case 9:
                        if (id == (u16)((((player << 24) >> 8) | (zone << 24)) >> 16))
                            sub_0806120C(id, (u16)((((p << 24) >> 8) | (z << 24)) >> 16), 0x20C);
                        break;
                    case 1:
                    case 6:
                        if (id == (u16)((((player << 24) >> 8) | (zone << 24)) >> 16))
                            sub_0806120C(id, (u16)((((p << 24) >> 8) | (z << 24)) >> 16), 0x218);
                        break;
                    }
                    e++;
                } while (e < (s32)ZB(p, z)->count);
            }
        }
        z++;
        if (z <= 0xA)
            goto loop_10;
        p = sp0;
        if (p <= 1)
            goto loop_9;
        goto block_24;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080609C4", sub_080612F4); /* 0x080612F4 size 0x28C */
/* Per-frame draw of the duel board markers: zone cursor effects, hand strip cursor sprites, then the optional callback. */
#if 0 /* NONMATCHING: structure matches; ROM multiplies z*0x94 before p*0xD64 (pl computed first), saves only r8/r9 (ours also sl) and hoists g+4 into r9 (105 differing lines) */
void sub_08061580(void)
{
    u32 p;
    u32 z0;
    u32 zn;
    s32 i;
    u16 x;
    u32 y;
    u16 t;
    int pl;
    struct DuelGlobals *g;

    if ((*(u8 *)&gUnk_0201CFB0 & 6) == 6) {
        p = gUnk_0201CFB0.w824;
        z0 = gUnk_0201CFB0.w828;
        zn = z0 + gUnk_0201CFB0.w82C;
        switch (z0) {
        case 0:
            pl = 1 & p;
            if (*(u32 *)ZB(pl, zn) << 20 != 0)
                sub_080612F4(p, zn, 0);
            break;
        case 5:
            sub_080612F4(p, zn, 5);
            break;
        }
        g = &gUnk_020192E0;
        if ((*((u8 *)g + 0x1B12) & 0x1C) == 0xC) {
            for (i = 0; i <= 4; i++) {
                if (sub_0804A528(g->f1B12_1, i, 0) != 0) {
                    if (((*(u16 *)((u8 *)g + 4 + (1 & g->f1B12_1) * 0xD64 + 0x26) >> i) & 1) == 0) {
                        x = sub_080623AC(g->f1B12_1, 0, i);
                        y = sub_080623EC(g->f1B12_1, 0, i);
                        x += 8;
                        y += 8;
                        x |= y << 16;
                        t = gUnk_081A427C[(gMain.frameCounter >> 3) & 7] + 0x5600;
                        sub_080761F0(x, 0x40, t);
                    }
                }
            }
        }
        if (gUnk_0201CFB0.cb85C != NULL)
            gUnk_0201CFB0.cb85C();
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080609C4", sub_08061580);
/* Draw the small card sprites of each player's hand/deck strip (row 0xB), skipping the one under the cursor. */
struct HandRow616 {
    struct DuelCard c[80];
    u8 pad[0xD64 - 80 * 4];
};
extern struct HandRow616 gUnk_02019968[2];
struct PlayerHdr616 {           /* 0xD64 bytes at 0x020192E4 + player * 0xD64 */
    u8 pad0[2];
    u8 handCount;               /* +2 */
    u8 pad3[0xD64 - 3];
};
extern struct PlayerHdr616 gHandHdr616_020192E4[2];
void sub_080616D0(void)
{
    s32 pl;
    u16 sel;
    u32 y;
    u32 x;
    u32 id;
    u16 t;
    s32 n;
    s32 i;
    struct DuelCard *p;

    if ((*(u8 *)&gUnk_0201CFB0 & 6) == 6) {
        for (pl = 0; pl <= 1; pl++) {
            /* Ternary (not if/else): sel then has 5 refs and loses r9 to the hoisted player offset. */
            sel = pl != 0 ? sub_0800A368(pl) : 1;
            y = sub_080623EC(pl, 0xB, 0);
            if (y + 0x20 <= 0xBF) {
                n = gHandHdr616_020192E4[1 & pl].handCount;
                for (i = 0; i < n; i++) {
                    /* Two statements so fold keeps (base + player offset) + i * 4. */
                    p = gUnk_02019968[1 & pl].c;
                    p += i;
                    x = sub_0806236C(pl, i, n);
                    id = p->id;
                    t = sel ? sub_08062140(id) + 0x1000 : 0x40;
                    if (id != 0) {
                        /* Through a cast pointer the flag stays gUnk_020192E0 + 0x1B2C (two literals);
                           a plain gUnk_020192E0.f1B2C_0 folds into one 0x0201AE0C literal. */
                        if (!(((struct DuelGlobals *)&gUnk_020192E0)->f1B2C_0 && gUnk_0201CFB0.w824 == pl && gUnk_0201CFB0.w828 == 0xB
                              && gUnk_0201CFB0.w82C == i))
                            sub_080761F0((y << 16) | x, 0x80, t + 0x400);
                    }
                }
            }
        }
    }
}
void sub_080617F0(void)
{
}
void sub_080617F4(void)
{
}
void sub_080617F8(void)
{
    u16 t = 0x324;

    if (gMain.frameCounter & 0x20)
        t += 0x20;
    if ((*(u8 *)&gUnk_0201CFB0 & 6) == 6) {
        sub_080761F0(0x00400058, 0x40C0, t |= 0x6000);
    }
}
/* Plot one pixel of colour `color` at (x, y) into 8bpp OBJ tile data at 0x06010000 (tile rows of 64 bytes, 2 tiles per 8 pixels of x, starting two rows down). */
void sub_08061848(u32 x, u32 y, u32 color)
{
    /* FAKEMATCH: retain the ROM's tile-column/address register. */
    register u32 tx __asm__("r4") = (x << 13) >> 16;
    u32 ty = (y << 13) >> 16;
    u32 xl = (x & 7) << 16;
    u32 yl = y & 7;
    u32 *p;
    u32 w;
    u8 buf[4];
    u32 px;
    u8 *bp;

    tx = (u16)(tx << 1);
    tx = (tx + ((u16)(ty + 2) << 5)) << 5;
    p = (u32 *)(0x06010000 + tx);
    p = (u32 *)((u32)p + ((xl >> 18) << 2));
    p = (u32 *)((u32)p + (yl << 3));
    w = *p;
    buf[0] = w;
    buf[1] = (u16)w >> 8;
    bp = buf;
    w >>= 16;
    bp[2] = w;
    w >>= 8;
    bp[3] = w;
    px = 0x30000;
    px &= xl;
    px >>= 16;
    buf[px] = color;
    *p = buf[0] | (buf[1] << 8) | ((buf[2] | (buf[3] << 8)) << 16);
}
/* Blit an 8x8 4bpp tile (`src`, 16 halfwords, 2 per row) as pixels through sub_08061848 at yx, colours offset by palBase; also loads the 16-colour palette `pal` at the matching OBJ palette slot. */
void sub_080618C4(u32 yx, u32 palArg, const u16 *srcArg, const void *pal)
{
    /* FAKEMATCH: preserve the ROM's source-pointer register before narrowing palette base. */
    register const u16 *src __asm__("r5") = srcArg;
    u16 palBase = palArg;
    u16 i;
    u16 j;
    u16 k;
    u16 w;
    u16 x0 = yx;
    u32 y0 = yx >> 16;

    if (src == 0 || pal == 0)
        return;
    sub_08075294((void *)(0x05000200 + ((palBase >> 4) << 5)), pal, 0x40);
    for (i = 0; i < 8; i++) {
        for (j = 0; j < 2; j++) {
            w = *src;
            for (k = 0, src++; k < 4; k++) {
                u32 c = (w >> (k * 4)) & 0xF;

                if (c != 0)
                    sub_08061848(x0 + k + j * 4, y0 + i, (u8)(c + palBase));
            }
        }
    }
}
/* Blit a 16x16 image made of four 8x8 tiles (0x20 bytes apart) at yx. */
void sub_0806196C(u32 yx, u16 palBase, const u16 *src, const void *pal)
{
    u16 x = yx;
    u32 y = yx >> 16;

    sub_080618C4(x | (y << 16), palBase, src, pal);
    src += 0x10;
    sub_080618C4((x + 8) | (y << 16), palBase, src, pal);
    src += 0x10;
    sub_080618C4(x | ((y + 8) << 16), palBase, src, pal);
    src += 0x10;
    sub_080618C4((x + 8) | ((y + 8) << 16), palBase, src, pal);
}
