	thumb_func_start EffectTimeWizardResolve
EffectTimeWizardResolve: @ 0x080386B0
	push {r4, r5, r6, r7, lr}
	ldr r4, _080386E0 @ =0xFFFFFE00
	add sp, r4
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _080386C4
	b _08038C26
_080386C4:
	ldr r0, _080386E4 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	sub r0, #0x60
	cmp r0, #0x20
	bls _080386D6
	b _08038C08
_080386D6:
	lsl r0, r0, #2
	ldr r1, _080386E8 @ =0x080386EC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080386E0: .4byte 0xFFFFFE00
_080386E4: .4byte 0x02017A40
_080386E8: .4byte 0x080386EC
_080386EC:
	.4byte _08038BB4
	.4byte _08038B98
	.4byte _08038B66
	.4byte _08038B62
	.4byte _08038AF0
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038A64
	.4byte _08038A2C
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038A0E
	.4byte _08038990
	.4byte _08038938
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _08038C08
	.4byte _080387B4
	.4byte _08038770
_08038770:
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	cmp r0, #0
	bne _080387A0
	ldr r0, _08038794 @ =0x00000206
	ldr r1, _08038798 @ =0x00000613
	ldr r3, _0803879C @ =0x080831E4
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	b _080387AC
	.align 2, 0
_08038794: .4byte 0x00000206
_08038798: .4byte 0x00000613
_0803879C: .4byte gStrCoinTossSelection
_080387A0:
	bl Random
	ldr r2, _080387B0 @ =0x0201AE60
	mov r1, #1
	and r0, r1
	strh r0, [r2, #0x14]
_080387AC:
	mov r0, #0x7F
	b _08038C28
_080387B0: .4byte 0x0201AE60
_080387B4:
	bl Random
	add r2, r0, #0
	mov r0, #1
	and r2, r0
	add r7, r2, #0
	mov r4, #1
	add r0, r4, #0
	ldrb r3, [r5, #2]
	and r0, r3
	mov r3, #0xE0
	cmp r0, #0
	beq _080387D0
	ldr r3, _080388BC @ =0x000080E0
_080387D0:
	ldr r6, _080388C0 @ =0x0201AE60
	ldrh r1, [r6, #0x14]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r4, #0
	ldrb r1, [r5, #2]
	and r0, r1
	mov r1, #0x12
	cmp r0, #0
	beq _080387EA
	ldr r1, _080388C4 @ =0x00008012
_080387EA:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrh r6, [r6, #0x14]
	cmp r7, r6
	bne _080388DC
	mov r4, #0
	mov r6, #1
_08038800:
	ldrb r2, [r5, #2]
	lsl r3, r2, #0x1F
	lsr r1, r3, #0x1F
	sub r1, r6, r1
	and r1, r6
	mov r0, #0x94
	add r2, r4, #0
	mul r2, r0
	ldr r0, _080388C8 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _080388CC @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803883C
	lsr r0, r3, #0x1F
	sub r0, r6, r0
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	ldrb r3, [r5, #2]
	lsl r1, r3, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r6, r1
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_0803883C:
	add r4, #1
	cmp r4, #4
	ble _08038800
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #0
	bgt _08038852
	b _08038B5E
_08038852:
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xF
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bgt _08038866
	b _08038B5E
_08038866:
	ldr r4, _080388D0 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _08038876
	b _08038B5E
_08038876:
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _08038884
	b _08038B5E
_08038884:
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0x22
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bne _080388B6
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _080388D4 @ =0x000004BA
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bne _080388B6
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _080388D8 @ =0x000007F2
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bne _080388B6
	b _08038B5E
_080388B6:
	mov r0, #0x78
	b _08038C28
	.align 2, 0
_080388BC: .4byte 0x000080E0
_080388C0: .4byte 0x0201AE60
_080388C4: .4byte 0x00008012
_080388C8: .4byte 0x00000D64
_080388CC: .4byte 0x0201930C
_080388D0: .4byte 0x0000058A
_080388D4: .4byte 0x000004BA
_080388D8: .4byte 0x000007F2
_080388DC:
	mov r6, #0
	mov r4, #0
_080388E0:
	ldrb r2, [r5, #2]
	lsl r3, r2, #0x1F
	lsr r2, r3, #0x1F
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _08038930 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08038934 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08038914
	add r0, r2, #0
	add r1, r4, #0
	bl GetZoneCardAtk
	add r6, r6, r0
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl DestroyFieldCardByEffect
_08038914:
	add r4, #1
	cmp r4, #4
	ble _080388E0
	ldrb r5, [r5, #2]
	lsl r4, r5, #0x1F
	lsr r4, r4, #0x1F
	add r0, r6, #0
	bl HalveRoundDown
	add r1, r0, #0
	add r0, r4, #0
	bl LoseLifePoints
	b _08038B5E
_08038930: .4byte 0x00000D64
_08038934: .4byte 0x0201930C
_08038938:
	add r5, sp, #0x100
	ldr r1, _08038978 @ =0x08083214
	ldr r0, _0803897C @ =0x08623E38
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r4, _08038980 @ =0x0822C720
	add r2, r2, r4
	add r0, r5, #0
	bl FormatStr
	ldr r0, _08038984 @ =0x08624758
	ldrh r0, [r0]
	lsl r2, r0, #6
	add r2, r2, r4
	mov r0, sp
	add r1, r5, #0
	bl FormatStr
	ldr r0, _08038988 @ =0x00000206
	ldr r1, _0803898C @ =0x00000613
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x77
	b _08038C28
	.align 2, 0
_08038978: .4byte gStrPromptTributeToSpecialSummonFmt
_0803897C: .4byte gUnk_08623E38
_08038980: .4byte gCardNames
_08038984: .4byte gUnk_08624758
_08038988: .4byte 0x00000206
_0803898C: .4byte 0x00000613
_08038990:
	ldr r0, _080389C4 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _0803899A
	b _08038B5E
_0803899A:
	mov r7, #0
	mov r6, #0
	ldr r3, _080389C8 @ =0x0201D810
	mov r1, #0xC3
	lsl r1, r1, #2
	add r0, r3, r1
	ldrh r1, [r0]
	cmp r1, #0
	beq _080389D6
	mov r2, #0x83
	lsl r2, r2, #2
	add r0, r3, r2
	add r4, r1, #0
_080389B4:
	ldrh r2, [r0]
	cmp r2, #1
	beq _080389CC
	cmp r2, #2
	bne _080389CE
	mov r6, #1
	b _080389CE
	.align 2, 0
_080389C4: .4byte 0x0201AE60
_080389C8: .4byte 0x0201D810
_080389CC:
	mov r7, #1
_080389CE:
	add r0, #2
	sub r4, #1
	cmp r4, #0
	bne _080389B4
_080389D6:
	cmp r7, #0
	beq _08038A08
	cmp r6, #0
	beq _08038A04
	ldr r0, _080389F8 @ =0x00000206
	ldr r1, _080389FC @ =0x00000613
	ldr r3, _08038A00 @ =0x08083250
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x76
	b _08038C28
_080389F8: .4byte 0x00000206
_080389FC: .4byte 0x00000613
_08038A00: .4byte gStrPromptSummonFromHandOrDeck
_08038A04:
	mov r0, #1
	b _08038A26
_08038A08:
	cmp r6, #0
	bne _08038A24
	b _08038B5E
_08038A0E:
	ldr r0, _08038A1C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08038A20
	cmp r0, #1
	beq _08038A24
	b _08038A28
_08038A1C: .4byte 0x0201AE60
_08038A20:
	mov r0, #1
	b _08038A26
_08038A24:
	mov r0, #2
_08038A26:
	strh r0, [r5, #0xC]
_08038A28:
	mov r0, #0x6E
	b _08038C28
_08038A2C:
	ldr r1, _08038A50 @ =0x08083288
	ldr r0, _08038A54 @ =0x08623E38
	ldrh r0, [r0]
	lsl r2, r0, #6
	ldr r3, _08038A58 @ =0x0822C720
	add r2, r2, r3
	mov r0, sp
	bl FormatStr
	ldr r0, _08038A5C @ =0x00000206
	ldr r1, _08038A60 @ =0x00000613
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
_08038A4A:
	mov r0, #0x6D
	b _08038C28
	.align 2, 0
_08038A50: .4byte gStrTimeWizardSelectTributeFmt
_08038A54: .4byte gUnk_08623E38
_08038A58: .4byte gCardNames
_08038A5C: .4byte 0x00000206
_08038A60: .4byte 0x00000613
_08038A64:
	mov r0, #0xE0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08038A4A
	ldr r0, _08038AB4 @ =0x0201CFB0
	ldr r2, _08038AB8 @ =0x00000824
	add r1, r0, r2
	ldr r4, [r1]
	ldr r3, _08038ABC @ =0x00000828
	add r1, r0, r3
	add r2, #8
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r3, r1, r0
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _08038AC0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08038AC4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08038AC8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08038ACC @ =0x000004BA
	cmp r1, r0
	beq _08038AD6
	cmp r1, r0
	bgt _08038AD0
	cmp r1, #0x22
	beq _08038AD6
	b _08038AE8
	.align 2, 0
_08038AB4: .4byte 0x0201CFB0
_08038AB8: .4byte 0x00000824
_08038ABC: .4byte 0x00000828
_08038AC0: .4byte 0x00000D64
_08038AC4: .4byte 0x0201930C
_08038AC8: .4byte gCardIdToNumber
_08038ACC: .4byte 0x000004BA
_08038AD0:
	ldr r0, _08038AE4 @ =0x000007F2
	cmp r1, r0
	bne _08038AE8
_08038AD6:
	add r0, r4, #0
	add r1, r3, #0
	bl TributeMonster
	mov r0, #0x64
	b _08038C28
	.align 2, 0
_08038AE4: .4byte 0x000007F2
_08038AE8:
	mov r0, #3
	bl PlaySE
	b _08038A4A
_08038AF0:
	mov r4, #0
	ldr r1, _08038B00 @ =0x0201D810
	mov r2, #0xC3
	lsl r2, r2, #2
	add r0, r1, r2
	add r3, r1, #0
	b _08038B58
	.align 2, 0
_08038B00: .4byte 0x0201D810
_08038B04:
	lsl r0, r4, #1
	mov r2, #0x83
	lsl r2, r2, #2
	add r1, r3, r2
	add r0, r0, r1
	ldrh r0, [r0]
	ldrh r1, [r5, #0xC]
	cmp r0, r1
	bne _08038B50
	lsl r0, r4, #2
	add r1, r3, #0
	add r1, #0xC
	add r4, r0, r1
	mov r0, #1
	ldrb r2, [r5, #2]
	and r0, r2
	mov r3, #0xC2
	cmp r0, #0
	beq _08038B2C
	ldr r3, _08038B4C @ =0x000080C2
_08038B2C:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	mov r2, #1
	mov r3, #1
	bl QueueSpecialSummonChoosePosition
	mov r0, #0x63
	b _08038C28
_08038B4C: .4byte 0x000080C2
_08038B50:
	add r4, #1
	mov r1, #0xC3
	lsl r1, r1, #2
	add r0, r3, r1
_08038B58:
	ldrh r0, [r0]
	cmp r4, r0
	blt _08038B04
_08038B5E:
	mov r0, #0xA
	b _08038C28
_08038B62:
	mov r0, #0x62
	b _08038C28
_08038B66:
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08038B88 @ =0x000004B2
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	ble _08038B5E
	ldr r0, _08038B8C @ =0x00000206
	ldr r1, _08038B90 @ =0x00000613
	ldr r3, _08038B94 @ =0x080832AC
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x61
	b _08038C28
_08038B88: .4byte 0x000004B2
_08038B8C: .4byte 0x00000206
_08038B90: .4byte 0x00000613
_08038B94: .4byte gStrSelectMagicFromDeck
_08038B98:
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _08038BB0 @ =0x000004B2
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x60
	b _08038C28
	.align 2, 0
_08038BB0: .4byte 0x000004B2
_08038BB4:
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _08038BFC @ =0x0201D810
	ldrb r3, [r2, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r2, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r2, #0xC
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x15
	lsr r1, r1, #0x14
	ldr r2, _08038C00 @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	bl AddDeckCardToHand
	cmp r0, #0
	beq _08038B5E
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	mov r1, #0x60
	cmp r0, #0
	beq _08038BEE
	ldr r1, _08038C04 @ =0x00008060
_08038BEE:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _08038B5E
_08038BFC: .4byte 0x0201D810
_08038C00: .4byte gCardIdToNumber
_08038C04: .4byte 0x00008060
_08038C08:
	mov r0, #1
	ldrb r3, [r5, #2]
	and r0, r3
	mov r2, #0x92
	cmp r0, #0
	beq _08038C16
	ldr r2, _08038C34 @ =0x00008092
_08038C16:
	ldrh r5, [r5, #2]
	lsl r1, r5, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08038C26:
	mov r0, #0
_08038C28:
	mov r3, #0x80
	lsl r3, r3, #2
	add sp, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08038C34: .4byte 0x00008092
	thumb_func_end EffectTimeWizardResolve

