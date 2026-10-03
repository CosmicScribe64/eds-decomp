	thumb_func_start StarterDeckSelect_Init
StarterDeckSelect_Init: @ 0x08064420
	push {r4, lr}
	ldr r4, _08064450 @ =0x02020310
	ldr r1, [r4]
	cmp r1, #0
	beq _08064454
	cmp r1, #1
	beq _08064480
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0xB2
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	bl StarterDeckSelect_DrawCursor
	mov r0, #2
	bl FadeFromBlack
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	b _08064502
	.align 2, 0
_08064450: .4byte 0x02020310
_08064454:
	mov r0, #1
	str r0, [r4, #8]
	str r0, [r4, #0xC]
	str r1, [r4, #0x10]
	bl PackList_InitVideo
	ldr r0, _08064478 @ =0x03000040
	ldr r1, _0806447C @ =0x0000040E
	add r0, r0, r1
	mov r1, #2
	ldrh r2, [r0]
	orr r1, r2
	strh r1, [r0]
	ldr r0, [r4]
	add r0, #1
	str r0, [r4]
	b _08064500
	.align 2, 0
_08064478: .4byte 0x03000040
_0806447C: .4byte 0x0000040E
_08064480:
	mov r2, #0x80
	lsl r2, r2, #2
	ldr r3, _08064508 @ =0x0863CF3C
	mov r0, #2
	mov r1, #0x10
	bl StarterDeckSelect_DrawBackground
	ldr r3, _0806450C @ =0x0863D12C
	mov r0, #0x82
	mov r1, #0
	mov r2, #0x20
	bl LoadBgImageMap1
	ldr r3, _08064510 @ =0x0863E39C
	mov r0, #0x8C
	mov r1, #0
	mov r2, #0xAC
	bl LoadBgImageMap1
	mov r2, #0x9C
	lsl r2, r2, #1
	ldr r3, _08064514 @ =0x0863F54C
	mov r0, #0x96
	mov r1, #0
	bl LoadBgImageMap1
	ldr r1, _08064518 @ =0x040000D4
	ldr r0, _0806451C @ =0x0867793C
	str r0, [r1]
	ldr r0, _08064520 @ =0x05000200
	str r0, [r1, #4]
	ldr r0, _08064524 @ =0x80000010
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _080644D6
_080644CE:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _080644CE
_080644D6:
	ldr r1, _08064518 @ =0x040000D4
	ldr r0, _08064528 @ =0x0867797C
	str r0, [r1]
	ldr r0, _0806452C @ =0x06012000
	str r0, [r1, #4]
	ldr r0, _08064530 @ =0x80000100
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	ldr r3, _08064534 @ =0x02020310
	cmp r0, #0
	bge _080644FA
_080644F2:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _080644F2
_080644FA:
	ldr r0, [r3]
	add r0, #1
	str r0, [r3]
_08064500:
	mov r0, #0
_08064502:
	pop {r4}
	pop {r1}
	bx r1
_08064508: .4byte gStarterDeckBgImage
_0806450C: .4byte gStarterDeckBoxBlackImage
_08064510: .4byte gStarterDeckBoxRedImage
_08064514: .4byte gStarterDeckBoxGreenImage
_08064518: .4byte 0x040000D4
_0806451C: .4byte gHandCursorPal
_08064520: .4byte 0x05000200
_08064524: .4byte 0x80000010
_08064528: .4byte gHandCursorGfx
_0806452C: .4byte 0x06012000
_08064530: .4byte 0x80000100
_08064534: .4byte 0x02020310
	thumb_func_end StarterDeckSelect_Init

