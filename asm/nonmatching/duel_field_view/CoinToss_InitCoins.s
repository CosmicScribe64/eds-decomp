	thumb_func_start CoinToss_InitCoins
CoinToss_InitCoins: @ 0x08024D48
	push {r4, r5, r6, lr}
	add r3, r0, #0
	mov r2, #0
	ldr r6, _08024DC4 @ =0x02017A30
	mov r1, #0
	mov r5, #3
	mov r4, #6
_08024D56:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #2
	add r0, r3, r0
	strb r5, [r0]
	strb r1, [r0, #1]
	strb r1, [r0, #2]
	strh r1, [r0, #4]
	strh r1, [r0, #6]
	strb r1, [r0, #0xA]
	strb r1, [r0, #9]
	strb r4, [r0, #8]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #7
	bls _08024D56
	add r0, r3, #0
	add r0, #0x61
	mov r4, #0
	strb r4, [r0]
	add r0, #3
	strb r4, [r0]
	add r0, #1
	strb r4, [r0]
	add r1, r3, #0
	add r1, #0x60
	mov r0, #3
	strb r0, [r1]
	mov r0, #0x62
	add r0, r0, r3
	mov ip, r0
	strb r4, [r0]
	add r3, #0x63
	mov r0, #2
	strb r0, [r3]
	ldrh r2, [r6, #6]
	mov r0, #0xFE
	lsl r0, r0, #7
	and r0, r2
	lsr r0, r0, #8
	strb r0, [r1]
	lsl r0, r2, #0x10
	lsr r0, r0, #0x1D
	mov r1, #4
	and r0, r1
	mov r1, ip
	strb r0, [r1]
	mov r0, #0x80
	and r0, r2
	cmp r0, #0
	beq _08024DC8
	strb r4, [r3]
	b _08024DCC
	.align 2, 0
_08024DC4: .4byte 0x02017A30
_08024DC8:
	mov r0, #0xA
	strb r0, [r3]
_08024DCC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end CoinToss_InitCoins
	.align 2, 0

