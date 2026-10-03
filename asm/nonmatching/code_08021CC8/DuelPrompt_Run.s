	thumb_func_start DuelPrompt_Run
DuelPrompt_Run: @ 0x080222F8
	push {r4, lr}
	ldr r1, _08022310 @ =0x020192E0
	ldr r0, _08022314 @ =0x00001B50
	add r4, r1, r0
	ldrb r3, [r4]
	mov r0, #2
	and r0, r3
	add r2, r1, #0
	cmp r0, #0
	bne _08022318
_0802230C:
	mov r0, #0
	b _080225D2
_08022310: .4byte 0x020192E0
_08022314: .4byte 0x00001B50
_08022318:
	mov r0, #5
	and r0, r3
	cmp r0, #5
	bne _08022334
	ldr r0, _0802232C @ =0x02017FB0
	ldr r1, _08022330 @ =0x00000307
	add r0, r0, r1
	ldrb r0, [r0]
	lsr r0, r0, #7
	b _08022594
_0802232C: .4byte 0x02017FB0
_08022330: .4byte 0x00000307
_08022334:
	ldrh r4, [r4]
	lsl r0, r4, #0x16
	lsr r0, r0, #0x1A
	sub r0, #1
	cmp r0, #0x13
	bhi _0802230C
	lsl r0, r0, #2
	ldr r1, _0802234C @ =0x08022350
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0802234C: .4byte 0x08022350
_08022350:
	.4byte _080223A0
	.4byte _080223CC
	.4byte _080223FC
	.4byte _08022420
	.4byte _08022444
	.4byte _08022468
	.4byte _0802247C
	.4byte _08022490
	.4byte _08022496
	.4byte _0802249C
	.4byte _080224A2
	.4byte _080224A8
	.4byte _080224C4
	.4byte _080224D8
	.4byte _080224F8
	.4byte _08022518
	.4byte _08022538
	.4byte _0802254C
	.4byte _08022568
	.4byte _0802257C
_080223A0:
	ldr r3, _080223C8 @ =0x00001B50
	add r0, r2, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	add r3, #2
	add r1, r2, r3
	ldrh r1, [r1]
	add r3, #2
	add r2, r2, r3
	ldrh r4, [r2]
	mov r2, #1
	and r2, r4
	mov r3, #2
	and r3, r4
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	bl DuelPrompt_Discard
	b _08022590
_080223C8: .4byte 0x00001B50
_080223CC:
	ldr r1, _080223F4 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	ldr r3, _080223F8 @ =0x00001B52
	add r1, r2, r3
	ldrh r1, [r1]
	add r3, #2
	add r2, r2, r3
	ldrh r4, [r2]
	mov r2, #1
	and r2, r4
	mov r3, #2
	and r3, r4
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	bl DuelPrompt_DiscardCost
	b _08022590
_080223F4: .4byte 0x00001B50
_080223F8: .4byte 0x00001B52
_080223FC:
	ldr r1, _08022418 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	ldr r3, _0802241C @ =0x00001B52
	add r1, r2, r3
	ldrh r1, [r1]
	add r3, #2
	add r2, r2, r3
	ldrh r2, [r2]
	bl DuelPrompt_DiscardRandom
	b _08022590
_08022418: .4byte 0x00001B50
_0802241C: .4byte 0x00001B52
_08022420:
	ldr r1, _0802243C @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	ldr r3, _08022440 @ =0x00001B52
	add r1, r2, r3
	ldrh r1, [r1]
	add r3, #2
	add r2, r2, r3
	ldrh r2, [r2]
	bl DuelPrompt_BanishRandom
	b _08022590
_0802243C: .4byte 0x00001B50
_08022440: .4byte 0x00001B52
_08022444:
	ldr r1, _08022460 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	ldr r3, _08022464 @ =0x00001B52
	add r1, r2, r3
	ldrh r1, [r1]
	add r3, #2
	add r2, r2, r3
	ldrh r2, [r2]
	bl DuelPrompt_BanishRandomFaceDown
	b _08022590
_08022460: .4byte 0x00001B50
_08022464: .4byte 0x00001B52
_08022468:
	ldr r1, _08022478 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	bl DuelPrompt_PickOpponentHandCard
	b _08022590
_08022478: .4byte 0x00001B50
_0802247C:
	ldr r3, _0802248C @ =0x00001B50
	add r0, r2, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	bl DuelPrompt_Tribute
	b _08022590
_0802248C: .4byte 0x00001B50
_08022490:
	bl DuelPrompt_SelectType
	b _08022590
_08022496:
	bl DuelPrompt_SelectAttribute
	b _08022590
_0802249C:
	bl DuelPrompt_SelectTwoAttributes
	b _08022590
_080224A2:
	bl DuelPrompt_PickOneOfTwoAttributes
	b _08022590
_080224A8:
	ldr r1, _080224BC @ =0x00001B52
	add r0, r2, r1
	ldrh r0, [r0]
	ldr r3, _080224C0 @ =0x00001B54
	add r1, r2, r3
	ldrh r1, [r1]
	bl DuelPrompt_PickOneOfFiveCards
	b _08022590
	.align 2, 0
_080224BC: .4byte 0x00001B52
_080224C0: .4byte 0x00001B54
_080224C4:
	ldr r1, _080224D4 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	bl DuelPrompt_SetMonsterFromHand
	b _08022590
_080224D4: .4byte 0x00001B50
_080224D8:
	ldr r3, _080224F4 @ =0x00001B50
	add r0, r2, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	add r3, #2
	add r1, r2, r3
	ldrh r1, [r1]
	add r3, #2
	add r2, r2, r3
	ldrh r2, [r2]
	bl DuelPrompt_SelectGraveyardMonster
	b _08022590
_080224F4: .4byte 0x00001B50
_080224F8:
	ldr r1, _08022510 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	ldr r3, _08022514 @ =0x00001B52
	add r1, r2, r3
	ldrh r1, [r1]
	bl DuelPrompt_ConfirmCardEffect
	b _08022590
	.align 2, 0
_08022510: .4byte 0x00001B50
_08022514: .4byte 0x00001B52
_08022518:
	ldr r1, _08022530 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	ldr r3, _08022534 @ =0x00001B52
	add r1, r2, r3
	ldrh r1, [r1]
	bl DuelPrompt_SelectOpponentReplacementTarget
	b _08022590
	.align 2, 0
_08022530: .4byte 0x00001B50
_08022534: .4byte 0x00001B52
_08022538:
	ldr r1, _08022548 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	bl DuelPrompt_OfferDiscardMagic
	b _08022590
_08022548: .4byte 0x00001B50
_0802254C:
	ldr r3, _08022564 @ =0x00001B50
	add r0, r2, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	add r3, #2
	add r1, r2, r3
	ldrh r1, [r1]
	bl DuelPrompt_ConfirmSpecialSummon
	b _08022590
	.align 2, 0
_08022564: .4byte 0x00001B50
_08022568:
	ldr r1, _08022578 @ =0x00001B50
	add r0, r2, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	bl DuelPrompt_ConfirmGraveyardSummon
	b _08022590
_08022578: .4byte 0x00001B50
_0802257C:
	ldr r3, _080225C0 @ =0x00001B50
	add r0, r2, r3
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1F
	add r3, #2
	add r1, r2, r3
	ldrh r1, [r1]
	bl DuelPrompt_SelectOwnReplacementTarget
_08022590:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_08022594:
	cmp r0, #0
	beq _080225D0
	ldr r1, _080225C4 @ =0x020192E0
	ldr r0, _080225C0 @ =0x00001B50
	add r4, r1, r0
	mov r0, #5
	ldrb r2, [r4]
	and r0, r2
	cmp r0, #1
	bne _080225B4
	ldr r0, _080225C8 @ =0x0000F0A2
	ldr r3, _080225CC @ =0x00001B64
	add r1, r1, r3
	mov r2, #0x10
	bl DuelLink_SendMessageData
_080225B4:
	mov r0, #3
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	b _0802230C
_080225C0: .4byte 0x00001B50
_080225C4: .4byte 0x020192E0
_080225C8: .4byte 0x0000F0A2
_080225CC: .4byte 0x00001B64
_080225D0:
	mov r0, #1
_080225D2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end DuelPrompt_Run

