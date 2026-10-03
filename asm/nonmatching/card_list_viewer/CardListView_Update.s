	thumb_func_start CardListView_Update
CardListView_Update: @ 0x0802AB0C
	push {r4, r5, lr}
	mov r5, #1
	ldr r4, _0802AB78 @ =0x0201D810
	ldrb r1, [r4]
	mov r0, #4
	and r0, r1
	cmp r0, #0
	beq _0802AB2C
	mov r0, #5
	neg r0, r0
	and r0, r1
	strb r0, [r4]
	ldr r0, _0802AB7C @ =0x06004200
	mov r1, #0
	bl TextCanvasToTiles
_0802AB2C:
	ldrb r3, [r4, #5]
	mov r0, #0x60
	and r0, r3
	cmp r0, #0
	beq _0802ABD8
	mov r0, #0x1C
	and r0, r3
	cmp r0, #0
	beq _0802AB8C
	lsl r0, r3, #0x1B
	lsr r0, r0, #0x1D
	sub r0, #1
	mov r1, #7
	and r0, r1
	lsl r0, r0, #2
	mov r2, #0x1D
	neg r2, r2
	and r2, r3
	orr r2, r0
	strb r2, [r4, #5]
	ldr r3, _0802AB80 @ =0x03000040
	ldr r4, _0802AB84 @ =0x0819A788
	mov r1, #0x1C
	and r1, r2
	lsl r0, r2, #0x19
	lsr r0, r0, #0x1E
	lsl r0, r0, #4
	add r1, r1, r0
	add r1, r1, r4
	lsl r2, r2, #0x1E
	lsr r2, r2, #0x1A
	ldrh r1, [r1]
	sub r2, r1, r2
	ldr r0, _0802AB88 @ =0x00004422
	add r3, r3, r0
	strh r2, [r3]
	mov r5, #0
	b _0802ABD8
_0802AB78: .4byte 0x0201D810
_0802AB7C: .4byte 0x06004200
_0802AB80: .4byte 0x03000040
_0802AB84: .4byte gCardListViewCursorSlide
_0802AB88: .4byte 0x00004422
_0802AB8C:
	lsl r0, r3, #0x19
	lsr r0, r0, #0x1E
	cmp r0, #1
	beq _0802AB9A
	cmp r0, #2
	beq _0802ABA2
	b _0802ABB6
_0802AB9A:
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1E
	sub r0, #1
	b _0802ABA8
_0802ABA2:
	lsl r0, r3, #0x1E
	lsr r0, r0, #0x1E
	add r0, #1
_0802ABA8:
	mov r1, #3
	and r0, r1
	mov r1, #4
	neg r1, r1
	and r1, r3
	orr r1, r0
	strb r1, [r4, #5]
_0802ABB6:
	ldr r0, _0802AC20 @ =0x0201D810
	mov r1, #0x61
	neg r1, r1
	ldrb r2, [r0, #5]
	and r1, r2
	strb r1, [r0, #5]
	ldr r0, _0802AC24 @ =0x03000040
	lsl r1, r1, #0x1E
	lsr r1, r1, #0x1A
	neg r1, r1
	ldr r2, _0802AC28 @ =0x00004422
	add r0, r0, r2
	strh r1, [r0]
	bl CardListView_DrawSelectedInfo
	bl CardListView_DrawSelectedCursorFrame
_0802ABD8:
	ldr r4, _0802AC20 @ =0x0201D810
	mov r0, #0x10
	ldrb r1, [r4]
	and r0, r1
	cmp r0, #0
	beq _0802AC18
	ldrb r1, [r4, #8]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1C
	bl CardListView_DrawButtons
	cmp r5, #0
	beq _0802AC18
	mov r2, #0xC3
	lsl r2, r2, #2
	add r0, r4, r2
	ldrh r0, [r0]
	cmp r0, #0
	beq _0802AC18
	ldrb r1, [r4, #5]
	lsl r0, r1, #0x1E
	lsr r0, r0, #0x1E
	ldrh r2, [r4, #6]
	add r0, r2, r0
	lsl r0, r0, #2
	add r1, r4, #0
	add r1, #0xC
	add r0, r0, r1
	bl CardListView_DrawCardStatus
_0802AC18:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_0802AC20: .4byte 0x0201D810
_0802AC24: .4byte 0x03000040
_0802AC28: .4byte 0x00004422
	thumb_func_end CardListView_Update

