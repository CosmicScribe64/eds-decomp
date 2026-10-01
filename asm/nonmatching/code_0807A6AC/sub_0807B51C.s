	thumb_func_start sub_0807B51C
sub_0807B51C: @ 0x0807B51C
	push {lr}
	add r1, r0, #0
	mov r0, #0x80
	lsl r0, r0, #9
	lsl r1, r1, #0x10
	asr r1, r1, #0x10
	bl Div
	lsl r0, r0, #0x10
	asr r0, r0, #0x10
	pop {r1}
	bx r1
	thumb_func_end sub_0807B51C

