	thumb_func_start EffectFaceUpMagicCheck
EffectFaceUpMagicCheck: @ 0x0802C538
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r3, r0, #0x18
	lsr r2, r1, #0x18
	sub r0, r2, #5
	cmp r0, #5
	bhi _0802C598
	mov r1, #1
	and r1, r3
	mov r0, #0x94
	mul r2, r0
	ldr r0, _0802C588 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802C58C @ =0x0201930C
	add r1, r2, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	cmp r3, #0
	beq _0802C598
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	beq _0802C598
	ldr r0, _0802C590 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #2
	ldr r1, _0802C594 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0802C598
	mov r0, #1
	b _0802C59A
_0802C588: .4byte 0x00000D64
_0802C58C: .4byte 0x0201930C
_0802C590: .4byte 0x000007FF
_0802C594: .4byte gCardStats
_0802C598:
	mov r0, #0
_0802C59A:
	bx lr
	thumb_func_end EffectFaceUpMagicCheck

