	thumb_func_start DeckReorder_Draw
DeckReorder_Draw: @ 0x08053864
	push {lr}
	ldr r1, _0805387C @ =0x02017A40
	ldr r0, _08053880 @ =0x0000053C
	add r2, r1, r0
	ldr r0, [r2]
	lsl r0, r0, #0xC
	lsr r0, r0, #0x18
	cmp r0, #0xA
	beq _08053884
	cmp r0, #0x14
	beq _0805389C
	b _080538BC
_0805387C: .4byte 0x02017A40
_08053880: .4byte 0x0000053C
_08053884:
	ldr r3, _08053898 @ =0x0000053E
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x18
	cmp r0, #0xF
	bgt _080538BC
	ldrb r2, [r2]
	sub r1, r2, #1
	b _080538AE
_08053898: .4byte 0x0000053E
_0805389C:
	ldr r3, _080538B8 @ =0x0000053E
	add r0, r1, r3
	ldrh r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x18
	cmp r0, #0xF
	bgt _080538BC
	ldrb r1, [r2]
	add r2, r1, #1
_080538AE:
	mov r0, #0
	bl DeckReorder_DrawSwap
	b _080538C2
	.align 2, 0
_080538B8: .4byte 0x0000053E
_080538BC:
	mov r0, #0
	bl DeckReorder_DrawCards
_080538C2:
	pop {r0}
	bx r0
	thumb_func_end DeckReorder_Draw
	.align 2, 0

