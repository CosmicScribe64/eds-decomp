	thumb_func_start EffectFinalDestinyResolve
EffectFinalDestinyResolve: @ 0x08035D78
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r6, r0, #0
	mov r0, #4
	ldrb r1, [r6, #4]
	and r0, r1
	cmp r0, #0
	bne _08035DF6
	mov r2, #0
_08035D8C:
	cmp r2, #0
	beq _08035D98
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r5, r0, #0x1F
	b _08035DA2
_08035D98:
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	mov r1, #1
	sub r5, r1, r0
_08035DA2:
	mov r4, #0
	add r2, #1
	mov r8, r2
	mov r0, #1
	and r0, r5
	ldr r1, _08035E04 @ =0x00000D64
	add r7, r0, #0
	mul r7, r1
_08035DB2:
	mov r0, #0x94
	mul r0, r4
	add r0, r0, r7
	ldr r1, _08035E08 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08035DEA
	cmp r4, #4
	bgt _08035DD4
	add r0, r5, #0
	add r1, r4, #0
	bl IsZoneTargetable
	cmp r0, #0
	beq _08035DEA
_08035DD4:
	add r0, r5, #0
	add r1, r4, #0
	bl DestroyFieldCardByEffect
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r5, #0
	add r2, r4, #0
	bl OnCardDestroyedByEffect
_08035DEA:
	add r4, #1
	cmp r4, #0xA
	ble _08035DB2
	mov r2, r8
	cmp r2, #1
	ble _08035D8C
_08035DF6:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08035E04: .4byte 0x00000D64
_08035E08: .4byte 0x0201930C
	thumb_func_end EffectFinalDestinyResolve

