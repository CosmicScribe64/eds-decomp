	thumb_func_start AiShouldSetMonster
AiShouldSetMonster: @ 0x080576BC
	push {r4, r5, lr}
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r1, r1, #0x10
	cmp r1, #0
	bne _080576EE
	ldr r0, _08057714 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #1
	ldr r1, _08057718 @ =0x08622AB4
	add r4, r0, r1
	ldrh r0, [r4]
	mov r1, #1
	bl HasFlipEffect
	cmp r0, #0
	beq _080576E0
	b _080577F4
_080576E0:
	ldrh r0, [r4]
	mov r1, #0
	bl HasFlipEffect
	cmp r0, #0
	beq _080576EE
	b _080577F4
_080576EE:
	ldr r2, _08057714 @ =0x000007FF
	and r2, r5
	lsl r0, r2, #1
	ldr r1, _08057718 @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	ldr r0, _0805771C @ =0x0000016D
	cmp r1, r0
	beq _080577F4
	ldr r0, _08057720 @ =0x000002DA
	cmp r1, r0
	bne _08057724
	mov r0, #0
	bl CountMonsters
	cmp r0, #0
	bgt _080577E0
	b _080577F4
	.align 2, 0
_08057714: .4byte 0x000007FF
_08057718: .4byte gCardIdToNumber
_0805771C: .4byte 0x0000016D
_08057720: .4byte 0x000002DA
_08057724:
	lsl r0, r2, #2
	ldr r1, _08057744 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _08057752
	cmp r0, #0x17
	ble _08057748
	cmp r0, #0x18
	beq _0805774C
	b _08057752
	.align 2, 0
_08057744: .4byte gCardStats
_08057748:
	mov r0, #0
	b _08057768
_0805774C:
	mov r0, #0xFA
	lsl r0, r0, #4
	b _08057768
_08057752:
	ldr r0, _080577A0 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _080577A4 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r0, r0, #1
_08057768:
	add r4, r0, #0
	mov r1, #1
	neg r1, r1
	mov r0, #0
	mov r2, #1
	mov r3, #0
	bl AiGetStrongestMonsterScore
	cmp r4, r0
	ble _080577F4
	ldr r0, _080577A0 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _080577A4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x15
	blt _080577B2
	cmp r0, #0x17
	ble _080577A8
	cmp r0, #0x18
	beq _080577AC
	b _080577B2
	.align 2, 0
_080577A0: .4byte 0x000007FF
_080577A4: .4byte gCardStats
_080577A8:
	mov r1, #0
	b _080577C8
_080577AC:
	mov r1, #0xFA
	lsl r1, r1, #4
	b _080577C8
_080577B2:
	ldr r0, _080577E4 @ =0x000007FF
	and r0, r5
	lsl r0, r0, #2
	ldr r1, _080577E8 @ =0x08621DE0
	add r0, r0, r1
	ldr r1, [r0]
	lsl r1, r1, #0xE
	lsr r1, r1, #0x17
	lsl r0, r1, #2
	add r0, r0, r1
	lsl r1, r0, #1
_080577C8:
	mov r0, #0xFA
	lsl r0, r0, #2
	cmp r1, r0
	bhi _080577E0
	ldr r1, _080577EC @ =0x020192E4
	ldrb r0, [r1, #3]
	cmp r0, #4
	bls _080577E0
	ldr r0, _080577F0 @ =0x000003E7
	ldrh r1, [r1]
	cmp r1, r0
	bhi _080577F4
_080577E0:
	mov r0, #0
	b _080577F6
_080577E4: .4byte 0x000007FF
_080577E8: .4byte gCardStats
_080577EC: .4byte 0x020192E4
_080577F0: .4byte 0x000003E7
_080577F4:
	mov r0, #1
_080577F6:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end AiShouldSetMonster

