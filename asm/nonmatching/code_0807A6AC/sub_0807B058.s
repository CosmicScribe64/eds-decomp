	thumb_func_start sub_0807B058
sub_0807B058: @ 0x0807B058
	push {r4, r5, r6, lr}
	add r6, r0, #0
	mov r5, #0
_0807B05E:
	lsl r1, r5, #2
	add r0, r6, #4
	add r4, r0, r1
	ldr r0, [r4]
	cmp r0, #0
	beq _0807B078
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807B078
	mov r0, #0
	str r0, [r4]
_0807B078:
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, #3
	bls _0807B05E
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	thumb_func_end sub_0807B058

