#include "global.h"
#include "duel.h"

/*
 * Duel script-command handlers (commands 0x79-0x84, dispatched by sub_0801ECA8).
 * Each one runs as a small multi-frame state machine on gUnk_020185C0.step and
 * clears gUnk_020185C0.running when finished. See wiki/functions/code-0800d8a4.md.
 */

/* Command block at 0x020185C0 (hypothesis: current duel command). */
struct DuelCmd {
    u16 hdr;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg1;           /* 0x002: usually a zone index */
    u16 arg2;           /* 0x004 */
    u16 arg3;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u8 step:7;          /* 0x80A: handler state */
    u8 unk80A_7:1;
    u8 unk80B;
    u8 unk80C;
    u8 unk80D_0:5;
    u8 running:1;       /* 0x80D bit 5: command in progress */
    u8 unk80D_6:2;
    u8 unk80E[6];
    struct DuelCard saved814;   /* 0x814: card word saved from a zone */
};

/* Card location passed to the card-move animations (sub_08024380 / sub_080242C4). */
struct CardLoc {
    u16 player:1;
    u16 area:4;
    u16 slot:9;
    u16 flag14:1;
    u16 flag15:1;
    u16 unk2;
};

extern struct DuelCmd gUnk_020185C0;
#define gCmd gUnk_020185C0
#define CMD_PLAYER() (gCmd.hdr >> 15)
/* Card ID of a zone, read as a whole word (agbcc: pointer deref, not a member access). */
#define ZONE_CARD_ID(zone) (((struct DuelCard *)(zone))->id)
/* Zone pointer with the zone term first (the ROM's address order in the later handlers). */
#define ZB(p, z) ((struct DuelZone *)((z) * 0x94 + (p) * 0xD64 + (u32)gUnk_0201930C))
#define ZONE(p, s) ((struct DuelZone *)((u8 *)gUnk_0201930C + (p) * 0xD64 + (s) * 0x94))

u32 sub_08062354(u32 slot);
void sub_080240A8(u32 player, u32 a);
void sub_08024288(u32 a, u32 b);
void sub_08024134(u32 player, u32 a, u32 slot);
void sub_080611AC(void);
void sub_08077AEC(u16 se);                          /* PlaySE */
void sub_08024380(struct CardLoc *loc, const void *anim, u32 a, u32 b);
void sub_080242C4(u32 id, struct CardLoc *from, struct CardLoc *to);
void sub_08060FD0(u32 player, u32 slot);
void sub_08007558(struct DuelCard *dst, struct DuelCard *src);
void sub_08008E44(u32 player, u32 slot);
void sub_0800747C(u32 player, u32 slot);
void sub_08008F14(u32 player, u32 slot);
u32 sub_08007994(u16 id);
void sub_08008E80(u32 player, u32 slot);
void sub_08008EB4(u32 player, u32 slot);
void sub_0802408C(u32 bg);
void sub_0802432C(struct CardLoc *from, struct CardLoc *to);
void sub_08075278(void *dst, u32 size);
void sub_08075294(void *dst, const void *src, u32 size);
extern const u8 gUnk_0868CAC0[];
extern const u16 gUnk_08622AB4[];
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

/* Command 0x7E: flip the face-down flag of zone arg1 (with animation). */
void sub_0800D8A4(void)
{
    u32 player = CMD_PLAYER();
    register u32 slot asm("r5") = gCmd.arg1;
    u32 arg = gCmd.arg2;
    struct DuelZonesPlayer *zones = &gUnk_0201930C[player];
    struct DuelZone *zone = &zones->zones[slot];

    if (ZONE_CARD_ID(zone) == 0) {
        gCmd.running = 0;
        return;
    }
    switch (gCmd.step) {
    case 0:
        sub_080240A8(player, sub_08062354(slot));
        gCmd.step++;
        break;
    case 1: {
        u32 a = (u8)slot << 8 | player;
        u32 fd = zone->flag6_0;
        u32 b = arg << 24;
        sub_08024288(1, a | (fd << 16 | b));
        gCmd.step++;
        break;
    }
    default:
        zone->flag6_0 = !zone->flag6_0;
        if (arg && !zone->flag6_1)
            zone->flag6_1 = 1;
        sub_080611AC();
        sub_08024134(player, 0, slot);
        gCmd.running = 0;
        break;
    }
}
/* Command 0x7F: toggle the flag6_1 flag of zone arg1 (stamping a serial when set). */
void sub_0800D990(void)
{
    u32 player = CMD_PLAYER();
    register u32 slot asm("r6") = gCmd.arg1;
    register struct DuelZonesPlayer *zones asm("r1") = &gUnk_0201930C[player];
    u32 off = 0x94 * slot;
    struct DuelZone *zone = (struct DuelZone *)((u8 *)zones + off);

    off += player * 0xD64;
    if (ZONE_CARD_ID((struct DuelZone *)((u8 *)gUnk_0201930C + off)) == 0) {
        gCmd.running = 0;
        return;
    }
    switch (gCmd.step) {
    case 0:
        sub_080240A8(player, sub_08062354(slot));
        gCmd.step++;
        break;
    case 1:
        sub_08024288(2, ((u8)slot << 8 | player) | ((zone->flag6_0 | zone->flag6_1 << 8) << 16));
        gCmd.step++;
        break;
    default:
        zone->flag6_1 = !zone->flag6_1;
        if (zone->flag6_1)
            zone->serial = (*(u16 *)((u8 *)gUnk_0201930C - 0x2C))++;
        sub_08024134(player, 0, slot);
        sub_080611AC();
        gCmd.running = 0;
        break;
    }
}
/* Byte view for card-word bit 18, kept separate from the shared DuelCard layout. */
struct ZoneFlags18 {
    u8 pad[2];
    u8 lo:2;
    u8 flag18:1;
    u8 hi:5;
};

void sub_0800DA84(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;

    switch (cmd->step) {
    case 0:
        if (((struct DuelCard *)(player * 0xD64 + slot * 0x94 + (u32)gUnk_0201930C))->id == 0) {
            sub_080611AC();
            cmd->running = 0;
        }
        sub_080240A8(player, sub_08062354(slot));
        cmd->step++;
        break;
    case 1:
        if (((u8 *)cmd)[4]) {
            struct DuelZone *zone;
            u32 pp;

            sub_08077AEC(8);
            from.player = player;
            from.area = 0;
            from.slot = slot;
            pp = player & 1;
            zone = (struct DuelZone *)(slot * 0x94 + pp * 0xD64 + (u32)gUnk_0201930C);
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            sub_08024380(&from, gUnk_0868CAC0, 0, 0);
        }
        sub_08060FD0(player, slot);
        cmd->step++;
        break;
    case 2: {
        struct DuelCard *saved = &cmd->saved814;
        u32 poff = (player & 1) * 0xD64;
        u8 *base = (u8 *)gUnk_0201930C;
        u8 *zonebase = base + poff;
        u32 zoff = slot * 0x94;
        struct DuelZone *zone;
        u32 word, id;

        sub_08007558(saved, (struct DuelCard *)(zonebase + zoff));
        zone = (struct DuelZone *)(zoff + poff + (u32)base);
        ((struct ZoneFlags18 *)zone)->flag18 = 0;
        sub_08008E44(player, slot);
        word = *(u32 *)saved;
        id = (word << 20) >> 20;
        if ((u16)(CARD_NUMBER(id) - 0x780) > 0x4F) {
            from.player = player;
            from.area = 0;
            from.slot = slot;
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            to.player = (word << 19) >> 31;
            to.area = 14;
            to.slot = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(id, &from, &to);
        }
        cmd->step++;
        break;
    }
    default:
        sub_080611AC();
        sub_08024134(player, 0, slot);
        gCmd.running = 0;
        break;
    }
}

/* Reusing `from` across the steps keeps the byte stores after its address escapes. */
void sub_0800DD04(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;

    switch (cmd->step) {
    case 0:
        if (((struct DuelCard *)(player * 0xD64 + slot * 0x94 + (u32)gUnk_0201930C))->id == 0)
            cmd->running = 0;
        else {
            sub_080240A8(player, sub_08062354(slot));
            cmd->step++;
        }
        break;
    case 1: {
        struct DuelZone *zone;
        u32 pp;

        sub_08077AEC(0x11);
        from.player = player;
        from.area = 0;
        from.slot = slot;
        pp = player & 1;
        zone = (struct DuelZone *)(slot * 0x94 + pp * 0xD64 + (u32)gUnk_0201930C);
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        sub_08024380(&from, gUnk_0868CAC0, 0, 0);
        sub_08060FD0(player, slot);
        cmd->step++;
        break;
    }
    case 2: {
        struct DuelCard *saved = &cmd->saved814;
        u32 poff = (player & 1) * 0xD64;
        u8 *zonebase = (u8 *)gUnk_0201930C + poff;
        u32 zoff = slot * 0x94;
        u32 word, id;

        sub_08007558(saved, (struct DuelCard *)(zonebase + zoff));
        sub_08008E80(player, slot);
        word = *(u32 *)saved;
        id = (word << 20) >> 20;
        if ((u16)(CARD_NUMBER(id) - 0x780) > 0x4F) {
            struct DuelZone *zone;

            from.player = player;
            from.area = 0;
            from.slot = slot;
            zone = (struct DuelZone *)(zoff + poff + (u32)gUnk_0201930C);
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            to.player = (word << 19) >> 31;
            to.area = 15;
            to.slot = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(id, &from, &to);
        }
        cmd->step++;
        break;
    }
    default:
        sub_08024134(player, 0, slot);
        sub_080611AC();
        gCmd.running = 0;
        break;
    }
}

void sub_0800DF94(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;

    switch (cmd->step) {
    case 0: {
        u32 zoff = slot * 0x94;
        u32 poff = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(zoff + poff + (u32)gUnk_0201930C);
        if (card->id == 0)
            cmd->running = 0;
        else {
            sub_080240A8(player, sub_08062354(slot));
            cmd->step++;
        }
        break;
    }
    case 1: {
        struct DuelZone *zone;
        u32 pp;

        sub_08077AEC(0x11);
        from.player = player;
        from.area = 0;
        from.slot = slot;
        pp = player & 1;
        zone = (struct DuelZone *)(slot * 0x94 + pp * 0xD64 + (u32)gUnk_0201930C);
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        sub_08024380(&from, gUnk_0868CAC0, 0, 0);
        sub_08060FD0(player, slot);
        cmd->step++;
        break;
    }
    case 2: {
        u32 one = 1;
        u32 pp = player & one;
        u32 zoff = slot * 0x94;
        u32 poff = pp * 0xD64;
        u32 zoneoff = zoff + poff;
        u8 *base = (u8 *)gUnk_0201930C;
        struct DuelZone *zone = (struct DuelZone *)(zoneoff + (u32)base);
        struct DuelCard *saved;

        ((u8 *)zone)[2] |= 0x10;
        saved = &cmd->saved814;
        sub_08007558(saved, (struct DuelCard *)(poff + (u32)base + zoff));
        sub_08008E80(player, slot);
        from.player = player;
        from.area = 0;
        from.slot = slot;
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        to.player = player;
        to.area = 15;
        to.slot = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4((*(u32 *)saved << 20) >> 20, &from, &to);
        cmd->step++;
        break;
    }
    default:
        sub_08024134(player, 0, slot);
        sub_080611AC();
        gCmd.running = 0;
        break;
    }
}


void sub_0800E1E0(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;
    u32 step = cmd->step;

    switch (step) {
    case 0: {
        u32 zoff = slot * 0x94;
        u32 poff = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(zoff + poff + (u32)gUnk_0201930C);
        if (card->id == 0) {
            cmd->running = 0;
            break;
        }
        sub_080240A8(player, sub_08062354(slot));
        cmd->step++;
        break;
    }
    case 1: {
            struct DuelCard *saved = &cmd->saved814;
            u32 poff = (player & 1) * 0xD64;
            u8 *base = (u8 *)gUnk_0201930C;
            u8 *zonebase = base + poff;
            u32 zoff = slot * 0x94;
            struct DuelZone *zone;
            u32 word, area, destSlot;

            sub_08007558(saved, (struct DuelCard *)(zonebase + zoff));
            sub_0800747C(player, slot);
            sub_08060FD0(player, slot);
            word = *(u32 *)saved;
            if ((u16)((*(const u16 *)(0x08622AB4 + ((word << 21) >> 20))) - 0x780) > 0x4F) {
                from.player = player;
                from.area = 0;
                from.slot = slot;
                zone = (struct DuelZone *)(zoff + poff + (u32)base);
                from.flag14 = zone->flag6_0;
                from.flag15 = zone->flag6_1;
                to.player = (word << 19) >> 31;
                area = sub_08007994(((u32)*(u16 *)saved << 20) >> 20) ? 12 : 11;
                to.area = area;
                if (sub_08007994(((u32)*(u16 *)saved << 20) >> 20) == 0) {
                    /* Keep the initialized player base in r1 while the saved owner is read into r0. */
                    register u8 *players asm("r1") = base - 0x28;
                    u32 owner = (*(u32 *)saved << 19) >> 31;
                    destSlot = *(u8 *)(players + (owner & step) * 0xD64 + 2);
                } else
                    destSlot = 0;
                to.slot = destSlot;
                to.flag14 = 0;
                to.flag15 = ((struct DuelZone *)((player & 1) * 0xD64 + slot * 0x94 + (u32)gUnk_0201930C))->flag6_1;
                sub_080242C4((*(u32 *)&gCmd.saved814 << 20) >> 20, &from, &to);
            }
        }
        gCmd.step++;
        break;
    default:
        sub_08008EB4(player, slot);
        sub_080611AC();
        sub_08024134(player, 0, slot);
        cmd->running = 0;
        break;
    }
}



void sub_0800E438(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gCmd;
    u32 player = cmd->hdr >> 15;
    u32 slot = cmd->arg1;

    switch (cmd->step) {
    case 0: {
        u32 zoff = slot * 0x94;
        u32 poff = player * 0xD64;
        struct DuelCard *card = (struct DuelCard *)(zoff + poff + (u32)gUnk_0201930C);
        if (card->id == 0)
            cmd->running = 0;
        else {
            sub_080240A8(player, sub_08062354(slot));
            cmd->step++;
        }
        break;
    }
    case 1: {
        struct DuelCard *saved = &cmd->saved814;
        u32 poff = (player & 1) * 0xD64;
        u8 *base = (u8 *)gUnk_0201930C;
        u8 *zonebase = base + poff;
        u32 zoff = slot * 0x94;
        struct DuelZone *zone;
        u32 word;

        sub_08007558(saved, (struct DuelCard *)(zonebase + zoff));
        zone = (struct DuelZone *)(zoff + poff + (u32)base);
        ((struct ZoneFlags18 *)zone)->flag18 = 0;
        sub_08060FD0(player, slot);
        word = *(u32 *)saved;
        if ((u16)((*(const u16 *)(0x08622AB4 + ((word << 21) >> 20))) - 0x780) > 0x4F) {
            from.player = player;
            from.area = 0;
            from.slot = slot;
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            to.player = (word << 19) >> 31;
            to.area = sub_08007994((((u32)*(u16 *)saved << 20) >> 20)) ? 12 : 13;
            to.slot = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4((*(u32 *)saved << 20) >> 20, &from, &to);
        }
        gCmd.step++;
        break;
    }
    default:
        sub_0800747C(player, slot);
        sub_08008F14(player, slot);
        sub_08024134(player, 0, slot);
        sub_080611AC();
        cmd->running = 0;
        break;
    }
}


#if 0 /* NONMATCHING: structure matches; register allocation/stack frame differ (ROM: base r9, a r7, b sl, c/d on stack, step r6) */
/* Command 0x84 (hypothesis): place the card saved in gCmd+0x814 into zone
 * (arg2) and clear zone (arg1). */
void sub_0800E630(void)
{
    s8 a = (u8)gCmd.arg1;      /* source zone: player */
    s8 b = gCmd.arg1 >> 8;     /* source zone: slot */
    u16 c = (u8)gCmd.arg2;      /* destination zone: player */
    s8 d = gCmd.arg2 >> 8;     /* destination zone: slot */
    u32 step = gCmd.step;

    if (step != 0) {
        if (step != 1) {
            struct DuelZone *zoneA = ZONE(a & 1, b);
            struct DuelZone *zoneC = ZONE(c & 1, d);

            sub_08075294(zoneC, zoneA, 0x94);
            sub_08007558(&zoneC->card, &gCmd.saved814);
            sub_08075278(zoneA, 0x94);
            zoneC->unk7 &= ~4;
            sub_08024134(c, 0, d);
            sub_080611AC();
            gCmd.running = 0;
            return;
        }
        {
            struct CardLoc from;
            struct CardLoc to;
            struct DuelZone *zone = ZONE(a & 1, b);
            u8 word;

            sub_08007558(&gCmd.saved814, &zone->card);
            sub_08060FD0(a, b);
            from.player = a & 1;
            from.area = 0;
            from.slot = b;
            from.flag14 = zone->flag6_0;
            from.flag15 = zone->flag6_1;
            to.player = c & 1;
            to.area = 0;
            to.slot = d;
            to.flag14 = from.flag14;
            to.flag15 = from.flag15;
            if (d > 4)
                from.flag14 = 0;
            word = *(u32 *)&gCmd.saved814;
            sub_080242C4(word & 0xFFF, &from, &to);
            gCmd.step++;
            return;
        }
    }
    if (ZONE_CARD_ID(ZB(a & 1, b)) == 0) {
        gCmd.running = 0;
        return;
    }
    sub_080240A8(a, sub_08062354(b));
    gCmd.step++;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800D8A4", sub_0800E630); /* 0x0800E630 size 0x244 */
#if 0 /* NONMATCHING: logic/structure matches; register allocation differs (ROM: base r9, a r7, b r8, c sl, d at sp+0x9c, step r6; built keeps c/d differently and spills to sp+0x9c) */
/* Command 0x84 (hypothesis): swap the contents of two zones. arg1/arg2 each
 * encode (player | slot << 8); step 0 checks both zones hold cards, step 1
 * animates the held card (sub_0802432C), step 2 performs the swap. */
void sub_0800E874(void)
{
    u32 a = (u8)gCmd.arg1;      /* source zone: player */
    u8 b = gCmd.arg1 >> 8;     /* source zone: slot */
    u32 c = (u8)gCmd.arg2;      /* destination zone: player */
    int d = gCmd.arg2 >> 8;     /* destination zone: slot */
    u32 step = gCmd.step;

    if (step == 0) {
        if (ZONE_CARD_ID(ZB(a & 1, b)) == 0 || ZONE_CARD_ID(ZB(c & 1, d)) == 0) {
            gCmd.running = 0;
            return;
        }
        sub_0802408C(0x50);
        gCmd.step++;
        return;
    }
    if (step == 1) {
        struct CardLoc from;
        struct CardLoc to;
        struct DuelZone *zone = ZONE(a & 1, b);

        sub_08007558(&gCmd.saved814, &zone->card);
        sub_08060FD0(a, b);
        sub_08060FD0(c, d);
        from.player = a & 1;
        from.area = 0;
        from.slot = b;
        from.flag14 = zone->flag6_0;
        from.flag15 = zone->flag6_1;
        to.player = c & 1;
        to.area = 0;
        to.slot = d;
        to.flag14 = from.flag14;
        to.flag15 = from.flag15;
        if (d > 4)
            from.flag14 = 0;
        sub_0802432C(&from, &to);
        gCmd.step++;
        return;
    }
    {
        struct DuelZone tmp;

        sub_08075294(&tmp, ZONE(c & 1, d), 0x94);
        sub_08075294(ZONE(c & 1, d), ZONE(a & 1, b), 0x94);
        sub_08075294(ZONE(a & 1, b), &tmp, 0x94);
        sub_080611AC();
        gCmd.running = 0;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800D8A4", sub_0800E874); /* 0x0800E874 size 0x234 */
