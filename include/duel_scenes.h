#ifndef GUARD_DUEL_SCENES_H
#define GUARD_DUEL_SCENES_H

/*
 * Full-screen duel scenes and the scene work areas.
 *
 * A duel command starts a scene with DuelScene_Start (enum DuelSceneId) and sets gDuelScene.arg; the
 * command runner then calls DuelScene_Run every frame. It fades the duel screen out and calls the scene
 * handler gDuelSceneHandlers[id] until it returns 1; the command then pushes DUEL_CMD_OPEN_DUEL_SCREEN to
 * bring the field back. Each handler runs a NULL-terminated table of steps (each returns non-zero to
 * advance) indexed by gDuelScene.sceneStep:
 *   coin toss       gCoinTossWork (0x02015280)       CoinToss_*
 *   die roll        gDiceScreen (0x0201F820)         DiceScreen_* (Skull Dice, Graceful Dice or a plain die)
 *   Exodia win      gSceneWork.u.exodia              ExodiaScene_*
 *   Destiny Board   gSceneWork.u.destinyBoard        DestinyBoardScene_*, ScrollLayer_*
 * gSceneWork (0x02020310) is also the work area of the pre-duel turn-order screen (gSceneWork.u.turnOrder,
 * functions in turn_order.h), and of the Get-a-pack list and the starter-deck screen, whose larger views
 * (struct PackListWork, struct StarterDeckSelectWork, 0x8070 bytes) are in booster.h.
 *
 * The three work areas share one layout: an OAM layer list at +0 (struct OamList), 32 OBJ affine records at
 * +0x618, a block of 20 script animations at +0x918 (count at +0xAA8) and the scene's own data from +0xAAC.
 * The coin toss work area copies the turn-order one with struct CoinToss inserted at +0xAAC: its fields
 * at +0xB14-0xB3F are the turn-order fields shifted by 0x68, set up but not used.
 *
 * Code: duel_cmd_queue.c (scene runner), duel_field_view.c and coin_toss_scene.c (coin toss),
 * coin_toss_scene.c and dice_scene.c (die roll), dice_scene.c (Exodia, Destiny Board helpers),
 * destiny_board_scene.c (Destiny Board steps), turn_order_scene.c.
 *
 * Every prototype is the function's definition as compiled; DiceScreen_AddOamPiece and
 * DiceScreen_DrawCharacter have old-style (K&R) definitions, so their prototypes use the promoted parameter
 * types. Units that call a function through a local declaration with other widths keep that view as a
 * commented local alias prototype when they migrate (build/readability/proto_mismatches.txt).
 */

#include "global.h"
#include "util.h"       /* struct Ease, Line, Timer, Tween */
#include "palette.h"    /* struct Fade, PalFade */
#include "sprite.h"     /* struct OamList, ObjAffine, AnimState */
#include "link.h"       /* struct LinkSync */

/* ------------------------------------------------------------------------------------------------------ */
/* Scene runner                                                                                           */
/* ------------------------------------------------------------------------------------------------------ */

/* DuelScene_Start id: index into gDuelSceneHandlers (verified against the table at 0x08198EF8). */
enum DuelSceneId {
    DUEL_SCENE_COIN_TOSS = 0,           /* CoinToss_Run */
    DUEL_SCENE_EXODIA_WIN = 1,          /* ExodiaScene_Run */
    DUEL_SCENE_DICE_GRACEFUL = 2,       /* DiceScreen_RunGraceful */
    DUEL_SCENE_DICE_SKULL = 3,          /* DuelScene_SkullDice */
    DUEL_SCENE_DICE_PLAIN = 4,          /* DiceScreen_RunPlain */
    DUEL_SCENE_DESTINY_BOARD_WIN = 5    /* DestinyBoardScene_Run */
};

/* gDuelScene (0x02017A30): the running full-screen scene. */
struct DuelScene {
    u16 (*handler)(void);   /* +0x0: gDuelSceneHandlers[id], called by DuelScene_RunHandler */
    u16 id:15;              /* +0x4 bits 0-14: enum DuelSceneId */
    u16 flag:1;             /* bit 15: second argument of DuelScene_Start; every caller passes 0, never read */
    u16 arg;                /* +0x6: scene parameter, set by the scene command after DuelScene_Start. Coin
                             *   toss: bits 0-6 face of coin i (1 = tails), bit 7 selects CoinToss.unk63,
                             *   bits 8-14 coin count, bit 15 called face (1 = tails). Dice: the face, 1-6 */
    u16 result;             /* +0x8: die face written by DiceScreen_Update at the end of the roll */
    u8 runnerStep;          /* +0xA: index into gDuelSceneRunnerSteps (fade out the duel screen, run the
                             *   handler) */
    u8 sceneStep;           /* +0xB: index into the scene handler's step table */
    u8 unkC;                /* +0xC: cleared with sceneStep */
    u8 unkD;                /* +0xD: cleared with sceneStep */
    u8 padE[2];
};

/* ------------------------------------------------------------------------------------------------------ */
/* Coin toss (gCoinTossWork)                                                                              */
/* ------------------------------------------------------------------------------------------------------ */

/* Coin.state */
enum CoinState {
    COIN_STATE_READY = 0,       /* not launched yet */
    COIN_STATE_FLYING = 1,
    COIN_STATE_LANDED = 2,
    COIN_STATE_MATCHED = 3      /* landed on the called face (CoinToss_MarkMatchingCoins) */
};

/* Coin.frame after landing and CoinToss.calledFace: the spin frame that shows each face. */
enum CoinFaceFrame {
    COIN_FRAME_HEADS = 0,
    COIN_FRAME_TAILS = 4
};

/* Coin.glintState as duel_field_view.c reads it (the glint over a coin showing the called face). */
enum CoinGlintState {
    GLINT_NONE = 0,
    GLINT_PLAYING = 1,
    GLINT_DONE = 2
};

/* The same Coin.glintState values under the names coin_toss_scene.c uses (CoinToss_AnimateHighlights,
 * CoinToss_MarkMatchingCoins, CoinToss_CountUnfinished). */
enum CoinHighlightState {
    COIN_HIGHLIGHT_NONE = 0,
    COIN_HIGHLIGHT_ANIMATING = 1,
    COIN_HIGHLIGHT_DONE = 2     /* resolved: CoinToss_CountUnfinished no longer counts the coin */
};

/* gCoinTossWork.launchMode (medium confidence). */
enum CoinLaunchMode {
    COIN_LAUNCH_ALL = 0,        /* launch one coin per frame (CoinToss_Run) */
    COIN_LAUNCH_STAGGERED = 1   /* one coin every 16 frames after a wait (CoinToss_RunStaggered) */
};

/* One coin of the toss (12 bytes). */
struct Coin {
    u8 spinTimer;           /* +0x0: frames until the next spin frame (reload 3) */
    u8 frame;               /* +0x1: spin frame 0-7; after landing the face (enum CoinFaceFrame) */
    u8 state;               /* +0x2: enum CoinState */
    u8 unk3;
    s16 height;             /* +0x4: height above the ground, 8.8 fixed point (0x30 * t - 5 * t * t) */
    u16 flightTime;         /* +0x6: flight parameter t, +40 per frame */
    u8 glintTimer;          /* +0x8: frames until the next glint frame (reload 6) */
    u8 glintFrame;          /* +0x9: glint frame 0-4 */
    u8 glintState;          /* +0xA: enum CoinGlintState / enum CoinHighlightState */
    u8 unkB;
};

/* The coins of one toss (gCoinTossWork.toss, CoinToss_InitCoins). */
struct CoinToss {
    struct Coin coins[8];   /* +0x00 */
    u8 count;               /* +0x60: number of coins (gDuelScene.arg bits 8-14) */
    u8 nextLaunch;          /* +0x61: next coin to launch */
    u8 calledFace;          /* +0x62: enum CoinFaceFrame of the call (arg bit 15); coins showing it glint */
    u8 unk63;               /* +0x63: 2 by default, then 0 (arg bit 7 set, as both commands do) or 10;
                             *   never read */
    u8 headsCount;          /* +0x64: coins that landed heads; not read by the scene */
    u8 done;                /* +0x65: every coin resolved (no glint pending) */
    u8 pad66[2];
};

/* One sparkle of the coin trail (8 bytes). */
struct Sparkle {
    u32 active:1;           /* +0x0 bit 0: slot in use; the code tests it as a word (lsl #31), so the
                             *   container must stay u32 */
    u8 timer:2;             /* bits 1-2: frame timer; it wraps every frame, so the frame advances every frame */
    u8 frame:5;             /* bits 3-7: index into gCoinSparkleTiles (a 0 entry ends the sparkle) */
    u8 unk1[3];
    u8 x;                   /* +0x4: screen x */
    u8 y;                   /* +0x5: screen y */
    u8 unk6[2];
};

/* Ring of 32 sparkles (gCoinTossSparkles = gCoinTossWork.sparkles). */
struct SparklePool {
    struct Sparkle sparkles[32];    /* +0x000 */
    u8 next;                        /* +0x100: next slot to use (taken mod 32) */
};

/* A 4-byte scroller (Scroller_Move / Scroller_SnapToStop / Scroller_StopAtEnds, turn_order.h): the
 * turn-order carousel and opponent card slide. */
struct Scroller {
    u8 pos;                 /* +0x0: position: carousel angle, or the opponent-card slide 0..0x30 */
    s8 speed;               /* +0x1: added to pos every frame (-4 / +4 while moving) */
    u8 stop;                /* +0x2: stop reached by Scroller_SnapToStop (the selected hand) */
    u8 unk3;
};

/* Bob of one FIRST / SECOND banner of the turn-order screen (TurnOrder_UpdateChoiceBob,
 * TurnOrder_DrawTurnChoice; struct Pair in turn_order_scene.c). */
struct ChoiceBob {
    s16 amp;                /* +0x0: bob amplitude (0..0x800) */
    s16 unk2;               /* +0x2: unused */
};

/* gCoinTossWork (0x02015280, 0xC58 bytes): work area of the coin toss, cleared by CoinToss_Init. */
struct CoinTossWork {
    struct OamList oamList;         /* +0x000: sprite layer list (OamListClear / OamListAddSprite / OamListFlush) */
    struct ObjAffine affine[32];    /* +0x618: ObjAffineInit; CoinToss_Load zeroes the scales of 0-2 */
    u8 unk918[0xAAC - 0x918];       /* +0x918: script-animation block of the shared layout; unused here */
    struct CoinToss toss;           /* +0xAAC */
    /* +0xB14-0xB3F: the turn-order fields shifted by 0x68 (TurnOrderSceneWork name in brackets), only
     * initialised by CoinToss_Load or moved; the coin toss reads none of them. */
    struct Scroller scrollers[4];   /* +0xB14: [scrollers]; moved every frame, never given a speed */
    u8 unkB24;                      /* +0xB24: set to 0xFF [opponentHand] */
    u8 unkB25;                      /* +0xB25 [opponentAnswered] */
    u8 unkB26;                      /* +0xB26 [result] */
    u8 unkB27;                      /* +0xB27 [turnChoice] */
    u16 unkB28;                     /* +0xB28 [blendMask] */
    u16 unkB2A;
    struct ChoiceBob unkB2C[2];     /* +0xB2C [choiceBob] */
    u16 unkB34;                     /* +0xB34 [blendLevel] */
    u8 unkB36;                      /* +0xB36: set to 0xFA [logoSwing] */
    u8 unkB37;                      /* +0xB37 [logoDrop] */
    u16 unkB38;                     /* +0xB38 [brightness] */
    u16 unkB3A;                     /* +0xB3A [brightnessStep] */
    u8 unkB3C;                      /* +0xB3C [carouselSpread] */
    u8 unkB3D;                      /* +0xB3D [carouselSpreadSpeed] */
    u16 unkB3E;
    u16 timer;                      /* +0xB40: frames since CoinToss_Load; CoinToss_Update returns 1 past 0x140 */
    u8 unkB42;                      /* +0xB42: cleared, unused */
    u8 unkB43[5];
    struct Fade fade;               /* +0xB48: FadeStart / FadeTick; fade.state (+0xB4E) 2 = faded out,
                                     *   3 = faded in */
    struct SparklePool sparkles;    /* +0xB50: alias symbol gCoinTossSparkles */
    u8 launchMode;                  /* +0xC54: enum CoinLaunchMode */
    u8 launchDelay;                 /* +0xC55: COIN_LAUNCH_STAGGERED countdown */
    u8 launchedCount;               /* +0xC56: coins launched in staggered mode (limits the sound effect) */
    u8 padC57;
};

/* ------------------------------------------------------------------------------------------------------ */
/* Die roll (gDiceScreen)                                                                                 */
/* ------------------------------------------------------------------------------------------------------ */

/* gDiceScreen.step: index into gDiceScreenSteps (gPlainDieScreenSteps for the plain die, whose entry
 * step is NULL), run by DiceScreen_Update. Verified against the tables at 0x08199A10 / 0x08199A28. */
enum DiceStep {
    DICE_STEP_ENTER = 0,    /* DiceScreen_CharacterEnter */
    DICE_STEP_HOLD = 1,     /* DiceScreen_HoldDie */
    DICE_STEP_THROW = 2,    /* DiceScreen_ThrowDie: the throw */
    DICE_STEP_BOUNCE = 3,   /* DiceScreen_ThrowDie: the bounce */
    DICE_STEP_ROLL = 4,     /* DiceScreen_RollToResult */
    DICE_STEP_DONE = 5      /* NULL end of the table */
};

/* gDiceScreen.variant, set by DiceScreen_Setup*. */
enum DiceVariant {
    DICE_VARIANT_SKULL = 0,     /* Skull Dice: the character, dice palette 2 */
    DICE_VARIANT_GRACEFUL = 1,  /* Graceful Dice: the character, which spirals away at the end */
    DICE_VARIANT_PLAIN = 2      /* a die without a character */
};

/* gDiceScreen (0x0201F820, 0xAF0 bytes): work area of the die roll, cleared by DiceScreen_PrepareRoll. */
struct DiceScreen {
    struct OamList oamList;         /* +0x000: layered OAM list (OamListClear / OamListAlloc / OamListFlush) */
    struct ObjAffine affine[32];    /* +0x618: ObjAffineInit */
    struct AnimState anims[20];     /* +0x918: AnimBlockInit block: [0] the character hovering and throwing,
                                     *   [1] the character's result pose */
    u16 animCount;                  /* +0xAA8: set by AnimBlockInit */
    u8 unkAAA[0xABC - 0xAAA];
    struct Fade fade;               /* +0xABC: screen fade; fade.state (+0xAC2) 2 = the fade-out is complete */
    u8 step;                        /* +0xAC4: enum DiceStep */
    u8 spinTimer;                   /* +0xAC5: +1 per frame while dieEase runs; tumble frame (spinTimer >> 2) & 7,
                                     *   also the phase of the character's bob */
    u8 rollAxis;                    /* +0xAC6: current roll axis (0-2), into gDieRollFrames */
    u8 finalAxis;                   /* +0xAC7: axis of the final roll, from gDieFacePaths */
    u8 rollStart;                   /* +0xAC8: start frame of the current roll (copied from finalRollStart) */
    u8 finalRollStart;              /* +0xAC9: start frame of the final roll ((quarter + 2) % 4) * 5 + 2 */
    u8 result;                      /* +0xACA: die face 1-6, copied to gDuelScene.result at the end */
    u8 resultTimer;                 /* +0xACB: countdown from 0xFF after the die stops; leaves at the wrap */
    u8 throwStarted;                /* +0xACC: set when DiceScreen_ThrowDie starts dieEase */
    u8 diePalette;                  /* +0xACD: OBJ palette of the die pieces (2 Skull Dice, else 0) */
    u8 unkACE[2];
    struct Ease dieEase;            /* +0xAD0: the die's hop and roll parameter */
    struct Ease enterEase;          /* +0xAD8: character entry 0 -> 0x100 (also drives its sway) */
    struct Ease holdEase;           /* +0xAE0: 15-frame wait before the throw */
    u16 charFade;                   /* +0xAE8: character alpha, 8.8 (0x1600, -0x18 per frame; written to
                                     *   BLDALPHA once <= 0x1000) */
    u8 variant;                     /* +0xAEA: enum DiceVariant */
    u8 unkAEB;
    u16 orbitAngle;                 /* +0xAEC: 8.8 angle of the Graceful Dice character's exit spiral (0x4000) */
    u16 orbitSpeed;                 /* +0xAEE: angle step, +8 per frame (starts at 0x2B0) */
};

/* ------------------------------------------------------------------------------------------------------ */
/* gSceneWork: Exodia and Destiny Board win scenes, turn-order screen                                      */
/* ------------------------------------------------------------------------------------------------------ */

/* gDuelScene.sceneStep while gExodiaSceneSteps runs (table at 0x08199DA4). */
enum ExodiaSceneStep {
    EXODIA_STEP_INIT = 0,
    EXODIA_STEP_LOAD_EYE = 1,
    EXODIA_STEP_FADE_IN_EYE = 2,
    EXODIA_STEP_ASSEMBLE_PIECES = 3,
    EXODIA_STEP_GATHER_PIECES = 4,
    EXODIA_STEP_LOAD_FLAMES = 5,
    EXODIA_STEP_FADE_IN_FLAMES = 6,
    EXODIA_STEP_BLEND_FLAMES = 7,
    EXODIA_STEP_FINALE = 8
};

/* gSceneWork.u.exodia.piecesState: sub-states of ExodiaScene_AssemblePieces. */
enum ExodiaPiecesState {
    PIECES_APPEAR = 0,      /* one piece appears every 15 frames */
    PIECES_FLASH_IN = 1,    /* white flash on the sprites */
    PIECES_FLASH_OUT = 2,
    PIECES_LAUNCH = 3       /* start the flight to the centre */
};

/* gDuelScene.sceneStep while gDestinyBoardSceneSteps runs (table at 0x08199DFC). */
enum DestinyBoardSceneStep {
    DESTINY_STEP_INIT = 0,
    DESTINY_STEP_LOAD = 1,
    DESTINY_STEP_UPDATE = 2,
    DESTINY_STEP_DISABLE_HBLANK = 3
};

/* gSceneWork.u.destinyBoard.animPhase in DestinyBoardScene_Update (medium confidence). */
enum DestinyBoardAnimPhase {
    DESTINY_PHASE_ANIM0 = 0,    /* anims[0] plays; anims[1] starts when it ends */
    DESTINY_PHASE_ANIM1 = 1,    /* anims[1] plays; anims[3] and the pause start when it ends */
    DESTINY_PHASE_PAUSE = 2,    /* 30 frames */
    DESTINY_PHASE_LETTERS = 3   /* the F-I-N-A-L letters fly off */
};

/* FinalLetter.state */
enum FinalLetterState {
    LETTER_AT_REST = 0,
    LETTER_FLYING = 1,          /* launched by DestinyBoardScene_LaunchLetters */
    LETTER_GONE = 2             /* reached the end of its arc, hidden */
};

/* A vertically scrolling BG layer of the Destiny Board scene (ScrollLayer_*). */
struct ScrollLayer {
    u8 moving:3;            /* +0x00 bits 0-2: nonzero = moving (set to 1 by the scene update) */
    u8 unk0_3:5;
    u8 unk1;
    s16 speed;              /* +0x02: 12.4 fixed pixels per frame */
    s16 target;             /* +0x04: 12.4 stop position */
    s16 pos;                /* +0x06: 12.4 vertical scroll (BGnVOFS = pos >> 4) */
    s8 row;                 /* +0x08: last streamed tile row (-1 after ScrollLayer_Init) */
    u8 unk9[3];
    const u16 *srcMap;      /* +0x0C: tall source map, 30 entries per row */
    u16 *bgMap;             /* +0x10: BG screen block, 32 entries per row */
};

/* One of the five F-I-N-A-L letters of the Destiny Board scene. */
struct FinalLetter {
    u8 t;                   /* +0x0: flight progress: pulse phase and arc parameter (+2 per frame, +1 for A) */
    u8 state;               /* +0x1: enum FinalLetterState */
    u8 unk2[2];
};

/* gSceneWork.u during the Exodia win scene. Offsets in the comments are from the start of gSceneWork. */
struct ExodiaSceneWork {
    u8 piecesState;                 /* +0xAAC: enum ExodiaPiecesState */
    u8 unkAAD[3];
    struct Line pieceLines[5];      /* +0xAB0: paths of the five flying pieces (x, y at +0 / +2) */
    u8 unkB14;                      /* +0xB14: cleared in ExodiaScene_Init, otherwise unused */
    u8 unkB15;                      /* +0xB15: likewise */
    u8 pulsePhase;                  /* +0xB16: sine phase: whiteout acceleration (step 4), sprite pulse and
                                     *   red tint (steps 6-8) */
    u8 unkB17;
    struct Fade fade;               /* +0xB18: state 1 running, 2 full, 3 empty */
    struct Timer timer;             /* +0xB20: TICK_DONE when expired */
    u8 wavePhase;                   /* +0xB24: ExodiaScene_HBlank phase */
    u8 waveTick;                    /* +0xB25: 2-frame divider of wavePhase */
    u8 unkB26[2];
    struct PalFade palFade;         /* +0xB28: red tint of every palette (palFade.step at +0x1728) */
    u8 prevPieceFrame;              /* +0x1734: last anims[0] frame that played SE 0x13 */
    u8 pieceFrame;                  /* +0x1735: current anims[0] frame */
    u8 unk1736[2];
};

/* gSceneWork.u during the Destiny Board win scene (offsets from the start of gSceneWork). */
struct DestinyBoardSceneWork {
    struct Fade fade;               /* +0xAAC: state 2 ends the update step */
    struct ScrollLayer layers[4];   /* +0xAB4: vertical scroll of BG0-3 */
    u8 animPhase;                   /* +0xB04: enum DestinyBoardAnimPhase */
    u8 layerDelay;                  /* +0xB05: countdown from 0x62; at 1 layers 1-3 start scrolling */
    u8 wavePhase;                   /* +0xB06: DestinyBoardScene_HBlank phase and horizontal drift */
    u8 waveTimer;                   /* +0xB07: 9-frame divider of wavePhase */
    u8 letterTimer;                 /* +0xB08: frames to the next letter launch (20 -> 0xFF) */
    u8 letterTick;                  /* +0xB09: launch step 0-5, index into gFinalLetterLaunchOrder */
    u8 unkB0A[2];
    struct FinalLetter letters[5];  /* +0xB0C: F, I, N, A, L */
    u8 pauseTimer;                  /* +0xB20: 30-frame pause before the letters launch */
    u8 unkB21[3];
};

/* gSceneWork.u during the turn-order screen (offsets from the start of gSceneWork; the enums are in
 * turn_order.h). */
struct TurnOrderSceneWork {
    struct Scroller scrollers[4];   /* +0xAAC: enum TurnOrderScroller: [0] card carousel (pos = angle, stop =
                                     *   selected hand), [1] opponent-card slide (0..0x30); [2], [3] unused */
    u8 opponentHand;                /* +0xABC: enum RpsHand, 0xFF before the first answer */
    u8 opponentAnswered;            /* +0xABD: 1 once the opponent's hand is in; gates the result banner */
    u8 result;                      /* +0xABE: enum RpsResult */
    u8 turnChoice;                  /* +0xABF: enum TurnChoice: cursor, then the choice (FIRST 0 / SECOND 1,
                                     *   0xFF none); & 1 is latched into gMain.firstPlayer */
    u16 blendMask;                  /* +0xAC0: enum TurnOrderBlendBit: sprite groups drawn semi-transparent */
    u8 unkAC2[2];
    struct ChoiceBob choiceBob[2];  /* +0xAC4: bob of the FIRST / SECOND banners */
    u16 blendLevel;                 /* +0xACC: 8.8 alpha for SetBldAlpha, 0 -> 0x1000 while the choice is up */
    s8 logoSwing;                   /* +0xACE: DUEL logo rotation, -12 -> 0 by 3 */
    u8 logoDrop;                    /* +0xACF: DUEL logo drop step 0-16, index into gSquareTable */
    u16 brightness;                 /* +0xAD0: 8.8 BLDY level of the white flash and the fade-out */
    u16 brightnessStep;             /* +0xAD2: added to brightness every frame */
    u8 carouselSpread;              /* +0xAD4: angle between the carousel cards, 0 -> 0x55 after the fade-in */
    u8 carouselSpreadSpeed;         /* +0xAD5: 6 while the cards spread out, then 0 */
    u8 unkAD6[6];
    struct Tween tween;             /* +0xADC: the banner animations; tween.state (+0xAF0) 2 = finished */
    u8 frame;                       /* +0xAF4: frame counter (bob phase, the CPU's turn pick) */
    u8 phase;                       /* +0xAF5: enum TurnOrderPhase, index into gTurnOrderRpsSubsteps /
                                     *   gTurnOrderChoiceSubsteps */
    u8 unkAF6[2];
    struct Fade fade;               /* +0xAF8 */
    struct Timer timer;             /* +0xB00: white-flash hold */
    u8 unkB04[9];
    u8 linkWaiting;                 /* +0xB0D: 1 while waiting for the partner's packet */
    u8 isLink;                      /* +0xB0E: 1 under TurnOrder_RunRpsLink (Link Battle), 0 under the Campaign
                                     *   runners */
    u8 unkB0F;
    struct LinkSync linkSync;       /* +0xB10: LinkSyncStep exchange; linkSync.rx.data (+0xB16) is the partner's
                                     *   value */
    u8 rematchSend;                 /* +0xB1C: value sent with message 0x53 after a draw: 2 = ready to replay */
    u8 rematchReady;                /* +0xB1D: the player pressed A on a linked draw */
    u8 rematchTimer;                /* +0xB1E: frames on the draw result, saturating at 0xFF */
    u8 unkB1F;
    u16 logoTimer;                  /* +0xB20: frames in TurnOrder_ShowDuelLogo; the flash comes at 90 */
    u8 unkB22[2];
};

/* gSceneWork (0x02020310, 0x1738 bytes): work area shared by the Exodia and Destiny Board win scenes and the
 * turn-order screen; also the Get-a-pack list and starter-deck screen (booster.h views). */
struct SceneWork {
    struct OamList oamList;         /* +0x000: OAM layer lists (OamListAlloc / OamListFlush / OamListClear) */
    struct ObjAffine aff[32];       /* +0x618: OBJ affine records (ObjAffineInit / ObjAffineApply) */
    struct AnimState anims[20];     /* +0x918: script animations (AnimBlockInit); anims[3] has its own symbol
                                     *   gFinalLettersAnim */
    u16 animCount;                  /* +0xAA8: number of anims, written by AnimBlockInit (block + 0x190) */
    u8 unkAAA[2];
    union {
        struct ExodiaSceneWork exodia;
        struct DestinyBoardSceneWork destinyBoard;
        struct TurnOrderSceneWork turnOrder;
    } u;                            /* +0xAAC: per-scene data */
};

/* Layout checks (agbcc pads every struct to 4 bytes). */
STATIC_ASSERT(sizeof(struct DuelScene) == 0x10, DuelSceneSize);
STATIC_ASSERT(OFFSET_OF(struct DuelScene, arg) == 0x6, DuelSceneArg);
STATIC_ASSERT(OFFSET_OF(struct DuelScene, result) == 0x8, DuelSceneResult);
STATIC_ASSERT(OFFSET_OF(struct DuelScene, runnerStep) == 0xA, DuelSceneRunnerStep);
STATIC_ASSERT(OFFSET_OF(struct DuelScene, unkD) == 0xD, DuelSceneUnkD);
STATIC_ASSERT(sizeof(struct Coin) == 0xC, CoinSize);
STATIC_ASSERT(OFFSET_OF(struct Coin, height) == 0x4, CoinHeight);
STATIC_ASSERT(OFFSET_OF(struct Coin, glintState) == 0xA, CoinGlintStateOffset);
STATIC_ASSERT(sizeof(struct CoinToss) == 0x68, CoinTossSize);
STATIC_ASSERT(OFFSET_OF(struct CoinToss, count) == 0x60, CoinTossCount);
STATIC_ASSERT(OFFSET_OF(struct CoinToss, done) == 0x65, CoinTossDone);
STATIC_ASSERT(sizeof(struct Sparkle) == 0x8, SparkleSize);
STATIC_ASSERT(OFFSET_OF(struct Sparkle, x) == 0x4, SparkleX);
STATIC_ASSERT(sizeof(struct SparklePool) == 0x104, SparklePoolSize);
STATIC_ASSERT(OFFSET_OF(struct SparklePool, next) == 0x100, SparklePoolNext);
STATIC_ASSERT(sizeof(struct Scroller) == 0x4, ScrollerSize);
STATIC_ASSERT(sizeof(struct ChoiceBob) == 0x4, ChoiceBobSize);
STATIC_ASSERT(sizeof(struct CoinTossWork) == 0xC58, CoinTossWorkSize);
STATIC_ASSERT(OFFSET_OF(struct CoinTossWork, toss) == 0xAAC, CoinTossWorkToss);
STATIC_ASSERT(OFFSET_OF(struct CoinTossWork, scrollers) == 0xB14, CoinTossWorkScrollers);
STATIC_ASSERT(OFFSET_OF(struct CoinTossWork, unkB2C) == 0xB2C, CoinTossWorkUnkB2C);
STATIC_ASSERT(OFFSET_OF(struct CoinTossWork, unkB3E) == 0xB3E, CoinTossWorkUnkB3E);
STATIC_ASSERT(OFFSET_OF(struct CoinTossWork, timer) == 0xB40, CoinTossWorkTimer);
STATIC_ASSERT(OFFSET_OF(struct CoinTossWork, fade) == 0xB48, CoinTossWorkFade);
STATIC_ASSERT(OFFSET_OF(struct CoinTossWork, sparkles) == 0xB50, CoinTossWorkSparkles);
STATIC_ASSERT(OFFSET_OF(struct CoinTossWork, launchMode) == 0xC54, CoinTossWorkLaunchMode);
STATIC_ASSERT(sizeof(struct DiceScreen) == 0xAF0, DiceScreenSize);
STATIC_ASSERT(OFFSET_OF(struct DiceScreen, anims) == 0x918, DiceScreenAnims);
STATIC_ASSERT(OFFSET_OF(struct DiceScreen, animCount) == 0xAA8, DiceScreenAnimCount);
STATIC_ASSERT(OFFSET_OF(struct DiceScreen, fade) == 0xABC, DiceScreenFade);
STATIC_ASSERT(OFFSET_OF(struct DiceScreen, step) == 0xAC4, DiceScreenStep);
STATIC_ASSERT(OFFSET_OF(struct DiceScreen, diePalette) == 0xACD, DiceScreenDiePalette);
STATIC_ASSERT(OFFSET_OF(struct DiceScreen, dieEase) == 0xAD0, DiceScreenDieEase);
STATIC_ASSERT(OFFSET_OF(struct DiceScreen, charFade) == 0xAE8, DiceScreenCharFade);
STATIC_ASSERT(OFFSET_OF(struct DiceScreen, orbitSpeed) == 0xAEE, DiceScreenOrbitSpeed);
STATIC_ASSERT(sizeof(struct ScrollLayer) == 0x14, ScrollLayerSize);
STATIC_ASSERT(OFFSET_OF(struct ScrollLayer, row) == 0x8, ScrollLayerRow);
STATIC_ASSERT(OFFSET_OF(struct ScrollLayer, srcMap) == 0xC, ScrollLayerSrcMap);
STATIC_ASSERT(sizeof(struct FinalLetter) == 0x4, FinalLetterSize);
STATIC_ASSERT(sizeof(struct ExodiaSceneWork) == 0xC8C, ExodiaSceneWorkSize);
STATIC_ASSERT(sizeof(struct DestinyBoardSceneWork) == 0x78, DestinyBoardSceneWorkSize);
STATIC_ASSERT(sizeof(struct TurnOrderSceneWork) == 0x78, TurnOrderSceneWorkSize);
STATIC_ASSERT(sizeof(struct SceneWork) == 0x1738, SceneWorkSize);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, anims) == 0x918, SceneWorkAnims);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, animCount) == 0xAA8, SceneWorkAnimCount);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u) == 0xAAC, SceneWorkU);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.exodia.pieceLines) == 0xAB0, SceneWorkExodiaPieceLines);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.exodia.fade) == 0xB18, SceneWorkExodiaFade);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.exodia.timer) == 0xB20, SceneWorkExodiaTimer);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.exodia.palFade) == 0xB28, SceneWorkExodiaPalFade);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.exodia.palFade.step) == 0x1728, SceneWorkExodiaPalFadeStep);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.exodia.prevPieceFrame) == 0x1734, SceneWorkExodiaPrevPieceFrame);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.destinyBoard.layers) == 0xAB4, SceneWorkDestinyBoardLayers);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.destinyBoard.animPhase) == 0xB04, SceneWorkDestinyBoardAnimPhase);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.destinyBoard.letters) == 0xB0C, SceneWorkDestinyBoardLetters);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.destinyBoard.pauseTimer) == 0xB20, SceneWorkDestinyBoardPauseTimer);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.opponentHand) == 0xABC, SceneWorkTurnOrderOpponentHand);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.blendMask) == 0xAC0, SceneWorkTurnOrderBlendMask);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.choiceBob) == 0xAC4, SceneWorkTurnOrderChoiceBob);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.blendLevel) == 0xACC, SceneWorkTurnOrderBlendLevel);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.carouselSpreadSpeed) == 0xAD5, SceneWorkTurnOrderSpreadSpeed);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.tween) == 0xADC, SceneWorkTurnOrderTween);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.tween.state) == 0xAF0, SceneWorkTurnOrderTweenState);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.frame) == 0xAF4, SceneWorkTurnOrderFrame);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.fade) == 0xAF8, SceneWorkTurnOrderFade);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.timer) == 0xB00, SceneWorkTurnOrderTimer);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.linkWaiting) == 0xB0D, SceneWorkTurnOrderLinkWaiting);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.linkSync) == 0xB10, SceneWorkTurnOrderLinkSync);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.linkSync.rx.data) == 0xB16, SceneWorkTurnOrderLinkSyncRxData);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.rematchSend) == 0xB1C, SceneWorkTurnOrderRematchSend);
STATIC_ASSERT(OFFSET_OF(struct SceneWork, u.turnOrder.logoTimer) == 0xB20, SceneWorkTurnOrderLogoTimer);

/* ------------------------------------------------------------------------------------------------------ */
/* Data                                                                                                   */
/* ------------------------------------------------------------------------------------------------------ */

extern struct DuelScene gDuelScene;             /* 0x02017A30 */
extern struct CoinTossWork gCoinTossWork;       /* 0x02015280 */
/* Alias symbol of gCoinTossWork.sparkles (0x02015DD0); duel_field_view.c passes it to the sparkle
 * functions by this name. */
extern struct SparklePool gCoinTossSparkles;
extern struct DiceScreen gDiceScreen;           /* 0x0201F820 */
extern struct SceneWork gSceneWork;             /* 0x02020310 */
/* Alias symbol of gSceneWork.anims[3] (0x02020C64): the letters animation of the Destiny Board scene,
 * whose frame templates DestinyBoardScene_DrawFinalLetters reads. */
extern struct AnimState gFinalLettersAnim;

/* ------------------------------------------------------------------------------------------------------ */
/* Functions                                                                                              */
/* ------------------------------------------------------------------------------------------------------ */

/* ---- Scene runner (duel_cmd_queue.c) ---- */

/* Starts scene `id` (enum DuelSceneId): handler = gDuelSceneHandlers[id], steps cleared; the two win
 * scenes also stop the duel music. Always clears gDuelScreen.uiGfxLoaded. */
void DuelScene_Start(u16 id, u32 flag);
/* Runner step 0: clears the VBlank callback and the duel screen flags, then fades the duel screen out;
 * returns 1 when it is gone. */
u16 DuelScene_FadeOutDuelScreen(void);
/* Runner step 1: returns gDuelScene.handler() (1 when the scene has finished), 0 without a handler. */
u16 DuelScene_RunHandler(void);
/* Unreferenced step that rebuilds the duel screen and fades it in (the game uses DUEL_CMD_OPEN_DUEL_SCREEN
 * instead). */
u16 DuelScene_FadeInDuelScreen(void);
/* Runs gDuelSceneRunnerSteps[runnerStep]; returns 1 when the scene has finished. */
u16 DuelScene_Run(void);
/* Unused HBlank handler: BG0, BG1 and BG3 HOFS from gDuelCmd.hofsTable, a 16-line wave. */
void HBlank_WaveBg013(void);
/* Unused HBlank handler: as HBlank_WaveBg013 for BG0-3. */
void HBlank_WaveAllBgs(void);

/* ---- Coin toss (duel_field_view.c, coin_toss_scene.c) ---- */

/* Scene handler of DUEL_SCENE_COIN_TOSS: runs gCoinTossSteps; returns 1 at the end of the table. */
u32 CoinToss_Run(void);
/* As CoinToss_Run, but the coins are launched one every 16 frames after a 31-frame wait. */
u32 CoinToss_RunStaggered(void);
/* Step 0: clears gCoinTossWork and the display, initialises the OAM list, affine records and sparkles. */
u32 CoinToss_Init(void);
/* Step 1: fade-in, backdrop bitmap, coin, glint and sparkle graphics, CoinToss_InitCoins; mode 4 on. */
u32 CoinToss_Load(void);
/* Step 2, per frame: launches, flies and draws the coins, marks and glints the matching ones, then fades
 * out; returns 1 when the fade-out is done or the timer passes 0x140. */
u32 CoinToss_Update(void);
/* Resets the eight coins and decodes gDuelScene.arg into count and calledFace. */
void CoinToss_InitCoins(struct CoinToss *toss);
/* Advances the spin frame of each flying coin every 4 frames. */
void CoinToss_AnimateSpin(struct Coin *coins, u8 count);
/* Unreferenced: as CoinToss_UpdateFlight, but every coin lands on one face (mask bit 0: 1 = heads) and
 * leaves no sparkle trail. */
void CoinToss_UpdateFlightUniform(struct Coin *coins, u8 count, u8 mask, u8 *headsCount);
/* Moves the flying coins along their arc; a landing coin takes its face from bit i of gDuelScene.arg and is
 * counted in *headsCount if heads. Spawns a sparkle under each coin every other frame. */
void CoinToss_UpdateFlight(struct Coin *coins, u8 count, u32 unused, u8 *headsCount);
/* Draws each coin as a 32x32 sprite, spread evenly across the screen. */
void CoinToss_DrawCoins(struct Coin *coins, u8 count);
/* Draws the glint over each coin whose glint is playing (frame set by calledFace). */
void CoinToss_DrawGlints(struct Coin *coins, u8 count, u8 calledFace);
/* Advances the glint of each coin every 7 frames; after frame 4 the glint is done. */
void CoinToss_AnimateHighlights(struct Coin *coins, u8 count);
/* Once every coin has landed: coins showing calledFace start their glint (state COIN_STATE_MATCHED), the
 * others are resolved at once. Runs only once. */
void CoinToss_MarkMatchingCoins(struct Coin *coins, u8 count, u8 calledFace);
/* Number of coins not resolved yet (glintState != COIN_HIGHLIGHT_DONE). */
u8 CoinToss_CountUnfinished(struct Coin *coins, u8 count);
/* Starts a sparkle at (x + Random() % 16, y) in the next slot of the pool; returns 1. */
u32 CoinToss_SpawnSparkle(u8 x, u8 y, struct SparklePool *pool);
/* Advances every active sparkle by one frame; it ends at the 0 entry of gCoinSparkleTiles (22 frames).
 * Declared u32, returns nothing. */
u32 CoinToss_UpdateSparkles(struct SparklePool *pool);
/* Adds every active sparkle to the coin-toss OAM list as a semi-transparent 16x16 sprite. Declared u32,
 * returns nothing. */
u32 CoinToss_DrawSparkles(struct SparklePool *pool);
/* Deactivates all 32 sparkles and resets next. */
void CoinToss_ClearSparkles(struct SparklePool *pool);

/* ---- Die roll (coin_toss_scene.c, dice_scene.c) ---- */

/* Scene handler of DUEL_SCENE_DICE_SKULL: runs gSkullDiceSceneSteps; returns 1 at the end of the table. */
u32 DuelScene_SkullDice(void);
/* Scene handler of DUEL_SCENE_DICE_GRACEFUL: runs gDiceScreenGracefulSteps. */
u16 DiceScreen_RunGraceful(void);
/* Scene handler of DUEL_SCENE_DICE_PLAIN: runs gDiceScreenPlainSteps. */
u16 DiceScreen_RunPlain(void);
/* Outer step 0: clears gDiceScreen and picks a roll path that ends on the face in gDuelScene.arg. */
u32 DiceScreen_PrepareRoll(void);
/* Outer step 1: display off, OAM list and affine records, eases and timers initialised. */
u32 DiceScreen_Init(void);
/* Outer step 2, Skull Dice: variant DICE_VARIANT_SKULL, die palette 2. */
u32 DiceScreen_SetupSkullDice(void);
/* Outer step 2, Graceful Dice: variant DICE_VARIANT_GRACEFUL. */
u32 DiceScreen_SetupGracefulDice(void);
/* Outer step 2, plain die: variant DICE_VARIANT_PLAIN (no character). */
u32 DiceScreen_SetupPlainDie(void);
/* Outer step 3: character animations, fade-in, backdrop and OBJ graphics; the plain die skips to
 * DICE_STEP_HOLD. */
u32 DiceScreen_LoadGraphics(void);
/* Outer step 4, per frame: runs gDiceScreenSteps[step]; when the fade-out is done stores the face in
 * gDuelScene.result and returns 1. */
u32 DiceScreen_Update(void);
/* DICE_STEP_ENTER: the character descends with a sideways sway, holding the die. */
u32 DiceScreen_CharacterEnter(void);
/* DICE_STEP_HOLD: 15-frame hold, then the throw sound. */
u32 DiceScreen_HoldDie(void);
/* DICE_STEP_THROW and DICE_STEP_BOUNCE: the die's arc and bounce; returns 1 when it lands. */
u32 DiceScreen_ThrowDie(void);
/* DICE_STEP_ROLL: the final roll to the result face, the character's exit; returns 1 on A or when the
 * result has been shown for 256 frames (starts the fade-out). */
u32 DiceScreen_RollToResult(void);
/* Allocates an OAM entry on `layer` of `list` (a struct OamList) and fills it from a 4-halfword sprite
 * piece at (x, y); returns the entry. K&R definition: in-unit callers pass unnarrowed u8/u16 arguments. */
u16 *DiceScreen_AddOamPiece(u16 *piece, int layer, int x, int y, int palette, int tileBase, void *list);
/* Emits the current frame of `anim` at (x, y) on layer 1, OR-ing attr0Flags into attr0 (0x400 =
 * semi-transparent); nothing for the plain die. K&R definition. */
void DiceScreen_DrawCharacter(struct AnimState *anim, int x, int y, int attr0Flags);
/* AnimStateTick(anim) (a struct AnimState) unless the variant is the plain die. */
void DiceScreen_TickCharacter(void *anim);

/* ---- Exodia win scene (dice_scene.c) ---- */

/* Scene handler of DUEL_SCENE_EXODIA_WIN: runs gExodiaSceneSteps (enum ExodiaSceneStep). The Millennium
 * Eye fades in, the five pieces appear and fly to the centre in a whiteout, then Exodia rises in flames. */
u16 ExodiaScene_Run(void);
/* EXODIA_STEP_INIT: clears gSceneWork and sets up the display and blending. */
u16 ExodiaScene_Init(void);
/* EXODIA_STEP_LOAD_EYE: Millennium Eye bitmap and the piece sprites, 60-frame timer. */
u16 ExodiaScene_LoadEye(void);
/* EXODIA_STEP_FADE_IN_EYE: fade-in from black until the timer expires. */
u16 ExodiaScene_FadeInEye(void);
/* EXODIA_STEP_ASSEMBLE_PIECES: the pieces appear one by one and flash (enum ExodiaPiecesState). */
u16 ExodiaScene_AssemblePieces(void);
/* EXODIA_STEP_GATHER_PIECES: the pieces fly to the centre in an accelerating whiteout. */
u16 ExodiaScene_GatherPieces(void);
/* EXODIA_STEP_LOAD_FLAMES: the Exodia-in-flames background and sprites, red palette fade, HBlank wave. */
u16 ExodiaScene_LoadFlames(void);
/* EXODIA_STEP_FADE_IN_FLAMES: fade-in from white, then starts the BG0 alpha ramp. */
u16 ExodiaScene_FadeInFlames(void);
/* EXODIA_STEP_BLEND_FLAMES: blends BG0 down to 10/16. */
u16 ExodiaScene_BlendFlames(void);
/* EXODIA_STEP_FINALE: pulsing red tint, then a fade to black; returns 1 at the end or on B. */
u16 ExodiaScene_Finale(void);
/* Per-frame sprites of steps 6-8: the two animations, the HBlank wave phase and the pulse. */
void ExodiaScene_DrawSprites(void);
/* gSceneWork.u.exodia.piecesState = PIECES_APPEAR; the argument is ignored. */
void ExodiaScene_ResetPieceState(void *unused);
/* Starts the five piece paths (LineInit) from gExodiaPieceStartPos to the screen centre. */
void ExodiaScene_StartPieceFlight(struct Line *lines);
/* HBlank handler: BG0 HOFS follows a sine of the scanline (the flame wave). */
void ExodiaScene_HBlank(void);
/* Emits the current frame of `anim` on OAM layer 0 as affine sprites in the second OBJ char block. */
void ExodiaScene_DrawAffineAnim(struct AnimState *anim, u8 priority, void *oamList);
/* Copies a linear 32x32-pixel (4x4-tile) block into 2D-mapped OBJ VRAM. */
void CopyObjTileBlock4x4(const u8 *src, u8 *dst);
/* CopyObjTileBlock4x4 of tile srcTile of src to OBJ tile 512 + dstTile (the bitmap-mode OBJ area); the
 * fourth argument is never read. */
void LoadObjTileBlock4x4(const u8 *src, u32 dstTile, u32 srcTile, u32 unused);

/* ---- Destiny Board win scene (dice_scene.c, destiny_board_scene.c) ---- */

/* Scene handler of DUEL_SCENE_DESTINY_BOARD_WIN: runs gDestinyBoardSceneSteps (enum DestinyBoardSceneStep). */
u32 DestinyBoardScene_Run(void);
/* DESTINY_STEP_INIT: clears the scene work, display off, animations, the four scroll layers. */
u32 DestinyBoardScene_Init(void);
/* DESTINY_STEP_LOAD: fade-in, maps, tiles, palettes, VBlank and HBlank handlers, music. */
u32 DestinyBoardScene_Load(void);
/* DESTINY_STEP_UPDATE, per frame: scroll layers, the ghost and board animations, then the F-I-N-A-L
 * letters (enum DestinyBoardAnimPhase); returns 1 when the fade-out is done. */
u32 DestinyBoardScene_Update(void);
/* DESTINY_STEP_DISABLE_HBLANK: masks the HBlank interrupt; returns 1. */
u32 DestinyBoardScene_DisableHBlank(void);
/* HBlank handler: two drifting wavy layers (BG0 and BG3 HOFS from gDestinyBoardWaveTable). */
void DestinyBoardScene_HBlank(void);
/* Advances the HBlank wave phase every 9 frames. */
void DestinyBoardScene_AdvanceWave(void);
/* VBlank callback: BGnVOFS = layers[n].pos >> 4, HOFS 0, for BG0-3. */
void DestinyBoardScene_VBlank(void);
/* Launches the next letter every 21 frames (gFinalLetterLaunchOrder); once letter N is far enough, fades
 * out. */
void DestinyBoardScene_LaunchLetters(void);
/* Puts the five letters back at rest. */
void DestinyBoardScene_ResetLetters(void);
/* Draws the five letters with their ghost auras relative to (x, y); flying letters pulse and follow an arc. */
void DestinyBoardScene_DrawFinalLetters(u32 unused, u16 x, u16 y);
/* Sets up a scroll layer: position 0, speed and target (12.4), stopped, row -1, the source map (30 entries
 * per row) and the BG map. */
void ScrollLayer_Init(u8 *srcMap, u8 *bgMap, s16 speed, s16 target, struct ScrollLayer *layer);
/* If the layer is moving: pos += speed, stopping exactly at target. */
void ScrollLayer_Move(struct ScrollLayer *layer);
/* When the scroll crosses an 8-pixel row, copies the source-map row about to enter the screen into the BG
 * map. */
void ScrollLayer_StreamRow(struct ScrollLayer *layer);

/* ---- Turn-order screen leftover (turn_order_scene.c) ---- */

/* Unreferenced: adds a 64x32 sprite (tile gUnk_0808270C[index], palette gUnk_08082710[index]) at
 * (0x58, 0x64) to layer 0 of gSceneWork's OAM list. */
void sub_080288DC(u8 index);

#endif /* GUARD_DUEL_SCENES_H */
