	thumb_func_start sub_0807255C
sub_0807255C: @ 0x0807255C
	push {lr}
	bl sub_0807373C
	ldr r0, _0807257C @ =0x03000040
	mov r1, #0x83
	lsl r1, r1, #3
	add r0, r0, r1
	mov r1, #0
	str r1, [r0]
	ldr r0, _08072580 @ =0x030049D0
	mov r1, #0x84
	lsl r1, r1, #4
	bl sub_08075278
	pop {r0}
	bx r0
_0807257C: .4byte 0x03000040
_08072580: .4byte 0x030049D0
	thumb_func_end sub_0807255C

