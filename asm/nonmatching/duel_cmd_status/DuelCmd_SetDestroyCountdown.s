	thumb_func_start DuelCmd_SetDestroyCountdown
DuelCmd_SetDestroyCountdown: @ 0x08012670
	push {r4, lr}
	ldr r2, _080126C0 @ =0x020185C0
	ldrh r0, [r2]
	lsr r3, r0, #0xF
	mov r0, #0x94
	ldrh r4, [r2, #2]
	add r1, r4, #0
	mul r1, r0
	ldr r0, _080126C4 @ =0x00000D64
	mul r0, r3
	add r1, r1, r0
	ldr r0, _080126C8 @ =0x0201930C
	add r4, r1, r0
	ldrh r3, [r4, #6]
	lsl r0, r3, #0x16
	lsr r0, r0, #0x1C
	cmp r0, #0
	beq _0801269A
	ldrh r1, [r2, #4]
	cmp r0, r1
	ble _080126AA
_0801269A:
	mov r1, #0xF
	ldrh r0, [r2, #4]
	and r1, r0
	lsl r1, r1, #6
	ldr r0, _080126CC @ =0xFFFFFC3F
	and r0, r3
	orr r0, r1
	strh r0, [r4, #6]
_080126AA:
	ldr r4, _080126D0 @ =0x0000080D
	add r1, r2, r4
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_080126C0: .4byte 0x020185C0
_080126C4: .4byte 0x00000D64
_080126C8: .4byte 0x0201930C
_080126CC: .4byte 0xFFFFFC3F
_080126D0: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetDestroyCountdown

