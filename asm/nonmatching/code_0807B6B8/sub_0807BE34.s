	thumb_func_start sub_0807BE34
sub_0807BE34: @ 0x0807BE34
	push {r4, lr}
	add r4, r0, #0
	ldr r0, _0807BE5C @ =0x03000000
	add r1, r0, #0
	add r1, #0x1C
	bl sub_080735D4
	mov r0, #0
	mov r1, #0
_0807BE46:
	strh r1, [r4, #2]
	add r4, #4
	add r0, #1
	lsl r0, r0, #0x18
	lsr r0, r0, #0x18
	cmp r0, #1
	bls _0807BE46
	mov r0, #1
	pop {r4}
	pop {r1}
	bx r1
_0807BE5C: .4byte 0x03000000
	thumb_func_end sub_0807BE34

