	thumb_func_start AiChainSetCard
AiChainSetCard: @ 0x08058924
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	mov r4, #5
	ldr r7, _080589A4 @ =0x0201A070
_0805892C:
	mov r0, #0x94
	mul r0, r4
	add r5, r0, r7
	ldr r0, [r5]
	lsl r0, r0, #0x14
	lsr r3, r0, #0x14
	cmp r3, #0
	beq _080589B8
	mov r0, #2
	ldrb r2, [r5, #6]
	and r0, r2
	cmp r0, #0
	bne _080589B8
	add r2, r5, #0
	add r2, #0x91
	mov r0, #4
	ldrb r2, [r2]
	and r0, r2
	cmp r0, #0
	beq _080589B8
	ldr r0, _080589A8 @ =0x000007FF
	and r3, r0
	lsl r0, r3, #1
	ldr r2, _080589AC @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	cmp r0, r1
	bne _080589B8
	ldr r0, _080589B0 @ =0x00008007
	mov r1, #0
	mov r2, #0
	mov r3, #0
	bl DuelCmd_Push
	mov r0, #1
	add r1, r4, #0
	mov r2, #0
	bl FlipFieldCard
	ldrb r1, [r6, #3]
	lsr r0, r1, #2
	lsl r0, r0, #0x19
	mov r1, #0x1F
	and r4, r1
	lsl r1, r4, #0x10
	ldr r2, _080589B4 @ =0x80200000
	orr r1, r2
	orr r0, r1
	ldr r1, [r5]
	lsl r1, r1, #0x14
	lsr r1, r1, #0x14
	orr r0, r1
	ldrh r2, [r6, #8]
	lsl r1, r2, #0x10
	ldrh r6, [r6, #6]
	orr r1, r6
	bl Chain_AddLink
	mov r0, #1
	b _080589C0
_080589A4: .4byte 0x0201A070
_080589A8: .4byte 0x000007FF
_080589AC: .4byte gCardIdToNumber
_080589B0: .4byte 0x00008007
_080589B4: .4byte 0x80200000
_080589B8:
	add r4, #1
	cmp r4, #9
	ble _0805892C
	mov r0, #0
_080589C0:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end AiChainSetCard
	.align 2, 0

