	thumb_func_start sub_0805797C
sub_0805797C: @ 0x0805797C
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	add r7, r1, #0
	add r5, r2, #0
	mov r0, #1
	add r1, r4, #0
	bl sub_0800C894
	add r6, r0, #0
	mov r3, #0
	mov r0, #3
	neg r0, r0
	ldrb r1, [r5]
	and r0, r1
	mov r2, #9
	neg r2, r2
	and r0, r2
	mov r1, #7
	and r4, r1
	lsl r4, r4, #4
	mov r1, #0x71
	neg r1, r1
	and r0, r1
	orr r0, r4
	strb r0, [r5]
	mov r0, #7
	add r1, r7, #0
	and r1, r0
	lsl r1, r1, #7
	ldr r0, _08057A28 @ =0xFFFFFC7F
	ldrh r4, [r5]
	and r0, r4
	orr r0, r1
	strh r0, [r5]
	mov r0, #5
	neg r0, r0
	ldrb r1, [r5, #1]
	and r0, r1
	and r0, r2
	strb r0, [r5, #1]
	strh r3, [r5, #2]
	strh r3, [r5, #4]
	mov r0, #0x94
	add r1, r7, #0
	mul r1, r0
	ldr r0, _08057A2C @ =0x0201930C
	add r4, r1, r0
	mov r0, #1
	ldrb r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _08057A3E
	mov r0, #0
	add r1, r7, #0
	bl sub_0800C8A8
	add r3, r0, #0
	mov r0, #2
	ldrb r4, [r4, #6]
	and r0, r4
	cmp r0, #0
	bne _08057A16
	ldr r0, _08057A30 @ =0x03000040
	ldr r4, _08057A34 @ =0x00004870
	add r0, r0, r4
	ldrb r0, [r0]
	lsl r1, r0, #0x1A
	lsr r0, r1, #0x1B
	cmp r0, #0xA
	bhi _08057A16
	add r0, #5
	mul r3, r0
	add r0, r3, #0
	cmp r3, #0
	bge _08057A14
	add r0, #0xF
_08057A14:
	asr r3, r0, #4
_08057A16:
	cmp r3, r6
	bge _08057A38
	mov r0, #8
	ldrb r1, [r5]
	orr r1, r0
	strb r1, [r5]
	ldrb r1, [r5, #1]
	b _08057A74
	.align 2, 0
_08057A28: .4byte 0xFFFFFC7F
_08057A2C: .4byte 0x0201930C
_08057A30: .4byte 0x03000040
_08057A34: .4byte 0x00004870
_08057A38:
	cmp r3, r6
	ble _08057A78
	b _08057A66
_08057A3E:
	mov r0, #0
	add r1, r7, #0
	bl sub_0800C894
	add r3, r0, #0
	cmp r3, r6
	bge _08057A5A
	mov r0, #8
	ldrb r1, [r5]
	orr r1, r0
	strb r1, [r5]
	ldrb r4, [r5, #1]
	orr r0, r4
	b _08057A64
_08057A5A:
	cmp r3, r6
	ble _08057A6C
	mov r0, #4
	ldrb r1, [r5, #1]
	orr r0, r1
_08057A64:
	strb r0, [r5, #1]
_08057A66:
	sub r0, r6, r3
	strh r0, [r5, #4]
	b _08057A78
_08057A6C:
	mov r0, #4
	ldrb r4, [r5, #1]
	orr r0, r4
	mov r1, #8
_08057A74:
	orr r0, r1
	strb r0, [r5, #1]
_08057A78:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0805797C
	.align 2, 0

