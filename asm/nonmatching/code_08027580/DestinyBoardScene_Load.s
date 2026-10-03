	thumb_func_start DestinyBoardScene_Load
DestinyBoardScene_Load: @ 0x08027754
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r1, _080278F4 @ =0xFFFFFE80
	ldr r3, _080278F8 @ =0x02020DBC
	mov r0, #0
	mov r2, #0
	bl FadeStart
	mov r0, #0
	str r0, [sp, #0]
	mov r1, #0xC0
	lsl r1, r1, #0x13
	ldr r2, _080278FC @ =0x01006000
	mov r0, sp
	bl CpuFastSet
	ldr r0, _08027900 @ =0x086E11A4
	ldr r1, _08027904 @ =0x0600E000
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r0, _08027908 @ =0x086E1C6C
	ldr r1, _0802790C @ =0x0600D000
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	mov r6, #0
	ldr r0, _08027910 @ =0x086E21D0
	mov r8, r0
_08027796:
	mov r5, #0
	lsl r7, r6, #8
_0802779A:
	lsl r4, r5, #4
	add r4, r7, r4
	lsl r4, r4, #1
	ldr r2, _08027914 @ =0x0600C000
	add r1, r4, r2
	mov r0, r8
	mov r2, #0x10
	mov r3, #8
	bl CopyMapRect
	ldr r0, _08027918 @ =0x0600F000
	add r4, r4, r0
	mov r0, r8
	add r1, r4, #0
	mov r2, #0x10
	mov r3, #8
	bl CopyMapRect
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #1
	bls _0802779A
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, #3
	bls _08027796
	ldr r0, _0802791C @ =0x086D0178
	mov r1, #0xC0
	lsl r1, r1, #0x13
	mov r4, #0x80
	lsl r4, r4, #4
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08027920 @ =0x086D2178
	ldr r1, _08027924 @ =0x06002000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08027928 @ =0x086D4178
	ldr r1, _0802792C @ =0x06004000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08027930 @ =0x086D6178
	ldr r1, _08027934 @ =0x06010000
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _08027938 @ =0x086D8178
	ldr r1, _0802793C @ =0x06010200
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _08027940 @ =0x086DA178
	ldr r1, _08027944 @ =0x06014000
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _08027948 @ =0x086DC178
	ldr r1, _0802794C @ =0x06014200
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _08027950 @ =0x086E22D0
	mov r1, #0xA0
	lsl r1, r1, #0x13
	mov r2, #0x80
	bl CpuFastSet
	ldr r0, _08027954 @ =0x086E24D0
	ldr r1, _08027958 @ =0x05000200
	mov r2, #0x80
	bl CpuFastSet
	ldr r1, _0802795C @ =0x04000008
	mov r2, #0xC0
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08027960 @ =0x00001A01
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08027964 @ =0x00001C02
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08027968 @ =0x00001E03
	add r0, r2, #0
	strh r0, [r1]
	sub r1, #0xE
	add r2, #0xFD
	add r0, r2, #0
	strh r0, [r1]
	ldr r0, _0802796C @ =0x03000040
	ldr r1, _08027970 @ =0x00000414
	add r0, r0, r1
	ldr r1, _08027974 @ =0x08026E11
	str r1, [r0]
	mov r6, #1
	ldr r2, _08027978 @ =0x02020310
	ldr r4, _0802797C @ =0x00000926
	mov r3, #0xFF
	add r7, r2, #0
_08027872:
	lsl r0, r6, #2
	add r0, r0, r6
	lsl r0, r0, #2
	add r0, r0, r2
	add r0, r0, r4
	ldrb r1, [r0]
	orr r1, r3
	strb r1, [r0]
	add r0, r6, #1
	lsl r0, r0, #0x18
	lsr r6, r0, #0x18
	cmp r6, #4
	bls _08027872
	ldr r2, _0802797C @ =0x00000926
	add r0, r7, r2
	mov r5, #0
	mov r4, #1
	strb r4, [r0]
	ldr r1, _08027980 @ =0x0000094E
	add r0, r7, r1
	strb r4, [r0]
	ldr r2, _08027984 @ =0x00000AC8
	add r0, r7, r2
	bl ScrollLayer_StreamRow
	ldr r1, _08027988 @ =0x00000ADC
	add r0, r7, r1
	bl ScrollLayer_StreamRow
	ldr r3, _0802798C @ =0x04000208
	strh r5, [r3]
	ldr r2, _08027990 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _08027994 @ =0x0000FFFD
	and r0, r1
	strh r0, [r2]
	ldr r1, _08027998 @ =0x03000000
	ldr r0, _0802799C @ =0x08026CC9
	str r0, [r1, #4]
	strh r4, [r3]
	strh r5, [r3]
	ldrh r0, [r2]
	mov r1, #2
	orr r0, r1
	strh r0, [r2]
	strh r4, [r3]
	ldr r1, _080279A0 @ =0x04000050
	ldr r2, _080279A4 @ =0x00003F41
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _080279A8 @ =0x0000100B
	add r0, r2, #0
	strh r0, [r1]
	mov r0, #0x14
	bl PlayBGM
	mov r0, #1
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080278F4: .4byte 0xFFFFFE80
_080278F8: .4byte 0x02020DBC
_080278FC: .4byte 0x01006000
_08027900: .4byte gDestinyBoardBg2Map
_08027904: .4byte 0x0600E000
_08027908: .4byte gDestinyBoardBg1Map
_0802790C: .4byte 0x0600D000
_08027910: .4byte gDestinyBoardWaveMap
_08027914: .4byte 0x0600C000
_08027918: .4byte 0x0600F000
_0802791C: .4byte gDestinyBoardBgTiles0
_08027920: .4byte gDestinyBoardBgTiles1
_08027924: .4byte 0x06002000
_08027928: .4byte gDestinyBoardBgTiles2
_0802792C: .4byte 0x06004000
_08027930: .4byte gDestinyBoardObjTiles0
_08027934: .4byte 0x06010000
_08027938: .4byte gDestinyBoardObjTiles1
_0802793C: .4byte 0x06010200
_08027940: .4byte gDestinyBoardObjTiles2
_08027944: .4byte 0x06014000
_08027948: .4byte gDestinyBoardObjTiles3
_0802794C: .4byte 0x06014200
_08027950: .4byte gDestinyBoardBgPal
_08027954: .4byte gDestinyBoardObjPal
_08027958: .4byte 0x05000200
_0802795C: .4byte 0x04000008
_08027960: .4byte 0x00001A01
_08027964: .4byte 0x00001C02
_08027968: .4byte 0x00001E03
_0802796C: .4byte 0x03000040
_08027970: .4byte 0x00000414
_08027974: .4byte DestinyBoardScene_VBlank
_08027978: .4byte 0x02020310
_0802797C: .4byte 0x00000926
_08027980: .4byte 0x0000094E
_08027984: .4byte 0x00000AC8
_08027988: .4byte 0x00000ADC
_0802798C: .4byte 0x04000208
_08027990: .4byte 0x04000200
_08027994: .4byte 0x0000FFFD
_08027998: .4byte 0x03000000
_0802799C: .4byte DestinyBoardScene_HBlank
_080279A0: .4byte 0x04000050
_080279A4: .4byte 0x00003F41
_080279A8: .4byte 0x0000100B
	thumb_func_end DestinyBoardScene_Load

