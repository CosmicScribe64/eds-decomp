	thumb_func_start StopBGM
StopBGM: @ 0x08077BA0
	push {lr}
	bl IsBgmEnabled
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08077BB0
	bl SoundStopBGM
_08077BB0:
	ldr r0, _08077BC0 @ =0x03000040
	ldr r1, _08077BC4 @ =0x0000485C
	add r0, r0, r1
	ldr r1, _08077BC8 @ =0x0000FFFF
	strh r1, [r0]
	pop {r0}
	bx r0
	.align 2, 0
_08077BC0: .4byte 0x03000040
_08077BC4: .4byte 0x0000485C
_08077BC8: .4byte 0x0000FFFF
	thumb_func_end StopBGM

