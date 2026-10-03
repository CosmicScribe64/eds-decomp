	thumb_func_start StopAllSound
StopAllSound: @ 0x08077C18
	push {lr}
	bl IsBgmEnabled
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077C28
	bl SoundStopBGM
_08077C28:
	bl IsSeEnabled
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077C36
	bl SoundStopAllSE
_08077C36:
	ldr r0, _08077C44 @ =0x03000040
	ldr r1, _08077C48 @ =0x0000485C
	add r0, r0, r1
	ldr r1, _08077C4C @ =0x0000FFFF
	strh r1, [r0]
	pop {r0}
	bx r0
_08077C44: .4byte 0x03000040
_08077C48: .4byte 0x0000485C
_08077C4C: .4byte 0x0000FFFF
	thumb_func_end StopAllSound

