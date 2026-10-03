	thumb_func_start EffectOwnMonsterTargetChainB
EffectOwnMonsterTargetChainB: @ 0x0803F8D4
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _0803F8EC @ =0x02017A40
	ldr r1, _0803F8F0 @ =0x000003E5
	add r2, r0, r1
	ldrb r0, [r2]
	cmp r0, #0
	beq _0803F8F4
	cmp r0, #1
	beq _0803F99C
	b _0803F9EC
	.align 2, 0
_0803F8EC: .4byte 0x02017A40
_0803F8F0: .4byte 0x000003E5
_0803F8F4:
	mov r0, #8
	neg r0, r0
	ldrb r2, [r4, #0xA]
	and r0, r2
	strb r0, [r4, #0xA]
	ldr r0, _0803F920 @ =0x000007FF
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r3, _0803F924 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0803F928 @ =0x00000524
	cmp r1, r0
	beq _0803F950
	cmp r1, r0
	bgt _0803F930
	ldr r0, _0803F92C @ =0x000003B1
	cmp r1, r0
	beq _0803F93C
	b _0803F978
	.align 2, 0
_0803F920: .4byte 0x000007FF
_0803F924: .4byte gCardIdToNumber
_0803F928: .4byte 0x00000524
_0803F92C: .4byte 0x000003B1
_0803F930:
	ldr r0, _0803F938 @ =0x00000527
	cmp r1, r0
	beq _0803F96C
	b _0803F978
_0803F938: .4byte 0x00000527
_0803F93C:
	ldr r0, _0803F944 @ =0x00000206
	ldr r1, _0803F948 @ =0x00000712
	ldr r3, _0803F94C @ =0x08084348
	b _0803F956
_0803F944: .4byte 0x00000206
_0803F948: .4byte 0x00000712
_0803F94C: .4byte gStrDesignateOneOwnMonster
_0803F950:
	ldr r0, _0803F960 @ =0x00000206
	ldr r1, _0803F964 @ =0x00000712
	ldr r3, _0803F968 @ =0x08084368
_0803F956:
	mov r2, #0xB
	bl TextBoxOpen
	b _0803F978
	.align 2, 0
_0803F960: .4byte 0x00000206
_0803F964: .4byte 0x00000712
_0803F968: .4byte gStrDesignateOwnMonsterToRecall
_0803F96C:
	ldr r0, _0803F988 @ =0x00000206
	ldr r1, _0803F98C @ =0x00000712
	ldr r3, _0803F990 @ =0x080843A0
	mov r2, #0xB
	bl TextBoxOpen
_0803F978:
	ldr r0, _0803F994 @ =0x02017A40
	ldr r1, _0803F998 @ =0x000003E5
	add r0, r0, r1
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _0803F9EC
	.align 2, 0
_0803F988: .4byte 0x00000206
_0803F98C: .4byte 0x00000712
_0803F990: .4byte gStrDesignateOwnMonsterToBanish
_0803F994: .4byte 0x02017A40
_0803F998: .4byte 0x000003E5
_0803F99C:
	ldr r1, _0803F9B0 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0803F9B4
	mov r0, #0
	strb r0, [r2]
	b _0803F9EE
	.align 2, 0
_0803F9B0: .4byte 0x03000040
_0803F9B4:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _0803F9EC
	ldr r0, _0803F9E0 @ =0x0201CFB0
	ldr r2, _0803F9E4 @ =0x00000824
	add r1, r0, r2
	ldr r1, [r1]
	ldr r3, _0803F9E8 @ =0x00000828
	add r2, r0, r3
	add r3, #4
	add r0, r0, r3
	ldr r2, [r2]
	ldr r0, [r0]
	add r2, r2, r0
	add r0, r4, #0
	bl TryAddEffectTarget
	mov r0, #1
	b _0803F9EE
	.align 2, 0
_0803F9E0: .4byte 0x0201CFB0
_0803F9E4: .4byte 0x00000824
_0803F9E8: .4byte 0x00000828
_0803F9EC:
	mov r0, #0
_0803F9EE:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectOwnMonsterTargetChainB

