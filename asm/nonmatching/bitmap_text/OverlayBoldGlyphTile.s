	thumb_func_start OverlayBoldGlyphTile
OverlayBoldGlyphTile: @ 0x08079FDC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r5, r0, #0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r0, #8
	mov sl, r0
	ldrb r1, [r1]
	cmp r3, #4
	bne _08079FFE
	mov r0, #0xF
	and r2, r0
_08079FFE:
	lsl r0, r1, #3
	ldr r1, _0807A0AC @ =0x0822BB00
	add r4, r0, r1
	cmp r3, #8
	bne _0807A0C0
	mov r6, #0
	ldr r0, _0807A0B0 @ =0xFFFFFF00
	mov r9, r0
	ldr r1, _0807A0B4 @ =0xFFFF00FF
	mov r8, r1
	lsl r7, r2, #8
	ldr r0, _0807A0B8 @ =0xFF00FFFF
	mov ip, r0
_0807A018:
	ldr r1, [r5]
	ldrb r3, [r4]
	mov r0, #0x80
	and r0, r3
	cmp r0, #0
	beq _0807A02A
	mov r0, r9
	and r0, r1
	add r1, r0, r2
_0807A02A:
	mov r0, #0x40
	and r0, r3
	cmp r0, #0
	beq _0807A038
	mov r0, r8
	and r0, r1
	add r1, r0, r7
_0807A038:
	mov r0, #0x20
	and r0, r3
	cmp r0, #0
	beq _0807A048
	mov r0, ip
	and r0, r1
	lsl r1, r2, #0x10
	add r1, r0, r1
_0807A048:
	mov r0, #0x10
	and r0, r3
	cmp r0, #0
	beq _0807A058
	ldr r0, _0807A0BC @ =0x00FFFFFF
	and r0, r1
	lsl r1, r2, #0x18
	add r1, r0, r1
_0807A058:
	stmia r5!, {r1}
	ldr r1, [r5]
	ldrb r3, [r4]
	mov r0, #8
	and r0, r3
	cmp r0, #0
	beq _0807A06C
	mov r0, r9
	and r0, r1
	add r1, r0, r2
_0807A06C:
	mov r0, #4
	and r0, r3
	cmp r0, #0
	beq _0807A07A
	mov r0, r8
	and r0, r1
	add r1, r0, r7
_0807A07A:
	mov r0, #2
	and r0, r3
	cmp r0, #0
	beq _0807A08A
	mov r0, ip
	and r0, r1
	lsl r1, r2, #0x10
	add r1, r0, r1
_0807A08A:
	mov r0, #1
	and r0, r3
	cmp r0, #0
	beq _0807A09A
	ldr r0, _0807A0BC @ =0x00FFFFFF
	and r0, r1
	lsl r1, r2, #0x18
	add r1, r0, r1
_0807A09A:
	stmia r5!, {r1}
	add r4, #1
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r6, sl
	bcc _0807A018
	b _0807A156
	.align 2, 0
_0807A0AC: .4byte gFontLatin8x8Bold
_0807A0B0: .4byte 0xFFFFFF00
_0807A0B4: .4byte 0xFFFF00FF
_0807A0B8: .4byte 0xFF00FFFF
_0807A0BC: .4byte 0x00FFFFFF
_0807A0C0:
	mov r6, #0
_0807A0C2:
	ldr r1, [r5]
	ldrb r3, [r4]
	mov r0, #0x80
	and r0, r3
	cmp r0, #0
	beq _0807A0D6
	mov r0, #0x10
	neg r0, r0
	and r0, r1
	add r1, r0, r2
_0807A0D6:
	mov r0, #0x40
	and r0, r3
	cmp r0, #0
	beq _0807A0E8
	mov r0, #0xF1
	neg r0, r0
	and r0, r1
	lsl r1, r2, #4
	add r1, r0, r1
_0807A0E8:
	mov r0, #0x20
	and r0, r3
	cmp r0, #0
	beq _0807A0F8
	ldr r0, _0807A164 @ =0xFFFFF0FF
	and r0, r1
	lsl r1, r2, #8
	add r1, r0, r1
_0807A0F8:
	mov r0, #0x10
	and r0, r3
	cmp r0, #0
	beq _0807A108
	ldr r0, _0807A168 @ =0xFFFF0FFF
	and r0, r1
	lsl r1, r2, #0xC
	add r1, r0, r1
_0807A108:
	mov r0, #8
	and r0, r3
	cmp r0, #0
	beq _0807A118
	ldr r0, _0807A16C @ =0xFFF0FFFF
	and r0, r1
	lsl r1, r2, #0x10
	add r1, r0, r1
_0807A118:
	mov r0, #4
	and r0, r3
	cmp r0, #0
	beq _0807A128
	ldr r0, _0807A170 @ =0xFF0FFFFF
	and r0, r1
	lsl r1, r2, #0x14
	add r1, r0, r1
_0807A128:
	mov r0, #2
	and r0, r3
	cmp r0, #0
	beq _0807A138
	ldr r0, _0807A174 @ =0xF0FFFFFF
	and r0, r1
	lsl r1, r2, #0x18
	add r1, r0, r1
_0807A138:
	mov r0, #1
	and r0, r3
	cmp r0, #0
	beq _0807A148
	ldr r0, _0807A178 @ =0x0FFFFFFF
	and r0, r1
	lsl r1, r2, #0x1C
	add r1, r0, r1
_0807A148:
	stmia r5!, {r1}
	add r4, #1
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	cmp r6, sl
	bcc _0807A0C2
_0807A156:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0807A164: .4byte 0xFFFFF0FF
_0807A168: .4byte 0xFFFF0FFF
_0807A16C: .4byte 0xFFF0FFFF
_0807A170: .4byte 0xFF0FFFFF
_0807A174: .4byte 0xF0FFFFFF
_0807A178: .4byte 0x0FFFFFFF
	thumb_func_end OverlayBoldGlyphTile

