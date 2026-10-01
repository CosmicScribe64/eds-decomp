#include "global.h"

int sub_08008524(int player, u16 number);
int sub_080086CC(int player, u16 number);
int sub_08009298(int player, int zone);
void sub_080197E0(int player, u16 x);
void sub_080199E0(int player, int n);
void sub_08019860(int player, int n);
void sub_08018544(int player, int zone, int a);
void sub_0802272C(int player, int n, int a, int b);
void sub_0801EC58(u32 msg, u16 zone, int a, int b);
extern const u16 gUnk_08623DF4[];
struct DuelZone { u32 w0; u8 unk4; u8 unk5; u8 flags6; u8 unk7[0x94 - 7]; };
#define ID(z) (((z)->w0 << 20) >> 20)
struct ZoneBits { u8 pad[6]; u8 lo : 2; u8 kind : 4; u8 hi : 2; u8 rest[0x94 - 7]; };
struct DuelZonesPlayer { struct DuelZone z[11]; u8 filler[0xD64 - 11 * 0x94]; };
extern struct DuelZonesPlayer gUnk_0201930C[];
extern u8 gUnk_020192E0[];
struct AE60 { u8 unk0[0x14]; u16 h14; };
extern struct AE60 gUnk_0201AE60;
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_086246BC[];
extern const u16 gUnk_08623F3E[], gUnk_08624084[], gUnk_086241A8[];
extern const char gUnk_080854AC[];
extern const char gUnk_0822C720[];
int sub_080090C8(int player, u16 number);
int sub_0800C8BC(int player, int zone);
void sub_08017AB4(int player, u16 cardId, u16 pos, u16 a);
void sub_080197C0(int player, u16 id);
void sub_08019980(int player, int lp);
void sub_08018ED8(int a, int b, int c, int d);
int sub_08009CAC(int player, u16 number);
void sub_08019554(int player, u16 id);
void sub_080753F4(char *dst, const char *fmt, const char *arg);
void sub_080602A4(int a, int b, int c, char *s);
void sub_08060308(int a, int b, int c);
struct EffEnt { u16 num; u8 rest0[6]; u16 (*fn)(void *, u16); u8 rest[12]; };
struct DuelPlayerB {
    u8 unk0[2];
    u8 handCount;                       /* +0x02 */
    u8 pad3[3];
    u8 count;                           /* +0x06 */
    u8 flags7lo : 3; u8 flags7bit3 : 1; u8 flags7bit4 : 1; u8 flags7hi : 3;  /* +0x07 */
    u8 pad8[0x684 - 8];
    u32 hand[80];                       /* +0x684 */
    u8 pad4[0xB84 - 0x684 - 0x140];
    u32 list[80];                       /* +0xB84 */
    u8 pad2[0xD64 - 0xB84 - 0x140];
};
extern struct DuelPlayerB gUnk_020192E4[];
extern const u16 gUnk_086249EE[];
int sub_08047058(u32 id);
int sub_08076F9C(void);
int sub_08008C6C(void);
int sub_0801A09C(int player, u16 number, int x);
int __modsi3(int, int);
extern struct EffEnt gUnk_0819A9D4[];
extern const u16 gUnk_08622AB4[];

/* For a face-down or valid card in the zone: if card 0x52 is present for either side, queues a request for it (hypothesis) */
void sub_08046738(int player, int zone)
{
    int pi = player & 1;
    int s1 = zone * 0x94 + pi * 0xD64;
    u8 *base = (u8 *)gUnk_0201930C;
    struct DuelZone *z = (struct DuelZone *)(s1 + (int)base);
    if (((z)->w0 << 20) != 0 && (z->flags6 & 2) != 0) {
        if (sub_080090C8(0, 0x52) > 0 || sub_080090C8(1, 0x52) > 0) {
            sub_080197C0(player, ID(z));
            sub_08017AB4(player, ID(z), (u8)player | (u8)zone << 8, 0xD);
        }
    }
}
void sub_080467B0(int player)
{
    int a = sub_080090C8(player, 0xA5);
    int other = 1 - player;
    int b = sub_080090C8(other, 0xA5);
    if (a > 0 || b > 0) {
        sub_080197E0(player, gUnk_08623F3E[0]);
        sub_08019980(player, a * 500);
        sub_08019980(other, b * 500);
    }
}
void sub_08046808(int player)
{
    int found;
    int p, z;
    u32 id = 0x148;
    if (sub_08008524(0, id) > 0 || sub_08008524(1, id) > 0) {
        found = 0;
        for (p = 0; p < 2;) {
            /* FAKEMATCH: keep the next player in r6 after initializing the zone loop. */
            register int next asm("r6");
            z = 0;
            next = p + 1;
            for (; z < 5; z++) {
                int s1 = z * 0x94 + (p & 1) * 0xD64;
                u8 *base = (u8 *)gUnk_0201930C;
                struct DuelZone *zn = (struct DuelZone *)(s1 + (int)base);
                if ((zn->w0 << 20) != 0 && (zn->flags6 & 3) == 2) {
                    if (sub_0800C8BC(p, z) == 1)
                        found = 1;
                }
            }
            p = next;
        }
        if (found != 0) {
            u32 msg = 0x73;
            if (player != 0)
                msg = 0x8073;
            sub_0801EC58(msg, gUnk_08624084[0], 1, 0);
            for (p = 0; p < 2;) {
                /* FAKEMATCH: keep the next player in r6 after initializing the zone loop. */
                register int next asm("r6");
                z = 0;
                next = p + 1;
                for (; z < 5; z++) {
                    int s1 = z * 0x94 + (p & 1) * 0xD64;
                    u8 *base = (u8 *)gUnk_0201930C;
                    struct DuelZone *zn = (struct DuelZone *)(s1 + (int)base);
                    if ((zn->w0 << 20) != 0 && (zn->flags6 & 3) == 2) {
                        if (sub_0800C8BC(p, z) == 1)
                            sub_08018ED8(p, z, 0, 0);
                    }
                }
                p = next;
            }
        }
    }
}
int sub_0804691C(int player)
{
    char buf[0x80];
    u8 *e = gUnk_020192E0;
    u8 *step = e + 0x1B22;
    switch (*step) {
    case 0: {
        int id = 0x1DA;
        if (sub_08009CAC(player, id) == 0)
            return 1;
        if (player != 0) {
            /* FAKEMATCH: materialize the base in r0 before loading the flag value. */
            register struct AE60 *flag __asm__("r0") = &gUnk_0201AE60;
            __asm__ __volatile__("" : : "r"(flag));
            flag->h14 = 1;
        } else {
            sub_080753F4(buf, gUnk_080854AC, gUnk_0822C720 + ((const u16 *)0x08623DF4)[id] * 0x40);
            sub_080602A4(0x206, 0x712, 0xB, buf);
            sub_08060308(1, 0, 0);
        }
        break;
    }
    case 1:
        if (gUnk_0201AE60.h14 != 0) {
            int id = 0x1DA;
            sub_080197E0(player, gUnk_086241A8[0]);
            sub_08019554(player, id);
        }
        break;
    default:
        return 1;
    }
    (*step)++;
    return 0;
}
/* Both the inline argument and the caller's card ID are narrow in the ROM. */
static inline u32 EffectCardType(u16 id)
{
    return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
}

/* Queue a prompt for each face-down type-0x15 card in spell/trap zones. */
void sub_080469DC(void)
{
    int p, z;
    for (p = 0; p < 2; p++) {
        for (z = 5; z <= 9; z++) {
            struct DuelZone *zn = (struct DuelZone *)(z * 0x94 + (p & 1) * 0xD64 + 0x0201930C);
            u16 id = (zn->w0 << 20) >> 20;
            if (id != 0 && (zn->flags6 & 2) != 0) {
                if (EffectCardType(id) == 0x15) {
                    u32 msg = 0xB1;
                    if (p != 0)
                        msg = 0x80B1;
                    sub_0801EC58(msg, z, 1, 0);
                }
            }
        }
    }
}
void sub_08046A74(int player)
{
    u32 id = 0x436;
    int n = sub_08008524(player, id);
    n += sub_08008524(1 - player, id);
    if (n > 0) {
        u32 msg;
        sub_080197E0(player, ((const u16 *)0x08623DF4)[id]);
        msg = 0x43;
        if (player != 0)
            msg = 0x8043;
        sub_0801EC58(msg, n * 500, 1, 0);
    }
}
void sub_08046AD0(void)
{
    int found;
    int p, z;
    u32 id = 0x464;
    if (sub_080086CC(0, id) > 0 || sub_080086CC(1, id) > 0) {
        found = 0;
        for (p = 0; p < 2; p++) {
            for (z = 0; z < 5; z++) {
                if (sub_08009298(p, z) != 0)
                    found = 1;
            }
        }
        if (found != 0) {
            sub_080197E0(0, gUnk_086246BC[0]);
            for (p = 0; p < 2; p++) {
                for (z = 0; z < 5; z++) {
                    if (sub_08009298(p, z) != 0)
                        sub_08018544(p, z, 1);
                }
            }
        }
    }
}
void sub_08046B54(int player, int zone)
{
    u32 id = 0x464;
    if (sub_080086CC(0, id) > 0 || sub_080086CC(1, id) > 0) {
        if (sub_08009298(player, zone) > 0) {
            sub_080197E0(player, ((const u16 *)0x08623DF4)[id]);
            sub_08018544(player, zone, 1);
        }
    }
}
void sub_08046BA8(int player)
{
    u32 id = 0x475;
    int n = sub_08008524(player, id);
    if (n > 0) {
        sub_080197E0(player, ((const u16 *)0x08623DF4)[id]);
        sub_080199E0(player, n * 2);
    }
}
void sub_08046BE0(int player, int mul)
{
    u32 id = 0x476;
    mul *= sub_08008524(player, id);
    if (mul > 0) {
        sub_080197E0(player, ((const u16 *)0x08623DF4)[id]);
        sub_0802272C(1 - player, mul, 0, 1);
    }
}
void sub_08046C20(int player, int idx)
{
    u32 id = 0x51A;
    int n = sub_08008524(player, id);
    n += sub_08008524(1 - player, id);
    if (n > 0) {
        sub_080197E0(player, ((const u16 *)0x08623DF4)[id]);
        sub_08019860(player, idx * 300);
    }
}
void sub_08046C6C(int player, int zone, u16 flag)
{
    if (flag != 0)
        sub_0801EC58(player ? 0x80D9 : 0xD9, zone, 1, 0);
    else
        sub_0801EC58(player ? 0x80D8 : 0xD8, zone, 0, 0);
}
void sub_08046CB0(int a, int b, int c)
{
    int pi = b & 1;
    int s1 = c * 0x94 + pi * 0xD64;
    u8 *zb = (u8 *)gUnk_0201930C;
    struct DuelZone *z = (struct DuelZone *)(s1 + (int)zb);
    if (((const u16 *)0x08622AB4)[(z->w0 << 21) >> 21] == 0x5EA && a != b) {
        u32 id = 0x453;
        if (sub_080086CC(0, id) <= 0 && sub_080086CC(1, id) <= 0 && c <= 4) {
            u32 msg;
            sub_080197C0(b, ID(z));
            msg = 0x4C;
            if (b != 0)
                msg = 0x804C;
            sub_0801EC58(msg, 1, 0, 0);
        }
    }
}
void sub_08046D3C(int player, int zone)
{
    /* FAKEMATCH: keep the queried position in r8 across the selection branches. */
    register int x asm("r8") = sub_08008C6C();
    int pi = player & 1;
    struct DuelZone *zn = (struct DuelZone *)(zone * 0x94 + pi * 0xD64 + 0x0201930C);
    u16 num;
    switch (((struct ZoneBits *)zn)->kind) {
    case 0:
        num = 0x605;
        break;
    case 1:
        num = 0x606;
        break;
    case 2:
        num = 0x607;
        break;
    case 3:
        num = 0x608;
        break;
    default:
        return;
    }
    if (sub_0801A09C(player, num, x) != 0) {
        /* FAKEMATCH: preserve the lookup result in r0 until the u16 call conversion. */
        register u32 id asm("r0");
        if (num == 0xFFFF) {
            id = 0;
        } else if (num <= 0x7CF) {
            /* FAKEMATCH: retain the table load after the byte-index calculation. */
            int index = num * 2;
            const u16 *table = (const u16 *)0x08623DF4;
            __asm__("" : "+r"(table));
            id = *(const u16 *)(index + (int)table);
        } else {
            num |= 0x30;
            {
                /* FAKEMATCH: use r4 for the table base after computing the byte index. */
                int index = num * 2;
                register const u16 *table asm("r4") = (const u16 *)0x08623DF4;
                __asm__("" : "+r"(table));
                id = *(const u16 *)(index + (int)table) + 1;
            }
        }
        sub_080197E0(player, id);
    } else {
        int i = 0;
        u8 *base = (u8 *)gUnk_020192E4;
        int offset = (player & 1) * 0xD64;
        struct DuelPlayerB *pl = (struct DuelPlayerB *)(offset + (int)base);
        if (i < pl->handCount) {
            u8 *handBase = base + 0x684;
            /* FAKEMATCH: keep the slot mask in r9 and preserve its reload for masking x. */
            register int mask asm("r9") = 0xF;
            __asm__("" : "+r"(mask));
            {
                /* FAKEMATCH: retain the masked position in ip through the hand scan. */
                register int lowX asm("r12") = x & mask;
                u32 *e = (u32 *)(offset + (int)handBase);
                int bound;
                do {
                    u32 cid = (*e << 20) >> 20;
                    if (((const u16 *)0x08622AB4)[cid & 0x7FF] == num) {
                        u32 msg = 0xC5;
                        if (player != 0)
                            msg = 0x80C5;
                        {
                            int packed = (i & mask) << 4;
                            /* FAKEMATCH: combine the saved position through r5. */
                            register int low asm("r5") = lowX;
                            __asm__("" : "+r"(low));
                            packed |= low;
                            {
                                /* FAKEMATCH: materialize the flag in r7, retaining its move to r0. */
                                register int flag asm("r7") = 0x100;
                                register int value asm("r0") = flag;
                                __asm__("" : "+r"(value));
                                packed |= value;
                            }
                            sub_0801EC58(msg, cid, packed, 0);
                        }
                        sub_080197E0(player, (*e << 20) >> 20);
                        return;
                    }
                    e++;
                    i++;
                    bound = pl->handCount;
                    /* FAKEMATCH: preserve the loop-bound reload in the original scratch register. */
                    __asm__("" : "+r"(bound));
                } while (i < bound);
            }
        }
    }
}
void sub_08046E8C(int player)
{
    if (sub_08008524(player, 0x5FD) != 0) {
        /* FAKEMATCH: keep the qualifying-card count in the original r4. */
        register int n asm("r4") = 0;
        u8 *base = (u8 *)gUnk_020192E4;
        int pi = (1 - player) & 1;
        /* FAKEMATCH: retain the offset in r2; integer address sums preserve ADD operand order. */
        register int offset asm("r2") = pi * 0xD64;
        int count = ((struct DuelPlayerB *)(offset + (int)base))->count;
        if (n < count) {
            int listOffset = 0xB84;
            /* FAKEMATCH: materialize the list base in r0 before adding the player offset. */
            register u8 *listBase asm("r0") = base + listOffset;
            u32 *cursor = (u32 *)(offset + (int)listBase);
            /* FAKEMATCH: keep the only hoisted lookup constant in r6. */
            register int mask asm("r6") = 0x7FF;
            /* FAKEMATCH: reuse r2 for the remaining-card counter. */
            register int remain asm("r2") = count;
            /* FAKEMATCH: this back edge leaves the table and type-mask loads in the loop. */
        loop:
            {
                u16 id = (*cursor << 20) >> 20;
                if (((((const u32 *)0x08621DE0)[id & mask] & 0x1F00000) >> 20) <= 0x14)
                    n++;
                cursor++;
            }
            if (--remain != 0)
                goto loop;
        }
        if (n > 0) {
            sub_080197E0(player, gUnk_086249EE[0]);
            sub_08019860(1 - player, n * 100);
        }
    }
}
/* Dice effect. Rolls a d6 and destroys every monster whose level (0 for types 0x15-0x17, 10 for 0x18) equals the roll (hypothesis: card 0x600) */
void sub_08046F20(int player)
{
    int z, p;
    u32 id = 0x600;
    if (sub_08008524(player, id) != 0) {
        int roll = sub_08076F9C() % 6 + 1;
        /* FAKEMATCH: retain the roll in the ROM's register before announcing it. */
        __asm__ __volatile__("" : : "r"(roll));
        sub_080197E0(player, ((const u16 *)0x08623DF4)[id]);
        {
            u32 msg = 0xE4;
            if (player != 0)
                msg = 0x80E4;
            sub_0801EC58(msg, roll, 0, 0);
        }
        {
            u32 msg = 0x12;
            if (player != 0)
                msg = 0x8012;
            sub_0801EC58(msg, 0, 0, 0);
        }
        for (p = 0; p < 2; p++) {
            for (z = 0; z <= 4; z++) {
                struct DuelZone *zn = (struct DuelZone *)(z * 0x94 + (p & 1) * 0xD64 + 0x0201930C);
                u16 cid = (zn->w0 << 20) >> 20;
                if (cid != 0 && (zn->flags6 & 2) != 0) {
                    int lv;
                    int hit;
                    int t = (((const u32 *)0x08621DE0)[cid & 0x7FF] & 0x1F00000) >> 20;
                    switch (t) {
                    case 0x15:
                    case 0x16:
                    case 0x17:
                        lv = 0;
                        break;
                    case 0x18:
                        lv = 10;
                        break;
                    default:
                        lv = (((const u32 *)0x08621DE0)[cid & 0x7FF] & 0x1E000000) >> 25;
                        break;
                    }
                    hit = 0;
                    if (lv == roll)
                        hit = 1;
                    if (lv > 5 && roll == 6)
                        hit = 1;
                    if (hit != 0)
                        sub_08018544(p, z, 1);
                }
            }
        }
    }
}
int sub_08047050(void)
{
    return 0;
}
int sub_08047054(void)
{
    return 0;
}
/* Binary-search the effect table gUnk_0819A9D4 (6-byte entries, 0x1AA of them, sorted by card number) for a card; -1 if absent. */
int sub_08047058(u32 id)
{
    int lo = 0;
    int hi = 0x1A9;
    u16 key = ((const u16 *)0x08622AB4)[(id << 21) >> 21];
    for (;;) {
        int mid = (lo + hi) / 2;
        u16 v = gUnk_0819A9D4[mid].num;
        if (key == v)
            return mid;
        if (lo == hi)
            return -1;
        if (key > v)
            lo = mid;
        if (key < v)
            hi = mid;
        if ((lo + hi) / 2 == mid)
            lo = hi;
    }
}
/* Runs the per-card effect handler from gUnk_0819A9D4 for the card in ref. Returns 1 if there is none and 0 if ref is NULL. */
u16 sub_080470C0(u16 *ref, int a, int b)
{
    if (ref != 0) {
        int idx = sub_08047058(*ref);
        u16 (*fn)(void *, u16);
        if (idx < 0 || (fn = gUnk_0819A9D4[idx].fn) == 0)
            return 1;
        return fn(ref, (u8)a | (u8)b << 8);
    }
    return 0;
}
int sub_08047114(int player)
{
    if (!gUnk_020192E4[player & 1].flags7bit3 && sub_08008524(player, 0x592) == 0
        && sub_08008524(0, 0x5F6) == 0 && sub_08008524(1, 0x5F6) == 0)
        return 1;
    return 0;
}
int sub_08047170(int player)
{
    if (gUnk_020192E4[player & 1].flags7bit4)
        return 0;
    if (sub_08008524(player, 0x592) != 0)
        return 0;
    if (sub_08008524(0, 0x5E6) != 0)
        return 0;
    if (sub_08008524(1, 0x5E6) != 0)
        return 0;
    if (sub_08008524(0, 0x5F6) != 0)
        return 0;
    if (sub_08008524(1, 0x5F6) != 0)
        return 0;
    return 1;
}
/* Reconstruction of the summon/tribute selection state machine (a disabled draft).
 * State names remain hypotheses; byte accesses and callees are ROM-derived.
 * The sequence switches intentionally have no default assignment, as in the
 * ROM; invalid sequence values retain the live required-card register. State
 * 70 likewise has no default text assignment. The draft stays disabled until
 * the whole unit is byte-equal, including those paths. */
struct SummonDuel {
    u8 prefix[0x1B28];
    u16 cardId;
    u16 selected;
    u8 flag0:1;
    u8 active:1;
    u8 command:4;
    u8 flags2Chi:2;
    u8 padding2D[3];
    u16 flags30:2;
    u16 step:8;
    u8 sequence:4;
    u32 choices:4;
    u8 padding32:6;
    u8 flag33lo:1;
    u8 player:1;
    u8 flag33hi:6;
    u16 flag34lo:1;
    u16 sourceIndex:8;
    u32 eventZone:8;
    u8 tail[4];
};
extern struct SummonDuel gSummonDuel asm("gUnk_020192E0");
struct SummonCursor {
    u8 prefix[0x824];
    int player;
    int area;
    int zone;
};
extern struct SummonCursor gUnk_0201CFB0;
struct SummonList {
    u8 prefix[5];
    u8 row:2;
    u8 rest5:6;
    u16 scroll;
    u8 pad8[4];
    u32 cards[128];
};
extern struct SummonList gUnk_0201D810;
struct SummonCardRef {
    u16 id;
    u8 player:1;
    u8 rest2:7;
    u8 filler[0x14 - 3];
};
struct SummonMain { u8 prefix[6]; u16 keys; };
extern struct SummonMain gUnk_03000040;
extern char *gUnk_0819D1C4[];
extern const char gUnk_08085604[], gUnk_0808563C[], gUnk_08085660[];
extern const char gUnk_08085698[], gUnk_080856BC[], gUnk_08085708[];
extern const char gUnk_08085710[], gUnk_08085758[], gUnk_0808577C[];
extern const char gUnk_08085784[], gUnk_080857C4[], gUnk_080857CC[];
extern const char gUnk_08085804[], gUnk_0808580C[], gUnk_08085814[];
extern const char gUnk_0808581C[], gUnk_08085850[];
extern const u16 gUnk_0862401E[], gUnk_086240CE[];
int sub_08008794(int player, u16 number);
int sub_08008A1C(int player);
int sub_08008A44(int player);
int sub_08008A6C(int player, int zone);
int sub_0800A2A8(int player, u16 number);
int sub_0800CAF0(int player, int zone);
void sub_08017FF4(int player, int zone);
void sub_080189FC(int player, int zone, int arg);
void sub_080193D4(int player, int zone, int arg, int arg2);
void sub_0801965C(int player, u32 *card);
void sub_0801FBCC(u32 msg, int arg);
void sub_0802AF34(int player, int arg, u16 number, int arg2);
int sub_0802B2FC(struct SummonCardRef *ref, u16 pos);
int sub_08044224(int player, u16 number, int arg);
int sub_08052F38(int mask);
void sub_08055B28(int player, int zone, int target, u16 tribute, u16 faceUp);
void sub_080561A0(int player, int zone, int target, u16 tribute, u16 faceUp);
u16 sub_0805ECFC(void);
void sub_08075434(char *dst, const char *format, int arg);
void sub_08077AEC(int sound);

struct SummonDuelFromPlayer {
    u8 prefix[0x1B24];
    u16 cardId;
    u16 selected;
    u8 flag0:1;
    u8 active:1;
    u8 command:4;
    u8 flags2Chi:2;
    u8 padding2D[3];
    u16 flags30:2;
    u16 step:8;
    u8 sequence:4;
    u32 choices:4;
    u8 padding32:6;
    u8 flag33lo:1;
    u8 player:1;
    u8 flag33hi:6;
    u16 flag34lo:1;
    u16 sourceIndex:8;
    u32 eventZone:8;
    u8 tail[4];
};
struct SummonDuelFromZone {
    u8 prefix[0x1AFC];
    u16 cardId;
    u16 selected;
    u8 flag0:1;
    u8 active:1;
    u8 command:4;
    u8 flags2Chi:2;
    u8 padding2D[3];
    u16 flags30:2;
    u16 step:8;
    u8 sequence:4;
    u32 choices:4;
    u8 padding32:6;
    u8 flag33lo:1;
    u8 player:1;
    u8 flag33hi:6;
    u16 flag34lo:1;
    u16 sourceIndex:8;
    u32 eventZone:8;
    u8 tail[4];
};
struct SummonPlayerHeader {
    u8 prefix[6];
    u8 count;
    u8 pad7[9];
    u8 flags10;
    u8 rest[0xD64 - 0x11];
};
/* FAKEMATCH: unsigned subtraction of the negated stride retains ADD operand order. */
#define SUMMON_HEADER(p) (*(struct SummonPlayerHeader *)((u32)&SD - (u32)(-((p) * 0xD64))))
#define SD gSummonDuel
#define SDP (*(struct SummonDuelFromPlayer *)gUnk_020192E4)
#define SDZ (*(struct SummonDuelFromZone *)gUnk_0201930C)
#define SC gUnk_0201CFB0
#define SUMMON_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define SUMMON_CARD_NAME(id) (gUnk_0822C720 + (id) * 0x40)
#define SUMMON_HUMAN(p) ((s32)((u32)gUnk_020192E4[(p) & 1].pad8[4] << 26) < 0)
#define SUMMON_POS ((u8)SC.player | ((u8)SC.zone << 8))
#define SUMMON_EVENT_POS ((u8)SC.area | ((u8)SC.zone << 8))
#define SUMMON_SELECTED_POS (((u8)SC.player << 8) | (u8)SC.zone)

static inline int SummonLevel(u16 id)
{
    int type = (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1F00000) >> 20;
    switch (type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return (((const u32 *)0x08621DE0)[id & 0x7FF] & 0x1E000000) >> 25;
    }
}

static inline u16 SummonCardId(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number <= 0x7CF)
        return ((const u16 *)0x08623DF4)[number & 0x7FF];
    return ((const u16 *)0x08623DF4)[(number - 0x7D0) & 0x7FF] + 1;
}
static inline u16 SummonCardIdSymbol(u16 number)
{
    if (number == 0xFFFF)
        return 0;
    if (number <= 0x7CF)
        return *(gUnk_08623DF4 + (number & 0x7FF));
    return *(gUnk_08623DF4 + ((number - 0x7D0) & 0x7FF)) + 1;
}

#if 0 /* NONMATCHING: summon/tribute state machine; remaining: case 40 offset-register (r1 vs r2/r3) + choices==0 base/offset form, case 80 pool CSE, one far jump; see build/fable/sub_080471E8/NOTES.md */
void sub_080471E8(u16 faceUp, u16 special)
{
    char text[0x80];
    char format[0x80];
    struct SummonCardRef ref;
    /* FAKEMATCH: prepare both prompt calls before their shared state-advance tail. */
    register int promptA asm("r0"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
    register int promptB asm("r1");
    register int promptC asm("r2"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
    register char *promptStr asm("r3");
    u16 required; /* Assigned only for sequence values 0..2, as in the ROM. */
    switch (SD.step) {
    case 0:
        switch (SUMMON_NUMBER(SD.cardId)) {
        case 0x37:
        case 0x38:
        case 0x42:
            sub_080753F4(format, gUnk_08085604, SUMMON_CARD_NAME(SD.cardId));
            sub_080753F4(text, format, SUMMON_CARD_NAME(gUnk_0862401E[0]));
            sub_080602A4(0x206, 0x712, 0xB, text);
            SD.step = 20;
            break;
        case 0x170:
            sub_080753F4(format, gUnk_08085604, SUMMON_CARD_NAME(SD.cardId));
            sub_080753F4(text, format, SUMMON_CARD_NAME(gUnk_086240CE[0]));
            sub_080602A4(0x206, 0x712, 0xB, text);
            SD.step = 20;
            break;
        case 0x175:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 30;
            break;
        case 0x34D:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 40;
            break;
        case 0x4E2:
            if (SUMMON_HEADER(SD.player).count == 1) {
                sub_08055B28(SD.player, SD.sourceIndex, sub_08008A44(SD.player), 0, 1);
                SD.active = 0;
            } else {
                sub_080602A4(0x206, 0x712, 0xB, gUnk_0819D1C4[1]);
                sub_08060308(1, 0, 0);
                SD.selected = 0;
                SD.step++;
            }
            break;
        case 0x4E9:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 80;
            break;
        case 0x5EA:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 50;
            break;
        case 0x5EB:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 60;
            break;
        case 0x5EC:
        case 0x5ED:
        case 0x5EE:
        case 0x5EF:
            SD.selected = 0;
            SD.sequence = 0;
            SD.choices = 0;
            SD.step = 70;
            break;
        case 0x546:
            switch (SD.command) {
            case 11:
            case 12:
                sub_080561A0(SD.player, SD.sourceIndex, sub_08008A44(SD.player), 0, faceUp);
                SD.active = 0;
                break;
            default:
                goto normal_summon;
            }
            break;
        default:
        normal_summon:
            switch (SummonLevel(SD.cardId)) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
                if (special)
                    sub_080561A0(SD.player, SD.sourceIndex, sub_08008A44(SD.player), 0, faceUp);
                else
                    sub_08055B28(SD.player, SD.sourceIndex, sub_08008A44(SD.player), 0, faceUp);
                if (SUMMON_NUMBER(SD.cardId) == 0x5F0) {
                    switch (SD.command) {
                    case 11:
                    case 12:
                        if (sub_08044224(1 - SD.player, 0x447, 0) != 0
                            && sub_08008A1C(1 - SD.player) > 0
                            && (u16)sub_08047170(1 - SD.player) != 0) {
                            u32 event;
                            u32 playerBit = (1u & SD.player) << 31;
                            /* FAKEMATCH: extract the event field through r2 before masking into r1. */
                            register u32 eventField asm("r2") = SD.eventZone;
                            asm("" : "+r"(eventField)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            event = ((eventField & 0x1F) << 16) | 0x0E400000;
                            /* FAKEMATCH: combine the event word before the player/card bits. */
                            asm("" : "+r"(event));
                            sub_0801FBCC(playerBit | event | SD.cardId, 0);
                        }
                    }
                }
                SD.active = 0;
                break;
            case 5:
            case 6:
                sub_080602A4(0x206, 0x712, 0xB, gUnk_0819D1C4[0]);
                sub_08060308(1, 0, 0);
                SD.selected = 0;
                SD.step = 10;
                break;
            default:
                sub_080602A4(0x206, 0x712, 0xB, gUnk_0819D1C4[1]);
                sub_08060308(1, 0, 0);
                SD.selected = 0;
                SD.step++;
                break;
            }
            break;
        }
        break;
    case 1:
        if (gUnk_0201AE60.h14 == 0) {
            SD.active = 0;
            break;
        }
        SD.step++;
    case 2:
        sub_080602A4(0x206, 0x412, 0xB, gUnk_0819D1C4[2]);
        SD.step++;
        break;
    case 3:
        if (gUnk_03000040.keys & 2) {
            sub_080602A4(0x206, 0x712, 0xB, gUnk_0819D1C4[1]);
            sub_08060308(1, 0, 0);
            SD.selected = 0;
            SD.step = 1;
        } else if (sub_08052F38(0xF0)) {
            if (sub_08008A6C(SC.player, SC.zone)) {
                SD.selected = SUMMON_SELECTED_POS;
                sub_08077AEC(1);
                sub_0801EC58(8, SC.player, SUMMON_EVENT_POS, 0);
                SD.step++;
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 5:
        if (gUnk_03000040.keys & 2) {
            sub_080602A4(0x206, 0x712, 0xB, gUnk_0819D1C4[1]);
            sub_08060308(1, 0, 0);
            {
                struct StepBits { u16 lo:2; u16 step:8; u16 hi:6; u8 pad[4]; };
                register u8 *duel asm("r2") = (u8 *)&SD; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                register int selectedOffset asm("r3") = 0x1B2A;
                register int stepOffset asm("r4"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                register struct StepBits *step asm("r2");
                asm("" : : "r"(duel), "r"(selectedOffset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                {
                    register u16 *selected asm("r1") = (u16 *)((u32)duel + selectedOffset); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    asm("" : : "r"(selected));
                    *selected = 0;
                }
                stepOffset = 0x1B30;
                asm("" : : "r"(stepOffset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                step = (struct StepBits *)((u32)duel + stepOffset);
                {
                    register u32 mask asm("r0") = 0xFFFFFC03; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    register u16 value asm("r5");
                    asm("" : : "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    value = *(u16 *)step;
                    asm("" : : "r"(value)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    *(u16 *)step = (mask & value) | 4;
                }
            }
        } else if (sub_08052F38(0xF0)) {
            if (SD.selected != ((u8)SC.zone | ((u8)SC.player << 8)) && sub_08008A6C(SC.player, SC.zone)) {
                sub_08077AEC(1);
                sub_0801EC58(8, SC.player, SUMMON_EVENT_POS, 0);
                SD.selected = (SD.selected & 15) | (((u8)(SD.selected >> 8) & 15) << 4)
                    | (((SC.zone & 15) | ((SC.player & 15) << 4)) << 8);
                SD.step++;
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 6:
        if (special)
            sub_080561A0(SD.player, SD.sourceIndex, SD.selected & 7, SD.selected | 0x8080, faceUp);
        else
            sub_08055B28(SD.player, SD.sourceIndex, SD.selected & 7, SD.selected | 0x8080, faceUp);
        SD.active = 0;
        break;
    case 10:
        if (!gUnk_0201AE60.h14) {
            SD.active = 0;
            break;
        }
        sub_080602A4(0x206, 0x412, 0xB, gUnk_0819D1C4[4]);
        SD.step++;
        break;
    case 11:
        if (gUnk_03000040.keys & 2) {
            sub_080602A4(0x206, 0x712, 0xB, gUnk_0819D1C4[0]);
            sub_08060308(1, 0, 0);
            SD.selected = 0;
            SD.step = 10;
        } else if (sub_08052F38(0xF0)) {
            if (sub_08008A6C(SC.player, SC.zone)) {
                sub_08077AEC(1);
                sub_0801EC58(8, SC.player, SUMMON_EVENT_POS, 0);
                SD.selected = (SC.zone & 15) | ((SC.player & 15) << 4);
                SD.step++;
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 12:
        sub_08055B28(SD.player, SD.sourceIndex, SD.selected & 7, SD.selected | 0x80, faceUp);
        SD.active = 0;
        break;
    case 20:
        if (gUnk_03000040.keys & 2) {
            sub_08077AEC(2);
            SD.active = 0;
        } else if (sub_08052F38(0xF0)) {
            struct PlayerBits { u8 low:1; u8 player:1; u8 high:6; u8 pad[4]; };
            /* FAKEMATCH: retain the ROM duel-base and player-field lifetimes. */
            register struct SummonDuel *duel asm("r8");
            register struct PlayerBits *player asm("r6"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            ref.id = (duel = &SD)->cardId;
            ref.player = (player = (struct PlayerBits *)((u8 *)duel + 0x1B33))->player;
            if (sub_0802B2FC(&ref, SUMMON_POS)) {
                u32 message;
                sub_08077AEC(1);
                message = player->player ? 0x8008 : 8;
                sub_0801EC58(message, SC.player, SUMMON_EVENT_POS, 0);
                sub_08017FF4(player->player, SC.zone);
                sub_080561A0(player->player, duel->sourceIndex, SC.zone, 0, faceUp);
                duel->active = 0;
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 30: {
        register char *dst asm("r3") = text; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
        register const char *fmt asm("r4") = gUnk_0808563C;
        register const char *names asm("r5"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
        unsigned offset = SummonCardId(SD.sequence + 0x172) * 0x40;
        names = gUnk_0822C720;
        asm("" : : "r"(names)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
        sub_080753F4(dst, fmt, (const char *)(offset + (u32)names));
        promptA = 0x206;
        promptB = 0x712;
        promptC = 0xB;
        promptStr = text;
        asm("" : : "r"(promptA), "r"(promptB), "r"(promptC), "r"(promptStr)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
        goto normal_prompt;
    }
    case 31:
        if (sub_08052F38(0xE0)) {
            /* FAKEMATCH: load the cursor through r1 before retaining it in r8. */
            register struct SummonCursor *cursorBase asm("r1") = &SC;
            __asm__("" : : "r"(cursorBase)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            {
                /* FAKEMATCH: preserve the cursor's original high-register lifetime. */
                register struct SummonCursor *cursor asm("r8") = cursorBase;
                int player = cursor->player;
                /* FAKEMATCH: materialize the selected-zone address through r2. */
                register int *zoneBasePtr asm("r2") = &cursor->zone;
                __asm__("" : : "r"(zoneBasePtr)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                {
                    int *zonePtr = zoneBasePtr;
                    int zone = *zonePtr;
                    int pi = player & 1;
                    int offset = zone * 0x94 + pi * 0xD64;
                    u8 *zoneBase = (u8 *)gUnk_0201930C;
                    struct DuelZone *card = (struct DuelZone *)(offset + (int)zoneBase);
                    u32 id = ID(card);
                    if (id != 0 && (card->flags6 & 2)) {
                        unsigned index = (id & 0x7FF) * 2;
                        /* FAKEMATCH: retain the byte index and r3 table load before reading sequence. */
                        register const u16 *numbers asm("r3") = (const u16 *)0x08622AB4;
                        const u16 *numberPtr;
                        unsigned sequence;
                        __asm__("" : : "r"(index), "r"(numbers)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        numberPtr = (const u16 *)(index + (int)numbers);
                        sequence = SDZ.sequence;
                        /* FAKEMATCH: keep r3 occupied until the sequence offset has loaded through r0. */
                        __asm__("" : : "r"(numbers));
                        if (*numberPtr == sequence + 0x172) {
                            u32 message = 8;
                            if (player)
                                message = 0x8008;
                            {
                                u16 eventPlayer = player;
                                u8 *area = (u8 *)&cursor->area;
                                sub_0801EC58(message, eventPlayer, ((u8)zone << 8) | *area, 0);
                            }
                            sub_08017FF4(player, zone);
                            SDZ.sequence++;
                            if (SDZ.sequence <= 2) {
                                SDZ.step--;
                            } else {
                                sub_080561A0(SDZ.player, SDZ.sourceIndex, *zonePtr, 0, faceUp);
                                SDZ.active = 0;
                            }
                        } else {
                            sub_08077AEC(3);
                        }
                    }
                }
            }
        }
        break;
    /* State 0 enters with sequence=0; state 41 repeats while sequence<=2. */
    case 40:
        switch (SD.sequence) {
        case 0: required = 0x2E1; break;
        case 1: required = 0x2F4; break;
        case 2: required = 0x320; break;
        }
        SD.choices = 0;
        if (sub_08008524(0, 0x58A) == 0 && sub_08008524(1, 0x58A) == 0
            && sub_080086CC(0, required) > 0)
            SD.choices |= 2;
        if (sub_0800A2A8(0, required) && (SD.sequence <= 1 || sub_08008A1C(0) > 0))
            SD.choices |= 1;
        {
            register unsigned raw asm("r0") = ((struct { u8 prefix[0x1B30]; u32 choices; } *)&SD)->choices; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            register unsigned shifted asm("r2") = raw << 14;
            register unsigned choices asm("r1") = shifted >> 28; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            asm("" : : "r"(raw), "r"(shifted), "r"(choices));
            if (choices & 1) {
                /* FAKEMATCH: preserve both short-circuit tests and keep choices live. */
                asm("" : "+r"(choices));
                if (choices & 2) {
                    asm("" : : "r"(choices)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    {
                        register char *dst asm("r3") = text; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register const char *fmt asm("r4") = gUnk_08085660;
                        register u32 id asm("r0"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register unsigned offset asm("r2");
                        register const char *names asm("r5"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        asm("" : : "r"(dst), "r"(fmt));
                        if (required == 0xFFFF) {
                            id = 0;
                        } else if (required <= 0x7CF) {
                            register unsigned index asm("r0") = (required & 0x7FF) * 2; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register const u16 *map asm("r2");
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gUnk_08623DF4;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map);
                        } else {
                            register int adjustment asm("r5") = -0x7D0; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register unsigned index asm("r0");
                            register const u16 *map asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            asm("" : : "r"(adjustment));
                            index = ((required + adjustment) & 0x7FF) * 2;
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gUnk_08623DF4;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map) + 1;
                        }
                        asm("" : : "r"(id)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset = id << 16;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset >>= 10;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        names = gUnk_0822C720;
                        asm("" : : "r"(names)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        sub_080753F4(dst, fmt, (const char *)(offset + (u32)names));
                    }
                }
            }
        }
        {
            register unsigned raw asm("r0") = ((struct { u8 prefix[0x1B30]; u32 choices; } *)&SD)->choices; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            register unsigned shifted asm("r2") = raw << 14;
            register unsigned choices asm("r1") = shifted >> 28; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            asm("" : : "r"(raw), "r"(shifted), "r"(choices));
            if (choices & 1) {
                /* FAKEMATCH: preserve both short-circuit tests and keep choices live. */
                asm("" : "+r"(choices));
                if (!(choices & 2)) {
                    asm("" : : "r"(choices)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                    {
                        register char *dst asm("r3") = text; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register const char *fmt asm("r4") = gUnk_08085698;
                        register u32 id asm("r0"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register unsigned offset asm("r2");
                        register const char *names asm("r5"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        asm("" : : "r"(dst), "r"(fmt));
                        if (required == 0xFFFF) {
                            id = 0;
                        } else if (required <= 0x7CF) {
                            register unsigned index asm("r0") = (required & 0x7FF) * 2; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register const u16 *map asm("r2");
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gUnk_08623DF4;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map);
                        } else {
                            register int adjustment asm("r5") = -0x7D0; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register unsigned index asm("r0");
                            register const u16 *map asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            asm("" : : "r"(adjustment));
                            index = ((required + adjustment) & 0x7FF) * 2;
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gUnk_08623DF4;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map) + 1;
                        }
                        asm("" : : "r"(id)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset = id << 16;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset >>= 10;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        names = gUnk_0822C720;
                        asm("" : : "r"(names)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        sub_080753F4(dst, fmt, (const char *)(offset + (u32)names));
                    }
                }
            }
        }
        if (!(SD.choices & 1) && (SD.choices & 2))
            {
                        register char *dst asm("r3") = text; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register const char *fmt asm("r4") = gUnk_0808563C;
                        register u32 id asm("r0"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        register unsigned offset asm("r2");
                        register const char *names asm("r5"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        asm("" : : "r"(dst), "r"(fmt));
                        if (required == 0xFFFF) {
                            id = 0;
                        } else if (required <= 0x7CF) {
                            register unsigned index asm("r0") = (required & 0x7FF) * 2; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register const u16 *map asm("r2");
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gUnk_08623DF4;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map);
                        } else {
                            register int adjustment asm("r5") = -0x7D0; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            register unsigned index asm("r0");
                            register const u16 *map asm("r1"); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            asm("" : : "r"(adjustment));
                            index = ((required + adjustment) & 0x7FF) * 2;
                            asm("" : : "r"(index)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            map = gUnk_08623DF4;
                            asm("" : : "r"(map)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                            id = *(const u16 *)(index + (u32)map) + 1;
                        }
                        asm("" : : "r"(id)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset = id << 16;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        offset >>= 10;
                        asm("" : "+r"(offset)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        names = gUnk_0822C720;
                        asm("" : : "r"(names)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
                        sub_080753F4(dst, fmt, (const char *)(offset + (u32)names));
                    }
        if ((*(u32 *)((u8 *)&SD + 0x1B30) & 0x3C000) == 0) {
            SD.active = 0;
            break;
        }
        sub_080602A4(0x206, 0x712, 0xB, text);
        SD.step++;
        break;
    case 41: {
        int mask;
        switch (SD.sequence) {
        case 0: required = 0x2E1; break;
        case 1: required = 0x2F4; break;
        case 2: required = 0x320; break;
        }
        {
            register unsigned choiceBits asm("r1") = ((struct { u8 prefix[0x1B30]; u32 choices; } *)&SD)->choices << 14; /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            register unsigned choices asm("r0") = choiceBits >> 28;
            mask = 1;
            mask &= choices;
            asm("" : : "r"(choices), "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            choiceBits = choices;
            asm("" : : "r"(choiceBits), "r"(mask)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
            if (choiceBits & 2)
                mask |= 0xE0;
        }
        if (sub_08052F38(mask)) {
            int player = SC.player;
            int zone = SC.zone;
            int id = sub_0805ECFC();
            if (id) {
                if (SUMMON_NUMBER(id) == required) {
                    u32 message = SC.player ? 0x8008 : 8;
                    sub_0801EC58(message, SC.player, SUMMON_EVENT_POS, 0);
                    switch (SC.area) {
                    case 11: sub_080193D4(player, zone, 0, 0); break;
                    case 0: sub_08017FF4(player, zone); break;
                    }
                    SD.sequence++;
                    if (SD.sequence <= 2)
                        SD.step--;
                    else
                        SD.step++;
                } else {
                    sub_08077AEC(3);
                }
            }
        }
        break;
    }
    case 42:
        sub_080561A0(SD.player, SD.sourceIndex, sub_08008A44(SD.player), 0, faceUp);
        SD.active = 0;
        break;
    case 50:
        if (SUMMON_HUMAN(SC.player))
            sub_080753F4(format, gUnk_080856BC, gUnk_08085708);
        else
            sub_080753F4(format, gUnk_08085710, gUnk_08085708);
        sub_08075434(text, format, 3);
        sub_080602A4(0x206, 0x712, 0xB, text);
        SD.step++;
        break;
    case 51:
    case 54:
    case 57:
        if (!SUMMON_HUMAN(SC.player)) {
            sub_0802AF34(SC.player, -1, SUMMON_NUMBER(SDP.cardId), 0);
            SDP.step++;
        } else if (sub_08052F38(0xF0)) {
            if (sub_0800C8BC(SC.player, SC.zone) == 3) {
                sub_080189FC(SC.player, SC.zone, 0);
                SDP.step++;
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 52:
    case 55:
        if (!SUMMON_HUMAN(SC.player))
            sub_0801965C(SC.player, &gUnk_0201D810.cards[gUnk_0201D810.scroll + gUnk_0201D810.row]);
        SDP.step++;
        break;
    case 53:
        sub_08075434(text, gUnk_08085758, 2);
        sub_080602A4(0x206, 0x412, 0xB, text);
        SD.step++;
        break;
    case 56:
        sub_08075434(text, gUnk_08085758, 1);
        sub_080602A4(0x206, 0x412, 0xB, text);
        SD.step++;
        break;
    case 58:
        if (!SUMMON_HUMAN(SC.player))
            sub_0801965C(SC.player, &gUnk_0201D810.cards[gUnk_0201D810.scroll + gUnk_0201D810.row]);
        SDP.step = 73;
        break;
    case 60:
        if (SUMMON_HUMAN(SC.player))
            sub_080753F4(format, gUnk_080856BC, gUnk_0808577C);
        else
            sub_080753F4(format, gUnk_08085710, gUnk_0808577C);
        sub_08075434(text, format, 2);
        sub_080602A4(0x206, 0x712, 0xB, text);
        SD.step++;
        break;
    case 61:
    case 64:
        if (!SUMMON_HUMAN(SC.player)) {
            sub_0802AF34(SC.player, -1, SUMMON_NUMBER(SDP.cardId), 0);
            SDP.step++;
        } else if (sub_08052F38(0xF0)) {
            if (sub_0800CAF0(SC.player, SC.zone) == 1) {
                sub_080189FC(SC.player, SC.zone, 0);
                SDP.step++;
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 62:
        if (!SUMMON_HUMAN(SC.player))
            sub_0801965C(SC.player, &gUnk_0201D810.cards[gUnk_0201D810.scroll + gUnk_0201D810.row]);
        SDP.step++;
        break;
    case 63:
        sub_08075434(text, gUnk_08085758, 1);
        sub_080602A4(0x206, 0x412, 0xB, text);
        SD.step++;
        break;
    case 65:
        if (!SUMMON_HUMAN(SC.player))
            sub_0801965C(SC.player, &gUnk_0201D810.cards[gUnk_0201D810.scroll + gUnk_0201D810.row]);
        SDP.step = 73;
        break;
    case 70:
        switch (SUMMON_NUMBER(SD.cardId)) {
        case 0x5EC:
            if (((s32)((u32)SUMMON_HEADER(SC.player & 1).flags10 << 26) < 0))
                sub_080753F4(text, gUnk_08085784, gUnk_080857C4);
            else
                sub_080753F4(text, gUnk_080857CC, gUnk_080857C4);
            break;
        case 0x5ED:
            if (((s32)((u32)SUMMON_HEADER(SC.player & 1).flags10 << 26) < 0))
                sub_080753F4(text, gUnk_08085784, gUnk_08085804);
            else
                sub_080753F4(text, gUnk_080857CC, gUnk_08085804);
            break;
        case 0x5EE:
            if (((s32)((u32)SUMMON_HEADER(SC.player & 1).flags10 << 26) < 0))
                sub_080753F4(text, gUnk_08085784, gUnk_0808580C);
            else
                sub_080753F4(text, gUnk_080857CC, gUnk_0808580C);
            break;
        case 0x5EF:
            if (((s32)((u32)SUMMON_HEADER(SC.player & 1).flags10 << 26) < 0))
                sub_080753F4(text, gUnk_08085784, gUnk_08085814);
            else
                sub_080753F4(text, gUnk_080857CC, gUnk_08085814);
            break;
        }
        promptA = 0x206;
        promptB = 0x712;
        promptC = 0xB;
        promptStr = text;
        asm("" : : "r"(promptA), "r"(promptB), "r"(promptC), "r"(promptStr)); /* FAKEMATCH: preserve the ROM temporary allocation/lifetime. */
    normal_prompt:
        sub_080602A4(promptA, promptB, promptC, promptStr);
        SD.step++;
        break;
    case 71:
        if (!SUMMON_HUMAN(SC.player)) {
            sub_0802AF34(SC.player, -1, SUMMON_NUMBER(SDP.cardId), 0);
            SDP.step++;
        } else if (sub_08052F38(0xF0)) {
            int kind = 0;
            switch (SUMMON_NUMBER(SDP.cardId)) {
            case 0x5EC: kind = 4; break;
            case 0x5ED: kind = 3; break;
            case 0x5EE: kind = 5; break;
            case 0x5EF: kind = 6; break;
            }
            if (sub_0800CAF0(SC.player, SC.zone) == kind) {
                sub_080189FC(SC.player, SC.zone, 0);
                SD.step++;
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 72:
        if (!SUMMON_HUMAN(SC.player))
            sub_0801965C(SC.player, &gUnk_0201D810.cards[gUnk_0201D810.scroll + gUnk_0201D810.row]);
        SDP.step++;
        break;
    case 73:
        sub_080561A0(SD.player, SD.sourceIndex, sub_08008A44(SD.player), 0, faceUp);
        SD.active = 0;
        break;
    case 80: {
        int player = SD.player;
        int number1 = 0x582;
        u16 first = sub_08008794(player, number1) != 0;
        /* FAKEMATCH: mix equivalent table-base forms to avoid a cached pointer. */
        int player2 = SD.player;
        int number2 = 0x584;
        u16 second = sub_08008794(player2, number2) != 0;
        if (first) {
            if (second) {
                sub_080753F4(format, gUnk_0808581C, SUMMON_CARD_NAME(((const u16 *)0x08623DF4)[number1]));
                sub_080753F4(text, format, SUMMON_CARD_NAME(*(gUnk_08623DF4 - -number2)));
                sub_080602A4(0x206, 0x712, 0xB, text);
                SD.step++;
            } else {
                sub_080753F4(text, gUnk_08085850, SUMMON_CARD_NAME(((const u16 *)0x08623DF4)[number1]));
                /* FAKEMATCH: retain the ROM name-table register lifetime. */
                asm("" : : "r"(gUnk_0822C720));
                sub_080602A4(0x206, 0x712, 0xB, text);
                SD.step++;
            }
        } else if (second) {
            sub_080753F4(text, gUnk_08085850, SUMMON_CARD_NAME(((const u16 *)0x08623DF4)[number2]));
            /* FAKEMATCH: retain the ROM name-table register lifetime. */
            asm("" : : "r"(gUnk_0822C720));
            sub_080602A4(0x206, 0x712, 0xB, text);
            SD.step++;
        } else {
            SD.active = 0;
        }
        break;
    }
    case 81:
        if (gUnk_03000040.keys & 2) {
            SD.step = 80;
        } else if (sub_08052F38(0xF0)) {
            if (sub_08008A6C(SC.player, SC.zone)) {
                /* The ROM uses cursor.area for this player term; preserve it. */
                int player = SC.area & 1;
                int zone = SC.zone;
                u32 word = *(u32 *)(player * 0xD64 + zone * 0x94 + (u32)gUnk_0201930C);
                u16 number = *(u16 *)((u8 *)gUnk_08622AB4 + ((word << 21) >> 20));
                if (number == 0x582 || number == 0x584) {
                    SDZ.selected = SC.zone;
                    /* FAKEMATCH: retain the number through the selection store. */
                    asm("" : : "r"(number));
                    sub_08077AEC(1);
                    sub_0801EC58(8, SC.player, SUMMON_EVENT_POS, 0);
                    SDZ.step++;
                } else {
                    sub_08077AEC(3);
                }
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 4:
    case 82:
        sub_080602A4(0x206, 0x412, 0xB, gUnk_0819D1C4[3]);
        SD.step++;
        break;
    case 83:
        if (gUnk_03000040.keys & 2) {
            SD.step = 80;
        } else if (sub_08052F38(0xF0)) {
            if ((u8)SD.selected != (u8)SC.zone && sub_08008A6C(SC.player, SC.zone)) {
                sub_08077AEC(1);
                sub_0801EC58(8, SC.player, SUMMON_EVENT_POS, 0);
                SD.selected = ((u8)SC.zone << 8) | (u8)SD.selected;
                SD.step++;
            } else {
                sub_08077AEC(3);
            }
        }
        break;
    case 84:
        sub_08017FF4(SD.player, (u8)SD.selected);
        sub_08017FF4(SD.player, SD.selected >> 8);
        sub_080561A0(SD.player, SD.sourceIndex, (u8)SD.selected & 7, 0, faceUp);
        SD.active = 0;
        break;
    default:
        SD.active = 0;
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08046738", sub_080471E8); /* 0x080471E8 size 0x1DF8 */

