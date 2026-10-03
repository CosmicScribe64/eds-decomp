	thumb_func_start DuelCmd_SetReturnAfterBattle
DuelCmd_SetReturnAfterBattle: @ 0x080128F8
	push {r4, lr}
	ldr r3, _08012940 @ =0x020185C0
	ldrh r0, [r3, #2]
	lsr r2, r0, #8
	mov r1, #1
	ldrb r4, [r3, #2]
	and r1, r4
	mov r0, #0x94
	mul r2, r0
	ldr r0, _08012944 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08012948 @ =0x0201930C
	add r2, r2, r0
	add r2, #0x8C
	mov r1, #1
	ldrh r0, [r3, #4]
	and r1, r0
	lsl r1, r1, #2
	mov r0, #5
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	orr r0, r1
	strb r0, [r2]
	ldr r0, _0801294C @ =0x0000080D
	add r3, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	strb r0, [r3]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_08012940: .4byte 0x020185C0
_08012944: .4byte 0x00000D64
_08012948: .4byte 0x0201930C
_0801294C: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetReturnAfterBattle

