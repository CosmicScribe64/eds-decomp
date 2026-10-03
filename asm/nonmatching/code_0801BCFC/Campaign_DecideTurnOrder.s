	thumb_func_start Campaign_DecideTurnOrder
Campaign_DecideTurnOrder: @ 0x0801BE0C
	push {lr}
	ldr r1, _0801BE24 @ =0x03000040
	ldr r0, _0801BE28 @ =0x00004888
	add r1, r1, r0
	mov r0, #0x30
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0
	bne _0801BE2C
	bl TurnOrder_RunRps
	b _0801BE50
_0801BE24: .4byte 0x03000040
_0801BE28: .4byte 0x00004888
_0801BE2C:
	ldr r1, _0801BE44 @ =0x020192E0
	ldr r0, _0801BE48 @ =0x00001B12
	add r1, r1, r0
	mov r0, #0xC0
	ldrb r1, [r1]
	and r0, r1
	cmp r0, #0x80
	beq _0801BE4C
	bl TurnOrder_RunCpuChoice
	b _0801BE50
	.align 2, 0
_0801BE44: .4byte 0x020192E0
_0801BE48: .4byte 0x00001B12
_0801BE4C:
	bl TurnOrder_RunPlayerChoice
_0801BE50:
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end Campaign_DecideTurnOrder

