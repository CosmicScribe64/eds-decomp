	thumb_func_start Title_FadeOut
Title_FadeOut: @ 0x0800545C
	push {lr}
	bl Title_DrawMenu
	mov r0, #4
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08005472
	mov r0, #0
	b _0800547E
_08005472:
	ldr r0, _08005484 @ =0x03000040
	ldr r1, _08005488 @ =0x00000414
	add r0, r0, r1
	mov r1, #0
	str r1, [r0]
	mov r0, #1
_0800547E:
	pop {r1}
	bx r1
	.align 2, 0
_08005484: .4byte 0x03000040
_08005488: .4byte 0x00000414
	thumb_func_end Title_FadeOut

