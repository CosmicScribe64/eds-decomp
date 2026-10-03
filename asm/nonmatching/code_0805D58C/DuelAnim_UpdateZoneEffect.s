	thumb_func_start DuelAnim_UpdateZoneEffect
DuelAnim_UpdateZoneEffect: @ 0x0805DB90
	push {r4, r5, r6, lr}
	ldr r5, _0805DBD0 @ =0x0201D7F0
	ldr r2, [r5]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl GetAreaX
	add r6, r0, #0
	ldr r2, [r5]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r2, #0x1B
	lsr r1, r1, #0x1C
	lsl r2, r2, #0x12
	lsr r2, r2, #0x17
	bl GetAreaY
	add r2, r0, #0
	add r4, r5, #0
	sub r4, #8
	ldrb r0, [r4]
	cmp r0, #1
	beq _0805DBE6
	cmp r0, #1
	bgt _0805DBD4
	cmp r0, #0
	beq _0805DBDA
	b _0805DC16
_0805DBD0: .4byte 0x0201D7F0
_0805DBD4:
	cmp r0, #2
	beq _0805DBF2
	b _0805DC16
_0805DBDA:
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl DuelCursor_Select
	b _0805DC0E
_0805DBE6:
	add r0, r5, #0
	sub r0, #0xC
	ldr r0, [r0]
	bl DuelSprAnim_Load
	b _0805DC0E
_0805DBF2:
	sub r0, r5, #6
	mov r1, #0
	ldsh r0, [r0, r1]
	add r0, r6, r0
	sub r1, r5, #4
	mov r3, #0
	ldsh r1, [r1, r3]
	add r1, r2, r1
	mov r2, #1
	bl DuelSprAnim_Draw
	ldrh r0, [r5, #0x12]
	cmp r0, #0
	bne _0805DC2C
_0805DC0E:
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0805DC2C
_0805DC16:
	bl LoadDuelUiGfx
	ldr r1, _0805DC34 @ =0x0201CFB0
	mov r0, #0x83
	lsl r0, r0, #4
	add r1, r1, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
_0805DC2C:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_0805DC34: .4byte 0x0201CFB0
	thumb_func_end DuelAnim_UpdateZoneEffect

