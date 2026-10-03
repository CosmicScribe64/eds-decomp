	thumb_func_start CoinToss_UpdateFlightUniform
CoinToss_UpdateFlightUniform: @ 0x08024E24
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	str r0, [sp, #0]
	mov r9, r3
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov sl, r1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #4]
	mov r7, #0
	cmp r7, sl
	bcs _08024EB8
_08024E46:
	lsl r0, r7, #1
	add r0, r0, r7
	lsl r0, r0, #2
	ldr r1, [sp, #0]
	add r5, r0, r1
	ldrb r6, [r5, #2]
	cmp r6, #1
	bne _08024EAE
	ldrh r0, [r5, #6]
	add r0, #0x28
	mov r1, #0
	mov r8, r1
	strh r0, [r5, #6]
	ldrh r1, [r5, #6]
	mov r0, #0xC0
	lsl r0, r0, #6
	bl MulFix8
	add r4, r0, #0
	ldrh r1, [r5, #6]
	add r0, r1, #0
	bl MulFix8
	add r1, r0, #0
	mov r0, #0xA0
	lsl r0, r0, #3
	bl MulFix8
	sub r4, r4, r0
	strh r4, [r5, #4]
	lsl r4, r4, #0x10
	cmp r4, #0
	bge _08024EAE
	mov r0, r8
	strh r0, [r5, #4]
	strh r0, [r5, #6]
	ldrb r0, [r5, #2]
	add r0, #1
	strb r0, [r5, #2]
	ldr r1, [sp, #4]
	and r6, r1
	mov r0, #0
	cmp r6, #0
	bne _08024EA0
	mov r0, #4
_08024EA0:
	strb r0, [r5, #1]
	cmp r0, #0
	bne _08024EAE
	mov r1, r9
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
_08024EAE:
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, sl
	bcc _08024E46
_08024EB8:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end CoinToss_UpdateFlightUniform

