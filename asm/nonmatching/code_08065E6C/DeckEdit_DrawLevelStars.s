	thumb_func_start DeckEdit_DrawLevelStars
DeckEdit_DrawLevelStars: @ 0x08065E6C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	lsl r1, r1, #0x10
	lsr r4, r1, #0x10
	lsl r2, r2, #0x10
	lsr r5, r2, #0x10
	lsl r3, r3, #0x18
	lsr r3, r3, #0x18
	mov r9, r3
	mov r8, r4
	mov r7, #0
	ldr r3, _08065EC0 @ =0x0201DB20
	ldr r1, _08065EC4 @ =0x00001C1C
	add r0, r3, r1
	ldrb r0, [r0]
	mov r2, #0xA5
	lsl r2, r2, #5
	add r1, r3, r2
	add r1, r0, r1
	ldrb r1, [r1]
	lsl r2, r0, #1
	mov r6, #0xC4
	lsl r6, r6, #3
	add r3, r3, r6
	add r2, r2, r3
	ldrh r2, [r2]
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r1, _08065EC8 @ =0x08621DE0
	add r6, r0, r1
	ldr r0, [r6]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r3, r0, #0x14
	b _08065EF8
_08065EC0: .4byte 0x0201DB20
_08065EC4: .4byte 0x00001C1C
_08065EC8: .4byte gCardStats
_08065ECC:
	mov r2, #0x1F
	add r1, r4, #0
	and r1, r2
	add r0, r5, #0
	and r0, r2
	lsl r0, r0, #5
	add r1, r1, r0
	lsl r1, r1, #1
	add r1, sl
	mov r2, #0xCD
	lsl r2, r2, #1
	add r0, r2, #0
	strh r0, [r1]
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r7, r9
	bne _08065EF8
	mov r4, r8
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
_08065EF8:
	add r2, r7, #0
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r7, r0, #0x18
	cmp r3, #0x15
	blt _08065F16
	cmp r3, #0x17
	ble _08065F0E
	cmp r3, #0x18
	beq _08065F12
	b _08065F16
_08065F0E:
	mov r0, #0
	b _08065F20
_08065F12:
	mov r0, #0xA
	b _08065F20
_08065F16:
	ldr r0, [r6]
	mov r1, #0xF0
	lsl r1, r1, #0x15
	and r0, r1
	lsr r0, r0, #0x19
_08065F20:
	cmp r2, r0
	bcc _08065ECC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end DeckEdit_DrawLevelStars
	.align 2, 0

