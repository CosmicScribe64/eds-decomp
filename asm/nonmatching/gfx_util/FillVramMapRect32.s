	thumb_func_start FillVramMapRect32
FillVramMapRect32: @ 0x0807A754
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	ldr r5, [sp, #0x2C]
	ldr r4, [sp, #0x30]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r9, r0
	lsl r1, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r8, r2
	lsl r3, r3, #0x18
	lsl r5, r5, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov sl, r4
	lsr r1, r1, #0xD
	lsl r0, r2, #1
	mov r2, #0xC0
	lsl r2, r2, #0x13
	add r0, r0, r2
	add r1, r1, r0
	lsr r3, r3, #0x12
	add r3, r1, r3
	mov r2, #0
	cmp r2, sl
	bcs _0807A7F8
	lsr r1, r5, #0x19
	lsl r1, r1, #0x18
	lsr r0, r1, #0x18
	mov ip, r0
	mov r0, #0x10
	mov r4, ip
	sub r0, r0, r4
	lsl r0, r0, #2
	str r0, [sp, #4]
	str r1, [sp, #0]
_0807A7A6:
	mov r4, #0
	add r2, #1
	str r2, [sp, #8]
	cmp r4, ip
	bcs _0807A7EA
	mov r6, r9
	lsl r1, r6, #0x10
	orr r1, r6
	ldr r0, [sp, #0]
	lsr r5, r0, #0x18
	mov r7, #0xF8
	lsl r7, r7, #3
_0807A7BE:
	lsl r0, r4, #1
	mov r6, r8
	add r2, r6, r0
	add r0, r2, #0
	sub r0, #0x20
	cmp r0, #0x1F
	bhi _0807A7D0
	add r0, r3, r7
	b _0807A7D8
_0807A7D0:
	cmp r2, #0x3F
	ble _0807A7DE
	add r0, r3, #0
	sub r0, #0x80
_0807A7D8:
	str r1, [r0]
	add r3, #4
	b _0807A7E0
_0807A7DE:
	stmia r3!, {r1}
_0807A7E0:
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, r5
	bcc _0807A7BE
_0807A7EA:
	ldr r0, [sp, #4]
	add r3, r3, r0
	ldr r1, [sp, #8]
	lsl r0, r1, #0x18
	lsr r2, r0, #0x18
	cmp r2, sl
	bcc _0807A7A6
_0807A7F8:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end FillVramMapRect32

