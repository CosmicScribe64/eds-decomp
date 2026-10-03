	thumb_func_start EffectBlockAttackChainB
EffectBlockAttackChainB: @ 0x0803FF8C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r0, _0803FFBC @ =0x02017A40
	ldr r1, _0803FFC0 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	bne _0803FFD0
	ldr r0, _0803FFC4 @ =0x00000206
	ldr r1, _0803FFC8 @ =0x00000712
	ldr r3, _0803FFCC @ =0x080845C8
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08040036
	.align 2, 0
_0803FFBC: .4byte 0x02017A40
_0803FFC0: .4byte 0x000003E5
_0803FFC4: .4byte 0x00000206
_0803FFC8: .4byte 0x00000712
_0803FFCC: .4byte gStrDesignateMonsterForDefensePosition
_0803FFD0:
	ldr r1, _0803FFE4 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803FFE8
	mov r0, #0
	strb r0, [r4]
	b _08040038
	.align 2, 0
_0803FFE4: .4byte 0x03000040
_0803FFE8:
	mov r0, #0xE0
	lsl r0, r0, #0xF
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08040036
	ldr r0, _08040028 @ =0x0201CFB0
	ldr r2, _0804002C @ =0x00000824
	add r1, r0, r2
	ldr r6, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r4, r1, r0
	ldrh r0, [r5]
	add r1, r6, #0
	add r2, r4, #0
	bl CanCardTargetZone
	cmp r0, #0
	beq _08040030
	add r0, r5, #0
	add r1, r6, #0
	add r2, r4, #0
	bl TryAddEffectTarget
	mov r0, #1
	b _08040038
	.align 2, 0
_08040028: .4byte 0x0201CFB0
_0804002C: .4byte 0x00000824
_08040030:
	mov r0, #3
	bl PlaySE
_08040036:
	mov r0, #0
_08040038:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectBlockAttackChainB
	.align 2, 0

