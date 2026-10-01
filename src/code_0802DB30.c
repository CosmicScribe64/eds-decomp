#include "global.h"
#include "duel.h"

/*
 * Duel effect-condition predicates, each taking (struct CardRef *ref, ?, u16 flag) and returning bool.
 * See wiki/functions/code-0802db30.md.
 * Uses the shared duel-state layouts from include/duel.h (struct DuelCard, DuelZone,
 * DuelPlayer, DuelState, DuelZonesPlayer).
 */

/*
 * Canonical struct DuelZone (duel.h) declares +0x07 as a plain `u8 unk7`, but sub_0802DBA8
 * reads its bit 5 as a 0/1 bitfield (ROM: ldrb [+7]; lsls #26; lsrs #31). Keep a one-byte
 * local view for that function.
 */
struct DuelZoneFlag7 {
    u8 pad[7];
    u8 unk7_0 : 5;
    u8 flag7_5 : 1;         /* +0x07 bit 5 */
    u8 unk7_6 : 2;
};

/* Byte 6 of a zone as a plain byte (its flags are tested with mov #2; ldrb; and). */
#define ZFLAGS(z) (((u8 *)(z))[6])

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
    u8 fillerA[0x14 - 0xA];
};

/* The card word read as a whole u32 (the code always loads it with ldr). */
#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_SUBTYPE(id) ((CARD_STATS(id) & 0xE0000) >> 17)
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)

extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */
extern const u16 gUnk_08622AB4[];   /* card ID to card number */
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
int sub_0802F808(int a, int player, struct CardRef *ref);
int sub_08008668(int player);


int sub_0802DB30(struct CardRef *ref, int a, u16 flag)
{
    if (flag == 0 && ref->kind == 2 && sub_08008860(ref->player) > 1) {
        int p = ref->player;
        struct DuelZone *z = ZB(p, ref->zone);

        if (!(((u8 *)z)[7] & 0x20) && sub_08008524(0, 0x58A) <= 0 && sub_08008524(1, 0x58A) <= 0)
            return 1;
    }
    return 0;
}

int sub_0802DBA8(struct CardRef *ref, int a, u16 flag)
{
    if (flag != 0)
        return 0;
    {
        int p = ref->player;
        struct DuelZone *z = ZB(p, ref->zone);

        return ((struct DuelZoneFlag7 *)z)->flag7_5;
    }
}

int sub_0802DBDC(struct CardRef *ref, int a, u16 flag)
{
    if (flag == 0 && sub_08008A1C(ref->player) > 1 && sub_08008524(0, 0x58A) <= 0
        && sub_08008524(1, 0x58A) <= 0 && sub_08009CAC(ref->player, 0x2E1) != 0
        && sub_08009CAC(ref->player, 0x2F4) != 0 && sub_08009CAC(ref->player, 0x320) != 0)
        return 1;
    return 0;
}

int sub_0802DC58(struct CardRef *ref, int a, u16 flag)
{
    if (flag == 0 && ref->kind == 0x13)
        return 1;
    return 0;
}

int sub_0802DC74(struct CardRef *ref, int a)
{
    int ret = 0;

    if (gUnk_020192E4[ref->player].lifePoints > 999)
        ret = 1;
    return ret;
}

int sub_0802DCA0(struct CardRef *ref)
{
    struct DuelPlayer *pl = gUnk_020192E4;
    int p = ref->player;

    p ^= 1;
    return pl[p].handCount != 0;
}

int sub_0802DCC4(struct CardRef *ref)
{
    int i;

    if (sub_08008A1C(ref->player) != 0 && sub_08047170(ref->player) != 0
        && (sub_080086CC(0, 0x2E4) > 0 || sub_080086CC(1, 0x2E4) > 0)) {
        for (i = 0; i < gUnk_020192E4[ref->player].handCount; i++) {
            u32 id = CARD_ID11(CARD_WORD(gUnk_020192E4[ref->player].hand[i]));

            if (CARD_TYPE(id) == 1
                && (sub_0800756C(CARD_NUMBER(id)) == 0 || sub_08008668(ref->player) != 0))
                return 1;
        }
    }
    return 0;
}

int sub_0802DD98(struct CardRef *ref)
{
    int player = (u8)ref->pos;
    int zone = ref->pos >> 8;
    int p;
    struct DuelZone *z;

    switch (ref->kind) {
    case 5:
    case 6:
    case 7:
        break;
    default:
        return 0;
    }
    p = player & 1;
    z = ZB(p, zone);
    if (!CARD_ID(CARD_WORD(z->card)) || !(ZFLAGS(z) & 2))
        return 0;
    return sub_0800C894(player, zone) <= 2000;
}


int sub_0802DDFC(struct CardRef *ref)
{
    int player = (u8)ref->pos;
    int zone = ref->pos >> 8;

    if (ref->kind == 5 || ref->kind == 6) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && player != ref->player) {
            switch (CARD_NUMBER(ref->id)) {
            case 0x3EA:
                return sub_0800C894(player, zone) > 999;
            case 0x2A8:
                return sub_0800C8A8(player, zone) <= 500;
            case 0x2A9:
                return sub_0800C894(player, zone) <= 500;
            }
        }
    }
    return 0;
}

int sub_0802DEC4(struct CardRef *ref)
{
    u8 buf[8];
    int i;

    if (sub_08047170(ref->player) == 0)
        return 0;
    for (i = 0; i < gUnk_020192E4[1 & ref->player].fusionCount; i++) {
        int p = ref->player;
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1 & ref->player].fusionDeck[i]));

        if (sub_0803D0E8(p, id, buf) != 0)
            return 1;
    }
    return 0;
}

int sub_0802DF5C(struct CardRef *ref)
{
    if (sub_08008860(ref->player) > 1 && sub_08008860(1 - ref->player) > 0)
        return 1;
    return 0;
}

/* The AI uses the three-argument usability interface; this test needs only ref. */
int sub_0802DF8C(struct CardRef *ref, int action, int flags)
{
    if (sub_08008A1C(ref->player) == 0 || sub_08047170(ref->player) == 0 || sub_080090C8(0, 0x402) > 0
        || sub_080090C8(1, 0x402) > 0)
        return 0;
    return sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) > 0;
}

int sub_0802E004(struct CardRef *ref)
{
    if (sub_08008524(1 - ref->player, 0x5E7) > 0)
        return 0;
    return sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) > 0;
}

int sub_0802E058(struct CardRef *ref, int a, u16 flag)
{
    int i;

    if (flag != 0 || gUnk_020192E4[1 & ref->player].lifePoints <= 499 || sub_08047114(ref->player) == 0)
        return 0;
    for (i = 0; i < gUnk_020192E4[1 & ref->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1 & ref->player].hand[i]));

        if (sub_08054398(ref->player, id) != 0 && sub_08007834(id) == 0)
            return 1;
    }
    return 0;
}
int sub_0802E114(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag != 0)
        return 0;
    if (tgt == 0)
        return 0;
    if ((1 & ((u8 *)tgt)[2]) == (1 & ((u8 *)ref)[2]))
        return 0;
    switch (CARD_NUMBER(ref->id)) {
    case 0x3FB:
        return CARD_NUMBER(tgt->id) == 0x14F;
    case 0x3FD:
        return CARD_NUMBER(tgt->id) == 0x3F0;
    case 0x3FE:
        return CARD_NUMBER(tgt->id) == 0x150;
    case 0x426:
        return CARD_NUMBER(tgt->id) == 0x29F;
    case 0x4DF:
        switch (CARD_NUMBER(tgt->id)) {
        case 0x151:
        case 0x152:
        case 0x153:
        case 0x154:
        case 0x155:
        case 0x156:
        case 0x157:
        case 0x158:
        case 0x159:
        case 0x3C8:
        case 0x3EE:
        case 0x3EF:
        case 0x3F2:
        case 0x401:
        case 0x40F:
        case 0x42E:
        case 0x42F:
        case 0x439:
            return 1;
        default:
            return 0;
        }
        break;
    case 0x5FB:
        switch (CARD_NUMBER(tgt->id)) {
        case 0x12C:
        case 0x12D:
        case 0x12E:
        case 0x12F:
        case 0x130:
        case 0x131:
        case 0x132:
        case 0x133:
        case 0x134:
        case 0x135:
        case 0x136:
        case 0x137:
        case 0x138:
        case 0x139:
        case 0x13A:
        case 0x13B:
        case 0x13C:
        case 0x13E:
        case 0x13F:
        case 0x140:
        case 0x141:
        case 0x142:
        case 0x143:
        case 0x144:
        case 0x145:
        case 0x146:
        case 0x147:
        case 0x28B:
        case 0x28D:
        case 0x290:
        case 0x3C2:
        case 0x3F0:
        case 0x3F4:
        case 0x3F5:
        case 0x3FF:
        case 0x403:
        case 0x412:
        case 0x413:
        case 0x416:
        case 0x417:
        case 0x422:
        case 0x424:
        case 0x42C:
        case 0x430:
        case 0x433:
        case 0x434:
        case 0x485:
        case 0x488:
        case 0x49E:
        case 0x4BB:
        case 0x4C4:
        case 0x521:
        case 0x58B:
        case 0x58C:
        case 0x58E:
        case 0x5A8:
        case 0x5A9:
        case 0x5AA:
        case 0x5AB:
        case 0x604:
        case 0x60A:
        case 0x60C:
        case 0x60E:
            return 1;
        default:
            return 0;
        }
        break;
    }
    return 0;
}

int sub_0802E3C8(struct CardRef *ref)
{
    if (gUnk_020192E4[ref->player].handCount == 0)
        return 0;
    return 1;
}

int sub_0802E3EC(struct CardRef *ref)
{
    if (sub_08008524(1 - ref->player, 0x5E7) > 0)
        return 0;
    if (gUnk_020192E4[0].graveCount == 0 && gUnk_020192E4[1].graveCount == 0)
        return 0;
    return 1;
}

int sub_0802E42C(struct CardRef *ref)
{
    int i;

    for (i = 0; i < gUnk_020192E4[1 & (int)ref->player].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1 & (int)ref->player].hand[i]));

        if (CARD_TYPE(id) <= 0x14)
            return 1;
    }
    return 0;
}

int sub_0802E4B0(struct CardRef *ref)
{
    if (sub_08008860(1 - ref->player) != 0 && sub_08008A1C(ref->player) != 0)
        return 1;
    return 0;
}

int sub_0802E4E0(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag == 0) {
        if (tgt == 0) {
            int p = ref->pos & 1;
            int zone = ref->pos >> 8;
            struct DuelZone *z = ZB(p, zone);

            if (CARD_ID(CARD_WORD(z->card)) != 0) {
                switch (ref->kind) {
                case 5:
                case 6:
                case 7:
                    return 1;
                }
            }
        } else {
            u16 id = tgt->id & 0x7FF;

            switch ((int)CARD_TYPE(id)) {
            case 0x15:
            case 0x16:
                if (CARD_NUMBER(id) != 0x603)
                    return 1;
            }
        }
    }
    return 0;
}

int sub_0802E564(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag == 0 && tgt != 0) {
        u32 id = tgt->id & 0x7FF;

        if (CARD_TYPE(id) == 0x16 && gUnk_020192E4[ref->player].handCount != 0 && CARD_NUMBER(id) != 0x603)
            return 1;
    }
    return 0;
}

int sub_0802E5D4(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag == 0 && tgt != 0 && CARD_TYPE(tgt->id) == 0x15 && gUnk_020192E4[ref->player].lifePoints > 999)
        return 1;
    return 0;
}

int sub_0802E62C(struct CardRef *ref, int a, u16 flag)
{
    int player = (u8)ref->pos;
    int zone = ref->pos >> 8;

    if (flag == 0) {
        int p = player & 1;
        struct DuelZone *z = ZB(p, zone);

        if (CARD_ID(CARD_WORD(z->card)) != 0) {
            if (player == ref->player && sub_08008AF8(ref->player, zone) == 0)
                return 0;
            if (player != ref->player && sub_08008AF8(ref->player, -1) == 0)
                return 0;
            switch (ref->kind) {
            case 5:
            case 6:
            case 7:
                return 1;
            }
        }
    }
    return 0;
}

int sub_0802E6A0(struct CardRef *ref)
{
    struct DuelPlayer *pl = gUnk_020192E4;
    int p = ref->player;

    p ^= 1;
    return pl[p].handCount != 0;
}

int sub_0802E6C4(struct CardRef *ref)
{
    return sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) > 0;
}

int sub_0802E6F8(struct CardRef *ref)
{
    if (sub_08047170(ref->player) != 0) {
        struct DuelPlayer *players = gUnk_020192E4;
        u32 shifted = (u32)((u8 *)ref)[2] << 31;

        if ((s32)((u32)((u8 *)&players[shifted >> 31])[0xB] << 28) < 0) {
            /* FAKEMATCH: re-extract the player bit for the call as the ROM does. */
            __asm__("" : "+r"(shifted));
            if (sub_08008A1C(shifted >> 31) != 0 && sub_08044224(ref->player, 0x41E, 0) != 0)
                return 1;
        }
    }
    return 0;
}

int sub_0802E754(struct CardRef *ref, int a, u16 flag)
{
    if (flag == 0 && gUnk_020192E0.linkSkip != ref->player)
        return 1;
    return 0;
}

int sub_0802E784(struct CardRef *ref, int a, u16 flag)
{
    u16 number;
    int k;

    sub_08008860(ref->player);
    sub_08008860(1 - ref->player);
    if (flag != 0)
        return 0;
    if ((u8)ref->pos == ref->player)
        return 0;
    if (ref->kind != 0x10)
        return 0;
    number = CARD_NUMBER(ref->id);
    switch (number) {
    case 0x3C0:
    case 0x44A:
    case 0x4BE:
    case 0x587:
    case 0x590:
        return 1;
    case 0x2AD:
    case 0x420:
        for (k = 0; k <= 9; k++) {
            int p = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(p, k);

            if (CARD_ID(CARD_WORD(z->card)) != 0) {
                int p2 = (1 - ref->player) & 1;
                struct DuelZone *z2 = ZB(p2, k);

                if (!(ZFLAGS(z2) & 1))
                    return 1;
            }
        }
        return 0;
    case 0x3B1:
        if ((ref->unk8 >> 8) > 4)
            return 0;
        if (sub_08008860(ref->player) == 0)
            return 0;
        if (sub_08008A1C(ref->player) <= 1)
            return 0;
        if (sub_08044224(ref->player, number, 0) <= 1)
            return 0;
        return 1;
    case 0x3DE:
        if ((ref->unk8 >> 8) > 4)
            return 0;
        if (sub_08008860(ref->player) == 0)
            return 0;
        if (sub_08008A1C(ref->player) == 0)
            return 0;
        if (sub_080088A4(1 - ref->player, 1, 0) <= 1)
            return 0;
        return 1;
    }
    return 0;
}

int sub_0802E900(struct CardRef *ref)
{
    if (sub_08008AF8(ref->player, -1) != 0 && sub_08008AF8(1 - ref->player, -1) != 0)
        return 1;
    return 0;
}

int sub_0802E938(struct CardRef *ref)
{
    if (gUnk_020192E0.phase1B12 != 1 || (sub_08008860(ref->player) <= 0 && sub_08008860(1 - ref->player) <= 0))
        return 0;
    return 1;
}

int sub_0802E980(struct CardRef *ref, int a, u16 n)
{
    int i, j;

    if (gUnk_020192E4[ref->player].handCount - n <= 4)
        return 0;
    for (i = 0; i < 2; i++) {
        for (j = 0; j <= 10; j++) {
            if (CARD_ID(CARD_WORD(gUnk_020192E4[i & 1].zones[j].card)) != 0)
                return 1;
        }
    }
    return 0;
}

int sub_0802E9E0(struct CardRef *ref, int a, u16 n)
{
    if (gUnk_020192E4[ref->player].handCount - n <= 1)
        return 0;
    return 1;
}

int sub_0802EA0C(struct CardRef *ref, struct CardRef *tgt, u16 flag)
{
    if (flag != 0 || tgt == 0 || (((u8 *)tgt)[2] & 1) == (((u8 *)ref)[2] & 1))
        return 0;
    return sub_0802F808(0x431, ref->player, tgt) > 0;
}

int sub_0802EA50(struct CardRef *ref)
{
    int ret = 0;

    if (gUnk_020192E4[ref->player].deckCount > 4)
        ret = 1;
    return ret;
}

int sub_0802EA74(struct CardRef *ref)
{
    int i;

    for (i = 0; i < gUnk_020192E4[(1 - ref->player) & 1].graveCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[(1 - ref->player) & 1].graveyard[i]));

        if (CARD_TYPE(id) == 0x16)
            return 1;
    }
    return 0;
}

int sub_0802EB00(struct CardRef *ref)
{
    if (sub_08047170(ref->player) == 0 || sub_08008A1C(ref->player) == 0)
        return 0;
    return sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) > 0;
}
