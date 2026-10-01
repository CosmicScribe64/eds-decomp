	thumb_func_start sub_08043B98
sub_08043B98: @ 0x08043B98
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x18
	mov r9, r0
	ldrh r0, [r0]
	str r0, [sp, #0]
	mov r0, #4
	mov r1, r9
	ldrb r1, [r1, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #0
	bne _08043C2C
	ldr r1, _08043BD8 @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r0, r1, r3
	ldrb r0, [r0]
	add r6, r1, #0
	cmp r0, #0x64
	bne _08043BCC
	b _08043CC0
_08043BCC:
	cmp r0, #0x64
	bgt _08043BDC
	cmp r0, #0x63
	bne _08043BD6
	b _08043D9C
_08043BD6:
	b _08044110
_08043BD8: .4byte 0x02017A40
_08043BDC:
	cmp r0, #0x78
	bne _08043BE2
	b _08043DB6
_08043BE2:
	cmp r0, #0x80
	beq _08043BE8
	b _08044110
_08043BE8:
	ldr r0, _08043C04 @ =0x000003E1
	add r5, r6, r0
	strb r2, [r5]
	ldr r4, _08043C08 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	ble _08043C0C
	mov r1, r9
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	b _08043C1E
_08043C04: .4byte 0x000003E1
_08043C08: .4byte 0x0000058A
_08043C0C:
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	ble _08043C34
	mov r3, r9
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
_08043C1E:
	lsr r0, r0, #0x1F
	lsl r1, r4, #1
	ldr r2, _08043C30 @ =0x08623DF4
	add r1, r1, r2
	ldrh r1, [r1]
	bl sub_080197E0
_08043C2C:
	mov r0, #0
	b _0804411A
_08043C30: .4byte gUnk_08623DF4
_08043C34:
	mov r0, r9
	mov r1, #0
	mov r2, #0
	bl sub_08043AA8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08043C2C
	mov r3, r9
	ldrh r0, [r3]
	bl sub_0804353C
	add r4, r0, #0
	cmp r4, #0
	blt _08043C2C
	mov r1, r9
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl sub_08043594
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08043C2C
	ldr r1, _08043C8C @ =0x0819A990
	lsl r0, r4, #2
	add r0, r0, r1
	ldrb r0, [r0, #3]
	lsr r0, r0, #2
	strb r0, [r5]
	ldrb r1, [r5]
	mov r2, #0xA2
	lsl r2, r2, #3
	add r0, r6, r2
	strb r1, [r0]
	mov r0, #1
	mov r3, r9
	ldrb r3, [r3, #2]
	and r0, r3
	cmp r0, #0
	beq _08043C90
	mov r0, #0x78
	b _0804411A
_08043C8C: .4byte gUnk_0819A990
_08043C90:
	ldr r0, _08043CAC @ =0x00000206
	ldr r1, _08043CB0 @ =0x00000612
	ldr r3, _08043CB4 @ =0x0808545C
	mov r2, #0xB
	bl sub_080602A4
	ldr r1, _08043CB8 @ =0x08043759
	ldr r2, _08043CBC @ =0x080437CD
	mov r0, #5
	bl sub_08060308
	mov r0, #0x64
	b _0804411A
	.align 2, 0
_08043CAC: .4byte 0x00000206
_08043CB0: .4byte 0x00000612
_08043CB4: .4byte gUnk_0808545C
_08043CB8: .4byte sub_08043758
_08043CBC: .4byte sub_080437CC
_08043CC0:
	mov r0, #0
	mov sl, r0
	ldr r1, _08043D5C @ =0x020192E4
	mov r3, r9
	ldrb r2, [r3, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _08043D60 @ =0x00000D64
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp sl, r0
	bge _08043C2C
	mov r7, #1
	ldr r0, _08043D64 @ =0x00000684
	add r0, r0, r1
	mov r8, r0
_08043CE2:
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r7, #0
	and r1, r0
	mov r2, sl
	lsl r6, r2, #2
	add r0, r1, #0
	mul r0, r3
	add r0, r6, r0
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08043D68 @ =0x08622AB4
	add r0, r0, r1
	ldrh r5, [r0]
	ldr r4, _08043D6C @ =0x0819A990
	mov r2, r9
	ldrh r0, [r2]
	str r3, [sp, #0x14]
	bl sub_0804353C
	lsl r0, r0, #2
	add r0, r0, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x13
	ldr r3, [sp, #0x14]
	cmp r5, r0
	bne _08043D78
	mov r3, r9
	ldrb r3, [r3, #2]
	lsl r1, r3, #0x1F
	lsr r1, r1, #0x1F
	add r0, r7, #0
	and r0, r1
	ldr r1, _08043D60 @ =0x00000D64
	mul r0, r1
	add r0, r8
	add r4, r0, r6
	ldr r0, _08043D70 @ =0x02017E28
	add r1, r4, #0
	bl sub_08007558
	add r0, r7, #0
	mov r2, r9
	ldrb r2, [r2, #2]
	and r0, r2
	mov r3, #0xC2
	cmp r0, #0
	beq _08043D4A
	ldr r3, _08043D74 @ =0x000080C2
_08043D4A:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x63
	b _0804411A
	.align 2, 0
_08043D5C: .4byte 0x020192E4
_08043D60: .4byte 0x00000D64
_08043D64: .4byte 0x00000684
_08043D68: .4byte gUnk_08622AB4
_08043D6C: .4byte gUnk_0819A990
_08043D70: .4byte 0x02017E28
_08043D74: .4byte 0x000080C2
_08043D78:
	mov r0, #1
	add sl, r0
	mov r1, r9
	ldrb r2, [r1, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r7, #0
	and r1, r0
	add r0, r1, #0
	mul r0, r3
	ldr r1, _08043D98 @ =0x020192E4
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp sl, r0
	blt _08043CE2
	b _08043C2C
_08043D98: .4byte 0x020192E4
_08043D9C:
	mov r2, r9
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r3, #0xFA
	lsl r3, r3, #2
	add r1, r6, r3
	mov r2, #1
	mov r3, #1
	bl sub_08056094
	mov r0, #0x62
	b _0804411A
_08043DB6:
	mov r0, #0
	str r0, [sp, #4]
	mov r1, #1
	neg r1, r1
	str r1, [sp, #8]
	mov r2, #0
	str r2, [sp, #0xC]
	str r1, [sp, #0x10]
	mov r3, r9
	ldrb r3, [r3, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl sub_08008860
	cmp r0, #4
	ble _08043DD8
	b _08043F72
_08043DD8:
	mov r0, #0
	mov sl, r0
	ldr r1, _08043E70 @ =0x020192E4
	mov r3, r9
	ldrb r2, [r3, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _08043E74 @ =0x00000D64
	mul r0, r3
	add r0, r0, r1
	ldr r1, [sp, #0xC]
	ldrb r0, [r0, #2]
	cmp r1, r0
	blt _08043DF6
	b _08043F72
_08043DF6:
	lsl r0, r2, #0x1F
	mov r2, #1
	mov r8, r2
	lsr r0, r0, #0x1F
	mov r2, sl
	lsl r1, r2, #2
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08043E78 @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r7, r0, #0x14
	add r0, r7, #0
	ldr r3, _08043E7C @ =0x000007FF
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _08043E80 @ =0x08622AB4
	add r6, r0, r1
	ldrh r5, [r6]
	ldr r4, _08043E84 @ =0x0819A990
	ldr r0, [sp, #0]
	bl sub_0804353C
	lsl r0, r0, #2
	add r0, r0, r4
	ldrh r0, [r0]
	lsl r0, r0, #0x13
	lsr r0, r0, #0x13
	cmp r5, r0
	bne _08043E4A
	mov r2, r9
	ldrb r2, [r2, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r6]
	bl sub_0800A2A8
	cmp r0, #1
	bgt _08043E4A
	mov r3, #0
	mov r8, r3
_08043E4A:
	add r0, r7, #0
	ldr r1, _08043E7C @ =0x000007FF
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _08043E88 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08043E94
	cmp r0, #0x17
	ble _08043E8C
	cmp r0, #0x18
	beq _08043E90
	b _08043E94
	.align 2, 0
_08043E70: .4byte 0x020192E4
_08043E74: .4byte 0x00000D64
_08043E78: .4byte 0x02019968
_08043E7C: .4byte 0x000007FF
_08043E80: .4byte gUnk_08622AB4
_08043E84: .4byte gUnk_0819A990
_08043E88: .4byte gUnk_08621DE0
_08043E8C:
	mov r0, #0
	b _08043EAA
_08043E90:
	mov r0, #0xA
	b _08043EAA
_08043E94:
	add r0, r7, #0
	ldr r3, _08043EDC @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08043EE0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08043EAA:
	cmp r0, #0
	bne _08043EB2
	mov r2, #0
	mov r8, r2
_08043EB2:
	mov r3, r8
	cmp r3, #0
	beq _08043F56
	add r0, r7, #0
	ldr r1, _08043EDC @ =0x000007FF
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _08043EE0 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08043EEC
	cmp r0, #0x17
	ble _08043EE4
	cmp r0, #0x18
	beq _08043EE8
	b _08043EEC
_08043EDC: .4byte 0x000007FF
_08043EE0: .4byte gUnk_08621DE0
_08043EE4:
	mov r0, #0
	b _08043F02
_08043EE8:
	mov r0, #0xA
	b _08043F02
_08043EEC:
	add r0, r7, #0
	ldr r3, _08043F2C @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08043F30 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08043F02:
	ldr r2, [sp, #4]
	cmp r2, r0
	bge _08043F56
	add r0, r7, #0
	ldr r3, _08043F2C @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _08043F30 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08043F3C
	cmp r0, #0x17
	ble _08043F34
	cmp r0, #0x18
	beq _08043F38
	b _08043F3C
_08043F2C: .4byte 0x000007FF
_08043F30: .4byte gUnk_08621DE0
_08043F34:
	mov r0, #0
	b _08043F50
_08043F38:
	mov r0, #0xA
	b _08043F50
_08043F3C:
	ldr r2, _08043FD8 @ =0x000007FF
	and r7, r2
	lsl r0, r7, #2
	ldr r3, _08043FDC @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08043F50:
	str r0, [sp, #4]
	mov r0, sl
	str r0, [sp, #8]
_08043F56:
	mov r1, #1
	add sl, r1
	ldr r1, _08043FE0 @ =0x020192E4
	mov r3, r9
	ldrb r2, [r3, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _08043FE4 @ =0x00000D64
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp sl, r0
	bge _08043F72
	b _08043DF6
_08043F72:
	mov r0, #0
	mov sl, r0
	mov r1, r9
	ldrb r5, [r1, #2]
	ldr r7, _08043FE8 @ =0x0201930C
	lsl r3, r5, #0x1F
	mov r2, #1
	mov r8, r2
	ldr r6, _08043FE4 @ =0x00000D64
	ldr r4, _08043FD8 @ =0x000007FF
_08043F86:
	lsr r0, r3, #0x1F
	mov r1, r8
	and r1, r0
	mov r0, #0x94
	mov r2, sl
	mul r2, r0
	add r0, r1, #0
	mul r0, r6
	add r0, r2, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0804406C
	lsr r1, r3, #0x1F
	mov r0, r8
	and r0, r1
	mul r0, r6
	add r0, r2, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08043FDC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08043FF4
	cmp r0, #0x17
	ble _08043FEC
	cmp r0, #0x18
	beq _08043FF0
	b _08043FF4
	.align 2, 0
_08043FD8: .4byte 0x000007FF
_08043FDC: .4byte gUnk_08621DE0
_08043FE0: .4byte 0x020192E4
_08043FE4: .4byte 0x00000D64
_08043FE8: .4byte 0x0201930C
_08043FEC:
	mov r0, #0
	b _08044006
_08043FF0:
	mov r0, #0xA
	b _08044006
_08043FF4:
	and r2, r4
	lsl r0, r2, #2
	ldr r2, _08044048 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08044006:
	ldr r1, [sp, #0xC]
	cmp r1, r0
	bge _0804406C
	lsr r0, r3, #0x1F
	mov r1, r8
	and r1, r0
	mov r0, #0x94
	mov r2, sl
	mul r2, r0
	add r0, r2, #0
	mul r1, r6
	add r0, r0, r1
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	add r0, r2, #0
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08044048 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08044054
	cmp r0, #0x17
	ble _0804404C
	cmp r0, #0x18
	beq _08044050
	b _08044054
_08044048: .4byte gUnk_08621DE0
_0804404C:
	mov r0, #0
	b _08044066
_08044050:
	mov r0, #0xA
	b _08044066
_08044054:
	and r2, r4
	lsl r0, r2, #2
	ldr r2, _080440BC @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08044066:
	str r0, [sp, #0xC]
	mov r0, sl
	str r0, [sp, #0x10]
_0804406C:
	mov r1, #1
	add sl, r1
	mov r2, sl
	cmp r2, #4
	ble _08043F86
	ldr r3, [sp, #4]
	cmp r3, #0
	bne _08044084
	ldr r0, [sp, #0xC]
	cmp r0, #0
	bne _08044084
	b _08043C2C
_08044084:
	ldr r1, [sp, #4]
	ldr r2, [sp, #0xC]
	cmp r1, r2
	ble _080440CC
	mov r0, #1
	and r0, r5
	mov r2, #0xC0
	cmp r0, #0
	beq _08044098
	ldr r2, _080440C0 @ =0x000080C0
_08044098:
	ldr r3, [sp, #8]
	lsl r1, r3, #0x10
	lsr r1, r1, #0x10
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl sub_0801EC58
	ldr r0, _080440C4 @ =0x02017A40
	ldr r2, _080440C8 @ =0x000003E1
	add r1, r0, r2
	ldrb r0, [r1]
	ldr r3, [sp, #4]
	cmp r0, r3
	ble _080440F0
	sub r0, r0, r3
	b _080440F2
	.align 2, 0
_080440BC: .4byte gUnk_08621DE0
_080440C0: .4byte 0x000080C0
_080440C4: .4byte 0x02017A40
_080440C8: .4byte 0x000003E1
_080440CC:
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, [sp, #0x10]
	bl sub_08017FF4
	ldr r0, _080440E8 @ =0x02017A40
	ldr r2, _080440EC @ =0x000003E1
	add r1, r0, r2
	ldrb r0, [r1]
	ldr r3, [sp, #0xC]
	cmp r0, r3
	ble _080440F0
	sub r0, r0, r3
	b _080440F2
_080440E8: .4byte 0x02017A40
_080440EC: .4byte 0x000003E1
_080440F0:
	mov r0, #0
_080440F2:
	strb r0, [r1]
	ldr r0, _08044108 @ =0x02017A40
	ldr r1, _0804410C @ =0x000003E1
	add r0, r0, r1
	ldrb r0, [r0]
	mov r1, #0x64
	cmp r0, #0
	beq _08044104
	mov r1, #0x78
_08044104:
	add r0, r1, #0
	b _0804411A
_08044108: .4byte 0x02017A40
_0804410C: .4byte 0x000003E1
_08044110:
	mov r2, #0xF8
	lsl r2, r2, #2
	add r1, r6, r2
	mov r0, #0
	strb r0, [r1]
_0804411A:
	add sp, #0x18
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_08043B98
	.align 2, 0

