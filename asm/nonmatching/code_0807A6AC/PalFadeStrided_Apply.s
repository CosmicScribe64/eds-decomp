	thumb_func_start PalFadeStrided_Apply
PalFadeStrided_Apply: @ 0x0807B3DC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r7, r0, #0
	ldr r0, [r7, #0x34]
	str r0, [sp, #0]
	mov r1, #0
	str r1, [sp, #4]
	ldrh r0, [r7, #0x3A]
	cmp r0, #1
	bne _0807B496
	add r0, r7, #0
	add r0, #0x30
	mov r8, r0
	ldrb r1, [r0]
	cmp r1, #0x1F
	bls _0807B40E
	mov r0, #0x20
	mov r1, r8
	strb r0, [r1]
	mov r0, #2
	strh r0, [r7, #0x3A]
_0807B40E:
	mov r0, #0
	mov ip, r0
	ldrh r1, [r7, #0x32]
	cmp ip, r1
	bcs _0807B496
	mov r0, #0xF8
	lsl r0, r0, #2
	mov sl, r0
	mov r1, #0xF8
	lsl r1, r1, #7
	mov r9, r1
_0807B424:
	ldr r0, [sp, #4]
	lsl r6, r0, #1
	ldr r1, [sp, #0]
	add r6, r6, r1
	mov r1, ip
	lsl r0, r1, #1
	add r0, r7, r0
	ldrh r5, [r0]
	mov r2, #0x1F
	and r2, r5
	lsl r3, r1, #2
	add r3, r7, r3
	mov r0, #0x10
	ldsb r0, [r3, r0]
	mov r1, r8
	ldrb r4, [r1]
	mul r0, r4
	asr r0, r0, #5
	add r2, r2, r0
	mov r0, #0x1F
	and r2, r0
	mov r0, sl
	and r0, r5
	mov r1, #0x11
	ldsb r1, [r3, r1]
	mul r1, r4
	add r0, r0, r1
	mov r1, sl
	and r0, r1
	orr r2, r0
	mov r1, r9
	and r1, r5
	mov r0, #0x12
	ldsb r0, [r3, r0]
	mul r0, r4
	lsl r0, r0, #5
	add r1, r1, r0
	mov r0, r9
	and r1, r0
	orr r2, r1
	strh r2, [r6]
	add r0, r7, #0
	add r0, #0x38
	ldrb r0, [r0]
	ldr r1, [sp, #4]
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #4]
	mov r0, ip
	add r0, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov ip, r0
	ldrh r0, [r7, #0x32]
	cmp ip, r0
	bcc _0807B424
_0807B496:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end PalFadeStrided_Apply
	.align 2, 0

