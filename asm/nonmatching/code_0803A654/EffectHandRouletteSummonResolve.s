	thumb_func_start EffectHandRouletteSummonResolve
EffectHandRouletteSummonResolve: @ 0x0803AC34
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	add r3, r1, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _0803AC4A
	b _0803B060
_0803AC4A:
	ldr r1, _0803AC68 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x78
	add r2, r1, #0
	cmp r0, #8
	bls _0803AC5E
	b _0803B060
_0803AC5E:
	lsl r0, r0, #2
	ldr r1, _0803AC6C @ =0x0803AC70
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0803AC68: .4byte 0x02017A40
_0803AC6C: .4byte 0x0803AC70
_0803AC70:
	.4byte _0803AF90
	.4byte _0803AF60
	.4byte _0803AEC8
	.4byte _0803AE1C
	.4byte _0803AE00
	.4byte _0803AD78
	.4byte _0803AD5C
	.4byte _0803ACC0
	.4byte _0803AC94
_0803AC94:
	add r0, r5, #0
	add r1, r3, #0
	mov r2, #0
	bl EffectHandMonsterAndTwoCardsPrepare
	cmp r0, #0
	bne _0803ACA4
	b _0803B060
_0803ACA4:
	ldr r0, _0803ACB4 @ =0x00000206
	ldr r1, _0803ACB8 @ =0x00000613
	ldr r3, _0803ACBC @ =0x08083690
	mov r2, #0xB
	bl TextBoxOpen
_0803ACB0:
	mov r0, #0x7F
	b _0803B062
_0803ACB4: .4byte 0x00000206
_0803ACB8: .4byte 0x00000613
_0803ACBC: .4byte gStrSelectHandMonster
_0803ACC0:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803ACB0
	ldrb r3, [r5, #2]
	lsl r2, r3, #0x1F
	mov r4, #1
	lsr r2, r2, #0x1F
	ldr r0, _0803AD38 @ =0x0201CFB0
	ldr r1, _0803AD3C @ =0x0000082C
	add r6, r0, r1
	ldr r0, [r6]
	lsl r0, r0, #2
	ldr r1, _0803AD40 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803AD44 @ =0x02019968
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	ldr r0, _0803AD48 @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _0803AD4C @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _0803AD54
	add r0, r2, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _0803AD54
	ldrb r1, [r5, #2]
	and r4, r1
	mov r3, #8
	cmp r4, #0
	beq _0803AD1A
	ldr r3, _0803AD50 @ =0x00008008
_0803AD1A:
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrb r0, [r6]
	lsl r2, r0, #8
	mov r0, #0xB
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, [r6]
	strh r0, [r5, #0xC]
	mov r0, #0x7E
	b _0803B062
	.align 2, 0
_0803AD38: .4byte 0x0201CFB0
_0803AD3C: .4byte 0x0000082C
_0803AD40: .4byte 0x00000D64
_0803AD44: .4byte 0x02019968
_0803AD48: .4byte 0x000007FF
_0803AD4C: .4byte gCardStats
_0803AD50: .4byte 0x00008008
_0803AD54:
	mov r0, #3
	bl PlaySE
	b _0803ACB0
_0803AD5C:
	ldr r0, _0803AD6C @ =0x00000206
	ldr r1, _0803AD70 @ =0x00000613
	ldr r3, _0803AD74 @ =0x080836BC
	mov r2, #0xB
	bl TextBoxOpen
_0803AD68:
	mov r0, #0x7D
	b _0803B062
_0803AD6C: .4byte 0x00000206
_0803AD70: .4byte 0x00000613
_0803AD74: .4byte gStrSelectHandMagicTrap
_0803AD78:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803AD68
	ldrb r4, [r5, #2]
	lsl r7, r4, #0x1F
	mov r3, #1
	lsr r2, r7, #0x1F
	ldr r0, _0803ADE0 @ =0x0201CFB0
	ldr r1, _0803ADE4 @ =0x0000082C
	add r1, r1, r0
	mov r8, r1
	ldr r6, [r1]
	lsl r0, r6, #2
	ldr r1, _0803ADE8 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803ADEC @ =0x02019968
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r2, _0803ADF0 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _0803ADF8
	and r3, r4
	mov r4, #8
	cmp r3, #0
	beq _0803ADC2
	ldr r4, _0803ADF4 @ =0x00008008
_0803ADC2:
	lsr r1, r7, #0x1F
	lsl r0, r6, #0x18
	mov r2, #0xB0
	lsl r2, r2, #0xC
	orr r2, r0
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r3, r8
	ldr r0, [r3]
	strh r0, [r5, #0xE]
	mov r0, #0x7C
	b _0803B062
_0803ADE0: .4byte 0x0201CFB0
_0803ADE4: .4byte 0x0000082C
_0803ADE8: .4byte 0x00000D64
_0803ADEC: .4byte 0x02019968
_0803ADF0: .4byte gCardStats
_0803ADF4: .4byte 0x00008008
_0803ADF8:
	mov r0, #3
	bl PlaySE
	b _0803AD68
_0803AE00:
	ldr r0, _0803AE10 @ =0x00000206
	ldr r1, _0803AE14 @ =0x00000613
	ldr r3, _0803AE18 @ =0x080836F0
	mov r2, #0xB
	bl TextBoxOpen
_0803AE0C:
	mov r0, #0x7B
	b _0803B062
_0803AE10: .4byte 0x00000206
_0803AE14: .4byte 0x00000613
_0803AE18: .4byte gStrSelectAnotherHandMagicTrap
_0803AE1C:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803AE0C
	ldrb r6, [r5, #2]
	lsl r7, r6, #0x1F
	mov r3, #1
	lsr r2, r7, #0x1F
	ldr r0, _0803AE9C @ =0x0201CFB0
	ldr r1, _0803AEA0 @ =0x0000082C
	add r1, r1, r0
	mov r8, r1
	ldr r4, [r1]
	lsl r0, r4, #2
	ldr r1, _0803AEA4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803AEA8 @ =0x02019968
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r2, _0803AEAC @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _0803AEC0
	ldrh r0, [r5, #0xE]
	cmp r4, r0
	beq _0803AEC0
	and r3, r6
	mov r6, #8
	cmp r3, #0
	beq _0803AE6C
	ldr r6, _0803AEB0 @ =0x00008008
_0803AE6C:
	lsr r1, r7, #0x1F
	lsl r0, r4, #0x18
	mov r2, #0xB0
	lsl r2, r2, #0xC
	orr r2, r0
	lsr r2, r2, #0x10
	add r0, r6, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r1, r8
	ldr r0, [r1]
	mov r3, #0
	strh r0, [r5, #0x10]
	ldr r0, _0803AEB4 @ =0x02017A40
	ldr r1, _0803AEB8 @ =0x000003E1
	add r2, r0, r1
	mov r1, #0x10
	strb r1, [r2]
	ldr r2, _0803AEBC @ =0x000003E2
	add r0, r0, r2
	strb r3, [r0]
	mov r0, #0x7A
	b _0803B062
_0803AE9C: .4byte 0x0201CFB0
_0803AEA0: .4byte 0x0000082C
_0803AEA4: .4byte 0x00000D64
_0803AEA8: .4byte 0x02019968
_0803AEAC: .4byte gCardStats
_0803AEB0: .4byte 0x00008008
_0803AEB4: .4byte 0x02017A40
_0803AEB8: .4byte 0x000003E1
_0803AEBC: .4byte 0x000003E2
_0803AEC0:
	mov r0, #3
	bl PlaySE
	b _0803AE0C
_0803AEC8:
	add r7, r5, #0
	add r7, #0xC
	ldr r6, _0803AF1C @ =0x02017E22
_0803AECE:
	bl Random
	mov r1, #3
	bl __modsi3
	add r4, r0, #0
	ldrb r3, [r6]
	cmp r4, r3
	beq _0803AECE
	ldr r0, _0803AF20 @ =0x02017A40
	ldr r1, _0803AF24 @ =0x000003E2
	add r2, r0, r1
	strb r4, [r2]
	ldr r3, _0803AF28 @ =0x000003E1
	add r1, r0, r3
	ldrb r0, [r1]
	cmp r0, #0
	beq _0803AF30
	sub r0, #1
	strb r0, [r1]
	mov r0, #1
	ldrb r5, [r5, #2]
	and r0, r5
	mov r1, #9
	cmp r0, #0
	beq _0803AF04
	ldr r1, _0803AF2C @ =0x00008009
_0803AF04:
	ldrb r2, [r2]
	lsl r0, r2, #1
	add r0, r7, r0
	ldrh r2, [r0]
	add r0, r1, #0
	mov r1, #0xB
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7A
	b _0803B062
	.align 2, 0
_0803AF1C: .4byte 0x02017E22
_0803AF20: .4byte 0x02017A40
_0803AF24: .4byte 0x000003E2
_0803AF28: .4byte 0x000003E1
_0803AF2C: .4byte 0x00008009
_0803AF30:
	ldrb r1, [r5, #2]
	mov r0, #1
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _0803AF3E
	ldr r3, _0803AF5C @ =0x00008008
_0803AF3E:
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrb r2, [r2]
	lsl r0, r2, #1
	add r0, r7, r0
	ldrb r0, [r0]
	lsl r2, r0, #8
	mov r0, #0xB
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x79
	b _0803B062
_0803AF5C: .4byte 0x00008008
_0803AF60:
	mov r4, #0
	ldr r6, _0803AF8C @ =0x02017E22
_0803AF64:
	ldrb r0, [r6]
	cmp r4, r0
	beq _0803AF82
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r4, #1
	add r1, r5, #0
	add r1, #0xC
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	mov r3, #0
	bl DiscardHandCard
_0803AF82:
	add r4, #1
	cmp r4, #2
	ble _0803AF64
	mov r0, #0x78
	b _0803B062
_0803AF8C: .4byte 0x02017E22
_0803AF90:
	ldr r3, _0803B000 @ =0x000003E2
	add r6, r2, r3
	ldrb r0, [r6]
	cmp r0, #0
	bne _0803B014
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	mov r4, #1
	lsr r2, r0, #0x1F
	ldr r1, _0803B004 @ =0x00000D64
	mul r1, r2
	ldr r2, _0803B008 @ =0x02019968
	add r1, r1, r2
	ldrh r3, [r5, #0xC]
	lsl r2, r3, #2
	add r6, r1, r2
	lsr r0, r0, #0x1F
	ldr r1, [r6]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl ShowRevealedCard
	add r0, r4, #0
	ldrb r1, [r5, #2]
	and r0, r1
	mov r3, #0xC2
	cmp r0, #0
	beq _0803AFCA
	ldr r3, _0803B00C @ =0x000080C2
_0803AFCA:
	ldrh r1, [r6]
	ldrh r2, [r6, #2]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r2, [r5, #2]
	and r4, r2
	mov r0, #0xCA
	cmp r4, #0
	beq _0803AFE2
	ldr r0, _0803B010 @ =0x000080CA
_0803AFE2:
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	add r1, r6, #0
	mov r2, #1
	mov r3, #0
	bl QueueSpecialSummonChoosePosition
	b _0803B054
	.align 2, 0
_0803B000: .4byte 0x000003E2
_0803B004: .4byte 0x00000D64
_0803B008: .4byte 0x02019968
_0803B00C: .4byte 0x000080C2
_0803B010: .4byte 0x000080CA
_0803B014:
	ldrb r0, [r5, #2]
	lsl r3, r0, #0x1F
	lsr r0, r3, #0x1F
	add r3, r0, #0
	ldrb r2, [r6]
	lsl r1, r2, #1
	add r4, r5, #0
	add r4, #0xC
	add r1, r4, r1
	ldrh r1, [r1]
	lsl r1, r1, #2
	ldr r2, _0803B058 @ =0x00000D64
	mul r2, r3
	add r1, r1, r2
	ldr r2, _0803B05C @ =0x02019968
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl ShowDestroyedCard
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldrb r6, [r6]
	lsl r1, r6, #1
	add r4, r4, r1
	ldrh r1, [r4]
	mov r2, #0
	mov r3, #1
	bl DiscardHandCard
_0803B054:
	mov r0, #0x77
	b _0803B062
_0803B058: .4byte 0x00000D64
_0803B05C: .4byte 0x02019968
_0803B060:
	mov r0, #0
_0803B062:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectHandRouletteSummonResolve

