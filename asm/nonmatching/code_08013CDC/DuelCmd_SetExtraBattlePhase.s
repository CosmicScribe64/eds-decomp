	thumb_func_start DuelCmd_SetExtraBattlePhase
DuelCmd_SetExtraBattlePhase: @ 0x080148C4
	push {r4, lr}
	ldr r1, _08014904 @ =0x020192E4
	ldr r3, _08014908 @ =0x020185C0
	mov r0, #0x80
	lsl r0, r0, #8
	ldrh r2, [r3]
	and r0, r2
	mov r2, #0
	cmp r0, #0
	beq _080148DA
	ldr r2, _0801490C @ =0x00000D64
_080148DA:
	add r2, r2, r1
	mov r1, #1
	ldrb r4, [r3, #2]
	and r1, r4
	lsl r1, r1, #6
	mov r0, #0x41
	neg r0, r0
	ldrb r4, [r2, #8]
	and r0, r4
	orr r0, r1
	strb r0, [r2, #8]
	ldr r0, _08014910 @ =0x0000080D
	add r1, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	pop {r4}
	pop {r0}
	bx r0
_08014904: .4byte 0x020192E4
_08014908: .4byte 0x020185C0
_0801490C: .4byte 0x00000D64
_08014910: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetExtraBattlePhase

