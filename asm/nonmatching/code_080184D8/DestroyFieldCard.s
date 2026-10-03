	thumb_func_start DestroyFieldCard
DestroyFieldCard: @ 0x08018544
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	add r6, r0, #0
	add r7, r1, #0
	lsl r2, r2, #0x10
	lsr r5, r2, #0x10
	mov r2, #1
	and r2, r6
	mov r0, #0x94
	mul r0, r7
	ldr r1, _08018610 @ =0x00000D64
	add r3, r2, #0
	mul r3, r1
	mov r8, r3
	add r0, r8
	ldr r1, _08018614 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r4, r0, #0x14
	mov r9, r4
	neg r3, r5
	orr r3, r5
	lsr r3, r3, #0x1F
	add r0, r6, #0
	add r1, r7, #0
	add r2, r5, #0
	bl SendFieldCardToGrave
	cmp r4, #0
	beq _08018602
	ldr r0, _08018618 @ =0x000007FF
	and r4, r0
	lsl r0, r4, #1
	ldr r1, _0801861C @ =0x08622AB4
	add r0, r0, r1
	ldrh r1, [r0]
	mov r0, #0xBF
	lsl r0, r0, #3
	cmp r1, r0
	beq _080185AA
	cmp r1, r0
	blt _08018602
	add r0, #0x10
	cmp r1, r0
	bgt _08018602
	sub r0, #3
	cmp r1, r0
	blt _08018602
_080185AA:
	add r0, r6, #0
	mov r1, r9
	bl ShowDestroyedCard
	mov r4, #5
_080185B4:
	cmp r4, r7
	beq _080185FC
	mov r0, #0x94
	mul r0, r4
	add r0, r8
	ldr r1, _08018614 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r1, r0, #0x14
	cmp r1, #0
	beq _080185FC
	ldr r0, _08018618 @ =0x000007FF
	and r1, r0
	lsl r0, r1, #1
	ldr r3, _0801861C @ =0x08622AB4
	add r0, r0, r3
	ldrh r1, [r0]
	mov r0, #0xBF
	lsl r0, r0, #3
	cmp r1, r0
	beq _080185F0
	cmp r1, r0
	blt _080185FC
	add r0, #0x10
	cmp r1, r0
	bgt _080185FC
	sub r0, #3
	cmp r1, r0
	blt _080185FC
_080185F0:
	add r0, r6, #0
	add r1, r4, #0
	add r2, r5, #0
	mov r3, #0
	bl SendFieldCardToGrave
_080185FC:
	add r4, #1
	cmp r4, #9
	ble _080185B4
_08018602:
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08018610: .4byte 0x00000D64
_08018614: .4byte 0x0201930C
_08018618: .4byte 0x000007FF
_0801861C: .4byte gCardIdToNumber
	thumb_func_end DestroyFieldCard

