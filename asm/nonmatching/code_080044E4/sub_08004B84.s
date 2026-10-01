	thumb_func_start sub_08004B84
sub_08004B84: @ 0x08004B84
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _08004BA4 @ =0x03000040
	ldr r1, _08004BA8 @ =0x00004858
	add r4, r0, r1
	ldrb r1, [r4]
	cmp r1, #1
	beq _08004C50
	cmp r1, #1
	bgt _08004BAC
	cmp r1, #0
	beq _08004BB2
	b _08004C90
_08004BA4: .4byte 0x03000040
_08004BA8: .4byte 0x00004858
_08004BAC:
	cmp r1, #2
	beq _08004C6E
	b _08004C90
_08004BB2:
	bl sub_08073498
	mov r0, #0x20
	mov r1, #3
	bl sub_08074B08
	ldr r0, _08004C38 @ =0x080813F0
	bl sub_080753CC
	lsl r1, r0, #3
	add r1, r1, r0
	mov r0, #0xF0
	sub r0, r0, r1
	lsr r1, r0, #0x1F
	add r0, r0, r1
	asr r0, r0, #1
	mov sl, r0
	mov r6, #1
_08004BD6:
	mov r7, #0
	sub r3, r6, #1
	mov r9, r3
	mov r0, sl
	add r5, r0, r6
_08004BE0:
	mov r4, #0
	mov r8, r5
_08004BE4:
	add r1, r4, r6
	ldr r2, _08004C3C @ =0x00001008
	cmp r6, #1
	bne _08004BEE
	add r2, #7
_08004BEE:
	mov r0, r8
	ldr r3, _08004C38 @ =0x080813F0
	bl sub_0807501C
	add r4, #1
	cmp r4, #0
	ble _08004BE4
	add r5, #1
	add r7, #1
	cmp r7, #1
	ble _08004BE0
	mov r6, r9
	cmp r6, #0
	bge _08004BD6
	ldr r0, _08004C40 @ =0x06004400
	mov r1, #0
	bl sub_08075114
	ldr r1, _08004C44 @ =0x03000040
	mov r7, #0x5F
	mov r2, #0x7F
	ldr r3, _08004C48 @ =0x00000F1A
	add r0, r1, r3
_08004C1C:
	strh r2, [r0]
	sub r2, #1
	sub r0, #2
	sub r7, #1
	cmp r7, #0
	bge _08004C1C
	ldr r0, _08004C4C @ =0x00004858
	add r1, r1, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_08004C32:
	mov r0, #0
	b _08004CAA
	.align 2, 0
_08004C38: .4byte gUnk_080813F0
_08004C3C: .4byte 0x00001008
_08004C40: .4byte 0x06004400
_08004C44: .4byte 0x03000040
_08004C48: .4byte 0x00000F1A
_08004C4C: .4byte 0x00004858
_08004C50:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0x80
	lsl r3, r3, #2
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r0, #1
	bl sub_08075BD0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08004C32
	b _08004C84
_08004C6E:
	ldr r1, _08004C8C @ =0x00004859
	add r2, r0, r1
	ldrb r0, [r2]
	add r1, r0, #1
	strb r1, [r2]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0x77
	bls _08004C32
	mov r0, #0
	strb r0, [r2]
_08004C84:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08004C32
_08004C8C: .4byte 0x00004859
_08004C90:
	mov r0, #1
	bl sub_08075B58
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08004C32
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08004CB8 @ =0x0000FDFF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
_08004CAA:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08004CB8: .4byte 0x0000FDFF
	thumb_func_end sub_08004B84

