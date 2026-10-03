	thumb_func_start EffectDustTornadoResolve
EffectDustTornadoResolve: @ 0x080365F0
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _08036600
	b _080367DC
_08036600:
	ldr r0, _0803661C @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #0x7E
	beq _080366E4
	cmp r0, #0x7E
	bgt _08036620
	cmp r0, #0x7D
	bne _08036618
	b _0803670C
_08036618:
	b _080367DC
	.align 2, 0
_0803661C: .4byte 0x02017A40
_08036620:
	cmp r0, #0x7F
	beq _0803666C
	cmp r0, #0x80
	beq _0803662A
	b _080367DC
_0803662A:
	mov r2, #7
	ldrb r0, [r4, #0xA]
	and r2, r0
	cmp r2, #1
	beq _08036636
	b _080367DC
_08036636:
	ldrb r5, [r4, #0xC]
	ldrh r4, [r4, #0xC]
	lsr r3, r4, #8
	and r2, r5
	mov r0, #0x94
	mul r0, r3
	ldr r1, _08036664 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _08036668 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _08036656
	b _080367DC
_08036656:
	add r0, r5, #0
	add r1, r3, #0
	mov r2, #1
	bl DestroyFieldCard
	mov r0, #0x7F
	b _080367DE
_08036664: .4byte 0x00000D64
_08036668: .4byte 0x0201930C
_0803666C:
	ldr r2, _080366D0 @ =0x020192E4
	ldrb r1, [r4, #2]
	lsl r3, r1, #0x1F
	lsr r1, r3, #0x1F
	ldr r0, _080366D4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	bne _08036682
	b _080367DC
_08036682:
	lsr r0, r3, #0x1F
	bl FindTrapInHand
	add r5, r0, #0
	mov r0, #1
	neg r0, r0
	cmp r5, r0
	bne _080366A2
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl FindNonFieldMagicInHand
	cmp r0, r5
	bne _080366A2
	b _080367DC
_080366A2:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeSpellTrapZone
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	bne _080366B6
	b _080367DC
_080366B6:
	ldr r0, _080366D8 @ =0x00000206
	ldr r1, _080366DC @ =0x00000712
	ldr r3, _080366E0 @ =0x08082EC0
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7E
	b _080367DE
_080366D0: .4byte 0x020192E4
_080366D4: .4byte 0x00000D64
_080366D8: .4byte 0x00000206
_080366DC: .4byte 0x00000712
_080366E0: .4byte gStrDustTornadoSetPrompt
_080366E4:
	ldr r0, _080366FC @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _080367DC
	ldr r0, _08036700 @ =0x00000206
	ldr r1, _08036704 @ =0x00000712
	ldr r3, _08036708 @ =0x08082EF4
	mov r2, #0xB
	bl TextBoxOpen
_080366F8:
	mov r0, #0x7D
	b _080367DE
_080366FC: .4byte 0x0201AE60
_08036700: .4byte 0x00000206
_08036704: .4byte 0x00000712
_08036708: .4byte gStrDustTornadoSelectCards
_0803670C:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _080367C6
	ldrb r3, [r4, #2]
	lsl r2, r3, #0x1F
	lsr r2, r2, #0x1F
	ldr r0, _08036760 @ =0x0201CFB0
	ldr r1, _08036764 @ =0x0000082C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #2
	ldr r1, _08036768 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803676C @ =0x02019968
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	ldr r0, _08036770 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r2, _08036774 @ =0x08621DE0
	add r0, r0, r2
	ldr r2, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bls _080367C0
	cmp r0, #0x16
	bgt _08036778
	cmp r0, #0x15
	blt _08036778
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r2, r0
	lsr r0, r2, #0x11
	b _0803677A
_08036760: .4byte 0x0201CFB0
_08036764: .4byte 0x0000082C
_08036768: .4byte 0x00000D64
_0803676C: .4byte 0x02019968
_08036770: .4byte 0x000007FF
_08036774: .4byte gCardStats
_08036778:
	mov r0, #0
_0803677A:
	cmp r0, #2
	beq _080367C0
	mov r0, #1
	and r0, r3
	mov r4, #0xC5
	cmp r0, #0
	beq _0803678A
	ldr r4, _080367B4 @ =0x000080C5
_0803678A:
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeSpellTrapZone
	ldr r1, _080367B8 @ =0x0201CFB0
	ldr r2, _080367BC @ =0x0000082C
	add r1, r1, r2
	ldr r2, [r1]
	mov r3, #0xF
	mov r1, #0xF
	and r2, r1
	lsl r2, r2, #4
	and r0, r3
	orr r2, r0
	add r0, r4, #0
	add r1, r5, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x64
	b _080367DE
_080367B4: .4byte 0x000080C5
_080367B8: .4byte 0x0201CFB0
_080367BC: .4byte 0x0000082C
_080367C0:
	mov r0, #3
	bl PlaySE
_080367C6:
	ldr r1, _080367D8 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080366F8
	mov r0, #0x7F
	b _080367DE
	.align 2, 0
_080367D8: .4byte 0x03000040
_080367DC:
	mov r0, #0
_080367DE:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectDustTornadoResolve

