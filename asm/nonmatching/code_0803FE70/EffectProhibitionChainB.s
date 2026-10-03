	thumb_func_start EffectProhibitionChainB
EffectProhibitionChainB: @ 0x080408B4
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r0, _080408D0 @ =0x02017A40
	ldr r1, _080408D4 @ =0x000003E5
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #4
	bls _080408C6
	b _080409C8
_080408C6:
	lsl r0, r0, #2
	ldr r1, _080408D8 @ =0x080408DC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_080408D0: .4byte 0x02017A40
_080408D4: .4byte 0x000003E5
_080408D8: .4byte 0x080408DC
_080408DC:
	.4byte _080408F0
	.4byte _08040924
	.4byte _0804095C
	.4byte _08040978
	.4byte _08040998
_080408F0:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r5, #0xA]
	and r0, r2
	strb r0, [r5, #0xA]
	ldr r0, _08040910 @ =0x00000206
	ldr r1, _08040914 @ =0x00000712
	ldr r3, _08040918 @ =0x08084814
	mov r2, #0xB
	bl TextBoxOpen
	ldr r0, _0804091C @ =0x02017A40
	ldr r3, _08040920 @ =0x000003E5
	add r0, r0, r3
	b _08040986
	.align 2, 0
_08040910: .4byte 0x00000206
_08040914: .4byte 0x00000712
_08040918: .4byte gStrDesignateCardToProhibit
_0804091C: .4byte 0x02017A40
_08040920: .4byte 0x000003E5
_08040924:
	bl DuelScreen_FadeOutStep
	cmp r0, #0
	beq _0804098C
	bl ResetVideo
	ldr r1, _0804094C @ =0x02017A40
	ldr r2, _08040950 @ =0x000003E6
	add r0, r1, r2
	mov r2, #0
	strb r2, [r0]
	ldr r3, _08040954 @ =0x000003E7
	add r0, r1, r3
	strb r2, [r0]
	ldr r0, _08040958 @ =0x000003E5
	add r1, r1, r0
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _0804098C
_0804094C: .4byte 0x02017A40
_08040950: .4byte 0x000003E6
_08040954: .4byte 0x000003E7
_08040958: .4byte 0x000003E5
_0804095C:
	bl ProhibitCardSelect_Run
	cmp r0, #0
	beq _0804098C
	bl DuelScreen_Init
	ldr r0, _08040970 @ =0x02017A40
	ldr r1, _08040974 @ =0x000003E5
	add r0, r0, r1
	b _08040986
_08040970: .4byte 0x02017A40
_08040974: .4byte 0x000003E5
_08040978:
	bl DuelScreen_FadeInStep
	cmp r0, #0
	beq _0804098C
	ldr r0, _08040990 @ =0x02017A40
	ldr r2, _08040994 @ =0x000003E5
	add r0, r0, r2
_08040986:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_0804098C:
	mov r0, #0
	b _080409CA
_08040990: .4byte 0x02017A40
_08040994: .4byte 0x000003E5
_08040998:
	mov r0, #8
	neg r0, r0
	ldrb r3, [r5, #0xA]
	and r0, r3
	strb r0, [r5, #0xA]
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r4, _080409D0 @ =0x03000040
	ldr r2, _080409D4 @ =0x00004872
	add r4, r4, r2
	ldrh r1, [r4]
	bl sub_08019820
	ldrh r1, [r4]
	add r0, r5, #0
	bl AddEffectTarget
	ldr r0, _080409D8 @ =0x02017A40
	ldr r3, _080409DC @ =0x000003E5
	add r0, r0, r3
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_080409C8:
	mov r0, #1
_080409CA:
	pop {r4, r5}
	pop {r1}
	bx r1
_080409D0: .4byte 0x03000040
_080409D4: .4byte 0x00004872
_080409D8: .4byte 0x02017A40
_080409DC: .4byte 0x000003E5
	thumb_func_end EffectProhibitionChainB

