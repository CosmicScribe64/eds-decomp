	thumb_func_start sub_08072BB4
sub_08072BB4: @ 0x08072BB4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	add r5, r3, #0
	lsl r0, r0, #0x10
	lsl r1, r1, #0x10
	lsl r2, r2, #0x10
	lsr r3, r2, #0x10
	ldr r6, _08072BD4 @ =0x0300045C
	lsl r2, r1, #8
	lsr r2, r2, #0x18
	mov r8, r2
	lsr r7, r1, #0x18
	lsr r0, r0, #0xF
	add r6, r0, r6
	b _08072BF6
_08072BD4: .4byte 0x0300045C
_08072BD8:
	ldrb r0, [r5]
	add r4, r3, #0
	lsl r1, r4, #5
	ldr r2, _08072C08 @ =0x06004000
	add r1, r1, r2
	mov r2, r8
	add r3, r7, #0
	bl sub_08072778
	add r0, r4, #1
	lsl r0, r0, #0x10
	lsr r3, r0, #0x10
	strh r4, [r6]
	add r6, #2
	add r5, #1
_08072BF6:
	ldrb r0, [r5]
	cmp r0, #0
	bne _08072BD8
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
_08072C08: .4byte 0x06004000
	thumb_func_end sub_08072BB4

