	thumb_func_start CardTrading_ThrowCard
CardTrading_ThrowCard: @ 0x0807D118
	push {r4, lr}
	ldr r4, _0807D15C @ =0x0201F780
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	ldrb r3, [r4, #3]
	lsl r2, r3, #0x1B
	lsr r2, r2, #0x1C
	bl CardTrading_DrawMenu
	ldrh r2, [r4, #2]
	mov r0, #0xFE
	lsl r0, r0, #1
	and r0, r2
	mov r1, #0xC0
	lsl r1, r1, #1
	cmp r0, r1
	beq _0807D164
	lsl r1, r2, #0x17
	lsr r0, r1, #0x19
	cmp r0, #0x5B
	bhi _0807D1A4
	add r0, #4
	mov r1, #0x7F
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0807D160 @ =0xFFFFFE03
	and r1, r2
	orr r1, r0
	strh r1, [r4, #2]
	b _0807D1A4
	.align 2, 0
_0807D15C: .4byte 0x0201F780
_0807D160: .4byte 0xFFFFFE03
_0807D164:
	ldrb r2, [r4, #3]
	lsl r1, r2, #0x1B
	lsr r0, r1, #0x1C
	cmp r0, #0xE
	bls _0807D190
	add r0, r4, #0
	add r0, #0x10
	bl LinkSyncStart
	ldr r0, _0807D18C @ =0x08087FA0
	bl DebugPrintf
	bl DebugPrintFlush
	mov r0, #0x80
	lsl r0, r0, #2
	strh r0, [r4, #0x1E]
	mov r0, #1
	b _0807D1A6
	.align 2, 0
_0807D18C: .4byte gStrDebugThrowItInNow
_0807D190:
	lsr r0, r1, #0x1C
	add r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r0, r0, #1
	mov r1, #0x1F
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r4, #3]
_0807D1A4:
	mov r0, #0
_0807D1A6:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end CardTrading_ThrowCard

