	thumb_func_start EffectDarknessApproachesCheck
EffectDarknessApproachesCheck: @ 0x0802BDF0
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r5, r0, #0x18
	lsr r3, r1, #0x18
	mov r7, #1
	add r1, r5, #0
	and r1, r7
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802BE54 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802BE58 @ =0x0201930C
	add r4, r2, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	add r1, r0, #0
	cmp r3, #4
	bgt _0802BE68
	cmp r0, #0
	beq _0802BE68
	ldr r0, _0802BE5C @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0802BE60 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	ldr r1, _0802BE64 @ =0xFFFFF880
	add r0, r0, r1
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0802BE68
	ldrh r0, [r6]
	add r1, r5, #0
	add r2, r3, #0
	bl CanCardTargetZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802BE68
	ldrb r4, [r4, #6]
	lsr r0, r4, #1
	and r0, r7
	b _0802BE6A
	.align 2, 0
_0802BE54: .4byte 0x00000D64
_0802BE58: .4byte 0x0201930C
_0802BE5C: .4byte 0x000007FF
_0802BE60: .4byte gCardIdToNumber
_0802BE64: .4byte 0xFFFFF880
_0802BE68:
	mov r0, #0
_0802BE6A:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectDarknessApproachesCheck

