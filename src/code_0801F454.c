#include "global.h"
#include "main.h"
#include "duel.h"
#include "duel_ui.h"

/* 20-byte action entry (lists in gUnk_02017A40; see code_08011BE0). */
struct ActEntry {
    u16 card;           /* +0x00: card ID in bits 0-10 */
    u16 flag2_0:1;      /* +0x02 bit 0 */
    u16 kind2:3;        /* +0x02 bits 1-3 */
    u16 val2_4:6;       /* +0x02 bits 4-9 */
    u16 val2_10:6;      /* +0x02 bits 10-15 */
    u8 flag4_0:1;       /* +0x04 bits 0-4 */
    u8 flag4_1:1;
    u8 flag4_2:1;
    u8 flag4_3:1;
    u8 flag4_4:1;
    u8 unk4_5:3;
    u8 unk5;
    u16 w6;             /* +0x06: low byte = player (hypothesis) */
    u16 w8;             /* +0x08 */
    u8 fillerA[0x14 - 0xA];
};

/* 0x02017A40: two entry lists. */
struct ActLists {
    struct ActEntry listA[32];  /* +0x000 */
    struct ActEntry listB[16];  /* +0x280 */
    u16 countB;                 /* +0x3C0 */
    u16 unk3C2;
    u16 countA;                 /* +0x3C4 */
};
extern struct ActLists gUnk_02017A40;

/* Duel control at 0x02015EE8 (gDuelCtrl, hypothesis). */
struct DuelCtrl {
    u8 phase;           /* +0: index into the phase table 0x08198F80 */
    u8 link:1;          /* +1 bit 0: link duel (hypothesis) */
    u8 unk1_1:7;
};
extern struct DuelCtrl gUnk_02015EE8;

/* Link state at 0x02017FB0 (fields used here; u32 containers, see code_08021CC8). */
struct LinkState {
    u8 filler0[0x200];
    u16 unk200;         /* +0x200 */
    u16 waitTimer;      /* +0x202 */
    u8 filler204[0x304 - 0x204];
    u32 unk304:24;
    u32 unk307_0:1;     /* +0x307 bit 0 */
    u32 unk307_1:1;     /* +0x307 bit 1 */
    u32 unk307_2:1;
    u32 unk307_3:1;     /* +0x307 bit 3 */
    u32 unk307_4:4;
    u8 filler308[0x484 - 0x308];
    u16 cmd[4];         /* +0x484: duel command received from the partner */
    u8 step48C;         /* +0x48C */
};

extern struct LinkState gUnk_02017FB0;

/*
 * duel.h's struct DuelState is documented only up to +0x1B20, but this unit also reads/writes cmdStep
 * at +0x1B40. Unit-local view: the canonical struct from duel.h plus the extra tail byte. It must be a
 * single symbol, otherwise the extra reference adds a literal-pool entry and changes sub_0801F454.
 */
struct DuelStateUnit {
    struct DuelState duel;                          /* canonical fields (duel.h) */
    u8 pad[0x1B40 - sizeof(struct DuelState)];
    u8 cmdStep;                                     /* +0x1B40 */
};
extern struct DuelStateUnit gUnk_020192E0u asm("gUnk_020192E0");

extern u8 gUnk_02015EF0[2];

struct Unk0201AE60 {
    u8 filler0[0x22];
    u8 step22;          /* +0x22 */
    u8 timer23;         /* +0x23 */
};
extern struct Unk0201AE60 gUnk_0201AE60;

extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */
extern const u16 gUnk_08622AB4[];   /* card ID to card number */
#define CARD_STATS(id) (gUnk_08621DE0[(id) & 0x7FF])
#define CARD_NUMBER(id) (gUnk_08622AB4[(id) & 0x7FF])
/* Through a constant address: GCC then loads the mask before the table. */
#define CARD_NUMBER_C(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_STATS_C(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_TYPE_C(id) ((CARD_STATS_C(id) & 0x1F00000) >> 20)
#define CARD_STATS_X(id) CARD_STATS_C(id)
#define CARD_TYPE_X(id) ((CARD_STATS_X(id) & 0x1F00000) >> 20)

void sub_08075294(void *dst, const void *src, u32 size);

/* Magic/Trap subtype (stats bits 17-19) of a Magic or Trap card, else 0 (as in code_08009A68). */
static inline int GetMagicSubtype(u16 id)
{
    u32 stats = CARD_STATS_X(id);

    switch ((int)((stats & 0x1F00000) >> 20)) {
    case 21:
    case 22:
        return (stats & 0xE0000) >> 17;
    default:
        return 0;
    }
}
/* a: bits 0-15 card, 16-20 val, 21-24 kind, 25-30 val2, 31 flag (explicit masks in the ROM) */
void sub_0801FA90(u16 toB, u32 a, u32 b);
u16 sub_080229BC(u16 head, const void *src, int size);
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void sub_0801F81C(void);
void sub_08075278(void *dst, u32 size);    /* zero fill */
void sub_08077BCC(void);
void sub_0801ECA8(void);
void sub_0801EBA8(void);
u16 sub_0802297C(u16 a, u16 b, u16 c, u16 d);
void sub_08058EDC(int player, u16 number);
extern const u16 gUnk_08623DF4[];   /* card number to card id */
#define CARD_ID_TABLE ((const u16 *)0x08623DF4)

/* Card number to card id; 0xFFFF maps to 0 (this copy does not mask numbers below 2000). */
static inline u16 CardNumberToId(u16 n)
{
    if (n == 0xFFFF)
        return 0;
    if (n < 2000)
        return *(CARD_ID_TABLE + n);
    return *(CARD_ID_TABLE + ((n - 2000) & 0x7FF)) + 1;
}
u32 sub_0802D0E0(struct ActEntry *e, int player, int zone);
u32 sub_0802D25C(struct ActEntry *e, int player, int zone);
int sub_08008524(int player, u16 number);

extern u8 gUnk_0201D810[];
extern u8 gUnk_0201CF90[];
extern u8 gUnk_02017A30[];
extern u8 gUnk_02018450[];

#define gMain gUnk_03000040

/* Run the duel command queue: pop the next command into the command block and run it. */
u32 sub_0801F454(void)
{
    int i;

    switch (gUnk_020192E0u.cmdStep) {
    case 0:
        if (gUnk_020185C0.queueCount == 0)
            return 0;
        sub_08075294(&gUnk_020185C0, &gUnk_020185C0.queue[0], 8);
        gUnk_020185C0.queueCount--;
        for (i = 0; i < gUnk_020185C0.queueCount; i++)
            sub_08075294(&gUnk_020185C0.queue[i], &gUnk_020185C0.queue[i + 1], 8);
        gUnk_02017FB0.unk307_1 = 1;
        gUnk_020185C0.running = 1;
        if (gUnk_02015EE8.link) {
            sub_080229BC(0xF041, &gUnk_020185C0, 8);
            gUnk_02017FB0.unk307_1 = 0;
            gUnk_02017FB0.unk200 = 0;
            gUnk_02017FB0.waitTimer = 0;
        }
        gUnk_020185C0.step = 0;
        gUnk_020185C0.timer = 0;
        gUnk_020192E0u.cmdStep++;
    case 1:
        sub_0801ECA8();
        if (gUnk_020185C0.running)
            return 1;
        gUnk_020192E0u.cmdStep++;
    case 2:
        if (!gUnk_02017FB0.unk307_1) {
            if (++gUnk_02017FB0.waitTimer < 0x78)
                return 1;
            gUnk_02017FB0.waitTimer = 0;
            sub_0802297C(0xEE00, 0, 0, 0);
            sub_08077BCC();
            gUnk_020192E0u.duel.linkError = 1;
            return 0;
        }
        gUnk_02017FB0.waitTimer = 0;
        if ((gUnk_020185C0.cmd & 0xFFF) != 4)
            sub_0801EBA8();
        gUnk_020192E0u.cmdStep = 0;
        if (gUnk_020185C0.queueCount)
            return 1;
        break;
    }
    return 0;
}
/* Link duel: run a duel command received from the partner (0x02017FB0+0x484). */
u32 sub_0801F628(void)
{
    switch (gUnk_02017FB0.step48C) {
    case 0:
        sub_08075294(&gUnk_020185C0, gUnk_02017FB0.cmd, 8);
        gUnk_020185C0.running = 1;
        gUnk_020185C0.step = 0;
        gUnk_020185C0.timer = 0;
        gUnk_02017FB0.step48C++;
    case 1:
        sub_0801ECA8();
        if (!gUnk_020185C0.running)
            gUnk_02017FB0.step48C++;
        else if ((gMain.frameCounter & 0xF) == 0)
            sub_0802297C(0xF042, 0, 0, 0);
        return 1;
    case 2:
        if ((gUnk_020185C0.cmd & 0xFFF) != 4)
            sub_0801EBA8();
        sub_0802297C(0xF043, 0, 0, 0);
        gUnk_02017FB0.step48C++;
        return 1;
    }
    gUnk_02017FB0.unk307_0 = 0;
    return 0;
}
/* Duel setup: clear all duel work areas. */
u32 sub_0801F744(void)
{
    sub_08075278(&gUnk_02015EE8, 8);
    sub_08075278(&gUnk_020192E0, 0x1B78);
    sub_08075278(&gUnk_0201CFB0, 0x860);
    sub_08075278(gUnk_0201D810, 0x310);
    sub_08075278(&gUnk_0201AE60, 0x2124);
    sub_08075278(&gUnk_020185C0, 0xD20);
    sub_08075278(gUnk_0201CF90, 0x14);
    sub_08075278(&gUnk_02017A40, 0x56C);
    sub_08075278(gUnk_02017A30, 0x10);
    sub_08075278(&gUnk_02017FB0, 0x494);
    sub_08075278(gUnk_02018450, 0x160);
    gUnk_020192E0.result = 3;
    sub_08077BCC();
    if (gMain.heldKeys & 0x300)
        gUnk_0201CFB0.fast = 1;
    return 1;
}
void sub_0801F81C(void)
{
    u16 number;

    switch (gMain.unk488A_0) {
    case 1:
        number = 0x149;
        break;
    case 2:
        number = 0x14A;
        break;
    case 3:
        number = 0x14B;
        break;
    case 4:
        number = 0x14C;
        break;
    case 5:
        number = 0x14D;
        break;
    case 6:
        number = 0x14E;
        break;
    case 7:
        number = 0x42D;
        break;
    case 8:
        number = 0x465;
        break;
    case 9:
        number = 0x466;
        break;
    case 10:
        number = 0x467;
        break;
    case 11:
        number = 0x468;
        break;
    case 12:
        number = 0x469;
        break;
    case 13:
        number = 0x46A;
        break;
    default:
        return;
    }
    sub_08058EDC(1, number);
    sub_0801EC58(0x8061, 0, 1, 0);
    sub_0801EC58(0x80C5, CardNumberToId(number), 0x10A, 0);
    sub_0801EC58(0x8011, gMain.unk488A_0, 1, 0);
}
/* Duel phase 1 (entry 1 of the phase table 0x08198F80). */
u32 sub_0801F97C(void)
{
    switch (gUnk_020192E0.phaseStep) {
    case 0:
        sub_0801EC58(0x10, 0, 0, 0);
        sub_0801EC58(0x12, 0, 0, 0);
        sub_0801F81C();
        gUnk_020192E0.phaseStep++;
        return 0;
    case 1:
        sub_0801EC58(gUnk_020192E0.linkSkip ? 0x8061 : 0x61, 0, 5, 0);
        sub_0801EC58(!gUnk_020192E0.linkSkip ? 0x8061 : 0x61, 0, 5, 0);
        sub_0801EC58(0x14, 0, 0, 0);
        gUnk_020192E0.phaseStep++;
        return 0;
    default:
        if (gUnk_020192E0.linkSkip) {
            gUnk_02015EF0[0] = 0;
            gUnk_02015EF0[1] = 0;
            gUnk_02015EE8.phase = 8;
            break;
        }
        return 1;
    }
    return 0;
}
u32 sub_0801FA48(struct ActEntry *e)
{
    if (gUnk_02015EE8.link && e->flag2_0 && CARD_NUMBER_C(e->card) != 0x3B6)
        return 1;
    return 0;
}
void sub_0801FA90(u16 toB, u32 a, u32 b)
{
    struct ActEntry *e;
    u16 m[5];

    if (!(a & 0xFFFF))
        return;
    if (gUnk_02015EE8.link && gUnk_020192E0.linkSkip) {
        /* forward to the link partner, from its point of view */
        if (a & 0x80000000)
            a &= ~0x80000000;
        else
            a |= 0x80000000;
        m[0] = toB;
        m[1] = a;
        m[2] = a >> 16;
        m[3] = b;
        m[4] = b >> 16;
        sub_080229BC(0xF072, m, 10);
        return;
    }
    if (toB)
        e = &gUnk_02017A40.listB[gUnk_02017A40.countB];
    else
        e = &gUnk_02017A40.listA[gUnk_02017A40.countA];
    e->card = a;
    e->flag2_0 = a >> 31;
    e->kind2 = (a & 0x1E00000) >> 21;
    e->val2_4 = (a & 0x1F0000) >> 16;
    e->flag4_0 = 0;
    e->flag4_1 = 0;
    e->flag4_2 = 0;
    e->flag4_3 = 0;
    e->flag4_4 = 0;
    e->val2_10 = (a & 0x7E000000) >> 25;
    e->w6 = b;
    e->w8 = b >> 16;
    if (toB)
        gUnk_02017A40.countB++;
    else
        gUnk_02017A40.countA++;
}
void sub_0801FBCC(u32 b, u32 c)
{
    sub_0801FA90(0, b, c);
}
void sub_0801FBE0(u32 b, u32 c)
{
    sub_0801FA90(1, b, c);
}
void sub_0801FBF4(u16 toB, struct ActEntry *src)
{
    struct ActEntry *e;
    u16 v;

    if (toB)
        e = &gUnk_02017A40.listB[gUnk_02017A40.countB];
    else
        e = &gUnk_02017A40.listA[gUnk_02017A40.countA];
    sub_08075294(e, src, 0x14);
    e->flag2_0 = 1;
    e->flag4_0 = 1;
    e->flag4_1 = 1;
    e->flag4_2 = 0;
    e->flag4_3 = 0;
    e->flag4_4 = 0;
    v = src->w6;
    e->w6 = (u8)(1 - v) | ((v >> 8) << 8);
    v = src->w8;
    e->w8 = (u8)(1 - v) | ((v >> 8) << 8);
    if (toB)
        gUnk_02017A40.countB++;
    else
        gUnk_02017A40.countA++;
}
u32 sub_0801FCA8(struct ActEntry *e)
{
    u32 ret = 1;
    u32 stats, type;
    int sub;

    if (e->kind2 == 3 && ((CARD_STATS_C(e->card) & 0x1F00000) >> 20) != 21)
        return 0;
    stats = CARD_STATS_C(e->card);
    type = (stats & 0x1F00000) >> 20;
    if (type <= 20)
        return 0;
    switch ((int)type) {
    case 21:
    case 22:
        sub = (stats & 0xE0000) >> 17;
        break;
    default:
        sub = 0;
        break;
    }
    switch (sub) {
    case 2:
    case 3:
    case 4:
        goto flag;
    }
    /* This empty compiler barrier preserves the ROM's card reload after the
     * subtype test. It emits no instructions and does not modify memory. */
    asm volatile ("" : : : "memory");
    switch (CARD_NUMBER_C(e->card)) {
    case 0x47:
    case 0x15B:
    case 0x4CE:
    case 0x4EA:
    case 0x609:
    flag:
        ret = e->flag4_3;
        break;
    }
    return ret;
}
/*
 * Unit-local zone view for sub_0801FD68. duel.h's struct DuelZone.card is a struct DuelCard.
 * Reading its 12-bit id makes agbcc emit a halfword load, but the ROM loads the whole u32 card
 * word and masks it (card << 20 >> 20). This view keeps a raw u32 card (ID in bits 0-11) so the
 * function matches. All other code in the unit uses the canonical struct DuelZone from duel.h.
 */
struct DuelZoneUnit {
    u32 card;           /* +0x00: card word, ID in bits 0-11 */
    u8 unk4[2];
    u8 unk6_0:1;
    u8 faceUp:1;        /* +0x06 bit 1 */
    u8 unk6_2:6;
    u8 filler7[0x94 - 7];
};
u32 sub_0801FD68(struct ActEntry *e, int player, int kind, int zone)
{
    u32 ret = 1;
    struct DuelZoneUnit *z;
    u32 id;

    switch (kind) {
    case 11:
        if (sub_0802D25C(e, player, zone))
            ret = 0x41;
        break;
    case 5:
        if (sub_0802D0E0(e, player, zone + 5))
            ret = 0x41;
        break;
    case 0:
        player &= 1; z = (struct DuelZoneUnit *)((u8 *)gUnk_0201930C + (zone * 0x94 + player * 0xD64));
        id = z->card << 20 >> 20;
        if (id != 0 && z->faceUp && CARD_NUMBER_C(id) == 0x5F5 && CARD_TYPE_C(e->card) == 22
            && !sub_08008524(0, 0x58A) && !sub_08008524(1, 0x58A) && CARD_NUMBER_C(e->card) != 0x603)
            ret = 0x41;
        break;
    }
    return ret;
}
u32 sub_0801FE54(void)
{
    struct Unk0201AE60 *base = &gUnk_0201AE60;
    u8 *p;
    u32 step;
    u32 copy;
    p = &base->step22;
    step = *p;
    copy = step;
    /* FAKEMATCH: keep the initialized step distinct from the switch copy.
     * This empty constraint emits no instructions and prevents case-value folding. */
    asm volatile ("" : "+r"(step));
    switch (copy) {
    case 0:
        if (!gUnk_02017FB0.unk307_3)
            break;
        goto next;
    case 1:
        if (base->timer23 <= 59) {
            base->timer23++;
            break;
        }
        goto next;
    default:
        return 1;
    next:
        *p = step + 1;
        break;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatching/code_0801F454", sub_0801FEA0); /* 0x0801FEA0 size 0x490 */
/* 0x02017A40 beyond the lists: resolution step and the current entry's two handlers. */
typedef u16 (*ActFn0330)(struct ActEntry *e, struct ActEntry *prev);
struct ActState0330 {
    struct ActEntry listA[32];
    struct ActEntry listB[16];  /* +0x280 */
    u16 countB;                 /* +0x3C0 */
    u8 pad3C2[0x3D0 - 0x3C2];
    u8 flag : 1;                /* +0x3D0 */
    u8 step : 7;
    u8 idx;                     /* +0x3D1 */
    u8 b3D2;
    u8 b3D3;
    u8 pad3D4[0x3E4 - 0x3D4];
    u8 b3E4;
    u8 b3E5;
    u8 pad3E6[0x480 - 0x3E6];
    ActFn0330 fnA;              /* +0x480 */
    ActFn0330 fnB;              /* +0x484 */
    u8 b488;
    u8 pad489[0x490 - 0x489];
    u8 b490;
    u8 b491;
};
extern struct ActState0330 gAct0330 asm("gUnk_02017A40");
struct EffDef0330 { u8 pad[0x10]; ActFn0330 fnA; ActFn0330 fnB; };
extern const struct EffDef0330 gUnk_0819A9D4[];
struct Step150 { u8 pad[0x150]; u8 flag : 1; u8 step : 7; };
extern struct Step150 gUnk_02017CC0;
struct LogEnt0330 { s16 i; struct ActEntry e; };
s32 sub_08047058(u16 card);
void sub_080197C0(int player, u16 card);
void sub_0801A7B4(void *p, int a);
s32 sub_0801A32C(void);
s32 sub_0801FEA0(struct ActEntry *e, u32 player);
s32 sub_0802D30C(struct ActEntry *e, u32 player);
#define LAST0330 (&gAct0330.listB[gAct0330.countB - 1])
#if 0 /* NONMATCHING: 1142 lines; first full draft (same 0x100 frame and case order); case bodies differ in size */
/* Effect chain resolution, one step per call: for each queued entry look up its two handlers (table at
 * 0x0819A9D4), run them, then log the chain over the link and hand control to the players (hypothesis). */
int sub_08020330(void)
{
    switch (gAct0330.step) {
    case 0:
        gAct0330.idx = 0;
        gAct0330.step++;
    case 1: {
        struct ActEntry *e;
        s32 r = sub_08047058(gAct0330.listB[gAct0330.idx].card);
        if (r == -1) {
            gAct0330.fnA = NULL;
            gAct0330.fnB = NULL;
        } else {
            gAct0330.fnA = gUnk_0819A9D4[r].fnA;
            gAct0330.fnB = gUnk_0819A9D4[r].fnB;
        }
        if (gAct0330.listB[gAct0330.idx].flag4_0)
            gAct0330.fnA = NULL;
        e = &gAct0330.listB[gAct0330.idx];
        if (e->flag4_1)
            gAct0330.fnB = NULL;
        sub_080197C0(e->flag2_0, e->card);
        gUnk_02017FB0.filler308[0] &= ~2 & ~8;
        gAct0330.b3E4 = 0;
        gAct0330.b3E5 = 0;
        gAct0330.step++;
        break;
    }
    case 2:
        if (gAct0330.fnA == NULL) {
            gAct0330.step += 2;
            break;
        }
        if ((u16)sub_0801FA48(&gAct0330.listB[gAct0330.idx]))
            sub_080229BC(0xF091, &gAct0330.listB[gAct0330.idx], 0x14);
        gAct0330.step++;
    case 3:
        if ((u16)sub_0801FA48(&gAct0330.listB[gAct0330.idx]) == 0) {
            if (gAct0330.fnA(&gAct0330.listB[gAct0330.idx],
                             gAct0330.countB > 1 ? &gAct0330.listB[gAct0330.countB - 2] : NULL))
                gUnk_02017FB0.filler308[0] |= 8;
        }
        if (gUnk_02017FB0.filler308[0] & 8)
            gAct0330.step++;
        break;
    case 4:
        if (gAct0330.fnB == NULL) {
            gAct0330.step += 2;
            break;
        }
        if ((u16)sub_0801FA48(&gAct0330.listB[gAct0330.idx]))
            sub_080229BC(0xF081, &gAct0330.listB[gAct0330.idx], 0x14);
        gAct0330.step++;
    case 5:
        if ((u16)sub_0801FA48(&gAct0330.listB[gAct0330.idx]) == 0) {
            if (gAct0330.fnB(&gAct0330.listB[gAct0330.idx],
                             gAct0330.countB > 1 ? &gAct0330.listB[gAct0330.countB - 2] : NULL))
                gUnk_02017FB0.filler308[0] |= 2;
        }
        if (gUnk_02017FB0.filler308[0] & 2)
            gAct0330.step++;
        break;
    case 6:
        gAct0330.idx++;
        if (gAct0330.idx < gAct0330.countB) {
            gAct0330.step = 1;
            break;
        }
        gAct0330.step++;
    case 7:
        if (gUnk_02015EE8.link) {
            s16 i;
            u8 buf[0x100];
            sub_0802297C(0xF061, gAct0330.countB, 0, 0);
            for (i = 0; i < gAct0330.countB; i++) {
                *(s16 *)buf = i;
                sub_08075294(buf + 2, &gAct0330.listB[i], 0x14);
                sub_080229BC(0xF062, buf, 0x16);
            }
            sub_0802297C(0xF063, gAct0330.countB, 0, 0);
            gUnk_02017FB0.filler308[0] &= 0x7F;
        }
        gAct0330.step++;
        break;
    case 8:
        sub_0801A7B4(&gUnk_02017CC0, 0);
        gUnk_02017CC0.step++;
        break;
    case 9:
        if (sub_0801A32C())
            gAct0330.step++;
        break;
    case 10:
        if (gUnk_02015EE8.link && !(gUnk_02017FB0.filler308[0] >> 7))
            break;
        gAct0330.b488 &= ~1;
        gAct0330.step++;
    case 11:
        if (sub_0802D30C(LAST0330, 1 - LAST0330->flag2_0)) {
            gAct0330.b490 = 0;
            gAct0330.b491 &= 0xF0 & 0x7F;
        } else {
            gAct0330.step++;
        }
        gAct0330.step++;
        break;
    case 12:
        if ((u16)sub_0801FEA0(LAST0330, 1 - LAST0330->flag2_0)) {
            if (gAct0330.b491 & 0x80)
                gAct0330.step = 1;
            else
                gAct0330.step++;
        }
        break;
    case 13:
        if (sub_0802D30C(LAST0330, LAST0330->flag2_0)) {
            gAct0330.b490 = 0;
            gAct0330.b491 &= 0x7F;
        } else {
            gAct0330.step++;
        }
        gAct0330.step++;
        break;
    case 14:
        if ((u16)sub_0801FEA0(LAST0330, LAST0330->flag2_0)) {
            if (gAct0330.b491 & 0x80)
                gAct0330.step = 1;
            else
                gAct0330.step++;
        }
        break;
    default:
        gAct0330.flag = 0;
        gAct0330.b3D2 = 1;
        gAct0330.b3D3 = 0;
        break;
    }
    return 1;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0801F454", sub_08020330); /* 0x08020330 size 0x7C4 */
