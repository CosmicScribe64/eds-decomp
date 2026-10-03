	thumb_func_start DrawFieldOverlay
DrawFieldOverlay: @ 0x08061580
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	ldr r1, _080615B4 @ =0x0201CFB0
	mov r0, #6
	ldrb r2, [r1]
	and r0, r2
	cmp r0, #6
	beq _08061596
	b _080616A2
_08061596:
	ldr r3, _080615B8 @ =0x00000824
	add r0, r1, r3
	ldr r4, [r0]
	ldr r2, _080615BC @ =0x00000828
	add r0, r1, r2
	ldr r2, [r0]
	add r3, #8
	add r0, r1, r3
	ldr r0, [r0]
	add r3, r2, r0
	cmp r2, #0
	beq _080615C0
	cmp r2, #5
	beq _080615F0
	b _080615FA
_080615B4: .4byte 0x0201CFB0
_080615B8: .4byte 0x00000824
_080615BC: .4byte 0x00000828
_080615C0:
	mov r2, #1
	and r2, r4
	mov r0, #0x94
	mul r0, r3
	ldr r1, _080615E8 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _080615EC @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080615FA
	add r0, r4, #0
	add r1, r3, #0
	mov r2, #0
	bl DrawZoneLinkMarkers
	b _080615FA
	.align 2, 0
_080615E8: .4byte 0x00000D64
_080615EC: .4byte 0x0201930C
_080615F0:
	add r0, r4, #0
	add r1, r3, #0
	mov r2, #5
	bl DrawZoneLinkMarkers
_080615FA:
	ldr r2, _080616B0 @ =0x020192E0
	ldr r0, _080616B4 @ =0x00001B12
	add r1, r2, r0
	mov r0, #0x1C
	ldrb r3, [r1]
	and r0, r3
	cmp r0, #0xC
	bne _08061692
	mov r5, #0
	add r6, r1, #0
	add r2, #4
	mov r9, r2
	mov r7, #1
	ldr r0, _080616B8 @ =0x081A427C
	mov r8, r0
_08061618:
	ldrb r1, [r6]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	add r1, r5, #0
	mov r2, #0
	bl CanMonsterAttack
	cmp r0, #0
	beq _0806168C
	ldrb r3, [r6]
	lsl r2, r3, #0x1E
	lsr r1, r2, #0x1F
	add r0, r7, #0
	and r0, r1
	ldr r1, _080616BC @ =0x00000D64
	mul r0, r1
	add r0, r9
	ldrh r0, [r0, #0x26]
	asr r0, r5
	and r0, r7
	cmp r0, #0
	bne _0806168C
	lsr r0, r2, #0x1F
	mov r1, #0
	add r2, r5, #0
	bl GetAreaX
	add r4, r0, #0
	ldrb r1, [r6]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1F
	mov r1, #0
	add r2, r5, #0
	bl GetAreaY
	add r4, #8
	add r0, #8
	lsl r0, r0, #0x10
	orr r4, r0
	ldr r0, _080616C0 @ =0x03000040
	ldr r2, _080616C4 @ =0x0000485E
	add r0, r0, r2
	ldrh r0, [r0]
	lsr r0, r0, #3
	mov r1, #7
	and r0, r1
	lsl r0, r0, #2
	add r0, r8
	ldr r2, [r0]
	mov r3, #0xAC
	lsl r3, r3, #7
	add r2, r2, r3
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	add r0, r4, #0
	mov r1, #0x40
	bl AddSprite
_0806168C:
	add r5, #1
	cmp r5, #4
	ble _08061618
_08061692:
	ldr r0, _080616C8 @ =0x0201CFB0
	ldr r1, _080616CC @ =0x0000085C
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _080616A2
	bl _call_via_r0
_080616A2:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080616B0: .4byte 0x020192E0
_080616B4: .4byte 0x00001B12
_080616B8: .4byte gZoneMarkerAnimTiles
_080616BC: .4byte 0x00000D64
_080616C0: .4byte 0x03000040
_080616C4: .4byte 0x0000485E
_080616C8: .4byte 0x0201CFB0
_080616CC: .4byte 0x0000085C
	thumb_func_end DrawFieldOverlay

