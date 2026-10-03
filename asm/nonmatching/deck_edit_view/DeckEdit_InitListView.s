	thumb_func_start DeckEdit_InitListView
DeckEdit_InitListView: @ 0x0806D644
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x10
	mov r0, #0
	mov r8, r0
	str r0, [sp, #8]
	mov r6, #0xC0
	lsl r6, r6, #0x13
	ldr r2, _0806D9CC @ =0x01004000
	add r0, sp, #8
	add r1, r6, #0
	bl CpuFastSet
	mov r1, r8
	str r1, [sp, #0xC]
	add r0, sp, #0xC
	ldr r5, _0806D9D0 @ =0x06010000
	ldr r2, _0806D9D4 @ =0x01002000
	add r1, r5, #0
	bl CpuFastSet
	ldr r0, _0806D9D8 @ =0x086E26D0
	ldr r1, _0806D9DC @ =0x0600E000
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r0, _0806D9E0 @ =0x086E3030
	mov r4, #0x80
	lsl r4, r4, #4
	add r1, r6, #0
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _0806D9E4 @ =0x086ED3B0
	ldr r1, _0806D9E8 @ =0x06004000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _0806D9EC @ =0x086E5030
	add r1, r5, #0
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _0806D9F0 @ =0x086E7030
	ldr r1, _0806D9F4 @ =0x06010200
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _0806D9F8 @ =0x086E9030
	ldr r1, _0806D9FC @ =0x06014000
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _0806DA00 @ =0x086EB030
	ldr r1, _0806DA04 @ =0x06014200
	mov r2, #0x10
	bl CopyTileSheetTo2D
	add r0, r5, #0
	mov r1, #0x8A
	bl ClearTile4bpp
	add r0, r5, #0
	mov r1, #0xAA
	bl ClearTile4bpp
	add r0, r5, #0
	mov r1, #0xCA
	bl ClearTile4bpp
	add r0, r5, #0
	mov r1, #0xEA
	bl ClearTile4bpp
	ldr r0, _0806DA08 @ =0x06006000
	bl sub_08066164
	ldr r0, _0806DA0C @ =0x081A70FC
	ldr r5, _0806DA10 @ =0x0201F238
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
	add r0, r5, #0
	add r0, #0xD6
	mov r2, r8
	strb r2, [r0]
	add r0, #0x14
	strb r2, [r0]
	add r0, #0x14
	strb r2, [r0]
	mov r3, #0x89
	lsl r3, r3, #1
	add r2, r5, r3
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	mov r0, #0x93
	lsl r0, r0, #1
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	add r3, #0x28
	add r2, r5, r3
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	mov r2, #0xA7
	lsl r2, r2, #1
	add r0, r5, r2
	ldrb r3, [r0]
	orr r1, r3
	strb r1, [r0]
	ldr r0, _0806DA14 @ =0x086ED030
	ldr r1, _0806DA18 @ =0x05000080
	mov r2, #0x20
	bl CpuFastSet
	ldr r0, _0806DA1C @ =0x086ED0B0
	ldr r1, _0806DA20 @ =0x05000020
	mov r2, #0x18
	bl CpuFastSet
	ldr r0, _0806DA24 @ =0x086ED190
	ldr r1, _0806DA28 @ =0x05000060
	mov r2, #8
	bl CpuFastSet
	ldr r0, _0806DA2C @ =0x086ED1B0
	ldr r1, _0806DA30 @ =0x05000200
	mov r2, #0x80
	lsl r2, r2, #1
	bl CpuSet
	ldr r1, _0806DA34 @ =0x05000044
	ldr r2, _0806DA38 @ =0x00007758
	add r0, r2, #0
	strh r0, [r1]
	ldr r0, _0806DA3C @ =0x06002000
	bl DeckEdit_LoadCardIconTiles
	ldr r0, _0806DA40 @ =0x08704EE8
	mov r1, #0xA0
	lsl r1, r1, #0x13
	mov r2, #0x10
	bl CpuSet
	mov r2, #1
	mov r7, #0
	ldr r3, _0806DA44 @ =0xFFFFE8E8
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
_0806D7D6:
	ldr r0, _0806DA48 @ =0x0201F73C
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
	blt _0806D846
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
	ldr r1, _0806DA4C @ =0x0600D000
	mov r2, #0
	bl DeckEdit_DrawListRowName
	ldr r2, _0806DA48 @ =0x0201F73C
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
	ldr r0, _0806DA50 @ =0x0201F3D0
	str r0, [sp, #0]
	add r0, r4, #0
	add r2, r5, #0
	ldr r3, _0806DA54 @ =0x00001BB8
	add r3, r8
	bl DeckEdit_InitFrameSlot
_0806D846:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	add r0, r7, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r7, #1
	bls _0806D7D6
	mov r7, #0
	ldr r1, _0806DA58 @ =0x0201DB20
	mov r8, r1
	mov r2, #0xA5
	lsl r2, r2, #5
	add r2, r8
	mov sl, r2
_0806D864:
	ldr r3, _0806DA48 @ =0x0201F73C
	ldrb r4, [r3]
	lsl r1, r4, #1
	ldr r2, _0806DA5C @ =0x0201E140
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
	ldr r0, _0806DA60 @ =0x00001494
	add r0, r8
	mov r9, r0
	add r1, r9
	ldrh r0, [r1]
	sub r0, #1
	add r5, r7, #1
	cmp r2, r0
	bge _0806D8E0
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
	ldr r1, _0806DA4C @ =0x0600D000
	mov r2, #0
	bl DeckEdit_DrawListRowName
	ldr r1, _0806DA48 @ =0x0201F73C
	ldrb r0, [r1]
	mov r2, sl
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r2, r0, #1
	ldr r3, _0806DA5C @ =0x0201E140
	add r2, r2, r3
	ldrh r2, [r2]
	add r2, r2, r7
	add r2, #1
	bl DeckEdit_GetListCard
	add r1, r0, #0
	add r0, r7, #3
	ldr r2, _0806DA50 @ =0x0201F3D0
	str r2, [sp, #0]
	add r2, r4, #0
	ldr r3, _0806DA54 @ =0x00001BB8
	add r3, r8
	bl DeckEdit_InitFrameSlot
_0806D8E0:
	lsl r0, r5, #0x18
	lsr r7, r0, #0x18
	cmp r7, #1
	bls _0806D864
	mov r7, r8
	ldr r0, _0806DA48 @ =0x0201F73C
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
	bne _0806D90E
	b _0806DA7C
_0806D90E:
	mov r0, #0xC4
	lsl r0, r0, #3
	add r5, r7, r0
	add r0, r2, r5
	ldrh r2, [r0]
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r6, _0806DA64 @ =0x0600C000
	ldr r2, _0806DA68 @ =0x0000063E
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
	ldr r2, _0806DA48 @ =0x0201F73C
	ldrb r0, [r2]
	mov r3, sl
	add r1, r0, r3
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, r2, r5
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	add r1, r0, #0
	ldr r0, _0806DA54 @ =0x00001BB8
	add r3, r7, r0
	ldr r2, _0806DA6C @ =0x000018B0
	add r0, r7, r2
	str r0, [sp, #0]
	mov r0, #2
	mov r2, #3
	bl DeckEdit_InitFrameSlot
	ldr r3, _0806DA48 @ =0x0201F73C
	ldrb r0, [r3]
	mov r2, sl
	add r1, r0, r2
	ldrb r1, [r1]
	lsl r2, r0, #1
	add r2, r2, r5
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	ldr r3, _0806DA70 @ =0x00000634
	add r5, r7, r3
	ldrb r2, [r5]
	lsl r3, r2, #1
	add r3, r3, r2
	lsl r1, r3, #4
	sub r1, r1, r3
	lsl r1, r1, #7
	ldr r3, _0806DA74 @ =0x06008000
	add r1, r1, r3
	bl LoadCardArt8bpp
	ldr r1, _0806DA78 @ =0x00000632
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
	b _0806DAA6
_0806D9CC: .4byte 0x01004000
_0806D9D0: .4byte 0x06010000
_0806D9D4: .4byte 0x01002000
_0806D9D8: .4byte gDeckEditFrameMap
_0806D9DC: .4byte 0x0600E000
_0806D9E0: .4byte gDeckEditBgTiles
_0806D9E4: .4byte gDeckEditLabelTiles
_0806D9E8: .4byte 0x06004000
_0806D9EC: .4byte gDeckEditObjTiles
_0806D9F0: .4byte gDeckEditCardStackObjTiles
_0806D9F4: .4byte 0x06010200
_0806D9F8: .4byte gDeckEditCardIconObjTiles
_0806D9FC: .4byte 0x06014000
_0806DA00: .4byte gDeckEditCardFrameObjTiles
_0806DA04: .4byte 0x06014200
_0806DA08: .4byte 0x06006000
_0806DA0C: .4byte gDeckEditAnimScripts
_0806DA10: .4byte 0x0201F238
_0806DA14: .4byte gDeckEditBgPals4to7
_0806DA18: .4byte 0x05000080
_0806DA1C: .4byte gDeckEditBgPals1to3
_0806DA20: .4byte 0x05000020
_0806DA24: .4byte gDeckEditBgPal3
_0806DA28: .4byte 0x05000060
_0806DA2C: .4byte gDeckEditObjPal
_0806DA30: .4byte 0x05000200
_0806DA34: .4byte 0x05000044
_0806DA38: .4byte 0x00007758
_0806DA3C: .4byte 0x06002000
_0806DA40: .4byte gUnk_08704EE8
_0806DA44: .4byte 0xFFFFE8E8
_0806DA48: .4byte 0x0201F73C
_0806DA4C: .4byte 0x0600D000
_0806DA50: .4byte 0x0201F3D0
_0806DA54: .4byte 0x00001BB8
_0806DA58: .4byte 0x0201DB20
_0806DA5C: .4byte 0x0201E140
_0806DA60: .4byte 0x00001494
_0806DA64: .4byte 0x0600C000
_0806DA68: .4byte 0x0000063E
_0806DA6C: .4byte 0x000018B0
_0806DA70: .4byte 0x00000634
_0806DA74: .4byte 0x06008000
_0806DA78: .4byte 0x00000632
_0806DA7C:
	ldr r0, _0806DB60 @ =0xFFFFF18C
	add r0, r9
	add r0, r2, r0
	ldrh r2, [r0]
	add r0, r3, #0
	bl DeckEdit_GetListCard
	ldr r1, _0806DB64 @ =0x0600C000
	ldr r2, _0806DB68 @ =0xFFFFF1AA
	add r2, r9
	ldrh r3, [r2]
	add r3, #0x20
	mov r2, #0xFF
	and r3, r2
	asr r3, r3, #3
	ldr r2, _0806DB6C @ =0xFFFFF1AC
	add r2, r9
	str r2, [sp, #0]
	mov r2, #0
	bl DeckEdit_DrawNoCardsText
_0806DAA6:
	ldr r4, _0806DB70 @ =0x0201DB20
	ldr r2, _0806DB74 @ =0x00001C1C
	add r0, r4, r2
	ldrb r0, [r0]
	bl DeckEdit_DrawStatementLabels
	ldr r0, _0806DB78 @ =0x04000040
	mov r2, #0xF0
	strh r2, [r0]
	ldr r1, _0806DB7C @ =0x04000044
	ldr r3, _0806DB80 @ =0x0000244C
	add r0, r3, #0
	strh r0, [r1]
	ldr r0, _0806DB84 @ =0x04000042
	strh r2, [r0]
	add r1, #2
	mov r0, #0x70
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806DB88 @ =0x00003E3D
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
	ldr r2, _0806DB8C @ =0x00001A01
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r3, _0806DB90 @ =0x00001C02
	add r0, r3, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806DB94 @ =0x00001E8B
	add r0, r2, #0
	strh r0, [r1]
	ldr r1, _0806DB98 @ =0xFFFFFE80
	mov r3, #0xC3
	lsl r3, r3, #3
	add r4, r4, r3
	mov r0, #0
	mov r2, #0
	add r3, r4, #0
	bl FadeStart
	ldr r0, _0806DB9C @ =0x04000010
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
	mov r2, #0xFE
	lsl r2, r2, #7
	add r0, r2, #0
	strh r0, [r1]
	ldr r0, _0806DBA0 @ =0x06006000
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
	.align 2, 0
_0806DB60: .4byte 0xFFFFF18C
_0806DB64: .4byte 0x0600C000
_0806DB68: .4byte 0xFFFFF1AA
_0806DB6C: .4byte 0xFFFFF1AC
_0806DB70: .4byte 0x0201DB20
_0806DB74: .4byte 0x00001C1C
_0806DB78: .4byte 0x04000040
_0806DB7C: .4byte 0x04000044
_0806DB80: .4byte 0x0000244C
_0806DB84: .4byte 0x04000042
_0806DB88: .4byte 0x00003E3D
_0806DB8C: .4byte 0x00001A01
_0806DB90: .4byte 0x00001C02
_0806DB94: .4byte 0x00001E8B
_0806DB98: .4byte 0xFFFFFE80
_0806DB9C: .4byte 0x04000010
_0806DBA0: .4byte 0x06006000
	thumb_func_end DeckEdit_InitListView

