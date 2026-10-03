	thumb_func_start EffectDestroyFieldMagicsResolve
EffectDestroyFieldMagicsResolve: @ 0x0803A2E4
	push {r4, lr}
	mov r1, #4
	ldrb r0, [r0, #4]
	and r1, r0
	cmp r1, #0
	bne _0803A316
	mov r4, #0
_0803A2F2:
	mov r0, #1
	and r0, r4
	ldr r1, _0803A320 @ =0x00000D64
	mul r0, r1
	ldr r1, _0803A324 @ =0x020198D4
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0803A310
	add r0, r4, #0
	mov r1, #0xA
	mov r2, #1
	bl DestroyFieldCard
_0803A310:
	add r4, #1
	cmp r4, #1
	ble _0803A2F2
_0803A316:
	mov r0, #0
	pop {r4}
	pop {r1}
	bx r1
	.align 2, 0
_0803A320: .4byte 0x00000D64
_0803A324: .4byte 0x020198D4
	thumb_func_end EffectDestroyFieldMagicsResolve

