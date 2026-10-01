	thumb_func_start sub_08053AF8
sub_08053AF8: @ 0x08053AF8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	ldr r5, _08053B1C @ =0x02017A40
	ldr r0, _08053B20 @ =0x0000053C
	add r0, r0, r5
	mov r8, r0
	ldr r0, [r0]
	lsl r6, r0, #0xC
	lsr r0, r6, #0x18
	cmp r0, #1
	beq _08053BC0
	cmp r0, #1
	bgt _08053B24
	cmp r0, #0
	beq _08053B32
	b _08053E4C
_08053B1C: .4byte 0x02017A40
_08053B20: .4byte 0x0000053C
_08053B24:
	cmp r0, #0xA
	bne _08053B2A
	b _08053D54
_08053B2A:
	cmp r0, #0x14
	bne _08053B30
	b _08053DD0
_08053B30:
	b _08053E4C
_08053B32:
	cmp r4, #0
	bne _08053B84
	ldr r0, _08053BA0 @ =0x00000206
	ldr r1, _08053BA4 @ =0x00000813
	ldr r3, _08053BA8 @ =0x080862C4
	mov r2, #0xB
	bl sub_080602A4
	ldr r1, _08053BAC @ =0x08053865
	ldr r2, _08053BB0 @ =0x080538C9
	mov r0, #5
	bl sub_08060308
	mov r3, #0xF
	ldr r2, _08053BB4 @ =0x0000053F
	add r1, r5, r2
	add r0, r3, #0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	lsr r1, r6, #0x1C
	mov r0, #0xA8
	lsl r0, r0, #3
	add r2, r5, r0
	and r1, r3
	mov r0, #0x10
	neg r0, r0
	ldrb r3, [r2]
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	bl sub_0805ED9C
	ldr r1, _08053BB8 @ =0x00000544
	add r0, r5, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r1, #1
	bl sub_0805F074
_08053B84:
	mov r3, r8
	ldr r2, [r3]
	lsl r1, r2, #0xC
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #0xC
	ldr r0, _08053BBC @ =0xFFF00FFF
	and r0, r2
	orr r0, r1
	str r0, [r3]
_08053B9C:
	mov r0, #0
	b _08053E4E
_08053BA0: .4byte 0x00000206
_08053BA4: .4byte 0x00000813
_08053BA8: .4byte gUnk_080862C4
_08053BAC: .4byte sub_08053864
_08053BB0: .4byte sub_080538C8
_08053BB4: .4byte 0x0000053F
_08053BB8: .4byte 0x00000544
_08053BBC: .4byte 0xFFF00FFF
_08053BC0:
	cmp r4, #0
	bne _08053BC6
	b _08053E4C
_08053BC6:
	add r0, r4, #0
	bl sub_080536D4
	ldr r0, _08053C0C @ =0x0000053F
	add r7, r5, r0
	ldrb r3, [r7]
	lsr r1, r3, #4
	mov r2, #0xA8
	lsl r2, r2, #3
	add r6, r5, r2
	mov r4, #0xF
	add r0, r4, #0
	ldrb r2, [r6]
	and r0, r2
	lsl r0, r0, #4
	orr r0, r1
	cmp r0, #0x1D
	bgt _08053C10
	add r2, r0, #1
	add r1, r2, #0
	and r1, r4
	lsl r1, r1, #4
	add r0, r4, #0
	and r0, r3
	orr r0, r1
	strb r0, [r7]
	lsr r2, r2, #4
	and r2, r4
	mov r0, #0x10
	neg r0, r0
	ldrb r3, [r6]
	and r0, r3
	orr r0, r2
	strb r0, [r6]
	b _08053B9C
_08053C0C: .4byte 0x0000053F
_08053C10:
	add r0, r4, #0
	and r0, r3
	strb r0, [r7]
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r6]
	and r0, r1
	strb r0, [r6]
	mov r2, r8
	ldrb r2, [r2]
	cmp r2, #3
	bhi _08053CA8
	mov r3, r8
	ldrb r3, [r3]
	lsl r2, r3, #2
	ldr r1, _08053C90 @ =0x00000544
	add r0, r5, r1
	add r0, r2, r0
	ldr r1, [r0]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r3, #0xA9
	lsl r3, r3, #3
	add r0, r5, r3
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldr r6, _08053C94 @ =0x000007FF
	and r1, r6
	lsl r1, r1, #1
	ldr r0, _08053C98 @ =0x08622AB4
	add r1, r1, r0
	ldrh r1, [r1]
	mov r0, #1
	bl sub_08056300
	cmp r0, #0
	bne _08053CA8
	and r4, r6
	lsl r0, r4, #1
	ldr r1, _08053C98 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #1
	bl sub_08056300
	cmp r0, #0
	beq _08053CA8
	mov r2, r8
	ldr r0, [r2]
	ldr r1, _08053C9C @ =0xFFF00FFF
	and r0, r1
	mov r1, #0xA0
	lsl r1, r1, #9
	orr r0, r1
	str r0, [r2]
	ldr r3, _08053CA0 @ =0x0000053E
	add r1, r5, r3
	ldr r0, _08053CA4 @ =0xFFFFF00F
	ldrh r2, [r1]
	and r0, r2
	b _08053D12
	.align 2, 0
_08053C90: .4byte 0x00000544
_08053C94: .4byte 0x000007FF
_08053C98: .4byte gUnk_08622AB4
_08053C9C: .4byte 0xFFF00FFF
_08053CA0: .4byte 0x0000053E
_08053CA4: .4byte 0xFFFFF00F
_08053CA8:
	ldr r7, _08053D18 @ =0x02017A40
	ldr r3, _08053D1C @ =0x0000053C
	add r6, r7, r3
	ldrb r0, [r6]
	cmp r0, #0
	beq _08053D38
	ldrb r0, [r6]
	lsl r2, r0, #2
	ldr r1, _08053D20 @ =0x00000544
	add r0, r7, r1
	add r0, r2, r0
	ldr r1, [r0]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r3, #4
	add r0, r7, r3
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldr r5, _08053D24 @ =0x000007FF
	and r1, r5
	lsl r1, r1, #1
	ldr r0, _08053D28 @ =0x08622AB4
	add r1, r1, r0
	ldrh r1, [r1]
	mov r0, #1
	bl sub_08056300
	cmp r0, #0
	beq _08053D38
	and r4, r5
	lsl r0, r4, #1
	ldr r1, _08053D28 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #1
	bl sub_08056300
	cmp r0, #0
	bne _08053D38
	ldr r0, [r6]
	ldr r1, _08053D2C @ =0xFFF00FFF
	and r0, r1
	mov r1, #0xA0
	lsl r1, r1, #8
	orr r0, r1
	str r0, [r6]
	ldr r2, _08053D30 @ =0x0000053E
	add r1, r7, r2
	ldr r0, _08053D34 @ =0xFFFFF00F
	ldrh r3, [r1]
	and r0, r3
_08053D12:
	strh r0, [r1]
	b _08053B9C
	.align 2, 0
_08053D18: .4byte 0x02017A40
_08053D1C: .4byte 0x0000053C
_08053D20: .4byte 0x00000544
_08053D24: .4byte 0x000007FF
_08053D28: .4byte gUnk_08622AB4
_08053D2C: .4byte 0xFFF00FFF
_08053D30: .4byte 0x0000053E
_08053D34: .4byte 0xFFFFF00F
_08053D38:
	ldr r0, _08053D4C @ =0x02017A40
	ldr r2, _08053D50 @ =0x0000053C
	add r1, r0, r2
	ldrb r0, [r1]
	cmp r0, #3
	bls _08053D46
	b _08053E4C
_08053D46:
	add r0, #1
	strb r0, [r1]
	b _08053B9C
_08053D4C: .4byte 0x02017A40
_08053D50: .4byte 0x0000053C
_08053D54:
	cmp r4, #0
	bne _08053D5A
	b _08053B9C
_08053D5A:
	ldr r3, _08053D8C @ =0x0000053E
	add r6, r5, r3
	ldrh r1, [r6]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x18
	cmp r0, #0xF
	bgt _08053D94
	mov r3, r8
	ldrb r2, [r3]
	sub r1, r2, #1
	add r0, r4, #0
	bl sub_08053770
	ldrh r2, [r6]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _08053D90 @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r6]
	b _08053B9C
_08053D8C: .4byte 0x0000053E
_08053D90: .4byte 0xFFFFF00F
_08053D94:
	mov r0, r8
	ldrb r0, [r0]
	lsl r1, r0, #2
	mov r2, #0xA8
	lsl r2, r2, #3
	add r0, r5, r2
	add r0, r1, r0
	ldr r3, _08053DC8 @ =0x00000544
	add r2, r5, r3
	add r1, r1, r2
	bl sub_08007560
	mov r1, r8
	ldr r0, [r1]
	ldr r1, _08053DCC @ =0xFFF00FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #5
	orr r0, r1
	mov r2, r8
	str r0, [r2]
	add r0, r4, #0
	bl sub_080536D4
	b _08053B9C
	.align 2, 0
_08053DC8: .4byte 0x00000544
_08053DCC: .4byte 0xFFF00FFF
_08053DD0:
	cmp r4, #0
	bne _08053DD6
	b _08053B9C
_08053DD6:
	ldr r3, _08053E08 @ =0x0000053E
	add r6, r5, r3
	ldrh r1, [r6]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x18
	cmp r0, #0xF
	bgt _08053E10
	mov r2, r8
	ldrb r1, [r2]
	add r2, r1, #1
	add r0, r4, #0
	bl sub_08053770
	ldrh r2, [r6]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x18
	add r1, #1
	mov r0, #0xFF
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _08053E0C @ =0xFFFFF00F
	and r0, r2
	orr r0, r1
	strh r0, [r6]
	b _08053B9C
_08053E08: .4byte 0x0000053E
_08053E0C: .4byte 0xFFFFF00F
_08053E10:
	mov r3, r8
	ldrb r3, [r3]
	lsl r1, r3, #2
	mov r2, #0xA9
	lsl r2, r2, #3
	add r0, r5, r2
	add r0, r1, r0
	ldr r3, _08053E44 @ =0x00000544
	add r2, r5, r3
	add r1, r1, r2
	bl sub_08007560
	mov r1, r8
	ldr r0, [r1]
	ldr r1, _08053E48 @ =0xFFF00FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #5
	orr r0, r1
	mov r2, r8
	str r0, [r2]
	add r0, r4, #0
	bl sub_080536D4
	b _08053B9C
	.align 2, 0
_08053E44: .4byte 0x00000544
_08053E48: .4byte 0xFFF00FFF
_08053E4C:
	mov r0, #1
_08053E4E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08053AF8

