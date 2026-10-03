	thumb_func_start GetCardIconGfx
GetCardIconGfx: @ 0x0801A238
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	ldr r2, _0801A25C @ =0x000007FF
	and r2, r3
	lsl r0, r2, #2
	ldr r1, _0801A260 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	beq _0801A268
	cmp r0, #0x16
	bne _0801A270
	ldr r0, _0801A264 @ =0x0867B17C
	b _0801A326
_0801A25C: .4byte 0x000007FF
_0801A260: .4byte gCardStats
_0801A264: .4byte gCardIconMagicGfx
_0801A268:
	ldr r0, _0801A26C @ =0x0867A97C
	b _0801A326
_0801A26C: .4byte gCardIconTrapGfx
_0801A270:
	lsl r0, r2, #1
	ldr r1, _0801A284 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0801A288 @ =0x00000776
	cmp r1, r0
	bne _0801A28C
	mov r0, #3
	b _0801A2EE
	.align 2, 0
_0801A284: .4byte gCardIdToNumber
_0801A288: .4byte 0x00000776
_0801A28C:
	cmp r1, r0
	blt _0801A29C
	mov r0, #0xEF
	lsl r0, r0, #3
	cmp r1, r0
	bgt _0801A29C
	mov r0, #1
	b _0801A2EE
_0801A29C:
	ldr r0, _0801A2C0 @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0801A2C4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	beq _0801A2CE
	cmp r0, #0x16
	bgt _0801A2C8
	cmp r0, #0x15
	beq _0801A2D2
	b _0801A2DA
	.align 2, 0
_0801A2C0: .4byte 0x000007FF
_0801A2C4: .4byte gCardStats
_0801A2C8:
	cmp r0, #0x17
	beq _0801A2D6
	b _0801A2DA
_0801A2CE:
	mov r0, #7
	b _0801A2EE
_0801A2D2:
	mov r0, #8
	b _0801A2EE
_0801A2D6:
	mov r0, #9
	b _0801A2EE
_0801A2DA:
	ldr r0, _0801A2FC @ =0x000007FF
	and r0, r3
	lsl r0, r0, #2
	ldr r1, _0801A300 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xC0
	lsl r1, r1, #0xC
	and r0, r1
	lsr r0, r0, #0x12
_0801A2EE:
	cmp r0, #2
	beq _0801A314
	cmp r0, #2
	bgt _0801A304
	cmp r0, #1
	beq _0801A30A
	b _0801A324
_0801A2FC: .4byte 0x000007FF
_0801A300: .4byte gCardStats
_0801A304:
	cmp r0, #3
	beq _0801A31C
	b _0801A324
_0801A30A:
	ldr r0, _0801A310 @ =0x0867917C
	b _0801A326
	.align 2, 0
_0801A310: .4byte gCardIconEffectGfx
_0801A314:
	ldr r0, _0801A318 @ =0x0867997C
	b _0801A326
_0801A318: .4byte gCardIconFusionGfx
_0801A31C:
	ldr r0, _0801A320 @ =0x0867A17C
	b _0801A326
_0801A320: .4byte gCardIconRitualGfx
_0801A324:
	ldr r0, _0801A328 @ =0x0867897C
_0801A326:
	bx lr
_0801A328: .4byte gCardIconNormalGfx
	thumb_func_end GetCardIconGfx

