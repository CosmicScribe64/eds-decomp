	thumb_func_start sub_08075CB4
sub_08075CB4: @ 0x08075CB4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	mov r4, #0
	ldr r0, _08075D50 @ =0x03000040
	mov ip, r0
	ldr r1, _08075D54 @ =0x081A7784
	mov r8, r1
	ldr r7, _08075D58 @ =0x0000040E
	add r7, ip
	ldr r6, _08075D5C @ =0x00004420
	add r6, ip
	ldr r5, _08075D60 @ =0x00004428
	add r5, ip
	ldr r3, _08075D64 @ =0x081A7764
_08075CD2:
	lsl r2, r4, #3
	ldrh r0, [r7]
	ldrh r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	beq _08075CE4
	ldr r1, [r3]
	ldrh r0, [r5]
	strh r0, [r1]
_08075CE4:
	mov r0, r8
	add r1, r2, r0
	ldrh r0, [r7]
	ldrh r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	beq _08075CF8
	ldr r1, [r1]
	ldrh r0, [r6]
	strh r0, [r1]
_08075CF8:
	add r6, #2
	add r5, #2
	add r3, #8
	add r4, #1
	cmp r4, #3
	ble _08075CD2
	ldr r1, _08075D58 @ =0x0000040E
	add r1, ip
	mov r0, #2
	ldrh r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08075D36
	ldr r6, _08075D68 @ =0x0000041C
	add r6, ip
	mov r5, #0xC0
	lsl r5, r5, #0x13
	mov r4, #7
_08075D1C:
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #0x80
	lsl r2, r2, #4
	bl sub_080752B0
	mov r0, #0x80
	lsl r0, r0, #4
	add r6, r6, r0
	add r5, r5, r0
	sub r4, #1
	cmp r4, #0
	bge _08075D1C
_08075D36:
	bl sub_08075C44
	bl sub_08075228
	bl sub_0807E554
	bl sub_08076F9C
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08075D50: .4byte 0x03000040
_08075D54: .4byte gUnk_081A7784
_08075D58: .4byte 0x0000040E
_08075D5C: .4byte 0x00004420
_08075D60: .4byte 0x00004428
_08075D64: .4byte gUnk_081A7764
_08075D68: .4byte 0x0000041C
	thumb_func_end sub_08075CB4

