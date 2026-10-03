	thumb_func_start TextBoxHandleChoiceInputCpu
TextBoxHandleChoiceInputCpu: @ 0x08052668
	push {r4, r5, r6, r7, lr}
	ldr r5, _08052684 @ =0x0201AE60
	add r7, r5, #0
	add r7, #0x22
	ldrb r0, [r7]
	add r4, r0, #0
	cmp r4, #1
	beq _080526C6
	cmp r4, #1
	bgt _08052688
	cmp r4, #0
	beq _0805268E
	b _0805270A
	.align 2, 0
_08052684: .4byte 0x0201AE60
_08052688:
	cmp r4, #2
	beq _08052706
	b _0805270A
_0805268E:
	add r6, r5, #0
	add r6, #0x23
	ldrb r0, [r6]
	lsl r1, r0, #0x18
	lsr r0, r1, #0x18
	cmp r0, #0x3F
	bhi _080526A4
	lsr r0, r1, #0x1A
	mov r1, #1
	and r0, r1
	strh r0, [r5, #0x14]
_080526A4:
	ldrb r0, [r6]
	cmp r0, #0x40
	bne _080526B4
	bl Random
	mov r1, #1
	and r0, r1
	strh r0, [r5, #0x14]
_080526B4:
	ldrb r0, [r6]
	cmp r0, #0xC0
	bls _080526C0
	strb r4, [r6]
	ldrb r0, [r7]
	b _08052700
_080526C0:
	add r0, #1
	strb r0, [r6]
	b _0805270A
_080526C6:
	add r5, #0x23
	ldrb r2, [r5]
	cmp r2, #0x3B
	bhi _08052700
	add r3, r2, #1
	strb r3, [r5]
	ldr r1, _080526F8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _080526E8
	ldr r0, _080526FC @ =0x0201CFB0
	ldrb r0, [r0]
	and r4, r0
	cmp r4, #0
	beq _0805270A
_080526E8:
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x33
	bhi _0805270A
	add r0, r2, #0
	add r0, #8
	strb r0, [r5]
	b _0805270A
_080526F8: .4byte 0x03000040
_080526FC: .4byte 0x0201CFB0
_08052700:
	add r0, #1
	strb r0, [r7]
	b _0805270A
_08052706:
	mov r0, #1
	b _0805270C
_0805270A:
	mov r0, #0
_0805270C:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end TextBoxHandleChoiceInputCpu
	.align 2, 0

