	thumb_func_start AiPickDiscard
AiPickDiscard: @ 0x08056CE8
	push {r4, r5, r6, r7, lr}
	mov r4, #1
	neg r4, r4
	mov r1, #0xFC
	lsl r1, r1, #2
	mov r0, #1
	bl FindHandCardByNumber
	cmp r0, r4
	bgt _08056D14
	mov r1, #0x91
	lsl r1, r1, #3
	mov r0, #1
	bl FindHandCardByNumber
	cmp r0, r4
	bgt _08056D14
	ldr r0, _08056D30 @ =0x00000447
	bl AiHasUsableSpellTrap
	cmp r0, #0
	beq _08056D3C
_08056D14:
	mov r0, #1
	bl CountFreeMonsterZones
	cmp r0, #0
	ble _08056D3C
	ldr r0, _08056D34 @ =0x020192E4
	mov r1, #1
	bl AiPickStrongestHandMonster
	add r4, r0, #0
	cmp r4, #0
	blt _08056D3C
	b _08056D7E
	.align 2, 0
_08056D30: .4byte 0x00000447
_08056D34: .4byte 0x020192E4
_08056D38:
	add r0, r2, #0
	b _08056D7E
_08056D3C:
	mov r2, #0
	ldr r0, _08056D84 @ =0x020192E4
	ldr r3, _08056D88 @ =0x00000D66
	add r1, r0, r3
	add r4, r0, #0
	ldrb r1, [r1]
	cmp r2, r1
	bge _08056D76
	mov r7, #0xED
	lsl r7, r7, #1
	add r0, r4, r3
	ldrb r3, [r0]
	ldr r0, _08056D8C @ =0x000013E8
	add r1, r4, r0
	ldr r6, _08056D90 @ =0x000007FF
	ldr r5, _08056D94 @ =0x08622AB4
_08056D5C:
	ldr r0, [r1]
	lsl r0, r0, #0x14
	lsr r0, r0, #0x14
	and r0, r6
	lsl r0, r0, #1
	add r0, r0, r5
	ldrh r0, [r0]
	cmp r0, r7
	beq _08056D38
	add r1, #4
	add r2, #1
	cmp r2, r3
	blt _08056D5C
_08056D76:
	add r0, r4, #0
	mov r1, #1
	bl AiPickWeakestHandCard
_08056D7E:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_08056D84: .4byte 0x020192E4
_08056D88: .4byte 0x00000D66
_08056D8C: .4byte 0x000013E8
_08056D90: .4byte 0x000007FF
_08056D94: .4byte gCardIdToNumber
	thumb_func_end AiPickDiscard

