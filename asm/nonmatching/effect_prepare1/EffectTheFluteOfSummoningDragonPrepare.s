	thumb_func_start EffectTheFluteOfSummoningDragonPrepare
EffectTheFluteOfSummoningDragonPrepare: @ 0x0802DCC4
	push {r4, r5, r6, lr}
	add r4, r0, #0
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CountFreeMonsterZones
	cmp r0, #0
	beq _0802DD8C
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl CanSpecialSummon
	cmp r0, #0
	beq _0802DD8C
	mov r5, #0xB9
	lsl r5, r5, #2
	mov r0, #0
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	bgt _0802DD00
	mov r0, #1
	add r1, r5, #0
	bl CountFaceUpMonstersByNumber
	cmp r0, #0
	ble _0802DD8C
_0802DD00:
	mov r5, #0
	ldr r3, _0802DD64 @ =0x020192E4
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0802DD68 @ =0x00000D64
	mul r0, r2
	add r0, r0, r3
	ldrb r0, [r0, #2]
	cmp r5, r0
	bge _0802DD8C
	add r6, r3, #0
_0802DD18:
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	lsl r1, r5, #2
	mul r0, r2
	add r1, r1, r0
	ldr r0, _0802DD6C @ =0x02019968
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x15
	lsr r2, r0, #0x15
	lsl r0, r2, #2
	ldr r1, _0802DD70 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #1
	bne _0802DD78
	lsl r0, r2, #1
	ldr r1, _0802DD74 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	bl IsToonMonster
	cmp r0, #0
	beq _0802DD5E
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	bl HasFaceUpToonWorld
	cmp r0, #0
	beq _0802DD78
_0802DD5E:
	mov r0, #1
	b _0802DD8E
	.align 2, 0
_0802DD64: .4byte 0x020192E4
_0802DD68: .4byte 0x00000D64
_0802DD6C: .4byte 0x02019968
_0802DD70: .4byte gCardStats
_0802DD74: .4byte gCardIdToNumber
_0802DD78:
	add r5, #1
	ldrb r1, [r4, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	ldr r2, _0802DD94 @ =0x00000D64
	mul r0, r2
	add r0, r0, r6
	ldrb r0, [r0, #2]
	cmp r5, r0
	blt _0802DD18
_0802DD8C:
	mov r0, #0
_0802DD8E:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
_0802DD94: .4byte 0x00000D64
	thumb_func_end EffectTheFluteOfSummoningDragonPrepare

