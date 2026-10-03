	thumb_func_start GetPack_InitScene
GetPack_InitScene: @ 0x08063040
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r5, _08063258 @ =0x03000040
	ldr r0, _0806325C @ =0x0000040E
	add r1, r5, r0
	mov r4, #0
	ldr r0, _08063260 @ =0x00000B83
	strh r0, [r1]
	mov r1, #0x80
	lsl r1, r1, #0x13
	mov r0, #0x40
	strh r0, [r1]
	add r1, #8
	mov r0, #4
	strh r0, [r1]
	add r1, #2
	ldr r2, _08063264 @ =0x00000105
	add r0, r2, #0
	strh r0, [r1]
	add r1, #2
	ldr r3, _08063268 @ =0x00000206
	add r0, r3, #0
	strh r0, [r1]
	add r1, #2
	ldr r2, _0806326C @ =0x00000307
	add r0, r2, #0
	strh r0, [r1]
	bl ResetVideo
	bl ClearBgMapBuffers
	bl LoadSystemGfx
	ldr r0, _08063270 @ =0x0400004C
	strh r4, [r0]
	bl SetBrightnessBlack
	ldr r0, _08063274 @ =0x05000200
	ldr r1, _08063278 @ =0x0867793C
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _0806327C @ =0x05000220
	ldr r1, _08063280 @ =0x0867795C
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _08063284 @ =0x06010000
	ldr r1, _08063288 @ =0x0867797C
	mov r4, #0x80
	lsl r4, r4, #4
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _0806328C @ =0x06010800
	ldr r1, _08063290 @ =0x0867817C
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _08063294 @ =0x06011000
	ldr r1, _08063298 @ =0x0867897C
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _0806329C @ =0x06011800
	ldr r1, _080632A0 @ =0x0867917C
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _080632A4 @ =0x06012000
	ldr r1, _080632A8 @ =0x0867997C
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _080632AC @ =0x06012800
	ldr r1, _080632B0 @ =0x0867A17C
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _080632B4 @ =0x06013000
	ldr r1, _080632B8 @ =0x0867B17C
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _080632BC @ =0x06013800
	ldr r1, _080632C0 @ =0x0867A97C
	add r2, r4, #0
	bl MemCopy16
	ldr r0, _080632C4 @ =0x05000020
	ldr r1, _080632C8 @ =0x0863CA9C
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _080632CC @ =0x05000040
	ldr r1, _080632D0 @ =0x0863CB3C
	mov r2, #0x20
	bl MemCopy16
	ldr r0, _080632D4 @ =0x06006600
	ldr r1, _080632D8 @ =0x0863CABC
	mov r2, #0x80
	bl MemCopy16
	ldr r0, _080632DC @ =0x06008000
	ldr r1, _080632E0 @ =0x0863CB5C
	mov r2, #0x90
	lsl r2, r2, #1
	bl MemCopy16
	ldr r3, _080632E4 @ =0x00001C1C
	add r2, r5, r3
	ldr r0, _080632E8 @ =0x00000C1C
	add r0, r0, r5
	mov r8, r0
	ldr r1, _080632EC @ =0x00001130
	mov ip, r1
	ldr r3, _080632F0 @ =0x00001131
	add r7, r3, #0
	ldr r0, _080632F4 @ =0x00001132
	add r6, r0, #0
	add r1, #3
	add r5, r1, #0
	mov r4, #0xF
_0806313A:
	mov r3, #0xF
	add r1, r2, #0
	add r1, #0x40
_08063140:
	mov r0, ip
	strh r0, [r2]
	strh r7, [r2, #2]
	strh r6, [r1]
	add r0, r2, #0
	add r0, #0x42
	strh r5, [r0]
	add r1, #4
	add r2, #4
	sub r3, #1
	cmp r3, #0
	bge _08063140
	add r2, #0x40
	sub r4, #1
	cmp r4, #0
	bge _0806313A
	mov r2, r8
	mov r1, #0x88
	lsl r1, r1, #6
	add r0, r1, #0
	strh r0, [r2]
	add r1, r2, #0
	add r1, #0xC0
	ldr r3, _080632F8 @ =0x00002201
	add r0, r3, #0
	strh r0, [r1]
	add r1, #0x3A
	add r3, #1
	add r0, r3, #0
	strh r0, [r1]
	ldr r1, _080632FC @ =0x00002203
	add r0, r1, #0
	strh r0, [r2, #0x3A]
	mov r4, #1
	add r3, #2
	add r6, r3, #0
	ldr r0, _08063300 @ =0x00002208
	add r3, r0, #0
	add r1, #2
	add r5, r1, #0
_08063190:
	lsl r0, r4, #0x10
	lsr r0, r0, #0xF
	add r1, r0, r2
	strh r6, [r1]
	add r0, r1, #0
	add r0, #0x40
	strh r3, [r0]
	add r0, #0x40
	strh r3, [r0]
	add r0, #0x40
	strh r5, [r0]
	add r4, #1
	cmp r4, #0x1C
	ble _08063190
	add r0, r2, #0
	add r0, #0x40
	ldr r3, _08063304 @ =0x00002206
	add r1, r3, #0
	strh r1, [r0]
	add r0, #0x40
	strh r1, [r0]
	sub r0, #6
	add r3, #1
	add r1, r3, #0
	strh r1, [r0]
	add r0, #0x40
	strh r1, [r0]
	bl ResetBgScroll
	ldr r2, _08063308 @ =0x02015160
	mov r1, #0x8B
	lsl r1, r1, #1
	add r0, r2, r1
	mov r1, #0
	strh r1, [r0]
	mov r3, #0x8C
	lsl r3, r3, #1
	add r0, r2, r3
	strh r1, [r0]
	mov r4, #4
	sub r3, #8
	add r0, r2, r3
_080631E4:
	strb r1, [r0]
	sub r0, #1
	sub r4, #1
	cmp r4, #0
	bge _080631E4
	bl ResetBgScroll
	ldr r6, _08063258 @ =0x03000040
	ldr r1, _0806330C @ =0x00000414
	add r0, r6, r1
	mov r4, #0
	str r4, [r0]
	ldr r3, _08063310 @ =0x04000208
	strh r4, [r3]
	ldr r2, _08063314 @ =0x04000200
	ldrh r1, [r2]
	ldr r0, _08063318 @ =0x0000FFFD
	and r0, r1
	strh r0, [r2]
	ldr r1, _0806331C @ =0x03000000
	ldr r0, _08063320 @ =0x08062421
	str r0, [r1, #4]
	mov r5, #1
	strh r5, [r3]
	strh r4, [r3]
	ldrh r0, [r2]
	mov r1, #2
	orr r0, r1
	strh r0, [r2]
	strh r5, [r3]
	ldr r0, _08063308 @ =0x02015160
	mov r2, #0x8A
	lsl r2, r2, #1
	add r0, r0, r2
	ldrb r0, [r0]
	lsl r1, r0, #0x1D
	lsr r1, r1, #0x18
	neg r1, r1
	ldr r2, _08063324 @ =0x0808658C
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1D
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r0, [r0]
	sub r1, r1, r0
	ldr r3, _08063328 @ =0x00004422
	add r0, r6, r3
	strh r1, [r0]
	ldr r0, _0806332C @ =0x00004420
	add r6, r6, r0
	mov r0, #3
	strh r0, [r6]
	mov r0, #1
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08063258: .4byte 0x03000040
_0806325C: .4byte 0x0000040E
_08063260: .4byte 0x00000B83
_08063264: .4byte 0x00000105
_08063268: .4byte 0x00000206
_0806326C: .4byte 0x00000307
_08063270: .4byte 0x0400004C
_08063274: .4byte 0x05000200
_08063278: .4byte gHandCursorPal
_0806327C: .4byte 0x05000220
_08063280: .4byte gCardIconPal
_08063284: .4byte 0x06010000
_08063288: .4byte gHandCursorGfx
_0806328C: .4byte 0x06010800
_08063290: .4byte gUnk_0867817C
_08063294: .4byte 0x06011000
_08063298: .4byte gCardIconNormalGfx
_0806329C: .4byte 0x06011800
_080632A0: .4byte gCardIconEffectGfx
_080632A4: .4byte 0x06012000
_080632A8: .4byte gCardIconFusionGfx
_080632AC: .4byte 0x06012800
_080632B0: .4byte gCardIconRitualGfx
_080632B4: .4byte 0x06013000
_080632B8: .4byte gCardIconMagicGfx
_080632BC: .4byte 0x06013800
_080632C0: .4byte gCardIconTrapGfx
_080632C4: .4byte 0x05000020
_080632C8: .4byte gPackSceneBgPal
_080632CC: .4byte 0x05000040
_080632D0: .4byte gPackCursorFramePal
_080632D4: .4byte 0x06006600
_080632D8: .4byte gPackSceneBgTiles
_080632DC: .4byte 0x06008000
_080632E0: .4byte gPackCursorFrameTiles
_080632E4: .4byte 0x00001C1C
_080632E8: .4byte 0x00000C1C
_080632EC: .4byte 0x00001130
_080632F0: .4byte 0x00001131
_080632F4: .4byte 0x00001132
_080632F8: .4byte 0x00002201
_080632FC: .4byte 0x00002203
_08063300: .4byte 0x00002208
_08063304: .4byte 0x00002206
_08063308: .4byte 0x02015160
_0806330C: .4byte 0x00000414
_08063310: .4byte 0x04000208
_08063314: .4byte 0x04000200
_08063318: .4byte 0x0000FFFD
_0806331C: .4byte 0x03000000
_08063320: .4byte GetPack_HBlank
_08063324: .4byte gPackCursorSlideOffsets
_08063328: .4byte 0x00004422
_0806332C: .4byte 0x00004420
	thumb_func_end GetPack_InitScene

