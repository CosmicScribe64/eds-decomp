	thumb_func_start SideDeckSwap_CancelExchange
SideDeckSwap_CancelExchange: @ 0x0806B2F8
	push {r4, lr}
	ldr r4, _0806B320 @ =0x0201DB20
	ldr r1, _0806B324 @ =0x00001C5A
	add r0, r4, r1
	mov r1, #0x1D
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	strb r1, [r0]
	ldr r0, _0806B328 @ =0x00001C3C
	add r2, r4, r0
	ldr r1, [r2]
	lsl r0, r1, #0xE
	lsr r3, r0, #0x1D
	cmp r3, #2
	beq _0806B32C
	cmp r3, #3
	beq _0806B36C
	b _0806B39E
	.align 2, 0
_0806B320: .4byte 0x0201DB20
_0806B324: .4byte 0x00001C5A
_0806B328: .4byte 0x00001C3C
_0806B32C:
	ldr r0, _0806B360 @ =0xFFFC7FFF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #9
	orr r0, r1
	str r0, [r2]
	ldr r1, _0806B364 @ =0x00001C3D
	add r2, r4, r1
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	ldr r2, _0806B368 @ =0x00001C1C
	add r4, r4, r2
	strb r3, [r4]
	mov r0, #2
	bl DeckEdit_StartListSlide
	ldrb r0, [r4]
	bl DeckEdit_DrawStatementLabels
	b _0806B39E
	.align 2, 0
_0806B360: .4byte 0xFFFC7FFF
_0806B364: .4byte 0x00001C3D
_0806B368: .4byte 0x00001C1C
_0806B36C:
	ldr r0, _0806B3A4 @ =0xFFFC7FFF
	and r0, r1
	mov r1, #0x80
	lsl r1, r1, #9
	orr r0, r1
	str r0, [r2]
	ldr r0, _0806B3A8 @ =0x00001C3D
	add r2, r4, r0
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	ldr r2, _0806B3AC @ =0x00001C1C
	add r4, r4, r2
	mov r0, #1
	strb r0, [r4]
	mov r0, #2
	bl DeckEdit_StartListSlide
	ldrb r0, [r4]
	bl DeckEdit_DrawStatementLabels
_0806B39E:
	pop {r4}
	pop {r0}
	bx r0
_0806B3A4: .4byte 0xFFFC7FFF
_0806B3A8: .4byte 0x00001C3D
_0806B3AC: .4byte 0x00001C1C
	thumb_func_end SideDeckSwap_CancelExchange

