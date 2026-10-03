	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_cmd_queue/CardMenu_Execute.s"
	.include "asm/nonmatching/duel_cmd_queue/AiRunTurn.s"
	.include "asm/nonmatching/duel_cmd_queue/DuelScene_Start.s"
	.include "asm/nonmatching/duel_cmd_queue/DuelScene_FadeOutDuelScreen.s"
	.include "asm/nonmatching/duel_cmd_queue/DuelScene_RunHandler.s"
	.include "asm/nonmatching/duel_cmd_queue/DuelScene_FadeInDuelScreen.s"
	.include "asm/nonmatching/duel_cmd_queue/DuelScene_Run.s"
	.include "asm/nonmatching/duel_cmd_queue/HBlank_WaveBg013.s"
	.include "asm/nonmatching/duel_cmd_queue/HBlank_WaveAllBgs.s"
	.include "asm/nonmatching/duel_cmd_queue/PlayDuelBGM.s"
	.include "asm/nonmatching/duel_cmd_queue/DuelCmd_Push.s"
	.include "asm/nonmatching/duel_cmd_queue/DuelCmd_Dispatch.s"
