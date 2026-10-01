#include "global.h"

int sub_0802CFA0(int player, u16 id, u16 x);
void sub_08019894(int player, u16 dmg, u16 a, u16 b);
void sub_080197E0(int player, u16 id);
void sub_080199E0(int player, int n);
void sub_08022784(int player, int a, int b);
int sub_0800A430(int player, int zone);
void sub_08017AB4(int player, u16 a, u16 b, u16 c);
void sub_08018ED8(int player, int zone, int a, int b);
void sub_080753F4(char *dst, const char *fmt, const char *arg);
void sub_080602A4(u16 a, u16 b, int c, const char *text);
void sub_08060308(int a, int b, int c);
u16 sub_0802297C(u16 a, u16 b, u16 c, u16 d);
void sub_080197C0(int player, u16 id);
int sub_0801A130(int player, int a);
u32 sub_0800A2A8(int player, int zone);
extern u8 gUnk_020192E0[];
extern const char gUnk_08085B7C[];
extern const u16 gUnk_08623E66[];
extern const char gUnk_0822C720[];
extern const u16 gUnk_08624730[];
extern u8 gUnk_02017FB0[];
struct Unk0201AE60 { u8 pad[0x14]; u16 v14; };
extern struct Unk0201AE60 gUnk_0201AE60;
struct Unk02015EE8 { u8 b0; u8 link : 1; };
extern struct Unk02015EE8 gUnk_02015EE8;
/* Duel progress word at 0x020192E0+0x1B16: bits 1-8 = step. */
struct StepW { u16 lo : 1; u16 step : 8; u16 hi : 7; u8 pad[6]; };
#define STEP (((struct StepW *)(gUnk_020192E0 + 0x1B16))->step)
#define STEP0 (((struct StepW *)(e + 0x1B16))->step)
/* Per-player flag byte at 0x020192E4+8 (stride 0xD64). */
struct PlF { u8 pad[8]; u8 f0 : 1; s8 f1 : 1; u8 rest : 6; u8 pad2[0xD64 - 9]; };
extern struct PlF gUnk_020192E4[];
struct ZoneF6 { u8 pad[6]; u8 b6; u8 pad7[0x94 - 7]; };

void sub_0801FBCC(u32 event, int arg);

/* Battle state at 0x02018450 (see code_0801CE68). */
struct BSide { u8 raw; u8 f1; u16 cardId; u8 pad4[6]; u16 damage; };
struct BState {
    u16 lo : 2;
    u16 bit2 : 1;       /* +0 bit 2 */
    u16 mid : 3;
    u16 atkSlot : 3;
    u16 defSlot : 3;    /* +0 bits 9-11 */
    u16 hi : 4;
    u16 cardId;         /* +2 */
    u8 pad4[4];
    struct BSide side[2];   /* +8, 12 bytes each */
};
extern struct BState gUnk_02018450;
/* View of a side at (byte) offset i*12 from the start of the battle state (fields at +8 ...). */
struct BSideV { u8 pad[8]; u8 raw; u8 f1; u16 cardId; u8 pad4[6]; u16 damage; };
#define BSV(i) ((struct BSideV *)((u8 *)&gUnk_02018450 + (i) * 12))

#if 0 /* NONMATCHING: the case bodies have the same shape but register allocation differs (e2 lands
       * in ip instead of r3, constant 1 in r5 instead of r4, B/side pointers in the default
       * arm). The result is 0x74 bytes shorter than the ROM. */
/* Battle resolution step machine (hypothesis: applies per-card effects after damage; steps 0-2 damage, 10-12 special). */
int sub_0804CB58(int p)
{
    char buf[256];
    u8 *e = gUnk_020192E0;
    switch (STEP0) {
    case 0:
        if (gUnk_02018450.side[p].damage != 0 && !gUnk_020192E4[p & 1].f0 && !gUnk_020192E4[p & 1].f1) {
            sub_08019894(p, gUnk_02018450.side[p].damage, (u8)p | gUnk_02018450.atkSlot << 8, (u8)(1 - p) | gUnk_02018450.defSlot << 8);
            switch (((const u16 *)0x08622AB4)[gUnk_02018450.side[(1 - p)].cardId & 0x7FF]) {
            case 0x71:
                sub_080197E0((1 - p), gUnk_02018450.side[(1 - p)].cardId);
                sub_08022784(p, 1, 1);
                break;
            case 0xDB:
                sub_080197E0((1 - p), gUnk_02018450.side[(1 - p)].cardId);
                sub_080199E0((1 - p), 1);
                break;
            case 0x20A:
                sub_080197E0((1 - p), gUnk_02018450.side[(1 - p)].cardId);
                sub_080199E0(p, 2);
                break;
            case 0x530:
            case 0x5E7:
                {
                    u32 ev = (((1 - p) & 1) << 31);
                    u32 b = gUnk_02018450.defSlot << 16;
                    b |= 0x1C400000;
                    ev |= b;
                    ev |= gUnk_02018450.side[(1 - p)].cardId;
                    sub_0801FBCC(ev,
                        ((((p & 15) | gUnk_02018450.atkSlot << 4) | ((((1 - p) & 15) | gUnk_02018450.defSlot << 4) << 8)) << 16) | gUnk_02018450.side[p].damage);
                }
                break;
            }
            {
                int k = ((const u16 *)0x08622AB4)[gUnk_02018450.side[p].cardId & 0x7FF];
                if (k == 0x2DA || k == 0x536) {
                    if (sub_0800A430(p, gUnk_02018450.atkSlot) != 0xFFFF && !gUnk_020192E4[(1 - p) & 1].f0 && !gUnk_020192E4[(1 - p) & 1].f1)
                        sub_08019894((1 - p), gUnk_02018450.side[p].damage, (u8)p | gUnk_02018450.atkSlot << 8, (u8)(1 - p) | gUnk_02018450.defSlot << 8);
                }
            }
        }
        STEP++;
        return 0;
    case 1:
        if (gUnk_02018450.side[(1 - p)].damage != 0 && !gUnk_020192E4[(1 - p) & 1].f0 && !gUnk_020192E4[(1 - p) & 1].f1
            && sub_0800A2A8((1 - p), 0x39) != 0) {
            STEP = 10;
            return 0;
        }
        STEP++;
        return 0;
    case 2:
        if (gUnk_02018450.side[(1 - p)].damage != 0 && !gUnk_020192E4[(1 - p) & 1].f0 && !gUnk_020192E4[(1 - p) & 1].f1) {
            sub_08019894((1 - p), gUnk_02018450.side[(1 - p)].damage, (u8)p | gUnk_02018450.atkSlot << 8, (u8)(1 - p) | gUnk_02018450.defSlot << 8);
            switch (((const u16 *)0x08622AB4)[gUnk_02018450.side[p].cardId & 0x7FF]) {
            case 0x71:
                sub_080197E0(p, gUnk_02018450.side[p].cardId);
                sub_08022784((1 - p), 1, 1);
                break;
            case 0xDB:
                sub_080197E0(p, gUnk_02018450.side[p].cardId);
                sub_080199E0(p, 1);
                break;
            case 0x20A:
                sub_080197E0(p, gUnk_02018450.side[p].cardId);
                sub_080199E0((1 - p), 2);
                break;
            case 0x530:
            case 0x5E7:
                {
                    u32 ev = p << 31;
                    u32 b = gUnk_02018450.atkSlot << 16;
                    b |= 0x1A400000;
                    ev |= b;
                    ev |= gUnk_02018450.side[p].cardId;
                    sub_0801FBCC(ev,
                        (((((1 - p) & 15) | gUnk_02018450.defSlot << 4) | (((p & 15) | gUnk_02018450.atkSlot << 4) << 8)) << 16) | gUnk_02018450.side[(1 - p)].damage);
                }
                break;
            }
            {
                int k = ((const u16 *)0x08622AB4)[gUnk_02018450.side[(1 - p)].cardId & 0x7FF];
                if (k == 0x2DA || k == 0x536) {
                    if (sub_0800A430((1 - p), gUnk_02018450.defSlot) != 0xFFFF && !gUnk_020192E4[p & 1].f0 && !gUnk_020192E4[p & 1].f1)
                        sub_08019894(p, gUnk_02018450.side[(1 - p)].damage, (u8)p | gUnk_02018450.atkSlot << 8, (u8)(1 - p) | gUnk_02018450.defSlot << 8);
                }
            }
        }
        STEP++;
        return 0;
    case 10:
        if (p != 0) {
            sub_080753F4(buf, gUnk_08085B7C, gUnk_0822C720 + (gUnk_08623E66[0] << 6));
            sub_080602A4(0x204, 0x915, 0xB, buf);
            STEP++;
            sub_08060308(1, 0, 0);
            return 0;
        }
        if (!gUnk_02015EE8.link) {
            gUnk_0201AE60.v14 = 1;
            return 0;
            STEP++;
        }
        sub_0802297C(0xF057, gUnk_08623E66[0], 0, 0);
        STEP++;
        gUnk_02017FB0[0x450] &= ~3;
        return 0;
    case 11:
        if (!(gUnk_02017FB0[0x450] & 2))
            return 0;
        gUnk_0201AE60.v14 = *(u16 *)(gUnk_02017FB0 + 0x45A);
        STEP++;
        return 0;
    case 12:
        if (gUnk_0201AE60.v14 != 0) {
            sub_080197C0((1 - p), gUnk_08623E66[0]);
            if (sub_0801A130((1 - p), 0x39) != 0)
                gUnk_02018450.side[(1 - p)].damage = 0;
        }
        STEP = 2;
        return 0;
    default: {
        u8 *e2 = gUnk_020192E0;
        ((struct PlF *)(e2 + 4))[1].f0 = 0;
        ((struct PlF *)(e2 + 4))[0].f0 = 0;
        if (((const u16 *)0x08622AB4)[BSV(1 - p)->cardId & 0x7FF] == 0x4B1 && (int)(BSV(1 - p)->raw << 28) >= 0) {
            s16 qb = (1 - p) & 1;
            u8 s1 = gUnk_02018450.defSlot * 0x94 + qb * 0xD64;
            u8 *zb = e2 + 0x2C;
            if (((struct ZoneF6 *)(s1 + (int)zb))->b6 & 1) {
                sub_080197E0((1 - p), BSV(1 - p)->cardId);
                sub_08018ED8((1 - p), gUnk_02018450.defSlot, 0, 0);
            }
        }
        {
            s16 i;
            for (i = 0; i < 2; i++) {
                s8 f = gUnk_02018450.side[i].raw;
                if ((int)(f << 25) < 0 && (int)(f << 28) >= 0) {
                    sub_08017AB4(p, gUnk_08624730[0], (u8)i | (i == p ? gUnk_02018450.atkSlot : gUnk_02018450.defSlot) << 8, 3);
                }
            }
        }
        return 1;
    }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804CB58", sub_0804CB58); /* 0x0804CB58 size 0x740 */

/* Battle hook: when bit 2 is set and the card at +2 is usable for (1-player), queue event 0x91 with the defender slot. Always returns 1. */
int sub_0804D298(int player)
{
    int p;
    u32 r;
    if (gUnk_02018450.bit2) {
        p = 1 - player;
        r = (u16)sub_0802CFA0(p, gUnk_02018450.cardId, 0);
        if (((const u16 *)0x08622AB4)[gUnk_02018450.cardId & 0x7FF] == 0x2FA) {
            if ((int)(gUnk_02018450.side[p].raw << 28) < 0)
                r = 0;
        }
        if (r != 0) {
            u32 ev = ((1 - player) & 1) << 31;
            u32 b = gUnk_02018450.defSlot << 16;
            b |= 0x24400000;
            ev |= b;
            ev |= gUnk_02018450.cardId;
            sub_0801FBCC(ev, 0);
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatching/code_0804CB58", sub_0804D320); /* 0x0804D320 size 0x84C */
