	thumb_func_start sub_08076A20
sub_08076A20: @ 0x08076A20
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	add r7, r2, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #4]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #8]
	ldr r6, [r7, #4]
	ldrh r0, [r7, #0xA]
	ldrh r1, [r7, #8]
	cmp r0, r1
	bcc _08076A52
	add r0, r7, #0
	bl sub_080769DC
	b _08076BD6
_08076A52:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r1, #0x40
	orr r0, r1
	strh r0, [r2]
	ldrh r0, [r6]
	add r6, #2
	strh r0, [r7, #0xE]
	mov r2, #0
	mov r8, r2
	cmp r8, r0
	blt _08076A6E
	b _08076BC8
_08076A6E:
	ldr r3, _08076B3C @ =0x03000040
	mov sl, r3
	mov r4, #0x3F
	mov r9, r4
_08076A76:
	ldrh r0, [r6]
	add r6, #2
	mov ip, r0
	ldrh r1, [r6]
	add r6, #2
	ldrh r0, [r6]
	str r0, [sp, #0xC]
	add r6, #2
	mov r2, r8
	lsl r5, r2, #3
	mov r4, sl
	add r3, r5, r4
	ldr r0, _08076B40 @ =0x00004431
	add r2, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r4, [r2]
	and r0, r4
	strb r0, [r2]
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	ldr r2, [sp, #0]
	lsl r0, r2, #0x10
	asr r0, r0, #0x10
	add r1, r1, r0
	ldr r4, _08076B44 @ =0x00004432
	add r2, r3, r4
	ldr r4, _08076B48 @ =0x000001FF
	add r0, r4, #0
	and r1, r0
	ldr r0, _08076B4C @ =0xFFFFFE00
	ldrh r4, [r2]
	and r0, r4
	orr r0, r1
	strh r0, [r2]
	ldr r0, [sp, #0xC]
	ldr r1, [sp, #4]
	add r4, r0, r1
	ldr r2, _08076B50 @ =0x00004430
	add r3, r3, r2
	strb r4, [r3]
	mov r1, ip
	ldr r3, [r7]
	add r3, #0x20
	ldrh r0, [r3]
	add r3, #2
	lsl r0, r0, #2
	add r3, r3, r0
	mov r4, #1
	add r8, r4
_08076ADA:
	ldrh r0, [r3]
	add r3, #2
	add r4, r0, #0
	lsl r0, r4, #5
	add r3, r3, r0
	sub r1, #1
	mov r0, #1
	neg r0, r0
	cmp r1, r0
	bne _08076ADA
	add r5, sl
	mov r1, ip
	mul r1, r4
	add r1, #1
	ldr r3, _08076B54 @ =0x00004434
	add r2, r5, r3
	ldr r4, _08076B58 @ =0x000003FF
	add r0, r4, #0
	and r1, r0
	ldr r0, _08076B5C @ =0xFFFFFC00
	ldrh r3, [r2]
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	ldr r4, _08076B60 @ =0x00004435
	add r2, r5, r4
	mov r0, #0xF0
	ldrb r1, [r2]
	orr r0, r1
	mov r1, #0xD
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r0, [r7]
	add r0, #0x22
	mov r2, ip
	lsl r1, r2, #2
	add r0, r0, r1
	ldrh r0, [r0]
	add r1, r0, #0
	mov r0, #0x80
	lsl r0, r0, #7
	cmp r1, r0
	beq _08076B88
	cmp r1, r0
	bgt _08076B64
	cmp r1, #0
	beq _08076B76
	b _08076BC0
_08076B3C: .4byte 0x03000040
_08076B40: .4byte 0x00004431
_08076B44: .4byte 0x00004432
_08076B48: .4byte 0x000001FF
_08076B4C: .4byte 0xFFFFFE00
_08076B50: .4byte 0x00004430
_08076B54: .4byte 0x00004434
_08076B58: .4byte 0x000003FF
_08076B5C: .4byte 0xFFFFFC00
_08076B60: .4byte 0x00004435
_08076B64:
	mov r0, #0x80
	lsl r0, r0, #8
	cmp r1, r0
	beq _08076B9C
	mov r0, #0xC0
	lsl r0, r0, #8
	cmp r1, r0
	beq _08076BB4
	b _08076BC0
_08076B76:
	ldr r3, _08076B84 @ =0x00004433
	add r1, r5, r3
	mov r0, r9
	ldrb r4, [r1]
	and r0, r4
	b _08076BBE
	.align 2, 0
_08076B84: .4byte 0x00004433
_08076B88:
	ldr r1, _08076B98 @ =0x00004433
	add r0, r5, r1
	mov r1, r9
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x40
	b _08076BA8
	.align 2, 0
_08076B98: .4byte 0x00004433
_08076B9C:
	ldr r3, _08076BB0 @ =0x00004433
	add r0, r5, r3
	mov r1, r9
	ldrb r4, [r0]
	and r1, r4
	mov r2, #0x80
_08076BA8:
	orr r1, r2
	strb r1, [r0]
	b _08076BC0
	.align 2, 0
_08076BB0: .4byte 0x00004433
_08076BB4:
	ldr r0, _08076BE8 @ =0x00004433
	add r1, r5, r0
	mov r0, #0xC0
	ldrb r2, [r1]
	orr r0, r2
_08076BBE:
	strb r0, [r1]
_08076BC0:
	ldrh r3, [r7, #0xE]
	cmp r8, r3
	bge _08076BC8
	b _08076A76
_08076BC8:
	ldr r4, [sp, #8]
	cmp r4, #0
	beq _08076BD6
	str r6, [r7, #4]
	ldrh r0, [r7, #0xA]
	add r0, #1
	strh r0, [r7, #0xA]
_08076BD6:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08076BE8: .4byte 0x00004433
	thumb_func_end sub_08076A20

