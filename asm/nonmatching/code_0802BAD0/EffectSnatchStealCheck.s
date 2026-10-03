	thumb_func_start EffectSnatchStealCheck
EffectSnatchStealCheck: @ 0x0802C004
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r4, r0, #0x18
	lsr r3, r1, #0x18
	cmp r3, #4
	bgt _0802C078
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r4
	beq _0802C078
	mov r7, #1
	add r1, r4, #0
	and r1, r7
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802C068 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802C06C @ =0x0201930C
	add r5, r2, r0
	ldr r0, [r5]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802C078
	ldrh r0, [r6]
	add r1, r4, #0
	add r2, r3, #0
	bl CanCardTargetZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802C078
	ldr r0, [r5]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0802C070 @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0802C074 @ =0x00000547
	ldrh r0, [r0]
	cmp r0, r1
	beq _0802C078
	ldrb r5, [r5, #6]
	lsr r0, r5, #1
	and r0, r7
	b _0802C07A
	.align 2, 0
_0802C068: .4byte 0x00000D64
_0802C06C: .4byte 0x0201930C
_0802C070: .4byte gCardIdToNumber
_0802C074: .4byte 0x00000547
_0802C078:
	mov r0, #0
_0802C07A:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectSnatchStealCheck

