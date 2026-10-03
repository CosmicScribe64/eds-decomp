	thumb_func_start CoinToss_SpawnSparkle
CoinToss_SpawnSparkle: @ 0x08025200
	push {r4, r5, r6, r7, lr}
	add r5, r2, #0
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	lsl r1, r1, #0x18
	lsr r6, r1, #0x18
	mov r0, #0x80
	lsl r0, r0, #1
	add r2, r5, r0
	ldrb r0, [r2]
	add r1, r0, #1
	strb r1, [r2]
	lsl r0, r0, #0x18
	mov r4, #0xF8
	lsl r4, r4, #0x15
	and r4, r0
	lsr r4, r4, #0x18
	bl Random
	add r1, r0, #0
	lsl r4, r4, #3
	add r5, r5, r4
	cmp r1, #0
	bge _08025232
	add r0, #0xF
_08025232:
	asr r0, r0, #4
	lsl r0, r0, #4
	sub r0, r1, r0
	add r0, r7, r0
	strb r0, [r5, #4]
	strb r6, [r5, #5]
	mov r0, #1
	ldrb r1, [r5]
	orr r0, r1
	mov r1, #7
	neg r1, r1
	and r0, r1
	mov r1, #7
	and r0, r1
	strb r0, [r5]
	mov r0, #1
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CoinToss_SpawnSparkle

