	thumb_func_start EffectBlastJugglerPrepare
EffectBlastJugglerPrepare: @ 0x0802D67C
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	add r3, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802D6C8
	mov r0, #0xFC
	ldrb r1, [r3, #3]
	and r0, r1
	cmp r0, #8
	beq _0802D698
	b _0802D6C8
_0802D694:
	mov r0, #1
	b _0802D6CA
_0802D698:
	mov r5, #0
	mov r7, #0
_0802D69C:
	mov r4, #0
	lsl r0, r7, #0x18
	lsr r6, r0, #0x18
_0802D6A2:
	lsl r1, r4, #0x18
	lsr r1, r1, #0x10
	orr r1, r6
	add r0, r3, #0
	str r3, [sp, #0]
	bl EffectBlastJugglerCheck
	ldr r3, [sp, #0]
	cmp r0, #0
	beq _0802D6BC
	add r5, #1
	cmp r5, #1
	bgt _0802D694
_0802D6BC:
	add r4, #1
	cmp r4, #4
	ble _0802D6A2
	add r7, #1
	cmp r7, #1
	ble _0802D69C
_0802D6C8:
	mov r0, #0
_0802D6CA:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end EffectBlastJugglerPrepare
	.align 2, 0

