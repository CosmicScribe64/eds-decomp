	thumb_func_start EffectReaperOfTheCardsResolve
EffectReaperOfTheCardsResolve: @ 0x08030400
	push {r4, r5, r6, r7, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _080304D6
	mov r2, #7
	ldrb r0, [r1, #0xA]
	and r2, r0
	cmp r2, #1
	bne _080304D6
	ldrb r5, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r6, r1, #8
	and r2, r5
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _08030468 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803046C @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	cmp r4, #0
	beq _080304D6
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _08030478
	ldr r0, _08030470 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _08030474 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _080304D6
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #1
	bl DestroyFieldCard
	b _080304D6
_08030468: .4byte 0x00000D64
_0803046C: .4byte 0x0201930C
_08030470: .4byte 0x000007FF
_08030474: .4byte gCardStats
_08030478:
	mov r0, #0x7F
	cmp r5, #0
	beq _08030480
	ldr r0, _080304B8 @ =0x0000807F
_08030480:
	add r7, r6, #0
	add r1, r7, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08019820
	ldr r0, _080304BC @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r2, _080304C0 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	bne _080304C4
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #1
	bl DestroyFieldCard
	b _080304D6
_080304B8: .4byte 0x0000807F
_080304BC: .4byte 0x000007FF
_080304C0: .4byte gCardStats
_080304C4:
	mov r0, #0x7F
	cmp r5, #0
	beq _080304CC
	ldr r0, _080304E0 @ =0x0000807F
_080304CC:
	add r1, r7, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080304D6:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080304E0: .4byte 0x0000807F
	thumb_func_end EffectReaperOfTheCardsResolve

