	thumb_func_start EffectGiftOfTheMysticalElfResolve
EffectGiftOfTheMysticalElfResolve: @ 0x08036570
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r0, #0
	mov r4, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _080365DA
	mov r0, #0
	mov r7, #1
	mov ip, r7
	ldr r1, _080365E8 @ =0x00000D64
	mov r8, r1
	ldr r6, _080365EC @ =0x0201930C
_08036590:
	mov r2, #0
	add r3, r0, #1
	mov r7, ip
	and r0, r7
	mov r1, r8
	mul r1, r0
	add r0, r1, #0
	add r1, r0, r6
_080365A0:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080365B4
	mov r0, #2
	ldrb r7, [r1, #6]
	and r0, r7
	cmp r0, #0
	beq _080365B4
	add r4, #1
_080365B4:
	add r1, #0x94
	add r2, #1
	cmp r2, #4
	ble _080365A0
	add r0, r3, #0
	cmp r0, #1
	ble _08036590
	cmp r4, #0
	ble _080365DA
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	lsl r2, r4, #2
	add r2, r2, r4
	lsl r1, r2, #4
	sub r1, r1, r2
	lsl r1, r1, #2
	bl GainLifePoints
_080365DA:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080365E8: .4byte 0x00000D64
_080365EC: .4byte 0x0201930C
	thumb_func_end EffectGiftOfTheMysticalElfResolve

