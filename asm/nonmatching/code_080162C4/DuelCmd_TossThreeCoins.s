	thumb_func_start DuelCmd_TossThreeCoins
DuelCmd_TossThreeCoins: @ 0x0801715C
	push {r4, r5, lr}
	ldr r4, _08017174 @ =0x020185C0
	ldr r0, _08017178 @ =0x0000080A
	add r5, r4, r0
	ldrb r1, [r5]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x19
	cmp r0, #0
	beq _0801717C
	cmp r0, #1
	beq _080171B0
	b _080171C6
_08017174: .4byte 0x020185C0
_08017178: .4byte 0x0000080A
_0801717C:
	mov r0, #0
	mov r1, #0
	bl DuelScene_Start
	ldr r1, _080171AC @ =0x02017A30
	mov r2, #0xE0
	lsl r2, r2, #2
	add r0, r2, #0
	ldrb r4, [r4, #2]
	orr r0, r4
	strh r0, [r1, #6]
	ldrb r2, [r5]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	mov r0, #0x80
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5]
	b _080171C6
	.align 2, 0
_080171AC: .4byte 0x02017A30
_080171B0:
	bl DuelScene_Run
	cmp r0, #0
	beq _080171C6
	ldr r0, _080171CC @ =0x0000080D
	add r1, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_080171C6:
	pop {r4, r5}
	pop {r0}
	bx r0
_080171CC: .4byte 0x0000080D
	thumb_func_end DuelCmd_TossThreeCoins

