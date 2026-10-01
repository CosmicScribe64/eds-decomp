#include "global.h"

/*
 * Duel effect-condition predicates. Each takes (struct CardRef *ref, ?, u16 flag)
 * and returns bool. See wiki/functions/code-0802eb58.md.
 */

/* A card instance word as stored in the duel state. */
struct DuelCard {
    u32 id : 12;        /* card ID (index into gUnk_08621DE0 / gUnk_08622AB4); 0 = none */
    u32 unk12 : 20;
};

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4;
    u8 unk5;
    u8 flag6_0 : 1;         /* +0x06 bit 0 */
    u8 flag6_1 : 1;         /* +0x06 bit 1: face-up (hypothesis) */
    u8 counter6 : 4;        /* +0x06 bits 2-5: per-turn counter */
    u8 unk6_6 : 2;
    u8 unk7_0 : 5;
    u8 flag7_5 : 1;         /* +0x07 bit 5 */
    u8 unk7_6 : 2;
    u8 unk8[2];
    u16 links[32];          /* +0x0A: low byte = player, high byte = zone */
    u16 linkKinds[32];      /* +0x4A: low byte = kind of link i */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[0x94 - 0x8C];
};

/* Byte 6 of a zone as a plain byte (its flags are tested with mov #2; ldrb; and). */
#define ZFLAGS(z) (((u8 *)(z))[6])

/* The zones addressed from their own base (player + 0x28). */
struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); p is player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

/* Card reference (0x14 bytes, see code_08009A68 / code_0800C894). */
struct CardRef {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;       /* +0x02 bits 10-15 (hypothesis: effect kind) */
    u16 unk4;
    u16 pos;            /* +0x06: player (low byte) | zone << 8 of a target */
    u16 unk8;           /* +0x08: high byte = a zone/count limit checked by sub_0802E784 (hypothesis) */
    u8 fillerA[0xC - 0xA];
    u16 posC;           /* +0x0C: second target position (player | zone << 8), see sub_0802F5F8 */
    u8 fillerE[0x14 - 0xE];
};

/* The card word read as a whole u32 (the code always loads it with ldr). */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_SUBTYPE(id) ((CARD_STATS(id) & 0xE0000) >> 17)
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

/* Per-player duel state (0xD64 bytes, two at 0x020192E4); only the fields used here. */
struct DuelPlayer {
    u16 lifePoints;                 /* +0x000 */
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 count904;                    /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 countB84;                    /* +0x006 */
    u8 unk7;
    u8 unk8_0 : 4;
    u8 flag8_4 : 1;                 /* +0x008 bit 4 */
    u8 flag8_5 : 1;                 /* +0x008 bit 5 */
    u8 unk8_6 : 2;
    u8 unk9_0 : 5;
    u8 flag9_5 : 1;                 /* +0x009 bit 5 */
    u8 unk9_6 : 2;
    u8 unkA;
    u8 unkB_0 : 3;
    u8 flagB_3 : 1;                 /* +0x00B bit 3 */
    u8 unkB_4 : 4;
    u8 unkC[0x28 - 0xC];
    struct DuelZone zones[11];      /* +0x028 */
    struct DuelCard hand[80];       /* +0x684 */
    u8 unk7C4[0x904 - 0x7C4];
    struct DuelCard list904[80];    /* +0x904 (hypothesis: graveyard) */
    struct DuelCard fusionDeck[80]; /* +0xA44 (hypothesis) */
    u8 unkB84[0xD64 - 0xB84];
};
extern struct DuelPlayer gUnk_020192E4[2];

/* Duel state at 0x020192E0: the two players start at +4. */
struct DuelGlobal {
    u8 unk0[4];
    struct DuelPlayer players[2];   /* +0x004 */
    u8 unk1acc[0x1B12 - 0x1ACC];
    u8 byte1B12;                    /* bit 1 side/turn player, bits 2-4 phase (hypotheses) */
};
extern struct DuelGlobal gUnk_020192E0;
extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */
extern const u16 gUnk_08622AB4[];   /* maps card ID to card number */
/* Card ID as an 11-bit field (lsl #21; lsr #21). */
#define CARD_ID11(w) (((w) << 21) >> 21)

int sub_08008860(int player);
int sub_08008A1C(int player);
int sub_08008524(int player, u16 number);
int sub_080086CC(int player, u16 number);
int sub_08009CAC(int player, u16 number);
int sub_08047170(int player);
int sub_0800756C(u16 number);
int sub_0800C894(int player, int zone);   /* ATK of the card in a zone */
int sub_0800C8A8(int player, int zone);
int sub_0803D0E8(int player, u16 id, void *buf);
int sub_080090C8(int player, u16 number);
int sub_08044224(int player, int number, int b);
int sub_08047114(int player);
int sub_08054398(int player, u16 id);
int sub_08007834(u16 id);
int sub_08008AF8(int player, int zone);
int sub_080088A4(int player, int a, int b);
int sub_0802F808(u16 number, int player, struct CardRef *ref);
int sub_08008668(int player);
u16 sub_0802EB00(struct CardRef *ref, int a, u16 flag);
u16 sub_0802F5F8(u16 number, struct CardRef *ref, int a, int b);
int sub_08009E4C(int player);
int sub_080094E4(void);
u16 sub_080470C0(struct CardRef *ref, int a, int b);
int sub_08008A44(int player);
int sub_0800849C(int player, u16 number, int c);

int sub_0802EB58(struct CardRef *ref)
{
    int i;

    for (i = 0; i <= 4; i++) {
        int p = (1 - ref->player) & 1;
        struct DuelZone *z1 = ZB(p, i);
        if (CARD_ID(CARD_WORD(z1->card)) != 0) {
            struct DuelZone *z2;
            p = (1 - ref->player) & 1;
            z2 = ZB(p, i);
            if (ZFLAGS(z2) & 2)
                return 1;
        }
    }
    return 0;
}
/*
 * Card-number switch (0x46E..0x518): can card `ref` be used on `tgt` (hypothesis).
 * The shared ret1/ret0/sub4 labels only exist to pick the ROM's cross-jump survivors.
 */
int sub_0802EBB8(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    u8 pos = ((u8 *)ref)[6];
    u8 b8 = ((u8 *)ref)[8];
    u16 num;
    u32 st;
    int k;

    if (flag != 0)
        return 0;
    num = CARD_NUMBER(ref->id);
    switch (num) {
    case 0x46E:
        if (pos != ref->player)
            return 0;
        return ref->kind == 0x14;
    case 0x46F:
        if (pos != ref->player)
            return 0;
        return ref->kind == 0x15;
    case 0x470:
        return ref->kind == 0x19;
    case 0x471:
        if (tgt != 0) {
            st = CARD_STATS(tgt->id);
            if (((st & 0x1F00000) >> 20) == 0x16 && ((st & 0xE0000) >> 17) == 2)
                goto ret1;
        }
        return ref->kind == 0x18;
    case 0x472:
        if (tgt == 0)
            return 0;
        st = CARD_STATS(tgt->id);
        if (((st & 0x1F00000) >> 20) != 0x16)
            return 0;
    sub4:
        if (((st & 0xE0000) >> 17) == 4)
            goto ret1;
        return 0;
    case 0x473:
        if (tgt == 0)
            return 0;
        st = CARD_STATS(tgt->id);
        if (((st & 0x1F00000) >> 20) != 0x15)
            return 0;
        if (((st & 0xE0000) >> 17) == 4)
            goto ret1;
        return 0;
    case 0x474:
    case 0x518:
        k = ((u8 *)ref)[3] >> 2;
        switch (k) {
        case 15:
            if (((u8 *)ref)[8] != ref->player)
                return 0;
        ret1:
            return 1;
        case 13:
        case 14:
            if ((((u8 *)ref)[8] & 0xF) == ref->player)
                goto ret1;
            return 0;
        }
        return 0;
    case 0x475:
        if (pos == ref->player)
            return 0;
        if (sub_0800849C(1 - ref->player, num, -1) > 0)
            return 0;
        return ref->kind == 0x1A;
    case 0x476:
        if (pos != ref->player)
            return 0;
        if (sub_0800849C(1 - ref->player, num, -1) > 0)
            return 0;
        return ref->kind == 0x1D;
    case 0x47D:
        if (pos != ref->player)
            return 0;
        if (ref->kind == 0x1B)
            goto ret1;
        return 0;
    case 0x515:
        k = ((u8 *)ref)[3] >> 2;
        switch (k) {
        case 0x1E:
            if (pos != ref->player)
                goto ret0;
            goto ret1;
        case 0x13:
            if (pos == ref->player || b8 == ref->player)
                goto ret1;
            return 0;
        }
        return 0;
    }
ret0:
    return 0;
}
int sub_0802EE18(struct CardRef *ref)
{
    int i;
    int n = 0;

    for (i = 0; i < gUnk_020192E4[1 & ref->player].count904; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1 & ref->player].list904[i]));

        if (CARD_TYPE(id) <= 0x14)
            n++;
        if (n > 4)
            return 1;
    }
    return 0;
}
int sub_0802EEAC(struct CardRef *ref)
{
    int i, j;

    for (i = 0; i <= 4; i++) {
        for (j = 0; j <= 1; j++) {
            int p = j & 1;
            struct DuelZone *z = ZB(p, i);
            if (CARD_ID(CARD_WORD(z->card)) != 0 && !(ZFLAGS(z) & 2))
                return 1;
        }
    }
    return 0;
}
int sub_0802EF00(struct CardRef *ref)
{
    int i;

    if (sub_08047170(ref->player) == 0)
        return 0;
    for (i = 0; i <= 1; i++) {
        if (sub_08008A1C(i) > 0 && sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) != 0)
            return 1;
    }
    return 0;
}
int sub_0802EF5C(struct CardRef *ref, int a, u16 flag)
{
    if (gUnk_020192E4[ref->player].lifePoints <= 0x31F)
        return 0;
    return sub_0802EB00(ref, a, flag);
}
int sub_0802EFA0(struct CardRef *ref)
{
    if ((((u8 *)ref)[3] >> 2) == 0xD && (((u8 *)ref)[8] & 0xF) != ref->player)
        return 1;
    return 0;
}
int sub_0802EFC4(void)
{
    int n = sub_080088A4(0, 1, 0);
    n += sub_080088A4(1, 1, 0);
    if (n <= 1)
        return 0;
    return 1;
}
#define ZP(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_020192E4->zones))
int sub_0802EFF0(struct CardRef *ref)
{
    int i;

    if (gUnk_020192E4[ref->player].lifePoints <= 999)
        return 0;
    for (i = 0; i <= 10; i++) {
        int p = (1 - ref->player) & 1;
        struct DuelZone *z1 = ZP(p, i);
        if (CARD_ID(CARD_WORD(z1->card)) != 0) {
            struct DuelZone *z2;
            p = (1 - ref->player) & 1;
            z2 = ZP(p, i);
            if (!(ZFLAGS(z2) & 2))
                return 1;
        }
    }
    return 0;
}
int sub_0802F068(struct CardRef *ref)
{
    return sub_080088A4(ref->player, 1, 0) > 0;
}
int sub_0802F088(struct CardRef *ref)
{
    return sub_080088A4(1 - ref->player, 1, 0) > 0;
}
int sub_0802F0AC(struct CardRef *ref, int a, u16 n)
{
    struct DuelPlayer *pl = gUnk_020192E4;

    if (pl[(1 - ref->player) & 1].handCount != 0 && pl[1 & ref->player].handCount - n > 0)
        return 1;
    return 0;
}
int sub_0802F0F4(struct CardRef *ref)
{
    if (sub_080088A4(1 - ref->player, 0, 0) == 0
        || (sub_08008524(ref->player, 0x22) <= 0 && sub_08008524(ref->player, 0x4BA) <= 0
            && sub_08008524(ref->player, 0x7F2) <= 0))
        return 0;
    return 1;
}
int sub_0802F154(struct CardRef *ref)
{
    if (sub_08008A1C(ref->player) == 0 || sub_08047170(ref->player) == 0
        || gUnk_020192E4[ref->player].flag8_4)
        return 0;
    return sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) > 0;
}
int sub_0802F1C8(struct CardRef *ref)
{
    if (sub_080088A4(1 - ref->player, 0, 0) != 0 && sub_080088A4(ref->player, 0, 0) != 0)
        return 1;
    return 0;
}






/* Monster level as the game computes it: Magic/Trap types 0x15-0x17 count as 0, type 0x18 as 10. */
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
/* Zone address from the duel-state base 0x020192E0 (players[0].zones = 0x0201930C). */
#define ZG(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_020192E0.players->zones))
extern u8 gUnk_02019968[];
#if 0 /* NONMATCHING: only the hand-count base differs. The build keeps the
       * flag-check gUnk_020192E4 base in sl across the loop, while the ROM
       * reloads it at the loop test (and swaps r3/r4 for base/0xD64). */
int sub_0802F200(struct CardRef *ref)
{
    int i;
    s16 lvl;

    if ((gUnk_020192E0.byte1B12 & 0x1C) != 8)
        return 0;
    if (sub_08008860(ref->player) == 0)
        return 0;
    if (sub_08008524(0, 0x58A) > 0 || sub_08008524(1, 0x58A) > 0)
        return 0;
    for (i = 0; i <= 4; i++) {
        int p = 1 & ref->player;
        struct DuelZone *z1 = ZG(p, i);
        u16 id = CARD_ID(CARD_WORD(z1->card));
        if (id != 0) {
            int p2 = 1 & ref->player;
            struct DuelZone *z2 = ZG(p2, i);
            if (ZFLAGS(z2) & 2) {
                switch (CARD_NUMBER(id)) {
                case 0x58:
                case 0x105:
                case 0x1FF:
                    return 1;
                }
            }
        }
    }
    if (gUnk_020192E4[1 & ref->player].flag8_4)
        return 0;
    for (i = 0; i < gUnk_020192E4[1 & ref->player].handCount; i++) {
        u16 id = CARD_ID(*(u32 *)(ref->player * 0xD64 + i * 4 + (u32)gUnk_02019968));

        if (CARD_TYPE(id) <= 0x14 && sub_08007834(id) == 0) {
            CARD_LEVEL(id, lvl);
            if (lvl > 4) {
                CARD_LEVEL(id, lvl);
                if (lvl <= 6) {
                    if (sub_08008A44(ref->player) != -1)
                        return 1;
                } else if (sub_08008AF8(ref->player, -1) > 0)
                    return 1;
            }
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0802EB58", sub_0802F200); /* 0x0802F200 size 0x250 */

int sub_0802F450(struct CardRef *ref)
{
    if (sub_08008524(0, 0x58A) > 0 || sub_08008524(1, 0x58A) > 0
        || sub_080086CC(ref->player, 0x39) <= 0)
        return 0;
    return 1;
}
int sub_0802F490(struct CardRef *ref)
{
    struct DuelPlayer *players = gUnk_020192E4;
    u32 shifted = (u32)((u8 *)ref)[2] << 31;

    if ((s32)((u32)((u8 *)&players[shifted >> 31])[8] << 26) >= 0) {
        /* FAKEMATCH: re-extract the player bit for the call as the ROM does. */
        __asm__("" : "+r"(shifted));
        if (sub_08008A1C(shifted >> 31) > 3)
            return 1;
    }
    return 0;
}
int sub_0802F4C8(void)
{
    if ((gUnk_020192E0.byte1B12 & 0x1C) == 8)
        return 1;
    return 0;
}
int sub_0802F4E8(struct CardRef *ref)
{
    int r = 0;
    struct DuelPlayer *pl = gUnk_020192E4;
    int p = ref->player;

    p ^= 1;
    if (pl[p].lifePoints <= 3000)
        r = 1;
    return r;
}
int sub_0802F518(struct CardRef *ref, int a, u16 flag)
{
    if (flag == 0 && gUnk_020192E4[(1 - ref->player) & 1].handCount > 5
        && gUnk_020192E4[1 & ref->player].handCount <= 2)
        return 1;
    return 0;
}
int sub_0802F560(struct CardRef *ref)
{
    if ((gUnk_020192E0.byte1B12 & 0x1C) == 8) {
        struct DuelPlayer *players = gUnk_020192E0.players;
        register u32 shifted __asm__("r3") = (u32)((u8 *)ref)[2] << 31;
        int one = 1;

        /* FAKEMATCH: keep the initialized mask as a separate register
         * value rather than folding it into the one-bit player range. */
        __asm__("" : "+r"(one));
        if ((s32)((u32)((u8 *)&players[shifted >> 31])[9] << 26) < 0)
            return 0;
        /* FAKEMATCH: preserve the original shifted value through both
         * flag tests, so each player extraction uses the ROM's r0. */
        __asm__("" : : "r"(shifted));
        if ((s32)((u32)((u8 *)&players[one & (shifted >> 31)])[8] << 27) < 0)
            return 0;
        __asm__("" : : "r"(shifted));
        return 1;
    }
    return 0;
}
int sub_0802F5B8(struct CardRef *ref, int a, u16 n)
{
    if (sub_08008860(ref->player) != 0 && gUnk_020192E4[ref->player].handCount != n)
        return 1;
    return 0;
}
/*
 * Can effect `number` (0x431 / 0x525 / ...) of card `ref` act on the zone (player a, zone b)?
 * The first read of ref->id is volatile only to stop gcse from hoisting the load above the
 * `number == 0x431` test (the ROM loads it in both places). The gotos reproduce the ROM's
 * block layout (shared return-0 block placed right after the zone check).
 */
u16 sub_0802F5F8(u16 number, struct CardRef *ref, int a, int b)
{
    if (ref == 0)
        return 0;
    if (number == 0x431) {
        u32 mask = 0x7FF;
        u32 id = ((volatile struct CardRef *)ref)->id & mask;
        if (CARD_TYPE(id) != 0x16)
            return 0;
    }
    switch (CARD_NUMBER(ref->id)) {
    case 0x12c ... 0x13c:
    case 0x13e ... 0x147:
    case 0x28b:
    case 0x28d:
    case 0x290:
    case 0x29b:
    case 0x3c2:
    case 0x3f4 ... 0x3f5:
    case 0x3ff:
    case 0x403:
    case 0x412 ... 0x413:
    case 0x416 ... 0x417:
    case 0x422:
    case 0x424:
    case 0x430:
    case 0x433 ... 0x434:
    case 0x485:
    case 0x49e:
    case 0x4bb:
    case 0x4c4:
    case 0x4dc:
    case 0x515:
    case 0x521:
    case 0x587:
    case 0x58b ... 0x58c:
    case 0x58e:
    case 0x5a8 ... 0x5ab:
    case 0x5f7:
    case 0x5f9:
    case 0x5fc:
    case 0x604:
    case 0x60a:
    case 0x60c:
    case 0x60e:
    {
        int player = (u8)ref->posC;
        int zone = ref->posC >> 8;
        int p = a & 1;
        struct DuelZone *z = ZB(p, b);

        if (CARD_ID(CARD_WORD(z->card)) != 0)
            goto cont;
    ret0:
        return 0;
    cont:
        if (a == player && b == zone)
            goto ret0;
        return sub_080470C0(ref, a, b);
    }
    default:
        goto ret0;
    }
}
int sub_0802F808(u16 number, int player, struct CardRef *ref)
{
    int i, j;
    int count = 0;

    switch (number) {
    case 0x431:
        for (i = 0; i <= 1; i++) {
            for (j = 0; j <= 4; j++) {
                if (sub_0802F5F8(number, ref, i, j))
                    count++;
            }
        }
        break;
    case 0x525:
        for (j = 0; j <= 4; j++) {
            if (sub_0802F5F8(number, ref, player, j))
                count++;
        }
        break;
    }
    return count;
}
int sub_0802F888(struct CardRef *ref, struct CardRef *tgt)
{
    if (ref->kind == 0x10 && ((u8 *)ref)[6] != ref->player)
        return sub_08008860(ref->player) > 1;
    if (tgt == 0 || (((u8 *)tgt)[2] & 1) == (((u8 *)ref)[2] & 1))
        return 0;
    return sub_0802F808(0x525, ref->player, tgt) > 0;
}
int sub_0802F8E8(void)
{
    return sub_08008A1C(0) + sub_08008A1C(1) > 1;
}
int sub_0802F90C(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag == 0 && tgt != 0) {
        u32 id = tgt->id & 0x7FF;

        if (CARD_TYPE(id) == 0x16 && CARD_NUMBER(id) != 0x603)
            return 1;
    }
    return 0;
}
int sub_0802F958(struct CardRef *ref, int a, u16 flag)
{
    int new_var;
    int i;

    if (flag != 0)
        return 0;
    for (i = 0; i < gUnk_020192E4[1 & ref->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1 & ref->player].hand[i]));

        if ((new_var = CARD_TYPE(id) != 0x16))
            continue;
        return 1;
    }
    return 0;
}
int sub_0802F9E4(struct CardRef *ref, int a, u16 flag)
{
    if (flag == 0 && ref->kind == 8) {
        int player = (u8)ref->pos;
        int zone = ref->pos >> 8;

        if (player != ref->player) {
            int p = player & 1;
            struct DuelZone *z = ZB(p, zone);

            if (CARD_ID(CARD_WORD(z->card)) != 0 && (1 & ZFLAGS(z)) && !(ZFLAGS(z) & 2))
                return 1;
        }
    }
    return 0;
}
int sub_0802FA44(struct CardRef *ref, int a, u16 flag)
{
    int i;
    int other = 0;
    int mon = 0;

    if (sub_08008A1C(ref->player) == 0)
        return 0;
    for (i = 0; i < gUnk_020192E4[ref->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[ref->player].hand[i]));

        if (CARD_TYPE(id) <= 0x14) {
            if (sub_08007834(id) == 0)
                mon++;
        } else
            other++;
    }
    if (flag != 0 && other != 0)
        other--;
    if (mon != 0 && other > 1)
        return 1;
    return 0;
}
int sub_0802FB14(struct CardRef *ref)
{
    if (sub_08009E4C(ref->player) == 0 || sub_08044224(ref->player, 0x58D, 0) <= 0)
        return 0;
    return 1;
}
int sub_0802FB48(void)
{
    if (sub_080094E4() == 0x14D)
        return 1;
    return 0;
}
