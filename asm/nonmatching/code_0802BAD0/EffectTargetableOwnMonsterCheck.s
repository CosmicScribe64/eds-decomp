	thumb_func_start EffectTargetableOwnMonsterCheck
EffectTargetableOwnMonsterCheck: @ 0x0802C4DC
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r5, r0, #0x18
	lsr r4, r1, #0x18
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	mul r0, r4
	ldr r1, _0802C528 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802C52C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r6, r0, #0x14
	ldrh r0, [r7]
	add r1, r5, #0
	add r2, r4, #0
	bl CanCardTargetZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802C530
	ldrb r7, [r7, #2]
	lsl r0, r7, #0x1F
	lsr r0, r0, #0x1F
	cmp r5, r0
	bne _0802C530
	cmp r4, #4
	bgt _0802C530
	cmp r6, #0
	beq _0802C530
	mov r0, #1
	b _0802C532
	.align 2, 0
_0802C528: .4byte 0x00000D64
_0802C52C: .4byte 0x0201930C
_0802C530:
	mov r0, #0
_0802C532:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectTargetableOwnMonsterCheck

