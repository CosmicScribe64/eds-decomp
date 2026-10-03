#ifndef GUARD_BATTLE_H
#define GUARD_BATTLE_H

/*
 * The Battle Phase. BattlePhase_Run calls the handler of the current stage, gBattleStageHandlers
 * [gDuel.battleStage] (enum BattleStage in constants/duel.h); a handler returns 1 when its stage is done,
 * and gDuel.battleStep is its step. Handlers may also jump: a replay goes back to SELECT_ATTACKER, EndAttack
 * loops to it for the next attack, and EndBattlePhase restarts at START for an extra Battle Phase.
 * The attack itself (attacker and target zones, both monsters' values, damage) is kept in gBattle, which
 * CalcBattle fills.
 *
 * Every prototype is the function's definition as compiled. Units that call a function through another
 * local declaration keep that view as a commented local alias prototype (build/readability/proto_mismatches.txt).
 */

#include "global.h"
#include "duel.h"
#include "battle_scene.h"   /* struct BattleScene (gBattle.scene) */

/* One side of a battle (0xC bytes): gBattle.side[player], filled by CalcBattle. The bitfield container
 * types (u16 or u8) are the ones the code accesses them with. */
struct BattleSide {
    u16 slot:3;                 /* +0x0 bits 0-2: monster zone of this side's monster */
    u8 destroyed:1;             /* +0x0 bit 3: destroyed by the battle (the damage step clears the zone) */
    u8 defensePos:1;            /* +0x0 bit 4: in defense position (bit 0 of the battle-scene flags) */
    u16 destroyedCopy:1;        /* +0x0 bit 5: copy of destroyed made at the end of CalcBattle (Time Machine) */
    u8 effectDestroy:1;         /* +0x0 bit 6: destroyed after the battle (Sword of Dragon's Soul link) */
    u8 unk0_7:1;
    u8 unk1;                    /* +0x1 */
    u16 cardId;                 /* +0x2: card ID of this side's monster */
    u16 atk;                    /* +0x4: effective ATK, including the battle boosts */
    u16 def;                    /* +0x6: effective DEF */
    u16 battleValue;            /* +0x8: value compared: ATK, or DEF for a defender in defense position */
    u16 damage;                 /* +0xA: life-point damage this side's player takes */
};

/*
 * gBattle (0x02018450, 0x160 bytes, cleared by Duel_Setup): the current attack. The attacking monster is
 * (gDuel.turnPlayer, atkSlot) and the target (1 - turnPlayer, defSlot); defSlot 5 means a direct attack.
 */
struct Battle {
    u16 attacker:1;             /* +0x0 bit 0: attacking player */
    u16 direct:1;               /* +0x0 bit 1: direct attack */
    u16 flipEffectPending:1;    /* +0x0 bit 2: a face-down defender flipped by the attack has a flip effect;
                                 * BattleStage_TriggerFlipEffect queues it */
    u16 attackDeclared:1;       /* +0x0 bit 3: BattleStage_DeclareAttack ran (a replay does not pay twice) */
    u16 attackCostsPaid:1;      /* +0x0 bit 4: BattleStage_PayAttackCosts ran */
    u16 zeroAttackerAtk:1;      /* +0x0 bit 5: the attacker's ATK counts as 0 (DuelCmd_ZeroAttackerAtk) */
    u16 atkSlot:3;              /* +0x0 bits 6-8: attacking monster's zone */
    u16 defSlot:3;              /* +0x0 bits 9-11: target monster's zone (5 = direct attack) */
    u16 unk0_12:4;
    u16 flipCardId;             /* +0x2: card ID of the defender flipped face up by the attack, else 0 */
    u8 calculated:1;            /* +0x4 bit 0: set by CalcBattle */
    u8 unk4_1:7;
    u8 unk5[3];
    struct BattleSide side[2];  /* +0x08: per player */
    struct DuelZone zones[2];   /* +0x20: copies of both battling zones, per player (the card words
                                 * passed to BanishBattleDestroyedCard / SendBattleDestroyedCardToGraveyard) */
    u16 monsterCountSnapshot[2]; /* +0x148: both players' monster counts when the attack was declared */
    u16 zoneSerialSnapshot[2];  /* +0x14C: DuelZone.serial of the attacker's and the target's zone */
    u16 destroyedLoc[2];        /* +0x150: player | zone << 8 of the attacker / target sent away by
                                 * BattleStage_DestroyMonsters (0xFFFF = none) */
    /* +0x154: state of the battle scene (BattleScene_Update), cleared by BattleScene_Init; also reached
     * through the alias symbol gBattleScene (0x020185A4, battle_scene.h). scene.state is at +0x15C,
     * scene.subState +0x15D, scene.timer +0x15F. */
    struct BattleScene scene;
};

extern struct Battle gBattle;   /* 0x02018450 */

/* ---- Running the Battle Phase ---- */

/* Run one stage handler per call (gBattleStageHandlers[gDuel.battleStage]); 1 after the last stage, when the
 * Main Phase 2 banner is pushed. */
int BattlePhase_Run(int player);
/* Stage 0: Battle Phase banner and the attackable-monster mask. */
int BattleStage_Start(int player);
/* Stage 1: choose the attacking monster (human cursor or CPU), or end the Battle Phase from its menu. */
int BattleStage_SelectAttacker(int player);
/* Stage 2: choose the target or a direct attack (Ring of Magnetism, Toons). */
int BattleStage_SelectTarget(int player);
/* Stage 3: check the attacker, save the replay snapshots and pay its own attack cost. */
int BattleStage_DeclareAttack(int player);
/* Stage 4: costs from the field: Gravekeeper's Servant (mill), Toll (500 LP per copy). */
int BattleStage_PayAttackCosts(int player);
/* Stage 5: the attack-declaration response window (RESPONSE_ATTACK_DECLARED). */
int BattleStage_RespondToAttack(int player);
/* Stage 6: flip a face-down target face up; posts RESPONSE_DAMAGE_STEP. */
int BattleStage_RevealDefender(int player);
/* Stage 7: CalcBattle, the defender's answers (Sanga/Kazejin/Suijin, replacement targets), the battle scene
 * and clearing the destroyed monsters' zones. */
int BattleStage_DamageCalc(int player);
/* Stage 8: apply both sides' battle damage and the 'inflicts battle damage' effects (Kuriboh can stop it). */
int BattleStage_InflictDamage(int player);
/* Stage 9: queue the flip effect of a defender flipped by the attack. */
int BattleStage_TriggerFlipEffect(int player);
/* Stage 10: battle effects and sending destroyed monsters away; the second call posts
 * RESPONSE_BATTLE_DESTROYED. */
int BattleStage_DestroyMonsters(int player);
/* Stage 11: after the attack: Kotodama, control takeover by key 1243; then the next attack. */
int BattleStage_EndAttack(int player);
/* Stage 12: end-of-Battle-Phase effects and the graveyard flags set in stage 10. */
int BattleStage_EndBattlePhase(int player);
/* Stage 13: Magical Hats decoys and borrowed monsters, one card per call; 1 when none is left. */
int BattleStage_Cleanup(int player);

/* ---- Battle calculation ---- */

/* Fill gBattle.side[] for the current attack: values with the card boosts, destroyed flags and damage. */
void CalcBattle(int attacker, u16 zeroAtk);
/* After a response window: 1 (and back to SELECT_ATTACKER) when the attack cannot go on or must be
 * replayed; 0 to continue. */
int Battle_CheckReplay(int player);
/* Stub that always returns 0, called before each 'when this card battles' effect. */
int IsBattleEffectBlocked(void);

/* ---- Attack eligibility ---- */

/* May the player enter the Battle Phase (not on the first turn, once per turn, some monster can attack)? */
int CanEnterBattlePhase(int player);
/* Rebuild players[player].attackableMask; resetAttacked also clears the attacked mask first. */
void BuildAttackableMask(struct DuelPlayer *players, int player, int unused, u16 resetAttacked);
/* May the monster in (player, zone) attack? checkCost also requires its attack cost to be payable. */
u16 CanMonsterAttack(int player, int zone, u16 checkCost);
/* Can the monster attack directly although the opponent has monsters (Mystic Lamp, Toons, ...)? */
int CanAttackDirectly(int player, int zone);
/* 1 if a face-up The Regulation of Tribe forbids monsters of this type to attack. */
u16 IsTypeForbiddenToAttack(u16 type);
/* The monster in (player, zone) has attacked: set its positionLocked and the player's attackedMask bit. */
void MarkMonsterAttacked(int player, int zone);

/* Compile-time layout checks (agbcc pads every struct to a multiple of 4 bytes). */
typedef char battle_h_check_side[sizeof(struct BattleSide) == 0xC ? 1 : -1];
typedef char battle_h_check_side_card[(u32)&((struct BattleSide *)0)->cardId == 0x2 ? 1 : -1];
typedef char battle_h_check_side_damage[(u32)&((struct BattleSide *)0)->damage == 0xA ? 1 : -1];
typedef char battle_h_check_battle[sizeof(struct Battle) == 0x160 ? 1 : -1];
typedef char battle_h_check_flip_card[(u32)&((struct Battle *)0)->flipCardId == 0x2 ? 1 : -1];
typedef char battle_h_check_sides[(u32)&((struct Battle *)0)->side == 0x8 ? 1 : -1];
typedef char battle_h_check_zones[(u32)&((struct Battle *)0)->zones == 0x20 ? 1 : -1];
typedef char battle_h_check_count[(u32)&((struct Battle *)0)->monsterCountSnapshot == 0x148 ? 1 : -1];
typedef char battle_h_check_serial[(u32)&((struct Battle *)0)->zoneSerialSnapshot == 0x14C ? 1 : -1];
typedef char battle_h_check_destroyed[(u32)&((struct Battle *)0)->destroyedLoc == 0x150 ? 1 : -1];
typedef char battle_h_check_scene[(u32)&((struct Battle *)0)->scene == 0x154 ? 1 : -1];
typedef char battle_h_check_scene_state[(u32)&((struct Battle *)0)->scene.state == 0x15C ? 1 : -1];
typedef char battle_h_check_scene_timer[(u32)&((struct Battle *)0)->scene.timer == 0x15F ? 1 : -1];

#endif /* GUARD_BATTLE_H */
