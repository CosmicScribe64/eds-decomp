#include "global.h"

/*
 * Duel "script command" handlers (continued from code_0800EAA8). The
 * dispatcher around 0x0801F3xx calls one of these per frame; each reads its
 * operands from the command block at 0x020185C0 and clears the "command
 * running" flag (bit 5 of byte 0x020185C0+0x80D) when it is done. Multi-frame
 * handlers keep their state in the 7-bit `step` counter at +0x80A.
 * See wiki/functions/code-0800fb10.md.
 */

/* A card instance word as stored in the duel state. */
struct DuelCard {
    u32 id:12;          /* bits 0-11: card ID; 0 = none */
    u32 owner:1;        /* bit 12: owning player */
    u32 unk13:5;
    u32 flag18:1;       /* bit 18 */
    u32 unk19:4;
    u32 flag23:1;       /* bit 23 */
    u32 flag24:1;       /* bit 24 */
    u32 unk25:3;
    u32 flag28:1;       /* bit 28 */
    u32 unk29:3;
};

/* Command block at 0x020185C0 (hypothesis: current duel command). */
struct DuelCmd {
    u16 cmd;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg2;           /* 0x002 */
    u16 arg4;           /* 0x004 */
    u16 arg6;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u16 step:7;         /* 0x80A bits 0-6: multi-frame handler state */
    u16 slot:7;         /* 0x80A bits 7-13: a zone chosen by a handler (sub_08010538) */
    u16 unk80A_14:2;
    u8 unk80C;
    u8 unk80D_0:5;
    u8 running:1;       /* 0x80D bit 5: command in progress */
    u8 unk80D_6:2;
    u8 filler80E[0x814 - 0x80E];
    struct DuelCard card;   /* 0x814: card being moved */
};

/* Per-player duel state (0xD64 bytes), two of them at 0x020192E4. */
struct DuelPlayer {
    u8 unk0[2];
    u8 numList684;          /* +0x002 */
    u8 unk3;
    u8 numList904;          /* +0x004 */
    u8 unk5[6];
    u8 unkB_0:3;            /* +0x00B bits 0-2 */
    u8 unkB_3:5;
    u8 fillerC[0x684 - 0xC];
    struct DuelCard list684[160];   /* +0x684 */
    struct DuelCard list904[160];   /* +0x904 */
    u8 fillerB84[0xD64 - 0xB84];
};

/* Card location descriptor passed to the card-move animation sub_080242C4. */
struct CardLoc {
    u16 player:1;       /* bit 0 */
    u16 area:4;         /* bits 1-4: 11 hand, 13, 14, 15 (hypothesis: graveyard-like areas) */
    u16 index:9;        /* bits 5-13 */
    u16 flag14:1;       /* bit 14 */
    u16 flag15:1;       /* bit 15 */
    u16 unk2;
};

extern struct DuelCmd gUnk_020185C0;
extern struct DuelPlayer gUnk_020192E4[2];
extern const u32 gUnk_08621DE0[];   /* card stats, indexed by card ID */
extern const u16 gUnk_08622AB4[];   /* card ID to card number */

/* Acting player of the current command (bit 15; read as a plain u16 shift). */
#define CMD_PLAYER() (gUnk_020185C0.cmd >> 15)
#define CARD_OWNER(w) (((w) << 19) >> 31)

void sub_080096F4(void *card);
void sub_08007C58(u32 player, struct DuelCard *card);
void sub_08007CE0(u32 player, struct DuelCard *card);
void sub_080080D8(u32 player, struct DuelCard *card);
u16 sub_08009B48(int player, struct DuelCard *card);
void sub_080611AC(void);
void sub_0802408C(u32);
void sub_08009D8C(u32 player, struct DuelCard *card);
void sub_080242C4(u32 id, struct CardLoc *from, struct CardLoc *to);
void sub_080240A8(u32, u32);
void sub_08024134(u32, u32, u32);
void sub_08009768(void *card);
u32 sub_08009C08(u32 player, u32 idx, struct DuelCard *card);
void sub_08009BA8(u32 player, u32 idx);
void sub_08009EAC(u32 player, struct DuelCard *card);
void sub_0800743C(struct DuelCard *card);

/* Card of the command block through a pointer, so id reads load the whole word. */
#define CMD_CARD ((struct DuelCard *)((u8 *)&gUnk_020185C0 + 0x814))

void sub_0800FB10(void)
{
    u32 card = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    sub_08007C58(CARD_OWNER(card), (struct DuelCard *)&card);
    gUnk_020185C0.running = 0;
}

void sub_0800FB48(void)
{
    u32 card = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    sub_08007CE0(CARD_OWNER(card), (struct DuelCard *)&card);
    gUnk_020185C0.running = 0;
}

void sub_0800FB80(void)
{
    struct DuelCmd *cmd = &gUnk_020185C0;
    gUnk_020192E4[cmd->cmd >> 15].unkB_0 = cmd->arg2;
    cmd->running = 0;
}
void sub_0800FBC8(void)
{
    u32 player = CMD_PLAYER();
    u32 card = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    sub_080080D8(player, (struct DuelCard *)&card);
    gUnk_020185C0.running = 0;
}

#define CARD_STATS(id) (((const u32 *)0x08621DE0)[(id) & 0x7FF])
#define CARD_NUMBER(id) (((const u16 *)0x08622AB4)[(id) & 0x7FF])
#define CARD_TYPE(id) ((CARD_STATS(id) & 0x1F00000) >> 20)
#define CARD_KIND(id) ((CARD_STATS(id) & 0xC0000) >> 18)

/* Card category (same inline as in code_08009A68): 3/1 for card numbers 1910/1911-1912,
 * 7/8/9 for types 22/21/23, else the monster kind (stats bits 18-19). */
static inline int GetCardSubtype(u16 id)
{
    switch (CARD_NUMBER(id)) {
    case 1910:
        return 3;
    case 1911:
    case 1912:
        return 1;
    }
    switch ((u8)CARD_TYPE(id)) {
    case 22:
        return 7;
    case 21:
        return 8;
    case 23:
        return 9;
    default:
        return CARD_KIND(id);
    }
}

/*
 * Returns card word arg2 | arg4 << 16 of the acting player from area 14 to
 * the hand, or to area 12 when its category is 2 (a fusion monster; hypothesis:
 * area 12 is the fusion deck). Then calls sub_0800743C, sub_08009EAC and
 * sub_08024134(player, 11, 0).
 */
void sub_0800FC00(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u32 card = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08009B48(player, (struct DuelCard *)&card);
        from.player = player;
        from.area = 14;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 11;
        to.index = gUnk_020192E4[player & 1].numList684;
        to.flag14 = 0;
        to.flag15 = 0;
        if (GetCardSubtype(CMD_CARD->id) == 2) {
            to.area = 12;
            to.index = 0;
        }
        sub_080242C4((&gUnk_020185C0.card)->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_0800743C((struct DuelCard *)&card);
        sub_08009EAC(player, (struct DuelCard *)&card);
        sub_08024134(player, 11, 0);
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Takes card arg2 of the acting player's area 14 list (sub_08009C08; stops if
 * it returns 0), animates it to area 13, then calls sub_0800743C, sub_08007C58 and
 * sub_08024134(player, 13, 0).
 */
void sub_0800FDD0(void)
{
    struct DuelCmd *cmd = &gUnk_020185C0;
    u32 idx;
    u32 player = cmd->cmd >> 15;
    struct CardLoc from, to;

    /* This empty compiler barrier keeps the player load before arg2, as in
     * the ROM. It emits no instructions and leaves cmd and player unchanged. */
    asm volatile ("" : "+r"(cmd) : "r"(player));
    idx = cmd->arg2;

    switch (cmd->step) {
    case 0:
        sub_080240A8(player, 11);
        cmd->step++;
        break;
    case 1:
        if (sub_08009C08(player, idx, ((struct DuelCard *)((u8 *)cmd + 0x814)))) {
            sub_08009B48(player, ((struct DuelCard *)((u8 *)cmd + 0x814)));
            from.player = player;
            from.area = 14;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player;
            to.area = 13;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 0;
            sub_080242C4(((struct DuelCard *)((u8 *)cmd + 0x814))->id, &from, &to);
            cmd->step++;
        } else {
            cmd->running = 0;
        }
        break;
    default:
        sub_0800743C(((struct DuelCard *)((u8 *)cmd + 0x814)));
        sub_08007C58(player, ((struct DuelCard *)((u8 *)cmd + 0x814)));
        sub_080611AC();
        sub_08024134(player, 13, 0);
        cmd->running = 0;
        break;
    }
}
/* Same as sub_0800FDD0 but places the card with sub_08007CE0. */
void sub_0800FF04(void)
{
    struct DuelCmd *cmd = &gUnk_020185C0;
    u32 idx;
    u32 player = cmd->cmd >> 15;
    struct CardLoc from, to;

    /* This empty compiler barrier keeps the player load before arg2, as in
     * the ROM. It emits no instructions and leaves cmd and player unchanged. */
    asm volatile ("" : "+r"(cmd) : "r"(player));
    idx = cmd->arg2;

    switch (cmd->step) {
    case 0:
        sub_080240A8(player, 11);
        cmd->step++;
        break;
    case 1:
        if (sub_08009C08(player, idx, ((struct DuelCard *)((u8 *)cmd + 0x814)))) {
            sub_08009B48(player, ((struct DuelCard *)((u8 *)cmd + 0x814)));
            from.player = player;
            from.area = 14;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player;
            to.area = 13;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 0;
            sub_080242C4(((struct DuelCard *)((u8 *)cmd + 0x814))->id, &from, &to);
            cmd->step++;
        } else {
            cmd->running = 0;
        }
        break;
    default:
        sub_0800743C(((struct DuelCard *)((u8 *)cmd + 0x814)));
        sub_08007CE0(player, ((struct DuelCard *)((u8 *)cmd + 0x814)));
        sub_080611AC();
        sub_08024134(player, 13, 0);
        cmd->running = 0;
        break;
    }
}
/*
 * Moves card word arg2 | arg4 << 16 of the acting player from area 14 to
 * area 15 (sub_08009B48, animation), then calls sub_08009768 and sub_08024134(player, 15, 0).
 */
void sub_08010038(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u32 card = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08009B48(player, (struct DuelCard *)&card);
        from.player = player;
        from.area = 14;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = player;
        to.area = 15;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_08009768(&card);
        sub_080611AC();
        sub_08024134(player, 15, 0);
        gUnk_020185C0.running = 0;
        break;
    }
}
void sub_08010124(void)
{
    u32 card = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    sub_08009B48(CARD_OWNER(card), (struct DuelCard *)&card);
    sub_080611AC();
    gUnk_020185C0.running = 0;
}

void sub_08009AD0(u32 player, u32 idx, struct DuelCard *card);

/*
 * Loop: while the acting player's list904 (area 14) is not empty, take entry
 * 0 (sub_08009AD0), animate it to area 13 and commit it (sub_0800743C +
 * sub_08007C58); then sub_08024134(player, 13, 0).
 */
void sub_08010160(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (gUnk_020192E4[player & 1].numList904 != 0) {
            sub_08009AD0(player, 0, CMD_CARD);
            from.player = player;
            from.area = 14;
            from.index = 0;
            from.flag14 = 0;
            from.flag15 = 1;
            to.player = player;
            to.area = 13;
            to.index = 0;
            to.flag14 = 0;
            to.flag15 = 0;
            sub_080242C4(CMD_CARD->id, &from, &to);
            gUnk_020185C0.step++;
        } else {
            gUnk_020185C0.step = 10;
        }
        break;
    case 2:
        sub_0800743C(CMD_CARD);
        sub_08007C58(player, CMD_CARD);
        sub_080611AC();
        gUnk_020185C0.step = 1;
        break;
    default:
        sub_08024134(player, 13, 0);
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Takes card arg2 of the opponent's area 14 list (sub_08009C08 / sub_08009BA8)
 * and animates it into the acting player's hand, then sets card bit 18 and
 * commits with sub_08009EAC.
 */
void sub_080102B0(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u32 other = 1 - player;
    u16 idx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(other, 14);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08009C08(other, idx, CMD_CARD);
        sub_08009BA8(other, idx);
        from.player = other;
        from.area = 14;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = player;
        to.area = 11;
        to.index = gUnk_020192E4[player & 1].numList684;
        to.flag14 = 0;
        to.flag15 = 0;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        CMD_CARD->flag18 = 1;
        sub_08009EAC(player, CMD_CARD);
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Moves card word arg2 | arg4 << 16 from its owner's area 15 to area 14
 * (hypothesis: from the graveyard to removed from play): sub_08009D8C, animation,
 * then sub_080096F4.
 */
void sub_080103DC(void)
{
    struct CardLoc from, to;
    u32 card = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    u32 owner = CARD_OWNER(card);

    switch (gUnk_020185C0.step) {
    case 0:
        sub_0802408C(0x50);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08009D8C(owner, (struct DuelCard *)&card);
        from.player = owner;
        from.area = 15;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = owner;
        to.area = 14;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_080096F4(&card);
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
void sub_080104B8(void)
{
    u32 w = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    sub_080096F4((void *)&w);
    gUnk_020185C0.running = 0;
}
void sub_080104EC(void)
{
    struct DuelCmd *cmd = &gUnk_020185C0;
    gUnk_020192E4[cmd->cmd >> 15].list904[cmd->arg2].flag24 = 0;
    cmd->running = 0;
}

void sub_08007B24(u32 player, u32 slot, struct DuelCard *card, u32 a3);
void sub_0800935C(u32 a, u32 b, u32 c);
u32 sub_08008C6C(u32 player);

/*
 * Takes entry arg2 of the acting player's area 14 list (sub_08009AD0) and
 * places it face-up (flag24 cleared) into a spell/trap zone of the opponent
 * chosen by sub_08008C6C (zone - 5 passed to sub_08007B24).
 */
void sub_08010538(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u16 idx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08009AD0(player, idx, CMD_CARD);
        gUnk_020185C0.card.flag24 = 0;
        gUnk_020185C0.slot = sub_08008C6C(1 - player);
        from.player = player;
        from.area = 14;
        from.index = 0;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = 1 - player;
        to.area = 5;
        to.index = gUnk_020185C0.slot;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    default:
        sub_08007B24(1 - player, gUnk_020185C0.slot - 5, CMD_CARD, 1);
        sub_0800935C((u8)(1 - player) | (gUnk_020185C0.card.unk25 << 8),
                     (u8)(1 - player) | (gUnk_020185C0.slot << 8), 10);
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
void sub_080106BC(void)
{
    struct DuelCmd *cmd = &gUnk_020185C0;
    gUnk_020192E4[cmd->cmd >> 15].list904[cmd->arg2].flag28 = 0;
    cmd->running = 0;
}
void sub_08010708(void)
{
    u32 *entry;
    u32 card = (gUnk_020185C0.arg4 << 16) | gUnk_020185C0.arg2;
    int i = 0;
    s16 player = CMD_PLAYER();

    for (; i < gUnk_020192E4[player].numList904; i++) {
        entry = (u32 *)&gUnk_020192E4[player].list904[i];
        if (*entry == card) {
            gUnk_020192E4[player].list904[i].flag23 = 1;
            gUnk_020185C0.running = 0;
            return;
        }
    }
    gUnk_020185C0.running = 0;
} /* 0x08010708 size 0x8C */

void sub_08007558(struct DuelCard *dst, struct DuelCard *src);
void sub_0800A0A8(u32 player);

/*
 * Moves hand card arg2 of the acting player to its owner's area 14 (like
 * sub_08011498 without the flag20 update); committed with sub_080096F4.
 * arg4 != 0 also calls sub_0800A0A8 first.
 */
void sub_08010794(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIdx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08007558(CMD_CARD, gUnk_020192E4[player & 1].list684 + handIdx);
        (gUnk_020192E4[player & 1].list684 + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 14;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        if (gUnk_020185C0.arg4 != 0)
            sub_0800A0A8(player);
        sub_080096F4(CMD_CARD);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/* Like sub_08010794 but to the owner's area 15, committed with sub_08009768. */
void sub_080108FC(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIdx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08007558(CMD_CARD, gUnk_020192E4[player & 1].list684 + handIdx);
        (gUnk_020192E4[player & 1].list684 + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 15;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        if (gUnk_020185C0.arg4 != 0)
            sub_0800A0A8(player);
        sub_08009768(CMD_CARD);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Hand card arg2 of the acting player to its owner's area 13, then
 * sub_08007C58 (arg4 != 0) or sub_08007CE0 to place it.
 */
void sub_08010A5C(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER();
    u16 handIdx = gUnk_020185C0.arg2;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(player, 11);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_08007558(CMD_CARD, gUnk_020192E4[player & 1].list684 + handIdx);
        (gUnk_020192E4[player & 1].list684 + handIdx)->id = 0;
        from.player = player;
        from.area = 11;
        from.index = handIdx;
        from.flag14 = 0;
        from.flag15 = 1;
        to.player = CMD_CARD->owner;
        to.area = 13;
        to.index = 0;
        to.flag14 = 0;
        to.flag15 = 1;
        sub_080242C4(CMD_CARD->id, &from, &to);
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_0800A0A8(player);
        if (gUnk_020185C0.arg4 != 0)
            sub_08007C58(player, CMD_CARD);
        else
            sub_08007CE0(player, CMD_CARD);
    default:
        sub_080611AC();
        gUnk_020185C0.running = 0;
        break;
    }
}
