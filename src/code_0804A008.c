#include "global.h"

struct DuelZone {
    u32 card;
    u8 unk4;
    u8 unk5;
    u8 f6_0 : 1;
    u8 f6_rest : 7;
    u8 flags7;
    u8 unk8[0x8C - 8];
    u8 b8C;
    u8 unk8D[0x91 - 0x8D];
    u8 f91_0 : 1;
    u8 f91_1 : 1;
    u8 f91_2 : 1;
    u8 f91_3 : 1;
    u8 f91_4 : 4;
    u8 unk92[2];
};
struct PlayerState {
    u16 lp;
    u8 unk2[6];
    u8 b8;
    u8 b9;
    u8 unkA[0x24 - 10];
    u16 w24;
    u16 w26;
    struct DuelZone zones[11];
    u8 filler[0xD64 - 0x28 - 11 * 0x94];
};
extern struct PlayerState gUnk_020192E4[2];
extern const u16 gUnk_08622AB4[];
struct DuelScreenView { u8 unk0[0x824]; u32 w824; u32 w828; u32 w82C; };
extern struct DuelScreenView gUnk_0201CFB0;
struct ScratchRef { u8 unk0[2]; u8 f0 : 1; u8 rest : 7; u8 unk3[0x11]; };
u16 sub_0805ECFC(void);
int sub_0800966C(u16 id);
int sub_080094E4(void);
int sub_0802CD28(u16 id);
int sub_0802DEC4(struct ScratchRef *ref, int a, int b);
int sub_08049514(u16 id, int a, int zone);
int sub_08049B74(u16 id, int a, int zone);
int sub_08049DF0(u16 id, int a, int zone);
int sub_0804A1C8(void);
void sub_0801DC04(void);
void sub_0801E260(void);
u32 sub_0805304C(void);
u16 sub_0804A008(void);
int sub_0800A368(int player);
void sub_0802AF34(int player, int row, int a, int b);
void sub_08077AEC(u16 se);
/* gBattle (0x02018450), byte view and halfword view. */
struct BattleB {
    u8 attacker : 1;
    u8 direct : 1;
    u8 unk2 : 1;
    u8 f3 : 1;
    u8 f4 : 1;
    u8 f5 : 1;
    u8 unk6 : 2;
    u8 unk1_0 : 1;
    u8 defSlot : 3;
    u8 unk1_4 : 4;
    u8 pad[0x20];
};
struct BattleH {
    u16 unk0_0 : 6;
    u16 atkSlot : 3;
    u16 defSlot : 3;
    u16 unk0_12 : 4;
    u8 pad[0x20];
};
extern struct BattleH gUnk_02018450;
#define BTB (*(struct BattleB *)&gUnk_02018450)
#define BTH (*(struct BattleH *)&gUnk_02018450)
#define BT_U16(off) (*(u16 *)((u8 *)&gUnk_02018450 + (off)))
/* Duel global 0x020192E0 +0x1B14 word view */
struct DGWord {
    u32 unk0 : 9;
    u32 stage : 8;
    u32 unk17 : 15;
};
#define DGW (*(struct DGWord *)((u8 *)&gUnk_020192E0 + 0x1B14))
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Level-like value into v: 0 for types 21-23, 10 for type 24, else stat bits 25-28. */
#define CARD_LEVEL(v, id)                                  \
    switch ((int)CARD_TYPE(id)) {                          \
    case 21:                                               \
    case 22:                                               \
    case 23:                                               \
        v = 0;                                             \
        break;                                             \
    case 24:                                               \
        v = 10;                                            \
        break;                                             \
    default:                                               \
        v = (CARD_STATS(id) & 0x1E000000) >> 25;           \
        break;                                             \
    }
#define TBL(id) (*(u16 *)((u8 *)gUnk_08622AB4 + (((id) & 0x7FF) << 1)))
struct CardInfo {
    u16 id;
    u8 attr : 5;
    u8 typeRest : 3;
    u8 unk3;
    int value;
    int unk8;
};
void sub_0800ABC8(int player, int index, struct CardInfo *out);
int sub_0800A78C(int player, int zone, u16 number);
int sub_0800A8CC(int player, int zone, u16 number);
int sub_08008AF8(int player, int zone);
int sub_08008860(int player);
u16 sub_0804A47C(u16 v);
/* Selection widget at 0x020192E0+0x1B2C (see code_0801CE68). */
struct SelMask {
    u16 flag0 : 1;
    u16 active : 1;
    u16 cursor : 4;
    u16 rows : 4;
    u32 mask : 16;
    u32 state : 8;
    u32 unk34 : 8;
    u32 unk42 : 8;
    u16 timer : 7;
    u16 player : 1;
    u32 zone : 7;
    u32 unk65 : 8;
    u32 unk73 : 23;
};
struct DuelGlobal {
    u8 unk0[0xC];
    u8 b0C;
    u8 unkD[0x1B10 - 0xD];
    u16 w1B10;
    u8 b1B12;
    u8 unk1B13[3];
    u16 f1B16_0 : 1;
    u16 cnt1B16 : 8;
    u16 f1B16_9 : 7;
    u8 unk1B18[0x1B26 - 0x1B18];
    u8 f1B26_0 : 1;
    u8 f1B26_1 : 7;
    u8 unk1B27[0x1B2C - 0x1B27];
    struct SelMask sel;
};
void sub_0801EC58(u16 msg, u16 a, u16 b, u16 c);
struct DGCnt { u16 f0 : 1; u16 cnt : 8; u16 rest : 7; u8 pad[0x20]; };
void sub_08024134(int player, int a, int b);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, u32 b, u32 c);
int sub_0805809C(int a);
void sub_0804F310(void);
void sub_0804F384(void);
extern const u8 gUnk_08085878[], gUnk_080858E4[];
struct MainView { u8 unk0[6]; u16 h6; };
extern struct MainView gUnk_03000040;
struct Unk02015F00 { u8 unk0[0xC]; u8 f0 : 4; u8 v : 3; u8 f7 : 1; u8 pad[0x20]; };
extern struct Unk02015F00 gUnk_02015F00;
struct Unk0201AE60 { u8 unk0[0x14]; u16 h14; u8 pad[0x20]; };
extern struct Unk0201AE60 gUnk_0201AE60;
extern struct DuelGlobal gUnk_020192E0;
int sub_08008524(int player, int id);
u16 sub_0804A528(int player, int zone, u16 flag);
void sub_0804A848(struct PlayerState *ps, int player, int a, u16 flag);
int sub_08008F74(int player);
u16 sub_08008FDC(int player);
struct Z7 { u8 pad[7]; u8 f0 : 5; u8 f5 : 1; u8 rest : 2; u8 pad2[0x8C]; };
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)&gUnk_020192E4[0].zones[0]))
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)&gUnk_020192E4[0].zones[0]))

#if 0 /* NONMATCHING: the ROM shares only the sub_08049DF0 call between the 3 call sites (the tail
       * sits in case 10). Ours also merges the argument setup and keeps the tail in the first
       * branch. */
u16 sub_0804A008(void)
{
    struct ScratchRef ref;
    u16 flags = 1;
    u16 id = sub_0805ECFC();
    if (sub_0800966C(id))
        return 1;
    if (gUnk_020192E0.b1B12 & 2) {
        struct DuelScreenView *cur = &gUnk_0201CFB0;
        if (cur->w828 == 5 && sub_0802CD28(id) > 1 && cur->w824 == 0) {
            s16 zn = cur->w82C;
            flags |= sub_08049DF0(id, 0, zn);
        }
        return flags;
    }
    switch (gUnk_0201CFB0.w828) {
    case 11:
        if (id == 0)
            return 0;
        if (gUnk_0201CFB0.w824 == 0)
            flags |= sub_08049514(id, 0, gUnk_0201CFB0.w82C);
        break;
    case 0:
        if (id == 0)
            return 0;
        if (gUnk_0201CFB0.w824 == 0)
            flags |= sub_08049B74(id, 0, gUnk_0201CFB0.w82C);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
        break;
    case 5:
        if (id == 0)
            return 0;
        if (gUnk_0201CFB0.w824 == 0)
            flags |= sub_08049DF0(id, 0, gUnk_0201CFB0.w82C);
        break;
    case 10:
        if (id == 0)
            return 0;
        if (gUnk_0201CFB0.w824 == 0)
            flags |= sub_08049DF0(id, 0, 5);
        break;
    case 13:
        flags = 0x200;
        if (((u32)(gUnk_020192E0.b1B12 << 27) >> 29) == 0)
            flags = 0x100;
        break;
    case 12:
        if (sub_080094E4() == 0x60B) {
            ref.f0 = 0;
            if (sub_0802DEC4(&ref, 0, 0))
                flags |= 0x400;
        }
        break;
    }
    return flags;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804A008", sub_0804A008); /* 0x0804A008 size 0x1C0 */
#if 0 /* NONMATCHING: the logic is decoded and the structure is close (0x370 vs 0x394 bytes). The ROM
       * keeps the flag0=1 stores of the 3 pick paths separate, keeps the sel pointer in r4
       * across sub_0804A008, and orders the zone address (p&1 first) differently. */
int sub_0804A1C8(void)
{
    u8 b;
    struct SelMask *sel;
    u32 p, row;
    u16 id;
    {
        struct DuelGlobal *e0 = &gUnk_020192E0;
        b = *((u8 *)e0 + 0x1B2C);
    }
    if (b & 1) {
        sub_0801DC04();
        return 1;
    }
    if (b & 2) {
        sub_0801E260();
        return 1;
    }
    if (sub_0805304C() == 0)
        return 0;
    p = gUnk_0201CFB0.w824;
    row = gUnk_0201CFB0.w828;
    id = sub_0805ECFC();
    switch (row) {
    case 0:
    case 5:
    case 10:
        if (id == 0)
            goto fail;
        if (gUnk_0201CFB0.w824 != 0) {
            struct DuelZone *zn = ZB(gUnk_0201CFB0.w824 & 1, gUnk_0201CFB0.w828 + gUnk_0201CFB0.w82C);
            if (!(((u8 *)zn)[6] & 2))
                goto fail;
        }
        sel->flag0 = 1;
        sel = &gUnk_020192E0.sel;
        goto pick;
    case 11:
        if (gUnk_0201CFB0.w824 != 0) {
            if (sub_0800A368(gUnk_0201CFB0.w824) == 0)
                goto fail;
        }
        if (id == 0)
            goto fail;
        sel = &gUnk_020192E0.sel;
        sel->flag0 = 1;
        goto pick;
    case 12:
    case 13:
        if (p != 0)
            goto fail;
        sel = &gUnk_020192E0.sel;
        sel->flag0 = 1;
    pick:
        {
            struct DuelGlobal *e = &gUnk_020192E0;
            e->sel.state = 0;
            sel->mask = sub_0804A008();
        }
        return 1;
    case 14:
    case 15:
        sub_08077AEC(1);
        sub_0802AF34(p, row, 0, 0);
        return 0;
    case 1:
    case 2:
    case 3:
    case 4:
    case 6:
    case 7:
    case 8:
    case 9:
    default:
        return 0;
    }
fail:
    sub_08077AEC(3);
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804A008", sub_0804A1C8); /* 0x0804A1C8 size 0x1D4 */

void sub_0804A39C(int player, int zone)
{
    u32 pl = player & 1;
    struct DuelZone *z = ZB(pl, zone);
    z->flags7 |= 4;
    gUnk_020192E4[pl].w26 |= 1 << zone;
}

int sub_0804A3D8(int player, int zone)
{
    int one = 1;
    int pl = player & one;
    int v = *(u16 *)((u8 *)gUnk_08622AB4 + ((ZB2(pl, zone)->card << 21) >> 20));
    switch (v) {
    case 0x182:
    case 0x18C:
    case 0x18D:
    case 0x1A5:
    case 0x1E7:
    case 0x27A:
        return 1;
    case 0x2D6:
    case 0x2D7:
    case 0x2D8:
    case 0x2FE:
        if (sub_08008F74(1 - player) == 0)
            return 1;
        return 0;
    case 0x32C:
        return sub_08008FDC(one - player);
    default:
        return 0;
    }
}
u16 sub_0804A47C(u16 v)
{
    int p, z;
    for (p = 0; p < 2; p++) {
        for (z = 5; z <= 9; z++) {
            struct DuelZone *zn = ZB(p & 1, z);
            u32 id = zn->card << 20 >> 20;
            if (id != 0 && (((u8 *)zn)[6] & 2) && !zn->f91_3
                && *(u16 *)((u8 *)gUnk_08622AB4 + ((id & 0x7FF) << 1)) == 0x47A
                && (*(u32 *)((u8 *)zn + 0x90) << 14 >> 27) == v)
                return 1;
        }
    }
    return 0;
}

/* The narrow local ID and helper argument preserve the original lookup shape. */
static inline u16 DuelCardNumber(u16 id)
{
    return ((const u16 *)0x08622AB4)[id & 0x7FF];
}

u16 sub_0804A528(int player, int zone, u16 flag)
{
    struct DuelZone *zn = &gUnk_020192E4[player & 1].zones[zone];
    struct CardInfo info;
    u32 fd = zn->f6_0;
    u16 id = zn->card << 20 >> 20;
    u32 attr, val, lv;
    if (id == 0)
        return 0;
    if (!(((u8 *)zn)[6] & 2))
        return 0;
    if ((gUnk_020192E4[player & 1].w26 >> zone) & 1)
        return 0;
    sub_0800ABC8(player, zone, &info);
    attr = info.attr;
    val = (u16)info.value;
    if (sub_0800A78C(player, zone, 0x15C))
        return 0;
    if (sub_0800A78C(player, zone, 0x4DC))
        return 0;
    if (sub_0800A8CC(player, zone, 0x60C))
        return 0;
    if (sub_0800A8CC(player, zone, 0x417) && attr != 7)
        return 0;
    if (sub_0804A47C(attr))
        return 0;
    if (sub_0800A78C(player, zone, 0x2D9))
        return 0;
    if (sub_0800A78C(player, zone, 0x534))
        return 0;
    if (sub_08008524(0, 0x46B) > 0 || sub_08008524(1, 0x46B) > 0) {
        if (val > 0x5DB)
            return 0;
    }
    if (sub_08008524(0, 0x52B) > 0 || sub_08008524(1, 0x52B) > 0) {
        CARD_LEVEL(lv, id);
        if (lv > 3)
            return 0;
    }
    if (sub_08008524(1 - player, 0x548) > 0 && attr == 10)
        return 0;
    if (sub_0800A78C(player, zone, 0x58B))
        return 0;
    if (DuelCardNumber(id) == 0x5F3 && sub_08008860(1 - player) == 0)
        return 0;
    if (sub_08008524(0, 0x536) > 0 || sub_08008524(1, 0x536) > 0) {
        if (DuelCardNumber(id) != 0x536)
            return 0;
    }
    if (flag != 0) {
        switch (DuelCardNumber(id)) {
        case 0x226:
            if (gUnk_020192E4[player & 1].lp <= 0x3E7)
                return 0;
            break;
        case 0x2D6:
        case 0x2D7:
        case 0x2D8:
        case 0x2FE:
            if (gUnk_020192E4[player & 1].lp <= 0x1F3)
                return 0;
            break;
        case 0x2E8:
        case 0x2F9:
            if (sub_08008AF8(player, zone) == 0)
                return 0;
            break;
        }
    }
    if (DuelCardNumber(id) == 0x31C) {
        id = 0;
        fd = id;
    }
    if (fd != 0)
        return 0;
    if (zn->flags7 & 0x10)
        return 0;
    if (zn->b8C & 0x18)
        return 0;
    return 1;
}

#if 0 /* NONMATCHING: register allocation of the player/zero/g/loop copies differs (the ROM copies
       * me/g into new registers at the loop head). */
void sub_0804A848(struct PlayerState *ps, int player, int a, u16 flag)
{
    struct PlayerState *me = &ps[player & 1];
    struct PlayerState *g;
    s16 w24;
    int i, x, zero;
    me->w24 = 0;
    if (sub_08008524(1 - player, 0x15B) != 0)
        return;
    if (sub_08008524(0, 0x4CE) != 0)
        return;
    zero = sub_08008524(1, 0x4CE);
    if (zero != 0)
        return;
    w24 = gUnk_020192E4[player & 1].lp;
    x = sub_08008524(0, 0x42A);
    x += sub_08008524(1, 0x42A);
    if (w24 < x * 500)
        return;
    g = &gUnk_020192E4[player & 1];
    if (flag != 0)
        me->w26 = zero;
    for (i = 0; i <= 4; i++) {
        if (sub_0804A528(player, i, 1) != 0) {
            if (flag != 0 || !((g->w26 >> i) & 1))
                me->w24 |= 1 << i;
        }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804A008", sub_0804A848); /* 0x0804A848 size 0xE4 */

int sub_0804A92C(int player)
{
    struct DuelGlobal *e = &gUnk_020192E0;
    struct PlayerState *ps, *ps0;
    if (e->w1B10 == 0)
        return 0;
    ps0 = (struct PlayerState *)((u8 *)e + 4);
    ps = ps0 + (player & 1);
    if ((ps->b9 << 27) < 0 && (ps->b8 << 25) >= 0)
        return 0;
    sub_0804A848(gUnk_020192E4, player, 0, 1);
    return gUnk_020192E4[player & 1].w24 != 0;
}
int sub_0804A998(void)
{
    return 0;
}
#if 0 /* NONMATCHING: the logic is decoded. The ROM keeps player in r7, done in r6 and ok in r8,
       * and recomputes (lsr 29) from one cached (lsl 23) for every zone access (no CSE of the
       * zone address). Ours CSEs and needs r9. */
int sub_0804A99C(int player)
{
    int ok = 0;
    if (sub_0804A528(player, BTH.atkSlot, 0) == 0) {
        int done = 0;
        u16 pl = player & 1;
        u32 sh = (u32)*(u16 *)&gUnk_02018450 << 23;
        if (*(u16 *)((u8 *)gUnk_08622AB4 + ((ZB(pl, sh >> 29)->card << 21) >> 20)) == 0x538
            && (ZB(pl, sh >> 29)->flags7 & 0x20)) {
            ((struct Z7 *)ZB(pl, sh >> 29))->f5 = 0;
            done = 1;
        }
        if (done == 0)
            sub_0804A39C(player, BTH.atkSlot);
        BTB.direct = 0;
        gUnk_020192E0.cnt1B16 = 0;
        DGW.stage = 1;
        return 1;
    }
    {
        int opp = 1 - player;
        if (sub_08008860(opp) != BT_U16(0x148 + opp * 2))
            ok = 1;
        if (!(*(u8 *)&gUnk_02018450 & 2)) {
            struct DuelZone *zn = ZB(opp & 1, BTB.defSlot);
            if ((zn->card << 20) == 0)
                ok = 1;
            zn = ZB(opp & 1, BTB.defSlot);
            if ((zn->card << 20) != 0 && BT_U16(0x14C + opp * 2) != *(u16 *)((u8 *)zn + 4))
                ok = 1;
        }
        if (*(u8 *)&gUnk_02018450 & 2) {
            if (sub_08008860(1 - player) > 0 && sub_0804A3D8(player, BTH.atkSlot) == 0)
                ok = 1;
        }
    }
    if (ok == 0)
        return 0;
    BTB.direct = 0;
    gUnk_020192E0.cnt1B16 = 0;
    DGW.stage = 1;
    BTB.f3 = 1;
    BTB.f4 = 1;
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804A008", sub_0804A99C); /* 0x0804A99C size 0x1F4 */

int sub_0804AB90(int player)
{
    struct DuelGlobal *e = &gUnk_020192E0;
    u32 c = e->cnt1B16;
    if (c == 0) {
        sub_0801EC58(player ? 0x8032 : 0x32, 1, 0, 0);
        e->cnt1B16++;
        return 0;
    }
    sub_0804A848((struct PlayerState *)((u8 *)e + 4), player, 0, 1);
    sub_0801EC58((e->b1B12 & 2) ? 0x8053 : 0x53, 0, 0, 0);
    return 1;
}
#if 0 /* NONMATCHING: the logic is decoded and the code is identical to the ROM before scheduling.
       * The ROM cross-jumps the `strh cnt; b ret0` tails and the sub_0801EC58 call of the p!=0
       * path with the default path, and ours does not (0x24C vs larger). */
int sub_0804AC18(int player)
{
    struct DuelGlobal *e = &gUnk_020192E0;
    struct DGCnt *c = (struct DGCnt *)((u8 *)e + 0x1B16);
    struct BattleH *bh;
    u8 msg, a1;
    switch (c->cnt) {
    case 0:
        sub_0804A848((struct PlayerState *)((u8 *)e + 4), player, 0, 0);
        BTB.direct = 0;
        BTB.unk2 = 0;
        BTB.f5 = 0;
        sub_08024134(player, 0, 0);
        c->cnt++;
        goto ret0;
    case 1:
        if (player == 0) {
            if (sub_0804A1C8() != 0) {
            ret0:
                return 0;
            }
            if (!(gUnk_03000040.h6 & 2))
                goto ret0;
            if ((e->b0C << 25) < 0)
                sub_080602A4(0x204, 0x616, 0xB, gUnk_08085878);
            else
                sub_080602A4(0x204, 0x616, 0xB, gUnk_080858E4);
            sub_08060308(5, (u32)sub_0804F310, (u32)sub_0804F384);
            {
                struct DuelGlobal *e2 = &gUnk_020192E0;
                e2->cnt1B16 = 10;
            }
            goto ret0;
        }
        if (sub_0805809C(0)) {
            bh = &BTH;
            bh->atkSlot = gUnk_02015F00.v;
            sub_0801EC58(0x8008, gUnk_0201CFB0.w824, bh->atkSlot << 8, 0);
            goto clear;
        }
        ((struct DGWord *)((u8 *)e + 0x1B14))->stage = 0xC;
        c->cnt = 0;
        goto ret0;
    case 10:
        switch (gUnk_0201AE60.h14) {
        case 0:
            ((struct DGWord *)((u8 *)e + 0x1B14))->stage = 0xC;
            c->cnt = 0;
            e->f1B26_0 = 0;
            goto ret0;
        case 1:
            ((struct DGWord *)((u8 *)e + 0x1B14))->stage = 0xC;
            c->cnt = 0;
            e->f1B26_0 = 1;
            goto ret0;
        case 2:
            c->cnt = 1;
            goto ret0;
        }
        goto ret0;
    default:
        msg = player ? 0x8008 : 8;
        a1 = player;
        bh = &BTH;
        sub_0801EC58(msg, a1, bh->atkSlot << 8, 0);
    clear:
        ((struct BattleB *)bh)->f4 = 0;
        ((struct BattleB *)bh)->f3 = 0;
        return 1;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0804A008", sub_0804AC18); /* 0x0804AC18 size 0x24C */
INCLUDE_ASM("asm/nonmatching/code_0804A008", sub_0804AE64); /* 0x0804AE64 size 0x7DC */
