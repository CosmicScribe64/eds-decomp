	thumb_func_start CoinToss_DrawGlints
CoinToss_DrawGlints: @ 0x08025054
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x28
	mov r9, r0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r8, r1
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	str r2, [sp, #0x24]
	mov r6, #0
	cmp r6, r8
	bcs _080250F0
	mov r0, #0x80
	lsl r0, r0, #2
	mov sl, r0
_0802507A:
	lsl r1, r6, #1
	add r0, r1, r6
	lsl r0, r0, #2
	add r0, r9
	add r7, r1, #0
	add r4, r6, #1
	ldrb r1, [r0, #0xA]
	cmp r1, #1
	bne _080250E8
	ldr r1, _08025100 @ =0x08081F90
	ldrb r0, [r0, #9]
	ldr r2, [sp, #0x24]
	cmp r2, #4
	beq _08025098
	add r0, #5
_08025098:
	lsl r0, r0, #1
	add r0, r1, r0
	ldrh r5, [r0]
	add r5, sl
	add r4, r6, #1
	lsl r0, r4, #4
	sub r0, r0, r4
	lsl r0, r0, #4
	mov r1, r8
	add r1, #1
	bl __divsi3
	add r2, r0, #0
	sub r2, #0x10
	add r0, r7, r6
	lsl r0, r0, #2
	add r0, r9
	ldrh r0, [r0, #4]
	lsl r0, r0, #0x10
	asr r0, r0, #0x18
	mov r3, #0x78
	sub r3, r3, r0
	mov r0, #0x20
	str r0, [sp, #0]
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xC]
	mov r1, sl
	str r1, [sp, #0x10]
	str r0, [sp, #0x14]
	str r0, [sp, #0x18]
	str r0, [sp, #0x1C]
	ldr r0, _08025104 @ =0x02015280
	str r0, [sp, #0x20]
	mov r0, #0
	add r1, r5, #0
	bl OamListAddSprite
_080250E8:
	lsl r0, r4, #0x18
	lsr r6, r0, #0x18
	cmp r6, r8
	bcc _0802507A
_080250F0:
	add sp, #0x28
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08025100: .4byte gCoinGlintTiles
_08025104: .4byte 0x02015280
	thumb_func_end CoinToss_DrawGlints

