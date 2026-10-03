	thumb_func_start EffectDarknessApproachesChainB
EffectDarknessApproachesChainB: @ 0x08040040
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r0, _08040070 @ =0x02017A40
	ldr r1, _08040074 @ =0x000003E5
	add r4, r0, r1
	ldrb r0, [r4]
	cmp r0, #0
	bne _08040084
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	ldr r0, _08040078 @ =0x00000206
	ldr r1, _0804007C @ =0x00000712
	ldr r3, _08040080 @ =0x08084620
	mov r2, #0xB
	bl TextBoxOpen
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _08040106
	.align 2, 0
_08040070: .4byte 0x02017A40
_08040074: .4byte 0x000003E5
_08040078: .4byte 0x00000206
_0804007C: .4byte 0x00000712
_08040080: .4byte gStrDesignateMonsterToSetFaceDown
_08040084:
	ldr r0, _080400E4 @ =0x00E000E0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08040106
	ldr r0, _080400E8 @ =0x0201CFB0
	ldr r2, _080400EC @ =0x00000824
	add r1, r0, r2
	ldr r4, [r1]
	add r2, #4
	add r1, r0, r2
	add r2, #4
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r3, r1, r0
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _080400F0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080400F4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _080400F8 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r2, _080400FC @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _08040100
	add r0, r5, #0
	add r1, r4, #0
	add r2, r3, #0
	bl TryAddEffectTarget
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08040100
	mov r0, #1
	b _08040108
	.align 2, 0
_080400E4: .4byte 0x00E000E0
_080400E8: .4byte 0x0201CFB0
_080400EC: .4byte 0x00000824
_080400F0: .4byte 0x00000D64
_080400F4: .4byte 0x0201930C
_080400F8: .4byte gCardIdToNumber
_080400FC: .4byte 0xFFFFF880
_08040100:
	mov r0, #3
	bl PlaySE
_08040106:
	mov r0, #0
_08040108:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectDarknessApproachesChainB
	.align 2, 0

