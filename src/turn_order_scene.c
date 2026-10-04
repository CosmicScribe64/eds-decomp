/*
 * Pre-duel turn-order screen, first half: the setup steps, the phases and two of the drawers.
 *
 * Before a Campaign or Link duel the player turns a carousel of rock/scissors/paper cards with Left/Right and
 * picks one with A. The CPU or the link partner answers, and JudgeRockPaperScissors shows WIN, LOSE or DRAW;
 * a draw replays the hands. The winner picks "FIRST to go" or "SECOND to go" (when the player loses to the
 * CPU, the CPU picks by frame parity), the chosen banner moves to the centre, the DUEL logo drops in and
 * the screen flashes white. The runners and TurnOrder_RpsMain, which runs the phases below and draws the
 * carousel, are in turn_order_steps.c; the other sprite drawers are in destiny_board_scene.c.
 *
 * Phases (gSceneWork.u.turnOrder.phase, enum TurnOrderPhase):
 *   TURN_ORDER_CHOOSE_HAND      TurnOrder_ChooseHand         pick a hand; the opponent answers
 *   TURN_ORDER_SHOW_RESULT      TurnOrder_ShowResult         WIN / LOSE / DRAW
 *   TURN_ORDER_CHOOSE_TURN      TurnOrder_ChooseTurn         the player won: FIRST or SECOND
 *   TURN_ORDER_ANIMATE_CHOICE   TurnOrder_AnimateTurnChoice  the chosen banner moves to the centre
 *   TURN_ORDER_DUEL_LOGO        TurnOrder_ShowDuelLogo       the DUEL logo drops in
 *   TURN_ORDER_FLASH_WHITE      TurnOrder_FlashWhite         white flash
 * Over the link the two games exchange the hand (LINKMSG_RPS_HAND), the turn choice (LINKMSG_TURN_CHOICE) and,
 * after a draw, a rematch flag (LINKMSG_RPS_REMATCH) with LinkSyncStep; the "Wait" sign shows meanwhile.
 */
#include "global.h"
#include "gba.h"                /* REG_*, DISPCNT_*, BLDCNT_*, BLDALPHA_BLEND, CpuFastSet, VRAM, PLTT, keys */
#include "main.h"               /* gMain, VBLANK_COPY_OAM */
#include "sound.h"              /* PlaySE, FadeOutBGM */
#include "constants/sound.h"    /* SE_CURSOR, SE_CONFIRM, SE_DUEL_LOGO_SWING */
#include "util.h"               /* MemClear16, MemCopy16, Random, Timer_*, Tween*, gSineTable, gSquareTable */
#include "palette.h"            /* FadeStart, SetBldAlpha, SetBldY */
#include "sprite.h"             /* struct AnimSeq, OamListClear, ObjAffineInit, AnimBlockInit */
#include "link.h"               /* LinkSyncStart, LinkSyncStep */
#include "duel_scenes.h"        /* gSceneWork, struct TurnOrderSceneWork, ChoiceBob, TurnOrder_DrawUnusedSprite */
#include "turn_order.h"         /* the screen's enums, steps, phases and drawers, its graphics */

/* OamListAddSprite returns the entry; these drawers OR attr0 and attr1 into its first word in one go. */
#define OAM_ATTR01(attr0, attr1) (((attr1) << 16) | (attr0))

/* Fills `size` bytes at dest with the word `value` (CpuFastSet with a fixed source). */
#define CpuFastFill(value, dest, size)                                                  \
{                                                                                       \
    vu32 tmp = (vu32)(value);                                                           \
    CpuFastSet((void *)&tmp, dest, CPU_FAST_SET_SRC_FIXED | (((size) / 4) & 0x1FFFFF)); \
}

/* 16-colour OBJ palette n. */
#define OBJ_PAL(n) ((void *)(OBJ_PLTT + (n) * 0x20))

/* ---- Local views kept on purpose (matching choices, see build/readability/HEADERS.md) ---- */

/* OamListAddSprite as this unit calls it: every argument as a full word (the definition narrows to u8/u16,
 * which would add narrowing at these call sites), and the entry returned as a u32 * so attr0 and attr1 can be
 * ORed in as one word. */
extern u32 *OamListAddSpriteWide(u32 layer, u32 tile, s32 x, s32 y, u32 width, u32 height, u32 bpp, u32 palette,
                                 u32 unused, u32 attr0Flags, u32 attr1Bits, u32 priority, struct OamList *list)
    asm("OamListAddSprite");

/* MulFix8 with int parameters and result: the ROM neither narrows the arguments nor sign-extends the result
 * (util.h: s16 MulFix8(s16, s16)). */
extern s32 MulFix8Int(s32 a, s32 b) asm("MulFix8");

/* ---- ROM data used only here ---- */

/* 0x0808270C / 0x08082710: {0x318, 0x398} and {11, 12}: the tiles and palettes of the unused
 * TurnOrder_DrawUnusedSprite. */
extern const u16 gTurnOrderUnusedSpriteTiles[];
extern const u8 gTurnOrderUnusedSpritePals[];

/* The screen's own data, at gSceneWork + 0xAAC.
 * Matching: the phase functions reach it both through this macro and through a local `struct SceneWork *work =
 * &gSceneWork` (work->u.turnOrder); which accesses use the pointer is part of the match. */
#define sTurn gSceneWork.u.turnOrder

/* Bytes of gSceneWork the screen uses (0xB24), cleared by TurnOrder_Init. */
#define TURN_ORDER_WORK_SIZE (OFFSET_OF(struct SceneWork, u) + sizeof(struct TurnOrderSceneWork))

/* ---- Drawers ---- */

/* Draws the chosen FIRST (0) / SECOND (1) banner at x 0x58 for the DUEL logo phases: `tween` is the struct
 * Tween as halfwords, [0] the banner's vertical scale (matrix 4 scaleY; 0xC0 squashes it when the logo lands)
 * and [1] its downward offset: y = 0x50 + tween[1] - tween[0] / 8. Semi-transparent when blendMask has
 * BLEND_TURN_CHOICE. The other arguments are those of TurnOrder_DrawTurnChoiceConfirm, unused. */
void TurnOrder_DrawChosenTurnBanner(u32 unused0, u32 unused1, u16 blendMask, void *unused3, u8 choice, s16 *tween)
{
    u32 width = 0x40;
    u32 height = 0x20;
    u32 *oam;

    switch (choice) {
    case TURN_CHOICE_FIRST:
        oam = OamListAddSpriteWide(0, gTurnChoiceBannerTileNums[0], 0x58,
                                   tween[1] - (MulFix8Int(tween[0], 0x2000) >> 8) + 0x50,
                                   width, height, 4, 3, 0x200, 0, 0, 0, &gSceneWork.oamList);
        *oam |= (blendMask & BLEND_TURN_CHOICE) ? OAM_ATTR01(OAM_ATTR0_AFFINE | OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(4))
                                                : OAM_ATTR01(OAM_ATTR0_AFFINE, OAM_ATTR1_MATRIX(4));
        break;
    case TURN_CHOICE_SECOND:
        oam = OamListAddSpriteWide(0, gTurnChoiceBannerTileNums[1], 0x58,
                                   tween[1] - (MulFix8Int(tween[0], 0x2000) >> 8) + 0x50,
                                   width, height, 4, 3, 0x200, 0, 0, 0, &gSceneWork.oamList);
        *oam |= (blendMask & BLEND_TURN_CHOICE) ? OAM_ATTR01(OAM_ATTR0_AFFINE | OAM_ATTR0_BLEND, OAM_ATTR1_MATRIX(4))
                                                : OAM_ATTR01(OAM_ATTR0_AFFINE, OAM_ATTR1_MATRIX(4));
        break;
    }
    gSceneWork.aff[4].angle = 0;
    gSceneWork.aff[4].scaleX = 0x100;
    gSceneWork.aff[4].scaleY = tween[0];
}

/* Draws the three 64x64 pieces of the DUEL logo (double-size affine sprites on matrix 0, palette 10). The
 * pieces lie on a line through a pivot, turned by `swing` (256 steps per turn; it starts at 0xF4 = -12 and
 * swings back to 0) and spaced 64 * cos / 64 * sin apart; `drop` (0..16) lowers them along a quadratic curve
 * (gSquareTable). Matrix 0 turns each piece by `swing` as well. */
void TurnOrder_DrawDuelLogo(u8 unused, u8 swing, u8 drop)
{
    u8 i;
    s32 cosSwing, sinSwing, spreadX, spreadY, pivotY, x, y, four = 4;
    u32 *oam;

    for (i = 0; i < 3; i++) {
        cosSwing = gSineTable[swing + 0x40];
        spreadX = MulFix8Int(cosSwing, 0x4000);
        /* FAKEMATCH: `four` stops fold-const from reassociating (A*i - 4) - B into A*i - (B + 4) */
        four = 4;
        /* gSineTable[0xF4 + 0x40] and [0xF4]: cos and sin of the start angle, so the pivot terms are 0 there */
        x = (s16)(spreadX >> 8) * i - four - (MulFix8Int(0x60, cosSwing - gSineTable[0xF4 + 0x40]) >> 8);
        sinSwing = gSineTable[swing];
        spreadY = MulFix8Int(sinSwing, 0x4000);
        pivotY = MulFix8Int(0x60, sinSwing - gSineTable[0xF4]);
        y = (((spreadY + 0xA) >> 8) * i - (pivotY >> 8) + (MulFix8Int(0x4E0, gSquareTable[drop]) >> 4) - 0x5E)
            & 0xFFFF;
        oam = OamListAddSpriteWide(0, gDuelLogoTileNums[i], x, y, 0x40, 0x40, 4, 10, 0x200, 0, 0, 0,
                                   &gSceneWork.oamList);
        *oam |= OAM_ATTR0_AFFINE_DOUBLE;
        gSceneWork.aff[0].angle = swing << 8;
        gSceneWork.aff[0].scaleX = 0x100;
        gSceneWork.aff[0].scaleY = 0x100;
    }
}

/* Unreferenced: a 64x32 sprite (tile gTurnOrderUnusedSpriteTiles[index], palette gTurnOrderUnusedSpritePals[index])
 * at (0x58, 0x64). Nothing is loaded at tile 0x318 or into palettes 11 and 12: a leftover. */
void TurnOrder_DrawUnusedSprite(u8 index)
{
    u32 width = 0x40;
    u32 height = 0x20;
    OamListAddSpriteWide(0, gTurnOrderUnusedSpriteTiles[index], 0x58, 0x64, width, height, 4,
                         gTurnOrderUnusedSpritePals[index], 0x200, 0, 0, 0, &gSceneWork.oamList);
}

/* ---- Rules ---- */

/* The result for the player (enum RpsResult) of `player` against `opponent` (both enum RpsHand): rock beats
 * scissors, scissors beat paper, paper beats rock. If either hand is out of range it returns `player`. */
u8 JudgeRockPaperScissors(u8 player, u8 opponent)
{
    switch (opponent) {
    case RPS_ROCK:
        switch (player) {
        case RPS_ROCK: return RPS_DRAW;
        case RPS_SCISSORS: return RPS_LOSE;
        case RPS_PAPER: return RPS_WIN;
        }
        break;
    case RPS_SCISSORS:
        switch (player) {
        case RPS_ROCK: return RPS_WIN;
        case RPS_SCISSORS: return RPS_DRAW;
        case RPS_PAPER: return RPS_LOSE;
        }
        break;
    case RPS_PAPER:
        switch (player) {
        case RPS_ROCK: return RPS_LOSE;
        case RPS_SCISSORS: return RPS_WIN;
        case RPS_PAPER:
        {
            u8 result = RPS_DRAW;

            /* FAKEMATCH: preserve this initialized result as a separate
             * return block, as in the ROM's final draw case. */
            __asm__("" : "+r"(result));
            return result;
        }
        }
        break;
    }
    return player;
}

/* The highlighted banner's bob grows by 0x20 per frame (up to 0x800), the other's shrinks by 0x40 (down to
 * 0). `choice` is enum TurnChoice; `bob` is TurnOrderSceneWork.choiceBob. */
void TurnOrder_UpdateChoiceBob(u8 choice, struct ChoiceBob *bob)
{
    u8 which = choice;
    if ((bob[which].amp += 0x20) > 0x800)
        bob[which].amp = 0x800;
    which ^= 1;
    if ((bob[which].amp -= 0x40) < 0)
        bob[which].amp = 0;
}

/* The CPU's turn choice for the player when the player loses: the parity of the frame counter
 * (enum TurnChoice). */
u8 TurnOrder_CpuPickTurn(u8 frame)
{
    return frame & 1;
}

/* ---- Script steps ---- */

/* Step 0: clears the work area, has VBlank copy only OAM, resets the BG1-3 scroll, hides every layer, and sets
 * up the OAM list, the "Wait" sign animation (stopped), the affine records and the phase state. Returns 1. */
u16 TurnOrder_Init(void)
{
    MemClear16(&gSceneWork, TURN_ORDER_WORK_SIZE);
    gMain.vblankFlags = VBLANK_COPY_OAM;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= ~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
    OamListClear((u8 *)&gSceneWork.oamList);
    SetBldAlpha(8);
    sTurn.frame = 0;
    sTurn.phase = TURN_ORDER_CHOOSE_HAND;
    sTurn.turnChoice = TURN_CHOICE_NONE;
    sTurn.linkWaiting = 0;
    sTurn.rematchSend = 0;
    sTurn.rematchReady = 0;
    AnimBlockInit(gTurnOrderWaitAnimList, (u8 *)gSceneWork.anims);
    gSceneWork.anims[0].active = ANIM_FINISHED;
    ObjAffineInit(gSceneWork.aff);
    sTurn.logoTimer = 0;
    return 1;
}

/* Copies `rows` rows of `width` tiles from a linear 4bpp sheet to 2D-mapped OBJ VRAM at OBJ tile 0x200 + tile
 * (the OBJ tiles usable in bitmap mode 4; one row of the 2D map is 32 tiles, 0x400 bytes). */
void TurnOrder_LoadObjTiles(const u8 *src, u32 tile, u32 width, s32 rows)
{
    u8 *dst = (u8 *)(OBJ_VRAM0 + 0x4000) + tile * 32;
    s32 i;
    for (i = 0; i < rows; i++) {
        MemCopy16(dst, src, width * 32);
        dst += 0x400;
        src += width * 32;
    }
}

/* Step 1: starts the fade-in, loads the corridor backdrop (mode 4 bitmap), the OBJ palettes and tiles, and
 * resets the affine, carousel, banner and logo state; then sets DISPCNT to mode 4 (BG2 is the bitmap) with
 * the BG and OBJ layers on. Returns 1. */
u16 TurnOrder_Load(void)
{
    u8 i;

    FadeStart(FADE_BLACK, -0x180, 0, &sTurn.fade);
    MemCopy16((void *)VRAM, gEgyptCorridorBitmap, 240 * 160);
    MemCopy16((void *)BG_PLTT, gEgyptCorridorPal, 0x200);
    MemCopy16(OBJ_PAL(0), gRockCardPal, 0x20);
    MemCopy16(OBJ_PAL(1), gScissorsCardPal, 0x20);
    MemCopy16(OBJ_PAL(2), gPaperCardPal, 0x20);
    MemCopy16(OBJ_PAL(3), gTurnChoiceBannerPal, 0x20);
    MemCopy16(OBJ_PAL(4), gSelectCardBannerPal, 0x20);
    MemCopy16(OBJ_PAL(5), gWinBannerPal, 0x20);
    MemCopy16(OBJ_PAL(6), gLoseBannerPal, 0x20);
    MemCopy16(OBJ_PAL(7), gDrawBannerPal, 0x20);
    MemCopy16(OBJ_PAL(9), gTurnChoiceBannerDimPal, 0x20);
    MemCopy16(OBJ_PAL(10), gDuelLogoPal, 0x20);
    MemCopy16(OBJ_PAL(13), gWaitSignPal, 0x20);
    TurnOrder_LoadObjTiles(gRockCardTiles, 0, 4, 8);
    TurnOrder_LoadObjTiles(gScissorsCardTiles, 4, 4, 8);
    TurnOrder_LoadObjTiles(gPaperCardTiles, 8, 4, 8);
    TurnOrder_LoadObjTiles(gTurnChoiceBannerTiles, 0x100, 0x10, 4);
    TurnOrder_LoadObjTiles(gSelectCardBannerTiles, 0x180, 0x10, 4);
    TurnOrder_LoadObjTiles(gWinBannerTiles, 0x10, 0x10, 4);
    TurnOrder_LoadObjTiles(gLoseBannerTiles, 0x90, 0x10, 4);
    TurnOrder_LoadObjTiles(gWaitSignTiles, 0x110, 4, 4);
    TurnOrder_LoadObjTiles(gDrawBannerTiles, 0x190, 0x10, 4);
    for (i = 0; i < 5; i++) {
        gSceneWork.aff[i].scaleX = 0x100;
        gSceneWork.aff[i].scaleY = 0x100;
        gSceneWork.aff[i].angle = 0;
    }
    for (i = 0; i < 4; i++) {
        sTurn.scrollers[i].speed = 0;
        sTurn.scrollers[i].pos = 0;
        sTurn.scrollers[i].stop = RPS_ROCK;
    }
    for (i = 0; i < 2; i++)
        sTurn.choiceBob[i].amp = 0;
    sTurn.carouselSpread = 0;
    sTurn.carouselSpreadSpeed = 0;
    sTurn.opponentHand = 0xFF;
    sTurn.opponentAnswered = 0;
    sTurn.blendMask = 0;
    sTurn.logoSwing = -12;
    sTurn.logoDrop = 0;
    sTurn.brightness = 0;
    sTurn.brightnessStep = 0;
    REG_DISPCNT = DISPCNT_MODE_4 | DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
    return 1;
}

/* ---- Phases ---- */

/* TURN_ORDER_CHOOSE_HAND: Left/Right turn the carousel; A (once it has stopped) picks the hand facing the
 * player. Over the link the hand is exchanged with LINKMSG_RPS_HAND, with the "Wait" sign up until the
 * partner's arrives. Otherwise the CPU answers: the same hand 1 time in 5 (a draw), else a hand the player
 * beats or loses to with equal odds. Then the result is judged and the opponent's card slides in. Draws the
 * SELECT A CARD banner while the opponent's card is still. Returns 0 (the phase moves itself on). */
u16 TurnOrder_ChooseHand(void)
{
    struct SceneWork *work = &gSceneWork;

    if (sTurn.linkWaiting == 0) {
        if ((gMain.newKeys & A_BUTTON) && work->u.turnOrder.scrollers[SCROLLER_CAROUSEL].speed == 0) {
            if (work->u.turnOrder.isLink == 1) {
                work->u.turnOrder.linkWaiting = 1;
                LinkSyncStart((u8 *)&work->u.turnOrder.linkSync);
            } else {
                if (Random() % 500 < 100)
                    work->u.turnOrder.opponentHand = work->u.turnOrder.scrollers[SCROLLER_CAROUSEL].stop;
                else if (Random() & 1)  /* the hand the player's hand beats */
                    work->u.turnOrder.opponentHand = (work->u.turnOrder.scrollers[SCROLLER_CAROUSEL].stop + 1) % 3;
                else                    /* the hand that beats it */
                    work->u.turnOrder.opponentHand = (work->u.turnOrder.scrollers[SCROLLER_CAROUSEL].stop + 2) % 3;
                sTurn.opponentAnswered = 1;
                sTurn.result = JudgeRockPaperScissors(sTurn.scrollers[SCROLLER_CAROUSEL].stop, sTurn.opponentHand);
                sTurn.phase++;
                sTurn.scrollers[SCROLLER_OPPONENT_CARD].speed = 4;
                sTurn.rematchTimer = 0;
            }
            PlaySE(SE_CONFIRM);
        } else if ((gMain.newKeys & DPAD_LEFT) && sTurn.scrollers[SCROLLER_CAROUSEL].speed == 0) {
            sTurn.scrollers[SCROLLER_CAROUSEL].speed = -4;
            PlaySE(SE_CURSOR);
        } else if ((gMain.newKeys & DPAD_RIGHT) && sTurn.scrollers[SCROLLER_CAROUSEL].speed == 0) {
            sTurn.scrollers[SCROLLER_CAROUSEL].speed = 4;
            PlaySE(SE_CURSOR);
        }
    }
    if (sTurn.linkWaiting != 0) {
        if (LinkSyncStep(LINKMSG_RPS_HAND, work->u.turnOrder.scrollers[SCROLLER_CAROUSEL].stop,
                         &work->u.turnOrder.linkSync)) {
            sTurn.scrollers[SCROLLER_OPPONENT_CARD].speed = 4;
            sTurn.opponentAnswered = 1;
            sTurn.opponentHand = sTurn.linkSync.rx.data;
            sTurn.result = JudgeRockPaperScissors(sTurn.scrollers[SCROLLER_CAROUSEL].stop, sTurn.linkSync.rx.data);
            sTurn.phase++;
            sTurn.linkWaiting = 0;
            LinkSyncStart((u8 *)&work->u.turnOrder.linkSync);
            TurnOrder_HideWaitSign();
            sTurn.rematchTimer = 0;
        } else {
            TurnOrder_ShowWaitSign();
        }
    }
    if (sTurn.scrollers[SCROLLER_OPPONENT_CARD].speed == 0)
        TurnOrder_DrawBanner(BANNER_SELECT_CARD, sTurn.blendMask);
    return 0;
}

/* TURN_ORDER_SHOW_RESULT, once the opponent's card is fully in (slide 0x30):
 *   RPS_DRAW + A: the card slides out and the hands are chosen again (linked: rematchReady instead);
 *   RPS_WIN + A: the carousel, card and banner turn semi-transparent and the FIRST/SECOND choice starts
 *     with the cursor on FIRST;
 *   RPS_LOSE: linked, the partner's choice comes with LINKMSG_TURN_CHOICE (ours is the opposite); else on A
 *     the CPU picks (TurnOrder_CpuPickTurn) and the phase skips to TURN_ORDER_ANIMATE_CHOICE. Until then the
 *     banner tween is restarted and TurnOrder_AnimateTurnChoice runs every frame.
 * On a linked draw both games exchange LINKMSG_RPS_REMATCH (2 = ready); when either side is ready the hands
 * are chosen again. rematchTimer counts the frames on a draw and stops at 0xFF; from then on the exchange is
 * polled every frame. Returns 0. */
u16 TurnOrder_ShowResult(void)
{
    struct SceneWork *work = &gSceneWork;
    u8 *rematchSend = &sTurn.rematchSend;

    if (work->u.turnOrder.linkWaiting == 0) {
        if (work->u.turnOrder.scrollers[SCROLLER_OPPONENT_CARD].pos == 0x30) {
            if ((gMain.newKeys & A_BUTTON) && sTurn.result == RPS_DRAW) {
                if (sTurn.isLink == 1) {
                    work->u.turnOrder.rematchReady = 1;
                } else {
                    sTurn.scrollers[SCROLLER_OPPONENT_CARD].speed = -4;
                    sTurn.opponentAnswered = 0;
                    sTurn.phase--;
                }
                PlaySE(SE_CONFIRM);
            } else if ((gMain.newKeys & A_BUTTON) && sTurn.result == RPS_WIN) {
                sTurn.blendLevel = 0;
                sTurn.blendMask = BLEND_CAROUSEL | BLEND_OPPONENT_CARD | BLEND_BANNER;
                sTurn.phase++;
                REG_BLDALPHA = BLDALPHA_BLEND(16, 0);
                REG_BLDY = 8;
                REG_BLDCNT = BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG2;
                SetBldAlpha(sTurn.blendLevel);
                sTurn.turnChoice = TURN_CHOICE_FIRST;
                PlaySE(SE_CONFIRM);
            } else if (sTurn.result == RPS_LOSE) {
                if (sTurn.isLink == 1) {
                    sTurn.linkWaiting = 1;
                    LinkSyncStart((u8 *)&work->u.turnOrder.linkSync);
                } else if (gMain.newKeys & A_BUTTON) {
                    sTurn.turnChoice = TurnOrder_CpuPickTurn(sTurn.frame);
                    sTurn.phase += 2;
                    PlaySE(SE_CONFIRM);
                }
                TweenInit(0, 0, 0x40, 0xF, 3, 1, &sTurn.tween, TWEEN_APPROACH);
                TurnOrder_AnimateTurnChoice(&sTurn.phase);
            }
        }
    }
    if (sTurn.linkWaiting == 1) {
        TurnOrder_ShowWaitSign();
        if (LinkSyncStep(LINKMSG_TURN_CHOICE, work->u.turnOrder.scrollers[SCROLLER_CAROUSEL].stop,
                         &work->u.turnOrder.linkSync)) {
            /* Matching: the partner's choice is read as a byte of the received halfword */
            sTurn.turnChoice = 1 ^ *(u8 *)&sTurn.linkSync.rx.data;
            sTurn.phase += 2;
            sTurn.linkWaiting = 0;
            TurnOrder_HideWaitSign();
        }
    }
    if (sTurn.isLink == 1 && sTurn.result == RPS_DRAW) {
        if (LinkSyncStep(LINKMSG_RPS_REMATCH, *rematchSend, &work->u.turnOrder.linkSync)) {
            if (work->u.turnOrder.linkSync.rx.data == 2 || *rematchSend == 2) {
                sTurn.scrollers[SCROLLER_OPPONENT_CARD].speed = -4;
                sTurn.opponentAnswered = 0;
                sTurn.phase--;
                sTurn.linkWaiting = 0;
                *rematchSend = 0;
                work->u.turnOrder.rematchReady = 0;
            } else {
                LinkSyncStart((u8 *)&work->u.turnOrder.linkSync);
                if (work->u.turnOrder.rematchReady != 0)
                    *rematchSend = 2;
            }
        }
    }
    if (sTurn.result == RPS_DRAW) {
        if (++sTurn.rematchTimer == 0) {
            sTurn.rematchTimer = 0xFF;
            if (LinkSyncStep(LINKMSG_RPS_REMATCH, *rematchSend, &work->u.turnOrder.linkSync)) {
                sTurn.scrollers[SCROLLER_OPPONENT_CARD].speed = -4;
                sTurn.opponentAnswered = 0;
                sTurn.phase--;
                sTurn.linkWaiting = 0;
                *rematchSend = 0;
                work->u.turnOrder.rematchReady = 0;
            }
        }
    }
    return 0;
}

/* TURN_ORDER_CHOOSE_TURN (the player won): Left picks FIRST, Right SECOND; A confirms: the DUEL logo pieces
 * are loaded over the result banners and the banner tween starts. Over the link the choice is sent with
 * LINKMSG_TURN_CHOICE and the phase moves on when it has arrived. Every frame the blend level rises by 0x80
 * (up to 0x1000), fading the semi-transparent carousel, card and banner out. Returns 0. */
u16 TurnOrder_ChooseTurn(void)
{
    struct SceneWork *work = &gSceneWork;

    if (work->u.turnOrder.linkWaiting == 0) {
        if ((gMain.newKeys & DPAD_LEFT) && work->u.turnOrder.turnChoice == TURN_CHOICE_SECOND) {
            work->u.turnOrder.turnChoice = TURN_CHOICE_FIRST;
            PlaySE(SE_CURSOR);
        } else if ((gMain.newKeys & DPAD_RIGHT) && sTurn.turnChoice == TURN_CHOICE_FIRST) {
            sTurn.turnChoice = TURN_CHOICE_SECOND;
            PlaySE(SE_CURSOR);
        }
        if (gMain.newKeys & A_BUTTON) {
            sTurn.blendLevel = 0x1000;
            TurnOrder_LoadObjTiles(gDuelLogoTiles0, 0x10, 8, 8);
            TurnOrder_LoadObjTiles(gDuelLogoTiles1, 0x18, 8, 8);
            TurnOrder_LoadObjTiles(gDuelLogoTiles2, 0x110, 8, 8);
            TweenInit(0, 0, 0x40, 0xF, 3, 1, &sTurn.tween, TWEEN_APPROACH);
            if (sTurn.isLink == 1) {
                sTurn.linkWaiting = 1;
                LinkSyncStart((u8 *)&work->u.turnOrder.linkSync);
            } else {
                TurnOrder_AnimateTurnChoice(&sTurn.phase);
                sTurn.phase++;
            }
            PlaySE(SE_CONFIRM);
        }
    }
    sTurn.blendLevel += 0x80;
    if (sTurn.blendLevel > 0x1000)
        sTurn.blendLevel = 0x1000;
    SetBldAlpha(sTurn.blendLevel >> 8);
    if (sTurn.linkWaiting != 0 && LinkSyncStep(LINKMSG_TURN_CHOICE, sTurn.turnChoice, &work->u.turnOrder.linkSync)) {
        sTurn.phase++;
        sTurn.linkWaiting = 0;
        TurnOrder_AnimateTurnChoice(&sTurn.phase);
    }
    return 0;
}

/* TURN_ORDER_ANIMATE_CHOICE: steps the banner tween and draws the FIRST/SECOND banners (the chosen one moves
 * to the centre). When the tween is done, reloads the DUEL logo pieces, sets the tween up as the banner's
 * (scale 0x100, offset 0) for TurnOrder_ShowDuelLogo and advances *phase (TurnOrderSceneWork.phase).
 * Returns 0. */
u16 TurnOrder_AnimateTurnChoice(u8 *phase)
{
    TweenUpdate(&sTurn.tween);
    /* the bob and tween records go to the drawer as s16 arrays, as it is defined */
    TurnOrder_DrawTurnChoiceConfirm(sTurn.turnChoice, sTurn.frame, sTurn.blendMask, (s16 *)sTurn.choiceBob,
                                    sTurn.turnChoice, (s16 *)&sTurn.tween);
    if (sTurn.tween.state == TWEEN_DONE) {
        TurnOrder_LoadObjTiles(gDuelLogoTiles0, 0x10, 8, 8);
        TurnOrder_LoadObjTiles(gDuelLogoTiles1, 0x18, 8, 8);
        TurnOrder_LoadObjTiles(gDuelLogoTiles2, 0x110, 8, 8);
        TweenInit(0x100, 0, 0x100, 0, 1, 0, &sTurn.tween, TWEEN_APPROACH);
        (*phase)++;
    }
    return 0;
}

/* TURN_ORDER_DUEL_LOGO: the DUEL logo drops in over 16 frames, then swings from -12 to rest by 3 per frame
 * (SE_DUEL_LOGO_SWING as it starts; the music fades out at rest). On frame 15 of the drop the chosen banner
 * is squashed (scale 0xC0) and then springs back while it moves down 0x38. After 90 frames, or on A, the
 * white flash starts (BLDCNT brighten, brightness step 0x300). Returns 0. */
u16 TurnOrder_ShowDuelLogo(void)
{
    if (sTurn.logoDrop < 0x10)
        sTurn.logoDrop++;
    if (sTurn.logoDrop >= 0x10) {
        if (sTurn.logoSwing < 0) {
            if ((u8)sTurn.logoSwing == 0xF4)
                PlaySE(SE_DUEL_LOGO_SWING);
            sTurn.logoSwing += 3;
        } else {
            sTurn.logoSwing = 0;
            FadeOutBGM();
        }
    }
    /* Matching: the ROM compares the frame count as a double (__floatsidf, __eqdf2). */
    if (sTurn.logoTimer++ == 90.0 || (gMain.newKeys & A_BUTTON)) {
        SetBldY(sTurn.brightness);
        REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_LIGHTEN;
        sTurn.brightnessStep = 0x300;
        sTurn.phase++;
        Timer_Reset(&sTurn.timer);
        FadeOutBGM();
    }
    /* Matching: tween.y is tested as s16 (ldsh). */
    if (sTurn.logoDrop == 0xF && (s16)sTurn.tween.y == 0)
        sTurn.tween.x = 0xC0;
    if (sTurn.logoSwing > -10 && (s16)sTurn.tween.y == 0)
        TweenInit(0xC0, 0, 0x100, 0x38, 6, 0x14, &sTurn.tween, TWEEN_APPROACH);
    TweenUpdate(&sTurn.tween);
    TurnOrder_DrawChosenTurnBanner(sTurn.turnChoice, sTurn.frame, sTurn.blendMask, sTurn.choiceBob, sTurn.turnChoice,
                                   (s16 *)&sTurn.tween);
    TurnOrder_DrawDuelLogo(sTurn.turnChoice, sTurn.logoSwing, sTurn.logoDrop);
    return 0;
}

/* TURN_ORDER_FLASH_WHITE: the brightness rises by brightnessStep (0x300, then 0x100 from 0xC00). Past 0x10FF
 * the BG and OBJ palettes are filled with white and a 20-frame timer starts; when it expires the BLDCNT
 * effect switches to darken for the fade-out (brightness 0, step 0x60) and the phase moves on. Returns 0. */
u16 TurnOrder_FlashWhite(void)
{
    sTurn.brightness += sTurn.brightnessStep;
    if (sTurn.timer.state == TICK_DONE) {
        sTurn.timer.state = TICK_IDLE;
        sTurn.brightness = 0;
        sTurn.brightnessStep = 0x60;
        sTurn.phase++;
        SetBldY(0);
        REG_BLDCNT = BLDCNT_TGT1_ALL | BLDCNT_EFFECT_DARKEN;
        return 0;
    }
    if (sTurn.timer.state != TICK_RUNNING && sTurn.brightness == 0xC00)
        sTurn.brightnessStep = 0x100;
    if (sTurn.timer.state != TICK_RUNNING && sTurn.brightness > 0x10FF) {
        CpuFastFill(-1, (void *)BG_PLTT, 0x200);
        CpuFastFill(-1, (void *)OBJ_PLTT, 0x200);
        Timer_Start(&sTurn.timer, 20);
    }
    SetBldY(sTurn.brightness >> 8);
    TurnOrder_DrawDuelLogo(sTurn.turnChoice, sTurn.logoSwing, sTurn.logoDrop);
    Timer_Tick(&sTurn.timer);
    return 0;
}
