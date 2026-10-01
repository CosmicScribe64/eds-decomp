	thumb_func_start sub_08017ADC
sub_08017ADC: @ 0x08017ADC
	push {r4, lr}
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r1, #0x86
	cmp r0, #0
	beq _08017AF2
	ldr r1, _08017B00 @ =0x00008086
_08017AF2:
	add r0, r1, #0
	add r1, r4, #0
	bl sub_0801EC58
	pop {r4}
	pop {r0}
	bx r0
_08017B00: .4byte 0x00008086
	thumb_func_end sub_08017ADC

