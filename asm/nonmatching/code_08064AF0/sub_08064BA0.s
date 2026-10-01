	thumb_func_start sub_08064BA0
sub_08064BA0: @ 0x08064BA0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r4, _08064BD8 @ =0x02020310
	ldrh r0, [r4, #0x28]
	bl sub_08064AF0
	mov r0, #1
	ldrb r1, [r4, #0x18]
	and r0, r1
	cmp r0, #0
	beq _08064BDC
	mov r0, #0x80
	lsl r0, r0, #0x13
	ldrh r1, [r0]
	mov r3, #0x90
	lsl r3, r3, #4
	add r2, r3, #0
	orr r1, r2
	strh r1, [r0]
	bl sub_08064698
_08064BD2:
	mov r0, #0
	b _08064DD0
	.align 2, 0
_08064BD8: .4byte 0x02020310
_08064BDC:
	ldrh r0, [r4, #0x1A]
	add r2, r0, #0
	ldrh r1, [r4, #0x1C]
	cmp r2, r1
	beq _08064BFA
	cmp r2, r1
	bls _08064BEE
	sub r0, #1
	b _08064BF0
_08064BEE:
	add r0, #1
_08064BF0:
	strh r0, [r4, #0x1A]
	ldr r0, _08064C64 @ =0x02020310
	ldrh r0, [r0, #0x1A]
	bl sub_08064908
_08064BFA:
	ldr r7, _08064C64 @ =0x02020310
	ldrh r3, [r7, #0x28]
	mov r8, r3
	cmp r3, #0
	beq _08064C80
	ldr r0, [r7, #0x20]
	ldr r4, [r7, #0x24]
	sub r2, r0, r4
	ldr r1, _08064C68 @ =0x080865CC
	sub r3, #1
	strh r3, [r7, #0x28]
	lsl r0, r3, #0x10
	lsr r0, r0, #0xF
	add r0, r0, r1
	ldrh r0, [r0]
	mul r2, r0
	add r0, r2, #0
	cmp r2, #0
	bge _08064C24
	ldr r1, _08064C6C @ =0x00000FFF
	add r0, r2, r1
_08064C24:
	asr r2, r0, #0xC
	add r2, r2, r4
	ldr r0, _08064C70 @ =0x03000040
	ldr r1, _08064C74 @ =0x0000442A
	add r5, r0, r1
	strh r2, [r5]
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldrh r2, [r1]
	ldr r0, _08064C78 @ =0x0000FEFF
	and r0, r2
	strh r0, [r1]
	ldrh r2, [r1]
	ldr r0, _08064C7C @ =0x0000F7FF
	and r0, r2
	strh r0, [r1]
	lsl r0, r3, #0x10
	lsr r4, r0, #0x10
	cmp r4, #0
	bne _08064BD2
	ldr r0, [r7, #0x14]
	str r0, [r7, #0x10]
	bl sub_080649D8
	mov r0, #8
	strh r0, [r7, #0x1C]
	strh r4, [r7, #0x2A]
	str r4, [r7, #0x20]
	str r4, [r7, #0x24]
	strh r4, [r5]
	b _08064BD2
	.align 2, 0
_08064C64: .4byte 0x02020310
_08064C68: .4byte gUnk_080865CC
_08064C6C: .4byte 0x00000FFF
_08064C70: .4byte 0x03000040
_08064C74: .4byte 0x0000442A
_08064C78: .4byte 0x0000FEFF
_08064C7C: .4byte 0x0000F7FF
_08064C80:
	ldr r2, _08064DE0 @ =0x03000040
	mov r9, r2
	mov r0, #0x20
	ldrh r3, [r2, #6]
	and r0, r3
	cmp r0, #0
	beq _08064D1C
	mov r0, #0
	bl sub_08077AEC
	add r5, r7, #0
	add r5, #0x6C
	ldrh r1, [r5]
	ldr r4, [r7, #0x10]
	add r0, r4, r1
	sub r0, #1
	bl __modsi3
	str r0, [r7, #0x14]
	mov r0, #0x10
	strh r0, [r7, #0x1C]
	mov r0, #8
	strh r0, [r7, #0x28]
	mov r0, #1
	strh r0, [r7, #0x2A]
	ldr r0, [r7, #0x24]
	sub r0, #0x50
	str r0, [r7, #0x20]
	ldr r6, _08064DE4 @ =0x080865DC
	ldrh r1, [r5]
	add r4, r4, r1
	sub r4, #1
	add r0, r4, #0
	bl __modsi3
	lsl r0, r0, #1
	add r1, r7, #0
	add r1, #0x2C
	add r0, r0, r1
	ldrh r2, [r0]
	lsl r1, r2, #3
	add r1, r1, r2
	lsl r1, r1, #3
	add r1, r1, r6
	ldrh r1, [r1]
	mov r0, #3
	bl sub_08064928
	mov r0, sp
	mov r3, r8
	strh r3, [r0]
	ldr r1, _08064DE8 @ =0x040000D4
	str r0, [r1]
	ldr r0, _08064DEC @ =0x0000141C
	add r0, r9
	str r0, [r1, #4]
	ldr r0, _08064DF0 @ =0x81000400
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _08064D08
_08064D00:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _08064D00
_08064D08:
	mov r0, #2
	mov r1, #0x78
	mov r2, #3
	bl sub_08064984
	ldr r1, _08064DF4 @ =0x02020310
	mov r0, #1
	ldrb r2, [r1, #0x18]
	orr r0, r2
	strb r0, [r1, #0x18]
_08064D1C:
	ldr r3, _08064DE0 @ =0x03000040
	mov sl, r3
	mov r7, #0x10
	add r0, r7, #0
	ldrh r1, [r3, #6]
	and r0, r1
	cmp r0, #0
	beq _08064DBA
	mov r0, #0
	bl sub_08077AEC
	ldr r4, _08064DF4 @ =0x02020310
	ldr r5, [r4, #0x10]
	add r0, r5, #1
	add r6, r4, #0
	add r6, #0x6C
	ldrh r1, [r6]
	bl __modsi3
	str r0, [r4, #0x14]
	mov r2, #0
	mov r9, r2
	strh r7, [r4, #0x1C]
	mov r0, #8
	strh r0, [r4, #0x28]
	mov r0, #2
	strh r0, [r4, #0x2A]
	ldr r0, [r4, #0x24]
	add r0, #0x50
	str r0, [r4, #0x20]
	ldr r3, _08064DE4 @ =0x080865DC
	mov r8, r3
	add r5, #3
	ldrh r1, [r6]
	add r0, r5, #0
	bl __modsi3
	lsl r0, r0, #1
	add r4, #0x2C
	add r0, r0, r4
	ldrh r2, [r0]
	lsl r1, r2, #3
	add r1, r1, r2
	lsl r1, r1, #3
	add r1, r8
	ldrh r1, [r1]
	mov r0, #3
	bl sub_08064928
	mov r0, sp
	mov r3, r9
	strh r3, [r0]
	ldr r1, _08064DE8 @ =0x040000D4
	str r0, [r1]
	ldr r0, _08064DEC @ =0x0000141C
	add r0, sl
	str r0, [r1, #4]
	ldr r0, _08064DF0 @ =0x81000400
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _08064DA6
_08064D9E:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _08064D9E
_08064DA6:
	mov r0, #2
	mov r1, #0x60
	mov r2, #3
	bl sub_08064984
	ldr r1, _08064DF4 @ =0x02020310
	mov r0, #1
	ldrb r2, [r1, #0x18]
	orr r0, r2
	strb r0, [r1, #0x18]
_08064DBA:
	ldr r1, _08064DE0 @ =0x03000040
	mov r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08064DC8
	b _08064BD2
_08064DC8:
	mov r0, #1
	bl sub_08077AEC
	mov r0, #1
_08064DD0:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08064DE0: .4byte 0x03000040
_08064DE4: .4byte gUnk_080865DC
_08064DE8: .4byte 0x040000D4
_08064DEC: .4byte 0x0000141C
_08064DF0: .4byte 0x81000400
_08064DF4: .4byte 0x02020310
	thumb_func_end sub_08064BA0

