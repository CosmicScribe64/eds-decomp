	thumb_func_start FillVramMapRectSeq
FillVramMapRectSeq: @ 0x0807A6AC
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
	lsr r7, r4, #0x18
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
	add r3, r1, r3
	mov r1, #0
	cmp r1, r8
	bcs _0807A748
	mov r0, #0x20
	sub r0, r0, r7
	lsl r0, r0, #1
	mov r9, r0
_0807A6EE:
	mov r4, #0
	add r5, r1, #1
	cmp r4, r7
	bcs _0807A73E
_0807A6F6:
	mov r0, ip
	add r1, r0, r4
	add r0, r1, #0
	sub r0, #0x20
	cmp r0, #0x1F
	bhi _0807A714
	mov r0, #0xF8
	lsl r0, r0, #3
	add r2, r3, r0
	add r1, r6, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	strh r1, [r2]
	b _0807A732
_0807A714:
	cmp r1, #0x3F
	ble _0807A728
	add r2, r3, #0
	sub r2, #0x80
	add r1, r6, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	strh r1, [r2]
	b _0807A732
_0807A728:
	add r1, r6, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	strh r1, [r3]
_0807A732:
	add r3, #2
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, r7
	bcc _0807A6F6
_0807A73E:
	add r3, r9
	lsl r0, r5, #0x18
	lsr r1, r0, #0x18
	cmp r1, r8
	bcc _0807A6EE
_0807A748:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end FillVramMapRectSeq

