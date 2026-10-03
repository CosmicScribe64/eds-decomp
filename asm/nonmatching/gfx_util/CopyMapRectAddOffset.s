	thumb_func_start CopyMapRectAddOffset
CopyMapRectAddOffset: @ 0x0807A9C0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	add r6, r0, #0
	add r5, r1, #0
	ldr r0, [sp, #0x28]
	ldr r1, [sp, #0x2C]
	ldr r4, [sp, #0x30]
	lsl r2, r2, #0x18
	lsr r7, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov ip, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov sl, r4
	mov r2, #0
	cmp r2, ip
	bcs _0807AA3A
	sub r0, r0, r7
	lsl r0, r0, #1
	mov r9, r0
	mov r0, #0x20
	sub r0, r0, r7
	lsl r0, r0, #1
	mov r8, r0
_0807AA04:
	mov r1, #0
	add r2, #1
	str r2, [sp, #4]
	cmp r1, r7
	bcs _0807AA2C
	ldr r0, [sp, #0]
	lsl r3, r0, #0xC
	mov r4, sl
	lsl r2, r4, #8
_0807AA16:
	ldrh r4, [r6]
	add r0, r4, r3
	add r0, r0, r2
	strh r0, [r5]
	add r6, #2
	add r5, #2
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, r7
	bcc _0807AA16
_0807AA2C:
	add r6, r9
	add r5, r8
	ldr r1, [sp, #4]
	lsl r0, r1, #0x18
	lsr r2, r0, #0x18
	cmp r2, ip
	bcc _0807AA04
_0807AA3A:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end CopyMapRectAddOffset
	.align 2, 0

