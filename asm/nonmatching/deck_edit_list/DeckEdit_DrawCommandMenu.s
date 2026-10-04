	thumb_func_start DeckEdit_DrawCommandMenu
DeckEdit_DrawCommandMenu: @ 0x08067660
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	add r7, r0, #0
	ldr r0, _08067804 @ =0x0201DB20
	ldr r2, _08067808 @ =0x00001C5A
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r1, r1, #0x1E
	lsr r1, r1, #0x1E
	mov r3, #0
	mov r8, r0
	cmp r1, #2
	bne _08067684
	mov r3, #4
_08067684:
	ldrb r0, [r7, #1]
	mov r4, #7
	mov sl, r4
	mov r1, sl
	and r1, r0
	add r2, r0, #0
	cmp r1, #0
	bne _08067696
	b _08067874
_08067696:
	mov r0, #0x60
	and r0, r2
	add r3, #2
	mov r9, r3
	cmp r0, #0
	beq _080676EC
	lsl r3, r2, #0x19
	lsr r1, r3, #0x1E
	mov r5, r9
	sub r1, r5, r1
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	ldr r1, _0806780C @ =0x086F17B0
	add r0, r0, r1
	ldr r1, _08067810 @ =0x0600E000
	lsr r3, r3, #0x1E
	mov r6, #0x1E
	str r6, [sp, #0]
	mov r5, #3
	str r5, [sp, #4]
	mov r4, #2
	str r4, [sp, #8]
	mov r2, #0x12
	bl CopyMapRectSetPalette
	ldrb r2, [r7, #1]
	lsl r3, r2, #0x19
	lsr r1, r3, #0x1E
	sub r1, r4, r1
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	ldr r1, _08067814 @ =0x086F17E8
	add r0, r0, r1
	ldr r1, _08067818 @ =0x0600E038
	lsr r3, r3, #0x1E
	str r6, [sp, #0]
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r2, #1
	bl CopyMapRectSetPalette
_080676EC:
	ldrb r2, [r7, #1]
	lsl r3, r2, #0x19
	lsr r1, r3, #0x1E
	mov r6, #2
	sub r1, r6, r1
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	ldr r5, _0806781C @ =0x00001C1C
	add r5, r8
	add r2, r7, #3
	ldrb r4, [r5]
	add r2, r4, r2
	ldrb r4, [r2]
	lsl r1, r4, #4
	sub r1, r1, r4
	lsl r1, r1, #3
	ldr r2, _08067820 @ =0x086F17D4
	add r1, r1, r2
	add r0, r0, r1
	ldr r1, _08067824 @ =0x0600E024
	lsr r3, r3, #0x1E
	mov r4, #0x1E
	mov r8, r4
	str r4, [sp, #0]
	mov r4, #3
	str r4, [sp, #4]
	str r6, [sp, #8]
	mov r2, #6
	bl CopyMapRectSetPalette
	ldrb r0, [r7, #1]
	lsl r3, r0, #0x19
	lsr r1, r3, #0x1E
	sub r1, r6, r1
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	add r2, r7, #6
	ldrb r5, [r5]
	add r2, r5, r2
	ldrb r5, [r2]
	lsl r1, r5, #4
	sub r1, r1, r5
	lsl r1, r1, #3
	ldr r2, _08067828 @ =0x086F17E0
	add r1, r1, r2
	add r0, r0, r1
	ldr r1, _0806782C @ =0x0600E030
	lsr r3, r3, #0x1E
	mov r5, r8
	str r5, [sp, #0]
	str r4, [sp, #4]
	str r6, [sp, #8]
	mov r2, #4
	bl CopyMapRectSetPalette
	ldrb r0, [r7, #1]
	lsl r3, r0, #0x19
	lsr r1, r3, #0x1E
	mov r2, r9
	sub r1, r2, r1
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	ldr r1, [r7]
	lsl r1, r1, #0xE
	lsr r2, r1, #0x1D
	lsl r2, r2, #1
	add r2, #0x40
	lsl r2, r2, #1
	ldr r5, _0806780C @ =0x086F17B0
	add r2, r2, r5
	add r0, r0, r2
	lsr r1, r1, #0x1D
	lsl r1, r1, #2
	add r1, #8
	ldr r2, _08067810 @ =0x0600E000
	add r1, r1, r2
	lsr r3, r3, #0x1E
	mov r2, r8
	str r2, [sp, #0]
	str r4, [sp, #4]
	str r6, [sp, #8]
	mov r2, #2
	bl CopyMapRectSetPalette
	ldrb r4, [r7, #1]
	lsl r3, r4, #0x19
	lsr r0, r3, #0x1E
	cmp r0, #1
	bhi _080677CA
	add r1, r0, #0
	lsl r0, r1, #4
	sub r0, r0, r1
	lsl r0, r0, #2
	ldr r5, _08067830 @ =0x086E26D0
	add r0, r0, r5
	lsl r1, r1, #6
	ldr r2, _08067810 @ =0x0600E000
	add r1, r1, r2
	lsr r3, r3, #0x1E
	sub r3, r6, r3
	mov r4, r8
	str r4, [sp, #0]
	mov r2, #0
	str r2, [sp, #4]
	str r2, [sp, #8]
	mov r2, #0x1E
	bl CopyMapRectAddOffset
_080677CA:
	ldrb r1, [r7, #1]
	lsl r0, r1, #0x19
	lsr r0, r0, #0x1E
	lsl r0, r0, #3
	mov r3, #0x19
	neg r3, r3
	and r3, r1
	orr r3, r0
	strb r3, [r7, #1]
	add r2, r3, #0
	mov r5, sl
	and r2, r5
	cmp r2, #3
	beq _080677EA
	cmp r2, #1
	bne _08067840
_080677EA:
	ldr r0, _08067834 @ =0x03000040
	ldr r1, _08067838 @ =0x00000414
	add r0, r0, r1
	ldr r1, _0806783C @ =0x08067541
	str r1, [r0]
	cmp r2, #3
	bne _0806784A
	mov r0, #8
	neg r0, r0
	and r3, r0
	strb r3, [r7, #1]
	b _0806784A
	.align 2, 0
_08067804: .4byte 0x0201DB20
_08067808: .4byte 0x00001C5A
_0806780C: .4byte gDeckEditMenuTilemap
_08067810: .4byte 0x0600E000
_08067814: .4byte gDeckEditMenuTilemapBarEnd
_08067818: .4byte 0x0600E038
_0806781C: .4byte 0x00001C1C
_08067820: .4byte gDeckEditMenuTilemapFilterLabels
_08067824: .4byte 0x0600E024
_08067828: .4byte gDeckEditMenuTilemapSortLabels
_0806782C: .4byte 0x0600E030
_08067830: .4byte gDeckEditFrameMap
_08067834: .4byte 0x03000040
_08067838: .4byte 0x00000414
_0806783C: .4byte DeckEdit_CommandLabelVBlank
_08067840:
	ldr r0, _08067860 @ =0x03000040
	ldr r2, _08067864 @ =0x00000414
	add r0, r0, r2
	ldr r1, _08067868 @ =0x08067631
	str r1, [r0]
_0806784A:
	ldr r2, _0806786C @ =0x0201DB20
	ldr r3, _08067870 @ =0x00001BB4
	add r1, r2, r3
	mov r0, #1
	ldrb r4, [r1]
	orr r0, r4
	strb r0, [r1]
	mov r8, r2
	ldrb r2, [r7, #1]
	b _0806787C
	.align 2, 0
_08067860: .4byte 0x03000040
_08067864: .4byte 0x00000414
_08067868: .4byte DeckEdit_CommandWindowVBlank
_0806786C: .4byte 0x0201DB20
_08067870: .4byte 0x00001BB4
_08067874:
	ldr r0, _080678F8 @ =0x03000040
	ldr r5, _080678FC @ =0x00000414
	add r0, r0, r5
	str r1, [r0]
_0806787C:
	ldr r0, _08067900 @ =0x081A6EAC
	lsl r2, r2, #0x19
	lsr r2, r2, #0x1E
	mov r1, #2
	sub r1, r1, r2
	lsl r1, r1, #3
	neg r1, r1
	mov r2, #0xFF
	and r1, r2
	str r1, [sp, #0]
	mov r1, #1
	str r1, [sp, #4]
	mov r4, #0
	str r4, [sp, #8]
	str r1, [sp, #0xC]
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	mov r1, r8
	str r1, [sp, #0x1C]
	mov r1, #0
	mov r2, #1
	mov r3, #0
	bl OamListAddSpriteGroup
	mov r0, #0x60
	ldrb r2, [r7, #1]
	and r0, r2
	cmp r0, #0x40
	bne _080678E6
	ldr r1, [r7]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x1D
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #3
	ldr r1, _08067904 @ =0x081A6D8C
	add r0, r0, r1
	mov r3, #1
	neg r3, r3
	str r3, [sp, #0]
	str r4, [sp, #4]
	str r4, [sp, #8]
	str r4, [sp, #0xC]
	str r4, [sp, #0x10]
	str r4, [sp, #0x14]
	str r4, [sp, #0x18]
	mov r4, r8
	str r4, [sp, #0x1C]
	mov r1, #0
	mov r2, #5
	bl OamListAddSpriteGroup
_080678E6:
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080678F8: .4byte 0x03000040
_080678FC: .4byte 0x00000414
_08067900: .4byte gDeckEditCommandTabSprite
_08067904: .4byte gDeckEditCommandLabelSprites
	thumb_func_end DeckEdit_DrawCommandMenu

