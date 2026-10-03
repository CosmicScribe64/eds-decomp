	thumb_func_start LinkBattle_DecideTurnOrder
LinkBattle_DecideTurnOrder: @ 0x0801A8A4
	push {lr}
	ldr r0, _0801A8C4 @ =0x020192E0
	ldr r1, _0801A8C8 @ =0x00001B12
	add r0, r0, r1
	mov r1, #0x21
	neg r1, r1
	ldrb r2, [r0]
	and r1, r2
	strb r1, [r0]
	bl TurnOrder_RunRpsLink
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	pop {r1}
	bx r1
	.align 2, 0
_0801A8C4: .4byte 0x020192E0
_0801A8C8: .4byte 0x00001B12
	thumb_func_end LinkBattle_DecideTurnOrder

