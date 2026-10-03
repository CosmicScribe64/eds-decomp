	thumb_func_start DuelCmd_EndBattlePhase
DuelCmd_EndBattlePhase: @ 0x0800D524
	ldr r1, _0800D570 @ =0x02015EE8
	mov r0, #1
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	beq _0800D540
	ldr r1, _0800D574 @ =0x020192E0
	ldr r0, _0800D578 @ =0x00001B12
	add r1, r1, r0
	mov r0, #2
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0800D55E
_0800D540:
	ldr r2, _0800D57C @ =0x020192E4
	mov r1, #0
	strh r1, [r2, #0x24]
	ldr r3, _0800D580 @ =0x00000D88
	add r0, r2, r3
	strh r1, [r0]
	ldr r0, _0800D584 @ =0x00001B10
	add r2, r2, r0
	ldr r0, [r2]
	ldr r1, _0800D588 @ =0xFFFE01FF
	and r0, r1
	mov r1, #0xC0
	lsl r1, r1, #5
	orr r0, r1
	str r0, [r2]
_0800D55E:
	ldr r1, _0800D58C @ =0x020185C0
	ldr r2, _0800D590 @ =0x0000080D
	add r1, r1, r2
	mov r0, #0x21
	neg r0, r0
	ldrb r3, [r1]
	and r0, r3
	strb r0, [r1]
	bx lr
_0800D570: .4byte 0x02015EE8
_0800D574: .4byte 0x020192E0
_0800D578: .4byte 0x00001B12
_0800D57C: .4byte 0x020192E4
_0800D580: .4byte 0x00000D88
_0800D584: .4byte 0x00001B10
_0800D588: .4byte 0xFFFE01FF
_0800D58C: .4byte 0x020185C0
_0800D590: .4byte 0x0000080D
	thumb_func_end DuelCmd_EndBattlePhase

