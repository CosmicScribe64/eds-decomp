#include "global.h"

/* Menu block 0x0201AE60 (see code_0804EFF0) */
struct Ui {
    u8 u0[8];
    u16 x;          /* +8 */
    u8 uA[4];
    u16 h;          /* +0xE */
    u8 u10[4];
    u16 sel;        /* +0x14 */
    u8 u16_[0x21 - 0x16];
    u8 b21;
    u8 state;       /* +0x22 */
    u8 timer;       /* +0x23 */
};
extern struct Ui gUnk_0201AE60;
struct MainKeys { u8 u0[4]; u16 keysHeld; u16 keys; };
extern struct MainKeys gUnk_03000040;
/* Duel global 0x020192E0 */
struct Duel {
    u8 u0[0x1B50];
    u8 b1B50;
    u8 pad;
    u16 h1B52[2];       /* +0x1B52 candidate card ids */
    u8 u1B56[0x1B62 - 0x1B56];
    u8 step;            /* +0x1B62 */
    u8 pad2;
    u16 sel;            /* +0x1B64 */
    u16 sel2;           /* +0x1B66 */
};
extern struct Duel gUnk_020192E0;
extern u16 gUnk_0201AE44;
extern const u8 gUnk_0808628C[], gUnk_08086290[];
extern const char *const gUnk_0819D264[];
void sub_080752D0(char *dst, const void *src);
void sub_080752E8(char *dst, const void *src);
int sub_08076F9C(void);
void sub_0805FBA4(void);
int sub_08052668(void);
struct Player { u8 unk0[2]; u8 handCount; u8 pad[0x684 - 3]; u32 hand[80]; u8 pad2[0xD64 - 0x684 - 0x140]; };
struct DuelP { u8 pad[4]; struct Player players[2]; };
struct Q1 { u8 pad[9]; u8 f0 : 1; u8 rest : 7; };
struct DuelQ { u8 pad[0xD68]; struct Q1 q1; };
extern const u32 gUnk_08621DE0[];
#define CARD_STATS(id) (*(gUnk_08621DE0 + ((id) & 0x7FF)))
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_LEVEL(id, r)                                     \
    switch ((int)CARD_TYPE(id)) {                             \
    case 0x15:                                                \
    case 0x16:                                                \
    case 0x17:                                                \
        r = 0;                                                \
        break;                                                \
    case 0x18:                                                \
        r = 10;                                               \
        break;                                                \
    default:                                                  \
        r = (CARD_STATS(id) & 0x1E000000) >> 25;              \
        break;                                                \
    }
int sub_08052F38(u32 mask);
int sub_080578F4(void);
int sub_08008AF8(int player, int a);
int sub_080563B8(int a, int b);
void sub_08017FF4(int a, int b);
int sub_08054398(int player, int id);
int sub_08007834(int id);
int sub_08008A44(int player);
void sub_08077AEC(u16 se);
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);
void sub_08075294(u32 dst, const void *src, u32 n);
void sub_08074B08(u32 a, u32 b);
void sub_0807501C(u32 a, u32 b, u32 c, const char *s);
void sub_08075114(u32 a, u32 b);
extern const u8 gUnk_0822C300[];
extern const char *const gUnk_0819D214[];
extern struct Player gUnk_020192E4[];
extern u8 gUnk_0201AE42;
struct ActBlk { u8 u0[0x4FC]; u8 b4FC; u8 count; };
extern struct ActBlk gUnk_02017A40;
struct DuelScreen { u8 unk0[0x808]; u8 b808; u8 pad[0x824 - 0x809]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreen gUnk_0201CFB0;
void sub_080240A8(int player, int a);
void sub_08024134(u32 player, u32 a, u32 b);
void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
void sub_080193D4(int player, int idx, int a, int b);
extern const u8 gUnk_08086298[];
extern const u8 gUnk_08086210[], gUnk_08086254[], gUnk_0808626C[], gUnk_08086018[], gUnk_08085FF4[];
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, void (*b)(void), int (*c)(void));
int sub_080522C0(void);
void sub_08052390(void);
void sub_08052190(void);
int sub_0805243C(void);

extern struct { u8 pad[0x82C]; u8 b82C; } gScr1A9C asm("gUnk_0201CFB0");
struct Scr1A9C {
    u8 pad[0x808];
    u16 lo808:3;
    u16 busy:1;
    u16 hi808:12;
    u8 pad80A[0x82C - 0x80A];
    u32 cursor;
};
#define gScr1A9Cb (*(struct Scr1A9C *)&gUnk_0201CFB0)
int sub_08051A9C(int player, u16 x, int count)
{
    u8 *step;
    if (gUnk_020192E4[player & 1].handCount != 0) {
        step = (u8 *)gUnk_020192E4 + 0x1B5E;
        if (*step == 0) {
            sub_080240A8(player, 0xB);
            gUnk_02017A40.b4FC = 0;
            gUnk_02017A40.count = count;
            (*step)++;
            return 0;
        }
        if (gUnk_02017A40.count != 0) {
            u8 *bp = &gUnk_02017A40.b4FC;
            if (*bp <= 9) {
                gScr1A9Cb.busy = 1;
                sub_08024134(player, 0xB, sub_08076F9C() % gUnk_020192E4[player & 1].handCount);
                (*bp)++;
                return 0;
            } else {
                u16 msg = 8;
                if (player != 0)
                    msg = 0x8008;
                sub_0801EC58(msg, (u16)player, ((u8)gScr1A9Cb.cursor << 8) | 0xB, 0);
                sub_080193D4(player, gScr1A9Cb.cursor, x, 1);
                gUnk_02017A40.count--;
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
struct Scr1BBC {
    u8 pad[0x808];
    u16 lo808:3;
    u16 busy:1;
    u16 hi808:12;
    u8 pad80A[0x82C - 0x80A];
    u32 cursor;
};
#define gScr1BBC (*(struct Scr1BBC *)&gUnk_0201CFB0)
#define MSG1BBC ((void (*)(u16, u16, int, int))sub_0801EC58)
int sub_08051BBC(int player, int unused, int count)
{
    u8 *base = (u8 *)gUnk_020192E4;
    struct Player *ps = (struct Player *)(base + (player & 1) * 0xD64);
    u8 *step;
    if (ps->handCount != 0) {
        step = base + 0x1B5E;
        if (*step == 0) {
            sub_080240A8(player, 0xB);
            gUnk_02017A40.b4FC = 0;
            gUnk_02017A40.count = count;
            (*step)++;
            return 0;
        }
        if (gUnk_02017A40.count != 0) {
            u8 *bp = &gUnk_02017A40.b4FC;
            if (*bp <= 9) {
                gScr1BBC.busy = 1;
                sub_08024134(player, 0xB, sub_08076F9C() % ps->handCount);
                (*bp)++;
                return 0;
            } else {
                u16 msg = 8;
                if (player != 0)
                    msg = 0x8008;
                MSG1BBC(msg, (u16)player, ((u8)gScr1BBC.cursor << 8) | 0xB, 0);
                MSG1BBC(player != 0 ? 0x80C1 : 0xC1, (u16)gScr1BBC.cursor, 1, 0);
                gUnk_02017A40.count--;
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
struct Scr1CD8 {
    u8 pad[0x808];
    u16 lo808:3;
    u16 busy:1;
    u16 hi808:12;
    u8 pad80A[0x82C - 0x80A];
    u32 cursor;
};
#define gScr1CD8 (*(struct Scr1CD8 *)&gUnk_0201CFB0)
#define MSG1CD8 ((void (*)(u16, u16, int, int))sub_0801EC58)
int sub_08051CD8(int player, int unused, int count)
{
    u8 *base = (u8 *)gUnk_020192E4;
    struct Player *ps = (struct Player *)(base + (player & 1) * 0xD64);
    u8 *step;
    if (ps->handCount != 0) {
        step = base + 0x1B5E;
        if (*step == 0) {
            sub_080240A8(player, 0xB);
            gUnk_02017A40.b4FC = 0;
            gUnk_02017A40.count = count;
            (*step)++;
            return 0;
        }
        if (gUnk_02017A40.count != 0) {
            u8 *bp = &gUnk_02017A40.b4FC;
            if (*bp <= 9) {
                gScr1CD8.busy = 1;
                sub_08024134(player, 0xB, sub_08076F9C() % ps->handCount);
                (*bp)++;
                return 0;
            } else {
                u16 msg = 8;
                if (player != 0)
                    msg = 0x8008;
                MSG1CD8(msg, (u16)player, ((u8)gScr1CD8.cursor << 8) | 0xB, 0);
                MSG1CD8(player != 0 ? 0x80CE : 0xCE, (u16)gScr1CD8.cursor, 1, 0);
                gUnk_02017A40.count--;
                *bp = 0;
                return 0;
            }
        }
    }
    return 1;
}
int sub_08051DF4(int player)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *step = &d->step;
    if (*step == 0) {
        int m = -1;
        if (sub_08008AF8(player, m) == 0)
            return 1;
        if (player != 0) {
            int r = sub_080563B8(m, 1);
            if (r > m)
                sub_08017FF4(player, r);
            return 1;
        }
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08085FF4);
        (*step)++;
        return 0;
    }
    if (sub_08052F38(0xF0)) {
        u32 a = gUnk_0201CFB0.w824;
        u32 b = gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C;
        sub_08077AEC(1);
        sub_0801EC58(player != 0 ? 0x8008 : 8, (u16)gUnk_0201CFB0.w824, (u8)gUnk_0201CFB0.w828 | (((u8)gUnk_0201CFB0.w82C) << 8), 0);
        sub_08017FF4(a, b);
        return 1;
    }
    return 0;
}
int sub_08051ED0(int player)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *step = &d->step;
    u8 st = *step;
    switch (st) {
    case 0:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08086018);
        (*step)++;
        return 0;
    case 1:
        if (sub_08052F38(1)) {
            int p = 1 & player;
            u16 id = CARD_ID(((struct DuelP *)d)->players[p].hand[gUnk_0201CFB0.w82C]);
            if (sub_08054398(player, id) != 0 && sub_08007834(id) == 0) {
                u32 lvl;
                switch ((int)((((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20)) {
                case 0x15:
                case 0x16:
                case 0x17:
                    lvl = 0;
                    break;
                case 0x18:
                    lvl = 10;
                    break;
                default:
                    lvl = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
                    break;
                }
                if (lvl <= 4) {
                    sub_0801EC58(player != 0 ? 0x80C4 : 0xC4, id,
                                 ((gUnk_0201CFB0.w82C & 0xF) << 4) | (sub_08008A44(player) & 0xF) | 0x200, 0);
                    gUnk_020192E0.step++;
                    return 0;
                }
            }
            sub_08077AEC(3);
        }
        return 0;
    default:
        return 1;
    }
}
struct PickCard { u32 id : 12; u32 flag12 : 1; u32 rest : 19; };
struct PickCursor { u8 pad0[5]; u8 sub : 2; u8 flags : 6; u16 index; u8 pad8[4]; struct PickCard cards[80]; };
extern struct PickCursor gUnk_0201D810;
extern struct PickCard gUnk_0201D81C[];
extern const u8 gUnk_08086058[], gUnk_080860A4[], gUnk_080860FC[];
int sub_08056ECC(u16 id);
void sub_0802AF34(s32 player, s32 skip, u16 number, u16 mode);
int sub_08052018(s32 player, u16 id, u16 mode)
{
    struct Duel *d;
    struct PickCard *chosen;
    u8 st;
    /* Reading the step before taking d gives the ROM's `ldr r0; ...; ldrb; adds r7,r0,#0` copy. */
    st = gUnk_020192E0.step;
    d = &gUnk_020192E0;
    switch (st) {
    case 0:
        if (player != 0) {
            chosen = &gUnk_0201D81C[sub_08056ECC(id)];
            break;
        }
        switch (((const u16 *)0x08622AB4)[id & 0x7FF]) {
        case 0x45C: sub_080602A4(0x206, 0x712, 0xB, gUnk_08086058); break;
        case 0x487: sub_080602A4(0x206, 0x712, 0xB, gUnk_080860A4); break;
        case 0x5F0: sub_080602A4(0x206, 0x712, 0xB, gUnk_080860FC); break;
        default: return 1;
        }
        gUnk_020192E0.step++;
        return 0;
    case 1:
        sub_0802AF34(player, -1, ((const u16 *)0x08622AB4)[id & 0x7FF], mode);
        gUnk_020192E0.step++;
        return 0;
    default:
        chosen = &gUnk_0201D810.cards[gUnk_0201D810.sub + gUnk_0201D810.index];
        if (d->u0[0x1B12] & 2)
            /* FAKEMATCH: `(&...)[0]` keeps the byte store relative to the cards base ([r2,#1])
               instead of folding +0xC+1 into one offset off the cursor ([r2,#13]). */
            (&gUnk_0201D810.cards[gUnk_0201D810.sub + gUnk_0201D810.index])[0].flag12 =
                1 - ((*(u32 *)&gUnk_0201D810.cards[gUnk_0201D810.sub + gUnk_0201D810.index] << 19) >> 31);
        break;
    }
    d->sel = *(u32 *)chosen;
    d->sel2 = ((u16 *)chosen)[1];
    return 1;
}
void sub_08052190(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u32 x = (u->x + 1) << 3;
    u32 y = (u->b21 - u->h) << 3;
    u8 *t;
    u32 attr;
    asm("" : "+r"(y)); /* FAKEMATCH: keep combine from folding y<<16 into D<<19 */
    t = &u->timer;
    if (*t == 0) {
        sub_08075294(0x050003E0, gUnk_0822C300, 0x20);
        sub_08074B08(0xC, 2);
        sub_0807501C(1, 1, 0xA0F, gUnk_0819D214[u->sel]);
        sub_0807501C(0, 0, 0xA02, gUnk_0819D214[u->sel]);
        sub_08075114(0x06016C80, 0);
        (*t)++;
    }
    attr = 0x364;
    sub_080761F0(x | (y << 16), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0((x + 0x20) | (y << 16), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0((y << 16) | (x + 0x40), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0(x | ((y + 8) << 16), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0((x + 0x20) | ((y + 8) << 16), 0x4040, attr | 0xF000);
    attr += 4;
    sub_080761F0((x + 0x40) | ((y + 8) << 16), 0x4040, attr | 0xF000);
}
int sub_080522C0(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *t = &u->timer;
    if (*t == 0)
        return 0;
    if (u->state == 0) {
        u->state++;
        return 0;
    }
    /* gUnk_03000040.keys must be read in every test (no local): a local makes the
     * final `& 1` compile with the and's result in the key register. */
    if (gUnk_03000040.keys & 0x20) {
        u->sel += 0x13;
        u->sel = u->sel % 0x14;
        *t = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 0x10) {
        u->sel += 1;
        u->sel = u->sel % 0x14;
        *t = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 1)
        return 1;
    return 0;
}
int sub_0805232C(void)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *s = &d->step;
    if (*s == 0) {
        sub_080602A4(0x206, 0x40F, 0xB, gUnk_08086210);
        sub_08060308(5, (void (*)(void))sub_08052190, sub_080522C0);
        (*s)++;
        return 0;
    }
    d->sel = gUnk_0201AE60.sel;
    return 1;
}
void sub_08052390(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u32 x = (u->x + 1) << 3;
    int y = (u->b21 - u->h) << 3;
    u8 *t = &u->timer;
    if (*t == 0) {
        sub_08075294(0x050003E0, gUnk_0822C300, 0x20);
        sub_08074B08(8, 4);
        sub_0807501C(4, 0x12, 0xC0F, gUnk_0819D264[u->sel]);
        sub_0807501C(3, 0x11, 0xC01, gUnk_0819D264[u->sel]);
        sub_08075114(0x06016C80, 0);
        (*t)++;
    }
    sub_080761F0(x | ((y - 0x10) << 16), 0x40C0, 0xF364);
}
int sub_0805243C(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *t = &u->timer;
    if (*t == 0)
        return 0;
    if (u->state == 0) {
        u->state++;
        return 0;
    }
    if (gUnk_03000040.keys & 0x20) {
        u->sel += 5;
        u->sel = u->sel % 6;
        *t = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 0x10) {
        u->sel += 1;
        u->sel = u->sel % 6;
        *t = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 1)
        return 1;
    return 0;
}
int sub_080524A8(void)
{
    u16 prev;
    if (gUnk_0201AE60.timer == 0)
        return 0;
    if (gUnk_0201AE60.state == 0) {
        gUnk_0201AE60.state++;
        return 0;
    }
    if (gUnk_03000040.keys & 0x20) {
        struct Ui *p = &gUnk_0201AE60;
        prev = gUnk_0201AE44;
        do {
            p->sel += 5;
            p->sel = p->sel % 6;
        } while (p->sel == prev);
        gUnk_0201AE60.timer = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 0x10) {
        struct Ui *p = &gUnk_0201AE60;
        prev = gUnk_0201AE44;
        do {
            p->sel += 1;
            p->sel = p->sel % 6;
        } while (p->sel == prev);
        gUnk_0201AE60.timer = 0;
        return 0;
    }
    if (gUnk_03000040.keys & 1) {
        if (gUnk_0201AE60.sel != gUnk_020192E0.sel)
            return 1;
    }
    return 0;
}
int sub_08052560(void)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *s = &d->step;
    if (*s == 0) {
        sub_080602A4(0x206, 0x40F, 0xB, gUnk_08086254);
        sub_08060308(5, (void (*)(void))sub_08052390, sub_0805243C);
        (*s)++;
        return 0;
    }
    d->sel = gUnk_0201AE60.sel;
    return 1;
}
int sub_080525C4(void)
{
    struct Duel *d = &gUnk_020192E0;
    u8 *step = &d->step;
    /* A switch rather than an if/else chain keeps both step blocks out of line. */
    switch (*step) {
    case 0:
        sub_080602A4(0x206, 0x40F, 0xB, gUnk_08086254);
        sub_08060308(5, (void (*)(void))sub_08052390, sub_0805243C);
        (*step)++;
        return 0;
    case 1:
        d->sel = gUnk_0201AE60.sel;
        sub_080602A4(0x206, 0x411, 0xB, gUnk_0808626C);
        sub_08060308(5, (void (*)(void))sub_08052390, sub_080524A8);
        (*step)++;
        return 0;
    }
    d->sel2 = gUnk_0201AE60.sel;
    return 1;
}
int sub_08052668(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *s = &u->state;
    int st = *s; /* the (u8) switch index is a separate pseudo, so CSE does not fold st == 1 into the case 1 increment */
    switch ((u8)st) {
    case 0:
        if (u->timer <= 0x3F)
            u->sel = (u->timer >> 2) & 1;
        if (u->timer == 0x40)
            u->sel = sub_08076F9C() & 1;
        if (u->timer > 0xC0) {
            u->timer = 0;
            (*s)++;
        } else
            u->timer++;
        break;
    case 1: {
        u8 v = u->timer;
        if (v <= 0x3B) {
            u->timer = v + 1;
            if ((gUnk_03000040.keysHeld & 2) || (*(u8 *)&gUnk_0201CFB0 & 1)) {
                if (u->timer <= 0x33)
                    u->timer = v + 8;
            }
        } else
            (*s)++;
        break;
    }
    case 2:
        return 1;
    }
    return 0;
}
int sub_08052714(void)
{
    char buf[0x80];
    if (gUnk_020192E0.step == 0) {
        const char *const *tbl;
        u16 *p;
        int i;
        sub_080752D0(buf, gUnk_08086254);
        sub_080752E8(buf, gUnk_0808628C);
        tbl = gUnk_0819D264;
        p = gUnk_020192E0.h1B52;
        for (i = 1; i >= 0; i--) {
            sub_080752E8(buf, gUnk_08086290);
            sub_080752E8(buf, tbl[*p]);
            sub_080752E8(buf, gUnk_0808628C);
            p++;
        }
        sub_080602A4(0x206, 0x50F, 0xB, buf);
        if (gUnk_020192E0.b1B50 & 4) {
            sub_08060308(5, sub_0805FBA4, sub_08052668);
        } else {
            sub_08060308(2, 0, 0);
            gUnk_020192E0.step++;
        }
        gUnk_020192E0.step++;
        return 0;
    }
    gUnk_020192E0.sel = gUnk_020192E0.h1B52[gUnk_0201AE60.sel];
    return 1;
}
int sub_08052810(int a)
{
    if (a != 0) {
        int r = sub_080578F4();
        gUnk_020192E0.sel = r;
        sub_0801EC58(0x8008, 0, (u8)r << 8 | 0xB, 0);
        return 1;
    } else {
        struct Duel *d = &gUnk_020192E0;
        u8 *step = &d->step;
        switch (*step) {
        case 0:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08086298);
            (*step)++;
            return 0;
        case 1:
            {
                struct Q1 *q = &((struct DuelQ *)d)->q1;
                q->f0 = 1;
            }
            if (sub_08052F38(0x10000)) {
                sub_08077AEC(1);
                sub_0801EC58(8, (u16)gUnk_0201CFB0.w824, (u8)gUnk_0201CFB0.w828 | (((u8)gUnk_0201CFB0.w82C) << 8), 0);
                (*step)++;
            }
            return 0;
        default:
            {
                struct Q1 *q = &((struct DuelQ *)d)->q1;
                q->f0 = 0;
            }
            d->sel = gUnk_0201CFB0.w82C;
            return 1;
        }
    }
}
struct Z908Zone {
    u32 card;
    u8 pad4[2];
    u8 flags;
    u8 pad7[3];
    u16 links[32];
    u16 kinds[32];
    u16 count;
    u8 pad8C[8];
};
struct Z908Player {
    u8 pad0[2];
    u8 handCount;
    u8 pad3[0x25];
    struct Z908Zone zones[11];
    u8 rest[0xD64 - 0x28 - 11 * 0x94];
};
/* The duel players at 0x020192E4 (declared above as struct Player) in this layout. */
#define gZ908Players ((struct Z908Player *)gUnk_020192E4)
/* Card ID test on a card word (lsl #20, compared with 0). */
#define Z908_ID(w) (((w) << 20) >> 20)
/* Zone pointer as base + zone * 0x94 + player * 0xD64 (the link scan's address order). */
#define Z908_ZB(p, z) ((struct Z908Zone *)((u8 *)gZ908Players[0].zones + (z) * 0x94 + (p) * 0xD64))

/*
 * Is (player, kind + slot) a valid pick for the 16-bit-per-player key mask?
 * kind 11 = hand (slot < handCount), 0 = monster zones (face/position bits),
 * 5 = magic/trap zones (types 0x15/0x16, else a kind-1/5/6 link from a
 * face-up monster zone of either player), 10 = field zone.
 */
int sub_08052908(int player, int kind, int slot, u32 mask)
{
    struct Z908Zone *zone = &gZ908Players[player & 1].zones[kind + slot];
    /* The signed shift keeps the id extraction apart from the later `id != 0`
     * tests, so only zone->card << 20 is shared (CSE), as in the ROM. */
    int type = (((const u32 *)0x08621DE0)[((s32)(zone->card << 20) >> 20) & 0x7FF] & 0x1F00000) >> 20;
    int p, i, k;

    switch (kind) {
    case 11:
        if (!((1 << (player << 4)) & mask)) return 0;
        if (slot < gZ908Players[player & 1].handCount) return 1;
        return 0;
    case 0: {
        int a, b;
        if (!((0xF0 << (player << 4)) & mask)) return 0;
        a = 0;
        b = 0;
        if (Z908_ID(zone->card) == 0) return 0;
        if (((0x20 << (player << 4)) & mask) && (zone->flags & 2)) a = 1;
        if (((0x10 << (player << 4)) & mask) && !(zone->flags & 2)) a = 1;
        if (((0x80 << (player << 4)) & mask) && (zone->flags & 1)) b = 1;
        if (((0x40 << (player << 4)) & mask) && !(zone->flags & 1)) b = 1;
        if (b && a) return 1;
        return 0;
    }
    case 5: {
        u32 sel = (0xE << (player << 4)) & mask;
        if (!sel) return 0;
        if (Z908_ID(zone->card) == 0) return 0;
        if (!(zone->flags & 2)) {
            if ((2 << (player << 4)) & mask) return 1;
            return 0;
        }
        if (sel == (2 << (player << 4))) return 0;
        switch (type) {
        case 0x16:
            if ((4 << (player << 4)) & mask) return 1;
            return 0;
        case 0x15:
            if ((8 << (player << 4)) & mask) return 1;
            return 0;
        }
        for (p = 0; p <= 1; p++) {
            for (i = 0; i <= 4; i++) {
                if (Z908_ID(Z908_ZB(p & 1, i)->card) && (Z908_ZB(p & 1, i)->flags & 2)) {
                    for (k = 0; k < Z908_ZB(p & 1, i)->count; k++) {
                        u16 link = Z908_ZB(p & 1, i)->links[k];
                        switch (Z908_ZB(p & 1, i)->kinds[k]) {
                        case 1:
                        case 5:
                        case 6:
                            if (link == (u16)((u8)player | ((u8)(slot + 5) << 8))) return 1;
                            break;
                        }
                    }
                }
            }
        }
        return 0;
    }
    case 10: {
        int ret;
        struct Z908Zone *f = Z908_ZB(player & 1, 10);
        if (!Z908_ID(f->card)) return 0;
        ret = 0;
        if (f->flags & 2) {
            if ((4 << (player << 4)) & mask)
                ret = 1;
        } else if ((2 << (player << 4)) & mask)
            ret = 1;
        if (ret) return 1;
        return 0;
    }
    }
    return 0;
}
