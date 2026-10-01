	thumb_func_start sub_0805B884
sub_0805B884: @ 0x0805B884
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _0805B8A0 @ =0x02015EF0
	ldrb r4, [r0, #0xA]
	mov sl, r0
	cmp r4, #0
	beq _0805B8A4
	cmp r4, #1
	beq _0805B8B6
	mov r0, #1
	b _0805BB5C
_0805B8A0: .4byte 0x02015EF0
_0805B8A4:
	bl sub_08059780
	mov r0, sl
	strb r4, [r0, #6]
	ldrb r0, [r0, #0xA]
	add r0, #1
	mov r1, sl
	strb r0, [r1, #0xA]
	b _0805BB5A
_0805B8B6:
	ldr r1, _0805B8CC @ =0x020192E4
	ldr r2, _0805B8D0 @ =0x00000D6B
	add r1, r1, r2
	mov r0, #0x20
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0805B8FC
	mov r0, #1
	b _0805BB5C
	.align 2, 0
_0805B8CC: .4byte 0x020192E4
_0805B8D0: .4byte 0x00000D6B
_0805B8D4:
	ldrb r0, [r5, #6]
	add r0, #1
	strb r0, [r5, #6]
	b _0805BB5A
_0805B8DC:
	ldrb r1, [r5, #6]
	mov r0, #1
	bl sub_08055EB0
	ldrb r0, [r5, #6]
	add r0, #1
	strb r0, [r5, #6]
	b _0805BB5A
_0805B8EC:
	ldrb r1, [r4, #6]
	mov r0, #1
	bl sub_08055EB0
	ldrb r0, [r4, #6]
	add r0, #1
	strb r0, [r4, #6]
	b _0805BB5A
_0805B8FC:
	mov r0, sl
	ldrb r0, [r0, #6]
	cmp r0, #4
	bls _0805B906
	b _0805BB52
_0805B906:
	ldr r0, _0805BA20 @ =0x02015EF0
	ldrb r3, [r0, #6]
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0805BA24 @ =0x0201A070
	add r2, r2, r0
	ldr r1, [r2]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	neg r0, r1
	orr r0, r1
	lsr r4, r0, #0x1F
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0805B954
	mov r0, #1
	add r1, r3, #0
	bl sub_0800C8BC
	cmp r0, #1
	bne _0805B954
	mov r0, #0
	mov r1, #0xA4
	lsl r1, r1, #1
	bl sub_08008524
	cmp r0, #0
	bne _0805B952
	mov r0, #1
	mov r1, #0xA4
	lsl r1, r1, #1
	bl sub_08008524
	cmp r0, #0
	beq _0805B954
_0805B952:
	mov r4, #0
_0805B954:
	ldr r5, _0805BA20 @ =0x02015EF0
	ldrb r1, [r5, #6]
	mov r0, #1
	mov r2, #0xAE
	lsl r2, r2, #1
	bl sub_0800A78C
	cmp r0, #0
	beq _0805B968
	mov r4, #0
_0805B968:
	ldrb r1, [r5, #6]
	mov r0, #1
	ldr r2, _0805BA28 @ =0x000004DC
	bl sub_0800A78C
	cmp r0, #0
	beq _0805B978
	mov r4, #0
_0805B978:
	cmp r4, #0
	beq _0805B8D4
	ldrb r1, [r5, #6]
	mov r0, #1
	bl sub_0805763C
	cmp r0, #0
	bne _0805B98A
	b _0805BB40
_0805B98A:
	mov r0, #1
	bl sub_08047114
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	ldrb r1, [r5, #6]
	mov r0, #1
	bl sub_0800C894
	add r7, r0, #0
	mov r6, #0x94
	ldrb r1, [r5, #6]
	add r0, r1, #0
	mul r0, r6
	ldr r4, _0805BA24 @ =0x0201A070
	add r2, r0, r4
	ldrb r1, [r2, #6]
	mov r3, #2
	mov r0, #2
	mov r8, r0
	and r0, r1
	cmp r0, #0
	bne _0805B9DE
	add r0, r1, #0
	orr r0, r3
	strb r0, [r2, #6]
	ldrb r1, [r5, #6]
	mov r0, #1
	bl sub_0800C894
	add r7, r0, #0
	ldrb r2, [r5, #6]
	add r1, r2, #0
	mul r1, r6
	add r1, r1, r4
	mov r2, #3
	neg r2, r2
	add r0, r2, #0
	ldrb r2, [r1, #6]
	and r0, r2
	strb r0, [r1, #6]
_0805B9DE:
	ldrb r1, [r5, #6]
	add r0, r1, #0
	mul r0, r6
	add r3, r0, r4
	ldr r0, [r3]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r2, _0805BA2C @ =0x08622AB4
	add r0, r0, r2
	ldrh r2, [r0]
	mov r0, #0xFA
	lsl r0, r0, #1
	cmp r2, r0
	beq _0805BA34
	ldr r0, _0805BA30 @ =0x000002FA
	cmp r2, r0
	bne _0805BA50
	mov r0, r8
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	bne _0805BA50
	mov r0, r9
	cmp r0, #0
	beq _0805BA50
	mov r0, #1
	bl sub_08055EB0
	ldrb r0, [r5, #6]
	add r0, #1
	strb r0, [r5, #6]
	b _0805BB5A
	.align 2, 0
_0805BA20: .4byte 0x02015EF0
_0805BA24: .4byte 0x0201A070
_0805BA28: .4byte 0x000004DC
_0805BA2C: .4byte gUnk_08622AB4
_0805BA30: .4byte 0x000002FA
_0805BA34:
	mov r0, r8
	ldrb r3, [r3, #6]
	and r0, r3
	cmp r0, #0
	bne _0805BA50
	mov r0, #0
	bl sub_08008860
	cmp r0, #0
	ble _0805BA50
	mov r1, r9
	cmp r1, #0
	beq _0805BA50
	b _0805B8DC
_0805BA50:
	add r0, r7, #0
	bl sub_08057C50
	cmp r0, #0
	beq _0805BACC
	cmp r7, #0
	ble _0805BACC
	ldr r4, _0805BAC0 @ =0x02015EF0
	mov r0, #0x94
	ldrb r2, [r4, #6]
	add r1, r2, #0
	mul r1, r0
	ldr r0, _0805BAC4 @ =0x0201A070
	add r1, r1, r0
	mov r0, #3
	ldrb r2, [r1, #6]
	and r0, r2
	cmp r0, #1
	bne _0805BA92
	ldr r0, [r1]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0805BAC8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl sub_08007590
	cmp r0, #0
	bne _0805BA92
	mov r2, r9
	cmp r2, #0
	beq _0805BA92
	b _0805B8EC
_0805BA92:
	ldr r4, _0805BAC0 @ =0x02015EF0
	ldrb r2, [r4, #6]
	mov r0, #0x94
	add r1, r2, #0
	mul r1, r0
	ldr r0, _0805BAC4 @ =0x0201A070
	add r1, r1, r0
	mov r0, #3
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #3
	bne _0805BB40
	mov r0, #1
	add r1, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_08018ED8
	ldrb r0, [r4, #6]
	add r0, #1
	strb r0, [r4, #6]
	b _0805BB5A
	.align 2, 0
_0805BAC0: .4byte 0x02015EF0
_0805BAC4: .4byte 0x0201A070
_0805BAC8: .4byte gUnk_08622AB4
_0805BACC:
	ldr r1, _0805BB0C @ =0x02015EF0
	mov r0, #0x94
	ldrb r1, [r1, #6]
	mul r1, r0
	ldr r0, _0805BB10 @ =0x0201A070
	add r2, r1, r0
	mov r0, #3
	ldrb r1, [r2, #6]
	and r0, r1
	cmp r0, #2
	bne _0805BB40
	ldr r0, _0805BB14 @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #0x80
	lsl r1, r1, #2
	and r0, r1
	cmp r0, #0
	beq _0805BB40
	ldr r0, [r2]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r2, _0805BB18 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0805BB1C @ =0x0000023D
	cmp r1, r0
	beq _0805BB26
	cmp r1, r0
	bgt _0805BB20
	cmp r1, #0x2F
	beq _0805BB26
	b _0805BB40
_0805BB0C: .4byte 0x02015EF0
_0805BB10: .4byte 0x0201A070
_0805BB14: .4byte 0x02015EE8
_0805BB18: .4byte gUnk_08622AB4
_0805BB1C: .4byte 0x0000023D
_0805BB20:
	ldr r0, _0805BB6C @ =0x00000463
	cmp r1, r0
	bne _0805BB40
_0805BB26:
	ldr r0, _0805BB70 @ =0x020192E4
	ldr r1, _0805BB74 @ =0x00000D64
	add r0, r0, r1
	ldr r1, _0805BB78 @ =0x00000BB8
	ldrh r0, [r0]
	cmp r0, r1
	bls _0805BB40
	bl sub_080577FC
	cmp r0, #0
	ble _0805BB40
	bl sub_08057854
_0805BB40:
	ldr r1, _0805BB7C @ =0x02015EF0
	ldrb r0, [r1, #6]
	add r0, #1
	strb r0, [r1, #6]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #4
	bhi _0805BB52
	b _0805B906
_0805BB52:
	mov r2, sl
	ldrb r0, [r2, #0xA]
	add r0, #1
	strb r0, [r2, #0xA]
_0805BB5A:
	mov r0, #0
_0805BB5C:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0805BB6C: .4byte 0x00000463
_0805BB70: .4byte 0x020192E4
_0805BB74: .4byte 0x00000D64
_0805BB78: .4byte 0x00000BB8
_0805BB7C: .4byte 0x02015EF0
	thumb_func_end sub_0805B884

