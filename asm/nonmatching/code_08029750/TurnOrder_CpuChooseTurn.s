	thumb_func_start TurnOrder_CpuChooseTurn
TurnOrder_CpuChooseTurn: @ 0x08029D10
	push {r4, r5, lr}
	sub sp, #0x10
	ldr r0, _08029D6C @ =0x086AC828
	mov r1, #0x10
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	ldr r0, _08029D70 @ =0x086AD028
	mov r1, #0x18
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	ldr r0, _08029D74 @ =0x086AD828
	mov r1, #0x88
	lsl r1, r1, #1
	mov r2, #8
	mov r3, #8
	bl TurnOrder_LoadObjTiles
	mov r5, #3
	str r5, [sp, #0]
	mov r0, #1
	str r0, [sp, #4]
	ldr r4, _08029D78 @ =0x02020DEC
	str r4, [sp, #8]
	mov r0, #0
	str r0, [sp, #0xC]
	mov r1, #0
	mov r2, #0x40
	mov r3, #0xF
	bl TweenInit
	strb r5, [r4, #0x19]
	bl Random
	mov r1, #1
	and r0, r1
	sub r4, #0x1D
	strb r0, [r4]
	mov r0, #1
	add sp, #0x10
	pop {r4, r5}
	pop {r1}
	bx r1
_08029D6C: .4byte gDuelLogoTiles0
_08029D70: .4byte gDuelLogoTiles1
_08029D74: .4byte gDuelLogoTiles2
_08029D78: .4byte 0x02020DEC
	thumb_func_end TurnOrder_CpuChooseTurn

