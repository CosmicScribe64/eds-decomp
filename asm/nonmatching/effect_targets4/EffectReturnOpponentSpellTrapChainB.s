	thumb_func_start EffectReturnOpponentSpellTrapChainB
EffectReturnOpponentSpellTrapChainB: @ 0x08041BC0
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _08041BF0 @ =0x02017A40
	ldr r1, _08041BF4 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _08041C04
	ldr r0, _08041BF8 @ =0x00000206
	ldr r1, _08041BFC @ =0x00000712
	ldr r3, _08041C00 @ =0x08084C98
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	b _08041C58
	.align 2, 0
_08041BF0: .4byte 0x02017A40
_08041BF4: .4byte 0x000003E5
_08041BF8: .4byte 0x00000206
_08041BFC: .4byte 0x00000712
_08041C00: .4byte gStrDesignateOpponentSpellTrapToReturn
_08041C04:
	ldr r1, _08041C18 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08041C1C
	mov r0, #0
	strb r0, [r5]
	b _08041C5A
	.align 2, 0
_08041C18: .4byte 0x03000040
_08041C1C:
	mov r0, #0xE0
	lsl r0, r0, #0xC
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08041C58
	ldr r0, _08041C50 @ =0x0201CFB0
	ldr r3, _08041C54 @ =0x00000824
	add r1, r0, r3
	ldr r1, [r1]
	add r3, #4
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08041C58
	mov r0, #1
	b _08041C5A
	.align 2, 0
_08041C50: .4byte 0x0201CFB0
_08041C54: .4byte 0x00000824
_08041C58:
	mov r0, #0
_08041C5A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectReturnOpponentSpellTrapChainB

