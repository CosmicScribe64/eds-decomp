	thumb_func_start DuelScene_SkullDice
DuelScene_SkullDice: @ 0x080260EC
	push {r4, lr}
	ldr r1, _08026114 @ =0x08199A40
	ldr r4, _08026118 @ =0x02017A30
	ldrb r2, [r4, #0xB]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0802611C
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802610E
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
_0802610E:
	mov r0, #0
	b _0802611E
	.align 2, 0
_08026114: .4byte gSkullDiceSceneSteps
_08026118: .4byte 0x02017A30
_0802611C:
	mov r0, #1
_0802611E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end DuelScene_SkullDice

