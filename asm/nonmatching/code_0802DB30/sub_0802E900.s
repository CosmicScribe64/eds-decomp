	thumb_func_start sub_0802E900
sub_0802E900: @ 0x0802E900
	push {r4, r5, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r5, #1
	neg r5, r5
	add r1, r5, #0
	bl sub_08008AF8
	cmp r0, #0
	beq _0802E930
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	add r1, r5, #0
	bl sub_08008AF8
	cmp r0, #0
	beq _0802E930
	mov r0, #1
	b _0802E932
_0802E930:
	mov r0, #0
_0802E932:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802E900

