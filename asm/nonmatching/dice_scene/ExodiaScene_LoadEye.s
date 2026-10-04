	thumb_func_start ExodiaScene_LoadEye
ExodiaScene_LoadEye: @ 0x080263C8
	push {r4, r5, r6, lr}
	mov r1, #0x80
	neg r1, r1
	ldr r5, _080264A4 @ =0x02020E28
	mov r0, #0
	mov r2, #0
	add r3, r5, #0
	bl FadeStart
	ldr r0, _080264A8 @ =0x08199D9C
	ldr r2, _080264AC @ =0xFFFFFE00
	add r1, r5, r2
	bl AnimBlockInit
	add r1, r5, #0
	sub r1, #0x70
	strh r0, [r1]
	ldr r0, _080264B0 @ =0x086B8568
	mov r1, #0xC0
	lsl r1, r1, #0x13
	mov r2, #0x96
	lsl r2, r2, #6
	bl CpuFastSet
	ldr r0, _080264B4 @ =0x086C1B68
	mov r1, #0xA0
	lsl r1, r1, #0x13
	mov r2, #0x80
	bl CpuFastSet
	ldr r0, _080264B8 @ =0x086B6368
	ldr r1, _080264BC @ =0x05000200
	mov r6, #0x80
	lsl r6, r6, #1
	add r2, r6, #0
	bl CpuSet
	ldr r0, _080264C0 @ =0x086B6568
	ldr r1, _080264C4 @ =0x06014000
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r4, _080264C8 @ =0x086CED78
	add r0, r4, #0
	mov r1, #4
	mov r2, #0x10
	mov r3, #0x40
	bl LoadObjTileBlock4x4
	add r0, r4, #0
	mov r1, #8
	mov r2, #0x20
	mov r3, #0x40
	bl LoadObjTileBlock4x4
	add r0, r4, #0
	mov r1, #0xC
	mov r2, #0x30
	mov r3, #0x40
	bl LoadObjTileBlock4x4
	add r0, r4, #0
	mov r1, #0x80
	mov r2, #0x40
	mov r3, #0x40
	bl LoadObjTileBlock4x4
	ldr r4, _080264CC @ =0x086CF778
	add r0, r4, #0
	mov r1, #0x88
	mov r2, #0x10
	mov r3, #0x40
	bl LoadObjTileBlock4x4
	add r0, r4, #0
	mov r1, #0x8C
	mov r2, #0x20
	mov r3, #0x40
	bl LoadObjTileBlock4x4
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #0x30
	mov r3, #0x40
	bl LoadObjTileBlock4x4
	mov r1, #0x82
	lsl r1, r1, #1
	add r0, r4, #0
	mov r2, #0x40
	mov r3, #0x40
	bl LoadObjTileBlock4x4
	mov r1, #0x80
	lsl r1, r1, #0x13
	ldr r2, _080264D0 @ =0x00001F04
	add r0, r2, #0
	strh r0, [r1]
	add r5, #8
	add r0, r5, #0
	bl Timer_Reset
	add r0, r5, #0
	mov r1, #0x3C
	bl Timer_Start
	mov r0, #1
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_080264A4: .4byte 0x02020E28
_080264A8: .4byte gExodiaPiecesAnimList
_080264AC: .4byte 0xFFFFFE00
_080264B0: .4byte gMillenniumEyeBitmap
_080264B4: .4byte gMillenniumEyePal
_080264B8: .4byte gExodiaPiecesObjPal
_080264BC: .4byte 0x05000200
_080264C0: .4byte gExodiaPiecesObjTiles
_080264C4: .4byte 0x06014000
_080264C8: .4byte gExodiaPiecesColorObjTiles
_080264CC: .4byte gExodiaPiecesGreyObjTiles
_080264D0: .4byte 0x00001F04
	thumb_func_end ExodiaScene_LoadEye

