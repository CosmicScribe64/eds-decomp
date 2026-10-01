	thumb_func_start sub_080769DC
sub_080769DC: @ 0x080769DC
	push {r4, lr}
	add r3, r0, #0
	ldr r0, [r3]
	ldrh r2, [r0, #0x20]
	add r0, #0x22
	strh r2, [r3, #0xC]
	lsl r1, r2, #2
	add r0, r0, r1
	str r0, [r3, #4]
	mov r4, #0
	cmp r4, r2
	bcs _08076A0A
	add r1, r0, #0
_080769F6:
	ldrh r0, [r1]
	add r1, #2
	lsl r0, r0, #5
	add r1, r1, r0
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	cmp r4, r2
	bcc _080769F6
	str r1, [r3, #4]
_08076A0A:
	ldr r0, [r3, #4]
	ldrh r1, [r0]
	add r0, #2
	str r0, [r3, #4]
	mov r0, #0
	strh r1, [r3, #8]
	strh r0, [r3, #0xA]
	pop {r4}
	pop {r0}
	bx r0
	thumb_func_end sub_080769DC
	.align 2, 0

