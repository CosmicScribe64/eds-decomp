	thumb_func_start DuelPhase_Draw
DuelPhase_Draw: @ 0x0804E420
	push {r4, r5, r6, lr}
	ldr r4, _0804E458 @ =0x020192E4
	ldr r1, _0804E45C @ =0x00001B0E
	add r0, r4, r1
	ldrb r3, [r0]
	lsl r2, r3, #0x1E
	mov r1, #1
	lsr r0, r2, #0x1F
	ldr r6, _0804E460 @ =0x00000D64
	mul r0, r6
	add r0, r0, r4
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1D
	lsr r5, r0, #0x1F
	cmp r5, #0
	beq _0804E464
	lsr r0, r2, #0x1F
	and r1, r0
	mul r1, r6
	add r1, r1, r4
	mov r0, #5
	neg r0, r0
	ldrb r2, [r1, #9]
	and r0, r2
	strb r0, [r1, #9]
	mov r0, #1
	b _0804E52A
	.align 2, 0
_0804E458: .4byte 0x020192E4
_0804E45C: .4byte 0x00001B0E
_0804E460: .4byte 0x00000D64
_0804E464:
	ldr r0, _0804E478 @ =0x00001B1C
	add r4, r4, r0
	ldrb r0, [r4]
	cmp r0, #1
	beq _0804E4AC
	cmp r0, #1
	bgt _0804E47C
	cmp r0, #0
	beq _0804E486
	b _0804E504
_0804E478: .4byte 0x00001B1C
_0804E47C:
	cmp r0, #2
	beq _0804E4CA
	cmp r0, #4
	beq _0804E4E8
	b _0804E504
_0804E486:
	mov r0, #2
	and r0, r3
	mov r1, #0x50
	cmp r0, #0
	beq _0804E492
	ldr r1, _0804E4A8 @ =0x00008050
_0804E492:
	add r0, r1, #0
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0804E528
	.align 2, 0
_0804E4A8: .4byte 0x00008050
_0804E4AC:
	mov r0, #2
	and r0, r3
	cmp r0, #0
	beq _0804E4B8
	mov r0, #1
	b _0804E4F2
_0804E4B8:
	lsr r0, r2, #0x1F
	mov r1, #0xD
	mov r2, #0
	bl DuelCursor_Select
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0804E528
_0804E4CA:
	ldr r0, _0804E4E0 @ =0x0201CFB0
	ldr r1, _0804E4E4 @ =0x00000808
	add r0, r0, r1
	mov r1, #8
	ldrb r2, [r0]
	orr r1, r2
	strb r1, [r0]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
	b _0804E528
_0804E4E0: .4byte 0x0201CFB0
_0804E4E4: .4byte 0x00000808
_0804E4E8:
	ldr r0, _0804E4FC @ =0x0201CFB0
	ldr r1, _0804E500 @ =0x0000085C
	add r0, r0, r1
	str r5, [r0]
	mov r0, #0
_0804E4F2:
	mov r1, #1
	bl DrawCards
	mov r0, #1
	b _0804E52A
_0804E4FC: .4byte 0x0201CFB0
_0804E500: .4byte 0x0000085C
_0804E504:
	bl DuelScreen_HandleInput
	cmp r0, #0
	bne _0804E528
	ldr r1, _0804E530 @ =0x03000040
	mov r0, #0x80
	lsl r0, r0, #1
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804E528
	ldr r0, _0804E534 @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	strb r1, [r0]
_0804E528:
	mov r0, #0
_0804E52A:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0804E530: .4byte 0x03000040
_0804E534: .4byte 0x020192E0
	thumb_func_end DuelPhase_Draw

