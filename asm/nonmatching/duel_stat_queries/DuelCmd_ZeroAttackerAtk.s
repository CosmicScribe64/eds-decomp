	thumb_func_start DuelCmd_ZeroAttackerAtk
DuelCmd_ZeroAttackerAtk: @ 0x0800D6D4
	ldr r1, _0800D6F0 @ =0x02018450
	mov r0, #0x20
	ldrb r2, [r1]
	orr r0, r2
	strb r0, [r1]
	ldr r1, _0800D6F4 @ =0x020185C0
	ldr r0, _0800D6F8 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bx lr
_0800D6F0: .4byte 0x02018450
_0800D6F4: .4byte 0x020185C0
_0800D6F8: .4byte 0x0000080D
	thumb_func_end DuelCmd_ZeroAttackerAtk

