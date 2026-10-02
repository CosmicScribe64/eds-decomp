#include "global.h"

/*
 * Duel card-effect executors. Each is `int f(struct CardRef *ref)` and returns 0
 * or a step code such as 0x7F, 0x80 or 0x92. See wiki/functions/code-08031bc8.md.
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

/* 0x020192E4 + 0x5F0: a card word in each player's state (stride 0xD64), hypothesis: a "set" spell/trap slot */
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

/* Player life points: gUnk_020192E4[p].lifePoints (u16 at +0, stride 0xD64). */
struct PlayerLP {
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 count904;
    u8 fusionCount;
    u8 filler[0xD64 - 6];
};
extern struct PlayerLP gUnk_020192E4[2];

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
/* Duel screen state at 0x0201CFB0: the word at +0x82C is passed to sub_080193B0. */
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
extern u8 gUnk_020192E0[];   /* duel global state, byte view */
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
/* Duel global state at 0x020192E0 (byte view gUnk_020192E0); fields used in this unit. */
struct DuelGlobals {
    u8 unk0[0x1B12];
    u8 b0 : 1;      /* +0x1B12 bit 1: a player index (hypothesis) */
    u8 b1 : 1;
    u8 rest : 6;
    u8 unk1B13[0x1B64 - 0x1B13];
    u16 w1B64;
};
#define DG ((struct DuelGlobals *)gUnk_020192E0)
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
int sub_08009C08(int player, int id, int *out);
int sub_0800CAF0(int player, int zone);
int sub_0800A158(int player);
int sub_0800A230(int player);
int sub_08008C6C(int player);
extern const u8 gUnk_08082EC0[];
extern const u8 gUnk_08082EF4[];
struct MainView { u8 unk0[6]; u16 h6; };
extern struct MainView gUnk_03000040;
int sub_08009C64(int player, u32 *packed);
/* 0x0201CF90: a byte whose bits 1-5 are used (hypothesis: a selected position) */
extern u8 gUnk_0201CF90;
extern const u8 gUnk_08083020[];
extern const u8 gUnk_0808306C[];
extern const u8 gUnk_080830B4[];
int sub_08047170(int player);
extern const u8 gUnk_08082F34[];
extern const u8 gUnk_08082F64[];
extern const u8 gUnk_08082FA4[];
struct S15F00 { u8 unk0[0x1B22]; u16 listPos; };    /* saved list position (hypothesis) */
#define S15F00 ((struct S15F00 *)gUnk_02015F00)
int sub_0801967C(int player, int number, int *out);
void sub_08007558(void *dst, const void *src);
extern const u8 gUnk_08083104[];
extern const u8 gUnk_0808313C[];
struct EffState544 {    /* 0x02017A40 view with a card word at +0x544 */
    u8 unk0[0x544];
    u32 card;
};
#define ES544 ((struct EffState544 *)gUnk_02017A40)
int sub_0802EA50(struct CardRef *ref, int a, int b);
void sub_080226CC(int player, int a, u16 *ids, int n);
void sub_08019820(int player);
void sub_08019800(int player, int id);
void sub_0801FBCC(u32 a, u32 b);
extern const u8 gUnk_08082E7C[];
extern const u8 gUnk_08082E9C[];
extern u32 gUnk_02017F84[];     /* 5 card words (hypothesis: revealed/drawn cards) */
struct EffStateCards {          /* 0x02017A40 view: card words at +0x544 */
    u8 unk0[0x544];
    u32 cards[8];
};
#define ESC ((struct EffStateCards *)gUnk_02017A40)
int sub_08007834(int id);
#define EFF_CNT gUnk_02017A40[0x3E2]
struct PF {     /* player state at 0x020192E4, stride 0xD64; fields used here */
    u16 lifePoints;
    u8 handCount;
    u8 deckCount;
    u8 unk4[0x7C4 - 4];
    u32 deck[80];
    u8 filler[0xD64 - 0x7C4 - 80 * 4];
};
#define PF ((struct PF *)gUnk_020192E4)
int sub_080361D0(struct CardRef *ref)
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
            for (j = 5; j <= 10; j++) {
                struct DuelZone *z = ZB(p & 1, j);

                if (CARD_ID(CARD_WORD(z->card)))
                    sub_08018AE8(p, j, 9);
            }
        }
    }
    return 0;
}
int sub_08036254(struct CardRef *ref, int arg)
{
    u16 ids[16];
    char buf[0x80];
    int i;

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (sub_0802EA50(ref, arg, 0) == 0)
                return 0;
            sub_080602A4(0x206, 0x512, 0xB, gUnk_08082E7C);
            EFF_SIDE = 5;
            return 0x7F;
        case 0x7F:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7E;
        case 0x7E: {
            u32 *card;

            EFF_SIDE--;
            card = &gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top];
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            sub_08007558(&ESC->cards[EFF_SIDE], card);
            if (EFF_SIDE != 0) {
                sub_08075434(buf, gUnk_08082E9C, EFF_SIDE);
                sub_080602A4(0x206, 0x512, 0xB, buf);
                return 0x7F;
            }
            return 0x7D;
        }
        case 0x7D:
            for (i = 0; i <= 4; i++)
                ids[i] = CARD_ID(gUnk_02017F84[i]);
            sub_080226CC(1 - ref->player, 12, ids, 5);
            return 0x7C;
        case 0x7C: {
            int first = 1;

            for (i = 0; i <= 4; i++) {
                u32 *pw = &gUnk_02017F84[i];
                u16 *ph = (u16 *)pw;

                if (CARD_ID(*pw) == DG->w1B64 && first != 0) {
                    /* the unit declares sub_08019820(int); the real one is (int player, u16 id) */
                    ((void (*)(int, u16))sub_08019820)(ref->player, CARD_ID(*pw));
                    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80CB : 0xCB, ph[0], ph[1], 0);
                    first = 0;
                } else {
                    u32 w;
                    u32 id;
                    u32 num;

                    sub_08019800(ref->player, CARD_ID(*pw));
                    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D7 : 0xD7, ph[0], ph[1], 0);
                    w = *pw;
                    id = CARD_ID(w);
                    /* an int-width temporary keeps the compare in SImode, so loop.c does not hoist 0x4DA */
                    num = CARD_NUMBER(id);
                    if (num == 0x4DA)
                        sub_0801FBCC(0x3C600000 | (((w << 19) >> 31) & 1) << 31 | id, 0);
                }
            }
            return 0x64;
        }
        default:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
        }
    }
    return 0;
}
int sub_080364DC(struct CardRef *ref)
{
    if (!ref->skip4)
        sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x8044 : 0x44, 0, 0, 0);
    return 0;
}
int sub_08036510(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        int out;
        int opp = 1 - ref->player;
        int id = (u32)ref->targets[0] << 20 >> 20;

        if (sub_08009C08(opp, id, &out))
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D5 : 0xD5, id, 0, 0);
    }
    return 0;
}
int sub_08036570(struct CardRef *ref)
{
    int count = 0;
    struct DuelZone **zonePtr; /* FAKEMATCH: taking the local address preserves register allocation. */

    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            int j;

            for (j = 0; j <= 4; j++) {
                struct DuelZone *z = ZB(i & 1, j);

                zonePtr = &z;
                if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(*zonePtr) & 2))
                    count++;
            }
        }
        if (count > 0)
            sub_08019980(ref->player, count * 300);
    }
    return 0;
} /* 0x08036570 size 0x80 */
static inline u32 EffectHandWord(int player, int index)
{
    int idx4 = index * 4;
    int poff = player * 0xD64;

    return *(u32 *)(idx4 + poff + (u32)gUnk_02019968);
}
int sub_080365F0(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int n = 7 & ((u8 *)ref)[0xA];

            if (n == 1) {
                u8 tp = ref->targets[0];
                int tz = ref->targets[0] >> 8;
                int p = tp & n;
                struct DuelZone *z = ZB(p, tz);

                if (CARD_ID(CARD_WORD(z->card))) {
                    sub_08018544(tp, tz, 1);
                    return 0x7F;
                }
            }
            break;
        }
        case 0x7F: {
            int r;

            if (gUnk_020192E4[ref->player].handCount == 0) {
                /* FAKEMATCH: an empty repeated test ends the extended block, so
                   the player bit is shifted again for sub_0800A158. */
                if (gUnk_020192E4[ref->player].handCount) {
                }
                break;
            }
            r = sub_0800A158(ref->player);
            {
                int fail = -1;

                /* Compiler hint: retain r across the second check and rematerialize -1 later. */
                __asm__("" : "+r"(fail));
                if (r == fail && sub_0800A230(ref->player) == r)
                    break;
            }
            if (sub_08008C6C(ref->player) == -1)
                break;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08082EC0);
            sub_08060308(1, 0, 0);
            return 0x7E;
        }
        case 0x7E:
            if (gUnk_0201AE60.flag14 == 0)
                break;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08082EF4);
            return 0x7D;
        case 0x7D:
            if (sub_08052F38(1) != 0) {
                int p = ref->player;
                u32 w = EffectHandWord(p, DSV->w82C);
                u16 id = CARD_ID(w);
                int kind;
                int t;

                if (CARD_TYPE(id) <= 0x14)
                    goto fail;
                t = CARD_TYPE(id);
                switch (t) {
                case 0x15:
                case 0x16:
                    kind = (CARD_STATS(id) & 0xE0000) >> 17;
                    break;
                default:
                    kind = 0;
                }
                if (kind == 2)
                    goto fail;
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80C5 : 0xC5, id,
                             (DSV->w82C & 0xF) << 4 | (sub_08008C6C(ref->player) & 0xF), 0);
                return 0x64;
            fail:
                sub_08077AEC(3);
            }
            if (gUnk_03000040.h6 & 2)
                return 0x7F;
            return 0x7D;
        }
    }
    return 0;
}

#define ZB2_367E4(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))
int sub_080367E4(struct CardRef *ref)
{
    u8 skip = 4 & ((u8 *)ref)[4];

    if (!skip && ref->numTargets == 2) {
        switch (EFF_PHASE) {
        case 0x80: {
            int pp = 1 & ref->player;
            struct DuelZone *z = ZB(pp, ref->zone);
            u32 packed;
            int j;

            if (CARD_WORD(z->card) << 20 == 0)
                return 0;
            packed = ref->targets[1] << 16 | ref->targets[0];
            if (sub_08009C64(ref->player, &packed) == 0)
                return 0;
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D3 : 0xD3, ref->targets[0], ref->targets[1], 0);
            sub_08055F70(ref->player, &packed, 1, 0, 0x20);
            EFF_SIDE = 0;
            for (j = 0; j <= 4; j++) {
                if (CARD_ID(CARD_WORD(ZB2_367E4(1 & ref->player, j)->card)) == ref->targets[0]
                    && (ZB2_367E4(1 & ref->player, j)->unk7[0] & 0x80))
                    EFF_SIDE++;
            }
            return 0x7F;
        }
        case 0x7F:
            switch (CARD_NUMBER(ref->id)) {
            case 0x447:
                sub_08017AB4(ref->player, ref->player | ref->zone << 8,
                             ref->player | ((u32)(gUnk_0201CF90 << 26) >> 27) << 8, 2);
                break;
            case 0x488:
                sub_08017B04(ref->player, ref->player | ref->zone << 8,
                             ref->player | ((u32)(gUnk_0201CF90 << 26) >> 27) << 8);
                break;
            }
            return 0x64;
        }
    }
    return 0;
}
int sub_08036990(struct CardRef *ref)
{
    if (!ref->skip4)
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8045 : 0x45, 0, 0, 0);
    return 0;
}
int sub_080369C4(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1) {
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

                if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && sub_0800CAF0(p, j) == ref->targets[0]) {
                    sub_08030028(p, j);
                    sub_08046CB0(ref->player, p, j);
                }
            }
        }
    }
    return 0;
}
#if 0 /* NONMATCHING: logic decoded, and the differences are register
       * allocation. The ROM hoists 1 into r10 and keeps a second copy of the
       * deck pointer in r8 (used only by the second message), and rereads
       * EFF_SIDE after each call. */
int sub_08036A68(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i;

            for (i = 0; i <= 1; i++) {
                s16 j;

                for (j = 0; j <= 4; j++) {
                    sub_08030028(i, j);
                    sub_08046CB0(ref->player, i, j);
                }
            }
            EFF_SIDE = DG->b1;
            EFF_CNT = 5;
            return 0x7F;
        }
        case 0x7F: {
            int p = EFF_SIDE & 1;
            u32 *deck;
            u32 w;
            int id;

            if (PF[p].deckCount == 0)
                return 0x78;
            deck = PF[p].deck;
            sub_0801EC58(EFF_SIDE ? 0x8061 : 0x61, 1, 1, 0);
            sub_08019840(EFF_SIDE, CARD_ID(*deck));
            w = *deck;
            if (((w << 19) >> 31) != EFF_SIDE && (int)(w << 14) < 0 && CARD_NUMBER(CARD_ID11(w)) == 0x2FA) {
                if (sub_08008A1C(1 - EFF_SIDE) > 0) {
                    sub_0801EC58(EFF_SIDE ? 0x80C2 : 0xC2, ((u16 *)deck)[0], ((u16 *)deck)[1], 0);
                    sub_08007558(&ESC->cards[0], deck);
                    return 0x7D;
                }
                sub_080193D4(EFF_SIDE, PF[EFF_SIDE & 1].handCount, 0, 1);
                return 0x7C;
            }
            id = CARD_ID(*deck);
            {
                u16 type = CARD_TYPE(id);
                u32 v;

                if (type > 0x14)
                    return 0x7C;
                switch (type) {
                case 0x15:
                case 0x16:
                case 0x17:
                    v = 0;
                    break;
                case 0x18:
                    v = 0xA;
                    break;
                default:
                    v = (CARD_STATS(id) & 0x1E000000) >> 25;
                }
                if (v > 4)
                    return 0x7C;
            }
            if (sub_08007834(id) != 0)
                return 0x7C;
            sub_0801EC58(EFF_SIDE ? 0x80C2 : 0xC2, ((u16 *)deck)[0], ((u16 *)deck)[1], 0);
            sub_08007558(&ESC->cards[0], deck);
            return 0x7E;
        }
        case 0x7E:
            sub_08056094(EFF_SIDE, &ESC->cards[0], 0, 0);
            return 0x7C;
        case 0x7D:
            sub_08056094(1 - EFF_SIDE, &ESC->cards[0], 1, 0);
            return 0x7C;
        case 0x7C:
            if (--EFF_CNT == 0) {
                EFF_SIDE = 1 - EFF_SIDE;
                EFF_CNT = 5;
                if (EFF_SIDE == DG->b1)
                    return 0x78;
            }
            return 0x7F;
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080361D0", sub_08036A68); /* 0x08036A68 size 0x2C8 */
int sub_08036D30(struct CardRef *ref)
{
    u32 *card = &gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (sub_08047170(ref->player) == 0)
                return 0;
            if (sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) == 0) {
                if (1 & ((u8 *)ref)[2])
                    return 0;
                sub_080602A4(0x206, 0x712, 0xB, gUnk_08082F34);
                return 0x64;
            }
            if (1 & ((u8 *)ref)[2]) {
                if (sub_08056ECC(ref->id) < 0)
                    return 0;
                gUnk_0201D810.row = 0;
                gUnk_0201D810.top = S15F00->listPos;
                return 0x7D;
            }
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08082F64);
            sub_08060308(1, 0, 0);
            return 0x7F;
        case 0x7F:
            if (gUnk_0201AE60.flag14 == 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08082FA4);
            return 0x7E;
        case 0x7E:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7D;
        case 0x7D:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 0x7C;
        case 0x7C:
            sub_08055F70(ref->player, &gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top], 1, 0, 0);
            return 0x7B;
        case 0x7B:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
int sub_08036F28(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) == 0)
                return 0;
            switch (CARD_NUMBER(ref->id)) {
            case 0x455:
                sub_080602A4(0x206, 0x712, 0xB, gUnk_08083020);
                break;
            case 0x462:
                sub_080602A4(0x206, 0x712, 0xB, gUnk_0808306C);
                break;
            default:
                return 0;
            }
            sub_08060308(1, 0, 0);
            return 0x7F;
        case 0x7F:
            if (gUnk_0201AE60.flag14 == 0)
                return 0;
            sub_080602A4(0x206, 0x712, 0xB, gUnk_080830B4);
            return 0x7E;
        case 0x7E:
            sub_0802AF34(ref->player, -1, CARD_NUMBER(ref->id), 0);
            return 0x7D;
        case 0x7D:
            sub_0801970C(ref->player, ((const u16 *)0x08622AB4)[CARD_ID11(gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top])]);
            return 0x7C;
        }
    }
    return 0;
}
int sub_08037080(struct CardRef *ref)
{
    if (!ref->skip4)
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
    return 0;
}
int sub_080370B8(struct CardRef *ref)
{
    char buf[0x80];
    int out;

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            switch (CARD_NUMBER(ref->id)) {
            case 0x45A:
                sub_08019860(1 - ref->player, 500);
                break;
            case 0x45B:
                sub_08019980(ref->player, 1000);
                break;
            }
            return 0x7F;
        case 0x7F:
            if (sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) == 0)
                return 0;
            if (1 & ((u8 *)ref)[2]) {
                gUnk_0201AE60.flag14 = 1;
                return 0x7E;
            }
            switch (CARD_NUMBER(ref->id)) {
            case 0x45A:
                sub_080753F4(buf, gUnk_08083104, (u8 *)0x0822C720 + ref->id * 64);
                break;
            case 0x45B:
            case 0x51B:
                sub_080753F4(buf, gUnk_0808313C, (u8 *)gUnk_0822C720 + ref->id * 64);
                break;
            }
            sub_080602A4(0x206, 0x712, 0xB, buf);
            sub_08060308(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gUnk_0201AE60.flag14 != 0) {
                int r = sub_0801967C(ref->player, CARD_NUMBER(ref->id), &out);

                if (r >= 0) {
                    u32 *dest = &ES544->card;
                    struct DuelCard *src = (struct DuelCard *)&gUnk_02019AA8[ref->player];

                    src = (struct DuelCard *)((u32)src + r * 4);
                    sub_08007558(dest, src);
                    return 0x7D;
                }
            }
            goto dflt;
        case 0x7D:
            switch (CARD_NUMBER(ref->id)) {
            case 0x45A:
                sub_08055F70(ref->player, &ES544->card, 1, 0, 0);
                break;
            case 0x45B:
                sub_08055F70(ref->player, &ES544->card, 0, 1, 0);
                break;
            case 0x51B:
                sub_08055F70(ref->player, &ES544->card, 0, 1, 0);
                return 0xA;
            }
            return 0x7F;
        default:
        dflt:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
        }
    }
    return 0;
}
