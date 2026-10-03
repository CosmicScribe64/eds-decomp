	thumb_func_start QueueSpecialSummon
QueueSpecialSummon: @ 0x08055F70
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	add r7, r1, #0
	ldr r0, [sp, #0x20]
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r9, r3
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov sl, r0
	ldr r4, _08056084 @ =0x0000047F
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08055FAA
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _08055FAC
_08055FAA:
	mov r6, #1
_08055FAC:
	ldr r0, [r7]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	bl IsSpecialSummonOnly
	cmp r0, #0
	beq _08055FBC
	mov r6, #1
_08055FBC:
	ldr r4, _08056088 @ =0x0201CF90
	mov r0, #1
	mov r8, r0
	add r1, r5, #0
	mov r2, r8
	and r1, r2
	mov r0, #2
	neg r0, r0
	ldrb r2, [r4]
	and r0, r2
	orr r0, r1
	strb r0, [r4]
	add r0, r5, #0
	bl FindFreeMonsterZone
	mov r1, #0x1F
	and r0, r1
	lsl r0, r0, #1
	mov r1, #0x3F
	neg r1, r1
	ldrb r2, [r4]
	and r1, r2
	orr r1, r0
	strb r1, [r4]
	ldr r0, _0805608C @ =0xFFFFC03F
	ldrh r1, [r4]
	and r0, r1
	strh r0, [r4]
	mov r2, r8
	and r6, r2
	lsl r1, r6, #6
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r4, #1]
	and r0, r2
	orr r0, r1
	mov r2, r9
	lsl r1, r2, #7
	mov r6, #0x7F
	and r0, r6
	orr r0, r1
	strb r0, [r4, #1]
	mov r0, #8
	neg r0, r0
	ldrb r1, [r4, #2]
	and r0, r1
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	strb r0, [r4, #2]
	mov r0, #3
	neg r0, r0
	ldrb r2, [r4, #3]
	and r0, r2
	add r1, #0x34
	and r0, r1
	strb r0, [r4, #3]
	ldr r3, [r7]
	lsl r3, r3, #0x14
	lsr r1, r3, #0x14
	mov r2, #1
	and r1, r2
	lsl r1, r1, #7
	and r0, r6
	orr r0, r1
	strb r0, [r4, #3]
	lsr r3, r3, #0x15
	ldr r0, _08056090 @ =0xFFFF8000
	ldrh r1, [r4, #4]
	and r0, r1
	orr r0, r3
	strh r0, [r4, #4]
	mov r2, sl
	strh r2, [r4, #0xC]
	add r0, r4, #0
	add r0, #8
	add r1, r7, #0
	bl CopyDuelCard
	mov r0, #0x1D
	neg r0, r0
	ldrb r1, [r4, #0xE]
	and r0, r1
	mov r1, #0x10
	orr r0, r1
	strb r0, [r4, #0xE]
	mov r1, #4
	mov r0, sl
	orr r0, r1
	strh r0, [r4, #0xC]
	bl SummonAction_Start
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08056084: .4byte 0x0000047F
_08056088: .4byte 0x0201CF90
_0805608C: .4byte 0xFFFFC03F
_08056090: .4byte 0xFFFF8000
	thumb_func_end QueueSpecialSummon

