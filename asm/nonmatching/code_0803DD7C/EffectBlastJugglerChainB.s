	thumb_func_start EffectBlastJugglerChainB
EffectBlastJugglerChainB: @ 0x0803E6DC
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldr r1, _0803E6FC @ =0x02017A40
	ldr r2, _0803E700 @ =0x000003E5
	add r0, r1, r2
	ldrb r0, [r0]
	add r2, r1, #0
	cmp r0, #4
	bls _0803E6F0
	b _0803E8EE
_0803E6F0:
	lsl r0, r0, #2
	ldr r1, _0803E704 @ =0x0803E708
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803E6FC: .4byte 0x02017A40
_0803E700: .4byte 0x000003E5
_0803E704: .4byte 0x0803E708
_0803E708:
	.4byte _0803E71C
	.4byte _0803E77C
	.4byte _0803E7B4
	.4byte _0803E850
	.4byte _0803E86C
_0803E71C:
	mov r0, #8
	neg r0, r0
	ldrb r3, [r4, #0xA]
	and r0, r3
	strb r0, [r4, #0xA]
	mov r0, #3
	ldrb r1, [r4, #3]
	and r0, r1
	mov r1, #8
	orr r0, r1
	strb r0, [r4, #3]
	add r0, r4, #0
	mov r1, #0
	mov r2, #0
	bl EffectBlastJugglerPrepare
	cmp r0, #0
	bne _0803E742
	b _0803E8D4
_0803E742:
	ldr r0, _0803E768 @ =0x00000206
	ldr r1, _0803E76C @ =0x00000712
	ldr r3, _0803E770 @ =0x08083ED0
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
_0803E758:
	ldr r0, _0803E774 @ =0x02017A40
	ldr r2, _0803E778 @ =0x000003E5
	add r0, r0, r2
_0803E75E:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803E8EE
	.align 2, 0
_0803E768: .4byte 0x00000206
_0803E76C: .4byte 0x00000712
_0803E770: .4byte gStrAskDestroyMonster
_0803E774: .4byte 0x02017A40
_0803E778: .4byte 0x000003E5
_0803E77C:
	ldr r0, _0803E79C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _0803E786
	b _0803E8D4
_0803E786:
	ldr r0, _0803E7A0 @ =0x00000206
	ldr r1, _0803E7A4 @ =0x00000712
	ldr r3, _0803E7A8 @ =0x08083EF4
	mov r2, #0xB
	bl TextBoxOpen
	ldr r0, _0803E7AC @ =0x02017A40
	ldr r3, _0803E7B0 @ =0x000003E5
	add r0, r0, r3
	b _0803E75E
	.align 2, 0
_0803E79C: .4byte 0x0201AE60
_0803E7A0: .4byte 0x00000206
_0803E7A4: .4byte 0x00000712
_0803E7A8: .4byte gStrDesignateAtk1000MonsterToDestroy
_0803E7AC: .4byte 0x02017A40
_0803E7B0: .4byte 0x000003E5
_0803E7B4:
	ldr r1, _0803E7C8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803E7D0
	ldr r0, _0803E7CC @ =0x000003E5
	add r1, r2, r0
	b _0803E87C
	.align 2, 0
_0803E7C8: .4byte 0x03000040
_0803E7CC: .4byte 0x000003E5
_0803E7D0:
	ldr r0, _0803E838 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _0803E7DC
	b _0803E8EE
_0803E7DC:
	ldr r0, _0803E83C @ =0x0201CFB0
	ldr r1, _0803E840 @ =0x00000824
	add r2, r0, r1
	ldr r3, _0803E844 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r5, r1, r0
	ldr r6, [r2]
	lsl r1, r5, #0x18
	lsr r1, r1, #0x10
	ldrb r2, [r2]
	orr r1, r2
	add r0, r4, #0
	bl EffectBlastJugglerCheck
	cmp r0, #0
	beq _0803E8E8
	add r0, r4, #0
	add r1, r6, #0
	add r2, r5, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803E8E8
	mov r0, #0
	bl CountMonsters
	add r4, r0, #0
	mov r0, #1
	bl CountMonsters
	add r4, r4, r0
	cmp r4, #1
	beq _0803E8D4
	ldr r0, _0803E848 @ =0x02017A40
	ldr r1, _0803E84C @ =0x000003E5
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803E8E8
	.align 2, 0
_0803E838: .4byte 0x00E000E0
_0803E83C: .4byte 0x0201CFB0
_0803E840: .4byte 0x00000824
_0803E844: .4byte 0x00000828
_0803E848: .4byte 0x02017A40
_0803E84C: .4byte 0x000003E5
_0803E850:
	ldr r0, _0803E860 @ =0x00000206
	ldr r1, _0803E864 @ =0x00000712
	ldr r3, _0803E868 @ =0x08083F38
	mov r2, #0xB
	bl TextBoxOpen
	b _0803E758
	.align 2, 0
_0803E860: .4byte 0x00000206
_0803E864: .4byte 0x00000712
_0803E868: .4byte gStrSelectAnotherMonsterToDestroy
_0803E86C:
	ldr r1, _0803E884 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803E88C
	ldr r3, _0803E888 @ =0x000003E5
	add r1, r2, r3
_0803E87C:
	mov r0, #0
	strb r0, [r1]
	b _0803E8F0
	.align 2, 0
_0803E884: .4byte 0x03000040
_0803E888: .4byte 0x000003E5
_0803E88C:
	ldr r0, _0803E8D8 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803E8EE
	ldr r0, _0803E8DC @ =0x0201CFB0
	ldr r1, _0803E8E0 @ =0x00000824
	add r2, r0, r1
	ldr r3, _0803E8E4 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r5, r1, r0
	ldr r6, [r2]
	lsl r0, r5, #0x18
	lsr r1, r0, #0x10
	ldrb r2, [r2]
	orr r1, r2
	ldrh r0, [r4, #0xC]
	cmp r0, r1
	beq _0803E8E8
	add r0, r4, #0
	bl EffectBlastJugglerCheck
	cmp r0, #0
	beq _0803E8E8
	add r0, r4, #0
	add r1, r6, #0
	add r2, r5, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803E8E8
_0803E8D4:
	mov r0, #1
	b _0803E8F0
_0803E8D8: .4byte 0x00E000E0
_0803E8DC: .4byte 0x0201CFB0
_0803E8E0: .4byte 0x00000824
_0803E8E4: .4byte 0x00000828
_0803E8E8:
	mov r0, #3
	bl PlaySE
_0803E8EE:
	mov r0, #0
_0803E8F0:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectBlastJugglerChainB
	.align 2, 0

