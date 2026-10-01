	thumb_func_start sub_0802BD28
sub_0802BD28: @ 0x0802BD28
	push {r4, r5, r6, lr}
	lsl r1, r1, #0x10
	lsl r0, r1, #8
	lsr r6, r0, #0x18
	lsr r5, r1, #0x18
	ldr r4, _0802BD84 @ =0x0000058A
	mov r0, #0
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0802BD90
	mov r0, #1
	add r1, r4, #0
	bl sub_08008524
	cmp r0, #0
	bgt _0802BD90
	cmp r5, #4
	bgt _0802BD90
	mov r3, #1
	add r1, r6, #0
	and r1, r3
	mov r0, #0x94
	add r2, r5, #0
	mul r2, r0
	ldr r0, _0802BD88 @ =0x00000D64
	mul r0, r1
	add r2, r2, r0
	ldr r0, _0802BD8C @ =0x0201930C
	add r1, r2, r0
	ldr r0, [r1]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802BD90
	ldrb r1, [r1, #6]
	mov r0, #2
	and r0, r1
	cmp r0, #0
	bne _0802BD90
	add r0, r3, #0
	and r0, r1
	cmp r0, #0
	beq _0802BD90
	mov r0, #1
	b _0802BD92
_0802BD84: .4byte 0x0000058A
_0802BD88: .4byte 0x00000D64
_0802BD8C: .4byte 0x0201930C
_0802BD90:
	mov r0, #0
_0802BD92:
	pop {r4, r5, r6}
	pop {r1}
	bx r1
	thumb_func_end sub_0802BD28

