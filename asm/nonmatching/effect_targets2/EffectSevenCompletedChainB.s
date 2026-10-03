	thumb_func_start EffectSevenCompletedChainB
EffectSevenCompletedChainB: @ 0x0803F9F4
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	ldr r0, _0803FA10 @ =0x02017A40
	ldr r1, _0803FA14 @ =0x000003E5
	add r6, r0, r1
	ldrb r0, [r6]
	cmp r0, #1
	beq _0803FA4C
	cmp r0, #1
	bgt _0803FA18
	cmp r0, #0
	beq _0803FA22
	b _0803FAF8
	.align 2, 0
_0803FA10: .4byte 0x02017A40
_0803FA14: .4byte 0x000003E5
_0803FA18:
	cmp r0, #2
	beq _0803FABC
	cmp r0, #3
	beq _0803FAE0
	b _0803FAF8
_0803FA22:
	ldr r0, _0803FA40 @ =0x00000206
	ldr r1, _0803FA44 @ =0x00000712
	ldr r3, _0803FA48 @ =0x08083E14
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #8
	neg r0, r0
	ldrb r3, [r5, #0xA]
	and r0, r3
	strb r0, [r5, #0xA]
_0803FA38:
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	b _0803FAF8
_0803FA40: .4byte 0x00000206
_0803FA44: .4byte 0x00000712
_0803FA48: .4byte gStrDesignateMonsterToEquip
_0803FA4C:
	ldr r1, _0803FA60 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803FA64
	mov r0, #0
	strb r0, [r6]
	b _0803FAFA
	.align 2, 0
_0803FA60: .4byte 0x03000040
_0803FA64:
	ldr r0, _0803FAA4 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803FAF8
	ldr r0, _0803FAA8 @ =0x0201CFB0
	ldr r1, _0803FAAC @ =0x00000824
	add r2, r0, r1
	ldr r3, _0803FAB0 @ =0x00000828
	add r1, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r1, [r1]
	ldr r0, [r0]
	add r4, r1, r0
	ldr r7, [r2]
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	ldrb r2, [r2]
	orr r1, r2
	add r0, r5, #0
	bl EffectEquipTargetCheck
	cmp r0, #0
	beq _0803FAB4
	add r0, r5, #0
	add r1, r7, #0
	add r2, r4, #0
	bl TryAddEffectTarget
	b _0803FA38
	.align 2, 0
_0803FAA4: .4byte 0x00E000E0
_0803FAA8: .4byte 0x0201CFB0
_0803FAAC: .4byte 0x00000824
_0803FAB0: .4byte 0x00000828
_0803FAB4:
	mov r0, #3
	bl PlaySE
	b _0803FAF8
_0803FABC:
	ldr r0, _0803FAD4 @ =0x00000206
	ldr r1, _0803FAD8 @ =0x00000613
	ldr r3, _0803FADC @ =0x080843E4
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	b _0803FA38
_0803FAD4: .4byte 0x00000206
_0803FAD8: .4byte 0x00000613
_0803FADC: .4byte gStrAskSevenCompletedStat
_0803FAE0:
	ldr r0, _0803FAF4 @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	add r1, #1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	add r0, r5, #0
	bl AddEffectTarget
	mov r0, #1
	b _0803FAFA
_0803FAF4: .4byte 0x0201AE60
_0803FAF8:
	mov r0, #0
_0803FAFA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectSevenCompletedChainB

