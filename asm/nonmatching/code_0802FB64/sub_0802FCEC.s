	thumb_func_start sub_0802FCEC
sub_0802FCEC: @ 0x0802FCEC
	push {r4, lr}
	mov r0, #0
	mov r1, #0
	mov r2, #0
	mov r3, #1
	bl sub_08008B70
	add r4, r0, #0
	mov r0, #1
	mov r1, #0
	mov r2, #0
	mov r3, #1
	bl sub_08008B70
	add r4, r4, r0
	mov r0, #0
	cmp r4, #1
	ble _0802FD12
	mov r0, #1
_0802FD12:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802FCEC

