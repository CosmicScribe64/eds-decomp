	thumb_func_start BattleScene_DrawAtk
BattleScene_DrawAtk: @ 0x0805DE34
	push {r4, r5, r6, lr}
	add r4, r1, #0
	add r5, r2, #0
	add r6, r3, #0
	lsl r1, r0, #4
	sub r1, r1, r0
	lsl r1, r1, #3
	add r4, r4, r1
	add r4, #0x47
	add r5, #0x7E
	lsl r0, r5, #0x10
	orr r0, r4
	mov r1, #0x80
	lsl r1, r1, #7
	ldr r2, _0805DE68 @ =0x0000202A
	bl AddSprite
	add r4, #4
	add r0, r4, #0
	add r1, r5, #0
	add r2, r6, #0
	bl BattleScene_DrawSmallNumber
	pop {r4, r5, r6}
	pop {r0}
	bx r0
_0805DE68: .4byte 0x0000202A
	thumb_func_end BattleScene_DrawAtk

