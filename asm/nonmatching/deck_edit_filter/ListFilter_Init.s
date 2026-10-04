	thumb_func_start ListFilter_Init
ListFilter_Init: @ 0x08069AE0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	mov r4, #0
	str r4, [sp, #0x18]
	mov r1, #0xC0
	lsl r1, r1, #0x13
	ldr r2, _08069D18 @ =0x01004000
	add r0, sp, #0x18
	bl CpuFastSet
	str r4, [sp, #0x1C]
	add r0, sp, #0x1C
	ldr r1, _08069D1C @ =0x06010000
	ldr r2, _08069D20 @ =0x01002000
	bl CpuFastSet
	mov r0, #0
	mov r7, #0
_08069B0C:
	mov r4, #0
	lsl r5, r0, #8
	add r6, r0, #1
_08069B12:
	lsl r1, r4, #3
	add r1, r1, r5
	lsl r1, r1, #1
	ldr r0, _08069D24 @ =0x0600F000
	add r1, r1, r0
	mov r0, #8
	str r0, [sp, #0]
	str r7, [sp, #4]
	str r7, [sp, #8]
	ldr r0, _08069D28 @ =0x086FC060
	mov r2, #8
	mov r3, #8
	bl CopyMapRectAddOffset
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, #3
	bls _08069B12
	lsl r0, r6, #0x10
	lsr r0, r0, #0x10
	cmp r0, #3
	bls _08069B0C
	ldr r0, _08069D2C @ =0x086FC0E0
	ldr r1, _08069D30 @ =0x0600E000
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r0, _08069D34 @ =0x086FC590
	ldr r6, _08069D38 @ =0x0600D000
	add r1, r6, #0
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r0, _08069D3C @ =0x086FCA40
	ldr r7, _08069D40 @ =0x0600C000
	add r1, r7, #0
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r5, _08069D44 @ =0x0201DB20
	ldr r2, _08069D48 @ =0x00001C1C
	add r4, r5, r2
	ldrb r0, [r4]
	lsl r2, r0, #2
	add r2, r2, r0
	str r6, [sp, #0]
	mov r0, #0x14
	mov sl, r0
	str r0, [sp, #4]
	mov r6, #0
	str r6, [sp, #8]
	mov r0, #7
	mov r9, r0
	str r0, [sp, #0xC]
	mov r0, #5
	mov r8, r0
	str r0, [sp, #0x10]
	str r6, [sp, #0x14]
	ldr r0, _08069D4C @ =0x086FD850
	mov r1, #0
	mov r3, #7
	bl CropMapBlock
	ldrb r0, [r4]
	lsl r2, r0, #2
	add r2, r2, r0
	str r7, [sp, #0]
	mov r0, sl
	str r0, [sp, #4]
	str r6, [sp, #8]
	mov r0, r9
	str r0, [sp, #0xC]
	mov r0, r8
	str r0, [sp, #0x10]
	str r6, [sp, #0x14]
	ldr r0, _08069D4C @ =0x086FD850
	mov r1, #0
	mov r3, #7
	bl CropMapBlock
	ldr r0, _08069D50 @ =0x086F2060
	mov r1, #0xC0
	lsl r1, r1, #0x13
	mov r4, #0x80
	lsl r4, r4, #4
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08069D54 @ =0x086F4060
	ldr r1, _08069D58 @ =0x06002000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08069D5C @ =0x086F6060
	ldr r1, _08069D60 @ =0x06004000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08069D64 @ =0x086F8060
	ldr r1, _08069D1C @ =0x06010000
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _08069D68 @ =0x086FA060
	ldr r1, _08069D6C @ =0x06010200
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _08069D70 @ =0x081A6118
	ldr r2, _08069D74 @ =0x00001718
	add r1, r5, r2
	bl AnimBlockInit
	ldr r0, _08069D78 @ =0x00001726
	add r2, r5, r0
	mov r1, #0xFF
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D7C @ =0x0000173A
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D80 @ =0x0000174E
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D84 @ =0x00001762
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D88 @ =0x00001776
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D8C @ =0x0000178A
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D90 @ =0x0000179E
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D94 @ =0x000017B2
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D98 @ =0x000017C6
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069D9C @ =0x000017DA
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069DA0 @ =0x000017EE
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069DA4 @ =0x00001802
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r0, _08069DA8 @ =0x00001816
	add r2, r5, r0
	ldrb r0, [r2]
	orr r0, r1
	strb r0, [r2]
	ldr r2, _08069DAC @ =0x0000182A
	add r0, r5, r2
	ldrb r2, [r0]
	orr r1, r2
	strb r1, [r0]
	ldr r0, _08069DB0 @ =0x086F1C60
	mov r1, #0xA0
	lsl r1, r1, #0x13
	mov r2, #0x80
	bl CpuFastSet
	ldr r0, _08069DB4 @ =0x086F1E60
	ldr r1, _08069DB8 @ =0x05000200
	mov r2, #0x80
	bl CpuFastSet
	ldr r1, _08069DBC @ =0x04000008
	mov r2, #0xC0
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08069DC0 @ =0x00001A01
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08069DC4 @ =0x00001C02
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08069DC8 @ =0x00001E02
	add r0, r2, #0
	strh r0, [r1]
	ldr r1, _08069DCC @ =0xFFFFFE80
	mov r0, #0xC3
	lsl r0, r0, #3
	add r3, r5, r0
	mov r0, #0
	mov r2, #0
	bl FadeStart
	ldr r0, _08069DD0 @ =0x04000010
	strh r6, [r0]
	add r0, #2
	strh r6, [r0]
	add r0, #2
	strh r6, [r0]
	add r0, #2
	strh r6, [r0]
	add r0, #2
	strh r6, [r0]
	add r0, #2
	strh r6, [r0]
	add r0, #2
	strh r6, [r0]
	add r0, #2
	strh r6, [r0]
	ldr r2, _08069DD4 @ =0x00001C50
	add r5, r5, r2
	mov r0, #0x10
	strb r0, [r5]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r2, #0xD0
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #1
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08069D18: .4byte 0x01004000
_08069D1C: .4byte 0x06010000
_08069D20: .4byte 0x01002000
_08069D24: .4byte 0x0600F000
_08069D28: .4byte gListFilterBgPatternMap
_08069D2C: .4byte gListFilterPanelMap
_08069D30: .4byte 0x0600E000
_08069D34: .4byte gListFilterFilterPageMap
_08069D38: .4byte 0x0600D000
_08069D3C: .4byte gListFilterSortPageMap
_08069D40: .4byte 0x0600C000
_08069D44: .4byte 0x0201DB20
_08069D48: .4byte 0x00001C1C
_08069D4C: .4byte gListFilterListIconMap
_08069D50: .4byte gListFilterBgTiles0
_08069D54: .4byte gListFilterBgTiles1
_08069D58: .4byte 0x06002000
_08069D5C: .4byte gListFilterBgTiles2
_08069D60: .4byte 0x06004000
_08069D64: .4byte gListFilterObjTiles0
_08069D68: .4byte gListFilterObjTiles1
_08069D6C: .4byte 0x06010200
_08069D70: .4byte gListFilterAnimScripts
_08069D74: .4byte 0x00001718
_08069D78: .4byte 0x00001726
_08069D7C: .4byte 0x0000173A
_08069D80: .4byte 0x0000174E
_08069D84: .4byte 0x00001762
_08069D88: .4byte 0x00001776
_08069D8C: .4byte 0x0000178A
_08069D90: .4byte 0x0000179E
_08069D94: .4byte 0x000017B2
_08069D98: .4byte 0x000017C6
_08069D9C: .4byte 0x000017DA
_08069DA0: .4byte 0x000017EE
_08069DA4: .4byte 0x00001802
_08069DA8: .4byte 0x00001816
_08069DAC: .4byte 0x0000182A
_08069DB0: .4byte gListFilterBgPal
_08069DB4: .4byte gListFilterObjPal
_08069DB8: .4byte 0x05000200
_08069DBC: .4byte 0x04000008
_08069DC0: .4byte 0x00001A01
_08069DC4: .4byte 0x00001C02
_08069DC8: .4byte 0x00001E02
_08069DCC: .4byte 0xFFFFFE80
_08069DD0: .4byte 0x04000010
_08069DD4: .4byte 0x00001C50
	thumb_func_end ListFilter_Init

