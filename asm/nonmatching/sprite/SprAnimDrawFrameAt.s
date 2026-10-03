	thumb_func_start SprAnimDrawFrameAt
SprAnimDrawFrameAt: @ 0x08076BEC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r5, r2, #0
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #4]
	ldr r0, [r5, #4]
	mov ip, r0
	ldrh r1, [r5, #0xA]
	ldrh r2, [r5, #8]
	cmp r1, r2
	bcc _08076C1E
	add r0, r5, #0
	bl SprAnimRewind
	b _08076D98
_08076C1E:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r1, #0x40
	orr r0, r1
	strh r0, [r2]
	mov r1, ip
	ldrh r0, [r1]
	mov r2, #2
	add ip, r2
	strh r0, [r5, #0xE]
	mov r7, #0
	cmp r7, r0
	blt _08076C3C
	b _08076D88
_08076C3C:
	ldr r0, _08076D00 @ =0x03000040
	mov r8, r0
	lsl r0, r4, #0x17
	lsr r0, r0, #0x17
	mov sl, r0
	mov r1, #1
	neg r1, r1
	mov r9, r1
_08076C4C:
	mov r2, ip
	ldrh r0, [r2]
	add r6, r0, #0
	mov r4, #6
	add ip, r4
	lsl r3, r7, #3
	mov r0, r8
	add r2, r3, r0
	ldr r4, _08076D04 @ =0x00004431
	add r1, r2, r4
	mov r0, #0x21
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	ldr r0, _08076D08 @ =0x00004432
	add r1, r2, r0
	ldr r0, _08076D0C @ =0xFFFFFE00
	ldrh r4, [r1]
	and r0, r4
	mov r4, sl
	orr r0, r4
	strh r0, [r1]
	ldr r0, _08076D10 @ =0x00004430
	add r2, r2, r0
	mov r1, sp
	ldrb r1, [r1]
	strb r1, [r2]
	add r4, r6, #0
	ldr r1, [r5]
	add r1, #0x20
	ldrh r0, [r1]
	add r1, #2
	lsl r0, r0, #2
	add r1, r1, r0
	add r7, #1
_08076C94:
	ldrh r0, [r1]
	add r1, #2
	add r2, r0, #0
	lsl r0, r2, #5
	add r1, r1, r0
	sub r4, #1
	cmp r4, r9
	bne _08076C94
	add r3, r8
	add r1, r6, #0
	mul r1, r2
	add r1, #1
	ldr r4, _08076D14 @ =0x00004434
	add r2, r3, r4
	ldr r4, _08076D18 @ =0x000003FF
	add r0, r4, #0
	and r1, r0
	ldr r0, _08076D1C @ =0xFFFFFC00
	ldrh r4, [r2]
	and r0, r4
	orr r0, r1
	strh r0, [r2]
	ldr r0, _08076D20 @ =0x00004435
	add r2, r3, r0
	mov r0, #0xF0
	ldrb r1, [r2]
	orr r0, r1
	mov r1, #0xD
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r2, _08076D04 @ =0x00004431
	add r1, r3, r2
	mov r2, #0x3F
	add r0, r2, #0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	ldr r0, [r5]
	add r0, #0x22
	lsl r1, r6, #2
	add r0, r0, r1
	ldrh r0, [r0]
	add r1, r0, #0
	mov r0, #0x80
	lsl r0, r0, #7
	cmp r1, r0
	beq _08076D48
	cmp r1, r0
	bgt _08076D24
	cmp r1, #0
	beq _08076D36
	b _08076D80
	.align 2, 0
_08076D00: .4byte 0x03000040
_08076D04: .4byte 0x00004431
_08076D08: .4byte 0x00004432
_08076D0C: .4byte 0xFFFFFE00
_08076D10: .4byte 0x00004430
_08076D14: .4byte 0x00004434
_08076D18: .4byte 0x000003FF
_08076D1C: .4byte 0xFFFFFC00
_08076D20: .4byte 0x00004435
_08076D24:
	mov r0, #0x80
	lsl r0, r0, #8
	cmp r1, r0
	beq _08076D5C
	mov r0, #0xC0
	lsl r0, r0, #8
	cmp r1, r0
	beq _08076D74
	b _08076D80
_08076D36:
	ldr r0, _08076D44 @ =0x00004433
	add r1, r3, r0
	add r0, r2, #0
	ldrb r2, [r1]
	and r0, r2
	b _08076D7E
	.align 2, 0
_08076D44: .4byte 0x00004433
_08076D48:
	ldr r4, _08076D58 @ =0x00004433
	add r0, r3, r4
	add r1, r2, #0
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x40
	b _08076D68
	.align 2, 0
_08076D58: .4byte 0x00004433
_08076D5C:
	ldr r4, _08076D70 @ =0x00004433
	add r0, r3, r4
	add r1, r2, #0
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x80
_08076D68:
	orr r1, r2
	strb r1, [r0]
	b _08076D80
	.align 2, 0
_08076D70: .4byte 0x00004433
_08076D74:
	ldr r4, _08076DA8 @ =0x00004433
	add r1, r3, r4
	mov r0, #0xC0
	ldrb r2, [r1]
	orr r0, r2
_08076D7E:
	strb r0, [r1]
_08076D80:
	ldrh r4, [r5, #0xE]
	cmp r7, r4
	bge _08076D88
	b _08076C4C
_08076D88:
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _08076D98
	mov r1, ip
	str r1, [r5, #4]
	ldrh r0, [r5, #0xA]
	add r0, #1
	strh r0, [r5, #0xA]
_08076D98:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08076DA8: .4byte 0x00004433
	thumb_func_end SprAnimDrawFrameAt

