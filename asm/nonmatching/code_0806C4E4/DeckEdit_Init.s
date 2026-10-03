	thumb_func_start DeckEdit_Init
DeckEdit_Init: @ 0x0806D1D8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r5, _0806D45C @ =0x0201DB20
	ldr r1, _0806D460 @ =0x00001C5C
	add r0, r5, #0
	bl MemClear16
	ldr r0, _0806D464 @ =0x03000040
	ldr r1, _0806D468 @ =0x0000040E
	add r0, r0, r1
	mov r4, #0
	mov r1, #1
	strh r1, [r0]
	ldr r0, _0806D46C @ =0x04000012
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #6
	strh r4, [r0]
	sub r0, #2
	strh r4, [r0]
	add r0, #0xC
	strh r4, [r0]
	add r0, #2
	strh r4, [r0]
	add r0, #0x12
	strh r4, [r0]
	add r0, #2
	strh r4, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _0806D470 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	add r0, r5, #0
	bl OamListClear
	ldr r2, _0806D474 @ =0x00000632
	add r0, r5, r2
	strh r4, [r0]
	mov r3, #0xC6
	lsl r3, r3, #3
	add r0, r5, r3
	strh r4, [r0]
	ldr r1, _0806D478 @ =0x0000063A
	add r0, r5, r1
	strh r4, [r0]
	add r2, #6
	add r0, r5, r2
	strh r4, [r0]
	add r3, #0xE
	add r0, r5, r3
	strh r4, [r0]
	add r1, #2
	add r0, r5, r1
	strh r4, [r0]
	mov r6, #0
	sub r2, #0x18
	add r2, r2, r5
	mov r9, r2
	mov r3, #0
	mov r8, r3
	mov r0, #0xA5
	lsl r0, r0, #5
	add r0, r0, r5
	mov ip, r0
	ldr r1, _0806D47C @ =0x00001494
	add r7, r5, r1
_0806D278:
	lsl r0, r6, #1
	mov r2, r9
	add r1, r0, r2
	strh r4, [r1]
	mov r3, ip
	add r1, r6, r3
	mov r2, r8
	strb r2, [r1]
	mov r2, #0
	add r1, r6, #1
	add r3, r0, #0
_0806D28E:
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r3, r0
	add r0, r0, r7
	strh r4, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r2, r0, #0x10
	cmp r2, #1
	bls _0806D28E
	lsl r0, r1, #0x10
	lsr r6, r0, #0x10
	cmp r6, #2
	bls _0806D278
	ldr r3, _0806D480 @ =0x00001C1D
	add r0, r5, r3
	mov r1, #0
	strb r1, [r0]
	ldr r2, _0806D484 @ =0x00001C1C
	add r0, r5, r2
	strb r1, [r0]
	ldr r3, _0806D488 @ =0x00000634
	add r0, r5, r3
	strb r1, [r0]
	ldr r2, _0806D48C @ =0x00000635
	add r0, r5, r2
	strb r1, [r0]
	ldr r3, _0806D490 @ =0x00001710
	add r1, r5, r3
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r3, _0806D494 @ =0x000018AC
	add r1, r5, r3
	mov r0, #0xFC
	lsl r0, r0, #8
	strh r0, [r1]
	mov r0, #0xC5
	lsl r0, r0, #3
	add r3, r5, r0
	mov r0, #0
	mov r1, #0
	mov r2, #0
	bl Ease_Init
	mov r1, #0xC8
	lsl r1, r1, #3
	add r0, r5, r1
	bl ClearKatakanaFlag
	ldr r2, _0806D498 @ =0x000018B0
	add r0, r5, r2
	bl ObjAffineInit
	mov r6, #1
	ldr r0, _0806D49C @ =0x000007FF
	add r3, r0, #0
	ldr r2, _0806D4A0 @ =0x08622AB4
	ldrh r1, [r2, #2]
	ldr r0, _0806D4A4 @ =0x0000FFFF
	cmp r1, r0
	beq _0806D3EC
	ldr r1, _0806D47C @ =0x00001494
	add r1, r1, r5
	mov sl, r1
	ldr r0, _0806D4A8 @ =0x00001496
	add r0, r0, r5
	mov r9, r0
	ldr r1, _0806D4AC @ =0x00001498
	add r1, r1, r5
	mov r8, r1
_0806D322:
	add r0, r6, #0
	and r0, r3
	lsl r0, r0, #1
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r2, _0806D4B0 @ =0xFFFFF880
	add r0, r0, r2
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0806D3C8
	ldr r0, _0806D4B4 @ =0x02011C20
	lsl r1, r6, #2
	add r4, r1, r0
	ldrh r3, [r4, #8]
	lsl r0, r3, #0x16
	add r7, r1, #0
	cmp r0, #0
	beq _0806D36A
	mov r1, #0xA5
	lsl r1, r1, #5
	add r0, r5, r1
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, sl
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #0
	bl DeckEdit_SetListCard
_0806D36A:
	ldrb r1, [r4, #9]
	lsl r0, r1, #0x1C
	lsr r0, r0, #0x1E
	cmp r0, #0
	bne _0806D37A
	lsr r0, r1, #6
	cmp r0, #0
	beq _0806D39A
_0806D37A:
	ldr r2, _0806D4B8 @ =0x000014A1
	add r0, r5, r2
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r9
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #1
	bl DeckEdit_SetListCard
_0806D39A:
	ldr r0, _0806D4B4 @ =0x02011C20
	add r0, r7, r0
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	lsr r0, r0, #0x1E
	cmp r0, #0
	beq _0806D3C8
	ldr r3, _0806D4BC @ =0x000014A2
	add r0, r5, r3
	ldrb r2, [r0]
	lsl r0, r2, #1
	add r0, r0, r2
	lsl r0, r0, #1
	add r0, r8
	ldrh r3, [r0]
	add r1, r3, #1
	strh r1, [r0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	add r0, r6, #0
	mov r1, #2
	bl DeckEdit_SetListCard
_0806D3C8:
	add r0, r6, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	mov r0, #0xCD
	lsl r0, r0, #2
	cmp r6, r0
	bhi _0806D3EC
	ldr r0, _0806D49C @ =0x000007FF
	add r3, r0, #0
	add r0, r6, #0
	and r0, r3
	lsl r0, r0, #1
	ldr r2, _0806D4A0 @ =0x08622AB4
	add r0, r0, r2
	ldr r1, _0806D4A4 @ =0x0000FFFF
	ldrh r0, [r0]
	cmp r0, r1
	bne _0806D322
_0806D3EC:
	bl DeckEdit_CountSideDeckMonsters
	ldr r7, _0806D45C @ =0x0201DB20
	ldr r1, _0806D484 @ =0x00001C1C
	add r6, r7, r1
	ldrb r1, [r6]
	lsl r2, r1, #1
	mov r3, #0xA5
	lsl r3, r3, #5
	add r4, r7, r3
	add r1, r1, r4
	ldrb r3, [r1]
	lsl r0, r3, #1
	add r0, r0, r3
	lsl r0, r0, #1
	add r0, r2, r0
	ldr r1, _0806D47C @ =0x00001494
	add r5, r7, r1
	add r0, r0, r5
	ldrh r0, [r0]
	mov r3, #0xC4
	lsl r3, r3, #3
	add r1, r7, r3
	add r2, r2, r1
	ldrh r1, [r2]
	ldr r3, _0806D4C0 @ =0x00001BB0
	add r2, r7, r3
	bl DeckEdit_CalcScrollBar
	ldr r0, _0806D4C4 @ =0x00001BB4
	add r2, r7, r0
	mov r0, #1
	ldrb r1, [r2]
	orr r1, r0
	strb r1, [r2]
	ldr r2, _0806D4C8 @ =0x00001BB6
	add r1, r7, r2
	ldrb r3, [r1]
	orr r0, r3
	strb r0, [r1]
	ldrb r1, [r6]
	add r4, r1, r4
	ldrb r2, [r4]
	lsl r0, r2, #1
	add r0, r0, r2
	add r0, r0, r1
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, #5
	bls _0806D4D0
	ldr r3, _0806D4CC @ =0x00001BB5
	add r0, r7, r3
	mov r1, #1
	b _0806D4D6
	.align 2, 0
_0806D45C: .4byte 0x0201DB20
_0806D460: .4byte 0x00001C5C
_0806D464: .4byte 0x03000040
_0806D468: .4byte 0x0000040E
_0806D46C: .4byte 0x04000012
_0806D470: .4byte 0x0000E0FF
_0806D474: .4byte 0x00000632
_0806D478: .4byte 0x0000063A
_0806D47C: .4byte 0x00001494
_0806D480: .4byte 0x00001C1D
_0806D484: .4byte 0x00001C1C
_0806D488: .4byte 0x00000634
_0806D48C: .4byte 0x00000635
_0806D490: .4byte 0x00001710
_0806D494: .4byte 0x000018AC
_0806D498: .4byte 0x000018B0
_0806D49C: .4byte 0x000007FF
_0806D4A0: .4byte gCardIdToNumber
_0806D4A4: .4byte 0x0000FFFF
_0806D4A8: .4byte 0x00001496
_0806D4AC: .4byte 0x00001498
_0806D4B0: .4byte 0xFFFFF880
_0806D4B4: .4byte 0x02011C20
_0806D4B8: .4byte 0x000014A1
_0806D4BC: .4byte 0x000014A2
_0806D4C0: .4byte 0x00001BB0
_0806D4C4: .4byte 0x00001BB4
_0806D4C8: .4byte 0x00001BB6
_0806D4CC: .4byte 0x00001BB5
_0806D4D0:
	ldr r3, _0806D510 @ =0x00001BB5
	add r0, r7, r3
	mov r1, #0
_0806D4D6:
	strb r1, [r0]
	ldr r2, _0806D514 @ =0x00001BB7
	add r0, r7, r2
	strb r1, [r0]
	ldr r4, _0806D518 @ =0x0201F6D8
	add r0, r4, #0
	bl DeckEdit_ResetFrameSlots
	add r0, r4, #0
	add r0, #0x68
	bl DeckEdit_ResetCardMove
	add r4, #0x7C
	mov r0, #2
	neg r0, r0
	ldrb r3, [r4]
	and r0, r3
	mov r1, #0x1F
	neg r1, r1
	and r0, r1
	strb r0, [r4]
	mov r0, #1
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0806D510: .4byte 0x00001BB5
_0806D514: .4byte 0x00001BB7
_0806D518: .4byte 0x0201F6D8
	thumb_func_end DeckEdit_Init

