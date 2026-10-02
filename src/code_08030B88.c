#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors. Each is `int f(struct CardRef *ref)` and returns 0
 * or a step code such as 0x7F, 0x80 or 0x92. See wiki/functions/code-08030b88.md.
 */

/* DuelCard/DuelZone/DuelZonesPlayer and gUnk_0201930C come from duel.h. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;       /* +0x04 bit 2: effect already negated/skipped (hypothesis) */
    u8 unk4_3 : 5;
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
#define CARD_ID11(w) (((w) << 21) >> 21)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern const u16 gUnk_08622AB4[];

/* Effect-resolution state at 0x02017A40 (byte view; only two bytes used here). */
extern u8 gUnk_02017A40[];
#define EFF_PHASE gUnk_02017A40[0x3E0]  /* 0x7E / 0x7F / 0x80: step of a multi-step effect (hypothesis) */
#define EFF_SIDE gUnk_02017A40[0x3E1]   /* player currently being processed */

/* Same two bytes seen through a struct, for functions that CSE the two addresses. */
struct EffectState {
    u8 unk[0x3E0];
    u8 phase;
    u8 side;
};
#define EFF_STATE ((struct EffectState *)gUnk_02017A40)

/* 0x020192E4 + 0x5F0: a card word in each player's state (stride 0xD64), hypothesis: a "set" spell/trap slot */
struct PlayerCard5F0 {
    struct DuelCard card;
    u8 filler[0xD64 - 4];
};
extern struct PlayerCard5F0 gUnk_020198D4[2];

int sub_08019554(int player, u16 no);
void sub_08019980(int player, int lp);
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
int sub_0802B28C(int player, int zone);
void sub_08030028(int player, int zone);
void sub_08046CB0(int player, int a, int b);
void sub_08017AB4(int player, u16 a, u16 b, u16 c);
u32 sub_08008300(u16 cardNo);
void sub_08042AB0(int player, int kind, u32 arg);
void sub_08018ED8(int player, int zone, int a, int b);
int sub_0800C8BC(int player, int zone);
void sub_080197C0(int player, u16 id);
void sub_08018DC8(int player, int zone, int a);
void sub_08019840(int player, u16 id);
void sub_08046AD0(void);
void sub_08018544(int player, int zone, int a);
void sub_08019860(int player, int lp);
void sub_080602A4(u32 a, u32 b, u32 c, const void *d);
void sub_08060308(u32 a, u32 b, u32 c);
u32 sub_08052F38(u32 keys);
void sub_080193B0(int player, int arg1, u16 arg2);
extern const u8 gUnk_08082AF0[];
extern const u8 gUnk_08082B10[];
extern const u8 gUnk_08082B58[];

/* Player state gUnk_020192E4[p] (canonical struct DuelPlayer from duel.h). */

/* 0x020192E4 + 0x7C4: deck card words (80 entries) of each player, stride 0xD64. */
struct PlayerDeck {
    u32 deck[80];
    u8 filler[0xD64 - 80 * 4];
};
extern struct PlayerDeck gUnk_02019AA8[2];
/* u16 at 0x0201AE60+0x14 (nonzero = flag; hypothesis: a "count/flag" of the current effect) */
struct AE60 {
    u8 unk0[0x14];
    u16 flag14;
};
extern struct AE60 gUnk_0201AE60;
/* Duel screen state at 0x0201CFB0: the word at +0x82C is passed to sub_080193B0. */
struct DuelScreen82C {
    u8 unk0[0x82C];
    u32 unk82C;
};
extern struct DuelScreen82C gUnk_0201CFB0;

/* Card-list viewer at 0x0201D810 (see code_0802AAC0 struct ListView); fields used here. */
struct ListView {
    u8 unk0[5];
    u8 row : 2;         /* +0x05 bits 0-1: cursor row */
    u8 unk5_2 : 6;
    u16 top;            /* +0x06: first visible entry */
    u8 unk8[4];
    u32 cards[0x80];    /* +0x0C: card words */
    u16 kinds[0x80];    /* +0x20C: per-entry kind (hypothesis) */
};
extern struct ListView gUnk_0201D810;
extern u8 gUnk_02015F00[];   /* u16 at +0x1B22 is the saved list position (hypothesis) */
extern const u8 gUnk_08082988[];
int sub_08008524(int player, u16 number);
int sub_08044224(int player, int number, int b);
int sub_08008A1C(int player);
void sub_08056ECC(u16 id);
void sub_0802AF34(int player, int area, int a2, int a3);
void sub_08056094(int player, u32 *card, int a, int b);
void sub_08055F70(int player, u32 *card, int a, int b, int c);
void sub_08046C20(int player, int a);
int sub_0801970C(int player, u16 number);
void sub_080753F4(void *dst, const void *a, const void *b);
extern const u8 gUnk_08082AB4[];
extern const u16 gUnk_08623DF4[];
extern const u8 gUnk_0822C720[];
extern const u8 gUnk_080829F0[];
extern const u8 gUnk_08082A4C[];
void sub_08017FF4(int player, int zone);
int sub_0802B9EC(struct CardRef *ref, u16 pos);


struct S15F00_30B88 { u8 unk0[0x1B22]; u16 listPos; };
#define S15F00_30B88 ((struct S15F00_30B88 *)gUnk_02015F00)
int sub_08030B88(struct CardRef *ref)
{
    int i, n;

    if (ref->skip4)
        return 0;
    switch (EFF_PHASE) {
    case 0x80:
        if (!sub_08008524(0, 0x3D) && !sub_08008524(1, 0x3D) && !sub_08008524(0, 0x4E1)
            && !sub_08008524(1, 0x4E1))
            return 0;
        if (sub_08044224(ref->player, 0x13D, 0) == 0)
            return 0;
        if (sub_08008A1C(ref->player) == 0)
            return 0;
        if (1 & ((u8 *)ref)[2]) {
            n = sub_08044224(1, 0x13D, 0);
            /* `cards + i` (not cards[i]) keeps 0x0201D81C as one pool constant, so the
             * found blocks address the viewer as base - 12. The u16 id locals in the first
             * and third loops add the RTL insns that make loop.c hoist the card table only
             * in its second pass (after the pointer copy), as the ROM does. */
            for (i = 0; i < n; i++) {
                u16 id = CARD_ID(*(gUnk_0201D810.cards + i));
                if (CARD_NUMBER(id) == 0x3E)
                    goto found1;
            }
            for (i = 0; i < n; i++) {
                if (CARD_NUMBER(CARD_ID(*(gUnk_0201D810.cards + i))) == 0x4E1)
                    goto found2;
            }
            for (i = 0; i < n; i++) {
                u16 id = CARD_ID(*(gUnk_0201D810.cards + i));
                if (CARD_NUMBER(id) == 0x3D)
                    goto found3;
            }
            sub_08056ECC(ref->id);
            gUnk_0201D810.row = 0;
            gUnk_0201D810.top = S15F00_30B88->listPos;
            return 0x7E;
        } else {
            sub_080602A4(0x205, 0x914, 0xB, gUnk_08082988);
            return 0x7F;
        }
    case 0x7F:
        sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
        return 0x7E;
    case 0x7E: {
        u16 *c = (u16 *)&gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top];

        switch (gUnk_0201D810.kinds[gUnk_0201D810.row + gUnk_0201D810.top]) {
        case 2:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, c[0], c[1], 0);
            break;
        case 1:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80C2 : 0xC2, c[0], c[1], 0);
            break;
        }
        return 0x7D;
    }
    case 0x7D:
        sub_08056094(ref->player, &gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row], 1, 1);
        return 0x7C;
    case 0x7C:
        if (gUnk_0201D810.kinds[gUnk_0201D810.top + gUnk_0201D810.row] == 2)
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
        return 0x64;
    found1:
        gUnk_0201D810.row = 0;
        gUnk_0201D810.top = i;
        return 0x7E;
    found2:
        gUnk_0201D810.row = 0;
        gUnk_0201D810.top = i;
        return 0x7E;
    found3:
        gUnk_0201D810.row = 0;
        gUnk_0201D810.top = i;
        return 0x7E;
    }
    return 0;
}
int sub_08030E4C(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        u8 tp = ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & 1;
        struct DuelZone *z = ZB(p, tz);
        if (CARD_NUMBER(CARD_ID11(CARD_WORD(z->card))) == 0x4B1 && z->flag6_0 && !z->flag6_1) {
            u16 msg = tp ? 0x807F : 0x7F;

            sub_0801EC58(msg, tz, 0, 0);
            sub_080197C0(tp, CARD_ID(CARD_WORD(z->card)));
        } else {
            int p2 = tp & 1;
            struct DuelZone *z2 = ZB(p2, tz);

            if (z2->flag6_0)
                sub_08018ED8(tp, tz, 1, 1);
        }
    }
    return 0;
}
int sub_08030F04(struct CardRef *ref)
{
    if (!ref->skip4) {
        int p;

        for (p = 0; p <= 1; p++) {
            int i;

            for (i = 0; i <= 4; i++) {
                struct DuelZone *z = ZB(p & 1, i);

                if (z->flag6_1 && !z->flag6_0 && CARD_ID(CARD_WORD(z->card)) && sub_0800C8BC(p, i) == 1)
                    sub_08018ED8(p, i, 0, 0);
            }
        }
    }
    return 0;
}
int sub_08030F84(struct CardRef *ref)
{
    if (CARD_ID(CARD_WORD(gUnk_020198D4[1 & ref->player]))) {
        u16 msg = (1 & ((u8 *)ref)[2]) ? 0x8011 : 0x11;

        sub_0801EC58(msg, sub_08008300(CARD_NUMBER(ref->id)), 1, 0);
        sub_08042AB0(1 - ref->player, 0x18, 0);
    }
    return 0;
}
int sub_08030FFC(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 1 - ref->player;
            EFF_PHASE--;
        case 0x7F: {
            int i;
            
            for (i = 0; i <= 4; i++) {
                if (sub_0802B28C(EFF_SIDE, i)) {
                    sub_08030028(EFF_SIDE, i);
                    sub_08046CB0(ref->player, EFF_SIDE, i);
                    return 0x7F;
                }
            }
            {
                /* FAKEMATCH: the side pointer pinned to r0 */
                register u8 *p asm("r0");
                u8 *b = gUnk_02017A40;
                int sd;

                p = b + 0x3E1;
                *p = 1 - *p;
                sd = ref->player;
                if (*(volatile u8 *)p == sd)
                    return 0x7F;
            }
        }
        }
    }
    return 0;
}
int sub_08031094(struct CardRef *ref)
{
    int i;

    if (ref->skip4)
        return 0;
    for (i = 0; i <= 4; i++) {
        if (sub_0802B28C(1 - ref->player, i)) {
            sub_08030028(1 - ref->player, i);
            sub_08046CB0(ref->player, 1 - ref->player, i);
            return 0x80;
        }
    }
    return 0;
}

int sub_080310EC(struct CardRef *ref)
{
    int amount = 0;

    if (!ref->skip4) {
        switch (CARD_NUMBER(ref->id)) {
        case 0x151:
            amount = 200;
            break;
        case 0x153:
            amount = 600;
            break;
        case 0x154:
            amount = 800;
            break;
        case 0x3EE:
            amount = 400;
            break;
        }
        if (amount != 0) {
            switch (ref->targets[0]) {
            case 0:
                sub_08019980(ref->player, amount);
                break;
            case 1:
                sub_08019980(1 - ref->player, amount);
                break;
            }
        }
    }
    return 0;
}
int sub_08031180(struct CardRef *ref)
{
    if (!ref->skip4) {
        int p;
        u16 lp;

        switch (CARD_NUMBER(ref->id)) {
        case 0x152:
            p = ref->player;
            lp = 500;
            break;
        case 0x155:
            p = ref->player;
            lp = 1000;
            break;
        case 0x523:
            sub_08019980(ref->player, 1000);
            {
                int other = ref->player;

                /* FAKEMATCH: initialized input keeps the reloaded player in r1,
                 * leaving r0 for the original 1-player subtraction. No code emitted. */
                asm("" : : "r"(other) : "r0");
                p = 1 - other;
            }
            lp = 1000;
            break;
        default:
            return 0;
        }
        sub_08019980(p, lp);
    }
    return 0;
}

int sub_08031208(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (CARD_NUMBER(ref->id)) {
        case 0x156:
            sub_08019860(1 - ref->player, 200);
            break;
        case 0x157:
            sub_08019860(1 - ref->player, 500);
            break;
        case 0x158:
            sub_08019860(1 - ref->player, 600);
            break;
        case 0x159:
            sub_08019860(1 - ref->player, 800);
            break;
        case 0x15A:
            sub_08019860(1 - ref->player, 1000);
            sub_08019860(ref->player, 500);
            break;
        case 0x3EF:
            sub_08019860(1 - ref->player, 300);
            break;
        case 0x40F: {
            struct DuelPlayer *pl = gUnk_020192E4;
            int p = (1 - ref->player) & 1;

            if (pl[p].handCount != 0) {
                int a = 1 - ref->player;
                int q = (1 - ref->player) & 1;

                sub_08019860(a, pl[q].handCount * 200);
            }
            break;
        }
        }
    }
    return 0;
}
int sub_08031334(struct CardRef *ref)
{
    if (!ref->skip4) {
        if (EFF_PHASE == 0x80) {
            int i;

            for (i = 0; i <= 4; i++) {
                int opp = 1 - ref->player;
                int p = opp & 1;
                struct DuelZone *z = ZB(p, i);

                if (CARD_ID(CARD_WORD(z->card)) && !z->flag6_1) {
                    sub_08018DC8(opp, i, 1);
                    sub_08019840(opp, CARD_ID(CARD_WORD(z->card)));
                }
            }
            return 0x7F;
        }
        sub_08046AD0();
    }
    return 0;
}
int sub_080313BC(struct CardRef *ref)
{
    if (ref->numTargets == 1) {
        int p = ref->targets[0] & 1;
        struct DuelZone *z = ZB(p, ref->targets[0] >> 8);

        if (CARD_ID(CARD_WORD(z->card)))
            sub_08017AB4(ref->player, ref->player | (ref->zone << 8), ref->targets[0], 2);
    }
    return 0;
}

int sub_08031410(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 4; i++) {
            int p = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(p, i);

            if (CARD_ID(CARD_WORD(z->card))) {
                int p2 = (1 - ref->player) & 1;
                struct DuelZone *z2 = ZB(p2, i);

                if (!z2->flag6_1)
                    sub_08018DC8(1 - ref->player, i, 1);
            }
        }
    }
    return 0;
}
int sub_0803148C(struct CardRef *ref)
{
    if (sub_08019554(ref->player, 0x3EB) == 0)
        sub_08019554(ref->player, 0x40A);
    return 0;
}

int sub_080314BC(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        sub_08017FF4(ref->player, ref->zone);
        for (i = 0; i < ref->numTargets && i <= 1; i++) {
            u8 tp = ref->targets[i];
            int tz = ref->targets[i] >> 8;
            int p = tp & 1;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && sub_0802B9EC(ref, ref->targets[i])) {
                sub_08030028(tp, tz);
                sub_08046CB0(ref->player, tp, tz);
            }
        }
    }
    return 0;
}
struct S15F00_31550 { u8 unk0[0x1B22]; u16 listPos; };
#define S15F00_31550 ((struct S15F00_31550 *)gUnk_02015F00)

int sub_08031550(struct CardRef *ref)
{
    u8 skip = ((u8 *)ref)[4] & 4;

    if (!skip) {
        switch (EFF_PHASE) {
        case 0x80:
            if (gUnk_020192E4[1 & ref->player].fusionCount == 0)
                return 0;
            switch (CARD_NUMBER(ref->id)) {
            case 0x1A3:
                if (sub_08008A1C(ref->player) == 0)
                    return 0;
                if (1 & ((u8 *)ref)[2]) {
                    sub_08056ECC(ref->id);
                    gUnk_0201D810.row = 0;
                    gUnk_0201D810.top = S15F00_31550->listPos;
                    return 0x7E;
                }
                sub_080602A4(0x205, 0x914, 0xB, gUnk_080829F0);
                return 0x7F;
            case 0x1F9:
                if (1 & ((u8 *)ref)[2]) {
                    sub_08056ECC(ref->id);
                    gUnk_0201D810.row = 0;
                    gUnk_0201D810.top = S15F00_31550->listPos;
                    return 0x7E;
                }
                sub_080602A4(0x205, 0x914, 0xB, gUnk_08082A4C);
                return 0x7F;
            }
            break;
        case 0x7F:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7E;
        case 0x7E: {
            u16 *c = (u16 *)&gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top];

            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80DC : 0xDC, c[0], c[1], 0);
            return 0x7D;
        }
        case 0x7D:
            switch (CARD_NUMBER(ref->id)) {
            case 0x1A3:
                sub_08055F70(ref->player, &gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top], 1, 0, skip);
                return 0x64;
            case 0x1F9: {
                u16 *c = (u16 *)&gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top];

                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x807C : 0x7C, c[0], c[1], 0);
                sub_08046C20(ref->player, 1);
                break;
            }
            }
            break;
        }
    }
    return 0;
}
int sub_08031780(struct CardRef *ref)
{
    char buf[0x100];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 2;
            EFF_PHASE--;
            /* fall through */
        case 0x7F: {
            int i;

            for (i = 0; i < gUnk_020192E4[1 & ref->player].deckCount; i++) {
                u32 w = CARD_WORD(gUnk_020192E4[1 & ref->player].deck[i]);
                u32 mask = 0x7FF; /* local mask (life 3): loop.c hoists it in its second pass, after the base copy */
                u16 n = ((const u16 *)0x08622AB4)[CARD_ID(w) & mask];

                if (n == 0x1A8) {
                    sub_080753F4(buf, gUnk_08082AB4, gUnk_0822C720 + (((const u16 *)0x08623DF4)[n] << 6));
                    sub_080602A4(0x205, 0x914, 0xB, buf);
                    sub_08060308(1, 0, 0);
                    return 0x7E;
                }
            }
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
        case 0x7E:
            if (gUnk_0201AE60.flag14 != 0 && sub_0801970C(ref->player, 0x1A8) != 0) {
                if (--EFF_SIDE != 0)
                    return 0x7F;
            }
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
int sub_080318F8(struct CardRef *ref)
{
    if (!ref->skip4) {
        u16 msg;

        sub_08019980(ref->player, 3000);
        msg = ref->player ? 0x8092 : 0x92;
        sub_0801EC58(msg, ref->zone, 0, 0);
    }
    return 0;
}

int sub_08031940(struct CardRef *ref)
{
    if (((u8 *)ref)[4] & 4)
        return 0;
    if ((((u8 *)ref)[0xA] & 7) != 1)
        return 0;
    {
        int player = (u8)ref->targets[0];
        int zone = ref->targets[0] >> 8;
        int p = 1 & player;
        struct DuelZone *z = ZB(p, zone);
        u16 id = CARD_ID(CARD_WORD(z->card));

        if (id == 0)
            return 0;
        if (z->flag6_1) {
            if (CARD_TYPE(id) != 0x15)
                sub_08018544(player, zone, 1);
            return 0;
        }
        sub_0801EC58(player ? 0x807F : 0x7F, zone, 0, 0);
        sub_08019840(player, id);
        if (CARD_TYPE(id) != 0x15)
            sub_08018544(player, zone, 1);
        else
            sub_0801EC58(player ? 0x807F : 0x7F, zone, 0, 0);
    }
    return 0;
}
int sub_08031A24(struct CardRef *ref)
{
    if (1 & ((u8 *)ref)[2]) {
        if (gUnk_020192E4[0].lifePoints <= 999 && gUnk_020192E4[1].lifePoints > 2000) {
            sub_0801EC58(0x8043, 2000, 1, 0);
            sub_08019860(1 - ref->player, 1000);
        }
    } else {
        switch (EFF_PHASE) {
        case 0x80:
            sub_080602A4(0x205, 0x914, 0xB, gUnk_08082AF0);
            sub_08060308(1, 0, 0);
            return 0x7F;
        case 0x7F:
            if (gUnk_0201AE60.flag14) {
                sub_0801EC58(0x43, 2000, 1, 0);
                sub_08019860(1 - ref->player, 1000);
            }
            return 0x7E;
        }
    }
    return 0;
}
int sub_08031AEC(struct CardRef *ref)
{
    if (!ref->skip4 && !(1 & ((u8 *)ref)[2])) {
        switch (EFF_PHASE) {
        case 0x80: {
            struct DuelPlayer *pl = gUnk_020192E4;
            int p = 1 & ref->player;

            if (pl[p].handCount == 0)
                return 0;
            sub_080602A4(0x205, 0x914, 0xB, gUnk_08082B10);
            sub_08060308(1, 0, 0);
            return 0x7F;
        }
        case 0x7F:
            if (gUnk_0201AE60.flag14 == 0)
                return 0;
            sub_080602A4(0x205, 0x914, 0xB, gUnk_08082B58);
            return 0x7E;
        case 0x7E:
            if (sub_08052F38(1) != 0) {
                sub_080193B0(ref->player, gUnk_0201CFB0.unk82C, 0);
                return 0x80;
            }
            return 0x7E;
        }
    }
    return 0;
}
