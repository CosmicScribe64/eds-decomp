	thumb_func_start AddCardToBanishedTemporarily
AddCardToBanishedTemporarily: @ 0x080097F0
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r3, r0, #0
	mov r8, r1
	ldr r2, [r3]
	lsl r0, r2, #0x13
	mov r1, #1
	mov r9, r1
	lsr r0, r0, #0x1F
	ldr r1, _080098A8 @ =0x00000D64
	add r5, r0, #0
	mul r5, r1
	ldr r6, _080098AC @ =0x02019E68
	add r1, r5, r6
	ldr r4, _080098B0 @ =0xFFFFF47C
	add r0, r6, r4
	add r4, r5, r0
	ldrb r7, [r4, #6]
	lsl r0, r7, #2
	add r1, r1, r0
	lsl r2, r2, #0x14
	lsr r2, r2, #0x14
	cmp r2, #0
	beq _0800989A
	ldr r0, _080098B4 @ =0x000007FF
	and r2, r0
	lsl r0, r2, #1
	ldr r2, _080098B8 @ =0x08622AB4
	add r0, r0, r2
	ldrh r0, [r0]
	ldr r7, _080098BC @ =0xFFFFF880
	add r0, r0, r7
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	cmp r0, #0x4F
	bls _0800989A
	add r0, r1, #0
	add r1, r3, #0
	bl CopyDuelCard
	ldrb r0, [r4, #6]
	lsl r2, r0, #1
	add r2, r2, r5
	mov r1, #0xA0
	lsl r1, r1, #1
	add r0, r6, r1
	add r2, r2, r0
	mov r7, r8
	lsl r0, r7, #0x18
	lsr r0, r0, #0x10
	mov r1, #1
	orr r0, r1
	strh r0, [r2]
	ldrb r0, [r4, #6]
	add r0, #1
	strb r0, [r4, #6]
	ldrb r3, [r4, #0xB]
	lsr r0, r3, #4
	mov r1, r9
	ldrb r2, [r4, #0xC]
	and r1, r2
	lsl r1, r1, #4
	orr r1, r0
	mov r0, r9
	lsl r0, r7
	orr r1, r0
	lsl r1, r1, #0x10
	lsr r2, r1, #0x10
	mov r0, #0xF
	and r2, r0
	lsl r2, r2, #4
	and r0, r3
	orr r0, r2
	strb r0, [r4, #0xB]
	lsr r1, r1, #0x14
	mov r7, r9
	and r1, r7
	mov r0, #2
	neg r0, r0
	ldrb r2, [r4, #0xC]
	and r0, r2
	orr r0, r1
	strb r0, [r4, #0xC]
_0800989A:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080098A8: .4byte 0x00000D64
_080098AC: .4byte 0x02019E68
_080098B0: .4byte 0xFFFFF47C
_080098B4: .4byte 0x000007FF
_080098B8: .4byte gCardIdToNumber
_080098BC: .4byte 0xFFFFF880
	thumb_func_end AddCardToBanishedTemporarily

