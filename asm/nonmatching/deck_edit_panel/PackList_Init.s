	thumb_func_start PackList_Init
PackList_Init: @ 0x08064AF4
	push {r4, r5, r6, lr}
	ldr r4, _08064B0C @ =0x02020310
	mov r1, #1
	add r5, r1, #0
	ldrb r0, [r4, #0x18]
	and r5, r0
	cmp r5, #0
	beq _08064B10
	bl PackList_FlushVram
_08064B08:
	mov r0, #0
	b _08064B9A
_08064B0C: .4byte 0x02020310
_08064B10:
	ldr r0, [r4]
	cmp r0, #1
	beq _08064B38
	cmp r0, #1
	bgt _08064B20
	cmp r0, #0
	beq _08064B26
	b _08064B92
_08064B20:
	cmp r0, #2
	beq _08064B50
	b _08064B92
_08064B26:
	str r1, [r4, #0xC]
	add r0, r4, #0
	add r0, #0x6C
	ldrh r0, [r0]
	sub r0, #1
	str r0, [r4, #0x10]
	bl PackList_InitVideo
	b _08064B8A
_08064B38:
	ldr r2, _08064B4C @ =0x000001FD
	mov r0, #2
	mov r1, #0x11
	bl PackList_DrawBackground
	ldr r0, [r4, #0x10]
	bl PackList_DrawCovers
	b _08064B8A
	.align 2, 0
_08064B4C: .4byte 0x000001FD
_08064B50:
	mov r6, #0x80
	lsl r6, r6, #0x13
	ldrh r0, [r6]
	mov r2, #0xA2
	lsl r2, r2, #5
	add r1, r2, #0
	orr r0, r1
	strh r0, [r6]
	mov r0, #0
	bl PackList_DebugNop
	mov r0, #2
	bl FadeFromBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08064B08
	mov r0, #0
	bl PackList_SetCoverAlpha
	strh r5, [r4, #0x1A]
	mov r0, #8
	strh r0, [r4, #0x1C]
	ldrh r0, [r6]
	mov r2, #0xB0
	lsl r2, r2, #4
	add r1, r2, #0
	orr r0, r1
	strh r0, [r6]
_08064B8A:
	ldr r0, [r4]
	add r0, #1
	str r0, [r4]
	b _08064B08
_08064B92:
	mov r0, #0
	bl PackList_DebugNop
	mov r0, #1
_08064B9A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end PackList_Init

