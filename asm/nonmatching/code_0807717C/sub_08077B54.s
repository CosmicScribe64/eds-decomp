	thumb_func_start sub_08077B54
sub_08077B54: @ 0x08077B54
	push {r4, lr}
	add r4, r0, #0
	bl sub_08077A5C
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077B68
	add r0, r4, #0
	bl sub_0807E674
_08077B68:
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_08077B54
	.align 2, 0

