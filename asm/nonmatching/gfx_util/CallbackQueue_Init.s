	thumb_func_start CallbackQueue_Init
CallbackQueue_Init: @ 0x0807B010
	mov r1, #0
	add r2, r0, #4
	mov r3, #0
_0807B016:
	lsl r0, r1, #2
	add r0, r2, r0
	str r3, [r0]
	add r0, r1, #1
	lsl r0, r0, #0x18
	lsr r1, r0, #0x18
	cmp r1, #3
	bls _0807B016
	bx lr
	thumb_func_end CallbackQueue_Init

