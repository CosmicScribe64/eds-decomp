	thumb_func_start AiCanChangePosition
AiCanChangePosition: @ 0x0805763C
	push {r4, r5, r6, lr}
	add r5, r0, #0
	add r6, r1, #0
	mov r1, #1
	and r1, r5
	mov r0, #0x94
	add r2, r6, #0
	mul r2, r0
	ldr r0, _080576AC @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _080576B0 @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _080576A8
	ldrb r2, [r2, #7]
	mov r0, #4
	and r0, r2
	cmp r0, #0
	bne _080576A8
	mov r0, #8
	and r0, r2
	cmp r0, #0
	bne _080576A8
	mov r2, #0xAE
	lsl r2, r2, #1
	add r0, r5, #0
	add r1, r6, #0
	bl CountZoneLinksFromCard
	cmp r0, #0
	bne _080576A8
	mov r4, #0xA4
	lsl r4, r4, #1
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	bgt _0805769C
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField2
	cmp r0, #0
	ble _080576B4
_0805769C:
	add r0, r5, #0
	add r1, r6, #0
	bl GetZoneCardType
	cmp r0, #1
	bne _080576B4
_080576A8:
	mov r0, #0
	b _080576B6
_080576AC: .4byte 0x00000D64
_080576B0: .4byte 0x0201930C
_080576B4:
	mov r0, #1
_080576B6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end AiCanChangePosition

