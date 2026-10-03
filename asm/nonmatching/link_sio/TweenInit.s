	thumb_func_start TweenInit
TweenInit: @ 0x0807B9D4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	ldr r4, [sp, #0x24]
	ldr r5, [sp, #0x28]
	ldr r7, [sp, #0x2C]
	ldr r6, [sp, #0x30]
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r9, r1
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov sl, r2
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #0]
	lsl r4, r4, #0x10
	lsr r1, r4, #0x10
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r6, r6, #0x18
	lsr r6, r6, #0x18
	cmp r6, #1
	beq _0807BA56
	cmp r6, #1
	bgt _0807BA1A
	cmp r6, #0
	beq _0807BA3A
	b _0807BAA0
_0807BA1A:
	cmp r6, #3
	bgt _0807BAA0
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	mov r0, #0x80
	lsl r0, r0, #7
	bl __divsi3
	mov r1, #0
	strh r0, [r7, #0xC]
	strh r1, [r7, #0xE]
	mov r0, #1
	strb r0, [r7, #0x14]
	mov r0, sl
	lsl r1, r0, #0x10
	b _0807BA6E
_0807BA3A:
	mov r2, r8
	strh r2, [r7]
	mov r0, r9
	strh r0, [r7, #2]
	mov r2, sl
	strh r2, [r7, #8]
	mov r0, sp
	ldrh r0, [r0]
	strh r0, [r7, #0xA]
	strh r1, [r7, #0xC]
	strh r5, [r7, #0xE]
	mov r0, #1
	strb r0, [r7, #0x14]
	b _0807BAA0
_0807BA56:
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	add r0, r1, #0
	add r0, #0x7F
	bl __divsi3
	mov r1, #0
	strh r0, [r7, #0xC]
	strh r1, [r7, #0xE]
	strb r6, [r7, #0x14]
	mov r2, sl
	lsl r1, r2, #0x10
_0807BA6E:
	asr r1, r1, #0x10
	mov r2, r8
	lsl r0, r2, #0x10
	asr r0, r0, #0x10
	sub r1, r1, r0
	strh r1, [r7, #0x10]
	ldr r0, [sp, #0]
	lsl r1, r0, #0x10
	asr r1, r1, #0x10
	mov r2, r9
	lsl r0, r2, #0x10
	asr r0, r0, #0x10
	sub r1, r1, r0
	strh r1, [r7, #0x12]
	mov r0, r8
	strh r0, [r7]
	strh r2, [r7, #2]
	strh r0, [r7, #4]
	mov r1, r9
	strh r1, [r7, #6]
	mov r2, sl
	strh r2, [r7, #8]
	mov r0, sp
	ldrh r0, [r0]
	strh r0, [r7, #0xA]
_0807BAA0:
	strb r6, [r7, #0x15]
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end TweenInit
	.align 2, 0

