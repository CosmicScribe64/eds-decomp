#include "global.h"

/*
 * Duel card-effect executors: int f(struct CardRef *ref), returning 0 (or 0x7F/0x80/0x92 style codes).
 * See wiki/functions/code-08031bc8.md.
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
#define ZB2(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))
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
    u8 unk6[0x28 - 6];
    struct DuelZone zones[11];      /* +0x28 (0x0201930C) */
    u8 unk5F4[0x684 - 0x28 - 11 * 0x94];
    struct DuelCard hand[80];       /* +0x684 */
    u8 filler[0xD64 - 0x684 - 80 * 4];
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
void sub_0802272C();
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
    u32 w824;
    u32 w828;
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

int sub_08008940(int player, int zone);
void sub_08018C3C(int player, int zone);
int sub_080088A4(int player, int a, int b);
u16 sub_08036030(struct CardRef *ref, int arg);
int sub_08076F9C(void);
extern const u8 gUnk_08083558[];
extern const u8 gUnk_08083588[];
extern const u8 gUnk_08083468[];
int sub_08009CAC(int player, int number);
void sub_08046BE0(int player, u16 a);
struct EffStateH { u8 unk0[0x544]; u16 lo; u16 hi; };
#define ESH ((struct EffStateH *)gUnk_02017A40)
extern const u8 gUnk_080834CC[];
extern const u16 gUnk_086247C8[];
int sub_08056D98(int player, int a, int b);
int sub_08008C94(int player, u16 a);
void sub_08017B04(int player, u16 a, u16 b);

int sub_0802C334(struct CardRef *ref, u16 pos);
extern const u16 gUnk_086243C8[];
extern const u8 gUnk_08083430[];
extern const u8 gUnk_08083508[];


int sub_08039638(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i;

            for (i = 0; i <= 1; i++) {
                int p;
                int k;

                if (i)
                    p = 1 - ref->player;
                else
                    p = ref->player;
                ref->targets[p] = gUnk_020192E4[p & 1].handCount;
                for (k = 0; k < gUnk_020192E4[p & 1].handCount; k++) {
                    if (((u32)CARD_WORD(gUnk_020192E4[p & 1].hand[k]) << 19 >> 31) != p)
                        ref->targets[p]--;
                    sub_080193D4(p, 0, i, 1);
                }
            }
            for (i = 0; i <= 1; i++) {
                int p;

                if (i)
                    p = 1 - ref->player;
                else
                    p = ref->player;
                sub_080199E0(p, ref->targets[p]);
            }
            return 0x7F;
        }
        case 0x7F:
            sub_08046BE0(ref->player, ref->targets[ref->player]);
            return 0x7E;
        case 0x7E:
            sub_08046BE0(1 - ref->player, ref->targets[1 - ref->player]);
            return 0x7D;
        }
    }
    return 0;
}
int sub_080397A0(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 4; i++) {
            if (sub_08008940(ref->player, i))
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80A3 : 0xA3, (u8)i | ref->zone << 8, 1, 0);
        }
    }
    return 0;
}
int sub_080397F8(struct CardRef *ref)
{
    char buf[0x80];

    switch (EFF_PHASE) {
    case 0x80:
        if (sub_08044224(ref->player, 0x4D8, 0) == 0)
            return 0;
        if (1 & ((u8 *)ref)[2])
            goto step7E;
        sub_080753F4(buf, gUnk_08083430, gUnk_0822C720 + gUnk_086243C8[0] * 64);
        sub_080602A4(0x206, 0x712, 0xB, buf);
        sub_08060308(1, 0, 0);
        return 0x7F;
    case 0x7F:
        if (gUnk_0201AE60.flag14 == 0)
            return 0;
    step7E:
        return 0x7E;
    case 0x7E:
        sub_0801970C(ref->player, 0x2EA);
        return 0x7D;
    }
    return 0;
}
#if 0 /* NONMATCHING: the loop hoisting order and registers of the zone scan differ (ROM: 1, 0xD64, zone base and 2 in r8, r6, r5, r4), as does the register allocation of the three 0x0201CFB0 field addresses in step 0x7D */
int sub_080398B0(struct CardRef *ref)
{
    char buf[0x80];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            int i;

            if (sub_08009CAC(ref->player, 0x4DA) == 0)
                return 0;
            if (sub_08008C6C(ref->player) == -1)
                return 0;
            for (i = 0; i <= 1; i++) {
                int j;
                struct DuelZone *z = gUnk_0201930C[i & 1].zones;

                for (j = 0; j <= 4; j++, z++) {
                    if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2))
                        goto ret7F;
                }
            }
            return 0;
        }
        case 0x7F:
            sub_080753F4(buf, gUnk_08083468, gUnk_0822C720 + ref->id * 64);
            sub_080602A4(0x206, 0x712, 0xB, buf);
            sub_08060308(1, 0, 0);
            return 0x7E;
        case 0x7E:
            if (gUnk_0201AE60.flag14 == 0)
                return 0;
            sub_080753F4(buf, gUnk_080834CC, gUnk_0822C720 + ref->id * 64);
            sub_080602A4(0x206, 0x712, 0xB, buf);
        ret7D:
            return 0x7D;
        case 0x7D: {
            u32 a;
            u32 c;
            int r;
            u16 *w;

            if (sub_08052F38(0xE000E0) != 0) {
                a = DSV->w824;
                c = DSV->w828 + DSV->w82C;
                r = sub_08008C6C(ref->player);
                sub_08077AEC(1);
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, (u16)DSV->w824, (u8)DSV->w82C << 8 | (u8)DSV->w828, 0);
                sub_08009C08(ref->player, ref->id, (int *)(w = &ESH->lo));
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80D3 : 0xD3, w[0], w[1], 0);
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8077 : 0x77, (u8)r | 0x100, w[0], w[1]);
                sub_08017B04(ref->player, ref->player | (u8)r << 8, (u8)a | (u8)c << 8);
                return 0x64;
            }
            if (!(gUnk_03000040.h6 & 2))
                goto ret7D;
        ret7F:
            return 0x7F;
        }
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08039638", sub_080398B0); /* 0x080398B0 size 0x254 */
int sub_08039B04(struct CardRef *ref)
{
    if (!ref->skip4 && sub_08008A1C(ref->player) > 3) {
        int n;
        int i;

        for (i = 0, n = 0; i <= 4 && n <= 3; i++) {
            if (sub_08008940(ref->player, i)) {
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80A3 : 0xA3, (u8)i | ref->zone << 8, 2, 0);
                n++;
            }
        }
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8049 : 0x49, 1, 1, 0);
    }
    return 0;
}
int sub_08039B90(struct CardRef *ref)
{
    switch (EFF_PHASE) {
    case 0x80:
        if (sub_080088A4(1 - ref->player, 1, 0) == 0)
            return 0;
        sub_080602A4(0x206, 0x613, 0xB, gUnk_08083508);
    ret7F:
        return 0x7F;
    case 0x7F:
        if (sub_08052F38(0xE00000) == 0)
            goto ret7F;
    {
        int praw = DSV->w824;
        int zone;
        int id;

        zone = DSV->w82C;
        id = (*(u32 *)((u32)gUnk_0201930C + zone * 0x94 + (praw & 1) * 0xD64) << 20) >> 20;
        /* FAKEMATCH: finish the initialized id extraction before loading
         * ref->player, preserving the ROM's selection-read schedule. */
        __asm__("" : : "r"(id));

        sub_08017AB4(ref->player, id, ref->player | ref->zone << 8, 8);
    }
        return 0x78;
    }
    return 0;
}
#define EFF_FN (*(u8 (**)(struct CardRef *, int))(gUnk_02017A40 + 0x4F8))
int sub_08039C48(struct CardRef *ref, struct CardRef *card)
{
    if (ref->skip4)
        return 0;
    if (EFF_PHASE == 0x80) {
        sub_08075294(&ES->cur, card, 0x14);
        { int np = 1 - ES->cur.player; ES->cur.player = np; }
        EFF_FN = gUnk_0819A9D4[sub_08047058(ES->cur.id)].fn;
    }
    switch (CARD_NUMBER(card->id)) {
    case 0x151 ... 0x159:
    case 0x3C8:
    case 0x3EE:
    case 0x3EF:
    case 0x3F2:
    case 0x401:
    case 0x40F:
    case 0x42E:
    case 0x42F:
    case 0x439:
        {
            u8 *es = (u8 *)0x02017A40;

            es[0x3E0] = (*(u8 (**)(struct CardRef *, int))(es + 0x4F8))((struct CardRef *)(es + 0x4E4), 0);
            if (es[0x3E0] == 0) {
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80B0 : 0xB0, 1, 0, 0);
                return 0;
            }
            return es[0x3E0];
        }
    }
    return 0;
}
int sub_08039D8C(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 1 && sub_0802C334(ref, ref->targets[0])) {
        int tp = (u8)ref->targets[0];
        int v = sub_0800C894(tp, ref->targets[0] >> 8);
        int j;
        int p;

        j = 0;
        p = 1 - tp;
        for (; j <= 4; j++) {
            struct DuelZone *z = ZB(p & 1, j);

            if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && sub_0800C8A8(p, j) < v && sub_0802B28C(p, j)) {
                sub_08030028(p, j);
                sub_08046CB0(ref->player, p, j);
            }
        }
    }
    return 0;
}
int sub_08039E30(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            int tp = (u8)ref->targets[0];
            int tz = ref->targets[0] >> 8;
            int r = sub_08008A44(ref->player);

            if (tp != ref->player) {
                int p = tp & n;
                struct DuelZone *z = ZB(p, tz);

                if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2) && r != -1 && sub_0800C8BC(tp, tz) == 7) {
                    sub_08019078(ref->player, ref->targets[0], ref->player | (u8)r << 8);
                    sub_08017AB4(ref->player, ref->id, ref->player | (u8)r << 8, 3);
                }
            }
        }
    }
    return 0;
}
int sub_08039EDC(struct CardRef *ref, int arg)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80: {
            struct PlayerLP *pl = gUnk_020192E4;

            if (pl[ref->player].handCount != 0)
                sub_0802272C(ref->player, 1, 0, 0);
            return 0x7F;
        }
        case 0x7F: {
            struct PlayerLP *pl = gUnk_020192E4;

            if (pl[(1 - ref->player) & 1].handCount != 0)
                sub_0802272C(1 - ref->player, 1, 0, 1);
            return 0x7E;
        }
        }
    }
    return 0;
}
#if 0 /* NONMATCHING: same zone-scan loop hoisting as sub_080398B0 (ROM: 1 in r8, 0xD64 in r9, base in r5, 2 in r4), plus ref in r6 and the EFF base in r8 */
int sub_08039F68(struct CardRef *ref)
{
    char buf[0x80];

    switch (EFF_PHASE) {
    case 0x80: {
        int i;
        int num;

        if (sub_08056D98(ref->player, num = 0x4EA, 0x3E7) == -1)
            return 0;
        if (!sub_08008C94(ref->player, num[gUnk_08623DF4]))
            return 0;
        for (i = 0; i <= 1; i++) {
            s16 j;
            struct DuelZone *z = ZB2(i & 1, 0);

            for (j = 0; j <= 4; j++, z++) {
                if (CARD_ID(CARD_WORD(z->card)) && (ZFLAGS(z) & 2))
                    goto found;
            }
        }
        return 0;
    }
    case 0x7F:
        sub_080753F4(buf, gUnk_080834CC, gUnk_0822C720 + gUnk_086247C8[0] * 64);
        sub_080602A4(0x206, 0x613, 0xB, buf);
    ret7E:
        return 0x7E;
    case 0x7E: {
        int p;
        u16 r;
        u32 a;
        u32 b;
        int pos;
        u16 *w;

        if (sub_08052F38(0xE000E0) == 0)
            goto ret7E;
        p = ref->player;
        r = sub_08008C6C(p);
        a = DSV->w824;
        b = DSV->w82C;
        sub_0801EC58(p ? 0x8077 : 0x77, (u8)r | 0x100, (w = &ESH->lo)[0], w[1]);
        sub_08017B04(ref->player, p | (u8)r << 8, pos = (u8)a | (u8)b << 8);
        ES->w542 = pos;
        return 0x7D;
    }
    case 0x7D: {
        int p2 = 1 - (u8)ES->w542;
        int t = sub_08008A44(p2);

        if (t >= 0)
            sub_08019078(ref->player, ES->w542, (u8)p2 | (u8)t << 8);
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
        return 0x7C;
    }
    }
    goto ret0;
found:
    sub_0801967C(ref->player, 0x4EA, (int *)gUnk_02017F84);
    return 0x7F;
ret0:
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_08039638", sub_08039F68); /* 0x08039F68 size 0x214 */
int sub_0803A17C(struct CardRef *ref)
{
    if (!ref->skip4) {
        sub_08018C3C(ref->player, ref->zone);
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
    }
    return 0;
}
int sub_0803A1C0(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (!(1 & ((u8 *)ref)[2])) {
                sub_080602A4(0x206, 0x613, 0xB, gUnk_08083558);
                sub_08060308(2, 0, 0);
            } else {
                gUnk_0201AE60.flag14 = sub_08076F9C() & 1;
            }
            return 0x7F;
        case 0x7F: {
            int r = sub_08076F9C() & 1;

            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80E0 : 0xE0, gUnk_0201AE60.flag14, r, 0);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
            if (r == gUnk_0201AE60.flag14) {
                if (gUnk_020192E4[1 & ref->player].handCount <= 4)
                    sub_080199E0(ref->player, 5 - gUnk_020192E4[1 & ref->player].handCount);
            } else {
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8046 : 0x46, 1, 0, 0);
            }
            return 0xA;
        }
        }
    }
    return 0;
}
int sub_0803A2E4(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 1; i++) {
            if (CARD_ID(CARD_WORD(gUnk_020198D4[i & 1].card)))
                sub_08018544(i, 10, 1);
        }
    }
    return 0;
}
int sub_0803A328(struct CardRef *ref)
{
    if (!ref->skip4) {
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x804A : 0x4A, 2, 0, 0);
        sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x804A : 0x4A, 2, 0, 0);
    }
    return 0;
}
int sub_0803A378(struct CardRef *ref)
{
    int i;

    for (i = 0; i <= 4; i++) {
        if (CARD_ID(CARD_WORD(ZB2(1 & ref->player, i)->card)) && (ZB2(1 & ref->player, i)->flag6_1)
            && sub_0800C8BC(ref->player, i) == 7 && sub_0802B28C(ref->player, i))
            sub_08017AB4(ref->player, ref->id, ref->player | (u8)i << 8, 3);
    }
    return 0;
}
int sub_0803A410(struct CardRef *ref)
{
    if (!ref->skip4) {
        int n = 7 & ((u8 *)ref)[0xA];

        if (n == 1) {
            int tp = (u8)ref->targets[0];
            int tz = ref->targets[0] >> 8;

            if (CARD_ID(CARD_WORD(gUnk_020192E4[tp & n].zones[tz].card))) {
                int count;
                int i;

                sub_08018C3C(tp, tz);
                count = gUnk_020192E4[n = ref->player & n].handCount; /* FAKEMATCH: reuse the dead target count. */
                for (i = 0; i < count; i++)
                    sub_080193B0(ref->player, 0, 1);
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 1, 0, 0);
                sub_080199E0(ref->player, count);
            }
        }
    }
    return 0;
} /* 0x0803A410 size 0xA8 */
u16 sub_0803A4B8(struct CardRef *ref, int arg)
{
    if (ref->skip4)
        return 0;
    if (ref->kind == 0x10) {
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, ref->player, (ref->targets[0] >> 8) << 8, 0);
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8038 : 0x38, ref->targets[0], 1, 0);
        return 0;
    }
    return sub_08036030(ref, arg);
}
int sub_0803A52C(struct CardRef *ref)
{
    u32 *card = &gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (ref->numTargets == 1 && sub_08044224(ref->player, 0x526, ref->targets[0])) {
                sub_080602A4(0x206, 0x712, 0xB, gUnk_08083588);
                return 0x7F;
            }
            break;
        case 0x7F:
            sub_0802AF34(ref->player, -1, 0x526, ref->targets[0]);
            return 0x7E;
        case 0x7E:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, ((u16 *)card)[0], ((u16 *)card)[1], 0);
            return 0x7D;
        case 0x7D:
            sub_08056094(ref->player, &gUnk_0201D810.cards[gUnk_0201D810.row + gUnk_0201D810.top], 0, 0x20);
            return 0x7C;
        case 0x7C:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            return 0x64;
        }
    }
    return 0;
}
