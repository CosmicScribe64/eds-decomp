	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/duel_response/CanActivateHandCard.s"
	.include "asm/nonmatching/duel_response/EventResponse_GetCommands.s"
	.include "asm/nonmatching/duel_response/EventResponse_CanPlayerRespond.s"
	.include "asm/nonmatching/duel_response/EventResponse_BuildPromptText.s"
	.include "asm/nonmatching/duel_response/EventResponse_Run.s"
	.include "asm/nonmatching/duel_response/EventResponse_Request.s"
	.include "asm/nonmatching/duel_response/EventResponse_Update.s"
	.include "asm/nonmatching/duel_response/IsZoneInChainList.s"
	.include "asm/nonmatching/duel_response/DuelLink_AnswerActivateQuery.s"
