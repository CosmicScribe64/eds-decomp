	thumb_func_start ClearMapRect
ClearMapRect: @ 0x0807A528
	push {r4, r5, r6, lr}
	add r3, r0, #0
	lsl r1, r1, #0x18
	lsr r4, r1, #0x18
	lsl r2, r2, #0x18
	lsr r5, r2, #0x18
	mov r1, #0
	cmp r1, r5
	bcs _0807A562
	mov r0, #0x20
	sub r0, r0, r4
	lsl r6, r0, #1
_0807A540:
	mov r0, #0
	add r2, r1, #1
	cmp r0, r4
	bcs _0807A558
	mov r1, #0
_0807A54A:
	strh r1, [r3]
	add r3, #2
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, r4
	bcc _0807A54A
_0807A558:
	add r3, r3, r6
	lsl r0, r2, #0x18
	lsr r1, r0, #0x18
	cmp r1, r5
	bcc _0807A540
_0807A562:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end ClearMapRect

