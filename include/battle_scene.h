#ifndef GUARD_BATTLE_SCENE_H
#define GUARD_BATTLE_SCENE_H

/*
 * Battle scene: the two battling cards shown full size side by side (player 0 left on BG2, player 1 right on
 * BG3, affine 8bpp), their ATK/DEF values, the damage and the destroyed card darkening.
 * DUEL_CMD_START_BATTLE_SCENE calls BattleScene_Init; DUEL_CMD_PLAY_BATTLE_SCENE runs BattleScene_Update until it
 * returns 1. The state lives in gBattle.scene (gBattle + 0x154), also addressed as gBattleScene.
 */

#include "global.h"

/* gBattle.scene.state, the steps of BattleScene_Update. */
enum BattleSceneState {
    BATTLE_SCENE_SHOW = 0,              /* show BG2/BG3/OBJ, darken everything */
    BATTLE_SCENE_OPEN = 1,              /* cards roll open while the screen fades in (subState 0-15) */
    BATTLE_SCENE_END_OPEN = 2,          /* remove the HBlank effect */
    BATTLE_SCENE_RESULT = 3,            /* values, hit and destroy sub-steps (enum BattleResultStep) */
    BATTLE_SCENE_FADE_OUT = 4,
    BATTLE_SCENE_DONE = 5               /* BattleScene_Update returns 1 */
};

/* gBattle.scene.subState during BATTLE_SCENE_RESULT. */
enum BattleResultStep {
    BATTLE_RESULT_START = 0,
    BATTLE_RESULT_SHOW_VALUES = 1,      /* show both values for 30 frames */
    BATTLE_RESULT_HIT = 2,              /* shake the damaged sides and flash the damage */
    BATTLE_RESULT_DESTROY = 3,          /* darken and hide the destroyed sides */
    BATTLE_RESULT_HOLD = 4              /* hold 60 frames or until B */
};

/* Per-side flag byte of BattleScene_Update / BattleScene_DrawValues (player 0 in bits 0-7, player 1 in 8-15). */
enum BattleSideFlags {
    BATTLE_SIDE_DEFENSE = 0x1,          /* show DEF instead of ATK */
    BATTLE_SIDE_DESTROYED = 0x2,
    BATTLE_SIDE_DAMAGE = 0x4            /* this side takes life-point damage */
};

/* gBattle.scene (12 bytes at gBattle + 0x154), cleared by BattleScene_Init. */
struct BattleScene {
    u8 unk0[8];                         /* +0x0: cleared, never read */
    u8 state;                           /* +0x8: enum BattleSceneState */
    u8 subState;                        /* +0x9: OPEN: fade frame 0-15 and HBlank table row; RESULT: BattleResultStep */
    u8 unkA;                            /* +0xA: zeroed in state 2, never read */
    u8 timer;                           /* +0xB: frame counter of the result sub-steps */
};

/* 0x020185A4: alias of gBattle.scene (battle.h embeds this struct there); prefer the member in new code. */
extern struct BattleScene gBattleScene;

/* Scene control */
/* Set up the scene for two card ids (0 leaves that side empty) and install BattleScene_HBlank. */
void BattleScene_Init(u16 cardId0, u16 cardId1);
/* Run one frame: values of player 0 / player 1, flags = BattleSideFlags per side; 1 when finished. */
u32 BattleScene_Update(u16 value0, u16 value1, u16 flags);
void BattleScene_HBlank(void);          /* per-line BG2/BG3 affine registers: the cards roll open vertically */
void BattleScene_ResetBgAffine(void);   /* undo the last BattleScene_HBlank values (identity BG2/BG3) */

/* Numbers */
void BattleScene_DrawSmallNumber(int x, int y, int value);              /* right-aligned 8x8 digits */
void BattleScene_DrawBigNumber(int x, int y, int value, int colour);    /* right-aligned 16x16 digits, colour 0-3 */
void BattleScene_DrawAtk(int side, int x, int y, u32 value);            /* "ATK" label and value on a card frame */
void BattleScene_DrawDef(int side, int x, int y, u32 value);            /* "DEF" label and value, one row lower */
/* Draw values[0] / values[1] with ATK or DEF labels; hideDestroyed skips BATTLE_SIDE_DESTROYED sides. */
void BattleScene_DrawValues(u16 flags, u32 *values, u16 hideDestroyed);
void BattleScene_DrawDamage(int side, int damage, u16 flash);           /* damage over a card; flash cycles colours */

/* Card graphics */
/* Load a card's 72x80 art and palette into BG 'bg' at tileBase, palette entries palBase. */
void BattleScene_LoadCardArt(int bg, u16 cardId, u16 tileBase, u16 palBase);
/* Load an 8bpp image pack (the card frame) into BG 'bg' at tileBase with palette offset palBase. */
void BattleScene_LoadCardFrame(int bg, u16 *imagePack, u16 tileBase, u16 palBase);
void BattleScene_SetCardArtMap(int bg, int x, int y, int tileBase);    /* 9x10 art tiles into the affine map */
void BattleScene_SetCardFrameMap(int bg, int x, int y, int tileBase);  /* 13x18 frame tiles around the art */

/* Compile-time layout checks (agbcc pads every struct to 4 bytes). */
typedef char battle_scene_h_check_state[(u32)&((struct BattleScene *)0)->state == 0x8 ? 1 : -1];
typedef char battle_scene_h_check_timer[(u32)&((struct BattleScene *)0)->timer == 0xB ? 1 : -1];
typedef char battle_scene_h_check_size[sizeof(struct BattleScene) == 0xC ? 1 : -1];

#endif /* GUARD_BATTLE_SCENE_H */
