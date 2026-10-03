	thumb_func_start AiGetStrongestMonsterScore
AiGetStrongestMonsterScore: @ 0x080573D0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x14
	add r7, r0, #0
	str r1, [sp, #0xC]
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0x10]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov sl, r3
	mov r0, #1
	neg r0, r0
	mov r8, r0
	mov r6, #0
	mov r0, #1
	and r0, r7
	ldr r1, _08057434 @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	mov r9, r2
_08057400:
	ldr r0, [sp, #0xC]
	cmp r6, r0
	beq _08057472
	mov r0, #0x94
	mul r0, r6
	add r0, r9
	ldr r1, _08057438 @ =0x0201930C
	add r4, r0, r1
	ldr r0, [r4]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08057472
	mov r5, #0
	ldrb r1, [r4, #6]
	mov r2, #2
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _0805743C
	add r0, r7, #0
	add r1, r6, #0
	mov r2, sp
	bl GetZoneCardStats
	b _08057458
	.align 2, 0
_08057434: .4byte 0x00000D64
_08057438: .4byte 0x0201930C
_0805743C:
	add r0, r1, #0
	orr r0, r2
	strb r0, [r4, #6]
	add r0, r7, #0
	add r1, r6, #0
	mov r2, sp
	bl GetZoneCardStats
	mov r1, #3
	neg r1, r1
	add r0, r1, #0
	ldrb r2, [r4, #6]
	and r0, r2
	strb r0, [r4, #6]
_08057458:
	ldr r0, [sp, #0x10]
	cmp r0, #0
	beq _08057462
	ldr r0, [sp, #4]
	add r5, r5, r0
_08057462:
	mov r1, sl
	cmp r1, #0
	beq _0805746C
	ldr r0, [sp, #8]
	add r5, r5, r0
_0805746C:
	cmp r5, r8
	ble _08057472
	mov r8, r5
_08057472:
	add r6, #1
	cmp r6, #4
	ble _08057400
	mov r0, r8
	add sp, #0x14
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiGetStrongestMonsterScore
	.align 2, 0

