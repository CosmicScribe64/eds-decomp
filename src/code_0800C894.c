#include "global.h"

/* Output of sub_0800ABC8 (card-in-zone info). */
struct ZoneCardInfo {
    u32 unk0;
    u32 unk4;
    u32 unk8;
};

/* Duel command block at 0x020185C0 (see code_0800D8A4 / code_0800EAA8). */
struct DuelCmd {
    u16 hdr;          /* +0x00: bits 0-11 command id, bit 15 acting player */
    u16 arg1;         /* +0x02 */
    u16 arg2;         /* +0x04 */
    u16 arg3;         /* +0x06 */
    u8 filler8[0x80A - 0x8];
    u8 step : 7;      /* +0x80A bits 0-6: multi-frame handler state */
    u8 unk80A_7 : 1;
    u8 unk80B;
    u32 unk80C_0 : 5;
    u32 timer : 7;    /* +0x80C bits 5-11: frame counter inside a step */
    u32 unk80C_12 : 1;
    u32 running : 1;  /* +0x80D bit 5: command in progress */
    u32 unk80C_14 : 2;
    u32 unk80C_16 : 16;
};

/* Per-player duel state, 0xD64 bytes; two of them at 0x020192E4. */
struct DuelPlayer {
    u8 filler0[8];
    u8 flag8_0 : 1;   /* +0x08 bit 0 */
    u8 flag8_1 : 1;   /* +0x08 bit 1 */
    u8 unk8_2 : 6;
    u8 filler9[0x24 - 0x9];
    u16 unk24;        /* +0x24 */
    u8 filler26[0xD64 - 0x26];
};

/*
 * First word of a field zone. Zones are 0x94 bytes, at 0x0201930C + player*0xD64 + slot*0x94.
 * Reads of the card id use a full-word ldr, so the code must see a 4-byte struct here
 * (a larger struct makes agbcc narrow the read to ldrh).
 */
struct ZoneWord {
    u32 cardId : 12;  /* bits 0-11 (0 = empty) */
    u32 unk0_12 : 20;
};

/* Card reference passed to sub_0802B558 (0x14 bytes on the stack; layout beyond +2 unknown). */
struct CardRef {
    u16 id;           /* +0x00 card id */
    u8 player : 1;    /* +0x02 bit 0 */
    u8 unk2_1 : 7;
    u8 filler3[0x14 - 0x3];
};

/* Duel-wide phase word at 0x020192E0+0x1B14 (== 0x020192E4+0x1B10). */
struct DuelPhaseWord {
    u32 unk0 : 9;
    u32 phase : 8;    /* bits 9-16 */
    u32 unk17 : 8;    /* bits 17-24 */
    u32 unk25 : 7;
};

/* 0x020192E0: global duel state (hypothesis: players[] live at +4). */
struct DuelGlobal {
    u8 filler0[0x1B12];
    u8 flags1B12;     /* bit 1: ?, bit 5: link error */
    u8 unk1B13;
    struct DuelPhaseWord phaseWord; /* +0x1B14 */
};

/* The same memory seen from 0x020192E4. */
struct DuelPlayers {
    struct DuelPlayer p[2];
    u8 filler1AC8[0x1B10 - 0x1AC8];
    struct DuelPhaseWord phaseWord; /* +0x1B10 */
};

/* 0x02015EE8: byte 1 bit 0 = link (two-GBA) duel (hypothesis). */
struct Unk02015EE8 {
    u8 unk0;
    u8 flags1;
};

struct Unk02018450 {
    u16 unk0_0 : 5;
    u16 unk0_5 : 1;   /* bit 5 */
    u16 unk0_6 : 3;   /* bits 6-8 */
    u16 unk0_9 : 3;   /* bits 9-11 */
    u16 unk0_12 : 4;
};

extern struct DuelCmd gUnk_020185C0;
extern struct Unk02018450 gUnk_02018450;
extern struct DuelGlobal gUnk_020192E0;
extern struct DuelPlayers gUnk_020192E4;
extern struct Unk02015EE8 gUnk_02015EE8;
extern u8 gUnk_0201930C[];
extern u16 gUnk_08622AB4[];

#define gCmd gUnk_020185C0
#define CMD_PLAYER() (gCmd.hdr >> 15)
#define CMD_DONE() (gCmd.running = 0)
#define ZONE(p, s) ((struct ZoneWord *)(gUnk_0201930C + (s) * 0x94 + (p) * 0xD64))

/* True unless this is a link duel whose flag 0x1B12 bit 1 is set (hypothesis: "not the slave side"). */
#define LINK_SKIP() ((gUnk_02015EE8.flags1 & 1) && (gUnk_020192E0.flags1B12 & 2))

void sub_0800ABC8(u32 player, u32 slot, struct ZoneCardInfo *out);
void sub_0804A39C(u32, u32);
u32 sub_08060B4C(void);
void sub_0805E3B8(u32, u32);
u32 sub_0805E788(u32, u32, u32);
void sub_080096F4(void *);
void sub_08009768(void *);
void sub_080611AC(void);
u16 sub_0802B558(struct CardRef *card, u16 pos);
void sub_08007A4C(u32 player, u32 slot, void *data, u16 a3, u16 a4);
s32 sub_0800AA40(s32, s32, u16);
s32 sub_08008524(s32, s32);
extern u32 gUnk_08621DE0[];
extern u8 gUnk_0201ADAD;
void sub_08024134(u32, s32, u16);
void sub_080752B0(u32, void *, u32);
void sub_08076714(u32, u32, u32, u32);
/* Duel screen / animation state at 0x0201CFB0 (fields used here). */
struct DuelScreen {
    u8 fast : 1;        /* +0x000 bit 0: fast-forward animations */
    u8 unk0_1 : 7;
    u8 filler1[0x808 - 1];
    u8 unk808_0 : 3;
    u8 busy : 1;        /* +0x808 bit 3 */
    u8 unk808_4 : 4;
};
extern struct DuelScreen gUnk_0201CFB0;
extern u16 gUnk_081A43E4[];
extern u8 gUnk_08687B9C[];
extern u8 gUnk_086883BC[];
extern u16 gUnk_03000040[];
extern u8 gUnk_020195F0[];

u32 sub_0800C894(u32 player, u32 slot)
{
    struct ZoneCardInfo info;
    sub_0800ABC8(player, slot, &info);
    return info.unk4;
}

u32 sub_0800C8A8(u32 player, u32 slot)
{
    struct ZoneCardInfo info;
    sub_0800ABC8(player, slot, &info);
    return info.unk8;
}

#if 0 /* NONMATCHING (score 24): Rewritten from asm. Keys: u16 best (re-read ldrh, bls); loop1 zone via
       * ZONE(player,i) so CSE path-following turns (p&1)*0xD64 into a copy of the entry off; loop3 zone written
       * (p&1)*0xD64 + s*0x94 (fold swaps, so 0x94/slot chain hoists before 0xD64 and the threshold runs out); u8
       * *flags=&gUnk_0201ADAD before loop3; staged zones/so locals in entry; separate z locals per loop. Remaining:
       * loop3 guard/body reload regs (slot*0x94 r1 vs r4, base r4 vs r1). */
struct C8BCZone {
    u32 card;               /* +0x00: bits 0-11 card id, bit 17 tested */
    u16 unk4;               /* +0x04 */
    u8 flags6;              /* +0x06: bit 1 face up */
    u8 filler7[3];
    u16 links[32];          /* +0x0A */
    u16 linkKinds[32];      /* +0x4A */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[4];
    u32 unk90;              /* +0x90 */
};
struct C8BCZoneB {
    u8 filler0[0xA];
    u8 linkBytes[64];       /* +0x0A */
};
#define C8BC_ZONE(p, s) ((struct C8BCZone *)(gUnk_0201930C + ((s) * 0x94 + ((p) & 1) * 0xD64)))
#define C8BC_ZONE2(p, s) ((struct C8BCZone *)(gUnk_0201930C + (((p) & 1) * 0xD64 + (s) * 0x94)))
#define C8BC_ZONEB2(p, s) ((struct C8BCZoneB *)(gUnk_0201930C + (((p) & 1) * 0xD64 + (s) * 0x94)))
#define C8BC_ZONEB(p, s) ((struct C8BCZoneB *)(gUnk_0201930C + ((s) * 0x94 + ((p) & 1) * 0xD64)))
#define C8BC_LINKED(p, s) ((struct C8BCZone *)(gUnk_0201930C + ((s) * 0x94 + (p) * 0xD64)))
#define C8BC_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define C8BC_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
u32 sub_0800C8BC(s32 player, s32 slot)
{
    u32 off;
    u32 id;
    u16 best;
    u32 result;
    int i, j;
    struct C8BCZone *z;
    u8 *zones;
    u32 so;

    off = (player & 1) * 0xD64;
    zones = gUnk_0201930C;
    so = slot * 0x94;
    best = 0;
    z = (struct C8BCZone *)(zones + (so + off));
    id = (z->card << 20) >> 20;
    if (id == 0)
        return 0;
    result = (C8BC_STATS(id) & 0x1F00000) >> 20;
    if (slot > 4 || !(z->flags6 & 2))
        return result;
    for (i = 0; i <= 4; i++) {
        u32 w;
        u32 cid;
        struct C8BCZone *z;
        z = C8BC_ZONE(player, i);
        w = z->card;
        cid = (w << 20) >> 20;
        if (cid != 0 && C8BC_NUMBER(cid) == 0x2FA && (s32)(w << 14) < 0
            && (z->flags6 & 2) && z->unk4 > best) {
            best = z->unk4;
            result = 10;
        }
        for (j = 0; j <= 1; j++) {
            z = (struct C8BCZone *)(gUnk_020195F0 + ((j & 1) * 0xD64 + i * 0x94));
            cid = (z->card << 20) >> 20;
            if (cid != 0 && C8BC_NUMBER(cid) == 0x479 && (z->flags6 & 2)
                && !(((u8 *)z)[0x91] & 8) && z->unk4 > best) {
                best = z->unk4;
                result = (z->unk90 << 14) >> 27;
            }
        }
    }
    {
    u8 *flags = &gUnk_0201ADAD;
    for (i = 0; i < C8BC_ZONE2(player, slot)->numLinks; i++) {
        u16 link;
        u8 kind;
        int lz, lp;
        struct C8BCZone *t;
        u16 tid;

        link = C8BC_ZONE2(player, slot)->links[i];
        kind = C8BC_ZONE2(player, slot)->linkKinds[i];
        lz = link >> 8;
        lp = C8BC_ZONEB2(player, slot)->linkBytes[i * 2] & 1;
        t = C8BC_LINKED(lp, lz);
        tid = (t->card << 20) >> 20;
        if (kind == 1 && tid != 0
            && !(((u8 *)t)[0x91] & 8)
            && sub_08008524(0, 0x601) == 0
            && sub_08008524(1, 0x601) == 0
            && !(*flags & 3)
            && C8BC_NUMBER(tid) == 0x60E
            && t->unk4 > best)
            result = 1;
    }
    }
    return result;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800C894", sub_0800C8BC); /* 0x0800C8BC size 0x234 */
/* Field zone (0x94 bytes) as read by sub_0800CAF0. */
struct CAF0Zone {
    u32 card;               /* +0x00: bits 0-11 card id */
    u8 filler4[2];
    u8 flags6;              /* +0x06: bit 1 face up */
    u8 filler7[3];
    u16 links[32];          /* +0x0A: (zone << 8) | player of a linked card */
    u16 linkKinds[32];      /* +0x4A: low byte = link kind */
    u16 numLinks;           /* +0x8A */
    u8 filler8C[4];
    u32 unk90;              /* +0x90: bits 13-17 replacement state; byte +0x91 bit 3 tested */
};
/* Byte view of links[], so the player byte is a separate ldrb from the same address as the ldrh. */
struct CAF0ZoneB {
    u8 filler0[0xA];
    u8 linkBytes[64];       /* +0x0A */
};
#define CAF0_ZONE(p, s) ((struct CAF0Zone *)(gUnk_0201930C + ((s) * 0x94 + ((p) & 1) * 0xD64)))
#define CAF0_ZONEB(p, s) ((struct CAF0ZoneB *)(gUnk_0201930C + ((s) * 0x94 + ((p) & 1) * 0xD64)))
/* Linked zone; the caller passes the already-masked player bit. */
#define CAF0_LINKED(p, s) ((struct CAF0Zone *)(gUnk_0201930C + ((s) * 0x94 + (p) * 0xD64)))
#define CAF0_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CAF0_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
/*
 * Card stats bits 29-31 of the card in (player, slot), or 0 for an empty zone. For a face-up
 * monster (slot 0-4), a kind-1 link to a card with number 0x5A8 (not flagged at +0x91 bit 3,
 * no 0x601 on either side, gUnk_0201ADAD bits 0-1 clear) replaces it with that zone's +0x90
 * bits 13-17.
 */
u32 sub_0800CAF0(s32 player, s32 slot)
{
    u32 id;
    u32 result;
    int i;
    u32 off;
    u8 *zones;

    /* The player offset and the zone base are staged before the slot offset (ROM order). */
    off = (player & 1) * 0xD64;
    zones = gUnk_0201930C;
    id = (((struct CAF0Zone *)(zones + (slot * 0x94 + off)))->card << 20) >> 20;
    if (id == 0)
        return 0;
    result = CAF0_STATS(id) >> 29;
    if (slot > 4 || !(CAF0_ZONE(player, slot)->flags6 & 2))
        return result;
    {
        /* Pointer set outside the loop: reloaded before its ldrb, after the #3 mask. */
        u8 *flags = &gUnk_0201ADAD;

        for (i = 0; i < CAF0_ZONE(player, slot)->numLinks; i++) {
            u16 link;
            u8 kind;
            int lz, lp;
            struct CAF0Zone *t;
            u16 tid;

            /* FAKEMATCH: an extra use of the zone base the loop hoists, so global alloc keeps it
             * in r9 and spills the hoisted linkKinds pointer to [sp] instead. */
            asm("" :: "r"(gUnk_0201930C));
            link = CAF0_ZONE(player, slot)->links[i];
            kind = CAF0_ZONE(player, slot)->linkKinds[i];
            lz = link >> 8;
            lp = CAF0_ZONEB(player, slot)->linkBytes[i * 2] & 1;
            t = CAF0_LINKED(lp, lz);
            tid = (t->card << 20) >> 20;
            if (kind == 1 && tid != 0
                && !(((u8 *)t)[0x91] & 8)
                && sub_08008524(0, 0x601) == 0
                && sub_08008524(1, 0x601) == 0
                && !(*flags & 3)
                && CAF0_NUMBER(tid) == 0x5A8)
                result = (t->unk90 << 14) >> 27;
        }
    }
    return result;
}
s32 sub_0800CC18(s32 player, s32 slot)
{
    u16 *tab = gUnk_08622AB4;
    u16 cardId;
    s32 p, s;
    cardId = *(u32 *)&gUnk_0201930C[(player & 1) * 0xD64 + slot * 0x94] << 20 >> 20;
    if (cardId == 0) {
        return 0;
    }
    for (p = 0; p <= 1; p++) {
        for (s = 0; s <= 4; s++) {
            u8 *zone = &gUnk_0201930C[s * 0x94 + (p & 1) * 0xD64];
            if ((*(u32 *)zone << 20) == 0) {
                continue;
            }
            if (!(zone[6] & 2)) {
                continue;
            }
            if (sub_0800AA40(p, s, tab[cardId & 0x7FF]) != -1) {
                return 1;
            }
        }
    }
    return 0;
}
/* Evaluates the card in (player, slot) against zone pos (targetPlayer, targetSlot) via sub_0802B558. */
u16 sub_0800CCCC(u32 player, u32 slot, u32 targetPlayer, u32 targetSlot)
{
    struct CardRef ref;
    ref.id = ZONE(player & 1, slot)->cardId;
    ref.player = player;
    return sub_0802B558(&ref, (u8)targetPlayer | ((u8)targetSlot << 8));
}
/* Counts the zones (both players, slots 0-4) for which sub_0800CCCC returns nonzero. */
int sub_0800CD24(u32 player, u32 slot)
{
    int count = 0;
    int p, s;
    for (p = 0; p <= 1; p++)
        for (s = 0; s <= 4; s++)
            if (sub_0800CCCC(player, slot, p, s))
                count++;
    return count;
}
u16 sub_0800CD68(s32 player, s32 slot)
{
    u16 *tab = gUnk_08622AB4;
    u16 cardId;
    s32 p, s;
    cardId = *(u32 *)&gUnk_0201930C[(player & 1) * 0xD64 + slot * 0x94] << 20 >> 20;
    if (cardId == 0) {
        return 0;
    }
    for (p = 0; p <= 1; p++) {
        for (s = 0; s <= 4; s++) {
            u8 *zone = &gUnk_0201930C[s * 0x94 + (p & 1) * 0xD64];
            if ((*(u32 *)zone << 20) == 0) {
                continue;
            }
            if (!(zone[6] & 2)) {
                continue;
            }
            if (sub_0800AA40(p, s, tab[cardId & 0x7FF]) != -1) {
                return (u8)p | ((u8)s << 8);
            }
        }
    }
    return 0xFFFF;
}
s32 sub_080623AC(u32, s32, u16);
s32 sub_080623EC(u32, s32, u16);
extern u8 gUnk_08687FBC[];

void sub_0800CE28(void)
{
    u32 player = gCmd.hdr >> 15;
    u16 slot = gCmd.arg1;
    u16 otherSlot = gCmd.arg2;
    s32 step = gCmd.step;

    switch (step) {
    case 0:
        gUnk_0201CFB0.busy = 1;
        sub_08024134(player, 0, slot);
        gCmd.step++;
        break;
    case 1:
        gUnk_0201CFB0.busy = 1;
        sub_08024134(1 - player, 0, otherSlot);
        gCmd.step++;
        break;
    case 2:
        gUnk_0201CFB0.busy = 0;
        gCmd.timer = 0;
        gCmd.step++;
        break;
    case 3: {
        s32 x = sub_080623AC(player, 0, slot);
        s32 y = sub_080623EC(player, 0, slot);
        u32 packed = (u32)(x + 8) | ((u32)(y + 8) << 16);
        s32 t;

        sub_08076714(packed, 0x40, 0x5200, (gCmd.timer * 4 + (player ? 0x40 : 0)) | 0x1000000);
        gCmd.timer++;
        t = gCmd.timer;
        if (t <= 31) {
            if ((gUnk_03000040[2] & 2) || gUnk_0201CFB0.fast)
                if (t <= 23)
                    gCmd.timer += 7;
        } else {
            gCmd.timer = 0;
            gCmd.step++;
        }
        break;
    }
    case 4: {
        u32 other = 1 - player;
        s32 dx = sub_080623AC(other, 0, otherSlot);
        s32 dy = sub_080623EC(other, 0, otherSlot);
        s32 t;

        dx -= sub_080623AC(player, 0, slot);
        dy -= sub_080623EC(player, 0, slot);
        dx *= gCmd.timer;
        dy *= gCmd.timer;
        dx /= 32;
        dy /= 32;
        dx += sub_080623AC(player, 0, slot) + 8;
        dy += sub_080623EC(player, 0, slot) + 8;
        dy = (dy << 16) | dx;
        sub_08076714(dy, 0x40, 0x5200, ((u32)gUnk_081A43E4[gCmd.timer] << 16) | (player ? 0x40 : 0));
        gCmd.timer++;
        t = gCmd.timer;
        if (t <= 31) {
            if ((gUnk_03000040[2] & 2) || gUnk_0201CFB0.fast)
                if (t <= 23)
                    gCmd.timer += 7;
        } else {
            gCmd.timer = 0;
            gCmd.step++;
        }
        break;
    }
    case 5:
        sub_080752B0(0x050003E0, gUnk_08687B9C, 0x20);
        sub_080752B0(0x06016C80, gUnk_08687FBC, 0x400);
        gCmd.timer = 0;
        gCmd.step++;
        break;
    case 6: {
        s32 t = gCmd.timer;
        if (t <= 95) {
            sub_08076714(0x300058, 0x40C0, 0xF364, (u32)gUnk_081A43E4[t & 31] << 16);
            gCmd.timer++;
            if ((gUnk_03000040[2] & 2) || gUnk_0201CFB0.fast)
                if (gCmd.timer <= 87)
                    gCmd.timer += 3;
            break;
        }
    }
    default:
        CMD_DONE();
        break;
    }
}
void sub_0800D234(void)
{
    u32 player = CMD_PLAYER();
    u16 arg = gUnk_020185C0.arg1;
    s32 step = gUnk_020185C0.step;

    switch (step) {
    case 0:
        gUnk_0201CFB0.busy = 1;
        sub_08024134(player, 0, arg);
        gUnk_020185C0.step++;
        break;
    case 1:
        gUnk_0201CFB0.busy = 0;
        sub_080752B0(0x050003E0, gUnk_08687B9C, 0x20);
        sub_080752B0(0x06016C80, gUnk_086883BC, 0x400);
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 2: {
        s32 t = gUnk_020185C0.timer;
        if (t <= 0x5F) {
            sub_08076714(0x00300058, 0x40C0, 0xF364, gUnk_081A43E4[t & 0x1F] << 16);
            gUnk_020185C0.timer++;
            if ((step & gUnk_03000040[2]) || gUnk_0201CFB0.fast) {
                if (gUnk_020185C0.timer <= 0x57) {
                    gUnk_020185C0.timer += 7;
                }
            }
            return;
        }
    }
    /* fallthrough */
    default:
        CMD_DONE();
        return;
    }
}
/* Zone byte +0x8C flags. */
struct ZoneFlags8C {
    u8 b0 : 1;
    u8 b1 : 1;        /* bit 1: cleared every call */
    u8 b2 : 1;
    u8 b3 : 1;        /* bit 3: moved to bit 4 */
    u8 b4 : 1;
    u8 rest : 3;
};

void sub_0800D398(void)
{
    struct DuelCmd *t = &gUnk_020185C0;
    s32 i = 0;
    struct DuelCmd *cmd;
    u32 off;
    u8 *gBase;
    u8 *pb;

    /* FAKEMATCH: the ROM loads &gCmd before i = 0 and copies it into the long-lived cmd register
     * afterwards; a memory clobber between the two stops combine from merging the load into the copy. */
    asm volatile("" ::: "memory");
    cmd = t;
    off = (cmd->hdr >> 15) * 0xD64;
    gBase = gUnk_0201930C;
    pb = gBase + off;

    do {
        u32 zOff = i * 0x94;
        struct ZoneFlags8C *f = (struct ZoneFlags8C *)(pb + zOff + 0x8C);
        u8 *z;

        if (f->b3) {
            f->b3 = 0;
            f->b4 = 1;
        }
        f->b1 = 0;
        /* Integer sum keeps the ROM's (zOff + off) + base operand order. */
        z = (u8 *)(zOff + off + (u32)gBase);
        /* A one-case switch keeps the 0x538 constant inside the loop (an == compare gets hoisted). */
        switch (((u16 *)0x08622AB4)[((struct ZoneWord *)z)->cardId & 0x7FF]) {
        case 0x538:
            z[7] |= 0x20;
        }
        i++;
    } while (i <= 4);
    cmd->running = 0;
}


void sub_0800D43C(void)
{
    sub_0804A39C(CMD_PLAYER(), gUnk_020185C0.arg1);
    CMD_DONE();
}

void sub_0800D468(void)
{
    if (sub_08060B4C()) {
        sub_0805E3B8(gUnk_020185C0.arg1, gUnk_020185C0.arg2);
        CMD_DONE();
    }
}

void sub_0800D498(void)
{
    if (sub_0805E788(gUnk_020185C0.arg1, gUnk_020185C0.arg2, gUnk_020185C0.arg3))
        CMD_DONE();
}

void sub_0800D4C8(void)
{
    u8 player = CMD_PLAYER();
    if (gCmd.arg1)
        gUnk_020192E4.p[player].flag8_0 = 1;
    if (gCmd.arg2)
        gUnk_020192E4.p[player].flag8_1 = 1;
    CMD_DONE();
}
void sub_0800D524(void)
{
    if (!LINK_SKIP()) {
        gUnk_020192E4.p[0].unk24 = 0;
        gUnk_020192E4.p[1].unk24 = 0;
        gUnk_020192E4.phaseWord.phase = 12;
    }
    CMD_DONE();
}
void sub_0800D594(void)
{
    if (!LINK_SKIP()) {
        gUnk_02018450.unk0_9 = (u8)(gCmd.arg1 >> 8);
        if (gCmd.arg2) {
            gUnk_020192E0.phaseWord.phase = 6;
            gUnk_020192E0.phaseWord.unk17 = 0;
        }
    }
    CMD_DONE();
}
void sub_0800D634(void)
{
    if (!LINK_SKIP()) {
        gUnk_02018450.unk0_6 = gCmd.arg1 >> 8;
        if (gCmd.arg2) {
            gUnk_020192E0.phaseWord.phase = 6;
            gUnk_020192E0.phaseWord.unk17 = 0;
        }
    }
    CMD_DONE();
}

void sub_0800D6D4(void)
{
    gUnk_02018450.unk0_5 = 1;
    CMD_DONE();
}

void sub_0800D6FC(void)
{
    u32 player = CMD_PLAYER();
    u16 arg = gCmd.arg1;
    if (!LINK_SKIP()) {
        gUnk_020192E0.phaseWord.phase = 11;
        gUnk_020192E0.phaseWord.unk17 = 0;
    }
    sub_0804A39C(player, arg);
    CMD_DONE();
}
void sub_0800D784(void)
{
    u32 player = CMD_PLAYER();
    u8 slot = gCmd.arg1;
    u8 flags = gCmd.arg1 >> 8;
    u16 flag0 = flags & 1;
    u16 flag1 = (u8)(flags & 2) >> 1;
    u32 data = (gCmd.arg3 << 16) | gCmd.arg2;
    sub_08007A4C(player, slot, &data, flag1, flag0);
    sub_080611AC();
    CMD_DONE();
}
void sub_0800D7D4(void)
{
    ZONE(CMD_PLAYER(), gCmd.arg1)->cardId = 0;
    sub_080611AC();
    CMD_DONE();
}

void sub_0800D824(void)
{
    u32 w = (gUnk_020185C0.arg2 << 16) | gUnk_020185C0.arg1;
    if (w << 20)
        sub_080096F4(&w);
    sub_080611AC();
    CMD_DONE();
}

void sub_0800D864(void)
{
    u32 w = (gUnk_020185C0.arg2 << 16) | gUnk_020185C0.arg1;
    if (w << 20)
        sub_08009768(&w);
    sub_080611AC();
    CMD_DONE();
}
