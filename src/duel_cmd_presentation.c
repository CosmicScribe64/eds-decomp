#include "global.h"
#include "gba.h"

/* Command block at 0x020185C0 (current duel command, see duel_cmd_deck). */
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
extern struct DuelCmd gDuelCmd;

/* Duel screen / animation state at 0x0201CFB0 (fields used here). */
struct DuelScreen {
    u8 fast:1;          /* +0x000 bit 0: (hypothesis) fast-forward animations */
    u8 unk0_1:1;
    u8 unk0_2:1;        /* +0x000 bit 2 */
    u8 unk0_3:5;
};
extern struct DuelScreen gDuelScreen;

/* gMain (0x03000040): only the fields used here. */
struct Main {
    u32 rngState;           /* +0x000 */
    u16 heldKeys;           /* +0x004 */
};
extern struct Main gMain;
#define gMain gMain
#define FAST_FORWARD() ((gMain.heldKeys & 2) || gDuelScreen.fast)

extern const u16 gPulseScaleCurve[];   /* zoom/alpha curve for the grid effects (hypothesis) */
extern const s32 gScatterScaleCurve[];   /* zoom curve, 1.0 == 0x100 (shared with duel_cmd_turn) */

void DuelScreen_ScrollToZone(u32 player, u32 a);
void UnloadDuelUiGfx(void);
void LoadCardFrame(u16);
void LoadCardPicture(u16);
void DrawCardInfo(u16);
void TextCellsClear(void);
void DuelInfo_DrawCardNameCentered(u16);
void LoadDuelUiGfx(void);
void AddSprite8bppAlpha(u32 yx, u32 shapeSize, u32 attr2);
void AddSprite8bpp(u32 yx, u32 shapeSize, u32 attr2);
void PlaySE(u16 se);  /* PlaySE */
void AddAffineSprite8bppAlpha(u32 yx, u32 shapeSize, u32 attr2, u32 extra);

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
#define gDuelCmdT16 (*(struct DuelCmdTimer16 *)&gDuelCmd)

/*
 * Grid zoom-in effect (sibling of DuelCmd_ShowCardAssemble). Steps 2-4 call LoadCardFrame / LoadCardPicture /
 * DrawCardInfo and step 5 calls TextCellsClear / DuelInfo_DrawCardNameCentered. Step 6 draws a 4x5 grid of 32x32
 * sprites that zooms in over 32 frames (x * timer / 32), fades with BLDALPHA, then holds
 * until timer 0x78.
 */
void DuelCmd_ShowCardZoomIn(void)
{
    int i, j;
    int x, y;
    int ta, tb, tc, td;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 5:
        TextCellsClear();
        DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
        gDuelCmd.step++;
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
                    AddAffineSprite8bppAlpha(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5), gPulseScaleCurve[(u8)tc] << 16);
                else
                    AddSprite8bppAlpha(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        td = gDuelCmd.timer;
        if (td < 0x78) {
            if (FAST_FORWARD() && td <= 0x6F)
                gDuelCmd.timer = td + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 7:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}
void DuelCmd_ShowCardEffect(void)
{
    int i, j;
    int x, y;
    int ta, tb, tc, td;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 5:
        TextCellsClear();
        DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if ((ta = gDuelCmd.timer) <= 0x1F) {
                    x -= 0x68;
                    y -= 0x40;
                    x *= ta;
                    y *= ta;
                    x /= 32;
                    y /= 32;
                    x += 0x68;
                    y += 0x40;
                }
                tb = gDuelCmd.timer;
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
                tc = gDuelCmd.timer;
                if (tc < 16)
                    AddAffineSprite8bppAlpha(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5), gPulseScaleCurve[tc] << 16);
                else
                    AddSprite8bppAlpha(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        td = gDuelCmd.timer;
        if (td < 0x78) {
            if (FAST_FORWARD() && td <= 0x6F)
                gDuelCmd.timer = td + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 7:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Grid zoom-in effect, table variant. Like DuelCmd_ShowCardZoomIn, but once the timer passes
 * 0x67 the grid is scaled by the curve gScatterScaleCurve (1.0 == 0x100, i.e. /256)
 * around the centre (0x68, 0x40), with a BLDALPHA fade-out over that range.
 */
/*
 * Grid zoom-in effect, table variant. Like DuelCmd_ShowCardZoomIn, but once the timer passes
 * 0x67 the grid is scaled by the curve gScatterScaleCurve (1.0 == 0x100, i.e. /256)
 * around the centre (0x68, 0x40), with a BLDALPHA fade-out over that range.
 */
void DuelCmd_ShowCardScatter(void)
{
    int i, j;
    int x, y, dx, dy, k;
    int ta, tb, tc, td;

    /* FAKEMATCH: the (u8) timer casts and gDuelCmdT16 keep the grid loop long enough
     * that loop.c leaves the timer address in it (see gDuelCmdT16 above). */

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 5:
        TextCellsClear();
        DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if ((ta = (u8)gDuelCmdT16.timer) > 0x67) {
                    dx = x - 0x68;
                    dy = y - 0x40;
                    k = ta - 0x68;
                    dx *= gScatterScaleCurve[k];
                    dy *= gScatterScaleCurve[k];
                    dx /= 256;
                    dy /= 256;
                    x = dx + 0x68;
                    y = dy + 0x40;
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
                    AddAffineSprite8bppAlpha(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5), gPulseScaleCurve[(u8)tc] << 16);
                else
                    AddSprite8bppAlpha(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        td = gDuelCmd.timer;
        if (td < 0x78) {
            if (FAST_FORWARD() && td <= 0x6F)
                gDuelCmd.timer = td + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 7:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Grid fade effect (sibling of DuelCmd_ShowCardAssemble). Steps 2-4 call LoadCardFrame / LoadCardPicture /
 * DrawCardInfo. Step 5 draws the 4x5 sprite grid growing from the top (y * timer / 16) with
 * a BLDALPHA fade-in, step 6 flashes it, and step 7 shrinks it again with a fade-out.
 */
void DuelCmd_ShowCardUnrollDown(void)
{
    int i, j;
    int x, y;
    int ta, tb, te;

    switch (gDuelCmd.step) {
    case 0:
        TextCellsClear();
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 5:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if (gDuelCmd.timer <= 0xF) {
                    y *= gDuelCmd.timer;
                    y /= 16;
                }
                AddSprite8bppAlpha(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tb = gDuelCmd.timer;
        if (tb < 16) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = tb | ((u8)(16 - tb) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 11)
            gDuelCmd.timer += 3;
        if (gDuelCmd.timer > 15) {
            TextCellsClear();
            DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++)
                AddSprite8bpp((j * 32 + 0x44) | ((i * 32 + 2) << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 0x17)
            gDuelCmd.timer += 7;
        if (gDuelCmd.timer > 0x1F) {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 7:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                x = j * 32 + 0x44;
                y = i * 32 + 2;
                if ((ta = gDuelCmd.timer) > 0x27) {
                    y *= (0x38 - ta);
                    y /= 16;
                }
                AddSprite8bppAlpha(x | (y << 16), 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tb = gDuelCmd.timer;
        if (tb > 0x27) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = (u8)(0x38 - tb) | ((u8)(tb - 0x28) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        te = gDuelCmd.timer;
        if (te < 0x38) {
            if (FAST_FORWARD() && te <= 0x2F)
                gDuelCmd.timer = te + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    case 8:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}
/*
 * Variant of DuelCmd_ShowCardUnrollDown where the grid slides in horizontally (x * timer / 16) and
 * SE 0x2C plays when it lands; step 6 flashes it with BLDY.
 */
void DuelCmd_ShowCardUnrollSideways(void)
{
    int i, j;
    int x, y;
    int ta, tb, tc, te;

    switch (gDuelCmd.step) {
    case 0:
        TextCellsClear();
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.step++;
        break;
    case 1:
        UnloadDuelUiGfx();
        gDuelCmd.step++;
        break;
    case 2:
        LoadCardFrame(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 3:
        LoadCardPicture(gDuelCmd.arg2);
        gDuelCmd.step++;
        break;
    case 4:
        DrawCardInfo(gDuelCmd.arg2);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 5: {
        int rowY;
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                rowY = (i * 32 + 2) << 16;
                x = j * 32 + 0x44;
                if (gDuelCmd.timer <= 0xF) {
                    x -= 0x68;
                    x *= gDuelCmd.timer;
                    x /= 16;
                    x += 0x68;
                }
                AddSprite8bppAlpha(x | rowY, 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        if (gDuelCmd.timer < 16) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = gDuelCmd.timer | ((u8)(16 - gDuelCmd.timer) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 11)
            gDuelCmd.timer += 3;
        if (gDuelCmd.timer > 15) {
            TextCellsClear();
            DuelInfo_DrawCardNameCentered(gDuelCmd.arg2);
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
            PlaySE(0x2C);
        }
        break;
    }
    case 6:
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                y = (i * 32 + 2) << 16;
                AddSprite8bpp((j * 32 + 0x44) | y, 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tc = gDuelCmd.timer;
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
        gDuelCmd.timer++;
        if (FAST_FORWARD() && gDuelCmd.timer <= 0x1B)
            gDuelCmd.timer += 3;
        if (gDuelCmd.timer > 0x1F) {
            gDuelCmd.timer = 0;
            gDuelCmd.step++;
        }
        break;
    case 7: {
        int rowY;
        for (i = 0; i <= 4; i++) {
            for (j = 0; j <= 3; j++) {
                rowY = (i * 32 + 2) << 16;
                x = j * 32 + 0x44;
                if ((ta = gDuelCmd.timer) > 0x27) {
                    x -= 0x68;
                    x *= (0x38 - ta);
                    x /= 16;
                    x += 0x68;
                }
                AddSprite8bppAlpha(x | rowY, 0x80, (u16)(j * 4) + ((u16)(i * 2 + 1) << 5));
            }
        }
        tb = gDuelCmd.timer;
        if (tb > 0x27) {
            REG_BLDCNT = 0xF40;
            REG_BLDALPHA = (u8)(0x38 - tb) | ((u8)(tb - 0x28) << 8);
        } else {
            REG_BLDCNT = 0;
            REG_BLDALPHA = 0;
        }
        te = gDuelCmd.timer;
        if (te < 0x38) {
            if (FAST_FORWARD() && te <= 0x2F)
                gDuelCmd.timer = te + 7;
            gDuelCmd.timer++;
            break;
        }
        gDuelCmd.step++;
        break;
    }
    case 8:
        gDuelCmd.step++;
        break;
    default:
        LoadDuelUiGfx();
        gDuelCmd.running = 0;
        break;
    }
}

