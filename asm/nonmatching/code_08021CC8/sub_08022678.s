	thumb_func_start sub_08022678
sub_08022678: @ 0x08022678
	push {r4, r5, r6, r7, lr}
	ldr r6, _080226B8 @ =0x020192E0
	ldr r4, _080226BC @ =0x00001B50
	add r5, r6, r4
	mov r4, #1
	and r0, r4
	lsl r0, r0, #2
	mov r4, #5
	neg r4, r4
	ldrb r7, [r5]
	and r4, r7
	orr r4, r0
	strb r4, [r5]
	mov r0, #0x3F
	and r1, r0
	lsl r1, r1, #4
	ldr r0, _080226C0 @ =0xFFFFFC0F
	ldrh r4, [r5]
	and r0, r4
	orr r0, r1
	strh r0, [r5]
	ldr r7, _080226C4 @ =0x00001B52
	add r0, r6, r7
	strh r2, [r0]
	ldr r0, _080226C8 @ =0x00001B54
	add r6, r6, r0
	strh r3, [r6]
	bl sub_080225D8
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_080226B8: .4byte 0x020192E0
_080226BC: .4byte 0x00001B50
_080226C0: .4byte 0xFFFFFC0F
_080226C4: .4byte 0x00001B52
_080226C8: .4byte 0x00001B54
	thumb_func_end sub_08022678

