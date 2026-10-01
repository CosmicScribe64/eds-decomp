	thumb_func_start sub_0807AA4C
sub_0807AA4C: @ 0x0807AA4C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r6, r0, #0
	add r5, r1, #0
	ldr r0, [sp, #0x2C]
	ldr r1, [sp, #0x30]
	ldr r4, [sp, #0x34]
	lsl r2, r2, #0x18
	lsr r7, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r8, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #0]
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r4, [sp, #4]
	mov r2, #0
	cmp r2, r8
	bcs _0807AACE
	sub r0, r0, r7
	lsl r0, r0, #1
	mov sl, r0
	mov r0, #0x20
	sub r0, r0, r7
	lsl r0, r0, #1
	mov r9, r0
_0807AA90:
	mov r1, #0
	add r2, #1
	str r2, [sp, #8]
	cmp r1, r7
	bcs _0807AAC0
	ldr r0, _0807AAE0 @ =0x000003FF
	mov ip, r0
	ldr r4, [sp, #0]
	lsl r3, r4, #0xC
	ldr r0, [sp, #4]
	lsl r2, r0, #8
_0807AAA6:
	mov r0, ip
	ldrh r4, [r6]
	and r0, r4
	orr r0, r3
	orr r0, r2
	strh r0, [r5]
	add r6, #2
	add r5, #2
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, r7
	bcc _0807AAA6
_0807AAC0:
	add r6, sl
	add r5, r9
	ldr r1, [sp, #8]
	lsl r0, r1, #0x18
	lsr r2, r0, #0x18
	cmp r2, r8
	bcc _0807AA90
_0807AACE:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0807AAE0: .4byte 0x000003FF
	thumb_func_end sub_0807AA4C

