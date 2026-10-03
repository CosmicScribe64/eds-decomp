	thumb_func_start EffectReturnBanishedToGravePrepare
EffectReturnBanishedToGravePrepare: @ 0x0802FFE4
	push {r4, r5, lr}
	add r5, r0, #0
	ldr r4, _08030018 @ =0x00000453
	mov r0, #0
	add r1, r4, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _08030020
	mov r0, #1
	add r1, r4, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _08030020
	ldrb r5, [r5, #2]
	lsl r0, r5, #0x1F
	lsr r0, r0, #0x1F
	ldr r1, _0803001C @ =0x0000060D
	mov r2, #0
	bl CollectEffectTargets
	cmp r0, #4
	ble _08030020
	mov r0, #1
	b _08030022
_08030018: .4byte 0x00000453
_0803001C: .4byte 0x0000060D
_08030020:
	mov r0, #0
_08030022:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end EffectReturnBanishedToGravePrepare

