	thumb_func_start DamageOpponentPerBanishedMonster
DamageOpponentPerBanishedMonster: @ 0x08046E8C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	ldr r1, _08046F04 @ =0x000005FD
	bl CountActiveCardsOnField
	cmp r0, #0
	beq _08046EFC
	mov r4, #0
	ldr r3, _08046F08 @ =0x020192E4
	mov r1, #1
	sub r0, r1, r5
	and r0, r1
	ldr r1, _08046F0C @ =0x00000D64
	add r2, r0, #0
	mul r2, r1
	add r0, r2, r3
	ldrb r1, [r0, #6]
	cmp r4, r1
	bge _08046EE2
	ldr r6, _08046F10 @ =0x00000B84
	add r0, r3, r6
	add r3, r2, r0
	ldr r6, _08046F14 @ =0x000007FF
	add r2, r1, #0
_08046EBC:
	ldr r0, [r3]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _08046F18 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _08046EDA
	add r4, #1
_08046EDA:
	add r3, #4
	sub r2, #1
	cmp r2, #0
	bne _08046EBC
_08046EE2:
	cmp r4, #0
	ble _08046EFC
	ldr r0, _08046F1C @ =0x086249EE
	ldrh r1, [r0]
	add r0, r5, #0
	bl ShowCardEffect
	mov r0, #1
	sub r0, r0, r5
	mov r1, #0x64
	mul r1, r4
	bl LoseLifePoints
_08046EFC:
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
_08046F04: .4byte 0x000005FD
_08046F08: .4byte 0x020192E4
_08046F0C: .4byte 0x00000D64
_08046F10: .4byte 0x00000B84
_08046F14: .4byte 0x000007FF
_08046F18: .4byte gCardStats
_08046F1C: .4byte gUnk_086249EE
	thumb_func_end DamageOpponentPerBanishedMonster

