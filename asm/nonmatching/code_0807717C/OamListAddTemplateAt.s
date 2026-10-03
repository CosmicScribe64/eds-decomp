	thumb_func_start OamListAddTemplateAt
OamListAddTemplateAt: @ 0x08077E40
	push {r4, r5, r6, lr}
	add r6, r0, #0
	add r0, r1, #0
	add r5, r2, #0
	add r4, r3, #0
	ldr r1, [sp, #0x1C]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	lsl r5, r5, #0x10
	lsr r5, r5, #0x10
	lsl r4, r4, #0x10
	lsr r4, r4, #0x10
	bl OamListAlloc
	mov r2, #0xFF
	lsl r2, r2, #8
	ldrh r1, [r6]
	and r2, r1
	lsl r4, r4, #0x10
	asr r4, r4, #0x10
	mov r1, #0xFF
	and r4, r1
	orr r2, r4
	strh r2, [r0]
	mov r1, #0xFE
	lsl r1, r1, #8
	ldrh r6, [r6, #2]
	and r1, r6
	lsl r5, r5, #0x17
	lsr r5, r5, #0x17
	orr r1, r5
	strh r1, [r0, #2]
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end OamListAddTemplateAt
	.align 2, 0

