	thumb_func_start sub_080722B0
sub_080722B0: @ 0x080722B0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	ldr r1, _08072320 @ =0x030049D0
	ldr r2, _08072324 @ =0x00000524
	add r0, r1, r2
	mov r2, #1
	ldrb r0, [r0]
	and r2, r0
	cmp r2, #0
	beq _080722D0
	add r0, r2, #0
_080722CC:
	cmp r0, #0
	bne _080722CC
_080722D0:
	ldr r0, _08072324 @ =0x00000524
	add r3, r1, r0
	ldrb r2, [r3]
	mov r0, #4
	and r0, r2
	cmp r0, #0
	beq _080723A4
	mov r0, #5
	neg r0, r0
	and r0, r2
	strb r0, [r3]
	mov r2, #0xA5
	lsl r2, r2, #3
	add r0, r1, r2
	ldr r3, _08072328 @ =0x0000052A
	add r4, r1, r3
	ldrh r0, [r0]
	ldrh r2, [r4]
	cmp r0, r2
	beq _080723A4
	ldrh r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #2
	add r3, #2
	add r7, r1, r3
	add r5, r0, r7
	ldrh r0, [r5]
	lsr r1, r0, #8
	mov r0, #0xF0
	and r1, r0
	cmp r1, #0x90
	beq _0807232C
	cmp r1, #0xA0
	beq _08072344
	add r0, r2, #1
	mov r1, #0x3F
	and r0, r1
	strh r0, [r4]
	b _080723A4
_08072320: .4byte 0x030049D0
_08072324: .4byte 0x00000524
_08072328: .4byte 0x0000052A
_0807232C:
	add r1, r5, #2
	mov r0, r8
	mov r2, #0xA
	bl sub_08075294
	ldrh r0, [r4]
	add r0, #1
	mov r1, #0x3F
	and r0, r1
	strh r0, [r4]
	ldrb r0, [r5]
	b _080723A6
_08072344:
	add r0, r2, #0
	bl sub_08072238
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080723A4
	add r6, r4, #0
	mov r9, r7
	mov r7, #0x3F
_08072356:
	ldrh r4, [r5]
	lsr r1, r4, #8
	mov r0, #0xF0
	and r1, r0
	cmp r1, #0xA0
	beq _08072380
	cmp r1, #0xB0
	bne _08072396
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	add r1, r5, #2
	mov r0, r8
	mov r2, #0xA
	bl sub_08075294
	ldrh r0, [r6]
	add r0, #1
	and r0, r7
	strh r0, [r6]
	add r0, r4, #0
	b _080723A6
_08072380:
	lsl r0, r4, #0x18
	lsr r0, r0, #0x17
	add r0, r8
	add r1, r5, #2
	mov r2, #0xA
	bl sub_08075294
	ldrh r0, [r6]
	add r0, #1
	and r0, r7
	strh r0, [r6]
_08072396:
	ldrh r1, [r6]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	mov r2, r9
	add r5, r0, r2
	b _08072356
_080723A4:
	mov r0, #0
_080723A6:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_080722B0
	.align 2, 0

