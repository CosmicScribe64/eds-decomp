	thumb_func_start sub_08002704
sub_08002704: @ 0x08002704
	push {lr}
	bl sub_08002388
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _08002724 @ =0x00001F04
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #4
	bl sub_08075AE4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	.align 2, 0
_08002724: .4byte 0x00001F04
	thumb_func_end sub_08002704

