	thumb_func_start sub_08077554
sub_08077554: @ 0x08077554
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r0, r6, #0
	bl sub_08077468
	ldr r7, _080775B4 @ =0x02011C20
	lsl r0, r6, #2
	add r5, r0, r7
	ldrb r0, [r5, #9]
	lsl r4, r0, #0x1A
	lsr r4, r4, #0x1E
	add r0, r6, #0
	bl sub_0807717C
	cmp r4, r0
	bge _080775AC
	ldr r2, _080775B8 @ =0x000020CA
	add r3, r7, r2
	ldrh r0, [r3]
	cmp r0, #0xE
	bhi _080775AC
	ldrb r2, [r5, #9]
	lsl r1, r2, #0x1A
	lsr r1, r1, #0x1E
	add r1, #1
	mov r0, #3
	and r1, r0
	lsl r1, r1, #4
	mov r0, #0x31
	neg r0, r0
	and r0, r2
	orr r0, r1
	strb r0, [r5, #9]
	ldrh r0, [r3]
	add r1, r0, #1
	strh r1, [r3]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	mov r2, #0x82
	lsl r2, r2, #6
	add r1, r7, r2
	add r0, r0, r1
	strh r6, [r0]
_080775AC:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_080775B4: .4byte 0x02011C20
_080775B8: .4byte 0x000020CA
	thumb_func_end sub_08077554

