#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors with the signature int f(struct CardRef *ref). They return 0
 * or a code like 0x7F, 0x80 or 0x92. See wiki/functions/code-08031bc8.md.
 *
 * Shared duel structs/globals (struct DuelCard/DuelZone/DuelZonesPlayer, the gUnk_020192E0 /
 * gUnk_020192E4 / gUnk_0201930C externs) come from include/duel.h. ZFLAGS is a raw byte view
 * of the zone flags byte at +0x06 (canonical struct DuelZone.flag6_0/flag6_1/counter6).
 */

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
int sub_08056ECC(u16 id);
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


int sub_08008860(int player);
int sub_08008AF8(int player, int a);
void sub_08022824(int player);
int sub_0802E4E0(struct CardRef *ref, int a, int b);
int sub_08022678(int player, int a, int b, int c);
/* Local view of gUnk_020192E0 (canonical struct DuelState in duel.h covers only +0x0000..+0x1ACC:
 * the u32 and the two players). This unit reads fields past that (+0x1B12 bit 1, +0x1B64 u16), so it
 * keeps a unit-local view; see wiki/functions/code-08035198.md. */
struct DuelGlobals {
    u8 unk0[0x1B12];
    u8 b0 : 1;      /* +0x1B12 bit 1: a player index (hypothesis) */
    u8 b1 : 1;
    u8 rest : 6;
    u8 unk1B13[0x1B64 - 0x1B13];
    u16 w1B64;
};
#define DG ((struct DuelGlobals *)&gUnk_020192E0)
void sub_08022784(int player, int a, int b);
void sub_0802272C(int player, int a, int b, int c);
int sub_0802C080(struct CardRef *ref, u16 pos);
int sub_0800CCCC(int p1, int z1, int p2, int z2);
int sub_0800CD68(int player, int zone);
void sub_08017C0C(u16 pos);
void *sub_08075294(void *dst, const void *src, u32 n);
int sub_08047058(u16 id);
/* Effect-resolution state at 0x02017A40, larger view. */
struct EffState {
    u8 unk0[0x3E0];
    u8 phase;       /* +0x3E0 */
    u8 side;        /* +0x3E1 */
    u8 unk3E2[0x4E4 - 0x3E2];
    struct CardRef cur;     /* +0x4E4: working copy of the CardRef being executed */
    u8 unk4F4[4];   /* +0x4F4: reserved tail of the 0x14-byte working copy */
    u8 (*fn)(struct CardRef *, int);    /* +0x4F8: executor picked from gUnk_0819A9D4 */
    u8 unk4FC[0x542 - 0x4FC];
    u16 w542;       /* +0x542: saved value (a stat) for the 0x78 step of sub_080358AC */
};
#define ES ((struct EffState *)gUnk_02017A40)
struct EffEntry {       /* 0x18 bytes each at 0x0819A9D4 (hypothesis: effect table) */
    u32 unk0;
    u8 (*fn)(struct CardRef *, int);
    u32 unk8[4];
};
extern struct EffEntry gUnk_0819A9D4[];
int sub_08008A44(int player);
void sub_08019078(int player, u16 a, u16 b);
int sub_0802E42C(struct CardRef *ref, int a, int b);
void sub_08077AEC(int a);
extern const u8 gUnk_08082D64[];
extern const u8 gUnk_08082DB4[];
/* Duel screen state at 0x0201CFB0, larger view (see struct DuelScreen82C). */
struct DuelScreenView {
    u8 unk0[0x824];
    u16 w824;
    u8 unk826[2];
    u8 b828;
    u8 unk829[3];
    u32 w82C;
};
#define DSV ((struct DuelScreenView *)&gUnk_0201CFB0)
/* 0x020192E4 + 0x684: a list of card words per player (stride 0xD64). */
struct PlayerWords {
    u32 w[0xD64 / 4];
};
extern struct PlayerWords gUnk_02019968[2];
int sub_0802E6F8(struct CardRef *ref, int a, int b);
int sub_0800C894(int player, int zone);
void sub_08075434(char *dst, const void *fmt, ...);
extern const u8 gUnk_08082DE8[];
#if 0 /* NONMATCHING: the ROM hoists `mov r4,#1` (const for the later `1 & ref byte`) above the
       * base loads and keeps the EFF base in r8, so register allocation differs. */
int sub_08035198(struct CardRef *ref, int arg)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 3;
            EFF_PHASE--;
        case 0x7F:
            if (EFF_SIDE == 0)
                return 0;
            if (sub_0802E42C(ref, arg, 0) == 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08082D64);
            sub_08060308(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gUnk_0201AE60.flag14 == 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08082DB4);
            return 0x7D;
        case 0x7D:
            if (sub_08052F38(1) != 0) {
                int p = ref->player;
                struct DuelScreenView *ds = DSV;
                s8 w = gUnk_02019968[p].w[ds->w82C];

                if (CARD_TYPE(CARD_ID11(w)) <= 0x14) {
                    sub_08077AEC(1);
                    sub_080193D4(ref->player, ds->w82C, 1, 1);
                    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, ds->w824, (u8)ds->w82C << 8 | ds->b828, 0);
                    EFF_SIDE--;
                    return 0x7F;
                }
                sub_08077AEC(3);
            }
            return 0x7D;
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08035198", sub_08035198); /* 0x08035198 size 0x17C */
int sub_08035314(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int p;
            int j;

            if (i)
                p = ref->player;
            else
                p = 1 - ref->player;
            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(p & 1, j);

                if (((u8 *)z)[7] & 0x40) {
                    sub_08030028(p, j);
                    sub_08046CB0(ref->player, p, j);
                }
            }
        }
    }
    return 0;
}
int sub_080353A4(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            int tp = (u8)ref->targets[0];
            int tz = ref->targets[0] >> 8;
            int r = sub_08008A44(ref->player);

            if (tp != ref->player) {
                int p = tp & n;
                struct DuelZone *z = ZB(p, tz);
                int id = CARD_ID(CARD_WORD(z->card));

                if (id && r != -1) {
                    if (CARD_NUMBER(id) == 0x4B1 && (ZFLAGS(z) & 3) == 1) {
                        sub_0801EC58(tp ? 0x807F : 0x7F, tz, 0, 0);
                        sub_080197C0(tp, CARD_ID(CARD_WORD(z->card)));
                        return 0;
                    } else {
                        sub_08019078(ref->player, ref->targets[0], ref->player | (u8)r << 8);
                        sub_08017AB4(ref->player, ref->id, ref->player | (u8)r << 8, 3);
                    }
                }
            }
        }
    }
    return 0;
}
int sub_08035480(struct CardRef *ref, u16 *idp)
{
    if (!ref->skip4) {
        if (idp) {
            int type = CARD_TYPE(*idp);

            switch (type) {
            case 0x15:
            case 0x16:
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80B0 : 0xB0, 1, 0, 0);
            }
        } else if (EFF_PHASE == 0x80) {
            if (sub_0802E4E0(ref, 0, 0)) {
                sub_0801EC58((u8)ref->pos ? 0x8091 : 0x91, ref->pos >> 8, 7, 0);
                return 0x7F;
            }
        } else {
            u8 tp = ref->pos;
            int tz = ref->pos >> 8;

            sub_08018544(tp, tz, 1);
        }
    }
    return 0;
}
int sub_0803552C(struct CardRef *ref)
{
    if (!ref->skip4) {
        if (EFF_PHASE == 0x80) {
            sub_0801EC58((u8)ref->pos ? 0x8091 : 0x91, ref->pos >> 8, 7, 0);
            return 0x7F;
        } else {
            u8 tp = ref->pos;
            int tz = ref->pos >> 8;

            sub_08018544(tp, tz, 1);
        }
    }
    return 0;
}
int sub_08035580(struct CardRef *ref)
{
    int n = sub_08008860(1 - ref->player);

    if (!ref->skip4 && n > 0)
        sub_08019860(1 - ref->player, n * 500);
    return 0;
}
int sub_080355C0(struct CardRef *ref)
{
    if (sub_0801970C(ref->player, 0x3EB) || sub_0801970C(ref->player, 0x40A))
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
    return 0;
}
int sub_08035614(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            u8 tp = ref->targets[0];
            int tz = ref->targets[0] >> 8;
            int p = tp & n;
            struct DuelZone *z = ZB(p, tz);

            if (!(ZFLAGS(z) & n))
                sub_08018ED8(tp, tz, 1, 0);
        }
    }
    return 0;
}
int sub_08035668(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int p;
            int j;

            if (i)
                p = ref->player;
            else
                p = 1 - ref->player;
            for (j = 0; j <= 10; j++) {
                struct DuelZone *z = ZB(p & 1, j);

                if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 2)) {
                    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, p, (u8)j << 8, 0);
                    sub_0801EC58(p ? 0x807F : 0x7F, j, 0, 0);
                    sub_08019788(ref->player, CARD_ID(CARD_WORD(z->card)));
                    sub_0801EC58(p ? 0x807F : 0x7F, j, 0, 0);
                }
            }
        }
    }
    return 0;
}
int sub_08035764(struct CardRef *ref)
{
    u8 tp = ref->pos;
    int tz = ref->pos >> 8;

    sub_08018AE8(tp, tz, 0);
    return 0;
}
int sub_0803577C(struct CardRef *ref, int arg)
{
    u32 *card = &gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (sub_0802E6F8(ref, arg, 0)) {
                sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
                return 0x7F;
            }
            break;
        case 0x7F:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 0x7E;
        case 0x7E:
            sub_08056094(ref->player, &gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top], 1, 0);
            return 0x7D;
        case 0x7D:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
int sub_08035868(struct CardRef *ref)
{
    if (DG->b1 != ref->player)
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8036 : 0x36, 1, 1, 0);
    return 0;
}
int sub_080358AC(struct CardRef *ref)
{
    u8 tp = ref->pos;
    int tz = ref->pos >> 8;

    if (!ref->skip4) {
        switch (CARD_NUMBER(ref->id)) {
        case 0x420: {
            int j;

            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(tp & 1, j);

                if (CARD_ID(CARD_WORD(z->card)) && !(ZFLAGS(z) & 1)) {
                    sub_08030028(tp, j);
                    sub_08046CB0(ref->player, tp, j);
                }
            }
            break;
        }
        case 0x44A: {
            int p = 1 & tp;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2))
                sub_08019980(ref->player, sub_0800C894(tp, tz));
            break;
        }
        case 0x4BE: {
            int p = 1 & tp;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2)) {
                sub_0801EC58(tp ? 0x803B : 0x3B, tz, 0, 0);
                sub_08019860(tp, sub_0800C894(tp, tz));
            }
            break;
        }
        case 0x587: {
            int p = tp & 1;
            struct DuelZone *z = ZB(p, tz);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2))
                sub_08017AB4(ref->player, ref->id, ref->pos, 3);
            break;
        }
        default:
            if (CARD_NUMBER(ref->id) == 0x2AD) {
                switch (EFF_PHASE) {
                case 0x80: {
                    int best = -1;
                    int bestZone = -1;
                    int count = 0;
                    int j;
                    char buf[0x80];

                    for (j = 0; j <= 4; j++) {
                        struct DuelZone *z = ZB(1 & tp, j);

                        if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 3) == 2) {
                            int v = sub_0800C894(tp, j);

                            if (v == best)
                                count++;
                            if (v > best) {
                                best = v;
                                bestZone = j;
                                count = 1;
                            }
                        }
                    }
                    if (bestZone < 0)
                        break;
                    if (count == 1 || (1 & ((u8 *)ref)[2])) {
                        sub_08030028(tp, bestZone);
                        sub_08046CB0(ref->player, tp, bestZone);
                    } else {
                        sub_08075434(buf, gUnk_08082DE8, best);
                        sub_080602A4(0x204, 0x817, 0xB, buf);
                        ES->w542 = best;
                        EFF_PHASE = 0x78;
                    ret78:
                        return 0x78;
                    }
                    break;
                }
                case 0x78:
                    if (sub_08052F38(0xF0 << (tp << 4)) == 0)
                        goto ret78;
                    if (sub_0800C894(tp, DSV->w82C) != ES->w542)
                        goto fail78;
                    sub_08030028(tp, DSV->w82C);
                    sub_08046CB0(ref->player, tp, DSV->w82C);
                    break;
                fail78:
                    sub_08077AEC(3);
                    goto ret78;
                default:
                    return 0;
                }
            }
        }
    }
    return 0;
}
int sub_08035BBC(struct CardRef *ref)
{
    if (!ref->skip4 && EFF_PHASE == 0x80) {
        if (sub_08008AF8(1 - ref->player, -1)) {
            sub_08022824(1 - ref->player);
            return 0x7F;
        }
    }
    return 0;
}
int sub_08035C0C(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            EFF_SIDE = 1 - ref->player;
            EFF_PHASE--;
        case 0x7F: {
            int j;

            for (j = 5; j <= 10; j++) {
                int side = EFF_SIDE;
                struct DuelZone *z = &gUnk_0201930C[(u8)side & 1].zones[j];

                if (CARD_ID(CARD_WORD(z->card))) {
                    sub_08018544(side, j, j);
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
int sub_08035CB0(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int j;

            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(i & 1, j);

                if (CARD_ID(CARD_WORD(z->card)))
                    sub_08018ED8(i, j, 1, 1);
            }
        }
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8048 : 0x48, 1, 0, 0);
    }
    return 0;
}
int sub_08035D44(struct CardRef *ref)
{
    if (!ref->skip4) {
        sub_080199E0(ref->player, 1);
        sub_08019980(1 - ref->player, 1000);
    }
    return 0;
}
int sub_08035D78(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int p;
            int j;

            if (i)
                p = ref->player;
            else
                p = 1 - ref->player;
            for (j = 0; j <= 10; j++) {
                struct DuelZone *z = ZB(p & 1, j);

                if (CARD_ID(CARD_WORD(z->card)) && (j > 4 || sub_0802B28C(p, j))) {
                    sub_08030028(p, j);
                    sub_08046CB0(ref->player, p, j);
                }
            }
        }
    }
    return 0;
}
int sub_08035E0C(struct CardRef *ref)
{
    int p = ref->player;
    int zone = ref->zone;

    if (!(8 & ((u8 *)ref)[4])) {
        struct DuelZone *z = ZB(p, zone);
        int id = CARD_ID(CARD_WORD(z->card));

        int n;

        if (id && (ZFLAGS(z) & 2) && CARD_NUMBER(id) == 0x42C) {
            n = 7 & ((u8 *)ref)[0xA];
            if (n == 1) {
                int tp = (u8)ref->targets[0];
                int tz = ref->targets[0] >> 8;
                int r = sub_08008A44(ref->player);

                if (tp != ref->player) {
                    int p2 = tp & n;
                    struct DuelZone *z2 = ZB(p2, tz);

                    if (CARD_ID(CARD_WORD(z2->card)) && (ZFLAGS(z2) & 2)) {
                        sub_08017B04(ref->player, ref->player | ref->zone << 8, tp | tz << 8);
                        if (!ref->skip4 && r != -1)
                            sub_08019078(ref->player, ref->targets[0], ref->player | (u8)r << 8);
                    }
                }
            }
        }
    }
    return 0;
}
int sub_08035F18(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            sub_08022678(ref->player, 6, 0, 0);
            return 0x7F;
        case 0x7F:
            sub_080193D4(1 - ref->player, DG->w1B64, 1, 1);
            return 0x64;
        }
    }
    return 0;
}
int sub_08035F80(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            sub_08022784(1 - ref->player, 1, 1);
            return 0x7F;
        case 0x7F:
            sub_0802272C(1 - ref->player, 1, 0, 1);
            return 0x7E;
        }
    }
    return 0;
}
int sub_08035FDC(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            u8 tp = ref->targets[0];
            int tz = ref->targets[0] >> 8;
            int p = tp & n;
            struct DuelZone *z = ZB(p, tz);

            if (ZFLAGS(z) & 2)
                sub_08018DC8(tp, tz, 0);
        }
    }
    return 0;
}
/* Executor view of ES->fn: the callbacks return a full int; the caller keeps only the low byte. */
typedef int (*EffFn36030)(struct CardRef *, int);
/* Dispatcher: at phase 0x80 copies ref into ES->cur, takes id/player from card and picks the
 * card's executor from gUnk_0819A9D4; then runs it and stores its step in EFF_PHASE. A null
 * executor or a zero step prints message 0xB0. */
int sub_08036030(struct CardRef *ref, struct CardRef *card)
{
    u8 *p;

    if (EFF_PHASE == 0x80) {
        sub_08075294(&ES->cur, ref, 0x14);
        ES->cur.id = card->id;
        ES->cur.player = card->player;
        *(EffFn36030 *)&ES->fn = (EffFn36030)gUnk_0819A9D4[sub_08047058(card->id)].fn;
        if (*(EffFn36030 *)&ES->fn == 0) {
            /* FAKEMATCH: dead store; it keeps the phase pointer from being shared with the
             * setup block, so the base is reloaded from the pool and ref stays in r7. */
            p = 0;
            goto fail;
        }
    }
    {
        struct EffState *e = ES;
        int step = (*(EffFn36030 *)&e->fn)(&e->cur, 0);

        p = &e->phase;
        *p = step;
        if (*p != 0)
            goto success;
    }
fail:
    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80B0 : 0xB0, 1, 0, 0);
    return 0;
success:
    return *p;
}
int sub_080360E8(struct CardRef *ref)
{
    u8 tp0 = ref->targets[0];
    int tz0 = ref->targets[0] >> 8;
    u8 tp1 = ref->targets[1];
    int tz1 = ref->targets[1] >> 8;

    if (!ref->skip4 && ref->numTargets == 2) {
        if (sub_0802C080(ref, ref->targets[0]) && sub_0800CCCC(tp0, tz0, tp1, tz1)) {
            if (sub_0800CD68(tp0, tz0) != ref->targets[1])
                sub_08017C0C(ref->targets[0]);
        }
    }
    return 0;
}
int sub_08036150(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            sub_08022678(ref->player, 6, 0, 0);
            return 0x7F;
        case 0x7F:
            sub_080193B0(1 - ref->player, DG->w1B64, 1);
            sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
