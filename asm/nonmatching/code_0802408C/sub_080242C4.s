	thumb_func_start sub_080242C4
sub_080242C4: @ 0x080242C4
	push {r4, r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	lsl r0, r0, #0x10
	lsr r0, r0, #0x10
	ldr r5, _0802431C @ =0x0201CFB0
	mov r3, #0x83
	lsl r3, r3, #4
	add r6, r5, r3
	mov r7, #1
	mov r8, r7
	mov r4, #1
	ldrb r3, [r6]
	and r4, r3
	mov r3, #6
	orr r4, r3
	strb r4, [r6]
	ldr r7, _08024320 @ =0x00000834
	add r3, r5, r7
	str r0, [r3]
	mov r0, #0x84
	lsl r0, r0, #4
	add r3, r5, r0
	ldr r0, [r1]
	str r0, [r3]
	ldr r3, _08024324 @ =0x00000844
	add r1, r5, r3
	ldr r0, [r2]
	str r0, [r1]
	add r7, #4
	add r0, r5, r7
	mov r1, #0
	strb r1, [r0]
	ldr r0, _08024328 @ =0x00000839
	add r5, r5, r0
	strb r1, [r5]
	mov r1, r8
	orr r4, r1
	strb r4, [r6]
	pop {r3}
	mov r8, r3
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
_0802431C: .4byte 0x0201CFB0
_08024320: .4byte 0x00000834
_08024324: .4byte 0x00000844
_08024328: .4byte 0x00000839
	thumb_func_end sub_080242C4

