	thumb_func_start sub_08030B88
sub_08030B88: @ 0x08030B88
	push {r4, r5, r6, r7, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _08030B98
	b _08030E44
_08030B98:
	ldr r0, _08030BB4 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	sub r0, #0x7C
	cmp r0, #4
	bls _08030BAA
	b _08030E44
_08030BAA:
	lsl r0, r0, #2
	ldr r1, _08030BB8 @ =0x08030BBC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08030BB4: .4byte 0x02017A40
_08030BB8: .4byte 0x08030BBC
_08030BBC:
	.4byte _08030DE8
	.4byte _08030DC0
	.4byte _08030D4C
	.4byte _08030D20
	.4byte _08030BD0
_08030BD0:
	mov r0, #0
	mov r1, #0x3D
	bl sub_08008524
	cmp r0, #0
	bne _08030C04
	mov r0, #1
	mov r1, #0x3D
	bl sub_08008524
	cmp r0, #0
	bne _08030C04
	ldr r5, _08030CE4 @ =0x000004E1
	mov r0, #0
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bne _08030C04
	mov r0, #1
	add r1, r5, #0
	bl sub_08008524
	cmp r0, #0
	bne _08030C04
	b _08030E44
_08030C04:
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r5, _08030CE8 @ =0x0000013D
	add r1, r5, #0
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	bne _08030C1A
	b _08030E44
_08030C1A:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008A1C
	cmp r0, #0
	bne _08030C2A
	b _08030E44
_08030C2A:
	mov r0, #1
	ldrb r2, [r4, #2]
	and r0, r2
	cmp r0, #0
	beq _08030D04
	mov r0, #1
	add r1, r5, #0
	mov r2, #0
	bl sub_08044224
	add r3, r0, #0
	mov r2, #0
	cmp r2, r3
	bge _08030C6A
	ldr r7, _08030CEC @ =0x0201D81C
	ldr r6, _08030CF0 @ =0x000007FF
	add r1, r7, #0
	ldr r5, _08030CF4 @ =0x08622AB4
_08030C4E:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, #0x3E
	bne _08030C62
	b _08030E30
_08030C62:
	add r1, #4
	add r2, #1
	cmp r2, r3
	blt _08030C4E
_08030C6A:
	mov r2, #0
	cmp r2, r3
	bge _08030C98
	ldr r0, _08030CEC @ =0x0201D81C
	mov ip, r0
	ldr r7, _08030CF0 @ =0x000007FF
	ldr r6, _08030CE4 @ =0x000004E1
	mov r1, ip
	ldr r5, _08030CF4 @ =0x08622AB4
_08030C7C:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r7
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r6
	bne _08030C90
	b _08030E2C
_08030C90:
	add r1, #4
	add r2, #1
	cmp r2, r3
	blt _08030C7C
_08030C98:
	mov r2, #0
	cmp r2, r3
	bge _08030CC2
	ldr r7, _08030CEC @ =0x0201D81C
	ldr r6, _08030CF0 @ =0x000007FF
	add r1, r7, #0
	ldr r5, _08030CF4 @ =0x08622AB4
_08030CA6:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, #0x3D
	bne _08030CBA
	b _08030E30
_08030CBA:
	add r1, #4
	add r2, #1
	cmp r2, r3
	blt _08030CA6
_08030CC2:
	ldrh r0, [r4]
	bl sub_08056ECC
	ldr r1, _08030CF8 @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1, #5]
	and r0, r2
	strb r0, [r1, #5]
	ldr r0, _08030CFC @ =0x02015F00
	ldr r3, _08030D00 @ =0x00001B22
	add r0, r0, r3
	ldrh r0, [r0]
	strh r0, [r1, #6]
	mov r0, #0x7E
	b _08030E46
	.align 2, 0
_08030CE4: .4byte 0x000004E1
_08030CE8: .4byte 0x0000013D
_08030CEC: .4byte 0x0201D81C
_08030CF0: .4byte 0x000007FF
_08030CF4: .4byte gUnk_08622AB4
_08030CF8: .4byte 0x0201D810
_08030CFC: .4byte 0x02015F00
_08030D00: .4byte 0x00001B22
_08030D04:
	ldr r0, _08030D14 @ =0x00000205
	ldr r1, _08030D18 @ =0x00000914
	ldr r3, _08030D1C @ =0x08082988
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7F
	b _08030E46
_08030D14: .4byte 0x00000205
_08030D18: .4byte 0x00000914
_08030D1C: .4byte gUnk_08082988
_08030D20:
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _08030D44 @ =0x000007FF
	ldrh r4, [r4]
	and r2, r4
	lsl r2, r2, #1
	ldr r3, _08030D48 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7E
	b _08030E46
	.align 2, 0
_08030D44: .4byte 0x000007FF
_08030D48: .4byte gUnk_08622AB4
_08030D4C:
	ldr r0, _08030D98 @ =0x0201D810
	mov ip, r0
	ldrb r1, [r0, #5]
	lsl r0, r1, #0x1E
	lsr r1, r0, #0x1E
	mov r2, ip
	ldrh r3, [r2, #6]
	add r1, r1, r3
	lsl r1, r1, #2
	add r2, #0xC
	add r2, r1, r2
	lsr r0, r0, #0x1E
	add r0, r0, r3
	lsl r0, r0, #1
	mov r1, #0x83
	lsl r1, r1, #2
	add r1, ip
	add r0, r0, r1
	ldrh r1, [r0]
	cmp r1, #1
	beq _08030DA0
	cmp r1, #2
	bne _08030DB6
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r3, #0x65
	cmp r0, #0
	beq _08030D88
	ldr r3, _08030D9C @ =0x00008065
_08030D88:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	b _08030DB6
	.align 2, 0
_08030D98: .4byte 0x0201D810
_08030D9C: .4byte 0x00008065
_08030DA0:
	ldrb r4, [r4, #2]
	and r1, r4
	mov r0, #0xC2
	cmp r1, #0
	beq _08030DAC
	ldr r0, _08030DBC @ =0x000080C2
_08030DAC:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	mov r3, #0
	bl sub_0801EC58
_08030DB6:
	mov r0, #0x7D
	b _08030E46
	.align 2, 0
_08030DBC: .4byte 0x000080C2
_08030DC0:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _08030DE4 @ =0x0201D810
	ldrb r3, [r2, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r2, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r2, #0xC
	add r1, r1, r2
	mov r2, #1
	mov r3, #1
	bl sub_08056094
	mov r0, #0x7C
	b _08030E46
_08030DE4: .4byte 0x0201D810
_08030DE8:
	ldr r1, _08030E24 @ =0x0201D810
	ldrb r2, [r1, #5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	ldrh r3, [r1, #6]
	add r0, r3, r0
	lsl r0, r0, #1
	mov r2, #0x83
	lsl r2, r2, #2
	add r1, r1, r2
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, #2
	bne _08030E1E
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _08030E12
	ldr r1, _08030E28 @ =0x00008060
_08030E12:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
_08030E1E:
	mov r0, #0x64
	b _08030E46
	.align 2, 0
_08030E24: .4byte 0x0201D810
_08030E28: .4byte 0x00008060
_08030E2C:
	mov r1, ip
	b _08030E32
_08030E30:
	add r1, r7, #0
_08030E32:
	sub r1, #0xC
	mov r0, #4
	neg r0, r0
	ldrb r3, [r1, #5]
	and r0, r3
	strb r0, [r1, #5]
	strh r2, [r1, #6]
	mov r0, #0x7E
	b _08030E46
_08030E44:
	mov r0, #0
_08030E46:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08030B88

