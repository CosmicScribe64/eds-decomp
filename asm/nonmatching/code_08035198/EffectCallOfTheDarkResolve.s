	thumb_func_start EffectCallOfTheDarkResolve
EffectCallOfTheDarkResolve: @ 0x08035314
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _0803538E
	mov r1, #0
	mov r2, #1
	mov r9, r2
_0803532E:
	cmp r1, #0
	beq _0803533A
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r5, r0, #0x1F
	b _08035344
_0803533A:
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	mov r2, r9
	sub r5, r2, r0
_08035344:
	mov r4, #0
	add r1, #1
	mov r8, r1
	add r0, r5, #0
	mov r1, r9
	and r0, r1
	ldr r1, _0803539C @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
_08035356:
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	add r1, r1, r7
	ldr r0, _080353A0 @ =0x0201930C
	add r1, r1, r0
	mov r0, #0x40
	ldrb r1, [r1, #7]
	and r0, r1
	cmp r0, #0
	beq _08035382
	add r0, r5, #0
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	ldrb r2, [r6, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_08035382:
	add r4, #1
	cmp r4, #4
	ble _08035356
	mov r1, r8
	cmp r1, #1
	ble _0803532E
_0803538E:
	mov r0, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0803539C: .4byte 0x00000D64
_080353A0: .4byte 0x0201930C
	thumb_func_end EffectCallOfTheDarkResolve

