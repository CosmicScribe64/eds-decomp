	thumb_func_start sub_08002FD0
sub_08002FD0: @ 0x08002FD0
	push {r4, r5, lr}
	ldr r1, _08003008 @ =0x08198380
	ldr r5, _0800300C @ =0x03000040
	ldr r0, _08003010 @ =0x00004859
	add r4, r5, r0
	ldrb r2, [r4]
	lsl r0, r2, #2
	add r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	beq _08003018
	bl _call_via_r0
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _08003004
	ldrb r0, [r4]
	add r0, #1
	mov r1, #0
	strb r0, [r4]
	ldr r2, _08003014 @ =0x0000485A
	add r0, r5, r2
	strb r1, [r0]
	add r2, #1
	add r0, r5, r2
	strb r1, [r0]
_08003004:
	mov r0, #0
	b _0800301A
_08003008: .4byte gUnk_08198380
_0800300C: .4byte 0x03000040
_08003010: .4byte 0x00004859
_08003014: .4byte 0x0000485A
_08003018:
	mov r0, #1
_0800301A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_08002FD0

