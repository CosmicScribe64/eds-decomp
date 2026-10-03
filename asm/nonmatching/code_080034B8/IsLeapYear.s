	thumb_func_start IsLeapYear
IsLeapYear: @ 0x08004280
	push {r4, lr}
	add r4, r0, #0
	mov r0, #3
	and r0, r4
	cmp r0, #0
	bne _080042A6
	add r0, r4, #0
	mov r1, #0x64
	bl __umodsi3
	cmp r0, #0
	bne _080042AA
	mov r1, #0xC8
	lsl r1, r1, #1
	add r0, r4, #0
	bl __umodsi3
	cmp r0, #0
	beq _080042AA
_080042A6:
	mov r0, #0
	b _080042AC
_080042AA:
	mov r0, #1
_080042AC:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end IsLeapYear
	.align 2, 0

