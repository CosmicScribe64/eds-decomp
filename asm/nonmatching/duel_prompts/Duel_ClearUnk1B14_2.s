	thumb_func_start Duel_ClearUnk1B14_2
Duel_ClearUnk1B14_2: @ 0x0802295C
	ldr r1, _08022970 @ =0x020192E0
	ldr r0, _08022974 @ =0x00001B14
	add r1, r1, r0
	ldr r0, _08022978 @ =0xFFFFFE03
	ldrh r2, [r1]
	and r0, r2
	strh r0, [r1]
	mov r0, #0
	bx lr
	.align 2, 0
_08022970: .4byte 0x020192E0
_08022974: .4byte 0x00001B14
_08022978: .4byte 0xFFFFFE03
	thumb_func_end Duel_ClearUnk1B14_2

