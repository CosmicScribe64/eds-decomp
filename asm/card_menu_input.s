	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/card_menu_input/CardMenu_GetAvailableCommands.s"
	.include "asm/nonmatching/card_menu_input/DuelScreen_HandleInput.s"
	.include "asm/nonmatching/card_menu_input/MarkMonsterAttacked.s"
	.include "asm/nonmatching/card_menu_input/CanAttackDirectly.s"
	.include "asm/nonmatching/card_menu_input/IsTypeForbiddenToAttack.s"
	.include "asm/nonmatching/card_menu_input/CanMonsterAttack.s"
	.include "asm/nonmatching/card_menu_input/BuildAttackableMask.s"
	.include "asm/nonmatching/card_menu_input/CanEnterBattlePhase.s"
	.include "asm/nonmatching/card_menu_input/IsBattleEffectBlocked.s"
	.include "asm/nonmatching/card_menu_input/Battle_CheckReplay.s"
	.include "asm/nonmatching/card_menu_input/BattleStage_Start.s"
	.include "asm/nonmatching/card_menu_input/BattleStage_SelectAttacker.s"
	.include "asm/nonmatching/card_menu_input/BattleStage_SelectTarget.s"
