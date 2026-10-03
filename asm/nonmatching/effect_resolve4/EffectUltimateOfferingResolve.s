	thumb_func_start EffectUltimateOfferingResolve
EffectUltimateOfferingResolve: @ 0x08034768
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _0803477C
	b _08034B9C
_0803477C:
	ldr r0, _08034798 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	sub r0, #0x64
	cmp r0, #0x1C
	bls _0803478E
	b _08034B9C
_0803478E:
	lsl r0, r0, #2
	ldr r1, _0803479C @ =0x080347A0
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08034798: .4byte 0x02017A40
_0803479C: .4byte 0x080347A0
_080347A0:
	.4byte _08034B6C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B00
	.4byte _08034AD0
	.4byte _08034AA8
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034A38
	.4byte _08034A18
	.4byte _080349B0
	.4byte _08034984
	.4byte _0803495C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034B9C
	.4byte _08034888
	.4byte _08034814
_08034814:
	mov r6, #0
	ldr r1, _0803487C @ =0x020192E4
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _08034880 @ =0x00000D64
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp r6, r0
	blt _0803482C
	b _08034B9C
_0803482C:
	mov r7, #1
	mov r8, r1
_08034830:
	lsl r0, r2, #0x1F
	lsr r2, r0, #0x1F
	add r1, r7, #0
	and r1, r2
	lsl r2, r6, #2
	mul r1, r3
	add r2, r2, r1
	ldr r1, _08034884 @ =0x02019968
	add r2, r2, r1
	ldr r1, [r2]
	lsl r1, r1, #0x14
	lsr r4, r1, #0x14
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl CanSummonFromHand
	cmp r0, #0
	beq _08034860
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08034860
	b _08034B80
_08034860:
	add r6, #1
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r7, #0
	and r1, r0
	ldr r3, _08034880 @ =0x00000D64
	add r0, r1, #0
	mul r0, r3
	add r0, r8
	ldrb r0, [r0, #2]
	cmp r6, r0
	blt _08034830
	b _08034B9C
_0803487C: .4byte 0x020192E4
_08034880: .4byte 0x00000D64
_08034884: .4byte 0x02019968
_08034888:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08034894
	b _08034B8C
_08034894:
	ldr r0, _080348F0 @ =0x0201CFB0
	ldr r3, _080348F4 @ =0x0000082C
	add r0, r0, r3
	ldr r6, [r0]
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r3, r0, #0x1F
	lsl r1, r6, #2
	ldr r2, _080348F8 @ =0x00000D64
	mul r2, r3
	add r1, r1, r2
	ldr r2, _080348FC @ =0x02019968
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r4, r1, #0x14
	add r0, r3, #0
	add r1, r4, #0
	bl CanSummonFromHand
	cmp r0, #0
	beq _08034954
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08034954
	strh r6, [r5, #0xC]
	ldr r0, _08034900 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r2, _08034904 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08034910
	cmp r0, #0x17
	ble _08034908
	cmp r0, #0x18
	beq _0803490C
	b _08034910
	.align 2, 0
_080348F0: .4byte 0x0201CFB0
_080348F4: .4byte 0x0000082C
_080348F8: .4byte 0x00000D64
_080348FC: .4byte 0x02019968
_08034900: .4byte 0x000007FF
_08034904: .4byte gCardStats
_08034908:
	mov r0, #0
	b _08034924
_0803490C:
	mov r0, #0xA
	b _08034924
_08034910:
	ldr r0, _08034940 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r3, _08034944 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08034924:
	cmp r0, #1
	blt _08034950
	cmp r0, #4
	bgt _08034948
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	mov r1, #0
	strh r0, [r5, #0xE]
	strh r1, [r5, #0x10]
	mov r0, #0x64
	b _08034B9E
_08034940: .4byte 0x000007FF
_08034944: .4byte gCardStats
_08034948:
	cmp r0, #6
	bgt _08034950
	mov r0, #0x6E
	b _08034B9E
_08034950:
	mov r0, #0x78
	b _08034B9E
_08034954:
	mov r0, #3
	bl PlaySE
	b _08034B8C
_0803495C:
	ldr r0, _08034978 @ =0x00000206
	ldr r1, _0803497C @ =0x00000712
	ldr r2, _08034980 @ =0x0819D1C4
	ldr r3, [r2, #4]
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x77
	b _08034B9E
_08034978: .4byte 0x00000206
_0803497C: .4byte 0x00000712
_08034980: .4byte gTributeSummonPrompts
_08034984:
	ldr r0, _080349A0 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _0803498E
	b _08034AD8
_0803498E:
	ldr r0, _080349A4 @ =0x00000206
	ldr r1, _080349A8 @ =0x00000412
	ldr r2, _080349AC @ =0x0819D1C4
	ldr r3, [r2, #8]
	mov r2, #0xB
	bl TextBoxOpen
_0803499C:
	mov r0, #0x76
	b _08034B9E
_080349A0: .4byte 0x0201AE60
_080349A4: .4byte 0x00000206
_080349A8: .4byte 0x00000412
_080349AC: .4byte gTributeSummonPrompts
_080349B0:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803499C
	ldr r4, _08034A00 @ =0x0201CFB0
	ldr r2, _08034A04 @ =0x00000824
	add r7, r4, r2
	ldr r0, [r7]
	ldr r3, _08034A08 @ =0x0000082C
	add r6, r4, r3
	ldr r1, [r6]
	bl IsTributableMonster
	cmp r0, #0
	beq _08034A10
	mov r0, #1
	bl PlaySE
	ldrh r1, [r7]
	ldr r2, _08034A0C @ =0x00000828
	add r0, r4, r2
	ldrb r3, [r6]
	lsl r2, r3, #8
	ldrb r0, [r0]
	orr r2, r0
	mov r0, #8
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, [r6]
	strh r0, [r5, #0xE]
	mov r2, #0x80
	neg r2, r2
	add r1, r2, #0
	orr r0, r1
	lsl r0, r0, #8
	strh r0, [r5, #0x10]
	mov r0, #0x75
	b _08034B9E
_08034A00: .4byte 0x0201CFB0
_08034A04: .4byte 0x00000824
_08034A08: .4byte 0x0000082C
_08034A0C: .4byte 0x00000828
_08034A10:
	mov r0, #3
	bl PlaySE
	b _0803499C
_08034A18:
	ldr r0, _08034A2C @ =0x00000206
	ldr r1, _08034A30 @ =0x00000412
	ldr r2, _08034A34 @ =0x0819D1C4
	ldr r3, [r2, #0xC]
	mov r2, #0xB
	bl TextBoxOpen
_08034A26:
	mov r0, #0x74
	b _08034B9E
	.align 2, 0
_08034A2C: .4byte 0x00000206
_08034A30: .4byte 0x00000412
_08034A34: .4byte gTributeSummonPrompts
_08034A38:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08034A26
	ldr r4, _08034A90 @ =0x0201CFB0
	ldr r3, _08034A94 @ =0x00000824
	add r7, r4, r3
	ldr r0, [r7]
	ldr r1, _08034A98 @ =0x0000082C
	add r6, r4, r1
	ldr r1, [r6]
	bl IsTributableMonster
	cmp r0, #0
	beq _08034AA0
	ldr r0, [r6]
	ldrh r2, [r5, #0xE]
	cmp r0, r2
	beq _08034AA0
	mov r0, #1
	bl PlaySE
	ldrh r1, [r7]
	ldr r3, _08034A9C @ =0x00000828
	add r0, r4, r3
	ldrb r3, [r6]
	lsl r2, r3, #8
	ldrb r0, [r0]
	orr r2, r0
	mov r0, #8
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, [r6]
	mov r2, #0x80
	neg r2, r2
	add r1, r2, #0
	orr r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	ldrh r3, [r5, #0x10]
	orr r0, r3
	b _08034B4C
_08034A90: .4byte 0x0201CFB0
_08034A94: .4byte 0x00000824
_08034A98: .4byte 0x0000082C
_08034A9C: .4byte 0x00000828
_08034AA0:
	mov r0, #3
	bl PlaySE
	b _08034A26
_08034AA8:
	ldr r0, _08034AC4 @ =0x00000206
	ldr r1, _08034AC8 @ =0x00000712
	ldr r2, _08034ACC @ =0x0819D1C4
	ldr r3, [r2]
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x6D
	b _08034B9E
_08034AC4: .4byte 0x00000206
_08034AC8: .4byte 0x00000712
_08034ACC: .4byte gTributeSummonPrompts
_08034AD0:
	ldr r0, _08034ADC @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _08034AE0
_08034AD8:
	mov r0, #0x80
	b _08034B9E
_08034ADC: .4byte 0x0201AE60
_08034AE0:
	ldr r0, _08034AF4 @ =0x00000206
	ldr r1, _08034AF8 @ =0x00000412
	ldr r2, _08034AFC @ =0x0819D1C4
	ldr r3, [r2, #0x10]
	mov r2, #0xB
	bl TextBoxOpen
_08034AEE:
	mov r0, #0x6C
	b _08034B9E
	.align 2, 0
_08034AF4: .4byte 0x00000206
_08034AF8: .4byte 0x00000412
_08034AFC: .4byte gTributeSummonPrompts
_08034B00:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08034AEE
	ldr r4, _08034B54 @ =0x0201CFB0
	ldr r0, _08034B58 @ =0x00000824
	add r7, r4, r0
	ldr r0, [r7]
	ldr r1, _08034B5C @ =0x0000082C
	add r6, r4, r1
	ldr r1, [r6]
	bl IsTributableMonster
	cmp r0, #0
	beq _08034B64
	mov r0, #1
	bl PlaySE
	ldrh r1, [r7]
	ldr r2, _08034B60 @ =0x00000828
	add r0, r4, r2
	ldrb r3, [r6]
	lsl r2, r3, #8
	ldrb r0, [r0]
	orr r2, r0
	mov r0, #8
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, [r6]
	strh r0, [r5, #0xE]
	mov r2, #0x80
	neg r2, r2
	add r1, r2, #0
	orr r0, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
_08034B4C:
	strh r0, [r5, #0x10]
	mov r0, #0x64
	b _08034B9E
	.align 2, 0
_08034B54: .4byte 0x0201CFB0
_08034B58: .4byte 0x00000824
_08034B5C: .4byte 0x0000082C
_08034B60: .4byte 0x00000828
_08034B64:
	mov r0, #3
	bl PlaySE
	b _08034AEE
_08034B6C:
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r5, #0xC]
	ldrh r2, [r5, #0xE]
	ldrh r3, [r5, #0x10]
	bl QueueNormalSummonChoosePosition
	mov r0, #0xA
	b _08034B9E
_08034B80:
	ldr r0, _08034B90 @ =0x00000206
	ldr r1, _08034B94 @ =0x00000712
	ldr r3, _08034B98 @ =0x08082D24
	mov r2, #0xB
	bl TextBoxOpen
_08034B8C:
	mov r0, #0x7F
	b _08034B9E
_08034B90: .4byte 0x00000206
_08034B94: .4byte 0x00000712
_08034B98: .4byte gStrSelectMonsterToSummonFromHand
_08034B9C:
	mov r0, #0
_08034B9E:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectUltimateOfferingResolve

