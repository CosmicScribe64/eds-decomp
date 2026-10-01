	thumb_func_start sub_08043230
sub_08043230: @ 0x08043230
	push {r4, lr}
	ldr r4, _08043250 @ =0x00000482
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _08043254
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bne _08043254
	mov r0, #0
	b _08043256
_08043250: .4byte 0x00000482
_08043254:
	mov r0, #1
_08043256:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_08043230

