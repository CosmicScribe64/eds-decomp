	thumb_func_start DuelPrompt_PostTribute
DuelPrompt_PostTribute: @ 0x08022824
	push {lr}
	mov r1, #7
	mov r2, #0
	mov r3, #0
	bl DuelPrompt_Post
	pop {r0}
	bx r0
	thumb_func_end DuelPrompt_PostTribute

