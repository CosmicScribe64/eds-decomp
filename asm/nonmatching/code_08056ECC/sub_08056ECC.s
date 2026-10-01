	thumb_func_start sub_08056ECC
sub_08056ECC: @ 0x08056ECC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	ldr r0, _08056F4C @ =0x000007FF
	mov r9, r0
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	lsl r0, r0, #1
	ldr r7, _08056F50 @ =0x08622AB4
	add r4, r0, r7
	ldrh r1, [r4]
	mov r0, #1
	mov r2, #0
	bl sub_08044224
	mov r8, r0
	ldr r0, _08056F54 @ =0x02015F00
	ldr r1, _08056F58 @ =0x00001B22
	add r3, r0, r1
	mov r0, #0
	strh r0, [r3]
	mov r0, r8
	cmp r0, #0
	bne _08056F0A
	b _080573BC
_08056F0A:
	ldr r0, _08056F5C @ =0x02015EE8
	ldr r0, [r0, #4]
	mov r1, #0x80
	lsl r1, r1, #2
	and r0, r1
	cmp r0, #0
	beq _08056FC6
	mov r6, #0
	cmp r6, r8
	bge _08056FB2
	add r2, r4, #0
	add r4, r3, #0
_08056F22:
	mov r3, #0
	lsl r0, r6, #2
	ldr r1, _08056F60 @ =0x0201D81C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	mov r1, r9
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r7
	ldrh r1, [r0]
	cmp r1, #0x2F
	beq _08056F94
	cmp r1, #0x2F
	ble _08056F68
	ldr r0, _08056F64 @ =0x0000023D
	cmp r1, r0
	beq _08056F94
	b _08056F9E
	.align 2, 0
_08056F4C: .4byte 0x000007FF
_08056F50: .4byte gUnk_08622AB4
_08056F54: .4byte 0x02015F00
_08056F58: .4byte 0x00001B22
_08056F5C: .4byte 0x02015EE8
_08056F60: .4byte 0x0201D81C
_08056F64: .4byte 0x0000023D
_08056F68:
	cmp r1, #0x14
	bgt _08056F9E
	cmp r1, #0x10
	blt _08056F9E
	ldrh r1, [r2]
	ldr r0, _08056F84 @ =0x0000023D
	cmp r1, r0
	beq _08056FA2
	cmp r1, r0
	bgt _08056F88
	cmp r1, #0x2F
	beq _08056FA2
	b _08056F9E
	.align 2, 0
_08056F84: .4byte 0x0000023D
_08056F88:
	ldr r0, _08056F90 @ =0x0000047B
	cmp r1, r0
	bne _08056F9E
	b _08056FA2
_08056F90: .4byte 0x0000047B
_08056F94:
	ldr r0, _08056FA8 @ =0x00000463
	ldrh r1, [r2]
	cmp r1, r0
	bne _08056F9E
	mov r3, #1
_08056F9E:
	cmp r3, #0
	beq _08056FAC
_08056FA2:
	strh r6, [r4]
	ldrh r0, [r4]
	b _080573C0
_08056FA8: .4byte 0x00000463
_08056FAC:
	add r6, #1
	cmp r6, r8
	blt _08056F22
_08056FB2:
	ldr r0, _08056FE4 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _08056FE8 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _08056FEC @ =0x00000463
	ldrh r0, [r0]
	cmp r0, r1
	bne _08056FC6
	b _080573BC
_08056FC6:
	ldr r0, _08056FE4 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _08056FE8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08056FF0 @ =0x000001AB
	cmp r1, r0
	beq _08056FFA
	cmp r1, r0
	bgt _08056FF4
	cmp r1, #0x65
	beq _08056FFA
	b _0805704C
	.align 2, 0
_08056FE4: .4byte 0x000007FF
_08056FE8: .4byte gUnk_08622AB4
_08056FEC: .4byte 0x00000463
_08056FF0: .4byte 0x000001AB
_08056FF4:
	ldr r0, _08057038 @ =0x00000443
	cmp r1, r0
	bne _0805704C
_08056FFA:
	mov r6, #0
	ldr r0, _0805703C @ =0x0201D81C
	mov r9, r0
	ldr r7, _08057040 @ =0x0819D2FC
_08057002:
	mov r2, #0
	cmp r2, r8
	bge _0805702C
	ldrh r3, [r7]
	mov r1, r9
	ldr r5, _08057044 @ =0x000007FF
	ldr r4, _08057048 @ =0x08622AB4
_08057010:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #1
	add r0, r0, r4
	ldrh r0, [r0]
	cmp r0, r3
	bne _08057024
	b _080573A8
_08057024:
	add r1, #4
	add r2, #1
	cmp r2, r8
	blt _08057010
_0805702C:
	add r7, #2
	add r6, #1
	cmp r6, #0xC
	bls _08057002
	b _08057388
	.align 2, 0
_08057038: .4byte 0x00000443
_0805703C: .4byte 0x0201D81C
_08057040: .4byte gUnk_0819D2FC
_08057044: .4byte 0x000007FF
_08057048: .4byte gUnk_08622AB4
_0805704C:
	mov r0, r8
	cmp r0, #0
	bgt _08057054
	b _080573BC
_08057054:
	mov r7, #0
	mov r1, #1
	neg r1, r1
	mov r9, r1
	mov sl, r7
	mov r6, #0
_08057060:
	mov r0, #0
	add r1, r6, #0
	mov r2, sp
	bl sub_0800ABC8
	ldr r0, [sp, #4]
	cmp sl, r0
	bge _08057072
	mov sl, r0
_08057072:
	add r6, #1
	cmp r6, #4
	ble _08057060
	mov r6, #0
	cmp r6, r8
	blt _08057080
	b _080572A6
_08057080:
	ldr r4, _080570B4 @ =0x000007FF
	mov r5, #0xF8
	lsl r5, r5, #0x11
	ldr r0, _080570B8 @ =0x0003FE00
	mov ip, r0
_0805708A:
	lsl r0, r6, #2
	ldr r1, _080570BC @ =0x0201D81C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _080570C0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080570CE
	cmp r0, #0x17
	ble _080570C4
	cmp r0, #0x18
	beq _080570C8
	b _080570CE
_080570B4: .4byte 0x000007FF
_080570B8: .4byte 0x0003FE00
_080570BC: .4byte 0x0201D81C
_080570C0: .4byte gUnk_08621DE0
_080570C4:
	mov r3, #0
	b _080570E6
_080570C8:
	mov r3, #0xFA
	lsl r3, r3, #4
	b _080570E6
_080570CE:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08057104 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, ip
	and r1, r0
	lsr r1, r1, #9
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r3, r0, #1
_080570E6:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08057104 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08057112
	cmp r0, #0x17
	ble _08057108
	cmp r0, #0x18
	beq _0805710C
	b _08057112
_08057104: .4byte gUnk_08621DE0
_08057108:
	mov r0, #0
	b _08057128
_0805710C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08057128
_08057112:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08057150 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08057154 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08057128:
	add r0, r0, r3
	cmp r7, r0
	blt _08057130
	b _0805729E
_08057130:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08057150 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08057162
	cmp r0, #0x17
	ble _08057158
	cmp r0, #0x18
	beq _0805715C
	b _08057162
	.align 2, 0
_08057150: .4byte gUnk_08621DE0
_08057154: .4byte 0x000001FF
_08057158:
	mov r0, #0
	b _0805717A
_0805715C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805717A
_08057162:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0805719C @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, ip
	and r1, r0
	lsr r1, r1, #9
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805717A:
	cmp sl, r0
	ble _08057210
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _0805719C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080571AA
	cmp r0, #0x17
	ble _080571A0
	cmp r0, #0x18
	beq _080571A4
	b _080571AA
_0805719C: .4byte gUnk_08621DE0
_080571A0:
	mov r0, #0
	b _080571C2
_080571A4:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _080571C2
_080571AA:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _080571E8 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, ip
	and r1, r0
	lsr r1, r1, #9
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_080571C2:
	mov r1, #0
	cmp r1, r0
	blt _08057210
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _080571E8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080571F6
	cmp r0, #0x17
	ble _080571EC
	cmp r0, #0x18
	beq _080571F0
	b _080571F6
	.align 2, 0
_080571E8: .4byte gUnk_08621DE0
_080571EC:
	mov r0, #0
	b _0805720C
_080571F0:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805720C
_080571F6:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08057230 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _08057234 @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805720C:
	cmp sl, r0
	bgt _0805729E
_08057210:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08057230 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08057242
	cmp r0, #0x17
	ble _08057238
	cmp r0, #0x18
	beq _0805723C
	b _08057242
	.align 2, 0
_08057230: .4byte gUnk_08621DE0
_08057234: .4byte 0x000001FF
_08057238:
	mov r3, #0
	b _0805725A
_0805723C:
	mov r3, #0xFA
	lsl r3, r3, #4
	b _0805725A
_08057242:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08057278 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	mov r0, ip
	and r1, r0
	lsr r1, r1, #9
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r3, r0, #1
_0805725A:
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08057278 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08057286
	cmp r0, #0x17
	ble _0805727C
	cmp r0, #0x18
	beq _08057280
	b _08057286
_08057278: .4byte gUnk_08621DE0
_0805727C:
	mov r0, #0
	b _0805729A
_08057280:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805729A
_08057286:
	and r2, r4
	lsl r0, r2, #2
	ldr r1, _080572E8 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	ldr r0, _080572EC @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805729A:
	add r7, r0, r3
	mov r9, r6
_0805729E:
	add r6, #1
	cmp r6, r8
	bge _080572A6
	b _0805708A
_080572A6:
	mov r0, #1
	neg r0, r0
	cmp r9, r0
	bgt _0805736E
	mov r9, r0
	mov r7, #0
	mov r6, #0
	cmp r6, r8
	bge _08057368
	ldr r3, _080572F0 @ =0x000007FF
	ldr r5, _080572F4 @ =0x0201D81C
	mov r4, #0xF8
	lsl r4, r4, #0x11
_080572C0:
	lsl r0, r6, #2
	add r0, r0, r5
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _080572E8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08057302
	cmp r0, #0x17
	ble _080572F8
	cmp r0, #0x18
	beq _080572FC
	b _08057302
_080572E8: .4byte gUnk_08621DE0
_080572EC: .4byte 0x000001FF
_080572F0: .4byte 0x000007FF
_080572F4: .4byte 0x0201D81C
_080572F8:
	mov r0, #0
	b _08057318
_080572FC:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08057318
_08057302:
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805733C @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08057318:
	cmp r7, r0
	bge _08057362
	add r0, r2, #0
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0805733C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0805734A
	cmp r0, #0x17
	ble _08057340
	cmp r0, #0x18
	beq _08057344
	b _0805734A
	.align 2, 0
_0805733C: .4byte gUnk_08621DE0
_08057340:
	mov r0, #0
	b _0805735E
_08057344:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0805735E
_0805734A:
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _0805737C @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0805735E:
	add r7, r0, #0
	mov r9, r6
_08057362:
	add r6, #1
	cmp r6, r8
	blt _080572C0
_08057368:
	mov r0, r9
	cmp r0, #0
	blt _08057388
_0805736E:
	ldr r0, _08057380 @ =0x02015F00
	ldr r1, _08057384 @ =0x00001B22
	add r0, r0, r1
	mov r1, r9
	strh r1, [r0]
	ldrh r0, [r0]
	b _080573C0
_0805737C: .4byte gUnk_08621DE0
_08057380: .4byte 0x02015F00
_08057384: .4byte 0x00001B22
_08057388:
	bl sub_08076F9C
	ldr r4, _080573A0 @ =0x02015F00
	mov r1, r8
	bl __modsi3
	ldr r1, _080573A4 @ =0x00001B22
	add r4, r4, r1
	strh r0, [r4]
	ldrh r0, [r4]
	b _080573C0
	.align 2, 0
_080573A0: .4byte 0x02015F00
_080573A4: .4byte 0x00001B22
_080573A8:
	ldr r0, _080573B4 @ =0x02015F00
	ldr r1, _080573B8 @ =0x00001B22
	add r0, r0, r1
	ldrh r2, [r0]
	add r0, r2, #0
	b _080573C0
_080573B4: .4byte 0x02015F00
_080573B8: .4byte 0x00001B22
_080573BC:
	mov r0, #1
	neg r0, r0
_080573C0:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08056ECC

