	thumb_func_start EffectPainfulChoicePrepare
EffectPainfulChoicePrepare: @ 0x0802EA50
	mov r3, #0
	ldr r2, _0802EA6C @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802EA70 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #3]
	cmp r0, #4
	bls _0802EA68
	mov r3, #1
_0802EA68:
	add r0, r3, #0
	bx lr
_0802EA6C: .4byte 0x020192E4
_0802EA70: .4byte 0x00000D64
	thumb_func_end EffectPainfulChoicePrepare

