	thumb_func_start sub_0803A7F4
sub_0803A7F4: @ 0x0803A7F4
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _0803A814
	ldrb r1, [r1, #2]
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xFA
	lsl r1, r1, #1
	bl sub_08019860
_0803A814:
	mov r0, #0
	pop {r1}
	bx r1
	thumb_func_end sub_0803A7F4
	.align 2, 0

