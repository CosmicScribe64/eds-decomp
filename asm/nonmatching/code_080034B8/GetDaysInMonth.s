	thumb_func_start GetDaysInMonth
GetDaysInMonth: @ 0x080042B4
	push {r4, lr}
	add r3, r0, #0
	ldr r2, _080042D4 @ =0x08198628
	sub r0, r1, #1
	add r0, r0, r2
	ldrb r4, [r0]
	cmp r1, #2
	bne _080042CC
	add r0, r3, #0
	bl IsLeapYear
	add r4, r4, r0
_080042CC:
	add r0, r4, #0
	pop {r4}
	pop {r1}
	bx r1
_080042D4: .4byte gDaysPerMonth
	thumb_func_end GetDaysInMonth

