	thumb_func_start sub_08001BEC
sub_08001BEC: @ 0x08001BEC
	add r2, r0, #0
	mov r0, #0xF5
	lsl r0, r0, #1
	cmp r2, r0
	bls _08001BFA
	mov r0, #0
	b _08001C0A
_08001BFA:
	ldr r0, _08001C0C @ =0x0813ADF4
	lsl r1, r2, #1
	add r1, r1, r2
	lsl r1, r1, #6
	add r1, r1, r2
	lsl r1, r1, #2
	add r1, r1, r0
	ldrh r0, [r1, #2]
_08001C0A:
	bx lr
_08001C0C: .4byte gUnk_0813ADF4
	thumb_func_end sub_08001BEC

