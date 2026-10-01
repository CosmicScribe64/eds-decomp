	thumb_func_start sub_080643E4
sub_080643E4: @ 0x080643E4
	sub sp, #4
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r1, _08064414 @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r0, _08064418 @ =0x02020310
	str r0, [r1, #4]
	ldr r0, _0806441C @ =0x81004038
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _0806440E
_08064406:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _08064406
_0806440E:
	add sp, #4
	bx lr
	.align 2, 0
_08064414: .4byte 0x040000D4
_08064418: .4byte 0x02020310
_0806441C: .4byte 0x81004038
	thumb_func_end sub_080643E4

