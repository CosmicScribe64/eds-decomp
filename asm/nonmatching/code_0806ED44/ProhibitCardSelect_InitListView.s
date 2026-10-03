	thumb_func_start ProhibitCardSelect_InitListView
ProhibitCardSelect_InitListView: @ 0x0806F404
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	mov r4, #0
	str r4, [sp, #8]
	mov r5, #0xC0
	lsl r5, r5, #0x13
	ldr r2, _0806F774 @ =0x01004000
	add r0, sp, #8
	add r1, r5, #0
	bl CpuFastSet
	str r4, [sp, #0xC]
	add r0, sp, #0xC
	ldr r6, _0806F778 @ =0x06010000
	ldr r2, _0806F77C @ =0x01002000
	add r1, r6, #0
	bl CpuFastSet
	ldr r0, _0806F780 @ =0x087025D8
	ldr r1, _0806F784 @ =0x0600E000
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r0, _0806F788 @ =0x08702C08
	mov r4, #0x80
	lsl r4, r4, #4
	add r1, r5, #0
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _0806F78C @ =0x086ED3B0
	ldr r1, _0806F790 @ =0x06004000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _0806F794 @ =0x086E5030
	add r1, r6, #0
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _0806F798 @ =0x086E7030
	ldr r1, _0806F79C @ =0x06010200
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _0806F7A0 @ =0x086E9030
	ldr r1, _0806F7A4 @ =0x06014000
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _0806F7A8 @ =0x086EB030
	ldr r1, _0806F7AC @ =0x06014200
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _0806F7B0 @ =0x06006000
	bl sub_08066164
	ldr r0, _0806F7B4 @ =0x081A70FC
	ldr r5, _0806F7B8 @ =0x0201F238
	add r1, r5, #0
	bl AnimBlockInit
	mov r1, #0xFF
	ldrb r0, [r5, #0xE]
	orr r0, r1
	strb r0, [r5, #0xE]
	add r2, r5, #0
	add r2, #0x22
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x28
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r2, #0x14
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x89
	lsl r0, r0, #1
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	mov r3, #0x93
	lsl r3, r3, #1
	add r2, r5, r3
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x9D
	lsl r0, r0, #1
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	mov r2, #0xA7
	lsl r2, r2, #1
	add r0, r5, r2
	ldrb r3, [r0]
	orr r1, r3
	strb r1, [r0]
	ldr r0, _0806F7BC @ =0x08702A88
	ldr r1, _0806F7C0 @ =0x05000080
	mov r2, #0x20
	bl CpuFastSet
	ldr r0, _0806F7C4 @ =0x08702B08
	ldr r1, _0806F7C8 @ =0x05000020
	mov r2, #0x18
	bl CpuFastSet
	ldr r0, _0806F7CC @ =0x08702BE8
	ldr r1, _0806F7D0 @ =0x05000060
	mov r2, #8
	bl CpuFastSet
	ldr r0, _0806F7D4 @ =0x086ED1B0
	ldr r1, _0806F7D8 @ =0x05000200
	mov r2, #0x80
	lsl r2, r2, #1
	bl CpuSet
	ldr r1, _0806F7DC @ =0x05000044
	ldr r2, _0806F7E0 @ =0x00007758
	add r0, r2, #0
	strh r0, [r1]
	ldr r0, _0806F7E4 @ =0x06002000
	bl DeckEdit_LoadCardIconTiles
	ldr r0, _0806F7E8 @ =0x08704EE8
	mov r1, #0xA0
	lsl r1, r1, #0x13
	mov r2, #0x10
	bl CpuSet
	mov r2, #1
	mov r7, #0
	ldr r3, _0806F7EC @ =0xFFFFE8E8
	add r3, r3, r5
	mov r8, r3
	mov r0, #0xC4
	lsl r0, r0, #3
	add r0, r8
	mov sl, r0
	mov r1, #0xA5
	lsl r1, r1, #5
	add r1, r8
	mov r9, r1
_0806F57C:
	ldr r0, _0806F7F0 @ =0x0201F73C
	ldrb r3, [r0]
	lsl r0, r3, #1
	mov r1, sl
	add r6, r0, r1
	mov r0, #0
	ldsh r1, [r6, r0]
	lsl r0, r2, #0x10
	asr r5, r0, #0x10
	add r1, r1, r5
	sub r1, #3
	cmp r1, #0
	blt _0806F5F2
	mov r1, r9
	add r0, r3, r1
	ldrb r1, [r0]
	ldrh r6, [r6]
	add r2, r6, r5
	sub r2, #3
	add r0, r3, #0
	bl DeckEdit_GetListCard
	sub r4, r5, #1
	lsl r3, r4, #1
	mov r1, #0xC8
	lsl r1, r1, #3
	add r1, r8
	str r1, [sp, #0]
	neg r1, r5
	add r1, #2
	str r1, [sp, #4]
	ldr r1, _0806F7F4 @ =0x0600D000
	mov r2, #0
	bl DeckEdit_DrawListRowName
	ldr r2, _0806F7F0 @ =0x0201F73C
	ldrb r0, [r2]
	mov r3, r9
	add r1, r0, r3
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, sl
	ldrh r2, [r2]
	add r2, r2, r5
	sub r2, #3
	bl DeckEdit_GetListCard
	add r1, r0, #0
	ldr r0, _0806F7F8 @ =0x0201F3D0
	str r0, [sp, #0]
	add r0, r4, #0
	add r2, r5, #0
	ldr r3, _0806F7FC @ =0x00001BB8
	add r3, r8
	bl DeckEdit_InitFrameSlot
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
_0806F5F2:
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, #1
	bls _0806F57C
	mov r7, #0
	ldr r1, _0806F800 @ =0x0201DB20
	mov r8, r1
	mov r2, #0xA5
	lsl r2, r2, #5
	add r2, r8
	mov sl, r2
_0806F60A:
	ldr r3, _0806F7F0 @ =0x0201F73C
	ldrb r4, [r3]
	lsl r1, r4, #1
	ldr r2, _0806F804 @ =0x0201E140
	add r0, r1, r2
	ldrh r6, [r0]
	add r2, r6, r7
	mov r3, sl
	add r0, r4, r3
	ldrb r3, [r0]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r1, r1, r0
	ldr r0, _0806F808 @ =0x00001494
	add r0, r8
	mov r9, r0
	add r1, r9
	ldrh r0, [r1]
	sub r0, #1
	add r5, r7, #1
	cmp r2, r0
	bge _0806F686
	add r2, r6, r5
	add r0, r4, #0
	add r1, r3, #0
	bl DeckEdit_GetListCard
	lsl r3, r7, #1
	add r3, #9
	mov r1, #0xC8
	lsl r1, r1, #3
	add r1, r8
	str r1, [sp, #0]
	add r4, r7, #4
	str r4, [sp, #4]
	ldr r1, _0806F7F4 @ =0x0600D000
	mov r2, #0
	bl DeckEdit_DrawListRowName
	ldr r1, _0806F7F0 @ =0x0201F73C
	ldrb r0, [r1]
	mov r2, sl
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r2, r0, #1
	ldr r3, _0806F804 @ =0x0201E140
	add r2, r2, r3
	ldrh r2, [r2]
	add r2, r2, r7
	add r2, #1
	bl DeckEdit_GetListCard
	add r1, r0, #0
	add r0, r7, #3
	ldr r2, _0806F7F8 @ =0x0201F3D0
	str r2, [sp, #0]
	add r2, r4, #0
	ldr r3, _0806F7FC @ =0x00001BB8
	add r3, r8
	bl DeckEdit_InitFrameSlot
_0806F686:
	lsl r0, r5, #0x18
	lsr r7, r0, #0x18
	cmp r7, #1
	bls _0806F60A
	mov r7, r8
	ldr r0, _0806F7F0 @ =0x0201F73C
	ldrb r3, [r0]
	lsl r2, r3, #1
	mov r1, #0xA5
	lsl r1, r1, #5
	add r1, r1, r7
	mov sl, r1
	add r0, r3, r1
	ldrb r1, [r0]
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r2, r0
	add r0, r9
	ldrh r0, [r0]
	cmp r0, #0
	bne _0806F6B4
	b _0806F824
_0806F6B4:
	mov r0, #0xC4
	lsl r0, r0, #3
	add r5, r7, r0
	add r0, r2, r5
	ldrh r2, [r0]
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r6, _0806F80C @ =0x0600C000
	ldr r2, _0806F810 @ =0x0000063E
	add r1, r7, r2
	ldrh r3, [r1]
	add r3, #0x20
	mov r4, #0xFF
	and r3, r4
	asr r3, r3, #3
	mov r1, #0xC8
	lsl r1, r1, #3
	add r1, r1, r7
	mov r8, r1
	str r1, [sp, #0]
	add r1, r6, #0
	mov r2, #0
	bl DeckEdit_DrawCursorRowName
	ldr r2, _0806F7F0 @ =0x0201F73C
	ldrb r0, [r2]
	mov r3, sl
	add r1, r0, r3
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, r2, r5
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	add r1, r0, #0
	ldr r0, _0806F7FC @ =0x00001BB8
	add r3, r7, r0
	ldr r2, _0806F814 @ =0x000018B0
	add r0, r7, r2
	str r0, [sp, #0]
	mov r0, #2
	mov r2, #3
	bl DeckEdit_InitFrameSlot
	ldr r3, _0806F7F0 @ =0x0201F73C
	ldrb r0, [r3]
	mov r2, sl
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, r2, r5
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	ldr r3, _0806F818 @ =0x00000634
	add r5, r7, r3
	ldrb r2, [r5]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r3, _0806F81C @ =0x06008000
	add r1, r1, r3
	bl LoadCardArt8bpp
	ldr r1, _0806F820 @ =0x00000632
	add r0, r7, r1
	ldrh r0, [r0]
	and r4, r0
	lsr r4, r4, #3
	add r4, #2
	ldrb r2, [r5]
	mov r0, #0x13
	add r1, r4, #0
	mov r3, #1
	bl DeckEdit_PlaceCardArt
	mov r0, #0
	bl DeckEdit_DrawCardIcons
	add r0, r6, #0
	mov r1, #0xB
	mov r2, #7
	mov r3, r8
	bl DeckEdit_DrawAtkDef
	add r0, r6, #0
	mov r1, #0x11
	mov r2, #7
	mov r3, #6
	bl DeckEdit_DrawLevelStars
	b _0806F84E
	.align 2, 0
_0806F774: .4byte 0x01004000
_0806F778: .4byte 0x06010000
_0806F77C: .4byte 0x01002000
_0806F780: .4byte gProhibitSelectFrameMap
_0806F784: .4byte 0x0600E000
_0806F788: .4byte gProhibitSelectBgTiles
_0806F78C: .4byte gDeckEditLabelTiles
_0806F790: .4byte 0x06004000
_0806F794: .4byte gDeckEditObjTiles
_0806F798: .4byte gDeckEditCardStackObjTiles
_0806F79C: .4byte 0x06010200
_0806F7A0: .4byte gDeckEditCardIconObjTiles
_0806F7A4: .4byte 0x06014000
_0806F7A8: .4byte gDeckEditCardFrameObjTiles
_0806F7AC: .4byte 0x06014200
_0806F7B0: .4byte 0x06006000
_0806F7B4: .4byte gDeckEditAnimScripts
_0806F7B8: .4byte 0x0201F238
_0806F7BC: .4byte gProhibitSelectBgPals4to7
_0806F7C0: .4byte 0x05000080
_0806F7C4: .4byte gProhibitSelectBgPals1to3
_0806F7C8: .4byte 0x05000020
_0806F7CC: .4byte gProhibitSelectBgPal3
_0806F7D0: .4byte 0x05000060
_0806F7D4: .4byte gDeckEditObjPal
_0806F7D8: .4byte 0x05000200
_0806F7DC: .4byte 0x05000044
_0806F7E0: .4byte 0x00007758
_0806F7E4: .4byte 0x06002000
_0806F7E8: .4byte gUnk_08704EE8
_0806F7EC: .4byte 0xFFFFE8E8
_0806F7F0: .4byte 0x0201F73C
_0806F7F4: .4byte 0x0600D000
_0806F7F8: .4byte 0x0201F3D0
_0806F7FC: .4byte 0x00001BB8
_0806F800: .4byte 0x0201DB20
_0806F804: .4byte 0x0201E140
_0806F808: .4byte 0x00001494
_0806F80C: .4byte 0x0600C000
_0806F810: .4byte 0x0000063E
_0806F814: .4byte 0x000018B0
_0806F818: .4byte 0x00000634
_0806F81C: .4byte 0x06008000
_0806F820: .4byte 0x00000632
_0806F824:
	ldr r0, _0806F8F4 @ =0xFFFFF18C
	add r0, r9
	add r0, r2, r0
	ldrh r2, [r0]
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r1, _0806F8F8 @ =0x0600C000
	ldr r2, _0806F8FC @ =0xFFFFF1AA
	add r2, r9
	ldrh r3, [r2]
	add r3, #0x20
	mov r2, #0xFF
	and r3, r2
	asr r3, r3, #3
	ldr r2, _0806F900 @ =0xFFFFF1AC
	add r2, r9
	str r2, [sp, #0]
	mov r2, #0
	bl DeckEdit_DrawNoCardsText
_0806F84E:
	ldr r0, _0806F904 @ =0x04000040
	mov r2, #0xF0
	strh r2, [r0]
	ldr r1, _0806F908 @ =0x04000044
	ldr r3, _0806F90C @ =0x0000244C
	add r0, r3, #0
	strh r0, [r1]
	ldr r0, _0806F910 @ =0x04000042
	strh r2, [r0]
	add r1, #2
	mov r0, #0x70
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806F914 @ =0x00003E3D
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	mov r0, #0x3C
	strh r0, [r1]
	sub r1, #0x42
	mov r3, #0xC0
	lsl r3, r3, #5
	add r0, r3, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806F918 @ =0x00001A01
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r3, _0806F91C @ =0x00001C02
	add r0, r3, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806F920 @ =0x00001E8B
	add r0, r2, #0
	strh r0, [r1]
	ldr r1, _0806F924 @ =0xFFFFFE80
	ldr r3, _0806F928 @ =0x0201E138
	mov r0, #0
	mov r2, #0
	bl FadeStart
	ldr r0, _0806F92C @ =0x04000010
	mov r1, #0
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	add r0, #2
	strh r1, [r0]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r3, #0xFE
	lsl r3, r3, #7
	add r0, r3, #0
	strh r0, [r1]
	ldr r0, _0806F930 @ =0x06006000
	mov r1, #0xC0
	lsl r1, r1, #2
	mov r2, #0
	str r2, [sp, #0]
	mov r2, #1
	mov r3, #0
	bl LoadDigitTiles
	mov r0, #1
	add sp, #0x10
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0806F8F4: .4byte 0xFFFFF18C
_0806F8F8: .4byte 0x0600C000
_0806F8FC: .4byte 0xFFFFF1AA
_0806F900: .4byte 0xFFFFF1AC
_0806F904: .4byte 0x04000040
_0806F908: .4byte 0x04000044
_0806F90C: .4byte 0x0000244C
_0806F910: .4byte 0x04000042
_0806F914: .4byte 0x00003E3D
_0806F918: .4byte 0x00001A01
_0806F91C: .4byte 0x00001C02
_0806F920: .4byte 0x00001E8B
_0806F924: .4byte 0xFFFFFE80
_0806F928: .4byte 0x0201E138
_0806F92C: .4byte 0x04000010
_0806F930: .4byte 0x06006000
	thumb_func_end ProhibitCardSelect_InitListView

