	thumb_func_start sub_08035D44
sub_08035D44: @ 0x08035D44
	push {r4, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08035D70
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	bl sub_080199E0
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xFA
	lsl r1, r1, #2
	bl sub_08019980
_08035D70:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08035D44

