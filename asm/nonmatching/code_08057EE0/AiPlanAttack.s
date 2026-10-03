	thumb_func_start AiPlanAttack
AiPlanAttack: @ 0x0805809C
	push {r4, lr}
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	bl AiBackupDuelState
	cmp r4, #0
	beq _080580B0
	bl AiSimSetAttackPositions
_080580B0:
	bl AiChooseAttack
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	bl AiRestoreDuelState
	add r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end AiPlanAttack
	.align 2, 0

