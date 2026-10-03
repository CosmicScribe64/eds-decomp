	thumb_func_start BanishBattleDestroyedCard
BanishBattleDestroyedCard: @ 0x08018664
	push {r4, r5, lr}
	add r4, r0, #0
	add r5, r1, #0
	mov r0, #0x7D
	cmp r4, #0
	beq _08018672
	ldr r0, _0801868C @ =0x0000807D
_08018672:
	ldrh r1, [r2]
	ldrh r2, [r2, #2]
	mov r3, #0
	bl DuelCmd_Push
	add r0, r4, #0
	add r1, r5, #0
	mov r2, #1
	bl DestroyLinkedCards
	pop {r4, r5}
	pop {r0}
	bx r0
_0801868C: .4byte 0x0000807D
	thumb_func_end BanishBattleDestroyedCard

