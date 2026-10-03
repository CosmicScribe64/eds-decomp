	thumb_func_start EffectOpponentHasHandPrepare
EffectOpponentHasHandPrepare: @ 0x0802DCA0
	ldr r2, _0802DCBC @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	eor r0, r1
	ldr r1, _0802DCC0 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	ldrb r2, [r1, #2]
	neg r0, r2
	orr r0, r2
	lsr r0, r0, #0x1F
	bx lr
_0802DCBC: .4byte 0x020192E4
_0802DCC0: .4byte 0x00000D64
	thumb_func_end EffectOpponentHasHandPrepare

