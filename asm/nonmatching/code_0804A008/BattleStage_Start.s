	thumb_func_start BattleStage_Start
BattleStage_Start: @ 0x0804AB90
	push {r4, r5, lr}
	add r1, r0, #0
	ldr r4, _0804ABD0 @ =0x020192E0
	ldr r0, _0804ABD4 @ =0x00001B16
	add r5, r4, r0
	ldrh r2, [r5]
	lsl r0, r2, #0x17
	lsr r0, r0, #0x18
	cmp r0, #0
	bne _0804ABE0
	mov r0, #0x32
	cmp r1, #0
	beq _0804ABAC
	ldr r0, _0804ABD8 @ =0x00008032
_0804ABAC:
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrh r2, [r5]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804ABDC @ =0xFFFFFE01
	and r0, r2
	orr r0, r1
	strh r0, [r5]
	mov r0, #0
	b _0804AC0A
_0804ABD0: .4byte 0x020192E0
_0804ABD4: .4byte 0x00001B16
_0804ABD8: .4byte 0x00008032
_0804ABDC: .4byte 0xFFFFFE01
_0804ABE0:
	add r0, r4, #4
	mov r2, #0
	mov r3, #1
	bl BuildAttackableMask
	ldr r0, _0804AC10 @ =0x00001B12
	add r1, r4, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	mov r1, #0x53
	cmp r0, #0
	beq _0804ABFC
	ldr r1, _0804AC14 @ =0x00008053
_0804ABFC:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #1
_0804AC0A:
	pop {r4, r5}
	pop {r1}
	bx r1
_0804AC10: .4byte 0x00001B12
_0804AC14: .4byte 0x00008053
	thumb_func_end BattleStage_Start

