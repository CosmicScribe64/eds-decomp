	thumb_func_start sub_080229BC
sub_080229BC: @ 0x080229BC
	push {r4, lr}
	sub sp, #0x100
	add r3, r1, #0
	add r4, r2, #0
	mov r1, sp
	strh r0, [r1]
	cmp r4, #0
	ble _080229D6
	mov r0, sp
	add r0, #2
	add r1, r3, #0
	bl sub_08075294
_080229D6:
	add r1, r4, #2
	mov r0, sp
	bl sub_080723B4
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	add sp, #0x100
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_080229BC
	.align 2, 0

