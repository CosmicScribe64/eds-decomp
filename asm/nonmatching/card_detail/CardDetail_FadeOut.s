	thumb_func_start CardDetail_FadeOut
CardDetail_FadeOut: @ 0x08006ABC
	push {lr}
	bl CardDetail_DrawSprites
	mov r0, #4
	bl FadeToBlack
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08006AD2
	mov r0, #0
	b _08006AE0
_08006AD2:
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08006AE4 @ =0x0000E0FF
	and r0, r1
	strh r0, [r2]
	mov r0, #1
_08006AE0:
	pop {r1}
	bx r1
_08006AE4: .4byte 0x0000E0FF
	thumb_func_end CardDetail_FadeOut

