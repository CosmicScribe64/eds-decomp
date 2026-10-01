	thumb_func_start sub_08005500
sub_08005500: @ 0x08005500
	push {r4, r5, r6, r7, lr}
	mov r0, #0
_08005504:
	mov r5, #0
	lsl r6, r0, #0x15
	add r7, r0, #1
	lsl r0, r0, #0x17
	mov r1, #0x80
	lsl r1, r1, #0x12
	add r4, r0, r1
_08005512:
	lsl r0, r5, #6
	orr r0, r6
	lsr r2, r4, #0x10
	ldr r1, _08005538 @ =0x000040C0
	bl sub_080761F0
	mov r0, #0x80
	lsl r0, r0, #0xC
	add r4, r4, r0
	add r5, #1
	cmp r5, #3
	ble _08005512
	add r0, r7, #0
	cmp r0, #4
	ble _08005504
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08005538: .4byte 0x000040C0
	thumb_func_end sub_08005500

