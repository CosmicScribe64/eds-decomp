	thumb_func_start DuelPrompt_PostRandomBanishFaceDown
DuelPrompt_PostRandomBanishFaceDown: @ 0x08022814
	push {lr}
	mov r1, #5
	mov r2, #0
	mov r3, #1
	bl DuelPrompt_Post
	pop {r0}
	bx r0
	thumb_func_end DuelPrompt_PostRandomBanishFaceDown

