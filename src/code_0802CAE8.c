#include "global.h"
#include "duel.h"
#include "duel_ui.h"

/*
 * Duel card-effect targeting: per-card "can this card be targeted" dispatcher (sub_0802CE38),
 * card kind/level helper, and card-effect prompt steps.  See wiki/functions/code-0802cae8.md.
 */

/* Byte 6 of a zone as a plain byte (its flags are tested with mov #2; ldrb; and). */
#define ZFLAGS(z) (((u8 *)(z))[6])

/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); p is player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

/* Zone pointer with the player term first. */
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))

/* The bitfield word at +2 of a CardRef-shaped entry. */
struct RefFlags {
    u8 player : 1;
    u8 unk_1 : 3;
    u16 zone : 6;
    u16 unk_10 : 6;
    u8 rest[0x12];
};

/* Card reference (0x14 bytes, see code_08009A68 / code_0800C894). */
struct CardRef {
    u16 id;             /* +0x00 card ID */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 unk2_10 : 6;
    u16 unk4;
    u16 unk6;
    u8 filler8[4];
    u16 unkC;           /* +0x0C: player | zone << 8 of a second card */
    u8 filler10[0x14 - 0x10];
};

/* The card word read as a whole u32 (the code always loads it with ldr). */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[0x7FF & (id)])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[0x7FF & (id)])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)


/* Local view of struct DuelPlayer (duel.h): the canonical struct exposes a plain u8 unk8 at
 * +0x08, but sub_0802D47C tests bit 4 of that byte as a bitfield, so keep the split here. */
struct DuelPlayerView {
    u16 lifePoints;         /* +0x00 */
    u8 handCount;           /* +0x02 */
    u8 deckCount;           /* +0x03 */
    u8 graveCount;          /* +0x04 */
    u8 fusionCount;         /* +0x05 */
    u8 countB84;            /* +0x06 */
    u8 unk7;                /* +0x07 */
    u8 unk8_0 : 4;          /* +0x08 */
    u8 flag8_4 : 1;         /* +0x08 bit 4 */
    u8 unk8_5 : 3;
    u8 unk9;                /* +0x09: bit 4 set by sub_0802CAE8 */
    u8 filler0A[0xD64 - 0xA];
};
extern struct DuelPlayerView gUnk_020192E4_view[2] asm("gUnk_020192E4");

/* Hand cards of a player (0x02019968 = player block + 0x684): card words. */
struct DuelHand {
    u32 cards[0xD64 / 4];
};
extern struct DuelHand gUnk_02019968[2];

/* Duel action lists at 0x02017A40: two lists of CardRef-shaped entries; +0x3E4 = step of the prompts below. */
struct ActLists {
    struct CardRef listA[32];   /* +0x000 */
    struct CardRef listB[16];   /* +0x280 */
    u16 countB;                 /* +0x3C0 */
    u16 pad3C2;
    u16 countA;                 /* +0x3C4 */
    u8 filler3C6[0x56C - 0x3C6];
};
extern struct ActLists gUnk_02017A40;

u16 sub_0802B1B8(u16 id, int player, int zone);
void sub_08017FF4(int player, int zone);
void sub_080602A4(u16 a, u16 b, int c, const char *text);
int sub_08052F38(u32 mask);
void sub_08077AEC(u16 id);
void sub_08022758(int player, int a, int b, int c);
void sub_080227CC(int player, int a);
void sub_08022784(int player, int a, int b);
void sub_080193D4(int player, int index, int a, int b);
void sub_080753F4(char *dst, const char *fmt, const char *arg);
extern const u16 gUnk_08623E66[];
extern const u8 gUnk_0822C720[];
extern const char gUnk_080827EC[];
extern const char gUnk_08082810[];
int sub_08007590(u16 number, int a);
int sub_08047058(u16 id);
int sub_0800966C(u16 id);

/* Per-card target rule table at 0x0819A9D4 (0x18-byte entries, indexed by sub_08047058(card id)). */
struct TargetRule {
    u32 unk0;
    u32 unk4;
    u16 (*checkZone)(struct CardRef *ref, u16 pos);                         /* +0x08: tried on every (player, zone) */
    u16 (*prepare)(struct CardRef *ref, struct CardRef *other, u16 x);      /* +0x0C: must accept first */
    u32 unk10;
    u32 unk14;
};
extern struct TargetRule gUnk_0819A9D4[];
void *sub_08075294(void *dst, const void *src, int n);
int sub_08008C94(int player, int id);
int sub_08008524(int player, u16 number);
int sub_0802CD28(u16 id);
int sub_08047170(int player);
int sub_08008A1C(int player);
u16 sub_0802D058(int player, int zone);
int sub_08044224(int player, int a, int b);
int sub_0802B9EC(struct CardRef *ref, u16 pos);
int sub_08008B70(int a, int b, int c, int d);
int sub_080088A4(int player, int a, int b);
int sub_0800A430(int player, int zone);
int sub_08008C6C(int player);

/* Output of sub_0800ABC8 (0xC bytes): card id, type | attribute << 5, and a value at +4. */
struct CardInfo {
    u16 id;
    u8 typeAttr;        /* +0x02: bits 5-7 = attribute */
    u8 unk3;
    int value;          /* +0x04 */
    int unk8;
};
void sub_0800ABC8(int player, int index, struct CardInfo *out);

int sub_0802CAE8(struct CardRef *ref)
{
    struct DuelPlayer *base = gUnk_020192E4;
    u8 *pl = (u8 *)&base[ref->player];

    pl[9] |= 0x10;
    return 1;
}

int sub_0802CB0C(struct CardRef *ref)
{
    char buf[256];
    u8 *base = (u8 *)&gUnk_02017A40;
    u8 *step = base + 0x3E4;

    switch (*step) {
    case 0: {
        const char *fmt = gUnk_080827EC;

        sub_080753F4(buf, fmt, (const char *)gUnk_0822C720 + gUnk_08623E66[0] * 0x40);
        sub_080602A4(0x206, 0x712, 0xB, fmt);
        (*step)++;
        return 0;
    }
    case 1:
        if (sub_08052F38(0xE0)) {
            int player = gUnk_0201CFB0.player;
            int zone = gUnk_0201CFB0.cursor;
            int t = player & 1;
            struct DuelZone *z = ZB(t, zone);
            int id = CARD_ID(CARD_WORD(z->card));

            if (id && (ZFLAGS(z) & 2) && CARD_NUMBER(id) == 0x39) {
                sub_08017FF4(player, zone);
                return 1;
            }
            sub_08077AEC(3);
        }
        return 0;
    }
    return 0;
}
int sub_0802CBEC(struct CardRef *ref)
{
    u8 *base = (u8 *)&gUnk_02017A40;
    u8 *step = base + 0x3E4;

    switch (*step) {
    case 0:
        sub_080602A4(0x206, 0x712, 0xB, gUnk_08082810);
        (*step)++;
        return 0;
    case 1:
        if (sub_08052F38(1)) {
            int player = gUnk_0201CFB0.player;
            int idx = gUnk_0201CFB0.cursor;
            int t = player & 1;

            if (CARD_TYPE(CARD_ID(*(u32 *)((u32)gUnk_02019968 + idx * 4 + t * 0xD64))) == 22) {
                sub_080193D4(player, idx, 0, 1);
                return 1;
            }
            sub_08077AEC(3);
        }
        return 0;
    }
    return 0;
}
int sub_0802CCA0(struct CardRef *ref)
{
    sub_08022758(ref->player, 1, 1, 0);
    return 1;
}
int sub_0802CCB8(struct CardRef *ref)
{
    u8 *base = (u8 *)&gUnk_02017A40;
    u8 *step = base + 0x3E4;

    if (*step == 0) {
        sub_080227CC(ref->player, 2);
        (*step)++;
        return 0;
    }
    return 1;
}
int sub_0802CCF0(struct CardRef *ref)
{
    u8 *base = (u8 *)&gUnk_02017A40;
    u8 *step = base + 0x3E4;

    if (*step == 0) {
        sub_08022784(ref->player, 0, 1);
        (*step)++;
        return 0;
    }
    return 1;
}
static inline int CardKindOf(u16 id)
{
    int v;

    switch (CARD_NUMBER(id)) {
    case 0x776:
        v = 3;
        break;
    case 0x777:
    case 0x778:
        v = 1;
        break;
    default:
        switch ((int)CARD_TYPE(id)) {
        case 22:
            v = 7;
            break;
        case 21:
            v = 8;
            break;
        case 23:
            v = 9;
            break;
        default:
            v = (CARD_STATS(id) & 0xC0000) >> 18;
            break;
        }
        break;
    }
    return v;
}

int sub_0802CD28(u16 id)
{
    u16 number = CARD_NUMBER(id);
    int type = CARD_TYPE(id);
    int v = CardKindOf(id);
    int w;
    u32 stats = CARD_STATS(id);
    int t2 = (stats & 0x1F00000) >> 20;

    switch (t2) {
    case 21:
    case 22:
        w = (stats & 0xE0000) >> 17;
        break;
    default:
        w = 0;
        break;
    }
    switch (type) {
    case 21:
        if (w == 1)
            return 3;
        return 2;
    case 22:
        if (w == 5)
            return 2;
        return 1;
    default:
        if (v == 1) {
            if (sub_08007590(number, 0))
                return 2;
            return 1;
        }
        return 0;
    }
}

/* This predicate returns a full-word 0 or 1 to the AI callers. */
int sub_0802CE38(struct CardRef *ref, struct CardRef *other, u16 x)
{
    int idx = sub_08047058(ref->id);
    u16 (*check)(struct CardRef *, u16);
    u16 (*prepare)(struct CardRef *, struct CardRef *, u16);
    int i;
    int j;

    if (sub_0800966C(ref->id))
        return 0;
    if (other) {
        if (sub_0802CD28(ref->id) < sub_0802CD28(other->id))
            return 0;
        if (sub_0802CD28(ref->id) == 1)
            return 0;
    }
    if (ref->unk2_10 == 0x11) {
        switch (CARD_NUMBER(ref->id)) {
        case 0x28A:
        case 0x291:
        case 0x2B0:
        case 0x3F7:
        case 0x3F8:
        case 0x433:
        case 0x434:
        case 0x43A:
        case 0x44B:
        case 0x49A:
        case 0x4B3:
        case 0x4B4:
        case 0x522:
        case 0x587:
        case 0x5FE:
            break;
        default:
            return 0;
        }
    }
    if (idx < 0)
        return 0;
    check = gUnk_0819A9D4[idx].checkZone;
    prepare = gUnk_0819A9D4[idx].prepare;
    if (check == 0 && prepare == 0)
        return 1;
    if (prepare != 0 && !prepare(ref, other, x))
        return 0;
    if (check == 0)
        return 1;
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 10; j++) {
            if (check(ref, (u8)i | ((u8)j << 8)))
                return 1;
        }
    }
    return 0;
}


/* The ROM returns the zero-extended low halfword in the full r0 register.
 * Keep the explicit narrowing while agreeing with the existing int callers. */
int sub_0802CFA0(int player, u16 id, u16 x)
{
    struct CardRef ref;

    ref.player = player;
    ref.id = id;
    return (u16)sub_0802CE38(&ref, 0, x);
}
u16 sub_0802CFD0(int player, int zone, u16 kind)
{
    struct CardRef ref;
    int p;
    register u16 *idp __asm__("r5");

    ref.player = player;
    idp = &ref.id;
    /* FAKEMATCH: preserve the r5 store address before its right-hand
     * side; keep the initialized player argument for a separate mask. */
    __asm__("" : "+r"(player));
    p = player & 1;
    *idp = CARD_ID(CARD_WORD(ZB2(p, zone)->card));
    ref.zone = zone;
    ref.unk2_10 = kind;
    if (ref.id == 0)
        return 0;
    return sub_0802CE38(&ref, 0, 0);
}
#if 0 /* NONMATCHING: the loops match structurally (list B at +0x282 via a base pointer with an
       * unfolded offset, list A at base+2). Only register allocation differs: the ROM keeps
       * the base pointer in r1 and copies it to r7, while we keep it in r6 and copy to r1. */
u16 sub_0802D058(int player, int zone)
{
    int i;
    struct RefFlags *q;
    u8 *base = (u8 *)&gUnk_02017A40;
    u8 *p;

    p = base;
    for (i = 0; i < *(u16 *)(base + 0x3C0); i++, p += 0x14) {
        struct RefFlags *e = (struct RefFlags *)(p + 0x282);

        if (e->player == player && e->zone == zone)
            return 1;
    }
    q = (struct RefFlags *)(base + 2);
    for (i = 0; i < *(u16 *)(base + 0x3C4); i++, q = (struct RefFlags *)((u8 *)q + 0x14)) {
        if (q->player == player && q->zone == zone)
            return 1;
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0802CAE8", sub_0802D058); /* 0x0802D058 size 0x88 */
u16 sub_0802D0E0(struct CardRef *ref, int player, int zone)
{
    struct CardRef tmp;
    int p = player & 1;
    struct DuelZone *z = ZB(p, zone);
    int id = CARD_ID(CARD_WORD(z->card));
    int faceUp = z->flag6_1;
    int p2;
    struct DuelZone *z2;

    sub_08075294(&tmp, ref, sizeof(struct CardRef));
    tmp.player = player;
    tmp.id = id;
    if (id == 0 || CARD_TYPE(id) <= 20 || sub_0802CD28(id) < sub_0802CD28(ref->id) || sub_0802D058(player, zone))
        return 0;
    switch (CARD_NUMBER(id)) {
    case 0x52C:
    case 0x3F9:
    case 0x594:
    case 0x5FC:
        faceUp = 0;
        break;
    }
    if (faceUp)
        return 0;
    p2 = player & 1;
    z2 = ZB(p2, zone);
    if (!(((u8 *)z2)[0x91] & 4))
        return 0;
    if (((u8 *)z2)[0x91] & 8)
        return 0;
    if (CARD_TYPE(id) == 21 && (sub_08008524(0, 0x2EF) || sub_08008524(1, 0x2EF)))
        return 0;
    return sub_0802CE38(&tmp, ref, 0);
}

u16 sub_0802D25C(struct CardRef *ref, int player, int idx)
{
    struct CardRef tmp;
    int id = CARD_ID(*(u32 *)((u32)gUnk_02019968 + idx * 4 + (player & 1) * 0xD64));
    int num = 0x7FF & id;
    u32 stats;

    sub_08075294(&tmp, ref, sizeof(struct CardRef));
    tmp.player = player;
    tmp.id = id;
    stats = ((const u32 *)0x08621DE0)[num];
    if (((stats & 0x1F00000) >> 20) == 22 && ((stats & 0xE0000) >> 17) == 5
        && sub_0802CD28(id) >= sub_0802CD28(ref->id) && sub_08008C94(player, id))
        return sub_0802CE38(&tmp, ref, 0);
    return 0;
}
int sub_0802D30C(struct CardRef *ref, int player)
{
    int i;
    int t;

    for (i = 5; i <= 9; i++) {
        if (sub_0802D0E0(ref, player, i))
            return 1;
    }
    if (player == gUnk_020192E0.linkSkip) {
        struct DuelPlayer *pp;

        i = 0;
        pp = gUnk_020192E0.players;
        t = player & 1;
        for (; i < pp[t].handCount; i++) {
            if (sub_0802D25C(ref, player, i))
                return 1;
        }
        return 0;
    }
    if (CARD_TYPE(ref->id) == 22) {
        int want;

        i = 0;
        t = player & 1;
        want = 0x5F5;
        for (; i <= 4; i++) {
            struct DuelZone *z = ZB(t, i);
            u16 id = CARD_ID(CARD_WORD(z->card));

            if ((ZFLAGS(z) & 2) && id && !sub_0802D058(player, i) && CARD_NUMBER(id) == want)
                return 1;
        }
    }
    return 0;
}
int sub_0802D414(struct CardRef *ref, int unused, u16 x)
{
    int i;

    if (x != 0)
        return 0;
    if (!(ZB2(ref->player, ref->zone)->unk7 & 0x20))
        return 0;
    for (i = 0; i <= 4; i++) {
        if (CARD_ID(CARD_WORD(ZB2((1 - ref->player) & 1, i)->card)))
            return 1;
    }
    return 0;
}
int sub_0802D47C(struct CardRef *ref, int unused, u16 x)
{
    if (x != 0 && !gUnk_020192E4_view[ref->player].flag8_4)
        return 1;
    return 0;
}
int sub_0802D4A8(struct CardRef *ref)
{
    int n;

    if (!sub_08047170(ref->player) || !sub_08008A1C(ref->player))
        return 0;
    n = sub_08008524(0, 0x3D) + sub_08008524(1, 0x3D) + sub_08008524(0, 0x4E1) + sub_08008524(1, 0x4E1);
    if (n == 0)
        return 0;
    return sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) > 0;
}
int sub_0802D53C(struct CardRef *ref)
{
    int i;
    int j;

    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            if (CARD_ID(CARD_WORD(ZB2(i & 1, j)->card)))
                return 1;
        }
    }
    return 0;
}
int sub_0802D580(struct CardRef *ref)
{
    int j;

    for (j = 0; j <= 4; j++) {
        if (CARD_ID(CARD_WORD(ZB2((1 - ref->player) & 1, j)->card)))
            return 1;
    }
    return 0;
}
int sub_0802D5C0(struct CardRef *ref)
{
    int i;

    for (i = 0; i <= 4; i++) {
        int p = (1 - ref->player) & 1;
        struct DuelZone *z = ZB(p, i);
        struct DuelZone *z2;

        if (CARD_ID(CARD_WORD(z->card))) {
            z2 = ZB((1 - ref->player) & 1, i);
            if (!(ZFLAGS(z2) & 2))
                return 1;
        }
    }
    return 0;
}
int sub_0802D620(struct CardRef *ref)
{
    if (gUnk_020192E4[ref->player].lifePoints <= 999)
        return 0;
    return sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) > 0;
}
int sub_0802D67C(struct CardRef *ref, int unused, u16 x)
{
    int count;
    int i;
    int j;

    if (x != 0)
        return 0;
    if (ref->unk2_10 != 2)
        return 0;
    count = 0;
    for (i = 0; i <= 1; i++) {
        for (j = 0; j <= 4; j++) {
            if (sub_0802B9EC(ref, (u8)i | ((u8)j << 8))) {
                count++;
                if (count > 1)
                    return 1;
            }
        }
    }
    return 0;
}
int sub_0802D6D4(struct CardRef *ref)
{
    int result;

    switch (CARD_NUMBER(ref->id)) {
    default:
    ret0:
        return 0;
    case 0x1A3:
    {
        struct DuelPlayer *players = gUnk_020192E4;
        register u32 shifted __asm__("r3") = (u32)((u8 *)ref)[2] << 31;
        int one = 1;

        /* FAKEMATCH: preserve the initialized mask separately and keep
         * the raw shifted player for the ROM's repeated extractions. */
        __asm__("" : "+r"(one) : "r"(shifted));
        if (players[shifted >> 31].lifePoints <= 4999)
            goto ret0;
        __asm__("" : : "r"(shifted));
        if (players[one & (shifted >> 31)].fusionCount == 0)
            goto ret0;
        /* FAKEMATCH: keep this extraction after the count comparison. */
        __asm__("" : "+r"(shifted));
        if (!sub_08047170(shifted >> 31))
            goto ret0;
        result = sub_08008A1C(ref->player);
        break;
    }
    case 0x1F9:
    {
        struct DuelPlayer *players = gUnk_020192E4;
        register u32 shifted __asm__("r3") = (u32)((u8 *)ref)[2] << 31;
        int one = 1;

        /* FAKEMATCH: the same initialized mask/player lifetimes as above. */
        __asm__("" : "+r"(one) : "r"(shifted));
        if (players[shifted >> 31].lifePoints <= 2999)
            goto ret0;
        __asm__("" : : "r"(shifted));
        result = players[one & (shifted >> 31)].fusionCount;
        __asm__("" : : "r"(shifted));
        break;
    }
    }
    if (result == 0)
        goto ret0;
    return 1;
}
int sub_0802D78C(struct CardRef *ref, int unused, u16 x)
{
    if (x == 0)
        return 0;
    return sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) > 0;
}
int sub_0802D7CC(struct CardRef *ref, int unused, u16 x)
{
    if (x != 0)
        return 0;
    return ((u32)ZB2(ref->player, ref->zone)->unk7 << 26) >> 31;
}
int sub_0802D800(struct CardRef *ref)
{
    int a;

    a = sub_08008B70(0, 0, 1, 1);
    a += sub_08008B70(1, 0, 1, 1);
    return a > 1;
}
int sub_0802D82C(struct CardRef *ref, int unused, u16 x)
{
    if (x != 0)
        return 0;
    if (ref->unk2_10 == 0x10) {
        u8 lo = ref->unk6;
        int zone = ref->unk6 >> 8;
        int one = 1;
        int p = lo & one;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && !(one & ZFLAGS(z)) && ref->player != lo)
            return 1;
    }
    if (sub_080088A4(ref->player, 1, 0) > 0)
        return 1;
    return 0;
}
int sub_0802D89C(struct CardRef *ref)
{
    struct CardInfo info;
    int i;

    if (sub_08008524(0, 0x58A) > 0 || sub_08008524(1, 0x58A) > 0)
        return 0;
    for (i = 0; i <= 4; i++) {
        struct DuelZonesPlayer *pz = &gUnk_0201930C[ref->player];
        struct DuelZone *z = &pz->zones[i];
        u16 id = CARD_ID(CARD_WORD(z->card));

        if (id && CARD_NUMBER(id) <= 0x76B && CARD_TYPE(id) <= 20) {
            sub_0800ABC8(ref->player, i, &info);
            if (info.value <= 1000 && (info.typeAttr & 0xE0) == 0x40)
                return 1;
        }
    }
    return 0;
}
int sub_0802D958(struct CardRef *ref)
{
    return sub_08008B70(1 - ref->player, 0, 0, 1) > 0;
}
int sub_0802D97C(struct CardRef *ref, struct CardRef *other, u16 x)
{
    int i;

    if (x != 0)
        return 0;
    if (other == 0)
        return 0;
    if (other->player == ref->player)
        return 0;
    switch (CARD_NUMBER(other->id)) {
    case 0x53:
    case 0xDF:
    case 0x3EC:
    case 0x437:
    case 0x46E:
    case 0x46F: {
        u8 lo = other->unkC;
        int zone = other->unkC >> 8;
        int p = lo & 1;
        struct DuelZone *z = ZB(p, zone);
        int id = CARD_ID(CARD_WORD(z->card));

        if (id == 0)
            return 0;
        if (lo == ref->player && zone == ref->zone)
            return 0;
        if (CARD_TYPE(id) != 21)
            return 0;
        return 1;
    }
    case 0x29F:
    case 0x425:
    case 0x426:
    case 0x42B:
        for (i = 5; i <= 9; i++) {
            int p = ref->player;
            struct DuelZone *z = ZB(p, i);
            u16 id = CARD_ID(CARD_WORD(z->card));

            if (id && i != ref->zone && CARD_TYPE(id) == 21)
                return 1;
        }
        break;
    }
    return 0;
}
int sub_0802DAD4(struct CardRef *ref, int unused, u16 x)
{
    int player = ref->player;
    int zone = ref->zone;

    if (x == 0 && sub_0800A430(player, zone) == 0xFFFF && sub_08008C6C(player) != -1)
        return ((u32)ZB2(player, zone)->unk7 << 26) >> 31;
    return 0;
}
