	thumb_func_start sub_080241C4
sub_080241C4: @ 0x080241C4
	push {lr}
	ldr r2, _080241E4 @ =0x0201CFB0
	ldr r1, _080241E8 @ =0x00000824
	add r0, r2, r1
	ldr r0, [r0]
	ldr r3, _080241EC @ =0x00000828
	add r1, r2, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r2, r3
	ldr r2, [r2]
	bl sub_08024134
	pop {r0}
	bx r0
	.align 2, 0
_080241E4: .4byte 0x0201CFB0
_080241E8: .4byte 0x00000824
_080241EC: .4byte 0x00000828
	thumb_func_end sub_080241C4

