	thumb_func_start CoinToss_MarkMatchingCoins
CoinToss_MarkMatchingCoins: @ 0x0802515C
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov ip, r2
	mov r1, #0
	mov r3, #0
	cmp r3, r4
	bcs _08025190
_08025172:
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r5
	ldrb r0, [r0, #2]
	cmp r0, #2
	beq _08025186
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
_08025186:
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, r4
	bcc _08025172
_08025190:
	cmp r1, #0
	bne _080251C0
	mov r3, #0
	cmp r3, r4
	bcs _080251C0
	mov r6, #1
	mov r2, #3
	mov r1, #2
_080251A0:
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r5
	ldrb r7, [r0, #1]
	cmp r7, ip
	bne _080251B4
	strb r6, [r0, #0xA]
	strb r2, [r0, #2]
	b _080251B6
_080251B4:
	strb r1, [r0, #0xA]
_080251B6:
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, r4
	bcc _080251A0
_080251C0:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end CoinToss_MarkMatchingCoins
	.align 2, 0

