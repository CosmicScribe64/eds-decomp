	thumb_func_start sub_08075278
sub_08075278: @ 0x08075278
	add r2, r0, #0
	add r0, r1, #1
	lsr r1, r0, #0x1F
	add r0, r0, r1
	asr r1, r0, #1
	cmp r1, #0
	beq _08075292
	mov r0, #0
_08075288:
	strh r0, [r2]
	add r2, #2
	sub r1, #1
	cmp r1, #0
	bne _08075288
_08075292:
	bx lr
	thumb_func_end sub_08075278

