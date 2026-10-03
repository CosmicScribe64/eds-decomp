	thumb_func_start DeckEdit_FlipCursorRowPage
DeckEdit_FlipCursorRowPage: @ 0x080650D4
	push {r4, r5, lr}
	ldr r2, _08065100 @ =0x0201DB20
	ldr r0, _08065104 @ =0x00001C34
	add r2, r2, r0
	ldrb r5, [r2]
	lsl r1, r5, #0x1F
	lsr r1, r1, #0x1F
	add r1, #1
	mov r4, #1
	and r1, r4
	mov r3, #2
	neg r3, r3
	add r0, r3, #0
	and r0, r5
	orr r0, r1
	and r1, r4
	and r0, r3
	orr r0, r1
	strb r0, [r2]
	pop {r4, r5}
	pop {r1}
	bx r1
_08065100: .4byte 0x0201DB20
_08065104: .4byte 0x00001C34
	thumb_func_end DeckEdit_FlipCursorRowPage

