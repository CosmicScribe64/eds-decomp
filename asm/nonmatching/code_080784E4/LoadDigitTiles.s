	thumb_func_start LoadDigitTiles
LoadDigitTiles: @ 0x08079474
	push {r4, r5, r6, lr}
	sub sp, #0x14
	add r5, r0, #0
	lsl r2, r2, #0x18
	lsr r6, r2, #0x18
	ldr r1, _080794D4 @ =0x08087B94
	mov r0, sp
	mov r2, #0xD
	bl memcpy
	add r1, sp, #0x10
	mov r0, #0
	strh r0, [r1]
	ldr r0, _080794D8 @ =0x040000D4
	str r1, [r0]
	str r5, [r0, #4]
	ldr r1, _080794DC @ =0x810000D0
	str r1, [r0, #8]
	ldr r1, [r0, #8]
	add r2, r0, #0
	ldr r0, [r2, #8]
	mov r1, #0x80
	lsl r1, r1, #0x18
	cmp r0, #0
	bge _080794AE
_080794A6:
	ldr r0, [r2, #8]
	and r0, r1
	cmp r0, #0
	bne _080794A6
_080794AE:
	mov r4, #0
_080794B0:
	lsl r0, r4, #5
	add r0, r5, r0
	mov r2, sp
	add r1, r2, r4
	add r2, r6, #0
	mov r3, #4
	bl OverlayBoldGlyphTile
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, #0xC
	bls _080794B0
	add sp, #0x14
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_080794D4: .4byte gDigitTileChars
_080794D8: .4byte 0x040000D4
_080794DC: .4byte 0x810000D0
	thumb_func_end LoadDigitTiles

