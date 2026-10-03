	thumb_func_start DuelCmd_SetCrushCardTurns
DuelCmd_SetCrushCardTurns: @ 0x0800FB80
	push {r4, lr}
	ldr r3, _0800FBB8 @ =0x020185C0
	ldr r4, _0800FBBC @ =0x020192E4
	ldrh r0, [r3]
	lsr r1, r0, #0xF
	ldr r0, _0800FBC0 @ =0x00000D64
	add r2, r1, #0
	mul r2, r0
	add r2, r2, r4
	mov r1, #7
	ldrb r4, [r3, #2]
	and r1, r4
	mov r0, #8
	neg r0, r0
	ldrb r4, [r2, #0xB]
	and r0, r4
	orr r0, r1
	strb r0, [r2, #0xB]
	ldr r0, _0800FBC4 @ =0x0000080D
	add r3, r3, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r3]
	and r0, r1
	strb r0, [r3]
	pop {r4}
	pop {r0}
	bx r0
_0800FBB8: .4byte 0x020185C0
_0800FBBC: .4byte 0x020192E4
_0800FBC0: .4byte 0x00000D64
_0800FBC4: .4byte 0x0000080D
	thumb_func_end DuelCmd_SetCrushCardTurns

