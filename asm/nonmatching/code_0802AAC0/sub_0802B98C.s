	thumb_func_start sub_0802B98C
sub_0802B98C: @ 0x0802B98C
	push {r4, r5, r6, r7, lr}
	add r6, r0, #0
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r4, r0, #0x18
	lsr r3, r1, #0x18
	cmp r3, #4
	bgt _0802B9E4
	ldrb r1, [r6, #2]
	lsl r0, r1, #0x1F
	lsr r0, r0, #0x1F
	cmp r4, r0
	beq _0802B9E4
	mov r7, #1
	add r1, r4, #0
	and r1, r7
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802B9DC @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802B9E0 @ =0x0201930C
	add r5, r2, r0
	ldr r0, [r5]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802B9E4
	ldrh r0, [r6]
	add r1, r4, #0
	add r2, r3, #0
	bl sub_0802B1B8
	lsl r0, r0, #0x10
	cmp r0, #0
	beq _0802B9E4
	add r0, r7, #0
	ldrb r5, [r5, #6]
	and r0, r5
	b _0802B9E6
_0802B9DC: .4byte 0x00000D64
_0802B9E0: .4byte 0x0201930C
_0802B9E4:
	mov r0, #0
_0802B9E6:
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	thumb_func_end sub_0802B98C

