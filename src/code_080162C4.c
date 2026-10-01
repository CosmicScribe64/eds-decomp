#include "global.h"
#include "main.h"
#include "duel.h"
#include "duel_ui.h"

/*
 * Duel command handlers (continued from code_0800EAA8): each one runs from the
 * command dispatcher, reads its operands from the command block at 0x020185C0,
 * and clears the "running" flag (bit 5 of byte +0x80D) when it is done.
 * Multi-frame handlers keep their state in the 7-bit `step` (+0x80A) and the
 * 7-bit `timer` (+0x80C bits 5-11).  See wiki/functions/code-080162c4.md.
 */

/* gMain (0x03000040) comes from main.h. */
#define gMain gUnk_03000040

/* Duel command block gUnk_020185C0 comes from duel_ui.h. */

/* Per-player duel state (gUnk_020192E4) and the duel state (gUnk_020192E0) come from duel.h. */
/* gUnk_020192E0 addressed through the gUnk_020192E4 symbol (lets CSE share the base, see sub_080162C4). */
#define DUEL_FROM_PLAYERS ((struct DuelState *)((u8 *)gUnk_020192E4 - 4))

/* Duel screen / animation state gUnk_0201CFB0 comes from duel_ui.h. */

/* Message box request block at 0x02017A30 (fields used here). */
struct DuelMsg {
    u8 filler0[6];
    u16 arg;                /* +0x006 */
};
extern struct DuelMsg gUnk_02017A30;

/*
 * Local view for sub_08016488 only. It touches gUnk_020192E0+0x1ACC as a u8 bitfield
 * (unk1ACC_0:4) and the ROM uses ldrb/strb there. duel.h models that word as
 * `u32 unk1ACC_0:15`, which makes agbcc emit ldrh/strh, so the unit keeps its own split.
 */
struct DuelStateUnk1ACCView {
    u32 unk0;                       /* +0x0000 */
    struct DuelPlayer players[2];   /* +0x0004 */
    u8 unk1ACC_0:4;                 /* +0x1ACC (unit-local split; canonical is u32:15) */
    u8 unk1ACC_4:4;
};
extern struct DuelStateUnk1ACCView gUnk_020192E0_lo asm("gUnk_020192E0");

extern u8 gUnk_02017A40[];
extern const u8 gUnk_08687B9C[];
extern const u8 gUnk_08687BBC[];
extern const u16 gUnk_081A4444[];
extern const u8 gUnk_0867F01C[];
extern const u8 gUnk_0867F03C[];
extern const u8 gUnk_0867F63C[];
extern const u8 gUnk_08688FBC[];
extern const u8 gUnk_08688FD8[];
extern const u16 gUnk_081A44FC[];   /* slide offsets, 16 entries */
extern const u16 gUnk_081A4424[];   /* pulse scale, 16 entries */
extern const u32 gUnk_08081728[];   /* slide-in x offsets, 16 entries */

#define REG_BLDCNT (*(vu16 *)0x04000050)
#define REG_BLDALPHA (*(vu16 *)0x04000052)
#define BLDALPHA(eva, evb) ((u8)(eva) | ((u8)(evb) << 8))

void sub_080752B0(void *dst, const void *src, u32 size);   /* CopyDoubleWords */
void sub_08075278(void *dst, u32 size);                    /* MemClear16 */
void sub_08077AEC(u16 se);                                 /* PlaySE */
void sub_080761F0(u32 yx, u16 shapeSize, u16 attr2);       /* AddSprite */
void sub_0807625C(u32 yx, u16 shapeSize, u16 attr2);
void sub_08076714(u32 yx, u16 shapeSize, u16 attr2, s32 affine);
void sub_08024134(u32 player, u32 a, u32 b);
void sub_080240A8(u32 player, u32 a);
void sub_0801E998(u16 msg, u32 a);
u32 sub_0801EAD8(void);
void sub_0801EBA8(void);
void sub_08060964(u32 a, u32 b);
u32 sub_08060CEC(u32 a);
u32 sub_08060D64(u32 a);
void sub_080609C4(void);
u32 sub_08060B2C(void);
u32 sub_08060B4C(void);
void sub_0806044C(u16 a);

#define FAST_FORWARD() ((gMain.heldKeys & 2) || gUnk_0201CFB0.fast)

/*
 * Restart the duel. Save both players' list7C4/listA44 (and their counts) into the
 * command block, clear the whole duel state and gUnk_02017A40, restore the lists,
 * and reset both players to 8000 LP.
 */
void sub_080162C4(void)
{
    int i;

    for (i = 0; i < 2; i++) {
        gUnk_020185C0.savedCount3[i] = gUnk_020192E4[i & 1].deckCount;
        sub_080752B0(gUnk_020185C0.savedList7C4[i], gUnk_020192E4[i & 1].deck, 0x140);
        gUnk_020185C0.savedCount5[i] = gUnk_020192E4[i & 1].fusionCount;
        sub_080752B0(gUnk_020185C0.savedListA44[i], gUnk_020192E4[i & 1].fusionDeck, 0x140);
    }
    sub_08075278(gUnk_02017A40, 0x56C);
    sub_08075278(gUnk_020192E4, 0x1B0C);
    for (i = 0; i < 2; i++) {
        gUnk_020192E4[i & 1].deckCount = gUnk_020185C0.savedCount3[i];
        sub_080752B0(gUnk_020192E4[i & 1].deck, gUnk_020185C0.savedList7C4[i], 0x140);
        gUnk_020192E4[i & 1].fusionCount = gUnk_020185C0.savedCount5[i];
        sub_080752B0(gUnk_020192E4[i & 1].fusionDeck, gUnk_020185C0.savedListA44[i], 0x140);
    }
    gUnk_020192E4[0].lifePoints = 8000;
    gUnk_020192E4[1].lifePoints = 8000;
    DUEL_FROM_PLAYERS->phase1B12 = 7;
    gUnk_020185C0.running = 0;
}

void sub_08016424(void)
{
    if (!gUnk_0201CFB0.flag0_2)
        sub_080609C4();
    else if (sub_08060B2C())
        gUnk_020185C0.running = 0;
}

void sub_08016460(void)
{
    if (sub_08060B4C())
        gUnk_020185C0.running = 0;
}

void sub_08016488(void)
{
    sub_0806044C(gUnk_020185C0.arg2);
    gUnk_020192E0_lo.unk1ACC_0 = gUnk_020185C0.arg2;
    gUnk_020185C0.running = 0;
}

/*
 * Duel-start banner: slide the two halves in (gUnk_08081728 offsets), run
 * sub_08060CEC / sub_08060D64 (4x speed when fast-forwarding), slide them out,
 * then set phase1B12 = 3 and notify sub_08060964 / sub_08024134.
 */
void sub_080164D0(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        sub_080752B0((void *)0x050003E0, gUnk_0867F01C, 0x20);
        sub_080752B0((void *)0x06016C80, gUnk_0867F63C, 0x200);
        sub_080240A8(0, 0);
        gUnk_020185C0.timer = 16;
        gUnk_020185C0.step++;
        break;
    case 1:
        if (gUnk_020185C0.timer > 0) {
            gUnk_020185C0.timer--;
            if (FAST_FORWARD() && gUnk_020185C0.timer > 4)
                gUnk_020185C0.timer -= 3;
            sub_080761F0(gUnk_08081728[gUnk_020185C0.timer] | 0x400000, 0x4080, 0xF364);
            sub_080761F0((0xD0 - gUnk_08081728[gUnk_020185C0.timer]) | 0x400000, 0x4080, 0xF36C);
            break;
        }
        sub_08077AEC(0x1D);
        gUnk_020185C0.step++;
    case 2:
        sub_080761F0(0x00400058, 0x4080, 0xF364);
        sub_080761F0(0x00400078, 0x4080, 0xF36C);
        if (sub_08060CEC(FAST_FORWARD() ? 4 : 1))
            gUnk_020185C0.step++;
        break;
    case 3:
        sub_080761F0(0x00400058, 0x4080, 0xF364);
        sub_080761F0(0x00400078, 0x4080, 0xF36C);
        if (sub_08060D64(FAST_FORWARD() ? 4 : 1)) {
            gUnk_020185C0.timer = 0;
            gUnk_020185C0.step++;
        }
        break;
    case 4:
        if (gUnk_020185C0.timer < 16) {
            sub_080761F0(gUnk_08081728[gUnk_020185C0.timer] | 0x400000, 0x4080, 0xF364);
            sub_080761F0((0xD0 - gUnk_08081728[gUnk_020185C0.timer]) | 0x400000, 0x4080, 0xF36C);
            gUnk_020185C0.timer++;
            if (FAST_FORWARD() && gUnk_020185C0.timer <= 0xB)
                gUnk_020185C0.timer += 3;
            break;
        }
        gUnk_020185C0.step++;
    default:
        gUnk_020192E0.phase1B12 = 3;
        sub_08060964(gUnk_020192E0.linkSkip, gUnk_020192E0.phase1B12);
        sub_08024134(gUnk_020192E0.linkSkip, 0, 0);
        gUnk_020185C0.running = 0;
        break;
    }
}
/* +0x80C view; the larger container preserves the original halfword RMW. */
struct BannerTimer {
    u32 reserved : 5;
    u32 timer : 7;
    u32 remainder : 20;
    u32 trailing;
};
void sub_08016848(u32 kind)
{
    u32 step = gUnk_020185C0.step;
    int blend;

    switch (step) {
    case 0:
        sub_080752B0((void *)0x050003E0, gUnk_0867F01C, 0x20);
        sub_080752B0((void *)0x06016C80, gUnk_0867F03C + kind * 0x200, 0x200);
        {
            u8 *base = (u8 *)&gUnk_020185C0;
            /* FAKEMATCH: preserve the initialized offset and RMW pointer roles. */
            register u32 offset asm("r3") = 0x80C;
            register struct BannerTimer *timer asm("r1");
            asm("" : "+r"(offset));
            timer = (struct BannerTimer *)base;
            timer = (struct BannerTimer *)((u32)timer + offset);
            timer->timer = 0;
        }
        gUnk_020185C0.step++;
        break;
    case 1:
        if (gUnk_020185C0.timer < 0x60) {
            blend = 0;
            if (gUnk_020185C0.timer < 16) {
                REG_BLDCNT = 0xF40;
                REG_BLDALPHA = BLDALPHA(gUnk_020185C0.timer, 16 - gUnk_020185C0.timer);
                blend = 1;
            }
            if (gUnk_020185C0.timer >= 0x50) {
                REG_BLDCNT = 0xF40;
                REG_BLDALPHA = BLDALPHA(0x60 - gUnk_020185C0.timer, gUnk_020185C0.timer - 0x50);
                blend = 1;
            }
            if (!blend) {
                REG_BLDCNT = 0;
                REG_BLDALPHA = 0;
            }
            sub_0807625C(0x00400058, 0x4080, 0xF364);
            sub_0807625C(0x00400078, 0x4080, 0xF36C);
            if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x57)
                gUnk_020185C0.timer += 7;
            gUnk_020185C0.timer++;
            break;
        }
        gUnk_020185C0.step = step + 1;
    default:
        gUnk_020192E0.phase1B12 = kind;
        sub_08060964(gUnk_020192E0.linkSkip, gUnk_020192E0.phase1B12);
        gUnk_020185C0.running = 0;
        break;
    }
}

/* Banner that slides in from the acting player's side, pulses for 0x40 frames, then slides back out (cf. sub_080134EC). */
void sub_08016A18(void)
{
    u32 player = gUnk_020185C0.cmd >> 15;
    s32 step = gUnk_020185C0.step;
    s32 timer;
    s32 y;

    switch (step) {
    case 0:
        gUnk_0201CFB0.busy = 0;
        sub_080752B0((void *)0x050003E0, gUnk_08688FBC, 0x20);
        sub_080752B0((void *)0x06016C80, gUnk_08688FD8, 0x400);
        gUnk_0201CFB0.cb85C = (void (*)(void))step;
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
            if (FAST_FORWARD() && gUnk_020185C0.timer <= 0xB)
                gUnk_020185C0.timer += 3;
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
            if (FAST_FORWARD() && gUnk_020185C0.timer > 4)
                gUnk_020185C0.timer -= 3;
            break;
        }
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}
void sub_08016D24(void)
{
    u32 player = gUnk_020185C0.cmd >> 15;
    u32 arg2 = gUnk_020185C0.arg2;
    u32 lo = (u8)gUnk_020185C0.arg4;
    u32 hi = gUnk_020185C0.arg4 >> 8;
    u32 step = gUnk_020185C0.step;

    switch (step) {
    case 0:
        gUnk_0201CFB0.busy = 1;
        sub_08024134(arg2, lo, hi);
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 1:
        gUnk_0201CFB0.busy = 0;
        if (gUnk_020185C0.timer < 64) {
            sub_08076714((gUnk_0201CFB0.cursorX + 8) | ((gUnk_0201CFB0.cursorY - gUnk_0201CFB0.scroll) << 16),
                         0x80, 0, (gUnk_081A4444[gUnk_020185C0.timer & 7] << 16) | (player << 6));
            gUnk_020185C0.timer++;
            if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x37)
                gUnk_020185C0.timer += 7;
        } else {
            gUnk_020185C0.step = step + 1;
        }
        break;
    default:
        gUnk_020185C0.running = 0;
        break;
    }
}

void sub_08016E74(void)
{
    u32 player = gUnk_020185C0.cmd >> 15;
    u32 arg2 = gUnk_020185C0.arg2;
    u32 arg4 = gUnk_020185C0.arg4;
    gUnk_0201CFB0.busy = 1;
    sub_08024134(player, arg2, arg4);
    gUnk_020185C0.running = 0;
}

void sub_08016EB8(void)
{
    u32 scale;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_08077AEC(0xB);
        sub_080752B0((void *)0x050003E0, gUnk_08687B9C, 0x20);
        sub_080752B0((void *)0x06016C80, gUnk_08687BBC, 0x400);
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 1:
        if (gUnk_020185C0.timer != 0 && gUnk_020185C0.timer <= 15) {
            scale = gUnk_020185C0.timer << 20;
            sub_08076714(0x00300058, 0x40C0, 0xF364, scale + 0x100000);
        }
        if (gUnk_020185C0.timer >= 16 && gUnk_020185C0.timer <= 96)
            sub_080761F0(0x00300058, 0x40C0, 0xF364);
        scale = gUnk_020185C0.timer;
        if (scale > 96 && scale < 128) {
            scale -= 0x60;
            sub_08076714(0x00300058, 0x40C0, 0xF364, scale << 24);
            if (gUnk_020185C0.timer == 0x7F)
                sub_0801EBA8();
        }
        gUnk_020185C0.timer++;
        if (gUnk_020185C0.timer == 0)
            gUnk_020185C0.step++;
        break;
    default:
        gUnk_020192E0.flag1B12_0 = 1;
        gUnk_020185C0.running = 0;
        break;
    }
}

void sub_08017024(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        sub_0801E998(1, 0);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_0801EAD8())
            gUnk_020185C0.running = 0;
        break;
    }
}

void sub_08017084(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        sub_0801E998(5, 0);
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_0801EAD8())
            gUnk_020185C0.running = 0;
        break;
    }
}

void sub_080170E4(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        sub_0801E998(0, 0);
        gUnk_02017A30.arg = (gUnk_020185C0.arg2 << 15) | 0x180 | (u8)gUnk_020185C0.arg4;
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_0801EAD8())
            gUnk_020185C0.running = 0;
        break;
    }
}

void sub_0801715C(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        sub_0801E998(0, 0);
        gUnk_02017A30.arg = 0x380 | (u8)gUnk_020185C0.arg2;
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_0801EAD8())
            gUnk_020185C0.running = 0;
        break;
    }
}

void sub_080171D0(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        sub_0801E998(2, 0);
        gUnk_02017A30.arg = gUnk_020185C0.arg2;
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_0801EAD8())
            gUnk_020185C0.running = 0;
        break;
    }
}

void sub_0801723C(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        sub_0801E998(3, 0);
        gUnk_02017A30.arg = gUnk_020185C0.arg2;
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_0801EAD8())
            gUnk_020185C0.running = 0;
        break;
    }
}

void sub_080172A8(void)
{
    switch (gUnk_020185C0.step) {
    case 0:
        sub_0801E998(4, 0);
        gUnk_02017A30.arg = gUnk_020185C0.arg2;
        gUnk_020185C0.step++;
        break;
    case 1:
        if (sub_0801EAD8())
            gUnk_020185C0.running = 0;
        break;
    }
}
