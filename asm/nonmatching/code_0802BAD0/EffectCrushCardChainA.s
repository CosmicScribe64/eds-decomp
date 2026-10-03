	thumb_func_start EffectCrushCardChainA
EffectCrushCardChainA: @ 0x0802C8B4
	push {r4, r5, lr}
	sub sp, #0xC
	add r5, r0, #0
	ldr r0, _0802C8D0 @ =0x02017A40
	mov r1, #0xF9
	lsl r1, r1, #2
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	beq _0802C8D4
	cmp r0, #1
	beq _0802C8F4
	b _0802C962
	.align 2, 0
_0802C8D0: .4byte 0x02017A40
_0802C8D4:
	ldr r0, _0802C8E8 @ =0x00000206
	ldr r1, _0802C8EC @ =0x00000712
	ldr r3, _0802C8F0 @ =0x0808277C
	mov r2, #0xB
	bl TextBoxOpen
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0802C962
_0802C8E8: .4byte 0x00000206
_0802C8EC: .4byte 0x00000712
_0802C8F0: .4byte gStrCrushCardTributePrompt
_0802C8F4:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0802C948
	ldrb r2, [r5, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802C930 @ =0x0201CFB0
	ldr r2, _0802C934 @ =0x0000082C
	add r4, r1, r2
	ldr r1, [r4]
	mov r2, sp
	bl GetZoneCardStats
	ldr r1, [sp, #4]
	mov r0, #0xFA
	lsl r0, r0, #2
	cmp r1, r0
	bgt _0802C928
	mov r0, sp
	ldrb r1, [r0, #2]
	mov r0, #0xE0
	and r0, r1
	cmp r0, #0x40
	beq _0802C938
_0802C928:
	mov r0, #3
	bl PlaySE
	b _0802C948
_0802C930: .4byte 0x0201CFB0
_0802C934: .4byte 0x0000082C
_0802C938:
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, [r4]
	bl TributeMonster
	mov r0, #1
	b _0802C964
_0802C948:
	ldr r1, _0802C96C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802C962
	ldr r0, _0802C970 @ =0x02017A40
	mov r1, #0xF9
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r1, [r0]
	sub r1, #1
	strb r1, [r0]
_0802C962:
	mov r0, #0
_0802C964:
	add sp, #0xC
	pop {r4, r5}
	pop {r1}
	bx r1
_0802C96C: .4byte 0x03000040
_0802C970: .4byte 0x02017A40
	thumb_func_end EffectCrushCardChainA

