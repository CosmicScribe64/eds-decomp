	thumb_func_start AiSimBattleDamage
AiSimBattleDamage: @ 0x08058074
	push {r4, lr}
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	bl AiBackupDuelState
	cmp r4, #0
	beq _08058088
	bl AiSimSetAttackPositions
_08058088:
	bl AiSimBattlePhase
	add r4, r0, #0
	bl AiRestoreDuelState
	add r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end AiSimBattleDamage
	.align 2, 0

