	thumb_func_start ChainListScreen_Run
ChainListScreen_Run: @ 0x0801A32C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _0801A34C @ =0x020185B8
	ldrb r0, [r0, #4]
	lsr r0, r0, #1
	cmp r0, #7
	bls _0801A342
	b _0801A7A0
_0801A342:
	lsl r0, r0, #2
	ldr r1, _0801A350 @ =0x0801A354
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0801A34C: .4byte 0x020185B8
_0801A350: .4byte 0x0801A354
_0801A354:
	.4byte _0801A374
	.4byte _0801A382
	.4byte _0801A38A
	.4byte _0801A390
	.4byte _0801A64C
	.4byte _0801A6AA
	.4byte _0801A70E
	.4byte _0801A77A
_0801A374:
	bl TextCellsClear
	mov r0, #0
	mov r1, #0
	bl DuelScreen_ScrollToZone
	b _0801A784
_0801A382:
	mov r0, #1
	bl DuelFieldDim
	b _0801A780
_0801A38A:
	bl UnloadDuelUiGfx
	b _0801A784
_0801A390:
	ldr r0, _0801A49C @ =0x05000200
	ldr r1, _0801A4A0 @ =0x0822C300
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _0801A4A4 @ =0x05000220
	ldr r1, _0801A4A8 @ =0x0867795C
	mov r2, #0x20
	bl CopyDoubleWords
	mov r0, #0x20
	mov r1, #2
	bl TextCanvasInit
	ldr r5, _0801A4AC @ =0x08198DE4
	ldr r0, _0801A4B0 @ =0x020185B8
	mov r9, r0
	ldrb r1, [r0, #4]
	lsl r3, r1, #0x1F
	lsr r0, r3, #0x1F
	mov r4, #0x4C
	mul r0, r4
	add r1, r5, #4
	add r0, r0, r1
	mov r2, #0xC0
	lsl r2, r2, #4
	mov r8, r2
	ldrb r2, [r0]
	mov r1, r8
	orr r2, r1
	lsr r3, r3, #0x1F
	mul r3, r4
	add r6, r5, #0
	add r6, #0xC
	add r3, r3, r6
	mov r0, #3
	mov r1, #3
	bl TextDrawString
	mov r2, r9
	ldrb r2, [r2, #4]
	lsl r1, r2, #0x1F
	lsr r0, r1, #0x1F
	mul r0, r4
	add r0, r0, r5
	ldrb r2, [r0]
	mov r3, r8
	orr r2, r3
	lsr r1, r1, #0x1F
	add r3, r1, #0
	mul r3, r4
	add r3, r3, r6
	mov r0, #2
	mov r1, #2
	bl TextDrawString
	ldr r0, _0801A4B4 @ =0x06010000
	mov r2, r9
	ldrb r2, [r2, #4]
	lsl r1, r2, #0x1F
	lsr r1, r1, #0x1F
	mul r1, r4
	add r5, #8
	add r1, r1, r5
	ldrh r1, [r1]
	bl TextCanvasToTiles
	mov r7, #0
	mov r3, r9
	ldr r0, [r3]
	mov r1, #0xA0
	lsl r1, r1, #1
	add r0, r0, r1
	ldrh r2, [r0]
	cmp r2, #4
	bls _0801A42C
	add r7, r2, #0
	sub r7, #4
_0801A42C:
	mov r3, #0
	mov sl, r3
	mov r3, r9
	ldrh r0, [r0]
	cmp r7, r0
	blt _0801A43A
	b _0801A612
_0801A43A:
	lsl r0, r7, #2
	add r0, r0, r7
	lsl r0, r0, #2
	ldr r1, [r3]
	add r1, r1, r0
	mov r8, r1
	mov r0, #0x20
	mov r1, #4
	bl TextCanvasInit
	ldr r4, _0801A4B8 @ =0x080817C8
	mov r0, #0x22
	mov r1, #5
	ldr r2, _0801A4BC @ =0x00000A01
	add r3, r4, #0
	bl TextDrawString
	mov r0, #0x21
	mov r1, #4
	ldr r2, _0801A4C0 @ =0x00000A07
	add r3, r4, #0
	bl TextDrawString
	add r0, r4, #0
	bl StrLen
	lsl r1, r0, #2
	add r5, r1, r0
	add r0, r5, #0
	add r0, #0x22
	add r4, r7, #1
	mov r1, #5
	ldr r2, _0801A4BC @ =0x00000A01
	add r3, r4, #0
	bl TextDrawNumber
	add r0, r5, #0
	add r0, #0x21
	mov r1, #4
	ldr r2, _0801A4C0 @ =0x00000A07
	add r3, r4, #0
	bl TextDrawNumber
	mov r9, r4
	cmp r7, #9
	bgt _0801A4C4
	add r5, #0xA
	b _0801A4C6
	.align 2, 0
_0801A49C: .4byte 0x05000200
_0801A4A0: .4byte gSystemFontPal
_0801A4A4: .4byte 0x05000220
_0801A4A8: .4byte gCardIconPal
_0801A4AC: .4byte gChainListHeaders
_0801A4B0: .4byte 0x020185B8
_0801A4B4: .4byte 0x06010000
_0801A4B8: .4byte gStrLink
_0801A4BC: .4byte 0x00000A01
_0801A4C0: .4byte 0x00000A07
_0801A4C4:
	add r5, #0xF
_0801A4C6:
	mov r0, #1
	mov r1, r8
	ldrb r1, [r1, #2]
	and r0, r1
	cmp r0, #0
	beq _0801A4FC
	add r5, #4
	add r0, r5, #0
	add r0, #0x22
	ldr r4, _0801A4F0 @ =0x080817D0
	mov r1, #5
	ldr r2, _0801A4F4 @ =0x00000A01
	add r3, r4, #0
	bl TextDrawString
	add r0, r5, #0
	add r0, #0x21
	mov r1, #4
	ldr r2, _0801A4F8 @ =0x00000A04
	b _0801A516
	.align 2, 0
_0801A4F0: .4byte gStrOpposite
_0801A4F4: .4byte 0x00000A01
_0801A4F8: .4byte 0x00000A04
_0801A4FC:
	add r5, #4
	add r0, r5, #0
	add r0, #0x22
	ldr r4, _0801A618 @ =0x080817DC
	mov r1, #5
	ldr r2, _0801A61C @ =0x00000A01
	add r3, r4, #0
	bl TextDrawString
	add r0, r5, #0
	add r0, #0x21
	mov r1, #4
	ldr r2, _0801A620 @ =0x00000A06
_0801A516:
	add r3, r4, #0
	bl TextDrawString
	add r0, r4, #0
	bl StrLen
	lsl r1, r0, #2
	add r1, r1, r0
	add r5, r5, r1
	mov r0, #8
	mov r2, r8
	ldrb r2, [r2, #4]
	and r0, r2
	cmp r0, #0
	beq _0801A560
	add r5, #4
	add r0, r5, #0
	add r0, #0x22
	ldr r4, _0801A624 @ =0x080817E4
	mov r1, #5
	ldr r2, _0801A628 @ =0x00000A0B
	add r3, r4, #0
	bl TextDrawString
	add r0, r5, #0
	add r0, #0x21
	mov r1, #4
	ldr r2, _0801A62C @ =0x00000A03
	add r3, r4, #0
	bl TextDrawString
	add r0, r4, #0
	bl StrLen
	lsl r1, r0, #2
	add r1, r1, r0
	add r5, r5, r1
_0801A560:
	mov r0, #4
	mov r3, r8
	ldrb r3, [r3, #4]
	and r0, r3
	cmp r0, #0
	beq _0801A592
	add r5, #4
	add r0, r5, #0
	add r0, #0x22
	ldr r4, _0801A630 @ =0x080817F0
	mov r1, #5
	ldr r2, _0801A634 @ =0x00000A0D
	add r3, r4, #0
	bl TextDrawString
	add r0, r5, #0
	add r0, #0x21
	mov r1, #4
	ldr r2, _0801A638 @ =0x00000A05
	add r3, r4, #0
	bl TextDrawString
	add r0, r4, #0
	bl StrLen
_0801A592:
	mov r0, r8
	ldrh r0, [r0]
	lsl r3, r0, #6
	ldr r4, _0801A63C @ =0x0822C720
	add r3, r3, r4
	mov r0, #0x23
	mov r1, #0x12
	ldr r2, _0801A61C @ =0x00000A01
	bl TextDrawString
	mov r1, r8
	ldrh r1, [r1]
	lsl r3, r1, #6
	add r3, r3, r4
	mov r0, #0x22
	mov r1, #0x11
	ldr r2, _0801A640 @ =0x00000A07
	bl TextDrawString
	mov r2, sl
	lsl r4, r2, #7
	add r4, #0x40
	lsl r4, r4, #5
	ldr r3, _0801A644 @ =0x06010000
	add r0, r4, r3
	mov r1, #0
	bl TextCanvasToTiles
	mov r5, #0
	mov r6, sl
	add r6, #1
	ldr r0, _0801A644 @ =0x06010000
	add r4, r4, r0
_0801A5D4:
	mov r1, r8
	ldrh r0, [r1]
	bl GetCardIconGfx
	add r1, r0, #0
	lsl r0, r5, #7
	add r1, r1, r0
	add r0, r4, #0
	mov r2, #0x80
	bl CopyDoubleWords
	mov r2, #0x80
	lsl r2, r2, #3
	add r4, r4, r2
	add r5, #1
	cmp r5, #3
	ble _0801A5D4
	mov sl, r6
	mov r7, r9
	ldr r1, _0801A648 @ =0x020185B8
	ldr r0, [r1]
	mov r3, #0xA0
	lsl r3, r3, #1
	add r0, r0, r3
	add r3, r1, #0
	ldrh r0, [r0]
	cmp r7, r0
	bge _0801A612
	cmp r6, #3
	bgt _0801A612
	b _0801A43A
_0801A612:
	mov r0, #0
	strb r0, [r3, #5]
	b _0801A786
_0801A618: .4byte gStrYours
_0801A61C: .4byte 0x00000A01
_0801A620: .4byte 0x00000A06
_0801A624: .4byte gStrDestroyed
_0801A628: .4byte 0x00000A0B
_0801A62C: .4byte 0x00000A03
_0801A630: .4byte gStrInvalidated
_0801A634: .4byte 0x00000A0D
_0801A638: .4byte 0x00000A05
_0801A63C: .4byte gCardNames
_0801A640: .4byte 0x00000A07
_0801A644: .4byte 0x06010000
_0801A648: .4byte 0x020185B8
_0801A64C:
	bl ChainListScreen_DrawHeader
	ldr r4, _0801A694 @ =0x020185B8
	ldrb r0, [r4, #5]
	mov r2, #1
	neg r2, r2
	mov r1, #1
	bl ChainListScreen_DrawRows
	ldrb r2, [r4, #5]
	cmp r2, #0xF
	bhi _0801A6A6
	add r3, r2, #1
	strb r3, [r4, #5]
	ldr r1, _0801A698 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0801A682
	ldr r1, _0801A69C @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0801A682
	b _0801A796
_0801A682:
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	cmp r0, #7
	bhi _0801A6A0
	add r0, r2, #0
	add r0, #8
	strb r0, [r4, #5]
	b _0801A796
	.align 2, 0
_0801A694: .4byte 0x020185B8
_0801A698: .4byte 0x03000040
_0801A69C: .4byte 0x0201CFB0
_0801A6A0:
	mov r0, #0x10
	strb r0, [r4, #5]
	b _0801A796
_0801A6A6:
	mov r0, #0
	b _0801A766
_0801A6AA:
	bl ChainListScreen_DrawHeader
	mov r2, #1
	neg r2, r2
	mov r0, #0
	mov r1, #0
	bl ChainListScreen_DrawRows
	ldr r3, _0801A6DC @ =0x020185B8
	ldrb r5, [r3, #5]
	add r1, r5, #1
	strb r1, [r3, #5]
	ldr r4, _0801A6E0 @ =0x03000040
	mov r0, #3
	ldrh r2, [r4, #6]
	and r0, r2
	cmp r0, #0
	bne _0801A6D6
	lsl r0, r1, #0x18
	lsr r2, r0, #0x18
	cmp r2, #0x78
	bls _0801A6E4
_0801A6D6:
	mov r0, #0x10
	strb r0, [r3, #5]
	b _0801A786
_0801A6DC: .4byte 0x020185B8
_0801A6E0: .4byte 0x03000040
_0801A6E4:
	mov r0, #2
	ldrh r4, [r4, #4]
	and r0, r4
	cmp r0, #0
	bne _0801A6FA
	ldr r1, _0801A704 @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801A796
_0801A6FA:
	cmp r2, #0x67
	bhi _0801A708
	add r0, r5, #0
	add r0, #0x11
	b _0801A70A
_0801A704: .4byte 0x0201CFB0
_0801A708:
	mov r0, #0x78
_0801A70A:
	strb r0, [r3, #5]
	b _0801A796
_0801A70E:
	bl ChainListScreen_DrawHeader
	ldr r4, _0801A754 @ =0x020185B8
	ldrb r0, [r4, #5]
	mov r2, #1
	neg r2, r2
	mov r1, #1
	bl ChainListScreen_DrawRows
	ldrb r2, [r4, #5]
	add r0, r2, #0
	cmp r0, #0
	beq _0801A766
	sub r3, r2, #1
	strb r3, [r4, #5]
	ldr r1, _0801A758 @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _0801A744
	ldr r1, _0801A75C @ =0x0201CFB0
	mov r0, #1
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0801A796
_0801A744:
	lsl r0, r3, #0x18
	lsr r0, r0, #0x18
	cmp r0, #8
	bls _0801A760
	add r0, r2, #0
	sub r0, #9
	strb r0, [r4, #5]
	b _0801A796
_0801A754: .4byte 0x020185B8
_0801A758: .4byte 0x03000040
_0801A75C: .4byte 0x0201CFB0
_0801A760:
	mov r0, #0
	strb r0, [r4, #5]
	b _0801A796
_0801A766:
	strb r0, [r4, #5]
	ldrb r2, [r4, #4]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r4, #4]
	b _0801A796
_0801A77A:
	mov r0, #1
	bl DuelFieldFadeFromBlack
_0801A780:
	cmp r0, #0
	beq _0801A796
_0801A784:
	ldr r3, _0801A79C @ =0x020185B8
_0801A786:
	ldrb r2, [r3, #4]
	lsr r1, r2, #1
	add r1, #1
	lsl r1, r1, #1
	mov r0, #1
	and r0, r2
	orr r0, r1
	strb r0, [r3, #4]
_0801A796:
	mov r0, #0
	b _0801A7A6
	.align 2, 0
_0801A79C: .4byte 0x020185B8
_0801A7A0:
	bl LoadDuelUiGfx
	mov r0, #1
_0801A7A6:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end ChainListScreen_Run

