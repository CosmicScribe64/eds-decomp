	thumb_func_start FlushOamBuffer
FlushOamBuffer: @ 0x08075C44
	push {r4, r5, r6, r7, lr}
	ldr r5, _08075CA0 @ =0x03000040
	ldr r0, _08075CA4 @ =0x0000040E
	add r1, r5, r0
	mov r0, #1
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08075C9A
	mov r0, #0xE0
	lsl r0, r0, #0x13
	ldr r1, _08075CA8 @ =0x00004430
	add r4, r5, r1
	mov r2, #0x80
	lsl r2, r2, #3
	add r1, r4, #0
	bl CopyDoubleWords
	ldr r2, _08075CAC @ =0x00004830
	add r0, r5, r2
	mov r1, #0
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
	mov r2, #0
	add r7, r4, #0
	mov r3, #0
	add r4, r5, #0
	ldr r6, _08075CB0 @ =0x00004435
	mov r5, #0xC
_08075C82:
	lsl r0, r2, #3
	add r0, r0, r7
	stmia r0!, {r3}
	str r3, [r0]
	add r1, r4, r6
	ldrb r0, [r1]
	orr r0, r5
	strb r0, [r1]
	add r4, #8
	add r2, #1
	cmp r2, #0x7F
	ble _08075C82
_08075C9A:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08075CA0: .4byte 0x03000040
_08075CA4: .4byte 0x0000040E
_08075CA8: .4byte 0x00004430
_08075CAC: .4byte 0x00004830
_08075CB0: .4byte 0x00004435
	thumb_func_end FlushOamBuffer

