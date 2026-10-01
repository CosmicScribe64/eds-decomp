	thumb_func_start sub_0805809C
sub_0805809C: @ 0x0805809C
	push {r4, lr}
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	bl sub_08057E08
	cmp r4, #0
	beq _080580B0
	bl sub_08057E70
_080580B0:
	bl sub_08057C94
	add r4, r0, #0
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	bl sub_08057E3C
	add r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0805809C
	.align 2, 0

