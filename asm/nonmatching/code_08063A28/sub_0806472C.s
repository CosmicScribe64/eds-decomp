	thumb_func_start sub_0806472C
sub_0806472C: @ 0x0806472C
	sub sp, #4
	mov r1, sp
	mov r0, #0
	strh r0, [r1]
	ldr r1, _0806475C @ =0x040000D4
	mov r0, sp
	str r0, [r1]
	ldr r0, _08064760 @ =0x02020310
	str r0, [r1, #4]
	ldr r0, _08064764 @ =0x81004038
	str r0, [r1, #8]
	ldr r0, [r1, #8]
	ldr r0, [r1, #8]
	mov r2, #0x80
	lsl r2, r2, #0x18
	cmp r0, #0
	bge _08064756
_0806474E:
	ldr r0, [r1, #8]
	and r0, r2
	cmp r0, #0
	bne _0806474E
_08064756:
	add sp, #4
	bx lr
	.align 2, 0
_0806475C: .4byte 0x040000D4
_08064760: .4byte 0x02020310
_08064764: .4byte 0x81004038
	thumb_func_end sub_0806472C

