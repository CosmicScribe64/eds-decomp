	thumb_func_start EffectDragonSeekerChainB
EffectDragonSeekerChainB: @ 0x0803EA9C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #0x80
	add r7, r0, #0
	ldr r0, _0803EAF0 @ =0x02017A40
	ldr r1, _0803EAF4 @ =0x000003E5
	add r2, r0, r1
	ldrb r0, [r2]
	cmp r0, #0
	bne _0803EB36
	mov r0, #8
	neg r0, r0
	ldrb r1, [r7, #0xA]
	and r0, r1
	strb r0, [r7, #0xA]
	mov r6, #0
	mov r8, r2
_0803EAC0:
	mov r4, #0
	lsl r0, r6, #0x18
	lsr r5, r0, #0x18
_0803EAC6:
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	orr r1, r5
	add r0, r7, #0
	bl EffectDragonSeekerCheck
	cmp r0, #0
	beq _0803EB28
	mov r0, #1
	ldrb r2, [r7, #2]
	and r0, r2
	cmp r0, #0
	beq _0803EAF8
	add r0, r7, #0
	add r1, r6, #0
	add r2, r4, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
_0803EAEC:
	mov r0, #1
	b _0803EB96
_0803EAF0: .4byte 0x02017A40
_0803EAF4: .4byte 0x000003E5
_0803EAF8:
	mov r0, sp
	ldr r1, _0803EB18 @ =0x08083F94
	ldr r2, _0803EB1C @ =0x08083FC8
	bl FormatStr
	ldr r0, _0803EB20 @ =0x00000206
	ldr r1, _0803EB24 @ =0x00000712
	mov r2, #0xB
	mov r3, sp
	bl TextBoxOpen
	mov r1, r8
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _0803EB94
_0803EB18: .4byte gStrDesignateTypeMonsterToDestroyFmt
_0803EB1C: .4byte gStrDragonType
_0803EB20: .4byte 0x00000206
_0803EB24: .4byte 0x00000712
_0803EB28:
	add r4, #1
	cmp r4, #4
	ble _0803EAC6
	add r6, #1
	cmp r6, #1
	ble _0803EAC0
	b _0803EAEC
_0803EB36:
	ldr r1, _0803EB48 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803EB4C
	mov r0, #0
	strb r0, [r2]
	b _0803EB96
_0803EB48: .4byte 0x03000040
_0803EB4C:
	ldr r0, _0803EBA4 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803EB94
	ldr r0, _0803EBA8 @ =0x0201CFB0
	ldr r2, _0803EBAC @ =0x00000824
	add r3, r0, r2
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r2, [r1]
	ldr r1, [r0]
	add r4, r2, r1
	ldr r5, [r3]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x10
	ldrb r3, [r3]
	orr r1, r3
	add r0, r7, #0
	bl EffectDragonSeekerCheck
	cmp r0, #0
	beq _0803EB8E
	add r0, r7, #0
	add r1, r5, #0
	add r2, r4, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _0803EAEC
_0803EB8E:
	mov r0, #3
	bl PlaySE
_0803EB94:
	mov r0, #0
_0803EB96:
	add sp, #0x80
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803EBA4: .4byte 0x00E000E0
_0803EBA8: .4byte 0x0201CFB0
_0803EBAC: .4byte 0x00000824
	thumb_func_end EffectDragonSeekerChainB

