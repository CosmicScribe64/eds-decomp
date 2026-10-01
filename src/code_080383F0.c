#include "global.h"
#include "duel.h"

/*
 * Duel card-effect executors with the signature int f(struct CardRef *ref). They return 0
 * or a step state (0x7E/0x7F). See wiki/functions/code-080383f0.md.
 */

/* struct DuelCard / DuelZone / DuelZonesPlayer come from duel.h. */
#define ZFLAGS(z) (((u8 *)(z))[6])
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
/* Same address, but with the player term first (the ROM adds the terms in this order in some places). */
#define ZBP(p, z) ((struct DuelZone *)((p) * 0xD64 + (z) * 0x94 + (u32)gUnk_0201930C))

struct CardRef {
    u16 id;             /* +0x00 */
    u8 player : 1;      /* +0x02 bit 0 */
    u8 unk2_1 : 3;
    u16 zone : 6;       /* +0x02 bits 4-9 */
    u16 kind : 6;
    u8 unk4_0 : 2;
    u8 skip4 : 1;       /* +0x04 bit 2: effect already handled/skipped (hypothesis) */
    u8 flag4_3 : 1;     /* +0x04 bit 3 */
    u8 unk4_4 : 4;
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
#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_ID11(w) (((w) << 21) >> 21)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

extern u8 gUnk_02017A40[];
#define EFF_PHASE gUnk_02017A40[0x3E0]  /* 0x7D-0x80: step of a multi-step effect (hypothesis) */
#define EFF_SIDE gUnk_02017A40[0x3E1]
/* gUnk_020192E0 is declared as struct DuelState in duel.h; below +0x1ACC it is addressed as bytes. */

void sub_08022678(int a, int b, int c, int d);
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void sub_08030028(int player, int zone);
void sub_08046CB0(int player, int a, int b);
int sub_08076F9C(void);
int sub_08044224(int player, int number, int b);
void sub_08056094(int player, u32 *card, int a, int b);
extern u16 gUnk_0201D81C[];   /* = gUnk_0201D810.cards[0] (list viewer, see code_0802AAC0) */
void sub_08019788(int player, int id);
int __modsi3(int a, int b);
void sub_08017AB4(int player, int a, u16 b, u16 c);
struct AE60 {
    u8 unk0[0x14];
    u16 flag14;
};
extern struct AE60 gUnk_0201AE60;
extern char gUnk_080831E4[];
void sub_080602A4(int a, int b, int c, char *s);
void sub_08060308(int a, int b, int c);
int sub_0802F200(struct CardRef *ref, int a, int b);
u32 sub_08052F38(u32 keys);
int sub_08007834(u16 id);
int sub_08008A44(int player);
int sub_08008AF8(int player, int exclude);
u16 sub_08008A6C(int player, int zone);
void sub_08077AEC(u16 id);
void sub_08017FF4(int player, int zone);
void sub_08055D3C(int player, int a, int b, int c);
void sub_08075294(void *dest, const void *src, u32 size);
int sub_080304E4(void *ref, int a);
int sub_080307D4(void *ref, int a);
extern const u32 gUnk_08621DE0[];
extern const u16 gUnk_08622AB4[];
extern char gUnk_08083300[];
extern char gUnk_08083350[];
extern char gUnk_0808339C[];
extern char gUnk_080833EC[];
/* Local view for sub_08038FB8 (non-matching draft, under `#if 0`). The canonical struct DuelPlayer
 * declares its byte at +8 as a plain u8 (unk8), but this draft reads and writes bit 4 of it
 * (the canonical field differs: unk8 as a u8 versus flag8_4 as bit 4), so the unit keeps a
 * bitfield view for that one byte. */
struct PlayerStateLocal {
    u16 lifePoints;
    u8 handCount;       /* +2 */
    u8 deckCount;       /* +3 */
    u8 pad4[4];
    u8 unk8_0 : 4;
    u8 flag8_4 : 1;     /* +8 bit 4 */
    u8 unk8_5 : 3;
    u8 pad9[0xD64 - 9];
};
extern struct PlayerStateLocal gPlayerState[2] asm("gUnk_020192E4");
struct HandRow {        /* 0x02019968 = 0x020192E4 + 0x684: hand card words */
    struct DuelCard c[80];
    u8 pad[0xD64 - 80 * 4];
};
extern struct HandRow gUnk_02019968[2];
/* Duel screen state at 0x0201CFB0: +0x824.. hold the selection */
struct DuelScreen {
    u8 pad0[0x824];
    u32 w824;           /* low half: player-ish selection (hypothesis) */
    u32 w828;           /* low byte: column */
    u32 idx82C;         /* +0x82C: selected index (low byte: row) */
};
extern struct CardRef gUnk_02017F24;
extern struct DuelScreen gUnk_0201CFB0;

/* Monster level: Magic/Trap types 0x15-0x17 count as 0, type 0x18 as 10. */
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
void sub_08019078(int player, u16 a, u16 b);

int sub_080383F0(struct CardRef *ref)
{
    if (!ref->skip4) {
        int i;

        for (i = 0; i <= 10; i++) {
            int pp = (1 - ref->player) & 1;
            struct DuelZone *z = ZB(pp, i);

            if (CARD_WORD(z->card) << 20 != 0) {
                struct DuelZone *z2 = ZB((1 - ref->player) & 1, i);

                if ((2 & ZFLAGS(z2)) == 0) {
                    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, 1 - ref->player, (u8)i << 8, 0);
                    sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x807F : 0x7F, i, 0, 0);
                    sub_08019788(ref->player, CARD_ID(CARD_WORD(ZBP((1 - ref->player) & 1, i)->card)));
                    sub_0801EC58(!(1 & ((u8 *)ref)[2]) ? 0x807F : 0x7F, i, 0, 0);
                }
            }
        }
    }
    return 0;
}
#if 0 /* NONMATCHING: the constant 1 is shared by all three "1 & x" tests (r7/r4 live across the
       * call), while the ROM has a separate `movs r0,#1` for the first one. This shifts the
       * pl/msg/1 registers (r4/r5/r7 vs r7/r4/r4). */
int sub_080384F4(struct CardRef *ref)
{
    int r = sub_08076F9C() % 6 + 1;

    if (!ref->skip4) {
        s16 msg;
        int pl;
        u8 plb;

        switch (CARD_NUMBER(ref->id)) {
        case 0x4B3:
            pl = ref->player;
            break;
            msg = 0xE2;
        case 0x4B4:
            pl = 1 - ref->player;
            msg = 0xE3;
            break;
        }
        if (1 & ((u8 *)ref)[2])
            msg |= 0x8000;
        sub_0801EC58(msg, r, 0, 0);
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
        plb = pl;
        {
            int i;

            for (i = 0; i <= 4; i++) {
                struct DuelZone *z = ZB(pl & 1, i);

                if ((2 & ZFLAGS(z)) != 0 && CARD_WORD(z->card) << 20 != 0)
                    sub_08017AB4(ref->player, ref->id, (u32)i << 24 >> 16 | plb, (0x30000 | r << 24) >> 16);
            }
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080383F0", sub_080384F4); /* 0x080384F4 size 0x110 */
int sub_08038604(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            sub_08022678(ref->player, 6, 0, 0);
            return 0x7F;
        case 0x7F: {
            u8 *g = (u8 *)&gUnk_020192E0;

            ref->targets[0] = *(u16 *)(g + 0x1B64);
            sub_08022678(1 - ref->player, 6, 0, 0);
            return 0x7E;
        }
        case 0x7E: {
            u8 *g = (u8 *)&gUnk_020192E0;
            u16 *p = (u16 *)(g + 0x1B64);

            ref->targets[1] = *p;
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80C7 : 0xC7, *p, ref->targets[0], 0);
            return 0x64;
        }
        }
    }
    return 0;
}
#if 0 /* NONMATCHING: complete translation, 0x58C versus ROM 0x588; 640 differing bytes.
       * Proper 0x200-byte text frame; loop player scheduling, shared result tails,
       * count/flag register homes and pool placement still differ. */
struct CoinListView {
    u8 unk0[5];
    u8 row : 2;
    u8 unk5_2 : 6;
    u16 top;
    u8 unk8[4];
    s8 cards[0x80];
    int kinds[0x80];
    u16 count;
};
extern struct CoinListView gUnk_0201D810;
int sub_08008A1C(int player);
int sub_08008524(int player, u16 number);
int sub_080086CC(int player, u16 number);
int sub_0800C894(int player, int zone);
int sub_080754A4(int value);
void sub_08019860(int player, int amount);
void sub_080753F4(char *dest, const char *fmt, const char *arg);
void sub_0802AF34(int player, int index, int number, int arg);
int sub_0801970C(int player, u16 number);
extern char gUnk_08083214[];
extern char gUnk_08083250[];
extern char gUnk_08083288[];
extern char gUnk_080832AC[];
extern const char gUnk_0822C720[][0x40];
extern const u16 gUnk_08623E38;
extern const u16 gUnk_08624758;

int sub_080386B0(struct CardRef *ref)
{
    char text[0x100];
    char intermediate[0x100];

    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (!(1 & ((u8 *)ref)[2])) {
                sub_080602A4(0x206, 0x613, 0xB, gUnk_080831E4);
                sub_08060308(2, 0, 0);
            } else {
                gUnk_0201AE60.flag14 = sub_08076F9C() & 1;
            }
            return 0x7F;
        case 0x7F: {
            u8 random = sub_08076F9C() & 1;
            int i;

            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80E0 : 0xE0, gUnk_0201AE60.flag14, random, 0);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
            if (random == gUnk_0201AE60.flag14) {
                for (i = 0; i <= 4; i++) {
                    int p = 1 - ref->player;
                    struct DuelZone *z = ZB(p & 1, i);

                    if (CARD_WORD(z->card) << 20) {
                        sub_08030028(1 - ref->player, i);
                        sub_08046CB0(ref->player, 1 - ref->player, i);
                    }
                }
                if (sub_08008A1C(ref->player) <= 0)
                    goto retA;
                if (sub_08044224(ref->player, 0xF, 0) <= 0)
                    goto retA;
                if (sub_08008524(0, 0x58A) != 0 || sub_08008524(1, 0x58A) != 0)
                    goto retA;
                if (sub_080086CC(ref->player, 0x22) == 0 && sub_080086CC(ref->player, 0x4BA) == 0 && sub_080086CC(ref->player, 0x7F2) == 0)
                    goto retA;
                return 0x78;
            } else {
                int total = 0;

                for (i = 0; i <= 4; i++) {
                    int p = ref->player;
                    struct DuelZone *z = ZB(p, i);

                    if (CARD_WORD(z->card) << 20) {
                        total += sub_0800C894(ref->player, i);
                        sub_08030028(ref->player, i);
                    }
                }
                sub_08019860(ref->player, sub_080754A4(total));
                goto retA;
            }
        }
        case 0x78:
            sub_080753F4(intermediate, gUnk_08083214, gUnk_0822C720[gUnk_08623E38]);
            sub_080753F4(text, intermediate, gUnk_0822C720[gUnk_08624758]);
            sub_080602A4(0x206, 0x613, 0xB, text);
            sub_08060308(1, 0, 0);
            return 0x77;
        case 0x77: {
            int hasOne = 0;
            int hasTwo = 0;
            int count;

            if (gUnk_0201AE60.flag14 == 0)
                goto retA;
            count = gUnk_0201D810.count;
            if (count != 0) {
                u16 *kind = gUnk_0201D810.kinds;

                do {
                    if (*kind != 1) {
                        if (*kind == 2)
                            hasTwo = 1;
                    } else {
                        hasOne = 1;
                    }
                    kind++;
                    count--;
                } while (count != 0);
            }
            if (hasOne) {
                if (hasTwo) {
                    sub_080602A4(0x206, 0x613, 0xB, gUnk_08083250);
                    sub_08060308(2, 0, 0);
                    return 0x76;
                }
                ref->targets[0] = 1;
                return 0x6E;
            }
            if (hasTwo) {
                ref->targets[0] = 2;
                return 0x6E;
            }
            goto retA;
        }
        case 0x76:
            switch (gUnk_0201AE60.flag14) {
            case 0:
                ref->targets[0] = 1;
                break;
            case 1:
                ref->targets[0] = 2;
                break;
            }
            return 0x6E;
        case 0x6E:
            sub_080753F4(text, gUnk_08083288, gUnk_0822C720[gUnk_08623E38]);
            sub_080602A4(0x206, 0x613, 0xB, text);
        ret6D:
            return 0x6D;
        case 0x6D:
            if (sub_08052F38(0xE0) != 0) {
                int player = gUnk_0201CFB0.w824;
                int zone = gUnk_0201CFB0.w828 + gUnk_0201CFB0.idx82C;
                int number = gUnk_08622AB4[CARD_ID11(CARD_WORD(ZB(player & 1, zone)->card))];

                switch (number) {
                case 0x22:
                case 0x4BA:
                case 0x7F2:
                    sub_08017FF4(player, zone);
                    return 0x64;
                default:
                    sub_08077AEC(3);
                    goto ret6D;
                }
            }
            goto ret6D;
        case 0x64: {
            int i;

            for (i = 0; i < gUnk_0201D810.count; i++) {
                if (gUnk_0201D810.kinds[i] == ref->targets[0]) {
                    u32 *card = &gUnk_0201D810.cards[i];

                    sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80C2 : 0xC2, ((u16 *)card)[0], ((u16 *)card)[1], 0);
                    sub_08056094(ref->player, card, 1, 1);
                    return 0x63;
                }
            }
        retA:
            return 0xA;
        }
        case 0x63:
            return 0x62;
        case 0x62:
            if (sub_08044224(ref->player, 0x4B2, 0) > 0) {
                sub_080602A4(0x206, 0x613, 0xB, gUnk_080832AC);
                return 0x61;
            }
            goto retA;
        case 0x61:
            sub_0802AF34(ref->player, -1, 0x4B2, 0);
            return 0x60;
        case 0x60: {
            int p = ref->player;
            u32 card = gUnk_0201D810.cards[gUnk_0201D810.top + gUnk_0201D810.row];

            if (sub_0801970C(p, gUnk_08622AB4[CARD_ID11(card)]))
                sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8060 : 0x60, 0, 0, 0);
            goto retA;
        }
        default:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080383F0", sub_080386B0); /* 0x080386B0 size 0x588 */
int sub_08038C38(struct CardRef *ref)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (!(1 & ((u8 *)ref)[2])) {
                sub_080602A4(0x206, 0x613, 0xB, gUnk_080831E4);
                sub_08060308(2, 0, 0);
            } else {
                gUnk_0201AE60.flag14 = sub_08076F9C() & 1;
            }
            return 0x7F;
        case 0x7F: {
            u8 r = sub_08076F9C() & 1;

            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80E0 : 0xE0, gUnk_0201AE60.flag14, r, 0);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
            sub_08017AB4(ref->player, ref->id, ref->player | ref->zone << 8, r == gUnk_0201AE60.flag14 ? 0x103 : 3);
            return 0xA;
        }
        default:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
            break;
        }
    }
    return 0;
}
int sub_08038D48(struct CardRef *ref)
{
    if (!ref->skip4) {
        int mask = 0;
        int cnt = 0;
        int i;

        for (i = 0; i <= 2; i++) {
            if ((sub_08076F9C() & 1) == 0)
                cnt++;
            else
                mask |= 1 << i;
        }
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x80E1 : 0xE1, mask, 0, 0);
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8012 : 0x12, 0, 0, 0);
        if (cnt > 1) {
            sub_08030028(1 - ref->player, ref->targets[0] >> 8);
            sub_08046CB0(ref->player, 1 - ref->player, ref->targets[0] >> 8);
        }
        sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8092 : 0x92, ref->zone, 0, 0);
    }
    return 0;
}
int sub_08038E18(struct CardRef *ref)
{
    u16 *cw = gUnk_0201D81C;

    if (ref->skip4) {
        if (!ref->flag4_3)
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8049 : 0x49, 1, 1, 0);
    } else {
        switch (EFF_PHASE) {
        case 0x80:
            if (sub_08044224(ref->player, CARD_NUMBER(ref->id), 0) == 0)
                break;
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8065 : 0x65, cw[0], cw[1], 0);
            return 0x7F;
        case 0x7F:
            sub_08056094(ref->player, (u32 *)cw, 1, 0);
            return 0x7E;
        default:
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8049 : 0x49, 1, 1, 0);
            break;
        }
    }
    return 0;
}
int sub_08038EF0(struct CardRef *ref)
{
    if (!ref->skip4 && ref->numTargets == 2) {
        u8 tp1 = ref->targets[0];
        int tz1 = ref->targets[0] >> 8;
        u8 tp2 = ref->targets[1];
        int tz2 = ref->targets[1] >> 8;

        switch (EFF_PHASE) {
        case 0x80: {
            int pa = tp1 & 1;
            struct DuelZone *za = ZB(pa, tz1);

            if (CARD_WORD(za->card) << 20 != 0) {
                int pb = tp2 & 1;
                struct DuelZone *zb = ZB(pb, tz2);

                if (CARD_WORD(zb->card) << 20 != 0) {
                    if (tp1 != ref->player && tp2 == ref->player) {
                        sub_08030028(tp1, tz1);
                        sub_08046CB0(ref->player, tp1, tz1);
                        return 0x7F;
                    }
                }
            }
            break;
        }
        case 0x7F:
            sub_08019078(ref->player, ref->targets[1], ref->targets[0]);
            break;
        }
    }
    return 0;
}
#if 0 /* WIP, NOT MATCHING: first full draft of every arm (0x62-0x80). The structure is verified by
       * hand against the asm, but register allocation and the frame differ (the ROM has
       * `sub sp,#0x80` and no spills, `ref` in r6, arg in r3, and the base of 0x02017A40 kept
       * in r2). */
int sub_08038FB8(struct CardRef *ref, int arg)
{
    if (!ref->skip4) {
        switch (EFF_PHASE) {
        case 0x80:
            if (sub_0802F200(ref, arg, 0) != 0)
                return 0x7F;
            break;
        case 0x7F: {
            s16 found = 0;
            int has = 0;
            int i;

            if (!gPlayerState[ref->player].flag8_4) {
                for (i = 0; i < gPlayerState[1 & ref->player].handCount; i++) {
                    int id = CARD_ID(CARD_WORD(gUnk_02019968[ref->player].c[i]));
                    u32 lvl;

                    if (CARD_TYPE(id) <= 0x14 && sub_08007834(id) == 0) {
                        CARD_LEVEL(id, lvl);
                        if (lvl > 4) {
                            CARD_LEVEL(id, lvl);
                            if (lvl <= 6) {
                                if (sub_08008A44(ref->player) != -1)
                                    found = 1;
                            } else {
                                if (sub_08008AF8(ref->player, -1) > 0)
                                    found = 1;
                            }
                        }
                    }
                }
            }
            if (gPlayerState[ref->player].flag8_4)
                found = 0;
            for (i = 0; i <= 4; i++) {
                struct DuelZone *z = ZB(1 & ref->player, i);
                int id = CARD_ID(CARD_WORD(z->card));

                if (id != 0) {
                    struct DuelZone *z2 = ZB(1 & ref->player, i);

                    if ((2 & ZFLAGS(z2)) != 0) {
                        switch (gUnk_08622AB4[id & 0x7FF]) {
                        case 0x58:
                        case 0x105:
                        case 0x1FF:
                            has = 1;
                            break;
                        }
                    }
                }
            }
            if (found != 0) {
                if (has == 0)
                    return 0x78;
                sub_080602A4(0x204, 0x717, 0xB, gUnk_08083300);
                sub_08060308(2, 0, 0);
                return 0x7E;
            }
            if (has != 0)
                return 0x64;
            break;
        }
        case 0x7E:
            if (gUnk_0201AE60.flag14 == 0)
                return 0x78;
            return 0x64;
        case 0x78:
            sub_080602A4(0x206, 0x712, 0xB, gUnk_08083350);
            gPlayerState[ref->player].flag8_4 = 1;
            return 0x77;
        case 0x77: {
            int idx;
            int id;
            u32 lvl;

            if (sub_08052F38(1) == 0)
                return 0x77;
            idx = gUnk_0201CFB0.idx82C;
            id = CARD_ID(CARD_WORD(gUnk_02019968[ref->player].c[idx]));
            if (CARD_TYPE(id) <= 0x14 && sub_08007834(id) == 0) {
                CARD_LEVEL(id, lvl);
                if (lvl > 4) {
                    CARD_LEVEL(id, lvl);
                    if (lvl <= 6) {
                        int p;

                        sub_08017FF4(ref->targets[0], 0);
                        p = ref->player;
                        sub_08055D3C(p, idx, sub_08008A44(p), 0);
                        return 0x6E;
                    }
                    sub_080602A4(0x206, 0x712, 0xB, gUnk_0808339C);
                    ref->targets[1] = idx;
                    return 0x76;
                }
            }
            sub_08077AEC(3);
            return 0x77;
        }
        case 0x76:
            if (sub_08052F38(0xF0) == 0)
                return 0x76;
            if (sub_08008A6C(ref->player, gUnk_0201CFB0.idx82C) == 0) {
                sub_08077AEC(3);
                return 0x76;
            }
            sub_08017FF4(ref->targets[0], 0);
            sub_08017FF4(ref->player, gUnk_0201CFB0.idx82C);
            sub_08055D3C(ref->player, ref->targets[1], gUnk_0201CFB0.idx82C, 0);
            return 0x6E;
        case 0x64:
            sub_080602A4(0x206, 0x411, 0xB, gUnk_080833EC);
            return 0x63;
        case 0x63: {
            int p7;
            int pos;
            struct DuelZone *z;

            if (sub_08052F38(0xE0) == 0)
                return 0x63;
            p7 = gUnk_0201CFB0.w824;
            pos = gUnk_0201CFB0.w828 + gUnk_0201CFB0.idx82C;
            z = ZB(1 & p7, pos);
            switch (gUnk_08622AB4[CARD_ID11(CARD_WORD(z->card))]) {
            case 0x58:
            case 0x105:
            case 0x1FF:
                break;
            default:
                sub_08077AEC(3);
                return 0x63;
            }
            sub_08077AEC(1);
            sub_0801EC58((1 & ((u8 *)ref)[2]) ? 0x8008 : 8, *(u16 *)&gUnk_0201CFB0.w824,
                         (*(u8 *)&gUnk_0201CFB0.idx82C << 8) | *(u8 *)&gUnk_0201CFB0.w828, 0);
            sub_08075294(&gUnk_02017F24, ref, 0x14);
            gUnk_02017F24.id = CARD_ID(CARD_WORD(ZB(1 & p7, pos)->card));
            gUnk_02017F24.zone = pos;
            return 0x62;
        }
        case 0x62: {
            struct CardRef *r62 = (struct CardRef *)&gUnk_02017A40[0x4E4];

            switch (gUnk_08622AB4[r62->id & 0x7FF]) {
            case 0x105:
                sub_080307D4(r62, 0);
                return 0x61;
            case 0x58:
            case 0x1FF:
                sub_080304E4(&gUnk_02017F24, 0);
                return 0x61;
            }
            break;
        }
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080383F0", sub_08038FB8); /* 0x08038FB8 size 0x680 */
