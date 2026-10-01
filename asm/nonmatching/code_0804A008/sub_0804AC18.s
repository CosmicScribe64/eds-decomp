	thumb_func_start sub_0804AC18
sub_0804AC18: @ 0x0804AC18
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r5, _0804AC38 @ =0x020192E0
	ldr r0, _0804AC3C @ =0x00001B16
	add r6, r5, r0
	ldrh r1, [r6]
	lsl r0, r1, #0x17
	lsr r0, r0, #0x18
	cmp r0, #1
	beq _0804AC90
	cmp r0, #1
	bgt _0804AC40
	cmp r0, #0
	beq _0804AC48
	b _0804AE28
	.align 2, 0
_0804AC38: .4byte 0x020192E0
_0804AC3C: .4byte 0x00001B16
_0804AC40:
	cmp r0, #0xA
	bne _0804AC46
	b _0804AD88
_0804AC46:
	b _0804AE28
_0804AC48:
	add r0, r5, #4
	add r1, r4, #0
	mov r2, #0
	mov r3, #0
	bl sub_0804A848
	ldr r2, _0804AC88 @ =0x02018450
	mov r0, #3
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #5
	neg r1, r1
	and r0, r1
	sub r1, #0x1C
	and r0, r1
	strb r0, [r2]
	add r0, r4, #0
	mov r1, #0
	mov r2, #0
	bl sub_08024134
	ldrh r2, [r6]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #1
	ldr r0, _0804AC8C @ =0xFFFFFE01
	and r0, r2
	b _0804AE1E
_0804AC88: .4byte 0x02018450
_0804AC8C: .4byte 0xFFFFFE01
_0804AC90:
	cmp r4, #0
	bne _0804AD18
	bl sub_0804A1C8
	cmp r0, #0
	beq _0804ACA0
_0804AC9C:
	mov r0, #0
	b _0804AE56
_0804ACA0:
	ldr r1, _0804ACC4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804AC9C
	ldrb r5, [r5, #0xC]
	lsl r0, r5, #0x19
	cmp r0, #0
	bge _0804ACD0
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _0804ACC8 @ =0x00000616
	ldr r3, _0804ACCC @ =0x08085878
	mov r2, #0xB
	bl sub_080602A4
	b _0804ACDE
_0804ACC4: .4byte 0x03000040
_0804ACC8: .4byte 0x00000616
_0804ACCC: .4byte gUnk_08085878
_0804ACD0:
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _0804ACFC @ =0x00000616
	ldr r3, _0804AD00 @ =0x080858E4
	mov r2, #0xB
	bl sub_080602A4
_0804ACDE:
	ldr r1, _0804AD04 @ =0x0804F311
	ldr r2, _0804AD08 @ =0x0804F385
	mov r0, #5
	bl sub_08060308
	ldr r2, _0804AD0C @ =0x020192E0
	ldr r3, _0804AD10 @ =0x00001B16
	add r2, r2, r3
	ldr r0, _0804AD14 @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	mov r1, #0x14
	orr r0, r1
	strh r0, [r2]
	b _0804AC9C
_0804ACFC: .4byte 0x00000616
_0804AD00: .4byte gUnk_080858E4
_0804AD04: .4byte sub_0804F310
_0804AD08: .4byte sub_0804F384
_0804AD0C: .4byte 0x020192E0
_0804AD10: .4byte 0x00001B16
_0804AD14: .4byte 0xFFFFFE01
_0804AD18:
	mov r0, #0
	bl sub_0805809C
	cmp r0, #0
	beq _0804AD60
	ldr r4, _0804AD48 @ =0x02018450
	ldr r0, _0804AD4C @ =0x02015F00
	ldrb r0, [r0, #0xC]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x1D
	lsl r0, r0, #6
	ldr r2, _0804AD50 @ =0xFFFFFE3F
	ldrh r3, [r4]
	and r2, r3
	orr r2, r0
	strh r2, [r4]
	ldr r0, _0804AD54 @ =0x00008008
	ldr r1, _0804AD58 @ =0x0201CFB0
	ldr r3, _0804AD5C @ =0x00000824
	add r1, r1, r3
	ldrh r1, [r1]
	lsl r2, r2, #0x17
	b _0804AE3A
	.align 2, 0
_0804AD48: .4byte 0x02018450
_0804AD4C: .4byte 0x02015F00
_0804AD50: .4byte 0xFFFFFE3F
_0804AD54: .4byte 0x00008008
_0804AD58: .4byte 0x0201CFB0
_0804AD5C: .4byte 0x00000824
_0804AD60:
	ldr r3, _0804AD7C @ =0x00001B14
	add r2, r5, r3
	ldr r0, [r2]
	ldr r1, _0804AD80 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #5
	orr r0, r1
	str r0, [r2]
	ldr r0, _0804AD84 @ =0xFFFFFE01
	ldrh r1, [r6]
	and r0, r1
	b _0804AE20
	.align 2, 0
_0804AD7C: .4byte 0x00001B14
_0804AD80: .4byte 0xFFFE01FF
_0804AD84: .4byte 0xFFFFFE01
_0804AD88:
	ldr r0, _0804AD9C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #1
	beq _0804ADE0
	cmp r0, #1
	bgt _0804ADA0
	cmp r0, #0
	beq _0804ADA6
	b _0804AC9C
	.align 2, 0
_0804AD9C: .4byte 0x0201AE60
_0804ADA0:
	cmp r0, #2
	beq _0804AE18
	b _0804AC9C
_0804ADA6:
	ldr r3, _0804ADD0 @ =0x00001B14
	add r2, r5, r3
	ldr r0, [r2]
	ldr r1, _0804ADD4 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #5
	orr r0, r1
	str r0, [r2]
	ldr r0, _0804ADD8 @ =0xFFFFFE01
	ldrh r1, [r6]
	and r0, r1
	strh r0, [r6]
	ldr r2, _0804ADDC @ =0x00001B26
	add r1, r5, r2
	mov r0, #2
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	b _0804AE04
	.align 2, 0
_0804ADD0: .4byte 0x00001B14
_0804ADD4: .4byte 0xFFFE01FF
_0804ADD8: .4byte 0xFFFFFE01
_0804ADDC: .4byte 0x00001B26
_0804ADE0:
	ldr r0, _0804AE08 @ =0x00001B14
	add r2, r5, r0
	ldr r0, [r2]
	ldr r1, _0804AE0C @ =0xFFFE01FF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #5
	orr r0, r1
	str r0, [r2]
	ldr r0, _0804AE10 @ =0xFFFFFE01
	ldrh r1, [r6]
	and r0, r1
	strh r0, [r6]
	ldr r2, _0804AE14 @ =0x00001B26
	add r1, r5, r2
	mov r0, #1
	ldrb r3, [r1]
	orr r0, r3
_0804AE04:
	strb r0, [r1]
	b _0804AC9C
_0804AE08: .4byte 0x00001B14
_0804AE0C: .4byte 0xFFFE01FF
_0804AE10: .4byte 0xFFFFFE01
_0804AE14: .4byte 0x00001B26
_0804AE18:
	ldr r0, _0804AE24 @ =0xFFFFFE01
	and r0, r1
	mov r1, #2
_0804AE1E:
	orr r0, r1
_0804AE20:
	strh r0, [r6]
	b _0804AC9C
_0804AE24: .4byte 0xFFFFFE01
_0804AE28:
	mov r0, #8
	cmp r4, #0
	beq _0804AE30
	ldr r0, _0804AE5C @ =0x00008008
_0804AE30:
	lsl r1, r4, #0x10
	lsr r1, r1, #0x10
	ldr r4, _0804AE60 @ =0x02018450
	ldrh r3, [r4]
	lsl r2, r3, #0x17
_0804AE3A:
	lsr r2, r2, #0x1D
	lsl r2, r2, #8
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x11
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	mov r1, #9
	neg r1, r1
	and r0, r1
	strb r0, [r4]
	mov r0, #1
_0804AE56:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0804AE5C: .4byte 0x00008008
_0804AE60: .4byte 0x02018450
	thumb_func_end sub_0804AC18

