	thumb_func_start sub_08079F10
sub_08079F10: @ 0x08079F10
	push {r4, lr}
	add r4, r0, #0
	mov r3, #0
	cmp r1, #0
	beq _08079F38
_08079F1A:
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r3, r0, #1
	ldrb r2, [r4]
	cmp r2, #0x39
	bgt _08079F30
	cmp r2, #0x30
	blt _08079F30
	add r0, r3, #0
	sub r0, #0x30
	add r3, r0, r2
_08079F30:
	sub r1, #1
	add r4, #1
	cmp r1, #0
	bne _08079F1A
_08079F38:
	add r0, r3, #0
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08079F10

