	thumb_func_start sub_0807A898
sub_0807A898: @ 0x0807A898
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r4, r0, #0
	ldr r0, [sp, #0x1C]
	lsl r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r7, r3, #0x18
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	mov ip, r0
	lsl r1, r1, #0x16
	lsr r6, r1, #0x16
	mov r0, #0xF0
	lsl r0, r0, #0x14
	and r0, r2
	lsr r0, r0, #0x18
	mov r9, r0
	mov r1, #0
	cmp r1, ip
	bcs _0807A8FA
	mov r0, #0x20
	sub r0, r0, r7
	lsl r0, r0, #1
	mov r8, r0
_0807A8CC:
	mov r2, #0
	add r5, r1, #1
	cmp r2, r7
	bcs _0807A8F0
	mov r0, r9
	lsl r3, r0, #0xC
_0807A8D8:
	add r0, r6, #0
	add r1, r0, #1
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	orr r0, r3
	strh r0, [r4]
	add r4, #2
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, r7
	bcc _0807A8D8
_0807A8F0:
	add r4, r8
	lsl r0, r5, #0x18
	lsr r1, r0, #0x18
	cmp r1, ip
	bcc _0807A8CC
_0807A8FA:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_0807A898
	.align 2, 0

