	thumb_func_start EffectDamageOpponentResolve
EffectDamageOpponentResolve: @ 0x08031208
	push {r4, r5, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	beq _08031218
	b _08031322
_08031218:
	ldr r0, _08031244 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08031248 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0803124C @ =0x00000159
	cmp r1, r0
	beq _080312A6
	cmp r1, r0
	bgt _08031250
	sub r0, #2
	cmp r1, r0
	beq _08031286
	cmp r1, r0
	bgt _08031292
	sub r0, #1
	cmp r1, r0
	beq _08031274
	b _08031322
	.align 2, 0
_08031244: .4byte 0x000007FF
_08031248: .4byte gCardIdToNumber
_0803124C: .4byte 0x00000159
_08031250:
	ldr r0, _08031264 @ =0x000003EF
	cmp r1, r0
	beq _080312DC
	cmp r1, r0
	bgt _08031268
	mov r0, #0xAD
	lsl r0, r0, #1
	cmp r1, r0
	beq _080312BA
	b _08031322
_08031264: .4byte 0x000003EF
_08031268:
	ldr r0, _08031270 @ =0x0000040F
	cmp r1, r0
	beq _080312F0
	b _08031322
_08031270: .4byte 0x0000040F
_08031274:
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xC8
	bl LoseLifePoints
	b _08031322
_08031286:
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	b _080312D2
_08031292:
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0x96
	lsl r1, r1, #2
	bl LoseLifePoints
	b _08031322
_080312A6:
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xC8
	lsl r1, r1, #2
	bl LoseLifePoints
	b _08031322
_080312BA:
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0xFA
	lsl r1, r1, #2
	bl LoseLifePoints
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
_080312D2:
	mov r1, #0xFA
	lsl r1, r1, #1
	bl LoseLifePoints
	b _08031322
_080312DC:
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	mov r1, #0x96
	lsl r1, r1, #1
	bl LoseLifePoints
	b _08031322
_080312F0:
	ldr r5, _0803132C @ =0x020192E4
	ldrb r4, [r4, #2]
	lsl r1, r4, #0x1F
	lsr r0, r1, #0x1F
	mov r2, #1
	sub r0, r2, r0
	and r0, r2
	ldr r3, _08031330 @ =0x00000D64
	mul r0, r3
	add r0, r0, r5
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _08031322
	lsr r0, r1, #0x1F
	sub r0, r2, r0
	lsr r1, r1, #0x1F
	sub r1, r2, r1
	and r1, r2
	mul r1, r3
	add r1, r1, r5
	mov r2, #0xC8
	ldrb r1, [r1, #2]
	mul r1, r2
	bl LoseLifePoints
_08031322:
	mov r0, #0
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0
_0803132C: .4byte 0x020192E4
_08031330: .4byte 0x00000D64
	thumb_func_end EffectDamageOpponentResolve

