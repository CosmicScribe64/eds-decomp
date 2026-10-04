	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/battle_phase3/BattleStage_EndAttack.s"
	.include "asm/nonmatching/battle_phase3/BattleStage_EndBattlePhase.s"
	.include "asm/nonmatching/battle_phase3/BattleStage_Cleanup.s"
	.include "asm/nonmatching/battle_phase3/BattlePhase_Run.s"
	.include "asm/nonmatching/battle_phase3/BattlePhase_UnusedNop.s"
	.include "asm/nonmatching/battle_phase3/DuelScreen_DrawPulseIconOverlay.s"
	.include "asm/nonmatching/battle_phase3/DuelPhase_Draw.s"
	.include "asm/nonmatching/battle_phase3/EndPhase_ReturnWickedWormBeast.s"
	.include "asm/nonmatching/battle_phase3/EndPhase_TransferMushroomMan2.s"
	.include "asm/nonmatching/battle_phase3/EndPhase_DestroyLowLevelMonsters.s"
	.include "asm/nonmatching/battle_phase3/DuelPhase_End.s"
