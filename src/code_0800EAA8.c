#include "global.h"
#include "duel.h"

/*
 * Duel "script command" handlers. The dispatcher sub_0801ECA8 switches on
 * (gUnk_020185C0.cmd & 0xFFF) - 1 and calls one of these; each handler reads
 * its operands from the command block at 0x020185C0 and clears the
 * "command running" flag (bit 5 of byte 0x020185C0+0x80D) when it is done.
 */

/* Command block at 0x020185C0 (hypothesis: current duel command). */
struct DuelCmd {
    u16 cmd;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg2;           /* 0x002: usually a zone/slot index */
    u16 arg4;           /* 0x004 */
    u16 arg6;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u16 step:7;         /* 0x80A bits 0-6: multi-frame handler state */
    u16 counter:7;      /* 0x80A bits 7-13 */
    u16 unk80A_14:2;
    u8 unk80C;
    u8 unk80D_0:5;
    u8 running:1;       /* 0x80D bit 5: command in progress */
    u8 unk80D_6:2;
    u8 filler80E[0x814 - 0x80E];
    u8 card[4];         /* 0x814 struct DuelCard: card being moved (u8 so the block stays 2-aligned) */
};

/* Card location for the card-move animation sub_080242C4 (4 bytes; see code_08010BDC). */
struct CardLoc {
    u16 player:1;       /* bit 0 */
    u16 area:4;         /* bits 1-4: 0 monster zone, 5 spell/trap, 10 field, 11 hand, 13, 14, 15 */
    u16 index:9;        /* bits 5-13 */
    u16 flag14:1;
    u16 flag15:1;
    u16 unk2;
};

/*
 * Local views kept because the canonical declarations in duel.h differ from what
 * this unit needs to match (duel.h is shared, so it is not changed here):
 *
 * - struct DuelZoneWord4 (below): canonical struct DuelZone declares +0x04 as `u16
 *   serial` and +0x06 as the u8 bitfields flag6_0/flag6_1/counter6; this unit reads and
 *   writes +0x04 as one u32 bitfield container (its counter sits at bits 18-21, i.e.
 *   +0x06 bits 2-5), so sub_0800EBF4 and sub_0800ECB0 keep this view.
 * - struct DuelCardBit22 (below): canonical struct DuelCard has flag20 at bit 20 and
 *   unk21:11 for bits 21-31, so the bit-22 flag sub_0800EBA0 writes has no canonical
 *   field name.
 * - struct DuelZone90 (below): canonical struct DuelZone stops at +0x8C (unk8C[8]), but
 *   sub_0800EB44 and sub_0800ECB0 write the u32 bitfield at +0x90 bits 13-17.
 */
struct DuelZoneWord4 {
    u8 filler[4];
    u32 unk4_0:18;
    u32 unk4_18:4;      /* 0x06 bits 2-5 (canonical counter6) */
    u32 unk4_22:10;
};

struct DuelCardBit22 {
    u32 unk0_0:22;      /* 0x00 bits 0-21: card id and flags */
    u32 flag0_22:1;     /* 0x02 bit 6 */
    u32 unk0_23:9;
};

struct DuelZone90 {
    u8 filler[0x90];
    u32 unk90_0:13;
    u32 unk90_13:5;     /* 0x90 bits 13-17 */
    u32 unk90_18:14;
};

/* struct DuelPlayerFlags: canonical struct DuelPlayer declares +0x07 as the bitfields
 * deckOut/winA/...; sub_0800F0F8 sets it with a plain `|= 1` (ldrb/orrs/strb), so a
 * whole-byte view matches. */
struct DuelPlayerFlags {
    u8 filler[7];
    u8 flags7;
};

extern struct DuelCmd gUnk_020185C0;

#define CMD_PLAYER (gUnk_020185C0.cmd >> 15)
#define CMD_CARD ((struct DuelCard *)gUnk_020185C0.card)
/* Halfword at cmd +0x80C seen as bitfields (padded past 4 bytes so it is accessed with ldrh/strh). */
struct Cmd80C {
    u16 unk0:5;
    u16 unk5:7;         /* bits 5-11: cleared by sub_0800F0F8 (hypothesis: animation counter) */
    u16 unk12:4;
    u8 pad[4];
};
#define CMD_80C (*(struct Cmd80C *)&gUnk_020185C0.unk80C)
/* card id of a zone: low 12 bits of its first word */
#define ZONE_CARD_ID(z) ((*(u32 *)(z) << 20) >> 20)
#define ZONE_CARD(p, s) ZONE_CARD_ID(ZONE(p, s))
#define ZONE(p, s) ((struct DuelZone *)((s) * 0x94 + (p) * 0xD64 + (u8 *)gUnk_0201930C))

void sub_0800935C(u16, u16, u16);
void sub_08009424(u16, u16, u16);
void sub_0805F96C(void);
int sub_08007F48(u32 player, void *card);
void sub_08007558(struct DuelCard *dst, void *src);
void sub_080096F4(struct DuelCard *card);
int sub_08007ED0(u32 player, u32 a, struct DuelCard *card);
void sub_08007A4C(u32 player, u32 zone, struct DuelCard *card, u32 a, u32 b);
void sub_08009768(struct DuelCard *card);
void sub_080080B4(u32 player, u16 a);
void sub_08009EAC(u32 player, void *card);
int sub_080080D8(u32 player, void *card);
void sub_080240A8(u32 player, u32 area);
void sub_08024134(u32 player, u32 area, u32 index);
void sub_080242C4(u32 id, struct CardLoc *from, struct CardLoc *to);
void sub_080611AC(void);
void sub_0802408C(u32 a);
void sub_080241F0(const void *a);
void sub_08007E68(u32 player, u32 a);
void sub_08024228(u32 a, u32 b, u32 c);
void sub_08077AEC(u16 se);                        /* PlaySE */
void sub_08024218(void);
void sub_08060578(void);
void sub_08022A9C(u32 player);
extern const u8 gUnk_08694EA8[];
/* Duel screen state at 0x0201CFB0 (see code_08012C4C); only the field used here. */
struct DuelScreen {
    u8 filler0[0x852];
    u16 unk852;         /* 0x852: nonzero while busy (hypothesis) */
};
extern struct DuelScreen gUnk_0201CFB0;
extern u8 gUnk_02015EE8[];      /* byte 1 bit 0: link duel (hypothesis) */
/* Link state at 0x02017FB0 (see code_08021CC8; u32 bitfield containers). */
struct Unk02017FB0 {
    u8 filler0[0x304];
    u32 unk304:8;
    u32 dirtyHand:1;    /* +0x305 bit 0 (hypothesis, per code_08021CC8) */
    u32 dirtyDeck:1;    /* +0x305 bit 1 */
    u32 unk305_2:22;
};
extern struct Unk02017FB0 gUnk_02017FB0;
void sub_08075294(void *dst, const void *src, u32 size);

void sub_0800EAA8(void)
{
    sub_0800935C(gUnk_020185C0.arg4, gUnk_020185C0.arg2, 1);
    sub_0805F96C();
    sub_080611AC();
    gUnk_020185C0.running = 0;
}
void sub_0800EADC(void)
{
    sub_0800935C(gUnk_020185C0.arg4, gUnk_020185C0.arg2, gUnk_020185C0.arg6);
    sub_0805F96C();
    sub_080611AC();
    gUnk_020185C0.running = 0;
}
void sub_0800EB10(void)
{
    sub_08009424(gUnk_020185C0.arg4, gUnk_020185C0.arg2, gUnk_020185C0.arg6);
    sub_0805F96C();
    sub_080611AC();
    gUnk_020185C0.running = 0;
}
void sub_0800EB44(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, (u8)gUnk_020185C0.arg2);
    ((struct DuelZone90 *)zone)->unk90_13 = gUnk_020185C0.arg4;
    sub_080611AC();
    gUnk_020185C0.running = 0;
}
void sub_0800EBA0(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gUnk_020185C0.arg2);
    ((struct DuelCardBit22 *)zone)->flag0_22 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}
void sub_0800EBF4(void)
{
    u16 val = gUnk_020185C0.arg4;
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gUnk_020185C0.arg2);
    if (ZONE_CARD_ID(zone))
        ((struct DuelZoneWord4 *)zone)->unk4_18 = val + ((struct DuelZoneWord4 *)zone)->unk4_18;
    gUnk_020185C0.running = 0;
}
void sub_0800EC54(void)
{
    u16 val = gUnk_020185C0.arg4;
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gUnk_020185C0.arg2);
    u8 old = ((struct DuelZoneWord4 *)zone)->unk4_18;
    if (ZONE_CARD_ID(zone))
        ((struct DuelZoneWord4 *)zone)->unk4_18 = val;
    gUnk_020185C0.running = 0;
}
void sub_0800ECB0(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gUnk_020185C0.arg2);
    if (ZONE_CARD_ID(zone)) {
        ((struct DuelZoneWord4 *)zone)->unk4_18 = 0;
        ((struct DuelZone90 *)zone)->unk90_13 = gUnk_020185C0.arg4;
    }
    gUnk_020185C0.running = 0;
}
void sub_0800ED1C(void)
{
    u32 player = CMD_PLAYER;
    struct DuelZone *zone = ZONE(player, gUnk_020185C0.arg2);
    zone->numLinks = 0;
    gUnk_020185C0.running = 0;
}
void sub_0800ED58(void)
{
    struct DuelZone *base = (struct DuelZone *)((u8 *)gUnk_0201930C + CMD_PLAYER * 0xD64);
    struct DuelZone *src = base + gUnk_020185C0.arg2;
    struct DuelZone *dst;

    __asm__("" : : : "r6");
    dst = base + gUnk_020185C0.arg4;
    sub_08075294(dst->links, src->links, 0x40);
    sub_08075294(dst->linkKinds, src->linkKinds, 0x40);
    dst->numLinks = src->numLinks;
    src->numLinks = 0;
    gUnk_020185C0.running = 0;
}
void sub_0800EDCC(void)
{
    if (gUnk_020185C0.arg4 != 0) {
        gUnk_020192E0.queueZone[gUnk_020192E0.queueCount] = ((u8)gUnk_020185C0.arg2 << 8) | CMD_PLAYER;
        gUnk_020192E0.queueArg[gUnk_020192E0.queueCount] = gUnk_020185C0.arg4;
        gUnk_020192E0.queueCount++;
    }
    gUnk_020185C0.running = 0;
}
#if 0 /* NONMATCHING: ROM hoists only the cmd load out of the loop (tests & 0x8000 inside); this hoists the masked value and is 4 bytes longer */
void sub_0800EE50(void)
{
    s32 i, j;
    for (i = 0; i < gUnk_020192E0.queueCount; i++) {
        if (gUnk_020192E0.queueZone[i] == (((u8)gUnk_020185C0.arg2 << 8) | ((gUnk_020185C0.cmd & 0x8000) ? 1 : 0))) {
            gUnk_020192E0.queueCount = (u16)(gUnk_020192E0.queueCount - 1);
            for (j = i; j < gUnk_020192E0.queueCount; j++) {
                gUnk_020192E0.queueZone[j] = gUnk_020192E0.queueZone[j + 1];
                gUnk_020192E0.queueArg[j] = gUnk_020192E0.queueArg[j + 1];
            }
        }
    }
    gUnk_020185C0.running = 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800EAA8", sub_0800EE50); /* 0x0800EE50 size 0xE8 */
void sub_0800EF38(void)
{
    int player = CMD_PLAYER;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_0802408C(0x50);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_080241F0(gUnk_08694EA8);
        gUnk_020185C0.counter = 0;
        gUnk_020185C0.step++;
        /* fallthrough */
    case 2:
        sub_08007E68(player, 3);
        sub_08024228(0, 0, 1);
        if (gUnk_0201CFB0.unk852 != 0)
            break;
        gUnk_020185C0.counter++;
        if ((s8)gUnk_020185C0.counter <= 3) {
            sub_08077AEC(7);
            sub_08024218();
        } else {
            gUnk_020185C0.step++;
        }
        break;
    case 3:
        sub_08060578();
        if ((gUnk_02015EE8[1] & 1) && !(gUnk_020192E0.linkSkip)) {
            sub_08022A9C(player);
            gUnk_020185C0.step = 10;
        } else {
            gUnk_020185C0.running = 0;
        }
        break;
    case 10:
        if (gUnk_02017FB0.dirtyDeck)
            gUnk_020185C0.running = 0;
        break;
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
/* Draw arg4 cards for the acting player (deck to hand animation,
 * sub_080080B4(player, arg2) per card). With an empty deck, set player flag +7
 * bit 0 and stop. */
void sub_0800F0F8(void)
{
    struct CardLoc from, to;
    int player = gUnk_020185C0.cmd >> 15;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        if (gUnk_020192E4[player].deckCount != 0)
            break;
        ((struct DuelPlayerFlags *)&gUnk_020192E4[player])->flags7 |= 1;
        gUnk_020185C0.running = 0;
        break;
    case 1:
        from.player = player;
        from.area = 13;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 11;
        to.index = gUnk_020192E4[player & 1].handCount;
        to.flag14 = 0;
        to.flag15 = 0;
        sub_080242C4(0, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_080080B4(player, gUnk_020185C0.arg2);
        gUnk_020185C0.arg4--;
        if (gUnk_020185C0.arg4 != 0) {
            ((struct Cmd80C *)&gUnk_020185C0.unk80C)->unk5 = 0;
            gUnk_020185C0.step--;
            break;
        }
        /* fallthrough */
    default:
        sub_08024134(player, 11, gUnk_020192E4[player].handCount - 1);
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/* Repeat arg2 times: take a card for the acting player with sub_08007ED0(player, 0, &cmd card) and
 * animate it from area 13 to area 14, then commit with sub_080096F4. */
void sub_0800F294(void)
{
    struct CardLoc from, to;
    u32 player = gUnk_020185C0.cmd >> 15;
    int p2 = player;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (gUnk_020185C0.arg2 == 0) {
            gUnk_020185C0.running = 0;
            break;
        }
        if (sub_08007ED0(player, 0, CMD_CARD)) {
            from.player = p2;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = p2;
            to.area = 14;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(CMD_CARD->id, &from, &to);
            gUnk_020185C0.step++;
        } else {
            sub_08024134(p2, 13, 0);
            sub_080611AC();
            gUnk_020185C0.running = 0;
        }
        break;
    case 2:
        sub_080096F4(CMD_CARD);
        sub_080611AC();
        gUnk_020185C0.arg2--;
        if (gUnk_020185C0.arg2 != 0)
            gUnk_020185C0.step--;
        else
            gUnk_020185C0.step++;
        break;
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
#if 0 /* NONMATCHING: case 0: ROM tests the stored counter by extracting it (lsl #18; lsr #25; cmp), this compiles to an and-mask test */
/* Repeat arg2 times (counter at cmd +0x80A bits 7-13): take a card for the acting player with
 * sub_08007ED0(player, 0, &cmd card), animate it from area 13 to area 15, commit with sub_08009768. */
void sub_0800F3D0(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gUnk_020185C0;
    u32 player = gUnk_020185C0.cmd >> 15;
    int p2 = player;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        cmd->counter = cmd->arg2;
        if (!(cmd->counter > 0)) {
            cmd->running = 0;
            break;
        }
        cmd->step++;
        break;
    case 1:
        if (sub_08007ED0(player, 0, CMD_CARD)) {
            from.player = p2 & 1;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = p2 & 1;
            to.area = 15;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(CMD_CARD->id, &from, &to);
            cmd->step++;
        } else {
            sub_08024134(p2, 13, 0);
            sub_080611AC();
            cmd->running = 0;
        }
        break;
    case 2:
        sub_08009768(CMD_CARD);
        sub_080611AC();
        cmd->counter--;
        if (cmd->counter > 0) {
            cmd->step--;
            break;
        }
        /* fallthrough */
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
#endif
/* Repeat arg2 times (counter at cmd +0x80A bits 7-13): take a card for the acting player with
 * sub_08007ED0(player, 0, &cmd card), animate it from area 13 to area 15, commit with sub_08009768. */
void sub_0800F3D0(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gUnk_020185C0;
    u32 player = gUnk_020185C0.cmd >> 15;
    u32 p2 = player;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        cmd->counter = cmd->arg2;
        if ((s8)cmd->counter == 0) {
            cmd->running = 0;
            break;
        }
        cmd->step++;
        break;
    case 1:
        if (sub_08007ED0(player, 0, CMD_CARD)) {
            from.player = p2 & 1;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = p2 & 1;
            to.area = 15;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(CMD_CARD->id, &from, &to);
            cmd->step++;
        } else {
            sub_08024134(p2, 13, 0);
            sub_080611AC();
            cmd->running = 0;
        }
        break;
    case 2:
        sub_08009768(CMD_CARD);
        sub_080611AC();
        cmd->counter--;
        if ((s8)cmd->counter > 0) {
            cmd->step--;
            break;
        }
        /* fallthrough */
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
/* Return card word (arg2 | arg4 << 16) from area 13 to the acting player's
 * hand. If its owner accepts it (sub_08007F48), animate it from area 13 to the
 * hand (area 11) and add it with sub_08009EAC. */
void sub_0800F544(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gUnk_020185C0;
    u32 player = cmd->cmd >> 15;
    u32 w = cmd->arg2 | (cmd->arg4 << 16);

    switch (cmd->step) {
    case 0:
        sub_080240A8(player, 11);
        cmd->step++;
        break;
    case 1:
        if (sub_08007F48(((struct DuelCard *)&w)->owner, &w) == 0) {
            cmd->running = 0;
            break;
        }
        from.player = player;
        from.area = 13;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 11;
        to.index = gUnk_020192E4[player & 1].handCount;
        to.flag14 = 0;
        to.flag15 = 0;
        sub_080242C4(((struct DuelCard *)cmd->card)->id, &from, &to);
        cmd->step++;
        break;
    default:
        sub_08009EAC(player, &w);
        sub_080611AC();
        sub_08024134(player, 11, 0);
        cmd->running = 0;
        break;
    }
}
/* Calls sub_08007F48(player, card word arg2 | arg4 << 16). */
void sub_0800F678(void)
{
    u32 player = CMD_PLAYER;
    u32 w = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;

    sub_08007F48(player, &w);
    gUnk_020185C0.running = 0;
}
/* Move card word (arg2 | arg4 << 16) of the acting player, if sub_08007F48 accepts it,
 * from area 13 to monster zone arg6 (animation), then place it there with sub_08007A4C. */
void sub_0800F6B0(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gUnk_020185C0;
    u32 player = gUnk_020185C0.cmd >> 15;
    u32 w = gUnk_020185C0.arg2 | (gUnk_020185C0.arg4 << 16);
    u32 zone = gUnk_020185C0.arg6;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_08007F48(player, &w) != 0) {
            sub_08007558(CMD_CARD, &w);
            from.player = player;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player;
            to.area = 0;
            to.index = zone;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(CMD_CARD->id, &from, &to);
            gUnk_020185C0.step++;
            break;
        }
        goto done;
    default:
        sub_08007A4C(player, zone, CMD_CARD, 0, 1);
        sub_08024134(player, 0, zone);
        sub_080611AC();
        cmd->running = 0;
        break;
    done:
        sub_080611AC();
        cmd->running = 0;
        break;
    }
}
/* Move card word (arg2 | arg4 << 16) of the acting player, if sub_08007F48 accepts it,
 * from area 13 to area 14 (animation), then commit with sub_080096F4. */
void sub_0800F7F0(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gUnk_020185C0;
    int player = gUnk_020185C0.cmd >> 15;
    u32 w = gUnk_020185C0.arg2 | (gUnk_020185C0.arg4 << 16);

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_08007F48(player, &w) != 0) {
            sub_08007558(CMD_CARD, &w);
            from.player = player & 1;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player & 1;
            to.area = 14;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(CMD_CARD->id, &from, &to);
            gUnk_020185C0.step++;
            break;
        }
        sub_080611AC();
        cmd->running = 0;
        break;
    default:
        sub_080096F4(CMD_CARD);
        sub_08024134(player, 14, 0);
        sub_080611AC();
        cmd->running = 0;
        break;
    }
}
/* Move card word (arg2 | arg4 << 16) of the acting player, if sub_08007F48 accepts it,
 * from area 13 to area 15 (animation), then commit with sub_08009768. */
void sub_0800F8F8(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gUnk_020185C0;
    int player = gUnk_020185C0.cmd >> 15;
    u32 w = gUnk_020185C0.arg2 | (gUnk_020185C0.arg4 << 16);

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_08007F48(player, &w) != 0) {
            sub_08007558(CMD_CARD, &w);
            from.player = player & 1;
            from.area = 13;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player & 1;
            to.area = 15;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(CMD_CARD->id, &from, &to);
            gUnk_020185C0.step++;
            break;
        }
        sub_080611AC();
        cmd->running = 0;
        break;
    default:
        sub_08009768(CMD_CARD);
        sub_080611AC();
        sub_08024134(player, 15, 0);
        cmd->running = 0;
        break;
    }
}
/* Move card word (arg2 | arg4 << 16) of the acting player, if sub_080080D8 accepts it,
 * from area 12 to area 14 (animation), then commit with sub_080096F4. */
void sub_0800FA04(void)
{
    struct CardLoc from, to;
    struct DuelCmd *cmd = &gUnk_020185C0;
    int player = gUnk_020185C0.cmd >> 15;
    u32 w = gUnk_020185C0.arg2 | (gUnk_020185C0.arg4 << 16);

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_080080D8(player, &w) != 0) {
            sub_08007558(CMD_CARD, &w);
            from.player = player & 1;
            from.area = 12;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player & 1;
            to.area = 14;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 1;
            sub_080242C4(CMD_CARD->id, &from, &to);
            gUnk_020185C0.step++;
            break;
        }
        sub_080611AC();
        cmd->running = 0;
        break;
    default:
        sub_080096F4(CMD_CARD);
        sub_080611AC();
        sub_08024134(player, 12, 0);
        cmd->running = 0;
        break;
    }
}
