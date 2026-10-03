	thumb_func_start TurnOrder_Init
TurnOrder_Init: @ 0x080289E8
	push {r4, r5, r6, lr}
	ldr r4, _08028A84 @ =0x02020310
	ldr r1, _08028A88 @ =0x00000B24
	add r0, r4, #0
	bl MemClear16
	ldr r1, _08028A8C @ =0x03000040
	ldr r0, _08028A90 @ =0x0000040E
	add r1, r1, r0
	mov r6, #0
	mov r5, #0
	mov r0, #1
	strh r0, [r1]
	ldr r0, _08028A94 @ =0x04000016
	strh r5, [r0]
	sub r0, #2
	strh r5, [r0]
	add r0, #6
	strh r5, [r0]
	sub r0, #2
	strh r5, [r0]
	add r0, #6
	strh r5, [r0]
	sub r0, #2
	strh r5, [r0]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08028A98 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	add r0, r4, #0
	bl OamListClear
	mov r0, #8
	bl SetBldAlpha
	ldr r1, _08028A9C @ =0x00000AF4
	add r0, r4, r1
	strb r6, [r0]
	ldr r2, _08028AA0 @ =0x00000AF5
	add r0, r4, r2
	strb r6, [r0]
	ldr r0, _08028AA4 @ =0x00000ABF
	add r1, r4, r0
	mov r0, #0xFF
	strb r0, [r1]
	ldr r1, _08028AA8 @ =0x00000B0D
	add r0, r4, r1
	strb r6, [r0]
	add r2, #0x27
	add r0, r4, r2
	strb r6, [r0]
	add r1, #0x10
	add r0, r4, r1
	strb r6, [r0]
	ldr r0, _08028AAC @ =0x0819A780
	ldr r2, _08028AB0 @ =0x00000918
	add r1, r4, r2
	bl AnimBlockInit
	ldr r1, _08028AB4 @ =0x00000926
	add r0, r4, r1
	strb r6, [r0]
	mov r2, #0xC3
	lsl r2, r2, #3
	add r0, r4, r2
	bl ObjAffineInit
	mov r0, #0xB2
	lsl r0, r0, #4
	add r4, r4, r0
	strh r5, [r4]
	mov r0, #1
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_08028A84: .4byte 0x02020310
_08028A88: .4byte 0x00000B24
_08028A8C: .4byte 0x03000040
_08028A90: .4byte 0x0000040E
_08028A94: .4byte 0x04000016
_08028A98: .4byte 0x0000E0FF
_08028A9C: .4byte 0x00000AF4
_08028AA0: .4byte 0x00000AF5
_08028AA4: .4byte 0x00000ABF
_08028AA8: .4byte 0x00000B0D
_08028AAC: .4byte gTurnOrderWaitAnimList
_08028AB0: .4byte 0x00000918
_08028AB4: .4byte 0x00000926
	thumb_func_end TurnOrder_Init

