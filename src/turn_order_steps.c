/*
 * turn_order_steps (0x08029750-0x0802AABF): the end of the pre-duel turn-order screen, then the drawing helpers
 * and the screen setup of the duel card-list viewer.
 *
 * Turn order (turn_order.h; the other steps and phases are in turn_order_scene.c, the sprite drawers in
 * destiny_board_scene.c). Its work area is gSceneWork.u.turnOrder (struct TurnOrderSceneWork). Four runners
 * step through a NULL-terminated script indexed by gMain.seqIndex1 and return 1 at its end:
 *   TurnOrder_RunRps / RunRpsLink  gTurnOrderRpsSteps: TurnOrder_Init, TurnOrder_Load, TurnOrder_NopStep,
 *                                  TurnOrder_RpsMain (rock-paper-scissors, then the winner picks FIRST/SECOND)
 *   TurnOrder_RunPlayerChoice(Link) gTurnOrderPlayerChoiceSteps: TurnOrder_InitChoice, TurnOrder_Load,
 *                                  TurnOrder_ChoiceMain (the player picks FIRST/SECOND directly)
 *   TurnOrder_RunCpuChoice         gTurnOrderCpuChoiceSteps: TurnOrder_InitChoice, TurnOrder_CpuChooseTurn,
 *                                  TurnOrder_Load, TurnOrder_ChoiceMain (the opponent's random pick is shown)
 * The two main steps run a phase table every frame: gTurnOrderRpsSubsteps (enum TurnOrderPhase) or
 * gTurnOrderChoiceSubsteps (the same list with TurnOrder_ShowChoice inserted at index 3).
 *
 * Card-list viewer (card_list_view.h; runner, input and Open in card_list_viewer.c): the cursor box, the four
 * name rows, the CARD PROPERTY and CARD STATUS panels, the button bar and CardListView_InitScreen.
 * Screen layout: BG0 (screenblock 0) the names, the title and the panel texts; BG1 (screenblock 1) the
 * cursor box, moved by scrolling BG1; BG2 (screenblock 2) the background images; BG3 (screenblock 3) the
 * 8bpp card portrait; OBJ the buttons and the status icons.
 */
#include "global.h"
#include "legacy/gba.h"                /* REG_*, keys, VRAM / palette addresses */
#include "constants/card_stats.h" /* enum CardType */
#include "constants/duel.h"     /* enum BanishKind */
#include "util.h"               /* MemCopy16, CopyDoubleWords, Random, struct Tween, TweenInit */
#include "palette.h"            /* struct Fade, FadeTick, SetBldAlpha, SetBldY, SetBrightnessBlack */
#include "bg.h"                 /* FillMapRect, DrawCardPortrait, LoadBgImage4bpp*, ClearBgMapBuffers, ResetVideo */
#include "sprite.h"             /* OamList*, ObjAffine*, AnimStateTick, AnimBlockDraw, AddSprite */
#include "text.h"               /* TextCanvas*, TextDraw*Glyph, DrawBgDecimal */
#include "save.h"               /* gSaveData.sjisText */
#include "card_data.h"          /* gCardNames, icon image tables, CARD_ID_MASK, CARD_STATS_* */
#include "link.h"               /* struct LinkSync, LinkSyncStart, LinkSyncStep */
#include "main_menu.h"          /* CB_MainMenu */

/* ---- Names the legacy gba.h lacks (until H0 installs the new one; build/readability/HEADERS.md) ---- */

/* Values as in the new gba.h; this block compiles away once it is installed. */
#ifndef DISPCNT_BG_ALL_ON
#define BG_VRAM                 0x06000000
#define BG_CHAR_SIZE            0x4000
#define BG_CHAR_ADDR(n)         (BG_VRAM + BG_CHAR_SIZE * (n))
#define DISPCNT_BG_ALL_ON       0x0F00
#define DISPCNT_OBJ_ON          0x1000
#define BGCNT_PRIORITY(n)       (n)
#define BGCNT_CHARBASE(n)       ((n) << 2)
#define BGCNT_256COLOR          0x0080
#define BGCNT_SCREENBASE(n)     ((n) << 8)
#define BLDCNT_EFFECT_BLEND     0x0040
#define BLDCNT_TGT2_BG2         0x0400
#define DMA_SRC_FIXED           0x0100
#define DMA_16BIT               0x0000
#define DMA_ENABLE              0x8000
#endif

/* ---- BEGIN header subset (pre-H0) ----
 * The parts of main.h and duel.h this unit and the headers below need, with the canonical headers' tags, names,
 * types and bitfield containers (unused bytes are padding). include/main.h and duel.h still hold the legacy
 * headers until the header switch (H0, build/readability/HEADERS.md); card_list_view.h and duel_screen.h
 * include duel.h, so this block also defines duel.h's include guard. After H0, replace the block (BEGIN to END)
 * with
 *     #include "legacy/main.h"
 *     #include "legacy/duel.h"
 * which gives identical assembly (checked against the staged headers). */

/* main.h */
enum VBlankFlag {
    VBLANK_COPY_OAM     = 0x1,          /* gMain.oamBuffer -> OAM */
    VBLANK_COPY_BG_MAPS = 0x2,          /* gMain.bgMapBuffer -> VRAM screenblocks 0-7 */
    VBLANK_BG0_VOFS     = 0x100,        /* gMain.bgVofs[0] -> REG_BG0VOFS */
    VBLANK_BG1_VOFS     = 0x200
};
struct Main {
    u32 rngState;                       /* +0x0000 */
    u16 heldKeys;                       /* +0x0004 */
    u16 newKeys;                        /* +0x0006 newly pressed, plus D-pad auto-repeat */
    u8 unk8[0x40E - 0x8];
    u16 vblankFlags;                    /* +0x040E enum VBlankFlag */
    u8 unk410[0x41C - 0x410];
    u16 bgMapBuffer[8][0x400];          /* +0x041C screenblocks 0-7, copied with VBLANK_COPY_BG_MAPS */
    u8 unk441C[0x4420 - 0x441C];
    u16 bgVofs[4];                      /* +0x4420 BG0-3 VOFS shadows */
    u8 unk4428[0x4859 - 0x4428];
    u8 seqIndex1;                       /* +0x4859 step index of the screen runners */
    u8 unk485A[0x4870 - 0x485A];
    u8 firstPlayer:1;                   /* +0x4870 bit 0: who takes the first turn, 0 = this player */
    u8 opponent:5;                      /* +0x4870 bits 1-5 */
    u8 result:2;                        /* +0x4870 bits 6-7 */
};
extern struct Main gMain;
void SetMainCallback(u16 (*callback)(void));
void ResetBgScroll(void);

/* duel.h */
#define GUARD_DUEL_H
struct DuelCard {
    u32 id:12;                          /* bits 0-11: card ID; 0 = empty slot */
    u32 owner:1;                        /* bit 12: owning player */
    u32 unk13:19;
};
struct DuelLoc {
    u16 player:1;
    u16 area:4;
    u16 index:9;
    u16 isDefense:1;
    u16 isFaceUp:1;
    u16 unk2;
};
struct DuelPlayer {
    u8 unk0[0x684];
    struct DuelCard hand[80];           /* +0x684 */
    struct DuelCard deck[80];           /* +0x7C4 */
    struct DuelCard graveyard[80];      /* +0x904 */
    struct DuelCard fusionDeck[80];     /* +0xA44 */
    struct DuelCard banished[80];       /* +0xB84 */
    u16 banishedInfo[80];               /* +0xCC4: parallel to banished[]: low byte enum BanishKind */
};
u32 IsSpecialSummonOnly(u16 cardId);
/* ---- END header subset ---- */

#include "duel_screen.h"        /* gDuelScreen, DuelScreen_FadeOutStep, TextDrawShadowedString */
#include "card_list_view.h"     /* gCardListView, the CardListView_* drawers defined here */
#include "duel_scenes.h"        /* gSceneWork, struct TurnOrderSceneWork */
#include "turn_order.h"         /* turn-order enums, steps and sprite drawers, tile tables */

/* ---- Local views kept on purpose (matching choices, see build/readability/HEADERS.md) ---- */

/* The fades as this unit calls them: returning u16, so each caller truncates the result (lsl #16) before
 * testing it. palette.h has the definitions' u32 return. */
u16 FadeFromBlackU16(s32 step) asm("FadeFromBlack");

/* DrawCardPortrait with word parameters: the portrait tile and palette arguments are computed and passed
 * unnarrowed (the definition takes u16). */
void DrawCardPortraitWide(u32 screenBlock, u32 cell, u32 cardId, u32 tileBase, u32 palBase) asm("DrawCardPortrait");

/* IsSpecialSummonOnly with a word parameter (duel.h: u16). Matching: CardListView_DrawCardStatus passes the
 * 12-bit card ID unnarrowed, sharing its shift with the card-table index. */
u32 IsSpecialSummonOnlyWide(u32 cardId) asm("IsSpecialSummonOnly");

/* Byte views of the banishedInfo arrays: the code reads the low byte (enum BanishKind) of an entry with ldrb
 * at byte offsets it computes itself. gDuelBanishedInfo is gDuelPlayers[0].banishedInfo (0x02019FA8). */
extern u8 gDuelPlayersBytes[] asm("gDuelPlayers");
extern u8 gDuelBanishedInfoBytes[] asm("gDuelBanishedInfo");

/* gCardListView's flag byte with u16 bitfield containers (in a struct the size of the viewer's first 12
 * bytes). Matching: with these, CardListView_DrawCardInfo computes the portraitPage toggle 1 - x as a
 * subtraction masked to one bit (subs; ands), as in the ROM; with the header's u8 bitfields it becomes an eor. */
struct CardListViewFlags16 {
    u16 active:1;
    u16 player:1;
    u16 textDirty:1;
    u16 portraitPage:1;
    u16 showSprites:1;
    u16 mode:3;
    u8 unk1[11];
};
/* A cast, not an asm("gCardListView") alias: the function also reads gCardListView directly, and the two
 * symbol forms would get two literal-pool entries. */
#define gCardListViewFlags16 (*(struct CardListViewFlags16 *)&gCardListView)

/* gMain.bgMapBuffer[0] (BG0's map, 0x0300045C) through its own symbol: the title and the level stars are
 * written through this literal, not through gMain + offset. */
extern u16 gBgMaps[];

/* ---- ROM data used only here ---- */

/* Step scripts of the runners (NULL-terminated). */
extern u16 (*const gTurnOrderRpsSteps[])(void);             /* 0x0819A718 */
extern u16 (*const gTurnOrderPlayerChoiceSteps[])(void);    /* 0x0819A72C */
extern u16 (*const gTurnOrderCpuChoiceSteps[])(void);       /* 0x0819A73C */
/* Phase tables (NULL-terminated). The main steps call each phase with &TurnOrderSceneWork.phase and test only
 * the low byte of its result (the phases are defined returning u16). */
extern u8 (*const gTurnOrderRpsSubsteps[])(u8 *phase);      /* 0x0819A6B0: enum TurnOrderPhase */
extern u8 (*const gTurnOrderChoiceSubsteps[])(u8 *phase);   /* 0x0819A6D0: ShowChoice at index 3 */

/* gTurnOrderChoiceSubsteps index of TurnOrder_ShowChoice (the later phases are one index higher than in
 * enum TurnOrderPhase). */
#define CHOICE_PHASE_SHOW_CHOICE 3

/* 0x08082703: gHandCardPalNums {0, 1, 2}, the OBJ palette of each hand card. Matching: the table is passed as
 * an integer address; it starts at an odd address, and the symbol form does not match. */
#define HAND_CARD_PAL_NUMS ((u8 *)0x08082703)

/* Card-list viewer graphics. */
extern const u8 gCardListViewButtonsPal[];                /* OBJ palette 0 of the buttons */
extern const u8 gCardListViewButtonsGfx[];      /* 0x08698C9C: Card View / Exit / Arrange / Decide buttons */
extern const u8 gCardListViewStatusIconsPal[];                /* OBJ palette 1 of the status icons */
extern const u8 gCardListViewStatusIconsGfx[];  /* 0x0869AD3C: 16x16 CARD STATUS icons */
extern const u8 gCardListViewTitlesPal[];                /* BG palette 7 of the titles */
extern const u8 gCardListViewTitlesGfx[];       /* 0x0869B55C: five 96x16 titles, 0x300 bytes each, by mode */
/* The image packs are declared u16 [] (not const) because the bg.h loaders take a u16 *. */
extern u16 gCardListViewBgImage[];                     /* image pack: 240x144 list background */
extern u16 gCardListViewInfoPanelImage[];       /* 0x0869D758: image pack: CARD PROPERTY / CARD STATUS panel */
extern u16 gCardListViewHeaderImage[];                     /* image pack: 240x16 header strip */
extern const u8 gCardListViewCursorFramePal[];                /* BG palette 4 of the cursor box */
extern const u8 gCardListViewCursorFrameGfx[];  /* 0x0869EEEC: cursor boxes, 0x60 tiles each: OPPONENT, YOU */
extern const u16 gStrCardListViewUnknown[];     /* 0x0808275C: "[ Unknown ]" */
extern const u16 gStrCardListViewNoCards[];     /* 0x08082768: "There are no cards." */

/* ---- Helpers ---- */

/* The turn-order screen's part of gSceneWork. */
#define sTurnOrder (gSceneWork.u.turnOrder)

/* Bytes of gSceneWork the turn-order screen uses (0xB24), cleared by TurnOrder_InitChoice. */
#define TURN_ORDER_WORK_SIZE (OFFSET_OF(struct SceneWork, u) + sizeof(struct TurnOrderSceneWork))

/* REG_DMA3CNT as one word: the count in bits 0-15, the control bits (DMA_*) in bits 16-31. */
#define DMA_CNT(control, count) (((u32)(control) << 16) | (count))

/* A cell of BG1's map buffer (screenblock 1, 32 cells per row): the cursor box. */
#define CURSOR_CELL(row, col) (gMain.bgMapBuffer[1][(row) * 32 + (col)])

/* The size << 8 | colour argument of the TextDraw* glyph functions. */
#define SIZE_COLOR(size, color) ((u16)(((u8)(size) << 8) | (color)))

/* Packed arguments of DrawBgDecimal: cell | colors << 16 and firstTile | digits << 16. */
#define DECIMAL_AT(cell, colors) ((cell) | ((colors) << 16))
#define DECIMAL_TILES(tile, digits) ((tile) | ((digits) << 16))

/* A 4bpp tile of BG character block 1 (the char base of all four viewer layers) and of the OBJ tile area. */
#define BG_CHAR1_TILE(tile) ((u8 *)BG_CHAR_ADDR(1) + (tile) * 32)
#define OBJ_TILE(tile) ((u8 *)OBJ_VRAM0 + (tile) * 32)
/* A 16-colour palette of BG / OBJ palette RAM. */
#define BG_PAL(n) ((u8 *)BG_PLTT + (n) * 32)
#define OBJ_PAL(n) ((u8 *)OBJ_PLTT + (n) * 32)

/* Byte 0 of gCardListView (active, player, textDirty, portraitPage, showSprites, mode) as a whole byte, and a
 * mode in place in it (bits 5-7). Matching: where the code keeps the masked byte (& 0xE0) in a variable, the
 * mode bitfield would be shifted down instead. */
#define CARDLIST_FLAGS (*(u8 *)&gCardListView)
#define MODE_BITS(mode) ((mode) << 5)

/* ======================================================================================================== */
/* Turn-order screen                                                                                        */
/* ======================================================================================================== */

/*
 * TURN_ORDER_FADE_OUT, the last phase of both phase tables. The white flash before it filled the palettes
 * with white and set BLDCNT to darken all layers, so raising BLDY by brightnessStep each frame fades the
 * screen from white to black. Once the level passes 0x1000 the choice is latched into gMain.firstPlayer
 * (TURN_CHOICE_FIRST = 0 = this player starts) and the phase returns 1, which ends the screen.
 */
u16 TurnOrder_FadeOutAndSetFirstPlayer(void)
{
    struct SceneWork *work = &gSceneWork;

    work->u.turnOrder.brightness += work->u.turnOrder.brightnessStep;
    if (work->u.turnOrder.brightness > 0x1000) {
        gMain.firstPlayer = work->u.turnOrder.turnChoice;
        return 1;
    }
    SetBldY(work->u.turnOrder.brightness >> 8);
    return 0;
}

/*
 * Step 3 of gTurnOrderRpsSteps, every frame. Ticks the scene fade: when it has faded out the script moves on by
 * fade.param, when it has faded in the OBJs blend over the corridor bitmap (BG2) and the carousel cards start
 * to spread out. When neither is running, the current phase (gTurnOrderRpsSubsteps[phase]) runs; the step
 * returns 1 when it reports the end. While the cards are on screen (phase <= TURN_ORDER_CHOOSE_TURN) it draws
 * the opponent's card, the player's carousel, the result banner once the opponent's card is in, and the
 * FIRST/SECOND banners once a choice is up; then the affine sets, the link "Wait" sign, the OAM list and the
 * scrollers.
 */
u16 TurnOrder_RpsMain(void)
{
    struct Fade *fade = &sTurnOrder.fade;
    u8 i;

    FadeTick(fade);
    if (fade->state == FADE_STATE_FADED_OUT)
        gMain.seqIndex1 += fade->param;
    if (fade->state == FADE_STATE_FADED_IN) {
        REG_BLDCNT = BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG2;
        fade->state = FADE_STATE_IDLE;
        sTurnOrder.carouselSpreadSpeed = 6;
    }
    if (fade->state == FADE_STATE_IDLE && sTurnOrder.carouselSpreadSpeed == 0) {
        if (gTurnOrderRpsSubsteps[sTurnOrder.phase](&sTurnOrder.phase))
            return 1;
    }
    if (sTurnOrder.carouselSpreadSpeed != 0) {
        sTurnOrder.carouselSpread += sTurnOrder.carouselSpreadSpeed;
        if (sTurnOrder.carouselSpread > 0x54) {
            /* 0x55: a third of a turn, the three cards evenly spaced */
            sTurnOrder.carouselSpread = 0x55;
            sTurnOrder.carouselSpreadSpeed = 0;
        }
    }
    if (sTurnOrder.phase <= TURN_ORDER_CHOOSE_TURN) {
        TurnOrder_DrawOpponentCard(sTurnOrder.opponentHand, sTurnOrder.scrollers[SCROLLER_OPPONENT_CARD].pos,
                                   sTurnOrder.blendMask);
        TurnOrder_DrawHandCarousel((u16 *)gHandCardTileNums, HAND_CARD_PAL_NUMS,
                                   sTurnOrder.scrollers[SCROLLER_CAROUSEL].pos,
                                   sTurnOrder.scrollers[SCROLLER_CAROUSEL].stop,
                                   sTurnOrder.scrollers[SCROLLER_OPPONENT_CARD].pos, sTurnOrder.blendMask,
                                   sTurnOrder.carouselSpread);
        /* The opponent's card has slid all the way in (0x30): show the result. */
        if (sTurnOrder.scrollers[SCROLLER_OPPONENT_CARD].pos == 0x30 && sTurnOrder.opponentAnswered == 1
            && sTurnOrder.opponentHand != 0xFF) {
            if (sTurnOrder.result == RPS_WIN)
                TurnOrder_DrawBanner(BANNER_WIN, sTurnOrder.blendMask);
            else if (sTurnOrder.result == RPS_LOSE)
                TurnOrder_DrawBanner(BANNER_LOSE, sTurnOrder.blendMask);
            else if (sTurnOrder.result == RPS_DRAW)
                TurnOrder_DrawBanner(BANNER_DRAW, sTurnOrder.blendMask);
        }
        if (sTurnOrder.turnChoice != TURN_CHOICE_NONE) {
            TurnOrder_UpdateChoiceBob(sTurnOrder.turnChoice, sTurnOrder.choiceBob);
            TurnOrder_DrawTurnChoice(sTurnOrder.turnChoice, sTurnOrder.frame, sTurnOrder.blendMask,
                                     (s16 *)sTurnOrder.choiceBob, sTurnOrder.turnChoice);
        }
    }
    for (i = 0; i <= 4; i++)
        ObjAffineApply(&gSceneWork.aff[i]);
    /* anims[0] is the link "Wait" sign (TurnOrder_ShowWaitSign / HideWaitSign), drawn with OBJ palette 1.
     * Matching: its state is read sign-extended (ldrsb; AnimState.active is u8). */
    if ((s8)gSceneWork.anims[0].active == ANIM_PLAYING) {
        AnimStateTick(&gSceneWork.anims[0]);
        AnimBlockDraw((u8 *)gSceneWork.anims, 0, 0, 1, 1, 0, 0, 0, 0, (u32)&gSceneWork.oamList);
    }
    OamListFlush(&gSceneWork.oamList);
    OamListClear((u8 *)&gSceneWork.oamList);
    Scroller_Move(sTurnOrder.scrollers);
    Scroller_SnapToStop(&sTurnOrder.scrollers[SCROLLER_CAROUSEL]);
    Scroller_StopAtEnds(&sTurnOrder.scrollers[SCROLLER_OPPONENT_CARD]);
    sTurnOrder.frame++;
    return 0;
}

/*
 * Phase 3 of gTurnOrderChoiceSubsteps, one frame: draws the chosen FIRST/SECOND banner starting its flight to
 * the centre (driven by the tween that TurnOrder_ChoiceMain or TurnOrder_CpuChooseTurn started) and moves on
 * to the next phase. Returns 0.
 */
u16 TurnOrder_ShowChoice(void)
{
    TurnOrder_DrawTurnChoiceConfirm(sTurnOrder.turnChoice, sTurnOrder.frame, sTurnOrder.blendMask,
                                    (s16 *)sTurnOrder.choiceBob, sTurnOrder.turnChoice, (s16 *)&sTurnOrder.tween);
    sTurnOrder.phase++;
    return 0;
}

/*
 * Step 0 of the choice-only scripts: clears the turn-order part of gSceneWork (0xB24 bytes) with a DMA3 fill,
 * copies only the OAM buffer in VBlank, zeroes the BG1-3 scroll, hides BG0-3 and the OBJs, clears the OAM
 * list, sets the OBJ blend to 8/16, and starts at the FIRST/SECOND choice (cursor on FIRST). Like
 * TurnOrder_Init, without the rock-paper-scissors setup. Returns 1.
 */
u16 TurnOrder_InitChoice(void)
{
    struct SceneWork *work;

    /* DMA3 16-bit fill with zero, waiting until it is done. */
    {
        vu16 zero = 0;
        /* FAKEMATCH: the DMA register pointer pinned to r1, as in the ROM (unpinned, r1 keeps the copy of sp
         * and the pointer goes to r3). */
        register vu32 *dma asm("r1") = &REG_DMA3SAD;

        dma[0] = (u32)&zero;
        dma[1] = (u32)&gSceneWork;
        dma[2] = DMA_CNT(DMA_ENABLE | DMA_SRC_FIXED | DMA_16BIT, TURN_ORDER_WORK_SIZE / 2);
        dma[2];
        while (dma[2] & DMA_CNT(DMA_ENABLE, 0))
            ;
    }
    work = &gSceneWork;
    gMain.vblankFlags = VBLANK_COPY_OAM;
    REG_BG1VOFS = 0;
    REG_BG1HOFS = 0;
    REG_BG2VOFS = 0;
    REG_BG2HOFS = 0;
    REG_BG3VOFS = 0;
    REG_BG3HOFS = 0;
    REG_DISPCNT &= (u16)~(DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON);
    OamListClear((u8 *)&work->oamList);
    SetBldAlpha(8);
    work->u.turnOrder.frame = 0;
    work->u.turnOrder.phase = TURN_ORDER_CHOOSE_TURN;
    work->u.turnOrder.turnChoice = TURN_CHOICE_FIRST;
    ObjAffineInit(work->aff);
    return 1;
}

/*
 * Last step of the choice-only scripts, every frame. At TURN_ORDER_CHOOSE_TURN Left/Right move the choice to
 * FIRST/SECOND and A starts the banner tween; in a link duel the choice is then sent and the phase moves on
 * when the exchange completes, otherwise at once. (The phase TurnOrder_ChooseTurn then finds the cursor
 * already moved.) Then the scene fade, the affine sets 0-3 at half size, the current phase
 * (gTurnOrderChoiceSubsteps[phase]; returns 1 when it reports the end), the FIRST/SECOND banners up to
 * TurnOrder_ShowChoice, and the OAM list.
 */
u16 TurnOrder_ChoiceMain(void)
{
    struct SceneWork *top = &gSceneWork;
    /* Matching: a second pointer for the code after the affine loop. Assigned there, it is hoisted into the
     * loop preheader and formed from the fade pointer (fade - 0xAF8), as in the ROM; one pointer for the whole
     * function does not match. */
    struct SceneWork *work;
    struct Fade *fade;
    u8 i;

    if (top->u.turnOrder.phase == TURN_ORDER_CHOOSE_TURN) {
        if (gMain.newKeys & DPAD_LEFT)
            top->u.turnOrder.turnChoice = TURN_CHOICE_FIRST;
        else if (gMain.newKeys & DPAD_RIGHT)
            top->u.turnOrder.turnChoice = TURN_CHOICE_SECOND;
        if (gMain.newKeys & A_BUTTON) {
            TweenInit(0, 0, 0x40, 0xF, 3, 1, &sTurnOrder.tween, TWEEN_APPROACH);
            if (sTurnOrder.isLink == 1) {
                sTurnOrder.linkWaiting = 1;
                LinkSyncStart((u8 *)&top->u.turnOrder.linkSync);
            } else {
                sTurnOrder.phase++;
            }
        }
        if (sTurnOrder.linkWaiting != 0) {
            /* Sent under LINKMSG_RPS_HAND (0x51) although it carries the turn choice. This script's only link
             * runner, TurnOrder_RunPlayerChoiceLink, has no callers. */
            if (LinkSyncStep(LINKMSG_RPS_HAND, sTurnOrder.turnChoice, &top->u.turnOrder.linkSync)) {
                sTurnOrder.phase++;
                sTurnOrder.linkWaiting = 0;
            }
        }
    }
    fade = &sTurnOrder.fade;
    FadeTick(fade);
    if (fade->state == FADE_STATE_FADED_OUT)
        gMain.seqIndex1 += fade->param;
    for (i = 0; i <= 3; i++) {
        gSceneWork.aff[i].angle = 0;
        gSceneWork.aff[i].scaleX = 0x80;
        gSceneWork.aff[i].scaleY = 0x80;
    }
    work = &gSceneWork;
    if (gTurnOrderChoiceSubsteps[work->u.turnOrder.phase](&work->u.turnOrder.phase))
        return 1;
    if (work->u.turnOrder.phase <= CHOICE_PHASE_SHOW_CHOICE) {
        if (work->u.turnOrder.turnChoice != TURN_CHOICE_NONE) {
            TurnOrder_UpdateChoiceBob(work->u.turnOrder.turnChoice, work->u.turnOrder.choiceBob);
            TurnOrder_DrawTurnChoice(work->u.turnOrder.turnChoice, work->u.turnOrder.frame,
                                     work->u.turnOrder.blendMask, (s16 *)work->u.turnOrder.choiceBob,
                                     work->u.turnOrder.turnChoice);
        }
    }
    for (i = 0; i <= 4; i++)
        ObjAffineApply(&gSceneWork.aff[i]);
    OamListFlush(&gSceneWork.oamList);
    OamListClear((u8 *)&gSceneWork.oamList);
    sTurnOrder.frame++;
    return 0;
}

/* Step 2 of gTurnOrderRpsSteps: does nothing and returns 1. */
u16 TurnOrder_NopStep(void)
{
    return 1;
}

/*
 * Step 1 of gTurnOrderCpuChoiceSteps: copies the three 64x64 DUEL logo pieces to OBJ tiles 0x210, 0x218 and
 * 0x310, starts the banner tween, picks FIRST or SECOND at random and skips the input (phase =
 * TurnOrder_ShowChoice). Returns 1.
 */
u16 TurnOrder_CpuChooseTurn(void)
{
    TurnOrder_LoadObjTiles(gDuelLogoTiles0, 0x10, 8, 8);
    TurnOrder_LoadObjTiles(gDuelLogoTiles1, 0x18, 8, 8);
    TurnOrder_LoadObjTiles(gDuelLogoTiles2, 0x110, 8, 8);
    TweenInit(0, 0, 0x40, 0xF, 3, 1, &sTurnOrder.tween, TWEEN_APPROACH);
    sTurnOrder.phase = CHOICE_PHASE_SHOW_CHOICE;
    sTurnOrder.turnChoice = Random() & 1;
    return 1;
}

/*
 * Runner of the rock-paper-scissors screen (Campaign, the first duel of a match): clears the link flag and runs
 * gTurnOrderRpsSteps[gMain.seqIndex1], moving to the next step when it returns non-zero. Returns 1 at the end
 * of the script, else 0.
 */
u16 TurnOrder_RunRps(void)
{
    u16 (*step)(void);

    sTurnOrder.isLink = 0;
    step = gTurnOrderRpsSteps[gMain.seqIndex1];
    if (step != NULL) {
        if (step())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}

/*
 * Link Battle runner of the rock-paper-scissors screen: as TurnOrder_RunRps with the link flag set (the steps
 * then exchange the picks over the cable); B goes back to the main menu.
 */
u16 TurnOrder_RunRpsLink(void)
{
    u16 (*step)(void);

    sTurnOrder.isLink = 1;
    step = gTurnOrderRpsSteps[gMain.seqIndex1];
    if (step != NULL) {
        if (gMain.newKeys & B_BUTTON) {
            SetMainCallback(CB_MainMenu);
            return 0;
        }
        if (step())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}

/* Runner of the choice-only screen in which the player picks FIRST/SECOND (Campaign, after the player lost the
 * previous duel of the match). Returns 1 at the end of gTurnOrderPlayerChoiceSteps. */
u16 TurnOrder_RunPlayerChoice(void)
{
    u16 (*step)(void);

    step = gTurnOrderPlayerChoiceSteps[gMain.seqIndex1];
    if (step != NULL) {
        if (step())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}

/* TurnOrder_RunPlayerChoice with the link flag set. No callers. */
u16 TurnOrder_RunPlayerChoiceLink(void)
{
    u16 (*step)(void);

    sTurnOrder.isLink = 1;
    step = gTurnOrderPlayerChoiceSteps[gMain.seqIndex1];
    if (step != NULL) {
        if (step())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}

/* Runner of the choice-only screen that shows the opponent's random FIRST/SECOND pick (Campaign, the other
 * duels of a match). Returns 1 at the end of gTurnOrderCpuChoiceSteps. */
u16 TurnOrder_RunCpuChoice(void)
{
    u16 (*step)(void);

    step = gTurnOrderCpuChoiceSteps[gMain.seqIndex1];
    if (step != NULL) {
        if (step())
            gMain.seqIndex1++;
        return 0;
    }
    return 1;
}

/* ======================================================================================================== */
/* Card-list viewer: drawing and screen setup                                                                */
/* ======================================================================================================== */

/*
 * Draws the cursor box into BG1's map buffer, rows 2-4, palette 4: the 8-tile owner tab, the top edge, the
 * corners and sides, and the bottom edge. isOpponent selects the frame: YOU (blue, BG tiles 0x169-0x177) or
 * OPPONENT (pink, 0x15A-0x168), both loaded by CardListView_InitScreen. Moving the cursor only scrolls BG1
 * (gMain.bgVofs[1]).
 */
void CardListView_DrawCursorFrame(u32 isOpponent)
{
    u16 tile = 0x169;
    u16 *cell;
    register u8 *base asm("r0");
    s32 i;

    if (isOpponent == 1)
        tile -= 15;
    tile += 0x4000;     /* palette 4 */
    for (i = 0; i < 8; i++)
        CURSOR_CELL(2, i) = tile++;     /* owner tab */
    {
        base = (u8 *)&gMain;
        /* FAKEMATCH: a fresh r0 base for this loop, kept apart from the saved global base by the empty asm,
         * stops the loop's start pointer from being hoisted before the first loop. It emits no instructions. */
        asm("" : : "r"(base));
        for (cell = (u16 *)(base + OFFSET_OF(struct Main, bgMapBuffer[1][2 * 32 + 8])), i = 0; i < 21; i++)
            *cell++ = tile;             /* top edge, columns 8-28 */
    }
    tile++;
    CURSOR_CELL(2, 29) = tile++;        /* top-right corner */
    CURSOR_CELL(3, 0) = tile++;         /* left side */
    CURSOR_CELL(3, 29) = tile++;        /* right side */
    CURSOR_CELL(4, 0) = tile++;         /* bottom-left corner */
    for (cell = &CURSOR_CELL(4, 1), i = 0; i < 28; i++)
        *cell++ = tile;                 /* bottom edge, columns 1-28 */
    CURSOR_CELL(4, 29) = tile + 1;      /* bottom-right corner */
}

/*
 * Draws the NUL-terminated string `str` into the text canvas at (x, y) with a one-pixel drop shadow: colour 9
 * at (x + 1, y + 1), then colour 7 at (x, y), in font size `size`. With gSaveData.sjisText the string holds
 * 2-byte Shift-JIS characters (lead byte first, so each u16 is byte-swapped before drawing), each advancing
 * `size` pixels; otherwise it is read as bytes, each advancing size / 2.
 */
void TextDrawShadowedString(s32 x, s32 y, const u16 *str, s32 size)
{
    if (gSaveData.sjisText) {
        while (*(u8 *)str != 0) {
            u16 ch = *str;
            ch = (ch >> 8) | ((u8)ch << 8);
            TextDrawSjisGlyph(ch, x + 1, y + 1, SIZE_COLOR(size, 9));
            TextDrawSjisGlyph(ch, x, y, SIZE_COLOR(size, 7));
            x += size;
            str++;
        }
    } else {
        const u8 *chr = (const u8 *)str;
        while (*chr != 0) {
            TextDrawLatinGlyph(*chr, x + 1, y + 1, SIZE_COLOR(size, 9));
            TextDrawLatinGlyph(*chr, x, y, SIZE_COLOR(size, 7));
            x += size / 2;
            chr++;
        }
    }
}

/*
 * Draws the names of up to four entries of `cards` (at most `count`) into a fresh 32x9-tile text canvas, one
 * row per 16 pixels, and maps the canvas onto BG0 rows 2-10 (tiles 0x10-0x12F); CardListView_Update converts
 * it to tiles because textDirty is set. In the banished list the opponent's face-down cards show
 * "[ Unknown ]". An empty entry is skipped without advancing `cards`, so it ends the list in effect.
 */
void CardListView_DrawNames(struct DuelCard *cards, s32 count)
{
    s32 i;
    struct DuelCard card;
    u32 word;
    u32 idBits;
    u8 *banishedInfo;
    u8 *map;
    int last;
    struct CardListView *view;
    const u8 *names;

    TextCanvasInit(32, 9);
    for (i = 0; i < 4 && i < count; i++) {
        /* Matching: the table base is set inside the loop, so loop.c hoists it and reload rematerialises it
         * at its use (see the FAKEMATCH below). */
        names = gCardNames;
        word = *(u32 *)cards;
        idBits = word << 20;
        card = *(struct DuelCard *)&word;
        if (idBits != 0) {
            s32 known;
            known = 1;
            if (gCardListView.mode == CARDLIST_MODE_BANISHED) {
                u8 *kind;
                banishedInfo = gDuelBanishedInfoBytes;
                /* Matching: the redundant & 1 is in the ROM (an and with the 1 held in `known`). */
                kind = &banishedInfo[(gCardListView.top + i) * 2
                                     + sizeof(struct DuelPlayer) * (gCardListView.player & 1)];
                if (*kind == BANISH_FACE_DOWN && gCardListView.player)
                    known = 0;
            }
            if (known) {
                const u16 *name = (const u16 *)((card.id * CARD_NAME_SIZE) + (u32)names);
                cards++;
                TextDrawShadowedString(8, i * 16 + 7, name, 10);
            } else {
                TextDrawShadowedString(8, i * 16 + 7, gStrCardListViewUnknown, 10);
                cards++;
                /* FAKEMATCH: an empty insn in this path lengthens the loop, so the giv i * 16 + 7 loses the
                 * register contest to `card` (r7 vs r8); `names` then gets no register and its reload in r0
                 * moves the reload of 16 into r1, as in the ROM. */
                asm volatile("");
            }
        }
    }
    i = 0;
    last = 0x11F;
    view = &gCardListView;
    map = (u8 *)&gMain;
    /* FAKEMATCH: keeps gMain + 0x49C unfused and orders the loop preheader as the ROM. Emits no instructions. */
    asm("" : "+r"(map) : "r"(i), "r"(last), "r"(view));
    for (; i <= last; i++)
        *(u16 *)(map + OFFSET_OF(struct Main, bgMapBuffer[0][2 * 32]) + i * 2) = i + 16;
    view->textDirty = TRUE;
}

/* Magic/Trap subtype (stats bits 17-19) of a Magic or Trap card's stats word, else 0. */
static inline int GetSpellSubtype(u32 stats)
{
    switch ((int)CARD_STATS_TYPE(stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        return CARD_STATS_SUBTYPE(stats);
    default:
        return 0;
    }
}

/* The card tables read through their integer address (gCardStats, 0x08621DE0). Matching: each use reloads the
 * table base literal, as in the ROM; the symbol form keeps the base in a register. */
#define STATS_BY_ADDR(id) (((u32 *)0x08621DE0)[(id) & CARD_ID_MASK])

/* Level stars shown for a card: 0 for Trap, Magic and Ticket cards, 10 for the Egyptian Gods. */
static inline int GetCardLevel(u32 id)
{
    switch ((int)CARD_STATS_TYPE(STATS_BY_ADDR(id))) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 10;
    default:
        return CARD_STATS_LEVEL(STATS_BY_ADDR(id));
    }
}

/* DEF shown for a card: 0 for Trap, Magic and Ticket cards, 4000 for the Egyptian Gods. */
static inline u16 GetCardDef(u32 id)
{
    switch ((int)CARD_STATS_TYPE(STATS_BY_ADDR(id))) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_DEF(STATS_BY_ADDR(id)) * CARD_STATS_POINTS_SCALE;
    }
}

/* ATK shown for a card (stats is its stats word): 0 for Trap, Magic and Ticket cards, 4000 for the Gods. */
static inline u16 GetCardAtk(u32 *stats, u32 id)
{
    switch ((int)CARD_STATS_TYPE(*stats)) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
    case CARD_TYPE_TICKET:
        return 0;
    case CARD_TYPE_DIVINE:
        return 4000;
    default:
        return CARD_STATS_ATK(STATS_BY_ADDR(id)) * CARD_STATS_POINTS_SCALE;
    }
}

/*
 * Draws the CARD PROPERTY panel of `card`: clears the panel (BG0) and the portrait (BG3); returns there for the
 * opponent's face-down banished cards. Otherwise flips portraitPage and draws the portrait into the other
 * tile/palette slot (tiles 0x178 / 0x22C, palettes 8 / 12). For a Magic or Trap card: the 魔 / 罠 icon and the
 * subtype icon; for a monster: the attribute and type icons, ATK, DEF and the level stars.
 */
void CardListView_DrawCardInfo(u32 *card)
{
    u32 *stats;
    u32 id;
    u32 player;
    int row;
    int i;
    int type;

    id = ((struct DuelCard *)card)->id;
    player = gCardListView.player;
    row = gCardListView.top + gCardListView.cursorRow;

    FillMapRect(0, 13 * 32 + 1, 19, 6);     /* BG0 rows 13-18: icons, ATK/DEF, stars */
    FillMapRect(3, 10 * 32 + 21, 10, 10);   /* BG3: the portrait */
    if (gCardListView.mode == CARDLIST_MODE_BANISHED) {
        /* gDuelPlayers[player].banishedInfo[row], low byte */
        u8 *players = gDuelPlayersBytes;
        int offset = row * 2;
        offset += sizeof(struct DuelPlayer) * player;
        players += OFFSET_OF(struct DuelPlayer, banishedInfo);
        if (players[offset] == BANISH_FACE_DOWN && player == 1)
            return;
    }

    gCardListViewFlags16.portraitPage = 1 - gCardListViewFlags16.portraitPage;
    DrawCardPortraitWide(3, 10 * 32 + 21, id, gCardListViewFlags16.portraitPage * 0xB4 + 0x178,
                         (gCardListViewFlags16.portraitPage * 4 + 8) * 16);

    if (id == 0)
        return;

    stats = &STATS_BY_ADDR(id);
    type = CARD_STATS_TYPE(*stats);
    switch (type) {
    case CARD_TYPE_TRAP:
    case CARD_TYPE_MAGIC:
        /* the 魔 (Magic, 0x08636DA0) or 罠 (Trap, 0x08636CD8) icon: gAttributeIconImages[8] / [7] */
        LoadBgImage4bpp(13 * 32 + 1, 0x50, 0x130, (u16 *)(type == CARD_TYPE_MAGIC ? 0x08636DA0 : 0x08636CD8));
        if (GetSpellSubtype(*stats))
            LoadBgImage4bpp(13 * 32 + 3, 0x60, 0x134,
                            (u16 *)gSpellSubtypeIconImages[GetSpellSubtype(STATS_BY_ADDR(id))]);
        break;
    default: {
        u32 *monsterStats;
        LoadBgImage4bpp(13 * 32 + 1, 0x50, 0x130,
                        (u16 *)gAttributeIconImages[CARD_STATS_ATTR(*(monsterStats = &STATS_BY_ADDR(id)))]);
        LoadBgImage4bpp(13 * 32 + 3, 0x60, 0x134, (u16 *)gMonsterTypeIconImages[CARD_STATS_TYPE(*monsterStats)]);
        DrawBgDecimal(DECIMAL_AT(13 * 32 + 7, 7), DECIMAL_TILES(0x13A, 4), GetCardAtk(monsterStats, id), FALSE);
        DrawBgDecimal(DECIMAL_AT(14 * 32 + 7, 7), DECIMAL_TILES(0x13E, 4), GetCardDef(id), FALSE);
        /* The level stars: tile 2 from BG0 row 13, column 12, eight per row. */
        for (i = 0; i < GetCardLevel(id); i++) {
            int col = i & 7;
            int starRow = (i >> 3) + 13;
            col += 12;
            gBgMaps[col + ((u16)starRow << 5)] = 2;
        }
        break;
    }
    }
}

/* CardListView_DrawCardInfo for the selected entry, cards[top + cursorRow]. */
void CardListView_DrawSelectedInfo(void)
{
    CardListView_DrawCardInfo(&gCardListView.cards[gCardListView.cursorRow + gCardListView.top]);
}

/* CardListView_DrawNames for the page from cards[top]. */
void CardListView_DrawPage(void)
{
    CardListView_DrawNames((struct DuelCard *)&gCardListView.cards[gCardListView.top],
                           gCardListView.count - gCardListView.top);
}

/* CardListView_DrawCursorFrame for the owner (card word bit 12) of the selected entry. */
void CardListView_DrawSelectedCursorFrame(void)
{
    CardListView_DrawCursorFrame((gCardListView.cards[gCardListView.cursorRow + gCardListView.top] << 19) >> 31);
}

/* enum CardType of a card ID, through the integer table address (see STATS_BY_ADDR). */
static inline u32 GetListCardType(u16 id)
{
    return CARD_STATS_TYPE(((const u32 *)0x08621DE0)[id & CARD_ID_MASK]);
}

/* OBJ attr2 of the CARD STATUS icons: palette 1, tiles from 0x100 (gCardListViewStatusIconsGfx). */
enum CardStatusIcon {
    STATUS_ICON_BIT19 = 0x1100,             /* card-word bit 19 set (meaning unknown) */
    STATUS_ICON_NO = 0x1102,                /* special-summon-only monster that was not properly summoned */
    STATUS_ICON_FROM_HAND = 0x1106,         /* target lists: where the target is */
    STATUS_ICON_FROM_DECK = 0x1108,
    STATUS_ICON_FROM_GRAVEYARD = 0x110A,
    STATUS_ICON_FUSION_MATERIAL = 0x110C,   /* card-word bit 20 (isFusionMaterial) */
    STATUS_ICON_BANISHED_FACE_DOWN = 0x110E,    /* BANISH_FACE_DOWN */
    STATUS_ICON_BANISHED_UNTIL_END = 0x1110,    /* BANISH_UNTIL_END_PHASE */
};

/* y of the CARD STATUS icons in AddSprite's yx argument (y << 16 | x). */
#define STATUS_ICON_Y (0x88 << 16)

/*
 * Draws the CARD STATUS icons of `card`, 16x16 sprites from x 8 in steps of 16: the bit-19 icon; in target
 * lists where the target is (hand, deck, graveyard); for monsters outside the fusion deck list the 'NO' icon of
 * a special-summon-only monster that was not properly summoned (card bits 14-15 clear), and in the graveyard and
 * banished lists the icon of a fusion material (bit 20); in the banished list the kind of banishment.
 */
void CardListView_DrawCardStatus(struct DuelCard *card)
{
    u32 x = 8;
    u32 type;
    u32 mode;

    /* Matching: the card bits are tested through the whole word (lsl, sign) and byte 1, as in the ROM. */
    if ((s32)(*(u32 *)card << 12) < 0) {    /* bit 19 */
        x |= STATUS_ICON_Y;
        AddSprite(x, SPRITE_SHAPE_16x16, STATUS_ICON_BIT19);
        x = 24;
    }
    if (gCardListView.mode == CARDLIST_MODE_TARGETS) {
        switch (gCardListView.sources[gCardListView.cursorRow + gCardListView.top]) {
        case CARDLIST_SRC_HAND:
            AddSprite(STATUS_ICON_Y | x, SPRITE_SHAPE_16x16, STATUS_ICON_FROM_HAND);
            x += 16;
            break;
        case CARDLIST_SRC_DECK:
            AddSprite(STATUS_ICON_Y | x, SPRITE_SHAPE_16x16, STATUS_ICON_FROM_DECK);
            x += 16;
            break;
        case CARDLIST_SRC_GRAVEYARD:
            AddSprite(STATUS_ICON_Y | x, SPRITE_SHAPE_16x16, STATUS_ICON_FROM_GRAVEYARD);
            x += 16;
            break;
        }
    }
    type = GetListCardType(card->id);
    if (type <= CARD_TYPE_REPTILE && gCardListView.mode != CARDLIST_MODE_FUSION_DECK) {
        /* bits 14-15 (unk14, normalSummoned) both clear */
        if (IsSpecialSummonOnlyWide(card->id) && (((u8 *)card)[1] & 0xC0) == 0) {
            AddSprite(STATUS_ICON_Y | x, SPRITE_SHAPE_16x16, STATUS_ICON_NO);
            x += 16;
        }
        mode = CARDLIST_FLAGS & 0xE0;
        if (mode == MODE_BITS(CARDLIST_MODE_BANISHED) || mode == MODE_BITS(CARDLIST_MODE_YOUR_GRAVEYARD)
            || mode == MODE_BITS(CARDLIST_MODE_OPPONENT_GRAVEYARD)) {
            if ((s32)(*(u32 *)card << 11) < 0) {    /* bit 20 */
                AddSprite(STATUS_ICON_Y | x, SPRITE_SHAPE_16x16, STATUS_ICON_FUSION_MATERIAL);
                x += 16;
            }
        }
    }
    {
        struct CardListView *view = &gCardListView;
        u32 flags = *(u8 *)view;    /* CARDLIST_FLAGS */

        if ((flags & 0xE0) == MODE_BITS(CARDLIST_MODE_BANISHED)) {
            u32 player = flags << 30;   /* bit 1 (player) at bit 31 */
            /* FAKEMATCH: the row is pinned to r0, and the empty asm keeps it there before the player base is
             * formed. It emits no instructions. */
            register int row asm("r0") = view->cursorRow + view->top;
            u8 *players;

            asm("" : : "r"(row));
            /* gDuelPlayers[player].banishedInfo[row], low byte */
            players = gDuelPlayersBytes;
            row *= 2;
            row += sizeof(struct DuelPlayer) * (player >> 31);
            players += OFFSET_OF(struct DuelPlayer, banishedInfo);
            switch (players[row]) {
            case BANISH_UNTIL_END_PHASE:
                x |= STATUS_ICON_Y;
                AddSprite(x, SPRITE_SHAPE_16x16, STATUS_ICON_BANISHED_UNTIL_END);
                break;
            case BANISH_FACE_DOWN:
                x |= STATUS_ICON_Y;
                AddSprite(x, SPRITE_SHAPE_16x16, STATUS_ICON_BANISHED_FACE_DOWN);
                break;
            }
        }
    }
}

/*
 * Draws the button bar at the top of the screen: a 16x16 icon for each enabled button (bit i of buttonMask,
 * enum CardListViewButton) from x 0x68, 18 pixels apart. The selected button gets its highlighted icon and its
 * 64x16 label (two 32x16 sprites at x 0xA0 and 0xC0). Each button has two OBJ tile rows from i * 64:
 * highlighted icon, plain icon (+2), label (+4, +8).
 */
void CardListView_DrawButtons(s32 button, u16 buttonMask)
{
    s32 x = 0x68;
    s32 i;

    for (i = 0; i <= 3; i++) {
        /* Matching: the tile numbers are computed before the mask test, in these widths. */
        u16 tileRow = i << 1;           /* two rows of 32 OBJ tiles per button */
        s32 tile = tileRow << 5;
        s32 plainTile = tile + 2;

        if ((buttonMask >> i) & 1) {
            if (i == button) {
                AddSprite(x, SPRITE_SHAPE_16x16, tile);
                AddSprite(0xA0, SPRITE_SHAPE_32x16, plainTile + 2);
                AddSprite(0xC0, SPRITE_SHAPE_32x16, tile + 8);
            } else {
                AddSprite(x, SPRITE_SHAPE_16x16, plainTile);
            }
            x += 18;
        }
    }
}

/*
 * Step 0 of gCardListViewSteps (also re-run after Card Detail closes): the screen setup, by initState.
 * Returns 1 when the screen is set up and has faded in.
 */
u16 CardListView_InitScreen(void)
{
    s32 i, j, k;
    u32 titles;

    switch (gCardListView.initState) {
    case 0:
        /* wait for the duel screen to fade out */
        if (DuelScreen_FadeOutStep()) {
            gDuelScreen.active = 0;
            gDuelScreen.uiGfxLoaded = 0;
            gCardListView.initState++;
        }
        return 0;
    case 1:
        /* video registers */
        ResetVideo();
        ClearBgMapBuffers();
        gMain.vblankFlags = VBLANK_COPY_OAM | VBLANK_COPY_BG_MAPS | VBLANK_BG0_VOFS | VBLANK_BG1_VOFS;
        REG_DISPCNT = 0;
        REG_BLDCNT = 0;
        REG_BG0CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(0);   /* names, panel text */
        REG_BG1CNT = BGCNT_PRIORITY(0) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(1);   /* cursor box */
        REG_BG2CNT = BGCNT_PRIORITY(2) | BGCNT_CHARBASE(1) | BGCNT_SCREENBASE(2);   /* backgrounds */
        REG_BG3CNT = BGCNT_PRIORITY(3) | BGCNT_CHARBASE(1) | BGCNT_256COLOR | BGCNT_SCREENBASE(3); /* portrait */
        gCardListView.initState++;
        return 0;
    case 2:
        /* graphics. Matching: the titles base is taken before the calls, so it is a call-crossing pseudo that
         * loses the register contest and is rematerialised at its use, as in the ROM. */
        titles = (u32)gCardListViewTitlesGfx;
        SetBrightnessBlack();
        ResetBgScroll();
        CopyDoubleWords(OBJ_PAL(0), gCardListViewButtonsPal, 0x20);
        CopyDoubleWords(OBJ_TILE(0), gCardListViewButtonsGfx, 0x2000);
        CopyDoubleWords(OBJ_PAL(1), gCardListViewStatusIconsPal, 0x20);
        CopyDoubleWords(OBJ_TILE(0x100), gCardListViewStatusIconsGfx, 0x800);
        /* BG2 (map offsets 0x400 + cell): header strip, list background, info panel */
        LoadBgImage4bppMap1(0x400, 0x10, 0x3C6, gCardListViewHeaderImage);
        LoadBgImage4bppMap1(0x400 + 2 * 32, 0x20, 0x354, gCardListViewBgImage);
        LoadBgImage4bppMap1(0x400 + 11 * 32, 0x30, 0x2E0, gCardListViewInfoPanelImage);
        MemCopy16(BG_PAL(4), gCardListViewCursorFramePal, 0x20);
        /* The OPPONENT (i = 0) and YOU (i = 1) cursor boxes, 15 tiles each from BG tile 0x15A + i * 15, taken
         * from a 3 x 32-tile sheet of 0x60 tiles per box: the tab (8), top edge and corner (2), left side,
         * right side, bottom-left corner and bottom edge (2), bottom-right corner. */
        for (i = 0; i < 2; i++) {
            u16 src = i * 0x60;
            u16 dst = i * 15;
            CopyDoubleWords(BG_CHAR1_TILE(dst + 0x15A), gCardListViewCursorFrameGfx + src * 32, 8 * 32);
            CopyDoubleWords(BG_CHAR1_TILE(dst + 0x162), gCardListViewCursorFrameGfx + (src + 0x1C) * 32, 2 * 32);
            CopyDoubleWords(BG_CHAR1_TILE(dst + 0x164), gCardListViewCursorFrameGfx + (src + 0x20) * 32, 32);
            CopyDoubleWords(BG_CHAR1_TILE(dst + 0x165), gCardListViewCursorFrameGfx + (src + 0x3D) * 32, 32);
            CopyDoubleWords(BG_CHAR1_TILE(dst + 0x166), gCardListViewCursorFrameGfx + (src + 0x40) * 32, 2 * 32);
            CopyDoubleWords(BG_CHAR1_TILE(dst + 0x168), gCardListViewCursorFrameGfx + (src + 0x5D) * 32, 32);
        }
        CopyDoubleWords(BG_PAL(7), gCardListViewTitlesPal, 0x20);
        /* The title of the list (none for the deck): 12 x 2 tiles at BG tile 0x142, palette 7, BG0 rows 0-1. */
        if (gCardListView.mode <= CARDLIST_MODE_TARGETS) {
            CopyDoubleWords(BG_CHAR1_TILE(0x142), (const u8 *)(gCardListView.mode * 0x300 + titles), 0x300);
            for (j = 0; j < 12; j++) {
                u16 col = j;
                gBgMaps[col] = j + 0x7142;
                gBgMaps[col + 32] = j + 0x714E;
            }
        }
        gCardListView.initState++;
        return 0;
    case 3:
        /* the list: the first page, or "There are no cards." */
        if (gCardListView.count != 0) {
            CardListView_DrawPage();
            CardListView_DrawSelectedInfo();
            CardListView_DrawSelectedCursorFrame();
            gMain.bgVofs[1] = -(gCardListView.cursorRow * 16);
            gCardListView.button = CARDLIST_BUTTON_CARD_VIEW;
            gCardListView.buttonMask |= 1 << CARDLIST_BUTTON_CARD_VIEW;
        } else {
            TextCanvasInit(32, 9);
            TextDrawShadowedString(0x42, 0x18, gStrCardListViewNoCards, 12);
            for (k = 0; k < 0x120; k++)
                gMain.bgMapBuffer[0][2 * 32 + k] = k + 16;
            TextCanvasToTiles((u16 *)BG_CHAR1_TILE(16), 0);
            gCardListView.button = CARDLIST_BUTTON_EXIT;
        }
        /* select the first enabled button */
        while (!((gCardListView.buttonMask >> gCardListView.button) & 1))
            gCardListView.button++;
        gCardListView.showSprites = 1;
        gCardListView.initState++;
        return 0;
    case 4:
        /* fade in */
        REG_DISPCNT |= DISPCNT_BG_ALL_ON | DISPCNT_OBJ_ON;
        if (FadeFromBlackU16(4))
            gCardListView.initState++;
        return 0;
    }
    return 1;
}
