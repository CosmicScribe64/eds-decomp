	thumb_func_start EffectTributeOpponentMonsterResolve
EffectTributeOpponentMonsterResolve: @ 0x08038FB8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x80
	add r6, r0, #0
	add r3, r1, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	beq _08038FD4
	b _08039624
_08038FD4:
	ldr r1, _08038FF4 @ =0x02017A40
	mov r2, #0xF8
	lsl r2, r2, #2
	add r0, r1, r2
	ldrb r0, [r0]
	sub r0, #0x62
	add r2, r1, #0
	cmp r0, #0x1E
	bls _08038FE8
	b _08039624
_08038FE8:
	lsl r0, r0, #2
	ldr r1, _08038FF8 @ =0x08038FFC
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_08038FF4: .4byte 0x02017A40
_08038FF8: .4byte 0x08038FFC
_08038FFC:
	.4byte _080395C8
	.4byte _080394BC
	.4byte _080394A0
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039448
	.4byte _08039314
	.4byte _080392D8
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _08039624
	.4byte _080392C8
	.4byte _0803908C
	.4byte _08039078
_08039078:
	add r0, r6, #0
	add r1, r3, #0
	mov r2, #0
	bl EffectCanTributeOpponentMonsterPrepare
	cmp r0, #0
	bne _08039088
	b _08039624
_08039088:
	mov r0, #0x7F
	b _08039626
_0803908C:
	mov r3, #0
	mov sl, r3
	ldr r1, _0803911C @ =0x020192E4
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	mov ip, r0
	mov r4, #1
	lsr r0, r0, #0x1F
	ldr r5, _08039120 @ =0x00000D64
	mul r0, r5
	add r0, r0, r1
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1B
	add r3, r2, #0
	cmp r0, #0
	bge _080390AE
	b _080391E4
_080390AE:
	mov r7, #0
	mov r1, ip
	lsr r0, r1, #0x1F
	and r4, r0
	add r0, r4, #0
	mul r0, r5
	ldr r1, _0803911C @ =0x020192E4
	add r0, r0, r1
	ldrb r0, [r0, #2]
	cmp sl, r0
	blt _080390C6
	b _080391E4
_080390C6:
	ldr r3, _08039124 @ =0x000007FF
	mov r8, r3
	mov r0, #0xF8
	lsl r0, r0, #0x11
	mov r9, r0
_080390D0:
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r7, #2
	mul r0, r5
	add r1, r1, r0
	ldr r0, _08039128 @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	add r0, r4, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _0803912C @ =0x08621DE0
	add r5, r0, r2
	ldr r0, [r5]
	mov r3, r9
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _080391CA
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _080391CA
	ldr r0, [r5]
	mov r1, r9
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08039138
	cmp r0, #0x17
	ble _08039130
	cmp r0, #0x18
	beq _08039134
	b _08039138
_0803911C: .4byte 0x020192E4
_08039120: .4byte 0x00000D64
_08039124: .4byte 0x000007FF
_08039128: .4byte 0x02019968
_0803912C: .4byte gCardStats
_08039130:
	mov r0, #0
	b _0803914E
_08039134:
	mov r0, #0xA
	b _0803914E
_08039138:
	add r0, r4, #0
	mov r2, r8
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _08039174 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0803914E:
	cmp r0, #4
	bls _080391CA
	add r0, r4, #0
	mov r1, r8
	and r0, r1
	lsl r0, r0, #2
	ldr r2, _08039174 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r3, r9
	and r0, r3
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08039180
	cmp r0, #0x17
	ble _08039178
	cmp r0, #0x18
	beq _0803917C
	b _08039180
_08039174: .4byte gCardStats
_08039178:
	mov r0, #0
	b _08039194
_0803917C:
	mov r0, #0xA
	b _08039194
_08039180:
	mov r0, r8
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _080391B0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08039194:
	cmp r0, #6
	bhi _080391B4
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	bl FindFreeMonsterZone
	mov r1, #1
	neg r1, r1
	cmp r0, r1
	beq _080391CA
	mov r3, #1
	mov sl, r3
	b _080391CA
_080391B0: .4byte gCardStats
_080391B4:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	bl CountTributableMonsters
	cmp r0, #0
	ble _080391CA
	mov r2, #1
	mov sl, r2
_080391CA:
	add r7, #1
	ldr r0, _08039264 @ =0x020192E4
	ldrb r2, [r6, #2]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	ldr r5, _08039268 @ =0x00000D64
	mul r1, r5
	add r1, r1, r0
	add r3, r2, #0
	ldrb r1, [r1, #2]
	cmp r7, r1
	bge _080391E4
	b _080390D0
_080391E4:
	lsl r1, r3, #0x1F
	lsr r0, r1, #0x1F
	ldr r2, _08039268 @ =0x00000D64
	mul r0, r2
	ldr r3, _08039264 @ =0x020192E4
	add r0, r0, r3
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1B
	cmp r0, #0
	bge _080391FC
	mov r0, #0
	mov sl, r0
_080391FC:
	mov r3, #0
	mov ip, r3
	mov r7, #0
	ldr r0, _08039264 @ =0x020192E4
	add r0, #0x28
	mov r8, r0
	add r4, r1, #0
	mov r6, #1
	add r5, r2, #0
	ldr r1, _0803926C @ =0x000007FF
	mov r9, r1
_08039212:
	lsr r0, r4, #0x1F
	add r1, r6, #0
	and r1, r0
	mov r0, #0x94
	add r3, r7, #0
	mul r3, r0
	add r0, r1, #0
	mul r0, r5
	add r0, r3, r0
	add r0, r8
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _08039282
	lsr r1, r4, #0x1F
	add r0, r6, #0
	and r0, r1
	add r1, r0, #0
	mul r1, r5
	add r1, r3, r1
	add r1, r8
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08039282
	mov r3, r9
	and r2, r3
	lsl r0, r2, #1
	ldr r1, _08039270 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08039274 @ =0x00000105
	cmp r1, r0
	beq _0803927E
	cmp r1, r0
	bgt _08039278
	cmp r1, #0x58
	beq _0803927E
	b _08039282
_08039264: .4byte 0x020192E4
_08039268: .4byte 0x00000D64
_0803926C: .4byte 0x000007FF
_08039270: .4byte gCardIdToNumber
_08039274: .4byte 0x00000105
_08039278:
	ldr r0, _08039298 @ =0x000001FF
	cmp r1, r0
	bne _08039282
_0803927E:
	mov r2, #1
	mov ip, r2
_08039282:
	add r7, #1
	cmp r7, #4
	ble _08039212
	mov r3, sl
	cmp r3, #0
	beq _0803929C
	mov r0, ip
	cmp r0, #0
	bne _080392A4
_08039294:
	mov r0, #0x78
	b _08039626
_08039298: .4byte 0x000001FF
_0803929C:
	mov r1, ip
	cmp r1, #0
	bne _080392D0
	b _08039624
_080392A4:
	mov r0, #0x81
	lsl r0, r0, #2
	ldr r1, _080392C0 @ =0x00000717
	ldr r3, _080392C4 @ =0x08083300
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #2
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7E
	b _08039626
_080392C0: .4byte 0x00000717
_080392C4: .4byte gStrPromptTributeUse
_080392C8:
	ldr r0, _080392D4 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08039294
_080392D0:
	mov r0, #0x64
	b _08039626
_080392D4: .4byte 0x0201AE60
_080392D8:
	ldr r0, _08039300 @ =0x00000206
	ldr r1, _08039304 @ =0x00000712
	ldr r3, _08039308 @ =0x08083350
	mov r2, #0xB
	bl TextBoxOpen
	ldr r2, _0803930C @ =0x020192E4
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08039310 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #0x10
	ldrb r2, [r1, #8]
	orr r0, r2
	strb r0, [r1, #8]
_080392FA:
	mov r0, #0x77
	b _08039626
	.align 2, 0
_08039300: .4byte 0x00000206
_08039304: .4byte 0x00000712
_08039308: .4byte gStrSelectHighLevelMonster
_0803930C: .4byte 0x020192E4
_08039310: .4byte 0x00000D64
_08039314:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _080392FA
	ldr r0, _08039378 @ =0x0201CFB0
	ldr r3, _0803937C @ =0x0000082C
	add r0, r0, r3
	ldr r0, [r0]
	mov r8, r0
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r2, r8
	lsl r1, r2, #2
	ldr r2, _08039380 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08039384 @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	ldr r0, _08039388 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r3, _0803938C @ =0x08621DE0
	add r5, r0, r3
	ldr r0, [r5]
	mov r7, #0xF8
	lsl r7, r7, #0x11
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08039440
	add r0, r4, #0
	bl IsSpecialSummonOnly
	cmp r0, #0
	bne _08039440
	ldr r0, [r5]
	and r0, r7
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08039398
	cmp r0, #0x17
	ble _08039390
	cmp r0, #0x18
	beq _08039394
	b _08039398
_08039378: .4byte 0x0201CFB0
_0803937C: .4byte 0x0000082C
_08039380: .4byte 0x00000D64
_08039384: .4byte 0x02019968
_08039388: .4byte 0x000007FF
_0803938C: .4byte gCardStats
_08039390:
	mov r0, #0
	b _080393AC
_08039394:
	mov r0, #0xA
	b _080393AC
_08039398:
	ldr r0, _080393D4 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _080393D8 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080393AC:
	cmp r0, #4
	bls _08039440
	ldr r0, _080393D4 @ =0x000007FF
	and r0, r4
	lsl r0, r0, #2
	ldr r2, _080393D8 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080393E4
	cmp r0, #0x17
	ble _080393DC
	cmp r0, #0x18
	beq _080393E0
	b _080393E4
	.align 2, 0
_080393D4: .4byte 0x000007FF
_080393D8: .4byte gCardStats
_080393DC:
	mov r0, #0
	b _080393F8
_080393E0:
	mov r0, #0xA
	b _080393F8
_080393E4:
	ldr r0, _08039418 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r3, _0803941C @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_080393F8:
	cmp r0, #6
	bhi _08039420
	ldrh r0, [r6, #0xC]
	mov r1, #0
	bl TributeMonster
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r4, r0, #0x1F
	add r0, r4, #0
	bl FindFreeMonsterZone
	add r2, r0, #0
	add r0, r4, #0
	mov r1, r8
	b _08039486
_08039418: .4byte 0x000007FF
_0803941C: .4byte gCardStats
_08039420:
	ldr r0, _08039434 @ =0x00000206
	ldr r1, _08039438 @ =0x00000712
	ldr r3, _0803943C @ =0x0808339C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, r8
	strh r0, [r6, #0xE]
_08039430:
	mov r0, #0x76
	b _08039626
_08039434: .4byte 0x00000206
_08039438: .4byte 0x00000712
_0803943C: .4byte gStrSelectSecondTribute
_08039440:
	mov r0, #3
	bl PlaySE
	b _080392FA
_08039448:
	mov r0, #0xF0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _08039430
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08039490 @ =0x0201CFB0
	ldr r2, _08039494 @ =0x0000082C
	add r4, r1, r2
	ldr r1, [r4]
	bl IsTributableMonster
	cmp r0, #0
	beq _08039498
	ldrh r0, [r6, #0xC]
	mov r1, #0
	bl TributeMonster
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, [r4]
	bl TributeMonster
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r6, #0xE]
	ldr r2, [r4]
_08039486:
	mov r3, #0
	bl QueueNormalSummonChoosePosition
	mov r0, #0x6E
	b _08039626
_08039490: .4byte 0x0201CFB0
_08039494: .4byte 0x0000082C
_08039498:
	mov r0, #3
	bl PlaySE
	b _08039430
_080394A0:
	ldr r0, _080394B0 @ =0x00000206
	ldr r1, _080394B4 @ =0x00000411
	ldr r3, _080394B8 @ =0x080833EC
	mov r2, #0xB
	bl TextBoxOpen
_080394AC:
	mov r0, #0x63
	b _08039626
_080394B0: .4byte 0x00000206
_080394B4: .4byte 0x00000411
_080394B8: .4byte gStrSelectEffectMonster
_080394BC:
	mov r0, #0xE0
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _080394AC
	ldr r0, _0803950C @ =0x0201CFB0
	ldr r2, _08039510 @ =0x00000824
	add r1, r0, r2
	ldr r7, [r1]
	ldr r3, _08039514 @ =0x00000828
	add r1, r0, r3
	add r2, #8
	add r0, r0, r2
	ldr r1, [r1]
	ldr r0, [r0]
	add r5, r1, r0
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	mul r0, r5
	ldr r1, _08039518 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0803951C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r3, _08039520 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _08039524 @ =0x00000105
	cmp r1, r0
	beq _0803952E
	cmp r1, r0
	bgt _08039528
	cmp r1, #0x58
	beq _0803952E
	b _080395C0
	.align 2, 0
_0803950C: .4byte 0x0201CFB0
_08039510: .4byte 0x00000824
_08039514: .4byte 0x00000828
_08039518: .4byte 0x00000D64
_0803951C: .4byte 0x0201930C
_08039520: .4byte gCardIdToNumber
_08039524: .4byte 0x00000105
_08039528:
	ldr r0, _080395A0 @ =0x000001FF
	cmp r1, r0
	bne _080395C0
_0803952E:
	mov r0, #1
	bl PlaySE
	mov r0, #1
	mov r8, r0
	ldrb r1, [r6, #2]
	and r0, r1
	mov r4, #8
	cmp r0, #0
	beq _08039544
	ldr r4, _080395A4 @ =0x00008008
_08039544:
	ldr r0, _080395A8 @ =0x0201CFB0
	ldr r2, _080395AC @ =0x00000824
	add r1, r0, r2
	ldrh r1, [r1]
	add r2, #4
	add r3, r0, r2
	add r2, #4
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r2, r0, #8
	ldrb r3, [r3]
	orr r2, r3
	add r0, r4, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r4, _080395B0 @ =0x02017F24
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #0x14
	bl MemCopy16
	mov r3, r8
	and r7, r3
	mov r0, #0x94
	mul r0, r5
	ldr r1, _080395B4 @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	ldr r1, _080395B8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	strh r0, [r4]
	mov r0, #0x3F
	and r5, r0
	lsl r1, r5, #4
	ldr r0, _080395BC @ =0xFFFFFC0F
	ldrh r2, [r4, #2]
	and r0, r2
	orr r0, r1
	strh r0, [r4, #2]
	mov r0, #0x62
	b _08039626
	.align 2, 0
_080395A0: .4byte 0x000001FF
_080395A4: .4byte 0x00008008
_080395A8: .4byte 0x0201CFB0
_080395AC: .4byte 0x00000824
_080395B0: .4byte 0x02017F24
_080395B4: .4byte 0x00000D64
_080395B8: .4byte 0x0201930C
_080395BC: .4byte 0xFFFFFC0F
_080395C0:
	mov r0, #3
	bl PlaySE
	b _080394AC
_080395C8:
	ldr r3, _080395EC @ =0x000004E4
	add r1, r2, r3
	ldr r0, _080395F0 @ =0x000007FF
	ldrh r2, [r1]
	and r0, r2
	lsl r0, r0, #1
	ldr r3, _080395F4 @ =0x08622AB4
	add r0, r0, r3
	ldrh r2, [r0]
	ldr r0, _080395F8 @ =0x00000105
	cmp r2, r0
	beq _08039618
	cmp r2, r0
	bgt _080395FC
	cmp r2, #0x58
	beq _08039602
	b _08039624
	.align 2, 0
_080395EC: .4byte 0x000004E4
_080395F0: .4byte 0x000007FF
_080395F4: .4byte gCardIdToNumber
_080395F8: .4byte 0x00000105
_080395FC:
	ldr r0, _08039610 @ =0x000001FF
	cmp r2, r0
	bne _08039624
_08039602:
	ldr r0, _08039614 @ =0x02017F24
	mov r1, #0
	bl EffectCatapultTurtleResolve
	mov r0, #0x61
	b _08039626
	.align 2, 0
_08039610: .4byte 0x000001FF
_08039614: .4byte 0x02017F24
_08039618:
	add r0, r1, #0
	mov r1, #0
	bl EffectTheLittleSwordsmanOfAileResolve
	mov r0, #0x61
	b _08039626
_08039624:
	mov r0, #0
_08039626:
	add sp, #0x80
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectTributeOpponentMonsterResolve
	.align 2, 0

