	thumb_func_start sub_080589C8
sub_080589C8: @ 0x080589C8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	ldr r1, _08058A18 @ =0x000007FF
	add r0, r1, #0
	mov r2, r8
	ldrh r2, [r2]
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _08058A1C @ =0x08622AB4
	add r0, r0, r3
	ldrh r2, [r0]
	ldr r0, _08058A20 @ =0x0000015B
	cmp r2, r0
	bne _080589EE
	b _08058B04
_080589EE:
	cmp r2, r0
	bgt _08058A48
	sub r0, #0xD
	cmp r2, r0
	bgt _08058A32
	sub r0, #5
	cmp r2, r0
	blt _08058A00
	b _08058CB8
_08058A00:
	cmp r2, #0xE4
	bne _08058A06
	b _08058D98
_08058A06:
	cmp r2, #0xE4
	bgt _08058A24
	cmp r2, #0xC8
	bne _08058A10
	b _08058D6C
_08058A10:
	cmp r2, #0xE3
	bne _08058A16
	b _08058D84
_08058A16:
	b _08058E08
_08058A18: .4byte 0x000007FF
_08058A1C: .4byte gUnk_08622AB4
_08058A20: .4byte 0x0000015B
_08058A24:
	cmp r2, #0xE5
	bne _08058A2A
	b _08058DAC
_08058A2A:
	cmp r2, #0xE6
	bne _08058A30
	b _08058DC0
_08058A30:
	b _08058E08
_08058A32:
	mov r0, #0xA8
	lsl r0, r0, #1
	cmp r2, r0
	beq _08058AEC
	cmp r2, r0
	blt _08058AC8
	add r0, #5
	cmp r2, r0
	ble _08058A46
	b _08058E08
_08058A46:
	b _08058CB8
_08058A48:
	ldr r0, _08058A6C @ =0x0000040E
	cmp r2, r0
	bne _08058A50
	b _08058D34
_08058A50:
	cmp r2, r0
	bgt _08058A8C
	sub r0, #0x1F
	cmp r2, r0
	bne _08058A5C
	b _08058DD4
_08058A5C:
	cmp r2, r0
	bgt _08058A78
	ldr r0, _08058A70 @ =0x0000029F
	cmp r2, r0
	bne _08058A68
	b _08058BBC
_08058A68:
	ldr r0, _08058A74 @ =0x000003EE
	b _08058AA2
_08058A6C: .4byte 0x0000040E
_08058A70: .4byte 0x0000029F
_08058A74: .4byte 0x000003EE
_08058A78:
	mov r0, #0xFC
	lsl r0, r0, #2
	cmp r2, r0
	bne _08058A82
	b _08058B90
_08058A82:
	add r0, #1
	cmp r2, r0
	bne _08058A8A
	b _08058CC4
_08058A8A:
	b _08058E08
_08058A8C:
	ldr r0, _08058AAC @ =0x00000437
	cmp r2, r0
	bne _08058A94
	b _08058CC4
_08058A94:
	cmp r2, r0
	bgt _08058AB0
	sub r0, #0x28
	cmp r2, r0
	bne _08058AA0
	b _08058DF0
_08058AA0:
	add r0, #0x1E
_08058AA2:
	cmp r2, r0
	bne _08058AA8
	b _08058CB8
_08058AA8:
	b _08058E08
	.align 2, 0
_08058AAC: .4byte 0x00000437
_08058AB0:
	ldr r0, _08058AC4 @ =0x0000046A
	cmp r2, r0
	ble _08058AB8
	b _08058E08
_08058AB8:
	sub r0, #5
	cmp r2, r0
	bge _08058AC0
	b _08058E08
_08058AC0:
	b _08058CB8
	.align 2, 0
_08058AC4: .4byte 0x0000046A
_08058AC8:
	mov r0, #1
	mov r3, r8
	ldrb r3, [r3, #2]
	and r0, r3
	cmp r0, #0
	beq _08058AD6
	b _08058CB8
_08058AD6:
	ldr r1, _08058AE8 @ =0x000003FB
_08058AD8:
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08058AE6
	b _08058E08
_08058AE6:
	b _08058EC8
_08058AE8: .4byte 0x000003FB
_08058AEC:
	mov r0, #1
	mov r4, r8
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0
	beq _08058AFA
	b _08058CB8
_08058AFA:
	ldr r1, _08058B00 @ =0x000003FE
	b _08058AD8
	.align 2, 0
_08058B00: .4byte 0x000003FE
_08058B04:
	mov r7, #0
	mov r5, #0
	mov r0, #1
	mov sl, r0
	ldr r1, _08058B80 @ =0x00000D64
	mov r9, r1
_08058B10:
	mov r4, #0
	add r6, r5, #1
	mov r2, sl
	and r5, r2
	mov r3, r9
	mul r3, r5
	add r5, r3, #0
_08058B1E:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r5
	ldr r1, _08058B84 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08058B3C
	bl sub_08007730
	cmp r0, #0
	beq _08058B3C
	add r7, #1
_08058B3C:
	add r4, #1
	cmp r4, #4
	ble _08058B1E
	add r5, r6, #0
	cmp r5, #1
	ble _08058B10
	ldr r1, _08058B88 @ =0x020192E4
	lsl r0, r7, #5
	sub r0, r0, r7
	lsl r0, r0, #2
	add r0, r0, r7
	lsl r0, r0, #2
	ldrh r1, [r1]
	cmp r1, r0
	bge _08058B6A
	ldr r1, _08058B8C @ =0x0000047E
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08058B6A
	b _08058EC8
_08058B6A:
	bl sub_08076F9C
	mov r1, #3
	bl __modsi3
	add r0, #1
	cmp r7, r0
	bgt _08058B7C
	b _08058E08
_08058B7C:
	ldr r1, _08058B8C @ =0x0000047E
	b _08058AD8
_08058B80: .4byte 0x00000D64
_08058B84: .4byte 0x0201930C
_08058B88: .4byte 0x020192E4
_08058B8C: .4byte 0x0000047E
_08058B90:
	mov r0, #1
	mov r4, r8
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0
	beq _08058B9E
	b _08058CB8
_08058B9E:
	ldr r1, _08058BB4 @ =0x000003FD
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08058BAE
	b _08058EC8
_08058BAE:
	ldr r1, _08058BB8 @ =0x00000402
	b _08058AD8
	.align 2, 0
_08058BB4: .4byte 0x000003FD
_08058BB8: .4byte 0x00000402
_08058BBC:
	mov r0, #1
	mov r1, r8
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #0
	bne _08058CB8
	mov r0, #1
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_08008B70
	cmp r0, #0
	beq _08058CB8
	ldr r1, _08058C5C @ =0x00000426
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08058BE8
	b _08058EC8
_08058BE8:
	mov r0, #1
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_08008B70
	cmp r0, #1
	ble _08058C14
	ldr r0, _08058C60 @ =0x020192E4
	ldr r2, _08058C64 @ =0x00000D66
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0
	beq _08058C14
	ldr r1, _08058C68 @ =0x00000405
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08058C14
	b _08058EC8
_08058C14:
	mov r5, #5
	ldr r3, _08058C6C @ =0x000007FF
	add r4, r3, #0
_08058C1A:
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08058C70 @ =0x0201A070
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08058CB2
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08058C74 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08058C78 @ =0x00000434
	cmp r1, r0
	bgt _08058C7C
	sub r0, #1
	cmp r1, r0
	bge _08058C9A
	sub r0, #0x3B
	cmp r1, r0
	bgt _08058CB2
	sub r0, #1
	cmp r1, r0
	blt _08058CB2
	b _08058C9A
	.align 2, 0
_08058C5C: .4byte 0x00000426
_08058C60: .4byte 0x020192E4
_08058C64: .4byte 0x00000D66
_08058C68: .4byte 0x00000405
_08058C6C: .4byte 0x000007FF
_08058C70: .4byte 0x0201A070
_08058C74: .4byte gUnk_08622AB4
_08058C78: .4byte 0x00000434
_08058C7C:
	ldr r0, _08058C90 @ =0x000004B4
	cmp r1, r0
	bgt _08058C94
	sub r0, #1
	cmp r1, r0
	bge _08058C9A
	sub r0, #0x79
	cmp r1, r0
	beq _08058C9A
	b _08058CB2
_08058C90: .4byte 0x000004B4
_08058C94:
	ldr r0, _08058CBC @ =0x000005A7
	cmp r1, r0
	bne _08058CB2
_08058C9A:
	and r2, r4
	lsl r0, r2, #1
	ldr r2, _08058CC0 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08058CB2
	b _08058EC8
_08058CB2:
	add r5, #1
	cmp r5, #9
	ble _08058C1A
_08058CB8:
	mov r0, #0
	b _08058ECA
_08058CBC: .4byte 0x000005A7
_08058CC0: .4byte gUnk_08622AB4
_08058CC4:
	mov r4, #1
	add r0, r4, #0
	mov r3, r8
	ldrb r3, [r3, #2]
	and r0, r3
	cmp r0, #0
	bne _08058CB8
	mov r0, r8
	ldrh r1, [r0, #0xC]
	add r3, r1, #0
	add r2, r4, #0
	and r2, r3
	lsr r1, r1, #8
	mov r0, #0x94
	mul r1, r0
	ldr r5, _08058D24 @ =0x00000D64
	add r0, r2, #0
	mul r0, r5
	add r0, r1, r0
	ldr r2, _08058D28 @ =0x0201930C
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08058CF8
	b _08058E08
_08058CF8:
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	and r0, r4
	mul r0, r5
	add r0, r1, r0
	add r0, r0, r2
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08058D2C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0x9B
	lsl r0, r0, #1
	cmp r1, r0
	beq _08058CB8
	ldr r0, _08058D30 @ =0x00000405
	cmp r1, r0
	beq _08058D20
	b _08058E08
_08058D20:
	b _08058CB8
	.align 2, 0
_08058D24: .4byte 0x00000D64
_08058D28: .4byte 0x0201930C
_08058D2C: .4byte gUnk_08622AB4
_08058D30: .4byte 0x00000405
_08058D34:
	mov r2, r8
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	and r1, r0
	lsl r1, r1, #1
	add r1, r1, r3
	ldrh r1, [r1]
	ldr r0, _08058D60 @ =0x0000024E
	cmp r1, r0
	bne _08058D4C
	b _08058EC8
_08058D4C:
	ldr r0, _08058D64 @ =0x000004C5
	cmp r1, r0
	bne _08058CB8
	ldr r0, _08058D68 @ =0x020192E4
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08058D5C
	b _08058EC8
_08058D5C:
	b _08058CB8
	.align 2, 0
_08058D60: .4byte 0x0000024E
_08058D64: .4byte 0x000004C5
_08058D68: .4byte 0x020192E4
_08058D6C:
	ldr r0, _08058D7C @ =0x020192E4
	ldr r3, _08058D80 @ =0x00000D64
	add r0, r0, r3
	ldrh r0, [r0]
	cmp r0, #0xC8
	bls _08058E08
	b _08058CB8
	.align 2, 0
_08058D7C: .4byte 0x020192E4
_08058D80: .4byte 0x00000D64
_08058D84:
	ldr r0, _08058D90 @ =0x020192E4
	ldr r4, _08058D94 @ =0x00000D64
	add r0, r0, r4
	mov r1, #0xFA
	lsl r1, r1, #1
	b _08058DDE
_08058D90: .4byte 0x020192E4
_08058D94: .4byte 0x00000D64
_08058D98:
	ldr r0, _08058DA4 @ =0x020192E4
	ldr r1, _08058DA8 @ =0x00000D64
	add r0, r0, r1
	mov r1, #0x96
	lsl r1, r1, #2
	b _08058DDE
_08058DA4: .4byte 0x020192E4
_08058DA8: .4byte 0x00000D64
_08058DAC:
	ldr r0, _08058DB8 @ =0x020192E4
	ldr r2, _08058DBC @ =0x00000D64
	add r0, r0, r2
	mov r1, #0xC8
	lsl r1, r1, #2
	b _08058DDE
_08058DB8: .4byte 0x020192E4
_08058DBC: .4byte 0x00000D64
_08058DC0:
	ldr r0, _08058DCC @ =0x020192E4
	ldr r3, _08058DD0 @ =0x00000D64
	add r0, r0, r3
	mov r1, #0xFA
	lsl r1, r1, #2
	b _08058DDE
_08058DCC: .4byte 0x020192E4
_08058DD0: .4byte 0x00000D64
_08058DD4:
	ldr r0, _08058DE8 @ =0x020192E4
	ldr r4, _08058DEC @ =0x00000D64
	add r0, r0, r4
	mov r1, #0x96
	lsl r1, r1, #1
_08058DDE:
	ldrh r0, [r0]
	cmp r0, r1
	bls _08058E08
	b _08058CB8
	.align 2, 0
_08058DE8: .4byte 0x020192E4
_08058DEC: .4byte 0x00000D64
_08058DF0:
	ldr r0, _08058E60 @ =0x020192E4
	ldr r1, _08058E64 @ =0x00000D64
	add r2, r0, r1
	ldr r3, _08058E68 @ =0x00000D66
	add r0, r0, r3
	mov r1, #0xC8
	ldrb r0, [r0]
	mul r0, r1
	ldrh r2, [r2]
	cmp r2, r0
	ble _08058E08
	b _08058CB8
_08058E08:
	ldr r0, _08058E6C @ =0x000007FF
	mov r4, r8
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08058E70 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08058E7C
	cmp r0, #0x16
	bne _08058EB8
	mov r0, #1
	mov r2, r8
	ldrb r2, [r2, #2]
	and r0, r2
	cmp r0, #0
	bne _08058EB8
	ldr r1, _08058E74 @ =0x00000482
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08058EC8
	ldr r0, _08058E60 @ =0x020192E4
	ldr r3, _08058E68 @ =0x00000D66
	add r0, r0, r3
	ldrb r0, [r0]
	cmp r0, #0
	beq _08058EB8
	ldr r1, _08058E78 @ =0x00000405
_08058E50:
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08058EB8
	b _08058EC8
	.align 2, 0
_08058E60: .4byte 0x020192E4
_08058E64: .4byte 0x00000D64
_08058E68: .4byte 0x00000D66
_08058E6C: .4byte 0x000007FF
_08058E70: .4byte gUnk_08621DE0
_08058E74: .4byte 0x00000482
_08058E78: .4byte 0x00000405
_08058E7C:
	mov r0, #1
	mov r4, r8
	ldrb r4, [r4, #2]
	and r0, r4
	cmp r0, #0
	bne _08058EB8
	ldr r1, _08058EAC @ =0x00000409
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08058EC8
	ldr r0, _08058EB0 @ =0x020192E4
	ldr r1, _08058EB4 @ =0x00000D64
	add r0, r0, r1
	mov r1, #0xFA
	lsl r1, r1, #2
	ldrh r0, [r0]
	cmp r0, r1
	bls _08058EB8
	add r1, #0x1E
	b _08058E50
	.align 2, 0
_08058EAC: .4byte 0x00000409
_08058EB0: .4byte 0x020192E4
_08058EB4: .4byte 0x00000D64
_08058EB8:
	ldr r1, _08058ED8 @ =0x000005A7
	mov r0, r8
	bl sub_08058924
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08058EC8
	b _08058CB8
_08058EC8:
	mov r0, #1
_08058ECA:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08058ED8: .4byte 0x000005A7
	thumb_func_end sub_080589C8

