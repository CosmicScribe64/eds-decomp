	thumb_func_start sub_08027CB8
sub_08027CB8: @ 0x08027CB8
	push {r4, r5, lr}
	add r3, r0, #0
	mov r2, #0
_08027CBE:
	lsl r0, r2, #2
	add r0, r0, r3
	ldrb r4, [r0]
	ldrb r5, [r0, #1]
	add r1, r4, r5
	strb r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #3
	bls _08027CBE
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08027CB8
	.align 2, 0

