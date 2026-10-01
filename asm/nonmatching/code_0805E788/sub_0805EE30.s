	thumb_func_start sub_0805EE30
sub_0805EE30: @ 0x0805EE30
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	add r6, r2, #0
	b _0805EE4E
_0805EE3A:
	ldrb r0, [r4]
	lsl r1, r5, #5
	ldr r2, _0805EE6C @ =0x0201CFB8
	add r1, r1, r2
	add r5, #1
	add r2, r6, #0
	mov r3, #9
	bl sub_08072778
	add r4, #1
_0805EE4E:
	ldrb r0, [r4]
	cmp r0, #0
	bne _0805EE3A
	ldr r0, _0805EE70 @ =0x0201CFB0
	ldr r1, _0805EE74 @ =0x00000808
	add r0, r0, r1
	mov r1, #1
	ldrb r2, [r0]
	orr r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0805EE6C: .4byte 0x0201CFB8
_0805EE70: .4byte 0x0201CFB0
_0805EE74: .4byte 0x00000808
	thumb_func_end sub_0805EE30

