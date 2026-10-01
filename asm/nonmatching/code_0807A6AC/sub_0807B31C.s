	thumb_func_start sub_0807B31C
sub_0807B31C: @ 0x0807B31C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	mov r8, r0
	ldr r5, [sp, #0x28]
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r9, r2
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r0, #0
	mov ip, r0
	mov r1, r8
	str r1, [r5, #0x34]
	mov r4, #0
	cmp r4, r7
	bcs _0807B3B6
	mov r0, #0x1F
	add r6, r3, #0
	and r6, r0
	add r0, r3, #0
	mov r1, #0xF8
	lsl r1, r1, #2
	and r0, r1
	lsr r0, r0, #5
	str r0, [sp, #0]
	mov r0, #0xF8
	lsl r0, r0, #7
	mov sl, r0
	add r0, r3, #0
	mov r1, sl
	and r0, r1
	lsr r0, r0, #0xA
	str r0, [sp, #4]
_0807B36A:
	lsl r3, r4, #1
	add r3, r5, r3
	mov r1, ip
	lsl r0, r1, #1
	add r0, r8
	ldrh r1, [r0]
	strh r1, [r3]
	lsl r2, r4, #2
	add r2, r5, r2
	mov r0, #0x1F
	and r0, r1
	sub r0, r6, r0
	strb r0, [r2, #0x10]
	mov r0, #0xF8
	lsl r0, r0, #2
	ldrh r1, [r3]
	and r0, r1
	lsr r0, r0, #5
	ldr r1, [sp, #0]
	sub r0, r1, r0
	strb r0, [r2, #0x11]
	mov r0, sl
	ldrh r3, [r3]
	and r0, r3
	lsr r0, r0, #0xA
	ldr r1, [sp, #4]
	sub r0, r1, r0
	strb r0, [r2, #0x12]
	mov r0, ip
	add r0, r9
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov ip, r0
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, r7
	bcc _0807B36A
_0807B3B6:
	mov r1, #0
	mov r0, #1
	strh r0, [r5, #0x3A]
	strh r7, [r5, #0x32]
	add r0, r5, #0
	add r0, #0x30
	strb r1, [r0]
	add r0, #8
	mov r1, r9
	strb r1, [r0]
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0807B31C
	.align 2, 0

