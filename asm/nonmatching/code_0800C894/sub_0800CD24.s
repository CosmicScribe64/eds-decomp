	thumb_func_start sub_0800CD24
sub_0800CD24: @ 0x0800CD24
	push {r4, r5, r6, r7, lr}
	mov r7, r9
	mov r6, r8
	push {r6, r7}
	mov r9, r0
	mov r8, r1
	mov r6, #0
	mov r5, #0
_0800CD34:
	mov r4, #0
	add r7, r5, #1
_0800CD38:
	mov r0, r9
	mov r1, r8
	add r2, r5, #0
	add r3, r4, #0
	bl sub_0800CCCC
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0800CD4C
	add r6, #1
_0800CD4C:
	add r4, #1
	cmp r4, #4
	ble _0800CD38
	add r5, r7, #0
	cmp r5, #1
	ble _0800CD34
	add r0, r6, #0
	pop {r3, r4}
	mov r8, r3
	mov r9, r4
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0800CD24
	.align 2, 0

