	thumb_func_start sub_0807A298
sub_0807A298: @ 0x0807A298
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	mov r5, #0
	mov r1, #0
_0807A2A0:
	add r0, r4, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x18
	add r6, r1, #1
	cmp r0, #0
	blt _0807A2D8
	ldr r7, _0807A2E8 @ =0x03004470
_0807A2AE:
	lsl r3, r5, #3
	add r3, r3, r7
	asr r0, r0, #0x18
	lsl r2, r0, #1
	add r2, r2, r0
	lsl r2, r2, #2
	add r1, r2, #0
	add r1, #0x14
	add r1, r4, r1
	ldmia r1!, {r0}
	stmia r3!, {r0}
	ldrh r0, [r1]
	strh r0, [r3]
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	add r2, r4, r2
	ldrb r2, [r2, #0x1C]
	lsl r0, r2, #0x18
	cmp r0, #0
	bge _0807A2AE
_0807A2D8:
	lsl r0, r6, #0x18
	lsr r1, r0, #0x18
	cmp r1, #0x13
	bls _0807A2A0
	add r0, r5, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0807A2E8: .4byte 0x03004470
	thumb_func_end sub_0807A298

