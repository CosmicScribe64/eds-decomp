	thumb_func_start DuelPhase_Main
DuelPhase_Main: @ 0x0804F460
	push {lr}
	ldr r1, _0804F480 @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r1, r2
	ldrb r0, [r0]
	add r3, r1, #0
	cmp r0, #0x15
	bls _0804F474
	b _0804F626
_0804F474:
	lsl r0, r0, #2
	ldr r1, _0804F484 @ =0x0804F488
	add r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
_0804F480: .4byte 0x020192E0
_0804F484: .4byte 0x0804F488
_0804F488:
	.4byte _0804F4E0
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F518
	.4byte _0804F590
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F626
	.4byte _0804F5A8
	.4byte _0804F5F4
_0804F4E0:
	mov r0, #0x52
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldr r2, _0804F510 @ =0x020192E0
	ldr r0, _0804F514 @ =0x00001B2C
	add r3, r2, r0
	mov r0, #2
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	mov r1, #3
	neg r1, r1
	and r0, r1
	strb r0, [r3]
	mov r0, #0xD9
	lsl r0, r0, #5
	add r2, r2, r0
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	b _0804F646
_0804F510: .4byte 0x020192E0
_0804F514: .4byte 0x00001B2C
_0804F518:
	mov r0, #0
	bl CanEnterBattlePhase
	cmp r0, #0
	beq _0804F55C
	ldr r0, _0804F544 @ =0x00000205
	ldr r1, _0804F548 @ =0x00000615
	ldr r3, _0804F54C @ =0x08085C28
	mov r2, #0xB
	bl TextBoxOpen
	ldr r1, _0804F550 @ =0x0804F311
	ldr r2, _0804F554 @ =0x0804F385
	mov r0, #5
	bl TextBoxSetMenu
	ldr r0, _0804F558 @ =0x020192E0
	mov r1, #0xD9
	lsl r1, r1, #5
	add r0, r0, r1
	mov r1, #0x14
	b _0804F644
_0804F544: .4byte 0x00000205
_0804F548: .4byte 0x00000615
_0804F54C: .4byte gStrEndMainPhaseMenu
_0804F550: .4byte PhaseMenu_DrawCursor
_0804F554: .4byte PhaseMenu_HandleInput
_0804F558: .4byte 0x020192E0
_0804F55C:
	ldr r0, _0804F580 @ =0x00000206
	ldr r1, _0804F584 @ =0x00000412
	ldr r3, _0804F588 @ =0x08085C8C
	mov r2, #0xB
	bl TextBoxOpen
	mov r0, #1
	mov r1, #0
	mov r2, #0
	bl TextBoxSetMenu
	ldr r0, _0804F58C @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	ldrb r1, [r0]
	add r1, #1
	b _0804F644
_0804F580: .4byte 0x00000206
_0804F584: .4byte 0x00000412
_0804F588: .4byte gStrEndYourTurn
_0804F58C: .4byte 0x020192E0
_0804F590:
	ldr r0, _0804F5A4 @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	bne _0804F610
	mov r0, #0xD9
	lsl r0, r0, #5
	add r1, r3, r0
_0804F59E:
	mov r0, #1
	strb r0, [r1]
	b _0804F646
_0804F5A4: .4byte 0x0201AE60
_0804F5A8:
	ldr r0, _0804F5BC @ =0x0201AE60
	ldrh r0, [r0, #0x14]
	cmp r0, #0
	beq _0804F5C0
	cmp r0, #1
	beq _0804F610
	mov r2, #0xD9
	lsl r2, r2, #5
	add r1, r3, r2
	b _0804F59E
_0804F5BC: .4byte 0x0201AE60
_0804F5C0:
	ldr r0, _0804F5E8 @ =0x00001B26
	add r1, r3, r0
	mov r0, #2
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	ldr r0, _0804F5EC @ =0x00001B14
	add r2, r3, r0
	ldr r0, [r2]
	ldr r1, _0804F5F0 @ =0xFFFE01FF
	and r0, r1
	str r0, [r2]
	mov r2, #0xD9
	lsl r2, r2, #5
	add r1, r3, r2
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	b _0804F646
_0804F5E8: .4byte 0x00001B26
_0804F5EC: .4byte 0x00001B14
_0804F5F0: .4byte 0xFFFE01FF
_0804F5F4:
	mov r0, #0
	bl BattlePhase_Run
	cmp r0, #0
	beq _0804F646
	ldr r3, _0804F614 @ =0x020192E0
	ldr r0, _0804F618 @ =0x00001B26
	add r1, r3, r0
	mov r2, #1
	add r0, r2, #0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	beq _0804F61C
_0804F610:
	mov r0, #1
	b _0804F648
_0804F614: .4byte 0x020192E0
_0804F618: .4byte 0x00001B26
_0804F61C:
	mov r1, #0xD9
	lsl r1, r1, #5
	add r0, r3, r1
	strb r2, [r0]
	b _0804F646
_0804F626:
	bl DuelScreen_HandleInput
	cmp r0, #0
	bne _0804F646
	ldr r1, _0804F64C @ =0x03000040
	mov r0, #2
	ldrh r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0804F646
	ldr r0, _0804F650 @ =0x020192E0
	mov r2, #0xD9
	lsl r2, r2, #5
	add r0, r0, r2
	mov r1, #0xA
_0804F644:
	strb r1, [r0]
_0804F646:
	mov r0, #0
_0804F648:
	pop {r1}
	bx r1
_0804F64C: .4byte 0x03000040
_0804F650: .4byte 0x020192E0
	thumb_func_end DuelPhase_Main

