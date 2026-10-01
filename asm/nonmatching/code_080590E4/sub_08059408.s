	thumb_func_start sub_08059408
sub_08059408: @ 0x08059408
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r0, #1
	add r1, r5, #0
	bl sub_0800A304
	mov sl, r0
	ldr r1, _08059448 @ =0x00000405
	mov r0, #0
	bl sub_08009280
	cmp r0, #0
	beq _0805945E
	ldr r0, _0805944C @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _08059450
	bl sub_08076F9C
	mov r1, #3
	and r1, r0
	cmp r1, #0
	beq _0805945E
	b _0805976C
_08059448: .4byte 0x00000405
_0805944C: .4byte 0x02015EE8
_08059450:
	bl sub_08076F9C
	mov r1, #7
	and r1, r0
	cmp r1, #0
	bne _0805945E
	b _0805976C
_0805945E:
	ldr r0, _08059468 @ =0x0000FFFF
	cmp r5, r0
	bne _0805946C
	mov r0, #0
	b _0805949A
_08059468: .4byte 0x0000FFFF
_0805946C:
	ldr r0, _08059480 @ =0x000007CF
	cmp r5, r0
	bhi _08059488
	add r0, #0x30
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _08059484 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _0805949A
_08059480: .4byte 0x000007CF
_08059484: .4byte gUnk_08623DF4
_08059488:
	ldr r3, _080594C0 @ =0xFFFFF830
	add r0, r5, r3
	ldr r1, _080594C4 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080594C8 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_0805949A:
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r3, _080594CC @ =0x08621DE0
	add r0, r0, r3
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _080594D0
	cmp r0, #0x15
	blt _080594D0
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _080594D2
	.align 2, 0
_080594C0: .4byte 0xFFFFF830
_080594C4: .4byte 0x000007FF
_080594C8: .4byte gUnk_08623DF4
_080594CC: .4byte gUnk_08621DE0
_080594D0:
	mov r0, #0
_080594D2:
	cmp r0, #2
	bne _080594E0
	bl sub_080094E4
	cmp r0, r5
	bne _080594E0
	b _0805976C
_080594E0:
	ldr r0, _08059504 @ =0x0000028D
	cmp r5, r0
	beq _0805956E
	cmp r5, r0
	bgt _08059520
	mov r0, #0x9F
	lsl r0, r0, #1
	cmp r5, r0
	beq _0805956E
	cmp r5, r0
	bgt _08059508
	sub r0, #2
	cmp r5, r0
	ble _080594FE
	b _080595F4
_080594FE:
	sub r0, #0x10
	b _0805953C
	.align 2, 0
_08059504: .4byte 0x0000028D
_08059508:
	mov r0, #0xA0
	lsl r0, r0, #1
	cmp r5, r0
	blt _080595F4
	add r0, #7
	cmp r5, r0
	ble _0805956E
	ldr r0, _0805951C @ =0x0000028B
	b _0805955C
	.align 2, 0
_0805951C: .4byte 0x0000028B
_08059520:
	ldr r0, _08059544 @ =0x00000412
	cmp r5, r0
	beq _0805956E
	cmp r5, r0
	bgt _08059550
	ldr r0, _08059548 @ =0x0000029B
	cmp r5, r0
	beq _0805956E
	cmp r5, r0
	blt _080595F4
	ldr r0, _0805954C @ =0x000003F5
	cmp r5, r0
	bgt _080595F4
	sub r0, #1
_0805953C:
	cmp r5, r0
	blt _080595F4
	b _0805956E
	.align 2, 0
_08059544: .4byte 0x00000412
_08059548: .4byte 0x0000029B
_0805954C: .4byte 0x000003F5
_08059550:
	ldr r0, _08059564 @ =0x000004B3
	cmp r5, r0
	beq _0805956E
	cmp r5, r0
	bgt _08059568
	sub r0, #0x15
_0805955C:
	cmp r5, r0
	beq _0805956E
	b _080595F4
	.align 2, 0
_08059564: .4byte 0x000004B3
_08059568:
	ldr r0, _08059588 @ =0x0000058C
	cmp r5, r0
	bne _080595F4
_0805956E:
	mov r6, #1
	mov r7, #0
	ldr r0, _0805958C @ =0xFFFFF830
	add r0, r0, r5
	mov r8, r0
	mov r4, sp
	ldr r1, _08059590 @ =0x0000FFFF
	mov r9, r1
_0805957E:
	cmp r5, r9
	bne _08059594
	mov r0, #0
	b _080595C8
	.align 2, 0
_08059588: .4byte 0x0000058C
_0805958C: .4byte 0xFFFFF830
_08059590: .4byte 0x0000FFFF
_08059594:
	ldr r0, _080595AC @ =0x000007CF
	cmp r5, r0
	bhi _080595B8
	ldr r3, _080595B0 @ =0x000007FF
	add r1, r3, #0
	add r0, r5, #0
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _080595B4 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _080595C8
_080595AC: .4byte 0x000007CF
_080595B0: .4byte 0x000007FF
_080595B4: .4byte gUnk_08623DF4
_080595B8:
	ldr r0, _0805967C @ =0x000007FF
	mov r3, r8
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _08059680 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_080595C8:
	strh r0, [r4]
	ldrb r0, [r4, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r4, #2]
	lsl r0, r7, #0x18
	mov r1, #0x80
	lsl r1, r1, #9
	orr r1, r0
	lsr r1, r1, #0x10
	mov r0, sp
	bl sub_0802B558
	cmp r0, #0
	beq _080595E8
	mov r6, #0
_080595E8:
	add r7, #1
	cmp r7, #4
	ble _0805957E
	cmp r6, #0
	beq _080595F4
	b _0805976C
_080595F4:
	mov r7, #5
	mov r4, sp
_080595F8:
	ldrb r0, [r4, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r4, #2]
	ldrb r1, [r4, #3]
	mov r0, #3
	and r0, r1
	strb r0, [r4, #3]
	mov r0, #0x3F
	add r2, r7, #0
	and r2, r0
	lsl r2, r2, #4
	ldrh r0, [r4, #2]
	ldr r3, _08059684 @ =0xFFFFFC0F
	add r1, r3, #0
	and r0, r1
	orr r0, r2
	strh r0, [r4, #2]
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08059688 @ =0x0201A070
	add r6, r1, r0
	ldr r0, [r6]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	strh r2, [r4]
	cmp r2, #0
	beq _0805966A
	add r1, r6, #0
	add r1, #0x91
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805966A
	mov r0, #2
	ldrb r1, [r6, #6]
	and r0, r1
	cmp r0, #0
	bne _0805966A
	ldr r3, _0805967C @ =0x000007FF
	add r0, r3, #0
	and r2, r0
	lsl r0, r2, #1
	ldr r1, _0805968C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r5
	bne _0805966A
	mov r0, sp
	mov r1, #0
	mov r2, #0
	bl sub_0802CE38
	cmp r0, #0
	bne _08059728
_0805966A:
	add r7, #1
	cmp r7, #9
	ble _080595F8
	ldr r0, _08059690 @ =0x0000FFFF
	cmp r5, r0
	bne _08059694
	mov r0, #0
	b _080596C2
	.align 2, 0
_0805967C: .4byte 0x000007FF
_08059680: .4byte gUnk_08623DF4
_08059684: .4byte 0xFFFFFC0F
_08059688: .4byte 0x0201A070
_0805968C: .4byte gUnk_08622AB4
_08059690: .4byte 0x0000FFFF
_08059694:
	ldr r0, _080596A8 @ =0x000007CF
	cmp r5, r0
	bhi _080596B0
	add r0, #0x30
	and r0, r5
	lsl r0, r0, #1
	ldr r3, _080596AC @ =0x08623DF4
	add r0, r0, r3
	ldrh r0, [r0]
	b _080596C2
_080596A8: .4byte 0x000007CF
_080596AC: .4byte gUnk_08623DF4
_080596B0:
	ldr r1, _08059714 @ =0xFFFFF830
	add r0, r5, r1
	ldr r1, _08059718 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _0805971C @ =0x08623DF4
	add r0, r0, r3
	ldrh r0, [r0]
	add r0, #1
_080596C2:
	lsl r1, r0, #0x10
	lsr r1, r1, #0x10
	mov r0, #1
	bl sub_08008C94
	cmp r0, #0
	beq _0805976C
	mov r0, sl
	cmp r0, #0
	blt _0805976C
	mov r2, sp
	ldrb r0, [r2, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r2, #2]
	ldrb r1, [r2, #3]
	mov r0, #3
	and r0, r1
	strb r0, [r2, #3]
	mov r1, sl
	lsl r0, r1, #2
	ldr r1, _08059720 @ =0x0201A6CC
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r2]
	mov r0, sp
	mov r1, #0
	mov r2, #1
	bl sub_0802CE38
	cmp r0, #0
	beq _0805976C
	ldr r1, _08059724 @ =0x02015EF0
	mov r3, sl
	strb r3, [r1, #0xB]
	mov r0, #0xC8
	strb r0, [r1, #6]
	mov r0, #1
	b _0805976E
_08059714: .4byte 0xFFFFF830
_08059718: .4byte 0x000007FF
_0805971C: .4byte gUnk_08623DF4
_08059720: .4byte 0x0201A6CC
_08059724: .4byte 0x02015EF0
_08059728:
	ldr r0, _08059760 @ =0x00008008
	lsl r2, r7, #0x18
	lsr r2, r2, #0x10
	mov r1, #1
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _08059764 @ =0x0000807F
	lsl r1, r7, #0x10
	lsr r1, r1, #0x10
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x1F
	and r7, r0
	lsl r0, r7, #0x10
	ldr r1, [r6]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	ldr r2, _08059768 @ =0x80200000
	orr r1, r2
	orr r0, r1
	mov r1, #0
	bl sub_0801FBCC
	mov r0, #1
	b _0805976E
_08059760: .4byte 0x00008008
_08059764: .4byte 0x0000807F
_08059768: .4byte 0x80200000
_0805976C:
	mov r0, #0
_0805976E:
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08059408
	.align 2, 0

