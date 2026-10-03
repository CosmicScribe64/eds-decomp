	thumb_func_start EffectSealOfTheAncientsResolve
EffectSealOfTheAncientsResolve: @ 0x080383F0
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	add r7, r0, #0
	mov r0, #4
	ldrb r1, [r7, #4]
	and r0, r1
	cmp r0, #0
	bne _080384D2
	mov r2, #0
	mov r8, r2
	mov r4, #1
	ldr r0, _080384E4 @ =0x00000D64
	mov sl, r0
	ldr r1, _080384E8 @ =0x0201930C
	mov r9, r1
_08038414:
	ldrb r3, [r7, #2]
	lsl r2, r3, #0x1F
	lsr r1, r2, #0x1F
	sub r1, r4, r1
	and r1, r4
	mov r0, #0x94
	mov r6, r8
	mul r6, r0
	mov r0, sl
	mul r0, r1
	add r0, r6, r0
	add r0, r9
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080384C8
	lsr r0, r2, #0x1F
	sub r0, r4, r0
	and r0, r4
	mov r1, sl
	mul r1, r0
	add r1, r6, r1
	add r1, r9
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _080384C8
	add r0, r4, #0
	and r0, r3
	mov r3, #8
	cmp r0, #0
	beq _08038458
	ldr r3, _080384EC @ =0x00008008
_08038458:
	lsr r1, r2, #0x1F
	sub r1, r4, r1
	lsl r1, r1, #0x10
	lsr r1, r1, #0x10
	mov r0, r8
	lsl r2, r0, #0x18
	lsr r2, r2, #0x10
	add r0, r3, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r4, #0
	ldrb r1, [r7, #2]
	and r0, r1
	mov r1, #0x7F
	cmp r0, #0
	bne _0803847C
	ldr r1, _080384F0 @ =0x0000807F
_0803847C:
	mov r2, r8
	lsl r0, r2, #0x10
	lsr r5, r0, #0x10
	add r0, r1, #0
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	ldrb r0, [r7, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	add r1, r0, #0
	sub r1, r4, r1
	and r1, r4
	mov r2, sl
	mul r2, r1
	add r1, r2, #0
	add r1, r6, r1
	add r1, r9
	ldr r1, [r1]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	bl ShowCardDetail
	add r0, r4, #0
	ldrb r1, [r7, #2]
	and r0, r1
	mov r1, #0x7F
	cmp r0, #0
	bne _080384BC
	ldr r1, _080384F0 @ =0x0000807F
_080384BC:
	add r0, r1, #0
	add r1, r5, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_080384C8:
	mov r2, #1
	add r8, r2
	mov r0, r8
	cmp r0, #0xA
	ble _08038414
_080384D2:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_080384E4: .4byte 0x00000D64
_080384E8: .4byte 0x0201930C
_080384EC: .4byte 0x00008008
_080384F0: .4byte 0x0000807F
	thumb_func_end EffectSealOfTheAncientsResolve

