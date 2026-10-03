	thumb_func_start EffectTargetableFaceUpMonsterCheck
EffectTargetableFaceUpMonsterCheck: @ 0x0802BD98
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r5, r0, #0x18
	lsr r3, r1, #0x18
	cmp r3, #4
	bgt _0802BDE8
	mov r7, #1
	add r1, r5, #0
	and r1, r7
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802BDE0 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802BDE4 @ =0x0201930C
	add r4, r2, r0
	ldr r0, [r4]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BDE8
	ldrh r0, [r6]
	add r1, r5, #0
	add r2, r3, #0
	bl CanCardTargetZone
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802BDE8
	ldrb r4, [r4, #6]
	lsr r0, r4, #1
	and r0, r7
	b _0802BDEA
	.align 2, 0
_0802BDE0: .4byte 0x00000D64
_0802BDE4: .4byte 0x0201930C
_0802BDE8:
	mov r0, #0
_0802BDEA:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectTargetableFaceUpMonsterCheck

