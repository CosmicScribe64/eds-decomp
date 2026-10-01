	thumb_func_start sub_0801C938
sub_0801C938: @ 0x0801C938
	push {r4, r5, r6, r7, lr}
	ldr r0, _0801C95C @ =0x03000040
	ldr r2, _0801C960 @ =0x00004870
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x1A
	lsr r5, r1, #0x1B
	add r7, r0, #0
	cmp r5, #0x19
	bne _0801C94E
	b _0801CD4E
_0801C94E:
	cmp r5, #0x19
	bgt _0801C964
	cmp r5, #0
	bne _0801C958
	b _0801CD4E
_0801C958:
	b _0801C96A
	.align 2, 0
_0801C95C: .4byte 0x03000040
_0801C960: .4byte 0x00004870
_0801C964:
	cmp r5, #0x1F
	bne _0801C96A
	b _0801CD4E
_0801C96A:
	ldr r0, _0801C990 @ =0x08081AE4
	lsl r1, r5, #1
	add r0, r1, r0
	ldrh r4, [r0]
	ldr r3, _0801C994 @ =0x0000488A
	add r0, r7, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x18
	add r6, r1, #0
	cmp r0, #4
	bls _0801C984
	b _0801CE46
_0801C984:
	lsl r0, r0, #2
	ldr r1, _0801C998 @ =0x0801C99C
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801C990: .4byte gUnk_08081AE4
_0801C994: .4byte 0x0000488A
_0801C998: .4byte 0x0801C99C
_0801C99C:
	.4byte _0801C9B0
	.4byte _0801CD44
	.4byte _0801CD68
	.4byte _0801CDAC
	.4byte _0801CE0C
_0801C9B0:
	mov r0, #0
	mov ip, r0
	ldr r2, _0801C9C8 @ =0x00004888
	add r1, r7, r2
	mov r0, #0xC
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #4
	bne _0801C9D0
	ldr r6, _0801C9CC @ =0x020192E0
	b _0801CB84
	.align 2, 0
_0801C9C8: .4byte 0x00004888
_0801C9CC: .4byte 0x020192E0
_0801C9D0:
	ldr r1, _0801C9F4 @ =0x020192E0
	ldr r3, _0801C9F8 @ =0x00001B12
	add r0, r1, r3
	ldrb r0, [r0]
	lsr r0, r0, #6
	add r6, r1, #0
	cmp r0, #2
	beq _0801CA00
	cmp r0, #2
	bgt _0801CA0A
	cmp r0, #1
	bne _0801CA0A
	ldr r0, _0801C9FC @ =0x00004889
	add r1, r7, r0
	ldrb r0, [r1]
	add r0, #1
	b _0801CA08
	.align 2, 0
_0801C9F4: .4byte 0x020192E0
_0801C9F8: .4byte 0x00001B12
_0801C9FC: .4byte 0x00004889
_0801CA00:
	ldr r2, _0801CA7C @ =0x00004889
	add r1, r7, r2
	ldrb r0, [r1]
	sub r0, #1
_0801CA08:
	strb r0, [r1]
_0801CA0A:
	ldr r0, _0801CA80 @ =0x00004888
	add r3, r7, r0
	ldrb r2, [r3]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1E
	add r1, #1
	mov r0, #3
	and r1, r0
	lsl r1, r1, #4
	mov r0, #0x31
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #2
	beq _0801CA88
	cmp r0, #3
	bne _0801CAD0
	ldr r1, _0801CA7C @ =0x00004889
	add r3, r7, r1
	mov r0, #0
	ldsb r0, [r3, r0]
	cmp r0, #0
	bge _0801CA4E
	ldr r2, _0801CA84 @ =0x00001B12
	add r0, r6, r2
	mov r1, #0x3F
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x80
	orr r1, r2
	strb r1, [r0]
_0801CA4E:
	ldrb r3, [r3]
	lsl r3, r3, #0x18
	asr r3, r3, #0x18
	cmp r3, #0
	ble _0801CA68
	ldr r1, _0801CA84 @ =0x00001B12
	add r0, r6, r1
	mov r1, #0x3F
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x40
	orr r1, r2
	strb r1, [r0]
_0801CA68:
	cmp r3, #0
	beq _0801CA6E
	b _0801CB84
_0801CA6E:
	ldr r3, _0801CA84 @ =0x00001B12
	add r1, r6, r3
	mov r0, #0xC0
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	b _0801CB84
_0801CA7C: .4byte 0x00004889
_0801CA80: .4byte 0x00004888
_0801CA84: .4byte 0x00001B12
_0801CA88:
	ldr r3, _0801CAA0 @ =0x00004889
	add r0, r7, r3
	mov r1, #0
	ldsb r1, [r0, r1]
	mov r0, #2
	neg r0, r0
	cmp r1, r0
	beq _0801CAA4
	cmp r1, #2
	beq _0801CABC
	b _0801CAD0
	.align 2, 0
_0801CAA0: .4byte 0x00004889
_0801CAA4:
	ldr r1, _0801CAB8 @ =0x00001B12
	add r0, r6, r1
	mov r1, #0x3F
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x80
	orr r1, r2
	strb r1, [r0]
	b _0801CB84
	.align 2, 0
_0801CAB8: .4byte 0x00001B12
_0801CABC:
	ldr r3, _0801CAEC @ =0x00001B12
	add r0, r6, r3
	mov r1, #0x3F
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0x40
	orr r1, r2
	strb r1, [r0]
	mov r3, #1
	mov ip, r3
_0801CAD0:
	mov r0, ip
	cmp r0, #0
	bne _0801CB84
	ldr r1, _0801CAEC @ =0x00001B12
	add r0, r6, r1
	ldrb r0, [r0]
	lsr r0, r0, #6
	cmp r0, #2
	beq _0801CB08
	cmp r0, #2
	bgt _0801CAF0
	cmp r0, #1
	beq _0801CAF6
	b _0801CB28
_0801CAEC: .4byte 0x00001B12
_0801CAF0:
	cmp r0, #3
	beq _0801CB1C
	b _0801CB28
_0801CAF6:
	ldr r0, _0801CB04 @ =0x080817FC
	lsl r1, r5, #4
	add r1, r1, r0
	ldrh r0, [r1, #6]
	bl sub_08001C10
	b _0801CB28
_0801CB04: .4byte gUnk_080817FC
_0801CB08:
	ldr r0, _0801CB18 @ =0x080817FC
	lsl r1, r5, #4
	add r1, r1, r0
	ldrh r0, [r1, #8]
	bl sub_08001C10
	b _0801CB28
	.align 2, 0
_0801CB18: .4byte gUnk_080817FC
_0801CB1C:
	ldr r0, _0801CB70 @ =0x080817FC
	lsl r1, r5, #4
	add r1, r1, r0
	ldrh r0, [r1, #8]
	bl sub_08001C10
_0801CB28:
	ldr r2, _0801CB74 @ =0x03000040
	ldr r3, _0801CB78 @ =0x00004859
	add r0, r2, r3
	mov r1, #0
	strb r1, [r0]
	ldr r4, _0801CB7C @ =0x0000485A
	add r0, r2, r4
	strb r1, [r0]
	add r3, #2
	add r0, r2, r3
	strb r1, [r0]
	add r4, #0x30
	add r2, r2, r4
	ldrh r4, [r2]
	lsl r0, r4, #0x14
	lsr r0, r0, #0x18
	add r0, #1
	mov r5, #0xFF
	and r0, r5
	lsl r0, r0, #4
	ldr r3, _0801CB80 @ =0xFFFFF00F
	add r1, r3, #0
	and r1, r4
	orr r1, r0
	lsr r0, r0, #4
	add r0, #1
	and r0, r5
	lsl r0, r0, #4
	and r1, r3
	orr r1, r0
	strh r1, [r2]
	mov r0, #0x15
	bl sub_08077B24
	b _0801CE46
	.align 2, 0
_0801CB70: .4byte gUnk_080817FC
_0801CB74: .4byte 0x03000040
_0801CB78: .4byte 0x00004859
_0801CB7C: .4byte 0x0000485A
_0801CB80: .4byte 0xFFFFF00F
_0801CB84:
	ldr r1, _0801CB9C @ =0x00001B12
	add r0, r6, r1
	ldrb r0, [r0]
	lsr r0, r0, #6
	cmp r0, #2
	beq _0801CBF8
	cmp r0, #2
	bgt _0801CBA0
	cmp r0, #1
	beq _0801CBA6
	b _0801CC3A
	.align 2, 0
_0801CB9C: .4byte 0x00001B12
_0801CBA0:
	cmp r0, #3
	beq _0801CC2C
	b _0801CC3A
_0801CBA6:
	ldr r0, _0801CBC8 @ =0x02011C20
	lsl r1, r5, #2
	add r1, r1, r0
	ldr r2, _0801CBCC @ =0x000020D0
	add r1, r1, r2
	ldrh r1, [r1]
	lsl r0, r1, #0x15
	lsr r0, r0, #0x15
	cmp r0, #4
	beq _0801CBD4
	cmp r0, #9
	beq _0801CBE4
	ldr r1, _0801CBD0 @ =0x080817FC
	lsl r0, r5, #4
	add r0, r0, r1
	ldrh r4, [r0]
	b _0801CBEC
_0801CBC8: .4byte 0x02011C20
_0801CBCC: .4byte 0x000020D0
_0801CBD0: .4byte gUnk_080817FC
_0801CBD4:
	ldr r1, _0801CBE0 @ =0x080817FC
	lsl r0, r5, #4
	add r0, r0, r1
	ldrh r4, [r0, #0xA]
	b _0801CBEC
	.align 2, 0
_0801CBE0: .4byte gUnk_080817FC
_0801CBE4:
	ldr r1, _0801CBF4 @ =0x080817FC
	lsl r0, r5, #4
	add r0, r0, r1
	ldrh r4, [r0, #0xC]
_0801CBEC:
	mov r0, #0x18
	bl sub_08077B24
	b _0801CC3A
_0801CBF4: .4byte gUnk_080817FC
_0801CBF8:
	ldr r1, _0801CC18 @ =0x080817FC
	lsl r0, r5, #4
	add r0, r0, r1
	ldrh r4, [r0, #2]
	ldr r0, _0801CC1C @ =0x03000040
	ldr r3, _0801CC20 @ =0x0000487C
	add r0, r0, r3
	ldr r1, [r0]
	mov r0, #0x80
	lsl r0, r0, #0x10
	cmp r1, r0
	bne _0801CC24
	mov r0, #0x1C
	bl sub_08077B24
	b _0801CC3A
_0801CC18: .4byte gUnk_080817FC
_0801CC1C: .4byte 0x03000040
_0801CC20: .4byte 0x0000487C
_0801CC24:
	mov r0, #0x19
	bl sub_08077B24
	b _0801CC3A
_0801CC2C:
	ldr r1, _0801CC6C @ =0x080817FC
	lsl r0, r5, #4
	add r0, r0, r1
	ldrh r4, [r0, #4]
	mov r0, #0x19
	bl sub_08077B24
_0801CC3A:
	ldr r0, _0801CC70 @ =0x03000040
	ldr r1, _0801CC74 @ =0x0000487C
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, #0x80
	lsl r0, r0, #0x10
	cmp r1, r0
	bne _0801CD0E
	ldr r0, _0801CC78 @ =0x020192E0
	ldr r2, _0801CC7C @ =0x00001B12
	add r0, r0, r2
	ldrb r0, [r0]
	lsr r0, r0, #6
	cmp r0, #1
	bne _0801CCC0
	add r0, r5, #0
	sub r0, #0xB
	cmp r0, #4
	bhi _0801CD0E
	lsl r0, r0, #2
	ldr r1, _0801CC80 @ =0x0801CC84
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801CC6C: .4byte gUnk_080817FC
_0801CC70: .4byte 0x03000040
_0801CC74: .4byte 0x0000487C
_0801CC78: .4byte 0x020192E0
_0801CC7C: .4byte 0x00001B12
_0801CC80: .4byte 0x0801CC84
_0801CC84:
	.4byte _0801CC98
	.4byte _0801CCA0
	.4byte _0801CCA8
	.4byte _0801CCB0
	.4byte _0801CCB8
_0801CC98:
	ldr r4, _0801CC9C @ =0x00002AFB
	b _0801CD0E
_0801CC9C: .4byte 0x00002AFB
_0801CCA0:
	ldr r4, _0801CCA4 @ =0x00002EE3
	b _0801CD0E
_0801CCA4: .4byte 0x00002EE3
_0801CCA8:
	ldr r4, _0801CCAC @ =0x000032CB
	b _0801CD0E
_0801CCAC: .4byte 0x000032CB
_0801CCB0:
	ldr r4, _0801CCB4 @ =0x000036B3
	b _0801CD0E
_0801CCB4: .4byte 0x000036B3
_0801CCB8:
	ldr r4, _0801CCBC @ =0x00003A9B
	b _0801CD0E
_0801CCBC: .4byte 0x00003A9B
_0801CCC0:
	add r0, r5, #0
	sub r0, #0xB
	cmp r0, #4
	bhi _0801CD0E
	lsl r0, r0, #2
	ldr r1, _0801CCD4 @ =0x0801CCD8
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0801CCD4: .4byte 0x0801CCD8
_0801CCD8:
	.4byte _0801CCEC
	.4byte _0801CCF4
	.4byte _0801CCFC
	.4byte _0801CD04
	.4byte _0801CD0C
_0801CCEC:
	ldr r4, _0801CCF0 @ =0x00002AF8
	b _0801CD0E
_0801CCF0: .4byte 0x00002AF8
_0801CCF4:
	ldr r4, _0801CCF8 @ =0x00002EE0
	b _0801CD0E
_0801CCF8: .4byte 0x00002EE0
_0801CCFC:
	ldr r4, _0801CD00 @ =0x000032C8
	b _0801CD0E
_0801CD00: .4byte 0x000032C8
_0801CD04:
	ldr r4, _0801CD08 @ =0x000036B0
	b _0801CD0E
_0801CD08: .4byte 0x000036B0
_0801CD0C:
	ldr r4, _0801CD54 @ =0x00003A98
_0801CD0E:
	add r0, r4, #0
	bl sub_08001C10
	ldr r2, _0801CD58 @ =0x03000040
	ldr r3, _0801CD5C @ =0x00004859
	add r0, r2, r3
	mov r1, #0
	strb r1, [r0]
	ldr r4, _0801CD60 @ =0x0000485A
	add r0, r2, r4
	strb r1, [r0]
	add r3, #2
	add r0, r2, r3
	strb r1, [r0]
	add r4, #0x30
	add r2, r2, r4
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801CD64 @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
_0801CD44:
	bl sub_08001AE4
	cmp r0, #0
	bne _0801CD4E
	b _0801CE46
_0801CD4E:
	mov r0, #1
	b _0801CE48
	.align 2, 0
_0801CD54: .4byte 0x00003A98
_0801CD58: .4byte 0x03000040
_0801CD5C: .4byte 0x00004859
_0801CD60: .4byte 0x0000485A
_0801CD64: .4byte 0xFFFFF00F
_0801CD68:
	bl sub_08001AE4
	cmp r0, #0
	beq _0801CE46
	ldr r2, _0801CD98 @ =0x03000040
	ldr r1, _0801CD9C @ =0x00004859
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _0801CDA0 @ =0x0000485A
	add r0, r2, r3
	strb r1, [r0]
	ldr r4, _0801CDA4 @ =0x0000485B
	add r0, r2, r4
	strb r1, [r0]
	ldr r0, _0801CDA8 @ =0x00004888
	add r1, r2, r0
	mov r0, #2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	add r4, #0x2F
	add r2, r2, r4
	b _0801CDD8
_0801CD98: .4byte 0x03000040
_0801CD9C: .4byte 0x00004859
_0801CDA0: .4byte 0x0000485A
_0801CDA4: .4byte 0x0000485B
_0801CDA8: .4byte 0x00004888
_0801CDAC:
	bl sub_0806EF74
	cmp r0, #0
	beq _0801CE46
	ldr r0, _0801CDF0 @ =0x0808198C
	add r0, r6, r0
	ldrh r0, [r0]
	bl sub_08001C10
	ldr r2, _0801CDF4 @ =0x03000040
	ldr r1, _0801CDF8 @ =0x00004859
	add r0, r2, r1
	mov r1, #0
	strb r1, [r0]
	ldr r3, _0801CDFC @ =0x0000485A
	add r0, r2, r3
	strb r1, [r0]
	ldr r4, _0801CE00 @ =0x0000485B
	add r0, r2, r4
	strb r1, [r0]
	ldr r0, _0801CE04 @ =0x0000488A
	add r2, r2, r0
_0801CDD8:
	ldrh r3, [r2]
	lsl r1, r3, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _0801CE08 @ =0xFFFFF00F
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	b _0801CE46
_0801CDF0: .4byte gUnk_0808198C
_0801CDF4: .4byte 0x03000040
_0801CDF8: .4byte 0x00004859
_0801CDFC: .4byte 0x0000485A
_0801CE00: .4byte 0x0000485B
_0801CE04: .4byte 0x0000488A
_0801CE08: .4byte 0xFFFFF00F
_0801CE0C:
	bl sub_08001AE4
	cmp r0, #0
	beq _0801CE46
	ldr r1, _0801CE50 @ =0x03000040
	ldr r2, _0801CE54 @ =0x00004857
	add r0, r1, r2
	ldrb r2, [r0]
	sub r2, #3
	mov r3, #0
	strb r2, [r0]
	ldr r4, _0801CE58 @ =0x0000488A
	add r2, r1, r4
	ldr r0, _0801CE5C @ =0xFFFFF00F
	ldrh r4, [r2]
	and r0, r4
	strh r0, [r2]
	ldr r2, _0801CE60 @ =0x00004858
	add r0, r1, r2
	strb r3, [r0]
	ldr r4, _0801CE64 @ =0x00004859
	add r0, r1, r4
	strb r3, [r0]
	add r2, #2
	add r0, r1, r2
	strb r3, [r0]
	add r4, #2
	add r1, r1, r4
	strb r3, [r1]
_0801CE46:
	mov r0, #0
_0801CE48:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0801CE50: .4byte 0x03000040
_0801CE54: .4byte 0x00004857
_0801CE58: .4byte 0x0000488A
_0801CE5C: .4byte 0xFFFFF00F
_0801CE60: .4byte 0x00004858
_0801CE64: .4byte 0x00004859
	thumb_func_end sub_0801C938

