	thumb_func_start sub_0800756C
sub_0800756C: @ 0x0800756C
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	ldr r0, _08007588 @ =0x000002D6
	cmp r1, r0
	blt _0800758C
	add r0, #2
	cmp r1, r0
	ble _08007582
	add r0, #0x26
	cmp r1, r0
	bne _0800758C
_08007582:
	mov r0, #1
	b _0800758E
	.align 2, 0
_08007588: .4byte 0x000002D6
_0800758C:
	mov r0, #0
_0800758E:
	bx lr
	thumb_func_end sub_0800756C

