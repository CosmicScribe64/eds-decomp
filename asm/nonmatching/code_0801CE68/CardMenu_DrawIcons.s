	thumb_func_start CardMenu_DrawIcons
CardMenu_DrawIcons: @ 0x0801D840
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r1, #0
	mov r5, #0
	ldr r4, _0801D8A0 @ =0x020192E0
	ldr r0, _0801D8A4 @ =0x0201CFB0
	mov ip, r0
	ldr r2, _0801D8A8 @ =0x00001B2C
	add r0, r4, r2
	ldr r0, [r0]
	lsl r2, r0, #6
	mov r3, #1
_0801D85E:
	lsr r0, r2, #0x10
	asr r0, r5
	and r0, r3
	cmp r0, #0
	beq _0801D86A
	add r1, #1
_0801D86A:
	add r5, #1
	cmp r5, #0xC
	ble _0801D85E
	ldr r7, _0801D8AC @ =0x00002624
	lsl r2, r1, #3
	mov r0, #0x78
	sub r6, r0, r2
	ldr r3, _0801D8A8 @ =0x00001B2C
	add r0, r4, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x1C
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #1
	mov r0, #0xA0
	sub r0, r0, r1
	mov r8, r0
	ldr r0, _0801D8B0 @ =0x00000828
	add r0, ip
	ldr r0, [r0]
	cmp r0, #0xC
	beq _0801D8B4
	cmp r0, #0xD
	bne _0801D8B8
	mov r0, #0xD8
	b _0801D8B6
_0801D8A0: .4byte 0x020192E0
_0801D8A4: .4byte 0x0201CFB0
_0801D8A8: .4byte 0x00001B2C
_0801D8AC: .4byte 0x00002624
_0801D8B0: .4byte 0x00000828
_0801D8B4:
	mov r0, #0x28
_0801D8B6:
	sub r6, r0, r2
_0801D8B8:
	ldr r4, _0801D920 @ =0x020192E0
	ldr r0, _0801D924 @ =0x00001B32
	add r2, r4, r0
	ldrh r3, [r2]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #2
	ldr r0, _0801D928 @ =0xFFFFFE03
	and r0, r3
	orr r0, r1
	strh r0, [r2]
	mov r5, #0
	ldr r1, _0801D92C @ =0x00001B2C
	add r4, r4, r1
	ldr r3, _0801D930 @ =0x081A4424
	mov sl, r3
	mov r9, r2
_0801D8E0:
	ldr r0, [r4]
	lsl r0, r0, #6
	lsr r0, r0, #0x10
	asr r0, r5
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _0801D944
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1C
	cmp r0, r5
	bne _0801D934
	mov r2, r8
	lsl r0, r2, #0x10
	orr r0, r6
	mov r3, r9
	ldrh r3, [r3]
	lsl r1, r3, #0x17
	lsr r1, r1, #0x1A
	mov r2, #0xF
	and r1, r2
	lsl r1, r1, #1
	add r1, sl
	ldrh r1, [r1]
	lsl r3, r1, #0x10
	mov r1, #0x40
	add r2, r7, #0
	bl AddAffineSprite
	b _0801D942
	.align 2, 0
_0801D920: .4byte 0x020192E0
_0801D924: .4byte 0x00001B32
_0801D928: .4byte 0xFFFFFE03
_0801D92C: .4byte 0x00001B2C
_0801D930: .4byte gPulseScaleCurve
_0801D934:
	mov r1, r8
	lsl r0, r1, #0x10
	orr r0, r6
	mov r1, #0x40
	add r2, r7, #0
	bl AddSprite
_0801D942:
	add r6, #0x10
_0801D944:
	add r0, r7, #4
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	add r5, #1
	cmp r5, #0xC
	ble _0801D8E0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end CardMenu_DrawIcons
	.align 2, 0

