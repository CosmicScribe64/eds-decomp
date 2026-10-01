	thumb_func_start sub_0807B534
sub_0807B534: @ 0x0807B534
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r0
	mov r3, #0
	mov r0, #8
	add r0, r8
	mov ip, r0
	mov r1, #0x80
	lsl r1, r1, #1
	mov r9, r1
	ldr r0, _0807B59C @ =0x03004476
	mov sl, r0
_0807B552:
	mov r2, #0
	lsl r6, r3, #1
	add r7, r3, #1
	add r0, r6, r3
	lsl r5, r0, #3
	lsl r4, r3, #2
_0807B55E:
	lsl r0, r2, #2
	add r0, r0, r5
	add r0, ip
	add r1, r4, r2
	lsl r1, r1, #3
	add r1, sl
	str r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #3
	bls _0807B55E
	add r0, r6, r3
	lsl r0, r0, #3
	add r0, r8
	mov r1, r9
	strh r1, [r0]
	strh r1, [r0, #2]
	mov r1, #0
	strh r1, [r0, #4]
	lsl r0, r7, #0x18
	lsr r3, r0, #0x18
	cmp r3, #0x1F
	bls _0807B552
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0807B59C: .4byte 0x03004476
	thumb_func_end sub_0807B534

