	thumb_func_start EffectMagicalHatsResolve
EffectMagicalHatsResolve: @ 0x080334E4
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	beq _080334FA
	b _080338BC
_080334FA:
	ldr r0, _08033518 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	sub r0, #0x79
	cmp r0, #7
	bls _0803350C
	b _080338BC
_0803350C:
	lsl r0, r0, #2
	ldr r1, _0803351C @ =0x08033520
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08033518: .4byte 0x02017A40
_0803351C: .4byte 0x08033520
_08033520:
	.4byte _08033898
	.4byte _080337BC
	.4byte _08033750
	.4byte _08033688
	.4byte _0803365C
	.4byte _08033600
	.4byte _080335D4
	.4byte _08033540
_08033540:
	mov r4, #7
	ldrb r3, [r7, #0xA]
	and r4, r3
	cmp r4, #1
	beq _0803354C
	b _080338BC
_0803354C:
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _080335B8 @ =0x000007FF
	ldrh r2, [r7]
	and r1, r2
	lsl r1, r1, #1
	ldr r3, _080335BC @ =0x08622AB4
	add r1, r1, r3
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #1
	bgt _0803356C
	b _080338BC
_0803356C:
	ldrb r2, [r7, #0xC]
	ldrh r0, [r7, #0xC]
	lsr r1, r0, #8
	and r4, r2
	mov r0, #0x94
	mul r0, r1
	ldr r1, _080335C0 @ =0x00000D64
	mul r1, r4
	add r0, r0, r1
	ldr r1, _080335C4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0803358C
	b _080338BC
_0803358C:
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r2
	beq _08033598
	b _080338BC
_08033598:
	add r0, r7, #0
	mov r1, #0
	mov r2, #0
	bl EffectAttackResponsePrepare
	cmp r0, #0
	bne _080335A8
	b _080338BC
_080335A8:
	ldr r0, _080335C8 @ =0x00000205
	ldr r1, _080335CC @ =0x00000914
	ldr r3, _080335D0 @ =0x08082B9C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7F
	b _080338BE
_080335B8: .4byte 0x000007FF
_080335BC: .4byte gCardIdToNumber
_080335C0: .4byte 0x00000D64
_080335C4: .4byte 0x0201930C
_080335C8: .4byte 0x00000205
_080335CC: .4byte 0x00000914
_080335D0: .4byte gStrMagicalHatsSelectFirst
_080335D4:
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _080335F8 @ =0x000007FF
	ldrh r7, [r7]
	and r2, r7
	lsl r2, r2, #1
	ldr r3, _080335FC @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7E
	b _080338BE
	.align 2, 0
_080335F8: .4byte 0x000007FF
_080335FC: .4byte gCardIdToNumber
_08033600:
	ldr r1, _08033644 @ =0x0201D810
	ldrb r2, [r1, #5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	ldrh r3, [r1, #6]
	add r0, r3, r0
	lsl r0, r0, #2
	add r1, #0xC
	add r4, r0, r1
	ldr r0, _08033648 @ =0x02017F84
	add r1, r4, #0
	bl CopyDuelCard
	mov r0, #1
	ldrb r7, [r7, #2]
	and r0, r7
	mov r3, #0x65
	cmp r0, #0
	beq _08033628
	ldr r3, _0803364C @ =0x00008065
_08033628:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _08033650 @ =0x00000205
	ldr r1, _08033654 @ =0x00000914
	ldr r3, _08033658 @ =0x08082BD0
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7D
	b _080338BE
_08033644: .4byte 0x0201D810
_08033648: .4byte 0x02017F84
_0803364C: .4byte 0x00008065
_08033650: .4byte 0x00000205
_08033654: .4byte 0x00000914
_08033658: .4byte gStrMagicalHatsSelectSecond
_0803365C:
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _08033680 @ =0x000007FF
	ldrh r7, [r7]
	and r2, r7
	lsl r2, r2, #1
	ldr r3, _08033684 @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7C
	b _080338BE
	.align 2, 0
_08033680: .4byte 0x000007FF
_08033684: .4byte gCardIdToNumber
_08033688:
	ldr r0, _08033730 @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r4, r1, r0
	ldr r0, _08033734 @ =0x02017F88
	mov r8, r0
	add r1, r4, #0
	bl CopyDuelCard
	mov r1, #1
	mov r9, r1
	mov r0, r9
	ldrb r2, [r7, #2]
	and r0, r2
	mov r3, #0x65
	cmp r0, #0
	beq _080336B6
	ldr r3, _08033738 @ =0x00008065
_080336B6:
	ldrh r1, [r4]
	ldrh r2, [r4, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, r8
	add r0, #4
	ldrh r2, [r7, #0xC]
	mov r1, r9
	and r1, r2
	ldr r6, _0803373C @ =0x00000D64
	mul r1, r6
	ldr r5, _08033740 @ =0x0201930C
	add r1, r1, r5
	lsr r2, r2, #8
	mov r4, #0x94
	mul r2, r4
	add r1, r1, r2
	bl CopyDuelCard
	ldr r0, _08033744 @ =0xFFFFFEA4
	add r0, r8
	ldrh r2, [r7, #0xC]
	mov r1, r9
	and r1, r2
	mul r1, r6
	add r1, r1, r5
	lsr r2, r2, #8
	mul r2, r4
	add r1, r1, r2
	mov r2, #0x94
	bl MemCopy16
	ldrh r1, [r7, #0xC]
	ldrb r0, [r7, #0xC]
	mov r2, #0x78
	cmp r0, #0
	beq _08033706
	ldr r2, _08033748 @ =0x00008078
_08033706:
	lsr r1, r1, #8
	add r0, r2, #0
	mov r2, #8
	mov r3, #0
	bl DuelCmd_Push
	ldrh r1, [r7, #0xC]
	ldrb r0, [r7, #0xC]
	mov r2, #0x8D
	cmp r0, #0
	beq _0803371E
	ldr r2, _0803374C @ =0x0000808D
_0803371E:
	lsr r1, r1, #8
	add r0, r2, #0
	mov r2, #0xA
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7B
	b _080338BE
	.align 2, 0
_08033730: .4byte 0x0201D810
_08033734: .4byte 0x02017F88
_08033738: .4byte 0x00008065
_0803373C: .4byte 0x00000D64
_08033740: .4byte 0x0201930C
_08033744: .4byte 0xFFFFFEA4
_08033748: .4byte 0x00008078
_0803374C: .4byte 0x0000808D
_08033750:
	mov r0, #0
	ldr r7, _080337B0 @ =0x02017E28
	mov r3, #0xAE
	lsl r3, r3, #1
	add r3, r3, r7
	mov r8, r3
_0803375C:
	add r6, r0, #1
_0803375E:
	bl Random
	mov r1, #3
	bl __modsi3
	add r5, r0, #0
	bl Random
	mov r1, #3
	bl __modsi3
	add r4, r0, #0
	cmp r5, r4
	beq _0803375E
	lsl r5, r5, #2
	add r5, r8
	add r0, r7, #0
	add r1, r5, #0
	bl CopyDuelCard
	lsl r4, r4, #2
	add r4, r8
	add r0, r5, #0
	add r1, r4, #0
	bl CopyDuelCard
	add r0, r4, #0
	add r1, r7, #0
	bl CopyDuelCard
	add r0, r6, #0
	cmp r0, #0xF
	ble _0803375C
	ldr r0, _080337B4 @ =0x02017A40
	ldr r1, _080337B8 @ =0x000003E1
	add r0, r0, r1
	mov r1, #0
	strb r1, [r0]
	mov r0, #0x7A
	b _080338BE
	.align 2, 0
_080337B0: .4byte 0x02017E28
_080337B4: .4byte 0x02017A40
_080337B8: .4byte 0x000003E1
_080337BC:
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	mov r8, r0
	ldr r0, _08033878 @ =0x02017A40
	ldr r3, _0803387C @ =0x000003E1
	add r1, r0, r3
	ldrb r1, [r1]
	lsl r1, r1, #2
	ldr r2, _08033880 @ =0x00000544
	add r0, r0, r2
	add r4, r1, r0
	mov r5, #0
	ldr r6, _08033884 @ =0x0000047F
	mov r0, #0
	add r1, r6, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _080337F4
	mov r0, #1
	add r1, r6, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _080337F6
_080337F4:
	mov r5, #1
_080337F6:
	mov r0, #1
	ldrb r3, [r7, #2]
	and r0, r3
	mov r6, #0xA8
	cmp r0, #0
	beq _08033804
	ldr r6, _08033888 @ =0x000080A8
_08033804:
	mov r0, r8
	lsl r1, r0, #0x18
	lsr r1, r1, #0x18
	mov r0, #2
	orr r5, r0
	lsl r0, r5, #8
	orr r1, r0
	ldrh r2, [r4]
	ldrh r3, [r4, #2]
	add r0, r6, #0
	bl DuelCmd_Push
	ldr r1, _08033878 @ =0x02017A40
	ldr r2, _0803387C @ =0x000003E1
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #2
	add r0, r0, r1
	ldr r3, _08033880 @ =0x00000544
	add r0, r0, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r1, _0803388C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0803385E
	ldrb r0, [r7, #0xC]
	mov r1, #0x8D
	cmp r0, #0
	beq _0803384E
	ldr r1, _08033890 @ =0x0000808D
_0803384E:
	mov r3, r8
	lsl r2, r3, #0x10
	lsr r2, r2, #0x10
	add r0, r1, #0
	mov r1, #0xA
	mov r3, #0
	bl DuelCmd_Push
_0803385E:
	ldr r1, _08033878 @ =0x02017A40
	ldr r0, _0803387C @ =0x000003E1
	add r1, r1, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #2
	bhi _08033894
	mov r0, #0x7A
	b _080338BE
	.align 2, 0
_08033878: .4byte 0x02017A40
_0803387C: .4byte 0x000003E1
_08033880: .4byte 0x00000544
_08033884: .4byte 0x0000047F
_08033888: .4byte 0x000080A8
_0803388C: .4byte gCardStats
_08033890: .4byte 0x0000808D
_08033894:
	mov r0, #0x79
	b _080338BE
_08033898:
	mov r0, #1
	ldrb r7, [r7, #2]
	and r0, r7
	mov r1, #0x60
	cmp r0, #0
	beq _080338A6
	ldr r1, _080338B8 @ =0x00008060
_080338A6:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x78
	b _080338BE
	.align 2, 0
_080338B8: .4byte 0x00008060
_080338BC:
	mov r0, #0
_080338BE:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectMagicalHatsResolve
	.align 2, 0

