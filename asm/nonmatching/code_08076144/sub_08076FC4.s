	thumb_func_start sub_08076FC4
sub_08076FC4: @ 0x08076FC4
	push {r4, r5, lr}
	add r5, r0, #0
	add r4, r1, #0
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r3, #0
	cmp r3, r2
	bcs _08076FEE
_08076FD4:
	ldrb r1, [r5]
	ldrb r0, [r4]
	add r4, #1
	add r5, #1
	cmp r1, r0
	beq _08076FE4
	mov r0, #1
	b _08076FF0
_08076FE4:
	add r0, r3, #1
	lsl r0, r0, #0x18
	lsr r3, r0, #0x18
	cmp r3, r2
	bcc _08076FD4
_08076FEE:
	mov r0, #0
_08076FF0:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08076FC4
	.align 2, 0

