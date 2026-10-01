	thumb_func_start sub_0802E4E0
sub_0802E4E0: @ 0x0802E4E0
	add r3, r0, #0
	lsl r2, r2, #0x10
	cmp r2, #0
	bne _0802E550
	cmp r1, #0
	bne _0802E524
	ldrh r0, [r3, #6]
	mov r2, #1
	and r2, r0
	lsr r0, r0, #8
	mov r1, #0x94
	mul r0, r1
	ldr r1, _0802E51C @ =0x00000D64
	mul r1, r2
	add r0, r0, r1
	ldr r1, _0802E520 @ =0x0201930C
	add r0, r0, r1
	ldr r0, [r0]
	lsl r0, r0, #0x14
	cmp r0, #0
	beq _0802E550
	ldrb r3, [r3, #3]
	lsr r0, r3, #2
	cmp r0, #7
	bgt _0802E550
	cmp r0, #5
	blt _0802E550
_0802E516:
	mov r0, #1
	b _0802E552
	.align 2, 0
_0802E51C: .4byte 0x00000D64
_0802E520: .4byte 0x0201930C
_0802E524:
	ldr r2, _0802E554 @ =0x000007FF
	ldrh r1, [r1]
	and r2, r1
	lsl r0, r2, #2
	ldr r1, _0802E558 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0802E550
	cmp r0, #0x15
	blt _0802E550
	lsl r0, r2, #1
	ldr r1, _0802E55C @ =0x08622AB4
	add r0, r0, r1
	ldr r1, _0802E560 @ =0x00000603
	ldrh r0, [r0]
	cmp r0, r1
	bne _0802E516
_0802E550:
	mov r0, #0
_0802E552:
	bx lr
_0802E554: .4byte 0x000007FF
_0802E558: .4byte gUnk_08621DE0
_0802E55C: .4byte gUnk_08622AB4
_0802E560: .4byte 0x00000603
	thumb_func_end sub_0802E4E0

