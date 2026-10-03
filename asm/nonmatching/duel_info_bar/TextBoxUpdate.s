	thumb_func_start TextBoxUpdate
TextBoxUpdate: @ 0x08060344
	push {r4, lr}
	ldr r1, _0806036C @ =0x0201AE60
	mov r3, #1
	add r0, r3, #0
	ldrb r2, [r1]
	and r0, r2
	cmp r0, #0
	beq _080603F8
	add r4, r1, #0
	add r4, #0x20
	ldrb r2, [r4]
	add r0, r2, #0
	cmp r0, #1
	beq _08060396
	cmp r0, #1
	bgt _08060370
	cmp r0, #0
	beq _08060376
	b _080603EA
	.align 2, 0
_0806036C: .4byte 0x0201AE60
_08060370:
	cmp r0, #2
	beq _080603CC
	b _080603EA
_08060376:
	add r0, r3, #0
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08060390
	bl TextBoxSlideIn
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080603F4
	ldrb r0, [r4]
	add r0, #1
	b _08060392
_08060390:
	add r0, r2, #1
_08060392:
	strb r0, [r4]
	b _080603F4
_08060396:
	mov r0, #8
	ldrh r3, [r1, #6]
	and r0, r3
	cmp r0, #0
	beq _080603C6
	ldr r1, [r1, #0x1C]
	cmp r1, #0
	beq _080603B6
	bl _call_via_r1
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080603F4
	ldrb r0, [r4]
	add r0, #1
	b _080603C8
_080603B6:
	bl TextBoxHandleInput
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080603F4
	ldrb r0, [r4]
	add r0, #1
	b _080603C8
_080603C6:
	add r0, r2, #1
_080603C8:
	strb r0, [r4]
	b _080603F4
_080603CC:
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080603E4
	bl TextBoxSlideOut
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080603F4
	ldrb r0, [r4]
	add r0, #1
	b _080603E6
_080603E4:
	add r0, r2, #1
_080603E6:
	strb r0, [r4]
	b _080603F4
_080603EA:
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080603F4:
	mov r0, #1
	b _080603FA
_080603F8:
	mov r0, #0
_080603FA:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end TextBoxUpdate

