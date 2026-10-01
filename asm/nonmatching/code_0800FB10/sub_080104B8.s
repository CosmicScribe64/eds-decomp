	thumb_func_start sub_080104B8
sub_080104B8: @ 0x080104B8
	push {r4, lr}
	sub sp, #4
	ldr r4, _080104E4 @ =0x020185C0
	ldrh r1, [r4, #4]
	lsl r0, r1, #0x10
	ldrh r1, [r4, #2]
	orr r0, r1
	str r0, [sp, #0]
	mov r0, sp
	bl sub_080096F4
	ldr r0, _080104E8 @ =0x0000080D
	add r4, r4, r0
	mov r0, #0x21
	neg r0, r0
	ldrb r1, [r4]
	and r0, r1
	strb r0, [r4]
	add sp, #4
	pop {r4}
	pop {r0}
	bx r0
_080104E4: .4byte 0x020185C0
_080104E8: .4byte 0x0000080D
	thumb_func_end sub_080104B8

