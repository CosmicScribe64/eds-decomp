	thumb_func_start EffectGraverobberPrepare
EffectGraverobberPrepare: @ 0x0802EA74
	push {r4, r5, r6, r7, lr}
	mov r3, #0
	ldr r5, _0802EAD0 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r2, r0, #0x1F
	lsr r0, r2, #0x1F
	mov r1, #1
	eor r0, r1
	ldr r1, _0802EAD4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r5
	ldrb r0, [r0, #4]
	cmp r3, r0
	bge _0802EAF6
	add r4, r2, #0
	mov r2, #1
	add r6, r1, #0
	ldr r0, _0802EAD8 @ =0x00000904
	add r0, r0, r5
	mov ip, r0
	add r7, r5, #0
	ldr r5, _0802EADC @ =0x000007FF
_0802EAA0:
	lsr r0, r4, #0x1F
	sub r0, r2, r0
	and r0, r2
	lsl r1, r3, #2
	mul r0, r6
	add r1, r1, r0
	add r1, ip
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _0802EAE0 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802EAE4
	mov r0, #1
	b _0802EAF8
	.align 2, 0
_0802EAD0: .4byte 0x020192E4
_0802EAD4: .4byte 0x00000D64
_0802EAD8: .4byte 0x00000904
_0802EADC: .4byte 0x000007FF
_0802EAE0: .4byte gCardStats
_0802EAE4:
	add r3, #1
	lsr r0, r4, #0x1F
	sub r0, r2, r0
	and r0, r2
	mul r0, r6
	add r0, r0, r7
	ldrb r0, [r0, #4]
	cmp r3, r0
	blt _0802EAA0
_0802EAF6:
	mov r0, #0
_0802EAF8:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectGraverobberPrepare
	.align 2, 0

