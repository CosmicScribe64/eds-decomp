	thumb_func_start sub_0807B090
sub_0807B090: @ 0x0807B090
	push {r4, lr}
	add r4, r0, #0
	ldr r1, [r4, #4]
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _0807B0B6
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0807B0B2
	ldrb r0, [r4]
	add r0, #1
	strb r0, [r4]
_0807B0B2:
	mov r0, #0
	b _0807B0B8
_0807B0B6:
	mov r0, #1
_0807B0B8:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0807B090
	.align 2, 0

