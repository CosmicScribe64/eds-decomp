	thumb_func_start sub_08052CE8
sub_08052CE8: @ 0x08052CE8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x2C
	str r1, [sp, #0x14]
	str r2, [sp, #0x18]
	str r3, [sp, #0x1C]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0x10]
	ldr r2, [r1]
	str r2, [sp, #4]
	ldr r0, [sp, #0x18]
	ldr r1, [r0]
	str r1, [sp, #8]
	ldr r0, [r3]
	str r0, [sp, #0xC]
	str r2, [sp, #0x20]
	str r1, [sp, #0x24]
	str r0, [sp, #0x28]
	mov r0, #3
	ldr r1, [sp, #0x10]
	and r0, r1
	cmp r0, #0
	bne _08052D20
	b _08052F14
_08052D20:
	mov r3, #0
	mov sl, r3
	mov r0, #5
	mov r9, r0
_08052D28:
	ldr r0, [sp, #0x10]
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _08052DD8
	ldr r0, [sp, #8]
	cmp r0, #5
	beq _08052D78
	cmp r0, #5
	bgt _08052D42
	cmp r0, #0
	beq _08052DB0
	b _08052DD8
_08052D42:
	cmp r0, #0xA
	beq _08052DC4
	cmp r0, #0xB
	bne _08052DD8
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _08052D70
	ldr r1, _08052D64 @ =0x020192E4
	ldrb r0, [r1, #2]
	cmp r0, #0
	beq _08052D68
	mov r3, sl
	str r3, [sp, #4]
	ldrb r0, [r1, #2]
	sub r0, #1
	b _08052DD6
	.align 2, 0
_08052D64: .4byte 0x020192E4
_08052D68:
	str r0, [sp, #4]
	mov r1, r9
	str r1, [sp, #8]
	b _08052DD6
_08052D70:
	str r0, [sp, #4]
	mov r3, r9
	str r3, [sp, #8]
	b _08052DD6
_08052D78:
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _08052DD4
	ldr r2, _08052D94 @ =0x020192E4
	ldr r1, _08052D98 @ =0x00000D66
	add r0, r2, r1
	ldrb r1, [r0]
	cmp r1, #0
	beq _08052D9C
	mov r0, #0xB
	str r0, [sp, #8]
	mov r3, sl
	str r3, [sp, #0xC]
	b _08052DD8
_08052D94: .4byte 0x020192E4
_08052D98: .4byte 0x00000D66
_08052D9C:
	ldrb r0, [r2, #2]
	cmp r0, #0
	beq _08052DAC
	str r1, [sp, #4]
	mov r0, #0xB
	str r0, [sp, #8]
	str r1, [sp, #0xC]
	b _08052DD8
_08052DAC:
	str r0, [sp, #4]
	b _08052DD6
_08052DB0:
	ldr r1, [sp, #4]
	cmp r1, #0
	beq _08052DBC
	mov r1, r9
	str r1, [sp, #8]
	b _08052DD6
_08052DBC:
	mov r3, #1
	str r3, [sp, #4]
	str r1, [sp, #0xC]
	b _08052DD8
_08052DC4:
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _08052DD4
	mov r0, r9
	str r0, [sp, #8]
	mov r1, sl
	str r1, [sp, #0xC]
	b _08052DD8
_08052DD4:
	str r0, [sp, #8]
_08052DD6:
	str r0, [sp, #0xC]
_08052DD8:
	mov r0, #2
	ldr r3, [sp, #0x10]
	and r0, r3
	cmp r0, #0
	beq _08052E9A
	ldr r0, [sp, #8]
	cmp r0, #5
	beq _08052E30
	cmp r0, #5
	bgt _08052DF2
	cmp r0, #0
	beq _08052E76
	b _08052E9A
_08052DF2:
	cmp r0, #0xA
	beq _08052E88
	cmp r0, #0xB
	bne _08052E9A
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _08052E0A
	mov r0, r9
	str r0, [sp, #8]
	mov r1, sl
	str r1, [sp, #0xC]
	b _08052E9A
_08052E0A:
	ldr r0, _08052E20 @ =0x020192E4
	ldr r3, _08052E24 @ =0x00000D66
	add r0, r0, r3
	ldrb r1, [r0]
	cmp r1, #0
	beq _08052E28
	mov r1, #1
	str r1, [sp, #4]
	ldrb r0, [r0]
	sub r0, #1
	b _08052E98
_08052E20: .4byte 0x020192E4
_08052E24: .4byte 0x00000D66
_08052E28:
	mov r3, #1
	str r3, [sp, #4]
	mov r0, r9
	b _08052E48
_08052E30:
	ldr r1, [sp, #4]
	cmp r1, #0
	beq _08052E3E
	mov r1, sl
	str r1, [sp, #8]
	str r1, [sp, #0xC]
	b _08052E9A
_08052E3E:
	ldr r0, _08052E50 @ =0x020192E4
	ldrb r2, [r0, #2]
	cmp r2, #0
	beq _08052E54
	mov r0, #0xB
_08052E48:
	str r0, [sp, #8]
	str r1, [sp, #0xC]
	b _08052E9A
	.align 2, 0
_08052E50: .4byte 0x020192E4
_08052E54:
	ldr r3, _08052E6C @ =0x00000D66
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0
	beq _08052E70
	mov r0, #1
	str r0, [sp, #4]
	mov r0, #0xB
	str r0, [sp, #8]
	str r2, [sp, #0xC]
	b _08052E9A
	.align 2, 0
_08052E6C: .4byte 0x00000D66
_08052E70:
	mov r1, #1
	str r1, [sp, #4]
	b _08052E98
_08052E76:
	ldr r1, [sp, #4]
	cmp r1, #0
	beq _08052E80
	str r0, [sp, #4]
	b _08052E98
_08052E80:
	mov r3, r9
	str r3, [sp, #8]
	str r1, [sp, #0xC]
	b _08052E9A
_08052E88:
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _08052E94
	mov r0, sl
	str r0, [sp, #8]
	b _08052E98
_08052E94:
	mov r1, r9
	str r1, [sp, #8]
_08052E98:
	str r0, [sp, #0xC]
_08052E9A:
	ldr r0, [sp, #4]
	ldr r3, [sp, #0x20]
	cmp r3, r0
	bne _08052EB6
	ldr r0, [sp, #8]
	ldr r1, [sp, #0x24]
	cmp r1, r0
	bne _08052EB6
	ldr r0, [sp, #0xC]
	ldr r3, [sp, #0x28]
	cmp r3, r0
	bne _08052EB6
	mov r0, #0
	b _08052F28
_08052EB6:
	ldr r0, [sp, #4]
	ldr r1, [sp, #8]
	ldr r2, [sp, #0xC]
	ldr r3, [sp, #0x4C]
	bl sub_08052908
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08052EEA
	ldr r3, [sp, #4]
	mov r8, r3
	ldr r7, [sp, #8]
	ldr r6, [sp, #0xC]
	add r5, sp, #8
	add r4, sp, #0xC
_08052ED4:
	ldr r0, [sp, #0x4C]
	str r0, [sp, #0]
	mov r0, #4
	add r1, sp, #4
	add r2, r5, #0
	add r3, r4, #0
	bl sub_08052B78
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08052F00
_08052EEA:
	ldr r0, [sp, #4]
	ldr r1, [sp, #0x14]
	str r0, [r1]
	ldr r0, [sp, #8]
	ldr r3, [sp, #0x18]
	str r0, [r3]
	ldr r0, [sp, #0xC]
	ldr r1, [sp, #0x1C]
	str r0, [r1]
	mov r0, #1
	b _08052F28
_08052F00:
	ldr r0, [sp, #4]
	cmp r0, r8
	bne _08052ED4
	ldr r0, [sp, #8]
	cmp r0, r7
	bne _08052ED4
	ldr r0, [sp, #0xC]
	cmp r0, r6
	bne _08052ED4
	b _08052D28
_08052F14:
	ldr r3, [sp, #0x4C]
	str r3, [sp, #0]
	ldr r0, [sp, #0x10]
	ldr r1, [sp, #0x14]
	ldr r2, [sp, #0x18]
	ldr r3, [sp, #0x1C]
	bl sub_08052B78
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_08052F28:
	add sp, #0x2C
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08052CE8

