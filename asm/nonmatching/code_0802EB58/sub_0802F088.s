	thumb_func_start sub_0802F088
sub_0802F088: @ 0x0802F088
	push {lr}
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #1
	mov r2, #0
	bl sub_080088A4
	mov r1, #0
	cmp r0, #0
	ble _0802F0A4
	mov r1, #1
_0802F0A4:
	add r0, r1, #0
	pop {r1}
	bx r1
	thumb_func_end sub_0802F088
	.align 2, 0

