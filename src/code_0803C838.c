#include "global.h"

/*
 * Duel card-effect executors. Each is `int f(struct CardRef *ref)` and returns 0
 * or a step state (0x7E or 0x7F). See wiki/functions/code-0803c838.md.
 */

struct DuelCard {
    u32 id : 12;
    u32 unk12 : 20;
};

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flag6_0 : 1;
    u8 flag6_1 : 1;
    u8 counter6 : 4;
    u8 unk6_6 : 2;
    u8 unk7[0x94 - 7];
};
#define ZFLAGS(z) (((u8 *)(z))[6])
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
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
extern u8 gUnk_020192E0[];   /* duel global state, byte view */

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
/* 0x020192E4: per-player duel state, only the parts used here (stride 0xD64). */
struct PlayerState {
    u16 lifePoints;
    u8 handCount;       /* +2 */
    u8 deckCount;       /* +3 */
    u8 pad4[3];
    u8 unk7_0 : 3;
    u8 flag7_3 : 1;     /* +7 bit 3 */
    u8 unk7_4 : 4;
    u8 unk8;
    u8 pad9[3];
    u8 flagsC;          /* +0xC */
    u8 padD[0xD64 - 0xD];
};
extern struct PlayerState gUnk_020192E4[2];
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
int sub_0802C674(struct CardRef *ref, u16 pos);
extern char gUnk_08083AD0[];
#define EFF_W542 (*(u16 *)&gUnk_02017A40[0x542])
int sub_0802FFE4(struct CardRef *ref, int a, int b);
extern char gUnk_08083B24[];
extern const u16 gUnk_0819A7C8[];
extern const u16 gUnk_0819A970[];
u16 sub_0803CB28(u16 n);
u16 sub_0803CE90(int player, u16 a, u16 b, u16 c);
u16 sub_0803D048(int player, u16 pos);
int sub_08008860(int player);
int sub_0803CB60(int a, int b, int c);
int sub_0803CC18(u16 a, u16 b, u16 c, u16 d);
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



struct EffState838 {
    u8 unk0[0x3E0];
    u8 phase;       /* +0x3E0 */
    u8 side;        /* +0x3E1 */
    u8 unk3E2[0x542 - 0x3E2];
    u16 w542;       /* +0x542 */
};
#define ES838 ((struct EffState838 *)gUnk_02017A40)
union ViewCard838 {
    struct {
        u32 lo : 20;
        u32 flag20 : 1;
        u32 hi : 11;
    } b;
    u16 h[2];
};
struct ListView838 {
    u8 unk0[0xC];
    union ViewCard838 cards[0x80];    /* +0x0C: card words */
};
#define LV838 ((struct ListView838 *)&gUnk_0201D810)
int sub_0803C838(struct CardRef *ref)
{
    int tp = (u8)ref->targets[0];
    int tz = ref->targets[0] >> 8;

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int n = 7 & ((u8 *)ref)[0xA];
            int a, m;

            if (n != 1)
                return 0;
            if (sub_0802C674(ref, ref->targets[0]) == 0)
                return 0;
            sub_08018C3C(tp, tz);
            ES838->w542 = CARD_ID(CARD_WORD(ZBP(tp & n, tz)->card));
            a = sub_08008A1C(ref->player);
            m = sub_08044224(ref->player, 0x60A, ES838->w542);
            if (m == 0)
                return 0;
            if (a < m)
                return 0;
            if (n & ((u8 *)ref)[2])
                return 0;
            return 0x7F;
        }
        case 0x7F:
            sub_080602A4(0x206, 0x613, 0xB, gUnk_08083AD0);
            sub_08060308(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gUnk_0201AE60.flag14 == 0)
                return 0;
            EFF_SIDE = sub_08044224(ref->player, 0x60A, ES838->w542);
            return 0x7D;
        case 0x7D:
            if (EFF_SIDE == 0)
                return 0;
            EFF_SIDE--;
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D3 : 0xD3, ((union ViewCard838 *)gUnk_0201D81C)[EFF_SIDE].h[0], ((union ViewCard838 *)gUnk_0201D81C)[EFF_SIDE].h[1], 0);
            return 0x7C;
        case 0x7C:
            LV838->cards[EFF_SIDE].b.flag20 = 0;
            sub_08056094(ref->player, (u32 *)&LV838->cards[EFF_SIDE], 1, 0x20);
            return 0x7D;
        }
    }
    return 0;
}
int sub_0803C9FC(struct CardRef *ref, int arg)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (sub_0802FFE4(ref, arg, 0) == 0)
                return 0;
            EFF_SIDE = 3;
            EFF_PHASE--;
        case 0x7F:
            if (sub_08044224(ref->player, 0x60D, 0) == 0)
                return 0;
            sub_080602A4(0x206, 0x613, 0xB, gUnk_08083B24);
            return 0x7E;
        case 0x7E:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7D;
        case 0x7D: {
            u16 *cw = (u16 *)&gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row];

            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80DE : 0xDE, cw[0], cw[1], 0);
            EFF_SIDE--;
            if (EFF_SIDE != 0)
                return 0x7F;
            return 0xA;
        }
        }
    }
    return 0;
}
int sub_0803CB04(struct CardRef *ref)
{
    u8 *base = (u8 *)gUnk_020192E4;
    u8 *ps = base + ref->player * 0xD64;

    ps[0xC] |= 0x20;
    return 0;
}
u16 sub_0803CB28(u16 n)
{
    switch (n) {
    case 0x6C:
    case 0x101:
    case 0x10C:
    case 0x281:
        return 1;
    default:
        return 0;
    }
}
int sub_0803CB60(int a, int b, int c)
{
    const u16 *e = gUnk_0819A7C8;
    u16 x = CARD_NUMBER(a);
    u16 y = CARD_NUMBER(b);
    u16 z = CARD_NUMBER(c);

    if (sub_0803CB28(y) != 0 && sub_0803CB28(z) != 0)
        return 0;
again:
    if (*(u32 *)e == 0x03E703E7 && e[2] == 0x3E7)
        return 0;
    if (e[0] == x) {
        if (y == e[1] && z == e[2])
            return 1;
        if (y == e[2] && z == e[1])
            return 1;
        if (sub_0803CB28(y) != 0 && (z == e[1] || z == e[2]))
            return 1;
        if (sub_0803CB28(z) != 0 && (y == e[1] || y == e[2]))
            return 1;
    }
    e += 4;
    goto again;
}
int sub_0803CC18(u16 a, u16 b, u16 c, u16 d)
{
    const u16 *e = gUnk_0819A970;
    u16 x = CARD_NUMBER(a);
    u16 y = CARD_NUMBER(b);
    u16 z = CARD_NUMBER(c);
    u16 w = CARD_NUMBER(d);

    if (sub_0803CB28(y) != 0 && sub_0803CB28(z) != 0)
        return 0;
    if (sub_0803CB28(y) != 0 && sub_0803CB28(w) != 0)
        return 0;
    if (sub_0803CB28(z) != 0 && sub_0803CB28(w) != 0)
        return 0;
again:
    if (*(u32 *)e == 0x03E703E7 && e[2] == 0x3E7)
        return 0;
    if (e[0] == x) {
        if (y == e[1]) {
            if (z == e[2] && w == e[3])
                return 1;
            if (z == e[3] && w == e[2])
                return 1;
            if (sub_0803CB28(z) != 0 && (w == e[2] || w == e[3]))
                return 1;
            if (sub_0803CB28(w) != 0 && (z == e[2] || z == e[3]))
                return 1;
            return 0;
        }
        if (y == e[2]) {
            if (z == e[1] && w == e[3])
                return 1;
            if (z == e[3] && w == e[1])
                return 1;
            if (sub_0803CB28(z) != 0 && (w == e[1] || w == e[3]))
                return 1;
            if (sub_0803CB28(w) != 0 && (z == e[1] || z == e[3]))
                return 1;
            return 0;
        }
        if (y == e[3]) {
            if (z == e[1] && w == e[2])
                return 1;
            if (z == e[2] && w == e[1])
                return 1;
            if (sub_0803CB28(z) != 0 && (w == e[1] || w == e[2]))
                return 1;
            if (sub_0803CB28(w) != 0 && (z == e[1] || z == e[2]))
                return 1;
            return 0;
        }
        if (sub_0803CB28(y) != 0) {
            if (z == e[1] && (w == e[2] || w == e[3]))
                return 1;
            if (z == e[2] && w == e[3])
                return 1;
            return 0;
        }
    }
    e += 4;
    goto again;
}
int sub_0803CDEC(int a, int b)
{
    u16 x = CARD_NUMBER(a);
    u16 y = CARD_NUMBER(b);
    u32 i;
    const u16 *e;

    if (x > 0x7CF)
        x -= 0x7D0;
    if (y > 0x7CF)
        y -= 0x7D0;
    for (i = 0, e = gUnk_0819A7C8; i <= 0x34; e += 4, i++) {
        if (x == e[0] && (e[1] == y || e[2] == y))
            return 1;
    }
    for (i = 0, e = gUnk_0819A970; i <= 3; e += 4, i++) {
        if (x == e[0] && (e[1] == y || e[2] == y || e[3] == y))
            return 1;
    }
    if (sub_0803CB28(y) != 0)
        return 1;
    return 0;
}
/* Hand card word; the player term written first makes agbcc compute i * 4 first, as the ROM does. */
#define HAND_WORD_CE90(p, i) (*(u32 *)((p) * 0xD64 + (i) * 4 + (u32)gUnk_02019968))
u16 sub_0803CE90(int player, u16 a, u16 b, u16 c)
{
    int i;
    u16 id; /* one id for all four scans: per-loop locals put it in r0/r1 instead of r2 */

    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = ZB(player & 1, i);
        id = CARD_ID(CARD_WORD(z->card));

        if (id != 0) {
            if (CARD_NUMBER(id) == a || CARD_NUMBER(id) == a + 0x7D0) {
                u16 pos = i + 0x4000;

                if (b != pos && c != pos)
                    return pos;
            }
        }
    }
    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        id = CARD_ID(HAND_WORD_CE90(player & 1, i));

        if (CARD_NUMBER(id) == a || CARD_NUMBER(id) == a + 0x7D0) {
            u16 pos = i + (int)0xFFFF8000;

            if (b != pos && c != pos)
                return pos;
        }
    }
    for (i = 0; i <= 4; i++) {
        struct DuelZone *z = ZB(player & 1, i);
        id = CARD_ID(CARD_WORD(z->card));

        if (id != 0 && sub_0803CB28(CARD_NUMBER(id)) != 0) {
            u16 pos = i + 0x4000;

            if (b != pos && c != pos)
                return pos;
        }
    }
    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        id = CARD_ID(HAND_WORD_CE90(player & 1, i));

        if (id != 0 && sub_0803CB28(CARD_NUMBER(id)) != 0) {
            u16 pos = i + (int)0xFFFF8000;

            if (b != pos && c != pos)
                return pos;
        }
    }
    return 0xFFFF;
}
u16 sub_0803D048(int player, u16 pos)
{
    if (pos & 0x8000) {
        int pa = 1 & player;
        u16 off = (pos & 0xFFF) * 4 + pa * 0xD64;
        return CARD_ID(*(u32 *)(off + (u32)gUnk_02019968));
    } else if (pos & 0x4000) {
        int pa = 1 & player;
        u32 off = (pos & 0xFFF) * 0x94 + pa * 0xD64;
        return CARD_ID(*(u32 *)(off + (u32)gUnk_0201930C));
    }
    return 0;
}

int sub_0803D0B8(u16 a, u16 b, u16 c, u16 d)
{
    int r;

    if (d != 0)
        r = sub_0803CC18(a, b, c, d);
    else
        r = sub_0803CB60(a, b, c);
    if ((u16)r == 0)
        return 0;
    return 1;
}
static inline int FusKind_D0E8(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 0x776:
        return 3;
    case 0x777:
    case 0x778:
        return 1;
    }
    switch ((int)CARD_TYPE(id)) {
    case 0x16:
        return 7;
    case 0x15:
        return 8;
    case 0x17:
        return 9;
    }
    return (CARD_STATS(id) & 0xC0000) >> 18;
}
#define FUS_WILD(p) sub_0803CB28(CARD_NUMBER(sub_0803D048(player, (p))))
int sub_0803D0E8(int player, u16 id, u16 *out)
{
    const u16 *e;
    u16 num;
    u32 i;

    if (CARD_TYPE(id) > 0x14)
        return 0;
    if (FusKind_D0E8(id) != 2)
        return 0;
    num = CARD_NUMBER(id);
    if (num > 0x7CF)
        num -= 0x7D0;

    for (i = 0, e = gUnk_0819A7C8; i <= 0x34; e += 4, i++) {
        if (num == e[0]) {
            u16 a = e[1];
            u16 b = e[2];
            out[1] = sub_0803CE90(player, a, 0xFFFF, 0xFFFF);
            out[0] = sub_0803CE90(player, b, out[1], 0xFFFF);
            if (out[0] == 0xFFFF)
                return 0;
            if (out[1] == 0xFFFF)
                return 0;
            if (FUS_WILD(out[0]) != 0 && FUS_WILD(out[1]) != 0)
                return 0;
            if ((out[0] & 0x8000) && (out[1] & 0x8000)) {
                if (sub_08008860(player) == 5)
                    return 0;
            }
            return 1;
        }
    }
    for (i = 0, e = gUnk_0819A970; i <= 3; e += 4, i++) {
        if (num == e[0]) {
            u16 a = e[1];
            u16 b = e[2];
            u16 c = e[3];
            out[2] = sub_0803CE90(player, a, 0xFFFF, 0xFFFF);
            out[1] = sub_0803CE90(player, b, out[2], 0xFFFF);
            out[0] = sub_0803CE90(player, c, out[2], out[1]);
            if (out[0] == 0xFFFF)
                return 0;
            if (out[1] == 0xFFFF)
                return 0;
            if (out[2] == 0xFFFF)
                return 0;
            if (FUS_WILD(out[0]) != 0 && FUS_WILD(out[1]) != 0)
                return 0;
            if (FUS_WILD(out[0]) != 0 && FUS_WILD(out[2]) != 0)
                return 0;
            if (FUS_WILD(out[1]) != 0 && FUS_WILD(out[2]) != 0)
                return 0;
            if ((out[0] & 0x8000) && (out[1] & 0x8000) && (out[2] & 0x8000)) {
                if (sub_08008860(player) == 5)
                    return 0;
            }
            return 1;
        }
    }
    return 0;
}
#undef FUS_WILD
struct FusList_3D3D0 {
    u8 pad[0x502];
    u32 cnt : 2;
    u32 rest : 6;
    u8 pad2;
    u16 list[3];
};
#define FL_3D3D0 ((struct FusList_3D3D0 *)gUnk_02017A40)

int sub_0803D3D0(u16 num)
{
    int i;

    for (i = 0; i < FL_3D3D0->cnt; i++) {
        if (FL_3D3D0->list[i] != 0) {
            int y = CARD_NUMBER(FL_3D3D0->list[i]);
            int x = CARD_NUMBER(num);

            if (x > 0x7CF)
                x -= 0x7D0;
            if (y > 0x7CF)
                y -= 0x7D0;
            if (y == x)
                return 1;
        }
    }
    return 0;
}
struct FusList_3D460 {
    u8 pad[0x502];
    u8 cnt : 2;
    u8 rest : 6;
    u8 pad2;
    u16 list[3];
};
#define FL_3D460 ((struct FusList_3D460 *)gUnk_02017A40)

void sub_0803D460(u16 num)
{
    int i;

    for (i = 0; i < FL_3D460->cnt; i++) {
        if (FL_3D460->list[i] != 0) {
            int y = CARD_NUMBER(FL_3D460->list[i]);
            int x = CARD_NUMBER(num);

            if (x > 0x7CF)
                x -= 0x7D0;
            if (y > 0x7CF)
                y -= 0x7D0;
            if (y == x) {
                FL_3D460->list[i] = 0;
                return;
            }
        }
    }
    if (sub_0803CB28(CARD_NUMBER(num)) == 0)
        return;
    for (i = 0; i < FL_3D460->cnt; i++) {
        if (FL_3D460->list[i] != 0 && sub_0803CB28(CARD_NUMBER(FL_3D460->list[i])) != 0) {
            FL_3D460->list[i] = 0;
            return;
        }
    }
}
INCLUDE_ASM("asm/nonmatching/code_0803C838", sub_0803D57C); /* 0x0803D57C size 0x800 */
