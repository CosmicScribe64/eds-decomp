	thumb_func_start EffectTributeNegateMagicResolve
EffectTributeNegateMagicResolve: @ 0x0803C1D4
	push {r4, r5, r6, lr}
	add r4, r0, #0
	add r6, r1, #0
	ldr r5, _0803C244 @ =0x0000058A
	mov r0, #0
	add r1, r5, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0803C23C
	mov r0, #1
	add r1, r5, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bgt _0803C23C
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldrh r2, [r4, #2]
	lsl r1, r2, #0x16
	lsr r1, r1, #0x1A
	bl TributeMonster
	cmp r6, #0
	beq _0803C23C
	ldr r0, _0803C248 @ =0x000007FF
	ldrh r6, [r6]
	and r0, r6
	lsl r0, r0, #2
	ldr r1, _0803C24C @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0803C23C
	mov r0, #1
	ldrb r4, [r4, #2]
	and r0, r4
	mov r1, #0xB0
	cmp r0, #0
	beq _0803C230
	ldr r1, _0803C250 @ =0x000080B0
_0803C230:
	add r0, r1, #0
	mov r1, #1
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
_0803C23C:
	mov r0, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0803C244: .4byte 0x0000058A
_0803C248: .4byte 0x000007FF
_0803C24C: .4byte gCardStats
_0803C250: .4byte 0x000080B0
	thumb_func_end EffectTributeNegateMagicResolve

