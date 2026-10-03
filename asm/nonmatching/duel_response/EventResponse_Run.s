	thumb_func_start EventResponse_Run
EventResponse_Run: @ 0x0804244C
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	sub sp, #0x100
	ldr r0, _0804247C @ =0x02017A40
	mov r1, #0x92
	lsl r1, r1, #3
	add r5, r0, r1
	ldrb r3, [r5]
	add r4, r3, #0
	mov r8, r0
	cmp r4, #0xB
	bne _0804246A
	b _0804272C
_0804246A:
	cmp r4, #0xB
	bgt _0804248E
	cmp r4, #1
	beq _0804251C
	cmp r4, #1
	bgt _08042480
	cmp r4, #0
	beq _080424B4
	b _08042A8E
_0804247C: .4byte 0x02017A40
_08042480:
	cmp r4, #2
	bne _08042486
	b _08042598
_08042486:
	cmp r4, #0xA
	bne _0804248C
	b _080425CC
_0804248C:
	b _08042A8E
_0804248E:
	cmp r4, #0xC8
	bne _08042494
	b _080428FC
_08042494:
	cmp r4, #0xC8
	bgt _080424A6
	cmp r4, #0x64
	bne _0804249E
	b _08042884
_0804249E:
	cmp r4, #0x65
	bne _080424A4
	b _080428C4
_080424A4:
	b _08042A8E
_080424A6:
	cmp r4, #0xC9
	bne _080424AC
	b _0804297C
_080424AC:
	cmp r4, #0xF0
	bne _080424B2
	b _08042A3C
_080424B2:
	b _08042A8E
_080424B4:
	ldr r1, _08042544 @ =0x00000491
	add r1, r8
	mov r0, #0x7F
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r1, #0x95
	lsl r1, r1, #3
	add r1, r8
	strh r4, [r1]
	lsl r0, r0, #0x1B
	ldr r3, _08042548 @ =0x000004AA
	add r3, r8
	lsr r2, r0, #0x1F
	mov r1, #2
	neg r1, r1
	ldrb r4, [r3]
	and r1, r4
	orr r1, r2
	strb r1, [r3]
	ldr r1, _0804254C @ =0x0000048A
	add r1, r8
	ldr r3, _08042550 @ =0x000004AB
	add r3, r8
	ldrb r1, [r1]
	lsl r2, r1, #2
	mov r1, #3
	ldrb r7, [r3]
	and r1, r7
	orr r1, r2
	strb r1, [r3]
	ldr r1, _08042554 @ =0x0000048C
	add r1, r8
	ldr r1, [r1]
	ldr r2, _08042558 @ =0x000004AE
	add r2, r8
	strh r1, [r2]
	lsr r1, r1, #0x10
	mov r2, #0x96
	lsl r2, r2, #3
	add r2, r8
	strh r1, [r2]
	lsr r0, r0, #0x1F
	bl EventResponse_CanPlayerRespond
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08042516
	b _080428E2
_08042516:
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
_0804251C:
	ldr r1, _08042544 @ =0x00000491
	add r1, r8
	mov r0, #0x10
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08042560
	ldr r1, _0804255C @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	mov r1, #0xC8
	cmp r0, #0
	beq _0804253A
	mov r1, #0x64
_0804253A:
	mov r0, #0x92
	lsl r0, r0, #3
	add r0, r8
	strb r1, [r0]
	b _080425BC
_08042544: .4byte 0x00000491
_08042548: .4byte 0x000004AA
_0804254C: .4byte 0x0000048A
_08042550: .4byte 0x000004AB
_08042554: .4byte 0x0000048C
_08042558: .4byte 0x000004AE
_0804255C: .4byte 0x02015EE8
_08042560:
	mov r0, #0x95
	lsl r0, r0, #3
	add r0, r8
	mov r1, sp
	bl EventResponse_BuildPromptText
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _08042594 @ =0x00000916
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r1, #0x92
	lsl r1, r1, #3
	add r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _080425BC
	.align 2, 0
_08042594: .4byte 0x00000916
_08042598:
	ldr r0, _080425C0 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _080425A2
	b _080428E2
_080425A2:
	mov r0, #0xA
	strb r0, [r5]
	ldr r2, _080425C4 @ =0x020192E0
	ldr r0, _080425C8 @ =0x00001B2C
	add r2, r2, r0
	mov r0, #2
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	neg r1, r1
	and r0, r1
	strb r0, [r2]
_080425BC:
	mov r0, #0
	b _08042A9E
_080425C0: .4byte 0x0201AE60
_080425C4: .4byte 0x020192E0
_080425C8: .4byte 0x00001B2C
_080425CC:
	ldr r6, _080425E4 @ =0x020192E0
	ldr r2, _080425E8 @ =0x00001B2C
	add r0, r6, r2
	ldrb r1, [r0]
	mov r4, #1
	add r0, r4, #0
	and r0, r1
	cmp r0, #0
	beq _080425EC
	bl CardMenu_Update
	b _080425BC
_080425E4: .4byte 0x020192E0
_080425E8: .4byte 0x00001B2C
_080425EC:
	mov r2, #2
	add r0, r2, #0
	and r0, r1
	cmp r0, #0
	beq _080425FA
	add r0, r3, #1
	b _080428E4
_080425FA:
	ldr r1, _0804260C @ =0x03000040
	add r0, r2, #0
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08042610
	strb r4, [r5]
	b _080425BC
	.align 2, 0
_0804260C: .4byte 0x03000040
_08042610:
	mov r3, #0xE
	ldr r4, _08042658 @ =0x00001B12
	add r1, r6, r4
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _08042622
	mov r3, #0xF
_08042622:
	add r0, r3, #0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _080425BC
	ldr r0, _0804265C @ =0x0201CFB0
	ldr r7, _08042660 @ =0x00000824
	add r1, r0, r7
	ldr r6, [r1]
	ldr r2, _08042664 @ =0x00000828
	add r1, r0, r2
	ldr r5, [r1]
	ldr r3, _08042668 @ =0x0000082C
	add r0, r0, r3
	ldr r7, [r0]
	bl DuelCursor_GetCardId
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r5, #0xF
	bhi _080425BC
	lsl r0, r5, #2
	ldr r1, _0804266C @ =0x08042670
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08042658: .4byte 0x00001B12
_0804265C: .4byte 0x0201CFB0
_08042660: .4byte 0x00000824
_08042664: .4byte 0x00000828
_08042668: .4byte 0x0000082C
_0804266C: .4byte 0x08042670
_08042670:
	.4byte _080426B0
	.4byte _080425BC
	.4byte _080425BC
	.4byte _080425BC
	.4byte _080425BC
	.4byte _080426B0
	.4byte _080425BC
	.4byte _080425BC
	.4byte _080425BC
	.4byte _080425BC
	.4byte _080426B0
	.4byte _080426B0
	.4byte _08042718
	.4byte _08042714
	.4byte _08042718
	.4byte _08042718
_080426B0:
	cmp r2, #0
	beq _08042710
	ldr r1, _080426F8 @ =0x020192E0
	ldr r0, _080426FC @ =0x00001B2C
	add r4, r1, r0
	mov r0, #1
	ldrb r2, [r4]
	orr r0, r2
	strb r0, [r4]
	ldr r3, _08042700 @ =0x00001B2F
	add r2, r1, r3
	mov r0, #3
	ldrb r3, [r2]
	and r0, r3
	strb r0, [r2]
	ldr r0, _08042704 @ =0x00001B30
	add r1, r1, r0
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _08042708 @ =0x02017EE8
	add r1, r6, #0
	add r2, r5, #0
	add r3, r7, #0
	bl EventResponse_GetCommands
	lsl r0, r0, #0x10
	lsr r0, r0, #6
	ldr r1, [r4]
	ldr r2, _0804270C @ =0xFC0003FF
	and r1, r2
	orr r1, r0
	str r1, [r4]
	b _080425BC
_080426F8: .4byte 0x020192E0
_080426FC: .4byte 0x00001B2C
_08042700: .4byte 0x00001B2F
_08042704: .4byte 0x00001B30
_08042708: .4byte 0x02017EE8
_0804270C: .4byte 0xFC0003FF
_08042710:
	mov r0, #3
	b _08042726
_08042714:
	mov r0, #3
	b _08042726
_08042718:
	add r0, r6, #0
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl CardListView_Open
	mov r0, #1
_08042726:
	bl PlaySE
	b _080425BC
_0804272C:
	ldr r6, _08042770 @ =0x020192E0
	ldr r3, _08042774 @ =0x00001B33
	add r0, r6, r3
	ldrb r4, [r0]
	lsr r7, r4, #2
	mov ip, r7
	ldr r0, _08042778 @ =0x00001B34
	add r3, r6, r0
	mov r5, #1
	add r0, r5, #0
	ldrb r1, [r3]
	and r0, r1
	lsl r0, r0, #6
	orr r0, r7
	cmp r0, #5
	beq _08042780
	cmp r0, #0xB
	bne _08042840
	mov r2, #0x95
	lsl r2, r2, #3
	add r2, r8
	mov r0, #1
	mov r1, #0
	bl CardMenu_PlaySpellTrapFromHand
	ldr r2, _0804277C @ =0x00001B2C
	add r1, r6, r2
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _08042840
	b _080425BC
	.align 2, 0
_08042770: .4byte 0x020192E0
_08042774: .4byte 0x00001B33
_08042778: .4byte 0x00001B34
_0804277C: .4byte 0x00001B2C
_08042780:
	ldr r7, _08042858 @ =0x00001B2C
	add r1, r6, r7
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	lsl r0, r4, #0x1E
	lsr r0, r0, #0x1F
	add r2, r5, #0
	and r2, r0
	ldrh r0, [r3]
	lsl r7, r0, #0x17
	lsr r1, r7, #0x18
	add r0, r5, #0
	ldrb r3, [r3]
	and r0, r3
	lsl r3, r0, #6
	mov r0, ip
	orr r3, r0
	add r1, r1, r3
	mov r0, #0x94
	mul r1, r0
	ldr r0, _0804285C @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r0, r6, #0
	add r0, #0x2C
	add r1, r1, r0
	mov r2, #2
	add r0, r2, #0
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _080427E0
	add r0, r2, #0
	and r0, r4
	mov r2, #0x7F
	cmp r0, #0
	beq _080427D2
	ldr r2, _08042860 @ =0x0000807F
_080427D2:
	lsr r1, r7, #0x18
	add r1, r1, r3
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080427E0:
	ldr r6, _08042864 @ =0x020192E0
	ldr r1, _08042868 @ =0x00001B33
	add r0, r6, r1
	ldrb r4, [r0]
	lsl r0, r4, #0x1E
	mov r2, #1
	lsr r0, r0, #0x1F
	lsl r0, r0, #0x1F
	ldr r5, _0804286C @ =0x02017A40
	ldr r3, _08042870 @ =0x000004AB
	add r1, r5, r3
	ldrb r1, [r1]
	lsr r1, r1, #2
	lsl r1, r1, #0x19
	orr r0, r1
	ldr r7, _08042874 @ =0x00001B34
	add r3, r6, r7
	ldrh r7, [r3]
	lsl r1, r7, #0x17
	lsr r1, r1, #0x18
	lsr r4, r4, #2
	ldrb r3, [r3]
	and r2, r3
	lsl r2, r2, #6
	orr r2, r4
	add r1, r1, r2
	mov r2, #0x1F
	and r1, r2
	lsl r1, r1, #0x10
	mov r2, #0x80
	lsl r2, r2, #0xE
	orr r1, r2
	orr r0, r1
	ldr r1, _08042878 @ =0x00001B28
	add r6, r6, r1
	ldrh r6, [r6]
	orr r0, r6
	ldr r3, _0804287C @ =0x000004AE
	add r2, r5, r3
	mov r4, #0x96
	lsl r4, r4, #3
	add r5, r5, r4
	ldrh r5, [r5]
	lsl r1, r5, #0x10
	ldrh r2, [r2]
	orr r1, r2
	bl Chain_AddPending
_08042840:
	ldr r0, _0804286C @ =0x02017A40
	ldr r7, _08042880 @ =0x00000491
	add r0, r0, r7
	mov r1, #0x80
	ldrb r2, [r0]
	orr r1, r2
	mov r2, #0x41
	neg r2, r2
	and r1, r2
	strb r1, [r0]
	b _08042A9C
	.align 2, 0
_08042858: .4byte 0x00001B2C
_0804285C: .4byte 0x00000D64
_08042860: .4byte 0x0000807F
_08042864: .4byte 0x020192E0
_08042868: .4byte 0x00001B33
_0804286C: .4byte 0x02017A40
_08042870: .4byte 0x000004AB
_08042874: .4byte 0x00001B34
_08042878: .4byte 0x00001B28
_0804287C: .4byte 0x000004AE
_08042880: .4byte 0x00000491
_08042884:
	ldr r0, _080428B4 @ =0x0000F051
	mov r1, #0x95
	lsl r1, r1, #3
	add r1, r8
	mov r2, #0x14
	bl DuelLink_SendMessageData
	ldr r1, _080428B8 @ =0x00000491
	add r1, r8
	mov r0, #0x7F
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	ldr r1, _080428BC @ =0x02017FB0
	ldr r4, _080428C0 @ =0x00000307
	add r1, r1, r4
	mov r0, #9
	neg r0, r0
	ldrb r7, [r1]
	and r0, r7
	strb r0, [r1]
	ldrb r0, [r5]
	add r0, #1
	b _080428E4
_080428B4: .4byte 0x0000F051
_080428B8: .4byte 0x00000491
_080428BC: .4byte 0x02017FB0
_080428C0: .4byte 0x00000307
_080428C4:
	ldr r0, _080428E8 @ =0x02017FB0
	ldr r1, _080428EC @ =0x00000307
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1C
	cmp r0, #0
	blt _080428D4
	b _080425BC
_080428D4:
	ldr r1, _080428F0 @ =0x00000491
	add r1, r8
	ldrb r4, [r1]
	mov r0, #0x80
	and r0, r4
	cmp r0, #0
	bne _080428F4
_080428E2:
	mov r0, #0xF0
_080428E4:
	strb r0, [r5]
	b _080425BC
_080428E8: .4byte 0x02017FB0
_080428EC: .4byte 0x00000307
_080428F0: .4byte 0x00000491
_080428F4:
	mov r0, #0x41
	neg r0, r0
	and r0, r4
	b _08042A9A
_080428FC:
	mov r5, #5
	mov r9, r8
	ldr r7, _0804296C @ =0x00000491
	add r7, r8
_08042904:
	ldrb r3, [r7]
	lsl r2, r3, #0x1B
	lsr r2, r2, #0x1F
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _08042970 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08042974 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08042958
	bl GetCardSpellSpeed
	add r4, r0, #0
	mov r6, #0x95
	lsl r6, r6, #3
	add r6, r8
	ldrb r0, [r7]
	lsl r1, r0, #0x1B
	lsr r1, r1, #0x1F
	add r0, r6, #0
	add r2, r5, #0
	bl CanActivateFieldCard
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r4, #1
	ble _08042958
	cmp r0, #0
	beq _08042958
	add r0, r6, #0
	mov r1, #0
	bl AiShouldActivateSetCard
	cmp r0, #0
	beq _08042958
	b _08042A70
_08042958:
	add r5, #1
	ldr r1, _08042978 @ =0x02017A40
	mov r8, r1
	cmp r5, #9
	ble _08042904
	mov r1, #0x92
	lsl r1, r1, #3
	add r1, r8
	mov r0, #0xF0
	b _08042A8A
_0804296C: .4byte 0x00000491
_08042970: .4byte 0x00000D64
_08042974: .4byte 0x0201930C
_08042978: .4byte 0x02017A40
_0804297C:
	ldr r0, _08042A20 @ =0x00000491
	add r0, r8
	ldrb r3, [r0]
	lsl r2, r3, #0x1B
	lsr r2, r2, #0x1F
	lsl r4, r3, #0x1C
	lsr r1, r4, #0x1C
	mov r0, #0x94
	mul r1, r0
	ldr r0, _08042A24 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08042A28 @ =0x0201930C
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _080429BA
	mov r0, #0x10
	and r0, r3
	mov r2, #0x7F
	cmp r0, #0
	beq _080429AE
	ldr r2, _08042A2C @ =0x0000807F
_080429AE:
	lsr r1, r4, #0x1C
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080429BA:
	ldr r5, _08042A30 @ =0x02017A40
	ldr r2, _08042A20 @ =0x00000491
	add r6, r5, r2
	ldrb r3, [r6]
	lsl r4, r3, #0x1B
	lsr r0, r4, #0x1F
	lsl r0, r0, #0x1F
	ldr r7, _08042A34 @ =0x000004AB
	add r1, r5, r7
	ldrb r1, [r1]
	lsr r1, r1, #2
	lsl r1, r1, #0x19
	orr r0, r1
	lsl r3, r3, #0x1C
	lsr r1, r3, #0xC
	mov r2, #0x80
	lsl r2, r2, #0xE
	orr r1, r2
	orr r0, r1
	lsr r4, r4, #0x1F
	lsr r3, r3, #0x1C
	mov r1, #0x94
	mul r1, r3
	ldr r2, _08042A24 @ =0x00000D64
	mul r2, r4
	add r1, r1, r2
	ldr r2, _08042A28 @ =0x0201930C
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	orr r0, r1
	ldr r1, _08042A38 @ =0x000004AE
	add r2, r5, r1
	mov r3, #0x96
	lsl r3, r3, #3
	add r5, r5, r3
	ldrh r5, [r5]
	lsl r1, r5, #0x10
	ldrh r2, [r2]
	orr r1, r2
	bl Chain_AddPending
	mov r0, #0x80
	ldrb r4, [r6]
	orr r0, r4
	mov r1, #0x41
	neg r1, r1
	and r0, r1
	strb r0, [r6]
	b _08042A9C
_08042A20: .4byte 0x00000491
_08042A24: .4byte 0x00000D64
_08042A28: .4byte 0x0201930C
_08042A2C: .4byte 0x0000807F
_08042A30: .4byte 0x02017A40
_08042A34: .4byte 0x000004AB
_08042A38: .4byte 0x000004AE
_08042A3C:
	ldr r3, _08042A6C @ =0x00000491
	add r3, r8
	ldrb r2, [r3]
	lsl r0, r2, #0x1B
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	lsl r1, r1, #4
	mov r0, #0x11
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	lsl r1, r0, #0x1B
	lsl r0, r0, #0x1A
	lsr r1, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r1, r0
	beq _08042A9C
	mov r0, #0
	strb r0, [r5]
	b _08042A9E
_08042A6C: .4byte 0x00000491
_08042A70:
	mov r0, #0xF
	and r5, r0
	mov r0, #0x10
	neg r0, r0
	ldrb r1, [r7]
	and r0, r1
	orr r0, r5
	strb r0, [r7]
	mov r1, #0x92
	lsl r1, r1, #3
	add r1, r9
	ldrb r0, [r1]
	add r0, #1
_08042A8A:
	strb r0, [r1]
	b _080425BC
_08042A8E:
	ldr r1, _08042AAC @ =0x00000491
	add r1, r8
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
_08042A9A:
	strb r0, [r1]
_08042A9C:
	mov r0, #1
_08042A9E:
	add sp, #0x100
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08042AAC: .4byte 0x00000491
	thumb_func_end EventResponse_Run

