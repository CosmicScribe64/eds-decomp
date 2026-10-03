	thumb_func_start EffectSanganResolve
EffectSanganResolve: @ 0x080301C0
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	beq _080301D0
	b _08030334
_080301D0:
	ldr r0, _080301E8 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _08030294
	cmp r0, #0x7F
	bgt _080301EC
	cmp r0, #0x7E
	beq _080302C0
	b _08030334
_080301E8: .4byte 0x02017A40
_080301EC:
	cmp r0, #0x80
	beq _080301F2
	b _08030334
_080301F2:
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803022C @ =0x000007FF
	ldrh r2, [r6]
	and r1, r2
	lsl r1, r1, #1
	ldr r3, _08030230 @ =0x08622AB4
	add r1, r1, r3
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bne _08030240
	mov r0, #1
	ldrb r6, [r6, #2]
	and r0, r6
	cmp r0, #0
	beq _0803021C
	b _08030334
_0803021C:
	ldr r0, _08030234 @ =0x00000205
	ldr r1, _08030238 @ =0x00000914
	ldr r3, _0803023C @ =0x0808284C
	mov r2, #0xB
	bl TextBoxOpen
_08030228:
	mov r0, #0x64
	b _08030336
_0803022C: .4byte 0x000007FF
_08030230: .4byte gCardIdToNumber
_08030234: .4byte 0x00000205
_08030238: .4byte 0x00000914
_0803023C: .4byte gStrNoDeckCardsToAdd
_08030240:
	mov r0, #1
	ldrb r1, [r6, #2]
	and r0, r1
	cmp r0, #0
	beq _08030278
	ldrh r0, [r6]
	bl AiPickCardListEntry
	ldr r1, _0803026C @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r2, [r1, #5]
	and r0, r2
	strb r0, [r1, #5]
	ldr r0, _08030270 @ =0x02015F00
	ldr r3, _08030274 @ =0x00001B22
	add r0, r0, r3
	ldrh r0, [r0]
	strh r0, [r1, #6]
	mov r0, #0x7E
	b _08030336
	.align 2, 0
_0803026C: .4byte 0x0201D810
_08030270: .4byte 0x02015F00
_08030274: .4byte 0x00001B22
_08030278:
	ldr r0, _08030288 @ =0x00000205
	ldr r1, _0803028C @ =0x00000914
	ldr r3, _08030290 @ =0x0808287C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7F
	b _08030336
_08030288: .4byte 0x00000205
_0803028C: .4byte 0x00000914
_08030290: .4byte gStrSelectDeckMonsterToAdd
_08030294:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _080302B8 @ =0x000007FF
	ldrh r6, [r6]
	and r2, r6
	lsl r2, r2, #1
	ldr r3, _080302BC @ =0x08622AB4
	add r2, r2, r3
	ldrh r2, [r2]
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7E
	b _08030336
	.align 2, 0
_080302B8: .4byte 0x000007FF
_080302BC: .4byte gCardIdToNumber
_080302C0:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r4, _08030328 @ =0x0201D810
	ldrb r2, [r4, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r4, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r5, r4, #0
	add r5, #0xC
	add r1, r1, r5
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl sub_08019820
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrb r2, [r4, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r4, [r4, #6]
	add r1, r4, r1
	lsl r1, r1, #2
	add r1, r1, r5
	ldr r1, [r1]
	lsl r1, r1, #0x15
	lsr r1, r1, #0x14
	ldr r3, _0803032C @ =0x08622AB4
	add r1, r1, r3
	ldrh r1, [r1]
	bl AddDeckCardToHand
	cmp r0, #0
	beq _08030228
	mov r0, #1
	ldrb r6, [r6, #2]
	and r0, r6
	mov r1, #0x60
	cmp r0, #0
	beq _0803031A
	ldr r1, _08030330 @ =0x00008060
_0803031A:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _08030228
_08030328: .4byte 0x0201D810
_0803032C: .4byte gCardIdToNumber
_08030330: .4byte 0x00008060
_08030334:
	mov r0, #0
_08030336:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectSanganResolve

