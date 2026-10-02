#include "global.h"
#include "gba.h"

/*
 * Debug menu (unused), link-cable helpers, text rendering.
 * See wiki/functions/code-080740bc.md
 */

#define REG_SIOMLT_SEND REG16(0x12A)

struct LinkSio {
    u8 pad0[0xA1E];
    u8 unkA1E;              /* 8 when the multi-player handshake succeeded (hypothesis: master flag) */
    u8 unkA1F;              /* stage 0/1 */
    u8 unkA20;
    u8 unkA21;
    u16 unkA22;             /* per-slot receive status bits */
    u8 unkA24;
    u8 padA25[7];
    s32 unkA2C;             /* state, -3..10 (see code_080750E0) */
    u32 unkA30;
    u16 *unkA34;
    u16 *unkA38;
    u16 txBuf[14];          /* +0xA3C */
    u8 padA58[0xAFC - 0xA58];
    s32 unkAFC;
    s32 unkB00;
    u32 unkB04;
    u16 *unkB08;
    u32 unkB0C;
    u16 unkB10;
    u16 unkB12;
    u16 unkB14;
};
extern struct LinkSio gUnk_03005B60;

struct Main {
    u8 pad0[6];
    u16 newKeys;            /* +6 */
    u8 pad06[0x40E - 8];
    u16 unk40E;
    u8 pad1[0x4857 - 0x410];
    u8 unk4857;
    u8 unk4858;
    u8 unk4859;
    u8 unk485A;
    u8 unk485B;
};
extern struct Main gUnk_03000040;

extern u16 sub_08026C90(void);
extern u16 sub_08027C58(void);
extern void sub_080770E8(void);
extern void sub_08077114(void);
extern u32 sub_08063BAC(void);
extern u32 sub_08063C14(void);
extern u32 sub_08063C7C(void);
extern u32 sub_08063CE4(void);
extern void sub_08077948(u32 id);
extern void sub_08073574(void);
extern void sub_080757AC(void);
extern void sub_080759F4(void);
extern void sub_08075630(void);
extern void sub_08004914(void *);
extern void sub_08004358(u32, u32, u32);
extern void sub_080042D8(u32, u32, u32);
extern void sub_08072BB4(u32, u32, u32, const void *);
extern void sub_08072C0C(u32, u32, u32, u32);
extern u8 gUnk_08087B34[], gUnk_08087B40[], gUnk_08087B58[], gUnk_08087B60[], gUnk_08087B68[], gUnk_08087B70[], gUnk_08087B78[];
struct DebugItem { char name[0x40]; void *cb; };
extern struct DebugItem gUnk_081A73A0[];
struct FlagRow { u32 mask; char name[0x20]; };
extern struct FlagRow gUnk_08087720[];
struct Save {
    u8 pad0[4];
    u8 mode : 7;
    u8 jpFont : 1;
    u8 pad5[0x2150 - 5];
    u16 unk2150;
};
extern void sub_080761F0(u32, u32, u32);
extern u16 (*gUnk_081A768C[])(void);
extern const u16 gUnk_081A76A0[];
extern void sub_08075278(void *, u32);
extern void sub_080741D8(void);
extern u16 sub_08074260(u8 *rx);
extern u32 sub_08072584(u16);
extern u32 sub_080728C0(u32);
extern u32 sub_080729F8(u32);
extern u8 sub_08074AB4(const u8 *);
extern void sub_08074D48(u8 ch, s32 x, s32 y, u16 sc);
extern void sub_08074E60(s32 x, s32 y, u16 sc, const u8 *str);
extern void sub_08074F50(s32 x, s32 y, u16 sc, const u8 *str);
extern void sub_08074C80(u16 sjis, s32 x, s32 y, u16 sc);
extern void sub_08074B74(u8 bits, s32 x, s32 y, u32 color);
extern void sub_08074BF8(u16 bits, s32 x, s32 y, u32 color);
extern u16 gUnk_081C0000[], gUnk_081D0200[], gUnk_081F8700[];
extern u16 gUnk_08228D00[], gUnk_08229500[], gUnk_08229F00[], gUnk_0822AB00[];
extern u8 gUnk_02000000[];
extern u8 gUnk_02010000;
extern struct Save gUnk_02011C20;
extern u32 __umodsi3(u32, u32);
extern void sub_080734D4(void);
extern void sub_0807289C(u32, u32);
extern u16 sub_08075AE4(u32);
extern void sub_080754F8(void *);
struct DateBits { u32 a : 12; u32 b : 4; u32 c : 5; };
extern u32 sub_080044E4(u32, u32, u32);
extern u8 gUnk_02017A30[];
extern u8 *gUnk_03006598;
extern void CpuSet(const void *src, void *dst, u32 cnt);

#if 0 /* NONMATCHING: same structure; differs only in register allocation of the LinkSio base (r1 copied to r5 vs loaded straight into r5) and hs/base swap; the (~0x40) neg form does match */
/* Link main step (hypothesis): stage 0 waits for the multi-player handshake bits in SIOCNT, sets up Timer3/IE, then stage 1 runs sub_08074260 each frame. */
u16 sub_080740BC(u8 *rx) {
    u32 hs;
    struct LinkSio *s = &gUnk_03005B60;
    switch (s->unkA1F) {
    case 0: {
        vu32 *sio;
        u32 *dst;
        dst = &s->unkB0C;
        sio = (vu32 *)&REG_SIOCNT;
        *dst = *sio;
        hs = *(u8 *)&s->unkB0C & 0x88;
        if (hs != 8)
            goto done;
        {
            u8 t = *(u8 *)&s->unkB0C & 4;
            if (t == 0 && s->unkA2C == 0xC) {
                REG_IME = 0;
                REG_IE &= 0xFF7F;
                REG_IE |= 0x40;
                REG_IME = 1;
                {
                    u32 v = *((vu8 *)sio + 1);
                    u16 m = ~0x40;
                    *((vu8 *)sio + 1) = v & m;
                }
                REG_IF = 0xC0;
                *(vu32 *)&REG_TM3CNT_L = 0xB1FC;
                s->unkA1E = hs;
                s->unkA24 = 1;
            }
        }
        if (gUnk_03005B60.txBuf[2] == 0)
            gUnk_03005B60.txBuf[2] = 0x1000;
        gUnk_03005B60.unkA1F = 1;
    }
    /* fallthrough */
    case 1:
        gUnk_03005B60.unkB14 = sub_08074260(rx);
        if ((gUnk_03005B60.unkB14 & 3) == 0 && gUnk_03005B60.unkA1E == 8)
            sub_080741D8();
        break;
    default:
        break;
    }
done:
    {
        u16 *p = &gUnk_03005B60.unkB14;
        u16 v = *p;
        if (gUnk_03005B60.unkA1E == 8)
            v |= 0x80;
        *p = v;
        return *p;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080740BC", sub_080740BC); /* 0x080740BC size 0x11C */

void sub_080741D8(void) {
    if (gUnk_03005B60.unkA1F != 0 && gUnk_03005B60.unkA24 != 0) {
        vu16 *sio = &REG_SIOCNT;
        u16 v = 0xFEFE;
        sio[1] = v;
        *sio = *sio | 0x80;
        REG_TM3CNT_H = 0xC0;
    }
}

/* Build the link send packet: txBuf[1] = ~(sum of txBuf[0..9]) after copying 12 halfwords from src to txBuf[2]. */
void sub_08074218(void *src) {
    u16 sum = 0;
    u32 i;
    u16 *p;
    gUnk_03005B60.txBuf[1] = 0;
    CpuSet(src, &gUnk_03005B60.txBuf[2], 0xC);
    for (i = 0, p = &gUnk_03005B60.txBuf[0]; i <= 9; p++, i++)
        sum += *p;
    gUnk_03005B60.txBuf[1] = ~sum;
}

/* Link receive step: rotates the RX double buffer (+0xA34/+0xA38, via +0xB08), validates each of the 2 received packets in the last buffer by its 0xFFFF checksum, copies good ones out to rx + slot*16, clears them, and returns the per-slot status bits. */
u16 sub_08074260(u8 *rx) {
    u32 zero;
    REG_IME = 0;
    gUnk_03005B60.unkB08 = gUnk_03005B60.unkA38;
    gUnk_03005B60.unkA38 = gUnk_03005B60.unkA34;
    gUnk_03005B60.unkA34 = gUnk_03005B60.unkB08;
    gUnk_03005B60.unkB10 = gUnk_03005B60.unkA21;
    gUnk_03005B60.unkA21 = 0;
    REG_IME = 1;
    gUnk_03005B60.unkA22 = 0;
    if (gUnk_03005B60.unkB10 != 0) {
        for (gUnk_03005B60.unkAFC = 0; gUnk_03005B60.unkAFC < 2; gUnk_03005B60.unkAFC++) {
            gUnk_03005B60.unkB08 = (u16 *)((u8 *)gUnk_03005B60.unkA38 + gUnk_03005B60.unkAFC * 24);
            gUnk_03005B60.unkB12 = 0;
            for (gUnk_03005B60.unkB00 = 0; (u32)gUnk_03005B60.unkB00 < 10; gUnk_03005B60.unkB00++)
                gUnk_03005B60.unkB12 += gUnk_03005B60.unkB08[gUnk_03005B60.unkB00];
            if (gUnk_03005B60.unkB12 == 0xFFFF) {
                CpuSet(gUnk_03005B60.unkB08 + 2, rx + gUnk_03005B60.unkAFC * 16, 8);
                gUnk_03005B60.unkA22 |= 1 << gUnk_03005B60.unkAFC;
            } else {
                gUnk_03005B60.unkA22 |= 1 << (gUnk_03005B60.unkAFC + 4);
            }
            zero = 0;
            CpuSet(&zero, gUnk_03005B60.unkB08 + 2, 0x05000004);
        }
    }
    gUnk_03005B60.unkA20 |= gUnk_03005B60.unkA22;
    return gUnk_03005B60.unkA22;
}

u32 sub_080743A4(void) {
    struct Main *m = &gUnk_03000040;
    u8 *step = &m->unk4859;
    switch (*step) {
    case 0:
        m->unk485A = 0;
        gUnk_02017A30[0xB] = 0;
        (*step)++;
        return 0;
    case 1:
        m->unk485A++;
        if (m->unk485A <= 7) {
            m->unk40E &= 0xFFFE;
        } else {
            m->unk40E |= 1;
            m->unk485A = 0;
            if (sub_08026C90() != 0)
                (*step)++;
        }
        return 0;
    default:
        return 1;
    }
}

u32 sub_08074430(void) {
    struct Main *m = &gUnk_03000040;
    u8 *step = &m->unk4859;
    switch (*step) {
    case 0:
        gUnk_02017A30[0xB] = 0;
        (*step)++;
        return 0;
    case 1:
        if (sub_08027C58() != 0)
            (*step)++;
        return 0;
    default:
        return 1;
    }
}

u32 sub_08074474(void) {
    sub_080770E8();
    return 1;
}

/* Debug menu "Get all card" */
u32 sub_08074480(void) {
    sub_08077114();
    return 1;
}

/* Debug menu "Next Level": bump the counters of a block of cards depending on the current level. */
u32 sub_0807448C(void) {
    s32 i, j;
    if (sub_08063BAC() == 0) {
        for (i = 0; i <= 4; ) {
            u32 id = i + 1;
            sub_08077948(id);
            sub_08077948(id);
            i = id;
        }
    } else if (sub_08063C14() == 0) {
        for (i = 0; i <= 4; i++) {
            u32 id = i + 6;
            sub_08077948(id);
            sub_08077948(id);
            sub_08077948(id);
        }
    } else if (sub_08063C7C() == 0) {
        for (i = 0; i <= 4; i++) {
            u32 id = i + 0xB;
            sub_08077948(id);
            sub_08077948(id);
            sub_08077948(id);
            sub_08077948(id);
        }
    } else if (sub_08063CE4() == 0) {
        for (i = 0; i <= 4; i++) {
            u32 id = i + 0x10;
            sub_08077948(id);
            sub_08077948(id);
            sub_08077948(id);
            sub_08077948(id);
            sub_08077948(id);
        }
    } else {
        i = 0;
        do {
            i++;
            for (j = 0x13; j >= 0; j--)
                sub_08077948(i);
        } while (i <= 0x13);
    }
    return 1;
}

u32 sub_08074554(void) {
    gUnk_03000040.unk40E = 3;
    REG_DISPCNT = 0x140;
    sub_08073574();
    REG_BG0CNT = 5;
    sub_080757AC();
    sub_080759F4();
    sub_08075630();
    return 1;
}

void sub_08074594(void) {
    struct DateBits d;
    sub_08004914(&d);
    sub_08004358(d.a, d.b, d.c);
    sub_080044E4(d.a, d.b, d.c);
    sub_080042D8(d.a, d.b, d.c);
    sub_08072BB4(0x33, 0x807, 0x3C0, gUnk_08087B34);
    sub_08072C0C(0x08070033, 0x000403C0, d.a, 1);
    sub_08072C0C(0x08070038, 0x000203C5, d.b, 1);
    sub_08072C0C(0x0807003B, 0x000203C8, d.c, 1);
}

/* Draw the date/time-dependent flag lines of the debug menu. */
void sub_08074638(void) {
    s32 line = 2;
    u16 y = 0x3A0;
    struct DateBits d;
    u32 mask;
    u32 i;
    sub_08004914(&d);
    mask = sub_080044E4(d.a, d.b, d.c);
    if (gUnk_02011C20.unk2150 != 0 && (u16)__umodsi3(gUnk_02011C20.unk2150, 0x3C) == 0) {
        sub_08072BB4(0x42, 0x802, y, gUnk_08087B40);
        line = 3;
        y += 0x20;
    }
    for (i = 0; i <= 0x1C; i++) {
        if (gUnk_08087720[i].mask & mask) {
            sub_08072BB4((((u32)line << 16) >> 11) + 2, 0x804, y, gUnk_08087720[i].name);
            line++;
            y = y + 0x20;
        }
    }
}

void sub_080746F0(void) {
    switch (gUnk_02011C20.mode) {
    case 1:
        sub_08072BB4(0x21, 0x805, 0x3F0, gUnk_08087B58);
        break;
    case 0:
        sub_08072BB4(0x21, 0x805, 0x3F0, gUnk_08087B60);
        break;
    case 2:
        sub_08072BB4(0x21, 0x805, 0x3F0, gUnk_08087B68);
        break;
    case 3:
        sub_08072BB4(0x21, 0x805, 0x3F0, gUnk_08087B70);
        break;
    case 4:
        sub_08072BB4(0x21, 0x805, 0x3F0, gUnk_08087B78);
        break;
    }
}

/* Debug menu step 0: draw the item list; step 1: enable BG0/BG3; then fade in. */
u16 sub_08074794(void) {
    struct Main *m = &gUnk_03000040;
    switch (m->unk4859) {
    case 0: {
        s32 i;
        struct DebugItem *it, *first;
        u32 y;
        sub_080734D4();
        sub_080746F0();
        sub_08072C0C(0x08070025, 0x000403F8, 0x83C, 1);
        sub_0807289C(0, 0x27D);
        i = 0;
        first = gUnk_081A73A0;
        if (first->cb != 0) {
            it = first;
            y = 0x20;
            do {
                u32 col = 2;
                s32 row = i * 2 + 4;
                if (row > 0x13) {
                    col = 0x10;
                    row -= 0x10;
                }
                col |= (u32)(row << 16) >> 11;
                sub_08072BB4(col, 0x807, y, it->name);
                it++;
                y += 0x10;
                i++;
            } while (it->cb != 0);
        }
        sub_08074594();
        sub_08074638();
        gUnk_03000040.unk4859++;
        return 0;
    }
    case 1:
        REG_DISPCNT |= 0x1100;
        m->unk4859++;
        return 0;
    default:
        return sub_08075AE4(4);
    }
}

#if 0 /* NONMATCHING: body identical except register allocation (gMain pointer r5 vs r4, missing copy for the second gMain pointer) */
/* Debug menu input: up/down move the cursor, A selects, B goes to the last item, left/right/L/R change the value at save+0x2150. */
u32 sub_08074868(void) {
    struct Main *m;
    u32 k;
    struct Main *m2;
    u8 *step;
    u8 *tbl;
    u32 off;
    u32 col;
    s32 row;
    u16 keys;
    k = gUnk_03000040.newKeys & 0x80;
    m = &gUnk_03000040;
    if (k) {
        u8 *s1 = &m->unk4859;
        (*s1)++;
        if (gUnk_081A73A0[*s1].cb == 0)
            *s1 = 0;
    }
    if (m->newKeys & 0x40) {
        u8 *s2 = &m->unk4859;
        if (*s2 == 0) {
            if (gUnk_081A73A0[*s2].cb != 0) {
                u8 *p = s2;
                do {
                    (*p)++;
                } while (gUnk_081A73A0[*p].cb != 0);
            }
        }
        m->unk4859--;
    }
    col = 1;
    m2 = m;
    row = *step * 2 + 4;
    step = &m2->unk4859;
    if (row > 0x13) {
        col = 0xF;
        row -= 0x10;
    }
    sub_080761F0((col << 3) | (row << 19), 0, 2);
    keys = m2->newKeys;
    if (keys & 1)
        return 1;
    if (keys & 2) {
        *step = 9;
        return 1;
    }
    if (keys & 0x10) {
        gUnk_02011C20.unk2150++;
        m2->unk4857--;
        sub_080734D4();
        sub_08074594();
    }
    if (m->newKeys & 0x20) {
        if (gUnk_02011C20.unk2150 != 0) {
            gUnk_02011C20.unk2150--;
            m->unk4857--;
            sub_080734D4();
            sub_08074594();
        }
    }
    m = &gUnk_03000040;
    if (m->newKeys & 0x100) {
        gUnk_02011C20.unk2150 += 0x1E;
        sub_080734D4();
        sub_08074594();
    }
    if (m->newKeys & 0x200) {
        u16 *p = &gUnk_02011C20.unk2150;
        u32 v = *p;
        if (v > 0x1E)
            v -= 0x1E;
        else
            v = 0;
        *p = v;
        sub_080734D4();
        sub_08074594();
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080740BC", sub_08074868); /* 0x08074868 size 0x180 */

/* Debug menu final step: launch the selected item's callback. */
u32 sub_080749E8(void) {
    struct Main *m;
    u8 *items;
    u32 off;
    REG_DISPCNT = 0;
    items = (u8 *)gUnk_081A73A0;
    m = &gUnk_03000040;
    off = m->unk4859 * 0x44;
    items += 0x40;
    sub_080754F8(*(void **)(off + (u32)items));
    m->unk4857 = 0;
    gUnk_02017A30[0xB] = 0;
    return 0;
}

/* CB_DebugMenu (unused): runs the current step function; when it returns non-zero, advance to the next step. */
u32 sub_08074A34(void) {
    u16 (**tbl)(void) = gUnk_081A768C;
    struct Main *m = &gUnk_03000040;
    u8 *idx = &m->unk4857;
    u16 (*cb)(void) = tbl[*idx];
    if (cb != 0) {
        u16 r = cb();
        if (r != 0) {
            u32 s = *idx;
            if (s <= 1) {
                m->unk4858 = 0;
                m->unk4859 = 0;
                m->unk485A = 0;
                m->unk485B = 0;
            }
            *idx = s + 1;
        }
        return 0;
    }
}

/* Maps ASCII 0x20..0x7E to its full-width Shift-JIS code, and anything else to 0. */
u16 sub_08074A90(u16 ch) {
    u32 x = ch - 0x20;
    if ((u16)x <= 0x5E)
        return gUnk_081A76A0[x];
    return 0;
}

/* Length (in visible characters) of the next word: stops at NUL, space, newline or "\n"; "@0", "@2", "@3" colour codes are zero-width. */
u8 sub_08074AB4(const u8 *p) {
    s32 n = 0;
    while (*p != 0) {
        switch (*p) {
        case 0x5C:
            if (p[1] != 0x6E)
                break;
            goto done;
        case 0x40:
            switch (p[1]) {
            case 0x30:
            case 0x32:
            case 0x33:
                p++;
                n--;
            }
            break;
        case 0:
        case 0x20:
        case 0xA:
            goto done;
        }
        p++;
        n++;
    }
done:
    return n;
}

/* Text canvas init: width/height in tiles, clear the 64KiB EWRAM canvas. */
void sub_08074B08(u32 w, u32 h) {
    gUnk_02000000[0x10000] = w;
    gUnk_02000000[0x10001] = h;
    gUnk_02000000[0x10004] = 0;
    sub_08075278(gUnk_02000000, 0x10000);
}

void sub_08074B38(u32 w, u32 h, u16 flag, u32 val) {
    u8 *p;
    gUnk_02000000[0x10000] = w;
    gUnk_02000000[0x10001] = h;
    p = &gUnk_02000000[0x10004];
    *p = (val & 0x7F) | (flag << 7);
    sub_08075278(gUnk_02000000, 0x10000);
}

void sub_08074B74(u8 bits, s32 x, s32 y, u32 color) {
    s32 xl = ((short)x) & 7; u8 yl = y & 7;
    s32 xt = x >> 3;
    s32 yt = y >> 3;
    s32 *new_var;
    s32 i;
    for (i = 0; i <= 7; i++) {
        if ((0x80 >> i) & bits)
            gUnk_02000000[xl + yl * 8 + ((*(new_var = &xt) + gUnk_02000000[0x10000] * yt) << 6)] = color;
        xl++;
        if (xl > 7) {
            xl = 0;
            xt++;
        }
    }
}

void sub_08074BF8(u16 bits, s32 x, s32 y, u32 color) {
    s32 xl = ((short)x) & 7; u8 yl = y & 7;
    s32 xt = x >> 3;
    s32 yt = y >> 3;
    s32 *new_var;
    s32 i;
    for (i = 0; i <= 15; i++) {
        if ((0x8000 >> i) & bits)
            gUnk_02000000[xl + yl * 8 + ((*(new_var = &xt) + gUnk_02000000[0x10000] * yt) << 6)] = color;
        xl++;
        if (xl > 7) {
            xl = 0;
            xt++;
        }
    }
}

/* Draw a Shift-JIS glyph (size 8/10/12 = high byte of sc, colour = low byte) at (x, y). */
void sub_08074C80(u16 sjis, s32 x, s32 y, u16 sc) {
    u8 size = sc >> 8;
    u8 color = sc;
    const u16 *p;
    s32 i;
    u32 n;
    switch (size) {
    case 8:
        p = (const u16 *)(sub_08072584(sjis) * 8 + (u32)gUnk_081C0000);
        for (i = 3; i >= 0; i--) {
            u32 v = *p++;
            v <<= 17;
            sub_08074B74((v << 8) >> 24, x, y++, color);
            sub_08074B74(v >> 24, x, y++, color);
        }
        break;
    case 10:
        p = (const u16 *)(sub_08072584(sjis) * 20 + (u32)gUnk_081D0200);
        goto rows;
    case 12:
        p = (const u16 *)(sub_08072584(sjis) * 24 + (u32)gUnk_081F8700);
    rows:
        n = size;
        if (n != 0) {
            i = n;
            do {
                u16 w = *p++;
                u16 sw = (w >> 8) | ((u8)w << 8);
                sub_08074BF8((u16)(sw << 1), x, y++, color);
            } while (--i != 0);
        }
        break;
    }
}

void sub_08074D48(u8 ch, s32 x, s32 y, u16 sc) {
    u8 size = sc >> 8;
    u8 color = sc;
    const u16 *p;
    u32 off;
    u32 base;
    s32 i;
    u32 n;
    switch (size) {
    case 8:
        off = ch * 8;
        base = (u32)gUnk_08228D00;
        p = (const u16 *)(off + base);
        for (i = 3; i >= 0; i--) {
            u32 v = *p++;
            v <<= 17;
            sub_08074B74((v << 8) >> 24, x, y++, color);
            sub_08074B74(v >> 24, x, y++, color);
        }
        break;
    case 10:
        off = ch * 10;
        base = (u32)gUnk_08229500;
        goto rows;
    case 12:
        off = ch * 12;
        base = (u32)gUnk_08229F00;
        goto rows;
    case 16:
        off = ch * 16;
        base = (u32)gUnk_0822AB00;
    rows:
        p = (const u16 *)(off + base);
        n = size >> 1;
        if (n != 0) {
            u32 cnt = n;
            do {
                u32 v = *p++;
                v <<= 17;
                sub_08074B74((v << 8) >> 24, x, y++, color);
                sub_08074B74(v >> 24, x, y++, color);
            } while (--cnt != 0);
        }
        break;
    }
}

/* Draw a glyph: Shift-JIS renderer when the save's Japanese-font flag is set, otherwise the Latin one. */
void sub_08074E20(u16 ch, s32 x, s32 y, u16 sc) {
    if (gUnk_02011C20.jpFont != 0)
        sub_08074C80(ch, x, y, sc);
    else
        sub_08074D48(ch, x, y, sc);
}

/* Draw a Shift-JIS string at (x, y) with word wrap; tracks the max x/y extents in the canvas header (+2/+3). */
void sub_08074E60(s32 x0, s32 y0, u16 sc, const u8 *str) {
    s32 size = sc >> 8;
    s32 x = x0;
    s32 y = y0;
    s32 startX = x;
    gUnk_02000000[0x10002] = 0;
    gUnk_02000000[0x10003] = 0;
    if (*str != 0) {
        do {
            u32 c = (str[0] << 8) | str[1];
            if (gUnk_02000000[0x10004] & 0x80) {
                if (x + size * 3 > gUnk_02000000[0x10000] * 8) {
                    if (sub_080728C0(c) == 0)
                        goto wrap;
                }
                if (x + size * 4 > gUnk_02000000[0x10000] * 8) {
                    if (sub_080729F8(c) != 0) {
                    wrap:
                        x = startX;
                        y += size + (((u32)gUnk_02000000[0x10004] << 25) >> 25);
                    }
                }
            }
            sub_08074C80(c, x, y, sc);
            if (gUnk_02000000[0x10002] < x + size)
                gUnk_02000000[0x10002] = x + size;
            if (gUnk_02000000[0x10003] < y + size)
                gUnk_02000000[0x10003] = y + size;
            x += size;
            str += 2;
        } while (*str != 0);
    }
}

/* Draw a Latin string at (x, y) with word wrap; tracks the max x/y extents in the canvas header (+2/+3). */
void sub_08074F50(s32 x0, s32 y0, u16 sc, const u8 *str) {
    s32 size = sc >> 8;
    s32 x = x0;
    s32 y = y0;
    s32 startX = x;
    gUnk_02000000[0x10002] = 0;
    gUnk_02000000[0x10003] = 0;
    if (*str != 0) {
        do {
            if (gUnk_02000000[0x10004] & 0x80) {
                s32 w = sub_08074AB4(str);
                if (x + ((w * size) >> 1) > (gUnk_02000000[0x10000] - 2) * 8) {
                    x = startX;
                    y += size + (((u32)gUnk_02000000[0x10004] << 25) >> 25);
                }
            }
            sub_08074D48(*str, x, y, sc);
            if (gUnk_02000000[0x10002] < x + (s32)((u32)size >> 1))
                gUnk_02000000[0x10002] = x + (s32)((u32)size >> 1);
            if (gUnk_02000000[0x10003] < y + size)
                gUnk_02000000[0x10003] = y + size;
            x += (u32)size >> 1;
            if (size == 0x10)
                x++;
            str++;
        } while (*str != 0);
    }
}

/* Draw a string: Shift-JIS path when the save's Japanese-font flag is set, otherwise Latin. */
void sub_0807501C(s32 x, s32 y, u16 sc, const u8 *str) {
    if (gUnk_02011C20.jpFont != 0)
        sub_08074E60(x, y, sc, str);
    else
        sub_08074F50(x, y, sc, str);
}

/* Draw a decimal number right-aligned at x (least significant digit first, moving left) using full-width Shift-JIS digits. */
void sub_08075050(s32 x, s32 y, u16 sc, s32 value) {
    s32 size = sc >> 8;
    do {
        sub_08074C80(value % 10 + 0x824F, x, y, sc);
        x -= size;
        value /= 10;
    } while (value != 0);
}

/* Same with Latin digits (advance is half the font size). */
void sub_0807509C(s32 x, s32 y, u16 sc, s32 value) {
    s32 half = sc >> 9;
    do {
        sub_08074D48(value % 10 + 0x30, x, y, sc);
        x -= half;
        value /= 10;
    } while (value != 0);
}

