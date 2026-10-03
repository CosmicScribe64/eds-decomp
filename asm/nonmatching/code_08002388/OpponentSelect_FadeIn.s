	thumb_func_start OpponentSelect_FadeIn
OpponentSelect_FadeIn: @ 0x08002A48
	push {r4, lr}
	ldr r4, _08002A94 @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	mov r1, #1
	bl OpponentSelect_DrawCursor
	mov r0, #0
	bl OpponentSelect_DrawLockedCovers
	ldr r3, _08002A98 @ =0x0819834C
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
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _08002A9C @ =0x00001F04
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #4
	bl FadeFromBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r4}
	pop {r1}
	bx r1
_08002A94: .4byte 0x0201F7E0
_08002A98: .4byte gOpponentSelectDuelists
_08002A9C: .4byte 0x00001F04
	thumb_func_end OpponentSelect_FadeIn

