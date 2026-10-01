	thumb_func_start sub_08017C0C
sub_08017C0C: @ 0x08017C0C
	push {r4, r5, r6, lr}
	add r2, r0, #0
	add r5, r1, #0
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r4, r6, #0x18
	lsr r4, r4, #0x18
	lsr r2, r2, #0x18
	add r0, r4, #0
	add r1, r2, #0
	bl sub_0800CD68
	add r2, r0, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	add r1, r6, #0
	mov r3, #1
	bl sub_08017ADC
	add r0, r4, #0
	add r1, r6, #0
	add r2, r5, #0
	bl sub_08017B04
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end sub_08017C0C

