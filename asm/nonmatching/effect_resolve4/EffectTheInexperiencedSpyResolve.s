	thumb_func_start EffectTheInexperiencedSpyResolve
EffectTheInexperiencedSpyResolve: @ 0x08034644
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _080346D8
	ldr r2, _080346DC @ =0x020192E4
	mov r8, r2
	ldrb r3, [r5, #2]
	lsl r0, r3, #0x1F
	mov r6, #1
	lsr r0, r0, #0x1F
	ldr r7, _080346E0 @ =0x00000D64
	mov r9, r7
	mov r1, r9
	mul r1, r0
	add r0, r1, #0
	add r0, r8
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _080346D8
	mov r0, #0x80
	lsl r0, r0, #9
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _080346F8
	add r0, r6, #0
	ldrb r2, [r5, #2]
	and r0, r2
	mov r3, #8
	cmp r0, #0
	beq _08034690
	ldr r3, _080346E4 @ =0x00008008
_08034690:
	ldr r4, _080346E8 @ =0x0201CFB0
	ldr r7, _080346EC @ =0x00000824
	add r0, r4, r7
	ldrh r1, [r0]
	ldr r2, _080346F0 @ =0x00000828
	add r0, r4, r2
	add r7, #8
	add r4, r4, r7
	ldrb r7, [r4]
	lsl r2, r7, #8
	ldrb r0, [r0]
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r5, [r5, #2]
	lsl r2, r5, #0x1F
	lsr r0, r2, #0x1F
	add r2, r0, #0
	sub r2, r6, r2
	and r2, r6
	ldr r1, [r4]
	lsl r1, r1, #2
	mov r3, r9
	mul r3, r2
	add r2, r3, #0
	add r1, r1, r2
	ldr r2, _080346F4 @ =0x00000684
	add r2, r8
	add r1, r1, r2
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl ShowCardDetail
_080346D8:
	mov r0, #0
	b _080346FA
_080346DC: .4byte 0x020192E4
_080346E0: .4byte 0x00000D64
_080346E4: .4byte 0x00008008
_080346E8: .4byte 0x0201CFB0
_080346EC: .4byte 0x00000824
_080346F0: .4byte 0x00000828
_080346F4: .4byte 0x00000684
_080346F8:
	mov r0, #0x80
_080346FA:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectTheInexperiencedSpyResolve
	.align 2, 0

