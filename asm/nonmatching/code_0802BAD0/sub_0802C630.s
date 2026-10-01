	thumb_func_start sub_0802C630
sub_0802C630: @ 0x0802C630
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r3, r0, #0x18
	lsr r2, r1, #0x18
	sub r0, r2, #5
	cmp r0, #4
	bhi _0802C670
	mov r1, #1
	and r1, r3
	mov r0, #0x94
	mul r2, r0
	ldr r0, _0802C668 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802C66C @ =0x0201930C
	add r1, r2, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802C670
	mov r0, #2
	ldrb r1, [r1, #6]
	and r0, r1
	cmp r0, #0
	bne _0802C670
	mov r0, #1
	b _0802C672
	.align 2, 0
_0802C668: .4byte 0x00000D64
_0802C66C: .4byte 0x0201930C
_0802C670:
	mov r0, #0
_0802C672:
	bx lr
	thumb_func_end sub_0802C630

