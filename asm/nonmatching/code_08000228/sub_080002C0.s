	thumb_func_start sub_080002C0
sub_080002C0: @ 0x080002C0
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	mov r8, r0
	lsl r1, r1, #0x10
	lsr r7, r1, #0x10
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	lsl r3, r3, #0x10
	mov r4, #0
	lsr r3, r3, #0xF
	add r3, #2
	cmp r4, r3
	bge _08000312
	ldr r5, _0800031C @ =0x80004000
	orr r5, r2
	add r6, r3, #0
_080002E6:
	mov r0, #0
	ldr r1, _08000320 @ =0x02014888
	bl sub_0807A320
	str r5, [r0]
	lsl r1, r4, #5
	add r1, r7, r1
	ldrh r2, [r0, #2]
	orr r1, r2
	strh r1, [r0, #2]
	lsl r1, r4, #2
	add r1, r8
	mov r3, #0x88
	lsl r3, r3, #6
	add r2, r3, #0
	orr r1, r2
	strh r1, [r0, #4]
	add r0, r4, #1
	lsl r0, r0, #0x18
	lsr r4, r0, #0x18
	cmp r4, r6
	blt _080002E6
_08000312:
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0800031C: .4byte 0x80004000
_08000320: .4byte 0x02014888
	thumb_func_end sub_080002C0

