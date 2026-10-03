	thumb_func_start EffectSummonSameNameFromDeckResolve
EffectSummonSameNameFromDeckResolve: @ 0x080370B8
	push {r4, r5, r6, lr}
	sub sp, #0x88
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #0
	beq _080370CE
	b _0803731E
_080370CE:
	ldr r5, _080370EC @ =0x02017A40
	mov r3, #0xF8
	lsl r3, r3, #2
	add r0, r5, r3
	ldrb r0, [r0]
	cmp r0, #0x7E
	bne _080370DE
	b _08037214
_080370DE:
	cmp r0, #0x7E
	bgt _080370F0
	cmp r0, #0x7D
	bne _080370E8
	b _08037274
_080370E8:
	b _08037304
	.align 2, 0
_080370EC: .4byte 0x02017A40
_080370F0:
	cmp r0, #0x7F
	beq _08037148
	cmp r0, #0x80
	beq _080370FA
	b _08037304
_080370FA:
	ldr r0, _08037118 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0803711C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _08037120 @ =0x0000045A
	cmp r1, r0
	beq _08037124
	add r0, #1
	cmp r1, r0
	beq _08037138
	b _08037300
	.align 2, 0
_08037118: .4byte 0x000007FF
_0803711C: .4byte gCardIdToNumber
_08037120: .4byte 0x0000045A
_08037124:
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xFA
	lsl r1, r1, #1
	bl LoseLifePoints
	b _08037300
_08037138:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #0xFA
	lsl r1, r1, #2
	bl GainLifePoints
	b _08037300
_08037148:
	ldrb r3, [r4, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r6, _08037180 @ =0x000007FF
	add r1, r6, #0
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	ldr r5, _08037184 @ =0x08622AB4
	add r1, r1, r5
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	bne _0803716A
	b _0803731E
_0803716A:
	mov r1, #1
	add r0, r1, #0
	ldrb r3, [r4, #2]
	and r0, r3
	cmp r0, #0
	beq _0803718C
	ldr r0, _08037188 @ =0x0201AE60
	strh r1, [r0, #0x14]
	mov r0, #0x7E
	b _08037320
	.align 2, 0
_08037180: .4byte 0x000007FF
_08037184: .4byte gCardIdToNumber
_08037188: .4byte 0x0201AE60
_0803718C:
	add r0, r6, #0
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r1, [r0]
	ldr r0, _080371AC @ =0x0000045B
	cmp r1, r0
	beq _080371D8
	cmp r1, r0
	bgt _080371B0
	sub r0, #1
	cmp r1, r0
	beq _080371BC
	b _080371E8
	.align 2, 0
_080371AC: .4byte 0x0000045B
_080371B0:
	ldr r0, _080371B8 @ =0x0000051B
	cmp r1, r0
	beq _080371D8
	b _080371E8
_080371B8: .4byte 0x0000051B
_080371BC:
	ldr r1, _080371D0 @ =0x08083104
	ldrh r4, [r4]
	lsl r2, r4, #6
	ldr r3, _080371D4 @ =0x0822C720
	add r2, r2, r3
	add r0, sp, #4
	bl FormatStr
	b _080371E8
	.align 2, 0
_080371D0: .4byte gStrGiantGermSummonPrompt
_080371D4: .4byte gCardNames
_080371D8:
	ldr r1, _08037204 @ =0x0808313C
	ldrh r4, [r4]
	lsl r2, r4, #6
	ldr r0, _08037208 @ =0x0822C720
	add r2, r2, r0
	add r0, sp, #4
	bl FormatStr
_080371E8:
	ldr r0, _0803720C @ =0x00000206
	ldr r1, _08037210 @ =0x00000712
	mov r2, #0xB
	add r3, sp, #4
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	mov r0, #0x7E
	b _08037320
	.align 2, 0
_08037204: .4byte gStrSameNameSetPrompt
_08037208: .4byte gCardNames
_0803720C: .4byte 0x00000206
_08037210: .4byte 0x00000712
_08037214:
	ldr r0, _0803725C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _08037304
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _08037260 @ =0x000007FF
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	ldr r3, _08037264 @ =0x08622AB4
	add r1, r1, r3
	ldrh r1, [r1]
	add r2, sp, #0x84
	bl RemoveDeckCardByNumber
	add r3, r0, #0
	cmp r3, #0
	blt _08037304
	ldr r1, _08037268 @ =0x00000544
	add r0, r5, r1
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	ldr r2, _0803726C @ =0x00000D64
	mul r1, r2
	ldr r2, _08037270 @ =0x02019AA8
	add r1, r1, r2
	lsl r2, r3, #2
	add r1, r1, r2
	bl CopyDuelCard
	mov r0, #0x7D
	b _08037320
	.align 2, 0
_0803725C: .4byte 0x0201AE60
_08037260: .4byte 0x000007FF
_08037264: .4byte gCardIdToNumber
_08037268: .4byte 0x00000544
_0803726C: .4byte 0x00000D64
_08037270: .4byte 0x02019AA8
_08037274:
	ldr r0, _08037294 @ =0x000007FF
	ldrh r3, [r4]
	and r0, r3
	lsl r0, r0, #1
	ldr r1, _08037298 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0803729C @ =0x0000045B
	cmp r1, r0
	beq _080372C8
	cmp r1, r0
	bgt _080372A0
	sub r0, #1
	cmp r1, r0
	beq _080372AC
	b _08037300
_08037294: .4byte 0x000007FF
_08037298: .4byte gCardIdToNumber
_0803729C: .4byte 0x0000045B
_080372A0:
	ldr r0, _080372A8 @ =0x0000051B
	cmp r1, r0
	beq _080372E4
	b _08037300
_080372A8: .4byte 0x0000051B
_080372AC:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _080372C4 @ =0x00000544
	add r1, r5, r3
	str r2, [sp, #0]
	mov r2, #1
	mov r3, #0
	bl QueueSpecialSummon
	b _08037300
	.align 2, 0
_080372C4: .4byte 0x00000544
_080372C8:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _080372E0 @ =0x00000544
	add r1, r5, r3
	str r2, [sp, #0]
	mov r2, #0
	mov r3, #1
	bl QueueSpecialSummon
	b _08037300
	.align 2, 0
_080372E0: .4byte 0x00000544
_080372E4:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	ldr r3, _080372FC @ =0x00000544
	add r1, r5, r3
	str r2, [sp, #0]
	mov r2, #0
	mov r3, #1
	bl QueueSpecialSummon
	mov r0, #0xA
	b _08037320
_080372FC: .4byte 0x00000544
_08037300:
	mov r0, #0x7F
	b _08037320
_08037304:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0x60
	cmp r0, #0
	beq _08037312
	ldr r1, _08037328 @ =0x00008060
_08037312:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0803731E:
	mov r0, #0
_08037320:
	add sp, #0x88
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_08037328: .4byte 0x00008060
	thumb_func_end EffectSummonSameNameFromDeckResolve

