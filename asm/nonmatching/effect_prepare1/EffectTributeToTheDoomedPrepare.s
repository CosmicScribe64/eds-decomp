	thumb_func_start EffectTributeToTheDoomedPrepare
EffectTributeToTheDoomedPrepare: @ 0x0802E3C8
	ldr r2, _0802E3E0 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802E3E4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r2
	ldrb r0, [r0, #2]
	cmp r0, #0
	beq _0802E3E8
	mov r0, #1
	b _0802E3EA
_0802E3E0: .4byte 0x020192E4
_0802E3E4: .4byte 0x00000D64
_0802E3E8:
	mov r0, #0
_0802E3EA:
	bx lr
	thumb_func_end EffectTributeToTheDoomedPrepare

