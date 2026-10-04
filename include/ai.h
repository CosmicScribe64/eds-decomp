#ifndef GUARD_AI_H
#define GUARD_AI_H

/*
 * The CPU opponent.
 *
 * In a single-player duel the CPU's turn is run by AiRunTurn (duel step 8). It walks gAiTurnPhases with
 * gAiState.turnPhase: the same draw, standby and end handlers the human's turn uses, except that the main
 * phase is AiRunStep. AiRunStep walks gAiSteps with gAiState.step (enum AiStep): summon, play spells and
 * effects, change positions, attack, set spell/traps. When AiChooseStrategy finds a scripted combo at the
 * start of the main phase, step 8 (AiStepRunStrategy) plays it instead until it finishes or fails.
 *
 * Most decisions are simulations: AiBackupDuelState copies both players and the duel flags into
 * gAiWork.duelBackup, the AI changes the real state (puts a monster on the field, turns its monsters to
 * attack position, plays out the battles) and AiRestoreDuelState puts everything back. Only gAiWork's own
 * fields (bestAttack, the sim counters) survive the restore.
 *
 * The CPU is always player 1. It reads hidden information: the human's hand and the top of its deck, and the
 * real stats of face-down monsters (see AiGetStrongestMonsterScore, AiPickMonsterToSet).
 *
 * Code: src/ai_*.c, summon_builders.c (tribute and hand picks), duel_cmd_queue.c (AiRunTurn). Wiki:
 * functions/ai-*-c.md, functions/summon-builders-c.md.
 */

#include "global.h"

struct ChainEntry;
struct DuelPlayer;

/* --- CPU turn state ------------------------------------------------------------------------------------- */

/* gAiState.step: index into gAiSteps (0x0819DD6C), run by AiRunStep; advanced when a step returns 1. */
enum AiStep {
    AI_STEP_START_MAIN_PHASE = 0,   /* AiStepStartMainPhase: clear gAiWork, pick a strategy (-> 8) */
    AI_STEP_SIMPLE_SPELLS = 1,      /* AiStepPlaySimpleSpells */
    AI_STEP_MAIN_PHASE_1 = 2,       /* AiStepMainPhase */
    AI_STEP_CHANGE_POSITIONS = 3,   /* AiStepChangePositions */
    AI_STEP_BATTLE = 4,             /* AiStepBattle */
    AI_STEP_MAIN_PHASE_2 = 5,       /* AiStepMainPhase again */
    AI_STEP_SET_SPELL_TRAPS = 6,    /* AiStepSetSpellTraps */
    AI_STEP_END = 7,                /* NULL entry: ends the CPU main phase */
    AI_STEP_STRATEGY = 8,           /* AiStepRunStrategy (returns to step 1 when the strategy ends) */
    AI_STEP_STRATEGY_END = 9,       /* NULL */
};

/* gAiState.phase inside AiStepMainPhase. Phases 1-4 restart at AI_MAIN_SUMMON after an activation. */
enum AiMainState {
    AI_MAIN_SUMMON = 0,             /* Normal (or tribute) Summon the best hand monster */
    AI_MAIN_RESET = 1,              /* clear the sub-state bytes */
    AI_MAIN_MONSTER_EFFECTS = 2,    /* AiActivateMonsterEffects */
    AI_MAIN_EXODIA_TRAPS = 3,       /* AiActivateExodiaTraps */
    AI_MAIN_SPELLS = 4,             /* AiPlaySpells */
    AI_MAIN_SET_MONSTER = 10,       /* Set AiPickMonsterToSet's monster (turn 0, and after the spells) */
};

/* gAiState.phase inside AiStepPlaySimpleSpells (gAiSimpleSpells). */
enum AiSimpleSpellsPhase {
    AI_SIMPLE_SPELLS_FIELD = 0,     /* activate a set copy from spell/trap zones 5-9 */
    AI_SIMPLE_SPELLS_HAND = 1,      /* look for a playable copy in the hand */
    AI_SIMPLE_SPELLS_PLACE = 2,     /* place it from the hand and activate it */
};

/* gAiState.subState inside AiPlaySpells (and AiActivateExodiaTraps: 0, 0x64, 0xC8). */
enum AiSpellState {
    AI_SPELLS_START = 0,
    AI_SPELLS_DRAW_AND_CLEAR = 1,       /* Pot of Greed, Fusion Sage, Dark Hole / Raigeki, Fissure, ... */
    AI_SPELLS_TAKE_AND_REVIVE = 2,      /* Change of Heart, Monster Reborn, Premature Burial */
    AI_SPELLS_GRACEFUL_CHARITY = 3,
    AI_SPELLS_CARD_DESTRUCTION = 4,     /* key 1221 */
    AI_SPELLS_LISTS = 5,                /* gAiGenericSpells, then gAiEquipSpells */
    AI_SPELLS_EXODIA_CLEAR = 100,       /* 0x64, AI_FLAG_EXODIA: Dark Hole to clear its own Sangan / Witch */
    AI_SPELLS_EXODIA_DRAW = 101,        /* 0x65: Graceful Charity / Pot of Greed */
    AI_SPELLS_EXODIA_SWORDS = 102,      /* 0x66: Swords of Revealing Light */
    AI_SPELLS_CONFIRM_HAND_CARD = 200,  /* 0xC8: confirm hand card gAiState.cardIndex in gDuel.cardMenu */
    AI_SPELLS_WAIT_PLAY = 201,          /* 0xC9: wait for CardMenu_PlaySpellTrapFromHand */
};

/* CPU turn state, gAiState (0x02015EF0). Cleared at the start of the CPU's turn; AiRunStep clears
 * stepState .. subState whenever a step finishes. */
struct AiState {
    u8 turnPhase;   /* +0x0: index into gAiTurnPhases, advanced by AiRunTurn */
    u8 step;        /* +0x1: enum AiStep, index into gAiSteps (AiRunStep) */
    u8 stepState;   /* +0x2: state of the current step handler (AiStepSetSpellTraps, the AiStrategy* steps) */
    u8 stepIndex;   /* +0x3: hand index scanned by AiStepSetSpellTraps; rounds left in the Banish*Summon
                     *       strategies */
    u8 unk4;        /* +0x4: only cleared */
    u8 flipZone;    /* +0x5: monster zone scanned by the unreferenced AiStepFlipSummon */
    u8 subState;    /* +0x6: enum AiSpellState in AiPlaySpells / AiActivateExodiaTraps; loop index of
                     *       AiStepPlaySimpleSpells (table) and AiStepChangePositions (zone) */
    u8 listIndex;   /* +0x7: index into gAiEffectMonsters (AiActivateMonsterEffects) or gAiGenericSpells /
                     *       gAiEquipSpells (AiPlaySpells) */
    u8 zoneIndex;   /* +0x8: monster zone scanned by AiActivateMonsterEffects */
    u8 unk9;        /* +0x9: only cleared */
    u8 phase;       /* +0xA: phase of AiStepMainPhase (enum AiMainState), AiStepPlaySimpleSpells (enum
                     *       AiSimpleSpellsPhase), AiStepChangePositions and AiStepBattle */
    u8 cardIndex;   /* +0xB: hand index (AiTryPlaySpellTrap) or spell/trap zone (AiSelectUsableSpellTrap) of
                     *       the card being played; the strategies copy it into gDuel.cardMenu.index */
};

typedef char ai_h_check_state_size[sizeof(struct AiState) == 0xC ? 1 : -1];
typedef char ai_h_check_state_phase[(u32)&((struct AiState *)0)->phase == 0xA ? 1 : -1];

extern struct AiState gAiState;

/* Bits of gDuelCtrl.aiFlags (include/duel_flow.h). */
enum AiFlag {
    AI_FLAG_CAREFUL = 0x1,      /* nothing sets it (name: medium confidence): AiRiskSetCounter backs off 3/4
                                 * instead of 1/8 of the time, Graceful Charity only in useful cases */
    AI_FLAG_EXODIA = 0x200,     /* set by LoadOpponentDeck for duelist 11 (Rare Hunter, the Exodia deck) */
};

/* --- Attack planning and simulation --------------------------------------------------------------------- */

/* One attack the CPU considered (8 bytes, copied whole). The CPU attacks with player 1's attackerZone
 * against player 0's targetZone. */
struct AttackPlan {
    u16 unk0_0:1;               /* bit 0: never written by the AI */
    u16 isDirect:1;             /* bit 1: direct attack (the battle step then uses defender slot 5) */
    u16 isValid:1;              /* bit 2: a plan was chosen */
    u16 wins:1;                 /* bit 3: the target is destroyed and the attacker survives;
                                 *        AiFindAttackTarget keeps only such plans */
    u16 attackerZone:3;         /* bits 4-6: player 1's attacking monster zone */
    u16 targetZone:3;           /* bits 7-9: player 0's attacked monster zone */
    u16 attackerDestroyed:1;    /* bit 10: the attacker loses (both on an ATK tie) */
    u16 targetDestroyed:1;      /* bit 11: the target is destroyed (also on an ATK tie) */
    u16 unk0_12:4;
    u16 unk2;                   /* +0x2: zeroed by AiEvalAttack */
    s16 damage;                 /* +0x4: predicted battle damage to the human (negative: to the CPU; 0 when
                                 *       a defender is beaten); the attacker's ATK for a direct attack */
    u16 unk6;                   /* +0x6: unused */
};

typedef char ai_h_check_attack_plan_size[sizeof(struct AttackPlan) == 0x8 ? 1 : -1];
typedef char ai_h_check_attack_plan_damage[(u32)&((struct AttackPlan *)0)->damage == 0x4 ? 1 : -1];

/* Counters of one simulated battle phase, the halfword at gAiWork + 0x1B20 (struct AiWork declares the
 * same bits inline; ai_summon uses those members). */
struct AiSimResult {
    u16 simAttackersLost:3;     /* bits 0-2: CPU monsters destroyed in AiSimBattlePhase */
    u16 simTargetsDestroyed:3;  /* bits 3-5: human monsters destroyed in AiSimBattlePhase */
    u16 simOwnMonsters:3;       /* bits 6-8: CountMonsters(1) after the simulated summon and battle */
    u16 simOwnMonsters2:3;      /* bits 9-11 (byte 1 bits 1-3): the same count, written and never read */
    u16 unused:4;
};

/* CPU work area, gAiWork (0x02015F00, 0x1B28 bytes; AiStepStartMainPhase clears it every main phase).
 * Some units reach parts of it through integer addresses (0x02015F14 = duelBackup in ai_picks' DMA copies) or
 * a byte pointer plus an offset (+0x1B24 in ai_strategy); those access forms are matching choices and stay. */
struct AiWork {
    u8 unk0[0xC];                   /* +0x0000: not used by the AI */
    struct AttackPlan bestAttack;   /* +0x000C: plan chosen by AiChooseAttack (outside duelBackup, so it
                                     *          survives AiRestoreDuelState; the battle step reads it) */
    u8 duelBackup[0x1B0C];          /* +0x0014: AiBackupDuelState's copy of 0x020192E4..0x0201ADEF (both
                                     *          DuelPlayers and gDuel +0x1ACC..+0x1B0F) */
    u16 simAttackersLost:3;         /* +0x1B20 bits 0-2: struct AiSimResult, declared inline because a */
    u16 simTargetsDestroyed:3;      /*   struct member would be padded to 4 bytes */
    u16 simOwnMonsters:3;
    u16 simOwnMonsters2:3;
    u16 unk1B20_12:4;
    u16 listPick;                   /* +0x1B22: card-list index the CPU chose (AiPickCardListEntry); the
                                     *          effect resolves copy it into gCardListView.top */
    u8 strategyActive:1;            /* +0x1B24 bit 0: a strategy is running; the AiStrategy* handlers clear
                                     *          it when they finish or give up */
    u8 strategy:7;                  /* +0x1B24 bits 1-7: enum AiStrategy (some units write the whole byte,
                                     *          strategy << 1 | 1) */
    u8 strategyZone;                /* +0x1B25: free monster zone the strategy summons into (also the zone
                                     *          whose effect Cyber-Stein / Valkyrion activate) */
    u8 unk1B26[2];
};

typedef char ai_h_check_work_size[sizeof(struct AiWork) == 0x1B28 ? 1 : -1];
typedef char ai_h_check_work_best[(u32)&((struct AiWork *)0)->bestAttack == 0xC ? 1 : -1];
typedef char ai_h_check_work_backup[(u32)&((struct AiWork *)0)->duelBackup == 0x14 ? 1 : -1];
typedef char ai_h_check_work_pick[(u32)&((struct AiWork *)0)->listPick == 0x1B22 ? 1 : -1];
typedef char ai_h_check_work_zone[(u32)&((struct AiWork *)0)->strategyZone == 0x1B25 ? 1 : -1];

extern struct AiWork gAiWork;

/* gAiWork.strategy: scripted combos, tried in this order by AiChooseStrategy and run by AiStepRunStrategy.
 * Several need effect keys that are not EDS cards, so this ROM never picks them. */
enum AiStrategy {
    AI_STRATEGY_CYBER_STEIN = 0,                /* Cyber-Stein -> Blue-Eyes Ultimate Dragon, Megamorph */
    AI_STRATEGY_VALKYRION = 1,                  /* Valkyrion the Magna Warrior from the three Magnets */
    AI_STRATEGY_FOUR_TOKENS_CANNON_SOLDIER = 2, /* key 1245 tokens fed to Cannon Soldier (never in EDS) */
    AI_STRATEGY_ELEGANT_EGOTIST = 3,            /* Harpie Lady -> Elegant Egotist, Rising Air Current */
    AI_STRATEGY_DOUBLE_MACHINE_ATK = 4,         /* key 1314 doubles the Machines' ATK (never in EDS) */
    AI_STRATEGY_NONE = 5,                       /* never chosen */
    AI_STRATEGY_BANISH_THREE_SUMMON = 6,        /* key 1514 (never in EDS) */
    AI_STRATEGY_BANISH_TWO_SUMMON = 7,          /* key 1515 (never in EDS) */
    AI_STRATEGY_TOON_WORLD = 8,                 /* Toon World, then the Toon monsters */
};

/* --- Opponent decks ------------------------------------------------------------------------------------- */

/* One CPU deck: gOpponentDecks[duelist] (0x0819DC6C) and gOpponentAltDecks (0x0819DD34, never used). */
struct DeckList {
    const u16 *cards;   /* +0x0: card numbers (not ids) */
    u16 count;          /* +0x4: number of cards */
    u16 pad;            /* +0x6: always 0 */
};

typedef char ai_h_check_deck_list_size[sizeof(struct DeckList) == 0x8 ? 1 : -1];


/* --- Turn driver and steps ------------------------------------------------------------------------------ */

/* Run one frame of the CPU's turn: call gAiTurnPhases[gAiState.turnPhase] and advance when it returns
 * nonzero. 1 when the turn is over. */
u16 AiRunTurn(void);
/* CPU main phase: call gAiSteps[gAiState.step] and advance when it returns nonzero. 1 at a NULL entry
 * (main phase over). */
int AiRunStep(void);
/* Step 0: clear gAiWork, announce Main Phase 1, then jump to AI_STEP_STRATEGY if AiChooseStrategy picked a
 * combo. */
int AiStepStartMainPhase(void);
/* Step 1: play the gAiSimpleSpells cards (heal, burn, removal) whose condition holds, set copies first,
 * then from the hand. */
int AiStepPlaySimpleSpells(void);
/* Steps 2 and 5: summon, monster effects, Exodia traps, spells, then Set a monster (enum AiMainState). */
int AiStepMainPhase(void);
/* Step 3: activate monster effects, then flip-summon or switch to attack the monsters that would beat an
 * opponent monster. */
int AiStepChangePositions(void);
/* Step 4: run the battle phase (skipped when no monster can attack); the attacks come from AiPlanAttack. */
u16 AiStepBattle(void);
/* Step 6: set Traps and some Quick-Play Magic from the hand, partly chosen by reading the human's hand. */
int AiStepSetSpellTraps(void);
/* Flip-summon the CPU's set flip-effect monsters when their effect helps. Unreferenced (gAiSteps[7] is
 * NULL). */
int AiStepFlipSummon(void);

/* --- Scripted strategies -------------------------------------------------------------------------------- */

/* Pick the first strategy (enum AiStrategy) whose cards and conditions are met; sets gAiWork.strategy and
 * strategyActive. 1 if one applies. */
int AiChooseStrategy(void);
/* Step 8: run the handler of gAiWork.strategy; when it clears strategyActive, continue with the normal
 * steps at AI_STEP_SIMPLE_SPELLS. */
int AiStepRunStrategy(void);
/* Strategy 0: clear the opponent's field, summon Cyber-Stein and use its effect for Blue-Eyes Ultimate
 * Dragon, play Megamorph, attack. */
int AiStrategyCyberStein(void);
/* Strategy 1: clear the opponent's field, send the three Magnet Warriors to the graveyard for Valkyrion,
 * use its effect, then play Premature Burial or Monster Reborn. */
int AiStrategyValkyrion(void);
/* Strategy 2: play key 1245 (four tokens) and feed them to Cannon Soldier. Never chosen in EDS. */
int AiStrategyFourTokensCannonSoldier(void);
/* Strategy 3: Harpie Lady, Elegant Egotist, then Rising Air Current. */
int AiStrategyElegantEgotist(void);
/* Strategy 4: play key 1314 (double the Machines' ATK). Never chosen in EDS. */
int AiStrategyDoubleMachineAtk(void);
/* Strategy 5: give up at once (never chosen). */
int AiStrategyNone(void);
/* Strategy 6: banish 3 graveyard monsters to Special Summon key 1514. Never chosen in EDS. */
int AiStrategyBanishThreeSummon(void);
/* Strategy 7: as strategy 6 with 2 monsters and key 1515. Never chosen in EDS. */
int AiStrategyBanishTwoSummon(void);
/* Strategy 8: play Toon World, then summon the Toon monsters of the hand. */
int AiStrategyToonWorld(void);

/* --- Spells, traps and effects -------------------------------------------------------------------------- */

/* Main-phase spell machine on gAiState.subState (enum AiSpellState): draw, removal, revival, then the
 * generic and equip lists. 1 when done. */
int AiPlaySpells(void);
/* With AI_FLAG_EXODIA: activate key 1447 or Backup Soldier to fetch Exodia pieces. 1 when done. */
int AiActivateExodiaTraps(void);
/* Activate the effect of a face-up gAiEffectMonsters monster (Time Wizard, Cannon Soldier, Relinquished,
 * Barrel Dragon); 1 if one was activated. */
int AiActivateMonsterEffects(void);
/* 1 if the CPU has a set copy of card `number` it can activate now. */
int AiHasUsableSpellTrap(u16 number);
/* AiHasUsableSpellTrap, and on success select that zone (gAiState.cardIndex, subState 0xC8). */
int AiSelectUsableSpellTrap(u16 number);
/* Play card `number`: activate a set copy, or select the hand copy in gAiState.cardIndex (the caller
 * confirms it). 1 on success. */
int AiTryPlaySpellTrap(u16 number);
/* 1 = go ahead; 0 now and then when the human has an activatable set counterNo (more often with
 * AI_FLAG_CAREFUL). */
int AiRiskSetCounter(u16 counterNo);
/* 1 if the human's monsters threaten the CPU's LP or outclass its strongest monster. */
int AiIsOpponentThreatening(void);
/* Should the CPU activate the set card described by entry (a response window)? Decided per card number; 0
 * for NULL. duel_response's CPU search passes a second argument (0) through a local two-parameter view. */
int AiShouldActivateSetCard(struct ChainEntry *entry);
/* Activate the CPU's first set, activatable copy of card number `number` in answer to the same event as
 * entry; 1 if found. */
u16 AiChainSetCard(struct ChainEntry *entry, int number);
/* CPU answer to the chain link entry (White Hole against Dark Hole, Magic Jammer, ...); 1 if it chained a
 * set card. */
int AiTryChainResponse(struct ChainEntry *entry);

/* --- Summons and tributes ------------------------------------------------------------------------------- */

/* Hand index of the level 1-4 monster whose simulated summon and battle phase deals the most damage, or
 * -1. */
int AiChooseSummonNoTribute(void);
/* AiChooseSummonNoTribute for every level, with the tributes AiPickTributeMonster would take; *outDamage =
 * the best damage. Hand index or -1. */
int AiChooseSummonWithTribute(int *outDamage);
/* Hand index of the monster to Set (flip effects by situation, else the best wall), or -1. */
int AiPickMonsterToSet(void);
/* CPU answer to the position prompt: 1 = Defense Position, 0 = face-up Attack. faceUp != 0 skips the
 * flip-effect rule. */
int AiShouldSetMonster(u16 cardId, u16 faceUp);
/* 1 if the CPU has enough monsters to tribute for card id cardId. */
int AiHasTributesFor(u16 cardId);
/* CPU monster zone (not excludeZone) to tribute: Sangan, Witch of the Black Forest, else the weakest;
 * Fusion monsters only if allowFusion. -1 if none. */
int AiPickTributeMonster(int excludeZone, u16 allowFusion);
/* CPU monster zone to tribute for an effect cost: a token first. ROM bug: '=' for '==' makes it take the
 * first occupied zone. -1 if none. */
int AiPickEffectTribute(int excludeZone);
/* 1 if the CPU should keep card `number`: the Exodia pieces always, staple spells/traps from tier 1,
 * Kuriboh and Cyber-Stein from tier 2. */
int AiIsKeyCard(int tier, u16 number);

/* --- Battle simulation ---------------------------------------------------------------------------------- */

/* Copy both DuelPlayers and gDuel +0x1ACC..+0x1B0F into gAiWork.duelBackup (DMA). */
void AiBackupDuelState(void);
/* Copy gAiWork.duelBackup back over the duel state. */
void AiRestoreDuelState(void);
/* Simulation: turn every CPU monster without a flip effect face-up in Attack Position, free to attack. */
void AiSimSetAttackPositions(void);
/* Simulation: remove the tributes named by the nibbles of tributeMask (zone | 8) and put hand card
 * handIndex face up on the field. */
void AiSimSummon(int handIndex, u16 tributeMask);
/* Simulation: play out the CPU's attacks (AiChooseAttack) and count the losses; returns the total battle
 * damage. */
int AiSimBattlePhase(void);
/* Damage of a simulated battle phase in the backup sandbox (assumeAttackPos: monsters in Attack Position
 * first). */
int AiSimBattleDamage(u16 assumeAttackPos);
/* AiChooseAttack in the backup sandbox; 1 if a winning attack exists (kept in gAiWork.bestAttack). */
int AiPlanAttack(u16 assumeAttackPos);
/* Try the CPU's monsters that may attack, weakest first, and store the first winning plan in
 * gAiWork.bestAttack; 1 if found. */
int AiChooseAttack(void);
/* Best plan for attackerZone: a direct attack if possible, else the winning attack with the most damage.
 * Returns plan->wins. */
u16 AiFindAttackTarget(int attackerZone, struct AttackPlan *plan);
/* Predict attackerZone (CPU) against targetZone (human) into *plan. Duelists 1-10 misjudge a face-down
 * defender's DEF. */
void AiEvalAttack(int attackerZone, int targetZone, struct AttackPlan *plan);
/* 1 if an attacker with ATK atk beats the human's monster in zone. */
u16 AiCanBeatMonster(int atk, int zone);
/* 1 if the human has no monster or an attacker with ATK atk beats one of them. */
int AiCanBeatAnyMonster(int atk);

/* --- Card picks and board queries ----------------------------------------------------------------------- */

/* CPU pick from the card-list candidates of card id cardId's effect (CollectEffectTargets); also stored in
 * gAiWork.listPick. -1 if none. */
int AiPickCardListEntry(u16 cardId);
/* Hand index the CPU discards: a strong monster when it can revive it, else Sinister Serpent, else the
 * weakest card. */
int AiPickDiscard(void);
/* Hand index of player's card with the lowest printed ATK + DEF that is not a key card (Normal monsters
 * first), or -1. */
int AiPickWeakestHandCard(struct DuelPlayer *players, int player);
/* Hand index of player's monster with the highest printed ATK that is neither special-summon-only nor a
 * key card (level 5+ first), or -1. */
int AiPickStrongestHandMonster(struct DuelPlayer *players, int player);
/* Index of the human's hand card the CPU picks: the first one in gAiHandPickPriority order, else a random
 * one. */
int AiPickOpponentHandCard(void);

/* Card numbers of the Magic/Trap cards the CPU values most (AiPickCardListEntry): Raigeki, Dark Hole, Change
 * of Heart, Pot of Greed, Harpie's Feather Duster, Monster Reborn, Snatch Steal, Graceful Charity, Mirror
 * Force, Magic Jammer, Seven Tools of the Bandit, Swords of Revealing Light, Heavy Storm. */
#define AI_POWER_CARD_COUNT         13
extern const u16 gAiPowerCards[];               /* 0x0819D2FC: [AI_POWER_CARD_COUNT] */
/* Card numbers in the order the CPU takes cards from the human's hand (AiPickOpponentHandCard). */
#define AI_HAND_PICK_PRIORITY_COUNT 26
extern const u16 gAiHandPickPriority[];         /* 0x0819D316: [AI_HAND_PICK_PRIORITY_COUNT] */
/* Index of player's first hand card with card number (u16)number, or -1 (the AI's own copy of
 * FindHandCardByNumber). */
int AiFindHandCardByNumber(int player, int number);
/* Highest ATK (useAtk) + DEF (useDef) among player's monsters except skipZone, or -1. Face-down monsters
 * count with their real stats. */
int AiGetStrongestMonsterScore(int player, int skipZone, u16 useAtk, u16 useDef);
/* Zone of player's monster with the highest score (as AiGetStrongestMonsterScore), or -1. */
int AiFindStrongestMonster(int player, int skipZone, u16 useAtk, u16 useDef);
/* Zone of player's monster with the lowest score, or -1. */
int AiFindWeakestMonster(int player, int skipZone, u16 useAtk, u16 useDef);
/* Total effective ATK of player's monster zones 0-4. */
int SumMonsterAtk(int player);
/* 1 if the monster in (player, zone) may change its battle position (not locked, not held by Spellbinding
 * Circle or Dragon Capture Jar). */
int AiCanChangePosition(int player, int zone);
/* Number of Exodia pieces (card numbers 16-20) in the CPU's deck. */
int AiCountExodiaInDeck(void);
/* Number of Exodia pieces in the CPU's graveyard. */
int AiCountExodiaInGraveyard(void);
/* Number of Exodia pieces in the CPU's monster zones. */
int AiCountExodiaOnField(void);

/* --- Opponent decks ------------------------------------------------------------------------------------- */

/* Build the CPU's deck for a Campaign duel from gOpponentDecks[gMain.opponent] (gOpponentAltDecks if
 * useAltDeck, never used); Rare Hunter also gets AI_FLAG_EXODIA. */
void LoadOpponentDeck(u16 useAltDeck);

#endif /* GUARD_AI_H */
