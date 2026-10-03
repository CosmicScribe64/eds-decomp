	thumb_func_start AiStepBattle
AiStepBattle: @ 0x0805BB80
	push {r4, lr}
	ldr r4, _0805BB98 @ =0x02015EF0
	ldrb r0, [r4, #0xA]
	cmp r0, #0
	bne _0805BBBC
	mov r0, #1
	bl CanEnterBattlePhase
	cmp r0, #0
	bne _0805BB9C
	mov r0, #1
	b _0805BBC6
_0805BB98: .4byte 0x02015EF0
_0805BB9C:
	ldrb r0, [r4, #0xA]
	add r0, #1
	strb r0, [r4, #0xA]
	ldr r2, _0805BBCC @ =0x020192E0
	ldr r0, _0805BBD0 @ =0x00001B14
	add r3, r2, r0
	ldr r0, [r3]
	ldr r1, _0805BBD4 @ =0xFFFE01FF
	and r0, r1
	str r0, [r3]
	ldr r1, _0805BBD8 @ =0x00001B16
	add r2, r2, r1
	ldr r0, _0805BBDC @ =0xFFFFFE01
	ldrh r1, [r2]
	and r0, r1
	strh r0, [r2]
_0805BBBC:
	mov r0, #1
	bl BattlePhase_Run
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
_0805BBC6:
	pop {r4}
	pop {r1}
	bx r1
_0805BBCC: .4byte 0x020192E0
_0805BBD0: .4byte 0x00001B14
_0805BBD4: .4byte 0xFFFE01FF
_0805BBD8: .4byte 0x00001B16
_0805BBDC: .4byte 0xFFFFFE01
	thumb_func_end AiStepBattle

