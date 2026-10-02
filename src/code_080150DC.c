#include "global.h"
#include "gba.h"

/* Command block at 0x020185C0 (current duel command, see code_0800EAA8). */
struct DuelCmd {
    u16 cmd;            /* 0x000: bits 0-11 command id, bit 15 acting player */
    u16 arg2;           /* 0x002 */
    u16 arg4;           /* 0x004 */
    u16 arg6;           /* 0x006 */
    u8 filler8[0x80A - 0x8];
    u16 step:7;         /* 0x80A bits 0-6: multi-frame handler state */
    u16 unk80A_7:9;
    u32 unk80C_0:5;
    u32 timer:7;        /* 0x80C bits 5-11: frame counter inside a step (u32 container: signed compares) */
    u32 unk80C_12:1;
    u32 running:1;      /* 0x80D bit 5: command in progress */
    u32 unk80C_14:2;
    u32 unk80C_16:16;
};
extern struct DuelCmd gUnk_020185C0;

/* Duel screen / animation state at 0x0201CFB0 (fields used here). */
struct DuelScreen {
    u8 fast:1;          /* +0x000 bit 0: (hypothesis) fast-forward animations */
    u8 unk0_1:1;
    u8 unk0_2:1;        /* +0x000 bit 2 */
    u8 unk0_3:5;
};
extern struct DuelScreen gUnk_0201CFB0;

/* gMain (0x03000040): only the fields used here. */
struct Main {
    u32 rngState;           /* +0x000 */
    u16 heldKeys;           /* +0x004 */
};
extern struct Main gUnk_03000040;
#define gMain gUnk_03000040
#define FAST_FORWARD() ((gMain.heldKeys & 2) || gUnk_0201CFB0.fast)

extern const u16 gUnk_081A4424[];   /* zoom/alpha curve for the grid effects (hypothesis) */
extern const s32 gUnk_08081768[];   /* zoom curve, 1.0 == 0x100 (shared with code_08013CDC) */

void sub_080240A8(u32 player, u32 a);
void sub_080619E8(void);
void sub_08061A1C(u16);
void sub_08061D24(u16);
void sub_08061E54(u16);
void sub_0805ED9C(void);
void sub_0805F00C(u16);
void sub_08060578(void);
void sub_080763D0(u32 yx, u32 shapeSize, u32 attr2);
void sub_080762D0(u32 yx, u32 shapeSize, u32 attr2);
void sub_08077AEC(u16 se);  /* PlaySE */
void sub_08076448(u32 yx, u32 shapeSize, u32 attr2, u32 extra);

/*
 * Halfword view of the DuelCmd timer (0x80C bits 5-11) for the grid loops below.
 * FAKEMATCH: reading the timer through a u16 container plus the (u8) casts in the
 * loop add RTL insns that combine deletes later. They lift the inner loop above
 * loop.c's hoisting threshold (13 * savings 3 * life 3 = 117 insns), so the timer
 * address stays inside the loop and the base copy is spilled, as in the ROM.
 */
struct DuelCmdTimer16 {
    u16 cmd;
    u16 arg2;
    u8 filler4[0x80C - 0x4];
    u16 unk80C_0:5;
    u16 timer:7;
    u16 unk80C_12:4;
};
#define gDuelCmdT16 (*(struct DuelCmdTimer16 *)&gUnk_020185C0)

/*
 * Grid zoom-in effect (sibling of sub_08014C30). Steps 2-4 call sub_08061A1C / sub_08061D24 /
 * sub_08061E54 and step 5 calls sub_0805ED9C / sub_0805F00C. Step 6 draws a 4x5 grid of 32x32
 * sprites that zooms in over 32 frames (x * timer / 32), fades with BLDALPHA, then holds
 * until timer 0x78.
 */
void sub_080150DC(void)
{
    int i, j;
    int x, y;
    int ta, tb, tc, td;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(0, 0);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_080619E8();
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_08061A1C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 3:
        sub_08061D24(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 4:
        sub_08061E54(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 5:
        sub_0805ED9C();
        sub_0805F00C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if ((ta = (u8)gDuelCmdT16.timer) <= 0x1F) {
                    x -= 0x68;
                    y -= 0x40;
                    x *= ta;
                    y *= ta;
                    x /= 32;
                    y /= 32;
                    x += 0x68;
                    y += 0x40;
                }
                tb = (u8)gDuelCmdT16.timer;
                if (tb < 16) {
                    REG_BLDCNT = 0xF40;
                    REG_BLDALPHA = (u8)tb | ((u8)(16 - tb) << 8);
                } else if (tb > 0x67) {
                    REG_BLDCNT = 0xF40;
                    REG_BLDALPHA = (u8)(0x78 - tb) | ((u8)(tb - 0x68) << 8);
                } else {
                    REG_BLDCNT = 0;
                    REG_BLDALPHA = 0;
                }
                tc = (u8)gDuelCmdT16.timer;
                if (tc < 16)
                    sub_08076448(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5), gUnk_081A4424[(u8)tc] << 16);
                else
                    sub_080763D0(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        td = gUnk_020185C0.timer;
        if (td < 0x78) {
            if (FAST_FORWARD() && td <= 0x6F)
                gUnk_020185C0.timer = td + 7;
            gUnk_020185C0.timer++;
            break;
        }
        gUnk_020185C0.step++;
        break;
    case 7:
        gUnk_020185C0.step++;
        break;
    default:
        sub_08060578();
        gUnk_020185C0.running = 0;
        break;
    }
}
void sub_080153D4(void)
{
    int i, j;
    int x, y;
    int ta, tb, tc, td;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(0, 0);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_080619E8();
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_08061A1C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 3:
        sub_08061D24(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 4:
        sub_08061E54(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 5:
        sub_0805ED9C();
        sub_0805F00C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if ((ta = gUnk_020185C0.timer) <= 0x1F) {
                    x -= 0x68;
                    y -= 0x40;
                    x *= ta;
                    y *= ta;
                    x /= 32;
                    y /= 32;
                    x += 0x68;
                    y += 0x40;
                }
                tb = gUnk_020185C0.timer;
                if (tb < 16) {
                    REG_BLDCNT = 0xF40;
                    REG_BLDALPHA = tb | ((u8)(16 - tb) << 8);
                } else if ((u32)((u8)tb - 0x10) <= 0x1F) {
                    REG_BLDCNT = 0x27A7;
                    REG_BLDY = tb - 0x10;
                } else if ((u32)(tb - 0x30) <= 0x1F) {
                    REG_BLDCNT = 0x27A7;
                    REG_BLDY = 0x4F - tb;
                } else if (tb > 0x67) {
                    REG_BLDCNT = 0xF40;
                    REG_BLDALPHA = (u8)(0x78 - tb) | ((u8)(tb - 0x68) << 8);
                } else {
                    REG_BLDCNT = 0;
                    REG_BLDALPHA = 0;
                }
                tc = gUnk_020185C0.timer;
                if (tc < 16)
                    sub_08076448(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5), gUnk_081A4424[tc] << 16);
                else
                    sub_080763D0(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        td = gUnk_020185C0.timer;
        if (td < 0x78) {
            if (FAST_FORWARD() && td <= 0x6F)
                gUnk_020185C0.timer = td + 7;
            gUnk_020185C0.timer++;
            break;
        }
        gUnk_020185C0.step++;
        break;
    case 7:
        gUnk_020185C0.step++;
        break;
    default:
        sub_08060578();
        gUnk_020185C0.running = 0;
        break;
    }
}

/*
 * Grid zoom-in effect, table variant. Like sub_080150DC, but once the timer passes
 * 0x67 the grid is scaled by the curve gUnk_08081768 (1.0 == 0x100, i.e. /256)
 * around the centre (0x68, 0x40), with a BLDALPHA fade-out over that range.
 */
#if 0 /* NONMATCHING: register allocation only. old_agbcc keeps a copy of &gUnk_020185C0 in a
       * register (adds r5,r4,#0) and reuses it for cases 2-4 and the step++ tails. It also
       * hoists the timer address &gUnk_020185C0+0x80C into r9 inside the step 6 grid loop.
       * The ROM rematerializes both literals at each use (and spills i*32 to the stack
       * instead). Everything else matches instruction for instruction. Tried re-reading
       * fields, explicit address casts, return vs break, and hoisted outer-loop locals; the
       * copy persists. */
void sub_08015720(void)
{
    int i, j;
    u32 x, y;
    int scale;
    int ta, tb, tc, td;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_080240A8(0, 0);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_080619E8();
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_08061A1C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 3:
        sub_08061D24(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 4:
        sub_08061E54(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 5:
        sub_0805ED9C();
        sub_0805F00C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 6:
        for (i = 0; i <= 4; i++) {
            int y0 = i * 32 + 2;
            int tile = (u16)(i * 2 + 1) << 5;
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = y0;
                if ((ta = gUnk_020185C0.timer) > 0x67) {
                    scale = gUnk_08081768[ta - 0x68];
                    x = (x - 0x68) * scale / 256 + 0x68;
                    y = (y - 0x40) * scale / 256 + 0x40;
                }
                tb = gUnk_020185C0.timer;
                if (tb < 16) {
                    REG_BLDCNT = 0xF40;
                    REG_BLDALPHA = tb | ((u8)(16 - tb) << 8);
                } else if (tb > 0x67) {
                    REG_BLDCNT = 0xF40;
                    REG_BLDALPHA = (u8)(0x78 - tb) | ((u8)(tb - 0x68) << 8);
                } else {
                    REG_BLDCNT = 0;
                    REG_BLDALPHA = 0;
                }
                tc = gUnk_020185C0.timer;
                if (tc < 16)
                    sub_08076448(x | (y << 16), 0x80, (u16)(j * 4) + tile, gUnk_081A4424[tc] << 16);
                else
                    sub_080763D0(x | (y << 16), 0x80, (u16)(j * 4) + tile);
            }
        }
        td = gUnk_020185C0.timer;
        if (td < 0x78) {
            if (FAST_FORWARD() && td <= 0x6F)
                gUnk_020185C0.timer = td + 7;
            gUnk_020185C0.timer++;
            break;
        }
        gUnk_020185C0.step++;
        break;
    case 7:
        gUnk_020185C0.step++;
        break;
    default:
        sub_08060578();
        gUnk_020185C0.running = 0;
        break;
    }
}
#endif
INCLUDE_ASM("asm/nonmatching/code_080150DC", sub_08015720); /* 0x08015720 size 0x30C */
/*
 * Grid fade effect (sibling of sub_08014C30). Steps 2-4 call sub_08061A1C / sub_08061D24 /
 * sub_08061E54. Step 5 draws the 4x5 sprite grid growing from the top (y * timer / 16) with
 * a BLDALPHA fade-in, step 6 flashes it, and step 7 shrinks it again with a fade-out.
 */
void sub_08015A2C(void)
{
    int i, j;
    int x, y;
    int ta, tb, te;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_0805ED9C();
        sub_080240A8(0, 0);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_080619E8();
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_08061A1C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 3:
        sub_08061D24(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 4:
        sub_08061E54(gUnk_020185C0.arg2);
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 5:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if (gUnk_020185C0.timer <= 0xF) {
                    y *= gUnk_020185C0.timer;
                    y /= 16;
                }
                sub_080763D0(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tb = gUnk_020185C0.timer;
        if (tb < 16) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = tb | ((u8)(16 - tb) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        gUnk_020185C0.timer++;
        if (FAST_FORWARD() && gUnk_020185C0.timer <= 11)
            gUnk_020185C0.timer += 3;
        if (gUnk_020185C0.timer > 15) {
            sub_0805ED9C();
            sub_0805F00C(gUnk_020185C0.arg2);
            gUnk_020185C0.timer = 0;
            gUnk_020185C0.step++;
        }
        break;
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++)
                sub_080762D0((j * 32 + 0x44) | ((i * 32 + 2) << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
        }
        gUnk_020185C0.timer++;
        if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x17)
            gUnk_020185C0.timer += 7;
        if (gUnk_020185C0.timer > 0x1F) {
            gUnk_020185C0.timer = 0;
            gUnk_020185C0.step++;
        }
        break;
    case 7:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if ((ta = gUnk_020185C0.timer) > 0x27) {
                    y *= (0x38 - ta);
                    y /= 16;
                }
                sub_080763D0(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tb = gUnk_020185C0.timer;
        if (tb > 0x27) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = (u8)(0x38 - tb) | ((u8)(tb - 0x28) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        te = gUnk_020185C0.timer;
        if (te < 0x38) {
            if (FAST_FORWARD() && te <= 0x2F)
                gUnk_020185C0.timer = te + 7;
            gUnk_020185C0.timer++;
            break;
        }
        gUnk_020185C0.step++;
        break;
    case 8:
        gUnk_020185C0.step++;
        break;
    default:
        sub_08060578();
        gUnk_020185C0.running = 0;
        break;
    }
}
/*
 * Variant of sub_08015A2C where the grid slides in horizontally (x * timer / 16) and
 * SE 0x2C plays when it lands; step 6 flashes it with BLDY.
 */
void sub_08015E40(void)
{
    int i, j;
    int x, y;
    int ta, tb, tc, te;

    switch (gUnk_020185C0.step) {
    case 0:
        sub_0805ED9C();
        sub_080240A8(0, 0);
        gUnk_020185C0.step++;
        break;
    case 1:
        sub_080619E8();
        gUnk_020185C0.step++;
        break;
    case 2:
        sub_08061A1C(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 3:
        sub_08061D24(gUnk_020185C0.arg2);
        gUnk_020185C0.step++;
        break;
    case 4:
        sub_08061E54(gUnk_020185C0.arg2);
        gUnk_020185C0.timer = 0;
        gUnk_020185C0.step++;
        break;
    case 5: {
        int rowY;
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                rowY = (i * 32 + 2) << 16;
                x = j * 32 + 0x44;
                if (gUnk_020185C0.timer <= 0xF) {
                    x -= 0x68;
                    x *= gUnk_020185C0.timer;
                    x /= 16;
                    x += 0x68;
                }
                sub_080763D0(x | rowY, 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        if (gUnk_020185C0.timer < 16) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = gUnk_020185C0.timer | ((u8)(16 - gUnk_020185C0.timer) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        gUnk_020185C0.timer++;
        if (FAST_FORWARD() && gUnk_020185C0.timer <= 11)
            gUnk_020185C0.timer += 3;
        if (gUnk_020185C0.timer > 15) {
            sub_0805ED9C();
            sub_0805F00C(gUnk_020185C0.arg2);
            gUnk_020185C0.timer = 0;
            gUnk_020185C0.step++;
            sub_08077AEC(0x2C);
        }
        break;
    }
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                y = (i * 32 + 2) << 16;
                sub_080762D0((j * 32 + 0x44) | y, 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tc = gUnk_020185C0.timer;
        if (tc < 0x20) {
            if (tc < 16) {
                REG_BLDY = tc;
                REG_BLDCNT = 0x1090;
            } else {
                REG_BLDY = 0x1F - tc;
                REG_BLDCNT = 0x1090;
            }
        } else {
            REG_BLDY = 0;
            REG_BLDCNT = 0;
        }
        gUnk_020185C0.timer++;
        if (FAST_FORWARD() && gUnk_020185C0.timer <= 0x1B)
            gUnk_020185C0.timer += 3;
        if (gUnk_020185C0.timer > 0x1F) {
            gUnk_020185C0.timer = 0;
            gUnk_020185C0.step++;
        }
        break;
    case 7: {
        int rowY;
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                rowY = (i * 32 + 2) << 16;
                x = j * 32 + 0x44;
                if ((ta = gUnk_020185C0.timer) > 0x27) {
                    x -= 0x68;
                    x *= (0x38 - ta);
                    x /= 16;
                    x += 0x68;
                }
                sub_080763D0(x | rowY, 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tb = gUnk_020185C0.timer;
        if (tb > 0x27) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = (u8)(0x38 - tb) | ((u8)(tb - 0x28) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        te = gUnk_020185C0.timer;
        if (te < 0x38) {
            if (FAST_FORWARD() && te <= 0x2F)
                gUnk_020185C0.timer = te + 7;
            gUnk_020185C0.timer++;
            break;
        }
        gUnk_020185C0.step++;
        break;
    }
    case 8:
        gUnk_020185C0.step++;
        break;
    default:
        sub_08060578();
        gUnk_020185C0.running = 0;
        break;
    }
}

