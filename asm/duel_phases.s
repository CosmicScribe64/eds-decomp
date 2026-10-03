	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_phases/UpdateMonstersAtTurnStart.s"
	.include "asm/nonmatching/duel_phases/DuelPhase_TurnStart.s"
	.include "asm/nonmatching/duel_phases/PhaseMenu_DrawCursor.s"
	.include "asm/nonmatching/duel_phases/PhaseMenu_HandleInput.s"
	.include "asm/nonmatching/duel_phases/DuelPhase_Main.s"
	.include "asm/nonmatching/duel_phases/GetMaintenanceLpCost.s"
	.include "asm/nonmatching/duel_phases/sub_0804F6A8.s"
	.include "asm/nonmatching/duel_phases/HasActivatableStandbyCard.s"
	.include "asm/nonmatching/duel_phases/ApplyStandbyPhaseEffects.s"
	.include "asm/nonmatching/duel_phases/DuelPhase_Standby.s"
