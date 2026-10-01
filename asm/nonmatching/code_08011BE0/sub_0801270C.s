	thumb_func_start sub_0801270C
sub_0801270C: @ 0x0801270C
	ldr r1, _08012720 @ =0x020185C0
	ldr r0, _08012724 @ =0x0000080D
	add r1, r1, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	bx lr
	.align 2, 0
_08012720: .4byte 0x020185C0
_08012724: .4byte 0x0000080D
	thumb_func_end sub_0801270C

