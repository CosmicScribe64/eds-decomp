#include "global.h"

struct Player { u8 unk0[2]; u8 handCount; u8 pad[0x684 - 3]; u32 hand[80]; u8 pad2[0xD64 - 0x684 - 0x140]; };
extern struct Player gUnk_020192E4[];
extern const u32 gUnk_08621DE0[];
#define CARD_STATS(id) (*(gUnk_08621DE0 + ((id) & 0x7FF)))
#define CARD_STATS_RAW(id) (*(gUnk_08621DE0 + (id)))
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Menu block 0x0201AE60 */
struct Ui {
    u8 u0[8];
    u16 x;      /* +8 */
    u16 y;      /* +0xA */
    u8 uC[2];
    u16 h;      /* +0xE */
    u8 u10[4];
    u16 sel;    /* +0x14 */
    u8 u16_[0x21 - 0x16];
    u8 b21;
    u8 state;   /* +0x22 */
    u8 timer;   /* +0x23 */
};
extern struct Ui gUnk_0201AE60;
/* Action list block 0x02017A40 (see code_0801F454). */
struct ActBlk {
    u8 u0[0x3D6];
    s16 effIdx;         /* +0x3D6 */
    u32 fn3D8;
    u8 u3DC[0x3E0 - 0x3DC];
    u8 b3E0;
    u8 b3E1;
    u8 u3E2[0x3E4 - 0x3E2];
    u8 b3E4;
    u8 b3E5;
    u8 u3E6[0x480 - 0x3E6];
    u32 fn480;
    u32 fn484;
    u8 u488[0x4FC - 0x488];
    u8 b4FC;
    u8 count;           /* +0x4FD */
};
extern struct ActBlk gUnk_02017A40;
/* Byte views of the duel global 0x020192E0 and the link block 0x02017FB0 */
struct MainKeys { u8 u0[6]; u16 keys; };
extern struct MainKeys gUnk_03000040;
struct Cnt2 { u8 b0; u8 b1; };
extern struct Cnt2 gUnk_02015EE8;
u16 sub_080512F0(void);
int sub_0804A1C8(void);
int sub_0801E944(void);
void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
void sub_0802297C(u16 msg, u16 a, u16 b, u16 c);
u16 sub_080229BC(u16 head, const void *src, int size);
u16 sub_08050E48(void);
u16 sub_08051098(void);
u16 sub_08050FF4(void);
u16 sub_08051140(void);
int sub_08042BE0(void);
int sub_0801A32C(void);
struct DG3 {
    u8 pad[0x1B12];
    u8 b1B12;
    u8 pad2[0x1B62 - 0x1B13];
    u8 step;
};
extern const u8 gUnk_08085FDC[];
int sub_08056CE8(void);
int sub_0805664C(struct Player *ps, int a);
int sub_0800A1C4(int player);
int sub_0800A158(int player);
int sub_08076F9C(void);
void sub_080240A8(int player, int a);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, void (*b)(void), int (*c)(void));
void sub_080516D8(void);
int sub_08051730(void);
void sub_08046BE0(int player, int a);
void sub_08042AB0(int player, int kind, u32 arg);
extern const u8 gUnk_08085DCC[], gUnk_08085E60[], gUnk_08085EBC[], gUnk_08085F5C[];
extern const u8 gUnk_08085D94[];
extern const u8 gUnk_0822C720[];
extern const u16 gUnk_08622AB4[];
extern const u16 gUnk_08623E66;
extern const u16 gUnk_0862467A;
void sub_080753F4(char *dst, const void *fmt, const void *name);
struct Bits8 { u8 g0 : 1; u8 g1 : 1; u8 g2 : 1; u8 g3 : 5; u8 pad[4]; };
struct T8 { u8 f0 : 1; u8 rest : 7; u8 pad[4]; };
struct T16 { u16 lo : 8; u16 hi : 8; };
struct EffEntry4 { u16 idx; u16 u2; u32 fn4; u8 pad[24 - 8]; };
struct HandW { u32 lo : 17; u32 f17 : 1; u32 f18 : 1; u32 rest : 13; };
struct DuelScreen { u8 unk0[0x82C]; u32 w82C; };
extern struct DuelScreen gUnk_0201CFB0;
struct LinkBlk;
#define OFF(t, f) ((u32)&((t *)0)->f)
struct LinkBlk {
    u8 u0[0x306];
    u8 b306;
    u8 b307;
    u32 f308_0 : 1;
    u32 f308_1 : 1;
    u32 f308_2 : 1;
    u32 f308_3 : 1;
    u32 f308_4 : 1;
    u32 f308_5 : 1;
    u32 f308_6 : 1;
    u32 f308_7 : 25;
    u8 u30C[0x450 - 0x30C];
    u32 f450_0 : 1;
    u32 f450_1 : 15;
    u16 h452;
    u16 h454;
    u16 h456;
    u16 h458;
    u16 h45A;
    u16 id45C;
    u8 b45E;
    u8 u45F[0x48D - 0x45F];
    u8 step48D;
    u8 step48E;
    u8 step48F;
};
typedef char chk452[(OFF(struct LinkBlk, h452) == 0x452) ? 1 : -1];
typedef char chk45C[(OFF(struct LinkBlk, id45C) == 0x45C) ? 1 : -1];
extern struct LinkBlk gUnk_02017FB0;
struct EffEntry { u16 idx; u16 u2; u32 fn4; u8 u8_[0x10 - 8]; u32 fn10; u32 fn14; };
extern struct EffEntry gUnk_0819A9D4[];
struct DuelSel {
    u8 unk0[0x1B10];
    u16 w1B10;      /* +0x1B10 */
    u8 b1B12;       /* +0x1B12 */
    u8 u1B13[0x1B20 - 0x1B13];
    u8 step;        /* +0x1B20 */
    u8 b1B21;       /* +0x1B21 */
    u8 u1B22[0x1B54 - 0x1B22];
    u16 w1B54;      /* +0x1B54 */
};
extern struct DuelSel gUnk_020192E0;
extern u8 gUnk_0201930C[];      /* per-player zone block (0x94-byte entries) */
extern struct Cnt2 gUnk_02015EF0;
void sub_080197E0(int player, u16 a);
void sub_08018544(int player, int idx, int a);
void sub_08075434(char *a, char *b, int n);
int sub_08047058(int id);
int sub_08052F38(u32 mask);
void sub_080193D4(int player, int idx, int a, int b);
void sub_08024134(u32 player, u32 a, u32 b);
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);
void sub_08077AEC(u16 se);
u16 sub_0805163C(u16 a, u16 b);

#if 0 /* NONMATCHING (score 156): Rewritten from the asm (score 156). Prologue, jump table, case 0/1 zone loops
       * and found blocks match up to registers. Forms that matter: D50 = *(struct *)&gUnk_020192E0 accessed directly
       * (GCSE makes the r8 base copy); zone loop pointer via integer macro (z*0x94 + (p&1)*0xD64 + (u32)gUnk_0201930C)
       * gives (t1+t2)+K with K hoisted; found-block zone access via a true ARRAY_REF (struct wrapper around
       * gUnk_0201930C) gives i*0x94 first; hand loops 'i = 0; ps = P50; for (; i < ps[p&1].handCount; i++)' load the sym
       * before the multiply; subtype via static inline (u16 id) with a switch (21,22) and switch(sub){case 2..4: ok=0}.
       * Remaining: case 2 should be 'while (D50.idx <= 4) { u8 n = D50.idx; z = (n*0x94 + p*0xD64 +
       * (u32)D50.players[0].zones); ... D50.idx = n + 1; }' (gives r8+0x1B21/r8+0x2C) but that raises the base pseudo's
       * refs so global-alloc gives it r7 instead of r8 (ROM: zone-loop p*0xD64 gets r7). Also the ROM recomputes 1-p for
       * the second call in the case-1 hand-found block, and its case-2 loop keeps movs #0x94 in the loop (loop.c
       * threshold). */
struct S50Card { u32 id:12; u32 b12:6; u32 f18:1; u32 b19:13; };
struct S50Zone {
    struct S50Card card;    /* +0x00 */
    u16 serial;             /* +0x04 */
    u16 f6_0:1;
    u16 f6_1:1;             /* +0x06 bit 1 */
    u16 f6_2:4;
    u16 cnt:4;              /* +0x06 bits 6..9 */
    u16 f6_10:6;
    u8 pad8[0x94 - 8];
};
struct S50Player {
    u16 lp;
    u8 handCount;
    u8 pad3[0x28 - 3];
    struct S50Zone zones[11];   /* +0x28 */
    struct S50Card hand[80];    /* +0x684 */
    u8 padx[0xD64 - 0x684 - 80 * 4];
};
struct S50Duel {
    u32 unk0;
    struct S50Player players[2];
    u8 pad[0x1B10 - 4 - 2 * 0xD64];
    u16 w1B10;
    u8 f0:1;
    u8 turn:1;
    u8 f2:6;
    u8 pad13[0x1B20 - 0x1B13];
    u8 step;
    u8 idx;
};
#define D50 (*(struct S50Duel *)&gUnk_020192E0)
#define P50 ((struct S50Player *)gUnk_020192E4)
struct S50ZP { struct S50Zone zones[11]; u8 rest[0xD64 - 11 * 0x94]; };
struct S50All { struct S50ZP pl[2]; };
#define ZP50 ((*(struct S50All *)gUnk_0201930C).pl)
#define Z50(p, z) (ZP50[(p) & 1].zones[z])
#define ZONE50(p, z) ((struct S50Zone *)((z) * 0x94 + ((p) & 1) * 0xD64 + (u32)gUnk_0201930C))

static inline int S50Sub(u16 id)
{
    u32 st = ((const u32 *)0x08621DE0)[id & 0x7FF];
    switch ((int)((st & 0x1F00000) >> 20)) {
    case 21:
    case 22:
        return (st & 0xE0000) >> 17;
    default:
        return 0;
    }
}

/* Per-card effect step machine (steps at 0x020192E0+0x1B20). Returns 1 when done. */
int sub_08050A70(void)
{
    char buf[0x80];
    int p = D50.turn;
    int i;
    struct S50Player *ps;

    switch (D50.step) {
    case 0:
        i = 0;
        ps = P50;
        for (; i < ps[p & 1].handCount; i++) {
            struct S50Card c = ps[p & 1].hand[i];
            if (c.f18) {
                sub_080197E0(p, gUnk_0862467A);
                sub_080193D4(p, i, 0, 1);
                return 0;
            }
        }
        for (i = 5; i <= 10; i++) {
            struct S50Zone *z = ZONE50(p, i);
            struct S50Card c = z->card;
            if (c.id != 0 && c.f18) {
                int ok = 1;
                if (z->f6_1) {
                    switch (S50Sub(c.id)) {
                    case 2:
                    case 3:
                    case 4:
                        ok = 0;
                    }
                }
                if (ok) {
                    Z50(p, i).card.f18 = 0;
                    sub_080197E0(p, gUnk_0862467A);
                    sub_08018544(p, i, 0);
                    return 0;
                }
            }
        }
        D50.step++;
        return 0;
    case 1:
        i = 0;
        ps = P50;
        for (; i < ps[(1 - p) & 1].handCount; i++) {
            struct S50Card c = ps[(1 - p) & 1].hand[i];
            if (c.f18) {
                sub_080197E0(1 - p, gUnk_0862467A);
                sub_080193D4(1 - p, i, 0, 1);
                return 0;
            }
        }
        for (i = 5; i <= 10; i++) {
            struct S50Zone *z = ZONE50(1 - p, i);
            struct S50Card c = z->card;
            if (c.id != 0 && c.f18) {
                int ok = 1;
                if (z->f6_1) {
                    switch (S50Sub(c.id)) {
                    case 2:
                    case 3:
                    case 4:
                        ok = 0;
                    }
                }
                if (ok) {
                    Z50(1 - p, i).card.f18 = 0;
                    sub_080197E0(1 - p, gUnk_0862467A);
                    sub_08018544(1 - p, i, 0);
                    return 0;
                }
            }
        }
        if (p)
            D50.step++;
        D50.step++;
        D50.idx = 0;
        return 0;
    case 2: {
        u8 *pi = &D50.idx;
        for (; *pi <= 4; (*pi)++) {
            struct S50Zone *z = (struct S50Zone *)(*pi * 0x94 + p * 0xD64 + (u32)D50.players[0].zones);
            struct S50Card c = z->card;
            if (c.id != 0 && z->cnt > 1) {
                sub_080753F4(buf, gUnk_08085D94, gUnk_0822C720 + c.id * 0x40);
                sub_08075434(buf, buf, z->cnt - 1);
                sub_080602A4(0x206, 0x712, 0xB, buf);
                (*pi)++;
                return 0;
            }
        }
        D50.step++;
        return 0;
    }
    case 3:
        sub_0801EC58((D50.turn) ? 0x8002 : 2, 0, 0, 0);
        D50.step++;
        return 0;
    case 4:
        sub_0801EC58((D50.turn) ? 0x8003 : 3, 0, 0, 0);
        D50.step++;
        return 0;
    default:
        if (!(gUnk_02015EE8.b1 & 1)) {
            if (!D50.turn) {
                gUnk_02015EF0.b0 = 0;
                gUnk_02015EF0.b1 = 0;
            }
        }
        if (gUnk_02015EE8.b1 & 1)
            sub_0802297C(0xF002, 0, 0, 0);
        D50.w1B10++;
        return 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08050A70", sub_08050A70); /* 0x08050A70 size 0x3D8 */
/* ROM table views at fixed addresses preserve the target lookup allocation. */
u16 sub_08050E48(void)
{
    char buf[0x100];
    switch (gUnk_02017FB0.h452) {
    case 0:
        switch (((const u16 *)0x08622AB4)[gUnk_02017FB0.h454 & 0x7FF]) {
        case 0x172:
        case 0x173:
        case 0x174:
            sub_080753F4(buf, gUnk_08085DCC, ((const u8 *)0x0822C720) + (gUnk_02017FB0.h454 << 6));
            sub_080602A4(0x204, 0xA14, 0xB, buf);
            sub_08060308(1, 0, 0);
            break;
        case 0x39:
            sub_080753F4(buf, gUnk_08085E60, ((const u8 *)0x0822C720) + (gUnk_08623E66 << 6));
            sub_080602A4(0x204, 0x915, 0xB, buf);
            sub_08060308(1, 0, 0);
            break;
        case 0x4DB:
            sub_080753F4(buf, gUnk_08085EBC, ((const u8 *)0x0822C720) + (gUnk_02017FB0.h454 << 6));
            sub_080602A4(0x206, 0x713, 0xB, buf);
            sub_08060308(1, 0, 0);
            break;
        case 0x5F2:
            sub_080753F4(buf, gUnk_08085F5C, ((const u8 *)0x0822C720) + (gUnk_02017FB0.h454 << 6));
            sub_080602A4(0x206, 0x713, 0xB, buf);
            sub_08060308(1, 0, 0);
            break;
        }
        gUnk_02017FB0.h452++;
        return 0;
    case 1:
        switch (((const u16 *)0x08622AB4)[gUnk_02017FB0.h454 & 0x7FF]) {
        case 0x172:
        case 0x173:
        case 0x174:
        case 0x39:
        case 0x4DB:
        case 0x5F2:
            gUnk_02017FB0.h45A = gUnk_0201AE60.sel;
            return 1;
        }
        return 0;
    default:
        return 1;
    }
}

u16 sub_08050FF4(void)
{
    struct LinkBlk *b = &gUnk_02017FB0;
    u8 *step = &b->step48D;
    switch (*step) {
    case 0:
        gUnk_02017A40.effIdx = sub_08047058(b->id45C);
        if (gUnk_02017A40.effIdx < 0)
            return 1;
        gUnk_02017A40.fn480 = gUnk_0819A9D4[gUnk_02017A40.effIdx].fn10;
        if (gUnk_02017A40.fn480 == 0)
            return 1;
        ((u8 *)&gUnk_02017A40)[0x3E4] = 0;
        (*step)++;
        return 0;
    case 1:
        if (((u16(*)(void *, void *))gUnk_02017A40.fn480)((u8 *)b + 0x45C, (u8 *)b + 0x470) == 0)
            return 0;
        (*step)++;
        return 0;
    default:
        return 1;
    }
}
u16 sub_08051098(void)
{
    struct LinkBlk *b = &gUnk_02017FB0;
    u8 *step = &b->step48E;
    switch (*step) {
    case 0:
        gUnk_02017A40.effIdx = sub_08047058(b->id45C);
        if (gUnk_02017A40.effIdx < 0)
            return 1;
        gUnk_02017A40.fn484 = gUnk_0819A9D4[gUnk_02017A40.effIdx].fn14;
        if (gUnk_02017A40.fn484 == 0)
            return 1;
        *(u8 *)((u8 *)&gUnk_02017A40 + 0x3E5) = 0;
        (*step)++;
        return 0;
    case 1:
        if (((u16(*)(void *, void *))gUnk_02017A40.fn484)((u8 *)b + 0x45C, (u8 *)b + 0x470) == 0)
            return 0;
        (*step)++;
        return 0;
    default:
        return 1;
    }
}
/* Link copy of an effect source/target ref (0x14 bytes, at link block +0x45C and +0x470). */
struct LinkRef {
    u16 id;
    u8 b2;              /* +0x02: bit 0 player (struct T8 view) */
    u8 u3[3];
    u16 pos;            /* +0x06: low byte player */
    u16 w8;             /* +0x08: low byte player */
    u8 uA[0x14 - 0xA];
};
#define LINK_REF(l, off) ((struct LinkRef *)((l) + (off)))
#define LINK_REF_PLAYER(l, off) (((struct T8 *)&LINK_REF(l, off)->b2)->f0)
/* Swaps the low-byte player of a packed halfword: 1 - player, high byte kept. */
#define FLIP_LO_PLAYER(h) ((u8)(1 - (h)) | ((h) >> 8 << 8))
/* Mirror both refs to the other side's point of view (player = 1 - player), then look up and start the
 * effect handler fn4 (step 0); call it with (ref, target or NULL) until it returns 0 (step 1). */
u16 sub_08051140(void)
{
    u8 *l = (u8 *)&gUnk_02017FB0;
    u8 *step = l + 0x48F;
    switch (*step) {
    case 0:
        /* FAKEMATCH: the u8 constant gives the minuend its own QImode register, as in the ROM */
        { u8 v = LINK_REF_PLAYER(l, 0x45C); u8 one = 1; LINK_REF_PLAYER(l, 0x45C) = one - v; }
        { u8 v = LINK_REF_PLAYER(l, 0x470); u8 one = 1; LINK_REF_PLAYER(l, 0x470) = one - v; }
        { u16 *hp = &LINK_REF(l, 0x45C)->pos; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &LINK_REF(l, 0x45C)->w8; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &LINK_REF(l, 0x470)->pos; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        { u16 *hp = &LINK_REF(l, 0x470)->w8; u16 h = *hp; *hp = FLIP_LO_PLAYER(h); }
        gUnk_02017A40.effIdx = sub_08047058(gUnk_02017FB0.id45C);
        if (gUnk_02017A40.effIdx < 0)
            return 1;
        gUnk_02017A40.fn3D8 = gUnk_0819A9D4[gUnk_02017A40.effIdx].fn4;
        if (gUnk_02017A40.fn3D8 == 0)
            return 1;
        gUnk_02017A40.b3E0 = 0x80;
        gUnk_02017A40.b3E1 = 0;
        (*step)++;
        return 0;
    case 1:
        if (l[0x490] & 1)
            gUnk_02017A40.b3E0 = ((u8(*)(void *, void *))gUnk_02017A40.fn3D8)(l + 0x45C, l + 0x470);
        else
            gUnk_02017A40.b3E0 = ((u8(*)(void *, void *))gUnk_02017A40.fn3D8)(l + 0x45C, 0);
        if (gUnk_02017A40.b3E0 == 0)
            gUnk_02017FB0.step48F++;
        return 0;
    default:
        return 1;
    }
}
#undef LINK_REF
#undef LINK_REF_PLAYER
#undef FLIP_LO_PLAYER
u16 sub_080512F0(void)
{
    if (gUnk_02017FB0.f450_0 && !((gUnk_02017FB0.b306 << 26) < 0)) {
        if (sub_08050E48() != 0) {
            gUnk_02017FB0.f450_0 = 0;
            sub_0802297C(0xF058, gUnk_02017FB0.h45A, gUnk_02017FB0.h456, gUnk_02017FB0.h458);
        }
        return 1;
    }
    if (gUnk_02017FB0.f308_2) {
        if (sub_08050FF4() != 0) {
            gUnk_02017FB0.f308_2 = 0;
            gUnk_02017FB0.b45E |= 1;
            sub_080229BC(0xF092, &gUnk_02017FB0.id45C, 0x14);
        }
        return 1;
    }
    if (gUnk_02017FB0.f308_0) {
        if (sub_08051098() != 0) {
            gUnk_02017FB0.f308_0 = 0;
            gUnk_02017FB0.b45E |= 1;
            sub_080229BC(0xF082, &gUnk_02017FB0.id45C, 0x14);
        }
        return 1;
    }
    if ((*(u16 *)&gUnk_02017FB0.b306 & 0x420) == 0x400) {
        if (sub_08042BE0() != 0)
            ((struct Bits8 *)&gUnk_02017FB0.b307)->g2 = 0;
        return 1;
    }
    if (gUnk_02017FB0.f308_4) {
        if (sub_08051140() != 0) {
            gUnk_02017FB0.f308_4 = 0;
            sub_0802297C(0xF073, 0, 0, 0);
        }
        return 1;
    }
    if (gUnk_02017FB0.f308_6) {
        if (sub_0801A32C() != 0) {
            gUnk_02017FB0.f308_6 = 0;
            sub_0802297C(0xF065, 0, 0, 0);
        }
        return 1;
    }
    return 0;
}
struct DG2 {
    u8 pad[0x1B12];
    u8 f0 : 1;
    u8 f1 : 1;
    u8 f2 : 6;
    u8 pad2[0x1B20 - 0x1B13];
    u8 step;
    u8 b1B21;
};
int sub_0805146C(void)
{
    struct Bits8 *q;
    ((struct DG2 *)&gUnk_020192E0)->f1 = 1;
    if (gUnk_02015EE8.b1 & 1) {
        u8 t = gUnk_02017FB0.b306;
        if ((t << 28) < 0) {
            gUnk_02015EE8.b0++;
            return 1;
        }
        if ((t << 26) < 0)
            return 0;
        if (sub_080512F0() != 0)
            return 0;
        q = (struct Bits8 *)((u8 *)&gUnk_020192E0 + 0x1B14);
        if (q->g1) {
            if (sub_0804A1C8() != 0)
                return 0;
            if (gUnk_03000040.keys & 2) {
                q->g1 = 0;
                sub_0801EC58(3, 0, 0, 0);
                sub_0802297C(0xF006, 0, 0, 0);
            }
        } else if (gUnk_03000040.keys & 1) {
            sub_0802297C(0xF004, 0, 0, 0);
        }
        {
            u8 t2 = gUnk_02017FB0.b306;
            if ((t2 << 29) >= 0)
                return 0;
            {
                int m = -5;
                m &= t2;
                gUnk_02017FB0.b306 = m;
            }
        }
    } else if (sub_0801E944() == 0) {
        return 0;
    }
    ((struct DG2 *)&gUnk_020192E0)->f1 = 0;
    gUnk_02015EE8.b0 -= 6;
    ((struct DG2 *)&gUnk_020192E0)->step = 0;
    ((struct DG2 *)&gUnk_020192E0)->b1B21 = 0;
    return 0;
}
static inline int W515Type(u16 id)
{
    return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
}
/* Count hand cards of `player` whose word has neither bit 17 nor bit 18; flag != 0 also requires
 * type <= 0x14. flag 0: just the hand count. */
int sub_080515A4(int player, u16 flag)
{
    int six;
    int i;
    int count = 0;
    int n;

    if (flag == 0)
        return gUnk_020192E4[player & 1].handCount;
    n = gUnk_020192E4[player & 1].handCount;
    for (i = 0; i < n; i++) {
        u32 *p = &gUnk_020192E4[player & 1].hand[i];
        if ((u32)W515Type((*p << 20) >> 20) <= 20 || flag == 0) {
            /* FAKEMATCH: the mask goes through a temporary (permuter find); it swaps count (r5) and the base (r6). */
            if (!(((u8 *)p)[2] & (six = 6)))
                count++;
        }
    }
    return count;
}

/* Returns 1 when player 0 hands over the selected card (a: type <= 0x14 only). */
u16 sub_0805163C(u16 a, u16 b)
{
    struct Player *ps = gUnk_020192E4;
    if (ps->handCount == 0)
        return 1;
    if (sub_08052F38(1) != 0) {
        u32 idx = gUnk_0201CFB0.w82C;
        u32 off4 = idx << 2;
        u32 *hp = gUnk_020192E4->hand;
        struct HandW *w = (struct HandW *)((u8 *)hp + off4);
        if (a == 0 || CARD_TYPE((*(u32 *)w << 20) >> 20) <= 0x14) {
            struct HandW v = *w;
            int ok = 1;
            if (v.f17)
                ok = 0;
            if (v.f18)
                ok = 0;
            if (ok != 0) {
                sub_080193D4(0, idx, b, 1);
                return 1;
            }
        }
        sub_08077AEC(3);
    }
    return 0;
}

/* Draws the cursor sprites for the pending list entries. */
void sub_080516D8(void)
{
    int i;
    int x = (gUnk_0201AE60.x + 1) << 3;
    int y = (gUnk_0201AE60.b21 - gUnk_0201AE60.h + 2) << 3;
    for (i = 0; i < gUnk_02017A40.count; i++) {
        /* FAKEMATCH: no-op self-store (found by the permuter). It enlarges the loop body enough that loop.c
         * keeps y << 16 inside the loop, and the extra uses of the count address give that pointer r6 ahead of y. */
        gUnk_02017A40.count += 0;
        sub_080761F0((x + i * 10) | (y << 16), 0, 0x431C);
    }
}
int sub_08051730(void)
{
    struct Ui *u = &gUnk_0201AE60;
    u8 *st = &u->timer;
    switch (*st) {
    case 0:
        if (sub_0805163C(gUnk_020192E0.w1B54 & 1, gUnk_020192E0.w1B54 & 2)) {
        inc:
            (*st)++;
        }
        break;
    case 1:
        sub_08024134(0, 0xB, 0);
        goto inc;
    case 2:
        gUnk_02017A40.count--;
        if (gUnk_02017A40.count == 0)
            return 1;
        *st = 0;
        return 0;
    }
    return 0;
}
int sub_080517BC(int player, int b, int c, u16 d)
{
    struct DG3 *e = (struct DG3 *)&gUnk_020192E0;
    u8 *step = &e->step;
    switch (*step) {
    case 0:
        sub_080240A8(player, 0xB);
        gUnk_02017A40.b4FC = 0;
        gUnk_02017A40.count = b;
        (*step)++;
        return 0;
    case 1:
        if (player != 0) {
            struct Player *ps;
            int r;
            if (gUnk_02017A40.count == 0)
                goto inc;
            ps = (struct Player *)((u8 *)e + 4);
            if (ps[player & 1].handCount == 0)
                goto inc;
            r = sub_08056CE8();
            if (r < 0) {
                r = sub_0805664C(ps, 1);
                if (r < 0) {
                    r = sub_0800A1C4(1);
                    if (r < 0) {
                        r = sub_0800A158(1);
                        if (r < 0) {
                            u8 *q = (u8 *)e + 0xD6A;
                            if (*q > 2)
                                r = sub_08076F9C() % *q;
                            else
                                r = 0;
                        }
                    }
                }
            }
            sub_080193D4(player, r, d, 1);
            gUnk_02017A40.count--;
            return 0;
        }
        sub_08024134(0, 0xB, 0);
        sub_080602A4(0x209, 0x50E, 0xB, gUnk_08085FDC);
        sub_08060308(5, sub_080516D8, sub_08051730);
    inc:
        ((struct DG3 *)&gUnk_020192E0)->step++;
        return 0;
    case 2:
        sub_08046BE0(player, b);
        (*step)++;
        return 0;
    case 3:
        sub_08042AB0(1 - ((u32)(e->b1B12 << 30) >> 31), 0x1D, (u8)player);
        (*step)++;
        return 0;
    default:
        return 1;
    }
}
int sub_0805194C(int player, int b, int c, u16 d)
{
    struct DG3 *e = (struct DG3 *)&gUnk_020192E0;
    u8 *step = &e->step;
    switch (*step) {
    case 0:
        sub_080240A8(player, 0xB);
        gUnk_02017A40.b4FC = 0;
        gUnk_02017A40.count = b;
        (*step)++;
        return 0;
    case 1:
        if (player != 0) {
            struct Player *ps;
            int r;
            if (gUnk_02017A40.count == 0)
                goto inc;
            ps = (struct Player *)((u8 *)e + 4);
            if (ps[player & 1].handCount == 0)
                goto inc;
            r = sub_08056CE8();
            if (r < 0) {
                r = sub_0805664C(ps, 1);
                if (r < 0) {
                    r = sub_0800A1C4(1);
                    if (r < 0) {
                        r = sub_0800A158(1);
                        if (r < 0) {
                            u8 *q = (u8 *)e + 0xD6A;
                            if (*q > 2)
                                r = sub_08076F9C() % *q;
                            else
                                r = 0;
                        }
                    }
                }
            }
            sub_080193D4(player, r, d, 1);
            gUnk_02017A40.count--;
            return 0;
        }
        sub_08024134(0, 0xB, 0);
        sub_080602A4(0x209, 0x50E, 0xB, gUnk_08085FDC);
        sub_08060308(5, sub_080516D8, sub_08051730);
    inc:
        ((struct DG3 *)&gUnk_020192E0)->step++;
        return 0;
    default:
        return 1;
    }
}
