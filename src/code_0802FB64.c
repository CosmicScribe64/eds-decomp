#include "global.h"

/*
 * Duel effect-condition predicates with the signature (struct CardRef *ref, ?, u16 flag),
 * each returning a bool. See wiki/functions/code-0802fb64.md.
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
    u16 unkE;           /* +0x0E */
    u8 fillerF[0x14 - 0x10];
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
extern const u16 gUnk_08622AB4[];   /* card ID to card number */
/* Card ID as an 11-bit field (lsl #21; lsr #21). */
#define CARD_ID11(w) (((w) << 21) >> 21)

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
u16 sub_0802D53C(struct CardRef *ref, struct CardRef *tgt, int c);
int sub_08008B70(int player, int a, int b, int c);
u32 sub_0802C080(struct CardRef *ref, u16 pos);
int sub_0800CCCC(int a, int b, int c, int d);
int sub_0800CD68(int a, int b);
int sub_08009DEC(int player);
void sub_0801EC58(int a, u16 b, u16 c, int d);
void sub_08018544(int a, int b, int c);
void sub_08018ED8(int a, int b, int c, int d);
int sub_0800C8BC(int player, int zone);
void sub_08017AB4(int player, u16 a, u16 b, int c);
void sub_08019820(int player, u16 id);
void sub_0802AF34(int player, int area, int a2, int a3);
int sub_0801970C(int player, u16 number);
int sub_0807548C(int a);
void sub_08019860(int player, int amount);
int sub_08017FF4(int player, int zone);
void sub_080189FC(int player, int zone, int c);
void sub_08017B04(int player, u16 pos, u16 c);
u16 sub_08030880(struct CardRef *ref);
int sub_0803E160(struct CardRef *ref);
void sub_080753F4(char *dst, const char *fmt, const char *arg);
void sub_080602A4(int a, int b, int c, char *s);
void sub_08060308(int a, int b, int c);
/* Duel card-list viewer at 0x0201D810 (see code_0802AAC0). */
struct ListView {
    u8 flags0;
    u8 step;
    u8 state;
    u8 unk3;
    u8 unk4;
    u8 row : 2;         /* +5 bits 0-1 */
    u8 unk5_2 : 6;
    u16 top;            /* +6 */
    u8 unk8[4];
    u32 cards[1];       /* +0xC */
};
extern struct ListView gUnk_0201D810;
struct Unk02015F00 {
    u8 pad[0x1B22];
    u16 savedTop;       /* +0x1B22: saved list position */
};
extern struct Unk02015F00 gUnk_02015F00;
extern char gUnk_0808284C[];
extern char gUnk_0808287C[];
int sub_08056ECC(u16 id);
void sub_08007558(void *dst, void *src);
int sub_08053AF8(int player);
void sub_08022A9C(int player);
/* Effect scratch state at 0x02017E20 (hypothesis: one struct; fields found through CSE'd offsets). */
struct Unk02017E20 {
    u8 counter;                 /* +0x000 */
    u8 pad1[0x15C - 1];
    u32 pad0 : 12;              /* +0x15C: four 8-bit fields starting at bit 12 */
    u32 f1 : 8;
    u32 f2 : 8;
    u32 f3 : 8;
    u32 f4 : 8;
    u32 saved[5];               /* +0x164: copy of the deck words (+0x7C4 of a player) */
};
extern struct Unk02017E20 gUnk_02017E20;
extern u8 gUnk_02017A40[];               /* duel/effect scratch; +0x3E0 = effect step (0x7D..0x80), +0x3E5 flag */
struct Unk0201AE60 { u8 pad[0x14]; u16 v14; };
extern struct Unk0201AE60 gUnk_0201AE60;
extern const char gUnk_080828CC[];
extern const char gUnk_0808292C[];
extern const char gUnk_0822C720[];
extern u16 gUnk_08624052;
int sub_0800849C(int player, u16 number, int c);


int sub_0802FB64(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag != 0)
        return 0;
    switch (ref->kind) {
    case 5:
    case 6:
    case 7:
        return sub_0802D53C(ref, tgt, 0);
    }
    return 0;
}
int sub_0802FB8C(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag == 0 && tgt == 0 && ref->kind == 0x10) {
        int player = (u8)ref->unk8;
        int zone = ref->unk8 >> 8;
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) != 0 && player == ref->player && sub_080086CC(player, 0x57D) > 0)
            return 1;
    }
    return 0;
}
int sub_0802FBF4(struct CardRef *ref)
{
    if (sub_08008524(0, 0x58A) > 0 || sub_08008524(1, 0x58A) > 0
        || sub_08044224(ref->player, 0x58D, 0) <= 0)
        return 0;
    return 1;
}
int sub_0802FC38(struct CardRef *ref)
{
    if (sub_08008AF8(ref->player, -1) > 1)
        return 1;
    return 0;
}
int sub_0802FC58(struct CardRef *ref)
{
    if (sub_08008524(0, 0x58A) > 0 || sub_08008524(1, 0x58A) > 0
        || sub_08044224(ref->player, 0x59F, 0) <= 0)
        return 0;
    return 1;
}
int sub_0802FC9C(struct CardRef *ref)
{
    if (gUnk_020192E4[ref->player].handCount <= 1)
        return 0;
    return 1;
}
int sub_0802FCC0(struct CardRef *ref, int a, u16 flag)
{
    if (flag != 0)
        return 0;
    if (gUnk_020192E4[ref->player].handCount != 0)
        return 1;
    return 0;
}
int sub_0802FCEC(void)
{
    int n = sub_08008B70(0, 0, 0, 1);
    n += sub_08008B70(1, 0, 0, 1);
    return n > 1;
}
int sub_0802FD18(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag == 0 && tgt != 0 && CARD_TYPE(tgt->id) == 0x16 && sub_08008524(0, 0x58A) <= 0
        && sub_08008524(1, 0x58A) <= 0 && CARD_NUMBER(tgt->id) != 0x603)
        return 1;
    return 0;
}
int sub_0802FD90(struct CardRef *ref, int a, u16 flag)
{
    int i, z, j, k;

    if (flag != 0)
        return 0;
    for (i = 0; i <= 1; i++) {
        int p;

        for (z = 0; z <= 4; z++) {
            struct DuelZone *zn;

            p = i & 1;
            zn = ZB(p, z);

            if (CARD_ID(CARD_WORD(zn->card)) != 0 && (ZFLAGS(zn) & 2)) {
                for (j = 0; j <= 1; j++) {
                    for (k = 5; k <= 9; k++) {
                        u8 ok = sub_0802C080(ref, (u8)j | ((u8)k << 8)) != 0;

                        if (sub_0800CCCC(j, k, i, z) == 0)
                            ok = 0;
                        if (sub_0800CD68(j, k) == (u16)((u8)i | ((u8)z << 8)))
                            ok = 0;
                        if (ok)
                            return 1;
                    }
                }
            }
        }
    }
    return 0;
}
int sub_0802FE6C(struct CardRef *ref)
{
    if (ref->kind != 0x10)
        return 0;
    if (((u8 *)ref)[6] == ref->player)
        return 0;
    if (sub_080088A4(1 - ref->player, 1, 0) <= 1)
        return 0;
    return 1;
}
int sub_0802FEA4(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    unsigned long long base = 0x08621DE0; /* FAKEMATCH: a 64-bit temp stops agbcc from hoisting the stats-table address out of the loop */
    int n;
    int i, j;
    int lvl;

    if (flag != 0 || tgt != 0)
        return 0;
    if (sub_08008524(1 - ref->player, 0x5E7) > 0)
        return 0;
    n = sub_08009DEC(ref->player);
    if (n == 0)
        return 0;
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            struct DuelZone *z = ZB(i & 1, j);
            u16 id = CARD_ID(CARD_WORD(z->card));

            if ((ZFLAGS(z) & 2) && id != 0) {
                u32 stats = ((const u32 *)(u32)base)[id & 0x7FF];

                switch ((int)((stats & 0x1F00000) >> 20)) {
                case 0x15:
                case 0x16:
                case 0x17:
                    lvl = 0;
                    break;
                case 0x18:
                    lvl = 10;
                    break;
                default:
                    lvl = (CARD_STATS(id) & 0x1E000000) >> 25;
                    break;
                }
                if (lvl <= n)
                    return 1;
            }
        }
    }
    return 0;
}
int sub_0802FF90(struct CardRef *ref, int a, u16 flag)
{
    if (flag == 0 && ref->kind == 0xE) {
        if ((ref->unk8 & 0xF) != ref->player) {
            u16 hi = ref->unk8 >> 8;
            int q = hi & 0xF;
            int zone = hi >> 4;
            int p = q & 1;
            struct DuelZone *z = ZB(p, zone);

            return z->flag6_0;
        }
    }
    return 0;
}
int sub_0802FFE4(struct CardRef *ref)
{
    if (sub_080086CC(0, 0x453) <= 0 && sub_080086CC(1, 0x453) <= 0 && sub_08044224(ref->player, 0x60D, 0) > 4)
        return 1;
    return 0;
}
void sub_08030028(int player, int zone)
{
    int p = 1 & player;
    struct DuelZone *first;
    struct DuelZone *z;
    u16 id;

    first = ZB(p, zone);
    /* FAKEMATCH: preserve the first address in r0 without an index copy. */
    __asm__("" : : "r"(first));
    id = CARD_ID(CARD_WORD(first->card));

    if (id != 0) {
        switch (CARD_NUMBER(id)) {
        case 0x2F:
        case 0x23D:
        case 0x4D9:
        case 0x4E9:
            sub_0801EC58(player ? 0x8090 : 0x90, zone, 1, 0);
            p = 1 & player;
            /* FAKEMATCH: keep the recomputed player mask in r2. */
            __asm__("" : : "r"(p));
            z = ZB(p, zone);
            ((u8 *)z)[1] |= 0x40;
        }
        sub_08018544(player, zone, 1);
    }
}
int sub_080300D8(struct CardRef *ref)
{
    int i, j;

    if (((u8 *)ref)[4] & 4)
        return 0;
    for (i = 0; i <= 1; i++) {
        for (j = 5; j <= 9; j++) {
            struct DuelZone *z = ZB(i & 1, j);
            int id = CARD_ID(CARD_WORD(z->card));

            if (id > 0 && (ZFLAGS(z) & 2) && CARD_NUMBER(id) == 0x148)
                sub_08018544(i, j, 1);
        }
    }
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            struct DuelZone *z = ZB(i & 1, j);

            if (CARD_ID(CARD_WORD(z->card)) != 0 && (ZFLAGS(z) & 3) == 3 && sub_0800C8BC(i, j) == 1)
                sub_08018ED8(i, j, 0, 0);
        }
    }
    return 0;
}
int sub_080301C0(struct CardRef *ref)
{
    if (((u8 *)ref)[4] & 4)
        return 0;
    switch (gUnk_02017A40[0x3E0]) {
    case 0x80:
        if (sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) == 0) {
            if (((u8 *)ref)[2] & 1)
                return 0;
            sub_080602A4(0x205, 0x914, 0xB, gUnk_0808284C);
            return 0x64;
        }
        if (((u8 *)ref)[2] & 1) {
            sub_08056ECC(ref->id);
            gUnk_0201D810.row = 0;
            gUnk_0201D810.top = gUnk_02015F00.savedTop;
            return 0x7E;
        }
        sub_080602A4(0x205, 0x914, 0xB, gUnk_0808287C);
        return 0x7F;
    case 0x7F:
        sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
        return 0x7E;
    case 0x7E:
        sub_08019820(ref->player, CARD_ID(gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row]));
        if (sub_0801970C(ref->player, ((const u16 *)0x08622AB4)[CARD_ID11(gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row])]) != 0)
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
        return 0x64;
    }
    return 0;
}
int sub_0803033C(struct CardRef *ref)
{
    int i, j;

    if (((u8 *)ref)[4] & 4)
        return 0;
    sub_0801EC58((((u8 *)ref)[2] & 1) ? 0x8092 : 0x92, ref->zone, 0, 0);
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            struct DuelZone *z = ZB(i & 1, j);

            if (CARD_ID(CARD_WORD(z->card)) != 0 && (ZFLAGS(z) & 2) && sub_0800C8BC(i, j) == 2)
                sub_08017AB4(ref->player, ref->player | (ref->zone << 8), (u8)i | ((u8)j << 8), 2);
        }
    }
    return 0;
}
int sub_08030400(struct CardRef *ref)
{
    if (((u8 *)ref)[4] & 4)
        return 0;
    if ((((u8 *)ref)[0xA] & 7) != 1)
        return 0;
    {
        int player = (u8)ref->posC;
        int zone = ref->posC >> 8;
        int p = 1 & player;
        struct DuelZone *z = ZB(p, zone);
        u16 id = CARD_ID(CARD_WORD(z->card));

        if (id == 0)
            return 0;
        if (ZFLAGS(z) & 2) {
            if (CARD_TYPE(id) == 0x15)
                sub_08018544(player, zone, 1);
            return 0;
        }
        sub_0801EC58(player ? 0x807F : 0x7F, zone, 0, 0);
        sub_08019820(player, id);
        if (CARD_TYPE(id) == 0x15)
            sub_08018544(player, zone, 1);
        else
            sub_0801EC58(player ? 0x807F : 0x7F, zone, 0, 0);
    }
    return 0;
}
int sub_080304E4(struct CardRef *ref)
{
    if ((((u8 *)ref)[0xA] & 7) != 1)
        return 0;
    {
        int player = (u8)ref->posC;
        int zone = ref->posC >> 8;
        int p = 1 & player;
        struct DuelZone *z = ZB(p, zone);
        int amount;

        if (CARD_ID(CARD_WORD(z->card)) == 0)
            return 0;
        amount = 0;
        switch (CARD_NUMBER(ref->id)) {
        case 0x58:
            amount = sub_0807548C(sub_0800C894(player, zone));
            break;
        case 0x1FF:
            amount = 500;
            break;
        }
        if (sub_08017FF4(player, zone) != 0)
            sub_08019860(1 - ref->player, amount);
    }
    return 0;
}
int sub_08030578(struct CardRef *ref)
{
    u32 id;

    if (((u8 *)ref)[4] & 4)
        return 0;
    if ((((u8 *)ref)[0xA] & 7) != 2)
        return 0;
    if (sub_08009CAC(ref->player, CARD_NUMBER(id = (u32)(ref->posC << 20) >> 20)) > 0) {
        sub_08019820(ref->player, id);
        {
            int player = ((u8 *)ref)[2] & 1;
            register int snd __asm__("r3") = 0xD2;

            if (player)
                snd = 0x80D2;
            {
                u16 pos = ref->posC;
                int zero = 0;

                /* FAKEMATCH: prepare r1/r2 before copying the sound id to r0. */
                __asm__("" : : "r"(pos), "r"(zero) : "r0");
                sub_0801EC58(snd, pos, zero, 0);
            }
        }
    }
    return 0;
}
int sub_080305EC(struct CardRef *ref)
{
    if (((u8 *)ref)[4] & 4)
        return 0;
    sub_0801EC58((((u8 *)ref)[2] & 1) ? 0x8060 : 0x60, 0, 0, 0);
    return 0;
}
#if 0 /* NONMATCHING: everything matches structurally (struct at 0x02017E20 with bitfields at
       * +0x15C and array at +0x164), but old_agbcc hoists the 'counter' address computation
       * (r5-0x164) above the bitfield RMWs (gcse hoisting, which the ROM does not do) and
       * puts the ref[2] byte in r4 instead of r3. */
int sub_08030620(struct CardRef *ref)
{
    int i;

    switch (gUnk_02017A40[0x3E0]) {
    case 0x80:
        if (gUnk_020192E4[ref->player].deckCount <= 4)
            return 0;
        for (i = 0; i <= 4; i++)
            sub_08007558(&gUnk_02017E20.saved[i], (u8 *)gUnk_020192E4[ref->player].unk7C4 + i * 4);
        gUnk_02017E20.f1 = 0;
        gUnk_02017E20.f2 = 0;
        gUnk_02017E20.f3 = 0;
        gUnk_02017E20.f4 = 0;
        gUnk_02017E20.counter -= 1;
    case 0x7F:
        if (sub_08053AF8(ref->player) != 0)
            return 0x7E;
        return 0x7F;
    case 0x7E:
        for (i = 0; i <= 4; i++)
            sub_08007558((u8 *)gUnk_020192E4[ref->player].unk7C4 + i * 4, &gUnk_02017E20.saved[i]);
        if (1 & ((u8 *)0x02015EE8)[1]) {
            sub_08022A9C(ref->player);
            return 0x7D;
        }
        return 0;
    case 0x7D:
        if (!(((u8 *)0x02017FB0)[0x305] & 2))
            return 0x7D;
        return 0;
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0802FB64", sub_08030620); /* 0x08030620 size 0x140 */
int sub_08030760(struct CardRef *ref)
{
    sub_0801EC58((((u8 *)ref)[2] & 1) ? 0x80D6 : 0xD6, 1, 0, 0);
    sub_0801EC58((((u8 *)ref)[2] & 1) ? 0x8060 : 0x60, 1, 0, 0);
    return 0;
}
int sub_080307AC(struct CardRef *ref)
{
    int p1 = (u8)ref->pos;
    int z1 = ref->pos >> 8;
    int p2 = (u8)ref->unk8;
    int z2 = ref->unk8 >> 8;

    sub_080189FC(p1, z1, 0);
    sub_080189FC(p2, z2, 0);
    return 0;
}
int sub_080307D4(struct CardRef *ref)
{
    if ((((u8 *)ref)[0xA] & 7) != 1)
        return 0;
    {
        int player = (u8)ref->posC;
        int zone = ref->posC >> 8;
        int p = 1 & player;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) == 0)
            return 0;
        if (sub_08017FF4(player, zone) != 0)
            sub_08017AB4(ref->player, ref->id, ref->player | (ref->zone << 8), 3);
    }
    return 0;
}
int sub_08030838(struct CardRef *ref)
{
    int n = sub_08008B70(1 - ref->player, 0, 0, 0);

    if (!(((u8 *)ref)[4] & 4) && n > 0)
        sub_08019860(1 - ref->player, n * 500);
    return 0;
}
u16 sub_08030880(struct CardRef *ref)
{
    int p = 1 & ref->player;
    struct DuelZone *z = ZB(p, ref->zone);

    if (CARD_ID(CARD_WORD(z->card)) != 0) {
        int step = ((u8 *)ref)[0xA] & 7;

        if (step == 1) {
            sub_08017B04(ref->player, ref->player | (ref->zone << 8), ref->posC);
        } else if (CARD_NUMBER(ref->id) == 0x3C2 && step == 2) {
            sub_08017B04(ref->player, ref->player | (ref->zone << 8), ref->posC);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8087 : 0x87, ref->zone, ref->unkE, 0);
        }
    }
    return 0;
}
u16 sub_08030928(struct CardRef *ref)
{
    char buf[0x100];

    if ((((u8 *)ref)[2] & 0xE) != 6)
        return sub_08030880(ref);
    switch (gUnk_02017A40[0x3E0]) {
    case 0x80:
        if (((u8 *)ref)[2] & 1)
            return 0;
        if (sub_08008AF8(ref->player, -1) == 0)
            return 0x64;
        sub_080753F4(buf, gUnk_080828CC, gUnk_0822C720 + (gUnk_08624052 << 6));
        sub_080602A4(0x205, 0x914, 0xB, buf);
        sub_08060308(1, 0, 0);
        return 0x7F;
    case 0x7F:
        if (gUnk_0201AE60.v14 == 0)
            return 0x64;
    {
        int offset = 0x3E5;

        gUnk_02017A40[offset] = 0;
        /* FAKEMATCH: retain the initialized offset through the store so
         * the zero value uses r0 and the offset uses r2, as in the ROM. */
        __asm__("" : : "r"(offset));
    }
    s7e:
        return 0x7E;
    case 0x7E:
        if (sub_0803E160(ref) == 0)
            goto s7e;
        return 0x7D;
    case 0x7D:
    {
        int player = (u8)ref->posC;
        u16 pos = ref->posC;
        int zone;

        zone = pos >> 8;
        /* FAKEMATCH: preserve the initialized position in r3 and reserve
         * r2 through argument extraction, reproducing the ROM registers. */
        __asm__("" : : "r"(pos) : "r2");

        if (sub_08017FF4(player, zone) != 0)
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D0 : 0xD0, ref->id, 0, 0);
        return 0x64;
    }
    }
    return 0;
}
u16 sub_08030A28(struct CardRef *ref)
{
    u8 b2 = ((u8 *)ref)[2];
    /* Reserve the original scratch register without emitting code. */
    __asm__("" : : : "r1");
    if ((b2 & 0xE) != 6)
        return sub_08030880(ref);
    if (!(((u8 *)ref)[4] & 4))
        sub_08019860(1 - ref->player, 500);
    return 0;
}
static inline u32 PlayerBitFromModeByte(u8 byte)
{
    register u32 shifted __asm__("r0") = (u32)byte << 31;

    /* FAKEMATCH: retain the ROM's lsl/lsr extraction rather than an and. */
    __asm__("" : : "r"(shifted));
    return shifted >> 31;
}

u16 sub_08030A64(struct CardRef *ref)
{
    char buf[0x100];
    register u8 b2 __asm__("r3") = ((u8 *)ref)[2];

    if ((b2 & 0xE) != 6)
        return sub_08030880(ref);
    switch (gUnk_02017A40[0x3E0]) {
    case 0x80:
        if (gUnk_020192E4[PlayerBitFromModeByte(b2)].lifePoints <= 0x1F3)
            return 0;
        sub_080753F4(buf, gUnk_0808292C, gUnk_0822C720 + (ref->id << 6));
        sub_080602A4(0x205, 0x914, 0xB, buf);
        sub_08060308(1, 0, 0);
        return 0x7F;
    case 0x7F:
        if (gUnk_0201AE60.v14 != 0) {
            sub_0801EC58((1 & b2) ? 0x8043 : 0x43, 500, 1, 0);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D0 : 0xD0, ref->id, 0, 0);
        }
        return 0x64;
    }
    return 0;
}
u16 sub_08030B4C(struct CardRef *ref)
{
    u8 b2 = ((u8 *)ref)[2];

    __asm__("" : : : "r1");
    if ((b2 & 0xE) != 6)
        return sub_08030880(ref);
    {
        register int player __asm__("r0") = b2 & 1;
        register int msg __asm__("r3") = 0xD0;

        if (player)
            msg = 0x80D0;
        sub_0801EC58(msg, ref->id, 0, 0);
    }
    return 0;
}
