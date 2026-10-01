	thumb_func_start sub_0800A3F4
sub_0800A3F4: @ 0x0800A3F4
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	add r7, r1, #0
	ldr r4, _0800A424 @ =0x0000049A
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	add r5, r0, #0
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	add r5, r5, r0
	cmp r5, #0
	ble _0800A428
	add r0, r6, #0
	add r1, r7, #0
	bl sub_08009298
	cmp r0, #0
	ble _0800A428
	add r0, r5, #0
	b _0800A42A
_0800A424: .4byte 0x0000049A
_0800A428:
	mov r0, #0
_0800A42A:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0800A3F4

