	thumb_func_start CoinToss_DrawCoins
CoinToss_DrawCoins: @ 0x08024FBC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x24
	mov sl, r0
	lsl r1, r1, #0x18
	lsr r1, r1, #0x18
	mov r8, r1
	mov r5, #0
	cmp r5, r8
	bcs _0802503A
	mov r0, #0x80
	lsl r0, r0, #2
	mov r9, r0
	mov r7, #0
_08024FDE:
	lsl r4, r5, #1
	add r4, r4, r5
	lsl r4, r4, #2
	add r4, sl
	ldrb r1, [r4, #1]
	lsl r0, r1, #1
	ldr r1, _0802504C @ =0x08081F80
	add r0, r0, r1
	ldrh r6, [r0]
	add r6, r9
	add r5, #1
	lsl r0, r5, #4
	sub r0, r0, r5
	lsl r0, r0, #4
	mov r1, r8
	add r1, #1
	bl __divsi3
	add r2, r0, #0
	sub r2, #0x10
	ldrh r4, [r4, #4]
	lsl r0, r4, #0x10
	asr r0, r0, #0x18
	mov r3, #0x78
	sub r3, r3, r0
	mov r0, #0x20
	str r0, [sp, #0]
	str r0, [sp, #4]
	mov r0, #4
	str r0, [sp, #8]
	str r7, [sp, #0xC]
	mov r0, r9
	str r0, [sp, #0x10]
	str r7, [sp, #0x14]
	str r7, [sp, #0x18]
	str r7, [sp, #0x1C]
	ldr r0, _08025050 @ =0x02015280
	str r0, [sp, #0x20]
	mov r0, #0
	add r1, r6, #0
	bl OamListAddSprite
	lsl r5, r5, #0x18
	lsr r5, r5, #0x18
	cmp r5, r8
	bcc _08024FDE
_0802503A:
	add sp, #0x24
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0802504C: .4byte gCoinSpinTiles
_08025050: .4byte 0x02015280
	thumb_func_end CoinToss_DrawCoins

