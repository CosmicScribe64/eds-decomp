	thumb_func_start sub_0807B028
sub_0807B028: @ 0x0807B028
	push {r4, lr}
	add r4, r0, #0
	ldrb r2, [r1]
	lsl r0, r2, #2
	add r3, r1, #4
	add r0, r3, r0
	ldr r0, [r0]
	cmp r0, #0
	beq _0807B03E
	mov r0, #0
	b _0807B052
_0807B03E:
	add r0, r2, #1
	strb r0, [r1]
	mov r1, #0xFF
	and r0, r1
	mov r1, #3
	and r0, r1
	lsl r0, r0, #2
	add r0, r3, r0
	str r4, [r0]
	add r0, r2, #0
_0807B052:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0807B028

