	thumb_func_start EffectFakeTrapResolve
EffectFakeTrapResolve: @ 0x0803283C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r6, r0, #0
	mov r8, r1
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	beq _08032856
	b _08032C94
_08032856:
	mov r2, r8
	cmp r2, #0
	bne _0803285E
	b _08032C94
_0803285E:
	mov r0, #1
	ldrb r2, [r6, #2]
	add r1, r0, #0
	mov r3, r8
	ldrb r3, [r3, #2]
	and r1, r3
	and r0, r2
	add r3, r2, #0
	cmp r1, r0
	bne _08032874
	b _08032C94
_08032874:
	ldr r0, _080328A0 @ =0x000007FF
	mov r4, r8
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r1, _080328A4 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _080328A8 @ =0x00000425
	cmp r1, r0
	bne _0803288C
	b _08032AB4
_0803288C:
	cmp r1, r0
	bgt _080328C4
	cmp r1, #0xDF
	beq _080328F4
	cmp r1, #0xDF
	bgt _080328AC
	cmp r1, #0x53
	beq _080328F4
	b _08032C94
	.align 2, 0
_080328A0: .4byte 0x000007FF
_080328A4: .4byte gCardIdToNumber
_080328A8: .4byte 0x00000425
_080328AC:
	ldr r0, _080328C0 @ =0x0000029F
	cmp r1, r0
	bne _080328B4
	b _080329C4
_080328B4:
	mov r0, #0xFB
	lsl r0, r0, #2
	cmp r1, r0
	beq _080328F4
	b _08032C94
	.align 2, 0
_080328C0: .4byte 0x0000029F
_080328C4:
	ldr r0, _080328E0 @ =0x00000437
	cmp r1, r0
	beq _080328F4
	cmp r1, r0
	bgt _080328E4
	sub r0, #0x11
	cmp r1, r0
	beq _080329C4
	add r0, #5
	cmp r1, r0
	bne _080328DC
	b _08032B9C
_080328DC:
	b _08032C94
	.align 2, 0
_080328E0: .4byte 0x00000437
_080328E4:
	ldr r0, _080329B0 @ =0x0000046F
	cmp r1, r0
	ble _080328EC
	b _08032C94
_080328EC:
	sub r0, #1
	cmp r1, r0
	bge _080328F4
	b _08032C94
_080328F4:
	mov r2, r8
	ldrb r7, [r2, #0xC]
	ldrh r0, [r2, #0xC]
	lsr r4, r0, #8
	mov r2, #1
	and r2, r7
	mov r0, #0x94
	mul r0, r4
	ldr r1, _080329B4 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080329B8 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	mov ip, r1
	cmp r5, #0
	bne _0803291C
	b _08032C94
_0803291C:
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	cmp r7, r0
	bne _08032930
	ldrh r1, [r6, #2]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x1A
	cmp r4, r0
	bne _08032930
	b _08032C94
_08032930:
	add r0, r5, #0
	ldr r2, _080329BC @ =0x000007FF
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _080329C0 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	mov r9, r1
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _0803294E
	b _08032C94
_0803294E:
	mov r2, #1
	mov sl, r2
	add r2, r7, #0
	mov r3, sl
	and r2, r3
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _080329B4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	add r1, ip
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08032990
	add r0, r7, #0
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl ShowRevealedCard
	add r0, r7, #0
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
_08032990:
	ldr r0, _080329BC @ =0x000007FF
	mov r2, r8
	ldrh r2, [r2]
	and r0, r2
	lsl r0, r0, #2
	ldr r3, _080329C0 @ =0x08621DE0
	add r0, r0, r3
	ldr r0, [r0]
	mov r4, r9
	and r0, r4
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _080329AC
	b _08032C94
_080329AC:
	mov r0, sl
	b _08032A8C
_080329B0: .4byte 0x0000046F
_080329B4: .4byte 0x00000D64
_080329B8: .4byte 0x0201930C
_080329BC: .4byte 0x000007FF
_080329C0: .4byte gCardStats
_080329C4:
	mov r4, #5
	mov r0, #1
	mov sl, r0
	ldr r1, _08032A4C @ =0x00000D64
	mov r9, r1
	ldr r7, _08032A50 @ =0x0201930C
_080329D0:
	ldrb r3, [r6, #2]
	lsl r2, r3, #0x1F
	lsr r0, r2, #0x1F
	mov r1, sl
	and r1, r0
	mov r0, #0x94
	add r3, r4, #0
	mul r3, r0
	mov r0, r9
	mul r0, r1
	add r0, r3, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	cmp r5, #0
	beq _08032A66
	ldr r0, _08032A54 @ =0x000007FF
	add r1, r0, #0
	add r0, r5, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08032A58 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _08032A5C
	lsr r1, r2, #0x1F
	mov r0, sl
	and r0, r1
	mov r1, r9
	mul r1, r0
	add r1, r3, r1
	add r1, r1, r7
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08032A66
	lsr r0, r2, #0x1F
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl ShowRevealedCard
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
	b _08032A66
_08032A4C: .4byte 0x00000D64
_08032A50: .4byte 0x0201930C
_08032A54: .4byte 0x000007FF
_08032A58: .4byte gCardStats
_08032A5C:
	lsr r0, r2, #0x1F
	add r1, r4, #0
	mov r2, #5
	bl DestroyFieldCard
_08032A66:
	add r4, #1
	cmp r4, #0xA
	ble _080329D0
	ldr r0, _08032AA8 @ =0x000007FF
	mov r4, r8
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #2
	ldr r1, _08032AAC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08032A8A
	b _08032C94
_08032A8A:
	mov r0, #1
_08032A8C:
	ldrb r6, [r6, #2]
	and r0, r6
	mov r1, #0xB0
	cmp r0, #0
	beq _08032A98
	ldr r1, _08032AB0 @ =0x000080B0
_08032A98:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _08032C94
	.align 2, 0
_08032AA8: .4byte 0x000007FF
_08032AAC: .4byte gCardStats
_08032AB0: .4byte 0x000080B0
_08032AB4:
	mov r4, #5
	mov r2, #1
	mov r9, r2
	ldr r3, _08032B3C @ =0x00000D64
	mov r8, r3
	ldr r7, _08032B40 @ =0x0201930C
_08032AC0:
	ldrb r0, [r6, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	mov r1, r9
	and r1, r0
	mov r0, #0x94
	add r3, r4, #0
	mul r3, r0
	mov r0, r8
	mul r0, r1
	add r0, r3, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	cmp r5, #0
	beq _08032B56
	ldr r0, _08032B44 @ =0x000007FF
	add r1, r0, #0
	add r0, r5, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08032B48 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _08032B4C
	lsr r1, r2, #0x1F
	mov r0, r9
	and r0, r1
	mov r1, r8
	mul r1, r0
	add r1, r3, r1
	add r1, r1, r7
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08032B56
	lsr r0, r2, #0x1F
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl ShowRevealedCard
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
	b _08032B56
_08032B3C: .4byte 0x00000D64
_08032B40: .4byte 0x0201930C
_08032B44: .4byte 0x000007FF
_08032B48: .4byte gCardStats
_08032B4C:
	lsr r0, r2, #0x1F
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_08032B56:
	add r4, #1
	cmp r4, #0xA
	ble _08032AC0
	mov r4, #5
_08032B5E:
	ldrb r0, [r6, #2]
	lsl r3, r0, #0x1F
	lsr r2, r3, #0x1F
	mov r5, #1
	sub r2, r5, r2
	and r2, r5
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _08032B94 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08032B98 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08032B8C
	lsr r0, r3, #0x1F
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_08032B8C:
	add r4, #1
	cmp r4, #0xA
	ble _08032B5E
	b _08032C7A
_08032B94: .4byte 0x00000D64
_08032B98: .4byte 0x0201930C
_08032B9C:
	mov r4, #0
	mov r1, #1
	mov r9, r1
	ldr r2, _08032C24 @ =0x00000D64
	mov r8, r2
	ldr r7, _08032C28 @ =0x0201930C
_08032BA8:
	ldrb r3, [r6, #2]
	lsl r2, r3, #0x1F
	lsr r0, r2, #0x1F
	mov r1, r9
	and r1, r0
	mov r0, #0x94
	add r3, r4, #0
	mul r3, r0
	mov r0, r8
	mul r0, r1
	add r0, r3, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r5, r0, #0x14
	cmp r5, #0
	beq _08032C3E
	ldr r0, _08032C2C @ =0x000007FF
	add r1, r0, #0
	add r0, r5, #0
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _08032C30 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _08032C34
	lsr r1, r2, #0x1F
	mov r0, r9
	and r0, r1
	mov r1, r8
	mul r1, r0
	add r1, r3, r1
	add r1, r1, r7
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _08032C3E
	lsr r0, r2, #0x1F
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	bl ShowRevealedCard
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
	b _08032C3E
_08032C24: .4byte 0x00000D64
_08032C28: .4byte 0x0201930C
_08032C2C: .4byte 0x000007FF
_08032C30: .4byte gCardStats
_08032C34:
	lsr r0, r2, #0x1F
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_08032C3E:
	add r4, #1
	cmp r4, #0xA
	ble _08032BA8
	mov r4, #0
_08032C46:
	ldrb r0, [r6, #2]
	lsl r3, r0, #0x1F
	lsr r2, r3, #0x1F
	mov r5, #1
	sub r2, r5, r2
	and r2, r5
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _08032CA4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08032CA8 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08032C74
	lsr r0, r3, #0x1F
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_08032C74:
	add r4, #1
	cmp r4, #0xA
	ble _08032C46
_08032C7A:
	add r0, r5, #0
	ldrb r6, [r6, #2]
	and r0, r6
	mov r1, #0xB0
	cmp r0, #0
	beq _08032C88
	ldr r1, _08032CAC @ =0x000080B0
_08032C88:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08032C94:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08032CA4: .4byte 0x00000D64
_08032CA8: .4byte 0x0201930C
_08032CAC: .4byte 0x000080B0
	thumb_func_end EffectFakeTrapResolve

