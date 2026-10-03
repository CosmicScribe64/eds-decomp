	thumb_func_start EffectGiantTrunadeResolve
EffectGiantTrunadeResolve: @ 0x080361D0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #4
	add r3, r0, #0
	mov r0, #4
	ldrb r1, [r3, #4]
	and r0, r1
	cmp r0, #0
	bne _0803623C
	mov r1, #0
	mov r2, #1
	mov r8, r2
_080361EA:
	cmp r1, #0
	beq _080361F6
	ldrb r2, [r3, #2]
	lsl r0, r2, #0x1F
	lsr r5, r0, #0x1F
	b _08036200
_080361F6:
	ldrb r2, [r3, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r2, r8
	sub r5, r2, r0
_08036200:
	mov r4, #5
	add r7, r1, #1
	add r0, r5, #0
	mov r1, r8
	and r0, r1
	ldr r1, _0803624C @ =0x00000D64
	add r6, r0, #0
	mul r6, r1
_08036210:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08036250 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08036230
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #9
	str r3, [sp, #0]
	bl ReturnFieldCardToHand
	ldr r3, [sp, #0]
_08036230:
	add r4, #1
	cmp r4, #0xA
	ble _08036210
	add r1, r7, #0
	cmp r1, #1
	ble _080361EA
_0803623C:
	mov r0, #0
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_0803624C: .4byte 0x00000D64
_08036250: .4byte 0x0201930C
	thumb_func_end EffectGiantTrunadeResolve

