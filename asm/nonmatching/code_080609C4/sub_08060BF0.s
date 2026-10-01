	thumb_func_start sub_08060BF0
sub_08060BF0: @ 0x08060BF0
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r1, _08060C4C @ =0x04000050
	ldr r2, _08060C50 @ =0x000027E7
	add r0, r2, #0
	strh r0, [r1]
	ldr r1, _08060C54 @ =0x03000040
	ldr r0, _08060C58 @ =0x00004832
	add r4, r1, r0
	ldrb r3, [r4]
	lsl r2, r3, #0x1A
	lsr r0, r2, #0x1A
	add r6, r1, #0
	cmp r0, #8
	bhi _08060C32
	add r1, r0, #0
	add r1, r1, r5
	mov r0, #0x3F
	and r1, r0
	mov r5, #0x40
	neg r5, r5
	add r2, r5, #0
	and r2, r3
	orr r2, r1
	strb r2, [r4]
	lsl r0, r2, #0x1A
	lsr r0, r0, #0x1A
	cmp r0, #9
	bls _08060C32
	and r2, r5
	mov r0, #9
	orr r2, r0
	strb r2, [r4]
_08060C32:
	ldr r2, _08060C5C @ =0x04000054
	ldr r1, _08060C58 @ =0x00004832
	add r0, r6, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r1, r0, #0x1A
	strh r1, [r2]
	add r0, r1, #0
	cmp r0, #8
	bls _08060C60
	mov r0, #1
	b _08060C62
	.align 2, 0
_08060C4C: .4byte 0x04000050
_08060C50: .4byte 0x000027E7
_08060C54: .4byte 0x03000040
_08060C58: .4byte 0x00004832
_08060C5C: .4byte 0x04000054
_08060C60:
	mov r0, #0
_08060C62:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_08060BF0

