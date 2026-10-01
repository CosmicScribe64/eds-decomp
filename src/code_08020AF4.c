#include "global.h"

/* Per-player duel state (0xD64 bytes), two of them at 0x020192E4 (fields used here). */
struct DuelPlayer {
    u16 lifePoints;     /* +0x000 */
    u8 filler2[5];
    u8 deckOut:1;       /* +0x007 bit 0: lost by drawing from an empty deck (hypothesis) */
    u8 winA:1;          /* +0x007 bit 1: sub_08021580 (all five zones 0x10-0x14) */
    u8 winExodia:1;     /* +0x007 bit 2: sub_080215CC (five special cards, hypothesis: Exodia) */
    u8 unk7_3:5;
    u8 filler8[0xD64 - 8];
};
extern struct DuelPlayer gUnk_020192E4[2];

/* Selection widget at 0x020192E0+0x1B2C (layout from code_0801CE68). */
struct SelMask {
    u16 flag0:1;        /* bit 0 */
    u16 active:1;       /* bit 1 */
    u16 cursor:4;       /* bits 2-5 */
    u16 rows:4;         /* bits 6-9 */
    u32 mask:16;        /* bits 10-25 */
    u16 state:8;        /* bits 26-33 (straddles 0x1B2F/0x1B30) */
    u32 unk34:8;
    u32 unk42:8;
    u16 timer:7;
    u16 player:1;
    u32 zone:7;
    u32 unk65:8;
    u32 unk73:23;
};

/* 0x020192E0 duel state (fields used here). */
struct DuelState {
    u32 unk0;
    struct DuelPlayer players[2];   /* +0x004 (= 0x020192E4) */
    union { u8 raw; struct { u8 low:4; u8 over:1; u8 high:3; } __attribute__((packed)) bits; } __attribute__((packed)) flags1ACC;
    u8 effectFlags;
    u8 filler1ACE[0x1B10 - 0x1ACE];
    u16 unk1B10;        /* +0x1B10 */
    u8 unk1B12_0:1;
    u8 linkSkip:1;      /* +0x1B12 bit 1 (see code_08011BE0) */
    u8 unk1B12_2:3;
    u8 linkError:1;     /* +0x1B12 bit 5 */
    u8 result:2;        /* +0x1B12 bits 6-7: duel result 1..3 (hypothesis) */
    u8 unk1B13_0:1;     /* +0x1B13 bit 0 */
    u8 unk1B13_1:7;
    u8 unk1B14;         /* +0x1B14: bit 1 = surrender/end requested (hypothesis) */
    u8 filler1B15[0x1B20 - 0x1B15];
    u8 phaseStep;       /* +0x1B20 */
    u8 phaseTimer;      /* +0x1B21 */
    u8 filler1B22[0x1B2C - 0x1B22];
    struct SelMask sel; /* +0x1B2C: selection widget (see code_0801CE68) */
};
/* Command block at 0x020185C0 (see code_0801F454): queue count at +0x808. */
struct DuelCmd {
    u8 filler0[0x808];
    u16 queueCount;     /* +0x808 */
};
extern struct DuelCmd gUnk_020185C0;
extern struct DuelState gUnk_020192E0;

/* Duel control at 0x02015EE8 (gDuelCtrl, hypothesis). */
struct DuelCtrl {
    u8 phase;           /* +0: index into the phase table 0x08198F80 */
    u8 link:1;          /* +1 bit 0: link duel (hypothesis) */
    u8 unk1_1:7;
};
extern struct DuelCtrl gUnk_02015EE8;

/* gMain (0x03000040): only the fields used here. */
struct Main {
    u8 filler0[0x4870];
    u16 unk4870_0:1;     /* +0x4870 bit 0 */
    u16 unk4870_1:5;
    u16 result:2;        /* +0x4870 bits 6-7: last duel result (copied from 0x020192E0+0x1B12) */
    u16 unk4870_8:8;
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040

/* 20-byte action entry (lists in gUnk_02017A40; see code_0801F454). */
struct ActEntry {
    u16 card;           /* +0x00: card ID in bits 0-10 */
    u16 flag2_0:1;      /* +0x02 bit 0: player (hypothesis) */
    u16 kind2:3;        /* +0x02 bits 1-3 */
    u16 val2_4:6;
    u16 val2_10:6;
    u8 flag4_0:1;
    u8 flag4_1:1;
    u8 flag4_2:1;
    u8 flag4_3:1;
    u8 flag4_4:1;
    u8 unk4_5:3;
    u8 unk5;
    u16 w6;
    u16 w8;
    u8 fillerA[0x14 - 0xA];
};

/* 0x02017A40: two entry lists. */
struct ActLists {
    struct ActEntry listA[32];  /* +0x000 */
    struct ActEntry listB[16];  /* +0x280 */
    u16 countB;                 /* +0x3C0 */
    u16 unk3C2;
    u16 countA;                 /* +0x3C4 */
    u8 filler3C6[0x3D0 - 0x3C6];
    u8 unk3D0;                  /* +0x3D0: bit 0 set calls sub_08020330 */
    u8 unk3D1;
    union { u8 raw; struct { u8 active:1; u8 stage:7; } __attribute__((packed)) bits; } __attribute__((packed)) resolveFlags;
    u8 unk3D3;
    u16 unk3D4;
    s16 effectIndex;
    u32 (*resolve)(struct ActEntry *, struct ActEntry *);
    u32 savedCard;
    u8 effectStep;
    u8 effectSub;
};
extern struct ActLists gUnk_02017A40;

extern const u16 gUnk_08622AB4[];   /* card ID to card number */
#define CARD_NUMBER(id) (gUnk_08622AB4[(id) & 0x7FF])
#define CARD_NUMBER_C(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

void sub_08075294(void *dst, const void *src, u32 size);
int sub_08009CAC(int player, u16 number);
u32 sub_08020330(void);
u32 sub_08020AF4(void);
u32 sub_0800A2A8(int player, int zone);
u16 sub_08021580(int player);
u16 sub_080215CC(int player);
int sub_08008524(int player, u16 number);
u16 sub_0802297C(u16 a, u16 b, u16 c, u16 d);
void sub_0801EC58(u16 msg, u16 arg1, u16 arg2, u16 arg3);
void sub_08077BA0(void);
u16 sub_08024440(void);
u16 sub_0801F628(void);
u16 sub_0801F454(void);
u16 sub_0802AED8(void);
u16 sub_08060344(void);
u16 sub_080222F8(void);
u16 sub_08055728(void);
u16 sub_08042B60(void);
u16 sub_080213C0(void);
u16 sub_08021628(void);
void sub_0804325C(int a);
void sub_080617F8(void);
void sub_080611AC(void);
void sub_080617F0(void);
void sub_08061580(void);
void sub_080616D0(void);
void sub_08060160(void);
void sub_080754BC(void);
extern u16 (*const gUnk_08198F80[])(void);  /* duel phase table, 10 entries + NULL */

/* Link state at 0x02017FB0 (fields used here; u32 containers, see code_08021CC8). */
struct LinkState {
    u8 filler0[0x304];
    u32 unk304:16;
    u32 unk306_0:6;
    u32 unk306_6:1;     /* +0x306 bit 6 */
    u32 unk306_7:1;
    u32 unk307_0:1;     /* +0x307 bit 0: remote command pending */
    u32 unk307_1:7;
    union { u8 raw; struct { u32 low:5; u32 done:1; u32 high:1; u32 ack:1; } bits; } flags308;
};
extern struct LinkState gUnk_02017FB0;

struct ResolveUi { u8 pad[0x824]; int player, kind, zone; };
extern struct ResolveUi gUnk_0201CFB0;
struct Unk0201AE60 {
    u8 flags0;          /* bit 0 */
    u8 filler1[0x18 - 1];
    void (*callback)(void); /* +0x18 */
    u8 filler1C[4];
    u8 unk20;           /* +0x20 */
};
extern struct Unk0201AE60 gUnk_0201AE60;

/* The original reserves 0x100 bytes for the first link-message workspace. */
struct ResolvePacket {
    u16 count;
    u16 hasPrevious;
    struct ActEntry current;
    struct ActEntry previous;
};
struct ResolveEffect {
    u16 number;
    u16 flags;
    u32 (*resolve)(struct ActEntry *, struct ActEntry *);
    u8 rest[0x10];
};
extern struct ResolveEffect gUnk_0819A9D4[];
extern const u32 gUnk_08621DE0[];
struct ResolveZone { u32 card; u8 pad4[0x91 - 4]; u8 flags91; u8 pad92[2]; };
struct ResolveBoard { struct ResolveZone zones[23]; u8 tail[0xD64 - 23 * 0x94]; };
extern struct ResolveBoard gUnk_0201930C[2];
static inline u8 ResolveDisabled(struct ResolveZone *zone) { return zone->flags91 & 8; }
extern u32 gUnk_02017E1C;
void sub_08007558(u32 *dest, u32 *src);
u32 sub_08007590(u16 number, u16 flag);
void sub_080184D8(int player, u16 number);
void sub_080197E0(int player, u16 id);
int sub_0801A32C(void);
void sub_0801A7B4(u32 entry, u16 player);
u32 sub_0801FA48(struct ActEntry *entry);
u32 sub_0801FCA8(struct ActEntry *entry);
u16 sub_080229BC(u16 head, const void *data, int size);
void sub_08024134(int player, int kind, int zone);
void sub_08046C20(int player, int arg);
int sub_08047058(u32 card);
#define RESOLVE_LAST (gUnk_02017A40.listB[gUnk_02017A40.countB - 1])
#define RESOLVE_PREV (gUnk_02017A40.listB[gUnk_02017A40.countB - 2])
#define RESOLVE_LINK (gUnk_02017FB0.flags308.raw)
#define RESOLVE_DUEL(offset) (((u8 *)&gUnk_020192E0)[offset])
#define RESOLVE_ZONE(player, zone) ((struct ResolveZone *)((u8 *)gUnk_0201930C + ((player) * 0xD64 + (zone) * 0x94)))
#define RESOLVE_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define RESOLVE_ADVANCE() (gUnk_02017A40.resolveFlags.bits.stage++)
#define RESOLVE_END() (gUnk_02017A40.resolveFlags.bits.stage = 100)
static inline u16 ResolveSubtype(u32 stats, int type) {
    switch (type) { case 21: case 22: return (stats & 0xE0000) >> 17; default: return 0; }
}
u32 sub_08020AF4(void)
{
    u16 packet[0x80];
    struct ResolvePacket pair;
    int stage = gUnk_02017A40.resolveFlags.raw >> 1;
    switch (stage) {
    case 0:
        if (gUnk_02015EE8.link) {
            int i;
            sub_0802297C(0xF061, gUnk_02017A40.countB, 0, 0);
            for (i = 0; i < gUnk_02017A40.countB; i++) {
                packet[0] = i;
                sub_08075294(packet + 1, &gUnk_02017A40.listB[i], 0x14);
                sub_080229BC(0xF062, packet, 0x16);
            }
            sub_0802297C(0xF064, gUnk_02017A40.countB, 0, 0);
            gUnk_02017FB0.flags308.bits.ack = 0;
        }
        RESOLVE_ADVANCE();
        return 1;
    case 1:
        sub_0801A7B4((u32)gUnk_02017A40.listB, 1);
        RESOLVE_ADVANCE();
        return 1;
    case 2:
        if (sub_0801A32C())
            RESOLVE_ADVANCE();
        return 1;
    case 3:
        if (gUnk_02015EE8.link && !(RESOLVE_LINK >> 7))
            return 1;
        sub_0801EC58(0x12, 0, 0, 0);
        RESOLVE_ADVANCE();
        return 1;
    case 4: {
        u32 stats, type;
        int disabled, subtype;
        gUnk_02017A40.effectIndex = sub_08047058(RESOLVE_LAST.card);
        if (gUnk_02017A40.effectIndex < 0) {
            RESOLVE_END();
            return 1;
        }
        gUnk_02017A40.resolve = gUnk_0819A9D4[gUnk_02017A40.effectIndex].resolve;
        if (!gUnk_02017A40.resolve) {
            RESOLVE_END();
            return 1;
        }
        stats = RESOLVE_STATS(RESOLVE_LAST.card);
        type = (stats & 0x1F00000) >> 20;
        if (type > 20) {
            disabled = 0;
            if (RESOLVE_LAST.kind2 != 3)
                disabled = ResolveDisabled(RESOLVE_ZONE(RESOLVE_LAST.flag2_0, RESOLVE_LAST.val2_4)) != 0;
            subtype = ResolveSubtype(stats, type);
            if (subtype != 2) {
                if (subtype == 3 && (gUnk_020192E0.effectFlags & 3))
                    disabled = 1;
            } else if (gUnk_020192E0.effectFlags & 4)
                disabled = 1;
            stats = RESOLVE_STATS(RESOLVE_LAST.card);
            switch ((stats & 0x1F00000) >> 20) {
            case 21:
                if (gUnk_020192E0.flags1ACC.raw & 0x80)
                    disabled = 1;
                if (((stats & 0xE0000) >> 17) == 4 && (gUnk_020192E0.effectFlags & 0x10))
                    disabled = 1;
                break;
            case 22:
                if (gUnk_020192E0.flags1ACC.raw & 0x40)
                    disabled = 1;
                if (((stats & 0xE0000) >> 17) == 4 && (gUnk_020192E0.effectFlags & 8))
                    disabled = 1;
                break;
            }
            if (CARD_NUMBER_C(RESOLVE_LAST.card) == 0x603) {
                disabled = 0;
                RESOLVE_LAST.flag4_2 = 0;
            }
            if (disabled) {
                {
                    u16 msg = RESOLVE_LAST.flag2_0 ? 0x80B1 : 0xB1;
                    u16 zone = RESOLVE_LAST.val2_4;
                    sub_0801EC58(msg, zone, 1, 0);
                }
                RESOLVE_LAST.flag4_2 = 1;
            }
        } else {
            if ((sub_08007590(CARD_NUMBER_C(RESOLVE_LAST.card), 1)
                 || sub_08007590(CARD_NUMBER_C(RESOLVE_LAST.card), 0))
                && RESOLVE_LAST.flag4_2) {
                RESOLVE_END();
                return 1;
            }
        }
        {
            u32 *dest = &gUnk_02017A40.savedCard;
            u8 *base = (u8 *)gUnk_0201930C + RESOLVE_LAST.flag2_0 * 0xD64;
            sub_08007558(dest, (u32 *)(base + RESOLVE_LAST.val2_4 * 0x94));
        }
        if ((u16)sub_0801FCA8(&RESOLVE_LAST)) {
            {
                u16 msg = RESOLVE_LAST.flag2_0 ? 0x8078 : 0x78;
                u16 zone = RESOLVE_LAST.val2_4;
                sub_0801EC58(msg, zone, 0, 0);
            }
        }
        if (RESOLVE_LAST.flag4_3 && RESOLVE_LAST.flag4_2) {
            if (CARD_NUMBER_C((gUnk_02017A40.savedCard << 20) >> 20) == 0x412) {
                sub_08046C20((gUnk_02017A40.savedCard << 19) >> 31, 1);
                sub_080197E0(0, (gUnk_02017A40.savedCard << 20) >> 20);
                {
                    u16 msg = (s32)(gUnk_02017A40.savedCard << 19) < 0 ? 0x806A : 0x6A;
                    u16 cardLow = gUnk_02017A40.savedCard;
                    u16 cardHigh = gUnk_02017A40.savedCard >> 16;
                    sub_0801EC58(msg, cardLow, cardHigh, 0);
                }
            } else {
                {
                    u16 msg = RESOLVE_LAST.flag2_0 ? 0x807C : 0x7C;
                    u16 cardLow = gUnk_02017A40.savedCard;
                    u16 cardHigh = gUnk_02017A40.savedCard >> 16;
                    sub_0801EC58(msg, cardLow, cardHigh, 0);
                }
                if (CARD_NUMBER_C((gUnk_02017A40.savedCard << 20) >> 20) == 0x14D) {
                    sub_080184D8(RESOLVE_LAST.flag2_0, 0x58F);
                    sub_080184D8(1 - RESOLVE_LAST.flag2_0, 0x58F);
                }
                sub_08046C20((gUnk_02017A40.savedCard << 19) >> 31, 1);
            }
            RESOLVE_END();
            return 0;
        } else {
            gUnk_02017A40.effectStep = 0x80;
            gUnk_02017A40.effectSub = 0;
            RESOLVE_ADVANCE();
            return 1;
        }
    }
    case 5:
        if ((u16)sub_0801FA48(&RESOLVE_LAST)) {
            pair.count = gUnk_02017A40.countB;
            if (gUnk_02017A40.countB > 1) {
                pair.hasPrevious = 1;
                sub_08075294(&pair.current, &RESOLVE_LAST, 0x14);
                sub_08075294(&pair.previous, &RESOLVE_PREV, 0x14);
            } else {
                pair.hasPrevious = 0;
                sub_08075294(&pair.current, &RESOLVE_LAST, 0x14);
            }
            sub_080229BC(0xF071, &pair, 0x2C);
        }
        gUnk_02017FB0.flags308.bits.done = 0;
        RESOLVE_ADVANCE();
        return 1;
    case 6:
        if (!(u16)sub_0801FA48(&RESOLVE_LAST)) {
            if (gUnk_02017A40.countB > 1)
                gUnk_02017A40.effectStep = gUnk_02017A40.resolve(&RESOLVE_LAST, &RESOLVE_PREV);
            else
                gUnk_02017A40.effectStep = gUnk_02017A40.resolve(&RESOLVE_LAST, 0);
            if (!gUnk_02017A40.effectStep)
                gUnk_02017FB0.flags308.bits.done = 1;
        }
        if ((s32)((u32)RESOLVE_LINK << 26) < 0) {
            if ((u16)sub_0801FCA8(&RESOLVE_LAST)) {
                u32 *saved = &gUnk_02017A40.savedCard;
                u16 message = RESOLVE_LAST.flag2_0 ? 0x807C : 0x7C;
                sub_0801EC58(message, ((u16 *)saved)[0], ((u16 *)saved)[1], 0);
                sub_08046C20((gUnk_02017A40.savedCard << 19) >> 31, 1);
            }
            RESOLVE_END();
        }
        return 1;
    case 100:
        if (--gUnk_02017A40.countB) {
            gUnk_02017A40.resolveFlags.raw &= 1;
            return 1;
        }
        /* fall through */
    default:
        gUnk_02017A40.resolveFlags.bits.active = 0;
        sub_08024134(gUnk_0201CFB0.player,
                    gUnk_0201CFB0.kind,
                    gUnk_0201CFB0.zone);
        return 1;
    }
    return 1;
}


u16 sub_080213C0(void)
{
    int i, j;
    int count[2];

    if (gUnk_02017A40.unk3D0 & 1)
        return sub_08020330();
    if (gUnk_02017A40.resolveFlags.raw & 1)
        return sub_08020AF4();
    if (gUnk_02017A40.countA != 0) {
        gUnk_02017A40.countB = 0;
        for (i = 0; i < gUnk_02017A40.countA; i++) {
            sub_08075294(&gUnk_02017A40.listB[i], &gUnk_02017A40.listA[i], 0x14);
            gUnk_02017A40.countB++;
        }
        count[0] = sub_08009CAC(0, 0x4DA);
        count[1] = sub_08009CAC(1, 0x4DA);
        for (i = 0; i < gUnk_02017A40.countB; i++) {
            u16 number = CARD_NUMBER(gUnk_02017A40.listB[i].card);
            if (number == 0x4DA) {
                if (count[gUnk_02017A40.listB[i].flag2_0] > 0) {
                    count[gUnk_02017A40.listB[i].flag2_0]--;
                } else {
                    gUnk_02017A40.countB--;
                    for (j = i; j < gUnk_02017A40.countB; j++)
                        sub_08075294(&gUnk_02017A40.listB[j], &gUnk_02017A40.listB[j + 1], 0x14);
                }
            }
        }
        gUnk_02017A40.countA = 0;
        {
            /* countB is u16: the negated sign bit is its nonzero test. */
            u32 negativeCount = -(u32)gUnk_02017A40.countB;
            u8 *active = &gUnk_02017A40.unk3D0;
            *active = negativeCount >> 31;
        }
        gUnk_02017A40.unk3D1 = 0;
        return 1;
    }
    return 0;
}

u16 sub_08021580(int player)
{
    if (sub_0800A2A8(player, 0x10) && sub_0800A2A8(player, 0x11) && sub_0800A2A8(player, 0x12)
        && sub_0800A2A8(player, 0x13) && sub_0800A2A8(player, 0x14))
        return 1;
    return 0;
}
/* Does the player have all five cards 0x5F8, 0x605-0x607, 0x608 (hypothesis: the five Exodia pieces)? */
u16 sub_080215CC(int player)
{
    if (sub_08008524(player, 0x5F8) && sub_08008524(player, 0x605) && sub_08008524(player, 0x606)
        && sub_08008524(player, 0x607) && sub_08008524(player, 0x608))
        return 1;
    return 0;
}
/* Check whether the duel is over; sets the result (0x1B12 bits 6-7: 1 = player 0 wins, 2 = player 1
 * wins, 3 = draw; hypothesis) and returns 1 if so. */
u16 sub_08021628(void)
{
    struct DuelPlayer *p0;

    if ((u8)(gUnk_02015EE8.phase - 2) > 6)
        return 0;
    if (gUnk_02015EE8.link && gUnk_020192E0.linkSkip)
        return 0;
    gUnk_020192E0.result = 3;
    p0 = gUnk_020192E0.players;
    if (gUnk_020192E0.players[0].lifePoints == 0 || gUnk_020192E0.players[1].lifePoints == 0) {
        if (gUnk_020192E0.players[0].lifePoints > gUnk_020192E0.players[1].lifePoints)
            gUnk_020192E0.result = 1;
        if (gUnk_020192E0.players[0].lifePoints < gUnk_020192E0.players[1].lifePoints)
            gUnk_020192E0.result = 2;
        gUnk_020192E0.flags1ACC.bits.over = 1;
        return 1;
    }
    if (p0->deckOut || gUnk_020192E0.players[1].deckOut) {
        if (!p0->deckOut)
            gUnk_020192E0.result = 1;
        if (!gUnk_020192E0.players[1].deckOut)
            gUnk_020192E0.result = 2;
        gUnk_020192E0.flags1ACC.bits.over = 1;
        return 1;
    }
    p0->winA = sub_08021580(0);
    gUnk_020192E0.players[1].winA = sub_08021580(1);
    if (p0->winA || gUnk_020192E0.players[1].winA) {
        if (!gUnk_020192E0.players[1].winA)
            gUnk_020192E0.result = 1;
        if (!p0->winA)
            gUnk_020192E0.result = 2;
        gUnk_020192E0.flags1ACC.bits.over = 1;
        return 1;
    }
    p0->winExodia = sub_080215CC(0);
    gUnk_020192E0.players[1].winExodia = sub_080215CC(1);
    if (p0->winExodia || gUnk_020192E0.players[1].winExodia) {
        if (!gUnk_020192E0.players[1].winExodia)
            gUnk_020192E0.result = 1;
        if (!p0->winExodia)
            gUnk_020192E0.result = 2;
        gUnk_020192E0.flags1ACC.bits.over = 1;
        return 1;
    }
    return 0;
}

/* Duel phase 0 (entry 0 of the phase table 0x08198F80). */
u32 sub_08021834(void)
{
    gUnk_020192E0.linkSkip = gMain.unk4870_0;
    if ((gUnk_02015EE8.link) && gUnk_020192E0.linkSkip) {
        gUnk_020192E0.unk1B10++;
        sub_0802297C(0xF001, 0, 0, 0);
        gUnk_02015EE8.phase = 8;
        return 0;
    }
    return 1;
}
/* Duel phase 9 (last entry of the phase table 0x08198F80): duel end. */
u32 sub_080218AC(void)
{
    switch (gUnk_020192E0.phaseStep) {
    case 0:
        sub_08077BA0();
        gUnk_020192E0.unk1B13_0 = 0;
        if ((gUnk_020192E0.players[0].winA) || (gUnk_020192E0.players[1].winA)) {
            sub_0801EC58(0x13, 0, 0, 0);
            sub_0801EC58(5, 0, 0, 0);
            sub_0801EC58(0x12, 0, 0, 0);
        }
        if ((gUnk_020192E4[0].winExodia) || (gUnk_020192E4[1].winExodia)) {
            sub_0801EC58(0x13, 0, 0, 0);
            sub_0801EC58(6, 0, 0, 0);
            sub_0801EC58(0x12, 0, 0, 0);
        }
        switch (gUnk_020192E0.result) {
        case 1:
            sub_0801EC58(4, 0, 0, 0);
            break;
        case 2:
            sub_0801EC58(4, 1, 0, 0);
            break;
        case 3:
            sub_0801EC58(4, 2, 0, 0);
            break;
        }
        gUnk_020192E0.phaseStep++;
        return 0;
    case 1:
        if (gUnk_020185C0.queueCount != 0)
            return 0;
        if (!gUnk_02015EE8.link)
            break;
        sub_0802297C(0xF003, 1 - gUnk_020192E0.result, 0, 0);
        gUnk_020192E0.phaseTimer = 0;
        gUnk_020192E0.phaseStep++;
        return 0;
    case 2:
        if (++gUnk_020192E0.phaseTimer <= 0x13)
            return 0;
        break;
    }
    return 1;
}
/* DuelMainStep: the shared Campaign/Link duel step (see program-flow). */
u32 sub_08021A48(void)
{
    u16 busy;
    u16 done;

    if (gUnk_08198F80[gUnk_02015EE8.phase] != NULL) {
        busy = sub_08024440();
        if (busy == 0) {
            if (gUnk_02015EE8.link && gUnk_02017FB0.unk307_0)
                busy = sub_0801F628();
            if (busy == 0) {
                busy = sub_0801F454();
                if (busy == 0)
                    busy = sub_0802AED8();
            }
        }
        if ((((u8 *)&gUnk_0201CFB0)[0] & 6) == 6 && (gUnk_0201AE60.flags0 & 1) && gUnk_0201AE60.unk20 <= 2) {
            if (gUnk_0201AE60.callback)
                gUnk_0201AE60.callback();
            else
                sub_08060160();
        }
        if (busy == 0 && !sub_08060344() && !sub_080222F8() && !sub_08055728() && !sub_080213C0()) {
            sub_0804325C(1);
            if (!sub_08042B60()) {
                switch (gUnk_02015EE8.phase) {
                case 3:
                case 4:
                case 5:
                case 6:
                    if (gUnk_020192E0.unk1B14 & 2) {
                        sub_080617F8();
                        goto end;
                    }
                    if (gUnk_02017FB0.unk306_6) {
                        int ok = 1;
                        if (gUnk_020192E0.sel.flag0) {
                            if (gUnk_020192E0.sel.state == 2) {
                                gUnk_020192E0.sel.flag0 = 0;
                                gUnk_020192E0.sel.state = 0;
                            } else {
                                ok = 0;
                            }
                        }
                        if (gUnk_020192E0.sel.active)
                            ok = 0;
                        if (ok) {
                            sub_0801EC58(0x8041, 1, 0, 0);
                            sub_0802297C(0xF005, 0, 0, 0);
                            gUnk_020192E0.unk1B14 |= 2;
                        }
                        sub_080611AC();
                        sub_080617F0();
                    }
                }
                done = gUnk_08198F80[gUnk_02015EE8.phase]();
                if (sub_08021628()) {
                    gUnk_02015EE8.phase = 9;
                    gUnk_020192E0.phaseStep = 0;
                    gUnk_020192E0.phaseTimer = 0;
                } else if (done) {
                    gUnk_02015EE8.phase++;
                    gUnk_020192E0.phaseStep = 0;
                    gUnk_020192E0.phaseTimer = 0;
                }
            }
        }
    end:
        sub_08061580();
        sub_080616D0();
        return 0;
    }
    gMain.result = gUnk_020192E0.result;
    sub_080754BC();
    return 1;
}

