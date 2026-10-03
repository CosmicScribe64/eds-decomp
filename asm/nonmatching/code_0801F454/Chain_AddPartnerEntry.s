	thumb_func_start Chain_AddPartnerEntry
Chain_AddPartnerEntry: @ 0x0801FBF4
	push {r4, r5, r6, lr}
	add r5, r1, #0
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r6, #0
	beq _0801FC20
	ldr r2, _0801FC1C @ =0x02017A40
	mov r1, #0xF0
	lsl r1, r1, #2
	add r0, r2, r1
	ldrh r3, [r0]
	lsl r1, r3, #2
	add r1, r1, r3
	lsl r1, r1, #2
	mov r3, #0xA0
	lsl r3, r3, #2
	add r0, r2, r3
	add r4, r1, r0
	b _0801FC32
	.align 2, 0
_0801FC1C: .4byte 0x02017A40
_0801FC20:
	ldr r2, _0801FC8C @ =0x02017A40
	mov r0, #0xF1
	lsl r0, r0, #2
	add r1, r2, r0
	ldrh r3, [r1]
	lsl r0, r3, #2
	add r0, r0, r3
	lsl r0, r0, #2
	add r4, r0, r2
_0801FC32:
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #0x14
	bl MemCopy16
	mov r2, #1
	ldrb r0, [r4, #2]
	orr r0, r2
	strb r0, [r4, #2]
	ldrb r0, [r4, #4]
	orr r0, r2
	mov r1, #2
	orr r0, r1
	mov r1, #5
	neg r1, r1
	and r0, r1
	sub r1, #4
	and r0, r1
	sub r1, #8
	and r0, r1
	strb r0, [r4, #4]
	ldrh r1, [r5, #6]
	sub r0, r2, r1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsr r1, r1, #8
	lsl r1, r1, #8
	orr r0, r1
	strh r0, [r4, #6]
	ldrh r0, [r5, #8]
	sub r2, r2, r0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsr r0, r0, #8
	lsl r0, r0, #8
	orr r2, r0
	strh r2, [r4, #8]
	cmp r6, #0
	beq _0801FC90
	ldr r0, _0801FC8C @ =0x02017A40
	mov r1, #0xF0
	lsl r1, r1, #2
	add r0, r0, r1
	b _0801FC98
	.align 2, 0
_0801FC8C: .4byte 0x02017A40
_0801FC90:
	ldr r0, _0801FCA4 @ =0x02017A40
	mov r3, #0xF1
	lsl r3, r3, #2
	add r0, r0, r3
_0801FC98:
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0801FCA4: .4byte 0x02017A40
	thumb_func_end Chain_AddPartnerEntry

