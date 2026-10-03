	.include "asm/macros.inc"

	.syntax divided
	.text

	.include "asm/nonmatching/effect_fusion/EffectSplitFusionResolve.s"
	.include "asm/nonmatching/effect_fusion/EffectReturnBanishedToGraveResolve.s"
	.include "asm/nonmatching/effect_fusion/EffectBanishCostFromFieldResolve.s"
	.include "asm/nonmatching/effect_fusion/IsFusionSubstitute.s"
	.include "asm/nonmatching/effect_fusion/CheckFusionRecipe2.s"
	.include "asm/nonmatching/effect_fusion/CheckFusionRecipe3.s"
	.include "asm/nonmatching/effect_fusion/IsMaterialOfFusion.s"
	.include "asm/nonmatching/effect_fusion/FindFusionMaterialSlot.s"
	.include "asm/nonmatching/effect_fusion/GetFusionSlotCardId.s"
	.include "asm/nonmatching/effect_fusion/CheckFusionRecipe.s"
	.include "asm/nonmatching/effect_fusion/FindFusionMaterials.s"
	.include "asm/nonmatching/effect_fusion/IsPendingFusionMaterial.s"
	.include "asm/nonmatching/effect_fusion/RemovePendingFusionMaterial.s"
	.include "asm/nonmatching/effect_fusion/EffectPolymerizationResolve.s"
