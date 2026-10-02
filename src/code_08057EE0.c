#include "global.h"

#include "gba.h"

/* Duel AI, part 2. See wiki/functions/code-08057ee0.md. */

/* A card instance word: low 12 bits = card id (0 = none). */
struct DuelCard {
    u32 id : 12;
    u32 unk12 : 1;
    u32 unk13 : 19;
};

/* One field zone, 0x94 bytes, 11 per player starting at player+0x28 (0x0201930C). */
struct DuelZone {
    struct DuelCard card;   /* +0x00 */
    u8 unk4[2];
    u8 f6_0 : 1;            /* +0x06 bit 0 = face-up? */
    u8 f6_1 : 1;            /* bit 1 = temporary evaluation flag */
    u8 f6_rest : 6;
    u8 f7_0 : 1;            /* +0x07 */
    u8 f7_1 : 1;
    u8 f7_2 : 1;
    u8 f7_3 : 1;
    u8 f7_rest : 4;
    u8 filler8[2];
    u16 links[32];          /* +0x0A */
    u16 linkKinds[32];      /* +0x4A */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[5];
    u8 unk91;
    u8 filler92[2];
};

/* Per-player duel state, 0xD64 bytes, two of them at 0x020192E4. */
struct DuelPlayer {
    u16 lp;                         /* +0x000 life points (hypothesis) */
    u8 handCount;                   /* +0x002 */
    u8 deckCount;                   /* +0x003 */
    u8 count904;                    /* +0x004 */
    u8 fusionCount;                 /* +0x005 */
    u8 countB84;                    /* +0x006 */
    u8 unk7[2];
    u8 unk9_0 : 1;
    u8 unk9_1 : 7;
    u8 fillerA[0x1C];
    u16 zoneMask;                   /* +0x026 per-zone bitmask */
    struct DuelZone zones[11];      /* +0x028 */
    struct DuelCard hand[80];       /* +0x684 */
    struct DuelCard deck[80];       /* +0x7C4 */
    struct DuelCard list904[80];    /* +0x904 */
    struct DuelCard fusionDeck[80]; /* +0xA44 */
    struct DuelCard listB84[80];    /* +0xB84 */
    u16 arrCC4[80];                 /* +0xCC4 */
};
extern struct DuelPlayer gUnk_020192E4[2];

struct DuelZonesPlayer {
    struct DuelZone zones[11];
    u8 filler[0xD64 - 11 * 0x94];
};
extern struct DuelZonesPlayer gUnk_0201930C[2];
/* Zone pointer by byte arithmetic, zone term first (the ROM's address order); callers pass player & 1. */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))

#define CARD_WORD(c) (*(u32 *)&(c))
#define CARD_ID(w) (((w) << 20) >> 20)
/* ROM tables through integer-constant pointers (the ROM reloads the table address at every use). */
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
/* Same tables through the extern symbols (the address is then a hoistable constant). */
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];
#define CARD_STATS_A(id) (gUnk_08621DE0[(id) & 0x7FF])
#define CARD_NUMBER_A(id) (gUnk_08622AB4[(id) & 0x7FF])
#define CARD_TYPE_A(id) ((CARD_STATS_A(id) & 0x1F00000) >> 20)
/* One attack option evaluated by the AI (8 bytes, copied with sub_08075294). */
struct AttackPlan {
    u16 f0 : 1;
    u16 f1 : 1;
    u16 f2 : 1;
    u16 lose : 1;               /* bit 3: target would not be beaten (own value < opponent's) */
    u16 src : 3;                /* bits 4-6: attacking zone */
    u16 dst : 3;                /* bits 7-9: target zone */
    u16 gt : 1;                 /* bit 10 */
    u16 le : 1;                 /* bit 11 */
    u16 rest : 4;
    u16 unk2;                   /* +0x02 */
    s16 diff;                   /* +0x04 */
    u16 unk6;
};
void sub_08075294(void *dest, const void *src, u32 size);

struct AiWork {
    u8 filler0[0xC];
    struct AttackPlan best;
    u8 filler14[0x1B20 - 0x14];
    union {
        struct {
            u8 cntA : 3;        /* +0x1B20 bits 0-2 */
            u8 cntB : 3;        /* bits 3-5 */
            u8 cntRest : 2;
        } b;
        struct {
            u16 a : 3;
            u16 b : 3;
            u16 c : 3;          /* bits 6-8 */
            u16 d : 3;          /* bits 9-11 */
            u16 rest : 4;
        } h;
    } fl;
};
extern struct AiWork gUnk_02015F00;
int sub_08008A44(int player);
int sub_0804A92C(int player);
void sub_0804A848(void *p, int a, int b, int c);
void sub_08057E08(void);
void sub_08057E3C(void);
void sub_08057E70(void);
u16 sub_08057C94(void);
int sub_08057F6C(void);
void sub_08057EE0(int hand, u16 mask);
int sub_080563B8(int a, int b);
int sub_080577FC(void);
int sub_08057854(void);
int sub_080578AC(void);
struct Unk02015EE8 {
    u32 unk0;
    u32 flags;                  /* +0x04 bit 9 */
};
extern struct Unk02015EE8 gUnk_02015EE8;

struct AiFlagsB1 {
    u8 pad : 1;
    u8 d : 3;                   /* byte +0x1B21 bits 1-3 */
    u8 rest : 4;
    u8 pad2[3];
};
extern struct AiFlagsB1 gUnk_02017A21;
int sub_08054398(int player, int id);
int sub_08056300(int a, int number);
int sub_08007834(int id);
int sub_08008860(int player);

/* Cost class of a card for the AI: 0 for Magic/Trap/Ritual, 10 for type 0x18, else a 4-bit field of the stats word. */
static inline u32 CardCost(u16 id)
{
    u32 r;

    switch ((int)CARD_TYPE(id)) {
    case 0x15:
    case 0x16:
    case 0x17:
        r = 0;
        break;
    case 0x18:
        r = 10;
        break;
    default:
        r = (CARD_STATS(id) & 0x1E000000) >> 25;
        break;
    }
    return r;
}

struct AiFlagsByte {
    u8 a : 3;
    u8 b : 3;                   /* bits 3-5 */
    u8 c : 2;
    u8 pad[3];
};
extern struct AiFlagsByte gUnk_02017A20;

union AiFlagsU {
    struct {
        u8 a : 3;
        u8 b : 3;
        u8 c : 2;
    } b;
    struct {
        u16 a : 3;
        u16 b : 3;
        u16 c : 3;              /* bits 6-8 */
        u16 d : 3;
        u16 rest : 4;
    } h;
};

/* Clears the card of every player-1 zone named by a nibble of `mask`, then puts hand card `hand` into a new zone (marked f6_1). */
void sub_08057EE0(int hand, u16 mask)
{
    struct DuelZone *z;
    int i;
    register int zone __asm__("r2");

    /* FAKEMATCH: keep the mask out of r2, which the ROM uses for the counter. */
    __asm__ __volatile__("" : : "r"(mask) : "r2");
    for (i = 0; i < 4; i++) {
        if (mask & 0xF)
            ZB(1, mask & 7)->card.id = 0;
        mask >>= 4;
    }
    zone = sub_08008A44(1);
    /* FAKEMATCH: retain the ROM's r2 copy before the stride multiplication. */
    __asm__ __volatile__("" : : "r"(zone));
    z = ZB(1, zone);
    z->card.id = CARD_ID(((u32 *)((u32)ZB(1, 0) + 0x65C))[hand]);
    z->f6_1 = 1;
    z->f6_0 = 0;
}

/* Repeatedly picks the best attack (sub_08057C94) and accumulates the damage; returns the total. */
int sub_08057F6C(void)
{
    int total = 0;

    gUnk_02015F00.fl.b.cntA = 0;
    gUnk_02015F00.fl.b.cntB = 0;
    if (sub_0804A92C(1)) {
        sub_0804A848(gUnk_020192E4, 1, 0, 0);
        while (sub_08057C94()) {
            gUnk_020192E4[1].zoneMask |= 1 << gUnk_02015F00.best.src;
            if (gUnk_02015F00.best.gt) {
                ((struct DuelZone *)(gUnk_02015F00.best.gt * 0x94 + ((u32)gUnk_020192E4 + 0xD8C)))->card.id = 0;
                gUnk_02015F00.fl.b.cntA++;
            }
            if (gUnk_02015F00.best.le) {
                ((struct DuelZone *)(gUnk_02015F00.best.le * 0x94 + ((u32)gUnk_020192E4 + 0x28)))->card.id = 0;
                gUnk_02015F00.fl.b.cntB++;
            }
            total += gUnk_02015F00.best.diff;
        }
    }
    return total;
}
/* Saves the duel state, optionally drops unplayable zones, runs the attack simulation, restores. */
int sub_08058074(u16 drop)
{
    int r;

    sub_08057E08();
    if (drop != 0)
        sub_08057E70();
    r = sub_08057F6C();
    sub_08057E3C();
    return r;
}
/* Same, but the simulation step is ChooseAttacker; returns its u16 result. */
/* Expose the original zero-extended result to word-valued AI callers. */
int sub_0805809C(u16 drop)
{
    u16 r;

    sub_08057E08();
    if (drop != 0)
        sub_08057E70();
    r = sub_08057C94();
    sub_08057E3C();
    return r;
}

/* Byte view of the AI work area flag byte +0x1B21 (the ROM reaches it through gUnk_02015F00). */
struct AiWorkB21x {
    u8 filler[0x1B21];
    u8 pad : 1;
    u8 d : 3;
    u8 rest : 4;
};

/* Like sub_08058358, but cards of cost class 5-6 or 0/>=7 also pick extra cards (sub_080563B8), packed
   into the mask passed to sub_08057EE0; returns the chosen hand index (-1 if none), *out = best total. */
int sub_080580C8(int *out)
{
    int best = -1;
    int bestCnt = 0;
    int base;
    int i;

    sub_08057E08();
    sub_08057E70();
    base = sub_08057F6C();
    sub_08057E3C();
    for (i = 0; i < gUnk_020192E4[1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1].hand[i]));
        int bad;
        int flag;
        u16 mask;
        int r;
        int a, b;
        u16 c;

        if (!sub_08054398(1, id))
            continue;
        if (sub_08056300(2, CARD_NUMBER(id)))
            continue;
        if (sub_08007834(id))
            continue;
        bad = 0;
        flag = 0;
        mask = 0;
        switch ((int)CardCost(id)) {
        case 1:
        case 2:
        case 3:
        case 4:
            break;
        case 5:
        case 6:
            a = sub_080563B8(-1, 0);
            if (a == -1)
                bad = 1;
            mask = a | 8;
            break;
        default:
            a = sub_080563B8(-1, 0);
            b = sub_080563B8(a, 0);
            if (a == -1 || b == -1)
                bad = 1;
            c = (b & 7) | 8;
            mask = (((u32)a << 20) >> 16) | c;
            break;
        }
        if (bad)
            continue;
        sub_08057E08();
        sub_08057EE0(i, mask);
        sub_08057E70();
        r = sub_08057F6C();
        gUnk_02015F00.fl.h.c = sub_08008860(1);
        ((struct AiWorkB21x *)&gUnk_02015F00)->d = sub_08008860(1);
        sub_08057E3C();
        if (base < r)
            flag = 1;
        if (r == base && bestCnt < gUnk_02015F00.fl.b.cntB && gUnk_02015F00.fl.h.c != 0)
            flag = 1;
        if (gUnk_02015EE8.flags & 0x200) {
            switch (CARD_NUMBER(id)) {
            case 0x2F:
            case 0x23D:
            case 0x463:
                if (gUnk_020192E4[1].lp + r > 1000 && sub_080577FC() > 0 && sub_08057854() == 0 && sub_080578AC() == 0)
                    flag = 1;
                break;
            }
        }
        if (flag) {
            if (r > 0)
                base = r;
            bestCnt = gUnk_02015F00.fl.b.cntB;
            best = i;
        }
    }
    *out = base;
    return best;
}

/* Byte view of the AI work area's flag byte +0x1B21 (the high byte of fl). */
struct AiWorkB21 {
    u8 filler[0x1B21];
    u8 pad : 1;
    u8 d : 3;                   /* +0x1B21 bits 1-3 */
    u8 rest : 4;
};

/* Chooses which hand card of player 1 to play by simulating each (sub_08057F6C); returns its index or -1. */
int sub_08058358(void)
{
    int best = -1;
    int bestCnt = 0;
    int base;
    int i;

    sub_08057E08();
    sub_08057E70();
    base = sub_08057F6C();
    sub_08057E3C();
    for (i = 0; i < gUnk_020192E4[1].handCount; i++) {
        u16 id = CARD_ID(CARD_WORD(gUnk_020192E4[1].hand[i]));
        int bad;
        int flag;
        int r;

        if (!sub_08054398(1, id))
            continue;
        if (sub_08056300(2, CARD_NUMBER(id)))
            continue;
        if (sub_08007834(id))
            continue;
        bad = 0;
        flag = 0;
        if (CardCost(id) > 4)
            bad = 1;
        if (bad)
            continue;
        sub_08057E08();
        sub_08057EE0(i, 0);
        sub_08057E70();
        r = sub_08057F6C();
        gUnk_02015F00.fl.h.c = sub_08008860(1);
        ((struct AiWorkB21 *)&gUnk_02015F00)->d = sub_08008860(1);
        sub_08057E3C();
        if (base < r)
            flag = 1;
        if (r == base && bestCnt < gUnk_02015F00.fl.b.cntB && gUnk_02015F00.fl.h.c != 0)
            flag = 1;
        if (flag) {
            if (r > 0)
                base = r;
            bestCnt = gUnk_02015F00.fl.b.cntB;
            best = i;
        }
    }
    return best;
}
int sub_080573D0(int p, int skip, int useAtk, int useDef);
int sub_08076F9C(void);
int sub_08056E04(int player, int number);
int sub_080088A4(int a, int b, int c);
int sub_0800C894(int player, int zone);
int sub_0805761C(int player);
int sub_08007730(int id);
int sub_08009298(int player, int zone);

/* Card as seen by the AI's "should I use this card now" test: word 0 = card id, +6 = a player/zone selector. */
struct AiCardRef {
    u16 id;
    u16 unk2[2];
    u16 sel;                    /* +0x06 */
};

static inline int CallZoneZero(int zone)
{
    int zero = 0;
    /* Set r1 before the index is copied into r0, as in both ROM calls. */
    __asm__ __volatile__("" : : "r"(zero) : "r0");
    return sub_0800C894(zone, zero);
}
/* AI: decides whether card `c` (NULL = no) is worth playing now; a big switch over the card number. */
int sub_08058514(struct AiCardRef *c)
{
    int a;
    int r;
    int z;
    int zone;
    int zz;

    if (c == 0)
        return 0;
    switch (CARD_NUMBER(c->id)) {
    case 0x2A8:
    case 0x2A9:
    case 0x2AD:
    case 0x3C0:
    case 0x3CA:
    case 0x3DE:
    case 0x3EA:
    case 0x447:
    case 0x44A:
    case 0x44B:
    case 0x474:
    case 0x475:
    case 0x47B:
    case 0x47F:
    case 0x4B3:
    case 0x4B4:
    case 0x517:
    case 0x518:
    case 0x52B:
    case 0x52D:
    case 0x587:
    case 0x58F:
    case 0x590:
    case 0x5A7:
    case 0x5F8:
    case 0x600:
        return 1;
    case 0x3AB:
        a = sub_080573D0(1, -1, 1, 0);
        if (a > -1 && a < gUnk_020192E4[1].lp && a > gUnk_020192E4[0].lp)
            return 1;
        a = sub_080573D0(0, -1, 1, 0);
        if (a <= -1)
            return 0;
        if (a >= gUnk_020192E4[1].lp)
            return 0;
        if (a > gUnk_020192E4[0].lp)
            return 1;
        if (gUnk_020192E4[0].lp - a < gUnk_020192E4[1].lp)
            return 1;
        return 0;
    case 0x408:
        if (sub_08008860(0) * 500 >= gUnk_020192E4[0].lp)
            return 1;
        {
            int t = sub_08008860(0);

            if (t > sub_08076F9C() % 3 + 1)
                return 1;
        }
        return 0;
    case 0x40E:
        if (sub_08056E04(1, 0x4C5))
            return 1;
        r = sub_08056E04(1, 0x42F);
        goto nonzero;
    case 0x444:
        {
            int t = sub_080088A4(0, 1, 0);

            t += sub_080088A4(1, 1, 0);
            if (t > 2)
                return 1;
        }
        return 0;
    case 0x420:
        zone = c->sel;
        if (CallZoneZero(zone) >= gUnk_020192E4[1].lp)
            return 1;
        if (sub_08008860(1) == 0 && sub_08008860(0) > 1)
            return 1;
        a = sub_0805761C(zone);
        if (a >= gUnk_020192E4[1].lp)
            return 1;
        if (a >= sub_0805761C(1 - zone))
            return 1;
        return 0;
    case 0x47E:
        a = 0;
        for (zone = 0; zone <= 1; zone++) {
            for (z = 0; z <= 4; z++) {
                u32 id = CARD_ID(CARD_WORD(((struct DuelZone *)((zone & 1) * 0xD64 + z * 0x94 + (u32)gUnk_0201930C))->card));

                if (id != 0 && sub_08007730(id))
                    a++;
            }
        }
        if (gUnk_020192E4[0].lp < a * 500)
            return 1;
        a = 0;
        for (z = 0; z <= 4; z++) {
            if (CARD_ID(CARD_WORD(ZB(0, z)->card)) != 0 && sub_08007730(CARD_ID(CARD_WORD(ZB(0, z)->card))))
                a++;
            if (CARD_ID(CARD_WORD(ZB(1, z)->card)) != 0 && sub_08007730(CARD_ID(CARD_WORD(ZB(1, z)->card))))
                a--;
        }
        if (a > 1)
            return 1;
        return 0;
    case 0x49A:
        for (zz = 0; zz <= 4; zz++) {
            struct DuelZone *dz = ZB(1, zz);

            if ((CARD_WORD(dz->card) << 20) != 0 && (dz->f6_1)) {
                if (sub_08009298(1, zz) > 0)
                    return 1;
            }
        }
        return 0;
    case 0x4BE:
        zone = c->sel;
        a = CallZoneZero(zone);
        if (a >= gUnk_020192E4[0].lp / 2)
            return 1;
        if (a >= gUnk_020192E4[1].lp)
            return 1;
        if (gUnk_020192E4[0].lp - a > gUnk_020192E4[1].lp)
            return 1;
        if (sub_08008860(0) == 1)
            return 1;
        return 0;
    case 0x591:
        r = sub_08008860(0);
    nonzero:
        if (r != 0)
            return 1;
        return 0;
    }
    return 0;
}
/* Opponent-side message/request block filled by the caller (fields +3, +6, +8 used). */
struct AiReq {
    u16 id;                     /* +0x00 card id */
    u8 side : 1;                /* +0x02 bit 0 = player/side flag */
    u8 rest2 : 7;
    u8 b3;
    u8 unk4[2];
    u16 h6;
    u16 h8;
    u8 unkA[2];
    u16 hC;                     /* +0x0C low byte = player, high byte = zone (hypothesis) */
};
void sub_0801EC58(int a, int b, int c, int d);
void sub_08018DC8(int a, int b, int c);
void sub_0801FBE0(u32 a, u32 b);

/* Finds the first eligible player-1 monster zone (5..9) holding card `number`, queues its activation; returns 1 if found. */
u16 sub_08058924(struct AiReq *req, int number)
{
    int i;

    for (i = 5; i <= 9; i++) {
        struct DuelZone *z = ZB(1, i);
        u32 id = CARD_ID(CARD_WORD(z->card));

        if (id != 0 && !z->f6_1 && (z->unk91 & 4) && CARD_NUMBER(id) == number) {
            u32 hi;
            u32 mid;

            sub_0801EC58(0x8007, 0, 0, 0);
            sub_08018DC8(1, i, 0);
            hi = req->b3 >> 2 << 25;
            mid = (i & 0x1F) << 16;
            mid |= 0x80200000;
            sub_0801FBE0(hi | mid | CARD_ID(CARD_WORD(z->card)), (req->h8 << 16) | req->h6);
            return 1;
        }
    }
    return 0;
}
int sub_08008B70(int a, int b, int c, int d);

#if 0 /* NONMATCHING (score 31): NONMATCHING (score 31): only the 0x3F1/0x437 case differs: ROM copies h (ldrh
       * r1; adds r3,r1,#0), ands with a copy of the constant 1 (adds r2,r4,#0; ands r2,r3) and zero-extends (u8)h with
       * lsl/lsr #24 after the branch; this draft loads the low byte with ldrb instead. Key facts: integer-constant table
       * form; 0x7FF mask in a u32 m reused as the 0x40E index chain (m &= side; m <<= 1; m += table) FAKEMATCH; zone var
       * reused for the 0x29F loop; ZP macro with player term first; x shared by 0x3F1 and 0x40E. */
#define ZP(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))
/* AI: second "should I play this card" test (same interface as sub_08058514): a switch over the card number that
   checks life-point thresholds / zone contents and asks sub_08058924 whether a matching monster is on the field. */
int sub_080589C8(struct AiReq *c)
{
    int a;
    int z;
    int zone;
    int num;
    int x;
    u32 m;

    m = 0x7FF;
    num = ((const u16 *)0x08622AB4)[c->id & m];
    switch (num) {
    case 0x149:
    case 0x14A:
    case 0x14B:
    case 0x14C:
    case 0x14D:
    case 0x14E:
    case 0x151:
    case 0x152:
    case 0x153:
    case 0x154:
    case 0x155:
    case 0x3EE:
    case 0x42D:
    case 0x465:
    case 0x466:
    case 0x467:
    case 0x468:
    case 0x469:
    case 0x46A:
        return 0;
    case 0x14F:
        if (c->side)
            return 0;
        if (sub_08058924(c, 0x3FB))
            return 1;
        break;
    case 0x150:
        if (c->side)
            return 0;
        if (sub_08058924(c, 0x3FE))
            return 1;
        break;
    case 0x15B:
        a = 0;
        for (zone = 0; zone <= 1; zone++) {
            for (z = 0; z <= 4; z++) {
                u32 id = CARD_ID(CARD_WORD(((struct DuelZone *)((zone & 1) * 0xD64 + z * 0x94 + (u32)gUnk_0201930C))->card));

                if (id != 0 && sub_08007730(id))
                    a++;
            }
        }
        if (gUnk_020192E4[0].lp < a * 500) {
            if (sub_08058924(c, 0x47E))
                return 1;
        }
        if (a > sub_08076F9C() % 3 + 1) {
            if (sub_08058924(c, 0x47E))
                return 1;
        }
        break;
    case 0x3F0:
        if (c->side)
            return 0;
        if (sub_08058924(c, 0x3FD))
            return 1;
        if (sub_08058924(c, 0x402))
            return 1;
        break;
    case 0x29F:
        if (c->side)
            return 0;
        if (!sub_08008B70(1, 0, 0, 0))
            return 0;
        if (sub_08058924(c, 0x426))
            return 1;
        if (sub_08008B70(1, 0, 0, 0) > 1 && gUnk_020192E4[1].handCount != 0 && sub_08058924(c, 0x405))
            return 1;
        for (zone = 5; zone <= 9; zone++) {
            struct DuelZone *dz = ZB(1, zone);
            u16 id = CARD_ID(CARD_WORD(dz->card));

            if (dz->f6_1)
                continue;
            switch (CARD_NUMBER(id)) {
            case 0x3F7:
            case 0x3F8:
            case 0x433:
            case 0x434:
            case 0x43A:
            case 0x4B3:
            case 0x4B4:
            case 0x5A7:
                if (sub_08058924(c, CARD_NUMBER(id)))
                    return 1;
                break;
            }
        }
        return 0;
    case 0x3F1:
    case 0x437: {
        u16 h;
        int pl;

        if (c->side)
            return 0;
        h = c->hC;
        pl = (u8)h;
        if ((CARD_WORD(ZP(h & 1, h >> 8)->card) << 20) == 0)
            break;
        x = CARD_NUMBER(CARD_ID(CARD_WORD(ZP(pl & 1, h >> 8)->card)));
        if (x == 0x136 || x == 0x405)
            return 0;
        break;
    }
    case 0x40E:
        m &= c->side;
        m <<= 1;
        m += 0x08622AB4;
        x = *(u16 *)m;
        if (x == 0x24E)
            return 1;
        if (x != 0x4C5)
            return 0;
        if (gUnk_020192E4[0].handCount != 0)
            return 1;
        return 0;
    case 0xC8:
        if (gUnk_020192E4[1].lp > 200)
            return 0;
        break;
    case 0xE3:
        if (gUnk_020192E4[1].lp > 500)
            return 0;
        break;
    case 0xE4:
        if (gUnk_020192E4[1].lp > 600)
            return 0;
        break;
    case 0xE5:
        if (gUnk_020192E4[1].lp > 800)
            return 0;
        break;
    case 0xE6:
        if (gUnk_020192E4[1].lp > 1000)
            return 0;
        break;
    case 0x3EF:
        if (gUnk_020192E4[1].lp > 300)
            return 0;
        break;
    case 0x40F:
        if (gUnk_020192E4[1].lp > gUnk_020192E4[1].handCount * 200)
            return 0;
        break;
    }
    switch ((int)CARD_TYPE(c->id)) {
    case 0x16:
        if (!(c->side)) {
            if (sub_08058924(c, 0x482))
                return 1;
            if (gUnk_020192E4[1].handCount != 0) {
                if (sub_08058924(c, 0x405))
                    return 1;
            }
        }
        break;
    case 0x15:
        if (!(c->side)) {
            if (sub_08058924(c, 0x409))
                return 1;
            if (gUnk_020192E4[1].lp > 1000) {
                if (sub_08058924(c, 0x406))
                    return 1;
            }
        }
        break;
    }
    if (sub_08058924(c, 0x5A7))
        return 1;
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08057EE0", sub_080589C8); /* 0x080589C8 size 0x514 */
extern const u16 gUnk_08623DF4[];
void sub_08007558(void *dst, void *src);
#define PL(p) gUnk_020192E4[(p) & 1]

/* Adds the card encoded by `w` (0xFFFF = none, 0..0x7CF, or 0x7D0.. = second table + 1) to the front of a list of
   player `player`: the fusion list (+0xA44, count +5) for kind 2 cards, otherwise the deck (+0x7C4, count +3). */
static inline u16 CardNumberToId58(u16 no)
{
    if (no == 0xFFFF)
        return 0;
    if (no < 2000)
        return ((const u16 *)0x08623DF4)[no & 0x7FF];
    return ((const u16 *)0x08623DF4)[(no - 2000) & 0x7FF] + 1;
}
void sub_08058EDC(int player, u16 w)
{
    u16 id;
    int kind;
    int n;
    struct DuelCard *front;

    id = CardNumberToId58(w);
    if (w > 0xFFF)
        return;
    if (CARD_TYPE(id) <= 0x14) {
        switch (CARD_NUMBER(id)) {
        case 0x776:
            kind = 3;
            break;
        case 0x777:
        case 0x778:
            kind = 1;
            break;
        default:
            switch ((int)CARD_TYPE(id)) {
            case 0x16:
                kind = 7;
                break;
            case 0x15:
                kind = 8;
                break;
            case 0x17:
                kind = 9;
                break;
            default:
                kind = (CARD_STATS(id) & 0xC0000) >> 18;
                break;
            }
            break;
        }
        if (kind == 2) {
            for (n = PL(player).fusionCount; n > 0; n--)
                sub_08007558(&PL(player).fusionDeck[n], &PL(player).fusionDeck[n - 1]);
            PL(player).fusionCount++;
            front = &PL(player).fusionDeck[0];
            front->id = id;
            front->unk12 = player;
            return;
        }
    }
    for (n = PL(player).deckCount; n > 0; n--)
        sub_08007558(&PL(player).deck[n], &PL(player).deck[n - 1]);
    PL(player).deckCount++;
    front = &PL(player).deck[0];
    front->id = id;
    front->unk12 = player;
}
