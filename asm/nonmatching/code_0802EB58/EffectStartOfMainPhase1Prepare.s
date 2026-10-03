	thumb_func_start EffectStartOfMainPhase1Prepare
EffectStartOfMainPhase1Prepare: @ 0x0802F560
	push {r4, lr}
	add r2, r0, #0
	ldr r3, _0802F5A4 @ =0x020192E0
	ldr r0, _0802F5A8 @ =0x00001B12
	add r1, r3, r0
	mov r0, #0x1C
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #8
	bne _0802F5B0
	add r4, r3, #4
	ldrb r2, [r2, #2]
	lsl r3, r2, #0x1F
	mov r2, #1
	lsr r0, r3, #0x1F
	ldr r1, _0802F5AC @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #9]
	lsl r0, r0, #0x1A
	cmp r0, #0
	blt _0802F5B0
	lsr r0, r3, #0x1F
	and r2, r0
	add r0, r2, #0
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #8]
	lsl r0, r0, #0x1B
	cmp r0, #0
	blt _0802F5B0
	mov r0, #1
	b _0802F5B2
	.align 2, 0
_0802F5A4: .4byte 0x020192E0
_0802F5A8: .4byte 0x00001B12
_0802F5AC: .4byte 0x00000D64
_0802F5B0:
	mov r0, #0
_0802F5B2:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectStartOfMainPhase1Prepare

