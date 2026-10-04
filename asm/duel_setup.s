	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_setup/DuelCmdQueue_Run.s"
	.include "asm/nonmatching/duel_setup/DuelCmd_RunRemote.s"
	.include "asm/nonmatching/duel_setup/Duel_Setup.s"
	.include "asm/nonmatching/duel_setup/SetupStartFieldCard.s"
	.include "asm/nonmatching/duel_setup/DuelPhase_Opening.s"
	.include "asm/nonmatching/duel_setup/Chain_IsPartnerEntry.s"
	.include "asm/nonmatching/duel_setup/Chain_Add.s"
	.include "asm/nonmatching/duel_setup/Chain_AddPending.s"
	.include "asm/nonmatching/duel_setup/Chain_AddLink.s"
	.include "asm/nonmatching/duel_setup/Chain_AddPartnerEntry.s"
	.include "asm/nonmatching/duel_setup/Chain_CardGoesToGrave.s"
	.include "asm/nonmatching/duel_setup/Chain_GetResponseCommands.s"
	.include "asm/nonmatching/duel_setup/Chain_WaitPartnerReply.s"
	.include "asm/nonmatching/duel_setup/Chain_AskResponse.s"
	.include "asm/nonmatching/duel_setup/Chain_Build.s"
