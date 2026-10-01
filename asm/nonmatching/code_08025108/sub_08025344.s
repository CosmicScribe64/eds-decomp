	thumb_func_start sub_08025344
sub_08025344: @ 0x08025344
	push {r4, r5, lr}
	add r3, r0, #0
	mov r2, #0
	mov r4, #2
	neg r4, r4
_0802534E:
	lsl r0, r2, #3
	add r0, r3, r0
	add r1, r4, #0
	ldrb r5, [r0]
	and r1, r5
	strb r1, [r0]
	add r0, r2, #1
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	cmp r2, #0x1F
	bls _0802534E
	mov r0, #0x80
	lsl r0, r0, #1
	add r1, r3, r0
	mov r0, #0
	strb r0, [r1]
	pop {r4, r5}
	pop {r0}
	bx r0
	thumb_func_end sub_08025344

