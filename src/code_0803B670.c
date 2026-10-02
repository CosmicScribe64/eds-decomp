#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors. Each is `int f(struct CardRef *ref)` and returns 0
 * or a step state (0x7E or 0x7F). See wiki/functions/code-0803b670.md.
 */

/* DuelCard/DuelZone/DuelZonesPlayer and gUnk_0201930C come from duel.h. */
#define ZFLAGS(z) (((u8 *)(z))[6])
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
/* Same address, but with the player term first (the ROM adds the terms in this order in some places). */
#define ZBP(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))

struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;       /* +0x04 bit 2: effect already handled/skipped (hypothesis) */
    u8 flag4_3 : 1;     /* +0x04 bit 3 */
    u8 unk4_4 : 4;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;  /* +0x0A bits 0-2 */
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[2];     /* +0x0C: low byte player, high byte zone */
};

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_ID11(w) (((w) << 21) >> 21)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern u8 gUnk_02017A40[];
#define EFF_PHASE gUnk_02017A40[0x3E0]  /* 0x7D-0x80: step of a multi-step effect (hypothesis) */
#define EFF_SIDE gUnk_02017A40[0x3E1]
/* gUnk_020192E0 (struct DuelState) comes from duel.h. */
/* sub_0803BED0 reads gUnk_020192E0 as raw bytes (byte at +0x1B12 and the u16s at +0x1B64/+0x1B66).
 * Indexing the canonical struct folds the offsets into one reloc constant, which does not match;
 * a byte-array view of the same symbol reproduces the ROM's base+offset codegen. This unit keeps
 * this local view for sub_0803BED0 only. */
struct DuelStateBytes {
    u8 bytes[0x1B68];
};
extern struct DuelStateBytes gDuelStateView asm("gUnk_020192E0");

void sub_08022678(int a, int b, int c, int d);
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void sub_08030028(int player, int zone);
void sub_08046CB0(int player, int a, int b);
int sub_08076F9C(void);
int sub_08044224(int player, int number, int b);
void sub_08056094(int player, u32 *card, int a, int b);
extern u16 gUnk_0201D81C[];   /* = gUnk_0201D810.cards[0] (list viewer, see code_0802AAC0) */
void sub_08019788(int player, int id);
int __modsi3(int a, int b);
void sub_08017AB4(int player, int a, u16 b, u16 c);
struct AE60 {
    u8 unk0[0x14];
    u16 flag14;
};
extern struct AE60 gUnk_0201AE60;
extern char gUnk_080831E4[];
void sub_080602A4(int a, int b, int c, char *s);
void sub_08060308(int a, int b, int c);
int sub_0802F200(struct CardRef *ref, int a, int b);
u32 sub_08052F38(u32 keys);
int sub_08007834(u16 id);
int sub_08008A44(int player);
int sub_08008AF8(int player, int exclude);
int sub_08008A6C(int player, int zone);
void sub_08077AEC(u16 id);
void sub_08017FF4(int player, int zone);
void sub_08055D3C(int player, int a, int b, int c);
void sub_08075294(void *dest, const void *src, u32 size);
int sub_080304E4(void *ref, int a);
int sub_080307D4(void *ref, int a);
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];
extern char gUnk_08083300[];
extern char gUnk_08083350[];
extern char gUnk_0808339C[];
extern char gUnk_080833EC[];
/* gUnk_020192E4 (struct DuelPlayer[2]) comes from duel.h. */
/* HandRow uses the canonical struct DuelCard from duel.h. */
struct HandRow {        /* 0x02019968 = 0x020192E4 + 0x684: hand card words */
    struct DuelCard c[80];
    u8 pad[0xD64 - 80 * 4];
};
extern struct HandRow gUnk_02019968[2];
/* Duel screen state at 0x0201CFB0: +0x824.. hold the selection */
struct DuelScreen {
    u8 pad0[0x824];
    u32 w824;           /* low half: player-ish selection (hypothesis) */
    u32 w828;           /* low byte: column */
    u32 idx82C;         /* +0x82C: selected index (low byte: row) */
};
extern struct CardRef gUnk_02017F24;
extern struct DuelScreen gUnk_0201CFB0;

/* Monster level: Magic/Trap types 0x15-0x17 count as 0, type 0x18 as 10. */
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
void sub_08019078(int player, u16 a, u16 b);

void sub_08017B04(int player, u16 a, u16 b);
int sub_0802C5E4(struct CardRef *ref, int a);
void sub_08018ED8(int a, int b, int c, int d);
void sub_08018AE8();
/* Card-list viewer at 0x0201D810 (see code_0802AAC0 struct ListView). */
struct ListView {
    u8 unk0[5];
    u8 row : 2;         /* +0x05 bits 0-1: cursor row */
    u8 unk5_2 : 6;
    u16 top;            /* +0x06: first visible entry */
    u8 unk8[4];
    u32 cards[0x80];    /* +0x0C: card words */
};
extern struct ListView gUnk_0201D810;
extern char gUnk_080837D4[];
extern char gUnk_08083828[];
extern char gUnk_08083880[];
void sub_0802AF34(int player, int area, int a2, int a3);
extern u8 gUnk_0201CF90;    /* bits 1-5: a zone position (hypothesis) */
int sub_08008524(int player, u16 number);
extern char gUnk_08083A6C[];
int sub_0802C080(struct CardRef *ref, u16 pos);
int sub_0800CCCC(int a, int b, int c, int d);
void sub_08017C0C(u16 a, u16 b);
void sub_08018544();
struct CardRef20 {
    struct CardRef r;
    u32 extra;
};
u16 sub_0802CE38(struct CardRef *ref, int a, int b);
void sub_080197C0(int player, int id);
void sub_0801FBCC(u32 a, int b);
void sub_08018C3C(int player, int zone);
void sub_08019800(int player, int id);
void sub_08019840(int player, int id);
void sub_08018DC8();
extern char gUnk_08083960[];
extern char gUnk_080839A4[];
extern char gUnk_080839DC[];
#define EFF_CNT gUnk_02017A40[0x3E2]
extern u8 gUnk_02015EE8[];
int sub_08047170(int player);
int sub_08008A1C(int player);
struct PosWord {
    u32 lo : 12;
    u32 flag12 : 1;     /* bit 12: player (hypothesis) */
    u32 rest : 19;
};
int sub_08009CAC(int player, u16 number);
int sub_08008C6C(int player);
int sub_08008A44(int player);
int sub_08009C08(int player, u16 id, u16 *out);
extern u16 gUnk_02017F84[];
extern char gUnk_080838D4[];
extern char gUnk_08083904[];
extern char gUnk_08083A14[];


int sub_0803B670(struct CardRef *ref)
{
    if (ref->numTargets == 2) {
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8087 : 0x87, ref->zone, ref->targets[0], 0);
        sub_08017B04(ref->player, ref->player | ref->zone << 8, ref->targets[1]);
    }
    return 0;
}
int sub_0803B6C0(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 1 - ref->player;
            EFF_PHASE--;
        case 0x7F: {
            int i;
            int sd;

            for (i = 0; i <= 4; i++) {
                if (sub_0802C5E4(ref, (u8)i << 8 | EFF_SIDE) != 0) {
                    sub_08030028(EFF_SIDE, i);
                    sub_08046CB0(ref->player, EFF_SIDE, i);
                    return 0x7F;
                }
            }
            EFF_SIDE = 1 - EFF_SIDE;
            sd = ref->player;
            if (*(volatile u8 *)&EFF_SIDE == sd)
                return 0x7F;
        }
        default:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8049 : 0x49, gUnk_020192E4[1 & ref->player].flag7_3, 1, 0);
            sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x8049 : 0x49, gUnk_020192E4[(1 - ref->player) & 1].flag7_3, 1, 0);
            break;
        }
    }
    return 0;
}
int sub_0803B7C4(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 2;
            EFF_PHASE--;
        case 0x7F:
            if (EFF_SIDE == 0)
                return 0;
            if (sub_08044224(ref->player, 0x5E7, 0) == 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, EFF_SIDE == 2 ? gUnk_080837D4 : gUnk_08083828);
            sub_08060308(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gUnk_0201AE60.flag14 == 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083880);
            return 0x7D;
        case 0x7D:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7C;
        case 0x7C: {
            u16 *cw = (u16 *)&gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row];

            sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x80D4 : 0xD4, cw[0], cw[1], 0);
            EFF_SIDE--;
            return 0x80;
        }
        }
    }
    return 0;
}
int sub_0803B924(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (1 & ((u8 *)ref)[2])
                return 0;
            if (sub_08044224(ref->player, 0x5E8, 0) == 0)
                return 0;
            if (sub_08008AF8(ref->player, ref->zone) == 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_080838D4);
            return 0x7F;
        case 0x7F:
            if (sub_08052F38(0xF0) != 0) {
                u8 *g = (u8 *)&gUnk_0201CFB0;
                u32 *p = (u32 *)(g + 0x82C);

                if (*p != ref->zone && sub_08008A6C(ref->player, *p) != 0) {
                    sub_08017FF4(ref->player, *p);
                    return 0x7E;
                }
                sub_08077AEC(3);
            }
            return 0x7F;
        case 0x7E:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083904);
            return 0x7D;
        case 0x7D:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7C;
        case 0x7C: {
            u16 *cw = (u16 *)&gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row];

            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80DC : 0xDC, cw[0], cw[1], 0);
            return 0x7B;
        }
        case 0x7B:
            sub_08056094(ref->player, &gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row], 1, 0);
            return 0x7A;
        case 0x7A:
            sub_08017AB4(ref->player, ref->id, ref->player | ((u32)gUnk_0201CF90 << 26 >> 27) << 8, 3);
            return 0x78;
        }
    }
    return 0;
}
int sub_0803BAE8(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 3;
            EFF_CNT = 0;
            EFF_PHASE--;
        case 0x7F:
            if (sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) <= 0)
                goto ret78;
            sub_080602A4(0x206, 0x712, 0xB, EFF_CNT != 0 ? gUnk_08083960 : gUnk_080839A4);
            sub_08060308(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gUnk_0201AE60.flag14 == 0) {
ret78:
                return 0x78;
            }
            sub_080602A4(0x206, 0x712, 0xB, gUnk_080839DC);
            return 0x7D;
        case 0x7D:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7C;
        case 0x7C: {
            u16 *cw = (u16 *)&gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top];

            sub_0801EC58((int)(gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top] << 19) < 0 ? 0x80D4 : 0xD4,
                         cw[0], cw[1], 0);
            EFF_SIDE--;
            EFF_CNT++;
            if (EFF_SIDE == 0)
                goto ret78;
            return 0x7F;
        }
        case 0x78:
            if (EFF_CNT != 0)
                sub_08017AB4(ref->player, ref->id, ref->player | ref->zone << 8, EFF_CNT << 8 | 0xB);
        default:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
            break;
        }
    }
    return 0;
}
#if 0 /* NONMATCHING: logic verified (`int tp = (u8)...` gets closest). The ROM
       * hoists the constant -1 into sl (live across all three compares and the
       * final one), with tp in r7 and tz in r9. This build reloads -1 and puts
       * tp in r8. */
int sub_0803BCE4(struct CardRef *ref)
{
    int none = -1;

    if (sub_08009CAC(ref->player, 0x5EA) != 0 && sub_08008C6C(ref->player) != none && sub_08008A44(ref->player) != none
        && ref->numTargets == 1) {
        s16 tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        u16 a = sub_08008A44(ref->player);

        if (ref->player != tp) {
            int pa = tp & 1;
            struct DuelZone *z = ZB(pa, tz);

            if (CARD_WORD(z->card) << 20 != 0 && (2 & ZFLAGS(z)) != 0) {
                int b = sub_08008C6C(ref->player);

                sub_08009C08(ref->player, ref->id, gUnk_02017F84);
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D3 : 0xD3, gUnk_02017F84[0], gUnk_02017F84[1], 0);
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8077 : 0x77, (u8)b | 0x100, gUnk_02017F84[0], gUnk_02017F84[1]);
                sub_08017B04(ref->player, ref->player | (u8)b << 8, tp | tz << 8);
                if (!ref->skip4 && a != none)
                    sub_08019078(ref->player, ref->targets[0], (u8)a << 8 | ref->player);
            }
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0803B670", sub_0803BCE4); /* 0x0803BCE4 size 0x154 */
int sub_0803BE38(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083A14);
            return 0x7F;
        case 0x7F:
            if (sub_08052F38(0xE00000) != 0) {
                u16 msg = !(1 & ((u8 *)ref)[2]) ? 0x80A1 : 0xA1;
                u8 *g = (u8 *)&gUnk_0201CFB0;
                u32 *p = (u32 *)(g + 0x82C);

                sub_0801EC58(msg, *(u16 *)p, 1, 0);
                sub_08018ED8(1 - ref->player, *p, 0, 0);
                return 0x7E;
            }
            return 0x7F;
        }
    }
    return 0;
}
int sub_0803BED0(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (sub_08044224(1 - ref->player, 0x447, 0) == 0)
                return 0;
            if (sub_08008A1C(1 - ref->player) == 0)
                return 0;
            if (sub_08047170(1 - ref->player) == 0)
                return 0;
            sub_08022678(1 - ref->player, 0x13, 0, 0);
            return 0x7F;
        case 0x7F: {
            u8 *g = gDuelStateView.bytes;

            if (*(u16 *)(g + 0x1B64) == 0)
                return 0;
            sub_08022678(1 - ref->player, 0xE, ref->id, 0);
            return 0x7E;
        }
        case 0x7E: {
            struct PosWord x;
            struct PosWord *px;
            u8 *g = gDuelStateView.bytes;

            *(u32 *)&x = *(u16 *)(g + 0x1B64) | *(u16 *)(g + 0x1B66) << 16;
            px = &x;
            if ((gUnk_02015EE8[1] & 1) && (g[0x1B12] & 2))
                px->flag12 = 1 - px->flag12;
            sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x80D3 : 0xD3, *(u32 *)&x & 0xFFFF, *(u32 *)&x >> 16, 0);
            if ((gUnk_02015EE8[1] & 1) && (gDuelStateView.bytes[0x1B12] & 2))
                px->flag12 = 1 - px->flag12;
            sub_08056094(1 - ref->player, (u32 *)&x, 1, 0x20);
            return 0x7D;
        }
        }
    }
    return 0;
}
int sub_0803C060(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        int i;

        for (i = 0; i < ref->numTargets; i++) {
            int tp = (u8)ref->targets[i];
            int tz = ref->targets[i] >> 8;
            int pa = tp & 1;
            struct DuelZone *z = ZB(pa, tz);

            if (CARD_WORD(z->card) << 20 != 0)
                sub_08018AE8(tp, tz, 1);
        }
    }
    return 0;
}
int sub_0803C0D0(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int n = sub_08044224(ref->player, 0x5F4, 0);

            EFF_SIDE = n;
            if ((u8)n <= 1)
                return 0;
            EFF_SIDE = 2;
            EFF_PHASE--;
        }
        case 0x7F:
            sub_080602A4(0x206, 0x613, 0xB, gUnk_08083A6C);
            return 0x7E;
        case 0x7E:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7D;
        case 0x7D: {
            u16 *cw = (u16 *)&gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row];

            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D2 : 0xD2, cw[0], cw[1], 0);
            EFF_SIDE--;
            if (EFF_SIDE != 0)
                return 0x7F;
            return 0xA;
        }
        }
    }
    return 0;
}
int sub_0803C1D4(struct CardRef *ref, u16 *card)
{
    if (sub_08008524(0, 0x58A) <= 0 && sub_08008524(1, 0x58A) <= 0) {
        sub_08017FF4(ref->player, ref->zone);
        if (card != 0 && CARD_TYPE(*card) == 0x16)
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80B0 : 0xB0, 1, 0, 0);
    }
    return 0;
}
int sub_0803C254(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        int i;

        for (i = 0; i <= 1; i++) {
            int j;

            for (j = 5; j <= 9; j++) {
                int sj = j;
                int pl = i;
                int tp = (u8)ref->targets[0];
                int tz = ref->targets[0] >> 8;
                int ok = 1;
                u8 p = i & 1;
                struct DuelZone *z = ZB(p, j);
                u16 id = CARD_ID(CARD_WORD(z->card));

                if (id != 0 && (2 & ZFLAGS(z)) != 0) {
                    u8 lvl;

                    switch ((int)CARD_TYPE(id)) {
                    case 0x15:
                    case 0x16:
                        lvl = (CARD_STATS(id) & 0xE0000) >> 17;
                        break;
                    default:
                        lvl = 0;
                        break;
                    }
                    if (lvl == 3) {
                        if (sub_0802C080(ref, (u8)pl | (u8)sj << 8) == 0)
                            ok = 0;
                        if (sub_0800CCCC(pl, sj, tp, tz) == 0)
                            ok = 0;
                        if (ok)
                            sub_08017C0C((u8)i | (u8)j << 8, ref->targets[0]);
                        else
                            sub_08018544(i, j, 1);
                    }
                }
            }
        }
    }
    return 0;
}
int sub_0803C368(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int pa = 1 & tp;
        struct DuelZone *z = ZB(pa, tz);

        if ((2 & ZFLAGS(z)) != 0 && CARD_WORD(z->card) << 20 != 0) {
            sub_0801EC58((u8)ref->pos != 0 ? 0x8035 : 0x35, ref->pos >> 8, 1, 0);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, (u8)ref->targets[0], (ref->targets[0] >> 8) << 8, 0);
            if ((1 & ZFLAGS(z)) != 0)
                sub_08018ED8(tp, tz, 0, 0);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8039 : 0x39, ref->targets[0], 1, 0);
        }
    }
    return 0;
}
int sub_0803C434(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        u8 tp = ref->targets[0];
        int tz = ref->targets[0] >> 8;
        u16 want = ref->targets[1];
        int pa = 1 & tp;
        struct DuelZone *z = ZB(pa, tz);
        u32 id = CARD_ID(CARD_WORD(z->card));

        if ((2 & ZFLAGS(z)) != 0 && id != 0) {
            u32 lvl;

            CARD_LEVEL(id, lvl);
            if (lvl == want) {
                sub_08030028(tp, tz);
                sub_08046CB0(ref->player, tp, tz);
            }
        }
    }
    return 0;
}
int sub_0803C4F8(struct CardRef *ref)
{
    u8 b = ((u8 *)ref)[8];
    int pl = b & 0xF;
    int zone = b >> 4;

    if (!ref->skip4) {
        int pa = pl & 1;
        struct DuelZone *z = ZB(pa, zone);

        if (CARD_WORD(z->card) << 20 != 0) {
            sub_08030028(pl, zone);
            sub_08046CB0(ref->player, pl, zone);
        }
    }
    return 0;
}
int sub_0803C550(struct CardRef *ref)
{
    if (ref->numTargets == 1) {
        u8 tp = ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int pa = 1 & tp;
        struct DuelZone *z = ZB(pa, tz);

        if (CARD_WORD(z->card) << 20 != 0 && tp != ref->player)
            sub_08018AE8(tp, tz, 1);
    }
    return 0;
}
int sub_0803C5A0(struct CardRef *ref)
{
    int tp = (u8)ref->targets[0];
    int tz = ref->targets[0] >> 8;
    int pa = 1 & tp;
    struct DuelZone *z = ZB(pa, tz);
    u32 id = CARD_ID(CARD_WORD(z->card));

    if (ref->skip4) {
        sub_08018544(ref->player, ref->zone, 1);
        return 0;
    }
    if (id != 0 && (2 & ZFLAGS(z)) == 0) {
        sub_08018DC8(tp, tz, 0);
        if (CARD_TYPE(id) != 0x15) {
            sub_08019840(tp, id);
            sub_08018DC8(tp, tz, 0);
        } else {
            switch (CARD_NUMBER(id)) {
            case 0x2A8: case 0x2A9: case 0x2AD: case 0x2B1:
            case 0x3B1: case 0x3B6: case 0x3C0: case 0x3C9:
            case 0x3DE: case 0x3EA: case 0x3FB: case 0x3FD: case 0x3FE:
            case 0x404: case 0x405: case 0x406: case 0x407:
            case 0x40E: case 0x420: case 0x426: case 0x431: case 0x44A:
            case 0x46E: case 0x46F: case 0x470: case 0x471: case 0x472: case 0x473: case 0x474: case 0x475: case 0x476:
            case 0x47D: case 0x4BE: case 0x4DF: case 0x515: case 0x518:
            case 0x525: case 0x529: case 0x52D: case 0x590: case 0x591: case 0x594:
            case 0x5F9: case 0x5FB: case 0x5FF:
                goto hit;
            default: {
                struct CardRef20 tmp;
                u32 w, hi;

                tmp.r.id = id;
                tmp.r.player = tp;
                tmp.r.zone = tz;
                tmp.r.kind = 0;
                if (sub_0802CE38(&tmp.r, 0, 0) == 0)
                    goto hit;
                sub_080197C0(tp, id);
                hi = tp << 31;
                w = (tz & 0x1F) << 16;
                w |= 0x200000;
                sub_0801FBCC(hi | w | id, 0);
                goto tail;
            }
            }
        }
    }
    goto tail;
hit:
    sub_08019800(tp, id);
    sub_08018544(tp, tz, 1);
tail:
    sub_08018C3C(ref->player, ref->zone);
    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
    return 0;
}
