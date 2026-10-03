	thumb_func_start EffectEquipFromDeckAndSwitchControlResolve
EffectEquipFromDeckAndSwitchControlResolve: @ 0x08039F68
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x80
	add r6, r0, #0
	ldr r0, _08039F94 @ =0x02017A40
	mov r8, r0
	mov r0, #0xF8
	lsl r0, r0, #2
	add r0, r8
	ldrb r0, [r0]
	cmp r0, #0x7E
	beq _0803A064
	cmp r0, #0x7E
	bgt _08039F98
	cmp r0, #0x7D
	bne _08039F90
	b _0803A0F8
_08039F90:
	b _0803A168
	.align 2, 0
_08039F94: .4byte 0x02017A40
_08039F98:
	cmp r0, #0x7F
	beq _0803A02C
	cmp r0, #0x80
	beq _08039FA2
	b _0803A168
_08039FA2:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r4, _0803A018 @ =0x000004EA
	ldr r2, _0803A01C @ =0x000003E7
	add r1, r4, #0
	bl FindDeckCardByNumber
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _08039FBC
	b _0803A168
_08039FBC:
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r4, #1
	ldr r7, _0803A020 @ =0x08623DF4
	add r1, r1, r7
	ldrh r1, [r1]
	bl CanPlaceSpellTrapCard
	cmp r0, #0
	bne _08039FD4
	b _0803A168
_08039FD4:
	mov r3, #0
	mov r0, #1
	mov r8, r0
	ldr r1, _0803A024 @ =0x00000D64
	mov r9, r1
	ldr r5, _0803A028 @ =0x0201930C
	mov r4, #2
_08039FE2:
	mov r2, #0
	add r0, r3, #0
	mov r7, r8
	and r0, r7
	mov r1, r9
	mul r1, r0
	add r0, r1, #0
	add r1, r0, r5
_08039FF2:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803A006
	add r0, r4, #0
	ldrb r7, [r1, #6]
	and r0, r7
	cmp r0, #0
	beq _0803A006
	b _0803A14C
_0803A006:
	add r1, #0x94
	add r2, #1
	cmp r2, #4
	ble _08039FF2
	add r3, #1
	cmp r3, #1
	ble _08039FE2
	b _0803A168
	.align 2, 0
_0803A018: .4byte 0x000004EA
_0803A01C: .4byte 0x000003E7
_0803A020: .4byte gCardNumberToId
_0803A024: .4byte 0x00000D64
_0803A028: .4byte 0x0201930C
_0803A02C:
	ldr r1, _0803A050 @ =0x080834CC
	ldr r0, _0803A054 @ =0x086247C8
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r0, _0803A058 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	ldr r0, _0803A05C @ =0x00000206
	ldr r1, _0803A060 @ =0x00000613
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
_0803A04A:
	mov r0, #0x7E
	b _0803A16A
	.align 2, 0
_0803A050: .4byte gStrSelectEquipTarget
_0803A054: .4byte gUnk_086247C8
_0803A058: .4byte gCardNames
_0803A05C: .4byte 0x00000206
_0803A060: .4byte 0x00000613
_0803A064:
	ldr r0, _0803A0DC @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803A04A
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r5, r0, #0x1F
	add r0, r5, #0
	bl FindFreeSpellTrapZone
	add r4, r0, #0
	ldr r0, _0803A0E0 @ =0x0201CFB0
	ldr r2, _0803A0E4 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	mov sl, r1
	ldr r7, _0803A0E8 @ =0x0000082C
	add r0, r0, r7
	ldr r0, [r0]
	mov r9, r0
	mov r7, #0x77
	cmp r5, #0
	beq _0803A096
	ldr r7, _0803A0EC @ =0x00008077
_0803A096:
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r1, #0x80
	lsl r1, r1, #1
	add r0, r1, #0
	add r1, r4, #0
	orr r1, r0
	ldr r0, _0803A0F0 @ =0x00000544
	add r0, r8
	ldrh r2, [r0]
	ldrh r3, [r0, #2]
	add r0, r7, #0
	bl DuelCmd_Push
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	lsl r4, r4, #8
	orr r5, r4
	mov r2, sl
	lsl r4, r2, #0x18
	lsr r4, r4, #0x18
	mov r7, r9
	lsl r1, r7, #0x18
	lsr r1, r1, #0x10
	orr r4, r1
	add r1, r5, #0
	add r2, r4, #0
	bl EquipCard
	ldr r0, _0803A0F4 @ =0x00000542
	add r0, r8
	strh r4, [r0]
	mov r0, #0x7D
	b _0803A16A
_0803A0DC: .4byte 0x00E000E0
_0803A0E0: .4byte 0x0201CFB0
_0803A0E4: .4byte 0x00000824
_0803A0E8: .4byte 0x0000082C
_0803A0EC: .4byte 0x00008077
_0803A0F0: .4byte 0x00000544
_0803A0F4: .4byte 0x00000542
_0803A0F8:
	ldr r5, _0803A144 @ =0x00000542
	add r5, r8
	mov r7, #1
	ldrb r0, [r5]
	sub r4, r7, r0
	add r0, r4, #0
	bl FindFreeMonsterZone
	add r3, r0, #0
	cmp r3, #0
	blt _0803A124
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r5]
	lsl r2, r4, #0x18
	lsl r3, r3, #0x18
	lsr r2, r2, #8
	orr r2, r3
	lsr r2, r2, #0x10
	bl MoveFieldCard
_0803A124:
	add r0, r7, #0
	ldrb r6, [r6, #2]
	and r0, r6
	mov r1, #0x60
	cmp r0, #0
	beq _0803A132
	ldr r1, _0803A148 @ =0x00008060
_0803A132:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7C
	b _0803A16A
	.align 2, 0
_0803A144: .4byte 0x00000542
_0803A148: .4byte 0x00008060
_0803A14C:
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803A160 @ =0x000004EA
	ldr r2, _0803A164 @ =0x02017F84
	bl RemoveDeckCardByNumber
	mov r0, #0x7F
	b _0803A16A
	.align 2, 0
_0803A160: .4byte 0x000004EA
_0803A164: .4byte 0x02017F84
_0803A168:
	mov r0, #0
_0803A16A:
	add sp, #0x80
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectEquipFromDeckAndSwitchControlResolve
	.align 2, 0

