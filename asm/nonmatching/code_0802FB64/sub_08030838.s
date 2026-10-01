	thumb_func_start sub_08030838
sub_08030838: @ 0x08030838
	push {r4, r5, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r5, #1
	sub r0, r5, r0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_08008B70
	add r2, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08030876
	cmp r2, #0
	ble _08030876
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	sub r0, r5, r0
	lsl r1, r2, #5
	sub r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r2
	lsl r1, r1, #2
	bl sub_08019860
_08030876:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08030838
	.align 2, 0

