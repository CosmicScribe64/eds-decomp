	thumb_func_start EffectNoblemanOfCrossoutCheck
EffectNoblemanOfCrossoutCheck: @ 0x0802C2AC
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r0, r0, #0x18
	lsr r2, r1, #0x18
	cmp r2, #4
	bgt _0802C2E8
	mov r3, #1
	add r1, r0, #0
	and r1, r3
	mov r0, #0x94
	mul r2, r0
	ldr r0, _0802C2E0 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802C2E4 @ =0x0201930C
	add r1, r2, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802C2E8
	ldrb r1, [r1, #6]
	lsr r1, r1, #1
	add r0, r3, #0
	bic r0, r1
	b _0802C2EA
	.align 2, 0
_0802C2E0: .4byte 0x00000D64
_0802C2E4: .4byte 0x0201930C
_0802C2E8:
	mov r0, #0
_0802C2EA:
	bx lr
	thumb_func_end EffectNoblemanOfCrossoutCheck

