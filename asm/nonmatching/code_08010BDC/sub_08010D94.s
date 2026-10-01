	thumb_func_start sub_08010D94
sub_08010D94: @ 0x08010D94
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	ldr r5, _08010DF0 @ =0x020185C0
	ldrh r0, [r5]
	lsr r0, r0, #0xF
	mov r9, r0
	ldrh r1, [r5, #2]
	str r1, [sp, #8]
	ldrh r2, [r5, #4]
	add r1, r2, #0
	mov r0, #0xF
	mov sl, r0
	and r0, r1
	mov sl, r0
	mov r0, #0xF0
	and r0, r1
	lsr r0, r0, #4
	str r0, [sp, #0xC]
	lsr r2, r2, #8
	mov r8, r2
	mov r0, #1
	mov r1, r8
	and r1, r0
	mov r8, r1
	mov r2, sl
	cmp r2, #4
	ble _08010DD6
	sub r0, #6
	add sl, r0
_08010DD6:
	ldr r1, _08010DF4 @ =0x0000080A
	add r4, r5, r1
	ldrb r2, [r4]
	lsl r0, r2, #0x19
	lsr r7, r0, #0x19
	cmp r7, #1
	beq _08010E20
	cmp r7, #1
	bgt _08010DF8
	cmp r7, #0
	beq _08010E00
	b _08010F54
	.align 2, 0
_08010DF0: .4byte 0x020185C0
_08010DF4: .4byte 0x0000080A
_08010DF8:
	cmp r7, #2
	bne _08010DFE
	b _08010F40
_08010DFE:
	b _08010F54
_08010E00:
	mov r0, r9
	mov r1, #0xB
	bl sub_080240A8
	ldrb r2, [r4]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	b _08010F68
_08010E20:
	ldr r1, _08010F14 @ =0x00000814
	add r0, r5, r1
	mov r1, r9
	and r1, r7
	ldr r2, _08010F18 @ =0x00000D64
	add r5, r1, #0
	mul r5, r2
	ldr r6, _08010F1C @ =0x02019968
	add r1, r5, r6
	ldr r2, [sp, #0xC]
	lsl r4, r2, #2
	add r1, r1, r4
	bl sub_08007558
	add r4, r4, r5
	add r4, r4, r6
	ldr r0, _08010F20 @ =0xFFFFF000
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	add r5, r7, #0
	mov r2, r9
	and r5, r2
	ldr r0, [sp, #0]
	mov r1, #2
	neg r1, r1
	and r0, r1
	orr r0, r5
	mov r2, #0x1F
	neg r2, r2
	mov ip, r2
	and r0, r2
	mov r1, #0x16
	orr r0, r1
	ldr r2, [sp, #0xC]
	lsl r1, r2, #5
	ldr r2, _08010F24 @ =0xFFFFC01F
	mov r9, r2
	and r0, r2
	orr r0, r1
	ldr r6, _08010F28 @ =0xFFFFBFFF
	and r0, r6
	mov r1, r8
	and r1, r7
	lsl r2, r1, #0xF
	ldr r3, _08010F2C @ =0xFFFF7FFF
	and r0, r3
	orr r0, r2
	str r0, [sp, #0]
	ldr r4, [sp, #4]
	mov r0, #2
	neg r0, r0
	and r4, r0
	orr r4, r5
	mov r1, ip
	and r4, r1
	mov r0, #0xA
	orr r4, r0
	mov r1, sl
	lsl r0, r1, #0x17
	lsr r0, r0, #0x12
	mov r1, r9
	and r4, r1
	orr r4, r0
	and r4, r6
	and r4, r3
	orr r4, r2
	str r4, [sp, #4]
	ldr r0, _08010F30 @ =0x000007FF
	ldr r1, [sp, #8]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08010F34 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _08010EEA
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	cmp r0, #2
	bne _08010EEA
	mov r0, #2
	neg r0, r0
	and r4, r0
	orr r4, r5
	mov r1, ip
	and r4, r1
	mov r0, #0x14
	orr r4, r0
	mov r0, r9
	and r4, r0
	and r4, r6
	and r4, r3
	orr r4, r2
	str r4, [sp, #4]
_08010EEA:
	add r2, sp, #4
	ldr r0, [sp, #8]
	mov r1, sp
	bl sub_080242C4
	ldr r2, _08010F38 @ =0x020185C0
	ldr r1, _08010F3C @ =0x0000080A
	add r2, r2, r1
	ldrb r3, [r2]
	lsl r1, r3, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _08010F68
	.align 2, 0
_08010F14: .4byte 0x00000814
_08010F18: .4byte 0x00000D64
_08010F1C: .4byte 0x02019968
_08010F20: .4byte 0xFFFFF000
_08010F24: .4byte 0xFFFFC01F
_08010F28: .4byte 0xFFFFBFFF
_08010F2C: .4byte 0xFFFF7FFF
_08010F30: .4byte 0x000007FF
_08010F34: .4byte gUnk_08621DE0
_08010F38: .4byte 0x020185C0
_08010F3C: .4byte 0x0000080A
_08010F40:
	mov r0, r9
	bl sub_0800A0A8
	ldr r0, _08010F78 @ =0x00000814
	add r2, r5, r0
	mov r0, r9
	mov r1, sl
	mov r3, r8
	bl sub_08007B24
_08010F54:
	bl sub_080611AC
	ldr r1, _08010F7C @ =0x020185C0
	ldr r2, _08010F80 @ =0x0000080D
	add r1, r1, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08010F68:
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08010F78: .4byte 0x00000814
_08010F7C: .4byte 0x020185C0
_08010F80: .4byte 0x0000080D
	thumb_func_end sub_08010D94

