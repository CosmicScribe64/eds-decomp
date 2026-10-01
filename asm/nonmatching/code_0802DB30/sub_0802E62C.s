	thumb_func_start sub_0802E62C
sub_0802E62C: @ 0x0802E62C
	push {r4, r5, lr}
	add r4, r0, #0
	lsl r2, r2, #0x10
	ldrb r5, [r4, #6]
	ldrh r0, [r4, #6]
	lsr r3, r0, #8
	cmp r2, #0
	bne _0802E698
	mov r2, #1
	and r2, r5
	mov r0, #0x94
	mul r0, r3
	ldr r1, _0802E690 @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802E694 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802E698
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	cmp r5, r0
	bne _0802E66A
	add r1, r3, #0
	bl sub_08008AF8
	cmp r0, #0
	beq _0802E698
_0802E66A:
	ldrb r0, [r4, #2]
	lsl r1, r0, #0x1F
	lsr r0, r1, #0x1F
	cmp r5, r0
	beq _0802E680
	mov r1, #1
	neg r1, r1
	bl sub_08008AF8
	cmp r0, #0
	beq _0802E698
_0802E680:
	ldrb r4, [r4, #3]
	lsr r0, r4, #2
	cmp r0, #7
	bgt _0802E698
	cmp r0, #5
	blt _0802E698
	mov r0, #1
	b _0802E69A
_0802E690: .4byte 0x00000D64
_0802E694: .4byte 0x0201930C
_0802E698:
	mov r0, #0
_0802E69A:
	pop {r4, r5}
	pop {r1}
	bx r1
	thumb_func_end sub_0802E62C

