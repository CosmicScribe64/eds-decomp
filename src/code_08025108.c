#include "global.h"
#include "gba.h"

/* One token of the toss screen (12 bytes), see code_0802408C. */
struct TossSlot {
    u8 timer;           /* +0x0 */
    u8 frame;           /* +0x1: animation frame / result face */
    u8 state;           /* +0x2: 1 = flying, 2 = landed, 3 = winner */
    u8 unk3;
    s16 y;              /* +0x4 */
    u16 t;              /* +0x6 */
    u8 resultTimer;     /* +0x8 */
    u8 resultFrame;     /* +0x9: 0-4 */
    u8 resultState;     /* +0xA: 1 = animating, 2 = done */
    u8 unkB;
};

/* Sparkle particle (8 bytes). */
struct Sparkle {
    u32 active:1;       /* +0 bit 0 (u32 container: tested with lsl #31) */
    u8 timer:2;         /* +0 bits 1-2 */
    u8 frame:5;         /* +0 bits 3-7: index into gUnk_08081FA4 (0 entry ends) */
    u8 unk1[3];
    u8 x;               /* +4 */
    u8 y;               /* +5 */
    u8 unk6[2];
};

/* Sparkle pool at 0x02015DD0 (= toss work area +0xB50). */
struct Sparkles {
    struct Sparkle p[32];
    u8 next;            /* +0x100: next slot (mod 32) */
};

extern u8 gUnk_02015280[];          /* toss work area (OAM builder argument) */
extern const u16 gUnk_08081FA4[];   /* sparkle tile per frame, 0-terminated */

/* Sprite group: list of 4-halfword OAM templates. */
struct SprGroup {
    u32 unk0;
    u16 *list;          /* +0x4: count entries of 4 halfwords */
    u32 unk8;
    u8 count;           /* +0xC */
    u8 unkD;
    u8 unkE;            /* +0xE */
    u8 unkF;
};
/* Value ramp driven by sub_0807B0EC/sub_0807B100 (init) and sub_0807B114 (tick). */
struct Ramp {
    u8 state;           /* +0: 2 = finished */
    u8 unk1;
    s16 cur;            /* +2 */
    s16 target;         /* +4 */
    s16 unk6;
};

/* Screen work area at 0x0201F820 (0xAF0 bytes, fields used here). */
struct Work1F820 {
    u8 unk0[0x618];
    u8 unk618[0x918 - 0x618];   /* +0x618: sub_0807B534 */
    struct SprGroup grp918;     /* +0x918: die sprite (hypothesis) */
    u8 filler928[4];
    struct SprGroup grp92C;     /* +0x92C */
    u8 filler93C[0xABC - 0x93C];
    u8 unkABC[6];       /* +0xABC: object for sub_0807883C */
    u8 unkAC2;          /* +0xAC2: 2 = finished */
    u8 unkAC3;
    u8 step;            /* +0xAC4: index into gUnk_08199A10 / gUnk_08199A28 */
    u8 unkAC5;
    u8 unkAC6;
    u8 unkAC7;
    u8 unkAC8;
    u8 unkAC9;
    u8 result;          /* +0xACA: copied to 0x02017A30+8 at the end */
    u8 unkACB;
    u8 unkACC;
    u8 unkACD;          /* +0xACD */
    u8 unkACE[2];
    struct Ramp rampAD0;    /* +0xAD0 */
    struct Ramp rampAD8;    /* +0xAD8 */
    struct Ramp rampAE0;    /* +0xAE0 */
    u16 unkAE8;
    u8 unkAEA;          /* +0xAEA: mode 0/1/2 (2 = no sprites) */
    u8 unkAEB;
    u16 unkAEC;
    u16 unkAEE;
};
extern struct Work1F820 gUnk_0201F820;

/* Message/sequence block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 unk0[6];
    u16 opts;           /* +0x6 */
    u16 arg8;           /* +0x8 */
    u8 unkA;
    u8 step;            /* +0xB */
};
extern struct DuelMsg gUnk_02017A30;


/* gMain (0x03000040): only the fields used here. */
struct Main {
    u32 rngState;       /* +0x000 */
    u16 heldKeys;       /* +0x004 */
    u16 newKeys;        /* +0x006 */
    u8 unk8[0x40E - 8];
    u16 unk40E;         /* +0x40E */
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040

extern const u8 gUnk_08082308[6][2][2];
extern const u8 gUnk_080822FC[][4];
void sub_08075278(void *dst, u32 size); /* MemClear16 */
void sub_0807B534(void *p);
void sub_0807B0EC(u32 a, u32 b, u32 c, struct Ramp *ramp);
void sub_0807B114(struct Ramp *ramp);
s32 sub_0807B4D0(s32 a, s32 b);     /* 8.8 fixed-point multiply */
extern const s16 gUnk_08087BA4[];   /* sine table, 256 entries */
struct DieFrame { u8 unk0; u8 count; u16 unk2; u16 *list; };
extern struct DieFrame *const gUnk_081999EC[];
extern const struct DieFrame gUnk_08081FD4[];
extern const u8 gUnk_081999F8[];
extern const u8 gUnk_08199A04[];
extern const u8 gUnk_086A12EC[];
extern const u8 gUnk_086AA8EC[];
extern const u8 gUnk_086B2168[];
extern const u8 gUnk_086B4168[];
extern const u8 gUnk_086B6168[];
void sub_08078670(const void *a, void *b);
void sub_080787F4(u32 a, u32 b, u32 c, void *p);
void sub_08077CEC(const void *src, void *dst, u32 n);
void sub_08077AEC(u32 se);
void sub_0807B100(u32 a, u32 b, u32 c, struct Ramp *ramp);
extern u16 (*const gUnk_08199A10[])(void);
extern u16 (*const gUnk_08199A28[])(void);
extern u16 (*const gUnk_08199A40[])(void);
u16 *sub_0807A320(u8 a, void *work);
void sub_080786D0(void *p);
void sub_0807883C(void *p);
void sub_0807A298(void *p);
void sub_0807A2EC(void *p);
s32 sub_08076F9C(void);             /* random */
u32 *sub_0807B6B8(u32 a, u32 tile, s32 x, s32 y, u32 w, u32 h, u32 a6, u32 a7, u32 a8,
                  u32 a9, u32 a10, u32 a11, void *work);


void sub_08025108(struct TossSlot *s, u8 n)
{
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].resultState == 1) {
            if (s[i].resultTimer == 0) {
                s[i].resultTimer = 6;
                if (++s[i].resultFrame == 5) {
                    s[i].resultFrame = 0;
                    s[i].resultState++;
                }
            } else {
                s[i].resultTimer--;
            }
        }
    }
}

void sub_0802515C(struct TossSlot *s, u8 n, u8 face)
{
    u8 flying = 0;
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].state != 2)
            flying++;
    }
    if (flying == 0) {
        for (i = 0; i < n; i++) {
            if (s[i].frame == face) {
                s[i].resultState = 1;
                s[i].state = 3;
            } else {
                s[i].resultState = 2;
            }
        }
    }
}

u8 sub_080251C8(struct TossSlot *s, u8 n)
{
    u8 count = 0;
    u8 i;

    for (i = 0; i < n; i++) {
        if (s[i].resultState != 2)
            count++;
    }
    return count;
}

u32 sub_08025200(u8 x, u8 y, struct Sparkles *sp)
{
    u8 k = sp->next++ % 32;
    s32 r = sub_08076F9C();
    struct Sparkle *p = &sp->p[k];

    p->x = x + r % 16;
    p->y = y;
    p->active = 1;
    p->timer = 0;
    p->frame = 0;
    return 1;
}

u32 sub_08025258(struct Sparkles *sp)
{
    u8 i;

    for (i = 0; i < 32; i++) {
        struct Sparkle *p = &sp->p[i];
        if (p->active) {
            if (--p->timer == 3) {
                p->timer = 0;
                if (gUnk_08081FA4[++p->frame] == 0)
                    p->active = 0;
            }
        }
    }
}

u32 sub_080252D4(struct Sparkles *sp)
{
    u8 i;

    for (i = 0; i < 32; i++) {
        struct Sparkle *p = &sp->p[i];
        if (p->active) {
            u32 *oam = sub_0807B6B8(1, gUnk_08081FA4[p->frame] + 0x200, p->x, p->y,
                                    16, 16, 4, 1, 0, 0, 0, 0, gUnk_02015280);
            *oam |= 0x400;
        }
    }
}

void sub_08025344(struct Sparkles *sp)
{
    u8 i;

    for (i = 0; i < 32; i++)
        sp->p[i].active = 0;
    sp->next = 0;
}

/* K&R definition: callers pass the u16/u8 arguments without narrowing them. */
u16 *sub_08025374(src, a, x, y, pal, tileBase, work)
    u16 *src;
    u8 a;
    u16 x;
    u16 y;
    u16 pal;
    u16 tileBase;
    void *work;
{
    u16 *oam = sub_0807A320(a, work);
    u16 *ret = oam;

    oam[0] = (src[0] & 0xFF00) | ((y + (src[0] & 0xFF)) & 0xFF);
    oam[1] = (src[1] & 0xFE00) | ((x + (src[1] & 0x1FF)) & 0x1FF);
    oam[2] = (src[2] & 0xFC0F) | (tileBase + ((src[2] & 0xF0) << 1));
    if (src[2] & 0x100)
        oam[2] += 0x10;
    if ((src[2] & 0xF000) == 0)
        oam[2] |= pal << 12;
    return ret;
}

void sub_08025430(g, x, y, attr0)
    struct SprGroup *g;
    u16 x;
    u16 y;
    u16 attr0;
{
    u8 i;

    if (gUnk_0201F820.unkAEA != 2) {
        for (i = 0; i < g->count; i++) {
            u16 *oam = sub_08025374(g->list + i * 4, 1, x, y, 0, 0x200, &gUnk_0201F820);
            *oam |= attr0;
        }
    }
}

void sub_080254A4(void *p)
{
    if (gUnk_0201F820.unkAEA != 2)
        sub_080786D0(p);
}
u32 sub_080254C8(void)
{
    gMain.unk40E = 1;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= 0xE0FF;
    sub_0807A2EC(&gUnk_0201F820);
    sub_0807B534(gUnk_0201F820.unk618);
    gUnk_0201F820.step = 0;
    gUnk_0201F820.unkAC5 = 0;
    gUnk_0201F820.unkAC6 = 0;
    gUnk_0201F820.unkACB = 0xFF;
    gUnk_0201F820.unkACC = 0;
    sub_0807B0EC(0, 0, 0, &gUnk_0201F820.rampAD0);
    sub_0807B100(0, 0x100, 2, &gUnk_0201F820.rampAD8);
    sub_0807B0EC(0, 0, 0, &gUnk_0201F820.rampAE0);
    gUnk_0201F820.unkAE8 = 0x1600;
    gUnk_0201F820.unkAEC = 0x4000;
    gUnk_0201F820.unkAEE = 0x2B0;
    return 1;
}
u32 sub_080255B4(void)
{
    switch (gUnk_0201F820.unkAEA) {
    case 0:
        sub_08078670(gUnk_081999F8, &gUnk_0201F820.grp918);
        break;
    case 1:
        sub_08078670(gUnk_08199A04, &gUnk_0201F820.grp918);
        break;
    case 2:
        sub_08078670(gUnk_08199A04, &gUnk_0201F820.grp918);
        gUnk_0201F820.rampAD8.cur = 0x100;
        gUnk_0201F820.rampAD8.target = 0x100;
        gUnk_0201F820.rampAD8.state = 0;
        sub_0807B100(0, 0xF, 1, &gUnk_0201F820.rampAE0);
        gUnk_0201F820.step = 1;
        break;
    }
    sub_080787F4(0, -0x180, 0, gUnk_0201F820.unkABC);
    CpuSet(gUnk_086A12EC, (void *)0x06000000, 0x4B00);
    CpuSet(gUnk_086AA8EC, (void *)0x05000000, 0x100);
    sub_08077CEC(gUnk_086B2168, (void *)0x06014000, 0x10);
    sub_08077CEC(gUnk_086B4168, (void *)0x06014200, 0x10);
    CpuSet(gUnk_086B6168, (void *)0x05000200, 0x100);
    REG_DISPCNT = 0x1F04;
    return 1;
}
u32 sub_080256D8(void)
{
    u16 x, y;
    s16 z;
    s32 bob;
    struct SprGroup *g;
    u8 i;

    x = sub_0807B4D0(0x100, gUnk_08087BA4[gUnk_0201F820.rampAD8.cur & 0xFF]) >> 4;
    y = (gUnk_0201F820.rampAD8.cur - 0xBE) / 2
        + (sub_0807B4D0(0x80, gUnk_08087BA4[((gUnk_0201F820.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gUnk_0201F820.unkACC == 0) {
        sub_0807B100(0, 10000, 4, &gUnk_0201F820.rampAD0);
        gUnk_0201F820.step = 2;
        gUnk_0201F820.unkACC = 1;
    }
    if (gUnk_0201F820.rampAD0.state)
        gUnk_0201F820.unkAC5++;
    if (gUnk_0201F820.step == 2) {
        z = -60 - ((gUnk_0201F820.rampAD0.cur * (100 - gUnk_0201F820.rampAD0.cur)) >> 8);
        for (i = 0; i < gUnk_08081FD4[(gUnk_0201F820.unkAC5 >> 2) & 7].count; i++)
            sub_08025374(gUnk_08081FD4[(gUnk_0201F820.unkAC5 >> 2) & 7].list + i * 4,
                         0, 0x68, z + 0x64, gUnk_0201F820.unkACD, 0x200, &gUnk_0201F820);
    } else {
        z = -((gUnk_0201F820.rampAD0.cur * (150 - gUnk_0201F820.rampAD0.cur)) >> 8);
        for (i = 0; i < gUnk_081999EC[gUnk_0201F820.unkAC6][(gUnk_0201F820.unkAC5 >> 2) & 7].count; i++)
            sub_08025374(gUnk_081999EC[gUnk_0201F820.unkAC6][(gUnk_0201F820.unkAC5 >> 2) & 7].list + i * 4,
                         0, 0x68, z + 0x64, gUnk_0201F820.unkACD, 0x200, &gUnk_0201F820);
    }
    g = &gUnk_0201F820.grp918;
    g->unkE = 1;
    sub_080254A4(g);
    bob = (sub_0807B4D0(0x100, gUnk_08087BA4[(gUnk_0201F820.unkAC5 * 2 + 0x20) & 0xFF]) >> 4) + 8;
    sub_08025430(g, x + 0x68, y - bob, 0);
    sub_0807B114(&gUnk_0201F820.rampAD0);
    if (z > 0) {
        gUnk_0201F820.rampAD0.cur = 0;
        gUnk_0201F820.rampAD0.state = 0;
        if (gUnk_0201F820.step == 2) {
            sub_0807B100(0, 10000, 4, &gUnk_0201F820.rampAD0);
            gUnk_0201F820.unkAC6 = sub_08076F9C() % 2;
        } else {
            gUnk_0201F820.unkAC8 = gUnk_0201F820.unkAC9;
            gUnk_0201F820.unkAC6 = gUnk_0201F820.unkAC7;
            sub_0807B100(gUnk_0201F820.unkAC8, gUnk_0201F820.unkAC8 + 10, 1, &gUnk_0201F820.rampAD0);
        }
        sub_08077AEC(0x1F);
        REG_BLDALPHA = 0x1010;
        REG_BLDY = 8;
        REG_BLDCNT = 0x3F40;
        return 1;
    }
    return 0;
}
u32 sub_080259DC(void)
{
    u16 x, y;
    u16 dx;
    struct SprGroup *g;
    u16 dy;
    s32 sx, sx2;
    s32 bob;
    u8 i;

    x = sub_0807B4D0(0x100, gUnk_08087BA4[gUnk_0201F820.rampAD8.cur & 0xFF]) >> 4;
    y = (gUnk_0201F820.rampAD8.cur - 0xBE) / 2
        + (sub_0807B4D0(0x80, gUnk_08087BA4[((gUnk_0201F820.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gMain.newKeys & 1) {
        sub_080787F4(0, 0x180, 0, gUnk_0201F820.unkABC);
        return 1;
    }
    if (gUnk_0201F820.rampAD0.state == 0) {
        sub_080787F4(0, 0x180, 0, gUnk_0201F820.unkABC);
        return 1;
    }
    if (gUnk_0201F820.rampAD0.state == 2) {
        if (--gUnk_0201F820.unkACB == 0xFF)
            gUnk_0201F820.rampAD0.state = 0;
    }
    for (i = 0; i < gUnk_081999EC[gUnk_0201F820.unkAC6][gUnk_0201F820.rampAD0.cur % 20].count; i++)
        sub_08025374(gUnk_081999EC[gUnk_0201F820.unkAC6][gUnk_0201F820.rampAD0.cur % 20].list + i * 4,
                     0, 0x68, (gUnk_0201F820.rampAD0.cur - gUnk_0201F820.unkAC8) * 2 + 0x64,
                     gUnk_0201F820.unkACD, 0x200, &gUnk_0201F820);
    dx = 0;
    dy = 0;
    if (gUnk_0201F820.rampAD0.state == 2) {
        g = &gUnk_0201F820.grp92C;
        if (gUnk_0201F820.unkAE8 != 0) {
            if (gUnk_0201F820.unkAE8 <= 0x1000)
                REG_BLDALPHA = gUnk_0201F820.unkAE8 >> 8;
            gUnk_0201F820.unkAE8 -= 0x18;
        }
        if (gUnk_0201F820.unkAEA == 1) {
            gUnk_0201F820.unkAEC += gUnk_0201F820.unkAEE;
            gUnk_0201F820.unkAEE += 8;
            dx = sub_0807B4D0(0x2000, gUnk_08087BA4[(gUnk_0201F820.unkAEC >> 8) + 0x40]) >> 8;
            dy = (sub_0807B4D0(0x1400, gUnk_08087BA4[gUnk_0201F820.unkAEC >> 8]) >> 8) - 0x14;
        }
    } else if (gUnk_0201F820.rampAD0.state != 0) {
        g = &gUnk_0201F820.grp918;
        if (gUnk_0201F820.rampAD0.cur % 3 == 0)
            sub_08077AEC(0x1F);
    }
    g->unkE = 1;
    y = (gUnk_0201F820.rampAD8.cur - 0xBE) / 2
        + (sub_0807B4D0(0x80, gUnk_08087BA4[((gUnk_0201F820.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gUnk_0201F820.rampAD0.state) {
        sub_080254A4(g);
        sx = dx + 0x68;
        sx2 = x + sx;
        bob = (sub_0807B4D0(0x100, gUnk_08087BA4[(gUnk_0201F820.unkAC5 * 2 + 0x20) & 0xFF]) >> 4) + 8;
        sub_08025430(g, sx2, y - bob + dy, 0x400);
    }
    sub_0807B114(&gUnk_0201F820.rampAD0);
    return 0;
}
u32 sub_08025CB8(void)
{
    u16 x, y;
    struct SprGroup *g;
    u8 i;

    x = sub_0807B4D0(0x100, gUnk_08087BA4[gUnk_0201F820.rampAD8.cur & 0xFF]) >> 4;
    y = (gUnk_0201F820.rampAD8.cur - 0xBE) / 2
        + (sub_0807B4D0(0x80, gUnk_08087BA4[((gUnk_0201F820.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    g = &gUnk_0201F820.grp918;
    g->unkE = 1;
    sub_080254A4(g);
    sub_08025430(g, x + 0x68, y - 8, 0);
    for (i = 0; i < gUnk_081999EC[gUnk_0201F820.unkAC6][gUnk_0201F820.rampAD0.cur % 20].count; i++)
        sub_08025374(gUnk_081999EC[gUnk_0201F820.unkAC6][gUnk_0201F820.rampAD0.cur % 20].list + i * 4,
                     0, x + 0x68, y + 0x14, gUnk_0201F820.unkACD, 0x200, &gUnk_0201F820);
    sub_0807B114(&gUnk_0201F820.rampAD8);
    if (gUnk_0201F820.rampAD8.state == 2) {
        gUnk_0201F820.rampAD8.state = 0;
        sub_0807B100(0, 0xF, 1, &gUnk_0201F820.rampAE0);
        return 1;
    }
    return 0;
}
u32 sub_08025DF0(void)
{
    u16 x, y;
    struct SprGroup *g;
    u8 i;

    x = sub_0807B4D0(0x100, gUnk_08087BA4[gUnk_0201F820.rampAD8.cur & 0xFF]) >> 4;
    y = (gUnk_0201F820.rampAD8.cur - 0xBE) / 2
        + (sub_0807B4D0(0x80, gUnk_08087BA4[((gUnk_0201F820.rampAD8.cur * 2) & 0xFF) + 0x40]) >> 4) - 8;
    if (gUnk_0201F820.unkAEA != 2) {
        g = &gUnk_0201F820.grp918;
        g->unkE = 1;
        sub_080254A4(g);
        for (i = 0; i < g->count; i++)
            sub_08025374(g->list + i * 4, 1, x + 0x68, y - 8, 0, 0x200, &gUnk_0201F820);
    }
    for (i = 0; i < gUnk_081999EC[gUnk_0201F820.unkAC6][gUnk_0201F820.rampAD0.cur % 20].count; i++)
        sub_08025374(gUnk_081999EC[gUnk_0201F820.unkAC6][gUnk_0201F820.rampAD0.cur % 20].list + i * 4,
                     0, x + 0x68, y + 0x14, gUnk_0201F820.unkACD, 0x200, &gUnk_0201F820);
    sub_0807B114(&gUnk_0201F820.rampAE0);
    if (gUnk_0201F820.rampAE0.state == 2) {
        gUnk_0201F820.rampAE0.state = 0;
        sub_08077AEC(0x1F);
        return 1;
    }
    return 0;
}
u32 sub_08025F50(void)
{
    sub_0807883C(gUnk_0201F820.unkABC);
    if (gUnk_0201F820.unkAC2 == 2) {
        gUnk_02017A30.arg8 = gUnk_0201F820.result;
        return 1;
    }
    if (gUnk_0201F820.unkAEA == 2) {
        if (gUnk_08199A28[gUnk_0201F820.step]) {
            if (gUnk_08199A28[gUnk_0201F820.step]())
                gUnk_0201F820.step++;
        }
    } else {
        if (gUnk_08199A10[gUnk_0201F820.step]) {
            if (gUnk_08199A10[gUnk_0201F820.step]())
                gUnk_0201F820.step++;
        }
    }
    sub_0807A298(&gUnk_0201F820);
    sub_0807A2EC(&gUnk_0201F820);
    return 0;
}

u32 sub_08025FC4(void)
{
    gUnk_0201F820.unkAEA = 0;
    gUnk_0201F820.unkACD = 2;
    return 1;
}

u32 sub_08025FE8(void)
{
    gUnk_0201F820.unkAEA = 1;
    gUnk_0201F820.unkACD = 0;
    return 1;
}

u32 sub_0802600C(void)
{
    gUnk_0201F820.unkAEA = 2;
    gUnk_0201F820.unkACD = 0;
    return 1;
}
u32 sub_08026030(void)
{
    struct Work1F820 *w = &gUnk_0201F820;
    u8 side;
    u16 n;
    u8 t;
    u8 raw;
    int sum;
    u8 *result;
    sub_08075278(w, sizeof(*w));
    side = sub_08076F9C() % 2;
    n = gUnk_02017A30.opts;
    result = &w->result;
    *result = n;
    w->unkAC7 = gUnk_08082308[(n - 1) % 6][side][0];
    raw = gUnk_08082308[(*result - 1) % 6][side][1];
    sum = raw + 2;
    t = sum % 4;
    w->unkAC9 = t * 5 + 2;
    *result = gUnk_080822FC[w->unkAC7][(t + 2) % 4];
    /* FAKEMATCH: keep the initialized result address live through the last store. */
    asm volatile ("" : : "r"(result));
    return 1;
}

u32 sub_080260EC(void)
{
    if (gUnk_08199A40[gUnk_02017A30.step]) {
        if (gUnk_08199A40[gUnk_02017A30.step]())
            gUnk_02017A30.step++;
        return 0;
    }
    return 1;
}
