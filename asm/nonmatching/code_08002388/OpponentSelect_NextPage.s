	thumb_func_start OpponentSelect_NextPage
OpponentSelect_NextPage: @ 0x08002E80
	push {r4, r5, r6, lr}
	ldr r5, _08002EF4 @ =0x03000040
	ldr r0, _08002EF8 @ =0x0000485A
	add r6, r5, r0
	ldrb r0, [r6]
	cmp r0, #0
	beq _08002F10
	cmp r0, #1
	beq _08002F7C
	ldr r4, _08002EFC @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	mov r1, #1
	bl OpponentSelect_DrawCursor
	ldr r3, _08002F00 @ =0x0819834C
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
	ldr r1, _08002F04 @ =0x00004832
	add r0, r5, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1A
	neg r0, r0
	bl OpponentSelect_SetBgScroll
	mov r0, #3
	bl FadeFromBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08002F98
	mov r0, #0
	bl OpponentSelect_SetBgScroll
	ldr r0, _08002F08 @ =0x00004859
	add r1, r5, r0
	mov r2, #0
	mov r0, #2
	strb r0, [r1]
	strb r2, [r6]
	ldr r1, _08002F0C @ =0x0000485B
	add r0, r5, r1
	strb r2, [r0]
	b _08002F98
	.align 2, 0
_08002EF4: .4byte 0x03000040
_08002EF8: .4byte 0x0000485A
_08002EFC: .4byte 0x0201F7E0
_08002F00: .4byte gOpponentSelectDuelists
_08002F04: .4byte 0x00004832
_08002F08: .4byte 0x00004859
_08002F0C: .4byte 0x0000485B
_08002F10:
	ldr r4, _08002F68 @ =0x0201F7E0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1D
	mov r1, #1
	bl OpponentSelect_DrawCursor
	ldr r3, _08002F6C @ =0x0819834C
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
	ldr r0, _08002F70 @ =0x00004832
	add r4, r5, r0
	ldrb r1, [r4]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1A
	bl OpponentSelect_SetBgScroll
	mov r0, #3
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08002F74
	ldrb r4, [r4]
	lsl r0, r4, #0x1A
	lsr r0, r0, #0x1A
	bl OpponentSelect_DrawLockedCovers
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	b _08002FAA
	.align 2, 0
_08002F68: .4byte 0x0201F7E0
_08002F6C: .4byte gOpponentSelectDuelists
_08002F70: .4byte 0x00004832
_08002F74:
	ldrb r4, [r4]
	lsl r0, r4, #0x1A
	lsr r0, r0, #0x1A
	b _08002FA6
_08002F7C:
	ldr r0, _08002F94 @ =0x0201F7E0
	ldrb r0, [r0]
	lsl r0, r0, #0x1D
	lsr r0, r0, #0x1D
	add r0, #1
	bl OpponentSelect_LoadPage
	ldrb r0, [r6]
	add r0, #1
	strb r0, [r6]
	b _08002FAA
	.align 2, 0
_08002F94: .4byte 0x0201F7E0
_08002F98:
	ldr r0, _08002FB4 @ =0x03000040
	ldr r1, _08002FB8 @ =0x00004832
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1A
	neg r0, r0
_08002FA6:
	bl OpponentSelect_DrawLockedCovers
_08002FAA:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08002FB4: .4byte 0x03000040
_08002FB8: .4byte 0x00004832
	thumb_func_end OpponentSelect_NextPage

