	thumb_func_start EffectTwoProngedAttackChainB
EffectTwoProngedAttackChainB: @ 0x0803FC88
	push {r4, lr}
	add r4, r0, #0
	ldr r1, _0803FCA8 @ =0x02017A40
	ldr r2, _0803FCAC @ =0x000003E5
	add r0, r1, r2
	ldrb r0, [r0]
	add r2, r1, #0
	cmp r0, #5
	bls _0803FC9C
	b _0803FE68
_0803FC9C:
	lsl r0, r0, #2
	ldr r1, _0803FCB0 @ =0x0803FCB4
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0803FCA8: .4byte 0x02017A40
_0803FCAC: .4byte 0x000003E5
_0803FCB0: .4byte 0x0803FCB4
_0803FCB4:
	.4byte _0803FCCC
	.4byte _0803FCF0
	.4byte _0803FD3C
	.4byte _0803FD64
	.4byte _0803FDD4
	.4byte _0803FE04
_0803FCCC:
	ldr r0, _0803FCE4 @ =0x00000206
	ldr r1, _0803FCE8 @ =0x00000712
	ldr r3, _0803FCEC @ =0x0808449C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #8
	neg r0, r0
	ldrb r3, [r4, #0xA]
	and r0, r3
	strb r0, [r4, #0xA]
	b _0803FDE0
_0803FCE4: .4byte 0x00000206
_0803FCE8: .4byte 0x00000712
_0803FCEC: .4byte gStrDesignateFirstOwnMonster
_0803FCF0:
	ldr r1, _0803FD2C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803FCFE
	b _0803FE10
_0803FCFE:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803FDEC
	ldr r0, _0803FD30 @ =0x0201CFB0
	ldr r2, _0803FD34 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803FD38 @ =0x00000828
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
	beq _0803FDEC
	b _0803FDE0
_0803FD2C: .4byte 0x03000040
_0803FD30: .4byte 0x0201CFB0
_0803FD34: .4byte 0x00000824
_0803FD38: .4byte 0x00000828
_0803FD3C:
	ldr r0, _0803FD50 @ =0x00000206
	ldr r1, _0803FD54 @ =0x00000712
	ldr r3, _0803FD58 @ =0x080844C0
	mov r2, #0xB
	bl TextBoxOpen
	ldr r0, _0803FD5C @ =0x02017A40
	ldr r2, _0803FD60 @ =0x000003E5
	add r0, r0, r2
	b _0803FDE6
_0803FD50: .4byte 0x00000206
_0803FD54: .4byte 0x00000712
_0803FD58: .4byte gStrDesignateSecondOwnMonster
_0803FD5C: .4byte 0x02017A40
_0803FD60: .4byte 0x000003E5
_0803FD64:
	ldr r1, _0803FDB4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0803FE10
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803FDEC
	ldr r0, _0803FDB8 @ =0x0201CFB0
	ldr r1, _0803FDBC @ =0x00000824
	add r2, r0, r1
	ldr r3, _0803FDC0 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r3, r1, r0
	ldr r1, [r2]
	lsl r0, r3, #0x18
	lsr r0, r0, #0x10
	ldrb r2, [r2]
	orr r0, r2
	ldrh r2, [r4, #0xC]
	cmp r2, r0
	beq _0803FDCC
	add r0, r4, #0
	add r2, r3, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803FDEC
	ldr r0, _0803FDC4 @ =0x02017A40
	ldr r3, _0803FDC8 @ =0x000003E5
	add r0, r0, r3
	b _0803FDE6
_0803FDB4: .4byte 0x03000040
_0803FDB8: .4byte 0x0201CFB0
_0803FDBC: .4byte 0x00000824
_0803FDC0: .4byte 0x00000828
_0803FDC4: .4byte 0x02017A40
_0803FDC8: .4byte 0x000003E5
_0803FDCC:
	mov r0, #3
	bl PlaySE
	b _0803FDEC
_0803FDD4:
	ldr r0, _0803FDF0 @ =0x00000206
	ldr r1, _0803FDF4 @ =0x00000712
	ldr r3, _0803FDF8 @ =0x080844E4
	mov r2, #0xB
	bl TextBoxOpen
_0803FDE0:
	ldr r0, _0803FDFC @ =0x02017A40
	ldr r1, _0803FE00 @ =0x000003E5
	add r0, r0, r1
_0803FDE6:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_0803FDEC:
	mov r0, #0
	b _0803FE6A
_0803FDF0: .4byte 0x00000206
_0803FDF4: .4byte 0x00000712
_0803FDF8: .4byte gStrDesignateOpponentMonsterToDestroy
_0803FDFC: .4byte 0x02017A40
_0803FE00: .4byte 0x000003E5
_0803FE04:
	ldr r1, _0803FE1C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803FE24
_0803FE10:
	ldr r3, _0803FE20 @ =0x000003E5
	add r1, r2, r3
	mov r0, #0
	strb r0, [r1]
	b _0803FE6A
	.align 2, 0
_0803FE1C: .4byte 0x03000040
_0803FE20: .4byte 0x000003E5
_0803FE24:
	mov r0, #0xF0
	lsl r0, r0, #0x10
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803FDEC
	ldr r0, _0803FE5C @ =0x0201CFB0
	ldr r2, _0803FE60 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803FE64 @ =0x00000828
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
	bne _0803FE68
	mov r0, #3
	bl PlaySE
	b _0803FDEC
	.align 2, 0
_0803FE5C: .4byte 0x0201CFB0
_0803FE60: .4byte 0x00000824
_0803FE64: .4byte 0x00000828
_0803FE68:
	mov r0, #1
_0803FE6A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectTwoProngedAttackChainB

