	thumb_func_start CoinToss_AnimateHighlights
CoinToss_AnimateHighlights: @ 0x08025108
	push {r4, r5, r6, lr}
	add r6, r0, #0
	lsl r1, r1, #0x18
	lsr r5, r1, #0x18
	mov r4, #0
	cmp r4, r5
	bcs _08025154
_08025116:
	lsl r0, r4, #1
	add r0, r0, r4
	lsl r0, r0, #2
	add r2, r0, r6
	ldrb r1, [r2, #0xA]
	cmp r1, #1
	bne _0802514A
	ldrb r0, [r2, #8]
	add r3, r0, #0
	cmp r3, #0
	bne _08025146
	mov r0, #6
	strb r0, [r2, #8]
	ldrb r0, [r2, #9]
	add r0, #1
	strb r0, [r2, #9]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #5
	bne _0802514A
	strb r3, [r2, #9]
	add r0, r1, #1
	strb r0, [r2, #0xA]
	b _0802514A
_08025146:
	sub r0, #1
	strb r0, [r2, #8]
_0802514A:
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, r5
	bcc _08025116
_08025154:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end CoinToss_AnimateHighlights
	.align 2, 0

