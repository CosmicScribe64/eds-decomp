	thumb_func_start sub_08068FEC
sub_08068FEC: @ 0x08068FEC
	ldr r2, _0806900C @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _08069010 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	lsr r0, r0, #0x1D
	and r1, r2
	lsl r1, r1, #2
	add r1, r1, r3
	ldr r1, [r1]
	lsr r1, r1, #0x1D
	sub r0, r0, r1
	lsr r0, r0, #0x1F
	bx lr
	.align 2, 0
_0806900C: .4byte 0x000007FF
_08069010: .4byte gUnk_08621DE0
	thumb_func_end sub_08068FEC

