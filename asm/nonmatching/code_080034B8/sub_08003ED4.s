	thumb_func_start sub_08003ED4
sub_08003ED4: @ 0x08003ED4
	ldr r1, _08003EE4 @ =0x0201F814
	mov r0, #0xE0
	ldrb r1, [r1, #1]
	and r0, r1
	cmp r0, #0
	bne _08003EE8
	mov r0, #0
	b _08003EEA
_08003EE4: .4byte 0x0201F814
_08003EE8:
	mov r0, #1
_08003EEA:
	bx lr
	thumb_func_end sub_08003ED4

