	thumb_func_start SummonStep_SpecialChoosePosition
SummonStep_SpecialChoosePosition: @ 0x080553B0
	push {r4, r5, lr}
	sub sp, #4
	ldr r4, _080553CC @ =0x0201CF90
	ldrh r1, [r4, #0xE]
	lsl r0, r1, #0x14
	lsr r0, r0, #0x19
	cmp r0, #1
	beq _08055438
	cmp r0, #1
	bgt _080553D0
	cmp r0, #0
	beq _080553DC
	b _080555A4
	.align 2, 0
_080553CC: .4byte 0x0201CF90
_080553D0:
	cmp r0, #2
	beq _08055494
	cmp r0, #3
	bne _080553DA
	b _080554E0
_080553DA:
	b _080555A4
_080553DC:
	ldrb r2, [r4]
	lsl r0, r2, #0x1F
	cmp r0, #0
	beq _0805540C
	ldrb r0, [r4, #3]
	lsr r1, r0, #7
	ldr r0, _08055404 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r0, r2
	lsl r0, r0, #1
	orr r0, r1
	ldrb r2, [r4, #1]
	lsl r1, r2, #0x19
	lsr r1, r1, #0x1F
	bl AiShouldSetMonster
	ldr r1, _08055408 @ =0x0201AE60
	strh r0, [r1, #0x14]
	b _080554BC
	.align 2, 0
_08055404: .4byte 0x00007FFF
_08055408: .4byte 0x0201AE60
_0805540C:
	ldr r0, _08055424 @ =0x00000207
	ldr r1, _08055428 @ =0x0000030F
	ldr r3, _0805542C @ =0x08086370
	mov r2, #0xB
	bl TextBoxOpen
	ldr r1, _08055430 @ =0x08054771
	ldr r2, _08055434 @ =0x0805487D
	mov r0, #5
	bl TextBoxSetMenu
	b _080554BC
_08055424: .4byte 0x00000207
_08055428: .4byte 0x0000030F
_0805542C: .4byte gStrSelectDisplayPosition
_08055430: .4byte SummonPositionMenu_Draw
_08055434: .4byte SummonPositionMenu_HandleInput
_08055438:
	ldr r0, _0805548C @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	lsl r0, r0, #7
	mov r1, #0x7F
	ldrb r2, [r4, #1]
	and r1, r2
	orr r1, r0
	strb r1, [r4, #1]
	lsl r0, r1, #0x18
	cmp r0, #0
	blt _08055454
	mov r0, #0x40
	orr r1, r0
	strb r1, [r4, #1]
_08055454:
	add r1, r4, #0
	add r1, #8
	mov r0, sp
	bl CopyDuelCard
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	mov r5, #0x77
	cmp r0, #0
	beq _0805546A
	ldr r5, _08055490 @ =0x00008077
_0805546A:
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	ldrb r0, [r4, #1]
	lsl r2, r0, #0x19
	lsr r2, r2, #0x1F
	lsr r0, r0, #7
	lsl r0, r0, #1
	orr r0, r2
	lsl r0, r0, #8
	orr r1, r0
	ldr r3, [sp, #0]
	lsl r2, r3, #0x10
	lsr r2, r2, #0x10
	lsr r3, r3, #0x10
	add r0, r5, #0
	b _080554B8
	.align 2, 0
_0805548C: .4byte 0x0201AE60
_08055490: .4byte 0x00008077
_08055494:
	ldrb r2, [r4]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r2, #0x1A
	lsr r2, r2, #0x1B
	mov r1, #0
	bl DuelCursor_Select
	ldrb r1, [r4, #3]
	lsr r0, r1, #7
	ldr r1, _080554D8 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r1, r2
	lsl r1, r1, #1
	orr r1, r0
	mov r0, #0x71
	mov r2, #1
	mov r3, #0
_080554B8:
	bl DuelCmd_Push
_080554BC:
	ldrh r2, [r4, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _080554DC @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r4, #0xE]
	mov r0, #0
	b _080555A6
	.align 2, 0
_080554D8: .4byte 0x00007FFF
_080554DC: .4byte 0xFFFFF01F
_080554E0:
	ldrb r1, [r4]
	lsl r0, r1, #0x1F
	mov r3, #0x90
	cmp r0, #0
	beq _080554EC
	ldr r3, _08055524 @ =0x00008090
_080554EC:
	lsl r1, r1, #0x1A
	lsr r1, r1, #0x1B
	ldrh r2, [r4, #0xC]
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r4, #3]
	lsr r1, r0, #7
	ldr r0, _08055528 @ =0x00007FFF
	ldrh r2, [r4, #4]
	and r0, r2
	lsl r5, r0, #1
	orr r5, r1
	ldr r0, _0805552C @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _08055530 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _08055534 @ =0x000004DE
	cmp r1, r0
	beq _0805553C
	ldr r0, _08055538 @ =0x000005F6
	cmp r1, r0
	beq _0805555E
	b _0805557E
	.align 2, 0
_08055524: .4byte 0x00008090
_08055528: .4byte 0x00007FFF
_0805552C: .4byte 0x000007FF
_08055530: .4byte gCardIdToNumber
_08055534: .4byte 0x000004DE
_08055538: .4byte 0x000005F6
_0805553C:
	ldrb r3, [r4]
	lsl r1, r3, #0x1F
	lsr r1, r1, #0x1F
	lsl r0, r1, #0x1F
	lsl r3, r3, #0x1A
	lsr r3, r3, #0x1B
	lsl r2, r3, #0x10
	mov r4, #0xC4
	lsl r4, r4, #0x14
	orr r2, r4
	orr r0, r2
	orr r0, r5
	lsl r3, r3, #8
	orr r1, r3
	bl Chain_AddPending
	b _0805557E
_0805555E:
	mov r4, #0
_08055560:
	ldr r0, _0805559C @ =0x0201CF90
	ldrb r1, [r0]
	lsl r0, r1, #0x1A
	lsr r0, r0, #0x1B
	cmp r4, r0
	beq _08055578
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	mov r2, #1
	bl DestroyFieldCard
_08055578:
	add r4, #1
	cmp r4, #4
	ble _08055560
_0805557E:
	ldr r3, _0805559C @ =0x0201CF90
	ldrh r2, [r3, #0xE]
	lsl r1, r2, #0x14
	lsr r1, r1, #0x19
	add r1, #1
	mov r0, #0x7F
	and r1, r0
	lsl r1, r1, #5
	ldr r0, _080555A0 @ =0xFFFFF01F
	and r0, r2
	orr r0, r1
	strh r0, [r3, #0xE]
	mov r0, #0
	b _080555A6
	.align 2, 0
_0805559C: .4byte 0x0201CF90
_080555A0: .4byte 0xFFFFF01F
_080555A4:
	mov r0, #1
_080555A6:
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end SummonStep_SpecialChoosePosition
	.align 2, 0

