	thumb_func_start sub_0806664C
sub_0806664C: @ 0x0806664C
	push {r4, r5, r6, lr}
	sub sp, #4
	add r5, r0, #0
	add r0, r1, #0
	add r4, r2, #0
	add r6, r3, #0
	ldr r2, [sp, #0x14]
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	str r2, [sp, #0]
	bl sub_0806635C
	lsl r1, r5, #4
	add r6, r6, r1
	strb r0, [r6, #5]
	mov r0, #1
	strb r0, [r6, #0xC]
	strb r4, [r6, #4]
	ldr r0, _080666A4 @ =0x08087464
	lsl r4, r4, #1
	add r0, r4, r0
	ldrh r0, [r0]
	strh r0, [r6, #6]
	ldr r0, _080666A8 @ =0x08087472
	add r4, r4, r0
	ldrh r1, [r4]
	strh r1, [r6, #0xE]
	add r5, #1
	strb r5, [r6, #0x12]
	lsl r0, r5, #1
	add r0, r0, r5
	lsl r0, r0, #3
	ldr r2, [sp, #0]
	add r2, r2, r0
	strh r1, [r2, #2]
	strh r1, [r2]
	add sp, #4
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_080666A4: .4byte gUnk_08087464
_080666A8: .4byte gUnk_08087472
	thumb_func_end sub_0806664C

