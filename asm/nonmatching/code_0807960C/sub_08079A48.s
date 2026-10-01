	thumb_func_start sub_08079A48
sub_08079A48: @ 0x08079A48
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	str r3, [sp, #0x10]
	ldr r3, [sp, #0x38]
	ldr r4, [sp, #0x3C]
	ldr r5, [sp, #0x40]
	ldr r6, [sp, #0x44]
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	str r1, [sp, #0xC]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	str r3, [sp, #0x14]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	mov sl, r5
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	mov r9, r6
	ldr r0, _08079B00 @ =0x0000813F
	cmp r7, r0
	bhi _08079A98
	add r0, r7, #0
	bl sub_08074A90
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	cmp r7, #0
	beq _08079B72
_08079A98:
	cmp r4, #8
	bne _08079B08
	add r0, r7, #0
	bl sub_08072584
	lsl r0, r0, #3
	ldr r1, _08079B04 @ =0x081C0000
	add r7, r0, r1
	mov r6, #3
_08079AAA:
	ldrh r4, [r7]
	add r7, #2
	lsl r4, r4, #0x11
	lsl r0, r4, #8
	lsr r0, r0, #0x18
	mov r2, r8
	add r1, r2, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	mov r5, r8
	ldr r1, [sp, #0x10]
	str r1, [sp, #0]
	mov r1, sl
	str r1, [sp, #4]
	mov r1, r9
	str r1, [sp, #8]
	ldr r1, [sp, #0xC]
	ldr r3, [sp, #0x14]
	bl sub_080798B8
	lsr r4, r4, #0x18
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	ldr r0, [sp, #0x10]
	str r0, [sp, #0]
	mov r1, sl
	str r1, [sp, #4]
	mov r0, r9
	str r0, [sp, #8]
	add r0, r4, #0
	ldr r1, [sp, #0xC]
	add r2, r5, #0
	ldr r3, [sp, #0x14]
	bl sub_080798B8
	sub r6, #1
	cmp r6, #0
	bge _08079AAA
	b _08079B72
	.align 2, 0
_08079B00: .4byte 0x0000813F
_08079B04: .4byte gUnk_081C0000
_08079B08:
	cmp r4, #0xA
	beq _08079B12
	cmp r4, #0xC
	beq _08079B28
	b _08079B72
_08079B12:
	add r0, r7, #0
	bl sub_08072584
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r0, _08079B24 @ =0x081D0200
	b _08079B36
	.align 2, 0
_08079B24: .4byte gUnk_081D0200
_08079B28:
	add r0, r7, #0
	bl sub_08072584
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	ldr r0, _08079B84 @ =0x081F8700
_08079B36:
	add r7, r1, r0
	cmp r4, #0
	beq _08079B72
	add r6, r4, #0
_08079B3E:
	ldrh r0, [r7]
	add r7, #2
	lsr r1, r0, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r0, r1
	lsl r0, r0, #0x11
	lsr r0, r0, #0x10
	mov r2, r8
	add r1, r2, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	ldr r1, [sp, #0x10]
	str r1, [sp, #0]
	mov r1, sl
	str r1, [sp, #4]
	mov r1, r9
	str r1, [sp, #8]
	ldr r1, [sp, #0xC]
	ldr r3, [sp, #0x14]
	bl sub_080799BC
	sub r6, #1
	cmp r6, #0
	bne _08079B3E
_08079B72:
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08079B84: .4byte gUnk_081F8700
	thumb_func_end sub_08079A48

