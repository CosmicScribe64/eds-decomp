	thumb_func_start SetVramMapTile
SetVramMapTile: @ 0x0807AC5C
	lsl r0, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	lsr r2, r2, #0x13
	add r1, r1, r2
	lsl r1, r1, #1
	lsr r0, r0, #0xD
	add r1, r1, r0
	mov r0, #0xC0
	lsl r0, r0, #0x13
	add r1, r1, r0
	mov r0, #0xFC
	lsl r0, r0, #8
	ldrh r2, [r1]
	and r0, r2
	orr r0, r3
	strh r0, [r1]
	bx lr
	thumb_func_end SetVramMapTile
	.align 2, 0

