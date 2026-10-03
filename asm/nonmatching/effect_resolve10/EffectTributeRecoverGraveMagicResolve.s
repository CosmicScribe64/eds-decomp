	thumb_func_start EffectTributeRecoverGraveMagicResolve
EffectTributeRecoverGraveMagicResolve: @ 0x0803B5B8
	push {lr}
	add r1, r0, #0
	ldr r0, _0803B5D4 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _0803B610
	cmp r0, #0x7F
	bgt _0803B5D8
	cmp r0, #0x7E
	beq _0803B62C
	b _0803B668
_0803B5D4: .4byte 0x02017A40
_0803B5D8:
	cmp r0, #0x80
	bne _0803B668
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803B600 @ =0x0000059F
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	beq _0803B668
	ldr r0, _0803B604 @ =0x00000206
	ldr r1, _0803B608 @ =0x00000712
	ldr r3, _0803B60C @ =0x0808379C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #0x7F
	b _0803B66A
	.align 2, 0
_0803B600: .4byte 0x0000059F
_0803B604: .4byte 0x00000206
_0803B608: .4byte 0x00000712
_0803B60C: .4byte gStrSelectGraveMagicToDeck
_0803B610:
	ldrb r1, [r1, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803B628 @ =0x0000059F
	mov r3, #0
	bl CardListView_Open
	mov r0, #0x7E
	b _0803B66A
	.align 2, 0
_0803B628: .4byte 0x0000059F
_0803B62C:
	mov r0, #1
	ldrb r1, [r1, #2]
	and r0, r1
	mov r2, #0xD1
	cmp r0, #0
	beq _0803B63A
	ldr r2, _0803B660 @ =0x000080D1
_0803B63A:
	ldr r0, _0803B664 @ =0x0201D810
	ldrb r3, [r0, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r1, r1, r0
	ldrh r1, [r1, #0xC]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x78
	b _0803B66A
	.align 2, 0
_0803B660: .4byte 0x000080D1
_0803B664: .4byte 0x0201D810
_0803B668:
	mov r0, #0
_0803B66A:
	pop {r1}
	bx r1
	thumb_func_end EffectTributeRecoverGraveMagicResolve
	.align 2, 0

