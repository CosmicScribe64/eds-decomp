	thumb_func_start sub_080666B4
sub_080666B4: @ 0x080666B4
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r3, #1
	strb r3, [r2]
	strb r0, [r2, #0xC]
	strb r1, [r2, #0xD]
	ldr r0, _080666D4 @ =0x08087488
	lsl r1, r1, #2
	add r1, r1, r0
	ldrh r0, [r1]
	add r0, #3
	strh r0, [r2, #0xE]
	ldrh r0, [r1, #2]
	sub r0, #0x28
	strh r0, [r2, #0x10]
	bx lr
_080666D4: .4byte gUnk_08087488
	thumb_func_end sub_080666B4

