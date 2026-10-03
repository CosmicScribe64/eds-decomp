	thumb_func_start EffectCeasefireResolve
EffectCeasefireResolve: @ 0x08037A1C
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	mov r0, #4
	mov r1, sl
	ldrb r1, [r1, #4]
	and r0, r1
	cmp r0, #0
	bne _08037ADC
	mov r5, #0
	mov r2, #1
	mov r8, r2
	ldr r7, _08037AEC @ =0x00000D64
_08037A3C:
	mov r4, #0
	add r0, r5, #0
	mov r1, r8
	and r0, r1
	add r6, r0, #0
	mul r6, r7
_08037A48:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r6
	ldr r1, _08037AF0 @ =0x0201930C
	add r1, r0, r1
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08037A6E
	mov r0, #3
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #1
	bne _08037A6E
	add r0, r5, #0
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
_08037A6E:
	add r4, #1
	cmp r4, #4
	ble _08037A48
	add r5, #1
	cmp r5, #1
	ble _08037A3C
	mov r6, #0
	mov r5, #0
	mov r2, #1
	mov r9, r2
	ldr r0, _08037AEC @ =0x00000D64
	mov r8, r0
_08037A86:
	mov r4, #0
	add r7, r5, #1
	mov r1, r9
	and r5, r1
	mov r2, r8
	mul r2, r5
	add r5, r2, #0
_08037A94:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r5
	ldr r1, _08037AF0 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	cmp r0, #0
	beq _08037AB2
	bl IsEffectMonster
	cmp r0, #0
	beq _08037AB2
	add r6, #1
_08037AB2:
	add r4, #1
	cmp r4, #4
	ble _08037A94
	add r5, r7, #0
	cmp r5, #1
	ble _08037A86
	cmp r6, #0
	ble _08037ADC
	mov r0, sl
	ldrb r0, [r0, #2]
	lsl r1, r0, #0x1F
	lsr r1, r1, #0x1F
	mov r0, #1
	sub r0, r0, r1
	lsl r1, r6, #5
	sub r1, r1, r6
	lsl r1, r1, #2
	add r1, r1, r6
	lsl r1, r1, #2
	bl LoseLifePoints
_08037ADC:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08037AEC: .4byte 0x00000D64
_08037AF0: .4byte 0x0201930C
	thumb_func_end EffectCeasefireResolve

