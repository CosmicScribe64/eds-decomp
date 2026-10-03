	thumb_func_start CoinToss_UpdateFlight
CoinToss_UpdateFlight: @ 0x08024EC8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	str r0, [sp, #0]
	str r3, [sp, #8]
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	str r1, [sp, #4]
	mov r6, #0
	cmp r6, r1
	bcs _08024FA8
_08024EE4:
	lsl r1, r6, #1
	add r0, r1, r6
	lsl r0, r0, #2
	ldr r2, [sp, #0]
	add r5, r0, r2
	ldrb r0, [r5, #2]
	mov r8, r0
	mov sl, r1
	add r7, r6, #1
	cmp r0, #1
	bne _08024F9E
	ldrh r0, [r5, #6]
	add r0, #0x28
	mov r1, #0
	mov r9, r1
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
	bge _08024F70
	mov r2, r9
	strh r2, [r5, #4]
	strh r2, [r5, #6]
	ldrb r0, [r5, #2]
	add r0, #1
	strb r0, [r5, #2]
	ldr r0, _08024F4C @ =0x02017A30
	ldrh r0, [r0, #6]
	asr r0, r6
	mov r1, r8
	and r0, r1
	cmp r0, #0
	beq _08024F50
	mov r0, #4
	strb r0, [r5, #1]
	b _08024F54
_08024F4C: .4byte 0x02017A30
_08024F50:
	mov r2, #0
	strb r2, [r5, #1]
_08024F54:
	mov r1, sl
	add r0, r1, r6
	lsl r0, r0, #2
	ldr r2, [sp, #0]
	add r0, r0, r2
	ldrb r0, [r0, #1]
	add r7, r6, #1
	cmp r0, #0
	bne _08024F9E
	ldr r1, [sp, #8]
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _08024F9E
_08024F70:
	mov r0, #0xF
	ldrh r5, [r5, #6]
	and r0, r5
	cmp r0, #0
	bne _08024F9E
	lsl r0, r7, #4
	sub r0, r0, r7
	lsl r0, r0, #4
	ldr r1, [sp, #4]
	add r1, #1
	bl __divsi3
	sub r0, #0x10
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	asr r2, r4, #0x18
	mov r1, #0x78
	sub r1, r1, r2
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	ldr r2, _08024FB8 @ =0x02015DD0
	bl CoinToss_SpawnSparkle
_08024F9E:
	lsl r0, r7, #0x18
	lsr r6, r0, #0x18
	ldr r2, [sp, #4]
	cmp r6, r2
	bcc _08024EE4
_08024FA8:
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08024FB8: .4byte 0x02015DD0
	thumb_func_end CoinToss_UpdateFlight

