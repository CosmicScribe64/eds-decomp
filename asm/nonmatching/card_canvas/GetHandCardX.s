	thumb_func_start GetHandCardX
GetHandCardX: @ 0x0806236C
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r5, r1, #0
	ldr r0, _0806239C @ =0x081A42A4
	lsl r1, r6, #7
	add r0, #0x58
	add r1, r1, r0
	ldr r4, [r1]
	cmp r2, #0
	beq _080623A2
	lsl r3, r5, #5
	cmp r2, #5
	ble _08062394
	lsl r0, r5, #7
	add r3, r0, r3
	add r0, r3, #0
	add r1, r2, #0
	bl __divsi3
	add r3, r0, #0
_08062394:
	cmp r6, #0
	bne _080623A0
	add r4, r4, r3
	b _080623A2
_0806239C: .4byte gDuelZonePositions
_080623A0:
	sub r4, r4, r3
_080623A2:
	add r0, r4, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end GetHandCardX
	.align 2, 0

