	thumb_func_start DuelCmd_SetStatChangesReversed
DuelCmd_SetStatChangesReversed: @ 0x08014A24
	push {r4, r5, lr}
	ldr r3, _08014A54 @ =0x020192E0
	ldr r4, _08014A58 @ =0x020185C0
	ldr r0, _08014A5C @ =0x00001ACD
	add r3, r3, r0
	mov r1, #1
	ldrb r5, [r4, #2]
	and r1, r5
	lsl r1, r1, #5
	mov r2, #0x21
	neg r2, r2
	add r0, r2, #0
	ldrb r5, [r3]
	and r0, r5
	orr r0, r1
	strb r0, [r3]
	ldr r0, _08014A60 @ =0x0000080D
	add r4, r4, r0
	ldrb r5, [r4]
	and r2, r5
	strb r2, [r4]
	pop {r4, r5}
	pop {r0}
	bx r0
_08014A54: .4byte 0x020192E0
_08014A58: .4byte 0x020185C0
_08014A5C: .4byte 0x00001ACD
_08014A60: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetStatChangesReversed

