	thumb_func_start DrawTextStrip
DrawTextStrip: @ 0x08079340
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r5, r0, #0
	add r6, r1, #0
	str r2, [sp, #4]
	ldr r0, [sp, #0x2C]
	ldr r1, [sp, #0x30]
	ldr r2, [sp, #0x34]
	ldr r4, [sp, #0x38]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	mov r8, r3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	str r0, [sp, #8]
	lsl r1, r1, #0x18
	lsr r7, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov sl, r2
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov r9, r4
	mov r2, #0
	ldrb r0, [r5]
	add r1, r5, #1
	cmp r0, #0
	beq _0807938E
_08079380:
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	ldrb r0, [r1]
	add r1, #1
	cmp r0, #0
	bne _08079380
_0807938E:
	lsl r0, r2, #2
	add r0, r0, r2
	add r0, #7
	asr r0, r0, #3
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	add r4, r0, #0
	mov r1, #2
	bl TextCanvasInit
	mov r1, #0xA0
	lsl r1, r1, #4
	add r0, r1, #0
	add r2, r7, #0
	orr r2, r0
	mov r0, #0
	mov r1, #0
	add r3, r5, #0
	bl TextDrawString
	ldr r0, [sp, #4]
	mov r1, sl
	bl TextCanvasToTiles
	str r4, [sp, #0]
	mov r0, r8
	add r1, r6, #0
	ldr r2, [sp, #8]
	mov r3, r9
	bl PutMapTileRun
	ldr r0, _080793FC @ =0x0600C7C8
	cmp r6, r0
	bne _080793D4
	ldr r6, _08079400 @ =0x0600BFC8
_080793D4:
	mov r1, r8
	add r0, r1, r4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r1, r6, #0
	add r1, #0x40
	str r4, [sp, #0]
	ldr r2, [sp, #8]
	mov r3, r9
	bl PutMapTileRun
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080793FC: .4byte 0x0600C7C8
_08079400: .4byte 0x0600BFC8
	thumb_func_end DrawTextStrip

