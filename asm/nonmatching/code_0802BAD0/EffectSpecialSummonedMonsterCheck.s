	thumb_func_start EffectSpecialSummonedMonsterCheck
EffectSpecialSummonedMonsterCheck: @ 0x0802C5E4
	lsl r1, r1, #0x10
	lsl r3, r1, #8
	lsr r3, r3, #0x18
	lsr r1, r1, #0x18
	mov r0, #1
	and r3, r0
	mov r0, #0x94
	add r2, r1, #0
	mul r2, r0
	ldr r0, _0802C624 @ =0x00000D64
	mul r0, r3
	add r2, r2, r0
	ldr r0, _0802C628 @ =0x0201930C
	add r2, r2, r0
	ldr r3, [r2]
	lsl r0, r3, #0x14
	lsr r0, r0, #0x14
	cmp r1, #4
	bgt _0802C62C
	cmp r0, #0
	beq _0802C62C
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _0802C62C
	mov r0, #0x80
	lsl r0, r0, #9
	and r3, r0
	lsr r0, r3, #0x10
	b _0802C62E
	.align 2, 0
_0802C624: .4byte 0x00000D64
_0802C628: .4byte 0x0201930C
_0802C62C:
	mov r0, #0
_0802C62E:
	bx lr
	thumb_func_end EffectSpecialSummonedMonsterCheck

