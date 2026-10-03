	thumb_func_start GetDayOfWeek
GetDayOfWeek: @ 0x080042D8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r8, r1
	sub r4, r2, #1
	mov r2, #3
	add r3, r6, #0
	and r3, r2
	ldr r0, _08004344 @ =0x000007CF
	cmp r6, r0
	bls _080042F4
	ldr r0, _08004348 @ =0xFFFFF830
	add r6, r6, r0
_080042F4:
	lsr r1, r6, #2
	ldr r0, _0800434C @ =0x000005B5
	mul r0, r1
	add r4, r4, r0
	add r1, r4, #1
	ldr r0, _08004350 @ =0x0000016D
	mul r0, r3
	add r4, r1, r0
	add r0, r6, #0
	and r0, r2
	cmp r0, #0
	bne _0800430E
	sub r4, #1
_0800430E:
	mov r5, #1
	cmp r5, r8
	bcs _08004332
	ldr r7, _08004354 @ =0x08198628
_08004316:
	ldrb r1, [r7]
	add r4, r1, r4
	cmp r5, #2
	bne _0800432A
	mov r1, #0xFA
	lsl r1, r1, #3
	add r0, r6, r1
	bl IsLeapYear
	add r4, r4, r0
_0800432A:
	add r7, #1
	add r5, #1
	cmp r5, r8
	bcc _08004316
_08004332:
	add r0, r4, #6
	mov r1, #7
	bl __modsi3
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08004344: .4byte 0x000007CF
_08004348: .4byte 0xFFFFF830
_0800434C: .4byte 0x000005B5
_08004350: .4byte 0x0000016D
_08004354: .4byte gDaysPerMonth
	thumb_func_end GetDayOfWeek

