	thumb_func_start TextDrawGlyph
TextDrawGlyph: @ 0x08074E20
	push {r4, r5, lr}
	add r4, r1, #0
	add r5, r2, #0
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	ldr r1, _08074E48 @ =0x02011C20
	mov r0, #0x80
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	beq _08074E4C
	add r0, r2, #0
	add r1, r4, #0
	add r2, r5, #0
	bl TextDrawSjisGlyph
	b _08074E58
	.align 2, 0
_08074E48: .4byte 0x02011C20
_08074E4C:
	lsl r0, r2, #0x18
	lsr r0, r0, #0x18
	add r1, r4, #0
	add r2, r5, #0
	bl TextDrawLatinGlyph
_08074E58:
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end TextDrawGlyph
	.align 2, 0

