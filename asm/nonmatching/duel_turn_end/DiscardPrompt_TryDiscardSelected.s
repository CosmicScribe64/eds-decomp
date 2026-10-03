	thumb_func_start DiscardPrompt_TryDiscardSelected
DiscardPrompt_TryDiscardSelected: @ 0x0805163C
	push {r4, r5, r6, lr}
	lsl r0, r0, #0x10
	lsr r5, r0, #0x10
	lsl r1, r1, #0x10
	lsr r6, r1, #0x10
	ldr r4, _08051654 @ =0x020192E4
	ldrb r0, [r4, #2]
	cmp r0, #0
	bne _08051658
	mov r0, #1
	b _080516D0
	.align 2, 0
_08051654: .4byte 0x020192E4
_08051658:
	mov r0, #1
	bl DuelCursor_PickTarget
	cmp r0, #0
	beq _080516CE
	ldr r0, _080516B8 @ =0x0201CFB0
	ldr r1, _080516BC @ =0x0000082C
	add r0, r0, r1
	ldr r3, [r0]
	lsl r1, r3, #2
	ldr r2, _080516C0 @ =0x00000684
	add r0, r4, r2
	add r2, r1, r0
	cmp r5, #0
	beq _0805168E
	ldr r0, [r2]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x13
	ldr r1, _080516C4 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x14
	bhi _080516C8
_0805168E:
	ldr r1, [r2]
	mov r2, #1
	lsl r0, r1, #0xE
	cmp r0, #0
	bge _0805169A
	mov r2, #0
_0805169A:
	lsl r0, r1, #0xD
	cmp r0, #0
	bge _080516A2
	mov r2, #0
_080516A2:
	cmp r2, #0
	beq _080516C8
	mov r0, #0
	add r1, r3, #0
	add r2, r6, #0
	mov r3, #1
	bl DiscardHandCard
	mov r0, #1
	b _080516D0
	.align 2, 0
_080516B8: .4byte 0x0201CFB0
_080516BC: .4byte 0x0000082C
_080516C0: .4byte 0x00000684
_080516C4: .4byte gCardStats
_080516C8:
	mov r0, #3
	bl PlaySE
_080516CE:
	mov r0, #0
_080516D0:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end DiscardPrompt_TryDiscardSelected
	.align 2, 0

