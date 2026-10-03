	thumb_func_start EffectWeatherReportResolve
EffectWeatherReportResolve: @ 0x08031E60
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov sl, r0
	mov r0, #0
	mov ip, r0
	mov r5, #5
	mov r4, #1
	ldr r1, _08031F18 @ =0x00000D64
	mov r8, r1
	ldr r7, _08031F1C @ =0x0201930C
	ldr r2, _08031F20 @ =0x0000015B
	mov r9, r2
_08031E7E:
	mov r6, sl
	ldrb r6, [r6, #2]
	lsl r3, r6, #0x1F
	lsr r1, r3, #0x1F
	sub r1, r4, r1
	and r1, r4
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	mov r0, r8
	mul r0, r1
	add r0, r2, r0
	add r0, r0, r7
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	lsr r0, r3, #0x1F
	sub r0, r4, r0
	and r0, r4
	mov r6, r8
	mul r6, r0
	add r0, r6, #0
	add r2, r2, r0
	add r2, r2, r7
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	beq _08031EDE
	cmp r1, #0
	beq _08031EDE
	ldr r2, _08031F24 @ =0x000007FF
	add r0, r2, #0
	and r1, r0
	lsl r0, r1, #1
	ldr r6, _08031F28 @ =0x08622AB4
	add r0, r0, r6
	ldrh r0, [r0]
	cmp r0, r9
	bne _08031EDE
	lsr r0, r3, #0x1F
	sub r0, r4, r0
	add r1, r5, #0
	mov r2, #1
	bl DestroyFieldCard
	mov r0, #1
	mov ip, r0
_08031EDE:
	add r5, #1
	cmp r5, #9
	ble _08031E7E
	mov r1, ip
	cmp r1, #0
	beq _08031F06
	mov r0, #1
	mov r2, sl
	ldrb r2, [r2, #2]
	and r0, r2
	mov r1, #0x47
	cmp r0, #0
	beq _08031EFA
	ldr r1, _08031F2C @ =0x00008047
_08031EFA:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08031F06:
	mov r0, #0
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
_08031F18: .4byte 0x00000D64
_08031F1C: .4byte 0x0201930C
_08031F20: .4byte 0x0000015B
_08031F24: .4byte 0x000007FF
_08031F28: .4byte gCardIdToNumber
_08031F2C: .4byte 0x00008047
	thumb_func_end EffectWeatherReportResolve

