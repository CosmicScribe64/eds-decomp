	thumb_func_start EffectBigEyeResolve
EffectBigEyeResolve: @ 0x08030620
	push {r4, r5, r6, lr}
	add r6, r0, #0
	ldr r0, _08030640 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7E
	beq _080306F4
	cmp r0, #0x7E
	bgt _08030644
	cmp r0, #0x7D
	bne _0803063C
	b _08030740
_0803063C:
	b _0803074E
	.align 2, 0
_08030640: .4byte 0x02017A40
_08030644:
	cmp r0, #0x7F
	beq _080306C2
	cmp r0, #0x80
	beq _0803064E
	b _0803074E
_0803064E:
	ldr r2, _080306D4 @ =0x020192E4
	ldrb r3, [r6, #2]
	lsl r0, r3, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _080306D8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #3]
	cmp r0, #4
	bls _0803074E
	mov r4, #0
_08030664:
	lsl r3, r4, #2
	ldr r5, _080306DC @ =0x02017F84
	add r0, r3, r5
	ldrb r2, [r6, #2]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	ldr r2, _080306D8 @ =0x00000D64
	mul r1, r2
	ldr r2, _080306E0 @ =0x02019AA8
	add r1, r1, r2
	add r1, r1, r3
	bl CopyDuelCard
	add r4, #1
	cmp r4, #4
	ble _08030664
	add r2, r5, #0
	sub r2, #8
	ldr r0, [r2]
	ldr r1, _080306E4 @ =0xFFF00FFF
	and r0, r1
	str r0, [r2]
	sub r1, r5, #6
	ldr r2, _080306E8 @ =0xFFFFF00F
	add r0, r2, #0
	ldrh r3, [r1]
	and r0, r3
	strh r0, [r1]
	sub r1, r5, #5
	mov r0, #0xF
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	sub r1, r5, #4
	mov r0, #0x10
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	ldrh r0, [r1]
	and r2, r0
	strh r2, [r1]
	ldr r2, _080306EC @ =0xFFFFFE9C
	add r1, r5, r2
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
_080306C2:
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	bl DeckReorder_Run
	cmp r0, #0
	beq _080306F0
	mov r0, #0x7E
	b _08030750
_080306D4: .4byte 0x020192E4
_080306D8: .4byte 0x00000D64
_080306DC: .4byte 0x02017F84
_080306E0: .4byte 0x02019AA8
_080306E4: .4byte 0xFFF00FFF
_080306E8: .4byte 0xFFFFF00F
_080306EC: .4byte 0xFFFFFE9C
_080306F0:
	mov r0, #0x7F
	b _08030750
_080306F4:
	mov r4, #0
_080306F6:
	ldrb r3, [r6, #2]
	lsl r1, r3, #0x1F
	mov r5, #1
	lsr r1, r1, #0x1F
	ldr r0, _08030730 @ =0x00000D64
	mul r0, r1
	ldr r1, _08030734 @ =0x02019AA8
	add r0, r0, r1
	lsl r1, r4, #2
	add r0, r0, r1
	ldr r2, _08030738 @ =0x02017F84
	add r1, r1, r2
	bl CopyDuelCard
	add r4, #1
	cmp r4, #4
	ble _080306F6
	ldr r0, _0803073C @ =0x02015EE8
	ldrb r0, [r0, #1]
	and r5, r0
	cmp r5, #0
	beq _0803074E
	ldrb r6, [r6, #2]
	lsl r0, r6, #0x1F
	lsr r0, r0, #0x1F
	bl DuelLink_SendDeck
_0803072C:
	mov r0, #0x7D
	b _08030750
_08030730: .4byte 0x00000D64
_08030734: .4byte 0x02019AA8
_08030738: .4byte 0x02017F84
_0803073C: .4byte 0x02015EE8
_08030740:
	ldr r0, _08030758 @ =0x02017FB0
	ldr r1, _0803075C @ =0x00000305
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	cmp r0, #0
	bge _0803072C
_0803074E:
	mov r0, #0
_08030750:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08030758: .4byte 0x02017FB0
_0803075C: .4byte 0x00000305
	thumb_func_end EffectBigEyeResolve

