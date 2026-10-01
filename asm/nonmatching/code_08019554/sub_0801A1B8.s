	thumb_func_start sub_0801A1B8
sub_0801A1B8: @ 0x0801A1B8
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	mov r9, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r8, r1
	mov r6, #0
_0801A1CC:
	lsl r0, r6, #7
	add r3, r0, #0
	add r3, #0x40
	lsl r0, r6, #5
	add r0, #0x10
	mov r1, r8
	cmp r1, #0
	beq _0801A1EA
	mov r1, r9
	mul r1, r0
	add r0, r1, #0
	cmp r0, #0
	bge _0801A1E8
	add r1, #0xF
_0801A1E8:
	asr r0, r1, #4
_0801A1EA:
	lsl r4, r0, #0x10
	mov r0, #0xA0
	lsl r0, r0, #5
	add r2, r3, r0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r1, #0x80
	str r3, [sp, #0]
	bl sub_080761F0
	mov r5, #1
	add r7, r4, #0
	add r6, #1
	ldr r3, [sp, #0]
	add r4, r3, #4
_0801A20A:
	lsl r0, r5, #5
	orr r0, r7
	mov r1, #0x80
	lsl r1, r1, #3
	add r2, r4, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r1, #0x80
	bl sub_080761F0
	add r4, #4
	add r5, #1
	cmp r5, #7
	ble _0801A20A
	cmp r6, #3
	ble _0801A1CC
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0801A1B8

