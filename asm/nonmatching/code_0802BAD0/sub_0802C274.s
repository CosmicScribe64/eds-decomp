	thumb_func_start sub_0802C274
sub_0802C274: @ 0x0802C274
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r3, r0, #0x18
	lsr r1, r1, #0x18
	sub r0, r1, #5
	cmp r0, #5
	bhi _0802C2A8
	mov r2, #1
	and r2, r3
	mov r0, #0x94
	mul r0, r1
	ldr r1, _0802C2A0 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802C2A4 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802C2A8
	mov r0, #1
	b _0802C2AA
_0802C2A0: .4byte 0x00000D64
_0802C2A4: .4byte 0x0201930C
_0802C2A8:
	mov r0, #0
_0802C2AA:
	bx lr
	thumb_func_end sub_0802C274

