#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors with the signature int f(struct CardRef *ref). They return 0
 * or a code like 0x7F, 0x80 or 0x92. See wiki/functions/code-08031bc8.md.
 */

/* struct DuelCard, struct DuelZone, struct DuelZonesPlayer and gUnk_0201930C come from duel.h. */
#define ZFLAGS(z) (((u8 *)(z))[6])
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

/* 0x020192E4 + 0x5F0: a card word in each player's state (stride 0xD64), possibly a "set" spell/trap slot (hypothesis) */
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

/* gUnk_020192E4 (struct DuelPlayer[2], with .handCount etc.) comes from duel.h. */

/* gUnk_02019AA8 (0x020192E4 + 0x7C4) is gUnk_020192E4[player].deck (struct DuelCard[80]) from duel.h. */
/* u16 at 0x0201AE60+0x14 (nonzero = flag; hypothesis: a "count/flag" of the current effect) */
struct AE60 {
    u8 unk0[0x14];
    u16 flag14;
};
extern struct AE60 gUnk_0201AE60;
/* Duel screen state at 0x0201CFB0. The word at +0x82C is passed to sub_080193B0. */
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
int sub_0802BAD0(struct CardRef *ref, u16 pos);
void sub_08018AE8(int player, int zone, int a);
void sub_08019CF0(int player, int a, int b);
void sub_08019788(int player, int id);
/* gUnk_020192E0 is struct DuelState in duel.h, which only covers up to the players (size 0x1ACC).
 * sub_080327F4 reads the flag byte at +0x1ACD, just past that range, so keep a unit-local tail view. */
struct DuelStateTail {
    u8 unk0[0x1ACD];
    u8 flags1ACD;
};
extern struct DuelStateTail gUnk_020192E0Tail asm("gUnk_020192E0");
int sub_0802BBDC(struct CardRef *ref, u16 pos);
void sub_0801919C(int player, u16 pos1, u16 pos2);
void sub_08017B04(int player, u16 a, u16 b);
void sub_08018ED8(int player, int zone, int a, int b);
void sub_08019800(int player, int id);
int sub_0800C8A8(int player, int zone);
int sub_08007590(u16 number, int a);
void sub_0801FBCC(u32 a, u32 b);
void sub_08018DC8(int player, int zone, int a);
void sub_080193D4(int player, int a, int b, int c);
void sub_080199E0(int player, int a);


int sub_08031BC8(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        u8 tp = ref->targets[0];
        int tz = ref->targets[0] >> 8;

        if (sub_0802BAD0(ref, tz << 8 | tp)) {
            sub_08030028(tp, tz);
            sub_08046CB0(ref->player, tp, tz);
        }
    }
    return 0;
}
int sub_08031C14(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        u8 tz;
        int tp;
        int p;
        int tzi; /* FAKEMATCH: an int copy of the u8 tz makes `tzi <= 4` a signed compare (bgt) */
        struct DuelZone *z;
        int id;

        tp = (u8)ref->targets[0];
        tz = ref->targets[0] >> 8;
        p = tp & 1;
        z = ZB(p, tz);
        id = CARD_ID(CARD_WORD(z->card));

        if (id) {
            switch (CARD_NUMBER(ref->id)) {
            case 0x3FF:
            case 0x4BB:
                if (CARD_NUMBER(id) == 0x4B1 && (ZFLAGS(z) & 3) == 1) {
                    sub_0801EC58(tp ? 0x807F : 0x7F, tz, 0, 0);
                    sub_080197C0(tp, CARD_ID(CARD_WORD(z->card)));
                    return 0;
                }
                break;
            }
            tzi = tz;
            {
                /* FAKEMATCH: the volatile read forces the second ref->id load (otherwise CSE'd
                 * with the switch's CARD_NUMBER read); putting the 0x7FF mask in a local makes
                 * its load precede that volatile ldrh, as in the ROM. */
                u32 m = 0x7FF;
                u16 vid = *(volatile u16 *)ref;
                int ct = (((const u32 *)0x08621DE0)[m & vid] & 0x1F00000) >> 20;

                if (ct == 0x16 && tzi <= 4 && sub_0802B28C(tp, tz) == 0)
                    return 0;
            }
            if (ref->player != tp)
                sub_0801EC58(tp ? 0x808B : 0x8B, tz, 1, 0);
            sub_08030028(tp, tz);
            sub_08046CB0(ref->player, tp, tz);
        }
    }
    return 0;
}
int sub_08031D30(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        u8 tp = ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & 1;
        struct DuelZone *z = ZB(p, tz);

        if (CARD_ID(CARD_WORD(z->card)))
            sub_08018AE8(tp, tz, 0);
    }
    return 0;
}
int sub_08031D80(struct CardRef *ref)
{
    if (!ref->skip4)
        sub_08019CF0(1 - ref->player, 5, 1);
    return 0;
}
int sub_08031DA8(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & 1;
        struct DuelZone *z = ZB(p, tz);
        int id = CARD_ID(CARD_WORD(z->card));

        if (id && tp != ref->player && !(ZFLAGS(z) & 2)) {
            sub_0801EC58(tp ? 0x807F : 0x7F, tz, 0, 0);
            sub_08019788(tp, id);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
            sub_0801EC58(tp ? 0x807F : 0x7F, tz, 0, 0);
        }
    }
    return 0;
}
int sub_08031E60(struct CardRef *ref)
{
    int found = 0;
    int i;

    for (i = 5; i <= 9; i++) {
        int p = (1 - ref->player) & 1;
        struct DuelZone *z = ZB(p, i);
        u16 id = CARD_ID(CARD_WORD(z->card));
        int p2 = (1 - ref->player) & 1;
        struct DuelZone *z2 = ZB(p2, i);

        if ((ZFLAGS(z2) & 2) && id && CARD_NUMBER(id) == 0x15B) {
            sub_08018544(1 - ref->player, i, 1);
            found = 1;
        }
    }
    if (found)
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8047 : 0x47, 1, 0, 0);
    return 0;
}
int sub_08031F30(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        int i;

        for (i = 0; i < ref->numTargets; i++) {
            int tp = (u8)ref->targets[i];
            int tz = ref->targets[i] >> 8;
            int p = 1 & tp;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 2))
                sub_08018544(tp, tz, 1);
        }
    }
    return 0;
}
int sub_08031FAC(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            struct DuelPlayer *pl = gUnk_020192E4;

            if (pl[ref->player].handCount != 0) {
                sub_080193D4(ref->player, 0, 0, 1);
                return 0x80;
            }
            return 0x7F;
        }
        case 0x7F: {
            struct DuelPlayer *pl = gUnk_020192E4;
            int opp = (1 - ref->player) & 1;

            if (pl[opp].handCount != 0) {
                sub_080193D4(1 - ref->player, 0, 1, 1);
                return 0x7F;
            }
            return 0x7E;
        }
        default:
            sub_080199E0(ref->player, 5);
            sub_080199E0(1 - ref->player, 5);
        }
    }
    return 0;
}
int sub_08032058(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i < ref->numTargets && i <= 1; i++) {
            u8 tp = ref->targets[i];
            int tz = ref->targets[i] >> 8;
            int p = tp & 1;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)))
                sub_08018AE8(tp, tz, 0);
        }
    }
    return 0;
}
int sub_080320C4(struct CardRef *ref)
{
    /* FAKEMATCH: the redundant outer & 1 puts movs #1 after the bit extraction */
    int count = gUnk_020192E4[((ref->player & 1) ^ 1) & 1].handCount;

    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 2; i++) {
            int p = (1 - ref->player) & 1;

            if (i < gUnk_020192E4[p].deckCount) {
                u16 p2 = (1 - ref->player) & 1;
                u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[p2].deck[i]));
                int id2 = id;

                sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x8061 : 0x61, 1, 1, 0);
                if (CARD_TYPE(id) == 0x16) {
                    sub_08019800(ref->player, id);
                    sub_080193D4(1 - ref->player, count, 1, 1);
                } else {
                    sub_08019840(ref->player, id2);
                    count++;
                }
            }
        }
    }
    return 0;
}


int sub_080321C8(struct CardRef *ref)
{
    __asm__("" : : : "r8");
    if (!ref->skip4 && ref->numTargets == 1) {
        int pl = ref->player;
        int rz = ref->zone;
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;

        if (pl != tp) {
            int p = pl & 1;
            struct DuelZone *z = ZB(p, rz);

            if (CARD_ID(CARD_WORD(z->card))) {
                int p2 = 1 & tp;
                struct DuelZone *z2 = ZB(p2, tz);

                if (CARD_ID(CARD_WORD(z2->card)))
                    sub_0801919C(pl, rz << 8 | pl, tp | tz << 8);
            }
        }
    }
    return 0;
}
int sub_08032258(struct CardRef *ref)
{
    if (ref->kind == 0x10) {
        int tp = (u8)ref->pos;
        int tz = ref->pos >> 8;
        int p = 1 & tp;
        struct DuelZone *z = ZB(p, tz);

        if (CARD_ID(CARD_WORD(z->card)) && !(1 & ZFLAGS(z)) && ref->player != tp)
            sub_08018ED8(tp, tz, 0, 0);
    }
    if (ref->numTargets == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        int p = tp & 1;
        struct DuelZone *z = ZB(p, tz);

        if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && ref->player == tp)
            sub_08017B04(ref->player, ref->player | (ref->zone << 8), ref->targets[0]);
        else
            sub_08018544(ref->player, ref->zone, 1);
    }
    return 0;
}
int sub_0803231C(struct CardRef *ref)
{
    int q; /* FAKEMATCH: extra copy of p feeds sub_0802BBDC and fixes scheduling */
    if (!ref->skip4) {
        int side;

        for (side = 0; side <= 1; side++) {
            int p;
            u8 pu;
            int i;

            if (side)
                p = ref->player;
            else
                p = 1 - ref->player;
            for (i = 0, pu = (q = p); i <= 4; i++) {
                q = pu;
                if (sub_0802BBDC(ref, (u8)i << 8 | q)) {
                    sub_08018544(p, i, 1);
                    sub_08046CB0(ref->player, p, i);
                }
            }
        }
    }
    return 0;
}
int sub_0800C894(int player, int zone);
static inline u32 CardAttack32390(u16 id)
{
    switch ((int)CARD_TYPE(id)) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 4000;
    }
    return ((CARD_STATS(id) >> 9) & 0x1FF) * 10;
}
#if 0 /* NONMATCHING: 192 lines; shape right, register allocation differs (opp r6 vs r4, id r4 vs r6) */
/* Two-step effect on the opponent's cards: step 0x7F walks the hand one card per call, step 0x80 the five
 * monster zones; cards with ATK over 1500 are destroyed (hypothesis from the calls). */
int sub_08032390(struct CardRef *ref)
{
    int opp = 1 - ref->player;
    int i;

    if (ref->skip4)
        return 0;
    switch (EFF_PHASE) {
    case 0x80:
        for (i = 0; i <= 4; i++) {
            int p = opp & 1;
            struct DuelZone *z = ZB(p, i);
            u16 id = CARD_ID(CARD_WORD(z->card));
            if (id != 0) {
                u32 faceDown = ((u32)ZFLAGS(z) << 30) >> 31;
                u16 msg = 8;
                if (ref->player)
                    msg = 0x8008;
                sub_0801EC58(msg, opp, i << 8, 0);
                if (faceDown == 0) {
                    msg = 0x7F;
                    if (opp)
                        msg = 0x807F;
                    sub_0801EC58(msg, i, 0, 0);
                    if (CardAttack32390(id) <= 1499) {
                        sub_08019840(opp, id);
                        msg = 0x7F;
                        if (opp)
                            msg = 0x807F;
                        sub_0801EC58(msg, i, 0, 0);
                    } else {
                        sub_08019800(opp, id);
                        sub_08030028(opp, i);
                        sub_08046CB0(ref->player, opp, i);
                    }
                } else if (sub_0800C894(opp, i) > 1499) {
                    sub_08030028(opp, i);
                    sub_08046CB0(ref->player, opp, i);
                }
            }
        }
        EFF_SIDE = 0;
        return 0x7F;
    case 0x7F:
        if (EFF_SIDE < gUnk_020192E4[opp & 1].handCount) {
            u32 id = CARD_ID(CARD_WORD(gUnk_020192E4[opp & 1].hand[EFF_SIDE]));
            u16 msg = 8;
            if (ref->player)
                msg = 0x8008;
            sub_0801EC58(msg, opp, (EFF_SIDE << 8) | 0xB, 0);
            if (CARD_TYPE(id) <= 0x14 && CardAttack32390(id) > 1499) {
                sub_08019800(opp, id);
                sub_080193D4(opp, EFF_SIDE, 1, 1);
                return 0x7F;
            }
            sub_08019840(opp, id);
            EFF_SIDE++;
            return 0x7F;
        }
        return 0x7E;
    default: {
        u16 msg = 0x69;
        if (opp)
            msg = 0x8069;
        sub_0801EC58(msg, 3, 0, 0);
        return 0;
    }
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08031BC8", sub_08032390); /* 0x08032390 size 0x2DC */
int sub_0803266C(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 5; i <= 10; i++) {
            int p = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(p, i);

            if (CARD_ID(CARD_WORD(z->card)))
                sub_08018544(1 - ref->player, i, 5);
        }
    }
    return 0;
}
#if 0 /* NONMATCHING: the logic is decoded (0x80/0x7F steps on one target; sub_0801FBCC packed
       * arg), but the control-flow branch layout and register allocation differ (ROM: tp r4,
       * tz r5, id r6, const 0x5FA r7, p spilled to r8). */
int sub_080326C4(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
        int tp = (u8)ref->targets[0];
        int tz = ref->targets[0] >> 8;
        u16 p = tp & 1;
        struct DuelZone *z = ZB(p, tz);
        int id = CARD_ID(CARD_WORD(z->card));

        switch (EFF_PHASE) {
        case 0x80:
            if (id && (ZFLAGS(z) & 3) == 1) {
                sub_08018DC8(tp, tz, 0);
                return 0x7F;
            }
            break;
        case 0x7F:
            if (sub_0800C8A8(tp, tz) > 2000) {
                sub_08019840(tp, id);
                sub_08018DC8(tp, tz, 0);
            } else {
                sub_08019800(tp, id);
                if (sub_08007590(CARD_NUMBER(id), 0) != 0 && sub_08008524(0, 0x5FA) == 0
                    && sub_08008524(1, 0x5FA) == 0)
                    sub_0801FBCC((u32)p << 31 | (tz & 0x1F) << 16 | 0x16400000 | id, 0);
                sub_08030028(tp, tz);
                sub_08046CB0(ref->player, tp, tz);
            }
            break;
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08031BC8", sub_080326C4); /* 0x080326C4 size 0x130 */
int sub_080327F4(struct CardRef *ref)
{
    if (!ref->skip4) {
        u16 msg = (1 & ((u8 *)ref)[2]) ? 0x801C : 0x1C;
        int f = 1 & ~(gUnk_020192E0Tail.flags1ACD >> 5);

        sub_0801EC58(msg, f, 0, 0);
    }
    return 0;
}
#if 0 /* NONMATCHING: 487 lines; case set and body order match the ROM's switch, register allocation differs from the first target-zone block on */
/* Sweep effect keyed by the triggering card number: face-up spells (type 0x15) on the player's side are
 * re-activated through sub_08018DC8/sub_08019840, other cards are removed with sub_08018544 (hypothesis). */
int sub_0803283C(struct CardRef *ref, struct CardRef *src)
{
    int z;

    if (ref->skip4 || src == NULL || src->player == ref->player)
        return 0;
    switch (CARD_NUMBER(src->id)) {
    case 0x53:
    case 0xDF:
    case 0x3EC:
    case 0x437:
    case 0x46E:
    case 0x46F: {
        u8 tp = src->targets[0];
        u32 tz = src->targets[0] >> 8;
        struct DuelZone *zn = ZB(tp & 1, tz);
        u32 id = CARD_ID(CARD_WORD(zn->card));
        if (id == 0)
            return 0;
        if (tp == ref->player && tz == ref->zone)
            return 0;
        if (CARD_TYPE(id) != 0x15)
            return 0;
        if (!(ZFLAGS(zn) & 2)) {
            sub_08018DC8(tp, tz, 0);
            sub_08019840(ref->player, id);
            sub_08018DC8(tp, tz, 0);
        }
        if (CARD_TYPE(src->id) <= 0x14)
            return 0;
        break;
    }
    case 0x29F:
    case 0x426:
        for (z = 5; z <= 10; z++) {
            struct DuelZone *zn = ZB(ref->player & 1, z);
            u32 id = CARD_ID(CARD_WORD(zn->card));
            if (id != 0) {
                if (CARD_TYPE(id) == 0x15) {
                    if (!(ZFLAGS(zn) & 2)) {
                        sub_08018DC8(ref->player, z, 0);
                        sub_08019840(ref->player, id);
                        sub_08018DC8(ref->player, z, 0);
                    }
                } else {
                    sub_08018544(ref->player, z, 5);
                }
            }
        }
        if (CARD_TYPE(src->id) <= 0x14)
            return 0;
        break;
    case 0x425:
        for (z = 5; z <= 10; z++) {
            struct DuelZone *zn = ZB(ref->player & 1, z);
            u32 id = CARD_ID(CARD_WORD(zn->card));
            if (id != 0) {
                if (CARD_TYPE(id) == 0x15) {
                    if (!(ZFLAGS(zn) & 2)) {
                        sub_08018DC8(ref->player, z, 0);
                        sub_08019840(ref->player, id);
                        sub_08018DC8(ref->player, z, 0);
                    }
                } else {
                    sub_08018544(ref->player, z, 1);
                }
            }
        }
        for (z = 5; z <= 10; z++) {
            if (CARD_WORD(ZB((1 - ref->player) & 1, z)->card) << 20)
                sub_08018544(ref->player, z, 1);
        }
        break;
    case 0x42B:
        for (z = 0; z <= 10; z++) {
            struct DuelZone *zn = ZB(ref->player & 1, z);
            u32 id = CARD_ID(CARD_WORD(zn->card));
            if (id != 0) {
                if (CARD_TYPE(id) == 0x15) {
                    if (!(ZFLAGS(zn) & 2)) {
                        sub_08018DC8(ref->player, z, 0);
                        sub_08019840(ref->player, id);
                        sub_08018DC8(ref->player, z, 0);
                    }
                } else {
                    sub_08018544(ref->player, z, 1);
                }
            }
        }
        for (z = 0; z <= 10; z++) {
            if (CARD_WORD(ZB((1 - ref->player) & 1, z)->card) << 20)
                sub_08018544(ref->player, z, 1);
        }
        break;
    default:
        return 0;
    }
    sub_0801EC58(ref->player ? 0x80B0 : 0xB0, 1, 0, 0);
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08031BC8", sub_0803283C); /* 0x0803283C size 0x474 */
