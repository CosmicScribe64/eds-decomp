#include "global.h"

/* UI block at 0x0201AE60: text box + menu state (see code_0804EFF0). */
struct Ui {
    u8 active : 1;      /* +0 bit 0 */
    u8 gfx_ok : 1;      /* bit 1: border gfx loaded */
    u8 pad1;
    u16 w2;
    u16 mode;           /* +4 menu kind (0/1 none, 2 = 3-choice cursor) */
    u16 w6;
    u16 x;              /* +8 */
    u16 y;              /* +0xA */
    u16 w;              /* +0xC */
    u16 h;              /* +0xE */
    u16 u10;
    u16 sel2;           /* +0x12 2-choice selection */
    u16 sel;            /* +0x14 */
    u16 pad16;
    void (*cb1)(void);  /* +0x18 cursor draw callback */
    u16 (*cb2)(void);   /* +0x1C input callback */
    u8 b20;
    u8 b21;
    u8 state;           /* +0x22 */
    u8 timer;           /* +0x23 */
    u8 gfx[1];          /* +0x24: 216 * 0x20 bytes of border tiles */
};
extern struct Ui gUnk_0201AE60;
extern u8 gUnk_0201AE84[];

struct MainKeys {
    u8 pad[4];
    u16 held;
    u16 keys;
};
extern struct MainKeys gUnk_03000040;

struct Scr {
    u8 b0;
    u8 pad1;
    u16 scroll;
    u8 pad4[3];
    u8 lo : 4;
    u8 hi : 4;          /* +7: high nibble = selected frame style */
    u8 pad8[0x800];
    u8 f0 : 1;
    u8 f1 : 1;
    u8 f2 : 1;
    u8 f3 : 1;
};
extern struct Scr gUnk_0201CFB0;
struct MainBig {
    u8 pad[0x485E];
    u16 counter;        /* +0x485E frame counter */
};
extern struct MainBig gUnk_03000040_b asm("gUnk_03000040");
extern const u16 gUnk_081A423C[];
extern const u16 gUnk_081A4424[];
extern const u32 gUnk_08086550[];
extern void sub_08076714(u32 a, u32 b, u32 c, u32 d);
extern void sub_0807326C(u32 a, u32 b, u32 c, u32 d);

struct MainMap2 {
    u8 pad[0x2C1C];
    u16 map[1];         /* BG map buffer at 0x03002C5C */
};
extern struct MainMap2 gUnk_03000040_m2 asm("gUnk_03000040");
extern u16 gUnk_03002C5C[];
extern u8 gUnk_0300045C[];
extern u16 gUnk_0300045C_h[] asm("gUnk_0300045C");
extern u8 gUnk_03001C5C[];
extern u8 gUnk_0867793C[];
extern u8 gUnk_0867795C[];
extern u8 gUnk_0867797C[];
extern u8 gUnk_0867817C[];
extern u8 gUnk_0867897C[];
extern u8 gUnk_0867917C[];
extern u8 gUnk_0867997C[];
extern u8 gUnk_0867A17C[];
extern u8 gUnk_0867A97C[];
extern u8 gUnk_0867B17C[];
extern u8 gUnk_0867B97C[];
extern u8 gUnk_0867E4BC[];
extern u8 gUnk_0867E6BC[];
extern u8 gUnk_0867EE3C[];
extern u8 gUnk_0867EE5C[];
extern u8 gUnk_0867EE9C[];
extern u8 gUnk_0867FC3C[];
extern u8 gUnk_0867FC5C[];
extern u8 gUnk_0868045C[];
extern u8 gUnk_0868047C[];
extern u8 gUnk_0868147C[];
extern u8 gUnk_0868149C[];
extern u8 gUnk_0868167C[];
extern u8 gUnk_0868187C[];
extern u8 gUnk_08681D7C[];
extern u8 gUnk_08681E7C[];
extern u8 gUnk_0868467C[];
extern u8 gUnk_0868487C[];
extern u8 gUnk_086849FC[];
extern u8 gUnk_08684B7C[];
extern u8 gUnk_08684EFC[];
extern u8 gUnk_0868557C[];
extern u8 gUnk_0868559C[];
extern void sub_08075630(void);
extern u32 gUnk_081A4214[];
/* Duel state: player array (0xD64 bytes each) at 0x020192E4 and its zone arrays (0x94 bytes each). */
struct DuelZone {
    u8 pad0[6];
    u8 f6_0 : 1;
    u8 faceDown : 1;
    u8 f6_rest : 6;
    u8 pad7[0x94 - 7];
};
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))
struct PlayerB {
    u8 pad[3];
    u8 b3, b4, b5, b6;
    u8 rest[0xD64 - 7];
};
extern struct PlayerB gUnk_020192E4[2];
extern u8 gUnk_020195F0[];
extern u8 gUnk_020198D4[];
struct DuelView {
    u8 pad[0x824];
    u32 w824;
    u32 w828;
    u32 w82C;
};
extern struct DuelView gUnk_0201CFB0_d asm("gUnk_0201CFB0");
extern const u8 gUnk_080864BC[];
extern const u8 gUnk_080864CC[];
extern const u8 gUnk_080864E4[];
extern const u8 gUnk_080864F0[];
extern const u8 gUnk_08086500[];
extern const u8 gUnk_08086510[];
extern const u8 gUnk_08086524[];
extern const u8 gUnk_08086538[];
extern u16 sub_0805ECFC(void);
extern void sub_0805ED9C(void);
extern void sub_0805F728(u32 a, u32 b);
extern void sub_0805F270(u32 a, u32 b, u32 c, u32 d);
extern void sub_0805F074(u32 a, u32 b);
extern void sub_0805F64C(u32 a, const u8 *txt, u32 c, u32 d);
extern s32 sub_080753CC(const u8 *str);
extern int sub_0800A368(int player);
extern u8 gUnk_02011C20[];
extern void sub_08074B08(u32 w, u32 h);
extern void sub_08074C80(u32 code, u32 x, u32 y, u32 attr);
extern void sub_08074D48(u32 ch, u32 x, u32 y, u32 attr);
extern u32 sub_08074AB4(const u8 *s);
extern void sub_08075114(void *dst, u32 n);
extern void sub_080608BC(u32 a, u32 b, u32 c, int val);
extern void sub_080761F0(u32 a, u32 b, u32 c);
extern void sub_080752B0(void *src, void *dst, u32 n);
extern void sub_08077AEC(int n);
extern void sub_0805FEA4(u32 row);
extern void sub_0805FD28(u16 a, u16 b, const u8 *text);
extern void sub_0805FBA4(void);
extern u16 sub_0805FC18(void);
extern u16 sub_080600D8(void);
extern u16 sub_0806007C_u(void) asm("sub_0806007C");
extern u16 sub_080600AC_u(void) asm("sub_080600AC");

void sub_0805F96C(void)
{
    u16 flag;
    u32 a;
    u32 b;
    u32 c;
    u32 a2;
    u32 sum;
    int v;
    flag = sub_0805ECFC();
    a = gUnk_0201CFB0_d.w824;
    b = gUnk_0201CFB0_d.w828;
    c = gUnk_0201CFB0_d.w82C;
    a2 = a;
    sum = b + c;
    sub_0805ED9C();
    switch (b) {
    case 0:
        if (a != 0)
            v = ZB2(a & 1, c)->faceDown;
        else
            v = 1;
        if (flag && v)
            sub_0805F728(a2, sum);
        break;
    case 5:
        if (a != 0)
            v = ((struct DuelZone *)((c * 0x94 + (a & 1) * 0xD64) + (u32)gUnk_020195F0))->faceDown;
        else
            v = 1;
        if (flag && v)
            sub_0805F270(flag, 1, a, b + c);
        break;
    case 10:
        if (a != 0)
            v = ((struct DuelZone *)((a & 1) * 0xD64 + (u32)gUnk_020198D4))->faceDown;
        else
            v = 1;
        if (flag && v)
            sub_0805F270(flag, 1, a, b + c);
        break;
    case 11:
        if (a != 0)
            v = (u16)sub_0800A368(a);
        else
            v = 1;
        if (flag && v)
            sub_0805F074(flag, 1);
        break;
    case 12:
        switch (a) {
        case 0: {
            const u8 *txt = gUnk_080864BC;
            u8 p = gUnk_020192E4[a].b5;
            sub_0805F64C(10, txt, p, sub_080753CC(txt));
            break;
        }
        case 1:
            sub_0805F64C(10, gUnk_080864CC, 0, 0);
            break;
        }
        break;
    case 13:
        switch (a) {
        case 0: {
            const u8 *txt = gUnk_080864E4;
            u8 p = gUnk_020192E4[a].b3;
            sub_0805F64C(10, txt, p, sub_080753CC(txt));
            break;
        }
        case 1:
            sub_0805F64C(10, gUnk_080864F0, 0, 0);
            break;
        }
        break;
    case 14:
        switch (a) {
        case 0: {
            const u8 *txt = gUnk_08086500;
            u8 p = gUnk_020192E4[a].b4;
            sub_0805F64C(10, txt, p, sub_080753CC(txt));
            break;
        }
        case 1: {
            const u8 *txt = gUnk_08086510;
            u8 p = gUnk_020192E4[a].b4;
            sub_0805F64C(10, txt, p, sub_080753CC(txt));
            break;
        }
        }
        break;
    case 15:
        switch (a) {
        case 0: {
            const u8 *txt = gUnk_08086524;
            u8 p = gUnk_020192E4[a].b6;
            sub_0805F64C(10, txt, p, sub_080753CC(txt));
            break;
        }
        case 1: {
            const u8 *txt = gUnk_08086538;
            u8 p = gUnk_020192E4[a].b6;
            sub_0805F64C(10, txt, p, sub_080753CC(txt));
            break;
        }
        }
        break;
    }
}
/* Draws the 3-choice menu cursor sprite (blinks while confirming). */
void sub_0805FBA4(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u32 x = (u->x + 1) * 8;
    int y = (u->y + 2) * 8 + u->sel * 12 - 2;
    y -= (u->h + u->y - u->b21 + 2) * 8;
    if (u->state == 1) {
        if (u->timer & 2)
            sub_080761F0((y << 16) | x, 0, 0x431F);
    } else {
        sub_080761F0((y << 16) | x, 0, 0x431F);
    }
}


/* Input handler of the 2-choice menu with cursor (up/down toggle, A confirm). State 1 = blink
   timer after confirming (held B or flag speeds it up), 2 = done (returns 1). */
u16 sub_0805FC18(void)
{
    u32 st = gUnk_0201AE60.state;
    u8 new_var; /* FAKEMATCH: a dead byte copy of st steers register allocation of the switch value */
    switch ((u8)st) {
    case 1:
        if (gUnk_0201AE60.timer <= 0x3B) {
            gUnk_0201AE60.timer++;
            new_var = st;
            if ((gUnk_03000040.held & 2) || (gUnk_0201CFB0.b0 & new_var)) {
                if (gUnk_0201AE60.timer <= 0x33)
                    gUnk_0201AE60.timer += 7;
            }
        } else
            gUnk_0201AE60.state = st + 1;
        break;
    case 2:
        return 1;
    default:
        if (gUnk_03000040.keys & 0x80) {
            sub_08077AEC(0);
            gUnk_0201AE60.sel = 1 - gUnk_0201AE60.sel;
        }
        if (gUnk_03000040.keys & 0x40) {
            sub_08077AEC(0);
            gUnk_0201AE60.sel = 1 - gUnk_0201AE60.sel;
        }
        if (gUnk_03000040.keys & 1) {
            sub_08077AEC(1);
            gUnk_0201AE60.state = 1;
            gUnk_0201AE60.timer = 0;
        }
        if (gUnk_03000040.keys & 2)
            sub_08077AEC(3);
        break;
    }
    return 0;
}

/* Copies the 216 border tile slots to VRAM 0x06009A40. */
void sub_0805FCF4(void)
{
    u8 *p = gUnk_0201AE84;
    int i;
    for (i = 0; i < 216; i++) {
        sub_080752B0(p, (void *)0x06009A40, 0x20);
        p += 0x20;
    }
    gUnk_0201AE60.gfx_ok = 1;
}

/* Lays out and draws the text `s` into the text box: a = x | y << 8, b = w | h << 8 (cells).
   '\n' = new line, '@d' = select colour d, 2-byte chars when bit 7 of 0x02011C20+4 is set. */
typedef void (*Fd28DrawW)(u16 sjis, s32 x, s32 y, u16 sc);
typedef void (*Fd28DrawB)(u8 ch, s32 x, s32 y, u16 sc);
typedef int (*Fd28Width)(const u8 *s);

void sub_0805FD28(u16 a, u16 b, const u8 *s)
{
    int col = 1;
    int px = 0;
    int py = 0;
    gUnk_0201AE60.x = (u8)a;
    gUnk_0201AE60.y = a >> 8;
    gUnk_0201AE60.w = (u8)b;
    gUnk_0201AE60.h = b >> 8;
    if (gUnk_0201AE60.w > 0x18)
        gUnk_0201AE60.w = 0x18;
    if (gUnk_0201AE60.h > 0xB)
        gUnk_0201AE60.h = 0xB;
    if (gUnk_0201AE60.w <= 4)
        gUnk_0201AE60.w = 5;
    if (gUnk_0201AE60.h <= 1)
        gUnk_0201AE60.h = 2;
    sub_0805FCF4();
    sub_08074B08(gUnk_0201AE60.w, gUnk_0201AE60.h);
    while (*s != 0) {
        switch (*s) {
        case 0x40:
            if ((u8)(s[1] - 0x30) <= 9) {
                col = s[1] - 0x30;
                if (col == 0)
                    col = 1;
                s++;
            }
            break;
        case 0xA:
            px = 0;
            py += 12;
            break;
        default:
            if (gUnk_02011C20[4] & 0x80) {
                if (px + 10 >= gUnk_0201AE60.w * 8) {
                    px = 0;
                    py += 12;
                }
                ((Fd28DrawW)sub_08074C80)((*s << 8) | s[1], px + 1, py + 1, (u8)gUnk_081A4214[9] | 0xA00);
                ((Fd28DrawW)sub_08074C80)((*s << 8) | s[1], px, py, (u8)gUnk_081A4214[col] | 0xA00);
                px += 10;
                s++;
            } else {
                if (((Fd28Width)sub_08074AB4)(s) * 5 + px > gUnk_0201AE60.w * 8) {
                    px = 0;
                    py += 12;
                }
                ((Fd28DrawB)sub_08074D48)(*s, px + 1, py + 1, (u8)gUnk_081A4214[9] | 0xA00);
                ((Fd28DrawB)sub_08074D48)(*s, px, py, (u8)gUnk_081A4214[col] | 0xA00);
                px += 5;
            }
            break;
        }
        s++;
    }
    sub_08075114(gUnk_0201AE84, *(u16 *)gUnk_081A4214);
}

/* Draws one scan row `row` of the text box frame (top border, h text rows, bottom border) into the
   BG map at 0x03002C5C; rows outside 0..0x13 are clipped. Frame tiles 0x82CE-0x82D6, text tiles 0x82D7+. */
void sub_0805FEA4(u32 row)
{
    u32 t = row - gUnk_0201AE60.h;
    u32 r = t - 2;
    u16 idx;
    u32 q;
    int j;
    int k;
    int base;
    u16 text;
    q = gUnk_0201AE60.x + 0xFFFF;
    q += (t - 4) * 32;
    idx = q;
    base = 0x82CE;
    text = 0x82D7;
    if (r <= 0x13) {
        for (j = 0; j < gUnk_0201AE60.w + 2; j++)
            gUnk_03000040_m2.map[idx + j] = 0;
    }
    r++;
    idx += 0x20;
    if (r <= 0x13) {
        gUnk_03000040_m2.map[idx] = base;
        for (j = 1; j <= gUnk_0201AE60.w; j++)
            gUnk_03000040_m2.map[idx + j] = base + 1;
        gUnk_03000040_m2.map[idx + gUnk_0201AE60.w + 1] = base + 2;
    }
    r++;
    idx += 0x20;
    for (j = 0; j < gUnk_0201AE60.h; j++) {
        if (r <= 0x13) {
            gUnk_03000040_m2.map[idx] = base + 3;
            for (k = 1; k <= gUnk_0201AE60.w; k++)
                gUnk_03000040_m2.map[idx + k] = text++;
            gUnk_03000040_m2.map[idx + gUnk_0201AE60.w + 1] = base + 5;
        }
        r++;
        idx += 0x20;
    }
    if (r <= 0x13) {
        gUnk_03000040_m2.map[idx] = base + 6;
        for (j = 1; j <= gUnk_0201AE60.w; j++)
            gUnk_03000040_m2.map[idx + j] = base + 7;
        gUnk_03000040_m2.map[idx + gUnk_0201AE60.w + 1] = base + 8;
    }
    r++;
    idx += 0x20;
    if (r <= 0x13) {
        for (j = 0; j < gUnk_0201AE60.w + 2; j++)
            gUnk_03000040_m2.map[idx + j] = 0;
    }
}
/* Scrolls the text box down one row; returns 1 at the end. */
int sub_0806007C(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *p = &u->b21;
    sub_0805FEA4(*p);
    if (*p < u->h + u->y + 2) {
        (*p)++;
        return 0;
    }
    return 1;
}

/* Scrolls the text box up one row; returns 1 at the top. */
int sub_080600AC(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *p = &u->b21;
    sub_0805FEA4(--*p);
    if (*p == 0)
        return 1;
    return 0;
}

/* Input handler of the 2-choice menu (A confirms, B = "no", left/right toggle); returns 1 when done. */
u16 sub_080600D8(void)
{
    struct Ui *u = &gUnk_0201AE60;
    switch (u->mode) {
    case 0:
        if (gUnk_03000040.keys & 1) {
            sub_08077AEC(2);
            return 1;
        }
        break;
    case 1:
        if (gUnk_03000040.keys & 1) {
            sub_08077AEC(1);
            switch (u->sel2) {
            case 0:
                u->sel = 1;
                break;
            case 1:
                u->sel = 0;
                break;
            }
            return 1;
        }
        if (gUnk_03000040.keys & 2) {
            sub_08077AEC(2);
            u->sel = 0;
            return 1;
        }
        if (gUnk_03000040.keys & 0x30)
            u->sel2 = 1 - u->sel2;
        break;
    }
    return 0;
}

/* Draws the cursor sprites of the text box menu (mode 0: arrow, mode 1: 2-choice markers). */
void sub_08060160(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u32 pos;
    u32 y;
    u32 t;
    switch (u->mode) {
    case 0:
        pos = (u->x + u->w - 2) * 8;
        pos |= (u->b21 - 4) << 19;
        sub_080761F0(pos, 0x40, (u16)(gUnk_081A423C[gUnk_03000040_b.counter >> 2 & 0x1F] * 4 + 0x42E4));
        break;
    case 1:
        pos = (u->x + (u->w >> 1)) * 8;
        pos -= 0x30;
        y = (u->b21 - 4) * 8;
        if (u->sel2 == 0)
            t = gUnk_081A4424[gUnk_03000040_b.counter >> 2 & 0xF];
        else
            t = 0x100;
        pos |= y << 16;
        sub_08076714(pos, 0x4080, 0x430C, t << 16);
        u = &gUnk_0201AE60;
        pos = (u->x + (u->w >> 1)) * 8;
        pos += 0x10;
        if (u->sel2 == 1)
            t = gUnk_081A4424[gUnk_03000040_b.counter >> 2 & 0xF];
        else
            t = 0x100;
        pos |= y << 16;
        sub_08076714(pos, 0x4080, 0x4314, t << 16);
        u = &gUnk_0201AE60;
        pos = (u->x + (u->w >> 1)) * 8;
        pos -= 0x30;
        pos += u->sel2 * 0x40;
        y += 8;
        pos |= y << 16;
        sub_080761F0(pos, 0x80, 0);
        break;
    }
}

/* Opens a text box: layout (a = x|w<<8?, b), text `text`; resets the menu state. */
void sub_080602A4(u16 a, u16 b, u16 c, const u8 *text)
{
    struct Ui *u = &gUnk_0201AE60;
    u->w2 = 0;
    sub_0805FD28(a, b, text);
    u->w6 = c;
    u->mode = 0;
    u->cb1 = 0;
    u->cb2 = 0;
    u->b20 = 0;
    u->b21 = 0;
    u->state = 0;
    u->timer = 0;
    u->sel = 0;
    u->active = 1;
    gUnk_0201CFB0.f3 = 0;
}

/* Selects the menu kind: 2 = 3-choice menu with cursor/input callbacks. */
void sub_08060308(u16 mode, void (*cb1)(void), u16 (*cb2)(void))
{
    struct Ui *u = &gUnk_0201AE60;
    u->mode = mode;
    u->cb1 = cb1;
    u->cb2 = cb2;
    switch (u->mode) {
    case 0:
    case 1:
        u->cb1 = 0;
        u->cb2 = 0;
        break;
    case 2:
        u->cb1 = sub_0805FBA4;
        u->cb2 = sub_0805FC18;
        break;
    }
}

/* Per-frame text-box driver. w6 contains the opening flags, not main keys.
 * FAKEMATCH: initialized state r2, callback r1, and next-state r0 retain the
 * target's switch copy and callback ABI registers. The next-value input and
 * two empty clobbers preserve distinct case store tails; no code is emitted. */
int sub_08060344(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *p;
    register u32 st __asm__("r2");
    int sw;
    register u16 (*cb)(void) __asm__("r1");
    register u32 next __asm__("r0");
    if (u->active) {
        p = &u->b20;
        st = *p;
        sw = st;
        switch (sw) {
        case 0:
            if (u->w6 & 1) {
                if (!sub_0806007C_u())
                    break;
                next = *p + 1;
            } else
                next = st + 1;
            *p = next;
            break;
        case 1:
            if (u->w6 & 8) {
                cb = u->cb2;
                if (cb) {
                    if (!cb())
                        break;
                    next = *p + 1;
                    goto menu_store;
                } else {
                    if (!sub_080600D8())
                        break;
                }
                next = *p + 1;
            } else
                next = st + 1;
            __asm__ volatile("" : : "r"(next));
        menu_store:
            *p = next;
            __asm__ volatile("" : : : "r3");
            break;
        case 2:
            if (u->w6 & sw) {
                if (!sub_080600AC_u())
                    break;
                next = *p + 1;
            } else
                next = st + 1;
            *p = next;
            __asm__ volatile("" : : : "r1");
            break;
        default:
            u->active = 0;
            break;
        }
        return 1;
    }
    return 0;
}


/* Scrolls BG0/BG1 vertically by one line. */
void sub_08060400(void)
{
    int v = gUnk_0201CFB0.scroll - 1;
    gUnk_0201CFB0.scroll = v;
    *(vu16 *)0x04000012 = v;
    *(vu16 *)0x04000010 = v;
}

/* BG0-3 control registers. */
void sub_0806041C(void)
{
    *(vu16 *)0x04000008 = 7;
    *(vu16 *)0x0400000A = 0x8106;
    *(vu16 *)0x0400000C = 0x8305;
    *(vu16 *)0x0400000E = 0x0504;
}

/* Fills the BG map (32x32) with 4x4-tile blocks of ascending tiles 0x5060.. and selects the frame
   style `n` (loaded from table 0x08086550) in the high nibble of 0x0201CFB0+7. */
void sub_0806044C(u16 n)
{
    u16 i;
    u16 j;
    sub_0807326C(0, 0x50, 0x60, gUnk_08086550[n]);
    for (i = 0; i <= 0x1F; i += 4) {
        for (j = 0; j <= 0x1F; j += 4) {
            int b = i * 32 + j;
            gUnk_0300045C_h[b + 0] = 0x5060;
            gUnk_0300045C_h[b + 1] = 0x5061;
            gUnk_0300045C_h[b + 2] = 0x5062;
            gUnk_0300045C_h[b + 3] = 0x5063;
            gUnk_0300045C_h[b + 0x20] = 0x5064;
            gUnk_0300045C_h[b + 0x21] = 0x5065;
            gUnk_0300045C_h[b + 0x22] = 0x5066;
            gUnk_0300045C_h[b + 0x23] = 0x5067;
            gUnk_0300045C_h[b + 0x40] = 0x5068;
            gUnk_0300045C_h[b + 0x41] = 0x5069;
            gUnk_0300045C_h[b + 0x42] = 0x506A;
            gUnk_0300045C_h[b + 0x43] = 0x506B;
            gUnk_0300045C_h[b + 0x60] = 0x506C;
            gUnk_0300045C_h[b + 0x61] = 0x506D;
            gUnk_0300045C_h[b + 0x62] = 0x506E;
            gUnk_0300045C_h[b + 0x63] = 0x506F;
        }
    }
    gUnk_0201CFB0.hi = n;
}

/* Loads the duel UI graphics: 7 palettes to OBJ palette 0x05000200.., 17 tile blocks to OBJ VRAM 0x06010000.. */
void sub_08060578(void)
{
    *(vu16 *)0x04000000 |= 0x40;
    sub_080752B0((void *)0x05000200, gUnk_0867793C, 0x20);
    sub_080752B0((void *)0x05000220, gUnk_0867795C, 0x20);
    sub_080752B0((void *)0x050002A0, gUnk_0868467C, 0x20);
    sub_080752B0((void *)0x05000240, gUnk_0867FC3C, 0x20);
    sub_080752B0((void *)0x05000260, gUnk_0868045C, 0x20);
    sub_080752B0((void *)0x05000280, gUnk_0868167C, 0x20);
    sub_080752B0((void *)0x050002C0, gUnk_0868557C, 0x20);
    sub_080752B0((void *)0x06010000, gUnk_0867797C, 0x800);
    sub_080752B0((void *)0x06010800, gUnk_0867817C, 0x800);
    sub_080752B0((void *)0x06011000, gUnk_0867897C, 0x800);
    sub_080752B0((void *)0x06011800, gUnk_0867917C, 0x800);
    sub_080752B0((void *)0x06012000, gUnk_0867997C, 0x800);
    sub_080752B0((void *)0x06012800, gUnk_0867A17C, 0x800);
    sub_080752B0((void *)0x06013000, gUnk_0867B17C, 0x800);
    sub_080752B0((void *)0x06013800, gUnk_0867A97C, 0x800);
    sub_080752B0((void *)0x06014000, gUnk_0868487C, 0x180);
    sub_080752B0((void *)0x06014180, gUnk_086849FC, 0x180);
    sub_080752B0((void *)0x06014300, gUnk_08684B7C, 0x180);
    sub_080752B0((void *)0x06014480, gUnk_0867FC5C, 0x800);
    sub_080752B0((void *)0x06014C80, gUnk_0868047C, 0x1000);
    sub_080752B0((void *)0x06015C80, gUnk_0868187C, 0x500);
    sub_080752B0((void *)0x06016180, gUnk_08681E7C, 0x200);
    sub_080752B0((void *)0x06016380, gUnk_08681D7C, 0x100);
    sub_080752B0((void *)0x06016480, gUnk_0868559C, 0x800);
    gUnk_0201CFB0.b0 |= 2;
}

/* Loads the BG graphics of the text box/menu frame (palettes to 0x05000020.., tiles to 0x06004E00..). */
void sub_0806075C(void)
{
    sub_08075630();
    sub_080752B0((void *)0x05000020, gUnk_0867793C, 0x20);
    sub_080752B0((void *)0x05000040, gUnk_0867795C, 0x20);
    sub_080752B0((void *)0x05000060, gUnk_0867E4BC, 0x20);
    sub_080752B0((void *)0x05000080, gUnk_0867EE3C, 0x20);
    sub_080752B0((void *)0x05000100, gUnk_0868147C, 0x20);
    sub_080752B0((void *)0x06004E00, gUnk_0867817C, 0x800);
    sub_080752B0((void *)0x06005600, gUnk_0867897C, 0x800);
    sub_080752B0((void *)0x06005E00, gUnk_0867917C, 0x800);
    sub_080752B0((void *)0x06006600, gUnk_0867997C, 0x800);
    sub_080752B0((void *)0x06006E00, gUnk_0867A17C, 0x800);
    sub_080752B0((void *)0x06007600, gUnk_0867B17C, 0x800);
    sub_080752B0((void *)0x06007E00, gUnk_0867A97C, 0x800);
    sub_080752B0((void *)0x06008600, gUnk_0867B97C, 0x200);
    sub_080752B0((void *)0x06008800, gUnk_08684EFC, 0x200);
    sub_080752B0((void *)0x06008880, gUnk_0867E6BC, 0x780);
    sub_080752B0((void *)0x06009000, gUnk_0867EE5C, 0x40);
    sub_080752B0((void *)0x06009040, gUnk_0867EE9C, 0x180);
    sub_080752B0((void *)0x060099C0, gUnk_0868149C, 0x120);
}

/* Draws `val` as a 5-digit number (leading zeros blank) into the BG map at block `a`, cell `b + 4`
   (right to left), tile base 0x3244 + c*10. */
void sub_080608BC(u32 a, u32 b, u32 c, int val)
{
    int v = val;
    u16 idx = ((b << 16) + 0x40000) >> 16;
    int i = 0;
    u8 *base = gUnk_0300045C;
    u32 ao = a * 0x800;
    u16 *cell;

    /* FAKEMATCH: keep the value live separately from the tilemap cell address. */
    __asm__ __volatile__("" : : "r"(v));
    for (; i <= 4; i++) {
        u8 *q = base + idx * 2;
        cell = (u16 *)(ao + (u32)q);
        if (v == 0 && i > 0)
            *cell = v;
        else {
            int t = v % 10 + 0x3244;
            *cell = t + c * 10;
        }
        v /= 10;
        idx--;
    }
}

void sub_08060934(int kind, int val)
{
    switch (kind) {
    case 0:
        sub_080608BC(3, 0x267, 0, val);
        break;
    case 1:
        sub_080608BC(3, 0x294, 0, val);
        break;
    }
}

/* Draws the two rows of 6 selector tiles into the map at 0x03001C5C; entry (a, b) is highlighted. */
void sub_08060964(int a, int b)
{
    int i;
    int j;
    for (i = 0; i <= 1; i++) {
        u16 idx;
        u16 t = 0x286;
        if (i != 0)
            t -= 0x13;
        idx = t;
        for (j = 0; j <= 5; j++) {
            int v = 0x288;
            if (i == a && j == b)
                v -= 6;
            ((u16 *)gUnk_03001C5C)[idx] = v + j + 0x4000;
            idx++;
        }
    }
}
