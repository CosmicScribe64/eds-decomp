	thumb_func_start EffectTargetableMonsterChainB
EffectTargetableMonsterChainB: @ 0x0803EBB0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r7, r0, #0
	mov r2, #1
	ldrb r0, [r7, #2]
	and r2, r0
	cmp r2, #0
	beq _0803EC74
	mov r0, #8
	neg r0, r0
	ldrb r2, [r7, #0xA]
	and r0, r2
	strb r0, [r7, #0xA]
	add r0, r7, #0
	mov r2, #0
	bl CanActivateEffect
	cmp r0, #0
	beq _0803EC66
	mov r5, #0
_0803EBDC:
	add r0, r5, #0
	bl CountMonsters
	cmp r0, #0
	ble _0803EC1C
	cmp r5, #0
	beq _0803EBFA
	add r0, r5, #0
	mov r1, #1
	neg r1, r1
	mov r2, #1
	mov r3, #1
	bl AiFindWeakestMonster
	b _0803EC08
_0803EBFA:
	mov r0, #0
	mov r1, #1
	neg r1, r1
	mov r2, #1
	mov r3, #1
	bl AiFindStrongestMonster
_0803EC08:
	add r2, r0, #0
	cmp r2, #0
	blt _0803EC1C
	add r0, r7, #0
	add r1, r5, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803EC66
_0803EC1C:
	add r5, #1
	cmp r5, #1
	ble _0803EBDC
	mov r5, #0
	mov r4, #1
	mov r9, r4
	ldr r0, _0803EC6C @ =0x00000D64
	mov r8, r0
_0803EC2C:
	mov r4, #0
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	mov r6, r8
	mul r6, r0
_0803EC38:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _0803EC70 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803EC5A
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803EC66
_0803EC5A:
	add r4, #1
	cmp r4, #4
	ble _0803EC38
	add r5, #1
	cmp r5, #1
	ble _0803EC2C
_0803EC66:
	mov r0, #1
	b _0803EDA8
	.align 2, 0
_0803EC6C: .4byte 0x00000D64
_0803EC70: .4byte 0x0201930C
_0803EC74:
	ldr r0, _0803ECCC @ =0x02017A40
	ldr r4, _0803ECD0 @ =0x000003E5
	add r3, r0, r4
	ldrb r0, [r3]
	cmp r0, #0
	bne _0803ED4C
	mov r0, #8
	neg r0, r0
	ldrb r2, [r7, #0xA]
	and r0, r2
	strb r0, [r7, #0xA]
	add r0, r7, #0
	mov r2, #0
	bl CanActivateEffect
	cmp r0, #0
	bne _0803ECAA
	ldr r0, _0803ECD4 @ =0x000007FF
	ldrh r4, [r7]
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _0803ECD8 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0803ECDC @ =0x000003FF
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803EC66
_0803ECAA:
	ldr r0, _0803ECD4 @ =0x000007FF
	ldrh r7, [r7]
	and r0, r7
	lsl r0, r0, #1
	ldr r2, _0803ECD8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	mov r0, #0x87
	lsl r0, r0, #2
	cmp r1, r0
	beq _0803ED00
	cmp r1, r0
	bgt _0803ECE0
	sub r0, #0x28
	cmp r1, r0
	beq _0803ECE6
	b _0803ED1C
_0803ECCC: .4byte 0x02017A40
_0803ECD0: .4byte 0x000003E5
_0803ECD4: .4byte 0x000007FF
_0803ECD8: .4byte gCardIdToNumber
_0803ECDC: .4byte 0x000003FF
_0803ECE0:
	ldr r0, _0803ECF0 @ =0x000003FF
	cmp r1, r0
	bne _0803ED1C
_0803ECE6:
	ldr r0, _0803ECF4 @ =0x00000206
	ldr r1, _0803ECF8 @ =0x00000712
	ldr r3, _0803ECFC @ =0x08083FD0
	b _0803ED06
	.align 2, 0
_0803ECF0: .4byte 0x000003FF
_0803ECF4: .4byte 0x00000206
_0803ECF8: .4byte 0x00000712
_0803ECFC: .4byte gStrDesignateMonsterToDestroy
_0803ED00:
	ldr r0, _0803ED10 @ =0x00000206
	ldr r1, _0803ED14 @ =0x00000712
	ldr r3, _0803ED18 @ =0x08084000
_0803ED06:
	mov r2, #0xB
	bl TextBoxOpen
	b _0803ED28
	.align 2, 0
_0803ED10: .4byte 0x00000206
_0803ED14: .4byte 0x00000712
_0803ED18: .4byte gStrDesignateMonsterToHaveReturned
_0803ED1C:
	ldr r0, _0803ED38 @ =0x00000206
	ldr r1, _0803ED3C @ =0x00000712
	ldr r3, _0803ED40 @ =0x08084044
	mov r2, #0xB
	bl TextBoxOpen
_0803ED28:
	ldr r0, _0803ED44 @ =0x02017A40
	ldr r4, _0803ED48 @ =0x000003E5
	add r0, r0, r4
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803EDA6
	.align 2, 0
_0803ED38: .4byte 0x00000206
_0803ED3C: .4byte 0x00000712
_0803ED40: .4byte gStrDesignateOneMonster
_0803ED44: .4byte 0x02017A40
_0803ED48: .4byte 0x000003E5
_0803ED4C:
	ldr r1, _0803ED5C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803ED60
	strb r2, [r3]
	b _0803EDA6
_0803ED5C: .4byte 0x03000040
_0803ED60:
	ldr r0, _0803EDB4 @ =0x00F000F0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803EDA6
	ldr r0, _0803EDB8 @ =0x0201CFB0
	ldr r2, _0803EDBC @ =0x00000824
	add r1, r0, r2
	ldr r5, [r1]
	ldr r4, _0803EDC0 @ =0x00000828
	add r1, r0, r4
	add r2, #8
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r4, r1, r0
	ldrh r0, [r7]
	add r1, r5, #0
	add r2, r4, #0
	bl CanCardTargetZone
	cmp r0, #0
	beq _0803EDA0
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0803EDA0
	b _0803EC66
_0803EDA0:
	mov r0, #3
	bl PlaySE
_0803EDA6:
	mov r0, #0
_0803EDA8:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0803EDB4: .4byte 0x00F000F0
_0803EDB8: .4byte 0x0201CFB0
_0803EDBC: .4byte 0x00000824
_0803EDC0: .4byte 0x00000828
	thumb_func_end EffectTargetableMonsterChainB

