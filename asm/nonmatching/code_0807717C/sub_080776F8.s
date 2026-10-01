	thumb_func_start sub_080776F8
sub_080776F8: @ 0x080776F8
	push {r4, r5, r6, r7, lr}
	lsl r0, r0, #0x10
	lsr r4, r0, #0x10
	add r0, r4, #0
	bl sub_08077468
	ldr r3, _0807776C @ =0x02011C20
	lsl r0, r4, #2
	add r5, r0, r3
	ldrb r2, [r5, #9]
	lsl r1, r2, #0x1A
	lsr r0, r1, #0x1E
	cmp r0, #0
	beq _0807777E
	sub r0, #1
	mov r1, #3
	and r0, r1
	lsl r0, r0, #4
	mov r1, #0x31
	neg r1, r1
	and r1, r2
	orr r1, r0
	strb r1, [r5, #9]
	mov r2, #0
	ldr r1, _08077770 @ =0x000020CA
	add r0, r3, r1
	ldrh r1, [r0]
	cmp r2, r1
	bge _0807777E
	mov r1, #0x82
	lsl r1, r1, #6
	add r5, r3, r1
	add r1, r0, #0
	add r6, r1, #0
	add r7, r3, #0
	mov r3, #0
_08077740:
	add r0, r3, r5
	ldrh r0, [r0]
	cmp r0, r4
	bne _08077774
	ldrh r0, [r1]
	sub r0, #1
	strh r0, [r1]
	cmp r2, r0
	bge _0807777E
	add r4, r6, #0
	mov r1, #0x82
	lsl r1, r1, #6
	add r0, r3, r1
	add r1, r0, r7
_0807775C:
	ldrh r0, [r1, #2]
	strh r0, [r1]
	add r1, #2
	add r2, #1
	ldrh r0, [r4]
	cmp r2, r0
	blt _0807775C
	b _0807777E
_0807776C: .4byte 0x02011C20
_08077770: .4byte 0x000020CA
_08077774:
	add r3, #2
	add r2, #1
	ldrh r0, [r1]
	cmp r2, r0
	blt _08077740
_0807777E:
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end sub_080776F8

