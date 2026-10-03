	thumb_func_start MemCopy16
MemCopy16: @ 0x08075294
	add r3, r0, #0
	add r0, r2, #1
	lsr r2, r0, #1
	cmp r2, #0
	beq _080752AC
_0807529E:
	ldrh r0, [r1]
	strh r0, [r3]
	add r1, #2
	add r3, #2
	sub r2, #1
	cmp r2, #0
	bne _0807529E
_080752AC:
	bx lr
	thumb_func_end MemCopy16
	.align 2, 0

