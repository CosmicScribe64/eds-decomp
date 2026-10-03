	thumb_func_start DebugMenu_HandleInput
DebugMenu_HandleInput: @ 0x08074868
	push {r4, r5, r6, lr}
	ldr r1, _08074918 @ =0x03000040
	mov r0, #0x80
	ldrh r2, [r1, #6]
	and r0, r2
	add r4, r1, #0
	cmp r0, #0
	beq _08074898
	ldr r3, _0807491C @ =0x00004859
	add r2, r4, r3
	ldrb r0, [r2]
	add r0, #1
	strb r0, [r2]
	ldr r1, _08074920 @ =0x081A73A0
	lsl r0, r0, #4
	ldrb r3, [r2]
	add r0, r0, r3
	lsl r0, r0, #2
	add r1, #0x40
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	bne _08074898
	strb r0, [r2]
_08074898:
	mov r0, #0x40
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _080748E6
	ldr r3, _0807491C @ =0x00004859
	add r2, r4, r3
	ldrb r0, [r2]
	cmp r0, #0
	bne _080748DC
	ldr r0, _08074920 @ =0x081A73A0
	ldrb r3, [r2]
	lsl r1, r3, #4
	add r1, r1, r3
	lsl r1, r1, #2
	add r3, r0, #0
	add r3, #0x40
	add r1, r1, r3
	ldr r0, [r1]
	cmp r0, #0
	beq _080748DC
	add r1, r2, #0
	add r2, r3, #0
_080748C6:
	ldrb r0, [r1]
	add r0, #1
	strb r0, [r1]
	lsl r0, r0, #4
	ldrb r3, [r1]
	add r0, r0, r3
	lsl r0, r0, #2
	add r0, r0, r2
	ldr r0, [r0]
	cmp r0, #0
	bne _080748C6
_080748DC:
	ldr r0, _0807491C @ =0x00004859
	add r1, r4, r0
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
_080748E6:
	mov r0, #1
	add r5, r4, #0
	ldr r1, _0807491C @ =0x00004859
	add r6, r5, r1
	ldrb r3, [r6]
	lsl r2, r3, #1
	add r1, r2, #4
	cmp r1, #0x13
	ble _080748FC
	mov r0, #0xF
	sub r1, #0x10
_080748FC:
	lsl r0, r0, #3
	lsl r1, r1, #0x13
	orr r0, r1
	mov r1, #0
	mov r2, #2
	bl AddSprite
	ldrh r1, [r5, #6]
	mov r0, #1
	and r0, r1
	cmp r0, #0
	beq _08074924
	mov r0, #1
	b _080749E2
_08074918: .4byte 0x03000040
_0807491C: .4byte 0x00004859
_08074920: .4byte gDebugMenuItems
_08074924:
	mov r0, #2
	and r0, r1
	cmp r0, #0
	beq _08074934
	mov r0, #9
	strb r0, [r6]
	mov r0, #1
	b _080749E2
_08074934:
	mov r0, #0x10
	and r0, r1
	cmp r0, #0
	beq _0807495A
	ldr r0, _080749C4 @ =0x02011C20
	ldr r1, _080749C8 @ =0x00002150
	add r0, r0, r1
	ldrh r1, [r0]
	add r1, #1
	strh r1, [r0]
	ldr r2, _080749CC @ =0x00004857
	add r1, r5, r2
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	bl ClearBgMapBuffer0
	bl DebugMenu_DrawDate
_0807495A:
	mov r0, #0x20
	ldrh r3, [r4, #6]
	and r0, r3
	cmp r0, #0
	beq _08074986
	ldr r0, _080749C4 @ =0x02011C20
	ldr r2, _080749C8 @ =0x00002150
	add r1, r0, r2
	ldrh r0, [r1]
	cmp r0, #0
	beq _08074986
	sub r0, #1
	strh r0, [r1]
	ldr r3, _080749CC @ =0x00004857
	add r1, r4, r3
	ldrb r0, [r1]
	sub r0, #1
	strb r0, [r1]
	bl ClearBgMapBuffer0
	bl DebugMenu_DrawDate
_08074986:
	ldr r4, _080749D0 @ =0x03000040
	mov r0, #0x80
	lsl r0, r0, #1
	ldrh r1, [r4, #6]
	and r0, r1
	cmp r0, #0
	beq _080749A8
	ldr r0, _080749C4 @ =0x02011C20
	ldr r2, _080749C8 @ =0x00002150
	add r0, r0, r2
	ldrh r1, [r0]
	add r1, #0x1E
	strh r1, [r0]
	bl ClearBgMapBuffer0
	bl DebugMenu_DrawDate
_080749A8:
	mov r0, #0x80
	lsl r0, r0, #2
	ldrh r4, [r4, #6]
	and r0, r4
	cmp r0, #0
	beq _080749E0
	ldr r0, _080749C4 @ =0x02011C20
	ldr r3, _080749C8 @ =0x00002150
	add r1, r0, r3
	ldrh r0, [r1]
	cmp r0, #0x1E
	bls _080749D4
	sub r0, #0x1E
	b _080749D6
_080749C4: .4byte 0x02011C20
_080749C8: .4byte 0x00002150
_080749CC: .4byte 0x00004857
_080749D0: .4byte 0x03000040
_080749D4:
	mov r0, #0
_080749D6:
	strh r0, [r1]
	bl ClearBgMapBuffer0
	bl DebugMenu_DrawDate
_080749E0:
	mov r0, #0
_080749E2:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end DebugMenu_HandleInput

