	thumb_func_start sub_080080B4
sub_080080B4: @ 0x080080B4
	push {r4, lr}
	sub sp, #4
	add r4, r0, #0
	mov r1, #0
	mov r2, sp
	bl sub_08007ED0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _080080D0
	add r0, r4, #0
	mov r1, sp
	bl sub_08009EAC
_080080D0:
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_080080B4

