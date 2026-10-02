#include "global.h"
#include "duel.h"
#include "duel_ui.h"

/*
 * Duel "script command" handlers (continued from code_0800EAA8). Each reads its
 * operands from the command block at 0x020185C0 and clears the "command
 * running" flag (bit 5 of byte 0x020185C0+0x80D) when done.
 * See wiki/functions/code-08011be0.md.
 */

/* The command block (gUnk_020185C0) and the card-location struct (struct DuelLoc)
 * come from duel_ui.h; struct DuelCard / DuelZone / DuelPlayer come from duel.h. */

/*
 * Local views kept because the canonical declarations in duel.h/duel_ui.h differ
 * from what this unit needs to match (the headers are shared, so they are not
 * changed here):
 *
 * - struct ZoneCard08011BE0: canonical struct DuelCard groups +0x00 bits 13..19
 *   as unk13 and names bit 20 flag20, but this unit sets bits 14..17 of a zone's
 *   card word individually (same 4-byte u32 container).
 * - struct DuelZone08011BE0: canonical struct DuelZone exposes +0x06/+0x07/+0x8C
 *   only as the byte fields counter6/unk7/unk8C[], but this unit matches the
 *   original per-bit field split (u32 containers at +0x04/+0x8C, u8 at +0x07).
 * - struct DuelPlayer08011BE0: canonical struct DuelPlayer names +0x0E as part of
 *   the u8 unkD[] region, but this unit reads a u16 array unkE[11] from +0x0E.
 * - struct DuelFlags08011BE0: canonical struct DuelState splits +0x1B12 into
 *   bitfields, but LINK_SKIP() tests bit 1 as a whole byte.
 * - struct CmdCard (gUnk_02018DD4): canonical struct DuelCard names bit 20, not
 *   the bit 17 this unit writes.
 */

/* Acting player of the current command (bit 15 of the command word). */
#define CMD_PLAYER() (gUnk_020185C0.cmd >> 15)

/* Card word at the start of a zone (canonical DuelCard lacks the bits 14..17). */
struct ZoneCard08011BE0 {
    u32 cardId:12;      /* bits 0-11: card id (0 = empty) */
    u32 unk0_12:2;
    u32 flag0_14:1;     /* 0x01 bit 6 */
    u32 flag0_15:1;     /* 0x01 bit 7 */
    u32 flag0_16:1;     /* 0x02 bit 0 */
    u32 flag0_17:1;     /* 0x02 bit 1 */
    u32 unk0_18:14;
};

/* One field zone of a player (0x94 bytes) at 0x0201930C + (p&1)*0xD64 + slot*0x94. */
struct DuelZone08011BE0 {
    struct ZoneCard08011BE0 w;  /* 0x00 */
    u32 unk4:16;
    u32 flag6_0:1;      /* 0x06 bit 0 */
    u32 flag6_1:1;      /* 0x06 bit 1 */
    u32 counter6_2:4;   /* 0x06 bits 2-5: counter (max 15) */
    u32 unk6_6:4;       /* 0x06 bits 6-9 */
    u32 flag7_2:1;      /* 0x07 bit 2 */
    u32 unk7_3:2;
    u8 flag7_5:1;       /* 0x07 bit 5 */
    u8 flag7_6:1;       /* 0x07 bit 6 */
    u8 flag7_7:1;       /* 0x07 bit 7 */
    u16 unk8;
    u8 unkA[0x40];
    u8 unk4A[0x40];
    u16 unk8A;
    u32 unk8C_0:2;
    u32 flag8C_2:1;     /* 0x8C bit 2 */
    u8 flag8C_3:1;      /* 0x8C bit 3 */
    u8 flag8C_4:1;      /* 0x8C bit 4 */
    u8 flag8C_5:1;      /* 0x8C bit 5 */
    u8 unk8C_6:2;
    u8 filler8D[0x91 - 0x8D];
    u8 unk91_0:3;
    u8 flag91_3:1;      /* 0x91 bit 3 */
    u8 unk91_4:4;
    u8 filler92[0x94 - 0x92];
};

/* Per-player duel state (0xD64 bytes), two of them from 0x020192E4. */
struct DuelPlayer08011BE0 {
    u8 filler0[0xE];
    u16 unkE[11];       /* 0x0E: per-slot value (hypothesis) */
    u8 filler24[0xD64 - 0x24];
};
extern struct DuelPlayer08011BE0 gUnk_020192E4_unkE[2] asm("gUnk_020192E4");

#define ZONE(p, s) ((struct DuelZone08011BE0 *)((u8 *)gUnk_0201930C + (s) * 0x94 + (p) * 0xD64))
#define ZONE_PS(p, s) ((struct DuelZone08011BE0 *)((u8 *)gUnk_0201930C + (p) * 0xD64 + (s) * 0x94))

void sub_080096F4(u32 *);
void sub_080611AC(void);
void sub_08007558(void *dst, void *src);

void sub_08075294(void *dst, void *src);
void sub_08077AEC(u32);
void sub_08024380(struct DuelLoc *loc, const void *, u32, u32);
void sub_08060FD0(u32, u32);
extern const u8 gUnk_0869771C[];

/* Card word of the command block (0x020185C0 + 0x814) seen as its own symbol. */
struct CmdCard {
    u32 id:12;
    u32 owner:1;        /* bit 12 */
    u32 unk13:4;
    u32 bit17:1;        /* bit 17 */
    u32 unk18:14;
};
extern struct CmdCard gUnk_02018DD4;
u32 sub_08062354(u32);
void sub_080240A8(u32, u32);
void sub_08007C58(u32 player, void *card);
void sub_080242C4(u32 id, struct DuelLoc *from, struct DuelLoc *to);
#define CMD_CARD ((struct CmdCard *)&gUnk_020185C0.card)

extern const u8 gUnk_0868DB94[];
extern const u8 gUnk_0868EC38[];
extern const u16 gUnk_08622AB4[];   /* maps card ID to card number */

/* Zone of the acting player, in the operand order some handlers need:
 * (p&1)*0xD64 + s*0x94 + base. Expects a local `player`. */
#define ZZ(s) ((struct DuelZone08011BE0 *)((player & 1) * 0xD64 + (s) * 0x94 + (u8 *)gUnk_0201930C))

void sub_08011BE0(void)
{
    u32 w = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    sub_080096F4(&w);
    sub_080611AC();
    gUnk_020185C0.running = 0;
}

/* 0x02015EE8: byte 1 bit 0 = link (two-GBA) duel (hypothesis). */
struct Unk02015EE8 {
    u8 unk0;
    u8 flags1;
};

/* 0x020192E0: canonical struct DuelState (duel.h) splits +0x1B12 into bitfields;
 * LINK_SKIP() tests bit 1 as a whole byte, so keep this local view. */
struct DuelFlags08011BE0 {
    u8 filler0[0x1B12];
    u8 flags1B12;       /* bit 1: ? (tested with the link flag) */
};

/* 0x02017A40: duel data; 20-byte entries from +0x258, entry count at +0x3C0. */
struct Unk02017A40Entry {
    u8 unk0[4];
    u8 unk4_0:2;
    u8 flag4_2:1;
    u8 flag4_3:1;
    u8 unk4_4:4;
    u8 unk5[15];
};

struct Unk02017A40 {
    u8 filler0[0x258];
    struct Unk02017A40Entry entries[18];
    u16 count;          /* +0x3C0 */
};

extern struct Unk02015EE8 gUnk_02015EE8;
extern struct DuelFlags08011BE0 gUnk_020192E0_flags asm("gUnk_020192E0");
extern struct Unk02017A40 gUnk_02017A40;

#define LINK_SKIP() ((gUnk_02015EE8.flags1 & 1) && (gUnk_020192E0_flags.flags1B12 & 2))

/* Flags the current (last) entry of 0x02017A40: bit 2, and bit 3 when arg2 != 0. */
void sub_08011C18(void)
{
    struct Unk02017A40Entry *e;

    if (!LINK_SKIP()) {
        if (gUnk_02017A40.count > 1) {
            e = &gUnk_02017A40.entries[gUnk_02017A40.count];
            e->flag4_2 = 1;
            if (gUnk_020185C0.arg2)
                e->flag4_3 = 1;
        }
    }
    gUnk_020185C0.running = 0;
}
/* No-op command: just finishes. */
void sub_08011C98(void)
{
    gUnk_020185C0.running = 0;
}

/* Link-skip test through the zones symbol (0x0201930C + 0x1AE6 = 0x020192E0 + 0x1B12). */
#define LINK_SKIP3() ((gUnk_02015EE8.flags1 & 1) && (((u8 *)gUnk_0201930C)[0x1AE6] & 2))

u16 sub_08009538(int player, int zone);
void sub_08017314(int player, int zone, u16 link0);

/* Zone +0x90 as a u16 container (flag91_3 = bit 11). The u8 container of
 * struct DuelZone08011BE0 lets CSE reuse the case's long-lived constant-1
 * register for the store mask, which swaps r7/r8 for the whole function. */
struct Zone90View08011CB4 {
    u8 filler0[0x90];
    u16 unk90_0:11;
    u16 flag91_3:1;
    u16 unk90_12:4;
};

/* Zone address helper: the inline evaluates `player & 1` before both products,
 * the ROM's order. */
static inline struct DuelZone08011BE0 *Zone08011CB4(u32 p, int s)
{
    return (struct DuelZone08011BE0 *)(s * 0x94 + p * 0xD64 + (u8 *)gUnk_0201930C);
}

/*
 * Sets zone arg2's flag91_3 from arg4. When set, plays the effect on the zone
 * (0x0868EC38 if its flag6_0 is set, else 0x0868DB94) and continues at step 3
 * (10 on the link-skip side); otherwise step 1, or finishes on the link-skip
 * side. Steps 1 and 3: if the zone holds card number 1068 (0x42C), pass the
 * link found by sub_08009538 to sub_08017314 (link0 = 0xFFFF, none).
 */
void sub_08011CB4(void)
{
    struct DuelLoc loc;
    u32 player = CMD_PLAYER();
    int slot = gUnk_020185C0.arg2;
    struct DuelZone08011BE0 *zone;
    u16 r;

    switch (gUnk_020185C0.step) {
    case 0:
        zone = Zone08011CB4(player & 1, slot);
        if ((*(u32 *)zone << 20) == 0) {
            gUnk_020185C0.running = 0;
            break;
        }
        ((struct Zone90View08011CB4 *)zone)->flag91_3 = gUnk_020185C0.arg4;
        if (gUnk_020185C0.arg4 != 0) {
            sub_08077AEC(0x10);
            loc.player = player;
            loc.area = 5;
            loc.index = slot - 5;
            if (slot > 9) {
                loc.area = 10;
                loc.index = 0;
            }
            loc.flag14 = zone->flag6_0;
            loc.flag15 = zone->flag6_1;
            sub_08024380(&loc, loc.flag14 ? gUnk_0868EC38 : gUnk_0868DB94, 0, 0);
            if (!LINK_SKIP3())
                gUnk_020185C0.step = 3;
            else
                gUnk_020185C0.step = 10;
        } else {
            if (!LINK_SKIP3())
                gUnk_020185C0.step++;
            else
                gUnk_020185C0.running = 0;
        }
        break;
    case 1:
        if (*(const u16 *)(0x08622AB4 + ((*(u32 *)Zone08011CB4(player & 1, slot) << 21) >> 20)) == 0x42C) {
            r = sub_08009538(player, slot);
            if (r != 0xFFFF)
                sub_08017314((u8)r, r >> 8, 0xFFFF);
        }
        gUnk_020185C0.running = 0;
        break;
    case 3:
        if (*(const u16 *)(0x08622AB4 + ((*(u32 *)Zone08011CB4(player, slot) << 21) >> 20)) == 0x42C) {
            r = sub_08009538(player, slot);
            if (r != 0xFFFF)
                sub_08017314((u8)r, r >> 8, 0xFFFF);
        }
        gUnk_020185C0.running = 0;
        break;
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
void sub_08011F38(void)
{
    u32 player = CMD_PLAYER();
    u16 slot = gUnk_020185C0.arg2;

    if (gUnk_020185C0.arg4 == 500) {
        u16 cur = gUnk_020192E4_unkE[player].unkE[slot];
        gUnk_020192E4_unkE[player].unkE[slot] = gUnk_020185C0.arg4 + cur;
    } else {
        gUnk_020192E4_unkE[player].unkE[slot] = gUnk_020185C0.arg4;
    }
    gUnk_020185C0.running = 0;
}


struct ZoneHead8 {
    u8 unk0[6];
    u8 flag6_0:1;
    u8 flag6_1:1;
    u8 counter6_2:4;
    u8 unk6_6:2;
};

void sub_08011FA0(void)
{
    u32 player = CMD_PLAYER();
    struct ZoneHead8 *zone = (struct ZoneHead8 *)(gUnk_020185C0.arg2 * 0x94 + player * 0xD64 + (u8 *)gUnk_0201930C);
    if (zone->counter6_2 < 15)
        zone->counter6_2++;
    gUnk_020185C0.running = 0;
}

/* The players array addressed as gUnk_020192E0 + 4 (not via .players[]), so agbcc
 * keeps the +4 base apart from the +0x26 field offset and derives it from the
 * zones literal (sym+0x2C) by CSE's related-value reuse (r3 - 0x28). */
#define PLAYERS_08011FFC ((struct DuelPlayer *)((u8 *)&gUnk_020192E0 + 4))

/* Sets zone flags of zone arg2 from the bits of arg4; in phase 3, if the acting
 * player equals linkSkip, also clears the slot bit in the player's zoneMask. */
void sub_08011FFC(void)
{
    /* FAKEMATCH: the separate copy `player = p` gives the shift result its own
     * short-lived pseudo (r0) that the `& 1` reuses, with the player kept in r6. */
    u32 p = CMD_PLAYER();
    u32 player = p;
    int slot = gUnk_020185C0.arg2;
    /* All addresses go through the one symbol gUnk_020192E0, so the 0x1B12 flag
     * byte and the players base are derived from the one zones literal. */
    struct DuelZone08011BE0 *zone = (struct DuelZone08011BE0 *)&gUnk_020192E0.players[p & 1].zones[slot];

    if (gUnk_020185C0.arg4 & 1)
        zone->w.flag0_14 = 1;
    if (gUnk_020185C0.arg4 & 2)
        zone->w.flag0_15 = 1;
    if (gUnk_020185C0.arg4 & 4)
        zone->w.flag0_16 = 1;
    if (gUnk_020185C0.arg4 & 8)
        zone->w.flag0_17 = 1;
    if (gUnk_020185C0.arg4 & 0x10)
        zone->flag7_6 = 1;
    if (gUnk_020185C0.arg4 & 0x20)
        zone->flag7_7 = 1;
    if (gUnk_020192E0.phase1B12 == 3 && player == gUnk_020192E0.linkSkip)
        PLAYERS_08011FFC[player & 1].zoneMask &= ~(1 << slot);
    gUnk_020185C0.running = 0;
}
extern const u8 gUnk_0868DB94[];
extern const u8 gUnk_0868EC38[];

/*
 * Counterpart of sub_08011FFC: step 0 plays an effect on zone arg2 (anim
 * 0x0868EC38 if the zone's flag6_0 is set, else 0x0868DB94); then clears the
 * zone flags selected by the bits of arg4.
 */
void sub_080120E0(void)
{
    struct DuelLoc loc;
    u32 player = CMD_PLAYER();
    u16 slot = gUnk_020185C0.arg2;
    struct DuelZone08011BE0 *zones = (struct DuelZone08011BE0 *)((u8 *)gUnk_0201930C + (player & 1) * 0xD64);
    struct DuelZone08011BE0 *zone = zones + slot;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_08077AEC(0x10);
        loc.player = player;
        loc.area = 0;
        loc.index = slot;
        loc.flag14 = ZZ(slot)->flag6_0;
        loc.flag15 = ZZ(slot)->flag6_1;
        sub_08024380(&loc, loc.flag14 ? gUnk_0868EC38 : gUnk_0868DB94, 0, 0);
        gUnk_020185C0.step++;
        break;
    default:
        if (gUnk_020185C0.arg4 & 1)
            zone->w.flag0_14 = 0;
        if (gUnk_020185C0.arg4 & 2)
            zone->w.flag0_15 = 0;
        if (gUnk_020185C0.arg4 & 4)
            zone->w.flag0_16 = 0;
        if (gUnk_020185C0.arg4 & 8)
            zone->w.flag0_17 = 0;
        if (gUnk_020185C0.arg4 & 0x10)
            zone->flag7_6 = 0;
        if (gUnk_020185C0.arg4 & 0x20)
            zone->flag7_7 = 0;
        gUnk_020185C0.running = 0;
        break;
    }
}
void sub_08012264(void)
{
    ZONE(CMD_PLAYER(), gUnk_020185C0.arg2)->flag7_5 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}

extern const u8 gUnk_08690D0C[];
extern const u16 gUnk_08622AB4[];   /* maps card ID to card number */
void sub_08008CFC(u32 player, u32 slot, u32 toArea15);

/*
 * Destroys the card in zone arg2 of the acting player: step 0 plays an effect
 * and clears the zone (sub_08060FD0); step 1 takes the card out
 * (sub_08008CFC) and, unless it is a token (card number 1920-1999), animates
 * it to its owner's area 15 (arg4 != 0) or 14.
 */
void sub_080122B4(void)
{
    struct DuelLoc from, to;
    u32 player = CMD_PLAYER();
    u32 slot = gUnk_020185C0.arg2;
    u32 mode = gUnk_020185C0.arg4;
    struct DuelZone08011BE0 *zones;
    struct CmdCard *card;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_08077AEC(0x10);
        from.player = player;
        from.area = 0;
        from.index = slot;
        from.flag14 = ZZ(slot)->flag6_0;
        from.flag15 = ZZ(slot)->flag6_1;
        sub_08024380(&from, gUnk_08690D0C, -16, -32);
        sub_08060FD0(player, slot);
        gUnk_020185C0.step++;
        break;
    case 1:
        card = CMD_CARD;
        zones = (struct DuelZone08011BE0 *)((u8 *)gUnk_0201930C + (player & 1) * 0xD64);
        sub_08007558(card, zones + slot);
        sub_08008CFC(player, slot, mode);
        /* Card number of the low 11 id bits, read through the table's integer address
         * (as in sub_0800E1E0) so the index is computed before the table address. */
        if ((u16)(*(const u16 *)(0x08622AB4 + (((u32)*(u16 *)card << 21) >> 20)) - 1920) > 79) {
            from.player = player;
            from.area = 0;
            from.index = slot;
            from.flag14 = ZZ(slot)->flag6_0;
            from.flag15 = ZZ(slot)->flag6_1;
            to.player = card->owner;
            to.area = mode ? 15 : 14;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(card->id, &from, &to);
        }
        gUnk_020185C0.step++;
        break;
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}

/*
 * Takes control of card: zone arg2 of the acting player is animated to the
 * opponent's area 13 (hypothesis) and handed over with sub_08007C58.
 */
void sub_080124E8(void)
{
    struct DuelLoc from, to;
    struct DuelZone08011BE0 *zones;
    u32 player = CMD_PLAYER();
    u16 slot = gUnk_020185C0.arg2;
    u32 other = player - 1;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, sub_08062354(slot));
        gUnk_020185C0.step++;
        break;
    case 1:
        zones = (struct DuelZone08011BE0 *)((u8 *)gUnk_0201930C + (player & 1) * 0xD64);
        sub_08007558((struct DuelZone08011BE0 *)&gUnk_02018DD4, zones + slot);
        gUnk_02018DD4.bit17 = 1;
        sub_08060FD0(player, slot);
        from.player = player;
        from.area = 0;
        from.index = slot;
        from.flag14 = ZZ(slot)->flag6_0;
        from.flag15 = ZZ(slot)->flag6_1;
        to.player = other;
        to.area = 13;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(gUnk_02018DD4.id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_08007C58(other, (u8 *)&gUnk_020185C0 + 0x814);
        ((struct DuelZone08011BE0 *)(player * 0xD64 + slot * 0x94 + (u8 *)gUnk_0201930C))->w.cardId = 0;
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/* Halfword view of zone +0x06 (u16 container). */
struct ZoneHead16 {
    u16 unk0[3];
    u16 flag6_0:1;
    u16 flag6_1:1;
    u16 counter6_2:4;
    u16 unk6_6:4;       /* bits 6-9 */
    u16 unk6_10:6;
};

/* Lowers zone value unk6_6 to arg4 (or sets it when 0). */
void sub_08012670(void)
{
    u32 player = CMD_PLAYER();
    int cur;
    struct ZoneHead16 *zone = (struct ZoneHead16 *)(gUnk_020185C0.arg2 * 0x94 + player * 0xD64 + (u8 *)gUnk_0201930C);
    cur = zone->unk6_6;
    if (cur == 0 || cur > gUnk_020185C0.arg4)
        zone->unk6_6 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_080126D4(void)
{
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_080126F0(void)
{
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_0801270C(void)
{
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_08012728(void)
{
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_08012744(void)
{
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_08012760(void)
{
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_0801277C(void)
{
    gUnk_020185C0.running = 0;
}

void sub_08012798(void)
{
    ZONE(CMD_PLAYER(), gUnk_020185C0.arg2)->flag8C_5 = 1;
    gUnk_020185C0.running = 0;
}

void sub_080127E0(void)
{
    ZONE(CMD_PLAYER(), gUnk_020185C0.arg2)->flag8C_3 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}

void sub_08012834(void)
{
    ZONE(CMD_PLAYER(), gUnk_020185C0.arg2)->flag8C_4 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_08012888(void)
{
    gUnk_020185C0.running = 0;
}

void sub_080128A4(void)
{
    ZONE(CMD_PLAYER(), gUnk_020185C0.arg2)->flag7_2 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}

void sub_080128F8(void)
{
    /* arg2 is a packed location: low byte player, high byte slot */
    u32 slot = gUnk_020185C0.arg2 >> 8;
    u32 player = *(u8 *)&gUnk_020185C0.arg2;
    ZONE(player & 1, slot)->flag8C_2 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}

/* No-op command: just finishes. */
void sub_08012950(void)
{
    gUnk_020185C0.running = 0;
}

extern const u16 gUnk_08623DF4[];   /* Maps a card number to its card ID. */
static inline u16 CardNumberToId(u16 num)
{
    if (num == 0xFFFF)
        return 0;
    else if (num < 2000)
        return *(((const u16 *)0x08623DF4) + (num & 0x7FF));
    else
        return *(((const u16 *)0x08623DF4) + ((num - 2000) & 0x7FF)) + 1;
}

/* Card word as written into a zone by sub_08007A4C. */
struct ZoneCard {
    u32 id:12;
    u32 owner:1;        /* bit 12 */
    u32 bit13:1;        /* bit 13 (hypothesis: controller) */
    u32 bit14:1;
    u32 bit15:1;
    u32 bit16:1;
    u32 bit17:1;
    u32 bit18:1;
    u32 unk19:13;
};

void sub_080240A8(u32, u32);
void sub_08007A4C(u32 player, u32 slot, struct ZoneCard *card, u32 faceDown, u32 a4);

/*
 * Summons a token: card number 1920 + arg4 (arg4 0..3; 1/2 with the face
 * flag) into monster zone (u8)arg2 of the acting player.
 */
void sub_0801296C(void)
{
    /* The ROM initializes the defined card flags; bits 19..31 stay untouched. */
    struct ZoneCard card;
    u32 player = CMD_PLAYER();
    u8 slot = gUnk_020185C0.arg2;
    u16 kind = gUnk_020185C0.arg4;
    u32 face;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(0, 0);
        gUnk_020185C0.step++;
        return;
    }
    switch (kind) {
    case 0:
        face = 0;
        break;
    case 1:
        face = 1;
        break;
    case 2:
        face = 1;
        break;
    case 3:
        face = 0;
        break;
    default:
        gUnk_020185C0.running = 0;
        return;
    }
    card.id = CardNumberToId(kind + 1920);
    card.owner = player;
    card.bit13 = player;
    card.bit14 = 1;
    card.bit15 = 0;
    card.bit16 = 1;
    card.bit17 = 0;
    card.bit18 = 0;
    sub_08007A4C(player, slot, &card, face, 1);
    ((struct DuelZone08011BE0 *)((player & 1) * 0xD64 + slot * 0x94 + (u8 *)gUnk_0201930C))->flag7_2 = 1;
    sub_080611AC();
    gUnk_020185C0.running = 0;
}

/* Writes card word (arg4 | arg6 << 16) into zone (u8)arg2 of the acting player. */
void sub_08012AE0(void)
{
    u32 player = CMD_PLAYER();
    u8 slot = gUnk_020185C0.arg2;
    u32 card = (gUnk_020185C0.arg6 << 16) | gUnk_020185C0.arg4;
    sub_08007558(&gUnk_020192E4[player].zones[slot], &card);
    sub_080611AC();
    gUnk_020185C0.running = 0;
}


/*
 * Moves the card in zone arg2 of the acting player to zone arg4 (copying the
 * zone with sub_08075294), marks the destination (flag6_0), clears the source
 * card, and plays an effect at the destination (sub_08024380).
 */
void sub_08012B34(void)
{
    struct DuelLoc loc;
    struct DuelZone08011BE0 *zones;
    u32 player = CMD_PLAYER();
    u16 from = gUnk_020185C0.arg2;
    u16 to = gUnk_020185C0.arg4;

    switch (gUnk_020185C0.step) {
    case 0:
        zones = (struct DuelZone08011BE0 *)((u8 *)gUnk_0201930C + (player & 1) * 0xD64);
        sub_08075294(zones + to, zones + from);
        ZZ(to)->flag6_1 = 0;
        ZZ(to)->flag6_0 = 1;
        ZZ(from)->w.cardId = 0;
        sub_08077AEC(0x10);
        loc.player = player;
        loc.area = 0;
        loc.index = to;
        loc.flag14 = 1;
        loc.flag15 = 0;
        sub_08024380(&loc, gUnk_0869771C, 0, 0);
        sub_08060FD0(player, from);
        gUnk_020185C0.step++;
        break;
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
