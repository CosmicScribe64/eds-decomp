	thumb_func_start sub_0805761C
sub_0805761C: @ 0x0805761C
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r5, #0
	mov r4, #0
_08057624:
	add r0, r6, #0
	add r1, r4, #0
	bl sub_0800C894
	add r5, r5, r0
	add r4, #1
	cmp r4, #4
	ble _08057624
	add r0, r5, #0
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0805761C

