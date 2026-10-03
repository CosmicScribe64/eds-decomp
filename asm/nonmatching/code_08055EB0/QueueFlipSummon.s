	thumb_func_start QueueFlipSummon
QueueFlipSummon: @ 0x08055EB0
	push {r4, r5, r6, lr}
	ldr r5, _08055F5C @ =0x0201CF90
	mov r2, #1
	add r3, r0, #0
	and r3, r2
	mov r2, #2
	neg r2, r2
	ldrb r4, [r5]
	and r2, r4
	orr r2, r3
	mov r4, #0x1F
	add r3, r1, #0
	and r3, r4
	lsl r3, r3, #1
	mov r4, #0x3F
	neg r4, r4
	and r2, r4
	orr r2, r3
	strb r2, [r5]
	mov r2, #0xFF
	add r3, r1, #0
	and r3, r2
	lsl r3, r3, #6
	ldr r2, _08055F60 @ =0xFFFFC03F
	ldrh r4, [r5]
	and r2, r4
	orr r2, r3
	strh r2, [r5]
	mov r2, #0x40
	ldrb r3, [r5, #1]
	orr r2, r3
	mov r6, #0x7F
	and r2, r6
	strb r2, [r5, #1]
	mov r2, #8
	neg r2, r2
	ldrb r4, [r5, #2]
	and r2, r4
	mov r3, #0x39
	neg r3, r3
	and r2, r3
	strb r2, [r5, #2]
	add r3, #0x36
	ldrb r2, [r5, #3]
	and r3, r2
	mov r2, #5
	neg r2, r2
	and r3, r2
	strb r3, [r5, #3]
	mov r4, #1
	and r0, r4
	mov r2, #0x94
	mul r1, r2
	ldr r2, _08055F64 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08055F68 @ =0x0201930C
	add r1, r1, r0
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r0, r1, #0x14
	and r0, r4
	lsl r0, r0, #7
	and r3, r6
	orr r3, r0
	strb r3, [r5, #3]
	lsr r1, r1, #0x15
	ldr r0, _08055F6C @ =0xFFFF8000
	ldrh r3, [r5, #4]
	and r0, r3
	orr r0, r1
	strh r0, [r5, #4]
	mov r0, #0x1D
	neg r0, r0
	ldrb r4, [r5, #0xE]
	and r0, r4
	mov r1, #0xC
	orr r0, r1
	strb r0, [r5, #0xE]
	mov r0, #3
	strh r0, [r5, #0xC]
	bl SummonAction_Start
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_08055F5C: .4byte 0x0201CF90
_08055F60: .4byte 0xFFFFC03F
_08055F64: .4byte 0x00000D64
_08055F68: .4byte 0x0201930C
_08055F6C: .4byte 0xFFFF8000
	thumb_func_end QueueFlipSummon

