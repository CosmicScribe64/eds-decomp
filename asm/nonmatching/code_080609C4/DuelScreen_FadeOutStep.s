	thumb_func_start DuelScreen_FadeOutStep
DuelScreen_FadeOutStep: @ 0x08060B4C
	push {lr}
	mov r0, #4
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08060B5E
	mov r0, #0
	b _08060B66
_08060B5E:
	mov r0, #0
	bl DuelScreen_Exit
	mov r0, #1
_08060B66:
	pop {r1}
	bx r1
	thumb_func_end DuelScreen_FadeOutStep
	.align 2, 0

