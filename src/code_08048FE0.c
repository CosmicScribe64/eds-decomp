#include "global.h"

extern u8 gUnk_020192E0[];
extern u8 gUnk_02017A40[];
extern const u16 gUnk_08624A0A[];
void sub_08055EB0(int player, int zone);
void sub_08018ED8(int a, int b, int c, int d);
extern const u32 gUnk_08621DE0[];
extern u8 gUnk_0201930C[];
int sub_0802CFA0(int player, u16 id, u16 x);
int sub_0802CFD0(int player, int zone, u16 kind);
int sub_08008AF8(int player, int exclude);
u16 sub_0800A430(int player, int zone);
int sub_08008C6C(int player);
u16 sub_0802B1B8(u16 id, int player, int zone);
int sub_08008524(int player, u16 number);
int sub_0800A78C(int player, int zone, u16 number);
int sub_08008C94(int player, u16 id);
u32 sub_08007834(u16 id);
int sub_08054398(int player, u16 id);
int sub_080086CC(int player, u16 cardNo);
u16 sub_08008860(int player);
int sub_08008A1C(int player);
int sub_08047170(int player);
int sub_08047114(int player);
void sub_08024134(int player, int a, int zone);
void sub_08018544(int player, int a, int b);
void sub_080197E0(int player, u16 id);
void sub_08019860(int player, int lp);
void sub_0801FBE0(u32 event, u32 value);
void sub_0801FBCC(u32 event, int value);
void sub_0801EC58(u16 msg, u16 card, u16 packed, u16 zero);
void sub_08046A74(int player);
extern u8 gUnk_020198D4[];
extern u16 gUnk_0862467A;
/* Field zone (0x94 bytes) with the fields used by the usability test. */
struct ZoneF { u8 pad0[6]; u8 b6; u8 pad7[0x91 - 7]; u8 b91; u8 pad92[0x94 - 0x92]; };
struct EB { u8 pad[0x310]; u8 zb[1]; };
struct ZoneG { u8 pad0[6]; u8 b6; u8 pad7[0x94 - 7]; };
struct ZonesF { struct ZoneF z[11]; u8 filler[0xD64 - 11 * 0x94]; };
struct PlB7 { u8 pad[7]; u8 b7; u8 rest[0xD64 - 8]; };
extern struct PlB7 gUnk_020192E4[];

/* Bit flags at 0x020192E0 + 0x1B2C..0x1B34 describing a pending zone request (hypothesis). */
struct ReqFlags { u8 b0 : 1; u8 b1 : 1; u8 rest : 6; u8 pad[7]; };           /* +0x1B2C */
struct ReqStep { u16 lo : 2; u16 cnt : 8; u16 hi : 6; u8 pad[6]; };          /* +0x1B30 */
struct ReqPlayer { u8 lo : 1; u8 player : 1; u8 hi : 6; u8 pad[6]; };  /* +0x1B33 */
struct ReqZone { u16 lo : 1; u16 zone : 8; u16 hi : 7; u8 pad[6]; };  /* +0x1B34 */
struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;
    u8 unk4_3 : 5;
    u8 unk5;
    u16 pos;
    u16 unk8;
    u8 numTargets : 3;
    u8 unkA_3 : 5;
    u8 unkB;
    u16 targets[3];
};
int sub_0803D57C(struct CardRef *ref, int a);

struct ZoneB7 { u8 pad[7]; u8 b7; u8 rest[0x94 - 8]; };
struct PlZones { struct ZoneB7 z[11]; u8 filler[0xD64 - 11 * 0x94]; };
#define REQ_PLAYER(e) (((struct ReqPlayer *)((e) + 0x1B33))->player)
#define REQ_ZONE(e) (((struct ReqZone *)((e) + 0x1B34))->zone)
#define ZONE_BYTE7(e, p, z) (*(u8 *)((z) * 0x94 + (p) * 0xD64 + ((e) + 0x2C) + 7))

/* --- sub_08049880 support --- */
#define CARD_NUM(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_STATS_N(n) (((const u32 *)0x08621DE0)[(n)])
#define CARD_NUM_N(n) (((const u16 *)0x08622AB4)[(n)])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define PLAYER_BASE(p) ((u8 *)gUnk_020192E4 + ((p) & 1) * 0xD64)
#define DECK_COUNT(p) (*(u8 *)(PLAYER_BASE(p) + 3))
#define DECK_WORD(p, i) (((u32 *)PLAYER_BASE(p))[0x7C4 / 4 + (i)])
#define LIST_COUNT(p) (*(u8 *)(PLAYER_BASE(p) + 4))
#define LIST_WORD(p, i) (((u32 *)PLAYER_BASE(p))[0x904 / 4 + (i)])
#define ZONE7(p, z) ((((u8 *)gUnk_0201930C)[(z) * 0x94 + ((p) & 1) * 0xD64 + 7] >> 5) & 1)
/* Second request zone field: bits 9-16 of the word at 0x020192E0+0x1B34 (sub_08049048). */
#define REQ_ZONE2(e) (((*(u32 *)((e) + 0x1B34)) << 15) >> 24)

/* Resolve the pending request: run sub_08055EB0 on its zone, mark the zone (byte +7 bit 2) and clear the request flag. */
void sub_08048FE0(void)
{
    u8 *e = gUnk_020192E0;
    struct ZoneB7 *z;
    u32 p, zi;
    int s1;
    u8 *zb;
    sub_08055EB0(REQ_PLAYER(e), REQ_ZONE(e));
    p = REQ_PLAYER(e);
    zi = REQ_ZONE(e);
    s1 = zi * 0x94 + p * 0xD64;
    zb = e + 0x2C;
    z = (struct ZoneB7 *)(s1 + (int)zb);
    z->b7 |= 4;
    ((struct ReqFlags *)(e + 0x1B2C))->b1 = 0;
}

#if 0 /* NONMATCHING: the ROM reloads 0x020192E0 and both card tables in every block and keeps arg0 in r8/arg1 in r9/arg2 in r6; the build keeps e in r6 and CSEs all of it, so the function is about 0xF0 bytes shorter. Control flow and all calls/literals are identical. */
/*
 * Request step driver on the counter at 0x020192E0+0x1B30 (bits 2-9). Step 0 picks a free
 * spell/trap zone (sub_08008C6C) into the second zone field (0x1B34 bits 9-16), handles trap
 * subtype 2, emits the magic/trap event and advances. Step 1 calls sub_08024134(player, 0, zone)
 * and advances. Other steps evaluate the requested zone/player (calls sub_080197E0/19860 on a
 * mismatch) and either feed a card reference to sub_0801FBE0/CC or, when arg0 is 0, just clear
 * the request flag (bit 1 of 0x1B2C).
 */
void sub_08049048(u16 arg0, u16 arg1, struct CardRef *arg2)
{
    u8 *e = gUnk_020192E0;
    u16 *step = (u16 *)(e + 0x1B30);
    u32 s = ((u32)*step << 22) >> 24;
    u32 player;
    u32 zone;
    u32 *zw;
    s16 i;

    if (s != 0) {
        if (s == 1) {
            sub_08024134(REQ_PLAYER(e), 0, REQ_ZONE2(e));
            *step = (*step & 0xFC03) | (((((u32)*step << 22) >> 24) + 1) & 0xFF) << 2;
            return;
        }
        if (arg0 != 0) {
            u8 *zb = e + 0x2C;

            player = REQ_PLAYER(e);
            zone = REQ_ZONE2(e);
            if ((*(u32 *)(zb + zone * 0x94 + player * 0xD64) << 13) < 0) {
                u32 w2 = *(u32 *)(zb + zone * 0x94 + (1 & player) * 0xD64);

                if (((w2 << 19) >> 31) != player) {
                    sub_080197E0(player, gUnk_0862467A);
                    sub_08019860(REQ_PLAYER(e), 0x7D0);
                }
            }
            player = REQ_PLAYER(e);
            zone = REQ_ZONE2(e);
            if (arg1 != 0) {
                u32 ev = (player << 31) | (((u32)((u8 *)arg2)[3] >> 2) << 25) |
                         ((zone & 0x1F) << 16) | *(u16 *)(e + 0x1B28);

                sub_0801FBE0(ev, (arg2->unk8 << 16) | arg2->pos);
            } else if (arg2 == NULL) {
                u32 ev = (player << 31) | ((zone & 0x1F) << 16) | *(u16 *)(e + 0x1B28);

                sub_0801FBCC(ev, 0);
            } else {
                u32 ev = (player << 31) | (((u32)((u8 *)arg2)[3] >> 2) << 25) |
                         ((zone & 0x1F) << 16) | *(u16 *)(e + 0x1B28);

                sub_0801FBCC(ev, (arg2->unk8 << 16) | arg2->pos);
            }
        }
        if (((e[0x1B12] << 27) >> 29) > 1) {
            u8 *pb = (u8 *)(e + 4) + REQ_PLAYER(e) * 0xD64;

            pb[9] |= 0x20;
        }
        ((struct ReqFlags *)(e + 0x1B2C))->b1 = 0;
        return;
    }

    /* step 0: pick a free zone, handle trap subtype 2, emit the event, advance */
    zw = (u32 *)(e + 0x1B34);
    *zw = (*zw & 0xFFFE01FF) | (((u32)sub_08008C6C(REQ_PLAYER(e)) & 0xFF) << 9);
    {
        s16 stats = CARD_STATS(*(u16 *)(e + 0x1B28));

        if (((stats & 0x1F00000) >> 20) == 0x16 && ((stats & 0xE0000) >> 17) == 2) {
            for (i = 0; i <= 1; i++) {
                s16 pp = (i != 0) ? 1 - REQ_PLAYER(e) : REQ_PLAYER(e);

                if ((*(u32 *)((u8 *)gUnk_020198D4 + (pp & 1) * 0xD64) << 20) != 0)
                    sub_08018544(pp, 0xA, 0);
            }
            {
                u32 *q = (u32 *)((u8 *)gUnk_020198D4 + 0x1540);

                *q = (*q & 0xFFFE01FF) | 0x1400;
            }
        }
    }
    {
        u16 msg = (e[0x1B33] & 2) ? 0x80C5 : 0xC5;
        s8 card = *(u16 *)(e + 0x1B28);
        int packed;

        zw = (u32 *)(e + 0x1B34);
        packed = (((*(u16 *)zw >> 1) & 0xF) << 4) | ((*zw >> 9) & 0xF) | ((arg0 & 1) << 8);
        sub_08046A74(REQ_PLAYER(e));
        *step = (*step & 0xFC03) | (((((u32)*step << 22) >> 24) + 1) & 0xFF) << 2;
        sub_0801EC58(msg, card, packed, 0);
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08048FE0", sub_08049048); /* size 0x388 */
/* Like sub_08048FE0 but with a step argument: for step 1 or 2 run sub_08018ED8 on the request zone first. */
void sub_080493D0(u16 step)
{
    u8 *e;
    struct ZoneB7 *z;
    u32 p, zi;
    int s1;
    u8 *zb;
    switch (step) {
    case 1:
    case 2: {
        u8 *e1 = gUnk_020192E0;
        sub_08018ED8(REQ_PLAYER(e1), REQ_ZONE(e1), 0, 0);
    }
    }
    e = gUnk_020192E0;
    p = REQ_PLAYER(e);
    zi = REQ_ZONE(e);
    s1 = zi * 0x94 + p * 0xD64;
    zb = e + 0x2C;
    z = (struct ZoneB7 *)(s1 + (int)zb);
    z->b7 |= 4;
    ((struct ReqFlags *)(e + 0x1B2C))->b1 = 0;
}
/* Two-step request driver on the step counter at 0x020192E0+0x1B30 (bits 2-9): step 0 sets 02017A40[0x3E0] = 0x80 and advances,
 * step 1 runs the picker sub_0803D57C on a new CardRef and advances when it returns 0; other steps clear the request flag. */
void sub_08049450(void)
{
    struct CardRef ref;
    u8 *e = gUnk_020192E0;

    switch (((struct ReqStep *)(e + 0x1B30))->cnt) {
    case 0:
        gUnk_02017A40[0x3E0] = 0x80;
        ((struct ReqStep *)(e + 0x1B30))->cnt++;
        /* fall through */
    case 1: {
        u16 id = gUnk_08624A0A[0];

        ref.id = id;
        ref.player = 0;
        ref.skip4 = 0;
        gUnk_02017A40[0x3E0] = sub_0803D57C(&ref, 0);
        if (gUnk_02017A40[0x3E0] == 0) {
            u8 *e2 = gUnk_020192E0; /* a fresh base: reusing e keeps it live across the call */

            ((struct ReqStep *)(e2 + 0x1B30))->cnt++;
        }
        break;
    }
    default:
        ((struct ReqFlags *)(e + 0x1B2C))->b1 = 0;
        break;
    }
}
struct Flags514 { u8 f0 : 1; u8 bit1 : 1; u8 phase : 3; u8 rest : 3; };
/* sub_08008860 returns int (see src/code_08007994.c); this unit declares it u16. */
#define CN8860(p) (((int (*)(int))sub_08008860)(p))
/*
 * Usability flags for card `id` held by `player`. Only when the duel phase (0x020192E0+0x1B12
 * bits 2-4) is 2 or 4: traps (type 0x16) get 0x10 (+0x40 when sub_0802CFA0 allows it), with
 * card numbers 0x520 and 0x605-0x608 special-cased; magic (0x15) gets 0x10; other cards go
 * through sub_08054398 / sub_08007834 and the sub_08047170 / sub_08047114 masks, then card
 * numbers 0x47 / 0x1A8 add 0x40. The shared tail adds 0x40 for trap subtype 5, drops it when
 * either side has card 0x49C, and clears 0x10/0x40 for spells and traps when player byte +7
 * bits 6-7 are set.
 *
 * The explicit `(u16)(flags | C)` forms and the one plain `flags |= 0x1800` are not
 * interchangeable here: the ROM keeps the lsl/lsr truncation after every OR except that one.
 */
int sub_08049514(u16 id, int player)
{
    u16 flags;
    u8 *e;
    struct CardRef ref; /* FAKEMATCH: unused, but the ROM reserves its 0x14-byte stack slot */
    u32 n;
    u8 *pb;
    int t;
    u8 *e2;

    flags = 0;
    e = gUnk_020192E0;
    switch (((u32)e[0x1B12] << 27) >> 29) {
    case 2:
    case 4:
        n = 0x7FF & id;
        switch ((((const u32 *)0x08621DE0)[n] & 0x1F00000) >> 20) {
        case 0x16:
            if (sub_08008C94(player, id) == 0)
                break;
            if (sub_0802CFA0(player, id, 1) != 0)
                flags = 0x40;
            flags = (u16)(flags | 0x10);
            switch (((const u16 *)0x08622AB4)[n]) {
            case 0x605:
            case 0x606:
            case 0x607:
            case 0x608:
                flags &= 0xFFEF;
                break;
            case 0x520:
                {
                    /* e + 4 first, as a separate term (also fixes later reload registers) */
                    u8 *zb = e + 4;
                    int s1 = (player & 1) * 0xD64;
                    pb = (u8 *)(s1 + (int)zb);
                }
                if ((pb[9] << 26) < 0 || (pb[8] << 27) < 0)
                    flags &= 0xFFBF;
                break;
            }
            break;
        case 0x15:
            if (sub_08008C94(player, id) != 0)
                flags = 0x10;
            break;
        default:
            if (sub_08054398(player, id) != 0) {
                if (sub_08007834(id) != 0) {
                    flags |= 0x1800;
                    if ((u16)sub_08047170(0) == 0)
                        flags = 0;
                } else {
                    if ((e[(player & 1) * 0xD64 + 0xC] << 27) >= 0)
                        flags = 0x30;
                    switch (((const u16 *)0x08622AB4)[n]) {
                    case 0x17A:
                        if (sub_080086CC(player, 0x4DB) <= 0)
                            break;
                    case 0x5F0:
                        flags = (u16)(flags | 0x1800);
                        break;
                    case 0x546:
                        if (CN8860(0) + 1 < CN8860(1) && sub_08008A1C(0) > 0)
                            flags = (u16)(flags | 0x1800);
                        if (sub_08008AF8(player, -1) == 0)
                            flags &= 0xFFCF;
                        break;
                    }
                }
                if ((u16)sub_08047170(0) == 0)
                    flags &= 0xE7FF;
                if ((u16)sub_08047114(0) == 0)
                    flags &= 0xFFDF;
            }
            switch (((const u16 *)0x08622AB4)[0x7FF & id]) {
            case 0x47:
                if (sub_08008C94(player, id) != 0 && sub_0802CFA0(player, id, 1) != 0 &&
                    sub_08008A1C(player) > 0 &&
                    (gUnk_020192E4[player & 1].rest[0] << 27) >= 0)
                    flags = (u16)(flags | 0x40);
                break;
            case 0x1A8:
                if (sub_0802CFA0(player, id, 1) != 0)
                    flags = (u16)(flags | 0x40);
                break;
            }
            break;
        }
        break;
    }
    e2 = gUnk_020192E0; /* a fresh base: the ROM reloads 0x020192E0 and adds 0x1B12 here */
    if (((struct Flags514 *)(e2 + 0x1B12))->bit1 == 0) {
        u32 st = ((const u32 *)0x08621DE0)[0x7FF & id];
        if (((st & 0x1F00000) >> 20) == 0x16 && ((st & 0xE0000) >> 17) == 5 &&
            sub_08008C94(player, id) != 0 && sub_0802CFA0(player, id, 1) != 0)
            flags = (u16)(flags | 0x40);
    }
    if (((((const u32 *)0x08621DE0)[0x7FF & id] & 0x1F00000) >> 20) == 0x16 && (flags & 0x40) != 0) {
        if (sub_08008524(0, 0x49C) > 0)
            flags &= 0xFFBF;
        if (sub_08008524(1, 0x49C) > 0)
            flags &= 0xFFBF;
    }
    t = (((const u32 *)0x08621DE0)[0x7FF & id] & 0x1F00000) >> 20;
    switch (t) {
    case 0x15:
    case 0x16:
        if (gUnk_020192E4[player & 1].b7 >> 6 != 0) {
            flags &= 0xFFEF;
            flags &= 0xFFBF;
        }
        break;
    }
    return flags;
}
/*
 * Usability lookup for the spell/trap command menu: given a card id, a player and a spell/trap
 * zone, return a flag (or a sub_0802CFD0 / sub_08008AF8 result) depending on the card's number.
 * Only the listed card numbers reach a special case; everything else returns 0.
 */
struct Z880 { u8 pad[7]; u8 lo7 : 5; u8 flag : 1; u8 hi7 : 2; u8 rest[0x94 - 8]; };
static inline int GetCardType880(u16 id) { return (((u32 *)0x08621DE0)[(id) & 0x7FF] & 0x1F00000) >> 20; }
#define CTYPE880(id) GetCardType880(id)

int sub_08049880(u16 id, int player, int zone)
{
    struct CardRef ref;
    u32 flag;
    int i;
    int target;

    flag = ((struct Z880 *)((player & 1) * 0xD64 + zone * 0x94 + (int)((u8 *)gUnk_020192E4 + 0x28)))->flag;
    ref.player = player;
    ref.id = id;

    switch (CARD_NUM(id)) {
    case 0x58:
    case 0x1FF:
        return sub_08008AF8(player, -1) > 0;
    case 0x105:
        return sub_08008AF8(player, zone) > 0;

    case 0xF:
    case 0x191:
    case 0x1A3:
    case 0x1AC:
    case 0x1F9:
    case 0x2E6:
    case 0x34D:
    case 0x458:
    case 0x596:
    case 0x598:
    case 0x599:
    case 0x59F:
    case 0x5A2:
    case 0x5A4:
    case 0x5E6:
        return ((u16 (*)(int, int, u16))sub_0802CFD0)(player, zone, 0);

    case 0x1A0:
    case 0x243:
    case 0x2DB:
        if ((gUnk_020192E0[0x1B12] & 0x1C) != 4)
            return 0;
        return ((u16 (*)(int, int, u16))sub_0802CFD0)(player, zone, 2);

    case 0x51:
    case 0x186:
        if (sub_0800A78C(player, zone, 0x291) == 0)
            return 0;
        target = 0;
        switch (CARD_NUM(id)) {
        case 0x51:
            target = 0x2E5;
            break;
        case 0x186:
            target = 0x187;
            break;
        }
        if (target <= 0)
            return 0;
        for (i = 0; i < gUnk_020192E4[player & 1].pad[3]; i++) {
            if (CARD_NUM((((u32 *)((u8 *)gUnk_020192E4 + 0x7C4 + (player & 1) * 0xD64))[i] << 20) >> 20) == target) {
                if (sub_08008524(0, 0x58A) > 0)
                    return 0;
                if (sub_08008524(1, 0x58A) > 0)
                    return 0;
                return 1;
            }
        }
        return 0;

    case 0x2DA:
    case 0x536:
        if (((int (*)(int, int))sub_0800A430)(player, zone) != 0xFFFF)
            return 0;
        if (sub_08008C6C(player) == -1)
            return 0;
        for (i = 0; i <= 4; i++) {
            if (((int (*)(u16, int, int))sub_0802B1B8)(id, 1 - player, i) != 0)
                return flag;
        }
        return 0;

    case 0x5E9:
        if (flag == 0)
            return 0;
        for (i = 0; i < gUnk_020192E4[player & 1].pad[4]; i++) {
            if ((u32)CTYPE880((((u32 *)((u8 *)gUnk_020192E4 + 0x904 + (player & 1) * 0xD64))[i] << 20) >> 20) <= 0x14)
                return 1;
        }
        return 0;

    default:
        return 0;
    }
}
/* Usability flag builder for a card in spell/trap zone `arg2` of `arg1` (only player 0 is handled). */
int sub_08008524(int player, u16 number);
int sub_0800C8BC(int player, int zone);
int sub_0800A78C(int player, int zone, u16 number);
int sub_08047114(int player);
int sub_08049880(u16 id, int player, int zone);
int sub_0804A528(int player, int zone, int a);
struct DZone { u32 w0; u8 unk4; u8 unk5; u8 b6; u8 b7; u8 rest[0x94 - 8]; };
struct DZones { struct DZone z[11]; u8 filler[0xD64 - 11 * 0x94]; };
#define CARD_NUM(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define ZONE_ADDR(p, z) ((struct DZone *)(0xD64 * ((p) & 1) + (u32)gUnk_0201930C + 0x94 * (z)))
#define ZONE_R(p, z) ((struct DZone *)(0x94 * (z) + 0xD64 * ((p) & 1) + (u32)gUnk_0201930C))
struct WfPl49B74 { u8 pad[0x2A]; u16 f2A; u8 rest[0xD64 - 0x2C]; };
struct WfDuel49B74 { struct WfPl49B74 p[2]; };
u16 sub_08049B74(u16 arg0, int arg1, int arg2)
{
    u16 id = arg0;
    int s1 = 0xD64 * (arg1 & 1);
    u32 base = (u32)gUnk_0201930C;
    struct DZone *z = (struct DZone *)(s1 + base + 0x94 * arg2);
    u16 flags = 0;
    int found = 0;
    u8 *e;
    if (arg1 != 0)
        return 0;
    if ((ZONE_R(0, arg2)->b6 & 2) != 0 && sub_0800C8BC(0, arg2) == 1 &&
        (sub_08008524(0, 0x148) != 0 || sub_08008524(1, 0x148) != 0))
        found = 1;
    e = gUnk_020192E0;
    switch ((int)((u32)(e[0x1B12] << 27) >> 29)) {
    case 1: {
        int t = arg1 & 1;
        int s1 = arg2 * 0x94 + t * 0xD64;
        u8 *zb = e + 0x2C;
        struct DZone *zn = (struct DZone *)(s1 + (int)zb);
        if ((zn->b6 & 2) != 0) {
            u16 cid = CARD_NUM(((u32)zn->w0 << 21) >> 21);
            switch (cid) {
            case 0x1A0:
            case 0x243:
            case 0x2DB:
                if (sub_0802CFD0(arg1, arg2, 2) != 0)
                    flags |= 0x40;
                break;
            }
        }
        break;
    }
    case 2:
    case 4:
        if ((z->b7 & 4) == 0 && (s32)(gUnk_020192E4[arg1 & 1].b7 << 26) >= 0 &&
            sub_0800A78C(arg1, arg2, 0x15C) == 0 &&
            sub_0800A78C(arg1, arg2, 0x4DC) == 0 && found == 0) {
            if ((z->b6 & 2) != 0) {
                if ((z->b6 & 1) != 0)
                    flags = (u16)(flags | 4);
                else
                    flags = (u16)(flags | 2);
            } else {
                flags = (u16)(flags | 8);
                if ((z->b6 & 1) == 0)
                    flags = (u16)(flags | 2);
                if ((u16)sub_08047114(0) == 0)
                    flags &= 0xFFF7;
            }
            if (sub_08008524(0, 0x536) > 0 || sub_08008524(1, 0x536) > 0) {
                int t = arg1 & 1;
                int s2 = arg2 * 0x94 + t * 0xD64;
                if (CARD_NUM(((u32)((struct DZone *)(s2 + (u32)gUnk_0201930C))->w0 << 21) >> 21) != 0x536)
                    flags &= 0xFFF1;
            }
        }
        {
            int t = arg1 & 1;
            int s2 = arg2 * 0x94 + t * 0xD64;
            if ((((struct DZone *)(s2 + (u32)gUnk_0201930C))->b6 & 2) != 0 && (u16)sub_08049880(id, arg1, arg2) != 0)
                flags = (u16)(flags | 0x40);
        }
        break;
    case 3:
        asm("" : "+r"(e)); /* FAKEMATCH: hide e's constant value from CSE so `e + t*0xD64` keeps its operand order */
        if (sub_0804A528(arg1, arg2, 1) != 0 &&
            !(((((struct WfDuel49B74 *)e)->p[arg1 & 1].f2A) >> arg2) & 1))
            flags = (u16)(flags | 0x80);
        break;
    }
    return flags;
}
/* Which usability flags (bit 6 = can be activated) does the card `id` have when set in spell/trap zone `zone` + 5 of `player`?
 * Only player 0 is ever evaluated (hypothesis: "is a face-down trap/spell of the current player usable"). */
/* Duel flags byte at gUnk_020192E0+0x1B12 (DuelState.linkSkip / phase1B12 in duel.h). */
struct Flags1B12 { u8 f0 : 1; u8 bit1 : 1; u8 phase : 3; u8 rest : 3; };
int sub_08049DF0(u16 id, int player, int zone)
{
    u8 *e;
    u32 flags;
    u32 n;
    u32 st;
    int t;
    struct ZonesF *pz = &((struct ZonesF *)gUnk_0201930C)[player & 1];
    struct ZoneF *zn = &pz->z[zone + 5];
    flags = 0;
    if (player != 0)
        return 0;
    n = 0x7FF & id;
    st = ((const u32 *)0x08621DE0)[n];
    switch ((st & 0x1F00000) >> 20) {
    case 0x16:
        if ((zn->b91 & 4) != 0) {
            if ((zn->b6 << 30) >= 0) {
                int f = 0;
                if (((st & 0xE0000) >> 17) == 5)
                    f = 1;
                {
                    struct Flags1B12 *fl = (struct Flags1B12 *)(gUnk_0201930C + 0x1AE6);
                    if ((fl->phase == 2 || fl->phase == 4) && fl->bit1 == 0)
                        f = 1;
                }
                if (f != 0 && sub_0802CFA0(player, id, 0) != 0)
                    flags |= 0x40;
            }
        }
        e = gUnk_020192E0;
        {
            struct Flags1B12 *fl = (struct Flags1B12 *)(e + 0x1B12);
            if (fl->phase == 1) {
                if (fl->bit1) {
                    if (((const u16 *)0x08622AB4)[0x7FF & id] == 0x489) {
                        if (sub_0802CFD0(1 - player, zone + 5, 3) != 0)
                            flags = (u16)(flags | 0x40);
                    }
                } else {
                    if (((const u16 *)0x08622AB4)[0x7FF & id] == 0x428) {
                        int s1 = (player & 1) * 0xD64 + zone * 0x94;
                        u8 *zb = e + 0x310;
                        if ((((struct ZoneG *)(s1 + (int)zb))->b6 & 2) == 0) {
                            if (sub_0802CFD0(player, zone + 5, 2) != 0)
                                flags = (u16)(flags | 0x40);
                        }
                    }
                }
            }
        }
        break;
    case 0x15:
        if ((zn->b91 & 4) != 0) {
            int face = (u32)(zn->b6 << 30) >> 31;
            switch (((const u16 *)0x08622AB4)[n]) {
            case 0x52C:
            case 0x3F9:
            case 0x594:
            case 0x5FC:
                face = 0;
                break;
            }
            if (face == 0) {
                if (sub_0802CFD0(player, zone + 5, 0) != 0)
                    flags = (u16)(flags | 0x40);
            }
        }
        break;
    default:
        break;
    }
    t = (((const u32 *)0x08621DE0)[0x7FF & id] & 0x1F00000) >> 20;
    switch (t) {
    case 0x15:
    case 0x16:
        if (gUnk_020192E4[player & 1].b7 >> 6 != 0)
            flags &= 0xFFBF;
        break;
    }
    return flags;
}

