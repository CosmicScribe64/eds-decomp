	thumb_func_start ApplyPumpkingBoost
ApplyPumpkingBoost: @ 0x08046738
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r6, r1, #0
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r6, #0
	mul r2, r0
	ldr r0, _080467A8 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _080467AC @ =0x0201930C
	add r4, r2, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080467A0
	mov r0, #2
	ldrb r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _080467A0
	mov r0, #0
	mov r1, #0x52
	bl CountActiveCardsOnField2
	cmp r0, #0
	bgt _0804677C
	mov r0, #1
	mov r1, #0x52
	bl CountActiveCardsOnField2
	cmp r0, #0
	ble _080467A0
_0804677C:
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r5, #0
	bl sub_080197C0
	ldr r1, [r4]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	lsl r2, r5, #0x18
	lsl r0, r6, #0x18
	lsr r2, r2, #8
	orr r2, r0
	lsr r2, r2, #0x10
	add r0, r5, #0
	mov r3, #0xD
	bl QueueAddZoneLink
_080467A0:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080467A8: .4byte 0x00000D64
_080467AC: .4byte 0x0201930C
	thumb_func_end ApplyPumpkingBoost

