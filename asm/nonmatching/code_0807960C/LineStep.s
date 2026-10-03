	thumb_func_start LineStep
LineStep: @ 0x0807A420
	add r2, r0, #0
	ldrb r3, [r2, #0x12]
	cmp r3, #0
	beq _0807A48C
	ldr r1, [r2]
	ldr r0, [r2, #4]
	cmp r1, r0
	bne _0807A436
	mov r0, #0
	strb r0, [r2, #0x12]
	b _0807A48C
_0807A436:
	cmp r3, #2
	bne _0807A464
	ldrh r1, [r2, #2]
	ldrh r3, [r2, #0xA]
	add r0, r1, r3
	strh r0, [r2, #2]
	ldrh r1, [r2, #0x10]
	ldrh r3, [r2, #0xC]
	add r0, r1, r3
	strh r0, [r2, #0x10]
	ldrh r1, [r2, #0x10]
	mov r3, #0xE
	ldsh r0, [r2, r3]
	cmp r1, r0
	blt _0807A48C
	ldrh r3, [r2, #0xE]
	sub r0, r1, r3
	strh r0, [r2, #0x10]
	ldrh r1, [r2]
	ldrh r3, [r2, #8]
	add r0, r1, r3
	strh r0, [r2]
	b _0807A48C
_0807A464:
	ldrh r1, [r2]
	ldrh r3, [r2, #8]
	add r0, r1, r3
	strh r0, [r2]
	ldrh r1, [r2, #0x10]
	ldrh r3, [r2, #0xE]
	add r0, r1, r3
	strh r0, [r2, #0x10]
	ldrh r1, [r2, #0x10]
	mov r3, #0xC
	ldsh r0, [r2, r3]
	cmp r1, r0
	blt _0807A48C
	ldrh r3, [r2, #0xC]
	sub r0, r1, r3
	strh r0, [r2, #0x10]
	ldrh r1, [r2, #2]
	ldrh r3, [r2, #0xA]
	add r0, r1, r3
	strh r0, [r2, #2]
_0807A48C:
	bx lr
	thumb_func_end LineStep
	.align 2, 0

