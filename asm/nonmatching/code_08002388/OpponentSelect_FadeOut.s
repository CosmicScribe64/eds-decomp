	thumb_func_start OpponentSelect_FadeOut
OpponentSelect_FadeOut: @ 0x08002CE8
	push {r4, lr}
	ldr r4, _08002D2C @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	mov r1, #0
	bl OpponentSelect_DrawCursor
	mov r0, #0
	bl OpponentSelect_DrawLockedCovers
	ldr r3, _08002D30 @ =0x0819834C
	ldrb r2, [r4]
	lsl r1, r2, #0x1D
	lsr r1, r1, #0x1D
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1D
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r0, r3
	ldrh r0, [r0]
	bl OpponentSelect_DrawDuelistInfo
	mov r0, #4
	bl FadeToBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_08002D2C: .4byte 0x0201F7E0
_08002D30: .4byte gOpponentSelectDuelists
	thumb_func_end OpponentSelect_FadeOut

