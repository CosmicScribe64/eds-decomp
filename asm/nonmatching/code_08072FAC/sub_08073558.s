	thumb_func_start sub_08073558
sub_08073558: @ 0x08073558
	push {lr}
	ldr r0, _0807356C @ =0x06004400
	ldr r1, _08073570 @ =0x02010014
	mov r2, #0xE0
	lsl r2, r2, #5
	bl sub_080752B0
	pop {r0}
	bx r0
	.align 2, 0
_0807356C: .4byte 0x06004400
_08073570: .4byte 0x02010014
	thumb_func_end sub_08073558

