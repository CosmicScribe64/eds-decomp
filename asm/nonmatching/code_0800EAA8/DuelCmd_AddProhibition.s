	thumb_func_start DuelCmd_AddProhibition
DuelCmd_AddProhibition: @ 0x0800EDCC
	push {r4, r5, r6, lr}
	ldr r5, _0800EE34 @ =0x020185C0
	ldrh r0, [r5, #4]
	cmp r0, #0
	beq _0800EE1E
	ldr r3, _0800EE38 @ =0x020192E0
	ldr r0, _0800EE3C @ =0x00001ACC
	add r4, r3, r0
	ldr r0, [r4]
	lsl r0, r0, #0xD
	lsr r0, r0, #0x1C
	lsl r0, r0, #1
	ldr r2, _0800EE40 @ =0x00001AD0
	add r1, r3, r2
	add r0, r0, r1
	ldrb r6, [r5, #2]
	lsl r1, r6, #8
	ldrh r6, [r5]
	lsr r2, r6, #0xF
	orr r1, r2
	strh r1, [r0]
	ldr r0, [r4]
	lsl r0, r0, #0xD
	lsr r0, r0, #0x1C
	lsl r0, r0, #1
	ldr r1, _0800EE44 @ =0x00001AF0
	add r3, r3, r1
	add r0, r0, r3
	ldrh r1, [r5, #4]
	strh r1, [r0]
	ldr r2, [r4]
	lsl r1, r2, #0xD
	lsr r1, r1, #0x1C
	add r1, #1
	mov r0, #0xF
	and r1, r0
	lsl r1, r1, #0xF
	ldr r0, _0800EE48 @ =0xFFF87FFF
	and r0, r2
	orr r0, r1
	str r0, [r4]
_0800EE1E:
	ldr r2, _0800EE4C @ =0x0000080D
	add r1, r5, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r6, [r1]
	and r0, r6
	strb r0, [r1]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0800EE34: .4byte 0x020185C0
_0800EE38: .4byte 0x020192E0
_0800EE3C: .4byte 0x00001ACC
_0800EE40: .4byte 0x00001AD0
_0800EE44: .4byte 0x00001AF0
_0800EE48: .4byte 0xFFF87FFF
_0800EE4C: .4byte 0x0000080D
	thumb_func_end DuelCmd_AddProhibition

