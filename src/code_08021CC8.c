#include "global.h"

/* A card word as stored in the duel lists (u32 container: tests compile to lsl + sign compare). */
struct DuelCard {
    u32 id:12;          /* card ID (index into gUnk_08621DE0) */
    u32 unk12:5;
    u32 flag17:1;       /* bit 17 */
    u32 flag18:1;       /* bit 18 */
    u32 unk19:13;
};
union DuelCardWord {
    u32 w;
    struct DuelCard c;
};

/* Per-player duel state (0xD64 bytes), two of them at 0x020192E4 (see code_08007994). */
struct DuelPlayer {
    u16 lifePoints;     /* +0x000 */
    u8 handCount;       /* +0x002 */
    u8 deckCount;       /* +0x003 */
    u8 graveCount;      /* +0x004 */
    u8 fusionCount;     /* +0x005 */
    u8 banishCount;     /* +0x006 */
    u8 filler7[0x684 - 0x7];
    union DuelCardWord hand[80];    /* +0x684 */
    u32 deck[80];       /* +0x7C4 */
    u32 grave[80];      /* +0x904 */
    u32 fusion[80];     /* +0xA44 */
    u32 banish[80];     /* +0xB84 */
    u8 fillerCC4[0xD64 - 0xCC4];
};
struct DuelState {
    u32 unk0;
    struct DuelPlayer players[2];   /* +0x004 (= 0x020192E4) */
    u8 filler1ACC[0x1B14 - 0x1ACC];
    u16 unk1B14_0:2;
    u16 unk1B14_2:7;    /* +0x1B14 bits 2-8 */
    u16 unk1B14_9:7;
    u8 filler1B16[0x1B43 - 0x1B16];
    u8 unk1B43;
    u8 unk1B44;
    u8 filler1B45[0x1B50 - 0x1B45];
    /* +0x1B50: pending duel message (sent to the link partner, see sub_080225D8) */
    u8 msgSent:1;       /* +0x1B50 bit 0: already forwarded over link (hypothesis) */
    u8 msgPending:1;    /* +0x1B50 bit 1 */
    u8 msgPlayer:1;     /* +0x1B50 bit 2 */
    u8 msgUnk3:1;
    u16 msgKind:6;      /* +0x1B50 bits 4-9: message kind */
    u16 msgUnkA:6;
    u16 msgArg;         /* +0x1B52 */
    u16 msgValue;       /* +0x1B54 */
    u16 msgRest[6];     /* +0x1B56 */
    u8 step;            /* +0x1B62: step of the prompt handlers below */
    u8 unk1B63;
    u16 result;         /* +0x1B64: prompt result */
};

extern struct DuelState gUnk_020192E0;

extern struct DuelPlayer gUnk_020192E4[2];
/* Same array through a constant address: GCC reloads it at every use instead of hoisting/CSEing it. */
extern u32 gUnk_02019968[];    /* = gUnk_020192E4[0].hand */
/* hand[i] of a player, with the offsets added in the order the ROM uses. */
#define HAND_CARD(p, i) (*(u32 *)((u8 *)gUnk_02019968 + (((p) & 1) * 0xD64 + (i) * 4)))
#define PLAYERS ((struct DuelPlayer *)0x020192E4)

/* Duel control at 0x02015EE8 (gDuelCtrl, hypothesis). */
struct DuelCtrl {
    u8 phase;           /* +0: index into the phase table 0x08198F80 */
    u8 link:1;          /* +1 bit 0: link duel (hypothesis) */
    u8 unk1_1:7;
};
extern struct DuelCtrl gUnk_02015EE8;

/* Link send buffer / state at 0x02017FB0 (fields used here). */
struct LinkState {
    u8 filler0[0x204];
    u16 msgId;          /* +0x204: message id (0xF0xx) */
    u16 msgArg;         /* +0x206 */
    u32 cards[0x3F];    /* +0x208: payload */
    u32 unk304:8;
    u32 dirtyHand:1;    /* +0x305 bit 0: list needs resending (hypothesis) */
    u32 dirtyDeck:1;    /* +0x305 bit 1 */
    u32 dirtyGrave:1;   /* +0x305 bit 2 */
    u32 dirtyFusion:1;  /* +0x305 bit 3 */
    u32 dirtyBanish:1;  /* +0x305 bit 4 */
    u32 unk305_5:3;
    u32 unk306_0:6;
    u32 unk306_6:1;     /* +0x306 bit 6 */
    u32 unk306_7:1;
    u32 unk307_0:7;
    u32 unk307_7:1;     /* +0x307 bit 7 */
};
extern struct LinkState gUnk_02017FB0;
extern u32 gUnk_020181B8[];     /* = gUnk_02017FB0.cards */

void sub_080225D8(void);
void sub_080753F4(char *dst, const char *fmt, ...);
void sub_080602A4(u32 a, u32 b, u32 c, const char *text);
void sub_08060308(u32 a, u32 b, u32 c);
u32 sub_08052F38(u32 keys);
void sub_08077AEC(u32 a);
void sub_080197C0(int player, u16 id);
u32 sub_080517BC(int player, u16 arg, u16 a, u16 b);
u32 sub_0805194C(int player, u16 arg, u16 a, u16 b);
u32 sub_08051A9C(int player, u16 arg, u16 value);
u32 sub_08051BBC(int player, u16 arg, u16 value);
u32 sub_08051CD8(int player, u16 arg, u16 value);
u32 sub_08052810(int player);
u32 sub_08051DF4(int player);
u32 sub_0805232C(void);
u32 sub_08052560(void);
u32 sub_080525C4(void);
u32 sub_08052714(void);
u32 sub_08053F98(u16 arg, u16 value);
u32 sub_08051ED0(int player);
u32 sub_08052018(int player, u16 arg, u16 value);
u32 sub_08021DA8(int player, u16 number);
u32 sub_08021EC8(int player);
u32 sub_080220D4(int unused, u16 card);
u32 sub_0802215C(int unused);
u32 sub_080221E4(int unused, u32 value);
u32 sub_0802226C(int unused, u32 value);
void sub_080193D4(int player, int idx, int a, int b);
extern const char gUnk_08081DB8[];
extern const char gUnk_08081DF0[];
extern const char gUnk_08081D34[];
extern const char gUnk_08081D70[];
extern const u16 gUnk_08623DF4[];   /* card number to card id */
/* Through a constant address, as the ROM's register allocation needs. */
#define CARD_ID_TABLE ((const u16 *)0x08623DF4)

/* Card number (0..1999, 2000+ = alternate art) to card id; 0xFFFF maps to 0 (same inline as code_080044E4). */
static inline u16 CardNumberToId(u16 n)
{
    if (n == 0xFFFF)
        return 0;
    if (n < 2000)
        return *(CARD_ID_TABLE + (n & 0x7FF));
    return *(CARD_ID_TABLE + ((n - 2000) & 0x7FF)) + 1;
}
u16 sub_08062140(u16 a);
void sub_08076714(u32 yx, u16 shapeSize, u16 attr2, u32 affine);

extern const u16 gUnk_081A4424[];   /* pulse scale, 16 entries */

/* gMain (0x03000040): only the fields used here. */
struct Main {
    u8 filler0[6];
    u16 newKeys;        /* +0x006 */
    u8 filler8[0x485E - 8];
    u16 frameCounter;   /* +0x485E */
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040
u32 sub_08054398(int player, u32 id);
u32 sub_08007834(u32 id);

extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */
#define CSTATS ((const u32 *)0x08621DE0)
#define CARD_STATS(id) (CSTATS[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_LEVEL(id) ((CARD_STATS(id) & 0x1E000000) >> 25)

extern const char gUnk_08081E34[];
extern const char gUnk_08081E6C[];
extern const char gUnk_08081EE0[];
extern const char gUnk_08081F28[];
extern const char gUnk_0822C720[][0x40];   /* card name table */
extern u16 gUnk_086249D4;

struct Unk0201AE60 {
    u8 filler0[0x14];
    u16 unk14;
};
extern struct Unk0201AE60 gUnk_0201AE60;

struct Unk0201CFB0 {
    u8 filler0[0x82C];
    u32 unk82C;
};
extern struct Unk0201CFB0 gUnk_0201CFB0;
void sub_08022678(int player, int kind, u16 arg, u16 value);
void sub_08075294(void *dst, const void *src, u32 size);
u32 sub_080723B4(void *buf, int len);
u16 sub_080229BC(u16 head, const void *src, int size);
void sub_08007558(u32 *dst, const u32 *src);

u32 sub_08021CC8(void)
{
    return 0;
}

void sub_08021CCC(void)
{
    gUnk_020192E0.unk1B43 = 0;
    gUnk_020192E0.unk1B44 = 0;
}
void sub_08021CEC(u16 a, u16 b, u16 pulse)
{
    u16 tile;

    tile = sub_08062140(a) | 0x1000;
    sub_08076714(0x00400050, 0x80, tile,
                 pulse == 0 ? gUnk_081A4424[(gMain.frameCounter & 0x1E) >> 1] << 16 : 0x01000000);
    if (b)
        tile = sub_08062140(a) | 0x1000;
    else
        tile = 0x40;
    sub_08076714(0x004000A0, 0x80, tile,
                 pulse ? (gUnk_081A4424[(gMain.frameCounter & 0x1E) >> 1] << 16) | 0x20 : 0x01000020);
}
u32 sub_08021DA8(int player, u16 number)
{
    
    switch (gUnk_020192E0.step) {
    case 0:
        sub_080197C0(player, CardNumberToId(number));
        gUnk_020192E0.step++;
        return 0;
    case 1:
        switch (number) {
        case 0x489:
            sub_080602A4(0x206, 0x712, 11, gUnk_08081D34);
            break;
        case 0x5ED:
        case 0x5EF:
            sub_080602A4(0x206, 0x712, 11, gUnk_08081D70);
            break;
        }
        sub_08060308(1, 0, 0);
        gUnk_020192E0.step++;
        return 0;
    default:
        gUnk_020192E0.result = gUnk_0201AE60.unk14;
        return 1;
    }
}
#if 0 /* NONMATCHING (score 37): NONMATCHING: player made opaque in the CPU branch (asm +r) so player&1 is
       * recomputed per pass like the ROM; i pinned to r4; inner loop as explicit if(j < *p1) + do-while where
       * p1=&players[1].handCount (pool form, hoisted in loop pass 2 after the step address as in the ROM). Left:
       * do-while keeps the inner count load inside the loop (ROM hoists it: needs a rotated for/while, VTOP); a for-loop
       * with the same count expression either matches the hoisted p1 movable (pool form, count merged) or never hoists
       * p1 (plain form, cse associates it to the hand base). */
#define EC_ID(w) (((w) << 20) >> 20)
u32 sub_08021EC8(int player)
{
    register int i asm("r4");
    int j;

    switch (gUnk_020192E0.step) {
    case 0:
        gUnk_020192E0.result = 0;
        for (i = 0; i < (gUnk_020192E0.players + (player & 1))->handCount; i++) {
            union DuelCardWord *c = &(gUnk_020192E0.players + (player & 1))->hand[i];
            if (((CSTATS[c->w << 21 >> 21] & 0x1F00000) >> 20) == 22 && !c->c.flag18) {
                if (player) {
                    asm("" : "+r"(player));
                    gUnk_020192E0.result = 1;
                    {
                    u8 *p1 = &gUnk_020192E0.players[1].handCount;
                    j = 0;
                    if (j < *p1) {
                        do {
                            if (((CSTATS[EC_ID((&gUnk_020192E0.players[1].hand[j])->w) & 0x7FF] & 0x1F00000) >> 20) == 22) {
                                sub_080193D4(1, j, 1, 1);
                                return 1;
                            }
                        } while (++j < gUnk_020192E0.players[1].handCount);
                    }
                    }
                    gUnk_020192E0.result = 0;
                    return 1;
                }
                sub_080602A4(0x206, 0x712, 11, gUnk_08081DB8);
                sub_08060308(1, 0, 0);
                gUnk_020192E0.step++;
                return 0;
            }
        }
        return 1;
    case 1:
        if (gUnk_0201AE60.unk14 == 0) {
            gUnk_020192E0.result = 0;
            return 1;
        }
        sub_080602A4(0x206, 0x712, 11, gUnk_08081DF0);
        gUnk_020192E0.step++;
        return 0;
    default:
        if (gMain.newKeys & 2) {
            gUnk_020192E0.step = 0;
            return 0;
        }
        if (sub_08052F38(1)) {
            u32 idx = gUnk_0201CFB0.unk82C;
            union DuelCardWord *c = &gUnk_020192E0.players[0].hand[idx];
            if (((CSTATS[c->w << 21 >> 21] & 0x1F00000) >> 20) == 22) {
                int ok = 1;
                if (c->c.flag17)
                    ok = 0;
                if (c->c.flag18)
                    ok = 0;
                if (ok) {
                    gUnk_020192E0.result = 1;
                    sub_080193D4(0, idx, 1, 1);
                    return 1;
                }
            }
            sub_08077AEC(3);
        }
        return 0;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08021CC8", sub_08021EC8); /* 0x08021EC8 size 0x20C */
u32 sub_080220D4(int unused, u16 card)
{
    char buf[0x80];

    if (gUnk_020192E0.step == 0) {
        gUnk_020192E0.result = 0;
        if (card == 0)
            return 1;
        sub_080753F4(buf, gUnk_08081E34, gUnk_0822C720[card]);
        sub_080602A4(0x206, 0x713, 11, buf);
        sub_08060308(1, 0, 0);
        gUnk_020192E0.step++;
        return 0;
    }
    gUnk_020192E0.result = gUnk_0201AE60.unk14;
    return 1;
}
u32 sub_0802215C(int unused)
{
    char buf[0x100];

    if (gUnk_020192E0.step != 0) {
        gUnk_020192E0.result = gUnk_0201AE60.unk14;
        return 1;
    }
    gUnk_020192E0.result = 0;
    sub_080753F4(buf, gUnk_08081E6C, gUnk_0822C720[gUnk_086249D4]);
    sub_080602A4(0x206, 0x713, 11, gUnk_08081E6C); /* passes the format, not buf */
    sub_08060308(1, 0, 0);
    gUnk_020192E0.step++;
    return 0;
}
u32 sub_080221E4(int unused, u32 value)
{
    switch (gUnk_020192E0.step) {
    case 0:
        gUnk_020192E0.result = 0;
        sub_080602A4(0x206, 0x713, 11, gUnk_08081EE0);
        gUnk_020192E0.step++;
        break;
    case 1:
        if (sub_08052F38(0xF0)) {
            if (gUnk_0201CFB0.unk82C != value) {
                gUnk_020192E0.result = gUnk_0201CFB0.unk82C;
                gUnk_020192E0.step++;
            }
            sub_08077AEC(3);
        }
        break;
    default:
        return 1;
    }
    return 0;
}
u32 sub_0802226C(int unused, u32 value)
{
    switch (gUnk_020192E0.step) {
    case 0:
        gUnk_020192E0.result = 0;
        sub_080602A4(0x206, 0x713, 11, gUnk_08081F28);
        gUnk_020192E0.step++;
        break;
    case 1:
        if (sub_08052F38(0xF00000)) {
            if (gUnk_0201CFB0.unk82C != value) {
                gUnk_020192E0.result = gUnk_0201CFB0.unk82C;
                gUnk_020192E0.step++;
            }
            sub_08077AEC(3);
        }
        break;
    default:
        return 1;
    }
    return 0;
}
/* Run the handler of the pending duel message; returns 1 while it is still running. */
u32 sub_080222F8(void)
{
    u16 done;

    if (!gUnk_020192E0.msgPending)
        return 0;
    if (gUnk_020192E0.msgSent && gUnk_020192E0.msgPlayer) {
        done = gUnk_02017FB0.unk307_7;
    } else {
        switch (gUnk_020192E0.msgKind) {
        case 1:
            done = sub_080517BC(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg, gUnk_020192E0.msgValue & 1, gUnk_020192E0.msgValue & 2);
            break;
        case 2:
            done = sub_0805194C(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg, gUnk_020192E0.msgValue & 1, gUnk_020192E0.msgValue & 2);
            break;
        case 3:
            done = sub_08051A9C(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg, gUnk_020192E0.msgValue);
            break;
        case 4:
            done = sub_08051BBC(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg, gUnk_020192E0.msgValue);
            break;
        case 5:
            done = sub_08051CD8(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg, gUnk_020192E0.msgValue);
            break;
        case 6:
            done = sub_08052810(gUnk_020192E0.msgPlayer);
            break;
        case 7:
            done = sub_08051DF4(gUnk_020192E0.msgPlayer);
            break;
        case 8:
            done = sub_0805232C();
            break;
        case 9:
            done = sub_08052560();
            break;
        case 10:
            done = sub_080525C4();
            break;
        case 11:
            done = sub_08052714();
            break;
        case 12:
            done = sub_08053F98(gUnk_020192E0.msgArg, gUnk_020192E0.msgValue);
            break;
        case 13:
            done = sub_08051ED0(gUnk_020192E0.msgPlayer);
            break;
        case 14:
            done = sub_08052018(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg, gUnk_020192E0.msgValue);
            break;
        case 15:
            done = sub_08021DA8(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg);
            break;
        case 16:
            done = sub_0802226C(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg);
            break;
        case 17:
            done = sub_08021EC8(gUnk_020192E0.msgPlayer);
            break;
        case 18:
            done = sub_080220D4(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg);
            break;
        case 19:
            done = sub_0802215C(gUnk_020192E0.msgPlayer);
            break;
        case 20:
            done = sub_080221E4(gUnk_020192E0.msgPlayer, gUnk_020192E0.msgArg);
            break;
        default:
            return 0;
        }
    }
    if (done) {
        if (gUnk_020192E0.msgSent && !gUnk_020192E0.msgPlayer)
            sub_080229BC(0xF0A2, &gUnk_020192E0.result, 0x10);
        gUnk_020192E0.msgPending = 0;
        return 0;
    }
    return 1;
}
void sub_080225D8(void)
{
    u16 m[9];

    gUnk_020192E0.msgPending = 1;
    gUnk_020192E0.step = 0;
    gUnk_020192E0.unk1B63 = 0;
    gUnk_020192E0.msgSent = 0;
    gUnk_020192E0.msgUnk3 = 0;
    if (gUnk_020192E0.msgPlayer && gUnk_02015EE8.link) {
        u16 kind = gUnk_020192E0.msgKind;
        if (kind == 3)
            return;
        m[0] = kind;
        sub_08075294(&m[1], &gUnk_020192E0.msgArg, 0x10);
        sub_080229BC(0xF0A1, m, 0x12);
        gUnk_02017FB0.unk307_7 = 0;
        gUnk_020192E0.msgSent = 1;
    }
}
void sub_08022678(int player, int kind, u16 arg, u16 value)
{
    gUnk_020192E0.msgPlayer = player;
    gUnk_020192E0.msgKind = kind;
    gUnk_020192E0.msgArg = arg;
    gUnk_020192E0.msgValue = value;
    sub_080225D8();
}
void sub_080226CC(int player, int kind, const u16 *src, int n)
{
    if (n > 8)
        n = 8;
    gUnk_020192E0.msgPlayer = player;
    gUnk_020192E0.msgKind = kind;
    sub_08075294(&gUnk_020192E0.msgArg, src, n * 2);
    sub_080225D8();
}
void sub_0802272C(int player, int arg, u16 x, u16 y)
{
    u16 flags = x != 0;
    if (y)
        flags |= 2;
    sub_08022678(player, 1, arg, flags);
}
void sub_08022758(int player, int arg, u16 x, u16 y)
{
    u16 flags = x != 0;
    if (y)
        flags |= 2;
    sub_08022678(player, 2, arg, flags);
}
void sub_08022784(int player, u16 arg, int value)
{
    if (gUnk_020192E0.msgPending && gUnk_020192E0.msgKind == 3)
        gUnk_020192E0.msgValue += value;
    else
        sub_08022678(player, 3, arg, value);
}
void sub_080227CC(int player, int value)
{
    if (gUnk_020192E0.msgPending && gUnk_020192E0.msgKind == 4)
        gUnk_020192E0.msgValue += value;
    else
        sub_08022678(player, 4, 0, value);
}

void sub_08022814(int player)
{
    sub_08022678(player, 5, 0, 1);
}

void sub_08022824(int player)
{
    sub_08022678(player, 7, 0, 0);
}
/* Monster level: 0 for types 21-23, 10 for type 24 (Divine), else stats bits 25-28. */
static inline u32 GetCardLevel(u16 id)
{
    int type = CARD_TYPE(id);

    switch (type) {
    case 21:
    case 22:
    case 23:
        return 0;
    case 24:
        return 10;
    default:
        return CARD_LEVEL(id);
    }
}

u32 sub_08022834(int player)
{
    int i;

    for (i = 0; i < gUnk_020192E4[player & 1].handCount; i++) {
        u16 id = HAND_CARD(player, i) << 20 >> 20;
        if (sub_08054398(player, id) && !sub_08007834(id) && GetCardLevel(id) <= 4) {
            sub_08022678(player, 13, 0, 0);
            return 1;
        }
    }
    return 0;
}
void sub_08022914(void)
{
    if (!gUnk_02017FB0.unk306_6 && gUnk_020192E0.unk1B14_2 == 0) {
        gUnk_02017FB0.unk306_6 = 1;
        gUnk_020192E0.unk1B14_2 = 0;
    }
}
u32 sub_0802295C(void)
{
    gUnk_020192E0.unk1B14_2 = 0;
    return 0;
}

struct Msg8 {
    u32 a:16;
    u32 b:16;
    u32 c:16;
    u32 d:16;
};

u16 sub_0802297C(u16 a, u16 b, u16 c, u16 d)
{
    struct Msg8 m;
    m.a = a;
    m.b = b;
    m.c = c;
    m.d = d;
    return sub_080723B4(&m, 8);
}

u16 sub_080229BC(u16 head, const void *src, int size)
{
    u16 buf[0x80];
    buf[0] = head;
    if (size > 0)
        sub_08075294(&buf[1], src, size);
    return sub_080723B4(buf, size + 2);
}
/* Send a card list to the link partner: {id, (player << 8) | count, cards[count]}; the do/while
 * form keeps agbcc from strength-reducing &buf[i], as in the ROM. */
void sub_080229EC(int player)
{
    int i;

    gUnk_02017FB0.msgId = 0xF021;
    gUnk_02017FB0.msgArg = gUnk_020192E4[player & 1].handCount | ((u8)player << 8);
    i = 0;
    if (i < gUnk_020192E4[player & 1].handCount)
        do
            sub_08007558(&gUnk_020181B8[i], &gUnk_020192E4[player & 1].hand[i].w);
        while (++i < gUnk_020192E4[player & 1].handCount);
    sub_080723B4(&gUnk_02017FB0.msgId, gUnk_020192E4[player & 1].handCount * 4 + 4);
    gUnk_02017FB0.dirtyHand = 0;
}
void sub_08022A9C(int player)
{
    int i;

    gUnk_02017FB0.msgId = 0xF022;
    gUnk_02017FB0.msgArg = gUnk_020192E4[player & 1].deckCount | ((u8)player << 8);
    i = 0;
    if (i < gUnk_020192E4[player & 1].deckCount)
        do
            sub_08007558(&gUnk_020181B8[i], &gUnk_020192E4[player & 1].deck[i]);
        while (++i < gUnk_020192E4[player & 1].deckCount);
    sub_080723B4(&gUnk_02017FB0.msgId, gUnk_020192E4[player & 1].deckCount * 4 + 4);
    gUnk_02017FB0.dirtyDeck = 0;
}
void sub_08022B4C(int player)
{
    int i;

    gUnk_02017FB0.msgId = 0xF023;
    gUnk_02017FB0.msgArg = gUnk_020192E4[player & 1].graveCount | ((u8)player << 8);
    i = 0;
    if (i < gUnk_020192E4[player & 1].graveCount)
        do
            sub_08007558(&gUnk_020181B8[i], &gUnk_020192E4[player & 1].grave[i]);
        while (++i < gUnk_020192E4[player & 1].graveCount);
    sub_080723B4(&gUnk_02017FB0.msgId, gUnk_020192E4[player & 1].graveCount * 4 + 4);
    gUnk_02017FB0.dirtyGrave = 0;
}
void sub_08022BFC(int player)
{
    int i;

    gUnk_02017FB0.msgId = 0xF024;
    gUnk_02017FB0.msgArg = gUnk_020192E4[player & 1].fusionCount | ((u8)player << 8);
    i = 0;
    if (i < gUnk_020192E4[player & 1].fusionCount)
        do
            sub_08007558(&gUnk_020181B8[i], &gUnk_020192E4[player & 1].fusion[i]);
        while (++i < gUnk_020192E4[player & 1].fusionCount);
    sub_080723B4(&gUnk_02017FB0.msgId, gUnk_020192E4[player & 1].fusionCount * 4 + 4);
    gUnk_02017FB0.dirtyFusion = 0;
}
void sub_08022CAC(int player)
{
    int i;

    gUnk_02017FB0.msgId = 0xF025;
    gUnk_02017FB0.msgArg = gUnk_020192E4[player & 1].banishCount | ((u8)player << 8);
    i = 0;
    if (i < gUnk_020192E4[player & 1].banishCount)
        do
            sub_08007558(&gUnk_020181B8[i], &gUnk_020192E4[player & 1].banish[i]);
        while (++i < gUnk_020192E4[player & 1].banishCount);
    sub_080723B4(&gUnk_02017FB0.msgId, gUnk_020192E4[player & 1].banishCount * 4 + 4);
    gUnk_02017FB0.dirtyBanish = 0;
}
