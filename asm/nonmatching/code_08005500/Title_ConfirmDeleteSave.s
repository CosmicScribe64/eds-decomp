	thumb_func_start Title_ConfirmDeleteSave
Title_ConfirmDeleteSave: @ 0x0800553C
	push {r4, r5, lr}
	ldr r1, _08005558 @ =0x03000040
	ldr r2, _0800555C @ =0x00004858
	add r0, r1, r2
	ldrb r0, [r0]
	add r3, r1, #0
	cmp r0, #0xC
	bls _0800554E
	b _08005708
_0800554E:
	lsl r0, r0, #2
	ldr r1, _08005560 @ =0x08005564
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_08005558: .4byte 0x03000040
_0800555C: .4byte 0x00004858
_08005560: .4byte 0x08005564
_08005564:
	.4byte _08005598
	.4byte _080055CC
	.4byte _0800562C
	.4byte _08005648
	.4byte _08005690
	.4byte _08005708
	.4byte _08005708
	.4byte _08005708
	.4byte _08005708
	.4byte _08005708
	.4byte _080056A4
	.4byte _080056CC
	.4byte _080056F8
_08005598:
	ldr r0, _080055C0 @ =0x0201527C
	ldr r0, [r0]
	lsl r1, r0, #0xF
	lsl r0, r0, #0xE
	lsr r1, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r1, r0
	bne _080055AA
	b _080056A0
_080055AA:
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #4
	strh r0, [r1]
	ldr r0, _080055C4 @ =0x0000040E
	add r1, r3, r0
	mov r0, #1
	strh r0, [r1]
	ldr r2, _080055C8 @ =0x00004858
	add r1, r3, r2
	b _08005682
_080055C0: .4byte 0x0201527C
_080055C4: .4byte 0x0000040E
_080055C8: .4byte 0x00004858
_080055CC:
	ldr r0, _08005610 @ =0x05000200
	ldr r1, _08005614 @ =0x087D01D4
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _08005618 @ =0x06014000
	ldr r1, _0800561C @ =0x087CC1D4
	mov r2, #0x80
	lsl r2, r2, #7
	bl CopyDoubleWords
	mov r0, #0xA0
	lsl r0, r0, #0x13
	ldr r1, _08005620 @ =0x087CBFD4
	mov r2, #0x80
	lsl r2, r2, #2
	bl MemCopy16
	mov r0, #0xC0
	lsl r0, r0, #0x13
	ldr r4, _08005624 @ =0x087C29D4
	mov r5, #0x96
	lsl r5, r5, #8
	add r1, r4, #0
	add r2, r5, #0
	bl MemCopy16
	ldr r0, _08005628 @ =0x0600A000
	add r1, r4, #0
	add r2, r5, #0
	bl MemCopy16
	b _080056B4
	.align 2, 0
_08005610: .4byte 0x05000200
_08005614: .4byte gDeletePromptObjPal
_08005618: .4byte 0x06014000
_0800561C: .4byte gDeletePromptObjGfx
_08005620: .4byte gDeletePromptBgPal
_08005624: .4byte gDeletePromptBgBitmap
_08005628: .4byte 0x0600A000
_0800562C:
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _08005644 @ =0x00001F04
	add r0, r2, #0
	strh r0, [r1]
	bl Title_DrawDeletePrompt
	mov r0, #2
	bl FadeFromBlack
	b _080056AE
	.align 2, 0
_08005644: .4byte 0x00001F04
_08005648:
	bl Title_DrawDeletePrompt
	ldr r4, _08005668 @ =0x03000040
	ldrh r1, [r4, #6]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _08005670
	mov r0, #2
	bl PlaySE
	ldr r2, _0800566C @ =0x00004858
	add r1, r4, r2
	mov r0, #0xA
	strb r0, [r1]
	b _08005708
_08005668: .4byte 0x03000040
_0800566C: .4byte 0x00004858
_08005670:
	mov r0, #8
	and r0, r1
	cmp r0, #0
	beq _08005708
	mov r0, #1
	bl PlaySE
	ldr r0, _0800568C @ =0x00004858
	add r1, r4, r0
_08005682:
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _08005708
	.align 2, 0
_0800568C: .4byte 0x00004858
_08005690:
	bl Title_DrawDeletePrompt
	mov r0, #2
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08005708
_080056A0:
	mov r0, #1
	b _0800570A
_080056A4:
	bl Title_DrawDeletePrompt
	mov r0, #2
	bl FadeToBlack
_080056AE:
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08005708
_080056B4:
	ldr r0, _080056C4 @ =0x03000040
	ldr r1, _080056C8 @ =0x00004858
	add r0, r0, r1
_080056BA:
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
	b _08005708
	.align 2, 0
_080056C4: .4byte 0x03000040
_080056C8: .4byte 0x00004858
_080056CC:
	bl SetBrightnessBlack
	bl ResetVideo
	bl ResetBgScroll
	bl Title_InitBgCnt
	ldr r0, _080056EC @ =0x03000040
	ldr r1, _080056F0 @ =0x0000040E
	add r2, r0, r1
	mov r1, #3
	strh r1, [r2]
	ldr r2, _080056F4 @ =0x00004858
	add r0, r0, r2
	b _080056BA
_080056EC: .4byte 0x03000040
_080056F0: .4byte 0x0000040E
_080056F4: .4byte 0x00004858
_080056F8:
	ldr r0, _08005710 @ =0x00004878
	add r1, r3, r0
	mov r2, #0
	mov r0, #1
	strb r0, [r1]
	ldr r1, _08005714 @ =0x00004858
	add r0, r3, r1
	strb r2, [r0]
_08005708:
	mov r0, #0
_0800570A:
	pop {r4, r5}
	pop {r1}
	bx r1
_08005710: .4byte 0x00004878
_08005714: .4byte 0x00004858
	thumb_func_end Title_ConfirmDeleteSave

