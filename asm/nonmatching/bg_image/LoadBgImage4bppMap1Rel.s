	thumb_func_start LoadBgImage4bppMap1Rel
LoadBgImage4bppMap1Rel: @ 0x080733F4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov sl, r0
	lsl r7, r1, #0x10
	lsr r0, r7, #0x10
	lsl r2, r2, #0x10
	lsr r6, r2, #0x10
	ldrh r2, [r3]
	lsl r1, r2, #1
	add r2, r1, #0
	add r2, #8
	add r2, r2, r3
	mov r9, r2
	add r1, #0x10
	add r1, r3, r1
	ldrh r4, [r2]
	lsl r2, r4, #5
	add r5, r1, r2
	add r4, r5, #0
	add r4, #8
	ldrh r1, [r5, #8]
	mov r8, r1
	add r1, r6, #0
	add r2, r3, #0
	bl LoadBgImage4bppGfx
	mov r3, #0
	ldrh r2, [r5]
	cmp r3, r2
	bcs _08073480
	ldr r0, _08073494 @ =0x03000C5C
	mov ip, r0
	lsr r0, r7, #0x14
	lsl r0, r0, #0xC
	str r0, [sp, #0]
_08073446:
	ldrh r1, [r4]
	mov r2, r8
	sub r0, r1, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r4, #2
	ldrh r2, [r4]
	add r4, #2
	mov r1, #0x3F
	and r1, r0
	mov r7, #0xFF
	lsl r7, r7, #8
	and r0, r7
	lsr r0, r0, #3
	orr r1, r0
	add r1, sl
	lsl r1, r1, #0x10
	lsr r1, r1, #0xF
	add r1, ip
	add r2, r2, r6
	ldr r0, [sp, #0]
	orr r2, r0
	strh r2, [r1]
	add r0, r3, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldrh r1, [r5]
	cmp r3, r1
	bcc _08073446
_08073480:
	mov r2, r9
	ldrh r0, [r2]
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08073494: .4byte 0x03000C5C
	thumb_func_end LoadBgImage4bppMap1Rel

