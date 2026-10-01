	thumb_func_start sub_0807B114
sub_0807B114: @ 0x0807B114
	push {r4, lr}
	add r2, r0, #0
	ldrb r0, [r2]
	cmp r0, #1
	bne _0807B148
	ldrh r0, [r2, #6]
	ldrh r3, [r2, #2]
	add r1, r0, r3
	strh r1, [r2, #2]
	lsl r0, r0, #0x10
	cmp r0, #0
	ble _0807B138
	lsl r1, r1, #0x10
	ldrh r3, [r2, #4]
	lsl r0, r3, #0x10
	cmp r1, r0
	blt _0807B148
	b _0807B142
_0807B138:
	lsl r1, r1, #0x10
	ldrh r3, [r2, #4]
	lsl r0, r3, #0x10
	cmp r1, r0
	bgt _0807B148
_0807B142:
	mov r0, #2
	strb r0, [r2]
	strh r3, [r2, #2]
_0807B148:
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_0807B114
	.align 2, 0

