	thumb_func_start QueueNormalSummon
QueueNormalSummon: @ 0x08055B28
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	mov r8, r1
	ldr r4, [sp, #0x20]
	lsl r3, r3, #0x10
	lsr r5, r3, #0x10
	lsl r4, r4, #0x10
	ldr r3, _08055B84 @ =0x0201CF90
	mov r0, #1
	mov r1, sl
	and r1, r0
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
	mov r1, r8
	and r1, r0
	lsl r1, r1, #6
	ldr r0, _08055B88 @ =0xFFFFC03F
	ldrh r7, [r3]
	and r0, r7
	orr r0, r1
	strh r0, [r3]
	cmp r4, #0
	beq _08055B8C
	mov r0, #0x40
	ldrb r1, [r3, #1]
	orr r0, r1
	mov r1, #0x7F
	and r0, r1
	b _08055B98
	.align 2, 0
_08055B84: .4byte 0x0201CF90
_08055B88: .4byte 0xFFFFC03F
_08055B8C:
	mov r0, #0x41
	neg r0, r0
	ldrb r2, [r3, #1]
	and r0, r2
	mov r1, #0x80
	orr r0, r1
_08055B98:
	strb r0, [r3, #1]
	ldr r4, _08055C28 @ =0x0000047F
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08055BB4
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _08055BBE
_08055BB4:
	ldr r1, _08055C2C @ =0x0201CF90
	mov r0, #0x40
	ldrb r3, [r1, #1]
	orr r0, r3
	strb r0, [r1, #1]
_08055BBE:
	cmp r5, #0
	beq _08055C30
	lsl r3, r5, #0x18
	lsr r2, r5, #8
	lsl r4, r2, #0x18
	ldr r6, _08055C2C @ =0x0201CF90
	mov r5, #7
	lsr r1, r3, #0x18
	and r1, r5
	mov r0, #8
	neg r0, r0
	ldrb r7, [r6, #2]
	and r0, r7
	orr r0, r1
	and r2, r5
	lsl r2, r2, #3
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	orr r0, r2
	strb r0, [r6, #2]
	lsr r1, r3, #0x1F
	lsl r1, r1, #1
	mov r0, #3
	neg r0, r0
	ldrb r2, [r6, #3]
	and r0, r2
	orr r0, r1
	mov r5, #1
	lsr r1, r4, #0x1F
	lsl r1, r1, #2
	mov r2, #5
	neg r2, r2
	and r0, r2
	orr r0, r1
	lsr r3, r3, #0x1C
	and r3, r5
	and r3, r5
	lsl r3, r3, #4
	mov r1, #0x11
	neg r1, r1
	and r0, r1
	orr r0, r3
	lsr r4, r4, #0x1C
	and r4, r5
	and r4, r5
	lsl r4, r4, #5
	sub r1, #0x10
	and r0, r1
	orr r0, r4
	strb r0, [r6, #3]
	mov r9, r6
	b _08055C52
_08055C28: .4byte 0x0000047F
_08055C2C: .4byte 0x0201CF90
_08055C30:
	ldr r2, _08055D1C @ =0x0201CF90
	mov r0, #8
	neg r0, r0
	ldrb r3, [r2, #2]
	and r0, r3
	mov r1, #0x39
	neg r1, r1
	and r0, r1
	strb r0, [r2, #2]
	mov r0, #3
	neg r0, r0
	ldrb r6, [r2, #3]
	and r0, r6
	add r1, #0x34
	and r0, r1
	strb r0, [r2, #3]
	mov r9, r2
_08055C52:
	mov r4, r9
	mov r3, #1
	mov r0, sl
	and r0, r3
	mov r7, r8
	lsl r1, r7, #2
	ldr r2, _08055D20 @ =0x00000D64
	add r6, r0, #0
	mul r6, r2
	add r1, r1, r6
	ldr r0, _08055D24 @ =0x02019968
	add r1, r1, r0
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r0, r1, #0x14
	and r0, r3
	lsl r0, r0, #7
	mov r2, #0x7F
	ldrb r3, [r4, #3]
	and r2, r3
	orr r2, r0
	strb r2, [r4, #3]
	ldr r5, _08055D28 @ =0x00007FFF
	lsr r1, r1, #0x15
	ldr r0, _08055D2C @ =0xFFFF8000
	ldrh r7, [r4, #4]
	and r0, r7
	orr r0, r1
	strh r0, [r4, #4]
	mov r0, #0x1D
	neg r0, r0
	ldrb r1, [r4, #0xE]
	and r0, r1
	mov r1, #4
	orr r0, r1
	strb r0, [r4, #0xE]
	mov r0, #3
	strh r0, [r4, #0xC]
	lsr r2, r2, #7
	add r0, r5, #0
	ldrh r3, [r4, #4]
	and r0, r3
	lsl r0, r0, #1
	orr r0, r2
	ldr r7, _08055D30 @ =0x000007FF
	mov r8, r7
	mov r1, r8
	and r0, r1
	lsl r0, r0, #1
	ldr r7, _08055D34 @ =0x08622AB4
	add r0, r0, r7
	ldrh r0, [r0]
	bl IsToonMonster
	cmp r0, #0
	bne _08055CD6
	mov r0, #2
	strh r0, [r4, #0xC]
	ldr r2, _08055D24 @ =0x02019968
	ldr r3, _08055D38 @ =0xFFFFF97C
	add r1, r2, r3
	add r1, r6, r1
	mov r0, #0x10
	ldrb r6, [r1, #8]
	orr r0, r6
	strb r0, [r1, #8]
_08055CD6:
	mov r0, r9
	ldrb r0, [r0, #3]
	lsr r1, r0, #7
	mov r2, r9
	ldrh r2, [r2, #4]
	and r5, r2
	lsl r0, r5, #1
	orr r0, r1
	mov r3, r8
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r7
	ldrh r0, [r0]
	bl IsToonMonster
	cmp r0, #0
	beq _08055D04
	mov r0, #0x40
	mov r6, r9
	ldrb r6, [r6, #1]
	orr r0, r6
	mov r7, r9
	strb r0, [r7, #1]
_08055D04:
	mov r0, sl
	bl PayChainEnergyCost
	bl SummonAction_Start
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08055D1C: .4byte 0x0201CF90
_08055D20: .4byte 0x00000D64
_08055D24: .4byte 0x02019968
_08055D28: .4byte 0x00007FFF
_08055D2C: .4byte 0xFFFF8000
_08055D30: .4byte 0x000007FF
_08055D34: .4byte gCardIdToNumber
_08055D38: .4byte 0xFFFFF97C
	thumb_func_end QueueNormalSummon

