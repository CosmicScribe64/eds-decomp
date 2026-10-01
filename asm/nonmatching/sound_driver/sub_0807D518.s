	thumb_func_start sub_0807D518
sub_0807D518: @ 0x0807D518
	push {r4, lr}
	lsl r1, r1, #4
	add r1, r1, r2
	lsl r1, r1, #4
	ldr r2, _0807D568 @ =0x08139550
	add r1, r1, r2
	ldr r3, _0807D56C @ =0x04000090
	ldr r2, [r1]
	str r2, [r3]
	add r3, #4
	ldr r2, [r1, #4]
	str r2, [r3]
	add r3, #4
	ldr r2, [r1, #8]
	str r2, [r3]
	ldr r2, _0807D570 @ =0x0400009C
	ldr r1, [r1, #0xC]
	str r1, [r2]
	mov r2, #0
	mov r1, #0xC4
	lsl r1, r1, #1
	add r3, r0, r1
	ldrh r1, [r3]
	mov r4, #0x80
	lsl r4, r4, #2
	add r0, r4, #0
	and r0, r1
	cmp r0, #0
	bne _0807D554
	mov r2, #0x40
_0807D554:
	add r0, r4, #0
	eor r0, r1
	strh r0, [r3]
	ldr r1, _0807D574 @ =0x04000070
	mov r0, #0x80
	orr r2, r0
	strh r2, [r1]
	pop {r4}
	pop {r0}
	bx r0
_0807D568: .4byte gUnk_08139550
_0807D56C: .4byte 0x04000090
_0807D570: .4byte 0x0400009C
_0807D574: .4byte 0x04000070
	thumb_func_end sub_0807D518

