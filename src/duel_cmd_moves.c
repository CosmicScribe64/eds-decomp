#include "global.h"
#include "main.h"
#include "duel.h"

/*
 * Duel "script command" handlers (continued from duel_cmd_deck): summon /
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

/* Location of a card, passed to the card-move animations (DuelAnim_MoveCard etc.). */
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

extern struct DuelCmd gDuelCmd;

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
extern struct DuelScreen gDuelScreen;

/* gMain (0x03000040) layout comes from main.h. */
#define gMain gMain
#define FAST_FORWARD() ((gMain.heldKeys & 2) || gDuelScreen.fast)

extern const u8 gDuelBannerPal[];
extern const u8 gSurrenderBannerGfx[];
extern const u16 gBounceScaleCurve[];
extern const u8 gLpDigitsPal[];
extern const u8 gLpDigitsGfx[];
extern const u8 gJustAMomentBannerGfx[];
extern const u16 gBannerSlideOffsets[];
extern const u16 gPulseScaleCurve[];

/* Banner graphics for DuelCmd_ShowDuelResult, indexed by arg2 (0-4). */
struct BannerGfx {
    const u8 *pal;      /* 0x20 bytes for OBJ palette 15 */
    const u8 *gfx;      /* 0x800 bytes for OBJ VRAM 0x06016C80 */
    u16 bgm;            /* passed to PlayJingle / SoundIsBGMPlaying */
};
extern const struct BannerGfx gDuelResultBanners[];

#define CMD_ID(c) ((c)->cmd & 0xFFF)
#define CMD_PLAYER(c) ((c)->cmd >> 15)
#define CMD_CARD ((struct DuelCard *)gDuelCmd.card)
extern const u32 gCardStats[];   /* card stats, indexed by card ID */
extern const u16 gCardIdToNumber[];   /* card ID to card number */
extern const u8 gSmokePuffAnim[];

#define ZONE(p, s) ((struct DuelZone *)((u8 *)gDuelPlayers[0].zones + (s) * 0x94 + (p) * 0xD64))
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

void AddSprite(u32 yx, u16 shapeSize, u16 attr2); /* AddSprite */
void PlaySE(u16 id); /* PlaySE */
void PlaceMonsterCard(u32, u32, void *, u32, u32);
void CopyDuelCard(struct DuelCard *dst, struct DuelCard *src);
void DuelScreen_ScrollToZone(u32, u32);
void DuelAnim_MoveCard(u32 id, struct CardPos *from, struct CardPos *to);
void DuelAnim_PlayZoneEffect(struct CardPos *pos, const void *, u32, u32);
void ClearZoneTiles(u32 player, u32 slot);
void DrawAllAreaTiles(void);
void ReturnTemporarilyBanishedCard(u32, u32);
void CopyDoubleWords(void *dst, const void *src, u32 size);   /* CopyDoubleWords */
void AddAffineSprite(u32 yx, u16 shapeSize, u16 attr2, s32 affine);
void DrawLifePoints(u32 player, u32 lp);  /* (hypothesis) update the LP display */
void SubtractLifePoints(struct DuelPlayer *players, u32 player, s32 amount);  /* subtract LP, clamp at 0 */
void DuelCursor_Refresh(void);
void DrawLpChangeAmount(u32 x, u32 y, s32 value, u32 palette);
void AddSprite(u32 yx, u16 shapeSize, u16 attr2); /* AddSprite */
void PlayJingle(u16);
u32 SoundIsBGMPlaying(u16);
void SendZoneCardToGraveyardOrBanished(u32, u32, u32);
void AddCardToBanishedTemporarily(struct DuelCard *, u32);

/* Summon/set a card: arg2 = slot (bits 0-7) | face-down flag (bit 8), arg4/arg6 = card word. */
void DuelCmd_SetMagicalHatsCard(void)
{
    u32 player = CMD_PLAYER(&gDuelCmd);
    u8 slot = gDuelCmd.arg2;
    u32 faceDown = (gDuelCmd.arg2 >> 8) & 1;
    struct DuelCard card;
    struct CardPos pos;
    u16 cardNo;

    *(u32 *)&card = (gDuelCmd.arg6 << 16) | gDuelCmd.arg4;
    cardNo = CARD_NUMBER(card.id);
    if (cardNo >= 1920)
        if (cardNo <= 1999)
            faceDown = 1;
    switch (gDuelCmd.step) {
    case 0:
        PlaceMonsterCard(player, slot, &card, 1, faceDown);
        if (CARD_TYPE(card.id) > 20) {
            u8 *zoneBytes = (u8 *)gDuelPlayers[0].zones + (slot * 0x94 + player * 0xD64);
            zoneBytes[0x8C] |= 2;
        }
        PlaySE(0x10);
        pos.player = player;
        pos.area = 0;
        pos.slot = slot;
        pos.flag14 = 1;
        pos.flag15 = faceDown;
        DuelAnim_PlayZoneEffect(&pos, gSmokePuffAnim, 0, 0);
        ClearZoneTiles(player, slot);
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}



/* Take a card off a field zone: arg2 = slot. Moves it into cmd.card and animates it to area 15. */
void DuelCmd_BanishMonsterUntilEndPhase(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER(&gDuelCmd);
    u16 slot = gDuelCmd.arg2;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
        break;
    case 1:
        /* FAKEMATCH: the extra uses raise player's and slot's refs so global-alloc
         * takes player first (r7), then slot evicts the local in r6 and step the
         * local in r5, as in the ROM. */
        asm("" :: "r"(player), "r"(slot));
        CopyDuelCard(CMD_CARD, &gDuelPlayers[player & 1].zones[slot].card);
        (gDuelPlayers[player & 1].zones + slot)->card.id = 0;
        ClearZoneTiles(player, slot);
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
        ((void (*)(u32, struct CardLoc *, struct CardLoc *))DuelAnim_MoveCard)(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        AddCardToBanishedTemporarily(CMD_CARD, slot);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

void DuelCmd_ReturnBanishedMonster(void)
{
    u32 player = CMD_PLAYER(&gDuelCmd);
    u16 slot = gDuelCmd.arg2;
    struct CardPos from, to;

    switch (gDuelCmd.step) {
    case 0:
        DuelScreen_ScrollToZone(player, 11);
        gDuelCmd.step++;
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
        DuelAnim_MoveCard(1, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        ReturnTemporarilyBanishedCard(player, slot);
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

/* Flip a field card to area 14/15 (area 15 when arg4 != 0): arg2 = slot. */
void DuelCmd_SendFusionMaterialToGrave(void)
{
    struct CardLoc from, to;
    u32 player = CMD_PLAYER(&gDuelCmd);
    u16 slot = gDuelCmd.arg2;
    u16 arg4 = gDuelCmd.arg4;

    switch (gDuelCmd.step) {
    case 0:
        ClearZoneTiles(player, slot);
        ZONE(player & 1, slot)->card.flag20 = 1;
        CopyDuelCard(CMD_CARD, &gDuelPlayers[player & 1].zones[slot].card);
        SendZoneCardToGraveyardOrBanished(player, slot, arg4);
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
        ((void (*)(u32, struct CardLoc *, struct CardLoc *))DuelAnim_MoveCard)(CMD_CARD->id, &from, &to);
        gDuelCmd.step++;
        break;
    default:
        DrawAllAreaTiles();
        gDuelCmd.running = 0;
        break;
    }
}

void sub_08013104(void)
{
    ((struct DuelZone08012C4C *)ZONE(CMD_PLAYER(&gDuelCmd), gDuelCmd.arg2))->unk8_0 = gDuelCmd.arg4;
    gDuelCmd.running = 0;
}

void DuelCmd_ClearZoneLinks2(void)
{
    ZONE(CMD_PLAYER(&gDuelCmd), gDuelCmd.arg2)->numLinks = 0;
    gDuelCmd.running = 0;
}

/* Banner arg2 (0-4): load its graphics, zoom it in over 0x40 frames, wait for its jingle, hold 0x1E frames. */
void DuelCmd_ShowDuelResult(void)
{
    s32 step = gDuelCmd.step;
    s32 timer;

    switch (step) {
    case 0:
        if (gDuelCmd.arg2 <= 4) {
            CopyDoubleWords((void *)0x050003E0, gDuelResultBanners[gDuelCmd.arg2].pal, 0x20);
            CopyDoubleWords((void *)0x06016C80, gDuelResultBanners[gDuelCmd.arg2].gfx, 0x800);
            PlayJingle(gDuelResultBanners[gDuelCmd.arg2].bgm);
            gDuelScreen.unk85C = step;
        }
        gDuelCmd.step++;
        gDuelCmd.timer = 0;
    case 1:
        timer = gDuelCmd.timer;
        if (timer < 0x40) {
            AddAffineSprite(0x00200058, 0xC0, 0xF364, ((0x900 - timer * 32) << 16) | (timer * 4));
            gDuelCmd.timer++;
        } else {
            AddSprite(0x00200058, 0xC0, 0xF364);
            gDuelCmd.step++;
        }
        break;
    case 2:
        AddSprite(0x00200058, 0xC0, 0xF364);
        if (SoundIsBGMPlaying(gDuelResultBanners[gDuelCmd.arg2].bgm) == 0) {
            gDuelCmd.step++;
            gDuelCmd.timer = 0;
        }
        break;
    case 3:
        AddSprite(0x00200058, 0xC0, 0xF364);
        if (gDuelCmd.timer < 0x1E)
            gDuelCmd.timer++;
        else
            gDuelCmd.step++;
        break;
    default:
        gDuelCmd.running = 0;
        break;
    }
}
/* Life points of the acting player drop to 0: banner animation, then lifePoints = 0 and DrawLifePoints. */
void DuelCmd_Surrender(void)
{
    u32 player = CMD_PLAYER(&gDuelCmd);
    u32 step = gDuelCmd.step;

    switch (step) {
    case 0:
        gDuelScreen.busy = 0;
        CopyDoubleWords((void *)0x050003E0, gDuelBannerPal, 0x20);
        CopyDoubleWords((void *)0x06016C80, gSurrenderBannerGfx, 0x400);
        gDuelScreen.unk85C = 0;
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 1:
        if (gDuelCmd.timer < 0x60) {
            AddAffineSprite(0x00300058, 0x40C0, 0xF364, gBounceScaleCurve[gDuelCmd.timer & 0x1F] << 16);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x77)
                gDuelCmd.timer += 7;
            break;
        }
    default:
        gDuelPlayers[player].lifePoints = 0;
        DrawLifePoints(player, 0);
        gDuelCmd.running = 0;
        break;
    }
}
/* Banner that slides in from the acting player's side, pulses for 0x40 frames, then slides back out. */
void DuelCmd_ShowJustAMomentBanner(void)
{
    u32 player = CMD_PLAYER(&gDuelCmd);
    s32 step = gDuelCmd.step;
    s32 timer;
    s32 y;

    switch (step) {
    case 0:
        gDuelScreen.busy = 0;
        CopyDoubleWords((void *)0x050003E0, gDuelBannerPal, 0x20);
        CopyDoubleWords((void *)0x06016C80, gJustAMomentBannerGfx, 0x400);
        gDuelScreen.unk85C = step;
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        PlaySE(0x16);
        break;
    case 1:
        timer = gDuelCmd.timer;
        if (timer < 16) {
            if (player)
                y = gBannerSlideOffsets[timer];
            else
                y = 0x80 - gBannerSlideOffsets[timer];
            AddSprite((y << 16) | 0x58, 0x40C0, 0xF364);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 7)
                gDuelCmd.timer += 7;
            break;
        }
        gDuelCmd.timer = 0;
        gDuelCmd.step = step + 1;
        PlaySE(0x16);
    case 2:
        if (gDuelCmd.timer < 0x40) {
            AddAffineSprite(0x00400058, 0x40C0, 0xF364, gPulseScaleCurve[gDuelCmd.timer & 0xF] << 16);
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x37)
                gDuelCmd.timer += 7;
            break;
        }
        gDuelCmd.timer = 16;
        gDuelCmd.step++;
    case 3:
        timer = gDuelCmd.timer;
        if (timer != 0) {
            if (player)
                y = gBannerSlideOffsets[timer - 1];
            else
                y = 0x80 - gBannerSlideOffsets[timer - 1];
            AddSprite((y << 16) | 0x58, 0x40C0, 0xF364);
            gDuelCmd.timer--;
            if (FAST_FORWARD() && gDuelCmd.timer > 8)
                gDuelCmd.timer -= 7;
            break;
        }
    default:
        gDuelCmd.running = 0;
        break;
    }
}

/* Draws a signed decimal number as 16-px digit sprites, right-aligned at x + 0x50. */
void DrawLpChangeAmount(u32 x, u32 y, s32 value, u32 palette)
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
        AddSprite((y << 16) | x, 0x40, tile);
    } else {
        do {
            AddSprite(x | (y << 16), 0x40, value % 10 * 4 + tile);
            value /= 10;
            x -= 16;
        } while (value != 0);
        AddSprite((y << 16) | x, 0x40, tile + sign * 4);
    }
}

/*
 * Life-point change animation for the acting player: show the amount (arg2) next to the
 * LP counter, then count it into lifePoints in steps of 100 / 10 / 1 per frame.
 * gain != 0 adds, gain == 0 subtracts (SubtractLifePoints).
 */
void DuelCmd_ChangeLifePoints(u16 gain)
{
    u32 player = CMD_PLAYER(&gDuelCmd);
    u32 x = player * 0x68 + 8;
    u32 y = 0x58 - player * 24;
    u16 se = gain ? 12 : 13;
    s32 step = gDuelCmd.step;
    struct DuelPlayer *lp;
    struct DuelPlayer *players;

    switch (step) {
    case 0:
        CopyDoubleWords((void *)0x050003E0, gLpDigitsPal, 0x20);
        CopyDoubleWords((void *)0x06016C80, gLpDigitsGfx, 0xC00);
        gDuelScreen.unk85C = step;
        DuelScreen_ScrollToZone(0, 0);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 1:
        DrawLpChangeAmount(x, y, gain ? gDuelCmd.arg2 : -gDuelCmd.arg2, gain != 0);
        if (gDuelCmd.timer < 0x5A) {
            gDuelCmd.timer++;
            if (FAST_FORWARD() && gDuelCmd.timer <= 0x4F)
                gDuelCmd.timer += 7;
            break;
        }
        PlaySE(se);
        gDuelCmd.timer = 0;
        gDuelCmd.step++;
        break;
    case 2:
        players = gDuelPlayers;
        lp = &players[player];
        if (lp->lifePoints == 0) {
            gDuelCmd.step = step + 1;
            break;
        }
        DrawLpChangeAmount(x, y, gain ? gDuelCmd.arg2 : -gDuelCmd.arg2, gain != 0);
        if (gDuelCmd.arg2 >= 100) {
            gDuelCmd.arg2 -= 100;
            if (gain)
                lp->lifePoints += 100;
            else
                SubtractLifePoints(players, player, 100);
            DrawLifePoints(player, gDuelPlayers[player].lifePoints);
            gDuelCmd.timer++;
            if (gDuelCmd.timer > 10) {
                PlaySE(se);
                gDuelCmd.timer = 0;
            }
            break;
        }
        if (gDuelCmd.arg2 >= 10) {
            gDuelCmd.arg2 -= 10;
            if (gain)
                lp->lifePoints += 10;
            else
                SubtractLifePoints(players, player, 10);
            DrawLifePoints(player, gDuelPlayers[player].lifePoints);
            gDuelCmd.timer++;
            if (gDuelCmd.timer > 10) {
                PlaySE(se);
                gDuelCmd.timer = 0;
            }
            break;
        }
        if (gDuelCmd.arg2 != 0) {
            gDuelCmd.arg2 -= 1;
            if (gain)
                lp->lifePoints += 1;
            else
                SubtractLifePoints(players, player, 1);
            DrawLifePoints(player, gDuelPlayers[player].lifePoints);
            gDuelCmd.timer++;
            if (gDuelCmd.timer > 10) {
                PlaySE(se);
                gDuelCmd.timer = 0;
            }
            break;
        }
    default:
        DuelCursor_Refresh();
        gDuelCmd.running = 0;
        break;
    }
}

/*
 * Local view of the duel flag word at gDuel+0x1ACC. duel.h folds bits 6-7
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
extern struct DuelFlags08012C4C gUnk_020192E0_flags asm("gDuel");

/* Duel rule-flag commands 0x15-0x1B: store arg2 into one bit of the flag bytes at 0x020192E0+0x1ACC. */
void DuelCmd_SetNegationFlag(void)
{
    switch (CMD_ID(&gDuelCmd)) {
    case 0x15:
        gUnk_020192E0_flags.flag1ACD_1 = gDuelCmd.arg2;
        break;
    case 0x16:
        gUnk_020192E0_flags.flag1ACD_2 = gDuelCmd.arg2;
        break;
    case 0x17:
        gUnk_020192E0_flags.flag1ACD_4 = gDuelCmd.arg2;
        break;
    case 0x18:
        gUnk_020192E0_flags.flag1ACD_3 = gDuelCmd.arg2;
        break;
    case 0x19:
        gUnk_020192E0_flags.flag1ACC_7 = gDuelCmd.arg2;
        break;
    case 0x1A:
        gUnk_020192E0_flags.flag1ACC_6 = gDuelCmd.arg2;
        break;
    case 0x1B:
        gUnk_020192E0_flags.flag1ACD_0 = gDuelCmd.arg2;
        break;
    }
    gDuelCmd.running = 0;
}
