	thumb_func_start TextCellsPutSjisString
TextCellsPutSjisString: @ 0x0805EDE0
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r4, r1, #0
	add r6, r2, #0
	b _0805EE06
_0805EDEA:
	ldrh r0, [r4]
	lsr r1, r0, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r0, r1
	lsl r1, r5, #5
	ldr r2, _0805EE24 @ =0x0201CFB8
	add r1, r1, r2
	add r5, #1
	add r2, r6, #0
	mov r3, #9
	bl RenderSjisGlyphTile
	add r4, #2
_0805EE06:
	ldrb r0, [r4]
	cmp r0, #0
	bne _0805EDEA
	ldr r0, _0805EE28 @ =0x0201CFB0
	ldr r1, _0805EE2C @ =0x00000808
	add r0, r0, r1
	mov r1, #1
	ldrb r2, [r0]
	orr r1, r2
	mov r2, #2
	orr r1, r2
	strb r1, [r0]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0805EE24: .4byte 0x0201CFB8
_0805EE28: .4byte 0x0201CFB0
_0805EE2C: .4byte 0x00000808
	thumb_func_end TextCellsPutSjisString

