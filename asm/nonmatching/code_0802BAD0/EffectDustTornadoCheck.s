	thumb_func_start EffectDustTornadoCheck
EffectDustTornadoCheck: @ 0x0802C230
	lsl r1, r1, #0x10
	lsl r2, r1, #8
	lsr r3, r2, #0x18
	lsr r1, r1, #0x18
	ldrb r0, [r0, #2]
	lsl r0, r0, #0x1F
	lsr r0, r0, #0x1F
	cmp r0, r3
	beq _0802C270
	sub r0, r1, #5
	cmp r0, #5
	bhi _0802C270
	mov r2, #1
	and r2, r3
	mov r0, #0x94
	mul r0, r1
	ldr r1, _0802C268 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802C26C @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802C270
	mov r0, #1
	b _0802C272
	.align 2, 0
_0802C268: .4byte 0x00000D64
_0802C26C: .4byte 0x0201930C
_0802C270:
	mov r0, #0
_0802C272:
	bx lr
	thumb_func_end EffectDustTornadoCheck

