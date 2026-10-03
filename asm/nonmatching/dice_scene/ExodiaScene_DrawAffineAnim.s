	thumb_func_start ExodiaScene_DrawAffineAnim
ExodiaScene_DrawAffineAnim: @ 0x08026220
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	mov r8, r2
	lsl r1, r1, #0x18
	lsr r0, r1, #0x18
	ldr r4, [r6, #4]
	mov r5, #0
	ldrb r1, [r6, #0xC]
	cmp r5, r1
	bcs _080262A8
	lsl r0, r0, #0xA
	mov r2, #0xC0
	lsl r2, r2, #4
	add r1, r2, #0
	and r0, r1
	mov r3, #0x80
	lsl r3, r3, #2
	add r1, r3, #0
	add r0, r0, r1
	mov r9, r0
_0802624E:
	mov r0, #0
	mov r1, r8
	bl OamListAlloc
	ldrh r3, [r4]
	mov r7, #0x80
	lsl r7, r7, #1
	add r1, r7, #0
	add r2, r3, #0
	orr r2, r1
	mov r7, #0xFF
	lsl r7, r7, #8
	add r1, r7, #0
	and r2, r1
	mov r1, #0xFF
	and r1, r3
	orr r2, r1
	strh r2, [r0]
	ldr r2, _080262B4 @ =0x0000C1FF
	add r1, r2, #0
	ldrh r3, [r4, #2]
	and r1, r3
	strh r1, [r0, #2]
	ldrh r3, [r4, #4]
	add r7, #0xF
	add r1, r7, #0
	add r2, r3, #0
	and r2, r1
	mov r1, #0xF0
	and r1, r3
	lsl r1, r1, #1
	orr r2, r1
	ldr r3, _080262B8 @ =0xFFFFF3FF
	add r1, r3, #0
	and r2, r1
	mov r7, r9
	orr r2, r7
	strh r2, [r0, #4]
	add r4, #8
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	ldrb r0, [r6, #0xC]
	cmp r5, r0
	bcc _0802624E
_080262A8:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080262B4: .4byte 0x0000C1FF
_080262B8: .4byte 0xFFFFF3FF
	thumb_func_end ExodiaScene_DrawAffineAnim

