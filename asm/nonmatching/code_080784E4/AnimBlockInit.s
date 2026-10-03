	thumb_func_start AnimBlockInit
AnimBlockInit: @ 0x08078670
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r5, r1, #0
	mov r3, #0
	mov r7, #0
	ldr r0, _080786CC @ =0x0000FFFF
	add r6, r0, #0
_0807867E:
	lsl r1, r3, #2
	add r1, r1, r3
	lsl r1, r1, #2
	add r1, r5, r1
	ldmia r4!, {r2}
	str r2, [r1]
	ldr r0, [r2, #4]
	str r0, [r1, #4]
	ldrb r0, [r2, #1]
	strb r0, [r1, #0xC]
	ldrh r0, [r1, #8]
	orr r0, r6
	strh r0, [r1, #8]
	ldrh r0, [r1, #0xA]
	orr r0, r6
	strh r0, [r1, #0xA]
	strb r7, [r1, #0xD]
	mov r0, #1
	strb r0, [r1, #0xE]
	ldrb r0, [r2]
	strb r0, [r1, #0xF]
	mov r0, #0x13
	sub r0, r0, r3
	strb r0, [r1, #0x10]
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	ldr r0, [r4]
	cmp r0, #0
	bne _0807867E
	mov r1, #0xC8
	lsl r1, r1, #1
	add r0, r5, r1
	strh r3, [r0]
	add r0, r3, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080786CC: .4byte 0x0000FFFF
	thumb_func_end AnimBlockInit

