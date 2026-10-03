	thumb_func_start GetCardCopiesInList
GetCardCopiesInList: @ 0x0806C534
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add r3, r0, #0
	lsl r1, r1, #0x10
	lsr r2, r1, #0x10
	cmp r0, #1
	beq _0806C564
	cmp r0, #1
	bgt _0806C54C
	cmp r0, #0
	beq _0806C552
	b _0806C588
_0806C54C:
	cmp r3, #2
	beq _0806C57C
	b _0806C588
_0806C552:
	ldr r0, _0806C560 @ =0x02011C20
	lsl r1, r2, #2
	add r1, r1, r0
	ldrh r1, [r1, #8]
	lsl r0, r1, #0x16
	lsr r0, r0, #0x16
	b _0806C588
_0806C560: .4byte 0x02011C20
_0806C564:
	ldr r1, _0806C578 @ =0x02011C20
	lsl r0, r2, #2
	add r0, r0, r1
	ldrb r1, [r0, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	lsr r1, r1, #6
	add r0, r0, r1
	b _0806C588
	.align 2, 0
_0806C578: .4byte 0x02011C20
_0806C57C:
	ldr r0, _0806C58C @ =0x02011C20
	lsl r1, r2, #2
	add r1, r1, r0
	ldrb r1, [r1, #9]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1E
_0806C588:
	bx lr
	.align 2, 0
_0806C58C: .4byte 0x02011C20
	thumb_func_end GetCardCopiesInList

