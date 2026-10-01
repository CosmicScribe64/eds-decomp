	thumb_func_start sub_080619E8
sub_080619E8: @ 0x080619E8
	push {lr}
	ldr r1, _08061A10 @ =0x0201CFB0
	mov r0, #3
	neg r0, r0
	ldrb r2, [r1]
	and r0, r2
	strb r0, [r1]
	mov r2, #0x80
	lsl r2, r2, #0x13
	ldrh r1, [r2]
	ldr r0, _08061A14 @ =0x0000FFBF
	and r0, r1
	strh r0, [r2]
	ldr r0, _08061A18 @ =0x06010000
	mov r1, #0x80
	lsl r1, r1, #9
	bl sub_08075278
	pop {r0}
	bx r0
_08061A10: .4byte 0x0201CFB0
_08061A14: .4byte 0x0000FFBF
_08061A18: .4byte 0x06010000
	thumb_func_end sub_080619E8

