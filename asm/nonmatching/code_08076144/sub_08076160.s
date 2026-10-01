	thumb_func_start sub_08076160
sub_08076160: @ 0x08076160
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r3, _080761C4 @ =0x03004470
	mov r8, r3
	ldr r6, _080761C8 @ =0x081A77A8
	mov r4, #0x7F
	add r3, r2, #0
	and r3, r4
	lsl r3, r3, #1
	add r3, r3, r6
	mov r7, #0
	ldsh r5, [r3, r7]
	add r3, r2, #0
	add r3, #0x20
	and r3, r4
	lsl r3, r3, #1
	add r3, r3, r6
	mov r7, #0
	ldsh r3, [r3, r7]
	add r2, #0x40
	and r2, r4
	lsl r2, r2, #1
	add r2, r2, r6
	mov r4, #0
	ldsh r2, [r2, r4]
	lsr r0, r0, #0xB
	add r8, r0
	mul r5, r1
	mul r3, r1
	mul r2, r1
	asr r5, r5, #8
	asr r3, r3, #8
	asr r2, r2, #8
	mov r7, r8
	strh r3, [r7, #6]
	strh r5, [r7, #0xE]
	strh r2, [r7, #0x16]
	strh r3, [r7, #0x1E]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080761C4: .4byte 0x03004470
_080761C8: .4byte gUnk_081A77A8
	thumb_func_end sub_08076160

