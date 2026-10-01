	thumb_func_start sub_080602A4
sub_080602A4: @ 0x080602A4
	push {r4, r5, r6, r7, lr}
	add r4, r2, #0
	add r2, r3, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	ldr r7, _080602FC @ =0x0201AE60
	mov r6, #0
	mov r5, #0
	strh r5, [r7, #2]
	bl sub_0805FD28
	strh r4, [r7, #6]
	strh r5, [r7, #4]
	str r5, [r7, #0x18]
	str r5, [r7, #0x1C]
	add r0, r7, #0
	add r0, #0x20
	strb r6, [r0]
	add r0, #1
	strb r6, [r0]
	add r0, #1
	strb r6, [r0]
	add r0, #1
	strb r6, [r0]
	strh r5, [r7, #0x14]
	mov r0, #1
	ldrb r1, [r7]
	orr r0, r1
	strb r0, [r7]
	ldr r1, _08060300 @ =0x0201CFB0
	ldr r2, _08060304 @ =0x00000808
	add r1, r1, r2
	mov r0, #9
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080602FC: .4byte 0x0201AE60
_08060300: .4byte 0x0201CFB0
_08060304: .4byte 0x00000808
	thumb_func_end sub_080602A4

