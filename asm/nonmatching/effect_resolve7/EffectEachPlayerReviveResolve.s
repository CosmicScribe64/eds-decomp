	thumb_func_start EffectEachPlayerReviveResolve
EffectEachPlayerReviveResolve: @ 0x0803732C
	push {r4, r5, r6, r7, lr}
	sub sp, #8
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _0803733E
	b _08037520
_0803733E:
	ldr r0, _0803737C @ =0x000007FF
	ldrh r2, [r4]
	and r0, r2
	lsl r0, r0, #1
	ldr r5, _08037380 @ =0x08622AB4
	add r0, r0, r5
	ldr r1, _08037384 @ =0x0000045C
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803738C
	ldrb r1, [r4, #2]
	mov r0, #0xE
	and r0, r1
	cmp r0, #6
	beq _0803738C
	mov r0, #1
	and r0, r1
	mov r2, #0x92
	cmp r0, #0
	beq _08037368
	ldr r2, _08037388 @ =0x00008092
_08037368:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	b _08037520
	.align 2, 0
_0803737C: .4byte 0x000007FF
_08037380: .4byte gCardIdToNumber
_08037384: .4byte 0x0000045C
_08037388: .4byte 0x00008092
_0803738C:
	ldr r2, _080373A8 @ =0x02017A40
	mov r0, #0xF8
	lsl r0, r0, #2
	add r3, r2, r0
	ldrb r0, [r3]
	add r7, r2, #0
	cmp r0, #0x7E
	beq _08037420
	cmp r0, #0x7E
	bgt _080373AC
	cmp r0, #0x7D
	bne _080373A6
	b _080374EE
_080373A6:
	b _08037520
_080373A8: .4byte 0x02017A40
_080373AC:
	cmp r0, #0x7F
	beq _080373CE
	cmp r0, #0x80
	beq _080373B6
	b _08037520
_080373B6:
	ldr r0, _0803740C @ =0x020192E0
	ldr r1, _08037410 @ =0x00001B12
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldr r5, _08037414 @ =0x000003E1
	add r1, r2, r5
	strb r0, [r1]
	ldrb r0, [r3]
	sub r0, #1
	strb r0, [r3]
_080373CE:
	ldr r0, _08037414 @ =0x000003E1
	add r5, r2, r0
	ldrb r0, [r5]
	bl CountFreeMonsterZones
	cmp r0, #0
	bgt _080373DE
	b _080374EA
_080373DE:
	ldrb r0, [r5]
	ldr r1, _08037418 @ =0x000007FF
	ldrh r2, [r4]
	and r1, r2
	lsl r1, r1, #1
	ldr r2, _0803741C @ =0x08622AB4
	add r1, r1, r2
	ldrh r1, [r1]
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #0
	ble _080374EA
	ldrb r0, [r5]
	ldrh r2, [r4]
	ldrb r4, [r4, #2]
	lsl r3, r4, #0x1F
	lsr r3, r3, #0x1F
	mov r1, #0xE
	bl DuelPrompt_Post
	mov r0, #0x7E
	b _08037522
_0803740C: .4byte 0x020192E0
_08037410: .4byte 0x00001B12
_08037414: .4byte 0x000003E1
_08037418: .4byte 0x000007FF
_0803741C: .4byte gCardIdToNumber
_08037420:
	ldr r3, _080374A4 @ =0x020192E0
	ldr r5, _080374A8 @ =0x00001B64
	add r1, r3, r5
	ldr r2, _080374AC @ =0x00001B66
	add r0, r3, r2
	ldrh r0, [r0]
	lsl r2, r0, #0x10
	ldrh r1, [r1]
	orr r2, r1
	str r2, [sp, #4]
	add r5, sp, #4
	ldr r1, _080374B0 @ =0x02015EE8
	mov r6, #1
	add r0, r6, #0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0803746A
	ldr r0, _080374B4 @ =0x00001B12
	add r1, r3, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0803746A
	lsl r1, r2, #0x13
	lsr r1, r1, #0x1F
	sub r1, r6, r1
	mov r0, #1
	and r1, r0
	lsl r1, r1, #4
	ldrb r2, [r5, #1]
	mov r0, #0x11
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #1]
_0803746A:
	ldr r1, _080374B8 @ =0x000003E1
	add r5, r7, r1
	ldrb r0, [r5]
	mov r3, #0xD3
	cmp r0, #0
	beq _08037478
	ldr r3, _080374BC @ =0x000080D3
_08037478:
	ldr r2, [sp, #4]
	lsl r1, r2, #0x10
	lsr r1, r1, #0x10
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r0, _080374C0 @ =0x000007FF
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r2, _080374C4 @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _080374C8 @ =0x0000045C
	cmp r1, r0
	beq _080374CC
	add r0, #0x2B
	cmp r1, r0
	beq _080374DA
	b _080374EA
_080374A4: .4byte 0x020192E0
_080374A8: .4byte 0x00001B64
_080374AC: .4byte 0x00001B66
_080374B0: .4byte 0x02015EE8
_080374B4: .4byte 0x00001B12
_080374B8: .4byte 0x000003E1
_080374BC: .4byte 0x000080D3
_080374C0: .4byte 0x000007FF
_080374C4: .4byte gCardIdToNumber
_080374C8: .4byte 0x0000045C
_080374CC:
	ldrb r0, [r5]
	add r1, sp, #4
	mov r2, #0
	mov r3, #0x20
	bl QueueSpecialSummonChoosePosition
	b _080374EA
_080374DA:
	ldrb r0, [r5]
	mov r1, #0x20
	str r1, [sp, #0]
	add r1, sp, #4
	mov r2, #0
	mov r3, #1
	bl QueueSpecialSummon
_080374EA:
	mov r0, #0x7D
	b _08037522
_080374EE:
	ldr r5, _08037510 @ =0x000003E1
	add r1, r2, r5
	mov r0, #1
	ldrb r2, [r1]
	sub r0, r0, r2
	strb r0, [r1]
	ldr r0, _08037514 @ =0x020192E0
	ldr r5, _08037518 @ =0x00001B12
	add r0, r0, r5
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1F
	ldrb r1, [r1]
	cmp r1, r0
	beq _0803751C
	mov r0, #0x7F
	b _08037522
_08037510: .4byte 0x000003E1
_08037514: .4byte 0x020192E0
_08037518: .4byte 0x00001B12
_0803751C:
	mov r0, #0x64
	b _08037522
_08037520:
	mov r0, #0
_08037522:
	add sp, #8
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectEachPlayerReviveResolve
	.align 2, 0

