	thumb_func_start LinkQueueMessage
LinkQueueMessage: @ 0x080723B4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #4
	mov r8, r0
	add r1, #1
	lsr r6, r1, #1
	ldr r1, _08072418 @ =0x030049D0
	mov r0, #0xC0
	lsl r0, r0, #2
	add r3, r1, r0
	add r5, r1, #0
	ldrh r2, [r3]
	cmp r2, #0x3F
	bls _080723D8
	b _080724FC
_080723D8:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #2
	cmp r6, #5
	bhi _08072444
	add r0, r0, r5
	lsl r1, r6, #0x18
	lsr r1, r1, #0x18
	mov r4, #0x90
	lsl r4, r4, #8
	add r2, r4, #0
	orr r1, r2
	strh r1, [r0]
	mov r2, #0
	add r4, r5, #0
_080723F6:
	cmp r2, r6
	bge _0807241C
	add r2, #1
	lsl r1, r2, #1
	ldrh r7, [r3]
	lsl r0, r7, #1
	add r0, r0, r7
	lsl r0, r0, #2
	add r1, r1, r0
	add r1, r1, r4
	mov r7, r8
	ldrh r0, [r7]
	strh r0, [r1]
	mov r0, #2
	add r8, r0
	b _08072430
	.align 2, 0
_08072418: .4byte 0x030049D0
_0807241C:
	add r2, #1
	lsl r1, r2, #1
	ldrh r7, [r3]
	lsl r0, r7, #1
	add r0, r0, r7
	lsl r0, r0, #2
	add r1, r1, r0
	add r1, r1, r4
	mov r0, #0
	strh r0, [r1]
_08072430:
	cmp r2, #4
	bls _080723F6
	mov r0, #0xC0
	lsl r0, r0, #2
	add r1, r5, r0
	ldrh r0, [r1]
	add r0, #1
	strh r0, [r1]
	mov r0, #1
	b _080724FE
_08072444:
	lsl r0, r6, #0x18
	mov sl, r1
	add r7, r3, #0
	lsr r0, r0, #0x18
	mov r9, r0
	mov r1, #0xB0
	lsl r1, r1, #8
	add r0, r1, #0
	mov r3, #0
	mov r2, sp
	strh r3, [r2]
	mov r4, r9
	orr r4, r0
	mov r9, r4
_08072460:
	cmp r6, #5
	bhi _080724A8
	ldrh r1, [r7]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, sl
	mov r2, r9
	strh r2, [r0]
	mov r2, #0
	ldr r6, _080724A4 @ =0x030049D0
	mov r3, #0xC0
	lsl r3, r3, #2
	add r5, r6, r3
	mov r3, r8
	mov r4, #2
_08072480:
	ldrh r1, [r5]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r4, r0
	add r0, r0, r6
	ldrh r1, [r3]
	strh r1, [r0]
	add r3, #2
	add r4, #2
	add r2, #1
	cmp r2, #4
	bls _08072480
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	mov r0, #1
	b _080724FE
_080724A4: .4byte 0x030049D0
_080724A8:
	sub r6, #5
	ldrh r2, [r7]
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, sl
	lsl r0, r6, #0x18
	lsr r0, r0, #0x18
	mov r3, #0xA0
	lsl r3, r3, #8
	add r2, r3, #0
	orr r0, r2
	strh r0, [r1]
	mov r2, #0
	ldr r4, _080724F8 @ =0x030049D0
	mov ip, r4
	mov r5, #0xC0
	lsl r5, r5, #2
	add r5, ip
	lsl r0, r6, #1
	mov r1, r8
	add r3, r0, r1
	mov r4, #2
_080724D6:
	ldrh r1, [r5]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #2
	add r0, r4, r0
	add r0, ip
	ldrh r1, [r3]
	strh r1, [r0]
	add r3, #2
	add r4, #2
	add r2, #1
	cmp r2, #4
	bls _080724D6
	ldrh r0, [r7]
	add r0, #1
	strh r0, [r7]
	b _08072460
_080724F8: .4byte 0x030049D0
_080724FC:
	mov r0, #0
_080724FE:
	add sp, #4
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end LinkQueueMessage
	.align 2, 0

