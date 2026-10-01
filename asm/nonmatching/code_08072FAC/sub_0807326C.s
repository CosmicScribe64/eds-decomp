	thumb_func_start sub_0807326C
sub_0807326C: @ 0x0807326C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	add r5, r3, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
	lsl r1, r1, #0x10
	mov r9, r1
	lsr r4, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	ldrh r0, [r5]
	lsl r1, r0, #1
	add r0, r1, #0
	add r0, #8
	add r0, r0, r5
	mov sl, r0
	add r1, #0x10
	add r1, r5, r1
	lsl r0, r2, #5
	ldr r2, _08073324 @ =0x06004000
	add r0, r0, r2
	mov r3, sl
	ldrh r3, [r3]
	lsl r2, r3, #5
	add r7, r1, r2
	add r6, r7, #0
	add r6, #8
	bl sub_08075294
	lsl r4, r4, #1
	mov r0, #0xA0
	lsl r0, r0, #0x13
	add r4, r4, r0
	add r1, r5, #0
	add r1, #8
	ldrh r5, [r5]
	lsl r2, r5, #1
	add r0, r4, #0
	bl sub_08075294
	mov r3, #0
	ldrh r5, [r7]
	cmp r3, r5
	bcs _0807330E
	mov r0, #0xFF
	lsl r0, r0, #8
	mov ip, r0
	mov r1, r9
	lsr r0, r1, #0x14
	lsl r4, r0, #0xC
_080732DC:
	ldrh r1, [r6]
	add r6, #2
	ldrh r2, [r6]
	add r6, #2
	mov r0, #0x3F
	and r0, r1
	mov r5, ip
	and r1, r5
	lsr r1, r1, #3
	orr r0, r1
	ldr r1, [sp, #0]
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	ldr r5, _08073328 @ =0x0300045C
	add r0, r0, r5
	add r2, r8
	orr r2, r4
	strh r2, [r0]
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldrh r0, [r7]
	cmp r3, r0
	bcc _080732DC
_0807330E:
	mov r1, sl
	ldrh r0, [r1]
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08073324: .4byte 0x06004000
_08073328: .4byte 0x0300045C
	thumb_func_end sub_0807326C

