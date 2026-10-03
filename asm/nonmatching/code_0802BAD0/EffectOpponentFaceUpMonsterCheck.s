	thumb_func_start EffectOpponentFaceUpMonsterCheck
EffectOpponentFaceUpMonsterCheck: @ 0x0802BEE8
	push {r4, lr}
	add r4, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r3, r0, #0x18
	lsr r2, r1, #0x18
	cmp r2, #4
	bgt _0802BF30
	ldrb r4, [r4, #2]
	lsl r0, r4, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r3
	beq _0802BF30
	mov r4, #1
	add r1, r3, #0
	and r1, r4
	mov r0, #0x94
	mul r2, r0
	ldr r0, _0802BF28 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802BF2C @ =0x0201930C
	add r1, r2, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BF30
	ldrb r1, [r1, #6]
	lsr r0, r1, #1
	and r0, r4
	b _0802BF32
	.align 2, 0
_0802BF28: .4byte 0x00000D64
_0802BF2C: .4byte 0x0201930C
_0802BF30:
	mov r0, #0
_0802BF32:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectOpponentFaceUpMonsterCheck

