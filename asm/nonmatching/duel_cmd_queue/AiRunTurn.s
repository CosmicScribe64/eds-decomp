	thumb_func_start AiRunTurn
AiRunTurn: @ 0x0801E944
	push {r4, lr}
	ldr r1, _0801E980 @ =0x08198EDC
	ldr r4, _0801E984 @ =0x02015EF0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0801E990
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0801E97A
	ldr r0, _0801E988 @ =0x020192E0
	mov r1, #0xD9
	lsl r1, r1, #5
	add r2, r0, r1
	mov r1, #0
	strb r1, [r2]
	ldr r2, _0801E98C @ =0x00001B21
	add r0, r0, r2
	strb r1, [r0]
	strb r1, [r4, #1]
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0801E97A:
	mov r0, #0
	b _0801E992
	.align 2, 0
_0801E980: .4byte gAiTurnPhases
_0801E984: .4byte 0x02015EF0
_0801E988: .4byte 0x020192E0
_0801E98C: .4byte 0x00001B21
_0801E990:
	mov r0, #1
_0801E992:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end AiRunTurn

