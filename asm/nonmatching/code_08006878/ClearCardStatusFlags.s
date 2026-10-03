	thumb_func_start ClearCardStatusFlags
ClearCardStatusFlags: @ 0x0800743C
	push {r4, r5, lr}
	mov r1, #0x41
	neg r1, r1
	ldrb r2, [r0, #1]
	and r1, r2
	mov r5, #0x7F
	and r1, r5
	strb r1, [r0, #1]
	mov r3, #2
	neg r3, r3
	add r1, r3, #0
	ldrb r2, [r0, #2]
	and r1, r2
	mov r2, #3
	neg r2, r2
	and r1, r2
	sub r2, #2
	and r1, r2
	mov r4, #0x11
	neg r4, r4
	and r1, r4
	sub r2, #0x1C
	and r1, r2
	and r1, r5
	strb r1, [r0, #2]
	ldrb r1, [r0, #3]
	and r3, r1
	and r3, r4
	strb r3, [r0, #3]
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end ClearCardStatusFlags

