	thumb_func_start DeckEdit_CountSideDeckMonsters
DeckEdit_CountSideDeckMonsters: @ 0x0806710C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r2, #0
	ldr r0, _080671C8 @ =0x0201DB20
	mov sl, r0
_0806711C:
	lsl r0, r2, #1
	ldr r1, _080671C8 @ =0x0201DB20
	ldr r3, _080671CC @ =0x00001712
	add r4, r1, r3
	add r3, r0, r4
	mov r1, #0
	strh r1, [r3]
	mov r5, #0
	add r0, r0, r2
	lsl r1, r0, #1
	ldr r6, _080671D0 @ =0x0201EFB8
	add r0, r1, r6
	add r6, r2, #1
	mov r9, r6
	ldrh r0, [r0]
	cmp r5, r0
	bcs _080671A2
	lsl r0, r2, #0x18
	lsr r7, r0, #0x18
	add r6, r3, #0
	ldr r2, _080671D4 @ =0xFFFFFD86
	add r0, r4, r2
	add r1, r1, r0
	mov r8, r1
	ldr r3, _080671D8 @ =0xFFFFE8EE
	add r3, r3, r4
	mov sl, r3
_08067152:
	mov r0, #2
	add r1, r7, #0
	add r2, r5, #0
	bl DeckEdit_GetListCard
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r1, _080671DC @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _08067176
	cmp r0, #0x15
	bge _08067194
_08067176:
	mov r0, #2
	add r1, r7, #0
	add r2, r5, #0
	bl DeckEdit_GetListCard
	ldr r1, _080671E0 @ =0x02011C20
	lsl r0, r0, #0x10
	lsr r0, r0, #0xE
	add r0, r0, r1
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	ldrh r2, [r6]
	add r0, r2, r0
	strh r0, [r6]
_08067194:
	add r0, r5, #1
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	mov r3, r8
	ldrh r3, [r3]
	cmp r5, r3
	bcc _08067152
_080671A2:
	mov r6, r9
	lsl r0, r6, #0x10
	lsr r2, r0, #0x10
	cmp r2, #1
	bls _0806711C
	ldr r0, _080671CC @ =0x00001712
	add r0, sl
	ldrh r1, [r0]
	ldr r0, _080671E4 @ =0x00001714
	add r0, sl
	strh r1, [r0]
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080671C8: .4byte 0x0201DB20
_080671CC: .4byte 0x00001712
_080671D0: .4byte 0x0201EFB8
_080671D4: .4byte 0xFFFFFD86
_080671D8: .4byte 0xFFFFE8EE
_080671DC: .4byte gCardStats
_080671E0: .4byte 0x02011C20
_080671E4: .4byte 0x00001714
	thumb_func_end DeckEdit_CountSideDeckMonsters

