	thumb_func_start sub_08078534
sub_08078534: @ 0x08078534
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x30
	add r7, r0, #0
	ldr r0, [sp, #0x50]
	ldr r4, [sp, #0x54]
	ldr r5, [sp, #0x58]
	ldr r6, [sp, #0x5C]
	mov ip, r6
	ldr r6, [sp, #0x60]
	mov r8, r6
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0x20]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #0x24]
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	str r3, [sp, #0x28]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov sl, r0
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r9, r4
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	mov r0, ip
	lsl r6, r0, #0x10
	lsr r6, r6, #0x10
	str r6, [sp, #0x2C]
	mov r1, r8
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	cmp r5, #4
	beq _08078596
	cmp r5, #4
	bgt _08078592
	cmp r5, #2
	bgt _08078600
	cmp r5, #1
	blt _08078600
	b _08078596
_08078592:
	cmp r5, #8
	bne _08078600
_08078596:
	mov r4, #0
	mov r2, #0xC8
	lsl r2, r2, #1
	add r0, r7, r2
	ldrh r0, [r0]
	cmp r4, r0
	bcs _0807865E
	mov r8, r4
_080785A6:
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r1, r7, r0
	mov r0, #0xE
	ldsb r0, [r1, r0]
	mov r2, #1
	neg r2, r2
	cmp r0, r2
	beq _080785EC
	mov r0, sp
	ldrh r0, [r0, #0x2C]
	strh r0, [r1, #8]
	strh r6, [r1, #0xA]
	ldr r0, [r1, #4]
	ldrb r2, [r1, #0xC]
	ldrh r3, [r1, #8]
	ldrh r1, [r1, #0xA]
	str r1, [sp, #0]
	str r5, [sp, #4]
	ldr r1, [sp, #0x24]
	str r1, [sp, #8]
	ldr r1, [sp, #0x28]
	str r1, [sp, #0xC]
	mov r1, sl
	str r1, [sp, #0x10]
	mov r1, r9
	str r1, [sp, #0x14]
	mov r1, r8
	str r1, [sp, #0x18]
	ldr r1, [sp, #0x64]
	str r1, [sp, #0x1C]
	ldr r1, [sp, #0x20]
	bl sub_08077EF4
_080785EC:
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	mov r2, #0xC8
	lsl r2, r2, #1
	add r0, r7, r2
	ldrh r0, [r0]
	cmp r4, r0
	bcc _080785A6
	b _0807865E
_08078600:
	mov r4, #0
	mov r6, #0xC8
	lsl r6, r6, #1
	add r0, r7, r6
	ldrh r0, [r0]
	cmp r4, r0
	bcs _0807865E
	mov r6, #1
	neg r6, r6
_08078612:
	lsl r0, r4, #2
	add r0, r0, r4
	lsl r0, r0, #2
	add r1, r7, r0
	mov r0, #0xE
	ldsb r0, [r1, r0]
	cmp r0, r6
	beq _0807864C
	ldr r0, [r1, #4]
	ldrb r2, [r1, #0xC]
	ldrh r3, [r1, #8]
	ldrh r1, [r1, #0xA]
	str r1, [sp, #0]
	str r5, [sp, #4]
	ldr r1, [sp, #0x24]
	str r1, [sp, #8]
	ldr r1, [sp, #0x28]
	str r1, [sp, #0xC]
	mov r1, sl
	str r1, [sp, #0x10]
	mov r1, r9
	str r1, [sp, #0x14]
	mov r1, #0
	str r1, [sp, #0x18]
	ldr r1, [sp, #0x64]
	str r1, [sp, #0x1C]
	ldr r1, [sp, #0x20]
	bl sub_08077EF4
_0807864C:
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	mov r2, #0xC8
	lsl r2, r2, #1
	add r0, r7, r2
	ldrh r0, [r0]
	cmp r4, r0
	bcc _08078612
_0807865E:
	add sp, #0x30
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08078534
	.align 2, 0

