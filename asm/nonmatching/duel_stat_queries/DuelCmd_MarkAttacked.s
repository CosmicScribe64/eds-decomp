	thumb_func_start DuelCmd_MarkAttacked
DuelCmd_MarkAttacked: @ 0x0800D43C
	push {r4, lr}
	ldr r4, _0800D460 @ =0x020185C0
	ldrh r1, [r4]
	lsr r0, r1, #0xF
	ldrh r1, [r4, #2]
	bl MarkMonsterAttacked
	ldr r0, _0800D464 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
_0800D460: .4byte 0x020185C0
_0800D464: .4byte 0x0000080D
	thumb_func_end DuelCmd_MarkAttacked

