	thumb_func_start BattleScene_SetCardArtMap
BattleScene_SetCardArtMap: @ 0x0805E100
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	lsl r0, r0, #0xB
	mov r4, #0xC0
	lsl r4, r4, #0x13
	add r6, r0, r4
	lsr r0, r3, #0x1F
	add r0, r3, r0
	asr r3, r0, #1
	lsr r0, r1, #0x1F
	add r0, r1, r0
	asr r0, r0, #1
	lsl r0, r0, #1
	add r6, r6, r0
	lsl r2, r2, #5
	add r6, r6, r2
	mov r2, #0
	mov r0, #1
	mov sl, r0
	mov r4, sl
	and r4, r1
	mov sl, r4
	mov r0, #0x80
	lsl r0, r0, #0x11
	mov r9, r0
	lsl r3, r3, #0x18
_0805E13A:
	mov r4, sl
	cmp r4, #0
	beq _0805E17E
	lsr r0, r3, #0x18
	lsl r0, r0, #8
	strh r0, [r6]
	mov r0, #0x80
	lsl r0, r0, #0x11
	add r3, r3, r0
	mov r4, #0x20
	add r4, r4, r6
	mov r8, r4
	add r2, #1
	mov ip, r2
	add r4, r6, #2
	mov r0, r9
	add r2, r3, r0
	mov r6, #0x80
	lsl r6, r6, #0x12
	mov r5, #3
_0805E162:
	lsr r1, r3, #0x18
	lsr r0, r2, #0x18
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r4]
	add r2, r2, r6
	mov r0, #0x80
	lsl r0, r0, #0x12
	add r3, r3, r0
	add r4, #2
	sub r5, #1
	cmp r5, #0
	bge _0805E162
	b _0805E1B8
_0805E17E:
	mov r4, #0x20
	add r4, r4, r6
	mov r8, r4
	add r2, #1
	mov ip, r2
	add r4, r6, #0
	mov r0, r9
	add r2, r3, r0
	mov r7, #0x80
	lsl r7, r7, #0x12
	mov r5, #3
_0805E194:
	lsr r1, r3, #0x18
	lsr r0, r2, #0x18
	lsl r0, r0, #8
	orr r1, r0
	strh r1, [r4]
	add r2, r2, r7
	mov r0, #0x80
	lsl r0, r0, #0x12
	add r3, r3, r0
	add r4, #2
	sub r5, #1
	cmp r5, #0
	bge _0805E194
	lsr r0, r3, #0x18
	strh r0, [r6, #8]
	mov r4, #0x80
	lsl r4, r4, #0x11
	add r3, r3, r4
_0805E1B8:
	mov r6, r8
	mov r2, ip
	cmp r2, #9
	ble _0805E13A
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end BattleScene_SetCardArtMap
	.align 2, 0

