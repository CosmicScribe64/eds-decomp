	thumb_func_start EffectTributeOpponentMonsterChainB
EffectTributeOpponentMonsterChainB: @ 0x08040C10
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _08040C40 @ =0x02017A40
	ldr r1, _08040C44 @ =0x000003E5
	add r5, r0, r1
	ldrb r0, [r5]
	cmp r0, #0
	bne _08040C54
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldr r0, _08040C48 @ =0x00000206
	ldr r1, _08040C4C @ =0x00000712
	ldr r3, _08040C50 @ =0x080848E4
	mov r2, #0xB
	bl TextBoxOpen
	ldrb r0, [r5]
	add r0, #1
	strb r0, [r5]
	mov r0, #0
	b _08040C9A
_08040C40: .4byte 0x02017A40
_08040C44: .4byte 0x000003E5
_08040C48: .4byte 0x00000206
_08040C4C: .4byte 0x00000712
_08040C50: .4byte gStrDesignateOpponentMonsterToTribute
_08040C54:
	ldr r1, _08040C68 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08040C6C
	mov r0, #0
	strb r0, [r5]
	b _08040C9A
	.align 2, 0
_08040C68: .4byte 0x03000040
_08040C6C:
	mov r0, #0xF0
	lsl r0, r0, #0x10
	bl DuelCursor_PickTarget
	cmp r0, #0
	bne _08040C7C
	mov r0, #0
	b _08040C9A
_08040C7C:
	ldr r0, _08040CA0 @ =0x0201CFB0
	ldr r3, _08040CA4 @ =0x00000824
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
	mov r0, #1
_08040C9A:
	pop {r4, r5}
	pop {r1}
	bx r1
_08040CA0: .4byte 0x0201CFB0
_08040CA4: .4byte 0x00000824
	thumb_func_end EffectTributeOpponentMonsterChainB

