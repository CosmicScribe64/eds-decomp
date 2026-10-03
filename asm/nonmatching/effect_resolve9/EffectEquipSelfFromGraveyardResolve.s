	thumb_func_start EffectEquipSelfFromGraveyardResolve
EffectEquipSelfFromGraveyardResolve: @ 0x080398B0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x88
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _080398CA
	b _08039AF0
_080398CA:
	ldr r2, _080398E8 @ =0x02017A40
	mov sl, r2
	mov r0, #0xF8
	lsl r0, r0, #2
	add r0, sl
	ldrb r0, [r0]
	cmp r0, #0x7E
	beq _080399A0
	cmp r0, #0x7E
	bgt _080398EC
	cmp r0, #0x7D
	bne _080398E4
	b _080399E0
_080398E4:
	b _08039AF0
	.align 2, 0
_080398E8: .4byte 0x02017A40
_080398EC:
	cmp r0, #0x7F
	beq _08039964
	cmp r0, #0x80
	beq _080398F6
	b _08039AF0
_080398F6:
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08039958 @ =0x000004DA
	bl CountGraveyardCardsByNumber
	cmp r0, #0
	bne _08039908
	b _08039AF0
_08039908:
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeSpellTrapZone
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _0803991C
	b _08039AF0
_0803991C:
	mov r3, #0
	mov r4, #1
	mov r8, r4
	ldr r6, _0803995C @ =0x00000D64
	ldr r5, _08039960 @ =0x0201930C
	mov r4, #2
_08039928:
	mov r2, #0
	add r0, r3, #0
	mov r7, r8
	and r0, r7
	mul r0, r6
	add r1, r0, r5
_08039934:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08039948
	add r0, r4, #0
	ldrb r7, [r1, #6]
	and r0, r7
	cmp r0, #0
	beq _08039948
	b _08039AE6
_08039948:
	add r1, #0x94
	add r2, #1
	cmp r2, #4
	ble _08039934
	add r3, #1
	cmp r3, #1
	ble _08039928
	b _08039AF0
_08039958: .4byte 0x000004DA
_0803995C: .4byte 0x00000D64
_08039960: .4byte 0x0201930C
_08039964:
	ldr r1, _08039990 @ =0x08083468
	ldrh r5, [r5]
	lsl r2, r5, #6
	ldr r0, _08039994 @ =0x0822C720
	add r2, r2, r0
	mov r0, sp
	bl FormatStr
	ldr r0, _08039998 @ =0x00000206
	ldr r1, _0803999C @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7E
	b _08039AF2
	.align 2, 0
_08039990: .4byte gStrEquipFromGraveyardPrompt
_08039994: .4byte gCardNames
_08039998: .4byte 0x00000206
_0803999C: .4byte 0x00000712
_080399A0:
	ldr r0, _080399CC @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _080399AA
	b _08039AF0
_080399AA:
	ldr r1, _080399D0 @ =0x080834CC
	ldrh r5, [r5]
	lsl r2, r5, #6
	ldr r3, _080399D4 @ =0x0822C720
	add r2, r2, r3
	mov r0, sp
	bl FormatStr
	ldr r0, _080399D8 @ =0x00000206
	ldr r1, _080399DC @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
_080399C6:
	mov r0, #0x7D
	b _08039AF2
	.align 2, 0
_080399CC: .4byte 0x0201AE60
_080399D0: .4byte gStrSelectEquipTarget
_080399D4: .4byte gCardNames
_080399D8: .4byte 0x00000206
_080399DC: .4byte 0x00000712
_080399E0:
	ldr r0, _08039AB4 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08039AD8
	ldr r0, _08039AB8 @ =0x0201CFB0
	ldr r4, _08039ABC @ =0x00000824
	add r6, r0, r4
	ldr r7, [r6]
	str r7, [sp, #0x80]
	ldr r1, _08039AC0 @ =0x00000828
	add r4, r0, r1
	ldr r2, _08039AC4 @ =0x0000082C
	add r2, r2, r0
	mov r8, r2
	ldr r1, [r4]
	ldr r0, [r2]
	add r1, r1, r0
	str r1, [sp, #0x84]
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeSpellTrapZone
	mov r9, r0
	mov r0, #1
	bl PlaySE
	mov r7, #1
	add r0, r7, #0
	ldrb r1, [r5, #2]
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _08039A28
	ldr r3, _08039AC8 @ =0x00008008
_08039A28:
	ldrh r1, [r6]
	mov r6, r8
	ldrb r6, [r6]
	lsl r2, r6, #8
	ldrb r4, [r4]
	orr r2, r4
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r5]
	ldr r6, _08039ACC @ =0x00000544
	add r6, sl
	add r2, r6, #0
	bl GetGraveyardCardById
	add r0, r7, #0
	ldrb r2, [r5, #2]
	and r0, r2
	mov r3, #0xD3
	cmp r0, #0
	beq _08039A5C
	ldr r3, _08039AD0 @ =0x000080D3
_08039A5C:
	ldrh r1, [r6]
	ldrh r2, [r6, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r7, #0
	ldrb r3, [r5, #2]
	and r0, r3
	mov r7, #0x77
	cmp r0, #0
	beq _08039A76
	ldr r7, _08039AD4 @ =0x00008077
_08039A76:
	mov r0, r9
	lsl r4, r0, #0x18
	lsr r4, r4, #0x18
	mov r1, #0x80
	lsl r1, r1, #1
	add r0, r1, #0
	add r1, r4, #0
	orr r1, r0
	ldrh r2, [r6]
	ldrh r3, [r6, #2]
	add r0, r7, #0
	bl DuelCmd_Push
	ldrb r5, [r5, #2]
	lsl r1, r5, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	lsl r4, r4, #8
	orr r1, r4
	ldr r3, [sp, #0x80]
	lsl r2, r3, #0x18
	ldr r4, [sp, #0x84]
	lsl r3, r4, #0x18
	lsr r2, r2, #8
	orr r2, r3
	lsr r2, r2, #0x10
	bl EquipCard
	mov r0, #0x64
	b _08039AF2
	.align 2, 0
_08039AB4: .4byte 0x00E000E0
_08039AB8: .4byte 0x0201CFB0
_08039ABC: .4byte 0x00000824
_08039AC0: .4byte 0x00000828
_08039AC4: .4byte 0x0000082C
_08039AC8: .4byte 0x00008008
_08039ACC: .4byte 0x00000544
_08039AD0: .4byte 0x000080D3
_08039AD4: .4byte 0x00008077
_08039AD8:
	ldr r1, _08039AEC @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08039AE6
	b _080399C6
_08039AE6:
	mov r0, #0x7F
	b _08039AF2
	.align 2, 0
_08039AEC: .4byte 0x03000040
_08039AF0:
	mov r0, #0
_08039AF2:
	add sp, #0x88
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectEquipSelfFromGraveyardResolve
	.align 2, 0

