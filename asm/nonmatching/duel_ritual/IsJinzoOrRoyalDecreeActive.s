	thumb_func_start IsJinzoOrRoyalDecreeActive
IsJinzoOrRoyalDecreeActive: @ 0x080431E4
	push {r4, lr}
	ldr r4, _08043220 @ =0x000002EF
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08043228
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08043228
	ldr r4, _08043224 @ =0x00000409
	mov r0, #0
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08043228
	mov r0, #1
	add r1, r4, #0
	bl CountActiveCardsOnField
	cmp r0, #0
	bne _08043228
	mov r0, #0
	b _0804322A
	.align 2, 0
_08043220: .4byte 0x000002EF
_08043224: .4byte 0x00000409
_08043228:
	mov r0, #1
_0804322A:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end IsJinzoOrRoyalDecreeActive

