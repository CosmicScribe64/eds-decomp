	thumb_func_start GetPack_DrawCardSprites
GetPack_DrawCardSprites: @ 0x080624A4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r6, #0
	ldr r4, _080624F0 @ =0x02015160
	mov r8, r6
	mov r7, #0
	ldr r0, _080624F4 @ =0x0000FFFF
	mov sl, r0
_080624BA:
	mov r1, #0x86
	lsl r1, r1, #1
	add r0, r4, r1
	add r0, r6, r0
	ldrb r2, [r0]
	cmp r2, #0x17
	bhi _08062574
	lsl r0, r2, #1
	ldr r1, _080624F8 @ =0x0808659C
	add r3, r0, r1
	mov r2, #0x80
	lsl r2, r2, #5
	add r0, r2, #0
	ldrh r1, [r3]
	and r0, r1
	cmp r0, #0
	beq _08062564
	mov r2, #0x81
	lsl r2, r2, #1
	add r0, r4, r2
	add r0, r8
	ldrh r1, [r0]
	add r2, r1, #0
	cmp r1, sl
	bne _080624FC
	mov r0, #0
	b _08062532
_080624F0: .4byte 0x02015160
_080624F4: .4byte 0x0000FFFF
_080624F8: .4byte gCardFlipAnimTiles
_080624FC:
	ldr r0, _08062514 @ =0x000007CF
	cmp r1, r0
	bhi _08062520
	ldr r2, _08062518 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0806251C @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _08062532
	.align 2, 0
_08062514: .4byte 0x000007CF
_08062518: .4byte 0x000007FF
_0806251C: .4byte gCardNumberToId
_08062520:
	ldr r1, _08062554 @ =0xFFFFF830
	add r0, r2, r1
	ldr r1, _08062558 @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r2, _0806255C @ =0x08623DF4
	add r0, r0, r2
	ldrh r0, [r0]
	add r0, #1
_08062532:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GetCardIconObjTile
	mov r2, #0x86
	lsl r2, r2, #1
	add r1, r4, r2
	add r1, r6, r1
	ldrb r1, [r1]
	lsl r1, r1, #1
	ldr r2, _08062560 @ =0x0808659C
	add r1, r1, r2
	ldrh r1, [r1]
	add r0, r1, r0
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	b _08062566
_08062554: .4byte 0xFFFFF830
_08062558: .4byte 0x000007FF
_0806255C: .4byte gCardNumberToId
_08062560: .4byte gCardFlipAnimTiles
_08062564:
	ldrh r3, [r3]
_08062566:
	mov r0, #4
	neg r0, r0
	add r1, r7, #0
	mov r2, #0x80
	bl AddSpriteXY
	b _080625DA
_08062574:
	mov r0, #4
	neg r0, r0
	mov r9, r0
	add r5, r7, #0
	mov r1, #0x81
	lsl r1, r1, #1
	add r0, r4, r1
	add r0, r8
	ldrh r1, [r0]
	cmp r1, sl
	bne _0806258E
	mov r0, #0
	b _080625C2
_0806258E:
	ldr r0, _080625A4 @ =0x000007CF
	cmp r1, r0
	bhi _080625B0
	ldr r2, _080625A8 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _080625AC @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	b _080625C2
_080625A4: .4byte 0x000007CF
_080625A8: .4byte 0x000007FF
_080625AC: .4byte gCardNumberToId
_080625B0:
	ldr r2, _080625F8 @ =0xFFFFF830
	add r0, r1, r2
	ldr r1, _080625FC @ =0x000007FF
	and r0, r1
	lsl r0, r0, #1
	ldr r1, _08062600 @ =0x08623DF4
	add r0, r0, r1
	ldrh r0, [r0]
	add r0, #1
_080625C2:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	bl GetCardIconObjTile
	mov r3, #0x80
	lsl r3, r3, #5
	orr r3, r0
	mov r0, r9
	add r1, r5, #0
	mov r2, #0x80
	bl AddSpriteXY
_080625DA:
	mov r2, #2
	add r8, r2
	add r7, #0x20
	add r6, #1
	cmp r6, #4
	bgt _080625E8
	b _080624BA
_080625E8:
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080625F8: .4byte 0xFFFFF830
_080625FC: .4byte 0x000007FF
_08062600: .4byte gCardNumberToId
	thumb_func_end GetPack_DrawCardSprites

