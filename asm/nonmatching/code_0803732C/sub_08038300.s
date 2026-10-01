	thumb_func_start sub_08038300
sub_08038300: @ 0x08038300
	push {lr}
	add r1, r0, #0
	mov r0, #4
	ldrb r2, [r1, #4]
	and r0, r2
	cmp r0, #0
	bne _0803831A
	ldrb r2, [r1, #2]
	lsl r0, r2, #0x1F
	lsr r0, r0, #0x1F
	ldrh r1, [r1, #6]
	bl sub_08019980
_0803831A:
	mov r0, #0
	pop {r1}
	bx r1
	thumb_func_end sub_08038300

