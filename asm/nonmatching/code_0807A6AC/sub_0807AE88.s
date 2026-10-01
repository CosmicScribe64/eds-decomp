	thumb_func_start sub_0807AE88
sub_0807AE88: @ 0x0807AE88
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r5, r0, #0
	add r4, r1, #0
	lsl r2, r2, #0x18
	lsr r6, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov ip, r3
	mov r1, #0
	cmp r1, ip
	bcs _0807AEE4
	mov r0, #0x20
	sub r0, r0, r6
	lsl r0, r0, #1
	mov r8, r0
_0807AEAC:
	mov r2, #0
	add r1, #1
	mov r9, r1
	cmp r2, r6
	bcs _0807AED8
	mov r3, #0xFF
_0807AEB8:
	add r0, r3, #0
	ldrh r1, [r5]
	and r0, r1
	add r1, r3, #0
	ldrh r7, [r5, #2]
	and r1, r7
	lsl r1, r1, #8
	orr r0, r1
	strh r0, [r4]
	add r4, #2
	add r5, #4
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, r6
	bcc _0807AEB8
_0807AED8:
	add r4, r8
	mov r1, r9
	lsl r0, r1, #0x18
	lsr r1, r0, #0x18
	cmp r1, ip
	bcc _0807AEAC
_0807AEE4:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0807AE88

