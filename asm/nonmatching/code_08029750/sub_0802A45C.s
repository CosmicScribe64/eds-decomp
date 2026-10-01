	thumb_func_start sub_0802A45C
sub_0802A45C: @ 0x0802A45C
	push {lr}
	ldr r1, _0802A478 @ =0x0201D810
	ldrb r2, [r1, #5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	ldrh r2, [r1, #6]
	add r0, r2, r0
	lsl r0, r0, #2
	add r1, #0xC
	add r0, r0, r1
	bl sub_0802A188
	pop {r0}
	bx r0
_0802A478: .4byte 0x0201D810
	thumb_func_end sub_0802A45C

