	thumb_func_start sub_0803EE6C
sub_0803EE6C: @ 0x0803EE6C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #4
	add r3, r0, #0
	mov r2, #1
	ldrb r0, [r3, #2]
	and r2, r0
	cmp r2, #0
	beq _0803EEEC
	mov r7, #0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r3, #0xA]
	and r0, r1
	strb r0, [r3, #0xA]
	mov r5, #0
	mov r2, #1
	mov r9, r2
	ldr r4, _0803EEE4 @ =0x00000D64
	mov r8, r4
_0803EE98:
	mov r4, #5
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r6, r8
	mul r6, r0
_0803EEA4:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0803EEE8 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803EED4
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0803EED4
	add r0, r3, #0
	add r1, r5, #0
	add r2, r4, #0
	str r3, [sp, #0]
	bl sub_0803DDAC
	add r7, #1
	ldr r3, [sp, #0]
	cmp r7, #2
	beq _0803EEE0
_0803EED4:
	add r4, #1
	cmp r4, #9
	ble _0803EEA4
	add r5, #1
	cmp r5, #1
	ble _0803EE98
_0803EEE0:
	mov r0, #1
	b _0803F00E
_0803EEE4: .4byte 0x00000D64
_0803EEE8: .4byte 0x0201930C
_0803EEEC:
	ldr r0, _0803EF04 @ =0x02017A40
	ldr r5, _0803EF08 @ =0x000003E5
	add r4, r0, r5
	ldrb r0, [r4]
	cmp r0, #1
	beq _0803EF4C
	cmp r0, #1
	bgt _0803EF0C
	cmp r0, #0
	beq _0803EF16
	b _0803F00C
	.align 2, 0
_0803EF04: .4byte 0x02017A40
_0803EF08: .4byte 0x000003E5
_0803EF0C:
	cmp r0, #2
	beq _0803EFA4
	cmp r0, #3
	beq _0803EFB8
	b _0803F00C
_0803EF16:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r3, #0xA]
	and r0, r2
	strb r0, [r3, #0xA]
	add r0, r3, #0
	bl sub_0802D800
	cmp r0, #0
	beq _0803EEE0
	ldr r0, _0803EF40 @ =0x00000206
	ldr r1, _0803EF44 @ =0x00000712
	ldr r3, _0803EF48 @ =0x080840A4
_0803EF30:
	mov r2, #0xB
	bl sub_080602A4
_0803EF36:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0803F00C
	.align 2, 0
_0803EF40: .4byte 0x00000206
_0803EF44: .4byte 0x00000712
_0803EF48: .4byte gUnk_080840A4
_0803EF4C:
	ldr r1, _0803EF5C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803EF60
_0803EF58:
	strb r2, [r4]
	b _0803F00C
_0803EF5C: .4byte 0x03000040
_0803EF60:
	ldr r0, _0803EF98 @ =0x00020002
	str r3, [sp, #0]
	bl sub_08052F38
	ldr r3, [sp, #0]
	cmp r0, #0
	beq _0803F00C
	ldr r0, _0803EF9C @ =0x0201CFB0
	ldr r5, _0803EFA0 @ =0x00000824
	add r1, r0, r5
	ldr r1, [r1]
	add r5, #4
	add r2, r0, r5
	add r5, #4
	add r0, r0, r5
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r3, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803EF36
	mov r0, #3
	bl sub_08077AEC
	b _0803F00C
_0803EF98: .4byte 0x00020002
_0803EF9C: .4byte 0x0201CFB0
_0803EFA0: .4byte 0x00000824
_0803EFA4:
	ldr r0, _0803EFAC @ =0x00000206
	ldr r1, _0803EFB0 @ =0x00000712
	ldr r3, _0803EFB4 @ =0x080840D8
	b _0803EF30
_0803EFAC: .4byte 0x00000206
_0803EFB0: .4byte 0x00000712
_0803EFB4: .4byte gUnk_080840D8
_0803EFB8:
	ldr r1, _0803F01C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0803EF58
	ldr r0, _0803F020 @ =0x00020002
	str r3, [sp, #0]
	bl sub_08052F38
	ldr r3, [sp, #0]
	cmp r0, #0
	beq _0803F00C
	ldr r0, _0803F024 @ =0x0201CFB0
	ldr r1, _0803F028 @ =0x00000824
	add r2, r0, r1
	ldr r4, _0803F02C @ =0x00000828
	add r1, r0, r4
	ldr r5, _0803F030 @ =0x0000082C
	add r0, r0, r5
	ldr r1, [r1]
	ldr r0, [r0]
	add r4, r1, r0
	ldr r1, [r2]
	lsl r0, r4, #0x18
	lsr r0, r0, #0x10
	ldrb r2, [r2]
	orr r0, r2
	ldrh r2, [r3, #0xC]
	cmp r2, r0
	beq _0803F006
	add r0, r3, #0
	add r2, r4, #0
	bl sub_0803DDAC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803F006
	b _0803EEE0
_0803F006:
	mov r0, #3
	bl sub_08077AEC
_0803F00C:
	mov r0, #0
_0803F00E:
	add sp, #4
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0803F01C: .4byte 0x03000040
_0803F020: .4byte 0x00020002
_0803F024: .4byte 0x0201CFB0
_0803F028: .4byte 0x00000824
_0803F02C: .4byte 0x00000828
_0803F030: .4byte 0x0000082C
	thumb_func_end sub_0803EE6C

