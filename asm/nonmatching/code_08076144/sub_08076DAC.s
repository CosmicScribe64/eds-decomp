	thumb_func_start sub_08076DAC
sub_08076DAC: @ 0x08076DAC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	add r5, r1, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #4]
	lsl r1, r0, #0x10
	lsr r3, r1, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #8]
	ldr r0, [r5, #4]
	mov ip, r0
	ldrh r1, [r5, #0xA]
	ldrh r2, [r5, #8]
	cmp r1, r2
	bcc _08076DE2
	add r0, r5, #0
	bl sub_080769DC
	b _08076F86
_08076DE2:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r1, #0x40
	orr r0, r1
	strh r0, [r2]
	mov r4, ip
	ldrh r0, [r4]
	mov r6, #2
	add ip, r6
	strh r0, [r5, #0xE]
	mov r6, #0
	cmp r6, r0
	blt _08076E00
	b _08076F76
_08076E00:
	ldr r0, _08076EE4 @ =0x03000040
	mov r9, r0
	ldr r1, _08076EE8 @ =0x000001FF
	add r0, r1, #0
	mov r8, r3
	mov r2, r8
	and r2, r0
	mov r8, r2
	mov r4, #1
	neg r4, r4
	mov sl, r4
_08076E16:
	mov r1, ip
	ldrh r0, [r1]
	add r7, r0, #0
	mov r2, #6
	add ip, r2
	lsl r3, r6, #3
	mov r4, r9
	add r2, r3, r4
	ldr r0, _08076EEC @ =0x00004431
	add r1, r2, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	ldr r0, _08076EF0 @ =0x00004432
	add r1, r2, r0
	ldr r0, _08076EF4 @ =0xFFFFFE00
	ldrh r4, [r1]
	and r0, r4
	mov r4, r8
	orr r0, r4
	strh r0, [r1]
	ldr r0, _08076EF8 @ =0x00004430
	add r2, r2, r0
	mov r1, sp
	ldrb r1, [r1, #8]
	strb r1, [r2]
	add r4, r7, #0
	ldr r1, [r5]
	add r1, #0x20
	ldrh r0, [r1]
	add r1, #2
	lsl r0, r0, #2
	add r1, r1, r0
	add r6, #1
	str r6, [sp, #0xC]
_08076E60:
	ldrh r0, [r1]
	add r1, #2
	add r2, r0, #0
	lsl r0, r2, #5
	add r1, r1, r0
	sub r4, #1
	cmp r4, sl
	bne _08076E60
	add r3, r9
	add r1, r7, #0
	mul r1, r2
	add r1, #1
	ldr r4, _08076EFC @ =0x00004434
	add r2, r3, r4
	ldr r6, _08076F00 @ =0x000003FF
	add r0, r6, #0
	and r1, r0
	ldr r0, _08076F04 @ =0xFFFFFC00
	ldrh r4, [r2]
	and r0, r4
	orr r0, r1
	strh r0, [r2]
	ldr r6, _08076F08 @ =0x00004435
	add r2, r3, r6
	mov r0, #0xF0
	ldrb r1, [r2]
	orr r0, r1
	mov r1, #0xD
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r2, _08076EEC @ =0x00004431
	add r1, r3, r2
	mov r4, #0x3F
	add r0, r4, #0
	ldrb r6, [r1]
	and r0, r6
	strb r0, [r1]
	ldr r0, _08076F0C @ =0x00004433
	add r2, r3, r0
	mov r0, #1
	ldr r1, [sp, #4]
	and r1, r0
	lsl r1, r1, #4
	mov r0, #0x11
	neg r0, r0
	ldrb r6, [r2]
	and r0, r6
	orr r0, r1
	strb r0, [r2]
	ldr r0, [r5]
	add r0, #0x22
	lsl r1, r7, #2
	add r0, r0, r1
	ldrh r0, [r0]
	add r1, r0, #0
	mov r0, #0x80
	lsl r0, r0, #7
	cmp r1, r0
	beq _08076F34
	cmp r1, r0
	bgt _08076F10
	cmp r1, #0
	beq _08076F22
	b _08076F6C
	.align 2, 0
_08076EE4: .4byte 0x03000040
_08076EE8: .4byte 0x000001FF
_08076EEC: .4byte 0x00004431
_08076EF0: .4byte 0x00004432
_08076EF4: .4byte 0xFFFFFE00
_08076EF8: .4byte 0x00004430
_08076EFC: .4byte 0x00004434
_08076F00: .4byte 0x000003FF
_08076F04: .4byte 0xFFFFFC00
_08076F08: .4byte 0x00004435
_08076F0C: .4byte 0x00004433
_08076F10:
	mov r0, #0x80
	lsl r0, r0, #8
	cmp r1, r0
	beq _08076F48
	mov r0, #0xC0
	lsl r0, r0, #8
	cmp r1, r0
	beq _08076F60
	b _08076F6C
_08076F22:
	ldr r0, _08076F30 @ =0x00004433
	add r1, r3, r0
	add r0, r4, #0
	ldrb r2, [r1]
	and r0, r2
	b _08076F6A
	.align 2, 0
_08076F30: .4byte 0x00004433
_08076F34:
	ldr r6, _08076F44 @ =0x00004433
	add r0, r3, r6
	add r1, r4, #0
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x40
	b _08076F54
	.align 2, 0
_08076F44: .4byte 0x00004433
_08076F48:
	ldr r6, _08076F5C @ =0x00004433
	add r0, r3, r6
	add r1, r4, #0
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x80
_08076F54:
	orr r1, r2
	strb r1, [r0]
	b _08076F6C
	.align 2, 0
_08076F5C: .4byte 0x00004433
_08076F60:
	ldr r4, _08076F98 @ =0x00004433
	add r1, r3, r4
	mov r0, #0xC0
	ldrb r6, [r1]
	orr r0, r6
_08076F6A:
	strb r0, [r1]
_08076F6C:
	ldr r6, [sp, #0xC]
	ldrh r0, [r5, #0xE]
	cmp r6, r0
	bge _08076F76
	b _08076E16
_08076F76:
	ldr r1, [sp, #0]
	cmp r1, #0
	beq _08076F86
	mov r2, ip
	str r2, [r5, #4]
	ldrh r0, [r5, #0xA]
	add r0, #1
	strh r0, [r5, #0xA]
_08076F86:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08076F98: .4byte 0x00004433
	thumb_func_end sub_08076DAC

