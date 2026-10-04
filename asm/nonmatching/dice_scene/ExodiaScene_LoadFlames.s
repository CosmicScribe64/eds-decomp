	thumb_func_start ExodiaScene_LoadFlames
ExodiaScene_LoadFlames: @ 0x080264D4
	push {r4, r5, r6, lr}
	mov r6, r8
	push {r6}
	sub sp, #0x18
	ldr r0, _0802661C @ =0x086C1D68
	mov r1, #0xC0
	lsl r1, r1, #0x13
	mov r4, #0x80
	lsl r4, r4, #4
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08026620 @ =0x086C3D68
	ldr r1, _08026624 @ =0x06002000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08026628 @ =0x086C5D68
	ldr r1, _0802662C @ =0x06004000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08026630 @ =0x086C7D68
	ldr r1, _08026634 @ =0x06006000
	add r2, r4, #0
	bl CpuFastSet
	ldr r0, _08026638 @ =0x086CAB78
	mov r5, #0xA0
	lsl r5, r5, #0x13
	add r1, r5, #0
	mov r2, #0x80
	bl CpuFastSet
	ldr r0, _0802663C @ =0x086CAD78
	ldr r1, _08026640 @ =0x06010000
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _08026644 @ =0x086CCD78
	ldr r1, _08026648 @ =0x06014000
	mov r2, #0x10
	bl CopyTileSheetTo2D
	ldr r0, _0802664C @ =0x086C9D68
	ldr r1, _08026650 @ =0x0600E000
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r0, _08026654 @ =0x086CA218
	ldr r1, _08026658 @ =0x0600D000
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r4, _0802665C @ =0x086CA6C8
	ldr r1, _08026660 @ =0x0600C000
	add r0, r4, #0
	mov r2, #0x1E
	mov r3, #0x14
	bl CopyMapRect
	ldr r1, _08026664 @ =0x00000000
	ldr r2, _08026668 @ =0x00000000
	ldr r0, _0802666C @ =0x0600C03C
	str r0, [sp, #0]
	mov r6, #0
	str r6, [sp, #4]
	str r6, [sp, #8]
	mov r0, #2
	str r0, [sp, #0xC]
	mov r0, #0x14
	str r0, [sp, #0x10]
	str r6, [sp, #0x14]
	add r0, r4, #0
	mov r3, #0x1E
	bl CopyMapBlock
	mov r1, #0x80
	neg r1, r1
	ldr r4, _08026670 @ =0x02020E28
	mov r0, #1
	mov r2, #0
	add r3, r4, #0
	bl FadeStart
	add r0, r4, #0
	add r0, #8
	mov r1, #0x96
	bl Timer_Start
	ldr r0, _08026674 @ =0x08199CC8
	ldr r2, _08026678 @ =0xFFFFFE00
	add r1, r4, r2
	bl AnimBlockInit
	ldr r1, _0802667C @ =0xFFFFFE10
	add r0, r4, r1
	mov r2, #1
	mov r8, r2
	mov r1, r8
	strb r1, [r0]
	ldr r1, _08026680 @ =0x04000008
	mov r2, #0xC0
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08026684 @ =0x00001A01
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _08026688 @ =0x00001C02
	add r0, r2, #0
	strh r0, [r1]
	sub r1, #0xC
	mov r2, #0xB8
	lsl r2, r2, #5
	add r0, r2, #0
	strh r0, [r1]
	mov r1, #0x80
	lsl r1, r1, #2
	add r3, r4, #0
	add r3, #0x10
	add r0, r5, #0
	mov r2, #0x1F
	bl PalFade_Start
	ldr r1, _0802668C @ =0xFFFFFB00
	add r0, r4, r1
	bl ObjAffineInit
	sub r0, r4, #2
	strb r6, [r0]
	ldr r3, _08026690 @ =0x04000208
	mov r5, #0
	strh r6, [r3]
	ldr r2, _08026694 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _08026698 @ =0x0000FFFD
	and r0, r1
	strh r0, [r2]
	ldr r1, _0802669C @ =0x03000000
	ldr r0, _080266A0 @ =0x080261E1
	str r0, [r1, #4]
	mov r0, r8
	strh r0, [r3]
	strb r5, [r4, #0xC]
	strb r5, [r4, #0xD]
	strh r6, [r3]
	ldrh r0, [r2]
	mov r1, #2
	orr r0, r1
	strh r0, [r2]
	mov r1, r8
	strh r1, [r3]
	mov r0, #1
	add sp, #0x18
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0802661C: .4byte gExodiaFlameBgTiles0
_08026620: .4byte gExodiaFlameBgTiles1
_08026624: .4byte 0x06002000
_08026628: .4byte gExodiaFlameBgTiles2
_0802662C: .4byte 0x06004000
_08026630: .4byte gExodiaFlameBgTiles3
_08026634: .4byte 0x06006000
_08026638: .4byte gExodiaFlameBgPal
_0802663C: .4byte gExodiaFlameArmsObjTiles
_08026640: .4byte 0x06010000
_08026644: .4byte gExodiaFlameObjTilesB
_08026648: .4byte 0x06014000
_0802664C: .4byte gExodiaFlameBg2Map
_08026650: .4byte 0x0600E000
_08026654: .4byte gExodiaFlameBg1Map
_08026658: .4byte 0x0600D000
_0802665C: .4byte gExodiaFlameBg0Map
_08026660: .4byte 0x0600C000
_08026664: .4byte 0x00000000
_08026668: .4byte 0x00000000
_0802666C: .4byte 0x0600C03C
_08026670: .4byte 0x02020E28
_08026674: .4byte gExodiaFlameAnimList
_08026678: .4byte 0xFFFFFE00
_0802667C: .4byte 0xFFFFFE10
_08026680: .4byte 0x04000008
_08026684: .4byte 0x00001A01
_08026688: .4byte 0x00001C02
_0802668C: .4byte 0xFFFFFB00
_08026690: .4byte 0x04000208
_08026694: .4byte 0x04000200
_08026698: .4byte 0x0000FFFD
_0802669C: .4byte 0x03000000
_080266A0: .4byte ExodiaScene_HBlank
	thumb_func_end ExodiaScene_LoadFlames

