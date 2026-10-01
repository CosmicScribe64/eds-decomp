	thumb_func_start sub_0802D47C
sub_0802D47C: @ 0x0802D47C
	lsl r2, r2, #0x10
	cmp r2, #0
	beq _0802D4A4
	ldr r2, _0802D49C @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802D4A0 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1B
	cmp r0, #0
	blt _0802D4A4
	mov r0, #1
	b _0802D4A6
_0802D49C: .4byte 0x020192E4
_0802D4A0: .4byte 0x00000D64
_0802D4A4:
	mov r0, #0
_0802D4A6:
	bx lr
	thumb_func_end sub_0802D47C

