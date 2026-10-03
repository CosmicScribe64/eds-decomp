	thumb_func_start OpponentSelect_DrawDuelistInfo
OpponentSelect_DrawDuelistInfo: @ 0x080036FC
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	mov r6, #2
	ldr r7, _08003824 @ =0x081984CC
	mov r5, #2
_0800370E:
	ldmia r7!, {r0}
	bl StrLen
	add r2, r6, #0
	add r2, #0x10
	lsl r1, r0, #2
	add r1, r1, r0
	add r6, r2, r1
	sub r5, #1
	cmp r5, #0
	bge _0800370E
	lsr r0, r6, #0x1F
	add r0, r6, r0
	asr r0, r0, #1
	mov r1, #0x78
	sub r7, r1, r0
	sub r0, r4, #1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x17
	bhi _08003818
	mov r6, #0x9C
	lsl r6, r6, #0xE
	add r0, r7, #0
	orr r0, r6
	mov r1, #0x81
	lsl r1, r1, #7
	mov r8, r1
	ldr r2, _08003828 @ =0x0000328A
	bl AddSprite
	ldr r5, _08003824 @ =0x081984CC
	ldr r0, [r5]
	bl StrLen
	lsl r1, r0, #2
	add r1, r1, r0
	add r7, r7, r1
	ldr r0, _0800382C @ =0x02011C20
	lsl r4, r4, #2
	add r4, r4, r0
	ldr r0, _08003830 @ =0x000020D0
	add r0, r0, r4
	mov r9, r0
	ldrh r1, [r0]
	lsl r2, r1, #0x15
	lsr r2, r2, #0x15
	add r0, r7, #0
	mov r1, #0x28
	bl OpponentSelect_DrawNumber
	add r7, #0x11
	add r0, r7, #0
	orr r0, r6
	ldr r2, _08003834 @ =0x0000328E
	mov r1, r8
	bl AddSprite
	ldr r0, [r5, #4]
	bl StrLen
	lsl r1, r0, #2
	add r1, r1, r0
	add r7, r7, r1
	mov r0, r9
	ldr r2, [r0]
	lsl r2, r2, #0xA
	lsr r2, r2, #0x15
	add r0, r7, #0
	mov r1, #0x28
	bl OpponentSelect_DrawNumber
	add r7, #0x11
	add r0, r7, #0
	orr r0, r6
	ldr r2, _08003838 @ =0x00003292
	mov r1, r8
	bl AddSprite
	ldr r0, [r5, #8]
	bl StrLen
	lsl r1, r0, #2
	add r1, r1, r0
	add r7, r7, r1
	ldr r1, _0800383C @ =0x000020D2
	add r4, r4, r1
	ldrh r4, [r4]
	lsr r2, r4, #6
	add r0, r7, #0
	mov r1, #0x28
	bl OpponentSelect_DrawNumber
	ldr r0, _08003840 @ =0x000A0048
	ldr r5, _08003844 @ =0x0201F7E0
	ldrb r1, [r5]
	lsl r2, r1, #0x1A
	lsr r2, r2, #0x1D
	lsl r2, r2, #6
	mov r1, #0xB0
	lsl r1, r1, #2
	add r2, r2, r1
	mov r1, #0xC0
	lsl r1, r1, #6
	add r4, r1, #0
	orr r2, r4
	mov r1, r8
	bl AddSprite
	ldr r0, _08003848 @ =0x000A0068
	ldrb r1, [r5]
	lsl r2, r1, #0x1A
	lsr r2, r2, #0x1D
	lsl r2, r2, #6
	mov r1, #0xB1
	lsl r1, r1, #2
	add r2, r2, r1
	orr r2, r4
	mov r1, r8
	bl AddSprite
	ldr r0, _0800384C @ =0x000A0088
	ldrb r5, [r5]
	lsl r2, r5, #0x1A
	lsr r2, r2, #0x1D
	lsl r2, r2, #6
	mov r1, #0xB2
	lsl r1, r1, #2
	add r2, r2, r1
	orr r2, r4
	mov r1, r8
	bl AddSprite
_08003818:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_08003824: .4byte gWinLoseDrawLabels
_08003828: .4byte 0x0000328A
_0800382C: .4byte 0x02011C20
_08003830: .4byte 0x000020D0
_08003834: .4byte 0x0000328E
_08003838: .4byte 0x00003292
_0800383C: .4byte 0x000020D2
_08003840: .4byte 0x000A0048
_08003844: .4byte 0x0201F7E0
_08003848: .4byte 0x000A0068
_0800384C: .4byte 0x000A0088
	thumb_func_end OpponentSelect_DrawDuelistInfo

