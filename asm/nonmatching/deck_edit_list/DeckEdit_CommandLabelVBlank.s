	thumb_func_start DeckEdit_CommandLabelVBlank
DeckEdit_CommandLabelVBlank: @ 0x08067540
	push {r4, r5, r6, lr}
	ldr r6, _08067568 @ =0x06010800
	ldr r1, _0806756C @ =0x0201DB20
	ldr r2, _08067570 @ =0x00001C5A
	add r0, r1, r2
	ldrb r0, [r0]
	lsl r0, r0, #0x1E
	lsr r0, r0, #0x1E
	add r2, r1, #0
	cmp r0, #1
	beq _08067578
	cmp r0, #2
	beq _080675B4
	ldr r3, _08067574 @ =0x00001C3C
	add r0, r1, r3
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1D
	b _080675D2
	.align 2, 0
_08067568: .4byte 0x06010800
_0806756C: .4byte 0x0201DB20
_08067570: .4byte 0x00001C5A
_08067574: .4byte 0x00001C3C
_08067578:
	ldr r3, _08067590 @ =0x00001C3C
	add r0, r1, r3
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1D
	cmp r0, #3
	bgt _08067598
	cmp r0, #2
	blt _08067598
	ldr r4, _08067594 @ =0x086F0FB0
	b _080675D8
	.align 2, 0
_08067590: .4byte 0x00001C3C
_08067594: .4byte gDeckEditSwapLabelGfx
_08067598:
	ldr r1, _080675AC @ =0x00001C3C
	add r0, r2, r1
	ldr r0, [r0]
	lsl r0, r0, #0xE
	lsr r0, r0, #0x1D
	lsl r0, r0, #0xA
	ldr r2, _080675B0 @ =0x086EF3B0
	add r4, r0, r2
	b _080675D8
	.align 2, 0
_080675AC: .4byte 0x00001C3C
_080675B0: .4byte gDeckEditCommandLabelGfx
_080675B4:
	ldr r3, _080675C8 @ =0x00001C3C
	add r0, r1, r3
	ldr r0, [r0]
	lsl r1, r0, #0xE
	lsr r0, r1, #0x1D
	cmp r0, #6
	bne _080675D0
	ldr r4, _080675CC @ =0x086F13B0
	b _080675D8
	.align 2, 0
_080675C8: .4byte 0x00001C3C
_080675CC: .4byte gDeckEditDecideLabelGfx
_080675D0:
	lsr r0, r1, #0x1D
_080675D2:
	lsl r0, r0, #0xA
	ldr r1, _0806761C @ =0x086EF3B0
	add r4, r0, r1
_080675D8:
	mov r5, #0
_080675DA:
	add r0, r4, #0
	add r1, r6, #0
	mov r2, #0x80
	bl CpuFastSet
	mov r2, #0x80
	lsl r2, r2, #2
	add r4, r4, r2
	mov r3, #0x80
	lsl r3, r3, #3
	add r6, r6, r3
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #1
	bls _080675DA
	ldr r1, _08067620 @ =0x04000042
	mov r0, #0xF0
	strh r0, [r1]
	ldr r2, _08067624 @ =0x04000046
	ldr r0, _08067628 @ =0x0201DB20
	ldr r1, _0806762C @ =0x00001C3D
	add r0, r0, r1
	ldrb r0, [r0]
	lsl r0, r0, #0x19
	lsr r0, r0, #0x1E
	lsl r0, r0, #0xB
	mov r1, #0x70
	orr r0, r1
	strh r0, [r2]
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0806761C: .4byte gDeckEditCommandLabelGfx
_08067620: .4byte 0x04000042
_08067624: .4byte 0x04000046
_08067628: .4byte 0x0201DB20
_0806762C: .4byte 0x00001C3D
	thumb_func_end DeckEdit_CommandLabelVBlank

