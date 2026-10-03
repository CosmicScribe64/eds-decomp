	thumb_func_start BattleStage_Cleanup
BattleStage_Cleanup: @ 0x0804E240
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	mov r8, r0
	mov r7, #0
	ldr r0, _0804E298 @ =0x0201930C
	mov ip, r0
	mov r5, #1
	mov r4, #0x80
	lsl r4, r4, #0x11
_0804E258:
	mov r6, #0
	add r2, r7, #0
	and r2, r5
	lsr r0, r4, #0x18
	mov r9, r0
	lsl r0, r7, #0x18
	lsr r3, r0, #0x18
_0804E266:
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _0804E29C @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r1, ip
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804E2F8
	add r0, r1, #0
	add r0, #0x8C
	ldrb r1, [r0]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _0804E2A0
	add r0, r7, #0
	add r1, r6, #0
	mov r2, #1
	bl DestroyFieldCard
_0804E294:
	mov r0, #0
	b _0804E30C
_0804E298: .4byte 0x0201930C
_0804E29C: .4byte 0x00000D64
_0804E2A0:
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _0804E2F8
	sub r0, r5, r7
	str r3, [sp, #0]
	bl FindFreeMonsterZone
	add r5, r0, #0
	mov r1, #0xA2
	ldr r3, [sp, #0]
	mov r0, r8
	cmp r0, #0
	beq _0804E2BE
	ldr r1, _0804E2E8 @ =0x000080A2
_0804E2BE:
	lsl r0, r6, #0x18
	lsr r4, r0, #0x10
	orr r4, r3
	add r0, r1, #0
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	cmp r5, #0
	blt _0804E2EC
	lsl r2, r5, #0x18
	lsr r2, r2, #0x10
	mov r0, r9
	orr r2, r0
	mov r0, r8
	add r1, r4, #0
	bl MoveFieldCard
	b _0804E294
	.align 2, 0
_0804E2E8: .4byte 0x000080A2
_0804E2EC:
	add r0, r7, #0
	add r1, r6, #0
	mov r2, #1
	bl DestroyFieldCard
	b _0804E294
_0804E2F8:
	add r6, #1
	cmp r6, #4
	ble _0804E266
	mov r0, #0xFF
	lsl r0, r0, #0x18
	add r4, r4, r0
	add r7, #1
	cmp r7, #1
	ble _0804E258
	mov r0, #1
_0804E30C:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end BattleStage_Cleanup
	.align 2, 0

