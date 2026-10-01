	thumb_func_start sub_0803B29C
sub_0803B29C: @ 0x0803B29C
	push {r4, r5, lr}
	add r4, r0, #0
	ldr r0, _0803B2B8 @ =0x02017A40
	mov r1, #0xF8
	lsl r1, r1, #2
	add r0, r0, r1
	ldrb r0, [r0]
	cmp r0, #0x7F
	beq _0803B34C
	cmp r0, #0x7F
	bgt _0803B2BC
	cmp r0, #0x7E
	beq _0803B368
	b _0803B474
_0803B2B8: .4byte 0x02017A40
_0803B2BC:
	cmp r0, #0x80
	beq _0803B2C2
	b _0803B474
_0803B2C2:
	ldrb r2, [r4, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldr r5, _0803B2F8 @ =0x0000058D
	add r1, r5, #0
	mov r2, #0
	bl sub_08044224
	cmp r0, #0
	bne _0803B2D8
	b _0803B474
_0803B2D8:
	ldr r0, _0803B2FC @ =0x000007FF
	ldrh r4, [r4]
	and r0, r4
	lsl r0, r0, #1
	ldr r3, _0803B300 @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	ldr r0, _0803B304 @ =0x00000596
	cmp r1, r0
	beq _0803B31C
	cmp r1, r0
	bgt _0803B308
	cmp r1, r5
	beq _0803B330
	b _0803B474
	.align 2, 0
_0803B2F8: .4byte 0x0000058D
_0803B2FC: .4byte 0x000007FF
_0803B300: .4byte gUnk_08622AB4
_0803B304: .4byte 0x00000596
_0803B308:
	ldr r0, _0803B318 @ =0x00000599
	cmp r1, r0
	beq _0803B31C
	add r0, #0xB
	cmp r1, r0
	beq _0803B330
	b _0803B474
	.align 2, 0
_0803B318: .4byte 0x00000599
_0803B31C:
	ldr r0, _0803B324 @ =0x00000206
	ldr r1, _0803B328 @ =0x00000712
	ldr r3, _0803B32C @ =0x0808372C
	b _0803B336
_0803B324: .4byte 0x00000206
_0803B328: .4byte 0x00000712
_0803B32C: .4byte gUnk_0808372C
_0803B330:
	ldr r0, _0803B340 @ =0x00000206
	ldr r1, _0803B344 @ =0x00000712
	ldr r3, _0803B348 @ =0x08083764
_0803B336:
	mov r2, #0xB
	bl sub_080602A4
	mov r0, #0x7F
	b _0803B476
_0803B340: .4byte 0x00000206
_0803B344: .4byte 0x00000712
_0803B348: .4byte gUnk_08083764
_0803B34C:
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	neg r1, r1
	ldr r2, _0803B364 @ =0x0000058D
	mov r3, #0
	bl sub_0802AF34
	mov r0, #0x7E
	b _0803B476
	.align 2, 0
_0803B364: .4byte 0x0000058D
_0803B368:
	ldr r0, _0803B388 @ =0x000007FF
	ldrh r1, [r4]
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0803B38C @ =0x08622AB4
	add r0, r0, r2
	ldrh r1, [r0]
	ldr r0, _0803B390 @ =0x00000596
	cmp r1, r0
	beq _0803B3A8
	cmp r1, r0
	bgt _0803B394
	sub r0, #9
	cmp r1, r0
	beq _0803B43C
	b _0803B474
_0803B388: .4byte 0x000007FF
_0803B38C: .4byte gUnk_08622AB4
_0803B390: .4byte 0x00000596
_0803B394:
	ldr r0, _0803B3A4 @ =0x00000599
	cmp r1, r0
	beq _0803B3DC
	add r0, #0xB
	cmp r1, r0
	beq _0803B410
	b _0803B474
	.align 2, 0
_0803B3A4: .4byte 0x00000599
_0803B3A8:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r2, #0xD0
	cmp r0, #0
	beq _0803B3B6
	ldr r2, _0803B3D4 @ =0x000080D0
_0803B3B6:
	ldr r0, _0803B3D8 @ =0x0201D810
	ldrb r3, [r0, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r1, r1, r0
	ldrh r1, [r1, #0xC]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r2, #0
	mov r2, #0
	b _0803B462
	.align 2, 0
_0803B3D4: .4byte 0x000080D0
_0803B3D8: .4byte 0x0201D810
_0803B3DC:
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r2, #0xD1
	cmp r0, #0
	beq _0803B3EA
	ldr r2, _0803B408 @ =0x000080D1
_0803B3EA:
	ldr r0, _0803B40C @ =0x0201D810
	ldrb r3, [r0, #5]
	lsl r1, r3, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r1, r1, r0
	ldrh r1, [r1, #0xC]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	add r0, r2, #0
	mov r2, #0
	b _0803B462
	.align 2, 0
_0803B408: .4byte 0x000080D1
_0803B40C: .4byte 0x0201D810
_0803B410:
	ldr r0, _0803B434 @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r3, #0xD2
	cmp r0, #0
	beq _0803B45C
	ldr r3, _0803B438 @ =0x000080D2
	b _0803B45C
	.align 2, 0
_0803B434: .4byte 0x0201D810
_0803B438: .4byte 0x000080D2
_0803B43C:
	ldr r0, _0803B46C @ =0x0201D810
	ldrb r2, [r0, #5]
	lsl r1, r2, #0x1E
	lsr r1, r1, #0x1E
	ldrh r3, [r0, #6]
	add r1, r3, r1
	lsl r1, r1, #2
	add r0, #0xC
	add r2, r1, r0
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r3, #0xDB
	cmp r0, #0
	beq _0803B45C
	ldr r3, _0803B470 @ =0x000080DB
_0803B45C:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	add r0, r3, #0
_0803B462:
	mov r3, #0
	bl sub_0801EC58
	mov r0, #0x78
	b _0803B476
_0803B46C: .4byte 0x0201D810
_0803B470: .4byte 0x000080DB
_0803B474:
	mov r0, #0
_0803B476:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0803B29C

