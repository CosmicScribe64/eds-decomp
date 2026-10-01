	thumb_func_start sub_08073184
sub_08073184: @ 0x08073184
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r0, r1, #0
	add r5, r2, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	lsl r0, r0, #0x10
	ldrh r2, [r5]
	lsl r1, r2, #1
	add r6, r1, #0
	add r6, #8
	add r6, r5, r6
	add r1, #0x10
	add r1, r5, r1
	lsr r0, r0, #0xB
	ldr r3, _080731CC @ =0x06004000
	add r0, r0, r3
	ldrh r3, [r6]
	lsl r2, r3, #5
	bl sub_08075294
	lsl r4, r4, #1
	mov r0, #0xA0
	lsl r0, r0, #0x13
	add r4, r4, r0
	add r1, r5, #0
	add r1, #8
	ldrh r5, [r5]
	lsl r2, r5, #1
	add r0, r4, #0
	bl sub_08075294
	ldrh r0, [r6]
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_080731CC: .4byte 0x06004000
	thumb_func_end sub_08073184

