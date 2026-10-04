#ifndef GUARD_TURN_ORDER_H
#define GUARD_TURN_ORDER_H

/*
 * Pre-duel turn-order screen (units destiny_board_scene, turn_order_scene and turn_order_steps):
 * rock-paper-scissors with a carousel of three hand cards, then the winner picks FIRST or SECOND, and the
 * DUEL logo drops in. Its work area is gSceneWork.u.turnOrder (struct TurnOrderSceneWork, duel_scenes.h).
 *
 * Runners (each returns 1 at the end of its step script): Campaign_DecideTurnOrder picks TurnOrder_RunRps
 * (first duel of a match), TurnOrder_RunPlayerChoice (the player lost the last duel) or
 * TurnOrder_RunCpuChoice; LinkBattle_DecideTurnOrder uses TurnOrder_RunRpsLink. TurnOrder_RpsMain runs the
 * phases (enum TurnOrderPhase) from the local tables gTurnOrderRpsSubsteps / gTurnOrderChoiceSubsteps.
 */

#include "global.h"

struct AnimSeq;
struct Scroller;
struct ChoiceBob;

/* Rock-paper-scissors hand: the carousel's Scroller.stop, opponentHand, JudgeRockPaperScissors arguments,
 * gHandCardTileNums index and the data of link message LINKMSG_RPS_HAND. */
enum RpsHand {
    RPS_ROCK = 0,
    RPS_SCISSORS = 1,
    RPS_PAPER = 2,
};

/* JudgeRockPaperScissors result, from the player's side (TurnOrderSceneWork.result). */
enum RpsResult {
    RPS_WIN = 0,
    RPS_LOSE = 1,
    RPS_DRAW = 2,
};

/* Turn order choice: TurnOrderSceneWork.turnChoice, TurnOrder_CpuPickTurn, gMain.firstPlayer. */
enum TurnChoice {
    TURN_CHOICE_FIRST = 0,      /* this player starts */
    TURN_CHOICE_SECOND = 1,
    TURN_CHOICE_NONE = 255,
};

/* TurnOrder_DrawBanner banner: index of gTurnOrderBannerTileNums / gTurnOrderBannerPalNums. */
enum TurnOrderBanner {
    BANNER_SELECT_CARD = 0,     /* "SELECT A CARD", drawn higher up */
    BANNER_WIN = 1,
    BANNER_LOSE = 2,
    BANNER_UNUSED = 3,
    BANNER_DRAW = 4,
};

/* Bits of TurnOrderSceneWork.blendMask: sprite groups drawn semi-transparent (OBJ mode 1). Set to 7 when
 * the FIRST/SECOND choice starts. */
enum TurnOrderBlendBit {
    BLEND_CAROUSEL = 1,
    BLEND_OPPONENT_CARD = 2,
    BLEND_BANNER = 4,
    BLEND_TURN_CHOICE = 8,
};

/* Link handshake message ids (LinkHandshakeStep) exchanged during the turn-order phases. */
enum TurnOrderLinkMsg {
    LINKMSG_RPS_HAND = 0x51,    /* the player's hand (enum RpsHand); TurnOrder_ChoiceMain also sends its turn
                                 * choice with this id (only reachable from TurnOrder_RunPlayerChoiceLink, which
                                 * has no callers) */
    LINKMSG_TURN_CHOICE = 0x52, /* the winner's choice (enum TurnChoice) */
    LINKMSG_RPS_REMATCH = 0x53, /* ready for another round after a draw */
};

/* TurnOrderSceneWork.phase: index of the phase table run by TurnOrder_RpsMain. The choice-only table
 * gTurnOrderChoiceSubsteps puts TurnOrder_ShowChoice at index 3, so from 3 on its values name other phases
 * (turn_order_steps numbers them locally). */
enum TurnOrderPhase {
    TURN_ORDER_CHOOSE_HAND = 0,
    TURN_ORDER_SHOW_RESULT = 1,
    TURN_ORDER_CHOOSE_TURN = 2,
    TURN_ORDER_ANIMATE_CHOICE = 3,
    TURN_ORDER_DUEL_LOGO = 4,
    TURN_ORDER_FLASH_WHITE = 5,
    TURN_ORDER_FADE_OUT = 6,
};

/* Index of TurnOrderSceneWork.scrollers. */
enum TurnOrderScroller {
    SCROLLER_CAROUSEL = 0,      /* carousel angle; Scroller.stop = selected hand */
    SCROLLER_OPPONENT_CARD = 1, /* opponent card slide, 0..0x30 */
};

/* ---- Graphics (OBJ tile numbers are relative to tile 0x200, the bitmap-mode OBJ area) ---- */

extern const u16 gHandCardTileNums[];           /* 0x080826E0: {0x200, 0x204, 0x208} rock, scissors, paper cards */
extern const u16 gTurnChoiceBannerTileNums[];   /* 0x080826E6: {0x300, 0x308} "FIRST to go", "SECOND to go" */
extern const u16 gDuelLogoTileNums[];           /* 0x08082706: {0x210, 0x218, 0x310} the three 64x64 DUEL pieces */
extern const u8 gHandCarouselStops[];           /* 0x080826DC: {0, 86, 171} carousel angles at which rock, scissors
                                                 * and paper face the player */
extern const u8 gHandCardPalNums[];             /* 0x08082703: {0, 1, 2} OBJ palette of each hand card */
extern const u16 gTurnOrderBannerTileNums[];    /* 0x080826EA: [5][2] the two 64x32 halves (OBJ tiles) of each
                                                 * banner (enum TurnOrderBanner) */
extern const u8 gTurnOrderBannerPalNums[];      /* 0x080826FE: {4, 5, 6, 8, 7} OBJ palette of each banner (8 is never
                                                 * loaded: entry 3 is unused) */
/* 0x0819A780: NULL-terminated AnimSeq * list (AnimBlockInit): the flashing link "Wait" sign. */
extern struct AnimSeq *gTurnOrderWaitAnimList[];

/* 240x160 Mode-4 bitmap of the hieroglyph corridor (backdrop of the coin toss, dice and turn-order
 * screens) and its 256-colour palette. */
extern const u8 gEgyptCorridorBitmap[];         /* 0x086A12EC */
extern const u8 gEgyptCorridorPal[];            /* 0x086AA8EC */

/* OBJ palettes */
extern const u8 gRockCardPal[];                 /* 0x086AAAEC: palette 0 */
extern const u8 gScissorsCardPal[];             /* 0x086AAB00: palette 1 */
extern const u8 gPaperCardPal[];                /* 0x086AAB20: palette 2 */
extern const u8 gTurnChoiceBannerPal[];         /* 0x086AAB40: palette 3, highlighted FIRST/SECOND banner */
extern const u8 gSelectCardBannerPal[];         /* 0x086AAB60: palette 4 */
extern const u8 gWinBannerPal[];                /* 0x086AABA0: palette 5 */
extern const u8 gLoseBannerPal[];               /* 0x086AABBC: palette 6 */
extern const u8 gDrawBannerPal[];               /* 0x086AABDC: palette 7 */
extern const u8 gTurnChoiceBannerDimPal[];      /* 0x086AAC00: palette 9, unselected FIRST/SECOND banner */
extern const u8 gDuelLogoPal[];                 /* 0x086AAB80: palette 10 */
extern const u8 gWaitSignPal[];                 /* 0x086AAC20: palette 13 */

/* OBJ tiles (4bpp, linear; TurnOrder_LoadObjTiles copies them to 2D-mapped OBJ VRAM) */
extern const u8 gRockCardTiles[];               /* 0x086AAC28: 32x64 rock (fist) card, tile 0x200 */
extern const u8 gScissorsCardTiles[];           /* 0x086AB028: 32x64 scissors card, tile 0x204 */
extern const u8 gPaperCardTiles[];              /* 0x086AB428: 32x64 paper (open hand) card, tile 0x208 */
extern const u8 gTurnChoiceBannerTiles[];       /* 0x086AB828: 128x32 FIRST + SECOND banners, tile 0x300 */
extern const u8 gSelectCardBannerTiles[];       /* 0x086AC028: 128x32 "SELECT A CARD", tile 0x380 */
extern const u8 gDuelLogoTiles0[];              /* 0x086AC828: 64x64 DUEL piece 1, tile 0x210 (over WIN) */
extern const u8 gDuelLogoTiles1[];              /* 0x086AD028: 64x64 DUEL piece 2, tile 0x218 */
extern const u8 gDuelLogoTiles2[];              /* 0x086AD828: 64x64 DUEL piece 3, tile 0x310 (over Wait) */
extern const u8 gWinBannerTiles[];              /* 0x086AE028: 128x32 "WIN !!", tile 0x210 */
extern const u8 gLoseBannerTiles[];             /* 0x086AE828: 128x32 "LOSE", tile 0x290 */
extern const u8 gDrawBannerTiles[];             /* 0x086AF028: 128x32 "DRAW", tile 0x390 */
extern const u8 gWaitSignTiles[];               /* 0x086AF828: two 32x16 "Wait" frames, tile 0x310 */

/* ---- Runners (step scripts indexed by gMain.seqIndex1; return 1 at the end) ---- */

/* Rock-paper-scissors turn-order screen (Campaign). */
u16 TurnOrder_RunRps(void);
/* Link Battle variant of TurnOrder_RunRps: exchanges picks over the link; B cancels. */
u16 TurnOrder_RunRpsLink(void);
/* No rock-paper-scissors: the player picks FIRST/SECOND directly. */
u16 TurnOrder_RunPlayerChoice(void);
/* TurnOrder_RunPlayerChoice with the link flag set. No callers. */
u16 TurnOrder_RunPlayerChoiceLink(void);
/* The opponent's random FIRST/SECOND pick is shown without a choice. */
u16 TurnOrder_RunCpuChoice(void);

/* ---- Script steps ---- */

/* Rock-paper-scissors step 0: clears gSceneWork (0xB24 bytes), resets scroll, hides the layers. */
u16 TurnOrder_Init(void);
/* Step 1: starts the fade-in and loads the corridor bitmap, the OBJ palettes and tiles. */
u16 TurnOrder_Load(void);
/* Empty step: returns 1 at once. */
u16 TurnOrder_NopStep(void);
/* Main step of the rock-paper-scissors script, every frame: the scene fade, the current phase
 * (enum TurnOrderPhase) and all sprites. */
u16 TurnOrder_RpsMain(void);
/* Step 0 of the choice-only scripts: clears gSceneWork and resets the video state like TurnOrder_Init. */
u16 TurnOrder_InitChoice(void);
/* Last step of the choice-only scripts, every frame: Left/Right/A of the FIRST/SECOND choice (link:
 * sends LINKMSG_TURN_CHOICE), then the choice phases. */
u16 TurnOrder_ChoiceMain(void);
/* CPU choice step 1: loads the DUEL logo tiles, picks the choice with Random() & 1 and starts the banner
 * tween, skipping the input. */
u16 TurnOrder_CpuChooseTurn(void);

/* ---- Phases (enum TurnOrderPhase; return 1 when the phase is done) ---- */

/* TURN_ORDER_CHOOSE_HAND: Left/Right turn the carousel, A picks the hand; the link partner or the CPU
 * answers (the CPU copies the hand 1 time in 5, else wins or loses with equal odds). */
u16 TurnOrder_ChooseHand(void);
/* TURN_ORDER_SHOW_RESULT: once the opponent's card is in, shows WIN/LOSE/DRAW; a draw restarts. */
u16 TurnOrder_ShowResult(void);
/* TURN_ORDER_CHOOSE_TURN: the winner picks FIRST (Left) or SECOND (Right); A confirms. */
u16 TurnOrder_ChooseTurn(void);
/* TURN_ORDER_ANIMATE_CHOICE: the chosen banner's tween; `phase` is TurnOrderSceneWork.phase. */
u16 TurnOrder_AnimateTurnChoice(u8 *phase);
/* Choice-only phase 3: the chosen FIRST/SECOND banner flies to the centre. */
u16 TurnOrder_ShowChoice(void);
/* TURN_ORDER_DUEL_LOGO: the DUEL logo drops in and swings to rest; after 90 frames or A starts the
 * white flash. */
u16 TurnOrder_ShowDuelLogo(void);
/* TURN_ORDER_FLASH_WHITE: brightens to white, fills the palettes with white and starts a 20-frame timer. */
u16 TurnOrder_FlashWhite(void);
/* TURN_ORDER_FADE_OUT: fades from white to black, then sets gMain.firstPlayer from the turn choice. */
u16 TurnOrder_FadeOutAndSetFirstPlayer(void);

/* ---- Rules ---- */

/* RPS_WIN / RPS_LOSE / RPS_DRAW from the player's side; returns `player` for an out-of-range hand. */
u8 JudgeRockPaperScissors(u8 player, u8 opponent);
/* The CPU's turn choice for the player after the player loses: frame & 1 (enum TurnChoice). */
u8 TurnOrder_CpuPickTurn(u8 frame);

/* ---- Sprites ---- */

/* gSceneWork.anims[0].active = 1: shows the flashing link "Wait" sign. */
void TurnOrder_ShowWaitSign(void);
/* gSceneWork.anims[0].active = 0xFF: hides the "Wait" sign. */
void TurnOrder_HideWaitSign(void);
/* scrollers[i].pos += scrollers[i].speed for the 4 entries of TurnOrderSceneWork.scrollers (u8 wrap). */
void Scroller_Move(struct Scroller *scrollers);
/* Stops the carousel at the gHandCarouselStops position it is passing and stores that stop (the hand). */
void Scroller_SnapToStop(struct Scroller *scroller);
/* speed = 0 when pos reaches 0x30 or 0 (the opponent card slide). */
void Scroller_StopAtEnds(struct Scroller *scroller);
/* Draws the three hand cards on a ring seen from the side; the selected card rises and the others sink as
 * the opponent's card slides in (`lift`). */
void TurnOrder_DrawHandCarousel(const u16 *tileNums, const u8 *palNums, u8 angle, u8 selected, u8 lift,
                                u16 blendMask, u8 spread);
/* Draws a 128x32 banner (enum TurnOrderBanner) from its two 64x32 halves. */
void TurnOrder_DrawBanner(u8 banner, u16 blendMask);
/* Draws the opponent's card for `hand`, `slide` steps into the screen. */
void TurnOrder_DrawOpponentCard(u8 hand, u8 slide, u16 blendMask);
/* Draws the FIRST and SECOND banners bobbing; `bob` points at struct ChoiceBob[2]. */
void TurnOrder_DrawTurnChoice(u32 unused, u8 frame, u16 blendMask, s16 *bob, u8 choice);
/* After the choice: both bobs decay; the chosen banner slides to the centre driven by `tween` (struct Tween)
 * while the other falls away rotating. `bob` points at struct ChoiceBob[2]. */
void TurnOrder_DrawTurnChoiceConfirm(u32 unused, u8 frame, u16 blendMask, s16 *bob, u8 choice, s16 *tween);
/* Draws the chosen FIRST (0) / SECOND (1) banner at the tween position; takes the same argument list as
 * TurnOrder_DrawTurnChoiceConfirm, of which the first, second and fourth are unused. */
void TurnOrder_DrawChosenTurnBanner(u32 unused0, u32 unused1, u16 blendMask, void *unused3, u8 choice,
                                    s16 *tween);
/* Draws the three 64x64 DUEL logo pieces, spread by the swing angle and dropped by `drop`. */
void TurnOrder_DrawDuelLogo(u8 unused, u8 swing, u8 drop);
/* bob[choice].amp += 0x20 (max 0x800), the other banner's amplitude -= 0x40 (min 0). */
void TurnOrder_UpdateChoiceBob(u8 choice, struct ChoiceBob *bob);
/* Copies `rows` rows of `width` linear 4bpp tiles to 2D-mapped OBJ VRAM at tile 0x200 + tile. */
void TurnOrder_LoadObjTiles(const u8 *src, u32 tile, u32 width, s32 rows);

#endif /* GUARD_TURN_ORDER_H */
