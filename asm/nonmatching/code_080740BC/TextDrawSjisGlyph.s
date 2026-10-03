	thumb_func_start TextDrawSjisGlyph
TextDrawSjisGlyph: @ 0x08074C80
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r1
	add r5, r2, #0
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	lsl r3, r3, #0x10
	lsr r4, r3, #0x18
	lsl r3, r3, #8
	lsr r3, r3, #0x18
	mov r8, r3
	cmp r4, #0xA
	beq _08074CEC
	cmp r4, #0xA
	bgt _08074CA8
	cmp r4, #8
	beq _08074CAE
	b _08074D36
_08074CA8:
	cmp r4, #0xC
	beq _08074D00
	b _08074D36
_08074CAE:
	bl SjisToGlyphIndex
	lsl r0, r0, #3
	ldr r1, _08074CE8 @ =0x081C0000
	add r6, r0, r1
	mov r7, #3
_08074CBA:
	ldrh r4, [r6]
	add r6, #2
	lsl r4, r4, #0x11
	lsl r0, r4, #8
	lsr r0, r0, #0x18
	add r2, r5, #0
	add r5, #1
	mov r1, r9
	mov r3, r8
	bl TextPlotRow8
	lsr r4, r4, #0x18
	add r2, r5, #0
	add r5, #1
	add r0, r4, #0
	mov r1, r9
	mov r3, r8
	bl TextPlotRow8
	sub r7, #1
	cmp r7, #0
	bge _08074CBA
	b _08074D36
_08074CE8: .4byte gFontKanji8x8
_08074CEC:
	bl SjisToGlyphIndex
	lsl r1, r0, #2
	add r1, r1, r0
	lsl r1, r1, #2
	ldr r0, _08074CFC @ =0x081D0200
	b _08074D0C
	.align 2, 0
_08074CFC: .4byte gFontKanji10x10
_08074D00:
	bl SjisToGlyphIndex
	lsl r1, r0, #1
	add r1, r1, r0
	lsl r1, r1, #3
	ldr r0, _08074D44 @ =0x081F8700
_08074D0C:
	add r6, r1, r0
	cmp r4, #0
	beq _08074D36
	add r7, r4, #0
_08074D14:
	ldrh r0, [r6]
	add r6, #2
	lsr r1, r0, #8
	lsl r0, r0, #0x18
	lsr r0, r0, #0x10
	orr r0, r1
	lsl r0, r0, #0x11
	lsr r0, r0, #0x10
	add r2, r5, #0
	add r5, #1
	mov r1, r9
	mov r3, r8
	bl TextPlotRow16
	sub r7, #1
	cmp r7, #0
	bne _08074D14
_08074D36:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08074D44: .4byte gFontKanji12x12
	thumb_func_end TextDrawSjisGlyph

