	thumb_func_start sub_08057C94
sub_08057C94: @ 0x08057C94
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	ldr r3, _08057CF4 @ =0x02015F00
	mov r1, #5
	neg r1, r1
	add r0, r1, #0
	ldrb r2, [r3, #0xC]
	and r0, r2
	mov r2, #3
	neg r2, r2
	and r0, r2
	strb r0, [r3, #0xC]
	mov r0, #0
	strh r0, [r3, #0x10]
	ldrb r0, [r3, #0xD]
	and r1, r0
	mov r0, #9
	neg r0, r0
	and r1, r0
	strb r1, [r3, #0xD]
	mov r1, #0
	str r1, [sp, #0x14]
	mov r5, #0
	mov r4, sp
_08057CCC:
	mov r0, #1
	add r1, r5, #0
	mov r2, #1
	bl sub_0804A528
	cmp r0, #0
	beq _08057CE4
	strh r5, [r4]
	add r4, #2
	ldr r2, [sp, #0x14]
	add r2, #1
	str r2, [sp, #0x14]
_08057CE4:
	add r5, #1
	cmp r5, #4
	ble _08057CCC
	ldr r0, [sp, #0x14]
	cmp r0, #0
	bne _08057D14
	b _08057DF2
	.align 2, 0
_08057CF4: .4byte 0x02015F00
_08057CF8:
	ldr r4, _08057D10 @ =0x02015F0C
	add r0, r4, #0
	add r1, r7, #0
	mov r2, #8
	bl sub_08075294
	mov r0, #4
	ldrb r1, [r4]
	orr r0, r1
	strb r0, [r4]
	mov r0, #1
	b _08057DF4
_08057D10: .4byte 0x02015F0C
_08057D14:
	ldr r2, [sp, #0x14]
	sub r2, #1
	str r2, [sp, #0x1C]
_08057D1A:
	mov r0, #1
	str r0, [sp, #0x18]
	mov r5, #0
	ldr r1, [sp, #0x1C]
	cmp r5, r1
	bge _08057DBE
	mov r2, #0xBA
	lsl r2, r2, #2
	mov sl, r2
_08057D2C:
	lsl r6, r5, #1
	mov r1, sp
	add r0, r1, r6
	ldrh r0, [r0]
	mov r8, r0
	add r5, #1
	lsl r0, r5, #1
	add r0, sp
	ldrh r7, [r0]
	mov r0, #0x94
	ldr r1, _08057D84 @ =0x0201A070
	mul r0, r7
	add r0, r0, r1
	ldr r4, [r0]
	lsl r4, r4, #0x14
	lsr r4, r4, #0x14
	mov r0, #1
	mov r1, r8
	bl sub_0800C894
	mov r9, r0
	mov r0, #1
	add r1, r7, #0
	bl sub_0800C894
	mov ip, r0
	mov r3, #1
	ldr r2, _08057D88 @ =0x000007FF
	add r0, r2, #0
	and r4, r0
	lsl r4, r4, #1
	ldr r0, _08057D8C @ =0x08622AB4
	add r4, r4, r0
	ldrh r2, [r4]
	add r1, r5, #0
	cmp r2, sl
	beq _08057D9A
	cmp r2, sl
	bgt _08057D94
	ldr r0, _08057D90 @ =0x00000226
	cmp r2, r0
	beq _08057D9A
	b _08057D9C
	.align 2, 0
_08057D84: .4byte 0x0201A070
_08057D88: .4byte 0x000007FF
_08057D8C: .4byte gUnk_08622AB4
_08057D90: .4byte 0x00000226
_08057D94:
	ldr r0, _08057E04 @ =0x000002F9
	cmp r2, r0
	bne _08057D9C
_08057D9A:
	mov r3, #0
_08057D9C:
	cmp r9, ip
	ble _08057DB6
	cmp r3, #0
	beq _08057DB6
	mov r2, sp
	add r0, r2, r6
	strh r7, [r0]
	lsl r0, r1, #1
	add r0, sp
	mov r2, r8
	strh r2, [r0]
	mov r0, #0
	str r0, [sp, #0x18]
_08057DB6:
	add r5, r1, #0
	ldr r1, [sp, #0x1C]
	cmp r5, r1
	blt _08057D2C
_08057DBE:
	ldr r2, [sp, #0x18]
	cmp r2, #0
	beq _08057D1A
	mov r5, #0
	ldr r0, [sp, #0x14]
	cmp r5, r0
	bge _08057DF2
	add r7, sp, #0xC
	mov r6, sp
_08057DD0:
	ldrh r4, [r6]
	mov r0, #1
	add r1, r4, #0
	bl sub_0800C894
	add r0, r4, #0
	add r1, r7, #0
	bl sub_08057A80
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08057CF8
	add r6, #2
	add r5, #1
	ldr r1, [sp, #0x14]
	cmp r5, r1
	blt _08057DD0
_08057DF2:
	mov r0, #0
_08057DF4:
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08057E04: .4byte 0x000002F9
	thumb_func_end sub_08057C94

