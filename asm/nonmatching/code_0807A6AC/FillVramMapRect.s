	thumb_func_start FillVramMapRect
FillVramMapRect: @ 0x0807A808
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r4, [sp, #0x1C]
	ldr r5, [sp, #0x20]
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	lsl r1, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov ip, r2
	lsl r3, r3, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	mov r8, r5
	lsr r1, r1, #0xD
	lsl r0, r2, #1
	mov r2, #0xC0
	lsl r2, r2, #0x13
	add r0, r0, r2
	add r1, r1, r0
	lsr r3, r3, #0x12
	add r1, r1, r3
	mov r2, #0
	cmp r2, r8
	bcs _0807A88C
	mov r0, #0x20
	sub r0, r0, r4
	lsl r0, r0, #1
	mov r9, r0
_0807A84A:
	mov r3, #0
	add r5, r2, #1
	cmp r3, r4
	bcs _0807A882
	mov r7, #0xF8
	lsl r7, r7, #3
_0807A856:
	mov r0, ip
	add r2, r0, r3
	add r0, r2, #0
	sub r0, #0x20
	cmp r0, #0x1F
	bhi _0807A868
	add r0, r1, r7
	strh r6, [r0]
	b _0807A876
_0807A868:
	cmp r2, #0x3F
	ble _0807A874
	add r0, r1, #0
	sub r0, #0x80
	strh r6, [r0]
	b _0807A876
_0807A874:
	strh r6, [r1]
_0807A876:
	add r1, #2
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, r4
	bcc _0807A856
_0807A882:
	add r1, r9
	lsl r0, r5, #0x18
	lsr r2, r0, #0x18
	cmp r2, r8
	bcc _0807A84A
_0807A88C:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end FillVramMapRect

