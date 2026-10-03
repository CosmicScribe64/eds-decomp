	thumb_func_start BattleScene_DrawValues
BattleScene_DrawValues: @ 0x0805DEA4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsr r7, r0, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	mov r8, r2
	mov r5, #0
	add r4, r1, #0
	mov r6, #0
_0805DEBA:
	mov r0, r8
	cmp r0, #0
	beq _0805DECA
	mov r0, #2
	lsl r0, r6
	and r0, r7
	cmp r0, #0
	bne _0805DEF0
_0805DECA:
	add r0, r7, #0
	asr r0, r6
	mov r1, #1
	and r0, r1
	cmp r0, #0
	beq _0805DEE4
	ldr r3, [r4]
	add r0, r5, #0
	mov r1, #0
	mov r2, #0
	bl BattleScene_DrawDef
	b _0805DEF0
_0805DEE4:
	ldr r3, [r4]
	add r0, r5, #0
	mov r1, #0
	mov r2, #0
	bl BattleScene_DrawAtk
_0805DEF0:
	add r4, #4
	add r6, #8
	add r5, #1
	cmp r5, #1
	ble _0805DEBA
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end BattleScene_DrawValues

