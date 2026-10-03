	thumb_func_start SetOamMatrixPacked
SetOamMatrixPacked: @ 0x0807609C
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	ldr r2, _08076108 @ =0x03004470
	mov r8, r2
	ldr r4, _0807610C @ =0x081A77A8
	mov r3, #0x7F
	add r2, r1, #0
	and r2, r3
	lsl r2, r2, #1
	add r2, r2, r4
	ldrh r7, [r2]
	add r2, r1, #0
	add r2, #0x20
	and r2, r3
	lsl r2, r2, #1
	add r2, r2, r4
	ldrh r5, [r2]
	add r2, r1, #0
	add r2, #0x40
	and r2, r3
	lsl r2, r2, #1
	add r2, r2, r4
	ldrh r6, [r2]
	lsr r0, r0, #0xB
	add r8, r0
	lsr r1, r1, #0xC
	cmp r1, #7
	bgt _08076110
	lsl r0, r7, #0x10
	asr r0, r0, #0x10
	add r4, r1, #1
	add r1, r4, #0
	bl __divsi3
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r0, r5, #0x10
	asr r0, r0, #0x10
	add r1, r4, #0
	bl __divsi3
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r0, r6, #0x10
	asr r0, r0, #0x10
	add r1, r4, #0
	bl __divsi3
	b _0807612C
	.align 2, 0
_08076108: .4byte 0x03004470
_0807610C: .4byte gSineTable128
_08076110:
	lsl r0, r7, #0x10
	asr r0, r0, #0x10
	sub r1, #8
	mul r0, r1
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r0, r5, #0x10
	asr r0, r0, #0x10
	mul r0, r1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r0, r6, #0x10
	asr r0, r0, #0x10
	mul r0, r1
_0807612C:
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	mov r0, r8
	strh r5, [r0, #6]
	strh r7, [r0, #0xE]
	strh r6, [r0, #0x16]
	strh r5, [r0, #0x1E]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end SetOamMatrixPacked

