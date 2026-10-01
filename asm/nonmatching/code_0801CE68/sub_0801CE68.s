	thumb_func_start sub_0801CE68
sub_0801CE68: @ 0x0801CE68
	push {r4, r5, r6, lr}
	sub sp, #4
	ldr r0, _0801CE8C @ =0x03000040
	ldr r2, _0801CE90 @ =0x0000488A
	add r1, r0, r2
	ldrh r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x18
	add r6, r0, #0
	cmp r1, #6
	bls _0801CE80
	b _0801D134
_0801CE80:
	lsl r0, r1, #2
	ldr r1, _0801CE94 @ =0x0801CE98
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801CE8C: .4byte 0x03000040
_0801CE90: .4byte 0x0000488A
_0801CE94: .4byte 0x0801CE98
_0801CE98:
	.4byte _0801CEB4
	.4byte _0801CEFC
	.4byte _0801CFB6
	.4byte _0801D01C
	.4byte _0801D0BC
	.4byte _0801CFB6
	.4byte _0801D124
_0801CEB4:
	mov r0, sp
	bl sub_08004914
	ldr r2, [sp, #0]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	lsl r1, r2, #0x10
	lsr r1, r1, #0x1C
	lsl r2, r2, #0xB
	lsr r2, r2, #0x1B
	bl sub_080044E4
	ldr r5, _0801CF44 @ =0x03000040
	ldr r3, _0801CF48 @ =0x0000487C
	add r1, r5, r3
	str r0, [r1]
	mov r1, #0xC0
	lsl r1, r1, #0xE
	and r1, r0
	cmp r1, #0
	bne _0801CEE0
	b _0801D134
_0801CEE0:
	ldr r0, _0801CF4C @ =0x0000488A
	add r3, r5, r0
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801CF50 @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	add r6, r5, #0
_0801CEFC:
	ldr r1, _0801CF48 @ =0x0000487C
	add r0, r6, r1
	ldr r0, [r0]
	mov r1, #0x80
	lsl r1, r1, #0xD
	and r0, r1
	cmp r0, #0
	bne _0801CF54
	ldr r2, _0801CF4C @ =0x0000488A
	add r5, r6, r2
	ldrh r4, [r5]
	lsl r0, r4, #0x14
	lsr r0, r0, #0x18
	add r0, #1
	mov r3, #0xFF
	and r0, r3
	lsl r0, r0, #4
	ldr r2, _0801CF50 @ =0xFFFFF00F
	add r1, r2, #0
	and r1, r4
	orr r1, r0
	lsr r0, r0, #4
	add r0, #1
	and r0, r3
	lsl r0, r0, #4
	and r1, r2
	orr r1, r0
	lsr r0, r0, #4
	add r0, #1
	and r0, r3
	lsl r0, r0, #4
	and r1, r2
	orr r1, r0
	strh r1, [r5]
_0801CF40:
	mov r0, #0
	b _0801D136
_0801CF44: .4byte 0x03000040
_0801CF48: .4byte 0x0000487C
_0801CF4C: .4byte 0x0000488A
_0801CF50: .4byte 0xFFFFF00F
_0801CF54:
	mov r0, #0x1F
	bl sub_08077B24
	ldr r0, _0801CFF0 @ =0x00000323
	bl sub_08001C10
	mov r0, sp
	bl sub_08004914
	bl sub_08076F9C
	ldr r2, _0801CFF4 @ =0x020192E0
	mov r1, #3
	and r0, r1
	ldr r3, _0801CFF8 @ =0x00001B12
	add r4, r2, r3
	lsl r0, r0, #6
	mov r5, #0x3F
	add r1, r5, #0
	ldrb r2, [r4]
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	ldr r0, [sp, #0]
	ldr r1, _0801CFFC @ =0x001FFFFF
	and r0, r1
	ldr r1, _0801D000 @ =0x000917D1
	cmp r0, r1
	bne _0801CF9C
	ldr r0, _0801D004 @ =0x00000322
	bl sub_08001C10
	add r0, r5, #0
	ldrb r3, [r4]
	and r0, r3
	strb r0, [r4]
_0801CF9C:
	ldr r0, _0801D008 @ =0x0000488A
	add r3, r6, r0
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801D00C @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
_0801CFB6:
	bl sub_08001AE4
	cmp r0, #0
	beq _0801CF40
	ldr r2, _0801D010 @ =0x03000040
	ldr r1, _0801D014 @ =0x00004859
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _0801D018 @ =0x0000485A
	add r0, r2, r3
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
	ldr r0, _0801D008 @ =0x0000488A
	add r2, r2, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801D00C @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	b _0801CF40
_0801CFF0: .4byte 0x00000323
_0801CFF4: .4byte 0x020192E0
_0801CFF8: .4byte 0x00001B12
_0801CFFC: .4byte 0x001FFFFF
_0801D000: .4byte 0x000917D1
_0801D004: .4byte 0x00000322
_0801D008: .4byte 0x0000488A
_0801D00C: .4byte 0xFFFFF00F
_0801D010: .4byte 0x03000040
_0801D014: .4byte 0x00004859
_0801D018: .4byte 0x0000485A
_0801D01C:
	ldr r0, _0801D04C @ =0x020192E0
	ldr r1, _0801D050 @ =0x00001B12
	add r0, r0, r1
	mov r4, #0xC0
	ldrb r0, [r0]
	and r4, r0
	cmp r4, #0
	bne _0801D064
	ldr r0, _0801D054 @ =0x00000321
	bl sub_08063B48
	cmp r0, #0
	beq _0801CF40
	ldr r2, _0801D058 @ =0x03000040
	ldr r3, _0801D05C @ =0x00004859
	add r0, r2, r3
	strb r4, [r0]
	ldr r1, _0801D060 @ =0x0000485A
	add r0, r2, r1
	strb r4, [r0]
	add r3, #2
	add r0, r2, r3
	strb r4, [r0]
	b _0801D086
_0801D04C: .4byte 0x020192E0
_0801D050: .4byte 0x00001B12
_0801D054: .4byte 0x00000321
_0801D058: .4byte 0x03000040
_0801D05C: .4byte 0x00004859
_0801D060: .4byte 0x0000485A
_0801D064:
	ldr r0, _0801D0A4 @ =0x00000385
	bl sub_08063B48
	cmp r0, #0
	bne _0801D070
	b _0801CF40
_0801D070:
	ldr r2, _0801D0A8 @ =0x03000040
	ldr r1, _0801D0AC @ =0x00004859
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _0801D0B0 @ =0x0000485A
	add r0, r2, r3
	strb r1, [r0]
	add r3, #1
	add r0, r2, r3
	strb r1, [r0]
_0801D086:
	ldr r0, _0801D0B4 @ =0x0000488A
	add r2, r2, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801D0B8 @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	b _0801CF40
	.align 2, 0
_0801D0A4: .4byte 0x00000385
_0801D0A8: .4byte 0x03000040
_0801D0AC: .4byte 0x00004859
_0801D0B0: .4byte 0x0000485A
_0801D0B4: .4byte 0x0000488A
_0801D0B8: .4byte 0xFFFFF00F
_0801D0BC:
	ldr r1, _0801D110 @ =0x0000487C
	add r0, r6, r1
	ldr r0, [r0]
	mov r1, #0x80
	lsl r1, r1, #0xE
	and r0, r1
	cmp r0, #0
	beq _0801D134
	mov r0, #0x1F
	bl sub_08077B24
	ldr r0, _0801D114 @ =0x00000321
	bl sub_08001C10
	ldr r2, _0801D118 @ =0x0000488A
	add r3, r6, r2
	ldrh r2, [r3]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801D11C @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	mov r0, sp
	bl sub_08004914
	mov r1, sp
	ldr r0, _0801D120 @ =0x000017D1
	ldrh r1, [r1]
	cmp r1, r0
	beq _0801D104
	b _0801CF40
_0801D104:
	mov r0, #0xC8
	lsl r0, r0, #2
	bl sub_08001C10
	b _0801CF40
	.align 2, 0
_0801D110: .4byte 0x0000487C
_0801D114: .4byte 0x00000321
_0801D118: .4byte 0x0000488A
_0801D11C: .4byte 0xFFFFF00F
_0801D120: .4byte 0x000017D1
_0801D124:
	ldr r0, _0801D130 @ =0x00000322
	bl sub_08063B48
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _0801D136
_0801D130: .4byte 0x00000322
_0801D134:
	mov r0, #1
_0801D136:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0801CE68
	.align 2, 0

