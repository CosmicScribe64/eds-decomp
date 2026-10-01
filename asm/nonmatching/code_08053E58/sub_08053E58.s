	thumb_func_start sub_08053E58
sub_08053E58: @ 0x08053E58
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _08053EE4 @ =0x0201AE60
	add r1, r0, #0
	add r1, #0x21
	ldrb r1, [r1]
	ldrh r0, [r0, #0xE]
	sub r0, r1, r0
	add r0, #2
	lsl r0, r0, #3
	mov r9, r0
	mov r6, #0
	ldr r0, _08053EE8 @ =0x020192E0
	mov r1, #0x28
	mov r8, r1
	ldr r3, _08053EEC @ =0x00001B52
	add r7, r0, r3
	ldr r0, _08053EF0 @ =0x0300489E
	mov sl, r0
_08053E84:
	ldrh r2, [r7]
	mov r1, sl
	ldrh r1, [r1]
	lsr r0, r1, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08053EF4 @ =0x081A4424
	add r0, r0, r3
	ldrh r5, [r0]
	ldr r0, _08053EE4 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r6, r0
	beq _08053EA4
	mov r5, #0x80
	lsl r5, r5, #1
_08053EA4:
	mov r0, r9
	lsl r4, r0, #0x10
	mov r1, r8
	orr r4, r1
	add r0, r2, #0
	bl sub_08062140
	add r2, r0, #0
	mov r3, #0x80
	lsl r3, r3, #5
	add r2, r2, r3
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r5, #0x10
	add r0, r4, #0
	mov r1, #0x80
	bl sub_08076714
	mov r0, #0x20
	add r8, r0
	add r7, #2
	add r6, #1
	cmp r6, #4
	ble _08053E84
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08053EE4: .4byte 0x0201AE60
_08053EE8: .4byte 0x020192E0
_08053EEC: .4byte 0x00001B52
_08053EF0: .4byte 0x0300489E
_08053EF4: .4byte gUnk_081A4424
	thumb_func_end sub_08053E58

