	thumb_func_start FadeOutBGM
FadeOutBGM: @ 0x08077BCC
	push {lr}
	bl IsBgmEnabled
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077BDE
	mov r0, #0x10
	bl SoundFadeOutBGM
_08077BDE:
	ldr r0, _08077BEC @ =0x03000040
	ldr r1, _08077BF0 @ =0x0000485C
	add r0, r0, r1
	ldr r1, _08077BF4 @ =0x0000FFFF
	strh r1, [r0]
	pop {r0}
	bx r0
_08077BEC: .4byte 0x03000040
_08077BF0: .4byte 0x0000485C
_08077BF4: .4byte 0x0000FFFF
	thumb_func_end FadeOutBGM

