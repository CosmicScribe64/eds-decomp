	thumb_func_start CardListView_DrawCardInfo
CardListView_DrawCardInfo: @ 0x0802A188
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	ldr r4, _0802A270 @ =0x0201D810
	ldrb r1, [r4]
	lsl r0, r1, #0x1E
	lsr r7, r0, #0x1F
	ldrb r2, [r4, #5]
	lsl r0, r2, #0x1E
	lsr r0, r0, #0x1E
	ldrh r1, [r4, #6]
	add r5, r1, r0
	ldr r1, _0802A274 @ =0x000001A1
	mov r0, #0
	mov r2, #0x13
	mov r3, #6
	bl FillMapRect
	ldr r1, _0802A278 @ =0x00000155
	mov r0, #3
	mov r2, #0xA
	mov r3, #0xA
	bl FillMapRect
	mov r0, #0xE0
	ldrb r4, [r4]
	and r0, r4
	cmp r0, #0x60
	bne _0802A1E2
	ldr r2, _0802A27C @ =0x020192E4
	lsl r0, r5, #1
	ldr r1, _0802A280 @ =0x00000D64
	mul r1, r7
	add r0, r0, r1
	ldr r1, _0802A284 @ =0x00000CC4
	add r2, r2, r1
	add r0, r0, r2
	ldrb r0, [r0]
	cmp r0, #2
	bne _0802A1E2
	cmp r7, #1
	bne _0802A1E2
	b _0802A454
_0802A1E2:
	ldr r3, _0802A270 @ =0x0201D810
	ldrb r2, [r3]
	lsl r0, r2, #0x1C
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r1, r1, r0
	mov r0, #1
	and r1, r0
	lsl r1, r1, #3
	mov r0, #9
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r3]
	ldr r1, _0802A278 @ =0x00000155
	lsl r0, r0, #0x1C
	lsr r3, r0, #0x1F
	mov r2, #0xB4
	mul r3, r2
	add r2, #0xC4
	add r3, r3, r2
	lsr r0, r0, #0x1F
	lsl r0, r0, #2
	add r0, #8
	lsl r0, r0, #4
	str r0, [sp, #0]
	mov r0, #3
	add r2, r6, #0
	bl DrawCardPortrait
	cmp r6, #0
	bne _0802A224
	b _0802A454
_0802A224:
	ldr r0, _0802A288 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _0802A28C @ =0x08621DE0
	add r4, r0, r1
	ldr r0, [r4]
	mov r5, #0xF8
	lsl r5, r5, #0x11
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0802A2EC
	cmp r0, #0x15
	blt _0802A2EC
	ldr r3, _0802A290 @ =0x08636CD8
	cmp r0, #0x16
	bne _0802A248
	add r3, #0xC8
_0802A248:
	ldr r0, _0802A274 @ =0x000001A1
	mov r1, #0x50
	mov r2, #0x98
	lsl r2, r2, #1
	bl LoadBgImage4bpp
	ldr r1, [r4]
	add r0, r1, #0
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0802A294
	cmp r0, #0x15
	blt _0802A294
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0802A296
	.align 2, 0
_0802A270: .4byte 0x0201D810
_0802A274: .4byte 0x000001A1
_0802A278: .4byte 0x00000155
_0802A27C: .4byte 0x020192E4
_0802A280: .4byte 0x00000D64
_0802A284: .4byte 0x00000CC4
_0802A288: .4byte 0x000007FF
_0802A28C: .4byte gCardStats
_0802A290: .4byte gTrapBadgeImage
_0802A294:
	mov r0, #0
_0802A296:
	cmp r0, #0
	bne _0802A29C
	b _0802A454
_0802A29C:
	ldr r1, _0802A2C4 @ =0x081989D0
	ldr r0, _0802A2C8 @ =0x000007FF
	and r6, r0
	lsl r0, r6, #2
	ldr r2, _0802A2CC @ =0x08621DE0
	add r0, r0, r2
	ldr r2, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r2
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0802A2D0
	cmp r0, #0x15
	blt _0802A2D0
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r2, r0
	lsr r0, r2, #0x11
	b _0802A2D2
_0802A2C4: .4byte gSpellSubtypeIconImages
_0802A2C8: .4byte 0x000007FF
_0802A2CC: .4byte gCardStats
_0802A2D0:
	mov r0, #0
_0802A2D2:
	lsl r0, r0, #2
	add r0, r1, r0
	ldr r3, [r0]
	ldr r0, _0802A2E8 @ =0x000001A3
	mov r1, #0x60
	mov r2, #0x9A
	lsl r2, r2, #1
	bl LoadBgImage4bpp
	b _0802A454
	.align 2, 0
_0802A2E8: .4byte 0x000001A3
_0802A2EC:
	ldr r0, _0802A340 @ =0x000001A1
	mov r2, #0x98
	lsl r2, r2, #1
	ldr r3, _0802A344 @ =0x081989A8
	ldr r4, _0802A348 @ =0x000007FF
	and r4, r6
	lsl r4, r4, #2
	ldr r1, _0802A34C @ =0x08621DE0
	add r4, r4, r1
	ldr r1, [r4]
	lsr r1, r1, #0x1D
	lsl r1, r1, #2
	add r1, r1, r3
	ldr r3, [r1]
	mov r1, #0x50
	bl LoadBgImage4bpp
	ldr r0, _0802A350 @ =0x000001A3
	mov r2, #0x9A
	lsl r2, r2, #1
	ldr r3, _0802A354 @ =0x081989EC
	ldr r1, [r4]
	mov r5, #0xF8
	lsl r5, r5, #0x11
	and r1, r5
	lsr r1, r1, #0x12
	add r1, r1, r3
	ldr r3, [r1]
	mov r1, #0x60
	bl LoadBgImage4bpp
	ldr r0, [r4]
	and r0, r5
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0802A362
	cmp r0, #0x17
	ble _0802A358
	cmp r0, #0x18
	beq _0802A35C
	b _0802A362
	.align 2, 0
_0802A340: .4byte 0x000001A1
_0802A344: .4byte gAttributeIconImages
_0802A348: .4byte 0x000007FF
_0802A34C: .4byte gCardStats
_0802A350: .4byte 0x000001A3
_0802A354: .4byte gMonsterTypeIconImages
_0802A358:
	mov r0, #0
	b _0802A378
_0802A35C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0802A378
_0802A362:
	ldr r0, _0802A3A8 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r2, _0802A3AC @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0802A378:
	add r2, r0, #0
	ldr r0, _0802A3B0 @ =0x000701A7
	ldr r1, _0802A3B4 @ =0x0004013A
	mov r3, #0
	bl DrawBgDecimal
	ldr r0, _0802A3A8 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _0802A3AC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0802A3C2
	cmp r0, #0x17
	ble _0802A3B8
	cmp r0, #0x18
	beq _0802A3BC
	b _0802A3C2
	.align 2, 0
_0802A3A8: .4byte 0x000007FF
_0802A3AC: .4byte gCardStats
_0802A3B0: .4byte 0x000701A7
_0802A3B4: .4byte 0x0004013A
_0802A3B8:
	mov r0, #0
	b _0802A3D8
_0802A3BC:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _0802A3D8
_0802A3C2:
	ldr r0, _0802A3F4 @ =0x000007FF
	and r0, r6
	lsl r0, r0, #2
	ldr r2, _0802A3F8 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	ldr r0, _0802A3FC @ =0x000001FF
	and r1, r0
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_0802A3D8:
	add r2, r0, #0
	ldr r0, _0802A400 @ =0x000701C7
	ldr r1, _0802A404 @ =0x0004013E
	mov r3, #0
	bl DrawBgDecimal
	mov r3, #0
	ldr r0, _0802A3F4 @ =0x000007FF
	and r6, r0
	lsl r0, r6, #2
	ldr r1, _0802A3F8 @ =0x08621DE0
	add r2, r0, r1
	ldr r4, _0802A408 @ =0x0300045C
	b _0802A426
_0802A3F4: .4byte 0x000007FF
_0802A3F8: .4byte gCardStats
_0802A3FC: .4byte 0x000001FF
_0802A400: .4byte 0x000701C7
_0802A404: .4byte 0x0004013E
_0802A408: .4byte 0x0300045C
_0802A40C:
	mov r0, #7
	and r0, r3
	asr r1, r3, #3
	add r1, #0xD
	add r0, #0xC
	lsl r1, r1, #0x10
	lsr r1, r1, #0xB
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r0, r4
	mov r1, #2
	strh r1, [r0]
	add r3, #1
_0802A426:
	ldr r0, [r2]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _0802A446
	cmp r0, #0x17
	ble _0802A43E
	cmp r0, #0x18
	beq _0802A442
	b _0802A446
_0802A43E:
	mov r0, #0
	b _0802A450
_0802A442:
	mov r0, #0xA
	b _0802A450
_0802A446:
	ldr r0, [r2]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_0802A450:
	cmp r3, r0
	blt _0802A40C
_0802A454:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end CardListView_DrawCardInfo

