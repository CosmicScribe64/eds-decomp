	thumb_func_start EffectStatModifierTargetChainB
EffectStatModifierTargetChainB: @ 0x0803FE70
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _0803FEAC @ =0x02017A40
	ldr r1, _0803FEB0 @ =0x000003E5
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne _0803FF3C
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldr r0, _0803FEB4 @ =0x000007FF
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r3, _0803FEB8 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0803FEBC @ =0x00000433
	cmp r1, r0
	beq _0803FEDE
	cmp r1, r0
	bgt _0803FEC0
	sub r0, #0x3C
	cmp r1, r0
	beq _0803FEDE
	add r0, #1
	b _0803FECC
_0803FEAC: .4byte 0x02017A40
_0803FEB0: .4byte 0x000003E5
_0803FEB4: .4byte 0x000007FF
_0803FEB8: .4byte gCardIdToNumber
_0803FEBC: .4byte 0x00000433
_0803FEC0:
	ldr r0, _0803FED4 @ =0x0000043A
	cmp r1, r0
	beq _0803FF0C
	cmp r1, r0
	bgt _0803FED8
	sub r0, #6
_0803FECC:
	cmp r1, r0
	beq _0803FEF8
	b _0803FF68
	.align 2, 0
_0803FED4: .4byte 0x0000043A
_0803FED8:
	ldr r0, _0803FEE8 @ =0x000005FE
	cmp r1, r0
	bne _0803FF68
_0803FEDE:
	ldr r0, _0803FEEC @ =0x00000206
	ldr r1, _0803FEF0 @ =0x00000712
	ldr r3, _0803FEF4 @ =0x08084520
	b _0803FF12
	.align 2, 0
_0803FEE8: .4byte 0x000005FE
_0803FEEC: .4byte 0x00000206
_0803FEF0: .4byte 0x00000712
_0803FEF4: .4byte gStrDesignateMonsterToIncreaseAtk
_0803FEF8:
	ldr r0, _0803FF00 @ =0x00000206
	ldr r1, _0803FF04 @ =0x00000712
	ldr r3, _0803FF08 @ =0x08084558
	b _0803FF12
_0803FF00: .4byte 0x00000206
_0803FF04: .4byte 0x00000712
_0803FF08: .4byte gStrDesignateMonsterToIncreaseDef
_0803FF0C:
	ldr r0, _0803FF28 @ =0x00000206
	ldr r1, _0803FF2C @ =0x00000712
	ldr r3, _0803FF30 @ =0x08084590
_0803FF12:
	mov r2, #0xB
	bl TextBoxOpen
	ldr r0, _0803FF34 @ =0x02017A40
	ldr r1, _0803FF38 @ =0x000003E5
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803FF82
	.align 2, 0
_0803FF28: .4byte 0x00000206
_0803FF2C: .4byte 0x00000712
_0803FF30: .4byte gStrDesignateMonsterToDecreaseDef
_0803FF34: .4byte 0x02017A40
_0803FF38: .4byte 0x000003E5
_0803FF3C:
	ldr r0, _0803FF6C @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803FF82
	ldr r0, _0803FF70 @ =0x0201CFB0
	ldr r2, _0803FF74 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803FF78 @ =0x00000828
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
	beq _0803FF7C
_0803FF68:
	mov r0, #1
	b _0803FF84
_0803FF6C: .4byte 0x00E000E0
_0803FF70: .4byte 0x0201CFB0
_0803FF74: .4byte 0x00000824
_0803FF78: .4byte 0x00000828
_0803FF7C:
	mov r0, #3
	bl PlaySE
_0803FF82:
	mov r0, #0
_0803FF84:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectStatModifierTargetChainB
	.align 2, 0

