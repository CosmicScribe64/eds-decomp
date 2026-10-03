	thumb_func_start EffectTargetableFaceUpMonsterChainB
EffectTargetableFaceUpMonsterChainB: @ 0x0803F738
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	mov r2, #1
	ldrb r0, [r0, #2]
	and r2, r0
	cmp r2, #0
	beq _0803F7D8
	mov r0, #8
	neg r0, r0
	mov r1, r8
	ldrb r1, [r1, #0xA]
	and r0, r1
	mov r2, r8
	strb r0, [r2, #0xA]
	mov r6, #0
	mov r3, #1
	mov sl, r3
	ldr r0, _0803F7D0 @ =0x00000D64
	mov r9, r0
_0803F768:
	mov r5, #1
	neg r5, r5
	add r7, r5, #0
	mov r4, #0
	add r0, r6, #0
	mov r1, sl
	and r0, r1
	mov r2, r9
	mul r2, r0
_0803F77A:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r2
	ldr r1, _0803F7D4 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803F7AA
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803F7AA
	add r0, r6, #0
	add r1, r4, #0
	str r2, [sp, #0]
	bl GetZoneCardAtk
	ldr r2, [sp, #0]
	cmp r0, r7
	ble _0803F7AA
	add r7, r0, #0
	add r5, r4, #0
_0803F7AA:
	add r4, #1
	cmp r4, #4
	ble _0803F77A
	cmp r5, #0
	blt _0803F7C4
	mov r0, r8
	add r1, r6, #0
	add r2, r5, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803F7CA
_0803F7C4:
	add r6, #1
	cmp r6, #1
	ble _0803F768
_0803F7CA:
	mov r0, #1
	b _0803F8C4
	.align 2, 0
_0803F7D0: .4byte 0x00000D64
_0803F7D4: .4byte 0x0201930C
_0803F7D8:
	ldr r0, _0803F81C @ =0x02017A40
	ldr r1, _0803F820 @ =0x000003E5
	add r3, r0, r1
	ldrb r0, [r3]
	cmp r0, #0
	bne _0803F870
	mov r0, #8
	neg r0, r0
	mov r2, r8
	ldrb r2, [r2, #0xA]
	and r0, r2
	mov r3, r8
	strb r0, [r3, #0xA]
	ldr r0, _0803F824 @ =0x000007FF
	ldrh r1, [r3]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0803F828 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0803F82C @ =0x000003AB
	cmp r1, r0
	beq _0803F80C
	ldr r0, _0803F830 @ =0x000005AB
	cmp r1, r0
	bne _0803F840
_0803F80C:
	ldr r0, _0803F834 @ =0x00000206
	ldr r1, _0803F838 @ =0x00000712
	ldr r3, _0803F83C @ =0x08084318
	mov r2, #0xB
	bl TextBoxOpen
	b _0803F84C
	.align 2, 0
_0803F81C: .4byte 0x02017A40
_0803F820: .4byte 0x000003E5
_0803F824: .4byte 0x000007FF
_0803F828: .4byte gCardIdToNumber
_0803F82C: .4byte 0x000003AB
_0803F830: .4byte 0x000005AB
_0803F834: .4byte 0x00000206
_0803F838: .4byte 0x00000712
_0803F83C: .4byte gStrDesignateOneMonsterToDestroy
_0803F840:
	ldr r0, _0803F85C @ =0x00000206
	ldr r1, _0803F860 @ =0x00000712
	ldr r3, _0803F864 @ =0x08084044
	mov r2, #0xB
	bl TextBoxOpen
_0803F84C:
	ldr r0, _0803F868 @ =0x02017A40
	ldr r3, _0803F86C @ =0x000003E5
	add r0, r0, r3
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803F8C2
	.align 2, 0
_0803F85C: .4byte 0x00000206
_0803F860: .4byte 0x00000712
_0803F864: .4byte gStrDesignateOneMonster
_0803F868: .4byte 0x02017A40
_0803F86C: .4byte 0x000003E5
_0803F870:
	ldr r1, _0803F8AC @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0803F8C0
	ldr r0, _0803F8B0 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803F8C2
	ldr r0, _0803F8B4 @ =0x0201CFB0
	ldr r2, _0803F8B8 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803F8BC @ =0x00000828
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	mov r0, r8
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803F8C2
	b _0803F7CA
	.align 2, 0
_0803F8AC: .4byte 0x03000040
_0803F8B0: .4byte 0x00E000E0
_0803F8B4: .4byte 0x0201CFB0
_0803F8B8: .4byte 0x00000824
_0803F8BC: .4byte 0x00000828
_0803F8C0:
	strb r2, [r3]
_0803F8C2:
	mov r0, #0
_0803F8C4:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectTargetableFaceUpMonsterChainB

