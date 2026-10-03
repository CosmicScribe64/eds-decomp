	thumb_func_start EffectFinalDestinyPrepare
EffectFinalDestinyPrepare: @ 0x0802E980
	push {r4, r5, r6, lr}
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	ldr r4, _0802E9A0 @ =0x020192E4
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0802E9A4 @ =0x00000D64
	mul r0, r1
	add r0, r0, r4
	ldrb r0, [r0, #2]
	sub r2, r0, r2
	cmp r2, #4
	bgt _0802E9AC
	b _0802E9D6
	.align 2, 0
_0802E9A0: .4byte 0x020192E4
_0802E9A4: .4byte 0x00000D64
_0802E9A8:
	mov r0, #1
	b _0802E9D8
_0802E9AC:
	mov r3, #0
	mov r6, #1
	add r5, r1, #0
	add r4, #0x28
_0802E9B4:
	mov r2, #0
	add r0, r3, #0
	and r0, r6
	add r1, r0, #0
	mul r1, r5
_0802E9BE:
	add r0, r1, r4
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	bne _0802E9A8
	add r1, #0x94
	add r2, #1
	cmp r2, #0xA
	ble _0802E9BE
	add r3, #1
	cmp r3, #1
	ble _0802E9B4
_0802E9D6:
	mov r0, #0
_0802E9D8:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end EffectFinalDestinyPrepare
	.align 2, 0

