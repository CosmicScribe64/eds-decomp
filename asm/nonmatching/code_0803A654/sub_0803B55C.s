	thumb_func_start sub_0803B55C
sub_0803B55C: @ 0x0803B55C
	push {r4, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _0803B588
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #3
	bl sub_08019CD0
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xC8
	lsl r1, r1, #2
	bl sub_08019860
_0803B588:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0803B55C

