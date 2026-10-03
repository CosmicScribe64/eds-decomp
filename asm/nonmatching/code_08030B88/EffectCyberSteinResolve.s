	thumb_func_start EffectCyberSteinResolve
EffectCyberSteinResolve: @ 0x08031550
	push {r4, r5, r6, lr}
	sub sp, #4
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, #0
	beq _08031566
	b _0803176C
_08031566:
	ldr r0, _08031584 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7E
	bne _08031576
	b _080316A8
_08031576:
	cmp r0, #0x7E
	bgt _08031588
	cmp r0, #0x7D
	bne _08031580
	b _080316E0
_08031580:
	b _0803176C
	.align 2, 0
_08031584: .4byte 0x02017A40
_08031588:
	cmp r0, #0x7F
	beq _0803167C
	cmp r0, #0x80
	beq _08031592
	b _0803176C
_08031592:
	ldr r2, _080315C8 @ =0x020192E4
	ldrb r6, [r5, #2]
	lsl r3, r6, #0x1F
	mov r4, #1
	lsr r1, r3, #0x1F
	ldr r0, _080315CC @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #5]
	cmp r0, #0
	bne _080315AA
	b _0803176C
_080315AA:
	ldr r0, _080315D0 @ =0x000007FF
	ldrh r1, [r5]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _080315D4 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _080315D8 @ =0x000001A3
	cmp r1, r0
	beq _080315DC
	add r0, #0x56
	cmp r1, r0
	beq _0803162C
	b _0803176C
	.align 2, 0
_080315C8: .4byte 0x020192E4
_080315CC: .4byte 0x00000D64
_080315D0: .4byte 0x000007FF
_080315D4: .4byte gCardIdToNumber
_080315D8: .4byte 0x000001A3
_080315DC:
	lsr r0, r3, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #0
	bne _080315E8
	b _0803176C
_080315E8:
	ldrb r3, [r5, #2]
	and r4, r3
	cmp r4, #0
	beq _08031618
	ldrh r0, [r5]
	bl AiPickCardListEntry
	ldr r1, _0803160C @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r4, [r1, #5]
	and r0, r4
	strb r0, [r1, #5]
	ldr r0, _08031610 @ =0x02015F00
	ldr r2, _08031614 @ =0x00001B22
	add r0, r0, r2
	b _0803164A
	.align 2, 0
_0803160C: .4byte 0x0201D810
_08031610: .4byte 0x02015F00
_08031614: .4byte 0x00001B22
_08031618:
	ldr r0, _08031620 @ =0x00000205
	ldr r1, _08031624 @ =0x00000914
	ldr r3, _08031628 @ =0x080829F0
	b _08031666
_08031620: .4byte 0x00000205
_08031624: .4byte 0x00000914
_08031628: .4byte gStrCyberSteinSelectPrompt
_0803162C:
	and r4, r6
	cmp r4, #0
	beq _08031660
	ldrh r0, [r5]
	bl AiPickCardListEntry
	ldr r1, _08031654 @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r3, [r1, #5]
	and r0, r3
	strb r0, [r1, #5]
	ldr r0, _08031658 @ =0x02015F00
	ldr r4, _0803165C @ =0x00001B22
	add r0, r0, r4
_0803164A:
	ldrh r0, [r0]
	strh r0, [r1, #6]
	mov r0, #0x7E
	b _0803176E
	.align 2, 0
_08031654: .4byte 0x0201D810
_08031658: .4byte 0x02015F00
_0803165C: .4byte 0x00001B22
_08031660:
	ldr r0, _08031670 @ =0x00000205
	ldr r1, _08031674 @ =0x00000914
	ldr r3, _08031678 @ =0x08082A4C
_08031666:
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7F
	b _0803176E
_08031670: .4byte 0x00000205
_08031674: .4byte 0x00000914
_08031678: .4byte gStrGaleDograSelectPrompt
_0803167C:
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _080316A0 @ =0x000007FF
	ldrh r5, [r5]
	and r2, r5
	lsl r2, r2, #1
	ldr r3, _080316A4 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7E
	b _0803176E
	.align 2, 0
_080316A0: .4byte 0x000007FF
_080316A4: .4byte gCardIdToNumber
_080316A8:
	ldr r0, _080316D8 @ =0x0201D810
	ldrb r4, [r0, #5]
	lsl r1, r4, #0x1E
	lsr r1, r1, #0x1E
	ldrh r2, [r0, #6]
	add r1, r2, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	mov r3, #0xDC
	cmp r0, #0
	beq _080316C8
	ldr r3, _080316DC @ =0x000080DC
_080316C8:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7D
	b _0803176E
_080316D8: .4byte 0x0201D810
_080316DC: .4byte 0x000080DC
_080316E0:
	ldr r0, _080316FC @ =0x000007FF
	ldrh r4, [r5]
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _08031700 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08031704 @ =0x000001A3
	cmp r1, r0
	beq _08031708
	add r0, #0x56
	cmp r1, r0
	beq _08031734
	b _0803176C
_080316FC: .4byte 0x000007FF
_08031700: .4byte gCardIdToNumber
_08031704: .4byte 0x000001A3
_08031708:
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _08031730 @ =0x0201D810
	ldrb r4, [r2, #5]
	lsl r1, r4, #0x1E
	lsr r1, r1, #0x1E
	ldrh r4, [r2, #6]
	add r1, r4, r1
	lsl r1, r1, #2
	add r2, #0xC
	add r1, r1, r2
	str r3, [sp, #0]
	mov r2, #1
	mov r3, #0
	bl QueueSpecialSummon
	mov r0, #0x64
	b _0803176E
	.align 2, 0
_08031730: .4byte 0x0201D810
_08031734:
	ldr r0, _08031778 @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	mov r0, #1
	ldrb r4, [r5, #2]
	and r0, r4
	mov r3, #0x7C
	cmp r0, #0
	beq _08031754
	ldr r3, _0803177C @ =0x0000807C
_08031754:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	bl LoseLpOnSendToGraveyard
_0803176C:
	mov r0, #0
_0803176E:
	add sp, #4
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08031778: .4byte 0x0201D810
_0803177C: .4byte 0x0000807C
	thumb_func_end EffectCyberSteinResolve

