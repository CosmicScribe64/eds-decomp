	thumb_func_start sub_08058074
sub_08058074: @ 0x08058074
	push {r4, lr}
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	bl sub_08057E08
	cmp r4, #0
	beq _08058088
	bl sub_08057E70
_08058088:
	bl sub_08057F6C
	add r4, r0, #0
	bl sub_08057E3C
	add r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08058074
	.align 2, 0

