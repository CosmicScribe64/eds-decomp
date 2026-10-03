	thumb_func_start EffectGainLpChosenPlayerChainB
EffectGainLpChosenPlayerChainB: @ 0x0803E658
	push {r4, lr}
	add r2, r0, #0
	mov r0, #1
	ldrb r1, [r2, #2]
	and r0, r1
	cmp r0, #0
	beq _0803E672
	add r0, r2, #0
	mov r1, #0
	bl AddEffectTarget
	mov r0, #1
	b _0803E6D2
_0803E672:
	ldr r0, _0803E688 @ =0x02017A40
	ldr r1, _0803E68C @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	beq _0803E690
	cmp r0, #1
	beq _0803E6C0
	mov r0, #1
	b _0803E6D2
	.align 2, 0
_0803E688: .4byte 0x02017A40
_0803E68C: .4byte 0x000003E5
_0803E690:
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2, #0xA]
	and r0, r1
	strb r0, [r2, #0xA]
	ldr r0, _0803E6B4 @ =0x00000205
	ldr r1, _0803E6B8 @ =0x00000514
	ldr r3, _0803E6BC @ =0x08083E8C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	b _0803E6CA
	.align 2, 0
_0803E6B4: .4byte 0x00000205
_0803E6B8: .4byte 0x00000514
_0803E6BC: .4byte gStrAskWhoseLpToRecover
_0803E6C0:
	ldr r0, _0803E6D8 @ =0x0201AE60
	ldrh r1, [r0, #0x14]
	add r0, r2, #0
	bl AddEffectTarget
_0803E6CA:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	mov r0, #0
_0803E6D2:
	pop {r4}
	pop {r1}
	bx r1
_0803E6D8: .4byte 0x0201AE60
	thumb_func_end EffectGainLpChosenPlayerChainB

