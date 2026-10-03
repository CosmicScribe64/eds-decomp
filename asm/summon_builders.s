	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/summon_builders/QueueFlipSummon.s"
	.include "asm/nonmatching/summon_builders/QueueSpecialSummon.s"
	.include "asm/nonmatching/summon_builders/QueueSpecialSummonChoosePosition.s"
	.include "asm/nonmatching/summon_builders/QueueSpecialSummonFromHand.s"
	.include "asm/nonmatching/summon_builders/AiIsKeyCard.s"
	.include "asm/nonmatching/summon_builders/AiPickTributeMonster.s"
	.include "asm/nonmatching/summon_builders/AiHasTributesFor.s"
	.include "asm/nonmatching/summon_builders/AiPickEffectTribute.s"
	.include "asm/nonmatching/summon_builders/AiPickWeakestHandCard.s"
	.include "asm/nonmatching/summon_builders/AiPickStrongestHandMonster.s"
	.include "asm/nonmatching/summon_builders/AiPickDiscard.s"
	.include "asm/nonmatching/summon_builders/FindDeckCardByNumber.s"
	.include "asm/nonmatching/summon_builders/AiFindHandCardByNumber.s"
	.include "asm/nonmatching/summon_builders/FindFusionDeckCardByNumber.s"
