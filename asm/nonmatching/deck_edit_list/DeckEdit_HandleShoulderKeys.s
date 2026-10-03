	thumb_func_start DeckEdit_HandleShoulderKeys
DeckEdit_HandleShoulderKeys: @ 0x08067474
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _080674CC @ =0x03000040
	ldrh r1, [r0, #6]
	mov r0, #0x80
	lsl r0, r0, #1
	and r0, r1
	cmp r0, #0
	beq _080674E0
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #3
	bne _08067498
	mov r0, #0
	strb r0, [r4]
_08067498:
	ldr r2, _080674D0 @ =0x0201DB20
	ldr r0, _080674D4 @ =0x00001C3C
	add r3, r2, r0
	ldr r0, [r3]
	ldr r1, _080674D8 @ =0xFFFC7FFF
	and r0, r1
	str r0, [r3]
	ldr r1, _080674DC @ =0x00001C3D
	add r2, r2, r1
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #2
	bl DeckEdit_StartListSlide
	ldrb r0, [r4]
	bl DeckEdit_DrawStatementLabels
	mov r0, #0
	bl PlaySE
	b _0806752A
_080674CC: .4byte 0x03000040
_080674D0: .4byte 0x0201DB20
_080674D4: .4byte 0x00001C3C
_080674D8: .4byte 0xFFFC7FFF
_080674DC: .4byte 0x00001C3D
_080674E0:
	mov r0, #0x80
	lsl r0, r0, #2
	and r0, r1
	cmp r0, #0
	beq _0806752A
	ldrb r0, [r4]
	cmp r0, #0
	beq _080674F4
	sub r0, #1
	b _080674F6
_080674F4:
	mov r0, #2
_080674F6:
	strb r0, [r4]
	ldr r2, _08067530 @ =0x0201DB20
	ldr r0, _08067534 @ =0x00001C3C
	add r3, r2, r0
	ldr r0, [r3]
	ldr r1, _08067538 @ =0xFFFC7FFF
	and r0, r1
	str r0, [r3]
	ldr r1, _0806753C @ =0x00001C3D
	add r2, r2, r1
	mov r0, #8
	neg r0, r0
	ldrb r1, [r2]
	and r0, r1
	mov r1, #3
	orr r0, r1
	strb r0, [r2]
	mov r0, #3
	bl DeckEdit_StartListSlide
	ldrb r0, [r4]
	bl DeckEdit_DrawStatementLabels
	mov r0, #0
	bl PlaySE
_0806752A:
	pop {r4}
	pop {r0}
	bx r0
_08067530: .4byte 0x0201DB20
_08067534: .4byte 0x00001C3C
_08067538: .4byte 0xFFFC7FFF
_0806753C: .4byte 0x00001C3D
	thumb_func_end DeckEdit_HandleShoulderKeys

