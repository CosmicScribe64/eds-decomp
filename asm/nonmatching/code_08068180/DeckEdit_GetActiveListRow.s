	thumb_func_start DeckEdit_GetActiveListRow
DeckEdit_GetActiveListRow: @ 0x08068668
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	beq _080686A0
	cmp r0, #1
	bgt _0806867A
	cmp r0, #0
	beq _08068680
	b _080686D8
_0806867A:
	cmp r0, #2
	beq _080686C4
	b _080686D8
_08068680:
	ldr r1, _08068698 @ =0x0201DB20
	mov r0, #0xA5
	lsl r0, r0, #5
	add r2, r1, r0
	mov r0, #0xE5
	lsl r0, r0, #3
	ldrb r2, [r2]
	mul r0, r2
	ldr r2, _0806869C @ =0x00000644
	add r1, r1, r2
	add r0, r0, r1
	b _080686D8
_08068698: .4byte 0x0201DB20
_0806869C: .4byte 0x00000644
_080686A0:
	ldr r1, _080686B8 @ =0x0201DB20
	ldr r0, _080686BC @ =0x000014A1
	add r2, r1, r0
	mov r0, #0xE5
	lsl r0, r0, #3
	ldrb r2, [r2]
	mul r0, r2
	ldr r2, _080686C0 @ =0x00000CAE
	add r1, r1, r2
	add r0, r0, r1
	b _080686D8
	.align 2, 0
_080686B8: .4byte 0x0201DB20
_080686BC: .4byte 0x000014A1
_080686C0: .4byte 0x00000CAE
_080686C4:
	ldr r1, _080686DC @ =0x0201DB20
	ldr r0, _080686E0 @ =0x000014A2
	add r2, r1, r0
	mov r0, #0xE5
	lsl r0, r0, #3
	ldrb r2, [r2]
	mul r0, r2
	ldr r2, _080686E4 @ =0x00000D4E
	add r1, r1, r2
	add r0, r0, r1
_080686D8:
	bx lr
	.align 2, 0
_080686DC: .4byte 0x0201DB20
_080686E0: .4byte 0x000014A2
_080686E4: .4byte 0x00000D4E
	thumb_func_end DeckEdit_GetActiveListRow

