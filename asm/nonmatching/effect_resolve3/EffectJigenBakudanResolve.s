	thumb_func_start EffectJigenBakudanResolve
EffectJigenBakudanResolve: @ 0x08032E7C
	push {r4, r5, r6, r7, lr}
	add r5, r0, #0
	mov r0, #4
	ldrb r1, [r5, #4]
	and r0, r1
	cmp r0, #0
	bne _08032F12
	mov r0, #0xFC
	ldrb r1, [r5, #3]
	and r0, r1
	cmp r0, #8
	bne _08032EF4
	mov r6, #0
	mov r4, #0
_08032E98:
	ldrb r0, [r5, #2]
	lsl r3, r0, #0x1F
	mov r7, #1
	lsr r2, r3, #0x1F
	mov r0, #0x94
	add r1, r4, #0
	mul r1, r0
	ldr r0, _08032EEC @ =0x00000D64
	mul r0, r2
	add r1, r1, r0
	ldr r0, _08032EF0 @ =0x0201930C
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08032ECE
	add r0, r2, #0
	add r1, r4, #0
	bl GetZoneCardAtk
	add r6, r6, r0
	ldrb r1, [r5, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	add r1, r4, #0
	bl TributeMonster
_08032ECE:
	add r4, #1
	cmp r4, #4
	ble _08032E98
	ldrb r5, [r5, #2]
	lsl r4, r5, #0x1F
	lsr r4, r4, #0x1F
	sub r4, r7, r4
	add r0, r6, #0
	bl HalveRoundUp
	add r1, r0, #0
	add r0, r4, #0
	bl LoseLifePoints
	b _08032F12
_08032EEC: .4byte 0x00000D64
_08032EF0: .4byte 0x0201930C
_08032EF4:
	mov r0, #1
	ldrb r1, [r5, #2]
	and r0, r1
	mov r2, #0x92
	cmp r0, #0
	beq _08032F02
	ldr r2, _08032F1C @ =0x00008092
_08032F02:
	ldrh r5, [r5, #2]
	lsl r1, r5, #0x16
	lsr r1, r1, #0x1A
	add r0, r2, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08032F12:
	mov r0, #0
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08032F1C: .4byte 0x00008092
	thumb_func_end EffectJigenBakudanResolve

