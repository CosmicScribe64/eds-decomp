	thumb_func_start CardListView_InitScreen
CardListView_InitScreen: @ 0x0802A6DC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r0, _0802A6FC @ =0x0201D810
	ldrb r1, [r0, #3]
	add r4, r0, #0
	cmp r1, #4
	bls _0802A6F2
	b _0802AAB0
_0802A6F2:
	lsl r0, r1, #2
	ldr r1, _0802A700 @ =0x0802A704
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
_0802A6FC: .4byte 0x0201D810
_0802A700: .4byte 0x0802A704
_0802A704:
	.4byte _0802A718
	.4byte _0802A748
	.4byte _0802A7A0
	.4byte _0802A980
	.4byte _0802AA84
_0802A718:
	bl DuelScreen_FadeOutStep
	cmp r0, #0
	bne _0802A722
	b _0802AA68
_0802A722:
	ldr r2, _0802A740 @ =0x0201CFB0
	mov r0, #5
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	neg r1, r1
	and r0, r1
	strb r0, [r2]
	ldr r1, _0802A744 @ =0x0201D810
	ldrb r0, [r1, #3]
	add r0, #1
	strb r0, [r1, #3]
	b _0802AA68
	.align 2, 0
_0802A740: .4byte 0x0201CFB0
_0802A744: .4byte 0x0201D810
_0802A748:
	bl ResetVideo
	bl ClearBgMapBuffers
	ldr r0, _0802A788 @ =0x03000040
	ldr r2, _0802A78C @ =0x0000040E
	add r0, r0, r2
	mov r2, #0
	ldr r1, _0802A790 @ =0x00000303
	strh r1, [r0]
	mov r0, #0x80
	lsl r0, r0, #0x13
	strh r2, [r0]
	add r0, #0x50
	strh r2, [r0]
	ldr r1, _0802A794 @ =0x04000008
	mov r0, #4
	strh r0, [r1]
	add r1, #2
	mov r3, #0x82
	lsl r3, r3, #1
	add r0, r3, #0
	strh r0, [r1]
	add r1, #2
	ldr r4, _0802A798 @ =0x00000206
	add r0, r4, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0802A79C @ =0x00000387
	add r0, r2, #0
	strh r0, [r1]
	b _0802AAA0
_0802A788: .4byte 0x03000040
_0802A78C: .4byte 0x0000040E
_0802A790: .4byte 0x00000303
_0802A794: .4byte 0x04000008
_0802A798: .4byte 0x00000206
_0802A79C: .4byte 0x00000387
_0802A7A0:
	bl SetBrightnessBlack
	bl ResetBgScroll
	ldr r0, _0802A91C @ =0x05000200
	ldr r1, _0802A920 @ =0x08698C7C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _0802A924 @ =0x06010000
	ldr r1, _0802A928 @ =0x08698C9C
	mov r2, #0x80
	lsl r2, r2, #6
	bl CopyDoubleWords
	ldr r0, _0802A92C @ =0x05000220
	ldr r1, _0802A930 @ =0x0869AD1C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _0802A934 @ =0x06012000
	ldr r1, _0802A938 @ =0x0869AD3C
	mov r2, #0x80
	lsl r2, r2, #4
	bl CopyDoubleWords
	mov r0, #0x80
	lsl r0, r0, #3
	ldr r2, _0802A93C @ =0x000003C6
	ldr r3, _0802A940 @ =0x0869E8E4
	mov r1, #0x10
	bl LoadBgImage4bppMap1
	mov r0, #0x88
	lsl r0, r0, #3
	mov r2, #0xD5
	lsl r2, r2, #2
	ldr r3, _0802A944 @ =0x0869C45C
	mov r1, #0x20
	bl LoadBgImage4bppMap1
	mov r0, #0xAC
	lsl r0, r0, #3
	mov r2, #0xB8
	lsl r2, r2, #2
	ldr r3, _0802A948 @ =0x0869D758
	mov r1, #0x30
	bl LoadBgImage4bppMap1
	ldr r0, _0802A94C @ =0x05000080
	ldr r1, _0802A950 @ =0x0869EECC
	mov r2, #0x20
	bl MemCopy16
	ldr r7, _0802A954 @ =0x06004000
	ldr r6, _0802A958 @ =0x0869EEEC
	mov r3, #0
	mov sl, r3
	mov r9, r3
	mov r4, #1
	mov r8, r4
_0802A81A:
	mov r0, r9
	lsr r5, r0, #0x10
	mov r1, sl
	lsr r4, r1, #0x10
	mov r2, #0xAD
	lsl r2, r2, #1
	add r0, r4, r2
	lsl r0, r0, #5
	add r0, r0, r7
	lsl r1, r5, #5
	add r1, r1, r6
	sub r2, #0x5A
	bl CopyDoubleWords
	mov r3, #0xB1
	lsl r3, r3, #1
	add r0, r4, r3
	lsl r0, r0, #5
	add r0, r0, r7
	add r1, r5, #0
	add r1, #0x1C
	lsl r1, r1, #5
	add r1, r1, r6
	mov r2, #0x40
	bl CopyDoubleWords
	mov r1, #0xB2
	lsl r1, r1, #1
	add r0, r4, r1
	lsl r0, r0, #5
	add r0, r0, r7
	add r1, r5, #0
	add r1, #0x20
	lsl r1, r1, #5
	add r1, r1, r6
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r2, _0802A95C @ =0x00000165
	add r0, r4, r2
	lsl r0, r0, #5
	add r0, r0, r7
	add r1, r5, #0
	add r1, #0x3D
	lsl r1, r1, #5
	add r1, r1, r6
	mov r2, #0x20
	bl CopyDoubleWords
	mov r3, #0xB3
	lsl r3, r3, #1
	add r0, r4, r3
	lsl r0, r0, #5
	add r0, r0, r7
	add r1, r5, #0
	add r1, #0x40
	lsl r1, r1, #5
	add r1, r1, r6
	mov r2, #0x40
	bl CopyDoubleWords
	mov r1, #0xB4
	lsl r1, r1, #1
	add r0, r4, r1
	lsl r0, r0, #5
	add r0, r0, r7
	add r1, r5, #0
	add r1, #0x5D
	lsl r1, r1, #5
	add r1, r1, r6
	mov r2, #0x20
	bl CopyDoubleWords
	mov r2, #0xF0
	lsl r2, r2, #0xC
	add sl, r2
	mov r3, #0xC0
	lsl r3, r3, #0xF
	add r9, r3
	mov r4, #1
	neg r4, r4
	add r8, r4
	mov r0, r8
	cmp r0, #0
	bge _0802A81A
	ldr r0, _0802A960 @ =0x050000E0
	ldr r1, _0802A964 @ =0x0869B53C
	mov r2, #0x20
	bl CopyDoubleWords
	ldr r0, _0802A968 @ =0x0201D810
	ldrb r0, [r0]
	lsl r2, r0, #0x18
	lsr r0, r2, #0x1D
	cmp r0, #4
	bls _0802A8DC
	b _0802AAA0
_0802A8DC:
	ldr r0, _0802A96C @ =0x06006840
	lsr r2, r2, #0x1D
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #8
	ldr r2, _0802A970 @ =0x0869B55C
	add r1, r1, r2
	mov r2, #0xC0
	lsl r2, r2, #2
	bl CopyDoubleWords
	mov r3, #0
	ldr r4, _0802A974 @ =0x0300045C
	ldr r0, _0802A978 @ =0x00007142
	add r6, r0, #0
	ldr r1, _0802A97C @ =0x0000714E
	add r5, r1, #0
_0802A8FE:
	lsl r0, r3, #0x10
	lsr r0, r0, #0x10
	lsl r1, r0, #1
	add r1, r1, r4
	add r2, r3, r6
	strh r2, [r1]
	add r0, #0x20
	lsl r0, r0, #1
	add r0, r0, r4
	add r1, r3, r5
	strh r1, [r0]
	add r3, #1
	cmp r3, #0xB
	ble _0802A8FE
	b _0802AAA0
_0802A91C: .4byte 0x05000200
_0802A920: .4byte gCardListViewButtonsPal
_0802A924: .4byte 0x06010000
_0802A928: .4byte gCardListViewButtonsGfx
_0802A92C: .4byte 0x05000220
_0802A930: .4byte gCardListViewStatusIconsPal
_0802A934: .4byte 0x06012000
_0802A938: .4byte gCardListViewStatusIconsGfx
_0802A93C: .4byte 0x000003C6
_0802A940: .4byte gCardListViewHeaderImage
_0802A944: .4byte gCardListViewBgImage
_0802A948: .4byte gCardListViewInfoPanelImage
_0802A94C: .4byte 0x05000080
_0802A950: .4byte gCardListViewCursorFramePal
_0802A954: .4byte 0x06004000
_0802A958: .4byte gCardListViewCursorFrameGfx
_0802A95C: .4byte 0x00000165
_0802A960: .4byte 0x050000E0
_0802A964: .4byte gCardListViewTitlesPal
_0802A968: .4byte 0x0201D810
_0802A96C: .4byte 0x06006840
_0802A970: .4byte gCardListViewTitlesGfx
_0802A974: .4byte 0x0300045C
_0802A978: .4byte 0x00007142
_0802A97C: .4byte 0x0000714E
_0802A980:
	mov r2, #0xC3
	lsl r2, r2, #2
	add r0, r4, r2
	ldrh r0, [r0]
	cmp r0, #0
	beq _0802A9D0
	bl CardListView_DrawPage
	bl CardListView_DrawSelectedInfo
	bl CardListView_DrawSelectedCursorFrame
	ldr r1, _0802A9C8 @ =0x03000040
	ldrb r3, [r4, #5]
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1A
	neg r0, r0
	ldr r2, _0802A9CC @ =0x00004422
	add r1, r1, r2
	strh r0, [r1]
	mov r0, #4
	neg r0, r0
	ldrb r3, [r4, #8]
	and r0, r3
	lsl r1, r0, #0x1A
	lsr r1, r1, #0x1C
	mov r2, #1
	orr r2, r1
	lsl r2, r2, #2
	mov r1, #0x3D
	neg r1, r1
	and r0, r1
	orr r0, r2
	strb r0, [r4, #8]
	b _0802AA14
	.align 2, 0
_0802A9C8: .4byte 0x03000040
_0802A9CC: .4byte 0x00004422
_0802A9D0:
	mov r0, #0x20
	mov r1, #9
	bl TextCanvasInit
	ldr r2, _0802AA6C @ =0x08082768
	mov r0, #0x42
	mov r1, #0x18
	mov r3, #0xC
	bl TextDrawShadowedString
	mov r1, #0
	ldr r3, _0802AA70 @ =0x0000011F
	ldr r0, _0802AA74 @ =0x03000040
	ldr r4, _0802AA78 @ =0x0000049C
	add r2, r0, r4
_0802A9EE:
	add r0, r1, #0
	add r0, #0x10
	strh r0, [r2]
	add r2, #2
	add r1, #1
	cmp r1, r3
	ble _0802A9EE
	ldr r0, _0802AA7C @ =0x06004200
	mov r1, #0
	bl TextCanvasToTiles
	ldr r2, _0802AA80 @ =0x0201D810
	mov r0, #4
	neg r0, r0
	ldrb r1, [r2, #8]
	and r0, r1
	mov r1, #1
	orr r0, r1
	strb r0, [r2, #8]
_0802AA14:
	ldr r2, _0802AA80 @ =0x0201D810
	ldrb r3, [r2, #8]
	lsl r1, r3, #0x1A
	lsr r1, r1, #0x1C
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1E
	asr r1, r0
	mov r0, #1
	and r1, r0
	add r4, r2, #0
	cmp r1, #0
	bne _0802AA5A
	mov r8, r4
	mov r7, #4
	neg r7, r7
	mov r6, #3
	mov r5, #1
_0802AA36:
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1E
	add r0, #1
	and r0, r6
	add r2, r7, #0
	and r2, r3
	orr r2, r0
	add r3, r2, #0
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1C
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	asr r1, r0
	and r1, r5
	cmp r1, #0
	beq _0802AA36
	mov r3, r8
	strb r2, [r3, #8]
_0802AA5A:
	mov r0, #0x10
	ldrb r1, [r4]
	orr r0, r1
	strb r0, [r4]
	ldrb r0, [r4, #3]
	add r0, #1
	strb r0, [r4, #3]
_0802AA68:
	mov r0, #0
	b _0802AAB2
_0802AA6C: .4byte gStrCardListViewNoCards
_0802AA70: .4byte 0x0000011F
_0802AA74: .4byte 0x03000040
_0802AA78: .4byte 0x0000049C
_0802AA7C: .4byte 0x06004200
_0802AA80: .4byte 0x0201D810
_0802AA84:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r0, [r2]
	mov r3, #0xF8
	lsl r3, r3, #5
	add r1, r3, #0
	orr r0, r1
	strh r0, [r2]
	mov r0, #4
	bl FadeFromBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802AA68
_0802AAA0:
	ldr r1, _0802AAAC @ =0x0201D810
	ldrb r0, [r1, #3]
	add r0, #1
	strb r0, [r1, #3]
	b _0802AA68
	.align 2, 0
_0802AAAC: .4byte 0x0201D810
_0802AAB0:
	mov r0, #1
_0802AAB2:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end CardListView_InitScreen

