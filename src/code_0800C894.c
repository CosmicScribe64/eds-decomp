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

#if 0 /* NONMATCHING: complex function with many register allocation and loop structure differences */
u32 sub_0800C8BC(s32 player, s32 slot)
{
    u32 off;
    u32 cardId;
    u32 var_sl;
    u32 var_r7;
    s32 i, j;

    off = (player & 1) * 0xD64;
    cardId = (*(u32 *)&gUnk_0201930C[slot * 0x94 + off] << 20) >> 20;
    if (cardId == 0) {
        return 0;
    }
    var_sl = 0;
    var_r7 = (gUnk_08621DE0[cardId & 0x7FF] & 0x01F00000) >> 20;
    if (slot <= 4 && (gUnk_0201930C[slot * 0x94 + off + 6] & 2)) {
        for (i = 0; i <= 4; i++) {
            u32 cid;
            cid = (*(u32 *)&gUnk_0201930C[i * 0x94 + off] << 20) >> 20;
            if (cid != 0 && gUnk_08622AB4[cid & 0x7FF] == 0x2FA
                && (s32)(*(u32 *)&gUnk_0201930C[i * 0x94 + off] << 14) < 0
                && (gUnk_0201930C[i * 0x94 + off + 6] & 2)
                && *(u32 *)&gUnk_0201930C[i * 0x94 + off + 4] > var_sl) {
                var_sl = *(u32 *)&gUnk_0201930C[i * 0x94 + off + 4];
                var_r7 = 0xA;
            }
            for (j = 0; j <= 1; j++) {
                u8 cid2;
                cid2 = (*(u32 *)&gUnk_020195F0[i * 0x94 + (j & 1) * 0xD64] << 20) >> 20;
                if (cid2 != 0 && gUnk_08622AB4[cid2 & 0x7FF] == 0x479
                    && (gUnk_020195F0[i * 0x94 + (j & 1) * 0xD64 + 6] & 2)
                    && !(gUnk_020195F0[i * 0x94 + (j & 1) * 0xD64 + 0x91] & 8)
                    && *(u32 *)&gUnk_020195F0[i * 0x94 + (j & 1) * 0xD64 + 4] > var_sl) {
                    var_sl = *(u32 *)&gUnk_020195F0[i * 0x94 + (j & 1) * 0xD64 + 4];
                    var_r7 = (*(u32 *)&gUnk_020195F0[i * 0x94 + (j & 1) * 0xD64 + 0x90] << 14) >> 27;
                }
            }
        }
        for (i = 0; i < *(u16 *)&gUnk_0201930C[slot * 0x94 + off + 0x8A]; i++) {
            u16 ref = *(u16 *)&gUnk_0201930C[slot * 0x94 + off + 0xA + i * 2];
            u32 tp = ref & 1;
            u32 ts = ref >> 8;
            s16 tcid;
            u8 *tz = &gUnk_0201930C[ts * 0x94 + tp * 0xD64];
            if (gUnk_0201930C[slot * 0x94 + off + 0x4A + i * 2] != 1) continue;
            tcid = (*(u32 *)tz << 20) >> 20;
            if (tcid != 0
                && !(tz[0x91] & 8)
                && sub_08008524(0, 0x601) == 0
                && sub_08008524(1, 0x601) == 0
                && !(gUnk_0201ADAD & 3)
                && gUnk_08622AB4[tcid & 0x7FF] == 0x60E
                && *(u32 *)&tz[4] > var_sl) {
                var_r7 = 1;
            }
        }
    }
    return var_r7;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800C894", sub_0800C8BC); /* 0x0800C8BC size 0x234 */
#if 0 /* NONMATCHING: loop structure, register allocation and stack frame differ */
u32 sub_0800CAF0(s32 player, s32 slot)
{
    u8 *zone;
    u32 cardId;
    u32 var_r7;
    s16 i;
    zone = &gUnk_0201930C[slot * 0x94 + (player & 1) * 0xD64];
    cardId = (*(u32 *)zone << 20) >> 20;
    if (cardId == 0) {
        return 0;
    }
    var_r7 = gUnk_08621DE0[cardId & 0x7FF] >> 29;
    if (slot > 4 || !(zone[6] & 2)) {
        return var_r7;
    }
    for (i = 0; i < *(u16 *)&zone[0x8A]; i++) {
        u16 ref = *(u16 *)&zone[0xA + i * 2];
        u32 s = ref >> 8;
        u32 p = ref & 1;
        u8 *tz = &gUnk_0201930C[s * 0x94 + p * 0xD64];
        u32 tcid;
        if (zone[0x4A + i * 2] != 1 || (tcid = (*(u32 *)tz << 20) >> 20, tcid == 0)) {
            continue;
        }
        if (tz[0x91] & 8) continue;
        if (sub_08008524(0, 0x601) != 0) continue;
        if (sub_08008524(1, 0x601) != 0) continue;
        if (gUnk_0201ADAD & 3) continue;
        if (gUnk_08622AB4[tcid & 0x7FF] != 0x5A8) continue;
        var_r7 = (*(u32 *)&tz[0x90] << 14) >> 27;
    }
    return var_r7;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800C894", sub_0800CAF0); /* 0x0800CAF0 size 0x128 */
#if 0 /* NONMATCHING: cardType computation hoisted outside inner loop; register allocation differs */
s32 sub_0800CC18(s32 player, s32 slot)
{
    u32 cardId;
    u16 p, s;
    s16 one;
    u32 d64;
    u32 masked;
    cardId = (*(u32 *)&gUnk_0201930C[slot * 0x94 + (player & 1) * 0xD64] << 20) >> 20;
    if (cardId == 0) {
        return 0;
    }
    one = 1;
    d64 = 0xD64;
    masked = (cardId & 0x7FF) << 16;
    for (p = 0; p <= 1; p++) {
        for (s = 0; s <= 4; s++) {
            u8 *zone = &gUnk_0201930C[s * 0x94 + (p & one) * d64];
            if ((*(u32 *)zone << 20) == 0) {
                continue;
            }
            if (!(zone[6] & 2)) {
                continue;
            }
            if (sub_0800AA40(p, s, *(u16 *)((u32)gUnk_08622AB4 + (masked >> 15))) != -1) {
                return 1;
            }
        }
    }
    return 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800C894", sub_0800CC18); /* 0x0800CC18 size 0xB4 */
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
#if 0 /* NONMATCHING: ROM-reviewed animation states; allocation, scheduling and shared tails differ */
s32 sub_080623AC(u32, s32, u16);
s32 sub_080623EC(u32, s32, u16);
extern u8 gUnk_08687FBC[];

void sub_0800CE28(void)
{
    u32 player = gCmd.hdr >> 15;
    int slot = gCmd.arg1;
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
        u16 x = sub_080623AC(player, 0, slot);
        s32 y = sub_080623EC(player, 0, slot);
        u32 packed = (u32)(x + 8) | ((u32)(y + 8) << 16);
        u32 a = gCmd.timer * 4;
        u32 b;
        s32 t;

        if (player) {
            b = a + 0x40;
            a = 0x1000000;
        } else
            b = 0x1000000;
        sub_08076714(packed, 0x40, 0x5200, a | b);
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
        s32 x, y, t;
        u32 packed, flags;

        dy -= sub_080623EC(player, 0, slot);
        dx -= sub_080623AC(player, 0, slot);
        /* ROM wraps the products as 32-bit words, then divides signed. */
        dx = (s32)((u32)dx * gCmd.timer);
        dy = (s32)((u32)dy * gCmd.timer);
        dx /= 32;
        dy /= 32;
        /* Preserve the second source-coordinate reads after interpolation. */
        x = dx + 8 + sub_080623AC(player, 0, slot);
        y = dy + 8 + sub_080623EC(player, 0, slot);
        packed = (u32)x | ((u32)y << 16);
        flags = (u32)gUnk_081A43E4[gCmd.timer] << 16;
        if (player)
            flags |= 0x40;
        sub_08076714(packed, 0x40, 0x5200, flags);
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
                    gCmd.timer += 3; /* This handler uses +3; D234 uses +7. */
            break;
        }
    }
    default:
        CMD_DONE();
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800C894", sub_0800CE28); /* 0x0800CE28 size 0x40C */
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
#if 0 /* NONMATCHING (48 diff lines): i=0 precedes the hoisted player offset (declaration order). The ROM keeps cmd in r9 and reloads the 0x08622AB4 table and 0x538 inside the loop, while the build hoists them. */
void sub_0800D398(void)
{
    struct DuelCmd *cmd = &gUnk_020185C0;
    s32 i = 0;
    u32 off = (cmd->hdr >> 15) * 0xD64;
    u8 *gBase = gUnk_0201930C;
    u8 *pb = gBase + off;
    s32 m9 = -9;
    s32 m3 = -3;

    do {
        u32 zOff = i * 0x94;
        u8 *f = pb + zOff + 0x8C;
        u8 *z;

        if (*f & 8)
            *f = (*f & m9) | 0x10;
        *f &= m3;
        z = zOff + off + gBase;
        if (*(u16 *)((u32)gUnk_08622AB4 + ((*(u32 *)z << 21) >> 20)) == 0x538)
            z[7] |= 0x20;
        i++;
    } while (i <= 4);
    cmd->running = 0;
}
#endif
INCLUDE_ASM("asm/nonmatching/code_0800C894", sub_0800D398); /* 0x0800D398 size 0xA4 */


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
