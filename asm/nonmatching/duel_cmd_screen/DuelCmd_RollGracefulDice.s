	thumb_func_start DuelCmd_RollGracefulDice
DuelCmd_RollGracefulDice: @ 0x080171D0
	push {r4, r5, lr}
	ldr r4, _080171E8 @ =0x020185C0
	ldr r0, _080171EC @ =0x0000080A
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _080171F0
	cmp r0, #1
	beq _0801721C
	b _08017232
_080171E8: .4byte 0x020185C0
_080171EC: .4byte 0x0000080A
_080171F0:
	mov r0, #2
	mov r1, #0
	bl DuelScene_Start
	ldr r1, _08017218 @ =0x02017A30
	ldrh r0, [r4, #2]
	strh r0, [r1, #6]
	ldrb r2, [r5]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5]
	b _08017232
	.align 2, 0
_08017218: .4byte 0x02017A30
_0801721C:
	bl DuelScene_Run
	cmp r0, #0
	beq _08017232
	ldr r2, _08017238 @ =0x0000080D
	add r1, r4, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08017232:
	pop {r4, r5}
	pop {r0}
	bx r0
_08017238: .4byte 0x0000080D
	thumb_func_end DuelCmd_RollGracefulDice

