	thumb_func_start DuelCmd_TossCoin
DuelCmd_TossCoin: @ 0x080170E4
	push {r4, r5, lr}
	ldr r4, _080170FC @ =0x020185C0
	ldr r0, _08017100 @ =0x0000080A
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _08017104
	cmp r0, #1
	beq _0801713C
	b _08017152
_080170FC: .4byte 0x020185C0
_08017100: .4byte 0x0000080A
_08017104:
	mov r0, #0
	mov r1, #0
	bl DuelScene_Start
	ldr r2, _08017138 @ =0x02017A30
	ldrh r3, [r4, #2]
	lsl r0, r3, #0xF
	mov r3, #0xC0
	lsl r3, r3, #1
	add r1, r3, #0
	orr r0, r1
	ldrb r4, [r4, #4]
	orr r0, r4
	strh r0, [r2, #6]
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
	b _08017152
_08017138: .4byte 0x02017A30
_0801713C:
	bl DuelScene_Run
	cmp r0, #0
	beq _08017152
	ldr r0, _08017158 @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08017152:
	pop {r4, r5}
	pop {r0}
	bx r0
_08017158: .4byte 0x0000080D
	thumb_func_end DuelCmd_TossCoin

