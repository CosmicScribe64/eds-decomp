	thumb_func_start EffectRedirectAttackResolve
EffectRedirectAttackResolve: @ 0x0803B1EC
	push {r4, r5, r6, lr}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _0803B276
	mov r5, #7
	ldrb r0, [r4, #0xA]
	and r5, r0
	cmp r5, #1
	bne _0803B276
	ldrh r1, [r4, #0xC]
	lsr r6, r1, #8
	add r1, r5, #0
	ldrb r0, [r4, #0xC]
	and r1, r0
	mov r0, #0x94
	add r2, r6, #0
	mul r2, r0
	ldr r0, _0803B280 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0803B284 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0803B276
	cmp r1, #0
	beq _0803B276
	ldr r0, _0803B288 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r1, _0803B28C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0803B290 @ =0x0000057D
	ldrh r0, [r0]
	cmp r0, r1
	bne _0803B276
	ldrb r1, [r4, #2]
	add r0, r5, #0
	and r0, r1
	mov r3, #8
	cmp r0, #0
	beq _0803B252
	ldr r3, _0803B294 @ =0x00008008
_0803B252:
	lsl r1, r1, #0x1F
	lsr r1, r1, #0x1F
	lsl r2, r6, #8
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r4, #2]
	and r5, r0
	mov r0, #0x38
	cmp r5, #0
	beq _0803B26C
	ldr r0, _0803B298 @ =0x00008038
_0803B26C:
	ldrh r1, [r4, #0xC]
	mov r2, #1
	mov r3, #0
	bl DuelCmd_Push
_0803B276:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	.align 2, 0
_0803B280: .4byte 0x00000D64
_0803B284: .4byte 0x0201930C
_0803B288: .4byte 0x000007FF
_0803B28C: .4byte gCardIdToNumber
_0803B290: .4byte 0x0000057D
_0803B294: .4byte 0x00008008
_0803B298: .4byte 0x00008038
	thumb_func_end EffectRedirectAttackResolve

