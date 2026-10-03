	thumb_func_start EffectNoblemanOfExterminationCheck
EffectNoblemanOfExterminationCheck: @ 0x0802C2EC
	push {r4, lr}
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r3, r0, #0x18
	lsr r2, r1, #0x18
	sub r0, r2, #5
	cmp r0, #4
	bhi _0802C32C
	mov r4, #1
	add r1, r3, #0
	and r1, r4
	mov r0, #0x94
	mul r2, r0
	ldr r0, _0802C324 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802C328 @ =0x0201930C
	add r1, r2, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802C32C
	ldrb r1, [r1, #6]
	lsr r1, r1, #1
	add r0, r4, #0
	bic r0, r1
	b _0802C32E
	.align 2, 0
_0802C324: .4byte 0x00000D64
_0802C328: .4byte 0x0201930C
_0802C32C:
	mov r0, #0
_0802C32E:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end EffectNoblemanOfExterminationCheck

