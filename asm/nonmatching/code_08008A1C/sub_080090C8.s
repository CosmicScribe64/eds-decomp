	thumb_func_start sub_080090C8
sub_080090C8: @ 0x080090C8
	push {lr}
	add r3, r0, #0
	lsl r2, r1, #0x10
	lsr r2, r2, #0x10
	ldr r0, _080090DC @ =0x020192E4
	add r1, r3, #0
	bl sub_08009038
	pop {r1}
	bx r1
_080090DC: .4byte 0x020192E4
	thumb_func_end sub_080090C8

