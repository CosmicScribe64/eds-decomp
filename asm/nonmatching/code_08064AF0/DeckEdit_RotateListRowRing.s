	thumb_func_start DeckEdit_RotateListRowRing
DeckEdit_RotateListRowRing: @ 0x08065058
	push {r4, r5, lr}
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	beq _08065068
	cmp r0, #2
	beq _0806509C
	b _080650C4
_08065068:
	ldr r0, _08065094 @ =0x0201DB20
	ldr r1, _08065098 @ =0x00001C34
	add r4, r0, r1
	ldrb r2, [r4]
	lsl r0, r2, #0x1B
	lsr r0, r0, #0x1C
	sub r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r1, r0, #1
	mov r5, #0x1F
	neg r5, r5
	add r3, r5, #0
	and r3, r2
	orr r3, r1
	strb r3, [r4]
	cmp r0, #0xF
	bne _080650C4
	and r3, r5
	mov r0, #0xC
	orr r3, r0
	b _080650C2
_08065094: .4byte 0x0201DB20
_08065098: .4byte 0x00001C34
_0806509C:
	ldr r0, _080650CC @ =0x0201DB20
	ldr r1, _080650D0 @ =0x00001C34
	add r4, r0, r1
	ldrb r2, [r4]
	lsl r0, r2, #0x1B
	lsr r0, r0, #0x1C
	add r0, #1
	mov r1, #0xF
	and r0, r1
	lsl r1, r0, #1
	mov r5, #0x1F
	neg r5, r5
	add r3, r5, #0
	and r3, r2
	orr r3, r1
	strb r3, [r4]
	cmp r0, #7
	bne _080650C4
	and r3, r5
_080650C2:
	strb r3, [r4]
_080650C4:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
_080650CC: .4byte 0x0201DB20
_080650D0: .4byte 0x00001C34
	thumb_func_end DeckEdit_RotateListRowRing

