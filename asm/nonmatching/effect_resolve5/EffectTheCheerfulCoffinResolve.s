	thumb_func_start EffectTheCheerfulCoffinResolve
EffectTheCheerfulCoffinResolve: @ 0x08035198
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	add r3, r1, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	beq _080351AE
	b _08035308
_080351AE:
	ldr r2, _080351C8 @ =0x02017A40
	mov r8, r2
	mov r2, #0xF8
	lsl r2, r2, #2
	add r2, r8
	ldrb r0, [r2]
	cmp r0, #0x7E
	beq _0803522C
	cmp r0, #0x7E
	bgt _080351CC
	cmp r0, #0x7D
	beq _08035254
	b _08035308
_080351C8: .4byte 0x02017A40
_080351CC:
	cmp r0, #0x7F
	beq _080351E4
	cmp r0, #0x80
	beq _080351D6
	b _08035308
_080351D6:
	ldr r0, _0803521C @ =0x000003E1
	add r0, r8
	mov r1, #3
	strb r1, [r0]
	ldrb r0, [r2]
	sub r0, #1
	strb r0, [r2]
_080351E4:
	ldr r0, _0803521C @ =0x000003E1
	add r0, r8
	ldrb r0, [r0]
	cmp r0, #0
	bne _080351F0
	b _08035308
_080351F0:
	add r0, r5, #0
	add r1, r3, #0
	mov r2, #0
	bl EffectTheCheerfulCoffinPrepare
	cmp r0, #0
	bne _08035200
	b _08035308
_08035200:
	ldr r0, _08035220 @ =0x00000206
	ldr r1, _08035224 @ =0x00000712
	ldr r3, _08035228 @ =0x08082D64
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7E
	b _0803530A
	.align 2, 0
_0803521C: .4byte 0x000003E1
_08035220: .4byte 0x00000206
_08035224: .4byte 0x00000712
_08035228: .4byte gStrCheerfulCoffinDiscardPrompt
_0803522C:
	ldr r0, _08035244 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08035308
	ldr r0, _08035248 @ =0x00000206
	ldr r1, _0803524C @ =0x00000712
	ldr r3, _08035250 @ =0x08082DB4
	mov r2, #0xB
	bl TextBoxOpen
_08035240:
	mov r0, #0x7D
	b _0803530A
_08035244: .4byte 0x0201AE60
_08035248: .4byte 0x00000206
_0803524C: .4byte 0x00000712
_08035250: .4byte gStrCheerfulCoffinSelectMonster
_08035254:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08035240
	ldrb r4, [r5, #2]
	lsl r2, r4, #0x1F
	mov r4, #1
	lsr r2, r2, #0x1F
	ldr r6, _080352DC @ =0x0201CFB0
	ldr r0, _080352E0 @ =0x0000082C
	add r7, r6, r0
	ldr r0, [r7]
	lsl r0, r0, #2
	ldr r1, _080352E4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080352E8 @ =0x02019968
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r1, _080352EC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08035300
	mov r0, #1
	bl PlaySE
	ldrb r2, [r5, #2]
	and r4, r2
	mov r3, #8
	cmp r4, #0
	beq _080352A4
	ldr r3, _080352F0 @ =0x00008008
_080352A4:
	ldr r4, _080352F4 @ =0x00000824
	add r0, r6, r4
	ldrh r1, [r0]
	ldr r2, _080352F8 @ =0x00000828
	add r0, r6, r2
	ldrb r4, [r7]
	lsl r2, r4, #8
	ldrb r0, [r0]
	orr r2, r0
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, [r7]
	mov r2, #1
	mov r3, #1
	bl DiscardHandCard
	ldr r1, _080352FC @ =0x000003E1
	add r1, r8
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	mov r0, #0x7F
	b _0803530A
_080352DC: .4byte 0x0201CFB0
_080352E0: .4byte 0x0000082C
_080352E4: .4byte 0x00000D64
_080352E8: .4byte 0x02019968
_080352EC: .4byte gCardStats
_080352F0: .4byte 0x00008008
_080352F4: .4byte 0x00000824
_080352F8: .4byte 0x00000828
_080352FC: .4byte 0x000003E1
_08035300:
	mov r0, #3
	bl PlaySE
	b _08035240
_08035308:
	mov r0, #0
_0803530A:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectTheCheerfulCoffinResolve

