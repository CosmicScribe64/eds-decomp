	thumb_func_start sub_0804A99C
sub_0804A99C: @ 0x0804A99C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	mov r0, #0
	mov r8, r0
	ldr r5, _0804AA58 @ =0x02018450
	ldrh r2, [r5]
	lsl r1, r2, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	mov r2, #0
	bl sub_0804A528
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804AA78
	mov r6, #0
	mov r2, #1
	and r2, r7
	ldrh r5, [r5]
	lsl r3, r5, #0x17
	lsr r0, r3, #0x1D
	mov r5, #0x94
	mul r0, r5
	ldr r1, _0804AA5C @ =0x00000D64
	mul r2, r1
	add r0, r0, r2
	ldr r4, _0804AA60 @ =0x0201930C
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0804AA64 @ =0x08622AB4
	add r0, r0, r1
	mov r1, #0xA7
	lsl r1, r1, #3
	ldrh r0, [r0]
	cmp r0, r1
	bne _0804AA16
	lsr r0, r3, #0x1D
	add r1, r0, #0
	mul r1, r5
	add r1, r1, r2
	add r1, r1, r4
	mov r0, #0x20
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	beq _0804AA16
	lsr r0, r3, #0x1D
	add r1, r0, #0
	mul r1, r5
	add r1, r1, r2
	add r1, r1, r4
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1, #7]
	and r0, r2
	strb r0, [r1, #7]
	mov r6, #1
_0804AA16:
	cmp r6, #0
	bne _0804AA28
	ldr r0, _0804AA58 @ =0x02018450
	ldrh r0, [r0]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	bl sub_0804A39C
_0804AA28:
	ldr r1, _0804AA58 @ =0x02018450
	mov r0, #3
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	ldr r2, _0804AA68 @ =0x020192E0
	ldr r5, _0804AA6C @ =0x00001B16
	add r1, r2, r5
	ldr r0, _0804AA70 @ =0xFFFFFE01
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	sub r5, #2
	add r2, r2, r5
	ldr r0, [r2]
	ldr r1, _0804AA74 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #2
	orr r0, r1
	str r0, [r2]
	b _0804AB6C
	.align 2, 0
_0804AA58: .4byte 0x02018450
_0804AA5C: .4byte 0x00000D64
_0804AA60: .4byte 0x0201930C
_0804AA64: .4byte gUnk_08622AB4
_0804AA68: .4byte 0x020192E0
_0804AA6C: .4byte 0x00001B16
_0804AA70: .4byte 0xFFFFFE01
_0804AA74: .4byte 0xFFFE01FF
_0804AA78:
	mov r6, #1
	sub r4, r6, r7
	add r0, r4, #0
	bl sub_08008860
	lsl r1, r4, #1
	mov ip, r1
	mov r2, #0xA4
	lsl r2, r2, #1
	add r1, r5, r2
	add r1, ip
	ldrh r1, [r1]
	cmp r0, r1
	beq _0804AA96
	mov r8, r6
_0804AA96:
	mov r0, #2
	ldrb r3, [r5]
	and r0, r3
	cmp r0, #0
	bne _0804AAFC
	and r4, r6
	ldrb r0, [r5, #1]
	lsl r3, r0, #0x1C
	lsr r0, r3, #0x1D
	mov r6, #0x94
	mul r0, r6
	ldr r1, _0804AB2C @ =0x00000D64
	add r2, r4, #0
	mul r2, r1
	add r0, r0, r2
	ldr r4, _0804AB30 @ =0x0201930C
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0804AAC4
	mov r1, #1
	mov r8, r1
_0804AAC4:
	lsr r0, r3, #0x1D
	mul r0, r6
	add r0, r0, r2
	add r0, r0, r4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804AAF0
	mov r0, #0xA6
	lsl r0, r0, #1
	add r1, r5, r0
	add r1, ip
	lsr r0, r3, #0x1D
	mul r0, r6
	add r0, r0, r2
	add r0, r0, r4
	ldrh r1, [r1]
	ldrh r0, [r0, #4]
	cmp r1, r0
	beq _0804AAF0
	mov r1, #1
	mov r8, r1
_0804AAF0:
	ldr r1, _0804AB34 @ =0x02018450
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0804AB20
_0804AAFC:
	mov r0, #1
	sub r0, r0, r7
	bl sub_08008860
	cmp r0, #0
	ble _0804AB20
	ldr r0, _0804AB34 @ =0x02018450
	ldrh r0, [r0]
	lsl r1, r0, #0x17
	lsr r1, r1, #0x1D
	add r0, r7, #0
	bl sub_0804A3D8
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0804AB20
	mov r2, #1
	mov r8, r2
_0804AB20:
	mov r3, r8
	cmp r3, #0
	bne _0804AB38
	mov r0, #0
	b _0804AB6E
	.align 2, 0
_0804AB2C: .4byte 0x00000D64
_0804AB30: .4byte 0x0201930C
_0804AB34: .4byte 0x02018450
_0804AB38:
	ldr r4, _0804AB78 @ =0x02018450
	mov r2, #3
	neg r2, r2
	ldrb r5, [r4]
	and r2, r5
	ldr r3, _0804AB7C @ =0x020192E0
	ldr r0, _0804AB80 @ =0x00001B16
	add r1, r3, r0
	ldr r0, _0804AB84 @ =0xFFFFFE01
	ldrh r5, [r1]
	and r0, r5
	strh r0, [r1]
	ldr r0, _0804AB88 @ =0x00001B14
	add r3, r3, r0
	ldr r0, [r3]
	ldr r1, _0804AB8C @ =0xFFFE01FF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #2
	orr r0, r1
	str r0, [r3]
	mov r0, #8
	orr r2, r0
	mov r0, #0x10
	orr r2, r0
	strb r2, [r4]
_0804AB6C:
	mov r0, #1
_0804AB6E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0804AB78: .4byte 0x02018450
_0804AB7C: .4byte 0x020192E0
_0804AB80: .4byte 0x00001B16
_0804AB84: .4byte 0xFFFFFE01
_0804AB88: .4byte 0x00001B14
_0804AB8C: .4byte 0xFFFE01FF
	thumb_func_end sub_0804A99C

