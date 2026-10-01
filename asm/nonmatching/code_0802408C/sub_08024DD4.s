	thumb_func_start sub_08024DD4
sub_08024DD4: @ 0x08024DD4
	push {r4, r5, lr}
	add r5, r0, #0
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	mov r1, #0
	cmp r1, r4
	bcs _08024E1C
_08024DE2:
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r2, r0, r5
	ldrb r0, [r2, #2]
	cmp r0, #1
	bne _08024E12
	ldrb r0, [r2]
	add r3, r0, #0
	cmp r3, #0
	bne _08024E0E
	mov r0, #3
	strb r0, [r2]
	ldrb r0, [r2, #1]
	add r0, #1
	strb r0, [r2, #1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #8
	bne _08024E12
	strb r3, [r2, #1]
	b _08024E12
_08024E0E:
	sub r0, #1
	strb r0, [r2]
_08024E12:
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, r4
	bcc _08024DE2
_08024E1C:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08024DD4
	.align 2, 0

