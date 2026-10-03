	thumb_func_start EffectBanishCostFromFieldResolve
EffectBanishCostFromFieldResolve: @ 0x0803CB04
	ldr r2, _0803CB20 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803CB24 @ =0x00000D64
	mul r1, r0
	add r1, r1, r2
	mov r0, #0x20
	ldrb r2, [r1, #0xC]
	orr r0, r2
	strb r0, [r1, #0xC]
	mov r0, #0
	bx lr
	.align 2, 0
_0803CB20: .4byte 0x020192E4
_0803CB24: .4byte 0x00000D64
	thumb_func_end EffectBanishCostFromFieldResolve

