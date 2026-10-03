	thumb_func_start EffectHasTwoHandCardsPrepare
EffectHasTwoHandCardsPrepare: @ 0x0802FC9C
	ldr r2, _0802FCB4 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802FCB8 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #1
	bls _0802FCBC
	mov r0, #1
	b _0802FCBE
_0802FCB4: .4byte 0x020192E4
_0802FCB8: .4byte 0x00000D64
_0802FCBC:
	mov r0, #0
_0802FCBE:
	bx lr
	thumb_func_end EffectHasTwoHandCardsPrepare

