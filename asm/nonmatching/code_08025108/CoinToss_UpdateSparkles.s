	thumb_func_start CoinToss_UpdateSparkles
CoinToss_UpdateSparkles: @ 0x08025258
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r7, r0, #0
	mov r6, #0
	ldr r0, _080252D0 @ =0x08081FA4
	mov r8, r0
	mov r0, #2
	neg r0, r0
	mov ip, r0
_0802526C:
	lsl r0, r6, #3
	add r4, r7, r0
	ldrb r3, [r4]
	lsl r0, r3, #0x1F
	cmp r0, #0
	beq _080252BA
	lsl r0, r3, #0x1D
	lsr r0, r0, #0x1E
	sub r0, #1
	mov r1, #3
	and r0, r1
	lsl r1, r0, #1
	mov r5, #7
	neg r5, r5
	add r2, r5, #0
	and r2, r3
	orr r2, r1
	strb r2, [r4]
	cmp r0, #3
	bne _080252BA
	add r1, r5, #0
	and r1, r2
	lsr r0, r1, #3
	add r0, #1
	lsl r2, r0, #3
	mov r3, #7
	and r3, r1
	orr r3, r2
	strb r3, [r4]
	mov r1, #0x1F
	and r0, r1
	lsl r0, r0, #1
	add r0, r8
	ldrh r0, [r0]
	cmp r0, #0
	bne _080252BA
	mov r0, ip
	and r3, r0
	strb r3, [r4]
_080252BA:
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, #0x1F
	bls _0802526C
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080252D0: .4byte gCoinSparkleTiles
	thumb_func_end CoinToss_UpdateSparkles

