	thumb_func_start RemoveAllDeckCardsByNumber
RemoveAllDeckCardsByNumber: @ 0x080590E4
	push {r4, r5, r6, r7, lr}
	sub sp, #4
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	mov r4, #0
	ldr r1, _08059130 @ =0x020192E4
	mov r2, #1
	and r2, r6
	ldr r3, _08059134 @ =0x00000D64
	add r0, r2, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r4, r0
	bge _08059152
	add r5, r2, #0
_08059106:
	lsl r1, r4, #2
	add r0, r5, #0
	mul r0, r3
	add r1, r1, r0
	ldr r0, _08059138 @ =0x02019AA8
	add r1, r1, r0
	ldr r0, [r1]
	lsl r0, r0, #0x15
	lsr r0, r0, #0x14
	ldr r1, _0805913C @ =0x08622AB4
	add r0, r0, r1
	ldrh r0, [r0]
	cmp r0, r7
	bne _08059140
	add r0, r6, #0
	add r1, r4, #0
	mov r2, sp
	bl TakeDeckCardAt
	b _08059142
	.align 2, 0
_08059130: .4byte 0x020192E4
_08059134: .4byte 0x00000D64
_08059138: .4byte 0x02019AA8
_0805913C: .4byte gCardIdToNumber
_08059140:
	add r4, #1
_08059142:
	ldr r1, _0805915C @ =0x020192E4
	ldr r3, _08059160 @ =0x00000D64
	add r0, r5, #0
	mul r0, r3
	add r0, r0, r1
	ldrb r0, [r0, #3]
	cmp r4, r0
	blt _08059106
_08059152:
	add sp, #4
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_0805915C: .4byte 0x020192E4
_08059160: .4byte 0x00000D64
	thumb_func_end RemoveAllDeckCardsByNumber

