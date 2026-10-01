	thumb_func_start sub_0805DF04
sub_0805DF04: @ 0x0805DF04
	push {r4, r5, lr}
	add r5, r1, #0
	lsl r2, r2, #0x10
	mov r1, #0x68
	mul r1, r0
	add r4, r1, #0
	add r4, #8
	add r3, r0, #0
	cmp r2, #0
	beq _0805DF22
	bl sub_08076F9C
	add r3, r0, #0
	mov r0, #3
	and r3, r0
_0805DF22:
	add r0, r4, #0
	mov r1, #0x40
	add r2, r5, #0
	bl sub_0805DDC4
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_0805DF04
	.align 2, 0

