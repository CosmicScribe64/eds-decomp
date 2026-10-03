	thumb_func_start EffectPatrolRoboResolve
EffectPatrolRoboResolve: @ 0x08031DA8
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r4, r0, #0
	mov r0, #4
	ldrb r1, [r4, #4]
	and r0, r1
	cmp r0, #0
	bne _08031E44
	mov r5, #7
	ldrb r0, [r4, #0xA]
	and r5, r0
	cmp r5, #1
	bne _08031E44
	ldrb r6, [r4, #0xC]
	ldrh r1, [r4, #0xC]
	lsr r3, r1, #8
	add r1, r6, #0
	and r1, r5
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _08031E50 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _08031E54 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	lsr r7, r0, #0x14
	cmp r7, #0
	beq _08031E44
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r6, r0
	beq _08031E44
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _08031E44
	mov r0, #0x7F
	cmp r6, #0
	beq _08031E04
	ldr r0, _08031E58 @ =0x0000807F
_08031E04:
	mov r8, r3
	mov r1, r8
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	add r0, r6, #0
	add r1, r7, #0
	bl ShowCardDetail
	ldrb r0, [r4, #2]
	and r5, r0
	mov r0, #0x92
	cmp r5, #0
	beq _08031E24
	ldr r0, _08031E5C @ =0x00008092
_08031E24:
	ldrh r4, [r4, #2]
	lsl r1, r4, #0x16
	lsr r1, r1, #0x1A
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #0x7F
	cmp r6, #0
	beq _08031E3A
	ldr r0, _08031E58 @ =0x0000807F
_08031E3A:
	mov r1, r8
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_08031E44:
	mov r0, #0
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08031E50: .4byte 0x00000D64
_08031E54: .4byte 0x0201930C
_08031E58: .4byte 0x0000807F
_08031E5C: .4byte 0x00008092
	thumb_func_end EffectPatrolRoboResolve

