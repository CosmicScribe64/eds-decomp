	thumb_func_start sub_08000C54
sub_08000C54: @ 0x08000C54
	push {r4, r5, r6, r7, lr}
	sub sp, #0x1C
	add r5, r0, #0
	ldr r6, [r5, #0x14]
	mov r0, #0x19
	ldsb r0, [r5, r0]
	cmp r0, #0
	bge _08000C66
	b _080011DE
_08000C66:
	add r4, r0, #0
	cmp r4, #2
	beq _08000C88
	cmp r4, #3
	bne _08000C72
	b _08000D9C
_08000C72:
	ldrb r0, [r6]
	cmp r0, #0
	bne _08000C7A
	b _080011C4
_08000C7A:
	sub r0, #9
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	bls _08000C86
	b _08000DD4
_08000C86:
	b _08000DC4
_08000C88:
	add r6, r5, #0
	add r6, #0xB8
	ldrb r1, [r6]
	add r7, r5, #0
	add r7, #0xB9
	add r0, r7, r1
	ldrb r0, [r0]
	cmp r0, #0
	beq _08000D78
	add r0, r1, #0
	add r0, #0xB9
	add r0, r5, r0
	ldrb r1, [r5, #0x1C]
	mov r2, #0x1E
	bl sub_08079F40
	cmp r0, #0
	bne _08000CB8
	ldr r0, _08000CF4 @ =0x08087B90
	ldrb r0, [r0]
	strb r0, [r5, #0x1C]
	ldrb r0, [r5, #0x1D]
	add r0, #1
	strb r0, [r5, #0x1D]
_08000CB8:
	strb r4, [r5, #0x18]
	ldr r1, _08000CF8 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #0
	beq _08000CFC
	add r2, sp, #0x14
	ldrb r0, [r6]
	add r1, r0, #1
	strb r1, [r6]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r0, r7, r0
	ldrb r0, [r0]
	strb r0, [r2]
	add r0, r1, #1
	strb r0, [r6]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	add r1, r7, r1
	ldrb r0, [r1]
	strb r0, [r2, #1]
	add r1, r2, #0
	mov r0, #0
	strb r0, [r1, #2]
	b _08000D12
	.align 2, 0
_08000CF4: .4byte gUnk_08087B90
_08000CF8: .4byte 0x02011C20
_08000CFC:
	add r2, sp, #0x14
	ldrb r0, [r6]
	add r1, r0, #1
	strb r1, [r6]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r0, r7, r0
	ldrb r0, [r0]
	strb r0, [r2]
	add r0, r2, #0
	strb r3, [r0, #1]
_08000D12:
	add r0, r5, #0
	add r0, #0x20
	ldrb r4, [r0]
	cmp r4, #1
	bgt _08000D5A
	cmp r4, #0
	blt _08000D5A
	ldrb r1, [r5, #0x1C]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #1
	add r1, r0, #0
	add r1, #0x1E
	mov r0, #0xD
	ldrb r2, [r5, #0x1D]
	mul r0, r2
	add r2, r0, #0
	add r2, #0x66
	ldr r3, _08000D70 @ =0x0600A000
	cmp r4, #0
	bne _08000D40
	mov r3, #0xC0
	lsl r3, r3, #0x13
_08000D40:
	ldrh r0, [r5, #0x24]
	str r0, [sp, #0]
	mov r0, #0xE
	str r0, [sp, #4]
	mov r0, #0xC
	str r0, [sp, #8]
	mov r0, #0xF0
	str r0, [sp, #0xC]
	mov r0, #8
	str r0, [sp, #0x10]
	add r0, sp, #0x14
	bl sub_08079E50
_08000D5A:
	ldr r1, _08000D74 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08000D68
	b _080011B0
_08000D68:
	ldrb r0, [r5, #0x1C]
	add r0, #2
	b _080011B4
	.align 2, 0
_08000D70: .4byte 0x0600A000
_08000D74: .4byte 0x02011C20
_08000D78:
	strb r0, [r5, #0x19]
	ldr r0, _08000D94 @ =0x02013DE0
	ldr r1, _08000D98 @ =0x0000137C
	add r0, r0, r1
	ldrb r0, [r0]
	and r4, r0
	cmp r4, #0
	beq _08000D8C
	mov r0, #1
	strb r0, [r5, #0x19]
_08000D8C:
	mov r0, #7
	strh r0, [r5, #0x24]
	b _080011DE
	.align 2, 0
_08000D94: .4byte 0x02013DE0
_08000D98: .4byte 0x0000137C
_08000D9C:
	mov r1, #1
	mov r0, #1
	strb r0, [r5, #0x19]
	add r0, r5, #0
	add r0, #0x20
	ldrb r2, [r0]
	eor r1, r2
	strb r1, [r0]
	ldr r1, _08000DC0 @ =0x08087B90
	ldrb r0, [r1]
	strb r0, [r5, #0x1C]
	ldrb r0, [r1, #1]
	strb r0, [r5, #0x1D]
	add r0, r5, #0
	bl sub_0800093C
	b _080011DE
	.align 2, 0
_08000DC0: .4byte gUnk_08087B90
_08000DC4:
	add r6, #1
	str r6, [r5, #0x14]
	ldrb r0, [r6]
	sub r0, #9
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	bls _08000DC4
_08000DD4:
	ldrb r0, [r6]
	cmp r0, #0x24
	beq _08000DDC
	b _0800107A
_08000DDC:
	add r6, #1
	ldrb r1, [r6]
	cmp r1, #0x24
	bne _08000DE6
	b _08001076
_08000DE6:
	add r0, r1, #0
	sub r0, #0x51
	cmp r0, #0x21
	bls _08000DF0
	b _08001076
_08000DF0:
	lsl r0, r0, #2
	ldr r1, _08000DFC @ =0x08000E00
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08000DFC: .4byte 0x08000E00
_08000E00:
	.4byte _08001048
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08000F64
	.4byte _08000EF0
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08001076
	.4byte _08000E88
	.4byte _08000FAC
	.4byte _08001076
	.4byte _08000E9C
	.4byte _08001076
	.4byte _08001076
	.4byte _08000EB4
	.4byte _08001076
	.4byte _08000EC8
	.4byte _08001024
	.4byte _08000F10
_08000E88:
	ldr r0, _08000E94 @ =0x02013DE0
	ldr r2, _08000E98 @ =0x000009A6
	add r0, r0, r2
	mov r1, #0
	b _08000EA4
	.align 2, 0
_08000E94: .4byte 0x02013DE0
_08000E98: .4byte 0x000009A6
_08000E9C:
	ldr r0, _08000EAC @ =0x02013DE0
	ldr r1, _08000EB0 @ =0x000009A6
	add r0, r0, r1
	mov r1, #1
_08000EA4:
	strb r1, [r0]
_08000EA6:
	add r6, #1
	b _08001076
	.align 2, 0
_08000EAC: .4byte 0x02013DE0
_08000EB0: .4byte 0x000009A6
_08000EB4:
	ldr r0, _08000EC4 @ =0x08087B90
	ldrb r0, [r0]
	strb r0, [r5, #0x1C]
	ldrb r0, [r5, #0x1D]
	add r0, #1
	strb r0, [r5, #0x1D]
	b _08000EA6
	.align 2, 0
_08000EC4: .4byte gUnk_08087B90
_08000EC8:
	add r1, r5, #0
	add r1, #0x20
	mov r0, #1
	ldrb r2, [r1]
	eor r0, r2
	strb r0, [r1]
	mov r0, #2
	strb r0, [r5, #0x18]
	ldr r1, _08000EEC @ =0x08087B90
	ldrb r0, [r1]
	strb r0, [r5, #0x1C]
	ldrb r0, [r1, #1]
	strb r0, [r5, #0x1D]
	add r0, r5, #0
	bl sub_0800093C
	b _08000EA6
	.align 2, 0
_08000EEC: .4byte gUnk_08087B90
_08000EF0:
	ldr r1, _08000F08 @ =0x02013DE0
	ldr r0, _08000F0C @ =0x0000137C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08000EA6
	mov r0, #0xFF
	strb r0, [r5, #0x19]
	b _08000EA6
	.align 2, 0
_08000F08: .4byte 0x02013DE0
_08000F0C: .4byte 0x0000137C
_08000F10:
	add r6, #1
	ldrb r1, [r6]
	add r0, r1, #0
	sub r0, #0x61
	cmp r0, #5
	bhi _08000F5C
	lsl r0, r0, #2
	ldr r1, _08000F28 @ =0x08000F2C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08000F28: .4byte 0x08000F2C
_08000F2C:
	.4byte _08000F44
	.4byte _08000F48
	.4byte _08000F4C
	.4byte _08000F50
	.4byte _08000F54
	.4byte _08000F58
_08000F44:
	mov r0, #0xA
	b _08000F60
_08000F48:
	mov r0, #0xB
	b _08000F60
_08000F4C:
	mov r0, #0xC
	b _08000F60
_08000F50:
	mov r0, #0xD
	b _08000F60
_08000F54:
	mov r0, #0xE
	b _08000F60
_08000F58:
	mov r0, #0xF
	b _08000F60
_08000F5C:
	add r0, r1, #0
	sub r0, #0x30
_08000F60:
	strh r0, [r5, #0x24]
	b _08000EA6
_08000F64:
	add r6, #1
	add r0, r6, #0
	bl sub_08079ED4
	ldr r1, _08000F98 @ =0x02013DE0
	ldr r2, _08000F9C @ =0x000012EB
	add r4, r1, r2
	strb r0, [r4]
	ldr r0, _08000FA0 @ =0x08080A20
	ldrb r1, [r4]
	bl sub_0801A7DC
	bl sub_0801A7E8
	ldrb r0, [r4]
	cmp r0, #0x27
	bls _08000F8A
	mov r0, #0
	strb r0, [r4]
_08000F8A:
	add r6, #2
	ldr r0, _08000FA4 @ =0x03000040
	ldr r1, _08000FA8 @ =0x00004859
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #4
	b _08001074
_08000F98: .4byte 0x02013DE0
_08000F9C: .4byte 0x000012EB
_08000FA0: .4byte gUnk_08080A20
_08000FA4: .4byte 0x03000040
_08000FA8: .4byte 0x00004859
_08000FAC:
	add r6, #1
	add r4, r5, #0
	add r4, #0xB9
	add r0, r6, #0
	mov r1, #4
	bl sub_08079F10
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	add r2, r1, #0
	ldr r0, _08000FCC @ =0x0000FFFF
	cmp r1, r0
	bne _08000FD0
	mov r0, #0
	b _08000FFE
	.align 2, 0
_08000FCC: .4byte 0x0000FFFF
_08000FD0:
	ldr r0, _08000FE4 @ =0x000007CF
	cmp r1, r0
	bhi _08000FEC
	add r0, #0x30
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _08000FE8 @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	b _08000FFE
_08000FE4: .4byte 0x000007CF
_08000FE8: .4byte gUnk_08623DF4
_08000FEC:
	ldr r1, _08001014 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _08001018 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0800101C @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_08000FFE:
	lsl r1, r0, #0x10
	lsr r1, r1, #0xA
	ldr r0, _08001020 @ =0x0822C720
	add r1, r1, r0
	add r0, r4, #0
	bl sub_080752D0
	add r6, #4
	mov r1, #0
	mov r0, #3
	b _0800106A
_08001014: .4byte 0xFFFFF830
_08001018: .4byte 0x000007FF
_0800101C: .4byte gUnk_08623DF4
_08001020: .4byte gUnk_0822C720
_08001024:
	add r6, #1
	add r4, r5, #0
	add r4, #0xB9
	add r0, r6, #0
	mov r1, #2
	bl sub_08079F10
	mov r1, #1
	bl sub_08000228
	add r1, r0, #0
	add r0, r4, #0
	bl sub_080752D0
	add r6, #2
	mov r1, #0
	mov r0, #5
	b _0800106A
_08001048:
	add r6, #1
	add r4, r5, #0
	add r4, #0xB9
	add r0, r6, #0
	mov r1, #2
	bl sub_08079F10
	mov r1, #0
	bl sub_08000228
	add r1, r0, #0
	add r0, r4, #0
	bl sub_080752D0
	add r6, #2
	mov r1, #0
	mov r0, #4
_0800106A:
	strh r0, [r5, #0x24]
	mov r0, #2
	strb r0, [r5, #0x19]
	add r0, r5, #0
	add r0, #0xB8
_08001074:
	strb r1, [r0]
_08001076:
	str r6, [r5, #0x14]
	b _080011DE
_0800107A:
	ldrb r1, [r5, #0x19]
	cmp r1, #1
	bne _08001084
	mov r0, #0
	strb r0, [r5, #0x18]
_08001084:
	ldrb r0, [r5, #0x18]
	sub r0, #1
	strb r0, [r5, #0x18]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #0xFF
	beq _08001094
	b _080011DE
_08001094:
	ldrb r1, [r5, #0x1C]
	add r0, r6, #0
	mov r2, #0x1E
	bl sub_08079F40
	cmp r0, #0
	bne _080010EE
	ldr r0, _080010D8 @ =0x08087B90
	ldrb r0, [r0]
	strb r0, [r5, #0x1C]
	ldrb r0, [r5, #0x1D]
	add r0, #1
	strb r0, [r5, #0x1D]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #3
	bls _080010EE
	ldr r0, _080010DC @ =0x02013DE0
	ldr r2, _080010E0 @ =0x0000137C
	add r4, r0, r2
	mov r0, #2
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _080010E8
	ldr r0, _080010E4 @ =0x08080A30
	ldrb r1, [r5, #0x1D]
	bl sub_0801A7DC
	mov r0, #1
	ldrb r2, [r4]
	orr r0, r2
	strb r0, [r4]
	b _080011DE
_080010D8: .4byte gUnk_08087B90
_080010DC: .4byte 0x02013DE0
_080010E0: .4byte 0x0000137C
_080010E4: .4byte gUnk_08080A30
_080010E8:
	mov r0, #0xFD
	strb r0, [r5, #0x19]
	b _080011DE
_080010EE:
	ldr r0, _08001130 @ =0x08087B90
	ldrb r1, [r5, #0x1C]
	ldrb r0, [r0]
	cmp r1, r0
	bne _0800110A
	ldrb r0, [r6]
	cmp r0, #0x20
	beq _08001102
	cmp r0, #0x2E
	bne _0800110A
_08001102:
	add r6, #1
	ldr r0, [r5, #0x14]
	add r0, #1
	str r0, [r5, #0x14]
_0800110A:
	ldr r1, _08001134 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #0
	beq _08001138
	add r1, sp, #0x18
	ldrb r0, [r6]
	mov r2, #0
	strb r0, [r1]
	ldrb r0, [r6, #1]
	strb r0, [r1, #1]
	strb r2, [r1, #2]
	ldr r0, [r5, #0x14]
	add r0, #2
	b _08001144
	.align 2, 0
_08001130: .4byte gUnk_08087B90
_08001134: .4byte 0x02011C20
_08001138:
	add r1, sp, #0x18
	ldrb r0, [r6]
	strb r0, [r1]
	strb r2, [r1, #1]
	ldr r0, [r5, #0x14]
	add r0, #1
_08001144:
	str r0, [r5, #0x14]
	add r6, r1, #0
	mov r0, #2
	strb r0, [r5, #0x18]
	add r0, r5, #0
	add r0, #0x20
	ldrb r4, [r0]
	cmp r4, #1
	bgt _08001194
	cmp r4, #0
	blt _08001194
	ldrb r2, [r5, #0x1C]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r1, r0, #0
	add r1, #0x1E
	mov r0, #0xD
	ldrb r2, [r5, #0x1D]
	mul r0, r2
	add r2, r0, #0
	add r2, #0x66
	ldr r3, _080011A8 @ =0x0600A000
	cmp r4, #0
	bne _0800117A
	mov r3, #0xC0
	lsl r3, r3, #0x13
_0800117A:
	ldrh r0, [r5, #0x24]
	str r0, [sp, #0]
	mov r0, #0xE
	str r0, [sp, #4]
	mov r0, #0xC
	str r0, [sp, #8]
	mov r0, #0xF0
	str r0, [sp, #0xC]
	mov r0, #8
	str r0, [sp, #0x10]
	add r0, r6, #0
	bl sub_08079E50
_08001194:
	ldr r1, _080011AC @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _080011B0
	ldrb r0, [r5, #0x1C]
	add r0, #2
	b _080011B4
	.align 2, 0
_080011A8: .4byte 0x0600A000
_080011AC: .4byte 0x02011C20
_080011B0:
	ldrb r0, [r5, #0x1C]
	add r0, #1
_080011B4:
	strb r0, [r5, #0x1C]
	ldr r0, _080011C0 @ =0x02014EB4
	bl sub_0800098C
	b _080011DE
	.align 2, 0
_080011C0: .4byte 0x02014EB4
_080011C4:
	mov r0, #0xFE
	strb r0, [r5, #0x19]
	ldr r0, _080011E8 @ =0x02013DE0
	ldr r1, _080011EC @ =0x0000137C
	add r2, r0, r1
	ldrb r1, [r2]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _080011DE
	mov r0, #1
	orr r0, r1
	strb r0, [r2]
_080011DE:
	add sp, #0x1C
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080011E8: .4byte 0x02013DE0
_080011EC: .4byte 0x0000137C
	thumb_func_end sub_08000C54

