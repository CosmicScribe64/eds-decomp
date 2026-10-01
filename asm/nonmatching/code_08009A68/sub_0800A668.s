	thumb_func_start sub_0800A668
sub_0800A668: @ 0x0800A668
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xC
	add r4, r0, #0
	lsl r2, r2, #0x10
	lsr r2, r2, #0x10
	str r2, [sp, #0]
	lsl r3, r3, #0x10
	lsr r3, r3, #0x10
	str r3, [sp, #4]
	mov r6, #0
	mov r8, r6
	mov r0, #1
	and r4, r0
	mov r0, #0x94
	mul r1, r0
	ldr r7, _0800A6CC @ =0x00000D64
	add r2, r4, #0
	mul r2, r7
	add r0, r1, r2
	ldr r3, _0800A6D0 @ =0x0201930C
	add r0, r0, r3
	mov ip, r0
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r6, r0
	bge _0800A772
	str r4, [sp, #8]
	mov r9, r1
	ldr r0, _0800A6D4 @ =0x000007FF
	mov sl, r0
	add r0, r2, #0
	add r0, #0x4A
	add r0, r9
	add r5, r0, r3
_0800A6B4:
	mov r2, r8
	lsl r1, r2, #1
	mov r0, ip
	add r0, #0xA
	add r0, r0, r1
	ldrh r0, [r0]
	ldrb r2, [r5]
	cmp r2, #1
	beq _0800A6D8
	cmp r2, #5
	beq _0800A754
	b _0800A756
_0800A6CC: .4byte 0x00000D64
_0800A6D0: .4byte 0x0201930C
_0800A6D4: .4byte 0x000007FF
_0800A6D8:
	mov r4, #0
	lsl r1, r0, #0x18
	lsr r1, r1, #0x18
	lsr r0, r0, #8
	and r1, r2
	mov r2, #0x94
	mul r0, r2
	mul r1, r7
	add r0, r0, r1
	add r0, r0, r3
	ldr r0, [r0]
	lsl r0, r0, #0x14
	lsr r2, r0, #0x14
	cmp r2, #0
	beq _0800A756
	ldr r0, [sp, #0]
	cmp r0, #0
	beq _0800A716
	add r0, r2, #0
	mov r1, sl
	and r0, r1
	lsl r0, r0, #2
	ldr r1, _0800A744 @ =0x08621DE0
	add r0, r0, r1
	ldr r0, [r0]
	mov r1, #0xF8
	lsl r1, r1, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bne _0800A718
_0800A716:
	mov r4, #1
_0800A718:
	ldr r0, [sp, #4]
	cmp r0, #0
	beq _0800A74E
	mov r1, sl
	and r2, r1
	lsl r0, r2, #2
	ldr r2, _0800A744 @ =0x08621DE0
	add r0, r0, r2
	ldr r1, [r0]
	mov r0, #0xF8
	lsl r0, r0, #0x11
	and r0, r1
	lsr r0, r0, #0x14
	cmp r0, #0x16
	bgt _0800A748
	cmp r0, #0x15
	blt _0800A748
	mov r0, #0xE0
	lsl r0, r0, #0xC
	and r1, r0
	lsr r0, r1, #0x11
	b _0800A74A
_0800A744: .4byte gUnk_08621DE0
_0800A748:
	mov r0, #0
_0800A74A:
	cmp r0, #3
	bne _0800A750
_0800A74E:
	mov r4, #1
_0800A750:
	cmp r4, #0
	beq _0800A756
_0800A754:
	add r6, #1
_0800A756:
	add r5, #2
	mov r0, #1
	add r8, r0
	ldr r7, _0800A784 @ =0x00000D64
	ldr r1, [sp, #8]
	add r0, r1, #0
	mul r0, r7
	add r0, r9
	ldr r3, _0800A788 @ =0x0201930C
	add r0, r0, r3
	add r0, #0x8A
	ldrh r0, [r0]
	cmp r8, r0
	blt _0800A6B4
_0800A772:
	add r0, r6, #0
	add sp, #0xC
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
_0800A784: .4byte 0x00000D64
_0800A788: .4byte 0x0201930C
	thumb_func_end sub_0800A668

