	thumb_func_start DeckStats_DrawNumbers
DeckStats_DrawNumbers: @ 0x0806CD14
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	mov r7, #0
	mov r0, #1
	mov sl, r0
	mov r1, #8
	mov r9, r1
	mov r6, #0
	ldr r0, _0806CDC8 @ =0x0201DB20
	mov r8, r0
_0806CD30:
	lsl r5, r7, #2
	ldr r1, _0806CDCC @ =0x02030000
	add r5, r5, r1
	ldrh r0, [r5]
	lsl r4, r7, #4
	add r4, #0x24
	str r4, [sp, #0]
	ldr r1, _0806CDD0 @ =0x081A6EB4
	str r1, [sp, #4]
	mov r1, sl
	str r1, [sp, #8]
	mov r1, r9
	str r1, [sp, #0xC]
	str r6, [sp, #0x10]
	str r6, [sp, #0x14]
	str r6, [sp, #0x18]
	mov r1, r8
	str r1, [sp, #0x1C]
	mov r1, #4
	mov r2, #1
	mov r3, #0xA8
	bl DrawNumberSprites
	ldrh r0, [r5, #2]
	str r4, [sp, #0]
	ldr r1, _0806CDD0 @ =0x081A6EB4
	str r1, [sp, #4]
	mov r1, sl
	str r1, [sp, #8]
	mov r1, r9
	str r1, [sp, #0xC]
	str r6, [sp, #0x10]
	str r6, [sp, #0x14]
	str r6, [sp, #0x18]
	mov r1, r8
	str r1, [sp, #0x1C]
	mov r1, #3
	mov r2, #1
	mov r3, #0xC8
	bl DrawNumberSprites
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, #5
	bls _0806CD30
	ldr r1, _0806CDCC @ =0x02030000
	ldrh r0, [r1, #0x18]
	mov r1, #0x8C
	str r1, [sp, #0]
	ldr r1, _0806CDD0 @ =0x081A6EB4
	str r1, [sp, #4]
	mov r1, #1
	str r1, [sp, #8]
	mov r1, #8
	str r1, [sp, #0xC]
	mov r1, #0
	str r1, [sp, #0x10]
	str r1, [sp, #0x14]
	str r1, [sp, #0x18]
	ldr r1, _0806CDC8 @ =0x0201DB20
	str r1, [sp, #0x1C]
	mov r1, #4
	mov r2, #1
	mov r3, #0x98
	bl DrawNumberSprites
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0806CDC8: .4byte 0x0201DB20
_0806CDCC: .4byte 0x02030000
_0806CDD0: .4byte gDeckEditDigitSprites
	thumb_func_end DeckStats_DrawNumbers

