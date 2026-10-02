#include "global.h"
#include "main.h"
#include "duel.h"

/*
 * Duel "script command" handlers (continued from code_0800EAA8): summon /
 * set / move card commands, the life-point counter animation, a few
 * banner animations, and the duel rule-flag command.  Each handler reads its
 * operands from the command block at 0x020185C0, runs a small per-frame state
 * machine in `step`, and clears the "command running" flag (bit 5 of byte
 * 0x020185C0+0x80D) when it is done.  See wiki/functions/code-08012c4c.md.
 */

/* Command block at 0x020185C0 (current duel command). */
struct DuelCmd {
    u16 cmd;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg2;           /* 0x002: usually a zone/slot index */
    u16 arg4;           /* 0x004 */
    u16 arg6;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u8 step:7;          /* 0x80A bits 0-6: multi-frame handler state */
    u8 unk80A_7:1;
    u8 unk80B;
    u32 unk80C_0:5;
    u32 timer:7;        /* 0x80C bits 5-11: frame counter inside a step */
    u32 unk80C_12:1;
    u32 running:1;      /* 0x80D bit 5: command in progress */
    u32 unk80C_14:2;
    u32 unk80C_16:16;
    u8 filler810[0x814 - 0x810];
    u8 card[4];         /* 0x814: struct DuelCard picked up by the command (u8 so the block stays 2-aligned) */
};

/* struct DuelZone / DuelPlayer / DuelState come from duel.h. */

/* Location of a card, passed to the card-move animations (sub_080242C4 etc.). */
struct CardPos {
    u32 player:1;       /* bit 0 */
    u32 area:4;         /* bits 1-4: 0 = field zone, 14/15 = ? */
    u32 slot:9;         /* bits 5-13 */
    u32 flag14:1;       /* bit 14 */
    u32 flag15:1;       /* bit 15 */
    u32 unk16:16;
};

/* Same layout as CardPos with 16-bit containers; the choice changes codegen (see wiki). */
struct CardLoc {
    u16 player:1;
    u16 area:4;
    u16 slot:9;
    u16 flag14:1;
    u16 flag15:1;
    u16 unk2;
};

extern struct DuelCmd gUnk_020185C0;

/* Duel screen / animation state at 0x0201CFB0 (fields used here). */
struct DuelScreen {
    u8 fast:1;              /* +0x000 bit 0: (hypothesis) fast-forward animations */
    u8 unk0_1:7;
    u8 filler1[0x808 - 0x1];
    u8 unk808_0:3;
    u8 busy:1;              /* +0x808 bit 3 */
    u8 unk808_4:4;
    u8 filler809[0x85C - 0x809];
    u32 unk85C;             /* +0x85C */
};
extern struct DuelScreen gUnk_0201CFB0;

/* gMain (0x03000040) layout comes from main.h. */
#define gMain gUnk_03000040
#define FAST_FORWARD() ((gMain.heldKeys & 2) || gUnk_0201CFB0.fast)

extern const u8 gUnk_08687B9C[];
extern const u8 gUnk_086887BC[];
extern const u16 gUnk_081A43E4[];
extern const u8 gUnk_0868247C[];
extern const u8 gUnk_0868267C[];
extern const u8 gUnk_08688BBC[];
extern const u16 gUnk_081A44FC[];
extern const u16 gUnk_081A4424[];

/* Banner graphics for sub_08013190, indexed by arg2 (0-4). */
struct BannerGfx {
    const u8 *pal;      /* 0x20 bytes for OBJ palette 15 */
    const u8 *gfx;      /* 0x800 bytes for OBJ VRAM 0x06016C80 */
    u16 bgm;            /* passed to sub_08077B70 / sub_0807E6B0 */
};
extern const struct BannerGfx gUnk_08198D68[];

#define CMD_ID(c) ((c)->cmd & 0xFFF)
#define CMD_PLAYER(c) ((c)->cmd >> 15)
#define CMD_CARD ((struct DuelCard *)gUnk_020185C0.card)
extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */
extern const u16 gUnk_08622AB4[];   /* card ID to card number */
extern const u8 gUnk_0869771C[];

#define ZONE(p, s) ((struct DuelZone *)((u8 *)gUnk_020192E4[0].zones + (s) * 0x94 + (p) * 0xD64))
/*
 * Local view of a zone's byte +0x08. duel.h declares it as `u8 unk8[2]`, but
 * sub_08013104 writes only bit 0, which the ROM compiles as a bitfield
 * read-modify-write (a plain byte store does not match).
 */
struct DuelZone08012C4C {
    u8 unk[8];
    u8 unk8_0:1;
    u8 unk8_1:7;
    u8 filler9[0x94 - 0x9];
};
#define CARD_TYPE(id) ((((const u32 *)0x08621DE0)[(id) & 0x7FF] & 0x1F00000) >> 20)
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])

void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2); /* AddSprite */
void sub_08077AEC(u16 id); /* PlaySE */
void sub_08007A4C(u32, u32, void *, u32, u32);
void sub_08007558(struct DuelCard *dst, struct DuelCard *src);
void sub_080240A8(u32, u32);
void sub_080242C4(u32 id, struct CardPos *from, struct CardPos *to);
void sub_08024380(struct CardPos *pos, const void *, u32, u32);
void sub_08060FD0(u32 player, u32 slot);
void sub_080611AC(void);
void sub_080098C0(u32, u32);
void sub_080752B0(void *dst, const void *src, u32 size);   /* CopyDoubleWords */
void sub_08076714(u32 yx, u16 shapeSize, u16 attr2, s32 affine);
void sub_08060934(u32 player, u32 lp);  /* (hypothesis) update the LP display */
void sub_08007418(struct DuelPlayer *players, u32 player, s32 amount);  /* subtract LP, clamp at 0 */
void sub_080241C4(void);
void sub_080137F8(u32 x, u32 y, s32 value, u32 palette);
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2); /* AddSprite */
void sub_08077B70(u16);
u32 sub_0807E6B0(u16);
void sub_08008CFC(u32, u32, u32);
void sub_080097F0(struct DuelCard *, u32);

/* Summon/set a card: arg2 = slot (bits 0-7) | face-down flag (bit 8), arg4/arg6 = card word. */
void sub_08012C4C(void)
{
    u32 player = CMD_PLAYER(&gUnk_020185C0);
    u8 slot = gUnk_020185C0.arg2;
    u32 faceDown = (gUnk_020185C0.arg2 >> 8) & 1;
    struct DuelCard card;
    struct CardPos pos;
    u16 cardNo;

    *(u32 *)&card = (gUnk_020185C0.arg6 << 16) | gUnk_020185C0.arg4;
    cardNo = CARD_NUMBER(card.id);
    if (cardNo >= 1920)
        if (cardNo <= 1999)
            faceDown = 1;
    switch (gUnk_020185C0.step) {
    case 0:
        sub_08007A4C(player, slot, &card, 1, faceDown);
        if (CARD_TYPE(card.id) > 20) {
            u8 *zoneBytes = (u8 *)gUnk_020192E4[0].zones + (slot * 0x94 + player * 0xD64);
            zoneBytes[0x8C] |= 2;
        }
        sub_08077AEC(0x10);
        pos.player = player;
        pos.area = 0;
        pos.slot = slot;
        pos.flag14 = 1;
        pos.flag15 = faceDown;
        sub_08024380(&pos, gUnk_0869771C, 0, 0);
        sub_08060FD0(player, slot);
        gUnk_020185C0.step++;
        break;
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}



/* Take a card off a field zone: arg2 = slot. Moves it into cmd.card and animates it to area 15. */
void sub_08012D7C(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER(&gUnk_020185C0);
    u16 slot = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        /* FAKEMATCH: the extra uses raise player's and slot's refs so global-alloc
         * takes player first (r7), then slot evicts the local in r6 and step the
         * local in r5, as in the ROM. */
        asm("" :: "r"(player), "r"(slot));
        sub_08007558(CMD_CARD, &gUnk_020192E4[player & 1].zones[slot].card);
        (gUnk_020192E4[player & 1].zones + slot)->card.id = 0;
        sub_08060FD0(player, slot);
        from.player = player;
        from.area = 0;
        from.slot = slot;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 15;
        to.slot = 0;
        to.flag14 = 0;
        to.flag15 = 0;
        ((void (*)(u32, struct CardLoc *, struct CardLoc *))sub_080242C4)(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_080097F0(CMD_CARD, slot);
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}

void sub_08012ED4(void)
{
    u32 player = CMD_PLAYER(&gUnk_020185C0);
    u16 slot = gUnk_020185C0.arg2;
    struct CardPos from, to;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        from.player = player;
        from.area = 15;
        from.slot = 0;
        from.flag14 = 0;
        from.flag15 = 0;
        to.player = player;
        to.area = 0;
        to.slot = slot;
        to.flag14 = 0;
        to.flag15 = 0;
        sub_080242C4(1, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_080098C0(player, slot);
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}

/* Flip a field card to area 14/15 (area 15 when arg4 != 0): arg2 = slot. */
void sub_08012FA4(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER(&gUnk_020185C0);
    u16 slot = gUnk_020185C0.arg2;
    u16 arg4 = gUnk_020185C0.arg4;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_08060FD0(player, slot);
        ZONE(player & 1, slot)->card.flag20 = 1;
        sub_08007558(CMD_CARD, &gUnk_020192E4[player & 1].zones[slot].card);
        sub_08008CFC(player, slot, arg4);
        from.player = player;
        from.area = 0;
        from.slot = slot;
        from.flag14 = ZONE(player & 1, slot)->flag6_0;
        from.flag15 = ZONE(player & 1, slot)->flag6_1;
        to.player = CMD_CARD->owner;
        to.area = arg4 ? 15 : 14;
        to.slot = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        ((void (*)(u32, struct CardLoc *, struct CardLoc *))sub_080242C4)(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}

void sub_08013104(void)
{
    ((struct DuelZone08012C4C *)ZONE(CMD_PLAYER(&gUnk_020185C0), gUnk_020185C0.arg2))->unk8_0 = gUnk_020185C0.arg4;
    gUnk_020185C0.running = 0;
}

void sub_08013154(void)
{
    ZONE(CMD_PLAYER(&gUnk_020185C0), gUnk_020185C0.arg2)->numLinks = 0;
    gUnk_020185C0.running = 0;
}

/* Banner arg2 (0-4): load its graphics, zoom it in over 0x40 frames, wait for its jingle, hold 0x1E frames. */
void sub_08013190(void)
{
    s32 step = gUnk_020185C0.step;
    s32 timer;

    switch (step) {
    case 0:
        if (gUnk_020185C0.arg2 <= 4) {
            sub_080752B0((void *)0x050003E0, gUnk_08198D68[gUnk_020185C0.arg2].pal, 0x20);
            sub_080752B0((void *)0x06016C80, gUnk_08198D68[gUnk_020185C0.arg2].gfx, 0x800);
            sub_08077B70(gUnk_08198D68[gUnk_020185C0.arg2].bgm);
            gUnk_0201CFB0.unk85C = step;
        }
        gUnk_020185C0.step++;
        gUnk_020185C0.timer = 0;
    case 1:
        timer = gUnk_020185C0.timer;
        if (timer < 0x40) {
            sub_08076714(0x00200058, 0xC0, 0xF364, ((0x900 - timer * 32) << 16) | (timer * 4));
            gUnk_020185C0.timer++;
        } else {
            sub_080761F0(0x00200058, 0xC0, 0xF364);
            gUnk_020185C0.step++;
        }
        break;
    case 2:
        sub_080761F0(0x00200058, 0xC0, 0xF364);
        if (sub_0807E6B0(gUnk_08198D68[gUnk_020185C0.arg2].bgm) == 0) {
            gUnk_020185C0.step++;
            gUnk_020185C0.timer = 0;
        }
        break;
    case 3:
        sub_080761F0(0x00200058, 0xC0, 0xF364);
        if (gUnk_020185C0.timer < 0x1E)
            gUnk_020185C0.timer++;
        else
            gUnk_020185C0.step++;
        break;
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
/* Life points of the acting player drop to 0: banner animation, then lifePoints = 0 and sub_08060934. */
void sub_08013390(void)
{
    u32 player = CMD_PLAYER(&gUnk_020185C0);
    u32 step = gUnk_020185C0.step;

    switch (step) {
    case 0:
        gUnk_0201CFB0.busy = 0;
        sub_080752B0((void *)0x050003E0, gUnk_08687B9C, 0x20);
        sub_080752B0((void *)0x06016C80, gUnk_086887BC, 0x400);
        gUnk_0201CFB0.unk85C = 0;
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 1:
        if (gUnk_020185C0.timer < 0x60) {
            sub_08076714(0x00300058, 0x40C0, 0xF364, gUnk_081A43E4[gUnk_020185C0.timer & 0x1F] << 16);
            gUnk_020185C0.timer++;
            if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x77)
                gUnk_020185C0.timer += 7;
            break;
        }
    default:
        gUnk_020192E4[player].lifePoints = 0;
        sub_08060934(player, 0);
        gUnk_020185C0.running = 0;
        break;
    }
}
/* Banner that slides in from the acting player's side, pulses for 0x40 frames, then slides back out. */
void sub_080134EC(void)
{
    u32 player = CMD_PLAYER(&gUnk_020185C0);
    s32 step = gUnk_020185C0.step;
    s32 timer;
    s32 y;

    switch (step) {
    case 0:
        gUnk_0201CFB0.busy = 0;
        sub_080752B0((void *)0x050003E0, gUnk_08687B9C, 0x20);
        sub_080752B0((void *)0x06016C80, gUnk_08688BBC, 0x400);
        gUnk_0201CFB0.unk85C = step;
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        sub_08077AEC(0x16);
        break;
    case 1:
        timer = gUnk_020185C0.timer;
        if (timer < 16) {
            if (player)
                y = gUnk_081A44FC[timer];
            else
                y = 0x80 - gUnk_081A44FC[timer];
            sub_080761F0((y << 16) | 0x58, 0x40C0, 0xF364);
            gUnk_020185C0.timer++;
            if (FAST_FORWARD() && gUnk_020185C0.timer <= 7)
                gUnk_020185C0.timer += 7;
            break;
        }
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step = step + 1;
        sub_08077AEC(0x16);
    case 2:
        if (gUnk_020185C0.timer < 0x40) {
            sub_08076714(0x00400058, 0x40C0, 0xF364, gUnk_081A4424[gUnk_020185C0.timer & 0xF] << 16);
            gUnk_020185C0.timer++;
            if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x37)
                gUnk_020185C0.timer += 7;
            break;
        }
        gUnk_020185C0.timer = 16;
        gUnk_020185C0.step++;
    case 3:
        timer = gUnk_020185C0.timer;
        if (timer != 0) {
            if (player)
                y = gUnk_081A44FC[timer - 1];
            else
                y = 0x80 - gUnk_081A44FC[timer - 1];
            sub_080761F0((y << 16) | 0x58, 0x40C0, 0xF364);
            gUnk_020185C0.timer--;
            if (FAST_FORWARD() && gUnk_020185C0.timer > 8)
                gUnk_020185C0.timer -= 7;
            break;
        }
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}

/* Draws a signed decimal number as 16-px digit sprites, right-aligned at x + 0x50. */
void sub_080137F8(u32 x, u32 y, s32 value, u32 palette)
{
    u16 tile = palette * 0x30 + 0xF364;
    u32 sign;

    if (value < 0) {
        sign = 11;
        value = -value;
    } else {
        sign = 10;
    }
    x += 0x50;
    if (value == 0) {
        sub_080761F0((y << 16) | x, 0x40, tile);
    } else {
        do {
            sub_080761F0(x | (y << 16), 0x40, value % 10 * 4 + tile);
            value /= 10;
            x -= 16;
        } while (value != 0);
        sub_080761F0((y << 16) | x, 0x40, tile + sign * 4);
    }
}

/*
 * Life-point change animation for the acting player: show the amount (arg2) next to the
 * LP counter, then count it into lifePoints in steps of 100 / 10 / 1 per frame.
 * gain != 0 adds, gain == 0 subtracts (sub_08007418).
 */
void sub_08013888(u16 gain)
{
    u32 player = CMD_PLAYER(&gUnk_020185C0);
    u32 x = player * 0x68 + 8;
    u32 y = 0x58 - player * 24;
    u16 se = gain ? 12 : 13;
    s32 step = gUnk_020185C0.step;
    struct DuelPlayer *lp;
    struct DuelPlayer *players;

    switch (step) {
    case 0:
        sub_080752B0((void *)0x050003E0, gUnk_0868247C, 0x20);
        sub_080752B0((void *)0x06016C80, gUnk_0868267C, 0xC00);
        gUnk_0201CFB0.unk85C = step;
        sub_080240A8(0, 0);
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_080137F8(x, y, gain ? gUnk_020185C0.arg2 : -gUnk_020185C0.arg2, gain != 0);
        if (gUnk_020185C0.timer < 0x5A) {
            gUnk_020185C0.timer++;
            if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x4F)
                gUnk_020185C0.timer += 7;
            break;
        }
        sub_08077AEC(se);
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 2:
        players = gUnk_020192E4;
        lp = &players[player];
        if (lp->lifePoints == 0) {
            gUnk_020185C0.step = step + 1;
            break;
        }
        sub_080137F8(x, y, gain ? gUnk_020185C0.arg2 : -gUnk_020185C0.arg2, gain != 0);
        if (gUnk_020185C0.arg2 >= 100) {
            gUnk_020185C0.arg2 -= 100;
            if (gain)
                lp->lifePoints += 100;
            else
                sub_08007418(players, player, 100);
            sub_08060934(player, gUnk_020192E4[player].lifePoints);
            gUnk_020185C0.timer++;
            if (gUnk_020185C0.timer > 10) {
                sub_08077AEC(se);
                gUnk_020185C0.timer = 0;
            }
            break;
        }
        if (gUnk_020185C0.arg2 >= 10) {
            gUnk_020185C0.arg2 -= 10;
            if (gain)
                lp->lifePoints += 10;
            else
                sub_08007418(players, player, 10);
            sub_08060934(player, gUnk_020192E4[player].lifePoints);
            gUnk_020185C0.timer++;
            if (gUnk_020185C0.timer > 10) {
                sub_08077AEC(se);
                gUnk_020185C0.timer = 0;
            }
            break;
        }
        if (gUnk_020185C0.arg2 != 0) {
            gUnk_020185C0.arg2 -= 1;
            if (gain)
                lp->lifePoints += 1;
            else
                sub_08007418(players, player, 1);
            sub_08060934(player, gUnk_020192E4[player].lifePoints);
            gUnk_020185C0.timer++;
            if (gUnk_020185C0.timer > 10) {
                sub_08077AEC(se);
                gUnk_020185C0.timer = 0;
            }
            break;
        }
    default:
        sub_080241C4();
        gUnk_020185C0.running = 0;
        break;
    }
}

/*
 * Local view of the duel flag word at gUnk_020192E0+0x1ACC. duel.h folds bits 6-7
 * (0x1ACC) and bits 0-4 (0x1ACD) into `unk1ACC_0` (bits 0..14) and `queueCount`
 * (bits 15..18), so this unit keeps its own finer bitfield split to match the ROM.
 */
struct DuelFlags08012C4C {
    u8 filler0[0x1ACC];
    u8 unk1ACC_0:6;
    u8 flag1ACC_6:1;
    u8 flag1ACC_7:1;
    u8 flag1ACD_0:1;
    u8 flag1ACD_1:1;
    u8 flag1ACD_2:1;
    u8 flag1ACD_3:1;
    u8 flag1ACD_4:1;
    u8 unk1ACD_5:3;
};
extern struct DuelFlags08012C4C gUnk_020192E0_flags asm("gUnk_020192E0");

/* Duel rule-flag commands 0x15-0x1B: store arg2 into one bit of the flag bytes at 0x020192E0+0x1ACC. */
void sub_08013BB0(void)
{
    switch (CMD_ID(&gUnk_020185C0)) {
    case 0x15:
        gUnk_020192E0_flags.flag1ACD_1 = gUnk_020185C0.arg2;
        break;
    case 0x16:
        gUnk_020192E0_flags.flag1ACD_2 = gUnk_020185C0.arg2;
        break;
    case 0x17:
        gUnk_020192E0_flags.flag1ACD_4 = gUnk_020185C0.arg2;
        break;
    case 0x18:
        gUnk_020192E0_flags.flag1ACD_3 = gUnk_020185C0.arg2;
        break;
    case 0x19:
        gUnk_020192E0_flags.flag1ACC_7 = gUnk_020185C0.arg2;
        break;
    case 0x1A:
        gUnk_020192E0_flags.flag1ACC_6 = gUnk_020185C0.arg2;
        break;
    case 0x1B:
        gUnk_020192E0_flags.flag1ACD_0 = gUnk_020185C0.arg2;
        break;
    }
    gUnk_020185C0.running = 0;
}
