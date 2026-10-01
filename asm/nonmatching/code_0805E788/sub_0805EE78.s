	thumb_func_start sub_0805EE78
sub_0805EE78: @ 0x0805EE78
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r4, r1, #0
	add r7, r2, #0
	add r5, r3, #0
	add r6, r6, r5
	cmp r5, #0
	ble _0805EEB6
_0805EE88:
	add r0, r4, #0
	mov r1, #0xA
	bl __modsi3
	add r0, #0x30
	sub r6, #1
	lsl r1, r6, #5
	ldr r2, _0805EED0 @ =0x0201CFB8
	add r1, r1, r2
	add r2, r7, #0
	mov r3, #9
	bl sub_08072778
	add r0, r4, #0
	mov r1, #0xA
	bl __divsi3
	add r4, r0, #0
	cmp r4, #0
	beq _0805EEC8
	sub r5, #1
	cmp r5, #0
	bgt _0805EE88
_0805EEB6:
	ldr r0, _0805EED4 @ =0x0201CFB0
	ldr r1, _0805EED8 @ =0x00000808
	add r0, r0, r1
	mov r1, #1
	ldrb r2, [r0]
	orr r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
_0805EEC8:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805EED0: .4byte 0x0201CFB8
_0805EED4: .4byte 0x0201CFB0
_0805EED8: .4byte 0x00000808
	thumb_func_end sub_0805EE78

