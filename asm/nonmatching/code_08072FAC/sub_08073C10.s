	thumb_func_start sub_08073C10
sub_08073C10: @ 0x08073C10
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x40
	str r0, [sp, #0x30]
	str r1, [sp, #0x34]
	mov r0, #1
	mov r8, r0
	mov r6, #0
	mov r1, #0
	str r1, [sp, #0x3C]
	mov r0, sp
	bl sub_080740BC
	lsl r0, r0, #0x10
	mov r1, #0xF0
	lsl r1, r1, #0xC
	and r1, r0
	lsr r1, r1, #0x10
	cmp r1, #2
	bne _08073C40
	b _08073E48
_08073C40:
	cmp r1, #2
	bgt _08073C4C
	cmp r1, #1
	bne _08073C4A
	b _08073DCC
_08073C4A:
	b _08073ED0
_08073C4C:
	cmp r1, #3
	beq _08073C52
	b _08073ED0
_08073C52:
	mov r2, #0
	str r2, [sp, #0x38]
	ldr r2, _08073D9C @ =0x03005B60
_08073C58:
	lsl r0, r6, #4
	add r0, sp
	mov r8, r0
	mov r3, #0xF0
	lsl r3, r3, #8
	add r0, r3, #0
	mov r4, r8
	ldrh r1, [r4]
	and r1, r0
	lsl r0, r6, #1
	mov sl, r0
	mov r3, #0x80
	lsl r3, r3, #5
	cmp r1, r3
	beq _08073D66
	mov r0, #0xC0
	lsl r0, r0, #6
	cmp r1, r0
	bne _08073D66
	ldr r0, _08073DA0 @ =0x04000128
	ldrh r1, [r0]
	mov r0, #0x30
	and r0, r1
	lsr r0, r0, #4
	cmp r6, r0
	bne _08073C94
	mov r4, #0xA4
	lsl r4, r4, #4
	add r0, r2, r4
	strh r3, [r0]
_08073C94:
	ldr r7, _08073D9C @ =0x03005B60
	ldr r0, _08073DA4 @ =0x00000A1A
	add r5, r7, r0
	add r5, r6, r5
	ldrb r2, [r5]
	ldr r0, _08073DA8 @ =0x080876B4
	add r1, r6, #0
	bl sub_0801A7DC
	mov r0, r8
	add r0, #2
	ldrb r2, [r5]
	add r1, r2, #1
	strb r1, [r5]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r1, r2, #8
	add r1, r1, r2
	lsl r1, r1, #2
	lsl r2, r6, #8
	add r2, r2, r6
	lsl r2, r2, #1
	mov r4, #0x83
	lsl r4, r4, #2
	add r3, r7, r4
	add r2, r2, r3
	mov r9, r2
	add r1, r9
	ldr r2, _08073DAC @ =0x00000AF4
	add r4, r7, r2
	add r4, sl
	ldrh r3, [r4]
	lsl r2, r3, #3
	sub r2, r2, r3
	lsl r2, r2, #1
	add r1, r1, r2
	mov r2, #7
	bl CpuSet
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
	ldrb r5, [r5]
	lsl r1, r5, #1
	add r1, r6, r1
	ldr r4, _08073DB0 @ =0x00000A14
	add r3, r7, r4
	add r1, r1, r3
	mov r2, r8
	ldrh r0, [r2]
	strb r0, [r1]
	ldr r4, [sp, #0x30]
	cmp r6, r4
	bne _08073D52
	ldr r1, _08073DB4 @ =0x00000A1C
	add r0, r7, r1
	ldrh r1, [r0]
	sub r1, #1
	strh r1, [r0]
	lsl r1, r1, #0x10
	ldr r0, _08073DB8 @ =0xFFFF0000
	cmp r1, r0
	beq _08073D52
	ldr r2, _08073DBC @ =0x00000A18
	add r4, r7, r2
	add r4, r6, r4
	ldrb r2, [r4]
	lsl r0, r2, #1
	add r0, r6, r0
	add r0, r0, r3
	ldrb r0, [r0]
	str r0, [sp, #0x3C]
	ldr r0, _08073DC0 @ =0x080876D4
	add r1, r6, #0
	ldr r3, [sp, #0x3C]
	bl sub_0801A7DC
	ldrb r1, [r4]
	add r0, r1, #1
	strb r0, [r4]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	lsl r0, r1, #8
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r9
	ldr r3, [sp, #0x3C]
	lsr r2, r3, #1
	ldr r1, [sp, #0x34]
	bl CpuSet
	mov r0, #1
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
_08073D52:
	ldr r3, _08073D9C @ =0x03005B60
	ldr r4, _08073DA4 @ =0x00000A1A
	add r2, r3, r4
	add r1, r6, r2
	mov r0, #1
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	ldr r4, _08073DC4 @ =0xFFFFF5E6
	add r2, r2, r4
_08073D66:
	ldr r1, _08073D9C @ =0x03005B60
	ldr r3, _08073DC8 @ =0x00000AF8
	add r0, r1, r3
	add r0, sl
	mov r1, #0
	strh r1, [r0]
	ldr r4, _08073D9C @ =0x03005B60
	sub r3, #4
	add r0, r4, r3
	add r0, sl
	strh r1, [r0]
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	ldr r0, [sp, #0x38]
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #0x38]
	cmp r0, #1
	bhi _08073D92
	b _08073C58
_08073D92:
	bl sub_0801A7E8
	ldr r0, [sp, #0x3C]
	b _08073ED2
	.align 2, 0
_08073D9C: .4byte 0x03005B60
_08073DA0: .4byte 0x04000128
_08073DA4: .4byte 0x00000A1A
_08073DA8: .4byte gUnk_080876B4
_08073DAC: .4byte 0x00000AF4
_08073DB0: .4byte 0x00000A14
_08073DB4: .4byte 0x00000A1C
_08073DB8: .4byte 0xFFFF0000
_08073DBC: .4byte 0x00000A18
_08073DC0: .4byte gUnk_080876D4
_08073DC4: .4byte 0xFFFFF5E6
_08073DC8: .4byte 0x00000AF8
_08073DCC:
	mov r0, sp
	ldrh r4, [r0]
	mov r1, #0xF0
	lsl r1, r1, #8
	and r1, r4
	mov r0, #0x80
	lsl r0, r0, #5
	cmp r1, r0
	beq _08073ED0
	mov r0, #0xC0
	lsl r0, r0, #6
	cmp r1, r0
	bne _08073ED0
	ldr r0, _08073E30 @ =0x080876F8
	ldr r5, _08073E34 @ =0x03005B60
	ldr r1, _08073E38 @ =0x00000A1A
	add r6, r5, r1
	ldrb r2, [r6]
	ldr r3, _08073E3C @ =0x000001FF
	and r3, r4
	mov r1, #0
	bl sub_0801A7DC
	bl sub_0801A7E8
	ldrb r2, [r6]
	lsl r1, r2, #1
	ldr r3, _08073E40 @ =0x00000A14
	add r0, r5, r3
	add r1, r1, r0
	mov r0, sp
	ldrh r0, [r0]
	strb r0, [r1]
	mov r0, sp
	add r0, #2
	ldrb r2, [r6]
	add r1, r2, #1
	strb r1, [r6]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r1, r2, #8
	add r1, r1, r2
	lsl r1, r1, #2
	mov r4, #0x83
	lsl r4, r4, #2
	add r2, r5, r4
	add r1, r1, r2
	ldr r2, _08073E44 @ =0x00000AF4
	b _08073EA6
	.align 2, 0
_08073E30: .4byte gUnk_080876F8
_08073E34: .4byte 0x03005B60
_08073E38: .4byte 0x00000A1A
_08073E3C: .4byte 0x000001FF
_08073E40: .4byte 0x00000A14
_08073E44: .4byte 0x00000AF4
_08073E48:
	add r7, sp, #0x10
	ldrh r4, [r7]
	mov r1, #0xF0
	lsl r1, r1, #8
	and r1, r4
	mov r0, #0x80
	lsl r0, r0, #5
	cmp r1, r0
	beq _08073ED0
	mov r0, #0xC0
	lsl r0, r0, #6
	cmp r1, r0
	bne _08073ED0
	ldr r0, _08073EE4 @ =0x080876F8
	ldr r5, _08073EE8 @ =0x03005B60
	ldr r3, _08073EEC @ =0x00000A1B
	add r6, r5, r3
	ldrb r2, [r6]
	ldr r3, _08073EF0 @ =0x000001FF
	and r3, r4
	mov r1, #1
	bl sub_0801A7DC
	bl sub_0801A7E8
	ldrb r4, [r6]
	lsl r0, r4, #1
	add r0, #1
	ldr r2, _08073EF4 @ =0x00000A14
	add r1, r5, r2
	add r0, r0, r1
	ldrh r1, [r7]
	strb r1, [r0]
	mov r0, sp
	add r0, #0x12
	ldrb r2, [r6]
	add r1, r2, #1
	strb r1, [r6]
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r1, r2, #8
	add r1, r1, r2
	lsl r1, r1, #2
	ldr r3, _08073EF8 @ =0x0000040E
	add r2, r5, r3
	add r1, r1, r2
	ldr r2, _08073EFC @ =0x00000AF6
_08073EA6:
	add r4, r5, r2
	ldrh r3, [r4]
	lsl r2, r3, #3
	sub r2, r2, r3
	lsl r2, r2, #1
	add r1, r1, r2
	mov r2, #7
	bl CpuSet
	mov r0, r8
	ldrb r1, [r6]
	and r0, r1
	strb r0, [r6]
	ldrh r0, [r4]
	add r0, #1
	strh r0, [r4]
	ldr r2, _08073F00 @ =0x00000A1C
	add r5, r5, r2
	ldrh r0, [r5]
	add r0, #1
	strh r0, [r5]
_08073ED0:
	mov r0, #0
_08073ED2:
	add sp, #0x40
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08073EE4: .4byte gUnk_080876F8
_08073EE8: .4byte 0x03005B60
_08073EEC: .4byte 0x00000A1B
_08073EF0: .4byte 0x000001FF
_08073EF4: .4byte 0x00000A14
_08073EF8: .4byte 0x0000040E
_08073EFC: .4byte 0x00000AF6
_08073F00: .4byte 0x00000A1C
	thumb_func_end sub_08073C10

