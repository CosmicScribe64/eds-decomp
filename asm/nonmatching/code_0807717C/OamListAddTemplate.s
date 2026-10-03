	thumb_func_start OamListAddTemplate
OamListAddTemplate: @ 0x08077ED4
	push {r4, lr}
	add r4, r0, #0
	add r0, r1, #0
	ldr r1, [sp, #8]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	bl OamListAlloc
	ldr r1, [r4]
	str r1, [r0]
	ldrh r1, [r4, #4]
	strh r1, [r0, #4]
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end OamListAddTemplate
	.align 2, 0

