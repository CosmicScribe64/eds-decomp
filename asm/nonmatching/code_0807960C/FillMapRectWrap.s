	thumb_func_start FillMapRectWrap
FillMapRectWrap: @ 0x08079834
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r1
	ldr r1, [sp, #0x24]
	ldr r4, [sp, #0x28]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov ip, r0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov sl, r2
	lsl r3, r3, #0x10
	lsr r0, r3, #0x10
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	str r4, [sp, #0]
	mov r2, #0
	cmp r2, r4
	bcs _080798A6
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	mov r9, r0
_0807986C:
	mov r1, #0
	add r5, r2, #1
	cmp r1, r6
	bcs _0807989C
	mov r3, sl
	lsl r0, r3, #0x10
	asr r3, r0, #0x10
	mov r4, #0x1F
	mov r7, r9
	add r0, r7, r2
	and r0, r4
	lsl r2, r0, #5
_08079884:
	add r0, r3, r1
	and r0, r4
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r8
	mov r7, ip
	strh r7, [r0]
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r1, r0, #0x10
	cmp r1, r6
	bcc _08079884
_0807989C:
	lsl r0, r5, #0x10
	lsr r2, r0, #0x10
	ldr r0, [sp, #0]
	cmp r2, r0
	bcc _0807986C
_080798A6:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end FillMapRectWrap
	.align 2, 0

