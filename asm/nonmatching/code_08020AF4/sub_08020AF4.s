	thumb_func_start sub_08020AF4
sub_08020AF4: @ 0x08020AF4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x12C
	ldr r0, _08020B24 @ =0x02017A40
	ldr r1, _08020B28 @ =0x000003D2
	add r6, r0, r1
	ldrb r2, [r6]
	lsr r5, r2, #1
	add r7, r0, #0
	cmp r5, #3
	bne _08020B10
	b _08020C30
_08020B10:
	cmp r5, #3
	bgt _08020B2C
	cmp r5, #1
	beq _08020BF8
	cmp r5, #1
	bgt _08020C14
	cmp r5, #0
	beq _08020B4A
	bl _0802137A @ far jump
_08020B24: .4byte 0x02017A40
_08020B28: .4byte 0x000003D2
_08020B2C:
	cmp r5, #5
	bne _08020B32
	b _0802118C
_08020B32:
	cmp r5, #5
	bge _08020B38
	b _08020C84
_08020B38:
	cmp r5, #6
	bne _08020B3E
	b _08021244
_08020B3E:
	cmp r5, #0x64
	bne _08020B46
	bl _08021360 @ far jump
_08020B46:
	bl _0802137A @ far jump
_08020B4A:
	ldr r1, _08020BDC @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08020BC2
	ldr r0, _08020BE0 @ =0x0000F061
	mov r2, #0xF0
	lsl r2, r2, #2
	add r4, r7, r2
	ldrh r1, [r4]
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	mov r6, #0
	ldrh r4, [r4]
	cmp r5, r4
	bge _08020B9E
	mov r3, #0xF0
	lsl r3, r3, #2
	add r5, r7, r3
	mov r0, #0xA0
	lsl r0, r0, #2
	add r4, r7, r0
_08020B7C:
	mov r0, sp
	strh r6, [r0]
	add r0, #2
	add r1, r4, #0
	mov r2, #0x14
	bl sub_08075294
	ldr r0, _08020BE4 @ =0x0000F062
	mov r1, sp
	mov r2, #0x16
	bl sub_080229BC
	add r4, #0x14
	add r6, #1
	ldrh r1, [r5]
	cmp r6, r1
	blt _08020B7C
_08020B9E:
	ldr r0, _08020BE8 @ =0x0000F064
	ldr r1, _08020BEC @ =0x02017A40
	mov r2, #0xF0
	lsl r2, r2, #2
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	mov r3, #0
	bl sub_0802297C
	ldr r1, _08020BF0 @ =0x02017FB0
	mov r3, #0xC2
	lsl r3, r3, #2
	add r1, r1, r3
	mov r0, #0x7F
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
_08020BC2:
	ldr r2, _08020BEC @ =0x02017A40
	ldr r0, _08020BF4 @ =0x000003D2
	add r2, r2, r0
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _080213A0
	.align 2, 0
_08020BDC: .4byte 0x02015EE8
_08020BE0: .4byte 0x0000F061
_08020BE4: .4byte 0x0000F062
_08020BE8: .4byte 0x0000F064
_08020BEC: .4byte 0x02017A40
_08020BF0: .4byte 0x02017FB0
_08020BF4: .4byte 0x000003D2
_08020BF8:
	mov r1, #0xA0
	lsl r1, r1, #2
	add r0, r7, r1
	mov r1, #1
	bl sub_0801A7B4
	ldrb r1, [r6]
	lsr r0, r1, #1
	add r0, #1
	lsl r0, r0, #1
	and r5, r1
	orr r5, r0
	strb r5, [r6]
	b _080213A0
_08020C14:
	bl sub_0801A32C
	cmp r0, #0
	bne _08020C1E
	b _080213A0
_08020C1E:
	ldrb r2, [r6]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r6]
	b _080213A0
_08020C30:
	ldr r1, _08020C74 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _08020C4E
	ldr r0, _08020C78 @ =0x02017FB0
	mov r2, #0xC2
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	lsr r0, r0, #7
	cmp r0, #0
	bne _08020C4E
	b _080213A0
_08020C4E:
	mov r0, #0x12
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r2, _08020C7C @ =0x02017A40
	ldr r3, _08020C80 @ =0x000003D2
	add r2, r2, r3
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _080213A0
	.align 2, 0
_08020C74: .4byte 0x02015EE8
_08020C78: .4byte 0x02017FB0
_08020C7C: .4byte 0x02017A40
_08020C80: .4byte 0x000003D2
_08020C84:
	mov r4, #0xF0
	lsl r4, r4, #2
	add r4, r4, r7
	mov r8, r4
	ldrh r1, [r4]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r7
	mov r5, #0xA0
	lsl r5, r5, #2
	add r0, r0, r5
	ldrh r0, [r0]
	bl sub_08047058
	ldr r2, _08020CC0 @ =0x000003D6
	add r1, r7, r2
	strh r0, [r1]
	lsl r0, r0, #0x10
	cmp r0, #0
	bge _08020CC4
	mov r0, #1
	ldrb r3, [r6]
	and r0, r3
	mov r1, #0xC8
	orr r0, r1
	strb r0, [r6]
	b _080213A0
	.align 2, 0
_08020CC0: .4byte 0x000003D6
_08020CC4:
	mov r4, #0xF6
	lsl r4, r4, #2
	add r3, r7, r4
	ldr r2, _08020CF0 @ =0x0819A9D4
	mov r0, #0
	ldsh r1, [r1, r0]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #3
	add r2, #4
	add r0, r0, r2
	ldr r0, [r0]
	str r0, [r3]
	cmp r0, #0
	bne _08020CF4
	mov r0, #1
	ldrb r1, [r6]
	and r0, r1
	mov r1, #0xC8
	orr r0, r1
	strb r0, [r6]
	b _080213A0
_08020CF0: .4byte gUnk_0819A9D4
_08020CF4:
	mov r2, r8
	ldrh r1, [r2]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r2, r0, r7
	add r0, r2, r5
	ldr r3, _08020D70 @ =0x000007FF
	mov r9, r3
	mov r1, r9
	ldrh r0, [r0]
	and r1, r0
	lsl r0, r1, #2
	ldr r4, _08020D74 @ =0x08621DE0
	add r0, r0, r4
	ldr r3, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r3
	lsr r4, r0, #0x14
	cmp r4, #0x14
	bhi _08020D24
	b _08020F18
_08020D24:
	mov r5, #0
	ldr r0, _08020D78 @ =0x00000282
	add r1, r2, r0
	ldrb r2, [r1]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1D
	cmp r0, #3
	beq _08020D5C
	lsl r2, r2, #0x1F
	lsr r2, r2, #0x1F
	ldrh r1, [r1]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x1A
	mov r1, #0x94
	mul r1, r0
	ldr r0, _08020D7C @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08020D80 @ =0x0201930C
	add r1, r1, r0
	add r1, #0x91
	mov r0, #8
	ldrb r1, [r1]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	neg r0, r0
	lsr r5, r0, #0x1F
_08020D5C:
	cmp r4, #0x16
	bgt _08020D84
	cmp r4, #0x15
	blt _08020D84
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r3, r0
	lsr r0, r3, #0x11
	b _08020D86
	.align 2, 0
_08020D70: .4byte 0x000007FF
_08020D74: .4byte gUnk_08621DE0
_08020D78: .4byte 0x00000282
_08020D7C: .4byte 0x00000D64
_08020D80: .4byte 0x0201930C
_08020D84:
	mov r0, #0
_08020D86:
	add r1, r0, #0
	cmp r1, #2
	beq _08020DA8
	cmp r1, #3
	bne _08020DBA
	ldr r0, _08020DA0 @ =0x020192E0
	ldr r2, _08020DA4 @ =0x00001ACD
	add r0, r0, r2
	ldrb r0, [r0]
	and r1, r0
	cmp r1, #0
	beq _08020DBA
	b _08020DB8
_08020DA0: .4byte 0x020192E0
_08020DA4: .4byte 0x00001ACD
_08020DA8:
	ldr r1, _08020DF4 @ =0x020192E0
	ldr r3, _08020DF8 @ =0x00001ACD
	add r1, r1, r3
	mov r0, #4
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08020DBA
_08020DB8:
	mov r5, #1
_08020DBA:
	ldr r2, _08020DFC @ =0x02017A40
	mov r4, #0xF0
	lsl r4, r4, #2
	add r0, r2, r4
	ldrh r0, [r0]
	sub r0, #1
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r2
	mov r0, #0xA0
	lsl r0, r0, #2
	add r1, r1, r0
	ldr r0, _08020E00 @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08020E04 @ =0x08621DE0
	add r0, r0, r1
	ldr r3, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08020E08
	cmp r0, #0x16
	beq _08020E3C
	b _08020E6A
_08020DF4: .4byte 0x020192E0
_08020DF8: .4byte 0x00001ACD
_08020DFC: .4byte 0x02017A40
_08020E00: .4byte 0x000007FF
_08020E04: .4byte gUnk_08621DE0
_08020E08:
	ldr r2, _08020E30 @ =0x020192E0
	ldr r4, _08020E34 @ =0x00001ACC
	add r1, r2, r4
	mov r0, #0x80
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08020E1A
	mov r5, #1
_08020E1A:
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r3, r0
	lsr r0, r3, #0x11
	cmp r0, #4
	bne _08020E6A
	ldr r0, _08020E38 @ =0x00001ACD
	add r1, r2, r0
	mov r0, #0x10
	b _08020E60
	.align 2, 0
_08020E30: .4byte 0x020192E0
_08020E34: .4byte 0x00001ACC
_08020E38: .4byte 0x00001ACD
_08020E3C:
	ldr r2, _08020EF4 @ =0x020192E0
	ldr r4, _08020EF8 @ =0x00001ACC
	add r1, r2, r4
	mov r0, #0x40
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08020E4E
	mov r5, #1
_08020E4E:
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r3, r0
	lsr r0, r3, #0x11
	cmp r0, #4
	bne _08020E6A
	ldr r0, _08020EFC @ =0x00001ACD
	add r1, r2, r0
	mov r0, #8
_08020E60:
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08020E6A
	mov r5, #1
_08020E6A:
	ldr r4, _08020F00 @ =0x02017A40
	mov r1, #0xF0
	lsl r1, r1, #2
	add r6, r4, r1
	ldrh r1, [r6]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r2, r0, r4
	mov r3, #0xA0
	lsl r3, r3, #2
	add r1, r2, r3
	ldr r0, _08020F04 @ =0x000007FF
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08020F08 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08020F0C @ =0x00000603
	ldrh r0, [r0]
	cmp r0, r1
	bne _08020EA8
	mov r5, #0
	add r3, #4
	add r1, r2, r3
	mov r0, #5
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_08020EA8:
	cmp r5, #0
	beq _08020F80
	ldrh r0, [r6]
	sub r0, #1
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r4
	ldr r3, _08020F10 @ =0x00000282
	add r1, r1, r3
	ldrb r2, [r1]
	lsl r0, r2, #0x1F
	mov r2, #0xB1
	cmp r0, #0
	beq _08020EC8
	ldr r2, _08020F14 @ =0x000080B1
_08020EC8:
	ldrh r1, [r1]
	lsl r1, r1, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #1
	mov r3, #0
	bl sub_0801EC58
	ldrh r1, [r6]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r4
	mov r3, #0xA1
	lsl r3, r3, #2
	add r0, r0, r3
	mov r1, #4
	ldrb r4, [r0]
	orr r1, r4
	strb r1, [r0]
	b _08020F80
_08020EF4: .4byte 0x020192E0
_08020EF8: .4byte 0x00001ACC
_08020EFC: .4byte 0x00001ACD
_08020F00: .4byte 0x02017A40
_08020F04: .4byte 0x000007FF
_08020F08: .4byte gUnk_08622AB4
_08020F0C: .4byte 0x00000603
_08020F10: .4byte 0x00000282
_08020F14: .4byte 0x000080B1
_08020F18:
	lsl r0, r1, #1
	ldr r1, _08020F7C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	mov r1, #1
	bl sub_08007590
	cmp r0, #0
	bne _08020F52
	mov r2, r8
	ldrh r0, [r2]
	sub r0, #1
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r7
	add r1, r1, r5
	mov r0, r9
	ldrh r1, [r1]
	and r0, r1
	lsl r0, r0, #1
	ldr r3, _08020F7C @ =0x08622AB4
	add r0, r0, r3
	ldrh r0, [r0]
	mov r1, #0
	bl sub_08007590
	cmp r0, #0
	beq _08020F80
_08020F52:
	mov r4, r8
	ldrh r0, [r4]
	sub r0, #1
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r7
	mov r0, #0xA1
	lsl r0, r0, #2
	add r1, r1, r0
	ldrb r1, [r1]
	lsl r0, r1, #0x1D
	cmp r0, #0
	bge _08020F80
	mov r0, #1
	ldrb r1, [r6]
	and r0, r1
	mov r1, #0xC8
	orr r0, r1
	strb r0, [r6]
	b _080213A0
_08020F7C: .4byte gUnk_08622AB4
_08020F80:
	ldr r4, _08021080 @ =0x02017E1C
	ldr r2, _08021084 @ =0xFFFFFC24
	add r6, r4, r2
	add r5, r4, #0
	sub r5, #0x1C
	ldrh r1, [r5]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r6
	ldr r3, _08021088 @ =0x00000282
	add r0, r0, r3
	ldrb r2, [r0]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	ldr r2, _0802108C @ =0x00000D64
	mul r1, r2
	ldr r2, _08021090 @ =0x0201930C
	add r1, r1, r2
	ldrh r0, [r0]
	lsl r0, r0, #0x16
	lsr r0, r0, #0x1A
	mov r2, #0x94
	mul r0, r2
	add r1, r1, r0
	add r0, r4, #0
	bl sub_08007558
	ldrh r3, [r5]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	ldr r1, _08021094 @ =0xFFFFFE90
	add r4, r4, r1
	add r0, r0, r4
	bl sub_0801FCA8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08020FFE
	ldrh r0, [r5]
	sub r0, #1
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r6
	ldr r2, _08021088 @ =0x00000282
	add r1, r1, r2
	ldrb r3, [r1]
	lsl r0, r3, #0x1F
	mov r2, #0x78
	cmp r0, #0
	beq _08020FEE
	ldr r2, _08021098 @ =0x00008078
_08020FEE:
	ldrh r1, [r1]
	lsl r1, r1, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08020FFE:
	ldr r2, _0802109C @ =0x02017A40
	mov r4, #0xF0
	lsl r4, r4, #2
	add r4, r4, r2
	mov r8, r4
	ldrh r1, [r4]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r3, r0, r2
	mov r1, #0xA1
	lsl r1, r1, #2
	add r0, r3, r1
	ldrb r1, [r0]
	lsl r0, r1, #0x1C
	add r7, r2, #0
	cmp r0, #0
	blt _08021026
	b _0802115C
_08021026:
	lsl r0, r1, #0x1D
	cmp r0, #0
	blt _0802102E
	b _0802115C
_0802102E:
	mov r2, #0xF7
	lsl r2, r2, #2
	add r6, r7, r2
	ldr r2, [r6]
	lsl r0, r2, #0x14
	lsr r0, r0, #0x14
	ldr r4, _080210A0 @ =0x000007FF
	mov r9, r4
	and r0, r4
	lsl r0, r0, #1
	ldr r4, _080210A4 @ =0x08622AB4
	add r0, r0, r4
	ldr r1, _080210A8 @ =0x00000412
	ldrh r0, [r0]
	cmp r0, r1
	bne _080210B0
	lsl r0, r2, #0x13
	lsr r0, r0, #0x1F
	mov r1, #1
	bl sub_08046C20
	ldr r1, [r6]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	mov r0, #0
	bl sub_080197E0
	ldr r2, [r6]
	lsl r0, r2, #0x13
	mov r3, #0x6A
	cmp r0, #0
	bge _08021070
	ldr r3, _080210AC @ =0x0000806A
_08021070:
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	b _0802112E
_08021080: .4byte 0x02017E1C
_08021084: .4byte 0xFFFFFC24
_08021088: .4byte 0x00000282
_0802108C: .4byte 0x00000D64
_08021090: .4byte 0x0201930C
_08021094: .4byte 0xFFFFFE90
_08021098: .4byte 0x00008078
_0802109C: .4byte 0x02017A40
_080210A0: .4byte 0x000007FF
_080210A4: .4byte gUnk_08622AB4
_080210A8: .4byte 0x00000412
_080210AC: .4byte 0x0000806A
_080210B0:
	ldr r5, _08021144 @ =0x00000282
	add r0, r3, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	mov r3, #0x7C
	cmp r0, #0
	beq _080210C0
	ldr r3, _08021148 @ =0x0000807C
_080210C0:
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, [r6]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r4
	ldr r1, _0802114C @ =0x0000014D
	ldrh r0, [r0]
	cmp r0, r1
	bne _08021122
	mov r2, r8
	ldrh r1, [r2]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r7
	add r0, r0, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r4, _08021150 @ =0x0000058F
	add r1, r4, #0
	bl sub_080184D8
	mov r3, r8
	ldrh r1, [r3]
	sub r1, #1
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r0, r7
	add r0, r0, r5
	ldrb r0, [r0]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	add r1, r4, #0
	bl sub_080184D8
_08021122:
	ldr r0, [r6]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	mov r1, #1
	bl sub_08046C20
_0802112E:
	ldr r0, _08021154 @ =0x02017A40
	ldr r4, _08021158 @ =0x000003D2
	add r0, r0, r4
	mov r1, #1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0xC8
	orr r1, r2
	strb r1, [r0]
	mov r0, #0
	b _080213A2
_08021144: .4byte 0x00000282
_08021148: .4byte 0x0000807C
_0802114C: .4byte 0x0000014D
_08021150: .4byte 0x0000058F
_08021154: .4byte 0x02017A40
_08021158: .4byte 0x000003D2
_0802115C:
	mov r3, #0xF8
	lsl r3, r3, #2
	add r1, r7, r3
	mov r2, #0
	mov r0, #0x80
	strb r0, [r1]
	ldr r4, _08021184 @ =0x000003E1
	add r0, r7, r4
	strb r2, [r0]
	ldr r0, _08021188 @ =0x000003D2
	add r3, r7, r0
	ldrb r2, [r3]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	b _080213A0
_08021184: .4byte 0x000003E1
_08021188: .4byte 0x000003D2
_0802118C:
	mov r1, #0xF0
	lsl r1, r1, #2
	add r4, r7, r1
	ldrh r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	mov r3, #0x9B
	lsl r3, r3, #2
	add r5, r7, r3
	add r0, r0, r5
	bl sub_0801FA48
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802120A
	add r1, sp, #0x100
	ldrh r0, [r4]
	strh r0, [r1]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #1
	bls _080211EA
	mov r0, #1
	strh r0, [r1, #2]
	add r0, sp, #0x104
	ldrh r2, [r4]
	lsl r1, r2, #2
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r5
	mov r2, #0x14
	bl sub_08075294
	add r0, sp, #0x118
	ldrh r3, [r4]
	lsl r1, r3, #2
	add r1, r1, r3
	lsl r1, r1, #2
	mov r4, #0x96
	lsl r4, r4, #2
	add r2, r7, r4
	add r1, r1, r2
	mov r2, #0x14
	bl sub_08075294
	b _08021200
_080211EA:
	mov r0, #0
	strh r0, [r1, #2]
	add r0, sp, #0x104
	ldrh r2, [r4]
	lsl r1, r2, #2
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r5
	mov r2, #0x14
	bl sub_08075294
_08021200:
	ldr r0, _08021234 @ =0x0000F071
	add r1, sp, #0x100
	mov r2, #0x2C
	bl sub_080229BC
_0802120A:
	ldr r1, _08021238 @ =0x02017FB0
	mov r3, #0xC2
	lsl r3, r3, #2
	add r1, r1, r3
	mov r0, #0x21
	neg r0, r0
	ldrb r4, [r1]
	and r0, r4
	strb r0, [r1]
	ldr r2, _0802123C @ =0x02017A40
	ldr r0, _08021240 @ =0x000003D2
	add r2, r2, r0
	ldrb r3, [r2]
	lsr r1, r3, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	b _080213A0
_08021234: .4byte 0x0000F071
_08021238: .4byte 0x02017FB0
_0802123C: .4byte 0x02017A40
_08021240: .4byte 0x000003D2
_08021244:
	mov r1, #0xF0
	lsl r1, r1, #2
	add r4, r7, r1
	ldrh r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	mov r3, #0x9B
	lsl r3, r3, #2
	add r5, r7, r3
	add r0, r0, r5
	bl sub_0801FA48
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _080212CC
	ldrh r0, [r4]
	cmp r0, #1
	bls _0802128E
	mov r1, #0xF6
	lsl r1, r1, #2
	add r3, r7, r1
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r0, r1, r5
	mov r4, #0x96
	lsl r4, r4, #2
	add r2, r7, r4
	add r1, r1, r2
	ldr r2, [r3]
	bl _call_via_r2
	mov r2, #0xF8
	lsl r2, r2, #2
	add r1, r7, r2
	b _080212AC
_0802128E:
	mov r3, #0xF6
	lsl r3, r3, #2
	add r1, r7, r3
	ldrh r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r2
	lsl r0, r0, #2
	add r0, r0, r5
	ldr r2, [r1]
	mov r1, #0
	bl _call_via_r2
	mov r3, #0xF8
	lsl r3, r3, #2
	add r1, r7, r3
_080212AC:
	strb r0, [r1]
	ldr r0, _0802134C @ =0x02017A40
	mov r4, #0xF8
	lsl r4, r4, #2
	add r0, r0, r4
	ldrb r0, [r0]
	cmp r0, #0
	bne _080212CC
	ldr r1, _08021350 @ =0x02017FB0
	mov r0, #0xC2
	lsl r0, r0, #2
	add r1, r1, r0
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
_080212CC:
	ldr r0, _08021350 @ =0x02017FB0
	mov r3, #0xC2
	lsl r3, r3, #2
	add r0, r0, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	cmp r0, #0
	bge _080213A0
	ldr r4, _0802134C @ =0x02017A40
	mov r0, #0xF0
	lsl r0, r0, #2
	add r5, r4, r0
	ldrh r1, [r5]
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #2
	mov r2, #0x9B
	lsl r2, r2, #2
	add r1, r4, r2
	add r0, r0, r1
	bl sub_0801FCA8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08021338
	mov r3, #0xF7
	lsl r3, r3, #2
	add r6, r4, r3
	ldrh r0, [r5]
	sub r0, #1
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	add r1, r1, r4
	ldr r4, _08021354 @ =0x00000282
	add r1, r1, r4
	ldrb r1, [r1]
	lsl r0, r1, #0x1F
	mov r3, #0x7C
	cmp r0, #0
	beq _08021320
	ldr r3, _08021358 @ =0x0000807C
_08021320:
	ldrh r1, [r6]
	ldrh r2, [r6, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, [r6]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x1F
	mov r1, #1
	bl sub_08046C20
_08021338:
	ldr r0, _0802134C @ =0x02017A40
	ldr r1, _0802135C @ =0x000003D2
	add r0, r0, r1
	mov r1, #1
	ldrb r2, [r0]
	and r1, r2
	mov r2, #0xC8
	orr r1, r2
	strb r1, [r0]
	b _080213A0
_0802134C: .4byte 0x02017A40
_08021350: .4byte 0x02017FB0
_08021354: .4byte 0x00000282
_08021358: .4byte 0x0000807C
_0802135C: .4byte 0x000003D2
_08021360:
	mov r3, #0xF0
	lsl r3, r3, #2
	add r1, r7, r3
	ldrh r0, [r1]
	sub r0, #1
	strh r0, [r1]
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802137A
	mov r0, #1
	and r0, r2
	strb r0, [r6]
	b _080213A0
_0802137A:
	ldr r4, _080213B0 @ =0x000003D2
	add r1, r7, r4
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r2, _080213B4 @ =0x0201CFB0
	ldr r3, _080213B8 @ =0x00000824
	add r0, r2, r3
	ldr r0, [r0]
	ldr r4, _080213BC @ =0x00000828
	add r1, r2, r4
	ldr r1, [r1]
	add r3, #8
	add r2, r2, r3
	ldr r2, [r2]
	bl sub_08024134
_080213A0:
	mov r0, #1
_080213A2:
	add sp, #0x12C
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_080213B0: .4byte 0x000003D2
_080213B4: .4byte 0x0201CFB0
_080213B8: .4byte 0x00000824
_080213BC: .4byte 0x00000828
	thumb_func_end sub_08020AF4

