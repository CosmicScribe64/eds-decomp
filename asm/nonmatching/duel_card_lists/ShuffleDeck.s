	thumb_func_start ShuffleDeck
ShuffleDeck: @ 0x08007E68
	push {r4, r5, r6, r7, lr}
	add r3, r1, #0
	ldr r4, _08007EC4 @ =0x020192E4
	mov r1, #1
	and r1, r0
	ldr r0, _08007EC8 @ =0x00000D64
	mul r1, r0
	add r2, r1, r4
	ldrb r0, [r2, #3]
	cmp r0, #0
	beq _08007EBC
	ldrb r0, [r2, #3]
	mul r3, r0
	cmp r3, #0
	ble _08007EBC
	ldr r5, _08007ECC @ =0x000007C4
	add r0, r4, r5
	add r7, r1, r0
	add r5, r3, #0
	add r6, r2, #0
_08007E90:
	bl Random
	ldrb r1, [r6, #3]
	bl __modsi3
	add r4, r0, #0
	bl Random
	ldrb r1, [r6, #3]
	bl __modsi3
	add r1, r0, #0
	lsl r4, r4, #2
	add r4, r7, r4
	lsl r1, r1, #2
	add r1, r7, r1
	add r0, r4, #0
	bl SwapDuelCards
	sub r5, #1
	cmp r5, #0
	bne _08007E90
_08007EBC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08007EC4: .4byte 0x020192E4
_08007EC8: .4byte 0x00000D64
_08007ECC: .4byte 0x000007C4
	thumb_func_end ShuffleDeck

