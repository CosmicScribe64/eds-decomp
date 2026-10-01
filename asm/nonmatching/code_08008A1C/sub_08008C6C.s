	thumb_func_start sub_08008C6C
sub_08008C6C: @ 0x08008C6C
	push {r4, r5, lr}
	add r5, r0, #0
	mov r4, #5
_08008C72:
	add r0, r5, #0
	add r1, r4, #0
	bl sub_08008C24
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08008C84
	add r0, r4, #0
	b _08008C8E
_08008C84:
	add r4, #1
	cmp r4, #9
	ble _08008C72
	mov r0, #1
	neg r0, r0
_08008C8E:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08008C6C

