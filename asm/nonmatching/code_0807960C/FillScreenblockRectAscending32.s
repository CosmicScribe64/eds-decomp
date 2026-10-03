	thumb_func_start FillScreenblockRectAscending32
FillScreenblockRectAscending32: @ 0x0807A5D4
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	ldr r5, [sp, #0x28]
	ldr r4, [sp, #0x2C]
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	lsl r1, r1, #0x18
	lsl r2, r2, #0x18
	lsr r2, r2, #0x18
	mov r9, r2
	lsl r3, r3, #0x18
	lsl r5, r5, #0x18
	lsl r4, r4, #0x18
	lsr r4, r4, #0x18
	mov sl, r4
	lsr r1, r1, #0xD
	lsl r0, r2, #1
	mov r2, #0xC0
	lsl r2, r2, #0x13
	add r0, r0, r2
	add r1, r1, r0
	lsr r3, r3, #0x12
	add r4, r1, r3
	mov r1, #0
	cmp r1, sl
	bcs _0807A69A
	lsr r0, r5, #0x19
	lsl r0, r0, #0x18
	lsr r2, r0, #0x18
	mov r8, r2
	str r0, [sp, #0]
	mov r0, #0x10
	sub r0, r0, r2
	lsl r0, r0, #2
	str r0, [sp, #4]
_0807A622:
	mov r5, #0
	add r1, #1
	mov ip, r1
	cmp r5, r8
	bcs _0807A68C
	ldr r0, [sp, #0]
	lsr r7, r0, #0x18
_0807A630:
	lsl r0, r5, #1
	mov r2, r9
	add r1, r2, r0
	add r0, r1, #0
	sub r0, #0x20
	cmp r0, #0x1F
	bhi _0807A646
	mov r0, #0xF8
	lsl r0, r0, #3
	add r3, r4, r0
	b _0807A64E
_0807A646:
	cmp r1, #0x3F
	ble _0807A66C
	ldr r1, _0807A668 @ =0xFFFFFF00
	add r3, r4, r1
_0807A64E:
	add r2, r6, #0
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r1, r6, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	lsl r1, r1, #0x10
	orr r1, r2
	str r1, [r3]
	add r4, #4
	b _0807A682
_0807A668: .4byte 0xFFFFFF00
_0807A66C:
	add r2, r6, #0
	add r0, r2, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	add r1, r6, #0
	add r0, r1, #1
	lsl r0, r0, #0x10
	lsr r6, r0, #0x10
	lsl r1, r1, #0x10
	orr r1, r2
	stmia r4!, {r1}
_0807A682:
	add r0, r5, #1
	lsl r0, r0, #0x18
	lsr r5, r0, #0x18
	cmp r5, r7
	bcc _0807A630
_0807A68C:
	ldr r2, [sp, #4]
	add r4, r4, r2
	mov r1, ip
	lsl r0, r1, #0x18
	lsr r1, r0, #0x18
	cmp r1, sl
	bcc _0807A622
_0807A69A:
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov r9, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	thumb_func_end FillScreenblockRectAscending32
	.align 2, 0

