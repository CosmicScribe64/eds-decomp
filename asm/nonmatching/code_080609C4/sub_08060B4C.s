	thumb_func_start sub_08060B4C
sub_08060B4C: @ 0x08060B4C
	push {lr}
	mov r0, #4
	bl sub_08075A6C
	lsl r0, r0, #0x10
	cmp r0, #0
	bne _08060B5E
	mov r0, #0
	b _08060B66
_08060B5E:
	mov r0, #0
	bl sub_08060AAC
	mov r0, #1
_08060B66:
	pop {r1}
	bx r1
	thumb_func_end sub_08060B4C
	.align 2, 0

