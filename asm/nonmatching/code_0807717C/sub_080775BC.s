	thumb_func_start sub_080775BC
sub_080775BC: @ 0x080775BC
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r0, r6, #0
	bl sub_08077468
	ldr r7, _08077610 @ =0x02011C20
	lsl r0, r6, #2
	add r5, r0, r7
	ldrb r0, [r5, #9]
	lsr r4, r0, #6
	add r0, r6, #0
	bl sub_0807717C
	cmp r4, r0
	bge _08077608
	ldr r2, _08077614 @ =0x000020CC
	add r3, r7, r2
	ldrh r0, [r3]
	cmp r0, #0x13
	bhi _08077608
	ldrb r2, [r5, #9]
	lsr r1, r2, #6
	add r1, #1
	lsl r1, r1, #6
	mov r0, #0x3F
	and r0, r2
	orr r0, r1
	strb r0, [r5, #9]
	ldrh r0, [r3]
	add r1, r0, #1
	strh r1, [r3]
	lsl r0, r0, #0x10
	lsr r0, r0, #0xF
	ldr r2, _08077618 @ =0x0000209E
	add r1, r7, r2
	add r0, r0, r1
	strh r6, [r0]
_08077608:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08077610: .4byte 0x02011C20
_08077614: .4byte 0x000020CC
_08077618: .4byte 0x0000209E
	thumb_func_end sub_080775BC

