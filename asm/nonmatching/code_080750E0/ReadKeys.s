	thumb_func_start ReadKeys
ReadKeys: @ 0x08075228
	push {r4, lr}
	ldr r0, _08075250 @ =0x04000130
	ldrh r0, [r0]
	mvn r0, r0
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r1, _08075254 @ =0x03000040
	add r2, r3, #0
	ldrh r0, [r1, #4]
	bic r2, r0
	strh r2, [r1, #6]
	strh r3, [r1, #4]
	ldrh r0, [r1, #8]
	cmp r3, r0
	beq _08075258
	mov r0, #0
	strh r0, [r1, #0xA]
	strh r3, [r1, #8]
	b _08075272
	.align 2, 0
_08075250: .4byte 0x04000130
_08075254: .4byte 0x03000040
_08075258:
	ldrh r4, [r1, #0xA]
	add r0, r4, #1
	strh r0, [r1, #0xA]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x14
	bls _08075272
	sub r0, r4, #1
	strh r0, [r1, #0xA]
	mov r0, #0xF0
	and r0, r3
	orr r2, r0
	strh r2, [r1, #6]
_08075272:
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end ReadKeys

