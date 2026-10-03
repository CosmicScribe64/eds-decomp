	thumb_func_start EffectBlastJugglerResolve
EffectBlastJugglerResolve: @ 0x080314BC
	push {r4, r5, r6, r7, lr}
	add r7, r0, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	bne _08031540
	ldrb r2, [r7, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r2, [r7, #2]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x1A
	bl TributeMonster
	ldrb r1, [r7, #0xA]
	lsl r0, r1, #0x1D
	mov r6, #0
	cmp r0, #0
	beq _08031540
_080314E4:
	lsl r1, r6, #1
	add r0, r7, #0
	add r0, #0xC
	add r3, r0, r1
	ldrb r5, [r3]
	ldrh r2, [r3]
	lsr r4, r2, #8
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _08031548 @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0803154C @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08031530
	ldrh r1, [r3]
	add r0, r7, #0
	bl EffectBlastJugglerCheck
	cmp r0, #0
	beq _08031530
	add r0, r5, #0
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	ldrb r1, [r7, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_08031530:
	add r6, #1
	ldrb r2, [r7, #0xA]
	lsl r0, r2, #0x1D
	lsr r0, r0, #0x1D
	cmp r6, r0
	bge _08031540
	cmp r6, #1
	ble _080314E4
_08031540:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08031548: .4byte 0x00000D64
_0803154C: .4byte 0x0201930C
	thumb_func_end EffectBlastJugglerResolve

