	thumb_func_start sub_08063AB8
sub_08063AB8: @ 0x08063AB8
	push {lr}
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0xF8
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	bl sub_0806245C
	bl sub_080624A4
	mov r0, #4
	bl sub_08075AE4
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08063AEA
	ldr r0, _08063AF0 @ =0x03000040
	ldr r1, _08063AF4 @ =0x00004859
	add r0, r0, r1
	ldrb r1, [r0]
	sub r1, #5
	strb r1, [r0]
_08063AEA:
	mov r0, #0
	pop {r1}
	bx r1
_08063AF0: .4byte 0x03000040
_08063AF4: .4byte 0x00004859
	thumb_func_end sub_08063AB8

