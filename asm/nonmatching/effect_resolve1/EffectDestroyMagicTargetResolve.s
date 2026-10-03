	thumb_func_start EffectDestroyMagicTargetResolve
EffectDestroyMagicTargetResolve: @ 0x08031940
	push {r4, r5, r6, r7, lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _08031A16
	mov r2, #7
	ldrb r0, [r1, #0xA]
	and r2, r0
	cmp r2, #1
	bne _08031A16
	ldrb r5, [r1, #0xC]
	ldrh r1, [r1, #0xC]
	lsr r6, r1, #8
	and r2, r5
	mov r0, #0x94
	add r1, r6, #0
	mul r1, r0
	ldr r0, _080319A8 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _080319AC @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	cmp r4, #0
	beq _08031A16
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _080319B8
	ldr r0, _080319B0 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r1, _080319B4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08031A16
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #1
	bl DestroyFieldCard
	b _08031A16
_080319A8: .4byte 0x00000D64
_080319AC: .4byte 0x0201930C
_080319B0: .4byte 0x000007FF
_080319B4: .4byte gCardStats
_080319B8:
	mov r0, #0x7F
	cmp r5, #0
	beq _080319C0
	ldr r0, _080319F8 @ =0x0000807F
_080319C0:
	add r7, r6, #0
	add r1, r7, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r5, #0
	add r1, r4, #0
	bl ShowRevealedCard
	ldr r0, _080319FC @ =0x000007FF
	and r4, r0
	lsl r0, r4, #2
	ldr r2, _08031A00 @ =0x08621DE0
	add r0, r0, r2
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _08031A04
	add r0, r5, #0
	add r1, r6, #0
	mov r2, #1
	bl DestroyFieldCard
	b _08031A16
_080319F8: .4byte 0x0000807F
_080319FC: .4byte 0x000007FF
_08031A00: .4byte gCardStats
_08031A04:
	mov r0, #0x7F
	cmp r5, #0
	beq _08031A0C
	ldr r0, _08031A20 @ =0x0000807F
_08031A0C:
	add r1, r7, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08031A16:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08031A20: .4byte 0x0000807F
	thumb_func_end EffectDestroyMagicTargetResolve

