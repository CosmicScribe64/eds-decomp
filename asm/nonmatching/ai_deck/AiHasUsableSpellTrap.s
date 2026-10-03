	thumb_func_start AiHasUsableSpellTrap
AiHasUsableSpellTrap: @ 0x0805930C
	push {r4, r5, r6, lr}
	sub sp, #0x14
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	mov r5, #5
	mov r4, sp
_08059318:
	mov r0, #0x94
	add r1, r5, #0
	mul r1, r0
	ldr r0, _0805935C @ =0x0201A070
	add r2, r1, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _08059364
	ldrb r0, [r4, #2]
	mov r1, #1
	orr r0, r1
	strb r0, [r4, #2]
	ldrb r1, [r4, #3]
	mov r0, #3
	and r0, r1
	strb r0, [r4, #3]
	ldr r0, [r2]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _08059360 @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r6
	bne _08059364
	mov r0, sp
	mov r1, #1
	add r2, r5, #0
	bl CanActivateFieldCard
	cmp r0, #0
	beq _08059364
	mov r0, #1
	b _0805936C
_0805935C: .4byte 0x0201A070
_08059360: .4byte gCardIdToNumber
_08059364:
	add r5, #1
	cmp r5, #9
	ble _08059318
	mov r0, #0
_0805936C:
	add sp, #0x14
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end AiHasUsableSpellTrap

