	thumb_func_start SjisToGlyphIndex
SjisToGlyphIndex: @ 0x08072584
	lsl r0, r0, #0x10
	lsr r1, r0, #0x18
	lsl r0, r0, #8
	mov r2, #0xC0
	lsl r2, r2, #0x18
	add r0, r0, r2
	lsr r2, r0, #0x18
	cmp r1, #0x9F
	bhi _0807259C
	add r0, r1, #0
	add r0, #0x80
	b _080725A0
_0807259C:
	add r0, r1, #0
	add r0, #0x40
_080725A0:
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	lsl r0, r1, #1
	add r0, r0, r1
	lsl r0, r0, #6
	add r0, r2, r0
	bx lr
	thumb_func_end SjisToGlyphIndex
	.align 2, 0

