	thumb_func_start QueueSpecialSummonFromHand
QueueSpecialSummonFromHand: @ 0x080561A0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r8, r0
	mov r9, r1
	ldr r4, [sp, #0x1C]
	lsl r3, r3, #0x10
	mov ip, r3
	lsr r5, r3, #0x10
	lsl r4, r4, #0x10
	ldr r3, _08056270 @ =0x0201CF90
	mov r7, #1
	mov r1, r8
	and r1, r7
	mov r0, #2
	neg r0, r0
	ldrb r6, [r3]
	and r0, r6
	orr r0, r1
	mov r1, #0x1F
	and r2, r1
	lsl r2, r2, #1
	mov r1, #0x3F
	neg r1, r1
	and r0, r1
	orr r0, r2
	strb r0, [r3]
	mov r0, #0xFF
	mov r1, r9
	and r1, r0
	lsl r1, r1, #6
	ldr r0, _08056274 @ =0xFFFFC03F
	ldrh r2, [r3]
	and r0, r2
	orr r0, r1
	strh r0, [r3]
	mov r0, #0x40
	ldrb r2, [r3, #1]
	orr r2, r0
	strb r2, [r3, #1]
	mov r1, #0
	add r6, r3, #0
	cmp r4, #0
	bne _080561FC
	mov r1, #1
_080561FC:
	lsl r1, r1, #7
	mov r0, #0x7F
	and r2, r0
	orr r2, r1
	strb r2, [r6, #1]
	cmp r5, #0
	beq _08056278
	lsl r3, r5, #0x18
	mov r5, ip
	lsr r2, r5, #0x18
	lsl r4, r2, #0x18
	mov r0, #7
	mov ip, r0
	lsr r1, r3, #0x18
	and r1, r0
	sub r0, #0xF
	ldrb r5, [r6, #2]
	and r0, r5
	orr r0, r1
	mov r1, ip
	and r2, r1
	lsl r2, r2, #3
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	orr r0, r2
	strb r0, [r6, #2]
	lsr r1, r3, #0x1F
	and r1, r7
	lsl r1, r1, #1
	mov r0, #3
	neg r0, r0
	ldrb r2, [r6, #3]
	and r0, r2
	orr r0, r1
	lsr r1, r4, #0x1F
	and r1, r7
	lsl r1, r1, #2
	mov r2, #5
	neg r2, r2
	and r0, r2
	orr r0, r1
	lsr r3, r3, #0x1C
	and r3, r7
	and r3, r7
	lsl r3, r3, #4
	mov r1, #0x11
	neg r1, r1
	and r0, r1
	orr r0, r3
	lsr r4, r4, #0x1C
	and r4, r7
	and r4, r7
	lsl r4, r4, #5
	sub r1, #0x10
	and r0, r1
	orr r0, r4
	b _08056294
_08056270: .4byte 0x0201CF90
_08056274: .4byte 0xFFFFC03F
_08056278:
	mov r0, #8
	neg r0, r0
	ldrb r3, [r6, #2]
	and r0, r3
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	strb r0, [r6, #2]
	mov r0, #3
	neg r0, r0
	ldrb r5, [r6, #3]
	and r0, r5
	add r1, #0x34
	and r0, r1
_08056294:
	strb r0, [r6, #3]
	mov r3, #1
	mov r0, r8
	and r0, r3
	mov r2, r9
	lsl r1, r2, #2
	ldr r2, _080562F4 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _080562F8 @ =0x02019968
	add r1, r1, r0
	ldr r2, [r1]
	lsl r2, r2, #0x14
	lsr r1, r2, #0x14
	and r1, r3
	lsl r1, r1, #7
	mov r0, #0x7F
	ldrb r3, [r6, #3]
	and r0, r3
	orr r0, r1
	strb r0, [r6, #3]
	lsr r2, r2, #0x15
	ldr r0, _080562FC @ =0xFFFF8000
	ldrh r5, [r6, #4]
	and r0, r5
	orr r0, r2
	strh r0, [r6, #4]
	mov r0, #0x1D
	neg r0, r0
	ldrb r1, [r6, #0xE]
	and r0, r1
	mov r1, #0x18
	orr r0, r1
	strb r0, [r6, #0xE]
	mov r0, #5
	strh r0, [r6, #0xC]
	mov r0, r8
	bl PayChainEnergyCost
	bl SummonAction_Start
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080562F4: .4byte 0x00000D64
_080562F8: .4byte 0x02019968
_080562FC: .4byte 0xFFFF8000
	thumb_func_end QueueSpecialSummonFromHand

