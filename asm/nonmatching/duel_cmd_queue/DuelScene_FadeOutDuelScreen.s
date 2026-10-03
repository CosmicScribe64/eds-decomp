	thumb_func_start DuelScene_FadeOutDuelScreen
DuelScene_FadeOutDuelScreen: @ 0x0801EA1C
	push {r4, lr}
	ldr r4, _0801EA68 @ =0x02017A30
	ldrb r1, [r4, #0xB]
	cmp r1, #0
	bne _0801EA58
	ldr r0, _0801EA6C @ =0x03000040
	ldr r2, _0801EA70 @ =0x00000414
	add r0, r0, r2
	str r1, [r0]
	bl ResetBgScroll
	ldr r2, _0801EA74 @ =0x0201CFB0
	ldr r3, _0801EA78 @ =0x00000808
	add r1, r2, r3
	mov r0, #9
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	mov r0, #3
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #5
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldrb r0, [r4, #0xB]
	add r0, #1
	strb r0, [r4, #0xB]
_0801EA58:
	bl DuelScreen_FadeOutStep
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0801EA68: .4byte 0x02017A30
_0801EA6C: .4byte 0x03000040
_0801EA70: .4byte 0x00000414
_0801EA74: .4byte 0x0201CFB0
_0801EA78: .4byte 0x00000808
	thumb_func_end DuelScene_FadeOutDuelScreen

