#include "global.h"

/* Duel screen block at 0x0201CFB0 (see code-08051a9c / code-0805d58c). */
struct DuelScreen {
    u8 pad0[0x808];
    u8 f808_0 : 1;
    u8 f808_1 : 1;
    u8 f808_rest : 6;
    u8 pad809[0x824 - 0x809];
    u32 player;     /* +0x824 */
    u32 mode;       /* +0x828 */
    u32 index;      /* +0x82C */
};
extern struct DuelScreen gUnk_0201CFB0;
extern u8 gUnk_0201930C[];
struct ZoneW {
    u32 w;
    u8 pad[0x90];
};
#define CARD_ID(w) (((w) << 20) >> 20)
/* ROM card stats table through an integer-constant pointer (the ROM reloads the address at every use). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* AI/UI value of a card: 0 for Magic/Trap/Ritual (types 0x15-0x17), 4000 for type 0x18, else field * 10. */
static inline u16 CardAtk(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    }
    return ((CARD_STATS(id) << 14) >> 23) * 10;
}
static inline u16 CardDef(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 4000;
    }
    return (CARD_STATS(id) & 0x1FF) * 10;
}
/* Monster level: Magic/Trap types count as 0, type 0x18 as 10. */
static inline u16 CardLevel(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        return 0;
    case 0x18:
        return 10;
    }
    return (CARD_STATS(id) & 0x1E000000) >> 25;
}
/* Card info block filled by sub_0800ABC8 (12 bytes). */
struct CardInfo {
    u16 id;         /* +0 */
    u8 attr : 5;    /* byte +2 bits 0-4 (monster attribute? index into gUnk_081A41A4) */
    u8 kind : 3;    /* byte +2 bits 5-7 (index into gUnk_081A41F8) */
    u32 atk;
    u32 def;
};
void sub_0800ABC8(int player, int slot, struct CardInfo *out);
void sub_0805F074(u16 id, u16 flag);
void sub_0805EF00(int a, u16 b, u16 *hdr);
extern const u8 gUnk_0808649C[];
extern const u8 gUnk_080864A4[];
extern const u8 gUnk_080864AC[];
extern const u8 gUnk_080864B4[];
extern u16 *const gUnk_081A41A4[];
extern u16 *const gUnk_081A41F8[];
/* One duel field zone (0x94 bytes); only byte +6 and the word at +0x90 are used here. */
struct UiZone {
    u8 pad0[6];
    u8 f6_0 : 1;
    u8 f6_1 : 1;        /* bit 1: "face-down" / evaluation flag (hypothesis) */
    u8 f6_2 : 4;        /* bits 2-5: counter shown as number sprites */
    u8 f6_6 : 2;
    u8 pad7[0x90 - 7];
    u32 w90;
};
extern u8 gUnk_0201930C[];
extern const u8 gUnk_08086490[];
extern const u8 gUnk_08086470[];
extern const u8 gUnk_08086478[];
extern const u8 gUnk_08086480[];
extern const u8 gUnk_08086488[];
/* gMain (0x03000040): 64 halfwords at +0x309C (used by sub_0805ED78). */
struct MainTbl {
    u8 pad[0x309C];
    u16 tbl[0x40];
};
extern struct MainTbl gUnk_03000040;
struct MainMap {
    u8 pad[0x2C1C];
    u16 map[0x400];
};
extern struct MainMap gUnk_03000040_m asm("gUnk_03000040");
void sub_080752B0(void *dst, const void *src, u32 size);
/* Text/tile buffer of 64 cells of 0x20 bytes (8x8 4bpp tiles) at 0x0201CFB8. */
extern u8 gUnk_0201CFB8[];
void sub_08072778(u32 ch, void *dst, u32 pal, u32 bpp);
void sub_08072808(u32 ch, void *dst, u32 pal, u32 bpp);
void sub_08075294(void *dst, const void *src, u32 size);
extern const u8 gUnk_0822C720[];
int sub_080753E0(const u8 *s);
void sub_08074B08(u32 w, u32 h);
void sub_0807501C(int x, int y, u16 attr, const u8 *str);
void sub_08075114(void *dst, u16 value);
void sub_0805EE30(int first, const u8 *str, u32 pal);
void sub_0805EE78(int first, int val, u32 pal, int digits);
void sub_0805EEDC(int a, u16 b);
void sub_080750E0(int x, int y, u16 attr, int val);
/* Save-mirror header at 0x02011C20: bit 7 of byte +4 selects double-width (2-byte) characters. */
struct SaveHdr {
    u8 pad[4];
    u8 flags;
};
extern struct SaveHdr gUnk_02011C20;

/* Transition state at 0x02018450 (+0x15C step, +0x15D sub-step, +0x15F timer). */
struct ScnE788 { u8 pad[0x15C]; u8 step; u8 sub; u8 unk15E; u8 t; };
extern struct ScnE788 gUnk_02018450;
struct KeysE788 { u8 pad[4]; u16 held; u16 pressed; };
extern struct KeysE788 gKeysE788 asm("gUnk_03000040");
/* Per-side pointers to the BG offset words shaken during the transition. */
extern s32 *const gUnk_081A4194[];
extern void (*IntrTable[])(void);
void sub_080757F4(void);
void sub_0805DD3C(void);
void sub_0805DEA4(u16 flags, s32 *pos, int mode);
void sub_0805DF04(int side, int delta, int mode);
s32 sub_08076F9C(void);
u16 sub_08075A6C(int speed);
void sub_08077AEC(int se);
#define E788_FAST ((gKeysE788.held & 2) || (((u8 *)&gUnk_0201CFB0)[0] & 1))
#if 0 /* NONMATCHING: 115 lines; structure and jump tables right, register choices differ (ROM uses r6 as a scratch for constants) */
/* Screen transition (fade, shake and blend) run once per frame; returns 1 when finished (hypothesis).
 * Holding B (or the fast flag) speeds up every timer. */
u32 sub_0805E788(u16 a, u16 b, u16 flags)
{
    s32 pos[2];
    int k;
    u8 v;

    pos[0] = a;
    pos[1] = b;
    switch (gUnk_02018450.step) {
    case 0:
        *(vu16 *)0x04000000 |= 0x1C00;
        *(vu16 *)0x04000050 = 0x3FFF;
        gUnk_02018450.step++;
    case 1:
        *(vu16 *)0x04000054 = 0xF - gUnk_02018450.sub;
        v = gUnk_02018450.sub;
        if (v <= 0xE) {
            gUnk_02018450.sub++;
            if (E788_FAST && gUnk_02018450.sub <= 0xB)
                gUnk_02018450.sub += 3;
        } else {
            gUnk_02018450.step++;
        }
        return 0;
    case 2:
        sub_080757F4();
        *(vu16 *)0x04000208 = 0;
        *(vu16 *)0x04000200 &= ~2;
        *(vu16 *)0x04000208 = 1;
        *(vu16 *)0x04000208 = 0;
        *(vu16 *)0x04000200 &= ~2;
        IntrTable[1] = 0;
        *(vu16 *)0x04000208 = 1;
        sub_0805DD3C();
        gUnk_02018450.sub = 0;
        gUnk_02018450.unk15E = 0;
        gUnk_02018450.t = 0;
        gUnk_02018450.step++;
    case 3:
        switch (gUnk_02018450.sub) {
        case 0:
            if (flags == 0) {
                gUnk_02018450.step++;
                return 0;
            }
            gUnk_02018450.sub++;
        case 1:
            if (gUnk_02018450.t <= 0x1D) {
                sub_0805DEA4(flags, pos, 0);
                gUnk_02018450.t++;
                if (E788_FAST && gUnk_02018450.t <= 0x15)
                    gUnk_02018450.t += 7;
                break;
            }
            sub_08077AEC(9);
            gUnk_02018450.t = 0;
            gUnk_02018450.sub++;
        case 2:
            sub_0805DEA4(flags, pos, 1);
            if (gUnk_02018450.t <= 0x1D) {
                for (k = 0; k <= 1; k++) {
                    if ((6 << (k * 8)) & flags) {
                        *gUnk_081A4194[k * 2] = ((sub_08076F9C() % 16) - 8) << 8;
                        *gUnk_081A4194[k * 2 + 1] = ((sub_08076F9C() % 16) - 8) << 8;
                    }
                    if ((4 << (k * 8)) & flags)
                        sub_0805DF04(k, pos[1 - k] - pos[k], 1);
                }
                gUnk_02018450.t++;
                if (E788_FAST && gUnk_02018450.t <= 0x15)
                    gUnk_02018450.t += 7;
                break;
            }
            for (k = 0; k <= 1; k++) {
                if ((6 << (k * 8)) & flags) {
                    *gUnk_081A4194[k * 2] = 0;
                    *gUnk_081A4194[k * 2 + 1] = 0;
                }
            }
            gUnk_02018450.t = 0;
            gUnk_02018450.sub++;
        case 3:
            sub_0805DEA4(flags, pos, 1);
            v = gUnk_02018450.t;
            if (v <= 0x1F) {
                *(vu16 *)0x04000050 = 0xC0;
                if (flags & 2)
                    *(vu16 *)0x04000050 |= 0x404;
                if (flags & 0x200)
                    *(vu16 *)0x04000050 |= 0x808;
                *(vu16 *)0x04000054 = gUnk_02018450.t;
                gUnk_02018450.t++;
                if (E788_FAST && gUnk_02018450.t <= 0x15)
                    gUnk_02018450.t += 7;
                if (gUnk_02018450.t == 0x20) {
                    if (flags & 2)
                        *(vu16 *)0x04000000 &= 0xFBFF;
                    if (flags & 0x200)
                        *(vu16 *)0x04000000 &= 0xF7FF;
                }
                for (k = 0; k <= 1; k++) {
                    if ((4 << (k * 8)) & flags)
                        sub_0805DF04(k, pos[1 - k] - pos[k], 0);
                }
                break;
            }
            *(vu16 *)0x04000050 = 0;
            *(vu16 *)0x04000054 = 0;
            gUnk_02018450.t = 0;
            gUnk_02018450.sub++;
        case 4:
            sub_0805DEA4(flags, pos, 1);
            for (k = 0; k <= 1; k++) {
                if ((4 << (k * 8)) & flags)
                    sub_0805DF04(k, pos[1 - k] - pos[k], 0);
            }
            if (gKeysE788.pressed & 2) {
                gUnk_02018450.step++;
            } else {
                v = gUnk_02018450.t;
                if (v <= 0x3B) {
                    gUnk_02018450.t++;
                    if (E788_FAST && gUnk_02018450.t <= 0x33)
                        gUnk_02018450.t += 7;
                } else {
                    gUnk_02018450.step++;
                }
            }
            break;
        }
        return 0;
    case 4:
        sub_0805DEA4(flags, pos, 1);
        for (k = 0; k <= 1; k++) {
            if ((4 << (k * 8)) & flags)
                sub_0805DF04(k, pos[1 - k] - pos[k], 0);
        }
        if (sub_08075A6C(E788_FAST ? 4 : 1))
            gUnk_02018450.step++;
        return 0;
    default:
        return 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0805E788", sub_0805E788); /* 0x0805E788 size 0x574 */
/* Card id (12 bits) of the card the duel screen cursor points at: zone `mode + index` of `player`
 * for modes 0/5/10, hand slot `index` for mode 11, else 0. */
u32 sub_0805ECFC(void)
{
    struct DuelScreen *sc = &gUnk_0201CFB0;
    int player = sc->player;
    int mode = sc->mode;
    int index = sc->index;
    int off = (player & 1) * 0xD64;
    u8 *z = gUnk_0201930C;
    /* FAKEMATCH: pinning the base and the byte offset to r2/r0 makes agbcc emit the ROM's
     * `adds r0, r2, r0` operand order for the final zone-address add. */
    register u8 *bp asm("r2") = off + z;
    register u32 mi asm("r0") = (mode + index) * 0x94;
    u32 id;

    id = CARD_ID(*(u32 *)((u32)bp + mi));

    switch (mode) {
    case 0:
    case 5:
    case 10:
        return id;
    case 11: {
        u8 *h = z + 11 * 0x94;
        u8 *hp = off + h;
        int hoff = index * 4;
        id = CARD_ID(*(u32 *)(hp + hoff));
        return id;
    }
    }
    return 0;
}
/* Fills 64 halfwords with consecutive values 0x28E..0x2CD. */
void sub_0805ED78(void)
{
    u8 *b = (u8 *)&gUnk_03000040;
    int i = 0x3F;
    int v = 0x2CD;
    u16 *p = (u16 *)(b + 0x311A);

    for (; i >= 0; i--) {
        *p = v;
        v--;
        p--;
    }
}
/* Clears all 64 cells of the text buffer and marks the duel screen dirty. */
void sub_0805ED9C(void)
{
    int i;
    u8 *cell = gUnk_0201CFB8;

    for (i = 0x3F; i >= 0; i--) {
        sub_08072778(0x20, cell, 0, 9);
        cell += 0x20;
    }
    gUnk_0201CFB0.f808_0 = 1;
    gUnk_0201CFB0.f808_1 = 1;
    sub_0805ED78();
}
/* Draws a halfword-character string (byte-swapped chars) into the buffer from cell `first`. */
void sub_0805EDE0(int first, u16 *str, u32 pal)
{
    while (*(u8 *)str != 0) {
        u16 c = *str;
        u32 hi = c >> 8;
        u32 lo = (u8)c << 8;
        sub_08072808(lo | hi, gUnk_0201CFB8 + first++ * 0x20, pal, 9);
        str++;
    }
    gUnk_0201CFB0.f808_0 = 1;
    gUnk_0201CFB0.f808_1 = 1;
}
/* Draws a byte-character string into the buffer from cell `first`. */
void sub_0805EE30(int first, const u8 *str, u32 pal)
{
    while (*str != 0) {
        sub_08072778(*str, gUnk_0201CFB8 + first++ * 0x20, pal, 9);
        str++;
    }
    gUnk_0201CFB0.f808_0 = 1;
    gUnk_0201CFB0.f808_1 = 1;
}
/* Draws `val` as up to `digits` decimal digits right-aligned at cell `first + digits - 1`. */
void sub_0805EE78(int first, int val, u32 pal, int digits)
{
    first += digits;
    while (digits > 0) {
        sub_08072778(val % 10 + 0x30, gUnk_0201CFB8 + --first * 0x20, pal, 9);
        val /= 10;
        if (val == 0)
            return;
        digits--;
    }
    gUnk_0201CFB0.f808_0 = 1;
    gUnk_0201CFB0.f808_1 = 1;
}
/* Copies one cell of the text buffer to tile `b` of the OBJ/BG tile area at 0x06004000. */
void sub_0805EEDC(int a, u16 b)
{
    sub_08075294(gUnk_0201CFB8 + a * 0x20, (void *)(0x06004000 + b * 0x20), 0x20);
}
/* Loads a palette+tile block `hdr` (u16 count of palette halfwords at +0, palette at +8, then a
 * tile count and two tile sets) into the text buffer cell `a`, palette slot `b` and patches the four
 * BG map entries of that cell to palette `b & 15`. */
#if 0 /* NONMATCHING: register allocation only (ROM: `cnt` r6 / `cell` r5, map base r8, map-entry pointers r5/r4/r3/r1; the build swaps `cnt`/`cell` and spills the map-entry pointers) */
void sub_0805EF00(int a, u16 b, u16 *hdr)
{
    u16 off = hdr[0] * 2;
    u16 *cnt = (u16 *)((u8 *)hdr + (off + 8));
    u8 *src = (u8 *)hdr + (off + 0x10);
    u8 cell = a << 5;
    u8 *buf = gUnk_0201CFB8;
    u32 n = *cnt;

    if (hdr != 0) {
        u8 pal;
        u16 *m, *e0, *e1, *e2, *e3;
        s16 a0, a1;

        sub_08075294(buf + cell, src, n * 16);
        sub_08075294(buf + 0x400 + cell, src + *cnt * 16, *cnt * 16);
        sub_080752B0((void *)(0x05000000 + b * 32), (u8 *)hdr + 8, hdr[0] * 2);
        pal = ((u32)b << 28) >> 16;
        m = (u16 *)((u8 *)&gUnk_03000040 + 0x2C1C);
        a0 = a;
        a1 = a + 1;
        e0 = &m[a0 + 0x240];
        e1 = &m[a1 + 0x240];
        e2 = &m[a0 + 0x260];
        e3 = &m[a1 + 0x260];
        *e0 &= 0xFFF;
        *e1 &= 0xFFF;
        *e2 &= 0xFFF;
        *e3 &= 0xFFF;
        *e0 |= pal;
        *e1 |= pal;
        *e2 |= pal;
        *e3 |= pal;
        buf[0x800] &= ~3;
    }
}
#else
INCLUDE_ASM("asm/nonmatching/code_0805E788", sub_0805EF00); /* 0x0805EF00 size 0x10C */
#endif
/* Draws a centred 2-row message box: the string `n` of the table at 0x0822C720 (64 bytes each). */
void sub_0805F00C(u16 n)
{
    const u8 *str = gUnk_0822C720 + (n << 6);
    int len = sub_080753E0(str);
    u32 w = 12;
    int x;

    if (len > 0xF)
        w = 10;
    sub_08074B08(0x20, 2);
    x = (int)(len * w) >> 1;
    sub_0807501C(0x78 - x, 9 - (w >> 1), (w << 8) | 8, str);
    sub_0807501C(0x77 - x, 8 - (w >> 1), (w << 8) | 7, str);
    sub_08075114(gUnk_0201CFB8, 9);
}
/* Card info header: name box (string `id`), then for monster cards (type <= 0x14) with `flag` the
 * ATK / DEF / level numbers and labels. */
void sub_0805F074(u16 id, u16 flag)
{
    /* FAKEMATCH: pinning the byte offset to r0 makes agbcc emit the ROM's lsls-before-ldr order
     * (harmless: it only fixes register/schedule allocation of the name-string address). */
    register u32 off asm("r0") = id << 6;
    const u8 *tbl = gUnk_0822C720;
    const u8 *str = tbl + off;
    int len = sub_080753E0(str);
    u32 w = 12;
    int v;

    if (len > 0xC)
        w = 10;
    sub_08074B08(0x20, 2);
    sub_0807501C(3, 9 - (w >> 1), (w << 8) | 8, str);
    sub_0807501C(2, 8 - (w >> 1), (w << 8) | 7, str);
    sub_08075114(gUnk_0201CFB8, 9);
    if (CARD_TYPE(id) <= 0x14 && flag != 0) {
        if ((gUnk_02011C20.flags & 0x7F) == 0) {
            sub_0805EE30(0x17, gUnk_08086470, 5);
            sub_0805EE30(0x37, gUnk_08086478, 4);
        } else {
            sub_0805EE30(0x17, gUnk_08086480, 5);
            sub_0805EE30(0x37, gUnk_08086488, 4);
        }
        v = CardAtk(id);
        sub_0805EE78(0x1A, v, 7, 4);
        v = CardDef(id);
        sub_0805EE78(0x3A, v, 7, 4);
        sub_0805EEDC(0x18, 3);
        v = CardLevel(id);
        sub_0805EE78(0x37, v, 7, 2);
    }
}
extern const u16 gUnk_08622AB4[];
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
/* Duel card detail (zone view): name box, a counter box for the special cards 0x47/0x15B/0x4CE, and the
 * ATK/DEF/level numbers; cards 0x479/0x5A8 also show an icon block. */
#if 0 /* NONMATCHING: control flow identical; broad register-allocation differences (name-string address, the zone-address products use r2/r1 swapped, `x`/`shown` allocation); it is the base for the permuter */
void sub_0805F270(u16 id, u16 flag, int player, int slot)
{
    const u8 *str = gUnk_0822C720 + (id << 6);
    s16 len = sub_080753E0(str);
    u32 w = 12;
    int x = 0xEC;
    int shown = 0;
    int pl;
    struct UiZone *z;
    int v;

    if (len > 0xF)
        w = 10;
    sub_08074B08(0x20, 2);
    sub_0807501C(3, 9 - (w >> 1), (w << 8) | 8, str);
    sub_0807501C(2, 8 - (w >> 1), (w << 8) | 7, str);
    pl = player & 1;
    z = (struct UiZone *)(slot * 0x94 + pl * 0xD64 + (u32)gUnk_0201930C);
    if (z->f6_1) {
        int n = CARD_NUMBER(id);
        switch (n) {
        case 0x47:
        case 0x15B:
        case 0x4CE: {
            u32 t;
            pl = player & 1;
            z = (struct UiZone *)(slot * 0x94 + pl * 0xD64 + (u32)gUnk_0201930C);
            x -= 0x3C;
            t = z->f6_2;
            if ((int)t <= 9) {
                sub_080750E0(x + 1, 4, 0xA08, t);
                sub_080750E0(x, 3, 0xA07, t);
                sub_0807501C(x + 0xB, 4, 0xA08, gUnk_08086490);
                sub_0807501C(x + 0xA, 3, 0xA04, gUnk_08086490);
            } else {
                x -= 0xA;
                sub_080750E0(x + 0xB, 4, 0xA08, t);
                sub_080750E0(x + 0xA, 3, 0xA07, t);
                sub_0807501C(x + 0x15, 4, 0xA08, gUnk_08086490);
                sub_0807501C(x + 0x14, 3, 0xA04, gUnk_08086490);
            }
            shown = 1;
            break;
        }
        }
    }
    sub_08075114(gUnk_0201CFB8, 9);
    if (CARD_TYPE(id) <= 0x14 && flag != 0 && shown == 0) {
        if ((gUnk_02011C20.flags & 0x7F) == 0) {
            sub_0805EE30(0x17, gUnk_08086470, 5);
            sub_0805EE30(0x37, gUnk_08086478, 4);
        } else {
            sub_0805EE30(0x17, gUnk_08086480, 5);
            sub_0805EE30(0x37, gUnk_08086488, 4);
        }
        v = CardAtk(id);
        sub_0805EE78(0x1A, v, 7, 4);
        v = CardDef(id);
        sub_0805EE78(0x3A, v, 7, 4);
        sub_0805EEDC(0x18, 3);
        v = CardLevel(id);
        sub_0805EE78(0x37, v, 7, 2);
    }
    pl = player & 1;
    z = (struct UiZone *)(slot * 0x94 + pl * 0xD64 + (u32)gUnk_0201930C);
    if (z->f6_1) {
        int n = CARD_NUMBER(id);
        if (n == 0x479)
            sub_0805EF00(0x1C, 9, gUnk_081A41A4[(z->w90 << 14) >> 27]);
        else if (n == 0x5A8)
            sub_0805EF00(0x1C, 9, gUnk_081A41F8[(z->w90 << 14) >> 27]);
    }
}
#else
INCLUDE_ASM("asm/nonmatching/code_0805E788", sub_0805F270); /* 0x0805F270 size 0x3DC */
#endif
/* Draws a two-row text box frame (rows of string `str`, palette/size byte `a`) and the number `val`
 * centred: `cnt` is the extra width in characters, each digit of `val` adds 1 (2 with 2-byte chars). */
void sub_0805F64C(int a, const u8 *str, int val, int cnt)
{
    sub_08074B08(0x20, 2);
    sub_0807501C(3, 9 - a / 2, ((u8)a << 8) | 0xD, str);
    sub_0807501C(2, 8 - a / 2, ((u8)a << 8) | 5, str);
    if (cnt > 0) {
        int v = val;
        int x;

        if (v > 9) {
            u8 dbl = gUnk_02011C20.flags & 0x80;
            do {
                cnt++;
                if (dbl)
                    cnt++;
                v /= 10;
            } while (v > 9);
        }
        x = a * cnt / 2;
        sub_080750E0(x + 3, 9 - a / 2, ((u8)a << 8) | 8, val);
        x += 2;
        sub_080750E0(x, 8 - a / 2, ((u8)a << 8) | 7, val);
    }
    sub_08075114(gUnk_0201CFB8, 9);
}
/* Card detail panel for the card in (player, slot): name, labels, ATK/DEF (palette 7 when equal to the
 * printed value, else 6), level and the attribute / kind icons. */
void sub_0805F728(int player, int slot)
{
    struct CardInfo info;
    int atk;
    int def;
    int x;
    u32 pal;

    sub_0800ABC8(player, slot, &info);
    sub_0805F074(info.id, 0);
    if ((gUnk_02011C20.flags & 0x7F) == 0) {
        sub_0805EE30(0x18, gUnk_0808649C, 5);
        sub_0805EE30(0x38, gUnk_080864A4, 4);
    } else {
        sub_0805EE30(0x18, gUnk_080864AC, 5);
        sub_0805EE30(0x38, gUnk_080864B4, 4);
    }
    atk = info.atk;
    pal = CardAtk(info.id) != info.atk ? 6 : 7;
    sub_0805EE78(0x19, atk, pal, 5);
    def = info.def;
    pal = CardDef(info.id) != info.def ? 6 : 7;
    sub_0805EE78(0x39, def, pal, 5);
    sub_0805EEDC(0x17, 3);
    sub_0805EE78(0x36, CardLevel(info.id), 7, 2);
    x = 0x13;
    if (CardLevel(info.id) > 9)
        x--;
    if (info.attr != 0 && info.attr <= 0x14)
        sub_0805EF00(x, 9, gUnk_081A41A4[info.attr]);
    if (info.kind != 0 && info.kind <= 6)
        sub_0805EF00(x + 2, 10, gUnk_081A41F8[info.kind]);
}
