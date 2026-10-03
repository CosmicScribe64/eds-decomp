	thumb_func_start LoadBgImage4bppMap4Rel
LoadBgImage4bppMap4Rel: @ 0x080730A8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r5, r3, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	str r0, [sp, #0]
	lsl r1, r1, #0x10
	mov sl, r1
	lsr r4, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	ldrh r0, [r5]
	lsl r1, r0, #1
	add r0, r1, #0
	add r0, #8
	add r0, r5, r0
	str r0, [sp, #4]
	add r1, #0x10
	add r1, r5, r1
	lsl r0, r2, #5
	ldr r2, _0807317C @ =0x06004000
	add r0, r0, r2
	ldr r3, [sp, #4]
	ldrh r3, [r3]
	lsl r2, r3, #5
	add r7, r1, r2
	add r6, r7, #0
	add r6, #8
	mov r3, #0
	str r3, [sp, #8]
	mov r9, r3
	bl MemCopy16
	lsl r4, r4, #1
	mov r0, #0xA0
	lsl r0, r0, #0x13
	add r4, r4, r0
	add r1, r5, #0
	add r1, #8
	ldrh r5, [r5]
	lsl r2, r5, #1
	add r0, r4, #0
	bl MemCopy16
	mov r2, #0
	ldrh r5, [r7]
	cmp r2, r5
	bcs _08073168
	mov r0, #0xFF
	lsl r0, r0, #8
	mov ip, r0
	mov r1, sl
	lsr r0, r1, #0x14
	lsl r4, r0, #0xC
_0807311E:
	ldrh r0, [r6]
	add r6, #2
	ldrh r3, [r6]
	add r6, #2
	mov r1, #0x3F
	and r1, r0
	mov r5, ip
	and r0, r5
	lsr r0, r0, #8
	cmp r2, #0
	bne _08073138
	str r1, [sp, #8]
	mov r9, r0
_08073138:
	ldr r5, [sp, #8]
	sub r1, r1, r5
	lsl r1, r1, #0x10
	mov r5, r9
	sub r0, r0, r5
	lsl r0, r0, #0x15
	orr r0, r1
	lsr r0, r0, #0x10
	ldr r1, [sp, #0]
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	ldr r5, _08073180 @ =0x0300245C
	add r0, r0, r5
	mov r5, r8
	add r1, r3, r5
	orr r1, r4
	strh r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	ldrh r0, [r7]
	cmp r2, r0
	bcc _0807311E
_08073168:
	ldr r1, [sp, #4]
	ldrh r0, [r1]
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0807317C: .4byte 0x06004000
_08073180: .4byte 0x0300245C
	thumb_func_end LoadBgImage4bppMap4Rel

