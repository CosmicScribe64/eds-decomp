	thumb_func_start EffectDestroyAndGiveMonsterChainB
EffectDestroyAndGiveMonsterChainB: @ 0x08040B14
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r0, _08040B30 @ =0x02017A40
	ldr r1, _08040B34 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #1
	beq _08040B68
	cmp r0, #1
	bgt _08040B38
	cmp r0, #0
	beq _08040B42
	b _08040C08
	.align 2, 0
_08040B30: .4byte 0x02017A40
_08040B34: .4byte 0x000003E5
_08040B38:
	cmp r0, #2
	beq _08040B98
	cmp r0, #3
	beq _08040BB4
	b _08040C08
_08040B42:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	ldr r0, _08040B5C @ =0x00000206
	ldr r1, _08040B60 @ =0x00000712
	ldr r3, _08040B64 @ =0x080844E4
	mov r2, #0xB
	bl TextBoxOpen
	b _08040BF2
	.align 2, 0
_08040B5C: .4byte 0x00000206
_08040B60: .4byte 0x00000712
_08040B64: .4byte gStrDesignateOpponentMonsterToDestroy
_08040B68:
	ldr r1, _08040B8C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08040BC0
	mov r0, #0xF0
	lsl r0, r0, #0x10
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08040BF8
	ldr r0, _08040B90 @ =0x0201CFB0
	ldr r3, _08040B94 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	b _08040BE0
_08040B8C: .4byte 0x03000040
_08040B90: .4byte 0x0201CFB0
_08040B94: .4byte 0x00000824
_08040B98:
	ldr r0, _08040BA8 @ =0x00000206
	ldr r1, _08040BAC @ =0x00000712
	ldr r3, _08040BB0 @ =0x08084888
	mov r2, #0xB
	bl TextBoxOpen
	b _08040BF2
	.align 2, 0
_08040BA8: .4byte 0x00000206
_08040BAC: .4byte 0x00000712
_08040BB0: .4byte gStrDesignateMonsterToGiveControl
_08040BB4:
	ldr r1, _08040BC8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08040BCC
_08040BC0:
	mov r0, #0
	strb r0, [r4]
	b _08040C0A
	.align 2, 0
_08040BC8: .4byte 0x03000040
_08040BCC:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08040BF8
	ldr r0, _08040BFC @ =0x0201CFB0
	ldr r2, _08040C00 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _08040C04 @ =0x00000828
_08040BE0:
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r5, #0
	bl TryAddEffectTarget
_08040BF2:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_08040BF8:
	mov r0, #0
	b _08040C0A
_08040BFC: .4byte 0x0201CFB0
_08040C00: .4byte 0x00000824
_08040C04: .4byte 0x00000828
_08040C08:
	mov r0, #1
_08040C0A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectDestroyAndGiveMonsterChainB

