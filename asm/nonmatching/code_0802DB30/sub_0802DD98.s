	thumb_func_start sub_0802DD98
sub_0802DD98: @ 0x0802DD98
	push {r4, lr}
	ldrb r4, [r0, #6]
	ldrh r1, [r0, #6]
	lsr r3, r1, #8
	ldrb r0, [r0, #3]
	lsr r0, r0, #2
	cmp r0, #7
	bgt _0802DDD2
	cmp r0, #5
	blt _0802DDD2
	mov r1, #1
	and r1, r4
	mov r0, #0x94
	add r2, r3, #0
	mul r2, r0
	ldr r0, _0802DDD8 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802DDDC @ =0x0201930C
	add r2, r2, r0
	ldr r0, [r2]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802DDD2
	mov r0, #2
	ldrb r2, [r2, #6]
	and r0, r2
	cmp r0, #0
	bne _0802DDE0
_0802DDD2:
	mov r0, #0
	b _0802DDF6
	.align 2, 0
_0802DDD8: .4byte 0x00000D64
_0802DDDC: .4byte 0x0201930C
_0802DDE0:
	add r0, r4, #0
	add r1, r3, #0
	bl sub_0800C894
	mov r2, #0
	mov r1, #0xFA
	lsl r1, r1, #3
	cmp r0, r1
	bgt _0802DDF4
	mov r2, #1
_0802DDF4:
	add r0, r2, #0
_0802DDF6:
	pop {r4}
	pop {r1}
	bx r1
	thumb_func_end sub_0802DD98

