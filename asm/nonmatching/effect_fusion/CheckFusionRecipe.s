	thumb_func_start CheckFusionRecipe
CheckFusionRecipe: @ 0x0803D0B8
	push {lr}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	cmp r3, #0
	beq _0803D0D4
	bl CheckFusionRecipe3
	b _0803D0D8
_0803D0D4:
	bl CheckFusionRecipe2
_0803D0D8:
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803D0E2
	mov r0, #1
	b _0803D0E4
_0803D0E2:
	mov r0, #0
_0803D0E4:
	pop {r1}
	bx r1
	thumb_func_end CheckFusionRecipe

