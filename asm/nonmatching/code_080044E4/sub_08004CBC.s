	thumb_func_start sub_08004CBC
sub_08004CBC: @ 0x08004CBC
	push {r4, lr}
	ldr r0, _08004CD4 @ =0x03000040
	ldr r1, _08004CD8 @ =0x00004858
	add r4, r0, r1
	ldrb r1, [r4]
	cmp r1, #1
	beq _08004CF8
	cmp r1, #1
	bgt _08004CDC
	cmp r1, #0
	beq _08004CE2
	b _08004D3C
_08004CD4: .4byte 0x03000040
_08004CD8: .4byte 0x00004858
_08004CDC:
	cmp r1, #2
	beq _08004D16
	b _08004D3C
_08004CE2:
	bl sub_08073498
	ldr r3, _08004CF4 @ =0x087D01F4
	mov r0, #0
	mov r1, #0
	mov r2, #0x20
	bl sub_08072FAC
	b _08004D2C
_08004CF4: .4byte gUnk_087D01F4
_08004CF8:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0x80
	lsl r3, r3, #1
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r0, #1
	bl sub_08075BD0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08004D32
	b _08004D2C
_08004D16:
	ldr r1, _08004D38 @ =0x00004859
	add r2, r0, r1
	ldrb r0, [r2]
	add r1, r0, #1
	strb r1, [r2]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x77
	bls _08004D32
	mov r0, #0
	strb r0, [r2]
_08004D2C:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08004D32:
	mov r0, #0
	b _08004D56
	.align 2, 0
_08004D38: .4byte 0x00004859
_08004D3C:
	mov r0, #1
	bl sub_08075B58
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08004D32
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08004D5C @ =0x0000FEFF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
_08004D56:
	pop {r4}
	pop {r1}
	bx r1
_08004D5C: .4byte 0x0000FEFF
	thumb_func_end sub_08004CBC

